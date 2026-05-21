/*
** ? 2020 Alphawave IP Inc.
*/

/**
 * Alphawave AlphaCORE100 C API functions
 *
 * Version: 1.0.0
 */

#include <stdint.h>
#include <math.h>
#include <string.h>
#include <stddef.h>

#include <bf_types/bf_types.h>
#include <lld/lld_reg_if.h>

#include "../aw_types.h"
#include "aw_alphacore.h"
#include "aw_pmd_rx_dsp_get.h"
#include "stdlib.h"

#include "../aw_vector_types.h"

// include the auto-generated register defnitions for debugging
#include "aw_4ln_regs.c"
#include "aw_4ln_flds.c"
#include "aw_4ln_cmnts.c"
// and the virtual field definitions
#include "aw_4ln_vregs.c"
#include "aw_4ln_vflds.c"
#include "aw_4ln_vcmnts.c"
#include "aw_4ln_vapi.h"
#include "aw_vectors_4ln.h"
#include <tof3_regs/tof3_reg_drv.h>

#include "aw_alphacore_atest_defines.h"

static char * aw_bist_pattern_decoder (uint32_t bist_pattern_encoded);
extern void bf_sys_usleep(uint32_t us);

//extern const char aw_library_version[];
int c_4ln_load_hexfile(mss_access_t *mss, char * fileName);

aw_serdes_driver_vector_t *aw_4ln_driver_install(void) {
  return aw_4ln_vector_get();
}

int aw_pmd_4ln_reg_defs_get(aw_reg_defs_t **regs,
                            aw_fld_defs_t **flds,
                            char **cmnts,
                            aw_reg_defs_t **vregs,
                            aw_fld_defs_t **vflds,
                            char **vcmnts) {
  *regs = aw_regs;
  *flds = aw_flds;
  *cmnts= (char*)aw_comments;
  *vregs = aw_vregs;
  *vflds = aw_vflds;
  *vcmnts= (char*)aw_vcomments;
  return 0;
}

int aw_pmd_4ln_power_on_reset_sequence_apply(mss_access_t *callers_mss) {
  mss_access_t local_mss = *callers_mss, *mss = &local_mss;
  uint32_t addr, data;

  addr = offsetof(tof3_reg, serdes.serdes0.serdes4ln_glue_regs.serdes_soft_reset);
  lld_subdev_write_register(mss->dev_id, mss->subdev_id, addr, 0);
  lld_subdev_write_register(mss->dev_id, mss->subdev_id, addr, 0xffff);
  bf_sys_usleep(1000);
  lld_subdev_write_register(mss->dev_id, mss->subdev_id, addr, 0);
  bf_sys_usleep(1000);

  // set DIG_SOC_CMN_OVRD_ICTL_REF_LS_ENA_A otherwise hangs pcie
  mss->phy_offset = offsetof(tof3_reg, serdes.serdes0);

  addr = mss->phy_offset + DIG_SOC_CMN_OVRD_ADDR + 0x20000;

  lld_subdev_read_register(mss->dev_id, mss->subdev_id, addr, &data);
  data |= DIG_SOC_CMN_OVRD_ICTL_REF_LS_ENA_A_MASK;
  lld_subdev_write_register(mss->dev_id, mss->subdev_id, addr, data);

  // program SRAM delays
  mss->phy_offset = offsetof(tof3_reg, serdes.serdes0);
  aw_pmd_4ln_rd_data_pipeline_stages_set(mss, 3 /*stages*/);

  return 0;
}

int aw_pmd_4ln_mss_reset(mss_access_t *mss) {

  // perform a simple check that the chip is assessible
  uint32_t r1, r2, r3;
  aw_pmd_4ln_rd_csr(mss, CMNMFSM_SCRATCH_REG1_ADDR, &r1);
  aw_pmd_4ln_rd_csr(mss, CMNMFSM_SCRATCH_REG1_ADDR, &r2);

  // if both read-back as -1 assume chip is not accessible
  if ((r1 == 0xffffffff) && (r2 == 0xffffffff)) {
    goto post_err;
  }
  // verify coonsistent reads
  if (r1 != r2) {
    goto post_err;
  }
  // verify writability
  aw_pmd_4ln_wr_csr(mss, CMNMFSM_SCRATCH_REG1_ADDR, 0x12345678);
  aw_pmd_4ln_rd_csr(mss, CMNMFSM_SCRATCH_REG1_ADDR, &r3);
  if (r3 != 0x12345678) {
    goto post_err;
  }
  // restore original contents
  aw_pmd_4ln_wr_csr(mss, CMNMFSM_SCRATCH_REG1_ADDR, r1);

  aw_pmd_4ln_power_on_reset_sequence_apply(mss);

  return 0;

post_err:
  return 0; //AW_ERR_CODE_READ_FAILURE;
}

/**
 * Default method for loading firmware and pointers. Needs to take in an array
 * that contains both pointers and SRAM values, or a seperate structure
 * containing pointer info, and then also the arry to the SRAM instructions.
 *
 * It will load the pointers to the CSRs via the PMI interface and the FW to
 * the SRAM via the PRAM interface.
 *
 */
int aw_pmd_4ln_uc_ucode_load2(mss_access_t *mss, uint32_t (*ucode_arr)[2], uint32_t ucode_len){
    for (uint32_t i = 0; i<ucode_len; i++){
        CHECK(pmd_write_addr(mss, ucode_arr[i][0], ucode_arr[i][1])); // addr, val
    }
    return AW_ERR_CODE_NONE;

}

int c_4ln_load_hexfile(mss_access_t *mss, char * fileName) {
    int pass = 0;

    FILE* file = fopen(fileName, "r"); /* should check the result */
    char line[256];
    uint32_t aw_ucode[2*AW_SRAM_SIZE][2];
    uint32_t i = 0;

    //bfn, make sure file exists
    if (file == NULL) return AW_ERR_CODE_INVALID_ARG_VALUE;

    while (fgets(line, sizeof(line), file)) {
        char * token = strtok(line, ","); // get first element (address)
        if (token == NULL) {
          USR_PRINTF("c_16ln_load_hexfile: hex file corrupted at line %d. Bail out ..\n", i);
          fclose(file);
          return AW_ERR_CODE_INVALID_ARG_VALUE;
        }
        uint32_t addr;
        sscanf(token, "%x", &addr); // convert to uint32_t
        token = strtok(NULL, ","); // get value
        if (token == NULL) {
          USR_PRINTF("c_16ln_load_hexfile: hex file corrupted at line %d. Bail out ..\n", i);
          fclose(file);
          return AW_ERR_CODE_INVALID_ARG_VALUE;
        }
        uint32_t value;
        sscanf(token, "%x", &value); // convert to uint32_t
        aw_ucode[i][0] = addr;
        aw_ucode[i][1] = value;
        i++;
        if (i >= (2*AW_SRAM_SIZE)) {
          USR_PRINTF("c_16ln_load_hexfile: hex file overruns buffer. Bail out ..\n");
          fclose(file);
          return AW_ERR_CODE_INVALID_ARG_VALUE;
        }
        continue;
    }

    fclose(file);

    //USR_PRINTF("Calling aw_pmd_uc_ucode_load\n");
    pass += aw_pmd_4ln_uc_ucode_load2(mss, aw_ucode, i);
    return pass;

}

static int c_4ln_check_exists(char * fileName) {
  FILE* file = fopen(fileName, "r");

  //bfn, make sure file exists
  if (file != NULL) {
    fclose(file);
  }
  return (file != NULL);
}

int aw_pmd_4ln_fw_load(mss_access_t *mss, char *path) {
  char *test_fw_nm = "/test_fw_4ln.hex";
  char *fw_to_load;

  if (c_4ln_check_exists(test_fw_nm)) {
    fw_to_load = test_fw_nm;
    printf("****************************************\n");
    printf("* LOADING EXPERIMENTAL FW              *\n");
    printf("* If unintentional, rm /test_fw_4ln.hex*\n");
    printf("****************************************\n");
  } else {
    fw_to_load = path;
  }
  aw_version_t version_st;

  aw_pmd_4ln_fw_version_get(mss, &version_st);
  printf("FW load (4ln) : %s v%d.%d.%d\n", fw_to_load,
           version_st.version_major, version_st.version_minor, version_st.version_patch);
  return c_4ln_load_hexfile(mss, fw_to_load);
}


/**********************************************************
 * Per-lane one time programming
 */
int aw_pmd_4ln_one_time_pgm(mss_access_t *mss) {
  // set map_en=1
  aw_pmd_4ln_ctrl_map_en_set(mss, 1);
  // set ANLT width regs
  aw_pmd_4ln_anlt_width_set(mss);
  // set clks/millisecond for AW FW
  aw_pmd_4ln_anlt_ms_per_clk_set(mss);

  // the 4ln IP seems to init the OVRD_REG5 to 0x1ffff7
  aw_pmd_4ln_isolate_lane_set(mss, 0);
  aw_pmd_4ln_isolate_lane_set(mss, 1);
  return 0;
}

/**********************************************************
 * return T/RXMFSM rate/width/power settings
 */
int aw_pmd_4ln_cur_rate_width_pstate(mss_access_t *mss,
                                      uint32_t *tx_rate,
                                      uint32_t *tx_width,
                                      uint32_t *tx_pstate,
                                      uint32_t *rx_rate,
                                      uint32_t *rx_width,
                                      uint32_t *rx_pstate) {
  pmd_read_field(mss, TXMFSM_STAT_ADDR, TXMFSM_STAT_TXMFSM_RATE_CUR_MASK, TXMFSM_STAT_TXMFSM_RATE_CUR_OFFSET, tx_rate);
  pmd_read_field(mss, TXMFSM_STAT_ADDR, TXMFSM_STAT_TXMFSM_WIDTH_CUR_MASK, TXMFSM_STAT_TXMFSM_WIDTH_CUR_OFFSET, tx_width);
  pmd_read_field(mss, TXMFSM_STAT_ADDR, TXMFSM_STAT_TXMFSM_POWER_CUR_MASK, TXMFSM_STAT_TXMFSM_POWER_CUR_OFFSET, tx_pstate);

  pmd_read_field(mss, RXMFSM_STAT_ADDR, RXMFSM_STAT_RXMFSM_RATE_CUR_MASK, RXMFSM_STAT_RXMFSM_RATE_CUR_OFFSET, rx_rate);
  pmd_read_field(mss, RXMFSM_STAT_ADDR, RXMFSM_STAT_RXMFSM_WIDTH_CUR_MASK, RXMFSM_STAT_RXMFSM_WIDTH_CUR_OFFSET, rx_width);
  pmd_read_field(mss, RXMFSM_STAT_ADDR, RXMFSM_STAT_RXMFSM_POWER_CUR_MASK, RXMFSM_STAT_RXMFSM_POWER_CUR_OFFSET, rx_pstate);
  return 0;
}

/**********************************************************
 * clear rxmfsm_eq_check_rxdisable otherwise LT doesn't
 * work.
 */
int aw_pmd_4ln_rxmfsm_eq_check_rxdisable_set(mss_access_t *mss, uint32_t val) {
  CHECK(pmd_write_field(mss, RXMFSM_CTRL_ADDR, RXMFSM_CTRL_RXMFSM_EQ_CHECK_RXDISABLE_MASK, RXMFSM_CTRL_RXMFSM_EQ_CHECK_RXDISABLE_OFFSET, val));
  return 0;
}

int aw_pmd_4ln_lt_info_get(mss_access_t *mss, uint32_t * lt_fsm_st, uint32_t * frame_lock) {
    CHECK(pmd_read_field(mss, ETH_ANLT_STATE_REG2_ADDR, ETH_ANLT_STATE_REG2_LT_STATE_CTRL_FSM_MASK, ETH_ANLT_STATE_REG2_LT_STATE_CTRL_FSM_OFFSET, lt_fsm_st));
    CHECK(pmd_read_field(mss, ETH_LT_STAT_ADDR, ETH_LT_STAT_LT_RX_FRAME_LOCK_MASK, ETH_LT_STAT_LT_RX_FRAME_LOCK_OFFSET, frame_lock));

    return 0;
}

int aw_pmd_4ln_rd_csr(mss_access_t *mss, uint32_t addr, uint32_t *rdata) {
  return pmd_read_addr(mss, addr, rdata);
}

int aw_pmd_4ln_wr_csr(mss_access_t *mss, uint32_t addr, uint32_t wdata) {
  return pmd_write_addr(mss, addr, wdata);
}

/********************************************************************
* 4ln IP (Raptors) supported rates.
*
* protocol: eth
lsref_mhz: 156.25
hsref_mhz: 3320.3125

*/
typedef struct rate_cfg_t {
  char    *name;
  uint32_t serdes_speed;
  uint32_t is_pam4;
  uint32_t rate_code;
  uint32_t width_code;
} rate_cfg_t;

#if 0
static rate_cfg_t rate_cfg_tbl[] = {
    // speed             serdes-speed is-PAM4 rate-code   width-code
    { "1.25g     (NRZ)",   1,         0,      0 /*  1G*/, 2 /* 20b?*/},
    { "3.125g    (NRZ)",   3,         0,      1 /*  3G*/, 2 /* 20b?*/},
    { "5.15625g  (NRZ)",   5,         0,      2 /*  5G*/, 3 /* 20b?*/},
    { "10.3125g  (NRZ)",  10,         0,      3 /* 10G*/, 2 /* 16b*/ },
    { "25.78125g (NRZ)",  25,         0,      4 /* 25G*/, 4 /* 32b*/ },
    { "27.1875g  (NRZ)",  27,         0,      5 /* 27G*/, 4 /* 32b*/ },
    { "53.125g  (PAM4)",  50,         1,      6 /* 53G*/, 6 /*128b*/ },
};
#endif
static rate_cfg_t rate_cfg_tbl[] = {
    // speed             serdes-speed is-PAM4 rate-code   width-code
    { "1.25g     (NRZ)",   1,         0,      0 /*  1G*/, 2 /* 20b?*/},
    { "3.125g    (NRZ)",   3,         0,      1 /*  3G*/, 2 /* 20b?*/},
    { "5.15625g  (NRZ)",   5,         0,      2 /*  5G*/, 3 /* 20b?*/},
    { "10.3125g  (NRZ)",  10,         0,      3 /* 10G*/, 2 /* 16b*/ },
    { "25.78125g (NRZ)",  25,         0,      4 /* 25G*/, 4 /* 32b*/ },
    { "27.1875g  (NRZ)",  27,         0,      5 /* 27G*/, 4 /* 32b*/ },
    { "53.125g  (PAM4)",  50,         1,      6 /* 53G*/, 6 /*128b*/ },
};

static rate_cfg_t pcie_rate_cfg_tbl[] = {
    { "2.5g      (NRZ)",   2,         0,      0 /* 53G*/, 2 /* 64b*/ },
    { "5g        (NRZ)",   5,         0,      1 /*106G*/, 2 /* 16b*/ },
    { "8g        (NRZ)",   8,         0,      2 /*106G*/, 2 /* 16b*/ },
    { "16g       (NRZ)",  16,         0,      3 /*106G*/, 2 /* 16b*/ },
};

int aw_pmd_4ln_speed_to_rate_and_width(mss_access_t *mss, uint32_t serdes_speed, bool is_pam4, uint32_t *rate, uint32_t *width) {
  uint32_t mode;

  for (mode = 0; mode < sizeof(rate_cfg_tbl)/sizeof(rate_cfg_tbl[0]); mode++) {
    if ((rate_cfg_tbl[mode].serdes_speed == serdes_speed) &&
        (rate_cfg_tbl[mode].is_pam4 == is_pam4)) {
      *rate = rate_cfg_tbl[mode].rate_code;
      *width= rate_cfg_tbl[mode].width_code;
      return 0;
    }
  }
  // not found in eth FW, check PCIe rates
  for (mode = 0; mode < sizeof(pcie_rate_cfg_tbl)/sizeof(pcie_rate_cfg_tbl[0]); mode++) {
    if ((pcie_rate_cfg_tbl[mode].serdes_speed == serdes_speed) &&
        (pcie_rate_cfg_tbl[mode].is_pam4 == is_pam4)) {
      *rate = pcie_rate_cfg_tbl[mode].rate_code;
      *width= pcie_rate_cfg_tbl[mode].width_code;
      return 0;
    }
  }
  // un-supported speed passed
  return AW_ERR_CODE_INVALID_ARG_VALUE;
}


// FFE tap converter
    /*
     * RX_FFE_ADAPT_COEF_RDREG0_TAP0_NT_BITWIDTH 0x00000005
     * RX_FFE_ADAPT_COEF_RDREG0_TAP1_NT_BITWIDTH 0x00000005
     * RX_FFE_ADAPT_COEF_RDREG0_TAP2_NT_BITWIDTH 0x00000005
     * RX_FFE_ADAPT_COEF_RDREG0_TAP3_NT_BITWIDTH 0x00000005
     * RX_FFE_ADAPT_COEF_RDREG0_TAP4_NT_BITWIDTH 0x00000005
     * RX_FFE_ADAPT_COEF_RDREG0_TAP5_NT_BITWIDTH 0x00000005
     * RX_FFE_ADAPT_COEF_RDREG1_TAP6_NT_BITWIDTH 0x00000006
     * RX_FFE_ADAPT_COEF_RDREG1_TAP7_NT_BITWIDTH 0x00000006
     * RX_FFE_ADAPT_COEF_RDREG1_TAP8_NT_BITWIDTH 0x00000007
     * RX_FFE_ADAPT_COEF_RDREG1_TAP9_NT_BITWIDTH 0x00000008
     * RX_FFE_ADAPT_COEF_RDREG2_TAP10_NT_BITWIDTH 0x00000008
     * RX_FFE_ADAPT_COEF_RDREG2_TAP11_NT_BITWIDTH 0x00000008
     * RX_FFE_ADAPT_COEF_RDREG2_TAP12_NT_BITWIDTH 0x00000009
     * RX_FFE_ADAPT_COEF_RDREG3_TAP13_NT_BITWIDTH 0x0000000A
     * RX_FFE_ADAPT_COEF_RDREG3_TAP14_NT_BITWIDTH 0x00000009
     * RX_FFE_ADAPT_COEF_RDREG3_TAP15_NT_BITWIDTH 0x00000008
     * RX_FFE_ADAPT_COEF_RDREG4_TAP16_NT_BITWIDTH 0x00000007
     * RX_FFE_ADAPT_COEF_RDREG4_TAP17_NT_BITWIDTH 0x00000006
    */
int32_t aw_pmd_4ln_ffe_tap_val(uint32_t fld_val, uint32_t ffe_tap) {
  uint32_t tap_width[18] = {5,5,5,5,5,5,6,6,7,8,8,8,9,10,9,8,7,6};

  if (fld_val & (1<<(tap_width[ffe_tap] - 1))) { // negative
    uint32_t cmp2, cmp1;

    cmp1 = ~fld_val;
    // truncate to "width" bits
    cmp2 = (cmp1 & ((1<<tap_width[ffe_tap]) - 1)) + 1;
    return 0 - cmp2;
  }
  return fld_val;
}

int32_t aw_pmd_4ln_arbwidth_val(uint32_t val, uint32_t width) {

  if (val & (1<<(width - 1))) { // negative
    uint32_t cmp2, cmp1;

    cmp1 = ~val;
    // truncate to "width" bits
    cmp2 = (cmp1 & ((1<<width) - 1)) + 1;
    return 0 - cmp2;
  }
  return val;
}

#if 0
static void graph_val(int32_t fld_val, uint32_t ffe_tap) {
  int32_t tap_val = aw_pmd_4ln_ffe_tap_val(fld_val, ffe_tap);

  // 64 character graph width, so 32 char pos and 32 char neg
  // 10 bit max value, 2^^9 pos/neg,
  // so scale factor is 2^^3 = 8
  // so a graph line is composed of 3 parts,
  // if scaled val < 0
  // blank-space from -32 to scaled val, dashes from scaled val to 0, blank space from 0 to 32
  // if scaled val > 0
  // blank-space from -32 to 0, dashes from 0 to scaled val, blank space from scaled val to 32
  int32_t scaled_val = tap_val / 8;
  int32_t spaces1;
  int32_t dashes;
  int32_t spaces2;

  if (tap_val != 0) {
    if (tap_val < 0) {
      if (scaled_val > -32) scaled_val--;
    } else {
      if (scaled_val < 32) scaled_val++;
    }
  }
  if (scaled_val < 0) {
    spaces1 = 32 - (0-scaled_val);
    dashes = (0-scaled_val);
    spaces2 = 32;
  } else {
    spaces1 = 32;
    dashes = scaled_val;
    spaces2 = 32 - scaled_val;
  }
  if (((spaces1 > 32) || (dashes > 32) || (spaces2 > 32)) ||
      ((spaces1 < 0) || (dashes < 0) || (spaces2 < 0))) {
    USR_PRINTF("spaces1 = %d : dashes = %d : spaces2 = %d", spaces1, dashes, spaces2);
    return;
  }

  for (int32_t s = 0; s < spaces1; s++) {
    USR_PRINTF(" ");
  }
  if (spaces1 == 32) USR_PRINTF("|");

  for (int32_t d = 0; d < dashes; d++) {
    USR_PRINTF("-");
  }
  if (spaces1 != 32) USR_PRINTF("|");

  for (int32_t s = 0; s < spaces2; s++) {
    USR_PRINTF(" ");
  }
}
#endif

/*****************************************************************
 * Set TX or RX lane based on individual fields
 * **************************************************************/
int bf_pmd_4ln_write_field(mss_access_t *mss, uint32_t addr, uint32_t fld_mask, uint32_t fld_offset, uint32_t wval){

  if (addr == DIG_SOC_LANE_OVRD_REG5_ADDR) {
    switch (fld_offset) {
    case DIG_SOC_LANE_OVRD_REG5_LANE_TX_BLOCK_DATA_ENA_OVRD_EN_A_OFFSET:
    //DIG_SOC_LANE_STAT_REG1_OCTL_RX_LINKEVAL_DIR_OFFSET/reserved no use case DIG_SOC_LANE_OVRD_REG5_CTRL_CLK_A_OFFSET:
    case DIG_SOC_LANE_OVRD_REG5_TXSOC_CLK_A_OFFSET:
    //case DIG_SOC_LANE_OVRD_REG5_LANE_DME_OVRD_EN_A_OFFSET:
    case DIG_SOC_LANE_OVRD_REG5_LANE_TXBEACON_OVRD_EN_A_OFFSET:
    //case DIG_SOC_LANE_OVRD_REG5_LANE_RXTERM_OVRD_EN_A_OFFSET:
    case DIG_SOC_LANE_OVRD_REG5_LANE_RXDISABLE_OVRD_EN_A_OFFSET:
    //no use case DIG_SOC_LANE_OVRD_REG5_LANE_PCIEL1_OVRD_EN_A_OFFSET:
    case DIG_SOC_LANE_OVRD_REG5_LANE_BYPASS_OVRD_EN_A_OFFSET:
    case DIG_SOC_LANE_OVRD_REG5_LANE_RESET_OVRD_EN_A_OFFSET:
    case DIG_SOC_LANE_OVRD_REG5_LANE_TXLEVEL_OVRD_EN_A_OFFSET:
    case DIG_SOC_LANE_OVRD_REG5_LANE_TXFIR_OVRD_EN_A_OFFSET:
    case DIG_SOC_LANE_OVRD_REG5_LANE_TXSTATE_OVRD_EN_A_OFFSET:
    //case DIG_SOC_LANE_OVRD_REG5_LANE_RXSTATE_OVRD_EN_A_OFFSET:
    //case DIG_SOC_LANE_OVRD_REG5_LANE_RXLINKEVAL_OVRD_EN_A_OFFSET:
    case DIG_SOC_LANE_OVRD_REG5_LANE_TXRXDET_OVRD_EN_A_OFFSET:
    //case DIG_SOC_LANE_OVRD_REG5_LANE_RXMARGIN_OVRD_EN_A_OFFSET:
    case DIG_SOC_LANE_OVRD_REG5_LANE_LOOPBACK_OVRD_EN_A_OFFSET:
    case DIG_SOC_LANE_OVRD_REG5_LANE_TXDATA_OVRD_EN_A_OFFSET:
    case DIG_SOC_LANE_OVRD_REG5_LANE_ETH_AN_CTRL_OVRD_EN_A_OFFSET:
      mss->derived_from_name_offset = mss->tx_lane_offset;
      break;
    case DIG_SOC_LANE_OVRD_REG5_LANE_DME_OVRD_EN_A_OFFSET:
    case DIG_SOC_LANE_OVRD_REG5_LANE_RXTERM_OVRD_EN_A_OFFSET:
    case DIG_SOC_LANE_OVRD_REG5_LANE_RXSTATE_OVRD_EN_A_OFFSET:
    case DIG_SOC_LANE_OVRD_REG5_LANE_RXLINKEVAL_OVRD_EN_A_OFFSET:
    case DIG_SOC_LANE_OVRD_REG5_LANE_RXMARGIN_OVRD_EN_A_OFFSET:
      mss->derived_from_name_offset = mss->rx_lane_offset;
      break;
    default:
      break;
    }
  } else if (addr == DIG_SOC_LANE_OVRD_REG1_ADDR) {
    if (fld_offset == DIG_SOC_LANE_OVRD_REG1_ICTL_CK_TX_BLOCK_DATA_ENA_A_OFFSET) {
      mss->derived_from_name_offset = mss->tx_lane_offset;
    }
  }

  //test ===========================================
  // handle settings that get remapped if map_en=1
  if (addr == DIG_SOC_LANE_OVRD_REG2_ADDR) {
    switch (fld_offset) {
    case DIG_SOC_LANE_OVRD_REG2_ICTL_RX_STATE_WIDTH_A_OFFSET:
    case DIG_SOC_LANE_OVRD_REG2_ICTL_RX_STATE_RATE_A_OFFSET:
    case DIG_SOC_LANE_OVRD_REG2_ICTL_RX_LINKEVAL_TYPE_A_OFFSET:
    case DIG_SOC_LANE_OVRD_REG2_ICTL_RX_LINKEVAL_REQ_A_OFFSET:
      mss->derived_from_name_offset = mss->tx_lane_offset;
    default: break;
    }
  } else if (addr == DIG_SOC_LANE_OVRD_REG3_ADDR) {
    switch (fld_offset) {
    case DIG_SOC_LANE_OVRD_REG3_ICTL_RX_STATE_POWER_A_OFFSET:
    case DIG_SOC_LANE_OVRD_REG3_ICTL_RX_STATE_REQ_A_OFFSET:
      mss->derived_from_name_offset = mss->tx_lane_offset;
    default: break;
    }
  } else if (addr == DIG_SOC_LANE_OVRD_REG5_ADDR) {
    switch (fld_offset) {
    case DIG_SOC_LANE_OVRD_REG5_LANE_RXSTATE_OVRD_EN_A_OFFSET:
    case DIG_SOC_LANE_OVRD_REG5_LANE_RXLINKEVAL_OVRD_EN_A_OFFSET:
      mss->derived_from_name_offset = mss->tx_lane_offset;
    default: break;
    }
  } else if (addr == DIG_SOC_LANE_STAT_REG1_ADDR) {
    switch (fld_offset) {
    case DIG_SOC_LANE_STAT_REG1_OCTL_RX_STATE_ACK_OFFSET:
    case DIG_SOC_LANE_STAT_REG1_OCTL_RX_LINKEVAL_DIR_OFFSET:
    case DIG_SOC_LANE_STAT_REG1_OCTL_RX_LINKEVAL_ACK_OFFSET:
      mss->derived_from_name_offset = mss->tx_lane_offset;
    default: break;
    }
  } else if (addr == DIG_SOC_LANE_STAT_REG3_ADDR) {
    switch (fld_offset) {
    case DIG_SOC_LANE_STAT_REG3_OCTL_RX_LINKEVAL_FOM_OFFSET:
      mss->derived_from_name_offset = mss->tx_lane_offset;
    default: break;
    }
  }
  //test ===========================================

  // handle settings that must be applied to BOTH TX and RX
  if (addr == DIG_SOC_LANE_OVRD_REG5_ADDR) {
    if ((fld_offset == DIG_SOC_LANE_OVRD_REG5_LANE_BYPASS_OVRD_EN_A_OFFSET) ||
        (fld_offset == DIG_SOC_LANE_OVRD_REG5_LANE_RESET_OVRD_EN_A_OFFSET)  ||
        (fld_offset == DIG_SOC_LANE_OVRD_REG5_LANE_LOOPBACK_OVRD_EN_A_OFFSET)) {
      // first do RX side, then TX side
      mss->derived_from_name_offset = mss->rx_lane_offset;
      pmd_write_field(mss, addr, fld_mask, fld_offset, wval);
      mss->derived_from_name_offset = mss->tx_lane_offset;
    }
  }
  return(pmd_write_field(mss, addr, fld_mask, fld_offset, wval));
}

int bf_pmd_4ln_read_field(mss_access_t *mss, uint32_t addr, uint32_t fld_mask, uint32_t fld_offset, uint32_t *rval){

  if (addr == DIG_SOC_LANE_OVRD_REG5_ADDR) {
    switch (fld_offset) {
    case DIG_SOC_LANE_OVRD_REG5_LANE_TX_BLOCK_DATA_ENA_OVRD_EN_A_OFFSET:
    //DIG_SOC_LANE_STAT_REG1_OCTL_RX_LINKEVAL_DIR_OFFSET/reserved no use case DIG_SOC_LANE_OVRD_REG5_CTRL_CLK_A_OFFSET:
    case DIG_SOC_LANE_OVRD_REG5_TXSOC_CLK_A_OFFSET:
    //case DIG_SOC_LANE_OVRD_REG5_LANE_DME_OVRD_EN_A_OFFSET:
    case DIG_SOC_LANE_OVRD_REG5_LANE_TXBEACON_OVRD_EN_A_OFFSET:
    //case DIG_SOC_LANE_OVRD_REG5_LANE_RXTERM_OVRD_EN_A_OFFSET:
    case DIG_SOC_LANE_OVRD_REG5_LANE_RXDISABLE_OVRD_EN_A_OFFSET:
    //no use case DIG_SOC_LANE_OVRD_REG5_LANE_PCIEL1_OVRD_EN_A_OFFSET:
    case DIG_SOC_LANE_OVRD_REG5_LANE_BYPASS_OVRD_EN_A_OFFSET:
    case DIG_SOC_LANE_OVRD_REG5_LANE_RESET_OVRD_EN_A_OFFSET:
    case DIG_SOC_LANE_OVRD_REG5_LANE_TXLEVEL_OVRD_EN_A_OFFSET:
    case DIG_SOC_LANE_OVRD_REG5_LANE_TXFIR_OVRD_EN_A_OFFSET:
    case DIG_SOC_LANE_OVRD_REG5_LANE_TXSTATE_OVRD_EN_A_OFFSET:
    //case DIG_SOC_LANE_OVRD_REG5_LANE_RXSTATE_OVRD_EN_A_OFFSET:
    //case DIG_SOC_LANE_OVRD_REG5_LANE_RXLINKEVAL_OVRD_EN_A_OFFSET:
    case DIG_SOC_LANE_OVRD_REG5_LANE_TXRXDET_OVRD_EN_A_OFFSET:
    //case DIG_SOC_LANE_OVRD_REG5_LANE_RXMARGIN_OVRD_EN_A_OFFSET:
    case DIG_SOC_LANE_OVRD_REG5_LANE_LOOPBACK_OVRD_EN_A_OFFSET:
    case DIG_SOC_LANE_OVRD_REG5_LANE_TXDATA_OVRD_EN_A_OFFSET:
    case DIG_SOC_LANE_OVRD_REG5_LANE_ETH_AN_CTRL_OVRD_EN_A_OFFSET:
      mss->derived_from_name_offset = mss->tx_lane_offset;
      break;
    case DIG_SOC_LANE_OVRD_REG5_LANE_DME_OVRD_EN_A_OFFSET:
    case DIG_SOC_LANE_OVRD_REG5_LANE_RXTERM_OVRD_EN_A_OFFSET:
    case DIG_SOC_LANE_OVRD_REG5_LANE_RXSTATE_OVRD_EN_A_OFFSET:
    case DIG_SOC_LANE_OVRD_REG5_LANE_RXLINKEVAL_OVRD_EN_A_OFFSET:
    case DIG_SOC_LANE_OVRD_REG5_LANE_RXMARGIN_OVRD_EN_A_OFFSET:
      mss->derived_from_name_offset = mss->rx_lane_offset;
      break;
    default:
      break;
    }
  } else if (addr == DIG_SOC_LANE_OVRD_REG1_ADDR) {
    if (fld_offset == DIG_SOC_LANE_OVRD_REG1_ICTL_CK_TX_BLOCK_DATA_ENA_A_OFFSET) {
      mss->derived_from_name_offset = mss->tx_lane_offset;
    }
  }

  //test ===========================================
  // handle settings that get remapped if map_en=1
  if (addr == DIG_SOC_LANE_OVRD_REG2_ADDR) {
    switch (fld_offset) {
    case DIG_SOC_LANE_OVRD_REG2_ICTL_RX_STATE_WIDTH_A_OFFSET:
    case DIG_SOC_LANE_OVRD_REG2_ICTL_RX_STATE_RATE_A_OFFSET:
    case DIG_SOC_LANE_OVRD_REG2_ICTL_RX_LINKEVAL_TYPE_A_OFFSET:
    case DIG_SOC_LANE_OVRD_REG2_ICTL_RX_LINKEVAL_REQ_A_OFFSET:
      mss->derived_from_name_offset = mss->tx_lane_offset;
    default: break;
    }
  } else if (addr == DIG_SOC_LANE_OVRD_REG3_ADDR) {
    switch (fld_offset) {
    case DIG_SOC_LANE_OVRD_REG3_ICTL_RX_STATE_POWER_A_OFFSET:
    case DIG_SOC_LANE_OVRD_REG3_ICTL_RX_STATE_REQ_A_OFFSET:
      mss->derived_from_name_offset = mss->tx_lane_offset;
    default: break;
    }
  } else if (addr == DIG_SOC_LANE_OVRD_REG5_ADDR) {
    switch (fld_offset) {
    case DIG_SOC_LANE_OVRD_REG5_LANE_RXSTATE_OVRD_EN_A_OFFSET:
    case DIG_SOC_LANE_OVRD_REG5_LANE_RXLINKEVAL_OVRD_EN_A_OFFSET:
      mss->derived_from_name_offset = mss->tx_lane_offset;
    default: break;
    }
  } else if (addr == DIG_SOC_LANE_STAT_REG1_ADDR) {
    switch (fld_offset) {
    case DIG_SOC_LANE_STAT_REG1_OCTL_RX_STATE_ACK_OFFSET:
    case DIG_SOC_LANE_STAT_REG1_OCTL_RX_LINKEVAL_DIR_OFFSET:
    case DIG_SOC_LANE_STAT_REG1_OCTL_RX_LINKEVAL_ACK_OFFSET:
      mss->derived_from_name_offset = mss->tx_lane_offset;
    default: break;
    }
  } else if (addr == DIG_SOC_LANE_STAT_REG3_ADDR) {
    switch (fld_offset) {
    case DIG_SOC_LANE_STAT_REG3_OCTL_RX_LINKEVAL_FOM_OFFSET:
      mss->derived_from_name_offset = mss->tx_lane_offset;
    default: break;
    }
  }
  //test ===========================================

  return(pmd_read_field(mss, addr, fld_mask, fld_offset, rval));
}

#define W_1G  3
#define W_10G 2
#define W_25G 4
#define W_50G 6
#define W_100G 7

int aw_pmd_4ln_anlt_width_set(mss_access_t *mss) {

    CHECK(pmd_write_field(mss, ETH_ANLT_WIDTH_REG1_ADDR, ETH_ANLT_WIDTH_REG1_AN_DEF_SPEC_WIDTH_0R_MASK, ETH_ANLT_WIDTH_REG1_AN_DEF_SPEC_WIDTH_0R_OFFSET, W_1G));
    CHECK(pmd_write_field(mss, ETH_ANLT_WIDTH_REG1_ADDR, ETH_ANLT_WIDTH_REG1_AN_DEF_SPEC_WIDTH_1R_MASK, ETH_ANLT_WIDTH_REG1_AN_DEF_SPEC_WIDTH_1R_OFFSET, W_10G));
    CHECK(pmd_write_field(mss, ETH_ANLT_WIDTH_REG1_ADDR, ETH_ANLT_WIDTH_REG1_AN_DEF_SPEC_WIDTH_2R_MASK, ETH_ANLT_WIDTH_REG1_AN_DEF_SPEC_WIDTH_2R_OFFSET, W_10G));
    CHECK(pmd_write_field(mss, ETH_ANLT_WIDTH_REG1_ADDR, ETH_ANLT_WIDTH_REG1_AN_DEF_SPEC_WIDTH_3R_MASK, ETH_ANLT_WIDTH_REG1_AN_DEF_SPEC_WIDTH_3R_OFFSET, W_10G));
    CHECK(pmd_write_field(mss, ETH_ANLT_WIDTH_REG1_ADDR, ETH_ANLT_WIDTH_REG1_AN_DEF_SPEC_WIDTH_4R_MASK, ETH_ANLT_WIDTH_REG1_AN_DEF_SPEC_WIDTH_4R_OFFSET, W_10G));
    CHECK(pmd_write_field(mss, ETH_ANLT_WIDTH_REG1_ADDR, ETH_ANLT_WIDTH_REG1_AN_DEF_SPEC_WIDTH_5R_MASK, ETH_ANLT_WIDTH_REG1_AN_DEF_SPEC_WIDTH_5R_OFFSET, W_25G));
    CHECK(pmd_write_field(mss, ETH_ANLT_WIDTH_REG1_ADDR, ETH_ANLT_WIDTH_REG1_AN_DEF_SPEC_WIDTH_6R_MASK, ETH_ANLT_WIDTH_REG1_AN_DEF_SPEC_WIDTH_6R_OFFSET, W_25G));
    CHECK(pmd_write_field(mss, ETH_ANLT_WIDTH_REG1_ADDR, ETH_ANLT_WIDTH_REG1_AN_DEF_SPEC_WIDTH_7R_MASK, ETH_ANLT_WIDTH_REG1_AN_DEF_SPEC_WIDTH_7R_OFFSET, W_25G));
    CHECK(pmd_write_field(mss, ETH_ANLT_WIDTH_REG2_ADDR, ETH_ANLT_WIDTH_REG2_AN_DEF_SPEC_WIDTH_8R_MASK, ETH_ANLT_WIDTH_REG2_AN_DEF_SPEC_WIDTH_8R_OFFSET, W_25G));
    CHECK(pmd_write_field(mss, ETH_ANLT_WIDTH_REG2_ADDR, ETH_ANLT_WIDTH_REG2_AN_DEF_SPEC_WIDTH_9R_MASK, ETH_ANLT_WIDTH_REG2_AN_DEF_SPEC_WIDTH_9R_OFFSET, W_25G));
    CHECK(pmd_write_field(mss, ETH_ANLT_WIDTH_REG2_ADDR, ETH_ANLT_WIDTH_REG2_AN_DEF_SPEC_WIDTH_10R_MASK, ETH_ANLT_WIDTH_REG2_AN_DEF_SPEC_WIDTH_10R_OFFSET, W_25G));
    CHECK(pmd_write_field(mss, ETH_ANLT_WIDTH_REG2_ADDR, ETH_ANLT_WIDTH_REG2_AN_DEF_SPEC_WIDTH_11R_MASK, ETH_ANLT_WIDTH_REG2_AN_DEF_SPEC_WIDTH_11R_OFFSET, W_25G));
    CHECK(pmd_write_field(mss, ETH_ANLT_WIDTH_REG2_ADDR, ETH_ANLT_WIDTH_REG2_AN_DEF_SPEC_WIDTH_12R_MASK, ETH_ANLT_WIDTH_REG2_AN_DEF_SPEC_WIDTH_12R_OFFSET, W_25G));
    CHECK(pmd_write_field(mss, ETH_ANLT_WIDTH_REG2_ADDR, ETH_ANLT_WIDTH_REG2_AN_DEF_SPEC_WIDTH_13R_MASK, ETH_ANLT_WIDTH_REG2_AN_DEF_SPEC_WIDTH_13R_OFFSET, W_50G));
    CHECK(pmd_write_field(mss, ETH_ANLT_WIDTH_REG2_ADDR, ETH_ANLT_WIDTH_REG2_AN_DEF_SPEC_WIDTH_14R_MASK, ETH_ANLT_WIDTH_REG2_AN_DEF_SPEC_WIDTH_14R_OFFSET, W_50G));
    CHECK(pmd_write_field(mss, ETH_ANLT_WIDTH_REG2_ADDR, ETH_ANLT_WIDTH_REG2_AN_DEF_SPEC_WIDTH_15R_MASK, ETH_ANLT_WIDTH_REG2_AN_DEF_SPEC_WIDTH_15R_OFFSET, W_50G));
    CHECK(pmd_write_field(mss, ETH_ANLT_WIDTH_REG3_ADDR, ETH_ANLT_WIDTH_REG3_AN_DEF_SPEC_WIDTH_16R_MASK, ETH_ANLT_WIDTH_REG3_AN_DEF_SPEC_WIDTH_16R_OFFSET, W_100G));
    CHECK(pmd_write_field(mss, ETH_ANLT_WIDTH_REG3_ADDR, ETH_ANLT_WIDTH_REG3_AN_DEF_SPEC_WIDTH_17R_MASK, ETH_ANLT_WIDTH_REG3_AN_DEF_SPEC_WIDTH_17R_OFFSET, W_100G));
    CHECK(pmd_write_field(mss, ETH_ANLT_WIDTH_REG3_ADDR, ETH_ANLT_WIDTH_REG3_AN_DEF_SPEC_WIDTH_18R_MASK, ETH_ANLT_WIDTH_REG3_AN_DEF_SPEC_WIDTH_18R_OFFSET, W_100G));
#if 0
    CHECK(pmd_write_field(mss, ETH_ANLT_WIDTH_REG3_ADDR, ETH_ANLT_WIDTH_REG3_AN_DEF_SPEC_WIDTH_19R_MASK, ETH_ANLT_WIDTH_REG3_AN_DEF_SPEC_WIDTH_19R_OFFSET, [19]));
    CHECK(pmd_write_field(mss, ETH_ANLT_WIDTH_REG3_ADDR, ETH_ANLT_WIDTH_REG3_AN_DEF_SPEC_WIDTH_20R_MASK, ETH_ANLT_WIDTH_REG3_AN_DEF_SPEC_WIDTH_20R_OFFSET, [20]));
    CHECK(pmd_write_field(mss, ETH_ANLT_WIDTH_REG3_ADDR, ETH_ANLT_WIDTH_REG3_AN_DEF_SPEC_WIDTH_21R_MASK, ETH_ANLT_WIDTH_REG3_AN_DEF_SPEC_WIDTH_21R_OFFSET, [21]));
    CHECK(pmd_write_field(mss, ETH_ANLT_WIDTH_REG3_ADDR, ETH_ANLT_WIDTH_REG3_AN_DEF_SPEC_WIDTH_22R_MASK, ETH_ANLT_WIDTH_REG3_AN_DEF_SPEC_WIDTH_22R_OFFSET, [22]));
#endif
    CHECK(pmd_write_field(mss, ETH_ANLT_WIDTH_REG3_ADDR, ETH_ANLT_WIDTH_REG3_AN_DEF_SPEC_WIDTH_23R_MASK, ETH_ANLT_WIDTH_REG3_AN_DEF_SPEC_WIDTH_23R_OFFSET, W_25G));
    CHECK(pmd_write_field(mss, ETH_ANLT_WIDTH_REG4_ADDR, ETH_ANLT_WIDTH_REG4_AN_DEF_SPEC_WIDTH_24R_MASK, ETH_ANLT_WIDTH_REG4_AN_DEF_SPEC_WIDTH_24R_OFFSET, W_25G));
    CHECK(pmd_write_field(mss, ETH_ANLT_WIDTH_REG4_ADDR, ETH_ANLT_WIDTH_REG4_AN_DEF_SPEC_WIDTH_25R_MASK, ETH_ANLT_WIDTH_REG4_AN_DEF_SPEC_WIDTH_25R_OFFSET, W_25G));
    CHECK(pmd_write_field(mss, ETH_ANLT_WIDTH_REG4_ADDR, ETH_ANLT_WIDTH_REG4_AN_DEF_SPEC_WIDTH_26R_MASK, ETH_ANLT_WIDTH_REG4_AN_DEF_SPEC_WIDTH_26R_OFFSET, W_25G));
    CHECK(pmd_write_field(mss, ETH_ANLT_WIDTH_REG4_ADDR, ETH_ANLT_WIDTH_REG4_AN_DEF_SPEC_WIDTH_27R_MASK, ETH_ANLT_WIDTH_REG4_AN_DEF_SPEC_WIDTH_27R_OFFSET, W_50G));

#if 0
    // set the rate/width AN is performed at
     CHECK(pmd_write_field(mss, ETH_AN_PMA_DEF_REG1_ADDR, ETH_AN_PMA_DEF_REG1_AN_DEF_AN_RATE_MASK, ETH_AN_PMA_DEF_REG1_AN_DEF_AN_RATE_OFFSET, 1));
     CHECK(pmd_write_field(mss, ETH_AN_PMA_DEF_REG1_ADDR, ETH_AN_PMA_DEF_REG1_AN_DEF_AN_WIDTH_MASK, ETH_AN_PMA_DEF_REG1_AN_DEF_AN_WIDTH_OFFSET, 2));
    // set the 10G rate code
    CHECK(pmd_write_field(mss, ETH_ANLT_RATE_REG1_ADDR, ETH_ANLT_RATE_REG1_AN_DEF_SPEC_RATE_2R_MASK, ETH_ANLT_RATE_REG1_AN_DEF_SPEC_RATE_2R_OFFSET, 1));
#else
    // set the rate/width AN is performed at
     CHECK(pmd_write_field(mss, ETH_AN_PMA_DEF_REG1_ADDR, ETH_AN_PMA_DEF_REG1_AN_DEF_AN_RATE_MASK, ETH_AN_PMA_DEF_REG1_AN_DEF_AN_RATE_OFFSET, 3));
     CHECK(pmd_write_field(mss, ETH_AN_PMA_DEF_REG1_ADDR, ETH_AN_PMA_DEF_REG1_AN_DEF_AN_WIDTH_MASK, ETH_AN_PMA_DEF_REG1_AN_DEF_AN_WIDTH_OFFSET, 3));
    // set the 10G rate code
    CHECK(pmd_write_field(mss, ETH_ANLT_RATE_REG1_ADDR, ETH_ANLT_RATE_REG1_AN_DEF_SPEC_RATE_2R_MASK, ETH_ANLT_RATE_REG1_AN_DEF_SPEC_RATE_2R_OFFSET, 3));
#endif


#if 0
    CHECK(pmd_write_field(mss, ETH_ANLT_RATE_REG1_ADDR, ETH_ANLT_RATE_REG1_AN_DEF_SPEC_RATE_0R_MASK, ETH_ANLT_RATE_REG1_AN_DEF_SPEC_RATE_0R_OFFSET, &newdef_get->an_def_spec_rate[0]));
    CHECK(pmd_write_field(mss, ETH_ANLT_RATE_REG1_ADDR, ETH_ANLT_RATE_REG1_AN_DEF_SPEC_RATE_1R_MASK, ETH_ANLT_RATE_REG1_AN_DEF_SPEC_RATE_1R_OFFSET, &newdef_get->an_def_spec_rate[1]));
    CHECK(pmd_write_field(mss, ETH_ANLT_RATE_REG1_ADDR, ETH_ANLT_RATE_REG1_AN_DEF_SPEC_RATE_2R_MASK, ETH_ANLT_RATE_REG1_AN_DEF_SPEC_RATE_2R_OFFSET, &newdef_get->an_def_spec_rate[2]));
    CHECK(pmd_write_field(mss, ETH_ANLT_RATE_REG1_ADDR, ETH_ANLT_RATE_REG1_AN_DEF_SPEC_RATE_3R_MASK, ETH_ANLT_RATE_REG1_AN_DEF_SPEC_RATE_3R_OFFSET, &newdef_get->an_def_spec_rate[3]));
    CHECK(pmd_write_field(mss, ETH_ANLT_RATE_REG1_ADDR, ETH_ANLT_RATE_REG1_AN_DEF_SPEC_RATE_4R_MASK, ETH_ANLT_RATE_REG1_AN_DEF_SPEC_RATE_4R_OFFSET, &newdef_get->an_def_spec_rate[4]));
    CHECK(pmd_write_field(mss, ETH_ANLT_RATE_REG1_ADDR, ETH_ANLT_RATE_REG1_AN_DEF_SPEC_RATE_5R_MASK, ETH_ANLT_RATE_REG1_AN_DEF_SPEC_RATE_5R_OFFSET, &newdef_get->an_def_spec_rate[5]));
    CHECK(pmd_write_field(mss, ETH_ANLT_RATE_REG1_ADDR, ETH_ANLT_RATE_REG1_AN_DEF_SPEC_RATE_6R_MASK, ETH_ANLT_RATE_REG1_AN_DEF_SPEC_RATE_6R_OFFSET, &newdef_get->an_def_spec_rate[6]));
    CHECK(pmd_write_field(mss, ETH_ANLT_RATE_REG1_ADDR, ETH_ANLT_RATE_REG1_AN_DEF_SPEC_RATE_7R_MASK, ETH_ANLT_RATE_REG1_AN_DEF_SPEC_RATE_7R_OFFSET, &newdef_get->an_def_spec_rate[7]));
    CHECK(pmd_write_field(mss, ETH_ANLT_RATE_REG1_ADDR, ETH_ANLT_RATE_REG1_AN_DEF_SPEC_RATE_8R_MASK, ETH_ANLT_RATE_REG1_AN_DEF_SPEC_RATE_8R_OFFSET, &newdef_get->an_def_spec_rate[8]));
    CHECK(pmd_write_field(mss, ETH_ANLT_RATE_REG1_ADDR, ETH_ANLT_RATE_REG1_AN_DEF_SPEC_RATE_9R_MASK, ETH_ANLT_RATE_REG1_AN_DEF_SPEC_RATE_9R_OFFSET, &newdef_get->an_def_spec_rate[9]));
    CHECK(pmd_write_field(mss, ETH_ANLT_RATE_REG2_ADDR, ETH_ANLT_RATE_REG2_AN_DEF_SPEC_RATE_10R_MASK, ETH_ANLT_RATE_REG2_AN_DEF_SPEC_RATE_10R_OFFSET, &newdef_get->an_def_spec_rate[10]));
    CHECK(pmd_write_field(mss, ETH_ANLT_RATE_REG2_ADDR, ETH_ANLT_RATE_REG2_AN_DEF_SPEC_RATE_11R_MASK, ETH_ANLT_RATE_REG2_AN_DEF_SPEC_RATE_11R_OFFSET, &newdef_get->an_def_spec_rate[11]));
    CHECK(pmd_write_field(mss, ETH_ANLT_RATE_REG2_ADDR, ETH_ANLT_RATE_REG2_AN_DEF_SPEC_RATE_12R_MASK, ETH_ANLT_RATE_REG2_AN_DEF_SPEC_RATE_12R_OFFSET, &newdef_get->an_def_spec_rate[12]));
    CHECK(pmd_write_field(mss, ETH_ANLT_RATE_REG2_ADDR, ETH_ANLT_RATE_REG2_AN_DEF_SPEC_RATE_13R_MASK, ETH_ANLT_RATE_REG2_AN_DEF_SPEC_RATE_13R_OFFSET, &newdef_get->an_def_spec_rate[13]));
    CHECK(pmd_write_field(mss, ETH_ANLT_RATE_REG2_ADDR, ETH_ANLT_RATE_REG2_AN_DEF_SPEC_RATE_14R_MASK, ETH_ANLT_RATE_REG2_AN_DEF_SPEC_RATE_14R_OFFSET, &newdef_get->an_def_spec_rate[14]));
    CHECK(pmd_write_field(mss, ETH_ANLT_RATE_REG2_ADDR, ETH_ANLT_RATE_REG2_AN_DEF_SPEC_RATE_15R_MASK, ETH_ANLT_RATE_REG2_AN_DEF_SPEC_RATE_15R_OFFSET, &newdef_get->an_def_spec_rate[15]));
    CHECK(pmd_write_field(mss, ETH_ANLT_RATE_REG2_ADDR, ETH_ANLT_RATE_REG2_AN_DEF_SPEC_RATE_16R_MASK, ETH_ANLT_RATE_REG2_AN_DEF_SPEC_RATE_16R_OFFSET, &newdef_get->an_def_spec_rate[16]));
    CHECK(pmd_write_field(mss, ETH_ANLT_RATE_REG2_ADDR, ETH_ANLT_RATE_REG2_AN_DEF_SPEC_RATE_17R_MASK, ETH_ANLT_RATE_REG2_AN_DEF_SPEC_RATE_17R_OFFSET, &newdef_get->an_def_spec_rate[17]));
    CHECK(pmd_write_field(mss, ETH_ANLT_RATE_REG2_ADDR, ETH_ANLT_RATE_REG2_AN_DEF_SPEC_RATE_18R_MASK, ETH_ANLT_RATE_REG2_AN_DEF_SPEC_RATE_18R_OFFSET, &newdef_get->an_def_spec_rate[18]));
    CHECK(pmd_write_field(mss, ETH_ANLT_RATE_REG2_ADDR, ETH_ANLT_RATE_REG2_AN_DEF_SPEC_RATE_19R_MASK, ETH_ANLT_RATE_REG2_AN_DEF_SPEC_RATE_19R_OFFSET, &newdef_get->an_def_spec_rate[19]));
    CHECK(pmd_write_field(mss, ETH_ANLT_RATE_REG3_ADDR, ETH_ANLT_RATE_REG3_AN_DEF_SPEC_RATE_20R_MASK, ETH_ANLT_RATE_REG3_AN_DEF_SPEC_RATE_20R_OFFSET, &newdef_get->an_def_spec_rate[20]));
    CHECK(pmd_write_field(mss, ETH_ANLT_RATE_REG3_ADDR, ETH_ANLT_RATE_REG3_AN_DEF_SPEC_RATE_21R_MASK, ETH_ANLT_RATE_REG3_AN_DEF_SPEC_RATE_21R_OFFSET, &newdef_get->an_def_spec_rate[21]));
    CHECK(pmd_write_field(mss, ETH_ANLT_RATE_REG3_ADDR, ETH_ANLT_RATE_REG3_AN_DEF_SPEC_RATE_22R_MASK, ETH_ANLT_RATE_REG3_AN_DEF_SPEC_RATE_22R_OFFSET, &newdef_get->an_def_spec_rate[22]));
    CHECK(pmd_write_field(mss, ETH_ANLT_RATE_REG3_ADDR, ETH_ANLT_RATE_REG3_AN_DEF_SPEC_RATE_23R_MASK, ETH_ANLT_RATE_REG3_AN_DEF_SPEC_RATE_23R_OFFSET, &newdef_get->an_def_spec_rate[23]));
    CHECK(pmd_write_field(mss, ETH_ANLT_RATE_REG3_ADDR, ETH_ANLT_RATE_REG3_AN_DEF_SPEC_RATE_24R_MASK, ETH_ANLT_RATE_REG3_AN_DEF_SPEC_RATE_24R_OFFSET, &newdef_get->an_def_spec_rate[24]));
    CHECK(pmd_write_field(mss, ETH_ANLT_RATE_REG3_ADDR, ETH_ANLT_RATE_REG3_AN_DEF_SPEC_RATE_25R_MASK, ETH_ANLT_RATE_REG3_AN_DEF_SPEC_RATE_25R_OFFSET, &newdef_get->an_def_spec_rate[25]));
    CHECK(pmd_write_field(mss, ETH_ANLT_RATE_REG3_ADDR, ETH_ANLT_RATE_REG3_AN_DEF_SPEC_RATE_26R_MASK, ETH_ANLT_RATE_REG3_AN_DEF_SPEC_RATE_26R_OFFSET, &newdef_get->an_def_spec_rate[26]));
    CHECK(pmd_write_field(mss, ETH_ANLT_RATE_REG3_ADDR, ETH_ANLT_RATE_REG3_AN_DEF_SPEC_RATE_27R_MASK, ETH_ANLT_RATE_REG3_AN_DEF_SPEC_RATE_27R_OFFSET, &newdef_get->an_def_spec_rate[27]));

    CHECK(pmd_write_field(mss, ETH_AN_PMA_DEF_REG1_ADDR, ETH_AN_PMA_DEF_REG1_AN_DEF_AN_RATE_MASK, ETH_AN_PMA_DEF_REG1_AN_DEF_AN_RATE_OFFSET, &newdef_get->an_def_an_rate));

    CHECK(pmd_write_field(mss, ETH_AN_NEWDEF_REG1_ADDR, ETH_AN_NEWDEF_REG1_NEWDEF1_LANE_NUM_U_MASK, ETH_AN_NEWDEF_REG1_NEWDEF1_LANE_NUM_U_OFFSET, &newdef_get->newdef1.lane_num_u));
    CHECK(pmd_write_field(mss, ETH_AN_NEWDEF_REG1_ADDR, ETH_AN_NEWDEF_REG1_NEWDEF1_LT_SEL_U_MASK, ETH_AN_NEWDEF_REG1_NEWDEF1_LT_SEL_U_OFFSET, &newdef_get->newdef1.lt_sel_u));
    CHECK(pmd_write_field(mss, ETH_AN_NEWDEF_REG1_ADDR, ETH_AN_NEWDEF_REG1_NEWDEF1_PAM_U_MASK, ETH_AN_NEWDEF_REG1_NEWDEF1_PAM_U_OFFSET, &newdef_get->newdef1.pam_u));
    CHECK(pmd_write_field(mss, ETH_AN_NEWDEF_REG1_ADDR, ETH_AN_NEWDEF_REG1_NEWDEF1_TIMER_SEL_U_MASK, ETH_AN_NEWDEF_REG1_NEWDEF1_TIMER_SEL_U_OFFSET, &newdef_get->newdef1.timer_sel_u));

    CHECK(pmd_write_field(mss, ETH_AN_NEWDEF_REG1_ADDR, ETH_AN_NEWDEF_REG1_NEWDEF2_LANE_NUM_U_MASK, ETH_AN_NEWDEF_REG1_NEWDEF2_LANE_NUM_U_OFFSET, &newdef_get->newdef2.lane_num_u));
    CHECK(pmd_write_field(mss, ETH_AN_NEWDEF_REG1_ADDR, ETH_AN_NEWDEF_REG1_NEWDEF2_LT_SEL_U_MASK, ETH_AN_NEWDEF_REG1_NEWDEF2_LT_SEL_U_OFFSET, &newdef_get->newdef2.lt_sel_u));
    CHECK(pmd_write_field(mss, ETH_AN_NEWDEF_REG1_ADDR, ETH_AN_NEWDEF_REG1_NEWDEF2_PAM_U_MASK, ETH_AN_NEWDEF_REG1_NEWDEF2_PAM_U_OFFSET, &newdef_get->newdef2.pam_u));
    CHECK(pmd_write_field(mss, ETH_AN_NEWDEF_REG1_ADDR, ETH_AN_NEWDEF_REG1_NEWDEF2_TIMER_SEL_U_MASK, ETH_AN_NEWDEF_REG1_NEWDEF2_TIMER_SEL_U_OFFSET, &newdef_get->newdef2.timer_sel_u));

    CHECK(pmd_write_field(mss, ETH_AN_NEWDEF_REG1_ADDR, ETH_AN_NEWDEF_REG1_NEWDEF3_LANE_NUM_U_MASK, ETH_AN_NEWDEF_REG1_NEWDEF3_LANE_NUM_U_OFFSET, &newdef_get->newdef3.lane_num_u));
    CHECK(pmd_write_field(mss, ETH_AN_NEWDEF_REG2_ADDR, ETH_AN_NEWDEF_REG2_NEWDEF3_LT_SEL_U_MASK, ETH_AN_NEWDEF_REG2_NEWDEF3_LT_SEL_U_OFFSET, &newdef_get->newdef3.lt_sel_u));
    CHECK(pmd_write_field(mss, ETH_AN_NEWDEF_REG2_ADDR, ETH_AN_NEWDEF_REG2_NEWDEF3_PAM_U_MASK, ETH_AN_NEWDEF_REG2_NEWDEF3_PAM_U_OFFSET, &newdef_get->newdef3.pam_u));
    CHECK(pmd_write_field(mss, ETH_AN_NEWDEF_REG2_ADDR, ETH_AN_NEWDEF_REG2_NEWDEF3_TIMER_SEL_U_MASK, ETH_AN_NEWDEF_REG2_NEWDEF3_TIMER_SEL_U_OFFSET, &newdef_get->newdef3.timer_sel_u));
#endif
    return AW_ERR_CODE_NONE;
}

int aw_pmd_4ln_anlt_ms_per_clk_set(mss_access_t *mss) {
  CHECK(pmd_write_field(mss, ETH_ANLT_CTRL_ADDR, ETH_ANLT_CTRL_ANLT_MS_PER_CK_MASK, ETH_ANLT_CTRL_ANLT_MS_PER_CK_OFFSET, 156250));
  return 0;
}

void bf_pmd_4ln_dump_csv_header(int branch) {

    // dump header line to csv file
    USR_PRINTF("fw_nsw,");
    USR_PRINTF("fw_nsw_eq,");
    USR_PRINTF("isi,");
    USR_PRINTF("port,");
    USR_PRINTF("lane,");
    USR_PRINTF("dev_port,");
    USR_PRINTF("spec,");
    USR_PRINTF("cmn_pstate,");
    USR_PRINTF("tx_pstate,");
    USR_PRINTF("rx_pstate,");
    USR_PRINTF("tx_rate,");
    USR_PRINTF("rx_rate,");
    USR_PRINTF("tx_width,");
    USR_PRINTF("rx_width,");
    USR_PRINTF("perf_mode,");
    USR_PRINTF("first_ber,");
    USR_PRINTF("first_bist_locked,");
    USR_PRINTF("ber,");
    USR_PRINTF("ber_snr,");
    USR_PRINTF("ber_bathtub,");
    USR_PRINTF("fom_peaks,");
    USR_PRINTF("fom_width_upp,");
    USR_PRINTF("fom_width_low,");
    USR_PRINTF("fom_width_avg,");
    USR_PRINTF("fom_ratio_p_w,");
    USR_PRINTF("fom_FW,");
    USR_PRINTF("fom_lms,");
    USR_PRINTF("fom_watchdog,");
    USR_PRINTF("mlsd_ber,");
    USR_PRINTF("mlsd_snr,");
    USR_PRINTF("snr_gain,");
    USR_PRINTF("lt_state,");
    USR_PRINTF("msb,");
    USR_PRINTF("lsb,");
    USR_PRINTF("pre3,");
    USR_PRINTF("pre2,");
    USR_PRINTF("pre1,");
    USR_PRINTF("c0,");
    USR_PRINTF("post1,");
    USR_PRINTF("vga_offset,");
    USR_PRINTF("ctle_rate,");
    USR_PRINTF("acc_boost,");
    USR_PRINTF("ctle_boost,");
    USR_PRINTF("vga_coarse,");
    USR_PRINTF("vga_fine,");
    USR_PRINTF("vga_cap,");
    USR_PRINTF("vga_cap1,");
    USR_PRINTF("vga_cap2,");
    USR_PRINTF("ctle_boost1,");
    USR_PRINTF("ctle_boost2,");
    USR_PRINTF("vga1,");
    USR_PRINTF("vga2,");
    USR_PRINTF("kprop,");
    USR_PRINTF("kint,");
    USR_PRINTF("meas_temp,");
    USR_PRINTF("cmn_osc,");
    USR_PRINTF("cmn_osc_int_avg,");
    USR_PRINTF("cmn_osc_int_min,");
    USR_PRINTF("cmn_osc_int_max,");
    USR_PRINTF("tx_osc,");
    USR_PRINTF("tx_osc_int_avg,");
    USR_PRINTF("tx_osc_int_min,");
    USR_PRINTF("tx_osc_int_max,");
    USR_PRINTF("rx_osc,");
    USR_PRINTF("rx_osc_int_avg,");
    USR_PRINTF("rx_osc_int_min,");
    USR_PRINTF("rx_osc_int_max,");
    USR_PRINTF("cmn_pstate,");
    USR_PRINTF("tx_pstate,");
    USR_PRINTF("rx_pstate,");
    USR_PRINTF("cdr_locked,");
    USR_PRINTF("cdr_offset,");
    USR_PRINTF("cdr_dir,");
    USR_PRINTF("rx_bist_locked,");
    USR_PRINTF("tx_bist_pattern,");
    USR_PRINTF("rx_bist_pattern,");
    USR_PRINTF("bist_mode,");
    USR_PRINTF("tx_dcd_d0,");
    USR_PRINTF("tx_dcd_d90,");
    USR_PRINTF("tx_iq,");
    USR_PRINTF("rx_dcd_d0,");
    USR_PRINTF("rx_dcd_d90,");
    USR_PRINTF("rx_iq,");
    USR_PRINTF("roaming_mode,");
    USR_PRINTF("roaming_window_sel1,");
    USR_PRINTF("roaming_window_sel2,");
    USR_PRINTF("roaming_bank0_energy,");
    USR_PRINTF("roaming_bank1_energy,");
    USR_PRINTF("ffe_pulse_response,");
    USR_PRINTF("eqbk_counter,");

    USR_PRINTF("branch,");
    if (branch == 0) {
        USR_PRINTF("dcoffset,");
        USR_PRINTF("dfe_coeff,");
        USR_PRINTF("pre1_ratio,");
        USR_PRINTF("post1_ratio,");
        USR_PRINTF("pre_ffe,");
        USR_PRINTF("pre_dfe_ffe,");
        USR_PRINTF("pre_ffe_ctle0,");
        USR_PRINTF("pre_ffe_ctle1,");
        USR_PRINTF("pre_ffe_ctle2,");
        USR_PRINTF("pre_ffe_ctle3,");
        USR_PRINTF("pre_ffe_ctle4,");
        USR_PRINTF("pre_ffe_ctle5,");
        USR_PRINTF("ffe_coeff,");
        USR_PRINTF("slicer_threshold_eh,");
        USR_PRINTF("slicer_threshold_ez,");
        USR_PRINTF("slicer_threshold_el,");
        USR_PRINTF("slicer_target_el3,");
        USR_PRINTF("slicer_target_el1,");
        USR_PRINTF("slicer_target_eh1,");
        USR_PRINTF("slicer_target_eh3,");
    } else {
        USR_PRINTF("dcoffset[branch],");
        USR_PRINTF("dfe_coeff[branch],");
        USR_PRINTF("pre1_ratio,");
        USR_PRINTF("post1_ratio,");
        USR_PRINTF("pre_ffe,");
        USR_PRINTF("pre_dfe_ffe,");
        USR_PRINTF("pre_ffe_ctle0,");
        USR_PRINTF("pre_ffe_ctle1,");
        USR_PRINTF("pre_ffe_ctle2,");
        USR_PRINTF("pre_ffe_ctle3,");
        USR_PRINTF("pre_ffe_ctle4,");
        USR_PRINTF("pre_ffe_ctle5,");
        USR_PRINTF("ffe_coeff[branch],");
        USR_PRINTF("slicer_threshold_eh[branch],");
        USR_PRINTF("slicer_threshold_ez[branch],");
        USR_PRINTF("slicer_threshold_el[branch],");
        USR_PRINTF("slicer_target_el3[branch],");
        USR_PRINTF("slicer_target_el1[branch],");
        USR_PRINTF("slicer_target_eh1[branch],");
        USR_PRINTF("slicer_target_eh3[branch],");
    }
    USR_PRINTF("symbol_size,");
    USR_PRINTF("symbol_per_cw,");
    USR_PRINTF("burst_bit_threshold,");
    USR_PRINTF("burst_err_found,");
    USR_PRINTF("prefec_hist[0],");
    USR_PRINTF("prefec_hist[1],");
    USR_PRINTF("prefec_hist[2],");
    USR_PRINTF("prefec_hist[3],");
    USR_PRINTF("prefec_hist[4],");
    USR_PRINTF("prefec_hist[5],");
    USR_PRINTF("prefec_hist[6],");
    USR_PRINTF("prefec_hist[7],");
    USR_PRINTF("prefec_hist[8],");
    USR_PRINTF("prefec_hist[9],");
    USR_PRINTF("prefec_hist[10],");
    USR_PRINTF("prefec_hist[11],");
    USR_PRINTF("prefec_hist[12],");
    USR_PRINTF("prefec_hist[13],");
    USR_PRINTF("prefec_hist[14],");
    USR_PRINTF("prefec_hist[15],");
    USR_PRINTF("prefec_hist[16],");
    USR_PRINTF("prefec_hist[17],");
    USR_PRINTF("prefec_hist[18],");
    USR_PRINTF("prefec_hist[19],");
    USR_PRINTF("prefec_hist[20],");
    USR_PRINTF("prefec_hist[21],");
    USR_PRINTF("prefec_hist[22],");
    USR_PRINTF("prefec_hist[23],");
    USR_PRINTF("prefec_hist[24],");
    USR_PRINTF("prefec_hist[25],");
    USR_PRINTF("prefec_hist[26],");
    USR_PRINTF("prefec_hist[27],");
    USR_PRINTF("prefec_hist[28],");
    USR_PRINTF("prefec_hist[29],");
    USR_PRINTF("prefec_hist[30]\n"); // end of csv header
}

int bf_pmd_4ln_read_status(mss_access_t *mss, int branch) {
    uint32_t err_count_done, err_count_overflow,err_cnt_55_32,err_cnt_31_0;
    uint64_t err_count;
    aw_dsp_param_t dsp_info_[AW_NUM_BRANCHES];
    aw_afe_data_t rx_afe_data_;
    aw_dcdiq_data_t rx_dcdiq_data_;
    aw_dcdiq_data_t tx_dcdiq_data_;

    uint32_t tx_pll_fine_code;
    uint32_t tx_pll_coarse_code;
    double tx_pll_ppm;
    double tx_pll_vco;

    uint32_t rx_pll_fine_code;
    uint32_t rx_pll_coarse_code;
    double rx_pll_ppm;
    double rx_pll_vco;

    uint32_t lcpll_fine_code;
    uint32_t lcpll_coarse_code;
    double lcpll_ppm;
    double lcpll_vco;
    uint32_t lcpll_lock;

    uint32_t timing_window = 9;

    //New
    uint32_t txmfsm_rate_cur;
    uint32_t rxmfsm_rate_cur;
    uint32_t msb;
    uint32_t lsb;
    vga_opt_t vga_cap;
    uint32_t tx_bist_encoded, rx_bist_encoded;
    char * tx_bist_pattern;
    char * rx_bist_pattern;
    uint32_t cdr_offset;
    uint32_t cdr_dir;
    uint32_t use_custom_cdr_offset;
    aw_txfir_config_t txfir_cfg;
    uint32_t tx_max_elements;
    uint32_t cmn_kp;
    uint32_t cmn_ki;
    uint32_t rx_kprop1;
    uint32_t rx_kint1;
    uint32_t rx_kprop2;
    uint32_t rx_kint2;
    uint32_t rx_kprop3;
    uint32_t rx_kint3;
    uint32_t acc_boost;
    uint32_t prefec_hist_sum,sum_prefec_bits,prefec_hist_weighted_sum;
    uint32_t symbol_size;
    uint32_t symbol_per_cw;
    uint32_t corr_num_syms, wall_mode, skip_syms, timer_num_cw;
    double   ber_from_prefec;
    uint32_t burst_bit_threshold;
    uint32_t burst_err_found;
    uint32_t prefec_hist[31];
    uint32_t cdr_lock;
    uint32_t vga_cap_1;
    uint32_t vga_cap_2;

    uint32_t timer_threshold;
    uint32_t mode_uint32;
    uint32_t width_encoded;
    uint32_t bist_state;
    uint32_t lt_state;
    uint32_t an_state_arb;
    uint32_t lt_done;
    uint32_t link_good;

    CHECK(pmd_read_field(mss, RX_DATABIST_TOP_REG2_ADDR, RX_DATABIST_TOP_REG2_TIMER_THRESHOLD_NT_MASK, RX_DATABIST_TOP_REG2_TIMER_THRESHOLD_NT_OFFSET, &timer_threshold));
    CHECK(pmd_read_field(mss, RX_DATABIST_TOP_REG1_ADDR, RX_DATABIST_TOP_REG1_BIST_MODE_NT_MASK, RX_DATABIST_TOP_REG1_BIST_MODE_NT_OFFSET, &mode_uint32));
    CHECK(pmd_read_field(mss, RX_DATABIST_TOP_REG3_ADDR, RX_DATABIST_TOP_REG3_DATA_WIDTH_NT_MASK, RX_DATABIST_TOP_REG3_DATA_WIDTH_NT_OFFSET, &width_encoded));
    CHECK(pmd_read_field(mss, RX_DATABIST_TOP_RDREG1_ADDR, RX_DATABIST_TOP_RDREG1_STATE_NT_MASK, RX_DATABIST_TOP_RDREG1_STATE_NT_OFFSET, &bist_state));
    CHECK(pmd_read_field(mss, ETH_ANLT_STATE_REG2_ADDR, ETH_ANLT_STATE_REG2_LT_STATE_CTRL_FSM_MASK, ETH_ANLT_STATE_REG2_LT_STATE_CTRL_FSM_OFFSET, &lt_state));
    CHECK(pmd_read_field(mss, ETH_ANLT_STATE_REG1_ADDR, ETH_ANLT_STATE_REG1_AN_STATE_ARB_MASK, ETH_ANLT_STATE_REG1_AN_STATE_ARB_OFFSET, &an_state_arb));
    CHECK(pmd_read_field(mss, ETH_ANLT_STATUS_ADDR, ETH_ANLT_STATUS_AN_LINK_GOOD_MASK, ETH_ANLT_STATUS_AN_LINK_GOOD_OFFSET, &link_good));
    CHECK(pmd_read_field(mss, ETH_ANLT_STATUS_ADDR, ETH_ANLT_STATUS_LT_DONE_MASK, ETH_ANLT_STATUS_LT_DONE_OFFSET, &lt_done));

    CHECK(pmd_read_field(mss, TXMFSM_STAT_ADDR, TXMFSM_STAT_TXMFSM_RATE_CUR_MASK, TXMFSM_STAT_TXMFSM_RATE_CUR_OFFSET, &txmfsm_rate_cur));
    CHECK(pmd_read_field(mss, RXMFSM_STAT_ADDR, RXMFSM_STAT_RXMFSM_RATE_CUR_MASK, RXMFSM_STAT_RXMFSM_RATE_CUR_OFFSET, &rxmfsm_rate_cur));

    CHECK(pmd_read_field(mss, RX_DATABIST_TOP_RDREG1_ADDR, RX_DATABIST_TOP_RDREG1_ERROR_CNT_DONE_NT_MASK, RX_DATABIST_TOP_RDREG1_ERROR_CNT_DONE_NT_OFFSET, &err_count_done));
    CHECK(pmd_read_field(mss, RX_DATABIST_TOP_RDREG3_ADDR, RX_DATABIST_TOP_RDREG3_ERROR_CNT_55T32_NT_MASK, RX_DATABIST_TOP_RDREG3_ERROR_CNT_55T32_NT_OFFSET, &err_cnt_55_32));
    CHECK(pmd_read_field(mss, RX_DATABIST_TOP_RDREG2_ADDR, RX_DATABIST_TOP_RDREG2_ERROR_CNT_NT_MASK, RX_DATABIST_TOP_RDREG2_ERROR_CNT_NT_OFFSET, &err_cnt_31_0));
    CHECK(pmd_read_field(mss, RX_DATABIST_TOP_RDREG1_ADDR, RX_DATABIST_TOP_RDREG1_ERROR_CNT_OVERFLOW_NT_MASK, RX_DATABIST_TOP_RDREG1_ERROR_CNT_OVERFLOW_NT_OFFSET, &err_count_overflow));

    CHECK(aw_pmd_4ln_rx_cdr_lock_get(mss, &cdr_lock));


    err_count = (uint64_t) err_cnt_55_32 << 32 | (uint64_t) err_cnt_31_0;

    //If branch = -1, read all branches
    if (branch == -1) {
        for (int i=0; i<AW_NUM_BRANCHES; i++) {
            CHECK(aw_pmd_4ln_rx_dsp_get(mss, i, &dsp_info_[i]));
        }
    } else {
        CHECK(aw_pmd_4ln_rx_dsp_get(mss, branch, &dsp_info_[branch]));
    }
    CHECK(aw_pmd_4ln_rx_afe_get(mss, &rx_afe_data_));
    CHECK(aw_pmd_4ln_rx_dcdiq_get(mss, &rx_dcdiq_data_));
    CHECK(aw_pmd_4ln_tx_dcdiq_get(mss, &tx_dcdiq_data_));

    CHECK(aw_pmd_4ln_tx_pll_fine_code_get(mss, &tx_pll_fine_code,0,-1));
    CHECK(aw_pmd_4ln_tx_pll_coarse_code_get(mss, &tx_pll_coarse_code,0,-1));
    CHECK(aw_pmd_4ln_tx_ppm_get(mss, timing_window, 50, &tx_pll_ppm, &tx_pll_vco, 312500000));

    CHECK(aw_pmd_4ln_rx_pll_fine_code_get(mss, &rx_pll_fine_code,0,-1));
    CHECK(aw_pmd_4ln_rx_pll_coarse_code_get(mss, &rx_pll_coarse_code,0,-1));
    CHECK(aw_pmd_4ln_rx_ppm_get(mss, timing_window, 50, &rx_pll_ppm, &rx_pll_vco, 312500000));

    CHECK(aw_pmd_4ln_cmn_pll_fine_code_get(mss, &lcpll_fine_code,0,-1));
    CHECK(aw_pmd_4ln_cmn_pll_coarse_code_get(mss, &lcpll_coarse_code,0,-1));
    CHECK(aw_pmd_4ln_lcpll_vco_counter_get(mss, timing_window, &lcpll_ppm, &lcpll_vco, 312500000));
    CHECK(aw_pmd_4ln_pll_lock_get(mss, &lcpll_lock,0,0));

    CHECK(aw_pmd_4ln_tx_hbridge_get(mss, &msb, &lsb));

    CHECK(aw_pmd_4ln_rx_vga_cap_adapt_get(mss, &vga_cap));
    CHECK(pmd_read_field(mss, RX_VGA_REG1_ADDR, RX_VGA_REG1_CAP1_LUT_0_A_MASK, RX_VGA_REG1_CAP1_LUT_0_A_OFFSET, &vga_cap_1));
    CHECK(pmd_read_field(mss, RX_VGA_REG2_ADDR, RX_VGA_REG2_CAP2_LUT_0_A_MASK, RX_VGA_REG2_CAP2_LUT_0_A_OFFSET, &vga_cap_2));


    CHECK(pmd_read_field(mss, TX_DATAPATH_REG2_ADDR, TX_DATAPATH_REG2_PATTERN_SEL_NT_MASK, TX_DATAPATH_REG2_PATTERN_SEL_NT_OFFSET, &tx_bist_encoded));
    tx_bist_pattern = aw_bist_pattern_decoder (tx_bist_encoded);

    CHECK(pmd_read_field(mss, RX_DATABIST_TOP_REG2_ADDR, RX_DATABIST_TOP_REG2_PATTERN_SEL_NT_MASK, RX_DATABIST_TOP_REG2_PATTERN_SEL_NT_OFFSET, &rx_bist_encoded));
    rx_bist_pattern = aw_bist_pattern_decoder (rx_bist_encoded);

    CHECK(aw_pmd_4ln_rx_cdr_offset_get(mss,&use_custom_cdr_offset,&cdr_offset,&cdr_dir));

    txfir_cfg.main_or_max = 0; // note: this is an input
    CHECK(aw_pmd_4ln_txfir_config_get(mss,&txfir_cfg));
    tx_max_elements = txfir_cfg.C1 + txfir_cfg.C0 + txfir_cfg.CM1 + txfir_cfg.CM2 + txfir_cfg.CM3;

    CHECK(pmd_read_field(mss, AFE_CMN_LCPLL_OSC_REG1_ADDR, AFE_CMN_LCPLL_OSC_REG1_KP_NT_MASK, AFE_CMN_LCPLL_OSC_REG1_KP_NT_OFFSET, &cmn_kp));
    CHECK(pmd_read_field(mss, AFE_CMN_LCPLL_OSC_REG1_ADDR, AFE_CMN_LCPLL_OSC_REG1_KI_MU_NT_MASK, AFE_CMN_LCPLL_OSC_REG1_KI_MU_NT_OFFSET, &cmn_ki));
    CHECK(pmd_read_field(mss, RX_ITR_DPLL_DLPF_REG1_ADDR, RX_ITR_DPLL_DLPF_REG1_KPROP1_NT_MASK, RX_ITR_DPLL_DLPF_REG1_KPROP1_NT_OFFSET, &rx_kprop1));
    CHECK(pmd_read_field(mss, RX_ITR_DPLL_DLPF_REG2_ADDR, RX_ITR_DPLL_DLPF_REG2_KINT1_NT_MASK, RX_ITR_DPLL_DLPF_REG2_KINT1_NT_OFFSET, &rx_kint1));
    CHECK(pmd_read_field(mss, RX_ITR_DPLL_DLPF_REG1_ADDR, RX_ITR_DPLL_DLPF_REG1_KPROP2_NT_MASK, RX_ITR_DPLL_DLPF_REG1_KPROP2_NT_OFFSET, &rx_kprop2));
    CHECK(pmd_read_field(mss, RX_ITR_DPLL_DLPF_REG2_ADDR, RX_ITR_DPLL_DLPF_REG2_KINT2_NT_MASK, RX_ITR_DPLL_DLPF_REG2_KINT2_NT_OFFSET, &rx_kint2));
    CHECK(pmd_read_field(mss, RX_ITR_DPLL_DLPF_REG1_ADDR, RX_ITR_DPLL_DLPF_REG1_KPROP3_NT_MASK, RX_ITR_DPLL_DLPF_REG1_KPROP3_NT_OFFSET, &rx_kprop3));
    CHECK(pmd_read_field(mss, RX_ITR_DPLL_DLPF_REG2_ADDR, RX_ITR_DPLL_DLPF_REG2_KINT3_NT_MASK, RX_ITR_DPLL_DLPF_REG2_KINT3_NT_OFFSET, &rx_kint3));

    CHECK(pmd_read_field(mss, RX_ACC_ADDR, RX_ACC_BOOST_NT_MASK, RX_ACC_BOOST_NT_OFFSET, &acc_boost));

    CHECK(aw_pmd_4ln_rx_prefec_config_get(mss, &symbol_size, &symbol_per_cw, &corr_num_syms, &wall_mode, &skip_syms, &timer_num_cw));

    CHECK(pmd_read_field(mss, RX_DATABIST_TOP_REG14_ADDR, RX_DATABIST_TOP_REG14_BURST_ERROR_BITS_THRESHOLD_NT_MASK, RX_DATABIST_TOP_REG14_BURST_ERROR_BITS_THRESHOLD_NT_OFFSET, &burst_bit_threshold));
    CHECK(pmd_read_field(mss, RX_DATABIST_TOP_RDREG12_ADDR, RX_DATABIST_TOP_RDREG12_BURST_ERR_FOUND_CNT_NT_MASK, RX_DATABIST_TOP_RDREG12_BURST_ERR_FOUND_CNT_NT_OFFSET, &burst_err_found));

    CHECK(aw_pmd_4ln_rx_prefec_get_results(mss, prefec_hist));

    prefec_hist_sum = 0;
    for(int i=0; i<31; i++) {
        prefec_hist_sum += prefec_hist[i];
    }

    sum_prefec_bits = symbol_per_cw * symbol_size * prefec_hist_sum;

    prefec_hist_weighted_sum = 0;

    for(int i=0; i<31; i++) {
        prefec_hist_weighted_sum += prefec_hist[i] * i;
    }

    if (sum_prefec_bits == 0) {
        ber_from_prefec = 1;
    } else {
        ber_from_prefec = prefec_hist_weighted_sum / sum_prefec_bits;
    }

    uint32_t cmn_pstate, tx_pstate, rx_pstate;
    aw_pmd_4ln_iso_cmn_pstate_get(mss, &cmn_pstate);
    aw_pmd_4ln_iso_tx_pstate_get(mss, &tx_pstate);
    aw_pmd_4ln_iso_rx_pstate_get(mss, &rx_pstate);

    uint32_t tx_rate, rx_rate;
    aw_pmd_4ln_iso_tx_rate_get(mss, &tx_rate);
    aw_pmd_4ln_iso_rx_rate_get(mss, &rx_rate);

    uint32_t tx_width, rx_width;
    aw_pmd_4ln_iso_tx_width_get(mss, &tx_width);
    aw_pmd_4ln_iso_rx_width_get(mss, &rx_width);

    uint32_t bist_mode;
    CHECK(pmd_read_field(mss, RX_DATABIST_TOP_REG1_ADDR,
                              RX_DATABIST_TOP_REG1_BIST_MODE_NT_MASK,
                              RX_DATABIST_TOP_REG1_BIST_MODE_NT_OFFSET,
                              &bist_mode));
    uint32_t roaming_mode=0, roaming_window_sel_1=0, roaming_window_sel_2=0;

#if 0
    // 4ln doesn't have these
    CHECK(pmd_read_field(mss, RX_DATABLOCK_ROAMING_ADDR,
                              RX_DATABLOCK_ROAMING_MODE_NT_MASK,
                              RX_DATABLOCK_ROAMING_MODE_NT_OFFSET,
                              &roaming_mode));
    CHECK(pmd_read_field(mss, RX_DATABLOCK_ROAMING_ADDR,
                              RX_DATABLOCK_ROAMING_WINDOW_SELECT_1_NT_MASK,
                              RX_DATABLOCK_ROAMING_WINDOW_SELECT_1_NT_OFFSET,
                              &roaming_window_sel_1));
    CHECK(pmd_read_field(mss, RX_DATABLOCK_ROAMING_ADDR,
                              RX_DATABLOCK_ROAMING_WINDOW_SELECT_2_NT_MASK,
                              RX_DATABLOCK_ROAMING_WINDOW_SELECT_2_NT_OFFSET,
                              &roaming_window_sel_2));
#endif //0

    // read cmn_osc 100x, compute min/max/avg
    uint32_t data, dmin=0x7fffffff, dmax=0, dtotal=0;

    for (int n = 0; n < 100; n++) {
      CHECK(pmd_write_field(mss, AFE_CMN_LCPLL_OSC_REG3_ADDR,
                                 AFE_CMN_LCPLL_OSC_REG3_RO_CSR_CAPTURE_A_MASK,
                                 AFE_CMN_LCPLL_OSC_REG3_RO_CSR_CAPTURE_A_OFFSET,
                                 0));
      CHECK(pmd_write_field(mss, AFE_CMN_LCPLL_OSC_REG3_ADDR,
                                 AFE_CMN_LCPLL_OSC_REG3_RO_CSR_CAPTURE_A_MASK,
                                 AFE_CMN_LCPLL_OSC_REG3_RO_CSR_CAPTURE_A_OFFSET,
                                 1));
      CHECK(pmd_read_field(mss, AFE_CMN_LCPLL_OSC_RDREG1_ADDR,
                                AFE_CMN_LCPLL_OSC_RDREG1_CLKGEN_OSC_NT_MASK,
                                AFE_CMN_LCPLL_OSC_RDREG1_CLKGEN_OSC_NT_OFFSET,
                                &data));
      if (data < dmin) dmin = data;
      if (data > dmax) dmax = data;
      dtotal += data;
    }
    CHECK(pmd_read_field(mss, AFE_CMN_VCO_ADAPT_RDREG3_ADDR,
                              AFE_CMN_VCO_ADAPT_RDREG3_CLKGEN_OSC_CAL_MASK,
                              AFE_CMN_VCO_ADAPT_RDREG3_CLKGEN_OSC_CAL_OFFSET,
                              &data));
    uint32_t cmn_osc = data;
    uint32_t cmn_osc_int_avg = dtotal/100;
    uint32_t cmn_osc_int_min = dmin;
    uint32_t cmn_osc_int_max = dmax;

    data = 0;
    dmin = 0x7fffffff;
    dmax = 0;
    dtotal = 0;
    for (int n = 0; n < 100; n++) {
      CHECK(pmd_write_field(mss, TX_SSCM_DLPF_REG3_ADDR,
                                 TX_SSCM_DLPF_REG3_RO_CSR_CAPTURE_A_MASK,
                                 TX_SSCM_DLPF_REG3_RO_CSR_CAPTURE_A_OFFSET,
                                 0));
      CHECK(pmd_write_field(mss, TX_SSCM_DLPF_REG3_ADDR,
                                 TX_SSCM_DLPF_REG3_RO_CSR_CAPTURE_A_MASK,
                                 TX_SSCM_DLPF_REG3_RO_CSR_CAPTURE_A_OFFSET,
                                 1));
      CHECK(pmd_read_field(mss, TX_SSCM_DLPF_RDREG2_ADDR,
                                TX_SSCM_DLPF_RDREG2_CTL_AFE_DS_OSC_INT_NT_MASK,
                                TX_SSCM_DLPF_RDREG2_CTL_AFE_DS_OSC_INT_NT_OFFSET,
                                &data));
      if (data < dmin) dmin = data;
      if (data > dmax) dmax = data;
      dtotal += data;
    }
    CHECK(pmd_read_field(mss, TX_VCO_ADAPT_RDREG2_ADDR,
                              TX_VCO_ADAPT_RDREG2_DS_OSC_CAL_MASK,
                              TX_VCO_ADAPT_RDREG2_DS_OSC_CAL_OFFSET,
                              &data));
    uint32_t tx_osc = data;
    uint32_t tx_osc_int_avg = dtotal/100;
    uint32_t tx_osc_int_min = dmin;
    uint32_t tx_osc_int_max = dmax;

    data = 0;
    dmin = 0x7fffffff;
    dmax = 0;
    dtotal = 0;
    for (int n = 0; n < 100; n++) {
      CHECK(pmd_write_field(mss, RX_SSCM_DLPF_REG3_ADDR,
                                 RX_SSCM_DLPF_REG3_RO_CSR_CAPTURE_A_MASK,
                                 RX_SSCM_DLPF_REG3_RO_CSR_CAPTURE_A_OFFSET,
                                 0));
      CHECK(pmd_write_field(mss, RX_SSCM_DLPF_REG3_ADDR,
                                 RX_SSCM_DLPF_REG3_RO_CSR_CAPTURE_A_MASK,
                                 RX_SSCM_DLPF_REG3_RO_CSR_CAPTURE_A_OFFSET,
                                 1));
      CHECK(pmd_read_field(mss, RX_SSCM_DLPF_RDREG2_ADDR,
                                RX_SSCM_DLPF_RDREG2_CTL_AFE_DS_OSC_INT_NT_MASK,
                                RX_SSCM_DLPF_RDREG2_CTL_AFE_DS_OSC_INT_NT_OFFSET,
                                &data));
      if (data < dmin) dmin = data;
      if (data > dmax) dmax = data;
      dtotal += data;
    }
    CHECK(pmd_read_field(mss, RX_VCO_ADAPT_RDREG2_ADDR,
                              RX_VCO_ADAPT_RDREG2_DS_OSC_CAL_MASK,
                              RX_VCO_ADAPT_RDREG2_DS_OSC_CAL_OFFSET,
                              &data));
    uint32_t rx_osc = data;
    uint32_t rx_osc_int_avg = dtotal/100;
    uint32_t rx_osc_int_min = dmin;
    uint32_t rx_osc_int_max = dmax;

    uint32_t roaming_bank0_energy=0, roaming_bank1_energy=0;

#if 0
    // 4ln doesn't have these
    //
    CHECK(pmd_read_field(mss, RX_FFE_ADAPT_ROAMING_RDREG5_ADDR,
                              RX_FFE_ADAPT_ROAMING_RDREG5_BANK0_ENERGY_NT_MASK,
                              RX_FFE_ADAPT_ROAMING_RDREG5_BANK0_ENERGY_NT_OFFSET,
                              &roaming_bank0_energy));
    CHECK(pmd_read_field(mss, RX_FFE_ADAPT_ROAMING_RDREG6_ADDR,
                              RX_FFE_ADAPT_ROAMING_RDREG6_BANK1_ENERGY_NT_MASK,
                              RX_FFE_ADAPT_ROAMING_RDREG6_BANK1_ENERGY_NT_OFFSET,
                              &roaming_bank1_energy));
#endif //0

    uint32_t taps[18] = {0};
    uint32_t pre_ffe[18] = {0};
    (void) pre_ffe;
    CHECK(pmd_read_field(mss, RXEQSTORE0_TAP_REG1_ADDR,
                              RXEQSTORE0_TAP_REG1_TAP0_NT_MASK,
                              RXEQSTORE0_TAP_REG1_TAP0_NT_OFFSET,
                              &taps[0]));
    CHECK(pmd_read_field(mss, RXEQSTORE0_TAP_REG1_ADDR,
                              RXEQSTORE0_TAP_REG1_TAP1_NT_MASK,
                              RXEQSTORE0_TAP_REG1_TAP1_NT_OFFSET,
                              &taps[1]));
    CHECK(pmd_read_field(mss, RXEQSTORE0_TAP_REG1_ADDR,
                              RXEQSTORE0_TAP_REG1_TAP2_NT_MASK,
                              RXEQSTORE0_TAP_REG1_TAP2_NT_OFFSET,
                              &taps[2]));
    CHECK(pmd_read_field(mss, RXEQSTORE0_TAP_REG1_ADDR,
                              RXEQSTORE0_TAP_REG1_TAP3_NT_MASK,
                              RXEQSTORE0_TAP_REG1_TAP3_NT_OFFSET,
                              &taps[3]));
    CHECK(pmd_read_field(mss, RXEQSTORE0_TAP_REG1_ADDR,
                              RXEQSTORE0_TAP_REG1_TAP4_NT_MASK,
                              RXEQSTORE0_TAP_REG1_TAP4_NT_OFFSET,
                              &taps[4]));
    CHECK(pmd_read_field(mss, RXEQSTORE0_TAP_REG1_ADDR,
                              RXEQSTORE0_TAP_REG1_TAP5_NT_MASK,
                              RXEQSTORE0_TAP_REG1_TAP5_NT_OFFSET,
                              &taps[5]));
    CHECK(pmd_read_field(mss, RXEQSTORE0_TAP_REG2_ADDR,
                              RXEQSTORE0_TAP_REG2_TAP6_NT_MASK,
                              RXEQSTORE0_TAP_REG2_TAP6_NT_OFFSET,
                              &taps[6]));
    CHECK(pmd_read_field(mss, RXEQSTORE0_TAP_REG2_ADDR,
                              RXEQSTORE0_TAP_REG2_TAP7_NT_MASK,
                              RXEQSTORE0_TAP_REG2_TAP7_NT_OFFSET,
                              &taps[7]));
    CHECK(pmd_read_field(mss, RXEQSTORE0_TAP_REG2_ADDR,
                              RXEQSTORE0_TAP_REG2_TAP8_NT_MASK,
                              RXEQSTORE0_TAP_REG2_TAP8_NT_OFFSET,
                              &taps[8]));
    CHECK(pmd_read_field(mss, RXEQSTORE0_TAP_REG2_ADDR,
                              RXEQSTORE0_TAP_REG2_TAP9_NT_MASK,
                              RXEQSTORE0_TAP_REG2_TAP9_NT_OFFSET,
                              &taps[9]));
    CHECK(pmd_read_field(mss, RXEQSTORE0_TAP_REG3_ADDR,
                              RXEQSTORE0_TAP_REG3_TAP10_NT_MASK,
                              RXEQSTORE0_TAP_REG3_TAP10_NT_OFFSET,
                              &taps[10]));
    CHECK(pmd_read_field(mss, RXEQSTORE0_TAP_REG3_ADDR,
                              RXEQSTORE0_TAP_REG3_TAP11_NT_MASK,
                              RXEQSTORE0_TAP_REG3_TAP11_NT_OFFSET,
                              &taps[11]));
    CHECK(pmd_read_field(mss, RXEQSTORE0_TAP_REG3_ADDR,
                              RXEQSTORE0_TAP_REG3_TAP12_NT_MASK,
                              RXEQSTORE0_TAP_REG3_TAP12_NT_OFFSET,
                              &taps[12]));
    CHECK(pmd_read_field(mss, RXEQSTORE0_TAP_REG4_ADDR,
                              RXEQSTORE0_TAP_REG4_TAP13_NT_MASK,
                              RXEQSTORE0_TAP_REG4_TAP13_NT_OFFSET,
                              &taps[13]));
    CHECK(pmd_read_field(mss, RXEQSTORE0_TAP_REG4_ADDR,
                              RXEQSTORE0_TAP_REG4_TAP14_NT_MASK,
                              RXEQSTORE0_TAP_REG4_TAP14_NT_OFFSET,
                              &taps[14]));
    CHECK(pmd_read_field(mss, RXEQSTORE0_TAP_REG4_ADDR,
                              RXEQSTORE0_TAP_REG4_TAP15_NT_MASK,
                              RXEQSTORE0_TAP_REG4_TAP15_NT_OFFSET,
                              &taps[15]));
    CHECK(pmd_read_field(mss, RXEQSTORE0_TAP_REG5_ADDR,
                              RXEQSTORE0_TAP_REG5_TAP16_NT_MASK,
                              RXEQSTORE0_TAP_REG5_TAP16_NT_OFFSET,
                              &taps[16]));
    CHECK(pmd_read_field(mss, RXEQSTORE0_TAP_REG5_ADDR,
                              RXEQSTORE0_TAP_REG5_TAP17_NT_MASK,
                              RXEQSTORE0_TAP_REG5_TAP17_NT_OFFSET,
                              &taps[17]));
    for (int t=0; t<18; t++) {
      pre_ffe[t] = aw_pmd_4ln_ffe_tap_val(taps[t], t);
    }

    uint32_t pre_dfe_ffe[18] = {0};
    (void)pre_dfe_ffe;
    CHECK(pmd_read_field(mss, RXEQSTORE1_TAP_REG1_ADDR,
                              RXEQSTORE1_TAP_REG1_TAP0_NT_MASK,
                              RXEQSTORE1_TAP_REG1_TAP0_NT_OFFSET,
                              &taps[0]));
    CHECK(pmd_read_field(mss, RXEQSTORE1_TAP_REG1_ADDR,
                              RXEQSTORE1_TAP_REG1_TAP1_NT_MASK,
                              RXEQSTORE1_TAP_REG1_TAP1_NT_OFFSET,
                              &taps[1]));
    CHECK(pmd_read_field(mss, RXEQSTORE1_TAP_REG1_ADDR,
                              RXEQSTORE1_TAP_REG1_TAP2_NT_MASK,
                              RXEQSTORE1_TAP_REG1_TAP2_NT_OFFSET,
                              &taps[2]));
    CHECK(pmd_read_field(mss, RXEQSTORE1_TAP_REG1_ADDR,
                              RXEQSTORE1_TAP_REG1_TAP3_NT_MASK,
                              RXEQSTORE1_TAP_REG1_TAP3_NT_OFFSET,
                              &taps[3]));
    CHECK(pmd_read_field(mss, RXEQSTORE1_TAP_REG1_ADDR,
                              RXEQSTORE1_TAP_REG1_TAP4_NT_MASK,
                              RXEQSTORE1_TAP_REG1_TAP4_NT_OFFSET,
                              &taps[4]));
    CHECK(pmd_read_field(mss, RXEQSTORE1_TAP_REG1_ADDR,
                              RXEQSTORE1_TAP_REG1_TAP5_NT_MASK,
                              RXEQSTORE1_TAP_REG1_TAP5_NT_OFFSET,
                              &taps[5]));
    CHECK(pmd_read_field(mss, RXEQSTORE1_TAP_REG2_ADDR,
                              RXEQSTORE1_TAP_REG2_TAP6_NT_MASK,
                              RXEQSTORE1_TAP_REG2_TAP6_NT_OFFSET,
                              &taps[6]));
    CHECK(pmd_read_field(mss, RXEQSTORE1_TAP_REG2_ADDR,
                              RXEQSTORE1_TAP_REG2_TAP7_NT_MASK,
                              RXEQSTORE1_TAP_REG2_TAP7_NT_OFFSET,
                              &taps[7]));
    CHECK(pmd_read_field(mss, RXEQSTORE1_TAP_REG2_ADDR,
                              RXEQSTORE1_TAP_REG2_TAP8_NT_MASK,
                              RXEQSTORE1_TAP_REG2_TAP8_NT_OFFSET,
                              &taps[8]));
    CHECK(pmd_read_field(mss, RXEQSTORE1_TAP_REG2_ADDR,
                              RXEQSTORE1_TAP_REG2_TAP9_NT_MASK,
                              RXEQSTORE1_TAP_REG2_TAP9_NT_OFFSET,
                              &taps[9]));
    CHECK(pmd_read_field(mss, RXEQSTORE1_TAP_REG3_ADDR,
                              RXEQSTORE1_TAP_REG3_TAP10_NT_MASK,
                              RXEQSTORE1_TAP_REG3_TAP10_NT_OFFSET,
                              &taps[10]));
    CHECK(pmd_read_field(mss, RXEQSTORE1_TAP_REG3_ADDR,
                              RXEQSTORE1_TAP_REG3_TAP11_NT_MASK,
                              RXEQSTORE1_TAP_REG3_TAP11_NT_OFFSET,
                              &taps[11]));
    CHECK(pmd_read_field(mss, RXEQSTORE1_TAP_REG3_ADDR,
                              RXEQSTORE1_TAP_REG3_TAP12_NT_MASK,
                              RXEQSTORE1_TAP_REG3_TAP12_NT_OFFSET,
                              &taps[12]));
    CHECK(pmd_read_field(mss, RXEQSTORE1_TAP_REG4_ADDR,
                              RXEQSTORE1_TAP_REG4_TAP13_NT_MASK,
                              RXEQSTORE1_TAP_REG4_TAP13_NT_OFFSET,
                              &taps[13]));
    CHECK(pmd_read_field(mss, RXEQSTORE1_TAP_REG4_ADDR,
                              RXEQSTORE1_TAP_REG4_TAP14_NT_MASK,
                              RXEQSTORE1_TAP_REG4_TAP14_NT_OFFSET,
                              &taps[14]));
    CHECK(pmd_read_field(mss, RXEQSTORE1_TAP_REG4_ADDR,
                              RXEQSTORE1_TAP_REG4_TAP15_NT_MASK,
                              RXEQSTORE1_TAP_REG4_TAP15_NT_OFFSET,
                              &taps[15]));
    CHECK(pmd_read_field(mss, RXEQSTORE1_TAP_REG5_ADDR,
                              RXEQSTORE1_TAP_REG5_TAP16_NT_MASK,
                              RXEQSTORE1_TAP_REG5_TAP16_NT_OFFSET,
                              &taps[16]));
    CHECK(pmd_read_field(mss, RXEQSTORE1_TAP_REG5_ADDR,
                              RXEQSTORE1_TAP_REG5_TAP17_NT_MASK,
                              RXEQSTORE1_TAP_REG5_TAP17_NT_OFFSET,
                              &taps[17]));
    for (int t=0; t<18; t++) {
      pre_dfe_ffe[t] = aw_pmd_4ln_ffe_tap_val(taps[t], t);
    }

    uint32_t pre_ffe_ctle0[18] = {0};
    CHECK(pmd_read_field(mss, RXEQSTORE2_TAP_REG1_ADDR,
                              RXEQSTORE2_TAP_REG1_TAP0_NT_MASK,
                              RXEQSTORE2_TAP_REG1_TAP0_NT_OFFSET,
                              &taps[0]));
    CHECK(pmd_read_field(mss, RXEQSTORE2_TAP_REG1_ADDR,
                              RXEQSTORE2_TAP_REG1_TAP1_NT_MASK,
                              RXEQSTORE2_TAP_REG1_TAP1_NT_OFFSET,
                              &taps[1]));
    CHECK(pmd_read_field(mss, RXEQSTORE2_TAP_REG1_ADDR,
                              RXEQSTORE2_TAP_REG1_TAP2_NT_MASK,
                              RXEQSTORE2_TAP_REG1_TAP2_NT_OFFSET,
                              &taps[2]));
    CHECK(pmd_read_field(mss, RXEQSTORE2_TAP_REG1_ADDR,
                              RXEQSTORE2_TAP_REG1_TAP3_NT_MASK,
                              RXEQSTORE2_TAP_REG1_TAP3_NT_OFFSET,
                              &taps[3]));
    CHECK(pmd_read_field(mss, RXEQSTORE2_TAP_REG1_ADDR,
                              RXEQSTORE2_TAP_REG1_TAP4_NT_MASK,
                              RXEQSTORE2_TAP_REG1_TAP4_NT_OFFSET,
                              &taps[4]));
    CHECK(pmd_read_field(mss, RXEQSTORE2_TAP_REG1_ADDR,
                              RXEQSTORE2_TAP_REG1_TAP5_NT_MASK,
                              RXEQSTORE2_TAP_REG1_TAP5_NT_OFFSET,
                              &taps[5]));
    CHECK(pmd_read_field(mss, RXEQSTORE2_TAP_REG2_ADDR,
                              RXEQSTORE2_TAP_REG2_TAP6_NT_MASK,
                              RXEQSTORE2_TAP_REG2_TAP6_NT_OFFSET,
                              &taps[6]));
    CHECK(pmd_read_field(mss, RXEQSTORE2_TAP_REG2_ADDR,
                              RXEQSTORE2_TAP_REG2_TAP7_NT_MASK,
                              RXEQSTORE2_TAP_REG2_TAP7_NT_OFFSET,
                              &taps[7]));
    CHECK(pmd_read_field(mss, RXEQSTORE2_TAP_REG2_ADDR,
                              RXEQSTORE2_TAP_REG2_TAP8_NT_MASK,
                              RXEQSTORE2_TAP_REG2_TAP8_NT_OFFSET,
                              &taps[8]));
    CHECK(pmd_read_field(mss, RXEQSTORE2_TAP_REG2_ADDR,
                              RXEQSTORE2_TAP_REG2_TAP9_NT_MASK,
                              RXEQSTORE2_TAP_REG2_TAP9_NT_OFFSET,
                              &taps[9]));
    CHECK(pmd_read_field(mss, RXEQSTORE2_TAP_REG3_ADDR,
                              RXEQSTORE2_TAP_REG3_TAP10_NT_MASK,
                              RXEQSTORE2_TAP_REG3_TAP10_NT_OFFSET,
                              &taps[10]));
    CHECK(pmd_read_field(mss, RXEQSTORE2_TAP_REG3_ADDR,
                              RXEQSTORE2_TAP_REG3_TAP11_NT_MASK,
                              RXEQSTORE2_TAP_REG3_TAP11_NT_OFFSET,
                              &taps[11]));
    CHECK(pmd_read_field(mss, RXEQSTORE2_TAP_REG3_ADDR,
                              RXEQSTORE2_TAP_REG3_TAP12_NT_MASK,
                              RXEQSTORE2_TAP_REG3_TAP12_NT_OFFSET,
                              &taps[12]));
    CHECK(pmd_read_field(mss, RXEQSTORE2_TAP_REG4_ADDR,
                              RXEQSTORE2_TAP_REG4_TAP13_NT_MASK,
                              RXEQSTORE2_TAP_REG4_TAP13_NT_OFFSET,
                              &taps[13]));
    CHECK(pmd_read_field(mss, RXEQSTORE2_TAP_REG4_ADDR,
                              RXEQSTORE2_TAP_REG4_TAP14_NT_MASK,
                              RXEQSTORE2_TAP_REG4_TAP14_NT_OFFSET,
                              &taps[14]));
    CHECK(pmd_read_field(mss, RXEQSTORE2_TAP_REG4_ADDR,
                              RXEQSTORE2_TAP_REG4_TAP15_NT_MASK,
                              RXEQSTORE2_TAP_REG4_TAP15_NT_OFFSET,
                              &taps[15]));
    CHECK(pmd_read_field(mss, RXEQSTORE2_TAP_REG5_ADDR,
                              RXEQSTORE2_TAP_REG5_TAP16_NT_MASK,
                              RXEQSTORE2_TAP_REG5_TAP16_NT_OFFSET,
                              &taps[16]));
    CHECK(pmd_read_field(mss, RXEQSTORE2_TAP_REG5_ADDR,
                              RXEQSTORE2_TAP_REG5_TAP17_NT_MASK,
                              RXEQSTORE2_TAP_REG5_TAP17_NT_OFFSET,
                              &taps[17]));
    for (int t=0; t<18; t++) {
      pre_ffe_ctle0[t] = aw_pmd_4ln_ffe_tap_val(taps[t], t);
    }

    uint32_t pre_ffe_ctle1[18] = {0};
    CHECK(pmd_read_field(mss, RXEQSTORE3_TAP_REG1_ADDR,
                              RXEQSTORE3_TAP_REG1_TAP0_NT_MASK,
                              RXEQSTORE3_TAP_REG1_TAP0_NT_OFFSET,
                              &taps[0]));
    CHECK(pmd_read_field(mss, RXEQSTORE3_TAP_REG1_ADDR,
                              RXEQSTORE3_TAP_REG1_TAP1_NT_MASK,
                              RXEQSTORE3_TAP_REG1_TAP1_NT_OFFSET,
                              &taps[1]));
    CHECK(pmd_read_field(mss, RXEQSTORE3_TAP_REG1_ADDR,
                              RXEQSTORE3_TAP_REG1_TAP2_NT_MASK,
                              RXEQSTORE3_TAP_REG1_TAP2_NT_OFFSET,
                              &taps[2]));
    CHECK(pmd_read_field(mss, RXEQSTORE3_TAP_REG1_ADDR,
                              RXEQSTORE3_TAP_REG1_TAP3_NT_MASK,
                              RXEQSTORE3_TAP_REG1_TAP3_NT_OFFSET,
                              &taps[3]));
    CHECK(pmd_read_field(mss, RXEQSTORE3_TAP_REG1_ADDR,
                              RXEQSTORE3_TAP_REG1_TAP4_NT_MASK,
                              RXEQSTORE3_TAP_REG1_TAP4_NT_OFFSET,
                              &taps[4]));
    CHECK(pmd_read_field(mss, RXEQSTORE3_TAP_REG1_ADDR,
                              RXEQSTORE3_TAP_REG1_TAP5_NT_MASK,
                              RXEQSTORE3_TAP_REG1_TAP5_NT_OFFSET,
                              &taps[5]));
    CHECK(pmd_read_field(mss, RXEQSTORE3_TAP_REG2_ADDR,
                              RXEQSTORE3_TAP_REG2_TAP6_NT_MASK,
                              RXEQSTORE3_TAP_REG2_TAP6_NT_OFFSET,
                              &taps[6]));
    CHECK(pmd_read_field(mss, RXEQSTORE3_TAP_REG2_ADDR,
                              RXEQSTORE3_TAP_REG2_TAP7_NT_MASK,
                              RXEQSTORE3_TAP_REG2_TAP7_NT_OFFSET,
                              &taps[7]));
    CHECK(pmd_read_field(mss, RXEQSTORE3_TAP_REG2_ADDR,
                              RXEQSTORE3_TAP_REG2_TAP8_NT_MASK,
                              RXEQSTORE3_TAP_REG2_TAP8_NT_OFFSET,
                              &taps[8]));
    CHECK(pmd_read_field(mss, RXEQSTORE3_TAP_REG2_ADDR,
                              RXEQSTORE3_TAP_REG2_TAP9_NT_MASK,
                              RXEQSTORE3_TAP_REG2_TAP9_NT_OFFSET,
                              &taps[9]));
    CHECK(pmd_read_field(mss, RXEQSTORE3_TAP_REG3_ADDR,
                              RXEQSTORE3_TAP_REG3_TAP10_NT_MASK,
                              RXEQSTORE3_TAP_REG3_TAP10_NT_OFFSET,
                              &taps[10]));
    CHECK(pmd_read_field(mss, RXEQSTORE3_TAP_REG3_ADDR,
                              RXEQSTORE3_TAP_REG3_TAP11_NT_MASK,
                              RXEQSTORE3_TAP_REG3_TAP11_NT_OFFSET,
                              &taps[11]));
    CHECK(pmd_read_field(mss, RXEQSTORE3_TAP_REG3_ADDR,
                              RXEQSTORE3_TAP_REG3_TAP12_NT_MASK,
                              RXEQSTORE3_TAP_REG3_TAP12_NT_OFFSET,
                              &taps[12]));
    CHECK(pmd_read_field(mss, RXEQSTORE3_TAP_REG4_ADDR,
                              RXEQSTORE3_TAP_REG4_TAP13_NT_MASK,
                              RXEQSTORE3_TAP_REG4_TAP13_NT_OFFSET,
                              &taps[13]));
    CHECK(pmd_read_field(mss, RXEQSTORE3_TAP_REG4_ADDR,
                              RXEQSTORE3_TAP_REG4_TAP14_NT_MASK,
                              RXEQSTORE3_TAP_REG4_TAP14_NT_OFFSET,
                              &taps[14]));
    CHECK(pmd_read_field(mss, RXEQSTORE3_TAP_REG4_ADDR,
                              RXEQSTORE3_TAP_REG4_TAP15_NT_MASK,
                              RXEQSTORE3_TAP_REG4_TAP15_NT_OFFSET,
                              &taps[15]));
    CHECK(pmd_read_field(mss, RXEQSTORE3_TAP_REG5_ADDR,
                              RXEQSTORE3_TAP_REG5_TAP16_NT_MASK,
                              RXEQSTORE3_TAP_REG5_TAP16_NT_OFFSET,
                              &taps[16]));
    CHECK(pmd_read_field(mss, RXEQSTORE3_TAP_REG5_ADDR,
                              RXEQSTORE3_TAP_REG5_TAP17_NT_MASK,
                              RXEQSTORE3_TAP_REG5_TAP17_NT_OFFSET,
                              &taps[17]));
    for (int t=0; t<18; t++) {
      pre_ffe_ctle1[t] = aw_pmd_4ln_ffe_tap_val(taps[t], t);
    }

    uint32_t pre_ffe_ctle2[18] = {0};
    CHECK(pmd_read_field(mss, RXEQSTORE4_TAP_REG1_ADDR,
                              RXEQSTORE4_TAP_REG1_TAP0_NT_MASK,
                              RXEQSTORE4_TAP_REG1_TAP0_NT_OFFSET,
                              &taps[0]));
    CHECK(pmd_read_field(mss, RXEQSTORE4_TAP_REG1_ADDR,
                              RXEQSTORE4_TAP_REG1_TAP1_NT_MASK,
                              RXEQSTORE4_TAP_REG1_TAP1_NT_OFFSET,
                              &taps[1]));
    CHECK(pmd_read_field(mss, RXEQSTORE4_TAP_REG1_ADDR,
                              RXEQSTORE4_TAP_REG1_TAP2_NT_MASK,
                              RXEQSTORE4_TAP_REG1_TAP2_NT_OFFSET,
                              &taps[2]));
    CHECK(pmd_read_field(mss, RXEQSTORE4_TAP_REG1_ADDR,
                              RXEQSTORE4_TAP_REG1_TAP3_NT_MASK,
                              RXEQSTORE4_TAP_REG1_TAP3_NT_OFFSET,
                              &taps[3]));
    CHECK(pmd_read_field(mss, RXEQSTORE4_TAP_REG1_ADDR,
                              RXEQSTORE4_TAP_REG1_TAP4_NT_MASK,
                              RXEQSTORE4_TAP_REG1_TAP4_NT_OFFSET,
                              &taps[4]));
    CHECK(pmd_read_field(mss, RXEQSTORE4_TAP_REG1_ADDR,
                              RXEQSTORE4_TAP_REG1_TAP5_NT_MASK,
                              RXEQSTORE4_TAP_REG1_TAP5_NT_OFFSET,
                              &taps[5]));
    CHECK(pmd_read_field(mss, RXEQSTORE4_TAP_REG2_ADDR,
                              RXEQSTORE4_TAP_REG2_TAP6_NT_MASK,
                              RXEQSTORE4_TAP_REG2_TAP6_NT_OFFSET,
                              &taps[6]));
    CHECK(pmd_read_field(mss, RXEQSTORE4_TAP_REG2_ADDR,
                              RXEQSTORE4_TAP_REG2_TAP7_NT_MASK,
                              RXEQSTORE4_TAP_REG2_TAP7_NT_OFFSET,
                              &taps[7]));
    CHECK(pmd_read_field(mss, RXEQSTORE4_TAP_REG2_ADDR,
                              RXEQSTORE4_TAP_REG2_TAP8_NT_MASK,
                              RXEQSTORE4_TAP_REG2_TAP8_NT_OFFSET,
                              &taps[8]));
    CHECK(pmd_read_field(mss, RXEQSTORE4_TAP_REG2_ADDR,
                              RXEQSTORE4_TAP_REG2_TAP9_NT_MASK,
                              RXEQSTORE4_TAP_REG2_TAP9_NT_OFFSET,
                              &taps[9]));
    CHECK(pmd_read_field(mss, RXEQSTORE4_TAP_REG3_ADDR,
                              RXEQSTORE4_TAP_REG3_TAP10_NT_MASK,
                              RXEQSTORE4_TAP_REG3_TAP10_NT_OFFSET,
                              &taps[10]));
    CHECK(pmd_read_field(mss, RXEQSTORE4_TAP_REG3_ADDR,
                              RXEQSTORE4_TAP_REG3_TAP11_NT_MASK,
                              RXEQSTORE4_TAP_REG3_TAP11_NT_OFFSET,
                              &taps[11]));
    CHECK(pmd_read_field(mss, RXEQSTORE4_TAP_REG3_ADDR,
                              RXEQSTORE4_TAP_REG3_TAP12_NT_MASK,
                              RXEQSTORE4_TAP_REG3_TAP12_NT_OFFSET,
                              &taps[12]));
    CHECK(pmd_read_field(mss, RXEQSTORE4_TAP_REG4_ADDR,
                              RXEQSTORE4_TAP_REG4_TAP13_NT_MASK,
                              RXEQSTORE4_TAP_REG4_TAP13_NT_OFFSET,
                              &taps[13]));
    CHECK(pmd_read_field(mss, RXEQSTORE4_TAP_REG4_ADDR,
                              RXEQSTORE4_TAP_REG4_TAP14_NT_MASK,
                              RXEQSTORE4_TAP_REG4_TAP14_NT_OFFSET,
                              &taps[14]));
    CHECK(pmd_read_field(mss, RXEQSTORE4_TAP_REG4_ADDR,
                              RXEQSTORE4_TAP_REG4_TAP15_NT_MASK,
                              RXEQSTORE4_TAP_REG4_TAP15_NT_OFFSET,
                              &taps[15]));
    CHECK(pmd_read_field(mss, RXEQSTORE4_TAP_REG5_ADDR,
                              RXEQSTORE4_TAP_REG5_TAP16_NT_MASK,
                              RXEQSTORE4_TAP_REG5_TAP16_NT_OFFSET,
                              &taps[16]));
    CHECK(pmd_read_field(mss, RXEQSTORE4_TAP_REG5_ADDR,
                              RXEQSTORE4_TAP_REG5_TAP17_NT_MASK,
                              RXEQSTORE4_TAP_REG5_TAP17_NT_OFFSET,
                              &taps[17]));
    for (int t=0; t<18; t++) {
      pre_ffe_ctle2[t] = aw_pmd_4ln_ffe_tap_val(taps[t], t);
    }

    uint32_t pre_ffe_ctle3[18] = {0};
    CHECK(pmd_read_field(mss, RXEQSTORE5_TAP_REG1_ADDR,
                              RXEQSTORE5_TAP_REG1_TAP0_NT_MASK,
                              RXEQSTORE5_TAP_REG1_TAP0_NT_OFFSET,
                              &taps[0]));
    CHECK(pmd_read_field(mss, RXEQSTORE5_TAP_REG1_ADDR,
                              RXEQSTORE5_TAP_REG1_TAP1_NT_MASK,
                              RXEQSTORE5_TAP_REG1_TAP1_NT_OFFSET,
                              &taps[1]));
    CHECK(pmd_read_field(mss, RXEQSTORE5_TAP_REG1_ADDR,
                              RXEQSTORE5_TAP_REG1_TAP2_NT_MASK,
                              RXEQSTORE5_TAP_REG1_TAP2_NT_OFFSET,
                              &taps[2]));
    CHECK(pmd_read_field(mss, RXEQSTORE5_TAP_REG1_ADDR,
                              RXEQSTORE5_TAP_REG1_TAP3_NT_MASK,
                              RXEQSTORE5_TAP_REG1_TAP3_NT_OFFSET,
                              &taps[3]));
    CHECK(pmd_read_field(mss, RXEQSTORE5_TAP_REG1_ADDR,
                              RXEQSTORE5_TAP_REG1_TAP4_NT_MASK,
                              RXEQSTORE5_TAP_REG1_TAP4_NT_OFFSET,
                              &taps[4]));
    CHECK(pmd_read_field(mss, RXEQSTORE5_TAP_REG1_ADDR,
                              RXEQSTORE5_TAP_REG1_TAP5_NT_MASK,
                              RXEQSTORE5_TAP_REG1_TAP5_NT_OFFSET,
                              &taps[5]));
    CHECK(pmd_read_field(mss, RXEQSTORE5_TAP_REG2_ADDR,
                              RXEQSTORE5_TAP_REG2_TAP6_NT_MASK,
                              RXEQSTORE5_TAP_REG2_TAP6_NT_OFFSET,
                              &taps[6]));
    CHECK(pmd_read_field(mss, RXEQSTORE5_TAP_REG2_ADDR,
                              RXEQSTORE5_TAP_REG2_TAP7_NT_MASK,
                              RXEQSTORE5_TAP_REG2_TAP7_NT_OFFSET,
                              &taps[7]));
    CHECK(pmd_read_field(mss, RXEQSTORE5_TAP_REG2_ADDR,
                              RXEQSTORE5_TAP_REG2_TAP8_NT_MASK,
                              RXEQSTORE5_TAP_REG2_TAP8_NT_OFFSET,
                              &taps[8]));
    CHECK(pmd_read_field(mss, RXEQSTORE5_TAP_REG2_ADDR,
                              RXEQSTORE5_TAP_REG2_TAP9_NT_MASK,
                              RXEQSTORE5_TAP_REG2_TAP9_NT_OFFSET,
                              &taps[9]));
    CHECK(pmd_read_field(mss, RXEQSTORE5_TAP_REG3_ADDR,
                              RXEQSTORE5_TAP_REG3_TAP10_NT_MASK,
                              RXEQSTORE5_TAP_REG3_TAP10_NT_OFFSET,
                              &taps[10]));
    CHECK(pmd_read_field(mss, RXEQSTORE5_TAP_REG3_ADDR,
                              RXEQSTORE5_TAP_REG3_TAP11_NT_MASK,
                              RXEQSTORE5_TAP_REG3_TAP11_NT_OFFSET,
                              &taps[11]));
    CHECK(pmd_read_field(mss, RXEQSTORE5_TAP_REG3_ADDR,
                              RXEQSTORE5_TAP_REG3_TAP12_NT_MASK,
                              RXEQSTORE5_TAP_REG3_TAP12_NT_OFFSET,
                              &taps[12]));
    CHECK(pmd_read_field(mss, RXEQSTORE5_TAP_REG4_ADDR,
                              RXEQSTORE5_TAP_REG4_TAP13_NT_MASK,
                              RXEQSTORE5_TAP_REG4_TAP13_NT_OFFSET,
                              &taps[13]));
    CHECK(pmd_read_field(mss, RXEQSTORE5_TAP_REG4_ADDR,
                              RXEQSTORE5_TAP_REG4_TAP14_NT_MASK,
                              RXEQSTORE5_TAP_REG4_TAP14_NT_OFFSET,
                              &taps[14]));
    CHECK(pmd_read_field(mss, RXEQSTORE5_TAP_REG4_ADDR,
                              RXEQSTORE5_TAP_REG4_TAP15_NT_MASK,
                              RXEQSTORE5_TAP_REG4_TAP15_NT_OFFSET,
                              &taps[15]));
    CHECK(pmd_read_field(mss, RXEQSTORE5_TAP_REG5_ADDR,
                              RXEQSTORE5_TAP_REG5_TAP16_NT_MASK,
                              RXEQSTORE5_TAP_REG5_TAP16_NT_OFFSET,
                              &taps[16]));
    CHECK(pmd_read_field(mss, RXEQSTORE5_TAP_REG5_ADDR,
                              RXEQSTORE5_TAP_REG5_TAP17_NT_MASK,
                              RXEQSTORE5_TAP_REG5_TAP17_NT_OFFSET,
                              &taps[17]));
    for (int t=0; t<18; t++) {
      pre_ffe_ctle3[t] = aw_pmd_4ln_ffe_tap_val(taps[t], t);
    }

    uint32_t pre_ffe_ctle4[18] = {0};
    CHECK(pmd_read_field(mss, RXEQSTORE6_TAP_REG1_ADDR,
                              RXEQSTORE6_TAP_REG1_TAP0_NT_MASK,
                              RXEQSTORE6_TAP_REG1_TAP0_NT_OFFSET,
                              &taps[0]));
    CHECK(pmd_read_field(mss, RXEQSTORE6_TAP_REG1_ADDR,
                              RXEQSTORE6_TAP_REG1_TAP1_NT_MASK,
                              RXEQSTORE6_TAP_REG1_TAP1_NT_OFFSET,
                              &taps[1]));
    CHECK(pmd_read_field(mss, RXEQSTORE6_TAP_REG1_ADDR,
                              RXEQSTORE6_TAP_REG1_TAP2_NT_MASK,
                              RXEQSTORE6_TAP_REG1_TAP2_NT_OFFSET,
                              &taps[2]));
    CHECK(pmd_read_field(mss, RXEQSTORE6_TAP_REG1_ADDR,
                              RXEQSTORE6_TAP_REG1_TAP3_NT_MASK,
                              RXEQSTORE6_TAP_REG1_TAP3_NT_OFFSET,
                              &taps[3]));
    CHECK(pmd_read_field(mss, RXEQSTORE6_TAP_REG1_ADDR,
                              RXEQSTORE6_TAP_REG1_TAP4_NT_MASK,
                              RXEQSTORE6_TAP_REG1_TAP4_NT_OFFSET,
                              &taps[4]));
    CHECK(pmd_read_field(mss, RXEQSTORE6_TAP_REG1_ADDR,
                              RXEQSTORE6_TAP_REG1_TAP5_NT_MASK,
                              RXEQSTORE6_TAP_REG1_TAP5_NT_OFFSET,
                              &taps[5]));
    CHECK(pmd_read_field(mss, RXEQSTORE6_TAP_REG2_ADDR,
                              RXEQSTORE6_TAP_REG2_TAP6_NT_MASK,
                              RXEQSTORE6_TAP_REG2_TAP6_NT_OFFSET,
                              &taps[6]));
    CHECK(pmd_read_field(mss, RXEQSTORE6_TAP_REG2_ADDR,
                              RXEQSTORE6_TAP_REG2_TAP7_NT_MASK,
                              RXEQSTORE6_TAP_REG2_TAP7_NT_OFFSET,
                              &taps[7]));
    CHECK(pmd_read_field(mss, RXEQSTORE6_TAP_REG2_ADDR,
                              RXEQSTORE6_TAP_REG2_TAP8_NT_MASK,
                              RXEQSTORE6_TAP_REG2_TAP8_NT_OFFSET,
                              &taps[8]));
    CHECK(pmd_read_field(mss, RXEQSTORE6_TAP_REG2_ADDR,
                              RXEQSTORE6_TAP_REG2_TAP9_NT_MASK,
                              RXEQSTORE6_TAP_REG2_TAP9_NT_OFFSET,
                              &taps[9]));
    CHECK(pmd_read_field(mss, RXEQSTORE6_TAP_REG3_ADDR,
                              RXEQSTORE6_TAP_REG3_TAP10_NT_MASK,
                              RXEQSTORE6_TAP_REG3_TAP10_NT_OFFSET,
                              &taps[10]));
    CHECK(pmd_read_field(mss, RXEQSTORE6_TAP_REG3_ADDR,
                              RXEQSTORE6_TAP_REG3_TAP11_NT_MASK,
                              RXEQSTORE6_TAP_REG3_TAP11_NT_OFFSET,
                              &taps[11]));
    CHECK(pmd_read_field(mss, RXEQSTORE6_TAP_REG3_ADDR,
                              RXEQSTORE6_TAP_REG3_TAP12_NT_MASK,
                              RXEQSTORE6_TAP_REG3_TAP12_NT_OFFSET,
                              &taps[12]));
    CHECK(pmd_read_field(mss, RXEQSTORE6_TAP_REG4_ADDR,
                              RXEQSTORE6_TAP_REG4_TAP13_NT_MASK,
                              RXEQSTORE6_TAP_REG4_TAP13_NT_OFFSET,
                              &taps[13]));
    CHECK(pmd_read_field(mss, RXEQSTORE6_TAP_REG4_ADDR,
                              RXEQSTORE6_TAP_REG4_TAP14_NT_MASK,
                              RXEQSTORE6_TAP_REG4_TAP14_NT_OFFSET,
                              &taps[14]));
    CHECK(pmd_read_field(mss, RXEQSTORE6_TAP_REG4_ADDR,
                              RXEQSTORE6_TAP_REG4_TAP15_NT_MASK,
                              RXEQSTORE6_TAP_REG4_TAP15_NT_OFFSET,
                              &taps[15]));
    CHECK(pmd_read_field(mss, RXEQSTORE6_TAP_REG5_ADDR,
                              RXEQSTORE6_TAP_REG5_TAP16_NT_MASK,
                              RXEQSTORE6_TAP_REG5_TAP16_NT_OFFSET,
                              &taps[16]));
    CHECK(pmd_read_field(mss, RXEQSTORE6_TAP_REG5_ADDR,
                              RXEQSTORE6_TAP_REG5_TAP17_NT_MASK,
                              RXEQSTORE6_TAP_REG5_TAP17_NT_OFFSET,
                              &taps[17]));
    for (int t=0; t<18; t++) {
      pre_ffe_ctle4[t] = aw_pmd_4ln_ffe_tap_val(taps[t], t);
    }

    uint32_t pre_ffe_ctle5[18] = {0};
    CHECK(pmd_read_field(mss, RXEQSTORE7_TAP_REG1_ADDR,
                              RXEQSTORE7_TAP_REG1_TAP0_NT_MASK,
                              RXEQSTORE7_TAP_REG1_TAP0_NT_OFFSET,
                              &taps[0]));
    CHECK(pmd_read_field(mss, RXEQSTORE7_TAP_REG1_ADDR,
                              RXEQSTORE7_TAP_REG1_TAP1_NT_MASK,
                              RXEQSTORE7_TAP_REG1_TAP1_NT_OFFSET,
                              &taps[1]));
    CHECK(pmd_read_field(mss, RXEQSTORE7_TAP_REG1_ADDR,
                              RXEQSTORE7_TAP_REG1_TAP2_NT_MASK,
                              RXEQSTORE7_TAP_REG1_TAP2_NT_OFFSET,
                              &taps[2]));
    CHECK(pmd_read_field(mss, RXEQSTORE7_TAP_REG1_ADDR,
                              RXEQSTORE7_TAP_REG1_TAP3_NT_MASK,
                              RXEQSTORE7_TAP_REG1_TAP3_NT_OFFSET,
                              &taps[3]));
    CHECK(pmd_read_field(mss, RXEQSTORE7_TAP_REG1_ADDR,
                              RXEQSTORE7_TAP_REG1_TAP4_NT_MASK,
                              RXEQSTORE7_TAP_REG1_TAP4_NT_OFFSET,
                              &taps[4]));
    CHECK(pmd_read_field(mss, RXEQSTORE7_TAP_REG1_ADDR,
                              RXEQSTORE7_TAP_REG1_TAP5_NT_MASK,
                              RXEQSTORE7_TAP_REG1_TAP5_NT_OFFSET,
                              &taps[5]));
    CHECK(pmd_read_field(mss, RXEQSTORE7_TAP_REG2_ADDR,
                              RXEQSTORE7_TAP_REG2_TAP6_NT_MASK,
                              RXEQSTORE7_TAP_REG2_TAP6_NT_OFFSET,
                              &taps[6]));
    CHECK(pmd_read_field(mss, RXEQSTORE7_TAP_REG2_ADDR,
                              RXEQSTORE7_TAP_REG2_TAP7_NT_MASK,
                              RXEQSTORE7_TAP_REG2_TAP7_NT_OFFSET,
                              &taps[7]));
    CHECK(pmd_read_field(mss, RXEQSTORE7_TAP_REG2_ADDR,
                              RXEQSTORE7_TAP_REG2_TAP8_NT_MASK,
                              RXEQSTORE7_TAP_REG2_TAP8_NT_OFFSET,
                              &taps[8]));
    CHECK(pmd_read_field(mss, RXEQSTORE7_TAP_REG2_ADDR,
                              RXEQSTORE7_TAP_REG2_TAP9_NT_MASK,
                              RXEQSTORE7_TAP_REG2_TAP9_NT_OFFSET,
                              &taps[9]));
    CHECK(pmd_read_field(mss, RXEQSTORE7_TAP_REG3_ADDR,
                              RXEQSTORE7_TAP_REG3_TAP10_NT_MASK,
                              RXEQSTORE7_TAP_REG3_TAP10_NT_OFFSET,
                              &taps[10]));
    CHECK(pmd_read_field(mss, RXEQSTORE7_TAP_REG3_ADDR,
                              RXEQSTORE7_TAP_REG3_TAP11_NT_MASK,
                              RXEQSTORE7_TAP_REG3_TAP11_NT_OFFSET,
                              &taps[11]));
    CHECK(pmd_read_field(mss, RXEQSTORE7_TAP_REG3_ADDR,
                              RXEQSTORE7_TAP_REG3_TAP12_NT_MASK,
                              RXEQSTORE7_TAP_REG3_TAP12_NT_OFFSET,
                              &taps[12]));
    CHECK(pmd_read_field(mss, RXEQSTORE7_TAP_REG4_ADDR,
                              RXEQSTORE7_TAP_REG4_TAP13_NT_MASK,
                              RXEQSTORE7_TAP_REG4_TAP13_NT_OFFSET,
                              &taps[13]));
    CHECK(pmd_read_field(mss, RXEQSTORE7_TAP_REG4_ADDR,
                              RXEQSTORE7_TAP_REG4_TAP14_NT_MASK,
                              RXEQSTORE7_TAP_REG4_TAP14_NT_OFFSET,
                              &taps[14]));
    CHECK(pmd_read_field(mss, RXEQSTORE7_TAP_REG4_ADDR,
                              RXEQSTORE7_TAP_REG4_TAP15_NT_MASK,
                              RXEQSTORE7_TAP_REG4_TAP15_NT_OFFSET,
                              &taps[15]));
    CHECK(pmd_read_field(mss, RXEQSTORE7_TAP_REG5_ADDR,
                              RXEQSTORE7_TAP_REG5_TAP16_NT_MASK,
                              RXEQSTORE7_TAP_REG5_TAP16_NT_OFFSET,
                              &taps[16]));
    CHECK(pmd_read_field(mss, RXEQSTORE7_TAP_REG5_ADDR,
                              RXEQSTORE7_TAP_REG5_TAP17_NT_MASK,
                              RXEQSTORE7_TAP_REG5_TAP17_NT_OFFSET,
                              &taps[17]));
    for (int t=0; t<18; t++) {
      pre_ffe_ctle5[t] = aw_pmd_4ln_ffe_tap_val(taps[t], t);
    }

    uint32_t eqbk_counter;
    aw_pmd_4ln_vfld_eqbk_counter_get(mss, &eqbk_counter);

    // dump data line to csv file
    // USR_PRINTF("fw_nsw,");
    USR_PRINTF("1,");
    // USR_PRINTF("fw_nsw_eq,");
    USR_PRINTF("TRUE,");   // ?
    // USR_PRINTF("isi,");
    USR_PRINTF("14,");     // ?
    // USR_PRINTF("port,");
    USR_PRINTF("1,");     // We dont have fp or dev_port here
    // USR_PRINTF("lane,");
    USR_PRINTF("1,");     // We dont have fp or dev_port here
    // USR_PRINTF("dev_port,");
    USR_PRINTF("1,");     // We dont have fp or dev_port here
    // USR_PRINTF("spec,");
    USR_PRINTF("eth_raptors_por,");
    // USR_PRINTF("cmn_pstate,");
    USR_PRINTF("%d,", cmn_pstate);
    // USR_PRINTF("tx_pstate,");
    USR_PRINTF("%d,", tx_pstate);
    // USR_PRINTF("rx_pstate,");
    USR_PRINTF("%d,", rx_pstate);
    // USR_PRINTF("tx_rate,");
    USR_PRINTF("%d,", tx_rate);
    // USR_PRINTF("rx_rate,");
    USR_PRINTF("%d,", rx_rate);
    // USR_PRINTF("tx_width,");
    USR_PRINTF("%d,", tx_width);
    // USR_PRINTF("rx_width,");
    USR_PRINTF("%d,", rx_width);
    // USR_PRINTF("perf_mode,");
    USR_PRINTF("0,");
    // USR_PRINTF("first_ber,");
    USR_PRINTF("0,");
    // USR_PRINTF("first_bist_locked,");
    USR_PRINTF("0,");
    // USR_PRINTF("ber);"
    USR_PRINTF("0,");           // dont have a BER
    // USR_PRINTF("ber_snr,");
    USR_PRINTF("0,");           // way too much math
    // USR_PRINTF("ber_bathtub,");
    USR_PRINTF("0,");
    // USR_PRINTF("fom_peaks,");
    USR_PRINTF("0,");
    // USR_PRINTF("fom_width_upp,");
    USR_PRINTF("0,");
    // USR_PRINTF("fom_width_low);"
    USR_PRINTF("0,");
    // USR_PRINTF("fom_width_avg,");
    USR_PRINTF("0,");
    // USR_PRINTF("fom_ratio_p_w,");
    USR_PRINTF("0,");
    // USR_PRINTF("fom_FW,");
    USR_PRINTF("0,");
    // USR_PRINTF("fom_lms,");
    USR_PRINTF("0,");
    // USR_PRINTF("fom_watchdog,");
    USR_PRINTF("0,");
    // USR_PRINTF("mlsd_ber,");
    USR_PRINTF("0,");
    // USR_PRINTF("mlsd_snr,");
    USR_PRINTF("0,");
    // USR_PRINTF("snr_gain,");
    USR_PRINTF("0,");
    // USR_PRINTF("lt_state,");
    USR_PRINTF("%d,", lt_state);
    // USR_PRINTF("msb,");
    USR_PRINTF("%d,", msb);
    // USR_PRINTF("lsb,");
    USR_PRINTF("%d,", lsb);
    // USR_PRINTF("pre3,");
    USR_PRINTF("%d,",  txfir_cfg.CM3);
    // USR_PRINTF("pre2,");
    USR_PRINTF("%d,",  txfir_cfg.CM2);
    // USR_PRINTF("pre1,");
    USR_PRINTF("%d,",  txfir_cfg.CM1);
    // USR_PRINTF("c0,");
    USR_PRINTF("%d,",  txfir_cfg.C0);
    // USR_PRINTF("post1,");
    USR_PRINTF("%d,",  txfir_cfg.C1);
    // USR_PRINTF("vga_offset,");
    USR_PRINTF("%d,",  rx_afe_data_.vga_offset);
    // USR_PRINTF("ctle_rate,");
    USR_PRINTF("%d,",  rx_afe_data_.ctle_rate);
    // USR_PRINTF("acc_boost,");
    USR_PRINTF("%d,",  acc_boost);
    // USR_PRINTF("ctle_boost,");
    USR_PRINTF("%d,",  rx_afe_data_.ctle_boost);
    // USR_PRINTF("vga_coarse,");
    USR_PRINTF("%d,",  rx_afe_data_.vga_coarse);
    // USR_PRINTF("vga_fine,");
    USR_PRINTF("%d,",  rx_afe_data_.vga_fine);
    // USR_PRINTF("vga_cap,");
    USR_PRINTF("%d,",  vga_cap.vga_cap);
    // USR_PRINTF("vga_cap1,");
    USR_PRINTF("%d,",  vga_cap_1);
    // USR_PRINTF("vga_cap2,");
    USR_PRINTF("%d,",  vga_cap_2);
    // USR_PRINTF("ctle_boost1,");
    USR_PRINTF("%d,",  0); // missing API, rx_vga_boost_lut_get
    // USR_PRINTF("ctle_boost2,");
    USR_PRINTF("%d,",  0); // missing API, rx_vga_boost_lut_get
    // USR_PRINTF("vga1,");
    USR_PRINTF("%d,",  0); // missing API, rx_vga_boost_lut_get
    // USR_PRINTF("vga2,");
    USR_PRINTF("%d,",  0); // missing API, rx_vga_boost_lut_get
    // USR_PRINTF("kprop,");
    USR_PRINTF("%d,",  rx_kprop1);
    // USR_PRINTF("kint,");
    USR_PRINTF("%d,",  rx_kint1);
    // USR_PRINTF("meas_temp,");
    USR_PRINTF("%d,",  0);
    // USR_PRINTF("cmn_osc,");
    USR_PRINTF("%d,",  cmn_osc);
    // USR_PRINTF("cmn_osc_int_avg,");
    USR_PRINTF("%d,",  cmn_osc_int_avg);
    // USR_PRINTF("cmn_osc_int_min,");
    USR_PRINTF("%d,",  cmn_osc_int_min);
    // USR_PRINTF("cmn_osc_int_max,");
    USR_PRINTF("%d,",  cmn_osc_int_max);
    // USR_PRINTF("tx_osc,");
    USR_PRINTF("%d,",  tx_osc);
    // USR_PRINTF("tx_osc_int_avg,");
    USR_PRINTF("%d,",  tx_osc_int_avg);
    // USR_PRINTF("tx_osc_int_min,");
    USR_PRINTF("%d,",  tx_osc_int_min);
    // USR_PRINTF("tx_osc_int_max,");
    USR_PRINTF("%d,",  tx_osc_int_max);
    // USR_PRINTF("rx_osc,");
    USR_PRINTF("%d,",  rx_osc);
    // USR_PRINTF("rx_osc_int_avg,");
    USR_PRINTF("%d,",  rx_osc_int_avg);
    // USR_PRINTF("rx_osc_int_min,");
    USR_PRINTF("%d,",  rx_osc_int_min);
    // USR_PRINTF("rx_osc_int_max,");
    USR_PRINTF("%d,",  rx_osc_int_max);
    // USR_PRINTF("cmn_pstate,");
    USR_PRINTF("%d,", cmn_pstate);
    // USR_PRINTF("tx_pstate,");
    USR_PRINTF("%d,", tx_pstate);
    // USR_PRINTF("rx_pstate,");
    USR_PRINTF("%d,", rx_pstate);
    // USR_PRINTF("cdr_locked,");
    USR_PRINTF("%d,", cdr_lock);
    // USR_PRINTF("cdr_offset,");
    USR_PRINTF("%d,", cdr_offset);
    // USR_PRINTF("cdr_dir,");
    USR_PRINTF("%d,", cdr_dir);
    // USR_PRINTF("rx_bist_locked,");
    USR_PRINTF("%d,", bist_state);
    // USR_PRINTF("tx_bist_pattern,");
    USR_PRINTF("%s,", tx_bist_pattern);
    // USR_PRINTF("rx_bist_pattern,");
    USR_PRINTF("%s,", rx_bist_pattern);
    // USR_PRINTF("bist_mode,");
    USR_PRINTF("%d,", bist_mode);
    // USR_PRINTF("tx_dcd_d0,");
    USR_PRINTF("%d,", tx_dcdiq_data_.d0);
    // USR_PRINTF("tx_dcd_d90,");
    USR_PRINTF("%d,", tx_dcdiq_data_.d90);
    // USR_PRINTF("tx_iq,");
    USR_PRINTF("%d,", tx_dcdiq_data_.iq);
    // USR_PRINTF("rx_dcd_d0,");
    USR_PRINTF("%d,", rx_dcdiq_data_.d0);
    // USR_PRINTF("rx_dcd_d90,");
    USR_PRINTF("%d,", rx_dcdiq_data_.d90);
    // USR_PRINTF("rx_iq,");
    USR_PRINTF("%d,", rx_dcdiq_data_.iq);
    // USR_PRINTF("roaming_mode,");
    USR_PRINTF("%d,", roaming_mode);
    // USR_PRINTF("roaming_window_sel1,");
    USR_PRINTF("%d,", roaming_window_sel_1);
    // USR_PRINTF("roaming_window_sel2,");
    USR_PRINTF("%d,", roaming_window_sel_2);
    // USR_PRINTF("roaming_bank0_energy,");
    USR_PRINTF("%d,", roaming_bank0_energy);
    // USR_PRINTF("roaming_bank1_energy,");
    USR_PRINTF("%d,", roaming_bank1_energy);
    // USR_PRINTF("ffe_pulse_response,");
    USR_PRINTF("%d,", -1);
    // USR_PRINTF("eqbk_counter,");
    USR_PRINTF("%d,", eqbk_counter);

    // if dumping all branches, put branch 0 values together with csv_line, and branch 1-63 values in seperate lines
    // record the start index of adapt section
    // adapt_start_index = len(csv_line);

    float pre1_ratio = (float)dsp_info_[branch].pre_ffe[14]/(float)dsp_info_[branch].pre_ffe[13];
    float post1_ratio = (float)dsp_info_[branch].pre_ffe[12]/(float)dsp_info_[branch].pre_ffe[13];

    // USR_PRINTF("branch,");
    USR_PRINTF("%d,", branch);

    if (branch == 0) {
        // USR_PRINTF("dcoffset,");
        USR_PRINTF("%d,", dsp_info_[0].dc_offset);
        // USR_PRINTF("dfe_coeff,");
        USR_PRINTF("%d,", dsp_info_[0].dfe);
        // USR_PRINTF("pre1_ratio,");
        USR_PRINTF("%f,", pre1_ratio);
        // USR_PRINTF("post1_ratio,");
        USR_PRINTF("%f,", post1_ratio);
        // USR_PRINTF("pre_ffe,");
            USR_PRINTF("\"[");
            for(int j=0; j<AW_FFE_NUM_TAPS; j++) {
                USR_PRINTF("%d",  dsp_info_[0].pre_ffe[j]);
                if (j!=(AW_FFE_NUM_TAPS-1)) {
                    USR_PRINTF(",");
                }
            }
            USR_PRINTF("]\",");
        // USR_PRINTF("pre_dfe_ffe,");
            USR_PRINTF("\"[");
            for(int j=0; j<AW_FFE_NUM_TAPS; j++) {
                USR_PRINTF("%d",  dsp_info_[0].pre_dfe_ffe[j]);
                if(j!=(AW_FFE_NUM_TAPS-1)) {
                    USR_PRINTF(",");
                }
            }
            USR_PRINTF("]\",");
        // USR_PRINTF("pre_ffe_ctle0,");
            USR_PRINTF("\"[");
            for(int j=0; j<AW_FFE_NUM_TAPS; j++) {
                USR_PRINTF("%d",  pre_ffe_ctle0[j]);
                if (j!=(AW_FFE_NUM_TAPS-1)) {
                    USR_PRINTF(",");
                }
            }
            USR_PRINTF("]\",");
        // USR_PRINTF("pre_ffe_ctle1,");
            USR_PRINTF("\"[");
            for(int j=0; j<AW_FFE_NUM_TAPS; j++) {
                USR_PRINTF("%d",  pre_ffe_ctle1[j]);
                if (j!=(AW_FFE_NUM_TAPS-1)) {
                    USR_PRINTF(",");
                }
            }
            USR_PRINTF("]\",");
        // USR_PRINTF("pre_ffe_ctle2,");
            USR_PRINTF("\"[");
            for(int j=0; j<AW_FFE_NUM_TAPS; j++) {
                USR_PRINTF("%d",  pre_ffe_ctle2[j]);
                if (j!=(AW_FFE_NUM_TAPS-1)) {
                    USR_PRINTF(",");
                }
            }
            USR_PRINTF("]\",");
        // USR_PRINTF("pre_ffe_ctle3,");
            USR_PRINTF("\"[");
            for(int j=0; j<AW_FFE_NUM_TAPS; j++) {
                USR_PRINTF("%d",  pre_ffe_ctle3[j]);
                if (j!=(AW_FFE_NUM_TAPS-1)) {
                    USR_PRINTF(",");
                }
            }
            USR_PRINTF("]\",");
        // USR_PRINTF("pre_ffe_ctle4,");
            USR_PRINTF("\"[");
            for(int j=0; j<AW_FFE_NUM_TAPS; j++) {
                USR_PRINTF("%d",  pre_ffe_ctle4[j]);
                if (j!=(AW_FFE_NUM_TAPS-1)) {
                    USR_PRINTF(",");
                }
            }
            USR_PRINTF("]\",");
        // USR_PRINTF("pre_ffe_ctle5,");
            USR_PRINTF("\"[");
            for(int j=0; j<AW_FFE_NUM_TAPS; j++) {
                USR_PRINTF("%d",  pre_ffe_ctle5[j]);
                if (j!=(AW_FFE_NUM_TAPS-1)) {
                    USR_PRINTF(",");
                }
            }
            USR_PRINTF("]\",");
        // USR_PRINTF("ffe_coeff,");
            USR_PRINTF("\"[");
            for(int j=0; j<AW_FFE_NUM_TAPS; j++) {
                USR_PRINTF("%d",dsp_info_[0].ffe[j]);
                USR_PRINTF("%d",  dsp_info_[0].ffe[j]);
                if(j!=(AW_FFE_NUM_TAPS-1)) {
                    USR_PRINTF(",");
                }
            }
            USR_PRINTF("]\",");
            //aw_pmd_4ln_arbwidth_val(uint32_t val, uint32_t width)
        // USR_PRINTF("slicer_threshold_eh,");
            USR_PRINTF("%d, ", aw_pmd_4ln_arbwidth_val(dsp_info_[0].thresholds.eh, 9));
        // USR_PRINTF("slicer_threshold_ez,");
            USR_PRINTF("%d, ", aw_pmd_4ln_arbwidth_val(dsp_info_[0].thresholds.ez, 9));
        // USR_PRINTF("slicer_threshold_el,");
            USR_PRINTF("%d, ", aw_pmd_4ln_arbwidth_val(dsp_info_[0].thresholds.el, 9));
        // USR_PRINTF("slicer_target_el3,");
            USR_PRINTF("%d, ", aw_pmd_4ln_arbwidth_val(dsp_info_[0].slicers.el3, 9));
        // USR_PRINTF("slicer_target_el1,");
            USR_PRINTF("%d, ", aw_pmd_4ln_arbwidth_val(dsp_info_[0].slicers.el1, 9));
        // USR_PRINTF("slicer_target_eh1,");
            USR_PRINTF("%d, ", aw_pmd_4ln_arbwidth_val(dsp_info_[0].slicers.eh1, 9));
        // USR_PRINTF("slicer_target_eh3,");
            USR_PRINTF("%d, ", aw_pmd_4ln_arbwidth_val(dsp_info_[0].slicers.eh3, 9));
    } else {
        // USR_PRINTF("dcoffset,");
        USR_PRINTF("%d,", dsp_info_[branch].dc_offset);
        // USR_PRINTF("dfe_coeff,");
        USR_PRINTF("%d,", dsp_info_[branch].dfe);
        // USR_PRINTF("pre1_ratio,");
        USR_PRINTF("%f,", pre1_ratio);
        // USR_PRINTF("post1_ratio,");
        USR_PRINTF("%f,", post1_ratio);
        // USR_PRINTF("pre_ffe,");
            USR_PRINTF("\"[");
            for(int j=0; j<AW_FFE_NUM_TAPS; j++) {
                USR_PRINTF("%d",  aw_pmd_4ln_ffe_tap_val(dsp_info_[branch].pre_ffe[j],j));
                if (j!=(AW_FFE_NUM_TAPS-1)) {
                    USR_PRINTF(",");
                }
            }
            USR_PRINTF("]\",");
        // USR_PRINTF("pre_dfe_ffe,");
            USR_PRINTF("\"[");
            for(int j=0; j<AW_FFE_NUM_TAPS; j++) {
                USR_PRINTF("%d",  aw_pmd_4ln_ffe_tap_val(dsp_info_[branch].pre_dfe_ffe[j],j));
                if(j!=(AW_FFE_NUM_TAPS-1)) {
                    USR_PRINTF(",");
                }
            }
            USR_PRINTF("]\",");
        // USR_PRINTF("pre_ffe_ctle0,");
            USR_PRINTF("\"[");
            for(int j=0; j<AW_FFE_NUM_TAPS; j++) {
                USR_PRINTF("%d",  pre_ffe_ctle0[j]);
                if (j!=(AW_FFE_NUM_TAPS-1)) {
                    USR_PRINTF(",");
                }
            }
            USR_PRINTF("]\",");
        // USR_PRINTF("pre_ffe_ctle1,");
            USR_PRINTF("\"[");
            for(int j=0; j<AW_FFE_NUM_TAPS; j++) {
                USR_PRINTF("%d",  pre_ffe_ctle1[j]);
                if (j!=(AW_FFE_NUM_TAPS-1)) {
                    USR_PRINTF(",");
                }
            }
            USR_PRINTF("]\",");
        // USR_PRINTF("pre_ffe_ctle2,");
            USR_PRINTF("\"[");
            for(int j=0; j<AW_FFE_NUM_TAPS; j++) {
                USR_PRINTF("%d",  pre_ffe_ctle2[j]);
                if (j!=(AW_FFE_NUM_TAPS-1)) {
                    USR_PRINTF(",");
                }
            }
            USR_PRINTF("]\",");
        // USR_PRINTF("pre_ffe_ctle3,");
            USR_PRINTF("\"[");
            for(int j=0; j<AW_FFE_NUM_TAPS; j++) {
                USR_PRINTF("%d",  pre_ffe_ctle3[j]);
                if (j!=(AW_FFE_NUM_TAPS-1)) {
                    USR_PRINTF(",");
                }
            }
            USR_PRINTF("]\",");
        // USR_PRINTF("pre_ffe_ctle4,");
            USR_PRINTF("\"[");
            for(int j=0; j<AW_FFE_NUM_TAPS; j++) {
                USR_PRINTF("%d",  pre_ffe_ctle4[j]);
                if (j!=(AW_FFE_NUM_TAPS-1)) {
                    USR_PRINTF(",");
                }
            }
            USR_PRINTF("]\",");
        // USR_PRINTF("pre_ffe_ctle5,");
            USR_PRINTF("\"[");
            for(int j=0; j<AW_FFE_NUM_TAPS; j++) {
                USR_PRINTF("%d",  pre_ffe_ctle5[j]);
                if (j!=(AW_FFE_NUM_TAPS-1)) {
                    USR_PRINTF(",");
                }
            }
            USR_PRINTF("]\",");
        // USR_PRINTF("ffe_coeff,");
            USR_PRINTF("\"[");
            for(int j=0; j<AW_FFE_NUM_TAPS; j++) {
                USR_PRINTF("%d",dsp_info_[branch].ffe[j]);
                USR_PRINTF("%d",  aw_pmd_4ln_ffe_tap_val(dsp_info_[branch].ffe[j],j));
                if(j!=(AW_FFE_NUM_TAPS-1)) {
                    USR_PRINTF(",");
                }
            }
            USR_PRINTF("]\",");
        // USR_PRINTF("slicer_threshold_eh,");
            USR_PRINTF("%d, ", aw_pmd_4ln_arbwidth_val(dsp_info_[branch].thresholds.eh, 9));
        // USR_PRINTF("slicer_threshold_ez,");
            USR_PRINTF("%d, ", aw_pmd_4ln_arbwidth_val(dsp_info_[branch].thresholds.ez, 9));
        // USR_PRINTF("slicer_threshold_el,");
            USR_PRINTF("%d, ", aw_pmd_4ln_arbwidth_val(dsp_info_[branch].thresholds.el, 9));
        // USR_PRINTF("slicer_target_el3,");
            USR_PRINTF("%d, ", aw_pmd_4ln_arbwidth_val(dsp_info_[branch].slicers.el3, 9));
        // USR_PRINTF("slicer_target_el1,");
            USR_PRINTF("%d, ", aw_pmd_4ln_arbwidth_val(dsp_info_[branch].slicers.el1, 9));
        // USR_PRINTF("slicer_target_eh1,");
            USR_PRINTF("%d, ", aw_pmd_4ln_arbwidth_val(dsp_info_[branch].slicers.eh1, 9));
        // USR_PRINTF("slicer_target_eh3,");
            USR_PRINTF("%d, ", aw_pmd_4ln_arbwidth_val(dsp_info_[branch].slicers.eh3, 9));
    }
    // USR_PRINTF("symbol_size,");
    USR_PRINTF("%d, ", symbol_size);
    // USR_PRINTF("symbol_per_cw,");
    USR_PRINTF("%d, ", symbol_per_cw);
    // USR_PRINTF("burst_bit_threshold,");
    USR_PRINTF("%d, ", burst_bit_threshold);
    // USR_PRINTF("burst_err_found,");
    USR_PRINTF("%d, ", burst_err_found);
    // USR_PRINTF("prefec_hist[0],");
    // USR_PRINTF("prefec_hist[1],");
    // USR_PRINTF("prefec_hist[2],");
    // USR_PRINTF("prefec_hist[3],");
    // USR_PRINTF("prefec_hist[4],");
    // USR_PRINTF("prefec_hist[5],");
    // USR_PRINTF("prefec_hist[6],");
    // USR_PRINTF("prefec_hist[7],");
    // USR_PRINTF("prefec_hist[8],");
    // USR_PRINTF("prefec_hist[9],");
    // USR_PRINTF("prefec_hist[10],");
    // USR_PRINTF("prefec_hist[11],");
    // USR_PRINTF("prefec_hist[12],");
    // USR_PRINTF("prefec_hist[13],");
    // USR_PRINTF("prefec_hist[14],");
    // USR_PRINTF("prefec_hist[15],");
    // USR_PRINTF("prefec_hist[16],");
    // USR_PRINTF("prefec_hist[17],");
    // USR_PRINTF("prefec_hist[18],");
    // USR_PRINTF("prefec_hist[19],");
    // USR_PRINTF("prefec_hist[20],");
    // USR_PRINTF("prefec_hist[21],");
    // USR_PRINTF("prefec_hist[22],");
    // USR_PRINTF("prefec_hist[23],");
    // USR_PRINTF("prefec_hist[24],");
    // USR_PRINTF("prefec_hist[25],");
    // USR_PRINTF("prefec_hist[26],");
    // USR_PRINTF("prefec_hist[27],");
    // USR_PRINTF("prefec_hist[28],");
    // USR_PRINTF("prefec_hist[29],");
    // USR_PRINTF("prefec_hist[30],");
    for(int i=0; i<31; i++) {
        USR_PRINTF("%d, ", prefec_hist[i]);
    }
    USR_PRINTF("\n");
    return 0;












    USR_PRINTF("------------------------------------------\n");
    USR_PRINTF("-----------  STATUS REGISTERS  -----------\n");
    USR_PRINTF("------------------------------------------\n\n");
    USR_PRINTF("txmfsm_rate_cur                  = %d\n",txmfsm_rate_cur);
    USR_PRINTF("rxmfsm_rate_cur                  = %d\n",rxmfsm_rate_cur);
    USR_PRINTF("err_count_done                   = %d\n",err_count_done);
    USR_PRINTF("err_count                        = 0x%16" PRIx64 "\n",err_count);
    USR_PRINTF("err_count_overflow               = %d\n",err_count_overflow);
    if (branch == -1) {
        for (int i=0; i<AW_NUM_BRANCHES; i++) {
            USR_PRINTF("BRANCH %2d: ffe                   = [",i);
            for(int j=0; j<AW_FFE_NUM_TAPS; j++) {
                USR_PRINTF("%d",dsp_info_[i].ffe[j]);
                if(j!=(AW_FFE_NUM_TAPS-1)) {
                    USR_PRINTF(",");
                }
            }
            USR_PRINTF("]\n");
            USR_PRINTF("BRANCH %2d: pre_ffe               = [",i);
            for(int j=0; j<AW_FFE_NUM_TAPS; j++) {
                USR_PRINTF("%d",dsp_info_[i].pre_ffe[j]);
                if(j!=(AW_FFE_NUM_TAPS-1)) {
                    USR_PRINTF(",");
                }
            }
            USR_PRINTF("]\n");
            USR_PRINTF("BRANCH %2d: pre_dfe_ffe               = [",i);
            for(int j=0; j<AW_FFE_NUM_TAPS; j++) {
                USR_PRINTF("%d", dsp_info_[i].pre_dfe_ffe[j]);
                if(j!=(AW_FFE_NUM_TAPS-1)) {
                    USR_PRINTF(",");
                }
            }

            USR_PRINTF("]\n");
            USR_PRINTF("BRANCH %2d: dc_offset             = %d\n",i, dsp_info_[i].dc_offset);
            USR_PRINTF("BRANCH %2d: dfe                   = %d\n",i, dsp_info_[i].dfe);
            USR_PRINTF("BRANCH %2d: thresholds.eh         = %d\n",i, dsp_info_[i].thresholds.eh);
            USR_PRINTF("BRANCH %2d: thresholds.ez         = %d\n",i, dsp_info_[i].thresholds.ez);
            USR_PRINTF("BRANCH %2d: thresholds.el         = %d\n",i, dsp_info_[i].thresholds.el);
            USR_PRINTF("BRANCH %2d: thresholds.thres_low  = %d\n",i, dsp_info_[i].thresholds.thres_low);
            USR_PRINTF("BRANCH %2d: thresholds.thres_high = %d\n",i, dsp_info_[i].thresholds.thres_hi);
            USR_PRINTF("BRANCH %2d: slicers.el3           = %d\n",i, dsp_info_[i].slicers.el3);
            USR_PRINTF("BRANCH %2d: slicers.el1           = %d\n",i, dsp_info_[i].slicers.el1);
            USR_PRINTF("BRANCH %2d: slicers.eh1           = %d\n",i, dsp_info_[i].slicers.eh1);
            USR_PRINTF("BRANCH %2d: slicers.eh3           = %d\n",i, dsp_info_[i].slicers.eh3);
        }
    } else {
        USR_PRINTF("BRANCH %2d: ffe                   = [",branch);
        for(int j=0; j<AW_FFE_NUM_TAPS; j++) {
            USR_PRINTF("%d",dsp_info_[branch].ffe[j]);
            if(j!=(AW_FFE_NUM_TAPS-1)) {
                USR_PRINTF(",");
            }
        }
        USR_PRINTF("]\n");
        USR_PRINTF("BRANCH %2d: pre_ffe               = [",branch);
        for(int j=0; j<AW_FFE_NUM_TAPS; j++) {
            USR_PRINTF("%d",dsp_info_[branch].pre_ffe[j]);
            if(j!=(AW_FFE_NUM_TAPS-1)) {
                USR_PRINTF(",");
            }
        }

        USR_PRINTF("]\n");
        USR_PRINTF("BRANCH %2d: pre_dfe_ffe           = [",branch);
        for(int j=0; j<AW_FFE_NUM_TAPS; j++) {
            USR_PRINTF("%d",dsp_info_[branch].pre_dfe_ffe[j]);
            if(j!=(AW_FFE_NUM_TAPS-1)) {
                USR_PRINTF(",");
            }
        }

        USR_PRINTF("]\n");
        USR_PRINTF("BRANCH %2d: dc_offset             = %d\n", branch, dsp_info_[branch].dc_offset);
        USR_PRINTF("BRANCH %2d: dfe                   = %d\n", branch, dsp_info_[branch].dfe);
        USR_PRINTF("BRANCH %2d: thresholds.eh         = %d\n", branch, dsp_info_[branch].thresholds.eh);
        USR_PRINTF("BRANCH %2d: thresholds.ez         = %d\n", branch, dsp_info_[branch].thresholds.ez);
        USR_PRINTF("BRANCH %2d: thresholds.el         = %d\n", branch, dsp_info_[branch].thresholds.el);
        USR_PRINTF("BRANCH %2d: thresholds.thres_low  = %d\n", branch, dsp_info_[branch].thresholds.thres_low);
        USR_PRINTF("BRANCH %2d: thresholds.thres_high = %d\n", branch, dsp_info_[branch].thresholds.thres_hi);
        USR_PRINTF("BRANCH %2d: slicers.el3           = %d\n", branch, dsp_info_[branch].slicers.el3);
        USR_PRINTF("BRANCH %2d: slicers.el1           = %d\n", branch, dsp_info_[branch].slicers.el1);
        USR_PRINTF("BRANCH %2d: slicers.eh1           = %d\n", branch, dsp_info_[branch].slicers.eh1);
        USR_PRINTF("BRANCH %2d: slicers.eh3           = %d\n", branch, dsp_info_[branch].slicers.eh3);
    }
    USR_PRINTF("cdr_lock                         = %d\n", cdr_lock);
    USR_PRINTF("err_count_overflow               = %d\n", err_count_overflow);
    USR_PRINTF("ctle_rate                        = %d\n", rx_afe_data_.ctle_rate);
    USR_PRINTF("ctle_boost                       = %d\n", rx_afe_data_.ctle_boost);
    USR_PRINTF("vga_coarse                       = %d\n", rx_afe_data_.vga_coarse);
    USR_PRINTF("vga_fine                         = %d\n", rx_afe_data_.vga_fine);
    USR_PRINTF("vga_offset                       = %d\n", rx_afe_data_.vga_offset);
    USR_PRINTF("RX dcdiq.d0                      = %d\n", rx_dcdiq_data_.d0);
    USR_PRINTF("RX dcdiq.d90                     = %d\n", rx_dcdiq_data_.d90);
    USR_PRINTF("RX dcdiq.iq                      = %d\n", rx_dcdiq_data_.iq);
    USR_PRINTF("TX dcdiq.d0                      = %d\n", tx_dcdiq_data_.d0);
    USR_PRINTF("TX dcdiq.d90                     = %d\n", tx_dcdiq_data_.d90);
    USR_PRINTF("TX dcdiq.iq                      = %d\n", tx_dcdiq_data_.iq);
    USR_PRINTF("CMNPLL coarse code               = %d\n", lcpll_coarse_code);
    USR_PRINTF("CMNPLL fine code                 = %d\n", lcpll_fine_code);
    USR_PRINTF("TXPLL coarse code                = %d\n", tx_pll_coarse_code);
    USR_PRINTF("TXPLL fine code                  = %d\n", tx_pll_fine_code);
    USR_PRINTF("TXPLL PPM                        = %f\n"  , tx_pll_ppm);
    USR_PRINTF("TXPLL VCO (Hz)                   = %f\n"  , tx_pll_vco);
    USR_PRINTF("RXPLL coarse code                = %d\n", rx_pll_coarse_code);
    USR_PRINTF("RXPLL fine code                  = %d\n", rx_pll_fine_code);
    USR_PRINTF("RXPLL PPM                        = %f\n"  , rx_pll_ppm);
    USR_PRINTF("RXPLL VCO (Hz)                   = %f\n"  , rx_pll_vco);
    USR_PRINTF("LCPLL Lock                       = %d\n", lcpll_lock);
    USR_PRINTF("LCPLL PPM                        = %f\n"  , lcpll_ppm);
    USR_PRINTF("LCPLL VCO (Hz)                   = %f\n"  , lcpll_vco);
    USR_PRINTF("msb                              = %d\n"  , msb);
    USR_PRINTF("lsb                              = %d\n"  , lsb);
    USR_PRINTF("vga_cap                          = %d\n"  , vga_cap.vga_cap);
    USR_PRINTF("vga_cap_1                        = %d\n"  , vga_cap_1);
    USR_PRINTF("vga_cap_2                        = %d\n"  , vga_cap_2);
    USR_PRINTF("tx_bist_pattern                  = %s\n"    , tx_bist_pattern);
    USR_PRINTF("rx_bist_pattern                  = %s\n"    , rx_bist_pattern);
    USR_PRINTF("cdr_offset                       = %d\n"  , cdr_offset);
    USR_PRINTF("TXFIR Settings                   \n");
    USR_PRINTF("  post1                          = %d\n"  , txfir_cfg.C1);
    USR_PRINTF("  c0                             = %d\n"  , txfir_cfg.C0);
    USR_PRINTF("  pre1                           = %d\n"  , txfir_cfg.CM1);
    USR_PRINTF("  pre2                           = %d\n"  , txfir_cfg.CM2);
    USR_PRINTF("  pre3                           = %d\n"  , txfir_cfg.CM3);
    USR_PRINTF("  max_elements                   = %d\n"  , tx_max_elements);
    USR_PRINTF("cmn_kp                           = %d\n"  , cmn_kp);
    USR_PRINTF("cmn_ki                           = %d\n"  , cmn_ki);
    USR_PRINTF("rx_kprop1                        = %d\n"  , rx_kprop1);
    USR_PRINTF("rx_kint1                         = %d\n"  , rx_kint1);
    USR_PRINTF("rx_kprop2                        = %d\n"  , rx_kprop2);
    USR_PRINTF("rx_kint2                         = %d\n"  , rx_kint2);
    USR_PRINTF("rx_kprop3                        = %d\n"  , rx_kprop3);
    USR_PRINTF("rx_kint3                         = %d\n"  , rx_kint3);
    USR_PRINTF("acc_boost                        = %d\n"  , acc_boost);
    USR_PRINTF("FEC Stats                        \n");
    USR_PRINTF("  symbol_size                    = %d\n"  , symbol_size);
    USR_PRINTF("  symbol_per_cw                  = %d\n"  , symbol_per_cw);
    USR_PRINTF("  ber_from_prefec                = %f\n"  , ber_from_prefec);
    USR_PRINTF("  burst_bit_threshold            = %d\n"  , burst_bit_threshold);
    USR_PRINTF("  burst_err_found                = %d\n"  , burst_err_found);
    for(int i=0; i<31; i++) {
        USR_PRINTF("  prefec_hist[%2d]           = %d\n"  , i,prefec_hist[i]);
    }

    USR_PRINTF("timer_threshold                  =%d\n"   , timer_threshold);
    USR_PRINTF("mode_uint32                      =%d\n"   , mode_uint32);
    USR_PRINTF("width_encoded                    =%d\n"   , width_encoded);
    USR_PRINTF("bist_state                       =%d\n"   , bist_state);
    USR_PRINTF("lt_state                         =%d\n"   , lt_state);
    USR_PRINTF("an_state_arb                     =%d\n"   , an_state_arb);
    USR_PRINTF("lt_done                          =%d\n"   , lt_done);
    USR_PRINTF("link_good                        =%d\n"   , link_good);

    return AW_ERR_CODE_NONE;
}

int aw_pmd_4ln_rx_dig_pwr_det_threshold_get(mss_access_t *mss, uint32_t *adc_valid_thresh_nt, uint32_t *adc_invalid_thresh_nt ) {

    CHECK(pmd_read_field(mss, RX_SIGNAL_DETECT_REG1_ADDR, RX_SIGNAL_DETECT_REG1_ADC_VALID_THRES_NT_MASK, RX_SIGNAL_DETECT_REG1_ADC_VALID_THRES_NT_OFFSET, adc_valid_thresh_nt));
    CHECK(pmd_read_field(mss, RX_SIGNAL_DETECT_REG1_ADDR, RX_SIGNAL_DETECT_REG1_ADC_INVALID_THRES_NT_MASK, RX_SIGNAL_DETECT_REG1_ADC_INVALID_THRES_NT_OFFSET, adc_invalid_thresh_nt));

    return AW_ERR_CODE_NONE;
}

int aw_pmd_4ln_rx_dig_pwr_det_threshold_set(mss_access_t *mss, uint32_t adc_valid_thresh_nt, uint32_t adc_invalid_thresh_nt ) {

    CHECK(pmd_write_field(mss, RX_SIGNAL_DETECT_REG1_ADDR, RX_SIGNAL_DETECT_REG1_ADC_VALID_THRES_NT_MASK, RX_SIGNAL_DETECT_REG1_ADC_VALID_THRES_NT_OFFSET, adc_valid_thresh_nt));
    CHECK(pmd_write_field(mss, RX_SIGNAL_DETECT_REG1_ADDR, RX_SIGNAL_DETECT_REG1_ADC_INVALID_THRES_NT_MASK, RX_SIGNAL_DETECT_REG1_ADC_INVALID_THRES_NT_OFFSET, adc_invalid_thresh_nt));

    return AW_ERR_CODE_NONE;
}


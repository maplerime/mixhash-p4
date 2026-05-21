/*
** ? 2020 Alphawave IP Inc.
*/

#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include "svdpi.h"

#include "aw_alphacore.h"



// APIs Exposed by the top level SV TB
extern void sv_initialize_dut(); // defined by SV in dbe_test_pkg
extern void sv_print(char * str);
extern void sv_error(int pass);
extern void sv_wait_apb_preset_done();
extern void sv_set_loopback_reg_cntrl(int lane, int loopback);
extern void sv_request_state_change(int lane, int power, int rate, int data_width, int iso_mode_ena , int blocking);
extern void sv_check_hs_ref_clk_and_rx_blk_clk(int lane, int timeout_us, int clock_cycles);
extern void sv_set_serial_device_bit_rate(int lane, char * rate);
extern void sv_enable_lane_ext_tx_prbs_gen(int lane);
extern void sv_request_rx_link_eval(int lane, int eval_type);
extern void sv_enable_lane_ext_rx_prbs_chk(int lane);
extern void sv_get_lane_ext_rx_prbs_chk_sts(int lane);
extern void sv_run_ext_loopback_data_and_checker(int lane);
extern void sv_print_clks(int lane);
extern void sv_check_clks(int lane, int rate, int width);
extern void sv_force_rx_present(int value);

// This struct is equivalent to the one defined
// in the SV world to gather all test args and
// pass it to the C world. Make sure this is always
// updated when new args are added to the SV counterpart.
struct c_args {
    int         blocking;
    int         run_lane_eq;
    int         isolation_mode;
    int         loopback;
    int         power;
    char        rate_arr[16];
    char        ate_vec_filename[64];
    char        hexfile[256];
    int         m_rnd_lane;
    int         rate_preset;
    int         data_width;
    int         refsel;
    int         fast_sim;
    int         broadcast_en;
    int         lt_en;
    int         vsr_en;
};

/*
 Static global test config parameters. Make sure to override these
 values with values from the UVM test by calling c_set_c_api_test_args
 from the UVM test.
*/
static int      s_blocking             = 1;
static int      s_run_lane_eq          = 0;
static int      s_isolation_mode       = 0;
static int      s_loopback             = 0;
static int      s_power                = 0;
static int      s_m_rnd_lane           = 0;
static int      s_rate_preset          = 0;
static int      s_data_width           = 0;
static int      s_refsel               = 0;
static int      s_fast_sim             = 0;
static int      s_broadcast_en         = 0;
static int      s_lt_en                = 0;
static int      s_vsr_en               = 0;
static char     s_rate_arr[16];
static char     s_hexfile[256];


//Custom static variables (eventually in config file)
static aw_bist_pattern_t     s_bist_pattern = AW_PRBS31;
static uint64_t              s_udp_pattern  = 0x3333333333333333;
static aw_bist_mode_t        s_bist_mode = AW_TIMER;
static aw_txfir_config_t     s_txfir_config = { .CM3=0, .CM2=0, .CM1=0, .C0=60, .C1=0, .main_or_max=1} ;
static aw_analog_loopback_txfir_config_t s_nes_txfir_cfg = { .nes_post1 =5, .nes_c0=15 } ;
static aw_eq_type_t          s_eq_type = AW_EQ_FULL_DIR;
static int                   s_pam_en = 0;

//Default timeouts (slow sim)
static int CMN_ACK_TIMEOUT_US          = 2000;
static int TX_ACK_TIMEOUT_US           = 1200;
static int TX_ACK_P1_TIMEOUT_US        = 1100;
static int TX_ACK_P2_TIMEOUT_US        = 900;
static int RX_ACK_TIMEOUT_US           = 900;
static int RX_ACK_P2_TIMEOUT_US        = 100;
static int RX_CDR_TIMEOUT_US           = 150;
static int RX_BIST_TIMEOUT_US          = 200;
static int RX_LINKEVAL_FULL_TIMEOUT_US = 4200;
static int RX_DATABIST_TIMER_THRESH    = 15000;
static int TX_RXDET_TIMEOUT_US         = 550;

int c_set_c_api_test_args(struct c_args* c_api_test_args) {

    s_blocking           = c_api_test_args->blocking;
    s_run_lane_eq        = c_api_test_args->run_lane_eq;
    s_isolation_mode     = c_api_test_args->isolation_mode;
    s_loopback           = c_api_test_args->loopback;
    s_power              = c_api_test_args->power;
    s_m_rnd_lane         = c_api_test_args->m_rnd_lane;
    s_rate_preset        = c_api_test_args->rate_preset;
    s_data_width         = c_api_test_args->data_width;

    for (uint32_t i = 0; i < sizeof(c_api_test_args->rate_arr); i++) {
        s_rate_arr[i]    = c_api_test_args->rate_arr[i];
    }
    for (uint32_t i = 0; i < sizeof(c_api_test_args->hexfile); i++) {
        s_hexfile[i]     = c_api_test_args->hexfile[i];
    }
    s_refsel             = c_api_test_args->refsel;
    s_fast_sim           = c_api_test_args->fast_sim;
    if (strstr(s_rate_arr,"PAM")) {
        s_pam_en = 1;
    }
    s_broadcast_en       = c_api_test_args->broadcast_en;
    s_lt_en              = c_api_test_args->lt_en;
    return 0;
}

/*
* Example function for gathering ADC data and dumping to CSV
*/
int aw_dump_adc_data(mss_access_t *mss, int num_samples)
{
    FILE* fptr = fopen("samples_adc.csv","w");
    if(fptr == NULL)
    {
        printf("Error!\n");
        return 1;
    }

    int adc_data[num_samples][AW_NUM_BRANCHES];
    aw_pmd_read_tracebuffer_adc(mss, num_samples, adc_data);

    char cwd[1000];
    if (getcwd(cwd, sizeof(cwd)) != NULL){
        printf("Current working dir: %s\n", cwd);
        fflush(stdout);
    }
    for (int j=0;j<AW_NUM_BRANCHES;j++) {
        // print samples in CSV format
        fprintf(fptr, "adc_br%d", j);
        if (j!=AW_NUM_BRANCHES-1){
            fprintf(fptr, ",");
        }
    }
    fprintf(fptr, "\n");
    for (int i=0;i<num_samples;i++) {
        for (int j=0;j<AW_NUM_BRANCHES;j++) {
            // print samples in CSV format
            fprintf(fptr, "%d", adc_data[i][j]);
            if (j!=AW_NUM_BRANCHES-1){
                fprintf(fptr, ",");
            }
        } fprintf(fptr, "\n");
    }
    fclose(fptr);
    return 0;
}

/*
* Example function for gathering FFE data and dumping to CSV
*/
int aw_dump_ffe_data(mss_access_t *mss, int num_samples)
{
    FILE* fptr = fopen("samples_ffe.csv","w");
    if(fptr == NULL)
    {
        printf("Error!\n");
        return 1;
    }

    printf("running aw_dump_ffe_data...\n");
    int num_branches_to_collect = 32; // collect all branches 64/2
    int all_ffe_data[num_branches_to_collect][2][num_samples];
    // collect data for all branches
    for (int branch_id=0;branch_id<num_branches_to_collect;branch_id++){
        aw_pmd_read_tracebuffer_ffe(mss, num_samples, all_ffe_data[branch_id], branch_id);
    }

    // print csv header
    int br_num = 0;
    for (int i=0; i<num_branches_to_collect; i++){
        for (int j=0; j<2; j++){
            fprintf(fptr, "ffe_br%d", br_num);
            br_num++;
            if (br_num!=64){
                fprintf(fptr, ",");
            }
        }
    }
    fprintf(fptr, "\n");
    for (int k=0;k<num_samples;k++){
        br_num = 0;
        for (int i=0; i<num_branches_to_collect; i++){
            for (int j=0; j<2; j++){
                fprintf(fptr, "%d", all_ffe_data[i][j][k]);
                br_num++;
                if (br_num!=num_branches_to_collect*2){
                    fprintf(fptr, ",");
                }
            }
        }
        fprintf(fptr, "\n");
    }
    fclose(fptr);
    return 0;
}

/*
* Example function for gathering DLPF Int codes and dumping to CSV
*/
int aw_dump_itr_dlpf_int_data(mss_access_t *mss, int num_samples)
{
    FILE* fptr = fopen("samples_itr_dlpf_int.csv","w");
    if(fptr == NULL)
    {
        printf("Error!\n");
        return 1;
    }

    printf("running aw_dump_itr_dlpf_data...\n");
    int itr_dlpf_int[num_samples];
    aw_pmd_read_tracebuffer_itr_dlpf_int(mss, num_samples, itr_dlpf_int);

    // print csv header
    fprintf(fptr, "itr_dlpf_int\n");
    for (int k=0;k<num_samples;k++){
        fprintf(fptr, "%d\n", itr_dlpf_int[k]);
    }
    fclose(fptr);
    return 0;
}

int c_initialize_dut() {
    svSetScope(svGetScopeFromName("dbe_test_pkg"));
    sv_initialize_dut();
    return 0;
}

int c_api_ate_load_timeout_overrides() {
    int pass = 0;

    if (s_fast_sim){
        CMN_ACK_TIMEOUT_US     = 10;
        TX_ACK_TIMEOUT_US      = 50;
        TX_ACK_P2_TIMEOUT_US   = 50;
        TX_ACK_P1_TIMEOUT_US   = 50;
        RX_ACK_TIMEOUT_US      = 50;
        RX_ACK_P2_TIMEOUT_US   = 50;
        RX_CDR_TIMEOUT_US      = 10;
        TX_RXDET_TIMEOUT_US    = 50;
        RX_LINKEVAL_FULL_TIMEOUT_US = 10;
    } else {
        if (!s_pam_en){

            if      (strcmp(s_rate_arr,"PCIE_GEN1"   ) == 0) { RX_LINKEVAL_FULL_TIMEOUT_US = 800; }
            else if (strcmp(s_rate_arr,"PCIE_GEN2"   ) == 0) { RX_LINKEVAL_FULL_TIMEOUT_US = 800; }
            else if (strcmp(s_rate_arr,"PCIE_GEN3"   ) == 0) { RX_LINKEVAL_FULL_TIMEOUT_US = 1850;  } // 1169 in SIM mode
            else if (strcmp(s_rate_arr,"PCIE_GEN4"   ) == 0) { RX_LINKEVAL_FULL_TIMEOUT_US = 2050;  } // 1290 in SIM mode
            else if (strcmp(s_rate_arr,"PCIE_GEN5"   ) == 0) { RX_LINKEVAL_FULL_TIMEOUT_US = 1500;  } // 900 in SIM mode
            else {
                RX_LINKEVAL_FULL_TIMEOUT_US = 800;
            }
        }
        if (s_run_lane_eq) {
            RX_CDR_TIMEOUT_US = 5;
        }
        //if (strstr(s_rate_arr,"PCIe")) {
        //    CMN_ACK_TIMEOUT_US     = 1000;
        //}
    }

    //Note: Rate names may be different across projects
    if      (strcmp(s_rate_arr,"PCIE_GEN1"   ) == 0) { RX_BIST_TIMEOUT_US = 250; }
    else if (strcmp(s_rate_arr,"PCIE_GEN2"   ) == 0) { RX_BIST_TIMEOUT_US = 150; }
    else if (strcmp(s_rate_arr,"PCIE_GEN3"   ) == 0) { RX_BIST_TIMEOUT_US = 80;  }
    else if (strcmp(s_rate_arr,"PCIE_GEN4"   ) == 0) { RX_BIST_TIMEOUT_US = 50;  }
    else if (strcmp(s_rate_arr,"PCIE_GEN5"   ) == 0) { RX_BIST_TIMEOUT_US = 50;  }
    else if (strcmp(s_rate_arr,"NRZ_1p25"    ) == 0) { RX_BIST_TIMEOUT_US = 200; }
    else if (strcmp(s_rate_arr,"NRZ_10p3125" ) == 0) { RX_BIST_TIMEOUT_US = 50;  }
    else if (strcmp(s_rate_arr,"NRZ_25p78125") == 0) { RX_BIST_TIMEOUT_US = 150;  }
    else if (strcmp(s_rate_arr,"NRZ_53p125"  ) == 0) { RX_BIST_TIMEOUT_US = 150;  }
    else if (strcmp(s_rate_arr,"PAM4_53p125" ) == 0) { RX_BIST_TIMEOUT_US = 200;  }
    else if (strcmp(s_rate_arr,"PAM4_106p25" ) == 0) { RX_BIST_TIMEOUT_US = 250;  }

    return pass;
}

int c_api_ate_print_config() {
    int pass = 0;

    USR_PRINTF("\n");
    USR_PRINTF("====================================\n");
    USR_PRINTF("======      C_API CONFIG      ======\n");
    USR_PRINTF("====================================\n");
    USR_PRINTF("\n");
    USR_PRINTF("s_blocking             = %d\n", s_blocking      );
    USR_PRINTF("s_run_lane_eq          = %d\n", s_run_lane_eq   );
    USR_PRINTF("s_isolation_mode       = %d\n", s_isolation_mode);
    USR_PRINTF("s_loopback             = %d\n", s_loopback      );
    USR_PRINTF("s_power                = %d\n", s_power         );
    USR_PRINTF("s_rate_arr             = %s\n", s_rate_arr      );
    USR_PRINTF("s_hexfile              = %s\n", s_hexfile       );
    USR_PRINTF("s_m_rnd_lane           = %d\n", s_m_rnd_lane    );
    USR_PRINTF("s_rate_preset          = %d\n", s_rate_preset   );
    USR_PRINTF("s_data_width           = %d\n", s_data_width    );
    USR_PRINTF("s_refsel               = %d\n", s_refsel        );
    USR_PRINTF("s_pam_en               = %d\n", s_pam_en        );
    USR_PRINTF("s_fast_sim             = %d\n", s_fast_sim      );
    USR_PRINTF("s_broadcast_en         = %d\n", s_broadcast_en  );
    USR_PRINTF("s_lt_en                = %d\n", s_lt_en         );
    USR_PRINTF("s_vsr_en               = %d\n", s_vsr_en        );
    USR_PRINTF("s_bist_pattern         = %d\n", s_bist_pattern  );
    // USR_PRINTF("s_udp_pattern          = 0x%X\n", s_udp_pattern );
    USR_PRINTF("s_bist_mode            = %d\n", s_bist_mode     );
    USR_PRINTF("s_eq_type              = %d\n", s_eq_type       );
    USR_PRINTF("s_txfir_config             \n"                  );
    USR_PRINTF("    CM3                = %d\n", s_txfir_config.CM3         );
    USR_PRINTF("    CM2                = %d\n", s_txfir_config.CM2         );
    USR_PRINTF("    CM1                = %d\n", s_txfir_config.CM1         );
    USR_PRINTF("    C0                 = %d\n", s_txfir_config.C0          );
    USR_PRINTF("    C1                 = %d\n", s_txfir_config.C1          );
    USR_PRINTF("    main_or_max        = %d\n", s_txfir_config.main_or_max );
    USR_PRINTF("s_nes_txfir_cfg            \n"                  );
    USR_PRINTF("    nes_post1          = %d\n", s_nes_txfir_cfg.nes_post1   );
    USR_PRINTF("    nes_c0             = %d\n", s_nes_txfir_cfg.nes_c0      );
    USR_PRINTF("\n");
    USR_PRINTF("===========================================\n");
    USR_PRINTF("======      TIMEOUTS/THRESHOLDS      ======\n");
    USR_PRINTF("===========================================\n");
    USR_PRINTF("CMN_ACK_TIMEOUT_US          = %d\n", CMN_ACK_TIMEOUT_US         );
    USR_PRINTF("TX_ACK_TIMEOUT_US           = %d\n", TX_ACK_TIMEOUT_US          );
    USR_PRINTF("RX_ACK_TIMEOUT_US           = %d\n", RX_ACK_TIMEOUT_US          );
    USR_PRINTF("TX_ACK_P2_TIMEOUT_US        = %d\n", TX_ACK_P2_TIMEOUT_US       );
    USR_PRINTF("RX_ACK_P2_TIMEOUT_US        = %d\n", RX_ACK_P2_TIMEOUT_US       );
    USR_PRINTF("TX_ACK_P1_TIMEOUT_US        = %d\n", TX_ACK_P1_TIMEOUT_US       );
    USR_PRINTF("RX_CDR_TIMEOUT_US           = %d\n", RX_CDR_TIMEOUT_US          );
    USR_PRINTF("RX_BIST_TIMEOUT_US          = %d\n", RX_BIST_TIMEOUT_US         );
    USR_PRINTF("RX_LINKEVAL_FULL_TIMEOUT_US = %d\n", RX_LINKEVAL_FULL_TIMEOUT_US);
    USR_PRINTF("RX_DATABIST_TIMER_THRESH    = %d\n", RX_DATABIST_TIMER_THRESH   );
    USR_PRINTF("TX_RXDET_TIMEOUT            = %d\n", TX_RXDET_TIMEOUT_US        );

    USR_PRINTF("\n");

    return pass;
}

int c_api_inject_three_errors(mss_access_t *mss, int num_err) {
    int pass = 0;

    for (int i= 0; i<num_err; i++) {
        USR_SLEEP(1);

        USR_PRINTF("Calling aw_pmd_tx_gen_err_inject_en_set\n");
        pmd_ate_vec_comment("Enable inject TX error");
        pass += aw_pmd_tx_gen_err_inject_en_set(mss,1);

        USR_SLEEP(1);

        USR_PRINTF("Calling aw_pmd_tx_gen_err_inject_en_set\n");
        pmd_ate_vec_comment("Disable inject TX error");
        pass += aw_pmd_tx_gen_err_inject_en_set(mss,0);
    }
    USR_SLEEP(1);

    return pass;
}


int c_sv_base_test() {
    int pass = 0;
    mss_access_t mss = {.phy_offset=0, .lane_offset=0};

    svSetScope(svGetScopeFromName("serdes_env_pkg"));
    sv_print("c_api_base_test: Calling wait_apb_preset_done task from the UVM test.");
    svSetScope(svGetScopeFromName("dbe_test_pkg"));
    sv_wait_apb_preset_done();

    svSetScope(svGetScopeFromName("serdes_env_pkg"));
    sv_print("c_api_base_test: Calling initialize_dut task from the UVM test.");

    aw_pmd_rx_termination_set(&mss, 1); // example API Call

    // What sv_initialize_dut does right now? So need C APIs to replace everything except for pin sequences.
    // wait_apb_preset_done
    // initialize_refclk unless both USE_ROM_SETTINGS/REFCLK_DET_EN are set.
    // set_sris_enable if SRIS_EN is set
    // process_hexfile using LOAD_HEXFILE args
    // Iso mode if ISOLATION_MODE is 1
    // common and lane cal - RUN_CMN_CAL/RUN_LANE_CAL
    // Lane eq - RUN_LANE_EQ
    svSetScope(svGetScopeFromName("dbe_test_pkg"));
    sv_initialize_dut();

    svSetScope(svGetScopeFromName("serdes_env_pkg"));
    sv_print("c_api_base_test: Calling set_loopback_reg_cntrl task from the UVM test.");
    svSetScope(svGetScopeFromName("dbe_test_pkg"));
    sv_set_loopback_reg_cntrl(s_m_rnd_lane, s_loopback);

    svSetScope(svGetScopeFromName("serdes_env_pkg"));
    sv_print("c_api_base_test: Calling request_state_change task from the UVM test.");
    svSetScope(svGetScopeFromName("dbe_test_pkg"));
    sv_request_state_change(s_m_rnd_lane, s_power, s_rate_preset, s_data_width, s_isolation_mode, s_blocking);

    svSetScope(svGetScopeFromName("serdes_env_pkg"));
    sv_print("c_api_base_test: Calling check_hs_ref_clk_and_rx_blk_clk task from the UVM test.");
    svSetScope(svGetScopeFromName("dbe_test_pkg"));
    sv_check_hs_ref_clk_and_rx_blk_clk(s_m_rnd_lane, 1, 200); // 1 us timeout and 200 clock cycle duration.

    svSetScope(svGetScopeFromName("serdes_env_pkg"));
    sv_print("c_api_base_test: Calling set_serial_device_bit_rate task from the UVM test.");
    svSetScope(svGetScopeFromName("dbe_test_pkg"));
    sv_set_serial_device_bit_rate(s_m_rnd_lane, s_rate_arr);

    return pass;
}

// This is functionally the same as c_sv_base_test except configures the lsref
// post divider ratio to div2(1) instead of default div1 to support 312.5 lsref
int c_sv_base_test_refdiv2() {
    int pass = 0;
    mss_access_t mss = {.phy_offset=0, .lane_offset=0};

    svSetScope(svGetScopeFromName("serdes_env_pkg"));
    sv_print("c_api_base_test: Calling wait_apb_preset_done task from the UVM test.");
    svSetScope(svGetScopeFromName("dbe_test_pkg"));
    sv_wait_apb_preset_done();

    svSetScope(svGetScopeFromName("serdes_env_pkg"));
    sv_print("c_api_base_test: Calling initialize_dut task from the UVM test.");

    aw_pmd_rx_termination_set(&mss, 1); // example API Call

    // What sv_initialize_dut does right now? So need C APIs to replace everything except for pin sequences.
    // wait_apb_preset_done
    // initialize_refclk unless both USE_ROM_SETTINGS/REFCLK_DET_EN are set.
    // set_sris_enable if SRIS_EN is set
    // process_hexfile using LOAD_HEXFILE args
    // Iso mode if ISOLATION_MODE is 1
    // common and lane cal - RUN_CMN_CAL/RUN_LANE_CAL
    // Lane eq - RUN_LANE_EQ
    svSetScope(svGetScopeFromName("dbe_test_pkg"));
    sv_initialize_dut();

    aw_pmd_cmn_clkgen_refdiv_set(&mss, 1); // Set post divider setting to div2(1)

    svSetScope(svGetScopeFromName("serdes_env_pkg"));
    sv_print("c_api_base_test: Calling set_loopback_reg_cntrl task from the UVM test.");
    svSetScope(svGetScopeFromName("dbe_test_pkg"));
    sv_set_loopback_reg_cntrl(s_m_rnd_lane, s_loopback);

    svSetScope(svGetScopeFromName("serdes_env_pkg"));
    sv_print("c_api_base_test: Calling request_state_change task from the UVM test.");
    svSetScope(svGetScopeFromName("dbe_test_pkg"));
    sv_request_state_change(s_m_rnd_lane, s_power, s_rate_preset, s_data_width, s_isolation_mode, s_blocking);

    svSetScope(svGetScopeFromName("serdes_env_pkg"));
    sv_print("c_api_base_test: Calling check_hs_ref_clk_and_rx_blk_clk task from the UVM test.");
    svSetScope(svGetScopeFromName("dbe_test_pkg"));
    sv_check_hs_ref_clk_and_rx_blk_clk(s_m_rnd_lane, 1, 200); // 1 us timeout and 200 clock cycle duration.

    svSetScope(svGetScopeFromName("serdes_env_pkg"));
    sv_print("c_api_base_test: Calling set_serial_device_bit_rate task from the UVM test.");
    svSetScope(svGetScopeFromName("dbe_test_pkg"));
    sv_set_serial_device_bit_rate(s_m_rnd_lane, s_rate_arr);

    return pass;
}
int c_int_lb_nes_basic() {
    int pass = 0;

    // Base test
    pass |= c_sv_base_test();

    svSetScope(svGetScopeFromName("serdes_env_pkg"));
    sv_print("c_api_base_test: Calling delay_us task from the UVM test.");
    svSetScope(svGetScopeFromName("dbe_test_pkg"));
    delay_us(1);

    svSetScope(svGetScopeFromName("serdes_env_pkg"));
    sv_print("c_api_base_test: Calling sv_enable_lane_ext_tx_prbs_gen task from the UVM test.");
    svSetScope(svGetScopeFromName("dbe_test_pkg"));
    sv_enable_lane_ext_tx_prbs_gen(s_m_rnd_lane);

    if (s_run_lane_eq) {
        svSetScope(svGetScopeFromName("serdes_env_pkg"));
        sv_print("c_api_base_test: Calling sv_request_rx_link_eval task from the UVM test.");
        svSetScope(svGetScopeFromName("dbe_test_pkg"));
        sv_request_rx_link_eval(s_m_rnd_lane, 0); // 0: Run FULL EQ
    }

    svSetScope(svGetScopeFromName("serdes_env_pkg"));
    sv_print("c_api_base_test: Calling sv_enable_lane_ext_rx_prbs_chk task from the UVM test.");
    svSetScope(svGetScopeFromName("dbe_test_pkg"));
    sv_enable_lane_ext_rx_prbs_chk(s_m_rnd_lane);

    svSetScope(svGetScopeFromName("serdes_env_pkg"));
    sv_print("c_api_base_test: Calling delay_us task from the UVM test.");
    svSetScope(svGetScopeFromName("dbe_test_pkg"));
    delay_us(1);

    svSetScope(svGetScopeFromName("serdes_env_pkg"));
    sv_print("c_api_base_test: Calling sv_get_lane_ext_rx_prbs_chk_sts task from the UVM test.");
    svSetScope(svGetScopeFromName("dbe_test_pkg"));
    sv_get_lane_ext_rx_prbs_chk_sts(s_m_rnd_lane);

    // Look at the value of pass in the error table and give out the relevant error code using UVM error.
    svSetScope(svGetScopeFromName("serdes_env_pkg"));
    sv_error(pass);

    return pass;
}


int c_ext_lb_basic() {
    int pass = 0;

    // Base test
    pass |= c_sv_base_test();

    svSetScope(svGetScopeFromName("serdes_env_pkg"));
    sv_print("c_api_base_test: Calling delay_us task from the UVM test.");
    svSetScope(svGetScopeFromName("dbe_test_pkg"));
    delay_us(1);

    svSetScope(svGetScopeFromName("serdes_env_pkg"));
    sv_print("c_api_base_test: Calling sv_run_ext_loopback_data_and_checker task from the UVM test.");
    svSetScope(svGetScopeFromName("dbe_test_pkg"));
    sv_run_ext_loopback_data_and_checker(s_m_rnd_lane);

    // Look at the value of pass in the error table and give out the relevant error code using UVM error.
    svSetScope(svGetScopeFromName("serdes_env_pkg"));
    sv_error(pass);

    return pass;
}

int c_api_ext_lb_refdiv2(){
    int pass = 0;

    // Base test
    pass |= c_sv_base_test_refdiv2();

    svSetScope(svGetScopeFromName("serdes_env_pkg"));
    sv_print("c_api_base_test: Calling delay_us task from the UVM test.");
    svSetScope(svGetScopeFromName("dbe_test_pkg"));
    delay_us(1);

    svSetScope(svGetScopeFromName("serdes_env_pkg"));
    sv_print("c_api_base_test: Calling sv_run_ext_loopback_data_and_checker task from the UVM test.");
    svSetScope(svGetScopeFromName("dbe_test_pkg"));
    sv_run_ext_loopback_data_and_checker(s_m_rnd_lane);

    // Look at the value of pass in the error table and give out the relevant error code using UVM error.
    svSetScope(svGetScopeFromName("serdes_env_pkg"));
    sv_error(pass);

    return pass;

}
int c_api_ate_base_test(mss_access_t *mss) {
    int pass = 0;

    //Override timeouts and thresholds if FAST_SIM is enabled
    //if (s_fast_sim){
    USR_PRINTF("Loading timeout overrides\n");
    pass += c_api_ate_load_timeout_overrides();
    //}

    //Print config parameters
    pass += c_api_ate_print_config();

    //TODO: Should this live in C test?
    svSetScope(svGetScopeFromName("serdes_env_pkg"));
    sv_print("c_api_base_test: Calling wait_apb_preset_done task from the UVM test.");
    svSetScope(svGetScopeFromName("dbe_test_pkg"));
    sv_wait_apb_preset_done();

    if (s_loopback == 1){
        //Set RX termination to floating if internal loopback
        pmd_ate_vec_comment("Set rx termination to floating");
        USR_PRINTF("Calling aw_pmd_rx_termination_set\n");
        pass += aw_pmd_rx_termination_set(mss, AW_ACC_TERM_FL_AC); // example API Call
    }

    //Reference clock select
    pmd_ate_vec_comment("Set lsref clock sel");
    USR_PRINTF("Calling aw_pmd_cmn_lsref_sel_set\n");
    pass += aw_pmd_cmn_lsref_sel_set(mss,s_refsel);

    //Load hexfile
    pmd_ate_vec_comment("Load hexfile");
    USR_PRINTF("Calling c_load_hexfile\n");
    pass += c_load_hexfile(mss,s_hexfile);

    //Isolation (required for C_API test)
    USR_PRINTF("Calling c_isolate\n");
    pmd_ate_vec_comment("Isolate cmn");
    pass += aw_pmd_isolate_cmn_set(mss,1);
    pmd_ate_vec_comment("Isolate tx and rx");
    pass += aw_pmd_isolate_lane_set(mss,1);

    //CMN Power up
    pmd_ate_vec_comment("Configure and power up CMN");
    USR_PRINTF("Calling aw_pmd_iso_request_cmn_state_change\n");
    pass += aw_pmd_iso_request_cmn_state_change(mss, AW_CMN_P0, CMN_ACK_TIMEOUT_US);

    return pass;
}


int c_api_ate_serial_lb() {
    int pass = 0;
    int expected_bist_errors;

    svSetScope(svGetScopeFromName("serdes_env_pkg"));

    mss_access_t mss = {.phy_offset=0, .lane_offset=0};

    //Set broadcast mode - Note, only for ATE vector generation
    // Base test
    USR_PRINTF("Calling c_api_ate_base_test\n");
    pass += c_api_ate_base_test(&mss);

    //TX Power up
    USR_PRINTF("Calling aw_pmd_iso_request_tx_state_change\n");
    pmd_ate_vec_comment("Configure TX rate/width/pstate and request TX state change");
    pass += aw_pmd_iso_request_tx_state_change(&mss, AW_P0, s_rate_preset, s_data_width, TX_ACK_TIMEOUT_US);

    //RX Power up
    USR_PRINTF("Calling aw_pmd_iso_request_rx_state_change\n");
    pmd_ate_vec_comment("Configure RX rate/width/pstate and request TX state change");
    pass += aw_pmd_iso_request_rx_state_change(&mss, AW_P0, s_rate_preset, s_data_width, RX_ACK_TIMEOUT_US);

    //TODO: Should this live in C test?
    //svSetScope(svGetScopeFromName("serdes_env_pkg"));
    //sv_print("c_api_base_test: Calling check_hs_ref_clk_and_rx_blk_clk task from the UVM test.");
    //svSetScope(svGetScopeFromName("dbe_test_pkg"));
    //sv_check_hs_ref_clk_and_rx_blk_clk(s_m_rnd_lane, 1, 200); // 1 us timeout and 200 clock cycle duration.

    //Calling sv clk checkers
    //sv_print_clks(s_m_rnd_lane);
    //sv_check_clks(s_m_rnd_lane, s_rate_preset, s_data_width);

    //Configuring/enable TX
    USR_PRINTF("Calling aw_pmd_txfir_config_set\n");
    pmd_ate_vec_comment("Configure TX FIR settings");
    pass += aw_pmd_txfir_config_set(&mss,s_txfir_config);

    //Very short reach channel settings - this will be project specific
    if (s_vsr_en) {
        USR_PRINTF("Calling aw_pmd_rx_vga_cap_set\n");
        pmd_ate_vec_comment("Overriding vga_cap");
        pass += aw_pmd_rx_vga_cap_set(&mss,7);
        USR_PRINTF("Calling aw_pmd_rx_ctle_adapt_set\n");
        pmd_ate_vec_comment("Disabling ctle/vga_cap adapt");
        pass += aw_pmd_rx_ctle_adapt_set(&mss,0,0);
    }

    USR_PRINTF("Calling aw_pmd_tx_gen_config_set\n");
    pmd_ate_vec_comment("Configure TX BIST settings");
    pass += aw_pmd_tx_gen_config_set(&mss,s_bist_pattern,s_udp_pattern);

    pmd_ate_vec_comment("Enable TX BIST");
    USR_PRINTF("Calling aw_pmd_gen_tx_en_set\n");
    pass += aw_pmd_gen_tx_en_set(&mss,1);

    //Enable NES Loopback
    if (s_loopback == 1){ //
        pmd_ate_vec_comment("Configure internal serial loopback");
        USR_PRINTF("Calling aw_pmd_analog_loopback_set\n");
        pass += aw_pmd_analog_loopback_set(&mss,1);
        pass += aw_pmd_force_signal_detect_config_set (&mss, AW_SIGDET_FORCE1);
    } else {
	USR_PRINTF("Disabling DFE Adaptations EXT loopback\n");
	aw_pmd_rx_dfe_adapt_set(&mss, 0);
    }

    //If enabled, trigger link eval
    if (s_run_lane_eq) {
        USR_PRINTF("Calling aw_pmd_rx_equalize\n");
        pmd_ate_vec_comment("Request full rx link eval");
        pass += aw_pmd_rx_equalize(&mss,s_eq_type,RX_LINKEVAL_FULL_TIMEOUT_US);
    }

    //Check for CDR Lock
    USR_PRINTF("Calling aw_pmd_rx_check_cdr_lock\n");
    pmd_ate_vec_comment("Check for CDR Lock");
    pass += aw_pmd_rx_check_cdr_lock(&mss,RX_CDR_TIMEOUT_US);

    //Configure/Enable RX
    USR_PRINTF("Calling aw_pmd_rx_chk_config_set\n");
    pmd_ate_vec_comment("Configure RX BIST settings");
    pass += aw_pmd_rx_chk_config_set(&mss,s_bist_pattern,s_bist_mode,s_udp_pattern,2,RX_DATABIST_TIMER_THRESH);

    USR_PRINTF("Calling aw_pmd_rx_chk_en_set\n");
    pmd_ate_vec_comment("Enable RX BIST");
    pass += aw_pmd_rx_chk_en_set(&mss,1);

    //If NRZ, inject 3 errors
    if (!s_pam_en) {
        c_api_inject_three_errors(&mss, 3);
        expected_bist_errors = 3;
    } else {
        expected_bist_errors = -1;
    }

    //Checking for RX Bist lock through demapper (not needed for customer)
    //USR_PRINTF("Calling aw_pmd_rx_sweep_demapper\n");
    //pass += aw_pmd_rx_sweep_demapper (&mss, s_pam_en, 10);

    pmd_ate_vec_comment("Check RXBIST");
    USR_PRINTF("Calling aw_pmd_rx_check_bist\n");
    pass += aw_pmd_rx_check_bist(&mss, s_bist_mode, RX_DATABIST_TIMER_THRESH, s_data_width, RX_BIST_TIMEOUT_US, expected_bist_errors);

    //Link Training example
    if (s_lt_en) {
        uint32_t clause =  4; //Clause 162

        uint32_t lt_running;
        uint32_t lt_done;
        uint32_t lt_training_failure;
        uint32_t lt_rx_ready;

        USR_PRINTF("Link Training Enabled\n");
        USR_PRINTF("Calling aw_pmd_analog_loopback_set\n");
        pass += aw_pmd_gen_tx_en_set(&mss,0);
        USR_PRINTF("Calling aw_pmd_rx_chk_en_set\n");
        pass += aw_pmd_rx_chk_en_set(&mss,0);

        pass += aw_pmd_anlt_link_training_en_set (&mss, 1);
        pass += aw_pmd_anlt_logical_lane_num_set (&mss, 0, 1);
        pass += aw_pmd_anlt_link_training_config_set (&mss, s_data_width, clause);
        pass += aw_pmd_anlt_link_training_start_set (&mss, 1);

        USR_SLEEP(300);
        pass += aw_pmd_anlt_link_training_status_get (&mss, &lt_running, &lt_done, &lt_training_failure, &lt_rx_ready);
        USR_PRINTF("lt_running = %d\n", lt_running);
        USR_PRINTF("lt_done = %d\n", lt_done);
        USR_PRINTF("lt_training_failure = %d\n", lt_training_failure);
        USR_PRINTF("lt_rx_ready = %d\n", lt_rx_ready);
    }

    //USR_PRINTF("Calling aw_pmd_read_status\n");
    //pass += aw_pmd_read_status(&mss);


    // Look at the value of pass in the error table and give out the relevant error code using UVM error.
    svSetScope(svGetScopeFromName("serdes_env_pkg"));
    sv_error(pass);

    return pass;
}


int c_api_ate_burnin() {
    int pass = 0;

    svSetScope(svGetScopeFromName("serdes_env_pkg"));

    mss_access_t mss = {.phy_offset=0, .lane_offset=0};

    //Set broadcast mode - Note, only for ATE vector generation
    if (s_broadcast_en) {
        pmd_set_lane(&mss, 99);
    }

    // Base test
    USR_PRINTF("Calling c_api_ate_base_test\n");
    pass += c_api_ate_base_test(&mss);


    //pmd_ate_vec_comment("Overriding TX postdiv to div8");
    //CHECK(pmd_write_field(&mss, RATE_CNTRL_TX_ADDR, RATE_CNTRL_TX_POSTDIV_NT_MASK, RATE_CNTRL_TX_POSTDIV_NT_OFFSET, 3));

    //TX Power up
    USR_PRINTF("Calling aw_pmd_iso_request_tx_state_change\n");
    pmd_ate_vec_comment("Configure TX rate/width/pstate and request TX state change");
    pass += aw_pmd_iso_request_tx_state_change(&mss, AW_P0, s_rate_preset, s_data_width, TX_ACK_TIMEOUT_US);

    //RX Power up
    USR_PRINTF("Calling aw_pmd_iso_request_rx_state_change\n");
    pmd_ate_vec_comment("Configure RX rate/width/pstate and request TX state change");
    pass += aw_pmd_iso_request_rx_state_change(&mss, AW_P0, s_rate_preset, s_data_width, RX_ACK_TIMEOUT_US);

    pmd_ate_vec_comment("Overriding TX postdiv to div16");
    CHECK(pmd_write_field(&mss, RATE_CNTRL_TX_ADDR, RATE_CNTRL_TX_POSTDIV_NT_MASK, RATE_CNTRL_TX_POSTDIV_NT_OFFSET, 4));
    CHECK(pmd_write_field(&mss, SEQ_CNTRL_TX_ADDR, SEQ_CNTRL_TX_POSTDIV_READY_A_MASK, SEQ_CNTRL_TX_POSTDIV_READY_A_OFFSET, 0));
    CHECK(pmd_write_field(&mss, SEQ_CNTRL_TX_ADDR, SEQ_CNTRL_TX_POSTDIV_READY_A_MASK, SEQ_CNTRL_TX_POSTDIV_READY_A_OFFSET, 1));

    pmd_ate_vec_comment("Overriding RX postdiv to div4");
    CHECK(pmd_write_field(&mss, RATE_CNTRL_RX_ADDR, RATE_CNTRL_RX_POSTDIV_NT_MASK, RATE_CNTRL_RX_POSTDIV_NT_OFFSET, 2));
    CHECK(pmd_write_field(&mss, SEQ_CNTRL_RX_ADDR, SEQ_CNTRL_RX_POSTDIV_READY_A_MASK, SEQ_CNTRL_RX_POSTDIV_READY_A_OFFSET, 0));
    CHECK(pmd_write_field(&mss, SEQ_CNTRL_RX_ADDR, SEQ_CNTRL_RX_POSTDIV_READY_A_MASK, SEQ_CNTRL_RX_POSTDIV_READY_A_OFFSET, 1));

    USR_PRINTF("Calling aw_pmd_tx_gen_config_set\n");
    pmd_ate_vec_comment("Configure TX BIST settings");
    pass += aw_pmd_tx_gen_config_set(&mss,s_bist_pattern,s_udp_pattern);

    pmd_ate_vec_comment("Enable TX BIST");
    USR_PRINTF("Calling aw_pmd_gen_tx_en_set\n");
    pass += aw_pmd_gen_tx_en_set(&mss,1);

    //Enable NES Loopback
    if (s_loopback == 1){ //
        pmd_ate_vec_comment("Configure internal serial loopback");
        USR_PRINTF("Calling aw_pmd_analog_loopback_set\n");
        pass += aw_pmd_analog_loopback_set(&mss,1);
        pass += aw_pmd_force_signal_detect_config_set (&mss, AW_SIGDET_FORCE1);
    }

    // Look at the value of pass in the error table and give out the relevant error code using UVM error.
    svSetScope(svGetScopeFromName("serdes_env_pkg"));
    sv_error(pass);

    return pass;
}


/**
int c_api_pup_cmn(){
    int pass = 0;

    svSetScope(svGetScopeFromName("serdes_env_pkg"));

    mss_access_t mss = {.phy_offset=0, .lane_offset=0};
    //pmd_write_field(&mss, LCPLL_CHECK_ADDR, LCPLL_CHECK_START_A_MASK, LCPLL_CHECK_START_A_OFFSET, 1);

    //Reference clock select
    //m_reg  = this.m_env.m_v_sqr.dbe_csr_model.get_reg_ref(99, "cmn_synth" );
    //m_reg.get_field_by_name("lsref_select_nt").set(m_tb_cfg.m_ls_refclk_mux);
    USR_PRINTF("Calling aw_pmd_cmn_lsref_sel_set\n");
    pass += aw_pmd_cmn_lsref_sel_set(&mss,s_refsel);


    //CHECK(pmd_write_field(&mss, CMN_REFCLK_ADDR, CMN_REFCLK_L2R0_LSREF_SELECT_NT_MASK, CMN_REFCLK_L2R0_LSREF_SELECT_NT_OFFSET, 2));


    //m_reg = this.m_env.m_v_sqr.dbe_csr_model.get_reg_ref(99, "cmn_refclk");
    if  (s_refsel == 6){
        //m_reg.get_field_by_name("l2r0_lsref_select_nt").set(2'b10);
        CHECK(pmd_write_field(&mss, CMN_REFCLK_ADDR, CMN_REFCLK_L2R0_LSREF_SELECT_NT_MASK, CMN_REFCLK_L2R0_LSREF_SELECT_NT_OFFSET, 2));
    }else if(s_refsel == 5){
        //m_reg.get_field_by_name("l2r0_lsref_select_nt").set(2'b10);
        CHECK(pmd_write_field(&mss, CMN_REFCLK_ADDR, CMN_REFCLK_L2R0_LSREF_SELECT_NT_MASK, CMN_REFCLK_L2R0_LSREF_SELECT_NT_OFFSET, 2));
    }else if(s_refsel == 4){
        //m_reg.get_field_by_name("l2r0_lsref_select_nt").set(2'b11);
        CHECK(pmd_write_field(&mss, CMN_REFCLK_ADDR, CMN_REFCLK_L2R0_LSREF_SELECT_NT_MASK, CMN_REFCLK_L2R0_LSREF_SELECT_NT_OFFSET, 3));

    }else if(s_refsel == 3){
        //m_reg.get_field_by_name("r2l0_lsref_select_nt").set(1%2);
        CHECK(pmd_write_field(&mss, CMN_REFCLK_ADDR, CMN_REFCLK_R2L0_LSREF_SELECT_NT_MASK, CMN_REFCLK_R2L0_LSREF_SELECT_NT_OFFSET, 1%2));
    }else if(s_refsel == 2){
        //m_reg.get_field_by_name("r2l1_lsref_select_nt").set(1%2);
        CHECK(pmd_write_field(&mss, CMN_REFCLK_ADDR, CMN_REFCLK_R2L1_LSREF_SELECT_NT_MASK, CMN_REFCLK_R2L1_LSREF_SELECT_NT_OFFSET, 1%2));
    }else if(s_refsel == 1){
        //m_reg.get_field_by_name("l2r0_lsref_select_nt").set(1%2);
        CHECK(pmd_write_field(&mss, CMN_REFCLK_ADDR, CMN_REFCLK_L2R0_LSREF_SELECT_NT_MASK, CMN_REFCLK_L2R0_LSREF_SELECT_NT_OFFSET, 1%2));
    }
    else if(s_refsel == 0){
        //m_reg.get_field_by_name("l2r1_lsref_select_nt").set(1%2);
        CHECK(pmd_write_field(&mss, CMN_REFCLK_ADDR, CMN_REFCLK_L2R1_LSREF_SELECT_NT_MASK, CMN_REFCLK_L2R1_LSREF_SELECT_NT_OFFSET, 1%2));
    }



    //m_reg = this.m_env.m_v_sqr.dbe_csr_model.get_reg_ref(99, "cmn_calstore");
    //m_reg.get_field_by_name("valid"            ).set(1    );
    CHECK(pmd_write_field(&mss, CMN_CALSTORE_ADDR, CMN_CALSTORE_VALID_MASK, CMN_CALSTORE_VALID_OFFSET, 1));

    //m_reg = this.m_env.m_v_sqr.dbe_csr_model.get_reg_ref(99, "cmn_calstore");
    //m_reg.get_field_by_name("clkgen_osc_cal_nt").set(69   );
    CHECK(pmd_write_field(&mss, CMN_CALSTORE_ADDR, CMN_CALSTORE_CLKGEN_OSC_CAL_NT_MASK, CMN_CALSTORE_CLKGEN_OSC_CAL_NT_OFFSET, 69));

    //m_reg = this.m_env.m_v_sqr.dbe_csr_model.get_reg_ref(99, "cmn_calstore");
    //m_reg.get_field_by_name("osc_cal_fine"     ).set('h138);
    CHECK(pmd_write_field(&mss, CMN_CALSTORE_ADDR, CMN_CALSTORE_OSC_CAL_FINE_MASK, CMN_CALSTORE_OSC_CAL_FINE_OFFSET, 0x138));

    //m_reg = this.m_env.m_v_sqr.dbe_csr_model.get_reg_ref(99, "cmn_calstore");
    //m_reg.get_field_by_name("termcal_nt"       ).set(0    );
    CHECK(pmd_write_field(&mss, CMN_CALSTORE_ADDR, CMN_CALSTORE_TERMCAL_NT_MASK, CMN_CALSTORE_TERMCAL_NT_OFFSET, 0));


    //reference
    //m_reg = this.m_env.m_v_sqr.dbe_csr_model.get_reg_ref(99, "cmn_refclk");
    //m_reg.get_field_by_name("r2l0_lsref_select_nt").set(1%2);
    //m_reg.update(.status(status), .map(this.m_env.m_v_sqr.dbe_csr_model.dbe_jtag_map));

    // Reset and Take control of APB logic
    //-----------------------------------------------------------
    /////m_jtag_init_apb_bridge_seq = jtag_init_apb_bridge_seq::type_id::create("m_jtag_init_apb_bridge_seq", this);
    //////m_jtag_init_apb_bridge_seq.start(m_env.m_jtag_agent.m_sequencer);
    //`uvm_info(get_type_name(), \$sformatf("Writte using JTAG MAP "), UVM_LOW)
   // m_reg = this.m_env.m_csr_top.get_reg_ref(99, "cmn_clkgen_occ");
    //m_reg.read(status, rd_data);
    //m_reg.get_field_by_name("occ1div_nt").set(occ_div);
    //m_reg.get_field_by_name("occ2div_nt").set(occ_div);
    CHECK(pmd_write_field(&mss, CMN_CLKGEN_OCC_ADDR, CMN_CLKGEN_OCC_OCC1DIV_NT_MASK, CMN_CLKGEN_OCC_OCC1DIV_NT_OFFSET, 58));
    CHECK(pmd_write_field(&mss, CMN_CLKGEN_OCC_ADDR, CMN_CLKGEN_OCC_OCC2DIV_NT_MASK, CMN_CLKGEN_OCC_OCC2DIV_NT_OFFSET, 58));

    //m_reg = this.m_env.m_csr_top.get_reg_ref(99, "rst_afe_cmn");
    //m_reg.read(status, rd_data);
    //m_reg.get_field_by_name("clkgen_occdiv_ba").set(1);
    CHECK(pmd_write_field(&mss, RST_AFE_CMN_ADDR, RST_AFE_CMN_CLKGEN_OCCDIV_BA_MASK, RST_AFE_CMN_CLKGEN_OCCDIV_BA_OFFSET, 1));

    //m_reg = this.m_env.m_csr_top.get_reg_ref(99, "pd_afe_cmn");
    //m_reg.read(status, rd_data);
    //m_reg.get_field_by_name("bias_ba").set(1);
    CHECK(pmd_write_field(&mss, PD_AFE_CMN_ADDR, PD_AFE_CMN_BIAS_BA_MASK, PD_AFE_CMN_BIAS_BA_OFFSET, 1));


    //#10us; //cmn_pup_bias delay
    svSetScope(svGetScopeFromName("dbe_test_pkg"));
    delay_us(10);

    //m_reg = this.m_env.m_csr_top.get_reg_ref(99, "pd_afe_cmn");
    //m_reg.read(status, rd_data);
    //m_reg.get_field_by_name("reg_lsref_ba").set(1);
    CHECK(pmd_write_field(&mss, PD_AFE_CMN_ADDR, PD_AFE_CMN_REG_LSREF_BA_MASK, PD_AFE_CMN_REG_LSREF_BA_OFFSET, 1));

    //#30us;//cmn_pup_lsref
    svSetScope(svGetScopeFromName("dbe_test_pkg"));
    delay_us(30);

    //m_reg = this.m_env.m_csr_top.get_reg_ref(99, "pd_afe_cmn");
    //m_reg.read(status, rd_data);
    //m_reg.get_field_by_name("lsrefbuf_ba").set(1);
    CHECK(pmd_write_field(&mss, PD_AFE_CMN_ADDR, PD_AFE_CMN_LSREFBUF_BA_MASK, PD_AFE_CMN_LSREFBUF_BA_OFFSET, 1));

    //#1us; //cmn_pup_lsrebuf
    svSetScope(svGetScopeFromName("dbe_test_pkg"));
    delay_us(1);

    //m_reg = this.m_env.m_v_sqr.dbe_csr_model.get_reg_ref(99, "cmn_clkgen");
    //m_reg.read(status, rd_data);
    //m_reg.get_field_by_name("refdiv_nt").set(1);
    CHECK(pmd_write_field(&mss, CMN_CLKGEN_ADDR, CMN_CLKGEN_REFDIV_NT_MASK, CMN_CLKGEN_REFDIV_NT_OFFSET, 1));

    //, .map(this.m_env.m_v_sqr.dbe_csr_model.dbe_jtag_map)
    //m_reg = this.m_env.m_v_sqr.dbe_csr_model.get_reg_ref(99, "rst_afe_cmn");
    //m_reg.read(status, rd_data);
    //m_reg.get_field_by_name("clkgen_refdiv_ba").set(1);
    CHECK(pmd_write_field(&mss, RST_AFE_CMN_ADDR, RST_AFE_CMN_CLKGEN_REFDIV_BA_MASK, RST_AFE_CMN_CLKGEN_REFDIV_BA_OFFSET, 1));

    //#100ns;
    svSetScope(svGetScopeFromName("dbe_test_pkg"));
    delay_us(1);


    //#1us;
    svSetScope(svGetScopeFromName("dbe_test_pkg"));
    delay_us(1);
    ///////////////////////


    //m_reg = this.m_env.m_v_sqr.dbe_csr_model.get_reg_ref(99, "afe_cmn_clkgen_reg2");
    //m_reg.read(status, rd_data);
    //m_reg.get_field_by_name("fbdivint").set(15);
    CHECK(pmd_write_field(&mss, AFE_CMN_CLKGEN_REG2_ADDR, AFE_CMN_CLKGEN_REG2_FBDIVINT_MASK, AFE_CMN_CLKGEN_REG2_FBDIVINT_OFFSET, 15));

    //m_reg = this.m_env.m_v_sqr.dbe_csr_model.get_reg_ref(99, "afe_cmn_lcpll_fracdiv_reg1");
    //m_reg.read(status, rd_data);
    //m_reg.get_field_by_name("fracn_value_nt").set(134217728);////cehck
    CHECK(pmd_write_field(&mss, AFE_CMN_LCPLL_FRACDIV_REG1_ADDR, AFE_CMN_LCPLL_FRACDIV_REG1_FRACN_VALUE_NT_MASK, AFE_CMN_LCPLL_FRACDIV_REG1_FRACN_VALUE_NT_OFFSET, 134217728));

    //m_reg = this.m_env.m_v_sqr.dbe_csr_model.get_reg_ref(99, "afe_cmn_clkgen_reg1");
    //m_reg.read(status, rd_data);
    //m_reg.get_field_by_name("hsrefdiv_nt").set(0);
    CHECK(pmd_write_field(&mss, AFE_CMN_CLKGEN_REG1_ADDR, AFE_CMN_CLKGEN_REG1_HSREFDIV_NT_MASK, AFE_CMN_CLKGEN_REG1_HSREFDIV_NT_OFFSET, 0));


    //m_reg = this.m_env.m_v_sqr.dbe_csr_model.get_reg_ref(99, "afe_cmn_clkgen_reg1");
    //m_reg.read(status, rd_data);
    //m_reg.get_field_by_name("vcodiv_nt").set(12);
    CHECK(pmd_write_field(&mss, AFE_CMN_CLKGEN_REG1_ADDR, AFE_CMN_CLKGEN_REG1_VCODIV_NT_MASK, AFE_CMN_CLKGEN_REG1_VCODIV_NT_OFFSET, 12));


    //m_reg = this.m_env.m_v_sqr.dbe_csr_model.get_reg_ref(99, "switchclk_dbe_cmn");
    //m_reg.read(status, rd_data);
    //m_reg.get_field_by_name("vco_adapt_div_nt").set(0);
    CHECK(pmd_write_field(&mss, SWITCHCLK_DBE_CMN_ADDR, SWITCHCLK_DBE_CMN_VCO_ADAPT_DIV_NT_MASK, SWITCHCLK_DBE_CMN_VCO_ADAPT_DIV_NT_OFFSET, 0));

    //m_reg = this.m_env.m_v_sqr.dbe_csr_model.get_reg_ref(99, "afe_cmn_vco_adapt_reg1");
    //m_reg.read(status, rd_data);
    //m_reg.get_field_by_name("freq_target_nt").set(21760);
    CHECK(pmd_write_field(&mss, AFE_CMN_VCO_ADAPT_REG1_ADDR, AFE_CMN_VCO_ADAPT_REG1_FREQ_TARGET_NT_MASK, AFE_CMN_VCO_ADAPT_REG1_FREQ_TARGET_NT_OFFSET, 21760));
    ////////////////////////////
    //m_reg = this.m_env.m_v_sqr.dbe_csr_model.get_reg_ref(99, "afe_cmn_lcpll_osc_reg1");
    //m_reg.read(status, rd_data);
    //m_reg.get_field_by_name("gain_load_a").set(0);
    CHECK(pmd_write_field(&mss, AFE_CMN_LCPLL_OSC_REG1_ADDR, AFE_CMN_LCPLL_OSC_REG1_GAIN_LOAD_A_MASK, AFE_CMN_LCPLL_OSC_REG1_GAIN_LOAD_A_OFFSET, 0));

    //m_reg = this.m_env.m_v_sqr.dbe_csr_model.get_reg_ref(99, "afe_cmn_lcpll_osc_reg1");
    //m_reg.read(status, rd_data);
    //m_reg.get_field_by_name("kp_nt").set('h150);
    CHECK(pmd_write_field(&mss, AFE_CMN_LCPLL_OSC_REG1_ADDR, AFE_CMN_LCPLL_OSC_REG1_KP_NT_MASK, AFE_CMN_LCPLL_OSC_REG1_KP_NT_OFFSET, 0x150));

    //m_reg = this.m_env.m_v_sqr.dbe_csr_model.get_reg_ref(99, "afe_cmn_lcpll_osc_reg1");
    //m_reg.read(status, rd_data);
    //m_reg.get_field_by_name("ki_mu_nt").set(8);
    CHECK(pmd_write_field(&mss, AFE_CMN_LCPLL_OSC_REG1_ADDR, AFE_CMN_LCPLL_OSC_REG1_KI_MU_NT_MASK, AFE_CMN_LCPLL_OSC_REG1_KI_MU_NT_OFFSET, 8));

    //m_reg = this.m_env.m_v_sqr.dbe_csr_model.get_reg_ref(99, "afe_cmn_lcpll_osc_reg2");
    //m_reg.read(status, rd_data);
    //m_reg.get_field_by_name("iir_gain_nt").set('h400);
    CHECK(pmd_write_field(&mss, AFE_CMN_LCPLL_OSC_REG2_ADDR, AFE_CMN_LCPLL_OSC_REG2_IIR_GAIN_NT_MASK, AFE_CMN_LCPLL_OSC_REG2_IIR_GAIN_NT_OFFSET, 0x400));

    //m_reg = this.m_env.m_v_sqr.dbe_csr_model.get_reg_ref(99, "afe_cmn_lcpll_osc_reg1");
    //m_reg.read(status, rd_data);
    //m_reg.get_field_by_name("gain_load_a").set(1);
    CHECK(pmd_write_field(&mss, AFE_CMN_LCPLL_OSC_REG1_ADDR, AFE_CMN_LCPLL_OSC_REG1_GAIN_LOAD_A_MASK, AFE_CMN_LCPLL_OSC_REG1_GAIN_LOAD_A_OFFSET, 1));
    /////////////////////////////////

    //m_reg = this.m_env.m_v_sqr.dbe_csr_model.get_reg_ref(99, "pd_afe_cmn");
    //m_reg.read(status, rd_data);
    //m_reg.get_field_by_name("reg_hsref_ba").set(1);
    CHECK(pmd_write_field(&mss, PD_AFE_CMN_ADDR, PD_AFE_CMN_REG_HSREF_BA_MASK, PD_AFE_CMN_REG_HSREF_BA_OFFSET, 1));

    //#30us; // cmn_pup_hsref
    svSetScope(svGetScopeFromName("dbe_test_pkg"));
    delay_us(30);

    //m_reg = this.m_env.m_v_sqr.dbe_csr_model.get_reg_ref(99, "pd_afe_cmn");
    //m_reg.read(status, rd_data);
    //m_reg.get_field_by_name("hsrefbuf_ba").set(1);
    CHECK(pmd_write_field(&mss, PD_AFE_CMN_ADDR, PD_AFE_CMN_HSREFBUF_BA_MASK, PD_AFE_CMN_HSREFBUF_BA_OFFSET, 1));

    //#1us; //cmn_pup_hsrefbuf
    svSetScope(svGetScopeFromName("dbe_test_pkg"));
    delay_us(1);

    //m_reg = this.m_env.m_v_sqr.dbe_csr_model.get_reg_ref(99, "pd_afe_cmn");
    //m_reg.read(status, rd_data);
    //m_reg.get_field_by_name("clkgen_ba").set(1);
    CHECK(pmd_write_field(&mss, PD_AFE_CMN_ADDR, PD_AFE_CMN_CLKGEN_BA_MASK, PD_AFE_CMN_CLKGEN_BA_OFFSET, 1));

    //#10us; //cmn_pup_clkgen
    svSetScope(svGetScopeFromName("dbe_test_pkg"));
    delay_us(10);

    //m_reg = this.m_env.m_v_sqr.dbe_csr_model.get_reg_ref(99, "rst_afe_cmn");
    //m_reg.read(status, rd_data);
    //m_reg.get_field_by_name("clkgen_dco_ba").set(1);
    //m_reg.get_field_by_name("clkgen_postdiv_ba").set(1);
    //m_reg.get_field_by_name("clkgen_ba").set(1);
    //m_reg.get_field_by_name("clkgen_pclk_ba").set(1);
    CHECK(pmd_write_field(&mss, RST_AFE_CMN_ADDR, RST_AFE_CMN_CLKGEN_DCO_BA_MASK, RST_AFE_CMN_CLKGEN_DCO_BA_OFFSET, 1));
    CHECK(pmd_write_field(&mss, RST_AFE_CMN_ADDR, RST_AFE_CMN_CLKGEN_POSTDIV_BA_MASK, RST_AFE_CMN_CLKGEN_POSTDIV_BA_OFFSET, 1));
    CHECK(pmd_write_field(&mss, RST_AFE_CMN_ADDR, RST_AFE_CMN_CLKGEN_BA_MASK, RST_AFE_CMN_CLKGEN_BA_OFFSET, 1));
    CHECK(pmd_write_field(&mss, RST_AFE_CMN_ADDR, RST_AFE_CMN_CLKGEN_PCLK_BA_MASK, RST_AFE_CMN_CLKGEN_PCLK_BA_OFFSET, 1));

    //#100ns; //cmn_pup_clkgen_rst
    svSetScope(svGetScopeFromName("dbe_test_pkg"));
    delay_us(1);

    //m_reg = this.m_env.m_v_sqr.dbe_csr_model.get_reg_ref(99, "switchclk_dbe_cmn");
   // m_reg.read(status, rd_data);
    //m_reg.get_field_by_name("vcodiv_sel_nt").set(1);
    CHECK(pmd_write_field(&mss, SWITCHCLK_DBE_CMN_ADDR, SWITCHCLK_DBE_CMN_VCODIV_SEL_NT_MASK, SWITCHCLK_DBE_CMN_VCODIV_SEL_NT_OFFSET, 1));

    //#100ns; //cmn_pup_vcodiv_switchclk
    svSetScope(svGetScopeFromName("dbe_test_pkg"));
    delay_us(1);

    //m_reg = this.m_env.m_v_sqr.dbe_csr_model.get_reg_ref(99, "rst_dbe_cmn");
    //m_reg.read(status, rd_data);
    //m_reg.get_field_by_name("vco_adapt_ba").set(1);
    CHECK(pmd_write_field(&mss, RST_DBE_CMN_ADDR, RST_DBE_CMN_VCO_ADAPT_BA_MASK, RST_DBE_CMN_VCO_ADAPT_BA_OFFSET, 1));


    //////////////////////////////////////////

    //m_reg = this.m_env.m_v_sqr.dbe_csr_model.get_reg_ref(99, "afe_cmn_vco_adapt_reg2");
    //m_reg.read(status, rd_data);
    //m_reg.get_field_by_name("clkgen_osc_cal_nt").set('h45);
    CHECK(pmd_write_field(&mss, AFE_CMN_VCO_ADAPT_REG2_ADDR, AFE_CMN_VCO_ADAPT_REG2_CLKGEN_OSC_CAL_NT_MASK, AFE_CMN_VCO_ADAPT_REG2_CLKGEN_OSC_CAL_NT_OFFSET, 0x45));

    //m_reg = this.m_env.m_v_sqr.dbe_csr_model.get_reg_ref(99, "afe_cmn_vco_adapt_reg2");
    //m_reg.read(status, rd_data);
    //m_reg.get_field_by_name("bypass_ena_a").set(1);
    CHECK(pmd_write_field(&mss, AFE_CMN_VCO_ADAPT_REG2_ADDR, AFE_CMN_VCO_ADAPT_REG2_BYPASS_ENA_A_MASK, AFE_CMN_VCO_ADAPT_REG2_BYPASS_ENA_A_OFFSET, 1));

    //m_reg = this.m_env.m_v_sqr.dbe_csr_model.get_reg_ref(99, "afe_cmn_lcpll_osc_reg4");
    //m_reg.read(status, rd_data);
    //m_reg.get_field_by_name("accum_bypass_value_nt").set('h4e000000);
    CHECK(pmd_write_field(&mss, AFE_CMN_LCPLL_OSC_REG4_ADDR, AFE_CMN_LCPLL_OSC_REG4_ACCUM_BYPASS_VALUE_NT_MASK, AFE_CMN_LCPLL_OSC_REG4_ACCUM_BYPASS_VALUE_NT_OFFSET, 0x4e000000));

    //m_reg = this.m_env.m_v_sqr.dbe_csr_model.get_reg_ref(99, "afe_cmn_lcpll_osc_reg3");
    //m_reg.read(status, rd_data);
    //m_reg.get_field_by_name("accum_bypass_a").set(1);
    CHECK(pmd_write_field(&mss, AFE_CMN_LCPLL_OSC_REG3_ADDR, AFE_CMN_LCPLL_OSC_REG3_ACCUM_BYPASS_A_MASK, AFE_CMN_LCPLL_OSC_REG3_ACCUM_BYPASS_A_OFFSET, 1));



    //m_reg = this.m_env.m_v_sqr.dbe_csr_model.get_reg_ref(99, "afe_cmn_lcpll_fracdiv_reg2");
    //m_reg.read(status, rd_data);
    //m_reg.get_field_by_name("fracn_enable_a").set(1);
    CHECK(pmd_write_field(&mss, AFE_CMN_LCPLL_FRACDIV_REG2_ADDR, AFE_CMN_LCPLL_FRACDIV_REG2_FRACN_ENABLE_A_MASK, AFE_CMN_LCPLL_FRACDIV_REG2_FRACN_ENABLE_A_OFFSET, 1));

    //m_reg = this.m_env.m_v_sqr.dbe_csr_model.get_reg_ref(99, "afe_cmn_lcpll_osc_reg1");
    //m_reg.read(status, rd_data);
    //m_reg.get_field_by_name("osc_enable_a").set(1);
    CHECK(pmd_write_field(&mss, AFE_CMN_LCPLL_OSC_REG1_ADDR, AFE_CMN_LCPLL_OSC_REG1_OSC_ENABLE_A_MASK, AFE_CMN_LCPLL_OSC_REG1_OSC_ENABLE_A_OFFSET, 1));

    //m_reg = this.m_env.m_v_sqr.dbe_csr_model.get_reg_ref(99, "afe_cmn_lcpll_osc_reg3");
    //m_reg.read(status, rd_data);
    //m_reg.get_field_by_name("accum_bypass_a").set(1);
    CHECK(pmd_write_field(&mss, AFE_CMN_LCPLL_OSC_REG3_ADDR, AFE_CMN_LCPLL_OSC_REG3_ACCUM_BYPASS_A_MASK, AFE_CMN_LCPLL_OSC_REG3_ACCUM_BYPASS_A_OFFSET, 1));

    //m_reg = this.m_env.m_v_sqr.dbe_csr_model.get_reg_ref(99, "afe_cmn_lcpll_fracdiv_reg2");
    //m_reg.read(status, rd_data);
    //m_reg.get_field_by_name("fracn_enable_a").set(1);
    CHECK(pmd_write_field(&mss, AFE_CMN_LCPLL_FRACDIV_REG2_ADDR, AFE_CMN_LCPLL_FRACDIV_REG2_FRACN_ENABLE_A_MASK, AFE_CMN_LCPLL_FRACDIV_REG2_FRACN_ENABLE_A_OFFSET, 1));

    //m_reg = this.m_env.m_v_sqr.dbe_csr_model.get_reg_ref(99, "afe_cmn_lcpll_osc_reg1");
    //m_reg.read(status, rd_data);
    //m_reg.get_field_by_name("osc_enable_a").set(1);
    CHECK(pmd_write_field(&mss, AFE_CMN_LCPLL_OSC_REG1_ADDR, AFE_CMN_LCPLL_OSC_REG1_OSC_ENABLE_A_MASK, AFE_CMN_LCPLL_OSC_REG1_OSC_ENABLE_A_OFFSET, 1));

    //m_reg = this.m_env.m_v_sqr.dbe_csr_model.get_reg_ref(99, "afe_cmn_lcpll_osc_reg3");
    //m_reg.read(status, rd_data);
    //m_reg.get_field_by_name("osc_bypass_a").set(0);
    //m_reg.get_field_by_name("accum_bypass_a").set(0);
    CHECK(pmd_write_field(&mss, AFE_CMN_LCPLL_OSC_REG3_ADDR, AFE_CMN_LCPLL_OSC_REG3_OSC_BYPASS_A_MASK, AFE_CMN_LCPLL_OSC_REG3_OSC_BYPASS_A_OFFSET, 0));
    CHECK(pmd_write_field(&mss, AFE_CMN_LCPLL_OSC_REG3_ADDR, AFE_CMN_LCPLL_OSC_REG3_ACCUM_BYPASS_A_MASK, AFE_CMN_LCPLL_OSC_REG3_ACCUM_BYPASS_A_OFFSET, 0));

    //m_reg = this.m_env.m_v_sqr.dbe_csr_model.get_reg_ref(99, "cmn_calstore");
    //m_reg.read(status, rd_data);
    //m_reg.get_field_by_name("valid").set(1);
    CHECK(pmd_write_field(&mss, CMN_CALSTORE_ADDR, CMN_CALSTORE_VALID_MASK, CMN_CALSTORE_VALID_OFFSET, 1));


    //#50us; // Wait for lock
    svSetScope(svGetScopeFromName("dbe_test_pkg"));
    delay_us(50);
    //i_reg.write_field(0, "afe_cmn_lcpll_osc_reg1[gain_load_a]",0,REG_TIMEOUT_NS);
    //m_reg = this.m_env.m_v_sqr.dbe_csr_model.get_reg_ref(99, "afe_cmn_lcpll_osc_reg1");
    //m_reg.read(status, rd_data);
    //m_reg.get_field_by_name("gain_load_a").set(0);
    CHECK(pmd_write_field(&mss, AFE_CMN_LCPLL_OSC_REG1_ADDR, AFE_CMN_LCPLL_OSC_REG1_GAIN_LOAD_A_MASK, AFE_CMN_LCPLL_OSC_REG1_GAIN_LOAD_A_OFFSET, 0));


    //i_reg.write_field(0, "afe_cmn_lcpll_osc_reg1[kp_nt]",'h14,REG_TIMEOUT_NS);      // FIXME
    //m_reg = this.m_env.m_v_sqr.dbe_csr_model.get_reg_ref(99, "afe_cmn_lcpll_osc_reg1");
    //m_reg.read(status, rd_data);
    //m_reg.get_field_by_name("kp_nt").set('h14);
    CHECK(pmd_write_field(&mss, AFE_CMN_LCPLL_OSC_REG1_ADDR, AFE_CMN_LCPLL_OSC_REG1_KP_NT_MASK, AFE_CMN_LCPLL_OSC_REG1_KP_NT_OFFSET, 0x14));
    //i_reg.write_field(0, "afe_cmn_lcpll_osc_reg1[ki_mu_nt]",'h12,REG_TIMEOUT_NS);     // FIXME
    //m_reg = this.m_env.m_v_sqr.dbe_csr_model.get_reg_ref(99, "afe_cmn_lcpll_osc_reg1");
    //m_reg.read(status, rd_data);
    //m_reg.get_field_by_name("ki_mu_nt").set('h12);
    CHECK(pmd_write_field(&mss, AFE_CMN_LCPLL_OSC_REG1_ADDR, AFE_CMN_LCPLL_OSC_REG1_KI_MU_NT_MASK, AFE_CMN_LCPLL_OSC_REG1_KI_MU_NT_OFFSET, 0x12));

    //#100ns;
    svSetScope(svGetScopeFromName("dbe_test_pkg"));
    delay_us(1);

    //i_reg.write_field(0, "afe_cmn_lcpll_osc_reg1[gain_load_a]",1,REG_TIMEOUT_NS);
    //m_reg = this.m_env.m_v_sqr.dbe_csr_model.get_reg_ref(99, "afe_cmn_lcpll_osc_reg1");
    //m_reg.read(status, rd_data);
    //m_reg.get_field_by_name("gain_load_a").set(1);
    CHECK(pmd_write_field(&mss, AFE_CMN_LCPLL_OSC_REG1_ADDR, AFE_CMN_LCPLL_OSC_REG1_GAIN_LOAD_A_MASK, AFE_CMN_LCPLL_OSC_REG1_GAIN_LOAD_A_OFFSET, 1));

    //#10us;
    svSetScope(svGetScopeFromName("dbe_test_pkg"));
    delay_us(10);

    //m_reg = this.m_env.m_v_sqr.dbe_csr_model.get_reg_ref(99, "switchclk_dbe_cmn");
    //m_reg.read(status, rd_data);
    //m_reg.get_field_by_name("sram_fast_sel_nt").set(1);
    CHECK(pmd_write_field(&mss, SWITCHCLK_DBE_CMN_ADDR, SWITCHCLK_DBE_CMN_SRAM_FAST_SEL_NT_MASK, SWITCHCLK_DBE_CMN_SRAM_FAST_SEL_NT_OFFSET, 1));

    //m_reg = this.m_env.m_v_sqr.dbe_csr_model.get_reg_ref(99, "switchclk_dbe_cmn");
    //m_reg.read(status, rd_data);
    //m_reg.get_field_by_name("master_fast_sel_nt").set(1);
    CHECK(pmd_write_field(&mss, SWITCHCLK_DBE_CMN_ADDR, SWITCHCLK_DBE_CMN_MASTER_FAST_SEL_NT_MASK, SWITCHCLK_DBE_CMN_MASTER_FAST_SEL_NT_OFFSET, 1));

    //#1us;
    svSetScope(svGetScopeFromName("dbe_test_pkg"));
    delay_us(1);

    //m_reg = this.m_env.m_v_sqr.dbe_csr_model.get_reg_ref(99, "afe_cmn_clkgen_reg2");
    //m_reg.read(status, rd_data);
    //m_reg.get_field_by_name("pclkdiv_a").set('h40);
    CHECK(pmd_write_field(&mss, AFE_CMN_CLKGEN_REG2_ADDR, AFE_CMN_CLKGEN_REG2_PCLKDIV_A_MASK, AFE_CMN_CLKGEN_REG2_PCLKDIV_A_OFFSET, 0x40));

    //m_reg = this.m_env.m_v_sqr.dbe_csr_model.get_reg_ref(99, "afe_cmn_clkgen_reg2");
    //m_reg.read(status, rd_data);
    //m_reg.get_field_by_name("pclkdiv_ready_a").set(1);
    CHECK(pmd_write_field(&mss, AFE_CMN_CLKGEN_REG2_ADDR, AFE_CMN_CLKGEN_REG2_PCLKDIV_READY_A_MASK, AFE_CMN_CLKGEN_REG2_PCLKDIV_READY_A_OFFSET, 1));
    //i_reg.write_field(0, "switchclk_dbe_cmn[pclk_sel_nt]",1,REG_TIMEOUT_NS);
    //m_reg = this.m_env.m_v_sqr.dbe_csr_model.get_reg_ref(99, "switchclk_dbe_cmn");
    //m_reg.read(status, rd_data);
    //m_reg.get_field_by_name("pclk_sel_nt").set(1);
    CHECK(pmd_write_field(&mss, SWITCHCLK_DBE_CMN_ADDR, SWITCHCLK_DBE_CMN_PCLK_SEL_NT_MASK, SWITCHCLK_DBE_CMN_PCLK_SEL_NT_OFFSET, 1));


    pmd_write_field(&mss, LCPLL_CHECK_ADDR, LCPLL_CHECK_START_A_MASK, LCPLL_CHECK_START_A_OFFSET, 1);
    //#50us;
    svSetScope(svGetScopeFromName("dbe_test_pkg"));
    delay_us(20);

    return pass;
}
*/

int c_api_ate_pll_lock() {
    int pass = 0;

    svSetScope(svGetScopeFromName("serdes_env_pkg"));

    mss_access_t mss = {.phy_offset=0, .lane_offset=0};

    // Base test
    USR_PRINTF("Calling c_api_ate_base_test\n");
    pass += c_api_ate_base_test(&mss);


    // Check for LCPLL Lock
    USR_PRINTF("Calling aw_pmd_pll_lock_get\n");
    pmd_ate_vec_comment("Checking for LCPLL Lock");
    uint32_t pll_lock;
    if (s_fast_sim){
        pass += aw_pmd_pll_lock_get(&mss, &pll_lock,0,0);
    } else {
        pass += aw_pmd_pll_lock_get(&mss, &pll_lock,1,1);
    }
    if (pll_lock == 1) {
        USR_PRINTF("LC PLL locked!\n");
    } else {
        USR_PRINTF("ERROR: LC PLL not nocked!\n");
    }

    //TX Power up
    USR_PRINTF("Calling aw_pmd_iso_request_tx_state_change\n");
    pmd_ate_vec_comment("Configure TX rate/width/pstate and request TX state change");
    pass += aw_pmd_iso_request_tx_state_change(&mss, AW_P0, s_rate_preset, s_data_width, TX_ACK_TIMEOUT_US);

    //RX Power up
    USR_PRINTF("Calling aw_pmd_iso_request_rx_state_change\n");
    pmd_ate_vec_comment("Configure RX rate/width/pstate and request RX state change");
    pass += aw_pmd_iso_request_rx_state_change(&mss, AW_P0, s_rate_preset, s_data_width, RX_ACK_TIMEOUT_US);

    //TODO: Should this live in C test?
    svSetScope(svGetScopeFromName("serdes_env_pkg"));
    sv_print("c_api_base_test: Calling check_hs_ref_clk_and_rx_blk_clk task from the UVM test.");
    svSetScope(svGetScopeFromName("dbe_test_pkg"));
    sv_check_hs_ref_clk_and_rx_blk_clk(s_m_rnd_lane, 1, 200); // 1 us timeout and 200 clock cycle duration.

    //Calling sv clk checkers
    sv_print_clks(s_m_rnd_lane);
    sv_check_clks(s_m_rnd_lane, s_rate_preset, s_data_width);

    uint32_t cmn_pll_fine_code;
    uint32_t cmn_pll_coarse_code;
    uint32_t tx_pll_fine_code;
    uint32_t tx_pll_coarse_code;
    uint32_t rx_pll_fine_code;
    uint32_t rx_pll_coarse_code;
    double tx_vco, rx_vco;
    double tx_ppm, rx_ppm;
    uint32_t expected_txrx_center_fine_code;
    uint32_t expected_lcpll_center_fine_code;

    if (strcmp(s_rate_arr,"PCIE_GEN3") == 0) {
        expected_txrx_center_fine_code = 90;
    } else if (strcmp(s_rate_arr,"PCIE_GEN4") == 0) {
        expected_txrx_center_fine_code = 90;
    } else if (strcmp(s_rate_arr,"PCIE_GEN5") == 0) {
        expected_txrx_center_fine_code = 90;
    } else {
        expected_txrx_center_fine_code = 128;
    }

    if (s_fast_sim){
        expected_lcpll_center_fine_code = 512;
    } else {
        expected_lcpll_center_fine_code = 332;
    }

    USR_PRINTF("Expected center fine code = %d\n",expected_txrx_center_fine_code);

    //Grab CMN PLL fine/coarse code
    USR_PRINTF("Calling aw_pmd_cmn_pll_fine_code_get\n");
    pmd_ate_vec_comment("Check for CMN PLL fine and coarse code");
    pass += aw_pmd_cmn_pll_fine_code_get(&mss, &cmn_pll_fine_code,expected_lcpll_center_fine_code,50);
    USR_PRINTF("CMN PLL fine code = %d\n", cmn_pll_fine_code);

    USR_PRINTF("Calling aw_pmd_cmn_pll_coarse_code_get\n");
    pass += aw_pmd_cmn_pll_coarse_code_get(&mss, &cmn_pll_coarse_code,0,-1);
    USR_PRINTF("CMN PLL coarse code = %d\n",cmn_pll_coarse_code);

    //Grab TX PLL fine/coarse code
    USR_PRINTF("Calling aw_pmd_tx_pll_fine_code_get\n");
    pmd_ate_vec_comment("Check for TX PLL fine and coarse code");
    pass += aw_pmd_tx_pll_fine_code_get(&mss, &tx_pll_fine_code,expected_txrx_center_fine_code,50);
    USR_PRINTF("TX PLL fine code = %d\n", tx_pll_fine_code);

    USR_PRINTF("Calling aw_pmd_tx_pll_coarse_code_get\n");
    pass += aw_pmd_tx_pll_coarse_code_get(&mss, &tx_pll_coarse_code,0,-1);
    USR_PRINTF("TX PLL coarse code = %d\n",tx_pll_coarse_code);

    USR_PRINTF("Calling aw_pmd_tx_ppm_get\n");
    pass += aw_pmd_tx_ppm_get(&mss, 12, 100, &tx_ppm, &tx_vco, 830078125); // Not sure about this

    //Grab RX PLL fine/coarse code
    USR_PRINTF("Calling aw_pmd_rx_pll_fine_code_get\n");
    pmd_ate_vec_comment("Check for RX PLL fine and coarse code");
    pass += aw_pmd_rx_pll_fine_code_get(&mss, &rx_pll_fine_code,expected_txrx_center_fine_code,50);
    USR_PRINTF("RX PLL fine code  = %d\n", rx_pll_fine_code);

    USR_PRINTF("Calling aw_pmd_rx_pll_coarse_code_get\n");
    pass += aw_pmd_rx_pll_coarse_code_get(&mss, &rx_pll_coarse_code,0,-1);
    USR_PRINTF("RX PLL coarse code = %d\n",rx_pll_coarse_code);



    USR_PRINTF("Calling aw_pmd_rx_ppm_get\n");
    pass += aw_pmd_rx_ppm_get(&mss, 12, 100, &rx_ppm, &rx_vco, 830078125); // Not sure about this
    USR_PRINTF("TX PPM  = %f\n", tx_ppm);
    USR_PRINTF("RX PPM  = %f\n", rx_ppm);
    USR_PRINTF("RX VCO  = %f\n", rx_vco);

    // Look at the value of pass in the error table and give out the relevant error code using UVM error.
    svSetScope(svGetScopeFromName("serdes_env_pkg"));
    sv_error(pass);

    return pass;
}


int c_api_ate_txdetectrx_pass() {
    int pass = 0;

    svSetScope(svGetScopeFromName("serdes_env_pkg"));

    mss_access_t mss = {.phy_offset=0, .lane_offset=0};

    // Base test
    USR_PRINTF("Calling c_api_ate_base_test\n");
    pass += c_api_ate_base_test(&mss);


    //TX Power up
    USR_PRINTF("Calling aw_pmd_iso_request_tx_state_change\n");
    pmd_ate_vec_comment("Configure TX rate/width/pstate and request TX state change");
    pass += aw_pmd_iso_request_tx_state_change(&mss, AW_P1, s_rate_preset, s_data_width, TX_ACK_P1_TIMEOUT_US);

    //Requesting tx_rxdet. Expecting 1
    USR_PRINTF("Calling aw_pmd_tx_rxdet\n");
    pmd_ate_vec_comment("Request TXDETRX and expected receiver present");
    pass += aw_pmd_tx_rxdet(&mss,1,TX_RXDET_TIMEOUT_US);

    // Look at the value of pass in the error table and give out the relevant error code using UVM error.
    svSetScope(svGetScopeFromName("serdes_env_pkg"));
    sv_error(pass);

    return pass;
}

int c_api_ate_txdetectrx_fail() {
    int pass = 0;

    svSetScope(svGetScopeFromName("serdes_env_pkg"));

    mss_access_t mss = {.phy_offset=0, .lane_offset=0};

    // Base test
    USR_PRINTF("Calling c_api_ate_base_test\n");
    pass += c_api_ate_base_test(&mss);


    //TX Power up
    USR_PRINTF("Calling aw_pmd_iso_request_tx_state_change\n");
    pmd_ate_vec_comment("Configure TX rate/width/pstate and request TX state change");
    pass += aw_pmd_iso_request_tx_state_change(&mss, AW_P1, s_rate_preset, s_data_width, TX_ACK_P1_TIMEOUT_US);

    svSetScope(svGetScopeFromName("dbe_test_pkg"));
    sv_force_rx_present(0);

    //Requesting tx_rxdet. Expecting 0
    USR_PRINTF("Calling aw_pmd_tx_rxdet\n");
    pmd_ate_vec_comment("Request TXDETRX and expected receiver not present");
    pass += aw_pmd_tx_rxdet(&mss,0,TX_RXDET_TIMEOUT_US);

    // Look at the value of pass in the error table and give out the relevant error code using UVM error.
    svSetScope(svGetScopeFromName("serdes_env_pkg"));
    sv_error(pass);

    return pass;
}

int c_api_ate_beacon() {
    int pass = 0;

    svSetScope(svGetScopeFromName("serdes_env_pkg"));

    mss_access_t mss = {.phy_offset=0, .lane_offset=0};

        //Set broadcast mode - Note, only for ATE vector generation
    if (s_broadcast_en) {
        pmd_set_lane(&mss, 99);
    }

    // Base test
    USR_PRINTF("Calling c_api_ate_base_test\n");
    pass += c_api_ate_base_test(&mss);

    //TX Power up to P2
    USR_PRINTF("Calling aw_pmd_iso_request_tx_state_change\n");
    pmd_ate_vec_comment("Configure TX rate/width/pstate and request TX state change");
    pass += aw_pmd_iso_request_tx_state_change(&mss, AW_P2, s_rate_preset, s_data_width, TX_ACK_P2_TIMEOUT_US);

    //RX Power up to P2
    USR_PRINTF("Calling aw_pmd_iso_request_rx_state_change\n");
    pmd_ate_vec_comment("Configure RX rate/width/pstate and request RX state change");
    pass += aw_pmd_iso_request_rx_state_change(&mss, AW_P2, s_rate_preset, s_data_width, RX_ACK_P2_TIMEOUT_US);

    //Check for rx signal detect = 0
    USR_PRINTF("Calling aw_pmd_rx_signal_detect_check\n");
    pmd_ate_vec_comment("Check for rx signal detect = 0");
    pass += aw_pmd_rx_signal_detect_check(&mss, 0);

    //Enable TX Beacon
    USR_PRINTF("Calling aw_pmd_tx_beacon_en_set\n");
    pmd_ate_vec_comment("Enable TX Beacon");
    pass += aw_pmd_tx_beacon_en_set(&mss, 1);

    //Wait 5us
    svSetScope(svGetScopeFromName("dbe_test_pkg"));
    delay_us(5);

    //Check for rx signal detect = 1
    USR_PRINTF("Calling aw_pmd_rx_signal_detect_check\n");
    pmd_ate_vec_comment("Check for rx signal detect = 1");
    pass += aw_pmd_rx_signal_detect_check(&mss, 1);

    //Disable TX Beacon
    USR_PRINTF("Calling aw_pmd_tx_beacon_en_set\n");
    pmd_ate_vec_comment("Disable TX Beacon");
    pass += aw_pmd_tx_beacon_en_set(&mss, 0);

    //Wait 5us
    svSetScope(svGetScopeFromName("dbe_test_pkg"));
    delay_us(5);

    //Check for rx signal detect = 0
    USR_PRINTF("Calling aw_pmd_rx_signal_detect_check\n");
    pmd_ate_vec_comment("Check for rx signal detect = 0");
    pass += aw_pmd_rx_signal_detect_check(&mss, 0);

    // Look at the value of pass in the error table and give out the relevant error code using UVM error.
    svSetScope(svGetScopeFromName("serdes_env_pkg"));
    sv_error(pass);

    return pass;
}

int c_api_ate_pmon() {
    int pass = 0;
    uint32_t pvt_measure_result;
    char str [50];
    svSetScope(svGetScopeFromName("serdes_env_pkg"));

    mss_access_t mss = {.phy_offset=0, .lane_offset=0};
     //Set broadcast mode - Note, only for ATE vector generation
    if (s_broadcast_en) {
        pmd_set_lane(&mss, 99);
    }

    // Base test
    USR_PRINTF("Calling c_api_ate_base_test\n");
    pass += c_api_ate_base_test(&mss);


    //TX Power up to P2
    //USR_PRINTF("Calling aw_pmd_iso_request_tx_state_change\n");
    //pmd_ate_vec_comment("Configure TX rate/width/pstate and request TX state change");
    //pass += aw_pmd_iso_request_tx_state_change(&mss, AW_P2, s_rate_preset, s_data_width, TX_ACK_TIMEOUT_US);

    for (int i=0; i< 8; i++) {
        USR_PRINTF("Calling c_api_ate_base_test with pmon_sel = %d\n",i);
        sprintf(str, "Collecting data for pmon_sel=%d",i);
        pmd_ate_vec_comment(str);
        pass += aw_pmd_measure_pmon(&mss, i, 8, 100, &pvt_measure_result);
    }

    // Look at the value of pass in the error table and give out the relevant error code using UVM error.
    svSetScope(svGetScopeFromName("serdes_env_pkg"));
    sv_error(pass);

    return pass;
}

int c_api_ate_reg_status() {
    int pass = 0;

    svSetScope(svGetScopeFromName("serdes_env_pkg"));

    mss_access_t mss = {.phy_offset=0, .lane_offset=0};

    sv_print("c_api_base_test: Calling wait_apb_preset_done task from the UVM test.");
    svSetScope(svGetScopeFromName("dbe_test_pkg"));
    sv_wait_apb_preset_done();

    USR_PRINTF("Calling aw_pmd_read_status\n");
    pass += aw_pmd_read_status(&mss,0);

    // Look at the value of pass in the error table and give out the relevant error code using UVM error.
    svSetScope(svGetScopeFromName("serdes_env_pkg"));
    sv_error(pass);

    return pass;
}

int c_api_ate_iddq() {
    int pass = 0;

    svSetScope(svGetScopeFromName("serdes_env_pkg"));

    mss_access_t mss = {.phy_offset=0, .lane_offset=0};

    //Set broadcast mode - Note, only for ATE vector generation
    if (s_broadcast_en) {
        pmd_set_lane(&mss, 99);
    }

    // Base test
    USR_PRINTF("Calling c_api_ate_base_test\n");
    pass += c_api_ate_base_test(&mss);

    //TX Power up to PD (lowest power state)
    USR_PRINTF("Calling aw_pmd_iso_request_tx_state_change\n");
    pmd_ate_vec_comment("Put TX in PD pstate");
    pass += aw_pmd_iso_request_tx_state_change(&mss, AW_PD, s_rate_preset, s_data_width, TX_ACK_TIMEOUT_US);

    //RX Power up to PD (lowest power state)
    USR_PRINTF("Calling aw_pmd_iso_request_rx_state_change\n");
    pmd_ate_vec_comment("Put RX in PD pstate");
    pass += aw_pmd_iso_request_rx_state_change(&mss, AW_PD, s_rate_preset, s_data_width, RX_ACK_TIMEOUT_US);

    // Look at the value of pass in the error table and give out the relevant error code using UVM error.
    svSetScope(svGetScopeFromName("serdes_env_pkg"));
    sv_error(pass);

    return pass;
}

int c_api_ate_powerdown() {
    int pass = 0;

    svSetScope(svGetScopeFromName("serdes_env_pkg"));

    mss_access_t mss = {.phy_offset=0, .lane_offset=0};

    //Set broadcast mode - Note, only for ATE vector generation
    if (s_broadcast_en) {
        pmd_set_lane(&mss, 99);
    }

    // Base test
    USR_PRINTF("Calling c_api_ate_base_test\n");
    pass += c_api_ate_base_test(&mss);


    pmd_ate_vec_comment("Powerdown sequence start");

    // uint32_t tx_block_valid;
    // uint32_t rx_block_valid;
    uint32_t lcpll_lock_check_done;

    pmd_ate_vec_comment("Applying lane reset");
    pass += aw_pmd_iso_tx_reset_set(&mss, 0);//Need to check if this is reqd
    pass += aw_pmd_iso_rx_reset_set(&mss, 0);//Need to check if this is reqd
    USR_SLEEP(100);

     //TX Power up to PD (lowest power state)
    USR_PRINTF("Calling aw_pmd_iso_request_tx_state_change\n");
    pmd_ate_vec_comment("Put TX in PD pstate");
    pass += aw_pmd_iso_request_tx_state_change(&mss, AW_PD, s_rate_preset, s_data_width, TX_ACK_TIMEOUT_US);

    //RX Power up to PD (lowest power state)
    USR_PRINTF("Calling aw_pmd_iso_request_rx_state_change\n");
    pmd_ate_vec_comment("Put RX in PD pstate");
    pass += aw_pmd_iso_request_rx_state_change(&mss, AW_PD, s_rate_preset, s_data_width, RX_ACK_TIMEOUT_US);

    pmd_ate_vec_comment("Check for de-assert of TX/RX state change");
    pass += aw_pmd_analog_loopback_set(&mss,0);
    pmd_ate_vec_comment("Disable TX BIST");
    pass += aw_pmd_gen_tx_en_set(&mss,0);
    pmd_ate_vec_comment("Disable RX BIST");
    pass += aw_pmd_rx_chk_en_set(&mss,0);
    pmd_ate_vec_comment("Resetting LCPLL Lock check");
    CHECK(pmd_write_field(&mss, LCPLL_CHECK_ADDR, LCPLL_CHECK_START_A_MASK, LCPLL_CHECK_START_A_OFFSET, 0));
    USR_SLEEP(50);
    pmd_ate_vec_comment("Check for LCPLL Lock check done de-assertion");

    CHECK(pmd_read_check_field(&mss, LCPLL_CHECK_RDREG_ADDR, LCPLL_CHECK_RDREG_DONE_A_MASK, LCPLL_CHECK_RDREG_DONE_A_OFFSET, RD_EQ, &lcpll_lock_check_done, 0, 0 /*NULL*/));
    pmd_ate_vec_comment("Powerdown sequence end");

    // Look at the value of pass in the error table and give out the relevant error code using UVM error.
    svSetScope(svGetScopeFromName("serdes_env_pkg"));
    sv_error(pass);

    return pass;
}

int c_api_ate_pup_cmn() {
    int pass = 0;

    svSetScope(svGetScopeFromName("serdes_env_pkg"));

    mss_access_t mss = {.phy_offset=0, .lane_offset=0};

    //Set broadcast mode - Note, only for ATE vector generation
    if (s_broadcast_en) {
        pmd_set_lane(&mss, 99);
    }

    // Base test
    USR_PRINTF("Calling c_api_ate_base_test\n");
    pass += c_api_ate_base_test(&mss);

    // Look at the value of pass in the error table and give out the relevant error code using UVM error.
    svSetScope(svGetScopeFromName("serdes_env_pkg"));
    sv_error(pass);

    return pass;
}

int c_api_ate_reg_rw_basic() {
    int pass = 0;
    uint32_t read_value;
    svSetScope(svGetScopeFromName("serdes_env_pkg"));

    mss_access_t mss = {.phy_offset=0, .lane_offset=0};

    USR_PRINTF("Writing CMN register. Inverted default.\n");
    CHECK(pmd_write_field(&mss, AFE_CMN_LCPLL_OSC_REG4_ADDR, AFE_CMN_LCPLL_OSC_REG4_ACCUM_BYPASS_VALUE_NT_MASK, AFE_CMN_LCPLL_OSC_REG4_ACCUM_BYPASS_VALUE_NT_OFFSET, 0xFFFFFFFF));
    USR_PRINTF("Checking CMN register. Inverted default.\n");
    CHECK(pmd_read_check_field(&mss, AFE_CMN_LCPLL_OSC_REG4_ADDR, AFE_CMN_LCPLL_OSC_REG4_ACCUM_BYPASS_VALUE_NT_MASK, AFE_CMN_LCPLL_OSC_REG4_ACCUM_BYPASS_VALUE_NT_OFFSET, RD_EQ, &read_value, 0xFFFFFFFF, 0));
    CHECK(pmd_write_field(&mss, AFE_CMN_LCPLL_OSC_REG4_ADDR, AFE_CMN_LCPLL_OSC_REG4_ACCUM_BYPASS_VALUE_NT_MASK, AFE_CMN_LCPLL_OSC_REG4_ACCUM_BYPASS_VALUE_NT_OFFSET, 0x00000000));
    USR_PRINTF("Writing CMN register.default.\n");
    CHECK(pmd_read_check_field(&mss, AFE_CMN_LCPLL_OSC_REG4_ADDR, AFE_CMN_LCPLL_OSC_REG4_ACCUM_BYPASS_VALUE_NT_MASK, AFE_CMN_LCPLL_OSC_REG4_ACCUM_BYPASS_VALUE_NT_OFFSET, RD_EQ, &read_value, 0x00000000, 0));
    USR_PRINTF("Checking CMN registger. default\n");

    for (int i = 0; i <= LANE_MAX; i++) {
        pmd_set_lane(&mss, i);
        USR_PRINTF("Writing LANE %d register. Inverted default.\n",i);
        CHECK(pmd_write_field(&mss, RX_DATABIST_TOP_REG15_ADDR, RX_DATABIST_TOP_REG15_ENABLE_BITS_31_0_NT_MASK, RX_DATABIST_TOP_REG15_ENABLE_BITS_31_0_NT_OFFSET, 0x00000000));
        USR_PRINTF("Checking LANE %d register. Inverted default.\n",i);
        CHECK(pmd_read_check_field(&mss, RX_DATABIST_TOP_REG15_ADDR, RX_DATABIST_TOP_REG15_ENABLE_BITS_31_0_NT_MASK, RX_DATABIST_TOP_REG15_ENABLE_BITS_31_0_NT_OFFSET, RD_EQ, &read_value, 0x00000000, 0));
        CHECK(pmd_write_field(&mss, RX_DATABIST_TOP_REG15_ADDR, RX_DATABIST_TOP_REG15_ENABLE_BITS_31_0_NT_MASK, RX_DATABIST_TOP_REG15_ENABLE_BITS_31_0_NT_OFFSET, 0xFFFFFFFF));
        USR_PRINTF("Writing LANE %d register.default.\n",i);
        CHECK(pmd_read_check_field(&mss, RX_DATABIST_TOP_REG15_ADDR, RX_DATABIST_TOP_REG15_ENABLE_BITS_31_0_NT_MASK, RX_DATABIST_TOP_REG15_ENABLE_BITS_31_0_NT_OFFSET, RD_EQ, &read_value, 0xFFFFFFFF, 0));
        USR_PRINTF("Checking LANE %d registger. default\n",i);
    }

    // Look at the value of pass in the error table and give out the relevant error code using UVM error.
    svSetScope(svGetScopeFromName("serdes_env_pkg"));
    sv_error(pass);

    return pass;
}

int c_api_ate_atest_adc_sweep() {
    int pass = 0;

    svSetScope(svGetScopeFromName("serdes_env_pkg"));

    mss_access_t mss = {.phy_offset=0, .lane_offset=0};

    //Set broadcast mode - Note, only for ATE vector generation
    if (s_broadcast_en) {
        pmd_set_lane(&mss, 99);
    }
    // Base test
    USR_PRINTF("Calling c_api_ate_base_test\n");
    pass += c_api_ate_base_test(&mss);

    USR_PRINTF("Enable ATEST ADC\n");
    pmd_ate_vec_comment("Enable ATEST ADC");
    pass += aw_pmd_atest_en(&mss, 1);

    //ATEST ADC termination
    uint32_t s_cmn_atest_term[32] = { 0 };
    uint32_t s_tx_atest_term[32] = { 0 };
    uint32_t s_rx_atest_a_term[32] = { 0 };
    uint32_t s_rx_atest_b_term[32] = { 0 };
    char * s_cmn_atest_block_name[32];
    char * s_tx_atest_block_name[32];
    char * s_rx_atest_a_block_name[32];
    char * s_rx_atest_b_block_name[32];
    char * s_cmn_atest_signal_name[32];
    char * s_tx_atest_signal_name[32];
    char * s_rx_atest_a_signal_name[32];
    char * s_rx_atest_b_signal_name[32];

    //PROJ SPECIFIC
    //ATEST ADC termination info: pulled from sweep_atest.csv
    s_cmn_atest_term [0 ] = 0;    s_cmn_atest_block_name [0 ] = "N/A                   ";  s_cmn_atest_signal_name [0 ] = "N/A                     ";
    s_cmn_atest_term [1 ] = 0;    s_cmn_atest_block_name [1 ] = "REGULATOR_HSREFBUF    ";  s_cmn_atest_signal_name [1 ] = "vref                    ";
    s_cmn_atest_term [2 ] = 0;    s_cmn_atest_block_name [2 ] = "REGULATOR_HSREFBUF    ";  s_cmn_atest_signal_name [2 ] = "vboost                  ";
    s_cmn_atest_term [3 ] = 0;    s_cmn_atest_block_name [3 ] = "REGULATOR_HSREFBUF    ";  s_cmn_atest_signal_name [3 ] = "vcharge                 ";
    s_cmn_atest_term [4 ] = 0;    s_cmn_atest_block_name [4 ] = "REGULATOR_HSREFBUF    ";  s_cmn_atest_signal_name [4 ] = "vddreg                  ";
    s_cmn_atest_term [5 ] = 3;    s_cmn_atest_block_name [5 ] = "REGULATOR_HSREFBUF    ";  s_cmn_atest_signal_name [5 ] = "vddh                    ";
    s_cmn_atest_term [6 ] = 4;    s_cmn_atest_block_name [6 ] = "REGULATOR_HSREFBUF    ";  s_cmn_atest_signal_name [6 ] = "vss                     ";
    s_cmn_atest_term [7 ] = 0;    s_cmn_atest_block_name [7 ] = "REGULATOR_LSREFBUF    ";  s_cmn_atest_signal_name [7 ] = "vref                    ";
    s_cmn_atest_term [8 ] = 0;    s_cmn_atest_block_name [8 ] = "REGULATOR_LSREFBUF    ";  s_cmn_atest_signal_name [8 ] = "vboost                  ";
    s_cmn_atest_term [9 ] = 0;    s_cmn_atest_block_name [9 ] = "REGULATOR_LSREFBUF    ";  s_cmn_atest_signal_name [9 ] = "vcharge                 ";
    s_cmn_atest_term [10] = 0;    s_cmn_atest_block_name [10] = "REGULATOR_LSREFBUF    ";  s_cmn_atest_signal_name [10] = "vddreg                  ";
    s_cmn_atest_term [11] = 0;    s_cmn_atest_block_name [11] = "LEVELSHIFTER          ";  s_cmn_atest_signal_name [11] = "p_vdd                   ";
    s_cmn_atest_term [12] = 4;    s_cmn_atest_block_name [12] = "LEVELSHIFTER          ";  s_cmn_atest_signal_name [12] = "p_vss                   ";
    s_cmn_atest_term [13] = 3;    s_cmn_atest_block_name [13] = "BANDGAP               ";  s_cmn_atest_signal_name [13] = "vddh                    ";
    s_cmn_atest_term [14] = 0;    s_cmn_atest_block_name [14] = "BANDGAP               ";  s_cmn_atest_signal_name [14] = "Vc                      ";
    s_cmn_atest_term [15] = 0;    s_cmn_atest_block_name [15] = "BANDGAP               ";  s_cmn_atest_signal_name [15] = "Vb                      ";
    s_cmn_atest_term [16] = 0;    s_cmn_atest_block_name [16] = "BANDGAP               ";  s_cmn_atest_signal_name [16] = "Vref2                   ";
    s_cmn_atest_term [17] = 0;    s_cmn_atest_block_name [17] = "BANDGAP               ";  s_cmn_atest_signal_name [17] = "Vref                    ";
    s_cmn_atest_term [18] = 4;    s_cmn_atest_block_name [18] = "BANDGAP               ";  s_cmn_atest_signal_name [18] = "vss                     ";
    s_cmn_atest_term [19] = 0;    s_cmn_atest_block_name [19] = "Clkgen(regulator)     ";  s_cmn_atest_signal_name [19] = "vref                    ";
    s_cmn_atest_term [20] = 0;    s_cmn_atest_block_name [20] = "Clkgen(regulator)     ";  s_cmn_atest_signal_name [20] = "vboost                  ";
    s_cmn_atest_term [21] = 0;    s_cmn_atest_block_name [21] = "Clkgen(regulator)     ";  s_cmn_atest_signal_name [21] = "vcharge                 ";
    s_cmn_atest_term [22] = 0;    s_cmn_atest_block_name [22] = "Clkgen(regulator)     ";  s_cmn_atest_signal_name [22] = "vddreg                  ";
    s_cmn_atest_term [23] = 0;    s_cmn_atest_block_name [23] = "Clkgen                ";  s_cmn_atest_signal_name [23] = "wa_vb1                  ";
    s_cmn_atest_term [24] = 0;    s_cmn_atest_block_name [24] = "Clkgen                ";  s_cmn_atest_signal_name [24] = "oa_choke                ";
    s_cmn_atest_term [25] = 0;    s_cmn_atest_block_name [25] = "Termcal               ";  s_cmn_atest_signal_name [25] = "Ib                      ";
    s_cmn_atest_term [26] = 0;    s_cmn_atest_block_name [26] = "Termcal               ";  s_cmn_atest_signal_name [26] = "Vb1                     ";
    s_cmn_atest_term [27] = 0;    s_cmn_atest_block_name [27] = "Lsrefbuf              ";  s_cmn_atest_signal_name [27] = "Vb1                     ";
    s_cmn_atest_term [28] = 0;    s_cmn_atest_block_name [28] = "Lsrefbuf              ";  s_cmn_atest_signal_name [28] = "Vb2                     ";
    s_cmn_atest_term [29] = 0;    s_cmn_atest_block_name [29] = "Hsrefbuf              ";  s_cmn_atest_signal_name [29] = "Vb1                     ";
    s_cmn_atest_term [30] = 0;    s_cmn_atest_block_name [30] = "Hsrefbuf              ";  s_cmn_atest_signal_name [30] = "Vb2                     ";
    s_cmn_atest_term [31] = 0;    s_cmn_atest_block_name [31] = "Not Connected         ";  s_cmn_atest_signal_name [31] = "N/A                     ";
    s_tx_atest_term  [0 ] = 0;    s_tx_atest_block_name  [0 ] = "N/A                   ";  s_tx_atest_signal_name  [0 ] = "N/A                     ";
    s_tx_atest_term  [1 ] = 0;    s_tx_atest_block_name  [1 ] = "afe_tx_datapath       ";  s_tx_atest_signal_name  [1 ] = "pa_vddl                 ";
    s_tx_atest_term  [2 ] = 0;    s_tx_atest_block_name  [2 ] = "afe_tx_driver_bias    ";  s_tx_atest_signal_name  [2 ] = "wa_vgp_replica_lsb      ";
    s_tx_atest_term  [3 ] = 0;    s_tx_atest_block_name  [3 ] = "afe_tx_driver_bias    ";  s_tx_atest_signal_name  [3 ] = "wa_vgp_msb              ";
    s_tx_atest_term  [4 ] = 3;    s_tx_atest_block_name  [4 ] = "afe_tx_driver_bias    ";  s_tx_atest_signal_name  [4 ] = "pa_vddh_gated           ";
    s_tx_atest_term  [5 ] = 0;    s_tx_atest_block_name  [5 ] = "afe_tx_driver_bias    ";  s_tx_atest_signal_name  [5 ] = "wa_vgp_ovr_lsb          ";
    s_tx_atest_term  [6 ] = 0;    s_tx_atest_block_name  [6 ] = "afe_tx_driver_bias    ";  s_tx_atest_signal_name  [6 ] = "wa_11_full_p            ";
    s_tx_atest_term  [7 ] = 0;    s_tx_atest_block_name  [7 ] = "afe_tx_driver_bias    ";  s_tx_atest_signal_name  [7 ] = "wa_10_full_m            ";
    s_tx_atest_term  [8 ] = 0;    s_tx_atest_block_name  [8 ] = "afe_tx_driver_bias    ";  s_tx_atest_signal_name  [8 ] = "wa_10_part_m            ";
    s_tx_atest_term  [9 ] = 0;    s_tx_atest_block_name  [9 ] = "afe_tx_driver_bias    ";  s_tx_atest_signal_name  [9 ] = "wa_10_vcm               ";
    s_tx_atest_term  [10] = 0;    s_tx_atest_block_name  [10] = "afe_tx_driver_bias    ";  s_tx_atest_signal_name  [10] = "wa_10_part_p            ";
    s_tx_atest_term  [11] = 0;    s_tx_atest_block_name  [11] = "afe_tx_driver_bias    ";  s_tx_atest_signal_name  [11] = "wa_10_full_p            ";
    s_tx_atest_term  [12] = 0;    s_tx_atest_block_name  [12] = "afe_tx_driver_bias    ";  s_tx_atest_signal_name  [12] = "wa_11_full_m            ";
    s_tx_atest_term  [13] = 0;    s_tx_atest_block_name  [13] = "afe_tx_driver_bias    ";  s_tx_atest_signal_name  [13] = "wa_11_part_m            ";
    s_tx_atest_term  [14] = 0;    s_tx_atest_block_name  [14] = "afe_tx_driver_bias    ";  s_tx_atest_signal_name  [14] = "wa_11_vcm               ";
    s_tx_atest_term  [15] = 0;    s_tx_atest_block_name  [15] = "afe_tx_driver_bias    ";  s_tx_atest_signal_name  [15] = "wa_11_part_p            ";
    s_tx_atest_term  [16] = 0;    s_tx_atest_block_name  [16] = "dco_reg               ";  s_tx_atest_signal_name  [16] = "vref                    ";
    s_tx_atest_term  [17] = 0;    s_tx_atest_block_name  [17] = "dco_reg               ";  s_tx_atest_signal_name  [17] = "vop_out                 ";
    s_tx_atest_term  [18] = 0;    s_tx_atest_block_name  [18] = "dco_reg               ";  s_tx_atest_signal_name  [18] = "vcharge                 ";
    s_tx_atest_term  [19] = 0;    s_tx_atest_block_name  [19] = "dco_reg               ";  s_tx_atest_signal_name  [19] = "vddreg                  ";
    s_tx_atest_term  [20] = 0;    s_tx_atest_block_name  [20] = "dco_vcobias           ";  s_tx_atest_signal_name  [20] = "ioa_osc                 ";
    s_tx_atest_term  [21] = 0;    s_tx_atest_block_name  [21] = "bitclk regulator      ";  s_tx_atest_signal_name  [21] = "vop_out                 ";
    s_tx_atest_term  [22] = 0;    s_tx_atest_block_name  [22] = "bitclk regulator      ";  s_tx_atest_signal_name  [22] = "vcharge                 ";
    s_tx_atest_term  [23] = 0;    s_tx_atest_block_name  [23] = "bitclk regulator      ";  s_tx_atest_signal_name  [23] = "vddreg                  ";
    s_tx_atest_term  [24] = 3;    s_tx_atest_block_name  [24] = "bitclk regulator      ";  s_tx_atest_signal_name  [24] = "vddh                    ";
    s_tx_atest_term  [25] = 4;    s_tx_atest_block_name  [25] = "bitclk regulator      ";  s_tx_atest_signal_name  [25] = "vss                     ";
    s_tx_atest_term  [26] = 0;    s_tx_atest_block_name  [26] = "hsref regulator       ";  s_tx_atest_signal_name  [26] = "vref                    ";
    s_tx_atest_term  [27] = 0;    s_tx_atest_block_name  [27] = "hsref regulator       ";  s_tx_atest_signal_name  [27] = "vop_out                 ";
    s_tx_atest_term  [28] = 0;    s_tx_atest_block_name  [28] = "hsref regulator       ";  s_tx_atest_signal_name  [28] = "vcharge                 ";
    s_tx_atest_term  [29] = 0;    s_tx_atest_block_name  [29] = "hsref regulator       ";  s_tx_atest_signal_name  [29] = "vddreg                  ";
    s_tx_atest_term  [30] = 3;    s_tx_atest_block_name  [30] = "hsref regulator       ";  s_tx_atest_signal_name  [30] = "vddh                    ";
    s_tx_atest_term  [31] = 4;    s_tx_atest_block_name  [31] = "hsref regulator       ";  s_tx_atest_signal_name  [31] = "vss                     ";
    s_rx_atest_a_term[0 ] = 0;    s_rx_atest_a_block_name[0 ] = "N/A                   ";  s_rx_atest_a_signal_name[0 ] = "N/A                     ";
    s_rx_atest_a_term[1 ] = 0;    s_rx_atest_a_block_name[1 ] = "afe_rx_tiadc_b1       ";  s_rx_atest_a_signal_name[1 ] = "oa_b1_sfbias_200u<0>    ";
    s_rx_atest_a_term[2 ] = 0;    s_rx_atest_a_block_name[2 ] = "afe_rx_tiadc_b1       ";  s_rx_atest_a_signal_name[2 ] = "pa_vddreg               ";
    s_rx_atest_a_term[3 ] = 3;    s_rx_atest_a_block_name[3 ] = "afe_rx_tiadc_b2       ";  s_rx_atest_a_signal_name[3 ] = "pa_vddh                 ";
    s_rx_atest_a_term[4 ] = 4;    s_rx_atest_a_block_name[4 ] = "afe_rx_tiadc_b2       ";  s_rx_atest_a_signal_name[4 ] = "pa_vss                  ";
    s_rx_atest_a_term[5 ] = 0;    s_rx_atest_a_block_name[5 ] = "afe_rx_tiadc_b2       ";  s_rx_atest_a_signal_name[5 ] = "ia_b2_bias_100u         ";
    s_rx_atest_a_term[6 ] = 0;    s_rx_atest_a_block_name[6 ] = "afe_rx_tiadc_b3       ";  s_rx_atest_a_signal_name[6 ] = "ia_ibias_b3_100u        ";
    s_rx_atest_a_term[7 ] = 0;    s_rx_atest_a_block_name[7 ] = "afe_rx_tiadc_b3       ";  s_rx_atest_a_signal_name[7 ] = "pa_vddl                 ";
    s_rx_atest_a_term[8 ] = 0;    s_rx_atest_a_block_name[8 ] = "afe_rx_tiadc_adc      ";  s_rx_atest_a_signal_name[8 ] = "ia_adc_bias_50u         ";
    s_rx_atest_a_term[9 ] = 4;    s_rx_atest_a_block_name[9 ] = "afe_rx_tiadc_adc      ";  s_rx_atest_a_signal_name[9 ] = "pa_vss                  ";
    s_rx_atest_a_term[10] = 0;    s_rx_atest_a_block_name[10] = "afe_rx_tiadc_adc      ";  s_rx_atest_a_signal_name[10] = "ioa_adc_ref_cmn         ";
    s_rx_atest_a_term[11] = 0;    s_rx_atest_a_block_name[11] = "afe_rx_tiadc_adc      ";  s_rx_atest_a_signal_name[11] = "ia_adc_bias_50u         ";
    s_rx_atest_a_term[12] = 4;    s_rx_atest_a_block_name[12] = "afe_rx_tiadc_adc      ";  s_rx_atest_a_signal_name[12] = "pa_vss                  ";
    s_rx_atest_a_term[13] = 0;    s_rx_atest_a_block_name[13] = "afe_rx_tiadc_adc      ";  s_rx_atest_a_signal_name[13] = "ioa_adc_ref_cmn         ";
    s_rx_atest_a_term[14] = 0;    s_rx_atest_a_block_name[14] = "afe_rx_tiadc_adc      ";  s_rx_atest_a_signal_name[14] = "ia_adc_bias_50u         ";
    s_rx_atest_a_term[15] = 4;    s_rx_atest_a_block_name[15] = "afe_rx_tiadc_adc      ";  s_rx_atest_a_signal_name[15] = "pa_vss                  ";
    s_rx_atest_a_term[16] = 0;    s_rx_atest_a_block_name[16] = "afe_rx_tiadc_adc      ";  s_rx_atest_a_signal_name[16] = "ioa_adc_ref_cmn         ";
    s_rx_atest_a_term[17] = 0;    s_rx_atest_a_block_name[17] = "afe_rx_tiadc_b1       ";  s_rx_atest_a_signal_name[17] = "oa_vdrv                 ";
    s_rx_atest_a_term[18] = 0;    s_rx_atest_a_block_name[18] = "afe_rx_ctle_vga_ana_to";  s_rx_atest_a_signal_name[18] = "oa_afe_rx_ctle_vga_m_hv ";
    s_rx_atest_a_term[19] = 0;    s_rx_atest_a_block_name[19] = "afe_rx_ctle_vga_ana_to";  s_rx_atest_a_signal_name[19] = "oa_afe_rx_ctle_vga_p_hv ";
    s_rx_atest_a_term[20] = 1;    s_rx_atest_a_block_name[20] = "afe_rx_ctle_vga_ana_to";  s_rx_atest_a_signal_name[20] = "wa_atest_bias_20u       ";
    s_rx_atest_a_term[21] = 2;    s_rx_atest_a_block_name[21] = "afe_rx_sigdet_ana_top ";  s_rx_atest_a_signal_name[21] = "wa_itst_10u_p           ";
    s_rx_atest_a_term[22] = 0;    s_rx_atest_a_block_name[22] = "afe_rx_sigdet_ana_top ";  s_rx_atest_a_signal_name[22] = "wa_afe_rx_pkdet_ref     ";
    s_rx_atest_a_term[23] = 0;    s_rx_atest_a_block_name[23] = "afe_rx_sigdet_ana_top ";  s_rx_atest_a_signal_name[23] = "wa_afe_rx_ref_p         ";
    s_rx_atest_a_term[24] = 0;    s_rx_atest_a_block_name[24] = "afe_rx_bscan          ";  s_rx_atest_a_signal_name[24] = "wa_itst_12p5u           ";
    s_rx_atest_a_term[25] = 0;    s_rx_atest_a_block_name[25] = "afe_rx_bscan          ";  s_rx_atest_a_signal_name[25] = "wa_vbias_a              ";
    s_rx_atest_a_term[26] = 0;    s_rx_atest_a_block_name[26] = "afe_rx_bscan          ";  s_rx_atest_a_signal_name[26] = "wa_hyst_p               ";
    s_rx_atest_a_term[27] = 0;    s_rx_atest_a_block_name[27] = "afe_regulator_erroramp";  s_rx_atest_a_signal_name[27] = "ia_erramp_n             ";
    s_rx_atest_a_term[28] = 0;    s_rx_atest_a_block_name[28] = "afe_regulator_battery ";  s_rx_atest_a_signal_name[28] = "ia_opamp                ";
    s_rx_atest_a_term[29] = 0;    s_rx_atest_a_block_name[29] = "afe_regulator_battery ";  s_rx_atest_a_signal_name[29] = "wa_vcharge_buff         ";
    s_rx_atest_a_term[30] = 0;    s_rx_atest_a_block_name[30] = "afe_regulator_core    ";  s_rx_atest_a_signal_name[30] = "pa_vddreg               ";
    s_rx_atest_a_term[31] = 3;    s_rx_atest_a_block_name[31] = "afe_regulator_core    ";  s_rx_atest_a_signal_name[31] = "pa_vddh                 ";
    s_rx_atest_b_term[0 ] = 0;    s_rx_atest_b_block_name[0 ] = "N/A                   ";  s_rx_atest_b_signal_name[0 ] = "N/A                     ";
    s_rx_atest_b_term[1 ] = 4;    s_rx_atest_b_block_name[1 ] = "afe_regulator_core    ";  s_rx_atest_b_signal_name[1 ] = "pa_vss                  ";
    s_rx_atest_b_term[2 ] = 0;    s_rx_atest_b_block_name[2 ] = "afe_regulator_erroramp";  s_rx_atest_b_signal_name[2 ] = "ia_erramp_n             ";
    s_rx_atest_b_term[3 ] = 0;    s_rx_atest_b_block_name[3 ] = "afe_regulator_battery ";  s_rx_atest_b_signal_name[3 ] = "ia_opamp                ";
    s_rx_atest_b_term[4 ] = 0;    s_rx_atest_b_block_name[4 ] = "afe_regulator_battery ";  s_rx_atest_b_signal_name[4 ] = "wa_vcharge_buff         ";
    s_rx_atest_b_term[5 ] = 0;    s_rx_atest_b_block_name[5 ] = "afe_regulator_core    ";  s_rx_atest_b_signal_name[5 ] = "pa_vddreg               ";
    s_rx_atest_b_term[6 ] = 3;    s_rx_atest_b_block_name[6 ] = "afe_regulator_core    ";  s_rx_atest_b_signal_name[6 ] = "pa_vddh                 ";
    s_rx_atest_b_term[7 ] = 4;    s_rx_atest_b_block_name[7 ] = "afe_regulator_core    ";  s_rx_atest_b_signal_name[7 ] = "pa_vss                  ";
    s_rx_atest_b_term[8 ] = 0;    s_rx_atest_b_block_name[8 ] = "afe_regulator_erroramp";  s_rx_atest_b_signal_name[8 ] = "ia_erramp_n             ";
    s_rx_atest_b_term[9 ] = 0;    s_rx_atest_b_block_name[9 ] = "afe_regulator_battery ";  s_rx_atest_b_signal_name[9 ] = "ia_opamp                ";
    s_rx_atest_b_term[10] = 0;    s_rx_atest_b_block_name[10] = "afe_regulator_battery ";  s_rx_atest_b_signal_name[10] = "wa_vcharge_buff         ";
    s_rx_atest_b_term[11] = 0;    s_rx_atest_b_block_name[11] = "afe_regulator_core    ";  s_rx_atest_b_signal_name[11] = "pa_vddreg               ";
    s_rx_atest_b_term[12] = 0;    s_rx_atest_b_block_name[12] = "afe_direct_synth_dco  ";  s_rx_atest_b_signal_name[12] = "ioa_osc                 ";
    s_rx_atest_b_term[13] = 0;    s_rx_atest_b_block_name[13] = "Not Connected         ";  s_rx_atest_b_signal_name[13] = "N/A                     ";
    s_rx_atest_b_term[14] = 0;    s_rx_atest_b_block_name[14] = "Not Connected         ";  s_rx_atest_b_signal_name[14] = "N/A                     ";
    s_rx_atest_b_term[15] = 0;    s_rx_atest_b_block_name[15] = "Not Connected         ";  s_rx_atest_b_signal_name[15] = "N/A                     ";
    s_rx_atest_b_term[16] = 0;    s_rx_atest_b_block_name[16] = "Not Connected         ";  s_rx_atest_b_signal_name[16] = "N/A                     ";
    s_rx_atest_b_term[17] = 0;    s_rx_atest_b_block_name[17] = "Not Connected         ";  s_rx_atest_b_signal_name[17] = "N/A                     ";
    s_rx_atest_b_term[18] = 0;    s_rx_atest_b_block_name[18] = "Not Connected         ";  s_rx_atest_b_signal_name[18] = "N/A                     ";
    s_rx_atest_b_term[19] = 0;    s_rx_atest_b_block_name[19] = "Not Connected         ";  s_rx_atest_b_signal_name[19] = "N/A                     ";
    s_rx_atest_b_term[20] = 0;    s_rx_atest_b_block_name[20] = "Not Connected         ";  s_rx_atest_b_signal_name[20] = "N/A                     ";
    s_rx_atest_b_term[21] = 0;    s_rx_atest_b_block_name[21] = "Not Connected         ";  s_rx_atest_b_signal_name[21] = "N/A                     ";
    s_rx_atest_b_term[22] = 0;    s_rx_atest_b_block_name[22] = "Not Connected         ";  s_rx_atest_b_signal_name[22] = "N/A                     ";
    s_rx_atest_b_term[23] = 0;    s_rx_atest_b_block_name[23] = "Not Connected         ";  s_rx_atest_b_signal_name[23] = "N/A                     ";
    s_rx_atest_b_term[24] = 0;    s_rx_atest_b_block_name[24] = "Not Connected         ";  s_rx_atest_b_signal_name[24] = "N/A                     ";
    s_rx_atest_b_term[25] = 0;    s_rx_atest_b_block_name[25] = "Not Connected         ";  s_rx_atest_b_signal_name[25] = "N/A                     ";
    s_rx_atest_b_term[26] = 0;    s_rx_atest_b_block_name[26] = "Not Connected         ";  s_rx_atest_b_signal_name[26] = "N/A                     ";
    s_rx_atest_b_term[27] = 0;    s_rx_atest_b_block_name[27] = "Not Connected         ";  s_rx_atest_b_signal_name[27] = "N/A                     ";
    s_rx_atest_b_term[28] = 0;    s_rx_atest_b_block_name[28] = "Not Connected         ";  s_rx_atest_b_signal_name[28] = "N/A                     ";
    s_rx_atest_b_term[29] = 0;    s_rx_atest_b_block_name[29] = "Not Connected         ";  s_rx_atest_b_signal_name[29] = "N/A                     ";
    s_rx_atest_b_term[30] = 0;    s_rx_atest_b_block_name[30] = "Not Connected         ";  s_rx_atest_b_signal_name[30] = "N/A                     ";
    s_rx_atest_b_term[31] = 0;    s_rx_atest_b_block_name[31] = "Not Connected         ";  s_rx_atest_b_signal_name[31] = "N/A                     ";


    uint32_t atest_cmn_adc_val;
    uint32_t atest_tx_adc_val;
    uint32_t atest_rx_a_adc_val;
    uint32_t atest_rx_b_adc_val;
    char str [50];
    //CMN_ATEST
    for (int i=1; i<= 30; i++) {
        sprintf(str, "cmn_atest_addr = %d, cmn_atest_term = %d, cmn_atest_block_name = %s, cmn_atest_signal_name = %s",i,s_cmn_atest_term[i],s_cmn_atest_block_name[i],s_cmn_atest_signal_name[i]);
        pmd_ate_vec_comment(str);
        pass += aw_pmd_atest_cmn_capture(&mss, i, s_cmn_atest_term[i], &atest_cmn_adc_val);
        USR_PRINTF("CMN_ATEST[%d] = %d\n",i,atest_cmn_adc_val);
    }

    //TX_ATEST
    for (int i=1; i<= 31; i++) {
        sprintf(str, "tx_atest_addr = %d, tx_atest_term = %d, tx_atest_block_name = %s, tx_atest_signal_name = %s",i,s_tx_atest_term[i],s_tx_atest_block_name[i],s_tx_atest_signal_name[i]);
        pmd_ate_vec_comment(str);
        pass += aw_pmd_atest_tx_capture(&mss, i, s_tx_atest_term[i], &atest_tx_adc_val);
        USR_PRINTF("TX_ATEST[%d] = %d\n",i,atest_tx_adc_val);
    }

    //RX_ATEST_A
    for (int i=1; i<= 31; i++) {
        sprintf(str, "rx_atest_a_addr = %d, rx_atest_a_term = %d, rx_atest_a_block_name = %s, rx_atest_a_signal_name = %s",i,s_rx_atest_a_term[i],s_rx_atest_a_block_name[i],s_rx_atest_a_signal_name[i]);
        pmd_ate_vec_comment(str);

        pass += aw_pmd_atest_rx_a_capture(&mss, i, s_rx_atest_a_term[i], &atest_rx_a_adc_val);
        USR_PRINTF("RX_ATEST_A[%d] = %d\n",i,atest_rx_a_adc_val);
    }

    //RX_ATEST_B
    for (int i=1; i<= 12; i++) {
        sprintf(str, "rx_atest_b_addr = %d, rx_atest_b_term = %d, rx_atest_b_block_name = %s, rx_atest_b_signal_name = %s",i,s_rx_atest_b_term[i],s_rx_atest_b_block_name[i],s_rx_atest_b_signal_name[i]);
        pmd_ate_vec_comment(str);
        pass += aw_pmd_atest_rx_b_capture(&mss, i, s_rx_atest_b_term[i], &atest_rx_b_adc_val);
        USR_PRINTF("RX_ATEST_B[%d] = %d\n",i,atest_rx_b_adc_val);
    }

    USR_PRINTF("Disable ATEST ADC\n");
    pmd_ate_vec_comment("Disable ATEST ADC");
    pass += aw_pmd_atest_en(&mss, 0);

    // Look at the value of pass in the error table and give out the relevant error code using UVM error.
    svSetScope(svGetScopeFromName("serdes_env_pkg"));
    sv_error(pass);

    return pass;
}

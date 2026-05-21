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

#include "aw_alphacore.h"
#include "aw_alphacore_custom.h"
#include "aw_pmd_rx_dsp_get.h"

int aw_pmd_rx_vga_cap_adapt_get(mss_access_t *mss, vga_opt_t *opts) {

    uint32_t en_n;
    uint32_t vga_cap_1 = 0;
    uint32_t vga_cap_2 = 0;

    CHECK(pmd_read_field(mss, DISABLE_VGA_CAP_ADAPT_ADDR, DISABLE_VGA_CAP_ADAPT_MASK, DISABLE_VGA_CAP_ADAPT_OFFSET, &en_n));
    opts->en = 1 - en_n;

    CHECK(pmd_read_field(mss, USE_CUSTOM_VGA_CAP_TAKEOVER_ADDR, USE_CUSTOM_VGA_CAP_TAKEOVER_MASK, USE_CUSTOM_VGA_CAP_TAKEOVER_OFFSET, &opts->use_custom_takeover_ratio));
    CHECK(pmd_read_field(mss, VGA_CAP_TAKEOVER_RATIO_ADDR, VGA_CAP_TAKEOVER_RATIO_MASK, VGA_CAP_TAKEOVER_RATIO_OFFSET, &opts->custom_takeover_ratio));
    CHECK(pmd_read_field(mss, NYQ_MASK_VGA_CAP_ADDR, NYQ_MASK_VGA_CAP_MASK, NYQ_MASK_VGA_CAP_OFFSET, &opts->custom_nyq_mask));
    CHECK(pmd_read_field(mss, RX_VGA_REG1_ADDR, RX_VGA_REG1_CAP1_LUT_0_A_MASK, RX_VGA_REG1_CAP1_LUT_0_A_OFFSET, &vga_cap_1));
    CHECK(pmd_read_field(mss, RX_VGA_REG2_ADDR, RX_VGA_REG2_CAP2_LUT_0_A_MASK, RX_VGA_REG2_CAP2_LUT_0_A_OFFSET, &vga_cap_2));

    if (vga_cap_1 == 0 && vga_cap_2 == 0) {
        opts->vga_cap = 0;  
    } else if (vga_cap_1 == 1 && vga_cap_2 == 0) {
        opts->vga_cap = 1;
    } else if (vga_cap_1 == 3 && vga_cap_2 == 0) {
        opts->vga_cap = 2;
    } else if (vga_cap_1 == 3 && vga_cap_2 == 1) {
        opts->vga_cap = 3;
    } else if (vga_cap_1 == 3 && vga_cap_2 == 3) {
        opts->vga_cap = 4;
    }

    return AW_ERR_CODE_NONE;
}

int aw_pmd_rx_vga_cap_adapt_set(mss_access_t *mss, vga_opt_t opts) {

    uint32_t vga_cap_1 = 0;
    uint32_t vga_cap_2 = 0;

    if (opts.en == 1) {
        CHECK(pmd_write_field(mss, DISABLE_VGA_CAP_ADAPT_ADDR, DISABLE_VGA_CAP_ADAPT_MASK, DISABLE_VGA_CAP_ADAPT_OFFSET, !opts.en));  
    }
    
    if (opts.vga_cap == 0) {
        vga_cap_1 = 0;
        vga_cap_2 = 0; 
    } else if (opts.vga_cap == 1) {
        vga_cap_1 = 1;
        vga_cap_2 = 0;
    } else if (opts.vga_cap == 2) {
        vga_cap_1 = 3;
        vga_cap_2 = 0;
    } else if (opts.vga_cap == 3) {
        vga_cap_1 = 3;
        vga_cap_2 = 1;
    } else if (opts.vga_cap == 4) {
        vga_cap_1 = 3;
        vga_cap_2 = 3;
    }

    // First VGA CAP setting. 
    CHECK(pmd_write_field(mss, RX_VGA_REG1_ADDR, RX_VGA_REG1_CAP1_LUT_0_A_MASK, RX_VGA_REG1_CAP1_LUT_0_A_OFFSET, vga_cap_1));
    CHECK(pmd_write_field(mss, RX_VGA_REG1_ADDR, RX_VGA_REG1_CAP1_LUT_1_A_MASK, RX_VGA_REG1_CAP1_LUT_1_A_OFFSET, vga_cap_1));
    CHECK(pmd_write_field(mss, RX_VGA_REG1_ADDR, RX_VGA_REG1_CAP1_LUT_2_A_MASK, RX_VGA_REG1_CAP1_LUT_2_A_OFFSET, vga_cap_1));
    CHECK(pmd_write_field(mss, RX_VGA_REG1_ADDR, RX_VGA_REG1_CAP1_LUT_3_A_MASK, RX_VGA_REG1_CAP1_LUT_3_A_OFFSET, vga_cap_1));
    CHECK(pmd_write_field(mss, RX_VGA_REG1_ADDR, RX_VGA_REG1_CAP1_LUT_4_A_MASK, RX_VGA_REG1_CAP1_LUT_4_A_OFFSET, vga_cap_1));
    CHECK(pmd_write_field(mss, RX_VGA_REG2_ADDR, RX_VGA_REG2_CAP1_LUT_5_A_MASK, RX_VGA_REG2_CAP1_LUT_5_A_OFFSET, vga_cap_1));
    CHECK(pmd_write_field(mss, RX_VGA_REG2_ADDR, RX_VGA_REG2_CAP1_LUT_6_A_MASK, RX_VGA_REG2_CAP1_LUT_6_A_OFFSET, vga_cap_1));
    CHECK(pmd_write_field(mss, RX_VGA_REG2_ADDR, RX_VGA_REG2_CAP1_LUT_7_A_MASK, RX_VGA_REG2_CAP1_LUT_7_A_OFFSET, vga_cap_1));
    // Second VGA CAP setting. 
    CHECK(pmd_write_field(mss, RX_VGA_REG2_ADDR, RX_VGA_REG2_CAP2_LUT_0_A_MASK, RX_VGA_REG2_CAP2_LUT_0_A_OFFSET, vga_cap_2));
    CHECK(pmd_write_field(mss, RX_VGA_REG2_ADDR, RX_VGA_REG2_CAP2_LUT_1_A_MASK, RX_VGA_REG2_CAP2_LUT_1_A_OFFSET, vga_cap_2));
    CHECK(pmd_write_field(mss, RX_VGA_REG3_ADDR, RX_VGA_REG3_CAP2_LUT_2_A_MASK, RX_VGA_REG3_CAP2_LUT_2_A_OFFSET, vga_cap_2));
    CHECK(pmd_write_field(mss, RX_VGA_REG3_ADDR, RX_VGA_REG3_CAP2_LUT_3_A_MASK, RX_VGA_REG3_CAP2_LUT_3_A_OFFSET, vga_cap_2));
    CHECK(pmd_write_field(mss, RX_VGA_REG3_ADDR, RX_VGA_REG3_CAP2_LUT_4_A_MASK, RX_VGA_REG3_CAP2_LUT_4_A_OFFSET, vga_cap_2));
    CHECK(pmd_write_field(mss, RX_VGA_REG3_ADDR, RX_VGA_REG3_CAP2_LUT_5_A_MASK, RX_VGA_REG3_CAP2_LUT_5_A_OFFSET, vga_cap_2));
    CHECK(pmd_write_field(mss, RX_VGA_REG3_ADDR, RX_VGA_REG3_CAP2_LUT_6_A_MASK, RX_VGA_REG3_CAP2_LUT_6_A_OFFSET, vga_cap_2));
    CHECK(pmd_write_field(mss, RX_VGA_REG4_ADDR, RX_VGA_REG4_CAP2_LUT_7_A_MASK, RX_VGA_REG4_CAP2_LUT_7_A_OFFSET, vga_cap_2));

    CHECK(pmd_write_field(mss, USE_CUSTOM_VGA_CAP_TAKEOVER_ADDR, USE_CUSTOM_VGA_CAP_TAKEOVER_MASK, USE_CUSTOM_VGA_CAP_TAKEOVER_OFFSET, opts.use_custom_takeover_ratio));
    CHECK(pmd_write_field(mss, VGA_CAP_TAKEOVER_RATIO_ADDR, VGA_CAP_TAKEOVER_RATIO_MASK, VGA_CAP_TAKEOVER_RATIO_OFFSET, opts.custom_takeover_ratio));
    CHECK(pmd_write_field(mss, NYQ_MASK_VGA_CAP_ADDR, NYQ_MASK_VGA_CAP_MASK, NYQ_MASK_VGA_CAP_OFFSET, opts.custom_nyq_mask));

    return AW_ERR_CODE_NONE;
}

int aw_pmd_rx_dig_pwr_det_threshold_set(mss_access_t *mss, uint32_t adc_valid_thresh_nt, uint32_t adc_invalid_thresh_nt ) {

    CHECK(pmd_write_field(mss, RX_SIGNAL_DETECT_REG1_ADDR, RX_SIGNAL_DETECT_REG1_ADC_VALID_THRES_NT_MASK, RX_SIGNAL_DETECT_REG1_ADC_VALID_THRES_NT_OFFSET, adc_valid_thresh_nt));
    CHECK(pmd_write_field(mss, RX_SIGNAL_DETECT_REG1_ADC_INVALID_THRES_NT_OFFSET, RX_SIGNAL_DETECT_REG1_ADC_INVALID_THRES_NT_OFFSET, RX_SIGNAL_DETECT_REG1_ADC_INVALID_THRES_NT_OFFSET, adc_invalid_thresh_nt));

    return AW_ERR_CODE_NONE;
}

int aw_pmd_rx_dig_pwr_det_threshold_get(mss_access_t *mss, uint32_t *adc_valid_thresh_nt, uint32_t *adc_invalid_thresh_nt ) {

    CHECK(pmd_read_field(mss, RX_SIGNAL_DETECT_REG1_ADDR, RX_SIGNAL_DETECT_REG1_ADC_VALID_THRES_NT_MASK, RX_SIGNAL_DETECT_REG1_ADC_VALID_THRES_NT_OFFSET, adc_valid_thresh_nt));
    CHECK(pmd_read_field(mss, RX_SIGNAL_DETECT_REG1_ADC_INVALID_THRES_NT_OFFSET, RX_SIGNAL_DETECT_REG1_ADC_INVALID_THRES_NT_OFFSET, RX_SIGNAL_DETECT_REG1_ADC_INVALID_THRES_NT_OFFSET, adc_invalid_thresh_nt));

    return AW_ERR_CODE_NONE;
}


int aw_pmd_rx_signal_detect_fsm_type_set(mss_access_t *mss, uint32_t valid_type, uint32_t invalid_type){
    CHECK(pmd_write_field(mss, RX_SIGNAL_DETECT_REG3_ADDR, RX_SIGNAL_DETECT_REG3_VALID_CTRL_TYPE_NT_MASK, RX_SIGNAL_DETECT_REG3_VALID_CTRL_TYPE_NT_OFFSET, valid_type));
    CHECK(pmd_write_field(mss, RX_SIGNAL_DETECT_REG3_ADDR, RX_SIGNAL_DETECT_REG3_INVALID_CTRL_TYPE_NT_MASK, RX_SIGNAL_DETECT_REG3_INVALID_CTRL_TYPE_NT_OFFSET, invalid_type));
    return AW_ERR_CODE_NONE;

}

int aw_pmd_rx_signal_detect_fsm_type_get(mss_access_t *mss, uint32_t *valid_type, uint32_t *invalid_type){
    CHECK(pmd_read_field(mss, RX_SIGNAL_DETECT_REG3_ADDR, RX_SIGNAL_DETECT_REG3_VALID_CTRL_TYPE_NT_MASK, RX_SIGNAL_DETECT_REG3_VALID_CTRL_TYPE_NT_OFFSET, valid_type));
    CHECK(pmd_read_field(mss, RX_SIGNAL_DETECT_REG3_ADDR, RX_SIGNAL_DETECT_REG3_INVALID_CTRL_TYPE_NT_MASK, RX_SIGNAL_DETECT_REG3_INVALID_CTRL_TYPE_NT_OFFSET, invalid_type));
    return AW_ERR_CODE_NONE;

}

int aw_pmd_rx_signal_detect_fsm_ctrl_set(mss_access_t *mss, uint32_t valid, uint32_t invalid){
    CHECK(pmd_write_field(mss, RX_SIGNAL_DETECT_REG3_ADDR, RX_SIGNAL_DETECT_REG3_VALID_CTRL_NT_MASK, RX_SIGNAL_DETECT_REG3_VALID_CTRL_NT_OFFSET, valid));
    CHECK(pmd_write_field(mss, RX_SIGNAL_DETECT_REG3_ADDR, RX_SIGNAL_DETECT_REG3_INVALID_CTRL_NT_MASK, RX_SIGNAL_DETECT_REG3_INVALID_CTRL_NT_OFFSET, invalid));
    return AW_ERR_CODE_NONE;
}

int aw_pmd_rx_signal_detect_fsm_ctrl_get(mss_access_t *mss, uint32_t *valid, uint32_t *invalid){
    CHECK(pmd_read_field(mss, RX_SIGNAL_DETECT_REG3_ADDR, RX_SIGNAL_DETECT_REG3_VALID_CTRL_NT_MASK, RX_SIGNAL_DETECT_REG3_VALID_CTRL_NT_OFFSET, valid));
    CHECK(pmd_read_field(mss, RX_SIGNAL_DETECT_REG3_ADDR, RX_SIGNAL_DETECT_REG3_INVALID_CTRL_NT_MASK, RX_SIGNAL_DETECT_REG3_INVALID_CTRL_NT_OFFSET, invalid));
    return AW_ERR_CODE_NONE;
}

int aw_pmd_rx_signal_detect_valid_pcs_sel_set(mss_access_t *mss, uint32_t sel){
    CHECK(pmd_write_field(mss, RX_SIGNAL_DETECT_REG3_ADDR, RX_SIGNAL_DETECT_REG3_VALID_PCS_SEL_NT_MASK, RX_SIGNAL_DETECT_REG3_VALID_PCS_SEL_NT_OFFSET, sel));
    return AW_ERR_CODE_NONE;
}

int aw_pmd_rx_signal_detect_valid_pcs_sel_get(mss_access_t *mss, uint32_t *sel){
    CHECK(pmd_read_field(mss, RX_SIGNAL_DETECT_REG3_ADDR, RX_SIGNAL_DETECT_REG3_VALID_PCS_SEL_NT_MASK, RX_SIGNAL_DETECT_REG3_VALID_PCS_SEL_NT_OFFSET, sel));
    return AW_ERR_CODE_NONE;
}

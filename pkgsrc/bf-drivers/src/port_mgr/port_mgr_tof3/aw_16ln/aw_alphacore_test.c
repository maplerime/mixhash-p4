
#if 1

// all uint32_t are signed, need to include custom precisions layout
typedef struct new_aw_dsp_param_s {
    /* Return adapted DC offset value for specified branch */
    uint32_t dc_offset;
    /* Return adapted DFE value for specified branch. */
    uint32_t dfe;
    aw_ffe_t ffe;
    aw_ffe_t pre_ffe;
    aw_thresholds_t thresholds;
    aw_slicers_t slicers;
} new_aw_dsp_param_t;

#include "new_aw_pmd_rx_dsp_get.c"

#if 0
//new for alireza
//
// To be included in aw_alphacore.c
//
//
int aw_pmd_16ln_rx_cdr_lock_get(mss_access_t *mss, uint32_t *rx_cdr_lock) {
    CHECK(pmd_read_field(mss, DIG_SOC_LANE_STAT_REG1_ADDR, DIG_SOC_LANE_STAT_REG1_OCTL_RX_DATA_VLD_MASK, DIG_SOC_LANE_STAT_REG1_OCTL_RX_DATA_VLD_OFFSET, rx_cdr_lock));
    return AW_ERR_CODE_NONE;
}

int aw_pmd_16ln_lcpll_vco_counter_get(mss_access_t *mss, uint32_t timing_window, double *lcpll_ppm, double *vco_freq, double refclk_freq){
    int poll_result;
    CHECK(pmd_write_field(mss, AFE_CMN_VCO_ADAPT_REG1_ADDR, AFE_CMN_VCO_ADAPT_REG1_TIMING_WINDOW_NT_MASK, AFE_CMN_VCO_ADAPT_REG1_TIMING_WINDOW_NT_OFFSET, timing_window));
    CHECK(pmd_write_field(mss, AFE_CMN_VCO_ADAPT_REG3_ADDR, AFE_CMN_VCO_ADAPT_REG3_REQ_A_MASK, AFE_CMN_VCO_ADAPT_REG3_REQ_A_OFFSET, 0));

    // assert req, check for poll error
    CHECK(pmd_write_field(mss, AFE_CMN_VCO_ADAPT_REG1_ADDR, AFE_CMN_VCO_ADAPT_REG1_FREQ_MEASURE_REQ_A_MASK, AFE_CMN_VCO_ADAPT_REG1_FREQ_MEASURE_REQ_A_OFFSET, 1));
    poll_result = pmd_poll_field(mss, AFE_CMN_VCO_ADAPT_RDREG1_ADDR, AFE_CMN_VCO_ADAPT_RDREG1_FREQ_MEASURE_ACK_NT_MASK, AFE_CMN_VCO_ADAPT_RDREG1_FREQ_MEASURE_ACK_NT_OFFSET, 1, 10);
    if (poll_result == -1) {
        USR_PRINTF("ERROR: polling for freq measure ack\n");
        return AW_ERR_CODE_POLL_TIMEOUT;
    } else {
        USR_PRINTF("freq measure ack received\n");
    }
    uint32_t freq_meas_result;
    uint32_t vco_counter;

    CHECK(pmd_read_field(mss, AFE_CMN_VCO_ADAPT_RDREG1_ADDR, AFE_CMN_VCO_ADAPT_RDREG1_FREQ_MEASURE_RESULT_NT_MASK, AFE_CMN_VCO_ADAPT_RDREG1_FREQ_MEASURE_RESULT_NT_OFFSET, &freq_meas_result));
    CHECK(pmd_read_field(mss, AFE_CMN_VCO_ADAPT_RDREG2_ADDR, AFE_CMN_VCO_ADAPT_RDREG2_VCO_COUNTER_NT_MASK, AFE_CMN_VCO_ADAPT_RDREG2_VCO_COUNTER_NT_OFFSET, &vco_counter));

    // de-assert req, check for poll error
    CHECK(pmd_write_field(mss, AFE_CMN_VCO_ADAPT_REG1_ADDR, AFE_CMN_VCO_ADAPT_REG1_FREQ_MEASURE_REQ_A_MASK, AFE_CMN_VCO_ADAPT_REG1_FREQ_MEASURE_REQ_A_OFFSET, 0));
    poll_result = pmd_poll_field(mss, AFE_CMN_VCO_ADAPT_RDREG1_ADDR, AFE_CMN_VCO_ADAPT_RDREG1_FREQ_MEASURE_ACK_NT_MASK, AFE_CMN_VCO_ADAPT_RDREG1_FREQ_MEASURE_ACK_NT_OFFSET, 0, 10);
    if (poll_result == -1) {
        USR_PRINTF("ERROR: polling for freq measure ack\n");
        return AW_ERR_CODE_POLL_TIMEOUT;
    } else {
        USR_PRINTF("freq measure ack received\n");
    }

    // compute ppm. result is in S3.12 format, need to convert first.
    if (((freq_meas_result >> 15) & 0x1) == 1) {
        freq_meas_result = 0xffff - freq_meas_result;
    }
    double ppm = pow(10,6)*freq_meas_result/pow(2, timing_window);
    *lcpll_ppm = ppm;

    //Compute vco freq
    *vco_freq = 10 * (vco_counter / (pow(2,timing_window) * (1/refclk_freq)));

    return AW_ERR_CODE_NONE;
}

char * aw_bist_pattern_decoder (uint32_t bist_pattern_encoded) {
    if (bist_pattern_encoded == 0){
        return "PRBS7";
    } else if (bist_pattern_encoded == 1){
        return "PRBS9";
    } else if (bist_pattern_encoded == 2){
        return "PRBS11";
    } else if (bist_pattern_encoded == 3){
        return "PRBS13";
    } else if (bist_pattern_encoded == 4){
        return "PRBS15";
    } else if (bist_pattern_encoded == 5){
        return "PRBS23";
    } else if (bist_pattern_encoded == 6){
        return "PRBS31";
    } else if (bist_pattern_encoded == 7){
        return "QPRBS13";
    } else if (bist_pattern_encoded == 8){
        return "JP03A";
    } else if (bist_pattern_encoded == 9){
        return "JP03B";
    } else if (bist_pattern_encoded == 10){
        return "Linearity";
    } else if (bist_pattern_encoded == 11){
        return "UDP";
    } else if (bist_pattern_encoded == 12){
        return "Full Clock";
    } else if (bist_pattern_encoded == 13){
        return "Half Clock";
    } else if (bist_pattern_encoded == 14){
        return "Quarter Clock";
    } else {
        return "NULL";
    }
}
#endif

int bfn_aw_pmd_16ln_rx_cdr_offset_get(mss_access_t *mss, uint32_t *cdr_offset, uint32_t *cdr_dir) {
    uint32_t cdr_offset_dir;
    CHECK(pmd_read_field(mss, RXMFSM_SCRATCH_REG11_ADDR, RXMFSM_SCRATCH_REG11_RXMFSM_SCRATCH11_MASK, RXMFSM_SCRATCH_REG11_RXMFSM_SCRATCH11_OFFSET, &cdr_offset_dir));
    *cdr_offset = (cdr_offset_dir & 0x00ff);
    *cdr_dir = (cdr_offset_dir & 0x0800) >> 8;
    return AW_ERR_CODE_NONE;
}

int aw_pmd_16ln_read_status_new(mss_access_t *mss, int branch) {
    uint32_t err_count_done, err_count_overflow,err_cnt_55_32,err_cnt_31_0;
    uint64_t err_count;
    new_aw_dsp_param_t dsp_info_[AW_NUM_BRANCHES];
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
#if defined(AWCFG_APOLLOCORE)
    uint32_t lsb1;
#endif
    uint32_t vga_cap;
    uint32_t tx_bist_encoded, rx_bist_encoded;
    char * tx_bist_pattern;
    char * rx_bist_pattern;
    uint32_t cdr_offset;
    uint32_t cdr_dir;
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


    CHECK(pmd_read_field(mss, TXMFSM_STAT_ADDR, TXMFSM_STAT_TXMFSM_RATE_CUR_MASK, TXMFSM_STAT_TXMFSM_RATE_CUR_OFFSET, &txmfsm_rate_cur));
    CHECK(pmd_read_field(mss, RXMFSM_STAT_ADDR, RXMFSM_STAT_RXMFSM_RATE_CUR_MASK, RXMFSM_STAT_RXMFSM_RATE_CUR_OFFSET, &rxmfsm_rate_cur));

    CHECK(pmd_read_field(mss, RX_DATABIST_TOP_RDREG1_ADDR, RX_DATABIST_TOP_RDREG1_ERROR_CNT_DONE_NT_MASK, RX_DATABIST_TOP_RDREG1_ERROR_CNT_DONE_NT_OFFSET, &err_count_done));
    CHECK(pmd_read_field(mss, RX_DATABIST_TOP_RDREG3_ADDR, RX_DATABIST_TOP_RDREG3_ERROR_CNT_55T32_NT_MASK, RX_DATABIST_TOP_RDREG3_ERROR_CNT_55T32_NT_OFFSET, &err_cnt_55_32));
    CHECK(pmd_read_field(mss, RX_DATABIST_TOP_RDREG2_ADDR, RX_DATABIST_TOP_RDREG2_ERROR_CNT_NT_MASK, RX_DATABIST_TOP_RDREG2_ERROR_CNT_NT_OFFSET, &err_cnt_31_0));
    CHECK(pmd_read_field(mss, RX_DATABIST_TOP_RDREG1_ADDR, RX_DATABIST_TOP_RDREG1_ERROR_CNT_OVERFLOW_NT_MASK, RX_DATABIST_TOP_RDREG1_ERROR_CNT_OVERFLOW_NT_OFFSET, &err_count_overflow));

    CHECK(aw_pmd_16ln_rx_cdr_lock_get(mss, &cdr_lock));


    err_count = (uint64_t) err_cnt_55_32 << 32 | (uint64_t) err_cnt_31_0;

    //If branch = -1, read all branches
    if (branch == -1) {
        for (int i=0; i<AW_NUM_BRANCHES; i++) {
            CHECK(new_aw_pmd_rx_dsp_get(mss, i, &dsp_info_[i]));
        }
    } else {
        CHECK(new_aw_pmd_rx_dsp_get(mss, branch, &dsp_info_[branch]));
    }
    CHECK(aw_pmd_16ln_rx_afe_get(mss, &rx_afe_data_));
    CHECK(aw_pmd_16ln_rx_dcdiq_get(mss, &rx_dcdiq_data_));
    CHECK(aw_pmd_16ln_tx_dcdiq_get(mss, &tx_dcdiq_data_));

    CHECK(aw_pmd_16ln_tx_pll_fine_code_get(mss, &tx_pll_fine_code,0,-1));
    CHECK(aw_pmd_16ln_tx_pll_coarse_code_get(mss, &tx_pll_coarse_code,0,-1));
    CHECK(aw_pmd_16ln_tx_ppm_get(mss, timing_window, 50, &tx_pll_ppm, &tx_pll_vco, 312500000));

    CHECK(aw_pmd_16ln_rx_pll_fine_code_get(mss, &rx_pll_fine_code,0,-1));
    CHECK(aw_pmd_16ln_rx_pll_coarse_code_get(mss, &rx_pll_coarse_code,0,-1));
    CHECK(aw_pmd_16ln_rx_ppm_get(mss, timing_window, 50, &rx_pll_ppm, &rx_pll_vco, 312500000));

    CHECK(aw_pmd_16ln_cmn_pll_fine_code_get(mss, &lcpll_fine_code,0,-1));
    CHECK(aw_pmd_16ln_cmn_pll_coarse_code_get(mss, &lcpll_coarse_code,0,-1));
    CHECK(aw_pmd_16ln_lcpll_vco_counter_get(mss, timing_window, &lcpll_ppm, &lcpll_vco, 312500000));
    CHECK(aw_pmd_16ln_pll_lock_get(mss, &lcpll_lock,0,0));

#if defined(AWCFG_APOLLOCORE)
    CHECK(aw_pmd_16ln_tx_hbridge_get(mss, &msb, &lsb, &lsb1));
#else
    CHECK(aw_pmd_16ln_tx_hbridge_get(mss, &msb, &lsb));
#endif

    CHECK(aw_pmd_16ln_rx_vga_cap_get(mss, &vga_cap));

    CHECK(pmd_read_field(mss, TX_DATAPATH_REG2_ADDR, TX_DATAPATH_REG2_PATTERN_SEL_NT_MASK, TX_DATAPATH_REG2_PATTERN_SEL_NT_OFFSET, &tx_bist_encoded));
    tx_bist_pattern = aw_bist_pattern_decoder (tx_bist_encoded);

    CHECK(pmd_read_field(mss, RX_DATABIST_TOP_REG2_ADDR, RX_DATABIST_TOP_REG2_PATTERN_SEL_NT_MASK, RX_DATABIST_TOP_REG2_PATTERN_SEL_NT_OFFSET, &rx_bist_encoded));
    rx_bist_pattern = aw_bist_pattern_decoder (rx_bist_encoded);

    CHECK(bfn_aw_pmd_16ln_rx_cdr_offset_get(mss,&cdr_offset,&cdr_dir));

    CHECK(aw_pmd_16ln_txfir_config_get(mss,&txfir_cfg));
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

    CHECK(aw_pmd_16ln_rx_prefec_config_get(mss, &symbol_size, &symbol_per_cw, &corr_num_syms, &wall_mode, &skip_syms, &timer_num_cw));

    CHECK(pmd_read_field(mss, RX_DATABIST_TOP_REG14_ADDR, RX_DATABIST_TOP_REG14_BURST_ERROR_BITS_THRESHOLD_NT_MASK, RX_DATABIST_TOP_REG14_BURST_ERROR_BITS_THRESHOLD_NT_OFFSET, &burst_bit_threshold));
    CHECK(pmd_read_field(mss, RX_DATABIST_TOP_RDREG12_ADDR, RX_DATABIST_TOP_RDREG12_BURST_ERR_FOUND_CNT_NT_MASK, RX_DATABIST_TOP_RDREG12_BURST_ERR_FOUND_CNT_NT_OFFSET, &burst_err_found));

    CHECK(aw_pmd_16ln_rx_prefec_get_results(mss, prefec_hist));

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

    USR_PRINTF("------------------------------------------\n");
    USR_PRINTF("-----------  STATUS REGISTERS  -----------\n");
    USR_PRINTF("------------------------------------------\n\n");
    USR_PRINTF("txmfsm_rate_cur                  = %d\n",txmfsm_rate_cur);
    USR_PRINTF("rxmfsm_rate_cur                  = %d\n",rxmfsm_rate_cur);
    USR_PRINTF("err_count_done                   = %d\n",err_count_done);
    USR_PRINTF("err_count                        = %16" PRIx64 "\n",err_count);
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
    USR_PRINTF("vga_cap                          = %d\n"  , vga_cap);
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
    return AW_ERR_CODE_NONE;
}

#endif //0


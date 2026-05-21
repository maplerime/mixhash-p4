/*
 * ? 2020 Alphawave IP Inc.
 */

/**
 * @brief Alphawave AlphaCORE100 C API functions.
 *
 * @file aw_alphacore.c
 *
 * @version 1.0.7
 */

#include <stdint.h>
#include <math.h>
#include <string.h>
#define __STDC_FORMAT_MACROS 1 // support PRIu formats
#include <inttypes.h>

#include "aw_alphacore.h"
#include "aw_pmd_rx_dsp_get.h"
#include "stdlib.h"

// include all our private code
#include <stddef.h>
#include "bf_alphacore.c"

//const char aw_library_version[] = "1.0.7";

static uint32_t aw_width_decoder (uint32_t width_encoded) {
    uint32_t width = 128;
    if (width_encoded == 7){
        width = 128 ;
    } else if (width_encoded == 6){
        width = 64;
    } else if (width_encoded == 5){
        width = 40;
    } else if (width_encoded == 4){
        width = 32;
    } else if (width_encoded == 3){
        width = 20;
    } else if (width_encoded == 2){
        width = 16;
    } else if (width_encoded == 1){
        width = 10;
    } else if (width_encoded == 0){
        width = 64;
    } else {
        USR_PRINTF("ERROR: Invalid width encoding\n");
        return 0;
    }
    return width;
}

uint64_t aw_pmd_4ln_digref_block_div_finder (double a, double b) {

    uint32_t num_dec = 0;
    uint64_t large ,small, i ;

    while(a-truncl(a)!=0||b-truncl(b)) {
        num_dec +=1;
        a*=10;
        b*=10;
    }

    large = AW_MAX(a,b);
    small = AW_MIN(a,b);
    i = large;

    while(1) {
        if(i%small==0){
            return (i)/(a);
        }
        i = i+large;
    }


}


int aw_pmd_4ln_cmn_clkgen_refdiv_set(mss_access_t *mss, uint32_t div){

    CHECK(pmd_write_field(mss, CMN_CLKGEN_ADDR, CMN_CLKGEN_REFDIV_NT_MASK, CMN_CLKGEN_REFDIV_NT_OFFSET, div));
    return AW_ERR_CODE_NONE;

}

int aw_pmd_4ln_cmn_clkgen_refdiv_get(mss_access_t *mss, uint32_t * div){

    CHECK(pmd_read_field(mss, CMN_CLKGEN_ADDR, CMN_CLKGEN_REFDIV_NT_MASK, CMN_CLKGEN_REFDIV_NT_OFFSET, div));
    return AW_ERR_CODE_NONE;

}
int aw_pmd_4ln_anlt_logical_lane_num_set (mss_access_t *mss, uint32_t logical_lane, uint32_t an_no_attached) {
    CHECK(pmd_write_field(mss, ETH_ANLT_CTRL_ADDR, ETH_ANLT_CTRL_ANLT_LANE_NUM_MASK, ETH_ANLT_CTRL_ANLT_LANE_NUM_OFFSET, logical_lane));
    CHECK(pmd_write_field(mss, ETH_LT_PRBS_ADDR, ETH_LT_PRBS_LT_POLYNOMIAL_SEL_C92_MASK, ETH_LT_PRBS_LT_POLYNOMIAL_SEL_C92_OFFSET, logical_lane));
    CHECK(pmd_write_field(mss, ETH_LT_PRBS_ADDR, ETH_LT_PRBS_LT_POLYNOMIAL_SEL_C136_MASK, ETH_LT_PRBS_LT_POLYNOMIAL_SEL_C136_OFFSET, logical_lane));
    CHECK(pmd_write_field(mss, ETH_ANLT_CTRL_ADDR, ETH_ANLT_CTRL_AN_NO_ATTACHED_MASK, ETH_ANLT_CTRL_AN_NO_ATTACHED_OFFSET, an_no_attached));

    return AW_ERR_CODE_NONE;
}

int aw_pmd_4ln_anlt_logical_lane_num_get (mss_access_t *mss, uint32_t * logical_lane, uint32_t * an_no_attached) {
    CHECK(pmd_read_field(mss, ETH_ANLT_CTRL_ADDR, ETH_ANLT_CTRL_ANLT_LANE_NUM_MASK, ETH_ANLT_CTRL_ANLT_LANE_NUM_OFFSET, logical_lane));
    CHECK(pmd_read_field(mss, ETH_ANLT_CTRL_ADDR, ETH_ANLT_CTRL_AN_NO_ATTACHED_MASK, ETH_ANLT_CTRL_AN_NO_ATTACHED_OFFSET, an_no_attached));

    return AW_ERR_CODE_NONE;
}

int aw_pmd_4ln_anlt_auto_neg_adv_ability_set (mss_access_t *mss, uint32_t *adv_ability, uint32_t *fec_ability, uint32_t nonce){
    uint32_t ability_1_temp = 0;
    uint32_t ability_2_temp = 0;

    //Iterate through array to enable auto_neg for each spec
    for (int i = 0; i<=18; i++) {
        if (i <= 10 && adv_ability[i] == 1) {
            //CHECK(pmd_read_field(mss, ETH_AN_ADV_ABILITY_REG2_ADDR, ETH_AN_ADV_ABILITY_REG2_AN_MR_ADV_ABILITY_1_MASK, ETH_AN_ADV_ABILITY_REG2_AN_MR_ADV_ABILITY_1_OFFSET, &ability_1_temp));
            ability_1_temp = ability_1_temp | (1 << (i + 5));
            //CHECK(pmd_write_field(mss, ETH_AN_ADV_ABILITY_REG2_ADDR, ETH_AN_ADV_ABILITY_REG2_AN_MR_ADV_ABILITY_1_MASK, ETH_AN_ADV_ABILITY_REG2_AN_MR_ADV_ABILITY_1_OFFSET, ability_1_temp));
        } else if (i > 10 && i <= 18 && adv_ability[i] == 1) {
            //CHECK(pmd_read_field(mss, ETH_AN_ADV_ABILITY_REG3_ADDR, ETH_AN_ADV_ABILITY_REG3_AN_MR_ADV_ABILITY_2_MASK, ETH_AN_ADV_ABILITY_REG3_AN_MR_ADV_ABILITY_2_OFFSET, &ability_2_temp));
            ability_2_temp = ability_2_temp | (1 << (i - 11));
            //CHECK(pmd_write_field(mss, ETH_AN_ADV_ABILITY_REG3_ADDR, ETH_AN_ADV_ABILITY_REG3_AN_MR_ADV_ABILITY_2_MASK, ETH_AN_ADV_ABILITY_REG3_AN_MR_ADV_ABILITY_2_OFFSET, ability_2_temp));
        }
    }
    //add FEC ability
    for (int i = 0; i<=4; i++) {
        if (fec_ability[i] == 1) {
            //CHECK(pmd_read_field(mss, ETH_AN_ADV_ABILITY_REG3_ADDR, ETH_AN_ADV_ABILITY_REG3_AN_MR_ADV_ABILITY_2_MASK, ETH_AN_ADV_ABILITY_REG3_AN_MR_ADV_ABILITY_2_OFFSET, &ability_2_temp));
            ability_2_temp = ability_2_temp | (1 << (i + 11));
            //CHECK(pmd_write_field(mss, ETH_AN_ADV_ABILITY_REG3_ADDR, ETH_AN_ADV_ABILITY_REG3_AN_MR_ADV_ABILITY_2_MASK, ETH_AN_ADV_ABILITY_REG3_AN_MR_ADV_ABILITY_2_OFFSET, ability_2_temp));
        }
    }

    ability_1_temp |= (nonce & 0x1F);


    CHECK(pmd_write_field(mss, ETH_AN_ADV_ABILITY_REG2_ADDR, ETH_AN_ADV_ABILITY_REG2_AN_MR_ADV_ABILITY_1_MASK, ETH_AN_ADV_ABILITY_REG2_AN_MR_ADV_ABILITY_1_OFFSET, ability_1_temp));
    CHECK(pmd_write_field(mss, ETH_AN_ADV_ABILITY_REG3_ADDR, ETH_AN_ADV_ABILITY_REG3_AN_MR_ADV_ABILITY_2_MASK, ETH_AN_ADV_ABILITY_REG3_AN_MR_ADV_ABILITY_2_OFFSET, ability_2_temp));


    return AW_ERR_CODE_NONE;

}

int aw_pmd_4ln_anlt_auto_neg_adv_ability_get (mss_access_t *mss, uint32_t * adv_ability, uint32_t *fec_ability) {
    uint32_t ability_1_temp;
    uint32_t ability_2_temp;

    CHECK(pmd_read_field(mss, ETH_AN_ADV_ABILITY_REG2_ADDR, ETH_AN_ADV_ABILITY_REG2_AN_MR_ADV_ABILITY_1_MASK, ETH_AN_ADV_ABILITY_REG2_AN_MR_ADV_ABILITY_1_OFFSET, &ability_1_temp));
    CHECK(pmd_read_field(mss, ETH_AN_ADV_ABILITY_REG3_ADDR, ETH_AN_ADV_ABILITY_REG3_AN_MR_ADV_ABILITY_2_MASK, ETH_AN_ADV_ABILITY_REG3_AN_MR_ADV_ABILITY_2_OFFSET, &ability_2_temp));

    for (int i = 0; i<=18; i++) {
        if (i <= 10) {
            if (((ability_1_temp >> (5+i)) & 1) == 1) {
                adv_ability[i] = 1;
            } else {
                adv_ability[i] = 0;
            }
        } else if (i > 10 && i <= 18) {
            if (((ability_2_temp >> (i - 11)) & 1) == 1) {
                adv_ability[i] = 1;
            } else {
                adv_ability[i] = 0;
            }
        }
    }

    for (int i = 0; i<=4; i++) {
            if (((ability_2_temp >> (i + 11)) & 1) == 1) {
                fec_ability[i] = 1;
            } else {
                fec_ability[i] = 0;
            }

    }

    return AW_ERR_CODE_NONE;

}

int aw_pmd_4ln_anlt_auto_neg_config_set (mss_access_t *mss, uint32_t status_check_disable, uint32_t next_page_en, uint32_t an_no_nonce_check){

    CHECK(pmd_write_field(mss,ETH_ANLT_CTRL_ADDR , ETH_ANLT_CTRL_AN_STATUS_CHECK_DISABLE_MASK, ETH_ANLT_CTRL_AN_STATUS_CHECK_DISABLE_OFFSET , status_check_disable));

    //enable next page testing
    if (next_page_en == 0) {
        CHECK(pmd_write_field(mss, ETH_AN_ADV_ABILITY_REG1_ADDR, ETH_AN_ADV_ABILITY_REG1_AN_MR_ADV_ABILITY_0_MASK, ETH_AN_ADV_ABILITY_REG1_AN_MR_ADV_ABILITY_0_OFFSET, 1));
    } else {
        CHECK(pmd_write_field(mss, ETH_AN_ADV_ABILITY_REG1_ADDR, ETH_AN_ADV_ABILITY_REG1_AN_MR_ADV_ABILITY_0_MASK, ETH_AN_ADV_ABILITY_REG1_AN_MR_ADV_ABILITY_0_OFFSET, 32769 /*0b1000000000000001*/));
    }

    //no nounce check, set to 1 for same PHY testing
    CHECK(pmd_write_field(mss, ETH_ANLT_CTRL_ADDR, ETH_ANLT_CTRL_AN_NO_NONCE_CHECK_MASK, ETH_ANLT_CTRL_AN_NO_NONCE_CHECK_OFFSET, an_no_nonce_check));

    return AW_ERR_CODE_NONE;
}

int aw_pmd_4ln_anlt_auto_neg_config_get (mss_access_t *mss, uint32_t * status_check_disable, uint32_t * next_page_en, uint32_t * an_no_nonce_check) {
    uint32_t ret;

    CHECK(pmd_read_field(mss, ETH_ANLT_CTRL_ADDR, ETH_ANLT_CTRL_AN_STATUS_CHECK_DISABLE_MASK, ETH_ANLT_CTRL_AN_STATUS_CHECK_DISABLE_OFFSET, status_check_disable));

    CHECK(pmd_read_field(mss, ETH_AN_ADV_ABILITY_REG1_ADDR, ETH_AN_ADV_ABILITY_REG1_AN_MR_ADV_ABILITY_0_MASK, ETH_AN_ADV_ABILITY_REG1_AN_MR_ADV_ABILITY_0_OFFSET, &ret));

    if ((ret >> 15) == 1) {
        *next_page_en = 1;
    } else {
        *next_page_en = 0;
    }

    CHECK(pmd_read_field(mss, ETH_ANLT_CTRL_ADDR, ETH_ANLT_CTRL_AN_NO_NONCE_CHECK_MASK, ETH_ANLT_CTRL_AN_NO_NONCE_CHECK_OFFSET, an_no_nonce_check));

    return AW_ERR_CODE_NONE;
}

int aw_pmd_4ln_anlt_auto_neg_start_set (mss_access_t *mss, uint32_t start){
    CHECK(pmd_write_field(mss, ETH_AN_CTRL_REG2_ADDR, ETH_AN_CTRL_REG2_AN_MR_AUTONEG_ENABLE_MASK, ETH_AN_CTRL_REG2_AN_MR_AUTONEG_ENABLE_OFFSET, start));

    return AW_ERR_CODE_NONE;
}

int aw_pmd_4ln_anlt_auto_neg_start_get (mss_access_t *mss, uint32_t * start){
    CHECK(pmd_read_field(mss, ETH_AN_CTRL_REG2_ADDR, ETH_AN_CTRL_REG2_AN_MR_AUTONEG_ENABLE_MASK, ETH_AN_CTRL_REG2_AN_MR_AUTONEG_ENABLE_OFFSET, start));

    return AW_ERR_CODE_NONE;
}

int aw_pmd_4ln_anlt_auto_neg_status_get (mss_access_t *mss, uint32_t * link_good){
    CHECK(pmd_read_field(mss, ETH_ANLT_STATUS_ADDR, ETH_ANLT_STATUS_AN_LINK_GOOD_MASK, ETH_ANLT_STATUS_AN_LINK_GOOD_OFFSET, link_good));

    return AW_ERR_CODE_NONE;
}

//bfn
#if 0
int aw_pmd_4ln_anlt_auto_neg_result_get (mss_access_t *mss, uint8_t no_consortium, uint32_t * an_result){
    //an_link_good has to be high for this check to make sense
    uint32_t an_link_good;
    CHECK(pmd_read_check_field(mss, ETH_ANLT_STATUS_ADDR, ETH_ANLT_STATUS_AN_LINK_GOOD_MASK, ETH_ANLT_STATUS_AN_LINK_GOOD_OFFSET, RD_EQ, &an_link_good,  1, 0 /*NULL*/));
 
    if(no_consortium == 1) {
        // If not using consortium use below code branch 
        uint32_t an_spec_1;
        uint32_t an_spec_2;
        uint32_t an_spec_3;
        uint32_t an_spec_4;
        CHECK(pmd_read_field(mss, ETH_AN_RESULT_REG1_ADDR, ETH_AN_RESULT_REG1_AN_SPEC_1_MASK, ETH_AN_RESULT_REG1_AN_SPEC_1_OFFSET, &an_spec_1));
        CHECK(pmd_read_field(mss, ETH_AN_RESULT_REG1_ADDR, ETH_AN_RESULT_REG1_AN_SPEC_2_MASK, ETH_AN_RESULT_REG1_AN_SPEC_2_OFFSET, &an_spec_2));
        CHECK(pmd_read_field(mss, ETH_AN_RESULT_REG1_ADDR, ETH_AN_RESULT_REG1_AN_SPEC_3_MASK, ETH_AN_RESULT_REG1_AN_SPEC_3_OFFSET, &an_spec_3));
        CHECK(pmd_read_field(mss, ETH_AN_RESULT_REG2_ADDR, ETH_AN_RESULT_REG2_AN_SPEC_4_MASK, ETH_AN_RESULT_REG2_AN_SPEC_4_OFFSET, &an_spec_4));

        // concatenate 4 an_spec_* read outs. an_spec will be 1-hot encoded, each bit corresponds to an spec number
        uint32_t an_spec = an_spec_1;
        an_spec = (an_spec_2 << 3)  | an_spec;
        an_spec = (an_spec_3 << 5)  | an_spec;
        an_spec = (an_spec_4 << 13) | an_spec;

        // there should only be 1 bit that is 1, all other bits should be 0
        int cntr = 0;
        for (int i = 0 ; i < 32 ; i++){
            int bit = (an_spec >> i) & 1;
            if (bit == 1){
                cntr++;
            }
        }
        if (cntr != 1){
            return AW_ERR_CODE_BAD_STATE;
        }

        // decode 1-hot
        *an_result = log(an_spec)/log(2);
    } else if (no_consortium == 0) {

        // If using consortium use below code branch 
        uint32_t an_econ_spec_1;
        CHECK(pmd_read_field(mss, ETH_AN_RESULT_REG2_ADDR, ETH_AN_RESULT_REG2_AN_ECON_SPEC_MASK, ETH_AN_RESULT_REG2_AN_ECON_SPEC_OFFSET, &an_econ_spec_1));

        // concatenate  an_econ_spec_* read outs. an_spec will be 1-hot encoded, each bit corresponds to an spec number
        uint32_t an_spec = an_econ_spec_1;

        // there should only be 1 bit that is 1, all other bits should be 0
        int cntr = 0;
        for (int i = 0 ; i < 5 ; i++) {
            int bit = (an_spec >> i) & 1;
            if (bit == 1) {
                cntr++;
            }
        }
        if (cntr != 1) {
            return AW_ERR_CODE_BAD_STATE;
        }

        // decode 1-hot
        *an_result = log(an_spec)/log(2);
    } else {
        USR_PRINTF("ERROR: Please set no_consortium field to either 0 or 1. Other values not accepted \n");
        return AW_ERR_CODE_BAD_STATE;
    }

    return AW_ERR_CODE_NONE;
}

#else

int aw_pmd_4ln_anlt_auto_neg_result_get (mss_access_t *mss, uint32_t * an_result){
    //an_link_good has to be high for this check to make sense
    uint32_t an_link_good;
    CHECK(pmd_read_check_field(mss, ETH_ANLT_STATUS_ADDR, ETH_ANLT_STATUS_AN_LINK_GOOD_MASK, ETH_ANLT_STATUS_AN_LINK_GOOD_OFFSET, RD_EQ, &an_link_good,  1, 0 /*NULL*/));

    uint32_t an_spec_1;
    uint32_t an_spec_2;
    uint32_t an_spec_3;
    uint32_t an_spec_4;
    uint32_t an_econ_spec;

    CHECK(pmd_read_field(mss, ETH_AN_RESULT_REG1_ADDR, ETH_AN_RESULT_REG1_AN_SPEC_1_MASK, ETH_AN_RESULT_REG1_AN_SPEC_1_OFFSET, &an_spec_1));
    CHECK(pmd_read_field(mss, ETH_AN_RESULT_REG1_ADDR, ETH_AN_RESULT_REG1_AN_SPEC_2_MASK, ETH_AN_RESULT_REG1_AN_SPEC_2_OFFSET, &an_spec_2));
    CHECK(pmd_read_field(mss, ETH_AN_RESULT_REG1_ADDR, ETH_AN_RESULT_REG1_AN_SPEC_3_MASK, ETH_AN_RESULT_REG1_AN_SPEC_3_OFFSET, &an_spec_3));
    CHECK(pmd_read_field(mss, ETH_AN_RESULT_REG2_ADDR, ETH_AN_RESULT_REG2_AN_SPEC_4_MASK, ETH_AN_RESULT_REG2_AN_SPEC_4_OFFSET, &an_spec_4));
    CHECK(pmd_read_field(mss, ETH_AN_RESULT_REG2_ADDR, ETH_AN_RESULT_REG2_AN_ECON_SPEC_MASK, ETH_AN_RESULT_REG2_AN_ECON_SPEC_OFFSET, &an_econ_spec));

    // concatenate 4 an_spec_* read outs. an_spec will be 1-hot encoded, each bit corresponds to an spec number
    uint32_t an_spec = an_spec_1;
    an_spec = (an_spec_2 << 3)  | an_spec;
    an_spec = (an_spec_3 << 5)  | an_spec;
    an_spec = (an_spec_4 << 13) | an_spec;
    an_spec = (an_econ_spec << 23) | an_spec;

    // there should only be 1 bit that is 1, all other bits should be 0
    int cntr = 0;
    for (int i = 0 ; i < 32 ; i++){
        int bit = (an_spec >> i) & 1;
        if (bit == 1){
            cntr++;
        }
    }
    if (cntr != 1){
        return AW_ERR_CODE_BAD_STATE;
    }

    // decode 1-hot
    *an_result = log(an_spec)/log(2);

    return AW_ERR_CODE_NONE;
}
#endif //0

int aw_pmd_4ln_anlt_auto_neg_page_rx_get(mss_access_t *mss, uint32_t *an_mr_page_rx, uint64_t *an_rx_link_code_word) {
    CHECK(pmd_read_field(mss, ETH_AN_STAT_ADDR, ETH_AN_STAT_AN_MR_PAGE_RX_MASK, ETH_AN_STAT_AN_MR_PAGE_RX_OFFSET, an_mr_page_rx));

    uint32_t an_rx_link_code_word_1;
    uint32_t an_rx_link_code_word_2;
    uint32_t an_rx_link_code_word_3;

    CHECK(pmd_read_field(mss, ETH_AN_RX_LINK_CODE_WORD_REG1_ADDR, ETH_AN_RX_LINK_CODE_WORD_REG1_AN_RX_LINK_CODE_WORD_1_MASK, ETH_AN_RX_LINK_CODE_WORD_REG1_AN_RX_LINK_CODE_WORD_1_OFFSET, &an_rx_link_code_word_1));
    CHECK(pmd_read_field(mss, ETH_AN_RX_LINK_CODE_WORD_REG2_ADDR, ETH_AN_RX_LINK_CODE_WORD_REG2_AN_RX_LINK_CODE_WORD_2_MASK, ETH_AN_RX_LINK_CODE_WORD_REG2_AN_RX_LINK_CODE_WORD_2_OFFSET, &an_rx_link_code_word_2));
    CHECK(pmd_read_field(mss, ETH_AN_RX_LINK_CODE_WORD_REG3_ADDR, ETH_AN_RX_LINK_CODE_WORD_REG3_AN_RX_LINK_CODE_WORD_3_MASK, ETH_AN_RX_LINK_CODE_WORD_REG3_AN_RX_LINK_CODE_WORD_3_OFFSET, &an_rx_link_code_word_3));

    *an_rx_link_code_word = ((uint64_t)an_rx_link_code_word_3 << 32) | ( an_rx_link_code_word_2 << 16) | an_rx_link_code_word_1;

    return AW_ERR_CODE_NONE;
}

int aw_pmd_4ln_anlt_auto_neg_next_page_set(mss_access_t *mss, uint64_t an_tx_np) {
    uint32_t an_mr_np_tx_1 = (an_tx_np & 0xFFFF);
    uint32_t an_mr_np_tx_2 = (an_tx_np >> 16) & 0xFFFF;
    uint32_t an_mr_np_tx_3 = (an_tx_np >> 32) & 0xFFFF;

    CHECK(pmd_write_field(mss,  ETH_AN_NP_REG1_ADDR, ETH_AN_NP_REG1_AN_MR_NP_TX_1_MASK, ETH_AN_NP_REG1_AN_MR_NP_TX_1_OFFSET, an_mr_np_tx_1));
    CHECK(pmd_write_field(mss,  ETH_AN_NP_REG2_ADDR, ETH_AN_NP_REG2_AN_MR_NP_TX_2_MASK, ETH_AN_NP_REG2_AN_MR_NP_TX_2_OFFSET, an_mr_np_tx_2));
    CHECK(pmd_write_field(mss,  ETH_AN_NP_REG3_ADDR, ETH_AN_NP_REG3_AN_MR_NP_TX_3_MASK, ETH_AN_NP_REG3_AN_MR_NP_TX_3_OFFSET, an_mr_np_tx_3));

    //now that NP is loaded, trigger NP
    CHECK(pmd_write_field(mss,  ETH_AN_NP_REG3_ADDR, ETH_AN_NP_REG3_AN_MR_NEXT_PAGE_LOADED_MASK, ETH_AN_NP_REG3_AN_MR_NEXT_PAGE_LOADED_OFFSET, 1));

    return AW_ERR_CODE_NONE;

}

int aw_pmd_4ln_anlt_auto_neg_newdef_get(mss_access_t *mss, aw_an_spec_t *newdef_get) {

    CHECK(pmd_read_field(mss, ETH_ANLT_WIDTH_REG1_ADDR, ETH_ANLT_WIDTH_REG1_AN_DEF_SPEC_WIDTH_0R_MASK, ETH_ANLT_WIDTH_REG1_AN_DEF_SPEC_WIDTH_0R_OFFSET, &newdef_get->an_def_spec_width[0]));
    CHECK(pmd_read_field(mss, ETH_ANLT_WIDTH_REG1_ADDR, ETH_ANLT_WIDTH_REG1_AN_DEF_SPEC_WIDTH_1R_MASK, ETH_ANLT_WIDTH_REG1_AN_DEF_SPEC_WIDTH_1R_OFFSET, &newdef_get->an_def_spec_width[1]));
    CHECK(pmd_read_field(mss, ETH_ANLT_WIDTH_REG1_ADDR, ETH_ANLT_WIDTH_REG1_AN_DEF_SPEC_WIDTH_2R_MASK, ETH_ANLT_WIDTH_REG1_AN_DEF_SPEC_WIDTH_2R_OFFSET, &newdef_get->an_def_spec_width[2]));
    CHECK(pmd_read_field(mss, ETH_ANLT_WIDTH_REG1_ADDR, ETH_ANLT_WIDTH_REG1_AN_DEF_SPEC_WIDTH_3R_MASK, ETH_ANLT_WIDTH_REG1_AN_DEF_SPEC_WIDTH_3R_OFFSET, &newdef_get->an_def_spec_width[3]));
    CHECK(pmd_read_field(mss, ETH_ANLT_WIDTH_REG1_ADDR, ETH_ANLT_WIDTH_REG1_AN_DEF_SPEC_WIDTH_4R_MASK, ETH_ANLT_WIDTH_REG1_AN_DEF_SPEC_WIDTH_4R_OFFSET, &newdef_get->an_def_spec_width[4]));
    CHECK(pmd_read_field(mss, ETH_ANLT_WIDTH_REG1_ADDR, ETH_ANLT_WIDTH_REG1_AN_DEF_SPEC_WIDTH_5R_MASK, ETH_ANLT_WIDTH_REG1_AN_DEF_SPEC_WIDTH_5R_OFFSET, &newdef_get->an_def_spec_width[5]));
    CHECK(pmd_read_field(mss, ETH_ANLT_WIDTH_REG1_ADDR, ETH_ANLT_WIDTH_REG1_AN_DEF_SPEC_WIDTH_6R_MASK, ETH_ANLT_WIDTH_REG1_AN_DEF_SPEC_WIDTH_6R_OFFSET, &newdef_get->an_def_spec_width[6]));
    CHECK(pmd_read_field(mss, ETH_ANLT_WIDTH_REG1_ADDR, ETH_ANLT_WIDTH_REG1_AN_DEF_SPEC_WIDTH_7R_MASK, ETH_ANLT_WIDTH_REG1_AN_DEF_SPEC_WIDTH_7R_OFFSET, &newdef_get->an_def_spec_width[7]));
    CHECK(pmd_read_field(mss, ETH_ANLT_WIDTH_REG2_ADDR, ETH_ANLT_WIDTH_REG2_AN_DEF_SPEC_WIDTH_8R_MASK, ETH_ANLT_WIDTH_REG2_AN_DEF_SPEC_WIDTH_8R_OFFSET, &newdef_get->an_def_spec_width[8]));
    CHECK(pmd_read_field(mss, ETH_ANLT_WIDTH_REG2_ADDR, ETH_ANLT_WIDTH_REG2_AN_DEF_SPEC_WIDTH_9R_MASK, ETH_ANLT_WIDTH_REG2_AN_DEF_SPEC_WIDTH_9R_OFFSET, &newdef_get->an_def_spec_width[9]));
    CHECK(pmd_read_field(mss, ETH_ANLT_WIDTH_REG2_ADDR, ETH_ANLT_WIDTH_REG2_AN_DEF_SPEC_WIDTH_10R_MASK, ETH_ANLT_WIDTH_REG2_AN_DEF_SPEC_WIDTH_10R_OFFSET, &newdef_get->an_def_spec_width[10]));
    CHECK(pmd_read_field(mss, ETH_ANLT_WIDTH_REG2_ADDR, ETH_ANLT_WIDTH_REG2_AN_DEF_SPEC_WIDTH_11R_MASK, ETH_ANLT_WIDTH_REG2_AN_DEF_SPEC_WIDTH_11R_OFFSET, &newdef_get->an_def_spec_width[11]));
    CHECK(pmd_read_field(mss, ETH_ANLT_WIDTH_REG2_ADDR, ETH_ANLT_WIDTH_REG2_AN_DEF_SPEC_WIDTH_12R_MASK, ETH_ANLT_WIDTH_REG2_AN_DEF_SPEC_WIDTH_12R_OFFSET, &newdef_get->an_def_spec_width[12]));
    CHECK(pmd_read_field(mss, ETH_ANLT_WIDTH_REG2_ADDR, ETH_ANLT_WIDTH_REG2_AN_DEF_SPEC_WIDTH_13R_MASK, ETH_ANLT_WIDTH_REG2_AN_DEF_SPEC_WIDTH_13R_OFFSET, &newdef_get->an_def_spec_width[13]));
    CHECK(pmd_read_field(mss, ETH_ANLT_WIDTH_REG2_ADDR, ETH_ANLT_WIDTH_REG2_AN_DEF_SPEC_WIDTH_14R_MASK, ETH_ANLT_WIDTH_REG2_AN_DEF_SPEC_WIDTH_14R_OFFSET, &newdef_get->an_def_spec_width[14]));
    CHECK(pmd_read_field(mss, ETH_ANLT_WIDTH_REG2_ADDR, ETH_ANLT_WIDTH_REG2_AN_DEF_SPEC_WIDTH_15R_MASK, ETH_ANLT_WIDTH_REG2_AN_DEF_SPEC_WIDTH_15R_OFFSET, &newdef_get->an_def_spec_width[15]));
    CHECK(pmd_read_field(mss, ETH_ANLT_WIDTH_REG3_ADDR, ETH_ANLT_WIDTH_REG3_AN_DEF_SPEC_WIDTH_16R_MASK, ETH_ANLT_WIDTH_REG3_AN_DEF_SPEC_WIDTH_16R_OFFSET, &newdef_get->an_def_spec_width[16]));
    CHECK(pmd_read_field(mss, ETH_ANLT_WIDTH_REG3_ADDR, ETH_ANLT_WIDTH_REG3_AN_DEF_SPEC_WIDTH_17R_MASK, ETH_ANLT_WIDTH_REG3_AN_DEF_SPEC_WIDTH_17R_OFFSET, &newdef_get->an_def_spec_width[17]));
    CHECK(pmd_read_field(mss, ETH_ANLT_WIDTH_REG3_ADDR, ETH_ANLT_WIDTH_REG3_AN_DEF_SPEC_WIDTH_18R_MASK, ETH_ANLT_WIDTH_REG3_AN_DEF_SPEC_WIDTH_18R_OFFSET, &newdef_get->an_def_spec_width[18]));
    CHECK(pmd_read_field(mss, ETH_ANLT_WIDTH_REG3_ADDR, ETH_ANLT_WIDTH_REG3_AN_DEF_SPEC_WIDTH_19R_MASK, ETH_ANLT_WIDTH_REG3_AN_DEF_SPEC_WIDTH_19R_OFFSET, &newdef_get->an_def_spec_width[19]));
    CHECK(pmd_read_field(mss, ETH_ANLT_WIDTH_REG3_ADDR, ETH_ANLT_WIDTH_REG3_AN_DEF_SPEC_WIDTH_20R_MASK, ETH_ANLT_WIDTH_REG3_AN_DEF_SPEC_WIDTH_20R_OFFSET, &newdef_get->an_def_spec_width[20]));
    CHECK(pmd_read_field(mss, ETH_ANLT_WIDTH_REG3_ADDR, ETH_ANLT_WIDTH_REG3_AN_DEF_SPEC_WIDTH_21R_MASK, ETH_ANLT_WIDTH_REG3_AN_DEF_SPEC_WIDTH_21R_OFFSET, &newdef_get->an_def_spec_width[21]));
    CHECK(pmd_read_field(mss, ETH_ANLT_WIDTH_REG3_ADDR, ETH_ANLT_WIDTH_REG3_AN_DEF_SPEC_WIDTH_22R_MASK, ETH_ANLT_WIDTH_REG3_AN_DEF_SPEC_WIDTH_22R_OFFSET, &newdef_get->an_def_spec_width[22]));
    CHECK(pmd_read_field(mss, ETH_ANLT_WIDTH_REG3_ADDR, ETH_ANLT_WIDTH_REG3_AN_DEF_SPEC_WIDTH_23R_MASK, ETH_ANLT_WIDTH_REG3_AN_DEF_SPEC_WIDTH_23R_OFFSET, &newdef_get->an_def_spec_width[23]));
    CHECK(pmd_read_field(mss, ETH_ANLT_WIDTH_REG4_ADDR, ETH_ANLT_WIDTH_REG4_AN_DEF_SPEC_WIDTH_24R_MASK, ETH_ANLT_WIDTH_REG4_AN_DEF_SPEC_WIDTH_24R_OFFSET, &newdef_get->an_def_spec_width[24]));
    CHECK(pmd_read_field(mss, ETH_ANLT_WIDTH_REG4_ADDR, ETH_ANLT_WIDTH_REG4_AN_DEF_SPEC_WIDTH_25R_MASK, ETH_ANLT_WIDTH_REG4_AN_DEF_SPEC_WIDTH_25R_OFFSET, &newdef_get->an_def_spec_width[25]));
    CHECK(pmd_read_field(mss, ETH_ANLT_WIDTH_REG4_ADDR, ETH_ANLT_WIDTH_REG4_AN_DEF_SPEC_WIDTH_26R_MASK, ETH_ANLT_WIDTH_REG4_AN_DEF_SPEC_WIDTH_26R_OFFSET, &newdef_get->an_def_spec_width[26]));
    CHECK(pmd_read_field(mss, ETH_ANLT_WIDTH_REG4_ADDR, ETH_ANLT_WIDTH_REG4_AN_DEF_SPEC_WIDTH_27R_MASK, ETH_ANLT_WIDTH_REG4_AN_DEF_SPEC_WIDTH_27R_OFFSET, &newdef_get->an_def_spec_width[27]));

    CHECK(pmd_read_field(mss, ETH_ANLT_RATE_REG1_ADDR, ETH_ANLT_RATE_REG1_AN_DEF_SPEC_RATE_0R_MASK, ETH_ANLT_RATE_REG1_AN_DEF_SPEC_RATE_0R_OFFSET, &newdef_get->an_def_spec_rate[0]));
    CHECK(pmd_read_field(mss, ETH_ANLT_RATE_REG1_ADDR, ETH_ANLT_RATE_REG1_AN_DEF_SPEC_RATE_1R_MASK, ETH_ANLT_RATE_REG1_AN_DEF_SPEC_RATE_1R_OFFSET, &newdef_get->an_def_spec_rate[1]));
    CHECK(pmd_read_field(mss, ETH_ANLT_RATE_REG1_ADDR, ETH_ANLT_RATE_REG1_AN_DEF_SPEC_RATE_2R_MASK, ETH_ANLT_RATE_REG1_AN_DEF_SPEC_RATE_2R_OFFSET, &newdef_get->an_def_spec_rate[2]));
    CHECK(pmd_read_field(mss, ETH_ANLT_RATE_REG1_ADDR, ETH_ANLT_RATE_REG1_AN_DEF_SPEC_RATE_3R_MASK, ETH_ANLT_RATE_REG1_AN_DEF_SPEC_RATE_3R_OFFSET, &newdef_get->an_def_spec_rate[3]));
    CHECK(pmd_read_field(mss, ETH_ANLT_RATE_REG1_ADDR, ETH_ANLT_RATE_REG1_AN_DEF_SPEC_RATE_4R_MASK, ETH_ANLT_RATE_REG1_AN_DEF_SPEC_RATE_4R_OFFSET, &newdef_get->an_def_spec_rate[4]));
    CHECK(pmd_read_field(mss, ETH_ANLT_RATE_REG1_ADDR, ETH_ANLT_RATE_REG1_AN_DEF_SPEC_RATE_5R_MASK, ETH_ANLT_RATE_REG1_AN_DEF_SPEC_RATE_5R_OFFSET, &newdef_get->an_def_spec_rate[5]));
    CHECK(pmd_read_field(mss, ETH_ANLT_RATE_REG1_ADDR, ETH_ANLT_RATE_REG1_AN_DEF_SPEC_RATE_6R_MASK, ETH_ANLT_RATE_REG1_AN_DEF_SPEC_RATE_6R_OFFSET, &newdef_get->an_def_spec_rate[6]));
    CHECK(pmd_read_field(mss, ETH_ANLT_RATE_REG1_ADDR, ETH_ANLT_RATE_REG1_AN_DEF_SPEC_RATE_7R_MASK, ETH_ANLT_RATE_REG1_AN_DEF_SPEC_RATE_7R_OFFSET, &newdef_get->an_def_spec_rate[7]));
    CHECK(pmd_read_field(mss, ETH_ANLT_RATE_REG1_ADDR, ETH_ANLT_RATE_REG1_AN_DEF_SPEC_RATE_8R_MASK, ETH_ANLT_RATE_REG1_AN_DEF_SPEC_RATE_8R_OFFSET, &newdef_get->an_def_spec_rate[8]));
    CHECK(pmd_read_field(mss, ETH_ANLT_RATE_REG1_ADDR, ETH_ANLT_RATE_REG1_AN_DEF_SPEC_RATE_9R_MASK, ETH_ANLT_RATE_REG1_AN_DEF_SPEC_RATE_9R_OFFSET, &newdef_get->an_def_spec_rate[9]));
    CHECK(pmd_read_field(mss, ETH_ANLT_RATE_REG2_ADDR, ETH_ANLT_RATE_REG2_AN_DEF_SPEC_RATE_10R_MASK, ETH_ANLT_RATE_REG2_AN_DEF_SPEC_RATE_10R_OFFSET, &newdef_get->an_def_spec_rate[10]));
    CHECK(pmd_read_field(mss, ETH_ANLT_RATE_REG2_ADDR, ETH_ANLT_RATE_REG2_AN_DEF_SPEC_RATE_11R_MASK, ETH_ANLT_RATE_REG2_AN_DEF_SPEC_RATE_11R_OFFSET, &newdef_get->an_def_spec_rate[11]));
    CHECK(pmd_read_field(mss, ETH_ANLT_RATE_REG2_ADDR, ETH_ANLT_RATE_REG2_AN_DEF_SPEC_RATE_12R_MASK, ETH_ANLT_RATE_REG2_AN_DEF_SPEC_RATE_12R_OFFSET, &newdef_get->an_def_spec_rate[12]));
    CHECK(pmd_read_field(mss, ETH_ANLT_RATE_REG2_ADDR, ETH_ANLT_RATE_REG2_AN_DEF_SPEC_RATE_13R_MASK, ETH_ANLT_RATE_REG2_AN_DEF_SPEC_RATE_13R_OFFSET, &newdef_get->an_def_spec_rate[13]));
    CHECK(pmd_read_field(mss, ETH_ANLT_RATE_REG2_ADDR, ETH_ANLT_RATE_REG2_AN_DEF_SPEC_RATE_14R_MASK, ETH_ANLT_RATE_REG2_AN_DEF_SPEC_RATE_14R_OFFSET, &newdef_get->an_def_spec_rate[14]));
    CHECK(pmd_read_field(mss, ETH_ANLT_RATE_REG2_ADDR, ETH_ANLT_RATE_REG2_AN_DEF_SPEC_RATE_15R_MASK, ETH_ANLT_RATE_REG2_AN_DEF_SPEC_RATE_15R_OFFSET, &newdef_get->an_def_spec_rate[15]));
    CHECK(pmd_read_field(mss, ETH_ANLT_RATE_REG2_ADDR, ETH_ANLT_RATE_REG2_AN_DEF_SPEC_RATE_16R_MASK, ETH_ANLT_RATE_REG2_AN_DEF_SPEC_RATE_16R_OFFSET, &newdef_get->an_def_spec_rate[16]));
    CHECK(pmd_read_field(mss, ETH_ANLT_RATE_REG2_ADDR, ETH_ANLT_RATE_REG2_AN_DEF_SPEC_RATE_17R_MASK, ETH_ANLT_RATE_REG2_AN_DEF_SPEC_RATE_17R_OFFSET, &newdef_get->an_def_spec_rate[17]));
    CHECK(pmd_read_field(mss, ETH_ANLT_RATE_REG2_ADDR, ETH_ANLT_RATE_REG2_AN_DEF_SPEC_RATE_18R_MASK, ETH_ANLT_RATE_REG2_AN_DEF_SPEC_RATE_18R_OFFSET, &newdef_get->an_def_spec_rate[18]));
    CHECK(pmd_read_field(mss, ETH_ANLT_RATE_REG2_ADDR, ETH_ANLT_RATE_REG2_AN_DEF_SPEC_RATE_19R_MASK, ETH_ANLT_RATE_REG2_AN_DEF_SPEC_RATE_19R_OFFSET, &newdef_get->an_def_spec_rate[19]));
    CHECK(pmd_read_field(mss, ETH_ANLT_RATE_REG3_ADDR, ETH_ANLT_RATE_REG3_AN_DEF_SPEC_RATE_20R_MASK, ETH_ANLT_RATE_REG3_AN_DEF_SPEC_RATE_20R_OFFSET, &newdef_get->an_def_spec_rate[20]));
    CHECK(pmd_read_field(mss, ETH_ANLT_RATE_REG3_ADDR, ETH_ANLT_RATE_REG3_AN_DEF_SPEC_RATE_21R_MASK, ETH_ANLT_RATE_REG3_AN_DEF_SPEC_RATE_21R_OFFSET, &newdef_get->an_def_spec_rate[21]));
    CHECK(pmd_read_field(mss, ETH_ANLT_RATE_REG3_ADDR, ETH_ANLT_RATE_REG3_AN_DEF_SPEC_RATE_22R_MASK, ETH_ANLT_RATE_REG3_AN_DEF_SPEC_RATE_22R_OFFSET, &newdef_get->an_def_spec_rate[22]));
    CHECK(pmd_read_field(mss, ETH_ANLT_RATE_REG3_ADDR, ETH_ANLT_RATE_REG3_AN_DEF_SPEC_RATE_23R_MASK, ETH_ANLT_RATE_REG3_AN_DEF_SPEC_RATE_23R_OFFSET, &newdef_get->an_def_spec_rate[23]));
    CHECK(pmd_read_field(mss, ETH_ANLT_RATE_REG3_ADDR, ETH_ANLT_RATE_REG3_AN_DEF_SPEC_RATE_24R_MASK, ETH_ANLT_RATE_REG3_AN_DEF_SPEC_RATE_24R_OFFSET, &newdef_get->an_def_spec_rate[24]));
    CHECK(pmd_read_field(mss, ETH_ANLT_RATE_REG3_ADDR, ETH_ANLT_RATE_REG3_AN_DEF_SPEC_RATE_25R_MASK, ETH_ANLT_RATE_REG3_AN_DEF_SPEC_RATE_25R_OFFSET, &newdef_get->an_def_spec_rate[25]));
    CHECK(pmd_read_field(mss, ETH_ANLT_RATE_REG3_ADDR, ETH_ANLT_RATE_REG3_AN_DEF_SPEC_RATE_26R_MASK, ETH_ANLT_RATE_REG3_AN_DEF_SPEC_RATE_26R_OFFSET, &newdef_get->an_def_spec_rate[26]));
    CHECK(pmd_read_field(mss, ETH_ANLT_RATE_REG3_ADDR, ETH_ANLT_RATE_REG3_AN_DEF_SPEC_RATE_27R_MASK, ETH_ANLT_RATE_REG3_AN_DEF_SPEC_RATE_27R_OFFSET, &newdef_get->an_def_spec_rate[27]));

    CHECK(pmd_read_field(mss, ETH_AN_PMA_DEF_REG1_ADDR, ETH_AN_PMA_DEF_REG1_AN_DEF_AN_RATE_MASK, ETH_AN_PMA_DEF_REG1_AN_DEF_AN_RATE_OFFSET, &newdef_get->an_def_an_rate));

    CHECK(pmd_read_field(mss, ETH_AN_NEWDEF_REG1_ADDR, ETH_AN_NEWDEF_REG1_NEWDEF1_LANE_NUM_U_MASK, ETH_AN_NEWDEF_REG1_NEWDEF1_LANE_NUM_U_OFFSET, &newdef_get->newdef1.lane_num_u));
    CHECK(pmd_read_field(mss, ETH_AN_NEWDEF_REG1_ADDR, ETH_AN_NEWDEF_REG1_NEWDEF1_LT_SEL_U_MASK, ETH_AN_NEWDEF_REG1_NEWDEF1_LT_SEL_U_OFFSET, &newdef_get->newdef1.lt_sel_u));
    CHECK(pmd_read_field(mss, ETH_AN_NEWDEF_REG1_ADDR, ETH_AN_NEWDEF_REG1_NEWDEF1_PAM_U_MASK, ETH_AN_NEWDEF_REG1_NEWDEF1_PAM_U_OFFSET, &newdef_get->newdef1.pam_u));
    CHECK(pmd_read_field(mss, ETH_AN_NEWDEF_REG1_ADDR, ETH_AN_NEWDEF_REG1_NEWDEF1_TIMER_SEL_U_MASK, ETH_AN_NEWDEF_REG1_NEWDEF1_TIMER_SEL_U_OFFSET, &newdef_get->newdef1.timer_sel_u));

    CHECK(pmd_read_field(mss, ETH_AN_NEWDEF_REG1_ADDR, ETH_AN_NEWDEF_REG1_NEWDEF2_LANE_NUM_U_MASK, ETH_AN_NEWDEF_REG1_NEWDEF2_LANE_NUM_U_OFFSET, &newdef_get->newdef2.lane_num_u));
    CHECK(pmd_read_field(mss, ETH_AN_NEWDEF_REG1_ADDR, ETH_AN_NEWDEF_REG1_NEWDEF2_LT_SEL_U_MASK, ETH_AN_NEWDEF_REG1_NEWDEF2_LT_SEL_U_OFFSET, &newdef_get->newdef2.lt_sel_u));
    CHECK(pmd_read_field(mss, ETH_AN_NEWDEF_REG1_ADDR, ETH_AN_NEWDEF_REG1_NEWDEF2_PAM_U_MASK, ETH_AN_NEWDEF_REG1_NEWDEF2_PAM_U_OFFSET, &newdef_get->newdef2.pam_u));
    CHECK(pmd_read_field(mss, ETH_AN_NEWDEF_REG1_ADDR, ETH_AN_NEWDEF_REG1_NEWDEF2_TIMER_SEL_U_MASK, ETH_AN_NEWDEF_REG1_NEWDEF2_TIMER_SEL_U_OFFSET, &newdef_get->newdef2.timer_sel_u));

    CHECK(pmd_read_field(mss, ETH_AN_NEWDEF_REG1_ADDR, ETH_AN_NEWDEF_REG1_NEWDEF3_LANE_NUM_U_MASK, ETH_AN_NEWDEF_REG1_NEWDEF3_LANE_NUM_U_OFFSET, &newdef_get->newdef3.lane_num_u));
    CHECK(pmd_read_field(mss, ETH_AN_NEWDEF_REG2_ADDR, ETH_AN_NEWDEF_REG2_NEWDEF3_LT_SEL_U_MASK, ETH_AN_NEWDEF_REG2_NEWDEF3_LT_SEL_U_OFFSET, &newdef_get->newdef3.lt_sel_u));
    CHECK(pmd_read_field(mss, ETH_AN_NEWDEF_REG2_ADDR, ETH_AN_NEWDEF_REG2_NEWDEF3_PAM_U_MASK, ETH_AN_NEWDEF_REG2_NEWDEF3_PAM_U_OFFSET, &newdef_get->newdef3.pam_u));
    CHECK(pmd_read_field(mss, ETH_AN_NEWDEF_REG2_ADDR, ETH_AN_NEWDEF_REG2_NEWDEF3_TIMER_SEL_U_MASK, ETH_AN_NEWDEF_REG2_NEWDEF3_TIMER_SEL_U_OFFSET, &newdef_get->newdef3.timer_sel_u));

    return AW_ERR_CODE_NONE;
}

int aw_pmd_4ln_anlt_auto_neg_rs_fec_int_ena_get (mss_access_t *mss,  uint32_t * an_rs_fec_int_ena) {
    CHECK(pmd_read_field(mss, ETH_AN_RESULT_REG1_ADDR, ETH_AN_RESULT_REG1_AN_RS_FEC_INT_ENA_MASK, ETH_AN_RESULT_REG1_AN_RS_FEC_INT_ENA_OFFSET, an_rs_fec_int_ena));

    return AW_ERR_CODE_NONE;
}

int aw_pmd_4ln_anlt_auto_neg_next_page_oui_compare_set(mss_access_t *mss, uint32_t np_expected_oui){
    //Set the OUI to compare against in the "recieved (OUI) tagged formatted Next Page"
    //ETC OUI = 0x6a737d
    //Broadcom OUI = 0x000AF7

    CHECK(pmd_write_field(mss, ETH_AN_CTRL_REG3_ADDR, ETH_AN_CTRL_REG3_ECON_CID_U_MASK, ETH_AN_CTRL_REG3_ECON_CID_U_OFFSET, np_expected_oui));

    return AW_ERR_CODE_NONE;

}

int aw_pmd_4ln_anlt_link_training_en_set (mss_access_t *mss, uint32_t en){
    CHECK(pmd_write_field(mss, ETH_LT_CTRL_ADDR, ETH_LT_CTRL_LT_MR_TRAINING_ENABLE_MASK, ETH_LT_CTRL_LT_MR_TRAINING_ENABLE_OFFSET, en));

    return AW_ERR_CODE_NONE;
}

int aw_pmd_4ln_anlt_link_training_en_get (mss_access_t *mss, uint32_t * en){
    CHECK(pmd_read_field(mss, ETH_LT_CTRL_ADDR, ETH_LT_CTRL_LT_MR_TRAINING_ENABLE_MASK, ETH_LT_CTRL_LT_MR_TRAINING_ENABLE_OFFSET, en));

    return AW_ERR_CODE_NONE;
}

//bfn
#if 0
int aw_pmd_4ln_anlt_link_training_config_set (mss_access_t *mss, uint32_t width, uint32_t clause, uint32_t mod){
    CHECK(pmd_write_field(mss, ETH_LT_NO_AN_CTRL_ADDR, ETH_LT_NO_AN_CTRL_LT_WIDTH_MASK, ETH_LT_NO_AN_CTRL_LT_WIDTH_OFFSET, width));

    CHECK(pmd_write_field(mss, ETH_LT_NO_AN_CTRL_ADDR, ETH_LT_NO_AN_CTRL_LT_CTRL_MASK, ETH_LT_NO_AN_CTRL_LT_CTRL_OFFSET, clause));

    //LT modulation config
    CHECK(pmd_write_field(mss, ETH_LT_SETTINGS_ADDR, ETH_LT_SETTINGS_LT_REG_FINAL_MOD_MASK, ETH_LT_SETTINGS_LT_REG_FINAL_MOD_OFFSET, mod));

    if (clause >= 3){
        CHECK(pmd_write_field(mss, ETH_LT_SETTINGS_ADDR, ETH_LT_SETTINGS_LT_REG_TRAINING_MOD_MASK, ETH_LT_SETTINGS_LT_REG_TRAINING_MOD_OFFSET, mod+2));
        CHECK(pmd_write_field(mss, ETH_LT_SETTINGS2_ADDR, ETH_LT_SETTINGS2_LT_REG_TRAINING_SWEEP_MOD_MASK, ETH_LT_SETTINGS2_LT_REG_TRAINING_SWEEP_MOD_OFFSET, mod+2));
        CHECK(pmd_write_field(mss, ETH_LT_SETTINGS2_ADDR, ETH_LT_SETTINGS2_LT_REG_TRAINING_INIT_MOD_MASK, ETH_LT_SETTINGS2_LT_REG_TRAINING_INIT_MOD_OFFSET, mod+2));
        CHECK(pmd_write_field(mss, RX_CNTRL_REG2_ADDR, RX_CNTRL_REG2_RX_GRAY_ENA_OVR_NT_MASK, RX_CNTRL_REG2_RX_GRAY_ENA_OVR_NT_OFFSET, 0));
        CHECK(pmd_write_field(mss, RX_CNTRL_REG2_ADDR, RX_CNTRL_REG2_RX_GRAY_ENA_NT_MASK, RX_CNTRL_REG2_RX_GRAY_ENA_NT_OFFSET, 0));
        CHECK(pmd_write_field(mss, TX_DATAPATH_REG2_ADDR, TX_DATAPATH_REG2_PAMCODE_OVR_EN_A_MASK, TX_DATAPATH_REG2_PAMCODE_OVR_EN_A_OFFSET, 0));
        CHECK(pmd_write_field(mss, TX_DATAPATH_REG1_ADDR, TX_DATAPATH_REG1_GRAY_CODE_ENABLE_A_MASK, TX_DATAPATH_REG1_GRAY_CODE_ENABLE_A_OFFSET, 0));
    }

    return AW_ERR_CODE_NONE;
}
#else
int aw_pmd_4ln_anlt_link_training_config_set (mss_access_t *mss, uint32_t width, uint32_t clause){
    CHECK(pmd_write_field(mss, ETH_LT_NO_AN_CTRL_ADDR, ETH_LT_NO_AN_CTRL_LT_WIDTH_MASK, ETH_LT_NO_AN_CTRL_LT_WIDTH_OFFSET, width));

    CHECK(pmd_write_field(mss, ETH_LT_NO_AN_CTRL_ADDR, ETH_LT_NO_AN_CTRL_LT_CTRL_MASK, ETH_LT_NO_AN_CTRL_LT_CTRL_OFFSET, clause));

    // test
    uint32_t mod = 0;
    //LT modulation config
    CHECK(pmd_write_field(mss, ETH_LT_SETTINGS_ADDR, ETH_LT_SETTINGS_LT_REG_FINAL_MOD_MASK, ETH_LT_SETTINGS_LT_REG_FINAL_MOD_OFFSET, mod));

    if (clause >= 3){
        CHECK(pmd_write_field(mss, ETH_LT_SETTINGS_ADDR, ETH_LT_SETTINGS_LT_REG_TRAINING_MOD_MASK, ETH_LT_SETTINGS_LT_REG_TRAINING_MOD_OFFSET, mod+2));
        CHECK(pmd_write_field(mss, ETH_LT_SETTINGS2_ADDR, ETH_LT_SETTINGS2_LT_REG_TRAINING_SWEEP_MOD_MASK, ETH_LT_SETTINGS2_LT_REG_TRAINING_SWEEP_MOD_OFFSET, mod+2));
        CHECK(pmd_write_field(mss, ETH_LT_SETTINGS2_ADDR, ETH_LT_SETTINGS2_LT_REG_TRAINING_INIT_MOD_MASK, ETH_LT_SETTINGS2_LT_REG_TRAINING_INIT_MOD_OFFSET, mod+2));
        CHECK(pmd_write_field(mss, RX_CNTRL_REG2_ADDR, RX_CNTRL_REG2_RX_GRAY_ENA_OVR_NT_MASK, RX_CNTRL_REG2_RX_GRAY_ENA_OVR_NT_OFFSET, 0));
        CHECK(pmd_write_field(mss, RX_CNTRL_REG2_ADDR, RX_CNTRL_REG2_RX_GRAY_ENA_NT_MASK, RX_CNTRL_REG2_RX_GRAY_ENA_NT_OFFSET, 0));
        CHECK(pmd_write_field(mss, TX_DATAPATH_REG2_ADDR, TX_DATAPATH_REG2_PAMCODE_OVR_EN_A_MASK, TX_DATAPATH_REG2_PAMCODE_OVR_EN_A_OFFSET, 0));
        CHECK(pmd_write_field(mss, TX_DATAPATH_REG1_ADDR, TX_DATAPATH_REG1_GRAY_CODE_ENABLE_A_MASK, TX_DATAPATH_REG1_GRAY_CODE_ENABLE_A_OFFSET, 0));
    }

    return AW_ERR_CODE_NONE;
}
#endif //0

int aw_pmd_4ln_anlt_link_training_config_get (mss_access_t *mss, uint32_t * width, uint32_t * clause){
    CHECK(pmd_read_field(mss, ETH_LT_NO_AN_CTRL_ADDR, ETH_LT_NO_AN_CTRL_LT_WIDTH_MASK, ETH_LT_NO_AN_CTRL_LT_WIDTH_OFFSET, width));
    CHECK(pmd_read_field(mss, ETH_LT_NO_AN_CTRL_ADDR, ETH_LT_NO_AN_CTRL_LT_CTRL_MASK, ETH_LT_NO_AN_CTRL_LT_CTRL_OFFSET, clause));

    return AW_ERR_CODE_NONE;
}

int aw_pmd_4ln_anlt_link_training_prbs_seed_set (mss_access_t *mss, uint32_t clause, uint32_t logical_lane){

    if (clause == 1){
        //CL72 single lane, set random seed
        CHECK(pmd_write_field(mss, ETH_LT_PRBS_ADDR, ETH_LT_PRBS_LT_SEED_11B_MASK, ETH_LT_PRBS_LT_SEED_11B_OFFSET, rand()&0x7FF));
    }
    else if (clause == 2){
        //CL92 (Table 92-5 PRBS parameters for each physical lane)
        if ( logical_lane == 0 ){
            //0b10101111110
            CHECK(pmd_write_field(mss, ETH_LT_PRBS_ADDR, ETH_LT_PRBS_LT_SEED_11B_MASK, ETH_LT_PRBS_LT_SEED_11B_OFFSET, 0x57e));
        } else if ( logical_lane == 1 ){
            //0b11001000101
            CHECK(pmd_write_field(mss, ETH_LT_PRBS_ADDR, ETH_LT_PRBS_LT_SEED_11B_MASK, ETH_LT_PRBS_LT_SEED_11B_OFFSET, 0x645));
        } else if ( logical_lane == 2 ){
            //0b11100101101
            CHECK(pmd_write_field(mss, ETH_LT_PRBS_ADDR, ETH_LT_PRBS_LT_SEED_11B_MASK, ETH_LT_PRBS_LT_SEED_11B_OFFSET, 0x72d));
        } else if ( logical_lane == 3 ){
            //0b11110110110
            CHECK(pmd_write_field(mss, ETH_LT_PRBS_ADDR, ETH_LT_PRBS_LT_SEED_11B_MASK, ETH_LT_PRBS_LT_SEED_11B_OFFSET, 0x7b6));
        }
    } else if (clause == 3 || clause == 4){
        //CL136/CL163  Table 136-8 Training patterns
        //Need to use %4 here for 400GBASE-CR8 / KR8
        if ( logical_lane%4 == 0 ){
            //0b0000010101011
            CHECK(pmd_write_field(mss, ETH_LT_PRBS_ADDR, ETH_LT_PRBS_LT_SEED_13B_MASK, ETH_LT_PRBS_LT_SEED_13B_OFFSET, 0x00ab));

        } else if ( logical_lane%4 == 1 ){
            //0b0011101000001
            CHECK(pmd_write_field(mss, ETH_LT_PRBS_ADDR, ETH_LT_PRBS_LT_SEED_13B_MASK, ETH_LT_PRBS_LT_SEED_13B_OFFSET, 0x0741));
        } else if ( logical_lane%4 == 2 ){
            //0b1001000101100
            CHECK(pmd_write_field(mss, ETH_LT_PRBS_ADDR, ETH_LT_PRBS_LT_SEED_13B_MASK, ETH_LT_PRBS_LT_SEED_13B_OFFSET, 0x122c));
        } else if ( logical_lane%4 == 3 ){
            //0b0100010000010
            CHECK(pmd_write_field(mss, ETH_LT_PRBS_ADDR, ETH_LT_PRBS_LT_SEED_13B_MASK, ETH_LT_PRBS_LT_SEED_13B_OFFSET, 0x0882));
        }
    }

    return AW_ERR_CODE_NONE;
}

int aw_pmd_4ln_anlt_link_training_start_set (mss_access_t *mss, uint32_t start){
    CHECK(pmd_write_field(mss, ETH_LT_NO_AN_CTRL_ADDR, ETH_LT_NO_AN_CTRL_LT_WITHOUT_AN_ENA_MASK, ETH_LT_NO_AN_CTRL_LT_WITHOUT_AN_ENA_OFFSET, start));

    return AW_ERR_CODE_NONE;
}

int aw_pmd_4ln_anlt_link_training_start_get (mss_access_t *mss, uint32_t * start){
    CHECK(pmd_read_field(mss, ETH_LT_NO_AN_CTRL_ADDR, ETH_LT_NO_AN_CTRL_LT_WITHOUT_AN_ENA_MASK, ETH_LT_NO_AN_CTRL_LT_WITHOUT_AN_ENA_OFFSET, start));

    return AW_ERR_CODE_NONE;
}

int aw_pmd_4ln_anlt_link_training_status_get (mss_access_t *mss, uint32_t * lt_running, uint32_t * lt_done, uint32_t * lt_training_failure, uint32_t * lt_rx_ready){
    CHECK(pmd_read_field(mss, ETH_LT_STAT_ADDR, ETH_LT_STAT_LT_RUNNING_MASK, ETH_LT_STAT_LT_RUNNING_OFFSET, lt_running));
    CHECK(pmd_read_field(mss, ETH_ANLT_STATUS_ADDR, ETH_ANLT_STATUS_LT_DONE_MASK, ETH_ANLT_STATUS_LT_DONE_OFFSET, lt_done));
    CHECK(pmd_read_field(mss, ETH_LT_STAT_ADDR, ETH_LT_STAT_LT_TRAINING_FAILURE_MASK, ETH_LT_STAT_LT_TRAINING_FAILURE_OFFSET, lt_training_failure));
    CHECK(pmd_read_field(mss, ETH_LT_STAT_ADDR, ETH_LT_STAT_LT_RX_READY_MASK, ETH_LT_STAT_LT_RX_READY_OFFSET, lt_rx_ready));

    return AW_ERR_CODE_NONE;
}

int aw_pmd_4ln_anlt_link_training_timeout_enable_set (mss_access_t *mss, uint32_t enable){
    enable = !enable; //This flag has to be inverted
    CHECK(pmd_write_field(mss, ETH_LT_SETTINGS_ADDR, ETH_LT_SETTINGS_LT_MAX_WAIT_DISABLE_C136B_MASK, ETH_LT_SETTINGS_LT_MAX_WAIT_DISABLE_C136B_OFFSET, enable));
    CHECK(pmd_write_field(mss, ETH_LT_SETTINGS_ADDR, ETH_LT_SETTINGS_LT_MAX_WAIT_DISABLE_C136_MASK, ETH_LT_SETTINGS_LT_MAX_WAIT_DISABLE_C136_OFFSET, enable));
    CHECK(pmd_write_field(mss, ETH_LT_SETTINGS_ADDR, ETH_LT_SETTINGS_LT_MAX_WAIT_DISABLE_C72_MASK, ETH_LT_SETTINGS_LT_MAX_WAIT_DISABLE_C72_OFFSET, enable));

    return AW_ERR_CODE_NONE;
}

int aw_pmd_4ln_ctrl_map_en_set(mss_access_t *mss, uint32_t anlt_ctrl_map_en){
    CHECK(pmd_write_field(mss, ETH_ANLT_CTRL_ADDR, ETH_ANLT_CTRL_MAP_EN_NT_MASK, ETH_ANLT_CTRL_MAP_EN_NT_OFFSET, anlt_ctrl_map_en));

    return AW_ERR_CODE_NONE;
}



int aw_pmd_4ln_refclk_termination_set(mss_access_t *mss, aw_refclk_term_mode_t lsrefbuf_term_mode){
    CHECK(pmd_write_field(mss, CMN_REFCLK_ADDR, CMN_REFCLK_LSREFBUF_TERM_MODE_NT_MASK   , CMN_REFCLK_LSREFBUF_TERM_MODE_NT_OFFSET   , lsrefbuf_term_mode ));
    return AW_ERR_CODE_NONE;
}

// NOTE: getter added
int aw_pmd_4ln_refclk_termination_get(mss_access_t *mss, aw_refclk_term_mode_t *lsrefbuf_term_mode){
    uint32_t rdval;
    CHECK(pmd_read_field(mss, CMN_REFCLK_ADDR, CMN_REFCLK_LSREFBUF_TERM_MODE_NT_MASK   , CMN_REFCLK_LSREFBUF_TERM_MODE_NT_OFFSET, &rdval));

    *lsrefbuf_term_mode = (aw_refclk_term_mode_t) rdval;

    return AW_ERR_CODE_NONE;
}


int aw_pmd_4ln_rx_termination_set(mss_access_t *mss, aw_acc_term_mode_t acc_term_mode){
    CHECK(pmd_write_field(mss, AFE_OCTERM_RX_ADDR, AFE_OCTERM_RX_ACC_TERM_MODE_NT_MASK, AFE_OCTERM_RX_ACC_TERM_MODE_NT_OFFSET, acc_term_mode ));
    return AW_ERR_CODE_NONE;
}

// NOTE: getter added
int aw_pmd_4ln_rx_termination_get(mss_access_t *mss, aw_acc_term_mode_t *acc_term_mode){
    uint32_t rdval;
    CHECK(pmd_read_field(mss, AFE_OCTERM_RX_ADDR, AFE_OCTERM_RX_ACC_TERM_MODE_NT_MASK, AFE_OCTERM_RX_ACC_TERM_MODE_NT_OFFSET, &rdval ));
    *acc_term_mode = (aw_acc_term_mode_t) rdval;
    return AW_ERR_CODE_NONE;
}

int aw_pmd_4ln_force_signal_detect_config_set(mss_access_t *mss, aw_force_sigdet_mode_t sigdet_mode){
    switch (sigdet_mode) {
        case AW_SIGDET_FORCE0:
            CHECK(pmd_write_field(mss, RX_SIGNAL_DETECT_REG3_ADDR, RX_SIGNAL_DETECT_REG3_FORCE_VALID_A_MASK, RX_SIGNAL_DETECT_REG3_FORCE_VALID_A_OFFSET, 0));
            CHECK(pmd_write_field(mss, RX_SIGNAL_DETECT_REG3_ADDR, RX_SIGNAL_DETECT_REG3_FORCE_INVALID_A_MASK, RX_SIGNAL_DETECT_REG3_FORCE_INVALID_A_OFFSET, 1));
            return AW_ERR_CODE_NONE;
        case AW_SIGDET_FORCE1:
            CHECK(pmd_write_field(mss, RX_SIGNAL_DETECT_REG3_ADDR, RX_SIGNAL_DETECT_REG3_FORCE_VALID_A_MASK, RX_SIGNAL_DETECT_REG3_FORCE_VALID_A_OFFSET, 1));
            CHECK(pmd_write_field(mss, RX_SIGNAL_DETECT_REG3_ADDR, RX_SIGNAL_DETECT_REG3_FORCE_INVALID_A_MASK, RX_SIGNAL_DETECT_REG3_FORCE_INVALID_A_OFFSET, 0));
            return AW_ERR_CODE_NONE;
        case AW_SIGDET_NORM:
            CHECK(pmd_write_field(mss, RX_SIGNAL_DETECT_REG3_ADDR, RX_SIGNAL_DETECT_REG3_FORCE_VALID_A_MASK, RX_SIGNAL_DETECT_REG3_FORCE_VALID_A_OFFSET, 0));
            CHECK(pmd_write_field(mss, RX_SIGNAL_DETECT_REG3_ADDR, RX_SIGNAL_DETECT_REG3_FORCE_INVALID_A_MASK, RX_SIGNAL_DETECT_REG3_FORCE_INVALID_A_OFFSET, 0));
            return AW_ERR_CODE_NONE;
        default:
            return AW_ERR_CODE_INVALID_ARG_VALUE;
    }
}

int aw_pmd_4ln_force_signal_detect_config_get(mss_access_t *mss, aw_force_sigdet_mode_t *sigdet_mode){
    uint32_t valid;
    uint32_t invalid;
    CHECK(pmd_read_field(mss, RX_SIGNAL_DETECT_REG3_ADDR, RX_SIGNAL_DETECT_REG3_FORCE_VALID_A_MASK, RX_SIGNAL_DETECT_REG3_FORCE_VALID_A_OFFSET, &valid));
    CHECK(pmd_read_field(mss, RX_SIGNAL_DETECT_REG3_ADDR, RX_SIGNAL_DETECT_REG3_FORCE_INVALID_A_MASK, RX_SIGNAL_DETECT_REG3_FORCE_INVALID_A_OFFSET, &invalid));
    if (valid == 0 && invalid == 1) {
        *sigdet_mode = AW_SIGDET_FORCE0;
        return AW_ERR_CODE_NONE;
    } else if (valid == 1 && invalid == 0) {
        *sigdet_mode = AW_SIGDET_FORCE1;
        return AW_ERR_CODE_NONE;
    } else if (valid == 0 && invalid == 0) {
        *sigdet_mode = AW_SIGDET_NORM;
        return AW_ERR_CODE_NONE;
    } else {
        return AW_ERR_CODE_BAD_STATE;
    }
}


int aw_pmd_4ln_tx_disable_set(mss_access_t *mss, uint32_t tx_disable){
    if (tx_disable == 1){
        CHECK(pmd_write_field(mss, TX_PHASE_FIFO_ADDR, TX_PHASE_FIFO_ELECIDLE_NT_MASK, TX_PHASE_FIFO_ELECIDLE_NT_OFFSET, 0xf));
        CHECK(pmd_write_field(mss, TX_PHASE_FIFO_ADDR, TX_PHASE_FIFO_BYPASS_ENA_A_MASK, TX_PHASE_FIFO_BYPASS_ENA_A_OFFSET, 1));
    } else if (tx_disable == 0) {
        CHECK(pmd_write_field(mss, TX_PHASE_FIFO_ADDR, TX_PHASE_FIFO_ELECIDLE_NT_MASK, TX_PHASE_FIFO_ELECIDLE_NT_OFFSET, 0));
        CHECK(pmd_write_field(mss, TX_PHASE_FIFO_ADDR, TX_PHASE_FIFO_BYPASS_ENA_A_MASK, TX_PHASE_FIFO_BYPASS_ENA_A_OFFSET, 0));
    } else {
        return AW_ERR_CODE_INVALID_ARG_VALUE;
    }
    return AW_ERR_CODE_NONE;
}

int aw_pmd_4ln_tx_disable_get(mss_access_t *mss, uint32_t *tx_disable){
    uint32_t elecidle;
    uint32_t bypass_ena;
    CHECK(pmd_read_field(mss, TX_PHASE_FIFO_ADDR, TX_PHASE_FIFO_ELECIDLE_NT_MASK, TX_PHASE_FIFO_ELECIDLE_NT_OFFSET, &elecidle));
    CHECK(pmd_read_field(mss, TX_PHASE_FIFO_ADDR, TX_PHASE_FIFO_BYPASS_ENA_A_MASK, TX_PHASE_FIFO_BYPASS_ENA_A_OFFSET, &bypass_ena));
    if (elecidle == 0x0f && bypass_ena == 1){
        *tx_disable = 1;
    } else {
        *tx_disable = 0;
    }
    return AW_ERR_CODE_NONE;
}

// NOTE: these are commented out, and not included in the API spec, but AW is
//       reserving in case there is a future usecase
// int aw_pmd_4ln_rx_disable_pin_override_set(mss_access_t *mss, int override_enable){}
// int aw_pmd_4ln_rx_disable_pin_override_get(mss_access_t *mss, int *override_enable){}
// int aw_pmd_4ln_rx_disable_set (mss_access_t *mss, int rx_disable){}
// int aw_pmd_4ln_rx_disable_get(mss_access_t *mss, int *rx_disable){}
// ENDNOTE

int aw_pmd_4ln_txfir_config_set(mss_access_t *mss, aw_txfir_config_t txfir_cfg, uint32_t fir_ovr_enable){
    uint32_t cm3_mask = 0x7;
    uint32_t cm2_mask = 0x7;
    uint32_t cm1_mask = 0x1F;
    uint32_t max_mask = 0x3F;
    uint32_t c1_mask = 0x1F;

    uint32_t max_ele;
    if (txfir_cfg.main_or_max == 0){
        max_ele = txfir_cfg.C1 + txfir_cfg.C0 + txfir_cfg.CM1 + txfir_cfg.CM2 + txfir_cfg.CM3 - 1;
    } else {
        max_ele = txfir_cfg.C0;
    }

    uint32_t fir = ((txfir_cfg.C1 & c1_mask) << 17);
    fir = fir + ((max_ele & max_mask) << 11);
    fir = fir + ((txfir_cfg.CM1 & cm1_mask) << 6);
    fir = fir + ((txfir_cfg.CM2 & cm2_mask) << 3);
    fir = fir + (txfir_cfg.CM3 & cm3_mask);

    CHECK(pmd_write_field(mss, FIR_ADDR, FIR_OVR_EN_A_MASK, FIR_OVR_EN_A_OFFSET, fir_ovr_enable));
    CHECK(pmd_write_field(mss, FIR_ADDR, FIR_VAL_A_MASK, FIR_VAL_A_OFFSET, fir));

    return AW_ERR_CODE_NONE;
}

int aw_pmd_4ln_txfir_config_get(mss_access_t *mss, aw_txfir_config_t *txfir_cfg){
    int cm3_mask = 0x00000007;
    int cm2_mask = 0x00000038;
    int cm1_mask = 0x000007c0;
    int max_mask = 0x0001f800;
    int c1_mask =  0x003e0000;

    uint32_t rdval;
    CHECK(pmd_read_field(mss, TX_FIR_RDREG_ADDR, TX_FIR_RDREG_AFE_VAL_NT_MASK, TX_FIR_RDREG_AFE_VAL_NT_OFFSET, &rdval));
    txfir_cfg->CM3 = (rdval & cm3_mask);
    txfir_cfg->CM2 = (rdval & cm2_mask) >> 3;
    txfir_cfg->CM1 = (rdval & cm1_mask) >> 6;
    int32_t max_ele = (rdval & max_mask) >> 11; // max_elements
    txfir_cfg->C1  = (rdval & c1_mask)  >> 17;
    if (txfir_cfg->main_or_max == 0){
    	txfir_cfg->C0 = max_ele + 1 - txfir_cfg->C1 - txfir_cfg->CM1 - txfir_cfg->CM2 - txfir_cfg->CM3;
    } else {
        txfir_cfg->C0 = max_ele;
    }
    return AW_ERR_CODE_NONE;
}
int aw_pmd_4ln_tx_tap_mode_get(uint32_t *max_rng_cm3, uint32_t *max_rng_cm2, uint32_t *max_rng_cm1, uint32_t *max_rng_c1, uint32_t *max_rng_c0){
    uint32_t cm3 = 4;
    uint32_t cm2 = 8;
    uint32_t cm1 = 24;
    uint32_t c1  = 24;
    uint32_t c0  = 60;
    *max_rng_cm3 = cm3;
    *max_rng_cm2 = cm2;
    *max_rng_cm1 = cm1;
    *max_rng_c1  = c1;
    *max_rng_c0  = c0;

    return AW_ERR_CODE_NONE;
}

int aw_pmd_4ln_tx_pam4_precoder_override_set(mss_access_t *mss, uint32_t en){
    // 1 to enable the PAM4 tx precoder, 0 to disable
    CHECK(pmd_write_field(mss, TX_DATAPATH_REG2_ADDR, TX_DATAPATH_REG2_PAMCODE_OVR_EN_A_MASK, TX_DATAPATH_REG2_PAMCODE_OVR_EN_A_OFFSET, en));
    return AW_ERR_CODE_NONE;
}

int aw_pmd_4ln_tx_pam4_precoder_override_get(mss_access_t *mss, uint32_t *en){
    // 1 = PAM4 tx precoder enabled
    // 0 = PAM4 tx precoder to disabled
    CHECK(pmd_read_field(mss, TX_DATAPATH_REG2_ADDR, TX_DATAPATH_REG2_PAMCODE_OVR_EN_A_MASK, TX_DATAPATH_REG2_PAMCODE_OVR_EN_A_OFFSET, en));
    return AW_ERR_CODE_NONE;
}

int aw_pmd_4ln_tx_pam4_precoder_enable_set(mss_access_t *mss, uint32_t gray_en, uint32_t plusd_en){
    CHECK(pmd_write_field(mss, TX_DATAPATH_REG1_ADDR, TX_DATAPATH_REG1_GRAY_CODE_ENABLE_A_MASK, TX_DATAPATH_REG1_GRAY_CODE_ENABLE_A_OFFSET, gray_en));
    CHECK(pmd_write_field(mss, TX_DATAPATH_REG1_ADDR, TX_DATAPATH_REG1_PLUSD_ENABLE_A_MASK, TX_DATAPATH_REG1_PLUSD_ENABLE_A_OFFSET, plusd_en));
    return AW_ERR_CODE_NONE;
}

int aw_pmd_4ln_tx_pam4_precoder_enable_get(mss_access_t *mss, uint32_t *gray_en, uint32_t *plusd_en){
    CHECK(pmd_read_field(mss, TX_DATAPATH_REG1_ADDR, TX_DATAPATH_REG1_GRAY_CODE_ENABLE_A_MASK, TX_DATAPATH_REG1_GRAY_CODE_ENABLE_A_OFFSET, gray_en));
    CHECK(pmd_read_field(mss, TX_DATAPATH_REG1_ADDR, TX_DATAPATH_REG1_PLUSD_ENABLE_A_MASK, TX_DATAPATH_REG1_PLUSD_ENABLE_A_OFFSET, plusd_en));
    return AW_ERR_CODE_NONE;
}

int aw_pmd_4ln_rx_pam4_precoder_override_set(mss_access_t *mss, uint32_t en){
    if (en == 1) {
        CHECK(pmd_write_field(mss, RX_CNTRL_REG2_ADDR, RX_CNTRL_REG2_RX_GRAY_ENA_OVR_NT_MASK, RX_CNTRL_REG2_RX_GRAY_ENA_OVR_NT_OFFSET, 0));
    } else {
        CHECK(pmd_write_field(mss, RX_CNTRL_REG2_ADDR, RX_CNTRL_REG2_RX_GRAY_ENA_OVR_NT_MASK, RX_CNTRL_REG2_RX_GRAY_ENA_OVR_NT_OFFSET, 1));
    }
    return AW_ERR_CODE_NONE;
}

int aw_pmd_4ln_rx_pam4_precoder_override_get(mss_access_t *mss, uint32_t *en){
    uint32_t ret;
    CHECK(pmd_read_field(mss, RX_CNTRL_REG2_ADDR, RX_CNTRL_REG2_RX_GRAY_ENA_OVR_NT_MASK, RX_CNTRL_REG2_RX_GRAY_ENA_OVR_NT_OFFSET, &ret));
    if (ret == 1) {
        *en = 0;
    } else {
        *en = 1;
    }
    return AW_ERR_CODE_NONE;
}

int aw_pmd_4ln_rx_pam4_precoder_enable_set(mss_access_t *mss, uint32_t gray_en, uint32_t plusd_en){
    CHECK(pmd_write_field(mss, RX_CNTRL_REG2_ADDR, RX_CNTRL_REG2_RX_GRAY_ENA_NT_MASK, RX_CNTRL_REG2_RX_GRAY_ENA_NT_OFFSET, gray_en));
    CHECK(pmd_write_field(mss, RX_DEMAPPER_ADDR, RX_DEMAPPER_PLUSD_ENABLE_A_MASK, RX_DEMAPPER_PLUSD_ENABLE_A_OFFSET, plusd_en));
    return AW_ERR_CODE_NONE;
}

int aw_pmd_4ln_rx_pam4_precoder_enable_get(mss_access_t *mss, uint32_t *gray_en, uint32_t *plusd_en){
    CHECK(pmd_read_field(mss, RX_CNTRL_REG2_ADDR, RX_CNTRL_REG2_RX_GRAY_ENA_NT_MASK, RX_CNTRL_REG2_RX_GRAY_ENA_NT_OFFSET, gray_en));
    CHECK(pmd_read_field(mss, RX_DEMAPPER_ADDR, RX_DEMAPPER_PLUSD_ENABLE_A_MASK, RX_DEMAPPER_PLUSD_ENABLE_A_OFFSET, plusd_en));
    return AW_ERR_CODE_NONE;
}

int aw_pmd_4ln_remote_loopback_set(mss_access_t *mss, uint32_t remote_loopback_enable){
    /**
     *
     * Function implementation has been deprecated, use aw_pmd_fep_clock_set & aw_pmd_fep_dat_set
     *
    **/
#if 0
    *mss = *mss;
    remote_loopback_enable = remote_loopback_enable;
#endif
    (void)mss;
    (void)remote_loopback_enable;

    USR_PRINTF("ERROR: aw_pmd_remote_loopback_set - function implementation has been deprecated, use aw_pmd_fep_clock_set & aw_pmd_fep_dat_set\n");
    return AW_ERR_CODE_NONE;
}

int aw_pmd_4ln_remote_loopback_get(mss_access_t *mss, uint32_t *remote_loopback_enable){
    // Return if the remote (far-end) loopback is enabled/disabled.
    CHECK(pmd_read_field(mss, TX_DATAPATH_REG1_ADDR, TX_DATAPATH_REG1_FEP_LOOPBACK_ENABLE_A_MASK, TX_DATAPATH_REG1_FEP_LOOPBACK_ENABLE_A_OFFSET, remote_loopback_enable));
    return AW_ERR_CODE_NONE;
}

int aw_pmd_4ln_fep_data_set(mss_access_t *mss, uint32_t datapath_en) {
    // NOTE: this function is only able to loopback the same physical lane to itself
    // Enable/disable remote (far-end) loopback (RX -> TX) datapath for the given lane.
    CHECK(pmd_write_field(mss, TX_LOOPBACK_CNTRL_ADDR, TX_LOOPBACK_CNTRL_ENA_NT_MASK, TX_LOOPBACK_CNTRL_ENA_NT_OFFSET, datapath_en));
    CHECK(pmd_write_field(mss, TX_DATAPATH_REG1_ADDR, TX_DATAPATH_REG1_FEP_LOOPBACK_ENABLE_A_MASK, TX_DATAPATH_REG1_FEP_LOOPBACK_ENABLE_A_OFFSET, datapath_en));
    CHECK(pmd_write_field(mss, TX_FEP_LOOPBACK_FIFO_TOP_ADDR, TX_FEP_LOOPBACK_FIFO_TOP_FIFO_ENABLE_A_MASK, TX_FEP_LOOPBACK_FIFO_TOP_FIFO_ENABLE_A_OFFSET, datapath_en));

    return AW_ERR_CODE_NONE;
}

int aw_pmd_4ln_fep_data_get(mss_access_t *mss, uint32_t *datapath_en) {
    // Return if the remote (far-end) loopback is enabled/disabled.
    // NOTE: This function is identical to `aw_pmd_remote_loopback_get`
    CHECK(pmd_read_field(mss, TX_DATAPATH_REG1_ADDR, TX_DATAPATH_REG1_FEP_LOOPBACK_ENABLE_A_MASK, TX_DATAPATH_REG1_FEP_LOOPBACK_ENABLE_A_OFFSET, datapath_en));

    return AW_ERR_CODE_NONE;
}

int aw_pmd_4ln_tx_dcd_iq_cal(mss_access_t *mss, uint32_t enable_d)
{
    int32_t poll_result;
    
    if (enable_d > 1){
        //Esxit if values passed is not binary, i.e. 1 or 0
        return AW_ERR_CODE_INVALID_ARG_VALUE;
    }

    // power up dcd
    CHECK(pmd_write_field(mss, PD_AFE_TX_ADDR, PD_AFE_TX_DCDIQ_BA_MASK, PD_AFE_TX_DCDIQ_BA_OFFSET, 1));
    CHECK(pmd_write_field(mss, RST_AFE_TX_ADDR, RST_AFE_TX_DCDIQ_BA_MASK, RST_AFE_TX_DCDIQ_BA_OFFSET, 1));
    CHECK(pmd_write_field(mss, TX_PHASE_ADAPT_REG1_ADDR, TX_PHASE_ADAPT_REG1_BYPASS_ENA_A_MASK, TX_PHASE_ADAPT_REG1_BYPASS_ENA_A_OFFSET, 0));

    // modify cal init parameters
    CHECK(pmd_write_field(mss, TX_PHASE_ADAPT_REG4_ADDR, TX_PHASE_ADAPT_REG4_MAX_ITER_NT_MASK, TX_PHASE_ADAPT_REG4_MAX_ITER_NT_OFFSET, 170));
    // Calibrate 0
    CHECK(pmd_write_field(mss, TX_PHASE_ADAPT_REG2_ADDR, TX_PHASE_ADAPT_REG2_TYPE_NT_MASK, TX_PHASE_ADAPT_REG2_TYPE_NT_OFFSET, 0));
    CHECK(pmd_write_field(mss, TX_PHASE_ADAPT_REG2_ADDR, TX_PHASE_ADAPT_REG2_REQ_A_MASK, TX_PHASE_ADAPT_REG2_REQ_A_OFFSET, 1));
    poll_result = pmd_poll_field(mss, TX_PHASE_ADAPT_RDREG_ADDR, TX_PHASE_ADAPT_RDREG_ACK_MASK, TX_PHASE_ADAPT_RDREG_ACK_OFFSET, 1, 1000);
    if (poll_result == -1) {
        return AW_ERR_CODE_POLL_TIMEOUT;
    }
    CHECK(pmd_write_field(mss, TX_PHASE_ADAPT_REG2_ADDR, TX_PHASE_ADAPT_REG2_REQ_A_MASK, TX_PHASE_ADAPT_REG2_REQ_A_OFFSET, 0));
    poll_result = pmd_poll_field(mss, TX_PHASE_ADAPT_RDREG_ADDR, TX_PHASE_ADAPT_RDREG_ACK_MASK, TX_PHASE_ADAPT_RDREG_ACK_OFFSET, 0, 1000);
    if (poll_result == -1) {
        return AW_ERR_CODE_POLL_TIMEOUT;
    }
    //Calibrate 1
    CHECK(pmd_write_field(mss, TX_PHASE_ADAPT_REG2_ADDR, TX_PHASE_ADAPT_REG2_TYPE_NT_MASK, TX_PHASE_ADAPT_REG2_TYPE_NT_OFFSET, 1));
    CHECK(pmd_write_field(mss, TX_PHASE_ADAPT_REG2_ADDR, TX_PHASE_ADAPT_REG2_REQ_A_MASK, TX_PHASE_ADAPT_REG2_REQ_A_OFFSET, 1));
    poll_result = pmd_poll_field(mss, TX_PHASE_ADAPT_RDREG_ADDR, TX_PHASE_ADAPT_RDREG_ACK_MASK, TX_PHASE_ADAPT_RDREG_ACK_OFFSET, 1, 1000);
    if (poll_result == -1) {
        return AW_ERR_CODE_POLL_TIMEOUT;
    }
    CHECK(pmd_write_field(mss, TX_PHASE_ADAPT_REG2_ADDR, TX_PHASE_ADAPT_REG2_REQ_A_MASK, TX_PHASE_ADAPT_REG2_REQ_A_OFFSET, 0));
    poll_result = pmd_poll_field(mss, TX_PHASE_ADAPT_RDREG_ADDR, TX_PHASE_ADAPT_RDREG_ACK_MASK, TX_PHASE_ADAPT_RDREG_ACK_OFFSET, 0, 1000);
    if (poll_result == -1) {
        return AW_ERR_CODE_POLL_TIMEOUT;
    }
    //Calibrate 2
    CHECK(pmd_write_field(mss, TX_PHASE_ADAPT_REG2_ADDR, TX_PHASE_ADAPT_REG2_TYPE_NT_MASK, TX_PHASE_ADAPT_REG2_TYPE_NT_OFFSET, 2));
    CHECK(pmd_write_field(mss, TX_PHASE_ADAPT_REG2_ADDR, TX_PHASE_ADAPT_REG2_REQ_A_MASK, TX_PHASE_ADAPT_REG2_REQ_A_OFFSET, 1));
    poll_result = pmd_poll_field(mss, TX_PHASE_ADAPT_RDREG_ADDR, TX_PHASE_ADAPT_RDREG_ACK_MASK, TX_PHASE_ADAPT_RDREG_ACK_OFFSET, 1, 1000);
    if (poll_result == -1) {
        return AW_ERR_CODE_POLL_TIMEOUT;
    }
    CHECK(pmd_write_field(mss, TX_PHASE_ADAPT_REG2_ADDR, TX_PHASE_ADAPT_REG2_REQ_A_MASK, TX_PHASE_ADAPT_REG2_REQ_A_OFFSET, 0));
    poll_result = pmd_poll_field(mss, TX_PHASE_ADAPT_RDREG_ADDR, TX_PHASE_ADAPT_RDREG_ACK_MASK, TX_PHASE_ADAPT_RDREG_ACK_OFFSET, 1, 1000);
    if (poll_result == -1) {
        return AW_ERR_CODE_POLL_TIMEOUT;
    }
    //Power down dcd
    CHECK(pmd_write_field(mss, PD_AFE_TX_ADDR, PD_AFE_TX_DCDIQ_BA_MASK, PD_AFE_TX_DCDIQ_BA_OFFSET, 1));
    CHECK(pmd_write_field(mss, RST_AFE_TX_ADDR, RST_AFE_TX_DCDIQ_BA_MASK, RST_AFE_TX_DCDIQ_BA_OFFSET, 1));
    CHECK(pmd_write_field(mss, TX_FEP_LOOPBACK_FIFO_TOP_ADDR, TX_FEP_LOOPBACK_FIFO_TOP_FIFO_ENABLE_A_MASK, TX_FEP_LOOPBACK_FIFO_TOP_FIFO_ENABLE_A_OFFSET, enable_d));
    CHECK(pmd_write_field(mss, TX_DATAPATH_REG1_ADDR, TX_DATAPATH_REG1_FEP_LOOPBACK_ENABLE_A_MASK, TX_DATAPATH_REG1_FEP_LOOPBACK_ENABLE_A_OFFSET, enable_d));
    return AW_ERR_CODE_NONE;        
}

int aw_pmd_4ln_fep_clock_set(mss_access_t *mss, uint8_t clock_en) {
    CHECK(pmd_write_field(mss, TX_LOOPBACK_CNTRL_ADDR, TX_LOOPBACK_CNTRL_ENA_NT_MASK, TX_LOOPBACK_CNTRL_ENA_NT_OFFSET, clock_en));
    CHECK(pmd_write_field(mss, LOOPBACK_CNTRL_ADDR, LOOPBACK_CNTRL_ENA_NT_MASK, LOOPBACK_CNTRL_ENA_NT_OFFSET, clock_en));
    CHECK(pmd_write_field(mss, LOOPBACK_CNTRL_ADDR, LOOPBACK_CNTRL_TX_BITCK_LOOPBACK_ENA_NT_MASK, LOOPBACK_CNTRL_TX_BITCK_LOOPBACK_ENA_NT_OFFSET, clock_en));
    CHECK(pmd_write_field(mss, LOOPBACK_CNTRL_ADDR, LOOPBACK_CNTRL_RX_BITCK_LOOPBACK_ENA_NT_MASK, LOOPBACK_CNTRL_RX_BITCK_LOOPBACK_ENA_NT_OFFSET, clock_en));
    
    CHECK(pmd_write_field(mss, SEQ_CNTRL_TX_ADDR, SEQ_CNTRL_TX_POSTDIV_READY_A_MASK, SEQ_CNTRL_TX_POSTDIV_READY_A_OFFSET, 0));
    CHECK(pmd_write_field(mss, SEQ_CNTRL_TX_ADDR, SEQ_CNTRL_TX_POSTDIV_READY_A_MASK, SEQ_CNTRL_TX_POSTDIV_READY_A_OFFSET, 1));
    
    if (clock_en != 0xff){
        aw_pmd_4ln_tx_dcd_iq_cal(mss, clock_en);    
    }
    return AW_ERR_CODE_NONE;
}

int aw_pmd_4ln_tx_postdiv_loopback_ena_set(mss_access_t *mss, uint8_t postdiv_loopback_ena) {
    CHECK(pmd_write_field(mss, TX_LOOPBACK_CNTRL_ADDR, TX_LOOPBACK_CNTRL_POSTDIV_LOOPBACK_ENA_A_MASK, TX_LOOPBACK_CNTRL_POSTDIV_LOOPBACK_ENA_A_OFFSET, postdiv_loopback_ena));

    // Pulse SEQ_CNTRL_TX_POSTDIV_READY_A
    CHECK(pmd_write_field(mss, SEQ_CNTRL_TX_ADDR, SEQ_CNTRL_TX_POSTDIV_READY_A_MASK, SEQ_CNTRL_TX_POSTDIV_READY_A_OFFSET, 0));
    USR_SLEEP(1);
    CHECK(pmd_write_field(mss, SEQ_CNTRL_TX_ADDR, SEQ_CNTRL_TX_POSTDIV_READY_A_MASK, SEQ_CNTRL_TX_POSTDIV_READY_A_OFFSET, 1));

    return AW_ERR_CODE_NONE;
}

int aw_pmd_4ln_tx_postdiv_loopback_ena_get(mss_access_t *mss, uint8_t *postdiv_loopback_ena) {
    CHECK(pmd_read_field(mss, TX_LOOPBACK_CNTRL_ADDR, TX_LOOPBACK_CNTRL_POSTDIV_LOOPBACK_ENA_A_MASK, TX_LOOPBACK_CNTRL_POSTDIV_LOOPBACK_ENA_A_OFFSET, (uint32_t *)postdiv_loopback_ena));

    return AW_ERR_CODE_NONE;
}

int aw_pmd_4ln_fep_clock_get(mss_access_t *mss, uint32_t *clock_en) {
    CHECK(pmd_read_field(mss, LOOPBACK_CNTRL_ADDR, LOOPBACK_CNTRL_TX_BITCK_LOOPBACK_ENA_NT_MASK, LOOPBACK_CNTRL_TX_BITCK_LOOPBACK_ENA_NT_OFFSET, clock_en));

    return AW_ERR_CODE_NONE;
}

int aw_pmd_4ln_analog_loopback_set(mss_access_t *mss, uint32_t analog_loopback_enable){
    // Enable/disable analog local (near-end) loopback (TX -> RX) for the given lane.
    // This loopback must be configured after rate/power state change
    // (after irst_*_ln[#] signals are asserted and the subsequent tasks
    // initiated by the assertion of the reset signals are carried out). Lane
    // remapping must not be used.

    // Set Analog Loopback 
    CHECK(pmd_write_field(mss, RX_ADAPT_VGA_OFFSET_CTLE_ADDR, RX_ADAPT_VGA_OFFSET_CTLE_ISOLATE_A_MASK, RX_ADAPT_VGA_OFFSET_CTLE_ISOLATE_A_OFFSET, analog_loopback_enable));
    CHECK(pmd_write_field(mss, LOOPBACK_CNTRL_ADDR, LOOPBACK_CNTRL_ENA_NT_MASK, LOOPBACK_CNTRL_ENA_NT_OFFSET, analog_loopback_enable));
    CHECK(pmd_write_field(mss, LOOPBACK_CNTRL_ADDR, LOOPBACK_CNTRL_TX_NES_LOOPBACK_ENA_NT_MASK, LOOPBACK_CNTRL_TX_NES_LOOPBACK_ENA_NT_OFFSET, analog_loopback_enable));
    CHECK(pmd_write_field(mss, LOOPBACK_CNTRL_ADDR, LOOPBACK_CNTRL_RX_NES_LOOPBACK_ENA_NT_MASK, LOOPBACK_CNTRL_RX_NES_LOOPBACK_ENA_NT_OFFSET, analog_loopback_enable));
    CHECK(pmd_write_field(mss, NES_MODE_ADDR, NES_MODE_MASK, NES_MODE_OFFSET, analog_loopback_enable));

    // Call Force Signal Detect Configuration Set
    if (analog_loopback_enable == 0){
        CHECK(aw_pmd_4ln_force_signal_detect_config_set(mss, AW_SIGDET_NORM));
    } else if (analog_loopback_enable == 1){
        CHECK(aw_pmd_4ln_force_signal_detect_config_set(mss, AW_SIGDET_FORCE1));
    }

    return AW_ERR_CODE_NONE;
}
int aw_pmd_4ln_analog_loopback_get(mss_access_t *mss, uint32_t *analog_loopback_enable){
    // Return if the analog local (near-end) loopback is enabled/disabled
    CHECK(pmd_read_field(mss, LOOPBACK_CNTRL_ADDR, LOOPBACK_CNTRL_TX_NES_LOOPBACK_ENA_NT_MASK, LOOPBACK_CNTRL_TX_NES_LOOPBACK_ENA_NT_OFFSET, analog_loopback_enable));
    return AW_ERR_CODE_NONE;
}

int aw_pmd_4ln_fes_loopback_set(mss_access_t *mss, uint32_t fes_loopback_enable) {
    CHECK(pmd_write_field(mss, LOOPBACK_CNTRL_ADDR, LOOPBACK_CNTRL_ENA_NT_MASK, LOOPBACK_CNTRL_ENA_NT_OFFSET, fes_loopback_enable));
    CHECK(pmd_write_field(mss, LOOPBACK_CNTRL_ADDR, LOOPBACK_CNTRL_RX_BITCK_LOOPBACK_ENA_NT_MASK, LOOPBACK_CNTRL_RX_BITCK_LOOPBACK_ENA_NT_OFFSET, fes_loopback_enable));
    CHECK(pmd_write_field(mss, LOOPBACK_CNTRL_ADDR, LOOPBACK_CNTRL_RX_FES_LOOPBACK_ENA_NT_MASK, LOOPBACK_CNTRL_RX_FES_LOOPBACK_ENA_NT_OFFSET, fes_loopback_enable));
    CHECK(pmd_write_field(mss, LOOPBACK_CNTRL_ADDR, LOOPBACK_CNTRL_TX_FES_LOOPBACK_ENA_NT_MASK, LOOPBACK_CNTRL_TX_FES_LOOPBACK_ENA_NT_OFFSET, fes_loopback_enable));
    return AW_ERR_CODE_NONE;
}

int aw_pmd_4ln_fes_loopback_get(mss_access_t *mss, uint32_t *fes_loopback_enable) {
    CHECK(pmd_read_field(mss, LOOPBACK_CNTRL_ADDR, LOOPBACK_CNTRL_TX_FES_LOOPBACK_ENA_NT_MASK, LOOPBACK_CNTRL_TX_FES_LOOPBACK_ENA_NT_OFFSET, fes_loopback_enable));
    return AW_ERR_CODE_NONE;
}

int aw_pmd_4ln_nep_loopback_set(mss_access_t *mss, uint32_t nep_loopback_enable){
    // Enable/disable analog local (near-end) loopback (TX -> RX) for the given lane.
    CHECK(pmd_write_field(mss, RX_ITR_DPLL_DLPF_REG5_ADDR, RX_ITR_DPLL_DLPF_REG5_INT_BYPASS_NT_MASK, RX_ITR_DPLL_DLPF_REG5_INT_BYPASS_NT_OFFSET, 0));
    CHECK(pmd_write_field(mss, RX_ITR_DPLL_DLPF_REG3_ADDR, RX_ITR_DPLL_DLPF_REG3_BYPASS_ENA_A_MASK, RX_ITR_DPLL_DLPF_REG3_BYPASS_ENA_A_OFFSET, nep_loopback_enable));
    CHECK(pmd_write_field(mss, RX_SIGNAL_DETECT_REG3_ADDR, RX_SIGNAL_DETECT_REG3_DPLL_ADAPT_START_OVR_A_MASK, RX_SIGNAL_DETECT_REG3_DPLL_ADAPT_START_OVR_A_OFFSET, nep_loopback_enable));
    CHECK(pmd_write_field(mss, RX_SIGNAL_DETECT_REG3_ADDR, RX_SIGNAL_DETECT_REG3_DPLL_ADAPT_START_OVREN_A_MASK, RX_SIGNAL_DETECT_REG3_DPLL_ADAPT_START_OVREN_A_OFFSET, nep_loopback_enable));
    CHECK(pmd_write_field(mss, SEQ_CNTRL_RX_ADDR, SEQ_CNTRL_RX_NEP_LOOPBACK_ENA_A_MASK, SEQ_CNTRL_RX_NEP_LOOPBACK_ENA_A_OFFSET, nep_loopback_enable));
    return AW_ERR_CODE_NONE;
}

int aw_pmd_4ln_nep_loopback_get(mss_access_t *mss, uint32_t *nep_loopback_enable){
    // Return if the NEP loopback is enabled/disabled
    CHECK(pmd_read_field(mss, SEQ_CNTRL_RX_ADDR, SEQ_CNTRL_RX_NEP_LOOPBACK_ENA_A_MASK, SEQ_CNTRL_RX_NEP_LOOPBACK_ENA_A_OFFSET, nep_loopback_enable));
    return AW_ERR_CODE_NONE;
}

int aw_pmd_4ln_tx_polarity_set(mss_access_t *mss, uint32_t tx_pol_flip){
    // Set tx polarity for a given lane.
    // 1 for inverted tx_polarity
    // 0 for normal operation
    CHECK(pmd_write_field(mss, TX_DATAPATH_REG2_ADDR, TX_DATAPATH_REG2_INVERT_ENABLE_A_MASK, TX_DATAPATH_REG2_INVERT_ENABLE_A_OFFSET, tx_pol_flip));
    return AW_ERR_CODE_NONE;
}

int aw_pmd_4ln_tx_polarity_get(mss_access_t *mss, uint32_t *tx_pol_flip){
    // 1 for inverted tx_polarity
    // 0 for normal operation
    CHECK(pmd_read_field(mss, TX_DATAPATH_REG2_ADDR, TX_DATAPATH_REG2_INVERT_ENABLE_A_MASK, TX_DATAPATH_REG2_INVERT_ENABLE_A_OFFSET, tx_pol_flip));
    return AW_ERR_CODE_NONE;
}

int aw_pmd_4ln_rx_polarity_set(mss_access_t *mss, uint32_t rx_pol_flip){
    // 1 for inverted rx_polarity
    // 0 for normal operation
    CHECK(pmd_write_field(mss, RX_DEMAPPER_ADDR, RX_DEMAPPER_INVERT_ENABLE_A_MASK, RX_DEMAPPER_INVERT_ENABLE_A_OFFSET, rx_pol_flip));
    return AW_ERR_CODE_NONE;
}

int aw_pmd_4ln_rx_polarity_get(mss_access_t *mss, uint32_t *rx_pol_flip){
    // 1 for inverted rx_polarity
    // 0 for normal operation
    CHECK(pmd_read_field(mss, RX_DEMAPPER_ADDR, RX_DEMAPPER_INVERT_ENABLE_A_MASK, RX_DEMAPPER_INVERT_ENABLE_A_OFFSET, rx_pol_flip));
    return AW_ERR_CODE_NONE;
}

int aw_pmd_4ln_tx_hbridge_set(mss_access_t *mss, tx_hbridge_t *tx_hbridge_st){
    // Set txhbridge, used to adjust Tx PAM4 eye linearity.
    if (tx_hbridge_st->bias_adj_en == 1){
        CHECK(pmd_write_field(mss, TX_HBRIDGE_ADDR, TX_HBRIDGE_BIAS_ADJ_NT_MASK, TX_HBRIDGE_BIAS_ADJ_NT_OFFSET, tx_hbridge_st->bias_adj));
    }
    if (tx_hbridge_st->rlm_ovr_en == 1){
        CHECK(pmd_write_field(mss, TX_HBRIDGE_ADDR, TX_HBRIDGE_LSB_OVR_ENA_NT_MASK, TX_HBRIDGE_LSB_OVR_ENA_NT_OFFSET, tx_hbridge_st->rlm_ovr));
    }
    CHECK(pmd_write_field(mss, TX_DETECTRX_REG1_ADDR, TX_DETECTRX_REG1_HB_SWING_NORMAL_VAL_NT_MASK, TX_DETECTRX_REG1_HB_SWING_NORMAL_VAL_NT_OFFSET, tx_hbridge_st->msb));
    CHECK(pmd_write_field(mss, TX_HBRIDGE_ADDR, TX_HBRIDGE_LSB_OVR_NT_MASK, TX_HBRIDGE_LSB_OVR_NT_OFFSET, tx_hbridge_st->lsb));
    return AW_ERR_CODE_NONE;
}

int aw_pmd_4ln_tx_hbridge_get(mss_access_t *mss, uint32_t *msb, uint32_t *lsb){
    CHECK(pmd_read_field(mss, TX_HBRIDGE_ADDR, TX_HBRIDGE_BIAS_ADJ_NT_MASK, TX_HBRIDGE_BIAS_ADJ_NT_OFFSET, msb));
    CHECK(pmd_read_field(mss, TX_HBRIDGE_ADDR, TX_HBRIDGE_LSB_OVR_NT_MASK, TX_HBRIDGE_LSB_OVR_NT_OFFSET, lsb));
    return AW_ERR_CODE_NONE;
}


// Everything previously defined in the given lane_cfg_t struct was not
// applicable. We are reserving these functions here incase there is a future
// usecase, if not, they will be removed.
// int aw_pmd_4ln_lane_cfg_set(mss_access_t *mss, lane_cfg_t lane_cfg){}
// int aw_pmd_4ln_lane_cfg_get(mss_access_t *mss, lane_cfg_t *lane_cfg){}

int aw_pmd_4ln_rx_dfe_adapt_set(mss_access_t *mss, uint32_t dfe_adapt_enable){
    CHECK(pmd_write_field(mss, RX_EQDFE_ADDR, RX_EQDFE_DFE_ENABLE_NT_MASK, RX_EQDFE_DFE_ENABLE_NT_OFFSET, dfe_adapt_enable));
    return AW_ERR_CODE_NONE;
}

int aw_pmd_4ln_rx_dfe_adapt_get(mss_access_t *mss, uint32_t *dfe_adapt_enable){
    CHECK(pmd_read_field(mss, RX_EQDFE_ADDR, RX_EQDFE_DFE_ENABLE_NT_MASK, RX_EQDFE_DFE_ENABLE_NT_OFFSET, dfe_adapt_enable));
    return AW_ERR_CODE_NONE;
}

// Enables or disables CTLE adaptation. If ctle adapt is disabled, ctle boost
// code will default to ctle_boost_a.
// If ctle adapt is enabled, initial ctle boost value will default to 0.

int aw_pmd_4ln_rx_ctle_adapt_set(mss_access_t *mss, uint32_t ctle_adapt_enable, uint32_t ctle_boost_a){
    uint32_t ccg_ctle;
    CHECK(pmd_read_field(mss, RX_CCG_ADDR, RX_CCG_AGC_ADAPT_ENA_A_MASK, RX_CCG_AGC_ADAPT_ENA_A_OFFSET, &ccg_ctle));
    CHECK(pmd_write_field(mss, RX_CCG_ADDR, RX_CCG_CTLE_ADAPT_ENA_A_MASK, RX_CCG_CTLE_ADAPT_ENA_A_OFFSET, 1));

    if (ctle_adapt_enable) {
        CHECK(pmd_write_field(mss, RX_CTLE_ADAPT_BYPASS_ADDR, RX_CTLE_ADAPT_BYPASS_SETTING_A_MASK, RX_CTLE_ADAPT_BYPASS_SETTING_A_OFFSET, 0));
        CHECK(pmd_write_field(mss, RX_CTLE_ADAPT_BYPASS_ADDR, RX_CTLE_ADAPT_BYPASS_ENABLE_A_MASK, RX_CTLE_ADAPT_BYPASS_ENABLE_A_OFFSET, 0));
        CHECK(pmd_write_field(mss, DISABLE_CTLE_ADAPT_ADDR, DISABLE_CTLE_ADAPT_MASK, DISABLE_CTLE_ADAPT_OFFSET, 0));
    } else {
        CHECK(pmd_write_field(mss, RX_CTLE_ADAPT_BYPASS_ADDR, RX_CTLE_ADAPT_BYPASS_SETTING_A_MASK, RX_CTLE_ADAPT_BYPASS_SETTING_A_OFFSET, ctle_boost_a));
        CHECK(pmd_write_field(mss, RX_CTLE_ADAPT_BYPASS_ADDR, RX_CTLE_ADAPT_BYPASS_ENABLE_A_MASK, RX_CTLE_ADAPT_BYPASS_ENABLE_A_OFFSET, 1));
        CHECK(pmd_write_field(mss, DISABLE_CTLE_ADAPT_ADDR, DISABLE_CTLE_ADAPT_MASK, DISABLE_CTLE_ADAPT_OFFSET, 1));
    }
    
    CHECK(pmd_write_field(mss, RX_CCG_ADDR, RX_CCG_CTLE_ADAPT_ENA_A_MASK, RX_CCG_CTLE_ADAPT_ENA_A_OFFSET, ccg_ctle));
    return AW_ERR_CODE_NONE;
}

int aw_pmd_4ln_rx_ctle_adapt_get(mss_access_t *mss, uint32_t *ctle_adapt_enable, uint32_t *ctle_boost_a){
    uint32_t bypass_en;
    CHECK(pmd_read_field(mss, RX_CTLE_ADAPT_BYPASS_ADDR, RX_CTLE_ADAPT_BYPASS_SETTING_A_MASK, RX_CTLE_ADAPT_BYPASS_SETTING_A_OFFSET, ctle_boost_a));
    CHECK(pmd_read_field(mss, RX_CTLE_ADAPT_BYPASS_ADDR, RX_CTLE_ADAPT_BYPASS_ENABLE_A_MASK, RX_CTLE_ADAPT_BYPASS_ENABLE_A_OFFSET, &bypass_en));
    if (bypass_en){
        *ctle_adapt_enable = 0;
    } else {
        *ctle_adapt_enable = 1;
    }
    return AW_ERR_CODE_NONE;
}

int aw_pmd_4ln_rx_background_adapt_enable_set(mss_access_t *mss, uint8_t rx_background_adapt)
{
    if (rx_background_adapt == 1){
        CHECK(pmd_write_field(mss, RXMFSM_CTRL_ADDR, RXMFSM_CTRL_RXMFSM_EQBK_POWER_STATE_MASK, RXMFSM_CTRL_RXMFSM_EQBK_POWER_STATE_OFFSET, 0));
    }
    else {
        CHECK(pmd_write_field(mss, RXMFSM_CTRL_ADDR, RXMFSM_CTRL_RXMFSM_EQBK_POWER_STATE_MASK, RXMFSM_CTRL_RXMFSM_EQBK_POWER_STATE_OFFSET, 7));
    }
    return AW_ERR_CODE_NONE;
}

int aw_pmd_4ln_rx_background_adapt_enable_get(mss_access_t *mss, uint32_t *rx_bkgrnd_adapt_enable){
    uint32_t adapt_en;
    CHECK(pmd_read_field(mss, RXMFSM_CTRL_ADDR, RXMFSM_CTRL_RXMFSM_EQBK_POWER_STATE_MASK, RXMFSM_CTRL_RXMFSM_EQBK_POWER_STATE_OFFSET, &adapt_en));
    if (adapt_en == 0){
        *rx_bkgrnd_adapt_enable = 1;
    } else {
        *rx_bkgrnd_adapt_enable = 0;
    }
    return AW_ERR_CODE_NONE;
}

int aw_pmd_4ln_rx_autoeq_set(mss_access_t *mss, uint32_t rx_autoeq_enable){
    CHECK(pmd_write_field(mss, RX_SIGNAL_DETECT_REG3_ADDR, RX_SIGNAL_DETECT_REG3_AUTOEQ_EN_NT_MASK, RX_SIGNAL_DETECT_REG3_AUTOEQ_EN_NT_OFFSET, rx_autoeq_enable));
    return AW_ERR_CODE_NONE;
}

int aw_pmd_4ln_rx_autoeq_get(mss_access_t *mss, uint32_t *rx_autoeq_enable){
    CHECK(pmd_read_field(mss, RX_SIGNAL_DETECT_REG3_ADDR, RX_SIGNAL_DETECT_REG3_AUTOEQ_EN_NT_MASK, RX_SIGNAL_DETECT_REG3_AUTOEQ_EN_NT_OFFSET, rx_autoeq_enable));
    return AW_ERR_CODE_NONE;
}

int aw_pmd_4ln_rx_ffe_tap_count_set(mss_access_t *mss, uint32_t tap_count) {

    // check if the tap_count is greater or equal to the total tap cnt. In such a case, don't use reduced FFE taps.
    if (tap_count >= AW_FFE_NUM_TAPS){
        CHECK(pmd_write_field(mss, USE_CUSTOM_FFE_TAP_DISABLE_ADDR, USE_CUSTOM_FFE_TAP_DISABLE_MASK, USE_CUSTOM_FFE_TAP_DISABLE_OFFSET, 0));
    }
    else {
        // For 11/15 reduced taps, there will be 3pre-taps. For 18 reduced taps, there will be 4 pretaps
        CHECK(pmd_write_field(mss, USE_CUSTOM_FFE_TAP_DISABLE_ADDR, USE_CUSTOM_FFE_TAP_DISABLE_MASK, USE_CUSTOM_FFE_TAP_DISABLE_OFFSET, 1));

        uint32_t m = 1 << AW_MAIN_TAP_NUM;
        uint32_t pre = -1;
        uint32_t post_tap_cnt = -1;

        if (tap_count == 18) { //18 taps
            pre = 0xF << (AW_MAIN_TAP_NUM + 1);
            post_tap_cnt = 13; // 18-5
        }
        else {
            pre = 7 << (AW_MAIN_TAP_NUM + 1);
            post_tap_cnt =  tap_count - 4;// 11-4
        }

        uint32_t post_tap_shift_index = AW_MAIN_TAP_NUM - post_tap_cnt;
        uint32_t post = ((1 << post_tap_cnt) - 1) << post_tap_shift_index;

        uint32_t total_disable = ~(pre | m | post);
        CHECK(pmd_write_field(mss, FFE_TAP_DISABLE_ADDR, FFE_TAP_DISABLE_MASK, FFE_TAP_DISABLE_OFFSET, total_disable));
    }

    return AW_ERR_CODE_NONE;
}

int aw_pmd_4ln_rx_ffe_tap_count_get(mss_access_t *mss, uint32_t* tap_count){
    uint32_t use_reduced_taps = 0;
    CHECK(pmd_read_field(mss, USE_CUSTOM_FFE_TAP_DISABLE_ADDR, USE_CUSTOM_FFE_TAP_DISABLE_MASK, USE_CUSTOM_FFE_TAP_DISABLE_OFFSET, &use_reduced_taps));

    if (use_reduced_taps == 1) {
        uint32_t total_disable = 0;
        CHECK(pmd_read_field(mss, FFE_TAP_DISABLE_ADDR, FFE_TAP_DISABLE_MASK, FFE_TAP_DISABLE_OFFSET, &total_disable));
        uint32_t taps_used = ~total_disable;

        uint32_t ffe_tap_vfield_size = 0;
        for (size_t i = 0; i < sizeof(FFE_TAP_DISABLE_MASK) * 8; i++) {
            if ((1<<i) & FFE_TAP_DISABLE_MASK)
                ffe_tap_vfield_size++;
        }

        // Look for 1s in taps_used. The right-most 1s refer to the taps that are in used
        // THe left-most 1s are leftover remnants from the uint32_t, which we don't look at
        uint32_t count = 0;
        for (uint32_t i = 0; i < ffe_tap_vfield_size; i++) {
            if ((1<<i) & taps_used)
                count++;
        }

        *tap_count = count;
    }
    else
        *tap_count = AW_FFE_NUM_TAPS;

    return AW_ERR_CODE_NONE;
}

int aw_pmd_4ln_rx_roaming_windows_set(mss_access_t *mss, aw_rx_roaming_mode_t mode, uint8_t window_select1, uint8_t window_select2) {
    // Description: {mode[13:12], window_select_2_nt[11:6], window_select_1_nt[5:0]}
    if (mode == AW_RESET_DEFAULT_ROAMING_MODE)
        CHECK(pmd_write_field(mss, USE_CUSTOM_ROAMING_WINDOWS_ADDR, USE_CUSTOM_ROAMING_WINDOWS_MASK, USE_CUSTOM_ROAMING_WINDOWS_OFFSET, 0));
    else {
        uint32_t roaming_cfg = (mode & 0x3) << 12 | (window_select2 & 0x3F) << 6 | (window_select1 & 0x3F);
        CHECK(pmd_write_field(mss, USE_CUSTOM_ROAMING_WINDOWS_ADDR, USE_CUSTOM_ROAMING_WINDOWS_MASK, USE_CUSTOM_ROAMING_WINDOWS_OFFSET, 1));
        CHECK(pmd_write_field(mss, ROAMING_WINDOWS_CFG_ADDR, ROAMING_WINDOWS_CFG_MASK, ROAMING_WINDOWS_CFG_OFFSET, roaming_cfg));
    }

    return AW_ERR_CODE_NONE;
}

int aw_pmd_4ln_rx_roaming_windows_get(mss_access_t *mss, aw_rx_roaming_mode_t *mode, uint8_t *window_select1, uint8_t *window_select2) {
    // Description: {mode[13:12], window_select_2_nt[11:6], window_select_1_nt[5:0]}

    uint32_t roaming_cfg = 0;
    uint32_t is_custom_roaming_window_enabled = 0;
    CHECK(pmd_read_field(mss, ROAMING_WINDOWS_CFG_ADDR, ROAMING_WINDOWS_CFG_MASK, ROAMING_WINDOWS_CFG_OFFSET, &roaming_cfg));
    CHECK(pmd_read_field(mss, USE_CUSTOM_ROAMING_WINDOWS_ADDR, USE_CUSTOM_ROAMING_WINDOWS_MASK, USE_CUSTOM_ROAMING_WINDOWS_OFFSET, &is_custom_roaming_window_enabled));

    *mode =  is_custom_roaming_window_enabled ? (roaming_cfg >> 12) & 0x3 : AW_RESET_DEFAULT_ROAMING_MODE;
    *window_select2 = (roaming_cfg >> 6) & 0x3F;
    *window_select1 = roaming_cfg & 0x3F;

    return AW_ERR_CODE_NONE;
}

int aw_pmd_4ln_rx_vga_cap_set(mss_access_t *mss, uint32_t vga_cap) {
    /**
     *
     * Function implementation has been deprecated. Use aw_pmd_4ln_rx_vga_cap_adapt_set instead
     *
    **/
#if 0
    *mss = *mss;
    vga_cap = vga_cap;
#endif
    (void)mss;
    (void)vga_cap;

    USR_PRINTF("ERROR: *cap_adapt_set - function implementation has been depricated\n");
    return AW_ERR_CODE_NONE;
}

int aw_pmd_4ln_rx_vga_cap_adapt_set(mss_access_t *mss, vga_opt_t *opts) {

    uint32_t vga_cap_1 = 0;
    uint32_t vga_cap_2 = 0;

    if (opts->en == 1) {
        CHECK(pmd_write_field(mss, DISABLE_VGA_CAP_ADAPT_ADDR, DISABLE_VGA_CAP_ADAPT_MASK, DISABLE_VGA_CAP_ADAPT_OFFSET, 0));
    } else {
        CHECK(pmd_write_field(mss, DISABLE_VGA_CAP_ADAPT_ADDR, DISABLE_VGA_CAP_ADAPT_MASK, DISABLE_VGA_CAP_ADAPT_OFFSET, 1));
    }

    switch (opts->vga_cap){
        case 0:
            vga_cap_1 = 0;
            vga_cap_2 = 0;
            break;
        case 1:
            vga_cap_1 = 4;
            vga_cap_2 = 0;
            break;
        case 2:
            vga_cap_1 = 8;
            vga_cap_2 = 0;
            break;
        case 3:
            vga_cap_1 = 12;
            vga_cap_2 = 0;
            break;
        case 4:
            vga_cap_1 = 16;
            vga_cap_2 = 0;
            break;
        case 5:
            vga_cap_1 = 20;
            vga_cap_2 = 0;
            break;
        case 6:
            vga_cap_1 = 20;
            vga_cap_2 = 4;
            break;
        case 7:
            vga_cap_1 = 20;
            vga_cap_2 = 8;
            break;
        case 8:
            vga_cap_1 = 20;
            vga_cap_2 = 12;
            break;
        case 9:
            vga_cap_1 = 20;
            vga_cap_2 = 16;
            break;
        case 10:
            vga_cap_1 = 20;
            vga_cap_2 = 20;
            break;
        default:
            return AW_ERR_CODE_INVALID_ARG_VALUE;
            break;
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

    CHECK(pmd_write_field(mss, USE_CUSTOM_VGA_CAP_TAKEOVER_ADDR, USE_CUSTOM_VGA_CAP_TAKEOVER_MASK, USE_CUSTOM_VGA_CAP_TAKEOVER_OFFSET, opts->use_custom_takeover_ratio));
    CHECK(pmd_write_field(mss, VGA_CAP_TAKEOVER_RATIO_ADDR, VGA_CAP_TAKEOVER_RATIO_MASK, VGA_CAP_TAKEOVER_RATIO_OFFSET, opts->custom_takeover_ratio));
    CHECK(pmd_write_field(mss, NYQ_MASK_VGA_CAP_ADDR, NYQ_MASK_VGA_CAP_MASK, NYQ_MASK_VGA_CAP_OFFSET, opts->custom_nyq_mask));

    return AW_ERR_CODE_NONE;
}

int aw_pmd_4ln_rx_vga_cap_get(mss_access_t *mss, uint32_t *vga_cap) {
    /**
     *
     * Function implementation has been deprecated. Use aw_pmd_4ln_rx_vga_cap_adapt_get instead
     *
    **/
#if 0
    *mss = *mss;
    *vga_cap = *vga_cap;
#endif
    (void)mss;
    (void)vga_cap;

    USR_PRINTF("ERROR: *cap_adapt_get - function implementation has been depricated\n");
    return AW_ERR_CODE_NONE;
}

int aw_pmd_4ln_rx_vga_cap_adapt_get(mss_access_t *mss, vga_opt_t *opts) {

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

    opts->vga_cap = (vga_cap_1 + vga_cap_2 + 2)/4;

    return AW_ERR_CODE_NONE;
}

int aw_pmd_4ln_rxeq_prbs_set(mss_access_t *mss, uint32_t prbs_en) {
    CHECK(pmd_write_field(mss, RXEQ_PRBS_ADDR, RXEQ_PRBS_MASK, RXEQ_PRBS_OFFSET, prbs_en));
    return AW_ERR_CODE_NONE;
}

int aw_pmd_4ln_rxeq_prbs_get(mss_access_t *mss, uint32_t *prbs_en) {
    CHECK(pmd_read_field(mss, RXEQ_PRBS_ADDR, RXEQ_PRBS_MASK, RXEQ_PRBS_OFFSET, prbs_en));
    return AW_ERR_CODE_NONE;
}

int aw_pmd_4ln_rx_c0_adapt_set(mss_access_t *mss, uint32_t en){

    CHECK(pmd_write_field(mss, DISABLE_C0_ADAPT_ADDR, DISABLE_C0_ADAPT_MASK, DISABLE_C0_ADAPT_OFFSET, !en));

    return AW_ERR_CODE_NONE;
}

int aw_pmd_4ln_rx_cdr_offset_set(mss_access_t *mss, uint32_t use_custom_cdr_offset, uint32_t cdr_offset, uint32_t cdr_dir) {
    uint32_t cdr_offset_dir;
    uint32_t cdr_offset_min = 0x0;
    uint32_t cdr_offset_max = 256;
    
    if ((cdr_offset >= cdr_offset_min && cdr_offset < cdr_offset_max) && (cdr_dir == 0x0 || cdr_dir == 0x1)) {
        cdr_offset_dir = (cdr_offset & 0x00ff) | (cdr_dir << 0x8);
        use_custom_cdr_offset = use_custom_cdr_offset & 0x1;
        CHECK(pmd_write_field(mss, USE_CUSTOM_CDR_OFFSET_ADDR, USE_CUSTOM_CDR_OFFSET_MASK, USE_CUSTOM_CDR_OFFSET_OFFSET, use_custom_cdr_offset));
        CHECK(pmd_write_field(mss, CDR_OFFSET_CFG_ADDR, CDR_OFFSET_CFG_MASK, CDR_OFFSET_CFG_OFFSET, cdr_offset_dir));
        return AW_ERR_CODE_NONE;
    } else {
        USR_PRINTF("ERROR: cdr_offset and/or cdr_dir out of range. Actual cdr_offset = 0x%x, cdr_dir=0x%x \n", cdr_offset, cdr_dir);
        return AW_ERR_CODE_CHECK_FAILURE;
    }
}

// Status APIs
int aw_pmd_4ln_rx_signal_detect_get(mss_access_t *mss, uint32_t *signal_detect){
    CHECK(bf_pmd_4ln_read_field(mss, DIG_SOC_LANE_STAT_REG3_ADDR, DIG_SOC_LANE_STAT_REG3_ODAT_RX_SIGNAL_DETECT_A_MASK, DIG_SOC_LANE_STAT_REG3_ODAT_RX_SIGNAL_DETECT_A_OFFSET, signal_detect));
    return AW_ERR_CODE_NONE;
}

int aw_pmd_4ln_rx_signal_detect_check(mss_access_t *mss, uint32_t signal_detect_expected){
    uint32_t signal_detect;
    CHECK(pmd_read_check_field(mss, DIG_SOC_LANE_STAT_REG3_ADDR, DIG_SOC_LANE_STAT_REG3_ODAT_RX_SIGNAL_DETECT_A_MASK, DIG_SOC_LANE_STAT_REG3_ODAT_RX_SIGNAL_DETECT_A_OFFSET, RD_EQ, &signal_detect,  signal_detect_expected, 0 /*NULL*/));
    if (signal_detect != signal_detect_expected) {
        USR_PRINTF("ERROR: Expected signal_detect = %d, Actual signal_detect = %d\n",signal_detect_expected, signal_detect);
        return AW_ERR_CODE_CHECK_FAILURE;
    } else {
        USR_PRINTF("Expected signal_detect = %d, Actual signal_detect = %d\n",signal_detect_expected, signal_detect);
        return AW_ERR_CODE_NONE;
    }
}


// timing_window: Sampling measurement window to be `2**timing_window-1` clock cycles
int aw_pmd_4ln_tx_ppm_get(mss_access_t *mss, uint32_t timing_window, uint32_t timeout_us, double *tx_ppm, double *vco_freq, double refclk_freq){
    // save timing window value prior to measurement
    uint32_t tx_synthdiv;
    uint32_t old_timing_window;
    CHECK(pmd_read_field(mss, TX_DS_ADDR, TX_DS_SYNTH_DIV_A_MASK, TX_DS_SYNTH_DIV_A_OFFSET, &tx_synthdiv));
    // save timing window value prior to measurement
    CHECK(pmd_read_field(mss, TX_VCO_ADAPT_REG2_ADDR, TX_VCO_ADAPT_REG2_TIMING_WINDOW_NT_MASK, TX_VCO_ADAPT_REG2_TIMING_WINDOW_NT_OFFSET, &old_timing_window));
    // assert req, check for poll error
    CHECK(pmd_write_field(mss, TX_VCO_ADAPT_REG2_ADDR, TX_VCO_ADAPT_REG2_TIMING_WINDOW_NT_MASK, TX_VCO_ADAPT_REG2_TIMING_WINDOW_NT_OFFSET, timing_window));
    CHECK(pmd_write_field(mss, TX_VCO_ADAPT_REG2_ADDR, TX_VCO_ADAPT_REG2_FREQ_MEASURE_REQ_A_MASK, TX_VCO_ADAPT_REG2_FREQ_MEASURE_REQ_A_OFFSET, 1));
    int poll_result;
    poll_result = pmd_poll_field(mss, TX_VCO_ADAPT_RDREG2_ADDR, TX_VCO_ADAPT_RDREG2_FREQ_MEASURE_ACK_NT_MASK, TX_VCO_ADAPT_RDREG2_FREQ_MEASURE_ACK_NT_OFFSET, 1, timeout_us);
    if (poll_result == -1) {
        USR_PRINTF("ERROR: polling for freq measure ack\n");
        return AW_ERR_CODE_POLL_TIMEOUT;
    } else {
        USR_PRINTF("freq measure ack received\n");
    }

    uint32_t freq_meas_result;
    uint32_t vco_counter;

    CHECK(pmd_read_field(mss, TX_VCO_ADAPT_RDREG2_ADDR, TX_VCO_ADAPT_RDREG2_FREQ_MEASURE_RESULT_NT_MASK, TX_VCO_ADAPT_RDREG2_FREQ_MEASURE_RESULT_NT_OFFSET, &freq_meas_result));
    CHECK(pmd_read_field(mss, TX_VCO_ADAPT_RDREG1_ADDR, TX_VCO_ADAPT_RDREG1_VCO_COUNTER_NT_MASK, TX_VCO_ADAPT_RDREG1_VCO_COUNTER_NT_OFFSET, &vco_counter));
    // de-assert req, check for poll error
    CHECK(pmd_write_field(mss, TX_VCO_ADAPT_REG2_ADDR, TX_VCO_ADAPT_REG2_FREQ_MEASURE_REQ_A_MASK, TX_VCO_ADAPT_REG2_FREQ_MEASURE_REQ_A_OFFSET, 0));
    poll_result = pmd_poll_field(mss, TX_VCO_ADAPT_RDREG2_ADDR, TX_VCO_ADAPT_RDREG2_FREQ_MEASURE_ACK_NT_MASK, TX_VCO_ADAPT_RDREG2_FREQ_MEASURE_ACK_NT_OFFSET, 0, timeout_us);
    if (poll_result == -1) {
        USR_PRINTF("ERROR: polling for freq measure ack\n");
        return AW_ERR_CODE_POLL_TIMEOUT;
    } else {
        USR_PRINTF("freq measure ack received\n");
    }

    // restore timing window to value prior to measurement
    CHECK(pmd_write_field(mss, TX_VCO_ADAPT_REG2_ADDR, TX_VCO_ADAPT_REG2_TIMING_WINDOW_NT_MASK, TX_VCO_ADAPT_REG2_TIMING_WINDOW_NT_OFFSET, old_timing_window));
    // compute ppm. result is in S3.12 format, need to convert first.
    if (((freq_meas_result >> 15) & 0x1) == 1) {
        freq_meas_result = 0xffff - freq_meas_result;
    }
    double ppm = pow(10,6)*freq_meas_result/pow(2, timing_window);
    *tx_ppm = ppm;

        //Convert tx_synthdiv to decimal representation
    USR_PRINTF("tx_synthdiv = 0x%x\n",tx_synthdiv);
    double tx_synthdiv_frac = (tx_synthdiv >> 2) + (tx_synthdiv & 0x3)* 0.25;
    USR_PRINTF("tx_synthdiv_frac = %f\n",tx_synthdiv_frac);
    //Compute vco freq
    *vco_freq = tx_synthdiv_frac * (vco_counter / (pow(2,timing_window) * (1/refclk_freq)));

    return AW_ERR_CODE_NONE;
}

// timing_window: Sampling measurement window to be `2**timing_window-1` clock cycles (?)
int aw_pmd_4ln_rx_ppm_get(mss_access_t *mss, uint32_t timing_window, uint32_t timeout_us, double *rx_ppm, double *vco_freq, double refclk_freq){
    // save timing window value prior to measurement
    uint32_t rx_synthdiv;
    uint32_t old_timing_window;

    CHECK(pmd_read_field(mss, RX_DS_ADDR, RX_DS_SYNTH_DIV_A_MASK, RX_DS_SYNTH_DIV_A_OFFSET, &rx_synthdiv));
    CHECK(pmd_read_field(mss, RX_VCO_ADAPT_REG2_ADDR, RX_VCO_ADAPT_REG2_TIMING_WINDOW_NT_MASK, RX_VCO_ADAPT_REG2_TIMING_WINDOW_NT_OFFSET, &old_timing_window));
    // assert req, check for poll error
    CHECK(pmd_write_field(mss, RX_VCO_ADAPT_REG2_ADDR, RX_VCO_ADAPT_REG2_TIMING_WINDOW_NT_MASK, RX_VCO_ADAPT_REG2_TIMING_WINDOW_NT_OFFSET, timing_window));
    CHECK(pmd_write_field(mss, RX_VCO_ADAPT_REG2_ADDR, RX_VCO_ADAPT_REG2_FREQ_MEASURE_REQ_A_MASK, RX_VCO_ADAPT_REG2_FREQ_MEASURE_REQ_A_OFFSET, 1));
    int poll_result;
    poll_result = pmd_poll_field(mss, RX_VCO_ADAPT_RDREG2_ADDR, RX_VCO_ADAPT_RDREG2_FREQ_MEASURE_ACK_NT_MASK, RX_VCO_ADAPT_RDREG2_FREQ_MEASURE_ACK_NT_OFFSET, 1, timeout_us);
    if (poll_result == -1) {
        USR_PRINTF("ERROR: polling for freq measure ack\n");
        return AW_ERR_CODE_POLL_TIMEOUT;
    } else {
        USR_PRINTF("freq measure ack received\n");
    }
    uint32_t freq_meas_result;
    uint32_t vco_counter;

    CHECK(pmd_read_field(mss, RX_VCO_ADAPT_RDREG2_ADDR, RX_VCO_ADAPT_RDREG2_FREQ_MEASURE_RESULT_NT_MASK, RX_VCO_ADAPT_RDREG2_FREQ_MEASURE_RESULT_NT_OFFSET, &freq_meas_result));
    CHECK(pmd_read_field(mss, RX_VCO_ADAPT_RDREG1_ADDR, RX_VCO_ADAPT_RDREG1_VCO_COUNTER_NT_MASK, RX_VCO_ADAPT_RDREG1_VCO_COUNTER_NT_OFFSET, &vco_counter));
    // de-assert req, check for poll error
    CHECK(pmd_write_field(mss, RX_VCO_ADAPT_REG2_ADDR, RX_VCO_ADAPT_REG2_FREQ_MEASURE_REQ_A_MASK, RX_VCO_ADAPT_REG2_FREQ_MEASURE_REQ_A_OFFSET, 0));
    poll_result = pmd_poll_field(mss, RX_VCO_ADAPT_RDREG2_ADDR, RX_VCO_ADAPT_RDREG2_FREQ_MEASURE_ACK_NT_MASK, RX_VCO_ADAPT_RDREG2_FREQ_MEASURE_ACK_NT_OFFSET, 0, timeout_us);
    if (poll_result == -1) {
        USR_PRINTF("ERROR: polling for freq measure ack\n");
        return AW_ERR_CODE_POLL_TIMEOUT;
    } else {
        USR_PRINTF("freq measure ack received\n");
    }

    // restore timing window to value prior to measurement
    CHECK(pmd_write_field(mss, RX_VCO_ADAPT_REG2_ADDR, RX_VCO_ADAPT_REG2_TIMING_WINDOW_NT_MASK, RX_VCO_ADAPT_REG2_TIMING_WINDOW_NT_OFFSET, old_timing_window));
    // compute ppm. result is in S3.12 format, need to convert first.
    if (((freq_meas_result >> 15) & 0x1) == 1) {
        freq_meas_result = 0xffff - freq_meas_result;
    }
    double ppm = pow(10,6)*freq_meas_result/pow(2, timing_window);
    *rx_ppm = ppm;
    //Convert rx_synthdiv to decimal representation
    double rx_synthdiv_frac = (rx_synthdiv >> 2) + (rx_synthdiv & 0x3)* 0.25;
    //Compute vco freq
    *vco_freq = rx_synthdiv_frac * (vco_counter / (pow(2,timing_window) * (1/refclk_freq)));

    return AW_ERR_CODE_NONE;
}

// 0 - Not locked
// 1 - Locked
int aw_pmd_4ln_rx_lock_status_get(mss_access_t *mss, uint32_t *pmd_rx_lock){
    CHECK(bf_pmd_4ln_read_field(mss, DIG_SOC_LANE_STAT_REG1_ADDR, DIG_SOC_LANE_STAT_REG1_OCTL_RX_DATA_VLD_MASK, DIG_SOC_LANE_STAT_REG1_OCTL_RX_DATA_VLD_OFFSET, pmd_rx_lock));
    return AW_ERR_CODE_NONE;
}

int aw_pmd_4ln_rx_dcdiq_get(mss_access_t *mss, aw_dcdiq_data_t *rx_dcdiq_data){
   CHECK(pmd_read_field(mss, RX_PHASE_ADAPT_RDREG_ADDR, RX_PHASE_ADAPT_RDREG_D0_DCD_A_MASK, RX_PHASE_ADAPT_RDREG_D0_DCD_A_OFFSET, &(rx_dcdiq_data->d0) ));
   CHECK(pmd_read_field(mss, RX_PHASE_ADAPT_RDREG_ADDR, RX_PHASE_ADAPT_RDREG_D90_DCD_A_MASK, RX_PHASE_ADAPT_RDREG_D90_DCD_A_OFFSET, &(rx_dcdiq_data->d90) ));
   CHECK(pmd_read_field(mss, RX_PHASE_ADAPT_RDREG_ADDR, RX_PHASE_ADAPT_RDREG_IQ_A_MASK, RX_PHASE_ADAPT_RDREG_IQ_A_OFFSET, &(rx_dcdiq_data->iq) ));
   return AW_ERR_CODE_NONE;
}

int aw_pmd_4ln_tx_dcdiq_get(mss_access_t *mss, aw_dcdiq_data_t *tx_dcdiq_data){
   CHECK(pmd_read_field(mss, TX_PHASE_ADAPT_RDREG_ADDR, TX_PHASE_ADAPT_RDREG_D0_DCD_A_MASK, TX_PHASE_ADAPT_RDREG_D0_DCD_A_OFFSET, &(tx_dcdiq_data->d0) ));
   CHECK(pmd_read_field(mss, TX_PHASE_ADAPT_RDREG_ADDR, TX_PHASE_ADAPT_RDREG_D90_DCD_A_MASK, TX_PHASE_ADAPT_RDREG_D90_DCD_A_OFFSET, &(tx_dcdiq_data->d90) ));
   CHECK(pmd_read_field(mss, TX_PHASE_ADAPT_RDREG_ADDR, TX_PHASE_ADAPT_RDREG_IQ_A_MASK, TX_PHASE_ADAPT_RDREG_IQ_A_OFFSET, &(tx_dcdiq_data->iq) ));
   return AW_ERR_CODE_NONE;
}

int aw_pmd_4ln_rx_afe_get(mss_access_t *mss, aw_afe_data_t *rx_afe_data){
    CHECK(pmd_read_field(mss, RX_CTLE_ADDR, RX_CTLE_RATE_NT_MASK, RX_CTLE_RATE_NT_OFFSET, &(rx_afe_data->ctle_rate) ));
    CHECK(pmd_read_field(mss, RX_CTLE_ADAPT_STATUS_ADDR, RX_CTLE_ADAPT_STATUS_CTLE_BOOST_A_MASK, RX_CTLE_ADAPT_STATUS_CTLE_BOOST_A_OFFSET, &(rx_afe_data->ctle_boost) ));
    CHECK(pmd_read_field(mss, RX_AGC_ADAPT_STATUS_ADDR, RX_AGC_ADAPT_STATUS_VGA_COARSE_A_MASK, RX_AGC_ADAPT_STATUS_VGA_COARSE_A_OFFSET, &(rx_afe_data->vga_coarse) ));
    CHECK(pmd_read_field(mss, RX_AGC_ADAPT_STATUS_ADDR, RX_AGC_ADAPT_STATUS_VGA_FINE_A_MASK, RX_AGC_ADAPT_STATUS_VGA_FINE_A_OFFSET, &(rx_afe_data->vga_fine) ));
    CHECK(pmd_read_field(mss, RX_ADAPT_VGA_OFFSET_STATUS_ADDR, RX_ADAPT_VGA_OFFSET_STATUS_VGA_OFFSET_A_MASK, RX_ADAPT_VGA_OFFSET_STATUS_VGA_OFFSET_A_OFFSET, &(rx_afe_data->vga_offset) ));
    return AW_ERR_CODE_NONE;
}

// BIST APIs
int aw_pmd_4ln_rx_invert_datapath(mss_access_t *mss, char modulation_mode[], uint32_t gray_code_en, uint32_t invert_en){
    // AWPRJ-11095 - Add API support to invert the BIST data path. 
    // AWPRJ-7752 - Ethernet: Inverted PRBS31 support
    //  - Workaround for 1p0 and 1p1
    //  - TODO: Add RTL (1p0, 1p1) ifdef guard.
    if (gray_code_en) {
        if (strcmp(modulation_mode, "pam4") == 0) {
            if (invert_en) {
                CHECK(pmd_write_field(mss, RX_DEMAPPER_ADDR, RX_DEMAPPER_EH3_VAL_NT_MASK, RX_DEMAPPER_EH3_VAL_NT_OFFSET, 2));
                CHECK(pmd_write_field(mss, RX_DEMAPPER_ADDR, RX_DEMAPPER_EH1_VAL_NT_MASK, RX_DEMAPPER_EH1_VAL_NT_OFFSET, 0));
                CHECK(pmd_write_field(mss, RX_DEMAPPER_ADDR, RX_DEMAPPER_EL1_VAL_NT_MASK, RX_DEMAPPER_EL1_VAL_NT_OFFSET, 1));
                CHECK(pmd_write_field(mss, RX_DEMAPPER_ADDR, RX_DEMAPPER_EL3_VAL_NT_MASK, RX_DEMAPPER_EL3_VAL_NT_OFFSET, 3));
                CHECK(pmd_write_field(mss, RX_CNTRL_REG2_ADDR, RX_CNTRL_REG2_RX_GRAYFLIP_NT_MASK, RX_CNTRL_REG2_RX_GRAYFLIP_NT_OFFSET, 0));
                CHECK(pmd_write_field(mss, RX_CNTRL_REG2_ADDR, RX_CNTRL_REG2_RX_GRAY_ENA_OVR_NT_MASK, RX_CNTRL_REG2_RX_GRAY_ENA_OVR_NT_OFFSET, 1));
                CHECK(pmd_write_field(mss, RX_CNTRL_REG2_ADDR, RX_CNTRL_REG2_RX_GRAY_ENA_NT_MASK, RX_CNTRL_REG2_RX_GRAY_ENA_NT_OFFSET, 1));
            } else {
                CHECK(pmd_write_field(mss, RX_CNTRL_REG2_ADDR, RX_CNTRL_REG2_RX_GRAY_ENA_NT_MASK, RX_CNTRL_REG2_RX_GRAY_ENA_NT_OFFSET, 0));
                CHECK(pmd_write_field(mss, RX_CNTRL_REG2_ADDR, RX_CNTRL_REG2_RX_GRAY_ENA_OVR_NT_MASK, RX_CNTRL_REG2_RX_GRAY_ENA_OVR_NT_OFFSET, 0));
                CHECK(pmd_write_field(mss, RX_CNTRL_REG2_ADDR, RX_CNTRL_REG2_RX_GRAYFLIP_NT_MASK, RX_CNTRL_REG2_RX_GRAYFLIP_NT_OFFSET, 0));
                CHECK(pmd_write_field(mss, RX_DEMAPPER_ADDR, RX_DEMAPPER_EH3_VAL_NT_MASK, RX_DEMAPPER_EH3_VAL_NT_OFFSET, RX_DEMAPPER_EH3_VAL_NT_RESET_VALUE));
                CHECK(pmd_write_field(mss, RX_DEMAPPER_ADDR, RX_DEMAPPER_EH1_VAL_NT_MASK, RX_DEMAPPER_EH1_VAL_NT_OFFSET, RX_DEMAPPER_EH1_VAL_NT_RESET_VALUE));
                CHECK(pmd_write_field(mss, RX_DEMAPPER_ADDR, RX_DEMAPPER_EL1_VAL_NT_MASK, RX_DEMAPPER_EL1_VAL_NT_OFFSET, RX_DEMAPPER_EL1_VAL_NT_RESET_VALUE));
                CHECK(pmd_write_field(mss, RX_DEMAPPER_ADDR, RX_DEMAPPER_EL3_VAL_NT_MASK, RX_DEMAPPER_EL3_VAL_NT_OFFSET, RX_DEMAPPER_EL3_VAL_NT_RESET_VALUE));
            }
        } else {
            return AW_ERR_CODE_INVALID_ARG_VALUE;
        }
    } else {
        if (strcmp(modulation_mode, "pam4") == 0 || strcmp(modulation_mode, "nrz") == 0) {
            if (invert_en) {
                CHECK(pmd_write_field(mss, RX_DEMAPPER_ADDR, RX_DEMAPPER_INVERT_ENABLE_A_MASK, RX_DEMAPPER_INVERT_ENABLE_A_OFFSET, 1));
            } else {
                CHECK(pmd_write_field(mss, RX_DEMAPPER_ADDR, RX_DEMAPPER_INVERT_ENABLE_A_MASK, RX_DEMAPPER_INVERT_ENABLE_A_OFFSET, 0));
            }
        } else {
            return AW_ERR_CODE_INVALID_ARG_VALUE;
        }
    }

    return AW_ERR_CODE_NONE;
}

int aw_pmd_4ln_tx_invert_datapath(mss_access_t *mss, char modulation_mode[], uint32_t gray_code_en, uint32_t invert_en){
    // AWPRJ-11095 - Add API support to invert the BIST data path. 
    // AWPRJ-7752 - Ethernet: Inverted PRBS31 support
    //  - Workaround for 1p0 and 1p1
    //  - TODO: Add RTL (1p0, 1p1) ifdef guard.
    if (gray_code_en) {
        if (strcmp(modulation_mode, "pam4") == 0) {
            if (invert_en) {
                CHECK(pmd_write_field(mss, TX_DATAPATH_REG1_ADDR, TX_DATAPATH_REG1_GRAY_CODE_MAPPING_NT_MASK, TX_DATAPATH_REG1_GRAY_CODE_MAPPING_NT_OFFSET, 0x1e));
                CHECK(pmd_write_field(mss, TX_DATAPATH_REG2_ADDR, TX_DATAPATH_REG2_PAMCODE_OVR_EN_A_MASK, TX_DATAPATH_REG2_PAMCODE_OVR_EN_A_OFFSET, 1));
                CHECK(pmd_write_field(mss, TX_DATAPATH_REG1_ADDR, TX_DATAPATH_REG1_GRAY_CODE_ENABLE_A_MASK, TX_DATAPATH_REG1_GRAY_CODE_ENABLE_A_OFFSET, 1));
            } else {
                CHECK(pmd_write_field(mss, TX_DATAPATH_REG1_ADDR, TX_DATAPATH_REG1_GRAY_CODE_ENABLE_A_MASK, TX_DATAPATH_REG1_GRAY_CODE_ENABLE_A_OFFSET, 0));
                CHECK(pmd_write_field(mss, TX_DATAPATH_REG2_ADDR, TX_DATAPATH_REG2_PAMCODE_OVR_EN_A_MASK, TX_DATAPATH_REG2_PAMCODE_OVR_EN_A_OFFSET, 0));
                CHECK(pmd_write_field(mss, TX_DATAPATH_REG1_ADDR, TX_DATAPATH_REG1_GRAY_CODE_MAPPING_NT_MASK, TX_DATAPATH_REG1_GRAY_CODE_MAPPING_NT_OFFSET, TX_DATAPATH_REG1_GRAY_CODE_MAPPING_NT_RESET_VALUE));
            }
        } else {
            return AW_ERR_CODE_INVALID_ARG_VALUE;
        }
    } else {
        if (strcmp(modulation_mode, "pam4") == 0 || strcmp(modulation_mode, "nrz") == 0) {
            if (invert_en) {
                CHECK(pmd_write_field(mss, TX_DATAPATH_REG2_ADDR, TX_DATAPATH_REG2_INVERT_ENABLE_A_MASK, TX_DATAPATH_REG2_INVERT_ENABLE_A_OFFSET, 1));
            } else {
                CHECK(pmd_write_field(mss, TX_DATAPATH_REG2_ADDR, TX_DATAPATH_REG2_INVERT_ENABLE_A_MASK, TX_DATAPATH_REG2_INVERT_ENABLE_A_OFFSET, 0));
            }
        } else {
            return AW_ERR_CODE_INVALID_ARG_VALUE;
        }
    }

    return AW_ERR_CODE_NONE;
}

int aw_pmd_4ln_rx_chk_config_set(mss_access_t *mss, aw_bist_pattern_t pattern, aw_bist_mode_t mode, uint64_t udp, uint32_t lock_thresh, uint32_t timer_thresh){
    CHECK(pmd_write_field(mss, RX_DATABIST_TOP_REG1_ADDR, RX_DATABIST_TOP_REG1_BIST_ENABLE_A_MASK, RX_DATABIST_TOP_REG1_BIST_ENABLE_A_OFFSET, 0));
    CHECK(pmd_write_field(mss, RX_DATABIST_TOP_REG1_ADDR, RX_DATABIST_TOP_REG1_ERROR_CNT_CLR_A_MASK, RX_DATABIST_TOP_REG1_ERROR_CNT_CLR_A_OFFSET, 0));
    CHECK(pmd_write_field(mss, RX_DATABIST_TOP_REG1_ADDR, RX_DATABIST_TOP_REG1_ERROR_CNT_CLR_A_MASK, RX_DATABIST_TOP_REG1_ERROR_CNT_CLR_A_OFFSET, 1));
    CHECK(pmd_write_field(mss, RX_DATABIST_TOP_REG1_ADDR, RX_DATABIST_TOP_REG1_ERROR_CNT_CLR_A_MASK, RX_DATABIST_TOP_REG1_ERROR_CNT_CLR_A_OFFSET, 0));

    uint32_t udp_31_0;
    uint32_t udp_63_32;
    uint32_t udp_en = 0;

    switch (pattern){
    case AW_PRBS7:
    case AW_PRBS9:
    case AW_PRBS11:
    case AW_PRBS13:
    case AW_PRBS15:
    case AW_PRBS23:
    case AW_PRBS31:
    case AW_QPRBS13:
    case AW_JP03A:
    case AW_JP03B:
    case AW_LINEARITY_PATTERN:
        CHECK(pmd_write_field(mss, RX_DATABIST_TOP_REG2_ADDR, RX_DATABIST_TOP_REG2_PATTERN_SEL_NT_MASK, RX_DATABIST_TOP_REG2_PATTERN_SEL_NT_OFFSET, pattern));
        break;
    case AW_USER_DEFINED_PATTERN:
        udp_31_0  = (uint32_t)(udp & 0xFFFFFFFF);
        udp_63_32 = (uint32_t)((udp >> 32) & 0xFFFFFFFF);
        udp_en = 1;
        break;
    case AW_FULL_RATE_CLOCK:
        udp_31_0  = 0xAAAAAAAA;
        udp_63_32 = 0xAAAAAAAA;
        udp_en = 1;
        break;
    case AW_HALF_RATE_CLOCK:
        udp_31_0  = 0xCCCCCCCC;
        udp_63_32 = 0xCCCCCCCC;
        udp_en = 1;
        break;
    case AW_QUARTER_RATE_CLOCK:
        udp_31_0  = 0xF0F0F0F0;
        udp_63_32 = 0xF0F0F0F0;
        udp_en = 1;
        break;
    case AW_PATT_32_1S_32_0S:
        udp_63_32 = 0xFFFFFFFF;
        udp_31_0  = 0x00000000;
        udp_en = 1;
        break;
    default:
        return AW_ERR_CODE_INVALID_ARG_VALUE;
    }
    if (udp_en) {
        CHECK(pmd_write_field(mss, RX_DATABIST_TOP_REG2_ADDR, RX_DATABIST_TOP_REG2_PATTERN_SEL_NT_MASK, RX_DATABIST_TOP_REG2_PATTERN_SEL_NT_OFFSET, AW_USER_DEFINED_PATTERN));
        CHECK(pmd_write_field(mss, RX_DATABIST_TOP_REG4_ADDR, RX_DATABIST_TOP_REG4_UDP_PATTERN_31_0_NT_MASK, RX_DATABIST_TOP_REG4_UDP_PATTERN_31_0_NT_OFFSET, udp_31_0));
        CHECK(pmd_write_field(mss, RX_DATABIST_TOP_REG5_ADDR, RX_DATABIST_TOP_REG5_UDP_PATTERN_63_32_NT_MASK, RX_DATABIST_TOP_REG5_UDP_PATTERN_63_32_NT_OFFSET, udp_63_32));
        CHECK(pmd_write_field(mss, RX_DATABIST_TOP_REG6_ADDR, RX_DATABIST_TOP_REG6_UDP_PATTERN_95_64_NT_MASK, RX_DATABIST_TOP_REG6_UDP_PATTERN_95_64_NT_OFFSET, udp_31_0));
        CHECK(pmd_write_field(mss, RX_DATABIST_TOP_REG7_ADDR, RX_DATABIST_TOP_REG7_UDP_PATTERN_127_96_NT_MASK, RX_DATABIST_TOP_REG7_UDP_PATTERN_127_96_NT_OFFSET, udp_63_32));
        CHECK(pmd_write_field(mss, RX_DATABIST_TOP_REG8_ADDR, RX_DATABIST_TOP_REG8_UDP_PATTERN_159_128_NT_MASK, RX_DATABIST_TOP_REG8_UDP_PATTERN_159_128_NT_OFFSET, udp_31_0));
        CHECK(pmd_write_field(mss, RX_DATABIST_TOP_REG9_ADDR, RX_DATABIST_TOP_REG9_UDP_PATTERN_191_160_NT_MASK, RX_DATABIST_TOP_REG9_UDP_PATTERN_191_160_NT_OFFSET, udp_63_32));
    }

    if (mode == AW_TIMER) {
        CHECK(pmd_write_field(mss, RX_DATABIST_TOP_REG1_ADDR, RX_DATABIST_TOP_REG1_WALL_CLOCK_ENABLE_A_MASK, RX_DATABIST_TOP_REG1_WALL_CLOCK_ENABLE_A_OFFSET, 0));
        CHECK(pmd_write_field(mss, RX_DATABIST_TOP_REG1_ADDR, RX_DATABIST_TOP_REG1_BIST_MODE_NT_MASK, RX_DATABIST_TOP_REG1_BIST_MODE_NT_OFFSET, 0));
        CHECK(pmd_write_field(mss, RX_DATABIST_TOP_REG1_ADDR, RX_DATABIST_TOP_REG1_LOCK_THRESHOLD_NT_MASK, RX_DATABIST_TOP_REG1_LOCK_THRESHOLD_NT_OFFSET, lock_thresh));
        CHECK(pmd_write_field(mss, RX_DATABIST_TOP_REG2_ADDR, RX_DATABIST_TOP_REG2_TIMER_THRESHOLD_NT_MASK, RX_DATABIST_TOP_REG2_TIMER_THRESHOLD_NT_OFFSET, timer_thresh));
    } else {
        CHECK(pmd_write_field(mss, RX_DATABIST_TOP_REG1_ADDR, RX_DATABIST_TOP_REG1_WALL_CLOCK_ENABLE_A_MASK, RX_DATABIST_TOP_REG1_WALL_CLOCK_ENABLE_A_OFFSET, 1));
        CHECK(pmd_write_field(mss, RX_DATABIST_TOP_REG1_ADDR, RX_DATABIST_TOP_REG1_BIST_MODE_NT_MASK, RX_DATABIST_TOP_REG1_BIST_MODE_NT_OFFSET, 1));
    }
    return AW_ERR_CODE_NONE;
}

// NOTE: changed from pmd_chk_rx_config_get -> pmd_rx_chk_config_get
int aw_pmd_4ln_rx_chk_config_get(mss_access_t *mss, aw_bist_pattern_t *pattern, aw_bist_mode_t *mode, uint64_t *udp, uint32_t *lock_thresh, uint32_t *timer_thresh){
    uint32_t patt;
    uint32_t udp_31_0;
    uint32_t udp_63_32;
    uint32_t *mode_uint32;

    CHECK(pmd_read_field(mss, RX_DATABIST_TOP_REG2_ADDR, RX_DATABIST_TOP_REG2_PATTERN_SEL_NT_MASK, RX_DATABIST_TOP_REG2_PATTERN_SEL_NT_OFFSET, &patt)); // 11 is UDP
    CHECK(pmd_read_field(mss, RX_DATABIST_TOP_REG4_ADDR, RX_DATABIST_TOP_REG4_UDP_PATTERN_31_0_NT_MASK, RX_DATABIST_TOP_REG4_UDP_PATTERN_31_0_NT_OFFSET, &udp_31_0));
    CHECK(pmd_read_field(mss, RX_DATABIST_TOP_REG5_ADDR, RX_DATABIST_TOP_REG5_UDP_PATTERN_63_32_NT_MASK, RX_DATABIST_TOP_REG5_UDP_PATTERN_63_32_NT_OFFSET, &udp_63_32));

    if (patt >= AW_USER_DEFINED_PATTERN){
        if (udp_31_0 == 0xAAAAAAAA){
            *pattern = AW_FULL_RATE_CLOCK;
        } else if (udp_31_0 == 0xCCCCCCCC){
            *pattern = AW_HALF_RATE_CLOCK;
        } else if (udp_31_0 == 0xF0F0F0F0){
            *pattern = AW_QUARTER_RATE_CLOCK;
        } else if (udp_31_0 == 0x00000000 && udp_63_32 == 0xFFFFFFFF){
            *pattern = AW_PATT_32_1S_32_0S;
        } else {
            *pattern = AW_USER_DEFINED_PATTERN;
            *udp = (uint64_t)udp_63_32 << 32 | (uint64_t)udp_31_0;
        }
    } else {
        *pattern = (aw_bist_pattern_t) patt;
        *udp = 0; // return AW_ERR_CODE_NONE for UDP if the mode is not UDP
    }

    mode_uint32 = (uint32_t*)mode;
    CHECK(pmd_read_field(mss, RX_DATABIST_TOP_REG1_ADDR, RX_DATABIST_TOP_REG1_BIST_MODE_NT_MASK, RX_DATABIST_TOP_REG1_BIST_MODE_NT_OFFSET, mode_uint32));
    CHECK(pmd_read_field(mss, RX_DATABIST_TOP_REG1_ADDR, RX_PPM_LOCK_DETECT_REG1_FREQ_LOCK_THRESHOLD_NT_MASK, RX_PPM_LOCK_DETECT_REG1_FREQ_LOCK_THRESHOLD_NT_OFFSET, lock_thresh));
    CHECK(pmd_read_field(mss, RX_DATABIST_TOP_REG2_ADDR, RX_DATABIST_TOP_REG2_TIMER_THRESHOLD_NT_MASK, RX_DATABIST_TOP_REG2_TIMER_THRESHOLD_NT_OFFSET, timer_thresh));
    return AW_ERR_CODE_NONE;
}

int aw_pmd_4ln_rx_chk_en_set(mss_access_t *mss, uint32_t enable){
    // 0 - disable RX BIST
    // 1 - enable RX BIST
    CHECK(pmd_write_field(mss, RX_DATABIST_TOP_REG1_ADDR, RX_DATABIST_TOP_REG1_BIST_ENABLE_A_MASK, RX_DATABIST_TOP_REG1_BIST_ENABLE_A_OFFSET, enable));
    return AW_ERR_CODE_NONE;
}

int aw_pmd_4ln_rx_chk_en_get(mss_access_t *mss, uint32_t *enable){
    // 0 - disable RX BIST
    // 1 - enable RX BIST
    CHECK(pmd_read_field(mss, RX_DATABIST_TOP_REG1_ADDR, RX_DATABIST_TOP_REG1_BIST_ENABLE_A_MASK, RX_DATABIST_TOP_REG1_BIST_ENABLE_A_OFFSET, enable));
    return AW_ERR_CODE_NONE;
}

int aw_pmd_4ln_rx_chk_lock_state_get(mss_access_t *mss, uint32_t *rx_bist_lock){
    // 0 - RX BIST not locked
    // 1 - RX BIST locked
    CHECK(pmd_read_field(mss, RX_DATABIST_TOP_RDREG1_ADDR, RX_DATABIST_TOP_RDREG1_LOCKED_NT_MASK, RX_DATABIST_TOP_RDREG1_LOCKED_NT_OFFSET, rx_bist_lock));
    return AW_ERR_CODE_NONE;
}

int aw_pmd_4ln_rx_chk_err_count_state_get(mss_access_t *mss, uint64_t *err_count, uint32_t *err_count_done, uint32_t *err_count_overflown){
    uint32_t bist_mode;
    uint32_t err_cnt_55_32;
    uint32_t err_cnt_31_0;
    uint32_t err_code;
    uint32_t bist_enable;
    CHECK(pmd_read_field(mss, RX_DATABIST_TOP_REG1_ADDR, RX_DATABIST_TOP_REG1_BIST_MODE_NT_MASK, RX_DATABIST_TOP_REG1_BIST_MODE_NT_OFFSET, &bist_mode));
    err_code = 0;
    if (bist_mode == 1){ //Wall Clock Mode
        CHECK(pmd_read_field(mss, RX_DATABIST_TOP_REG1_ADDR, RX_DATABIST_TOP_REG1_BIST_ENABLE_A_MASK, RX_DATABIST_TOP_REG1_BIST_ENABLE_A_OFFSET, &bist_enable));
        if (bist_enable == 1){//If BIST is enabled
            CHECK(pmd_read_check_field(mss, RX_DATABIST_TOP_RDREG1_ADDR, RX_DATABIST_TOP_RDREG1_ERROR_CNT_OVERFLOW_NT_MASK, RX_DATABIST_TOP_RDREG1_ERROR_CNT_OVERFLOW_NT_OFFSET, RD_EQ, err_count_overflown, 0, 0 /*NULL*/));
            if (*err_count_overflown){
                err_code = 1; //Error: err_c overflown
                USR_PRINTF("ERROR: BIST error counter overflown.");
            } else {
                CHECK(pmd_read_field(mss, RX_DATABIST_TOP_RDREG3_ADDR, RX_DATABIST_TOP_RDREG3_ERROR_CNT_55T32_NT_MASK, RX_DATABIST_TOP_RDREG3_ERROR_CNT_55T32_NT_OFFSET, &err_cnt_55_32));
                CHECK(pmd_read_field(mss, RX_DATABIST_TOP_RDREG2_ADDR, RX_DATABIST_TOP_RDREG2_ERROR_CNT_NT_MASK, RX_DATABIST_TOP_RDREG2_ERROR_CNT_NT_OFFSET, &err_cnt_31_0));
                *err_count = (uint64_t)err_cnt_55_32 << 32 | (uint64_t)err_cnt_31_0;
            }
            *err_count_done = 1;
        } else {
            err_code = 2; //BIST not enabled
            USR_PRINTF("ERROR: BIST is not enabled.");
        }
    } else { //Timer mode
        CHECK(pmd_read_check_field(mss, RX_DATABIST_TOP_RDREG1_ADDR, RX_DATABIST_TOP_RDREG1_ERROR_CNT_DONE_NT_MASK, RX_DATABIST_TOP_RDREG1_ERROR_CNT_DONE_NT_OFFSET, RD_EQ, err_count_done, 1, 0 /*NULL*/));
        CHECK(pmd_read_field(mss, RX_DATABIST_TOP_RDREG3_ADDR, RX_DATABIST_TOP_RDREG3_ERROR_CNT_55T32_NT_MASK, RX_DATABIST_TOP_RDREG3_ERROR_CNT_55T32_NT_OFFSET, &err_cnt_55_32));
        CHECK(pmd_read_field(mss, RX_DATABIST_TOP_RDREG2_ADDR, RX_DATABIST_TOP_RDREG2_ERROR_CNT_NT_MASK, RX_DATABIST_TOP_RDREG2_ERROR_CNT_NT_OFFSET, &err_cnt_31_0));
        *err_count = (uint64_t) err_cnt_55_32 << 32 | (uint64_t) err_cnt_31_0;
        CHECK(pmd_read_check_field(mss, RX_DATABIST_TOP_RDREG1_ADDR, RX_DATABIST_TOP_RDREG1_ERROR_CNT_OVERFLOW_NT_MASK, RX_DATABIST_TOP_RDREG1_ERROR_CNT_OVERFLOW_NT_OFFSET, RD_EQ, err_count_overflown, 0, 0 /*NULL*/));
        if (*err_count_done == 0) { //If not done then set err_count_done flag to 0
            *err_count_overflown = 0;
            USR_PRINTF("RX BIST timer has not completed yet.");
        } else { //If done then set variables
            *err_count_done = 1;
        }
    }
    if (err_code != 0){
        return AW_ERR_CODE_FUNC_FAILURE;
    } else {
        return AW_ERR_CODE_NONE;
    }
}

int aw_pmd_4ln_rx_chk_err_count_state_clear(mss_access_t *mss){
    CHECK(pmd_write_field(mss, RX_DATABIST_TOP_REG1_ADDR, RX_DATABIST_TOP_REG1_ERROR_CNT_CLR_A_MASK, RX_DATABIST_TOP_REG1_ERROR_CNT_CLR_A_OFFSET, 1));
    CHECK(pmd_write_field(mss, RX_DATABIST_TOP_REG1_ADDR, RX_DATABIST_TOP_REG1_ERROR_CNT_CLR_A_MASK, RX_DATABIST_TOP_REG1_ERROR_CNT_CLR_A_OFFSET, 0));
    return AW_ERR_CODE_NONE;
}

int aw_pmd_4ln_tx_gen_config_set(mss_access_t *mss, aw_bist_pattern_t pattern, uint64_t udp){
    uint32_t udp_31_0;
    uint32_t udp_63_32;
    uint32_t udp_en = 0;

    switch (pattern){
        case AW_PRBS7:
        case AW_PRBS9:
        case AW_PRBS11:
        case AW_PRBS13:
        case AW_PRBS15:
        case AW_PRBS23:
        case AW_PRBS31:
        case AW_QPRBS13:
        case AW_JP03A:
        case AW_JP03B:
        case AW_LINEARITY_PATTERN:
            CHECK(pmd_write_field(mss, TX_DATAPATH_REG2_ADDR, TX_DATAPATH_REG2_PATTERN_SEL_NT_MASK, TX_DATAPATH_REG2_PATTERN_SEL_NT_OFFSET, pattern));
            break;
        case AW_USER_DEFINED_PATTERN:
            udp_31_0  = (uint32_t)(udp & 0xFFFFFFFF);
            udp_63_32 = (uint32_t)((udp >> 32) & 0xFFFFFFFF);
            udp_en = 1;
            break;
        case AW_FULL_RATE_CLOCK:
            udp_31_0 = 0xAAAAAAAA;
            udp_63_32 = 0xAAAAAAAA;
            udp_en = 1;
            break;
        case AW_HALF_RATE_CLOCK:
            udp_31_0 = 0xCCCCCCCC;
            udp_63_32 = 0xCCCCCCCC;
            udp_en = 1;
            break;
        case AW_QUARTER_RATE_CLOCK:
            udp_31_0 = 0xF0F0F0F0;
            udp_63_32 = 0xF0F0F0F0;
            udp_en = 1;
            break;
        case AW_PATT_32_1S_32_0S:
            udp_63_32 = 0xFFFFFFFF;
            udp_31_0 = 0x00000000;
            udp_en = 1;
            break;
        default:
            return AW_ERR_CODE_INVALID_ARG_VALUE; // Error
    }
    if (udp_en) {
        CHECK(pmd_write_field(mss, TX_DATAPATH_REG2_ADDR, TX_DATAPATH_REG2_PATTERN_SEL_NT_MASK, TX_DATAPATH_REG2_PATTERN_SEL_NT_OFFSET, AW_USER_DEFINED_PATTERN));
        CHECK(pmd_write_field(mss, TX_DATAPATH_REG8_ADDR, TX_DATAPATH_REG8_UDP_PATTERN_63_32_NT_MASK, TX_DATAPATH_REG8_UDP_PATTERN_63_32_NT_OFFSET, udp_63_32));
        CHECK(pmd_write_field(mss, TX_DATAPATH_REG7_ADDR, TX_DATAPATH_REG7_UDP_PATTERN_31_0_NT_MASK, TX_DATAPATH_REG7_UDP_PATTERN_31_0_NT_OFFSET, udp_31_0));
    }
    return AW_ERR_CODE_NONE;
}

int aw_pmd_4ln_tx_gen_config_get(mss_access_t *mss, aw_bist_pattern_t *pattern, uint64_t *udp){
    uint32_t patt;
    uint32_t udp_31_0;
    uint32_t udp_63_32;

    CHECK(pmd_read_field(mss, TX_DATAPATH_REG2_ADDR, TX_DATAPATH_REG2_PATTERN_SEL_NT_MASK, TX_DATAPATH_REG2_PATTERN_SEL_NT_OFFSET, &patt));
    CHECK(pmd_read_field(mss, TX_DATAPATH_REG8_ADDR, TX_DATAPATH_REG8_UDP_PATTERN_63_32_NT_MASK, TX_DATAPATH_REG8_UDP_PATTERN_63_32_NT_OFFSET, &udp_63_32));
    CHECK(pmd_read_field(mss, TX_DATAPATH_REG7_ADDR, TX_DATAPATH_REG7_UDP_PATTERN_31_0_NT_MASK, TX_DATAPATH_REG7_UDP_PATTERN_31_0_NT_OFFSET, &udp_31_0));

    *pattern = patt;

    switch (patt){
    case AW_PRBS7:
    case AW_PRBS9:
    case AW_PRBS11:
    case AW_PRBS13:
    case AW_PRBS15:
    case AW_PRBS23:
    case AW_PRBS31:
    case AW_QPRBS13:
    case AW_JP03A:
    case AW_JP03B:
    case AW_LINEARITY_PATTERN:
        CHECK(pmd_read_field(mss, TX_DATAPATH_REG2_ADDR, TX_DATAPATH_REG2_PATTERN_SEL_NT_MASK, TX_DATAPATH_REG2_PATTERN_SEL_NT_OFFSET, &patt));
        return AW_ERR_CODE_NONE;
        break;
    case AW_USER_DEFINED_PATTERN:
        if (udp_31_0 == 0xAAAAAAAA && udp_63_32 == 0xAAAAAAAA) {
            *pattern = AW_FULL_RATE_CLOCK;
        } else if (udp_31_0 == 0xCCCCCCCC && udp_63_32 == 0xCCCCCCCC) {
            *pattern = AW_HALF_RATE_CLOCK;
        } else if (udp_31_0 == 0xF0F0F0F0 && udp_63_32 == 0xF0F0F0F0) {
            *pattern = AW_QUARTER_RATE_CLOCK;
        } else if (udp_31_0 == 0x00000000 && udp_63_32 == 0xFFFFFFFF) {
            *pattern = AW_PATT_32_1S_32_0S;
        } else {
            *pattern = AW_USER_DEFINED_PATTERN;
            *udp = (uint64_t)udp_63_32 << 32 | (uint64_t)udp_31_0;
        }
        return AW_ERR_CODE_NONE;
        break;
    default:
        return AW_ERR_CODE_INVALID_ARG_VALUE;
    }

    return AW_ERR_CODE_NONE;
}

int aw_pmd_4ln_tx_gen_en_set(mss_access_t *mss, uint32_t enable){
    // 0 - disable TX BIST
    // 1 - enable TX BIST
    CHECK(pmd_write_field(mss, TX_DATAPATH_REG1_ADDR, TX_DATAPATH_REG1_BIST_ENABLE_A_MASK, TX_DATAPATH_REG1_BIST_ENABLE_A_OFFSET, enable));
    return AW_ERR_CODE_NONE;
}

int aw_pmd_4ln_tx_gen_en_get(mss_access_t *mss, uint32_t *enable){
    CHECK(pmd_read_field(mss, TX_DATAPATH_REG1_ADDR, TX_DATAPATH_REG1_BIST_ENABLE_A_MASK, TX_DATAPATH_REG1_BIST_ENABLE_A_OFFSET, enable));
    return AW_ERR_CODE_NONE;
}

int aw_pmd_4ln_tx_gen_err_inject_config_set(mss_access_t *mss, uint64_t err_pattern, uint32_t err_rate){
    // 'err_pattern': 64bit error injection pattern
    // 'err_rate':
    //     0 - inject 1 bit error
    //     not 0 - error injection rate
    uint32_t err_pattern_31_0 = err_pattern & 0xFFFFFFFF;
    uint32_t err_pattern_63_32 = err_pattern >> 32 & 0xFFFFFFFF;
    CHECK(pmd_write_field(mss, TX_DATAPATH_REG10_ADDR, TX_DATAPATH_REG10_ERROR_PATTERN_63_32_NT_MASK, TX_DATAPATH_REG10_ERROR_PATTERN_63_32_NT_OFFSET, err_pattern_63_32));
    CHECK(pmd_write_field(mss, TX_DATAPATH_REG9_ADDR, TX_DATAPATH_REG9_ERROR_PATTERN_31_0_NT_MASK, TX_DATAPATH_REG9_ERROR_PATTERN_31_0_NT_OFFSET, err_pattern_31_0));
    CHECK(pmd_write_field(mss, TX_DATAPATH_REG1_ADDR, TX_DATAPATH_REG1_TXDATA_ERROR_RATE_NT_MASK, TX_DATAPATH_REG1_TXDATA_ERROR_RATE_NT_OFFSET, err_rate));
    return AW_ERR_CODE_NONE;
}

int aw_pmd_4ln_tx_gen_err_inject_config_get(mss_access_t *mss, uint64_t *err_pattern, uint32_t *err_rate){
    uint32_t err_pattern_63_32;
    uint32_t err_pattern_31_0;
    CHECK(pmd_read_field(mss, TX_DATAPATH_REG10_ADDR, TX_DATAPATH_REG10_ERROR_PATTERN_63_32_NT_MASK, TX_DATAPATH_REG10_ERROR_PATTERN_63_32_NT_OFFSET, &err_pattern_63_32));
    CHECK(pmd_read_field(mss, TX_DATAPATH_REG9_ADDR, TX_DATAPATH_REG9_ERROR_PATTERN_31_0_NT_MASK, TX_DATAPATH_REG9_ERROR_PATTERN_31_0_NT_OFFSET, &err_pattern_31_0));
    CHECK(pmd_read_field(mss, TX_DATAPATH_REG1_ADDR, TX_DATAPATH_REG1_TXDATA_ERROR_RATE_NT_MASK, TX_DATAPATH_REG1_TXDATA_ERROR_RATE_NT_OFFSET, err_rate));
    *err_pattern = (uint64_t)err_pattern_63_32 << 32 | (uint64_t)err_pattern_31_0;
    return AW_ERR_CODE_NONE;

}

int aw_pmd_4ln_tx_gen_err_inject_en_set(mss_access_t *mss, uint32_t enable){
    CHECK(pmd_write_field(mss, TX_DATAPATH_REG1_ADDR, TX_DATAPATH_REG1_TXDATA_ERROR_ENABLE_A_MASK, TX_DATAPATH_REG1_TXDATA_ERROR_ENABLE_A_OFFSET, enable));
    return AW_ERR_CODE_NONE;
}

int aw_pmd_4ln_tx_gen_err_inject_en_get(mss_access_t *mss, uint32_t *enable){
    CHECK(pmd_read_field(mss, TX_DATAPATH_REG1_ADDR, TX_DATAPATH_REG1_TXDATA_ERROR_ENABLE_A_MASK, TX_DATAPATH_REG1_TXDATA_ERROR_ENABLE_A_OFFSET, enable));
    return AW_ERR_CODE_NONE;
}

int aw_pmd_4ln_rx_sweep_demapper(mss_access_t *mss, uint32_t npam4_nrz, uint32_t timeout_us){
    // 0 - PAM4 mode
    // 1 - NRZ mode
    uint32_t rx_bist_lock = 0;
    int poll_result;

    if (npam4_nrz){ // nrz
        for (uint32_t i = 0; i<=1; i++) {
            CHECK(pmd_write_field(mss, RX_DEMAPPER_ADDR, RX_DEMAPPER_INVERT_ENABLE_A_MASK, RX_DEMAPPER_INVERT_ENABLE_A_OFFSET, i));

            poll_result = pmd_poll_field(mss, RX_DATABIST_TOP_RDREG1_ADDR, RX_DATABIST_TOP_RDREG1_LOCKED_NT_MASK, RX_DATABIST_TOP_RDREG1_LOCKED_NT_OFFSET, 1, timeout_us);

            if (poll_result == 0) {
                USR_PRINTF("RX_DEMAPPER_INVERT_ENABLE_A_OFFSET = %d\n",i);
                rx_bist_lock = 1;
                break;
            }
        }
    } else {
        CHECK(pmd_write_field(mss, RX_CNTRL_REG2_ADDR, RX_CNTRL_REG2_RX_GRAY_ENA_OVR_NT_MASK, RX_CNTRL_REG2_RX_GRAY_ENA_OVR_NT_OFFSET, 1));
        uint32_t params[24][4] = {
                                {0,1,2,3},
                                {0,1,3,2},
                                {0,2,1,3},
                                {0,2,3,1},
                                {0,3,1,2},
                                {0,3,2,1},
                                {1,0,2,3},
                                {1,0,3,2},
                                {1,2,0,3},
                                {1,2,3,0},
                                {1,3,0,2},
                                {1,3,2,0},
                                {2,0,1,3},
                                {2,0,3,1},
                                {2,1,0,3},
                                {2,1,3,0},
                                {2,3,0,1},
                                {2,3,1,0},
                                {3,0,1,2},
                                {3,0,2,1},
                                {3,1,0,2},
                                {3,1,2,0},
                                {3,2,0,1},
                                {3,2,1,0}
                                };
        for (uint32_t i = 0; i<=23; i++){
            CHECK(pmd_write_field(mss, RX_DEMAPPER_ADDR, RX_DEMAPPER_EL3_VAL_NT_MASK, RX_DEMAPPER_EL3_VAL_NT_OFFSET, params[i][0]));
            CHECK(pmd_write_field(mss, RX_DEMAPPER_ADDR, RX_DEMAPPER_EL1_VAL_NT_MASK, RX_DEMAPPER_EL1_VAL_NT_OFFSET, params[i][1]));
            CHECK(pmd_write_field(mss, RX_DEMAPPER_ADDR, RX_DEMAPPER_EH1_VAL_NT_MASK, RX_DEMAPPER_EH1_VAL_NT_OFFSET, params[i][2]));
            CHECK(pmd_write_field(mss, RX_DEMAPPER_ADDR, RX_DEMAPPER_EH3_VAL_NT_MASK, RX_DEMAPPER_EH3_VAL_NT_OFFSET, params[i][3]));
            poll_result = pmd_poll_field(mss, RX_DATABIST_TOP_RDREG1_ADDR, RX_DATABIST_TOP_RDREG1_LOCKED_NT_MASK, RX_DATABIST_TOP_RDREG1_LOCKED_NT_OFFSET, 1, timeout_us);

            if (poll_result == 0) {
                USR_PRINTF("RX_DEMAPPER_EL3 = %d ; RX_DEMAPPER_EL1 = %d ; RX_DEMAPPER_EH1 = %d ;RX_DEMAPPER_EH3 = %d ;\n",params[i][0], params[i][1], params[i][2], params[i][3]);
                rx_bist_lock = 1;
                break;
            }
        }
    }

    if (rx_bist_lock ==1 ){
        USR_PRINTF("RX BIST locked after sweeping demapper\n");
        return AW_ERR_CODE_NONE;
    } else {
        USR_PRINTF("ERROR: RXBIST did not lock after sweeping demapper\n");
        return AW_ERR_CODE_FUNC_FAILURE; // no bist lock, couldn't find a mapping
    }
}

int aw_pmd_4ln_gen_tx_swap_msb_lsb_set(mss_access_t *mss, uint32_t en){
    // 0 - MSB/LSB Swap disabled in TX BIST Gen
    // 1 - MSB/LSB Swap enabled in TX BIST Gen
    CHECK(pmd_write_field(mss, TX_DATAPATH_REG2_ADDR, TX_DATAPATH_REG2_SWAP_MSB_LSB_A_MASK, TX_DATAPATH_REG2_SWAP_MSB_LSB_A_OFFSET, en));
    return AW_ERR_CODE_NONE;
}
int aw_pmd_4ln_gen_tx_swap_msb_lsb_get(mss_access_t *mss, uint32_t *en){
    CHECK(pmd_read_field(mss, TX_DATAPATH_REG2_ADDR, TX_DATAPATH_REG2_SWAP_MSB_LSB_A_MASK, TX_DATAPATH_REG2_SWAP_MSB_LSB_A_OFFSET, en));
    return AW_ERR_CODE_NONE;
}
int aw_pmd_4ln_gen_rx_swap_msb_lsb_set(mss_access_t *mss, uint32_t en){
    // 0 - MSB/LSB Swap disabled in RX BIST Gen
    // 1 - MSB/LSB Swap enabled in RX BIST Gen
    CHECK(pmd_write_field(mss, RX_DATABIST_TOP_REG1_ADDR, RX_DATABIST_TOP_REG1_SWAP_MSB_LSB_A_MASK, RX_DATABIST_TOP_REG1_SWAP_MSB_LSB_A_OFFSET, en));
    return AW_ERR_CODE_NONE;
}
int aw_pmd_4ln_gen_rx_swap_msb_lsb_get(mss_access_t *mss, uint32_t *en){
    // 0 - MSB/LSB Swap disabled in RX BIST Gen
    // 1 - MSB/LSB Swap enabled in RX BIST Gen
    CHECK(pmd_read_field(mss, RX_DATABIST_TOP_REG1_ADDR, RX_DATABIST_TOP_REG1_SWAP_MSB_LSB_A_MASK, RX_DATABIST_TOP_REG1_SWAP_MSB_LSB_A_OFFSET, en));
    return AW_ERR_CODE_NONE;
}

// UC APIs

/**
 * Default method for loading firmware and pointers. Needs to take in an array
 * that contains both pointers and SRAM values, or a seperate structure
 * containing pointer info, and then also the arry to the SRAM instructions.
 *
 * It will load the pointers to the CSRs via the PMI interface and the FW to
 * the SRAM via the PRAM interface.
 *
 */
int aw_pmd_4ln_uc_ucode_load(mss_access_t *mss, aw_ucode_t *ucode, uint32_t ucode_len){
    for (uint32_t i = 0; i<ucode_len; i++){
        CHECK(pmd_write_addr(mss, ucode[i].address, ucode[i].value)); // addr, val
    }
    return AW_ERR_CODE_NONE;
}

// int aw_pmd_4ln_pll_lock_get(mss_access_t *mss, uint32_t *pll_lock){
//     CHECK(pmd_read_field(mss, SEQ_CNTRL_CMN_ADDR, SEQ_CNTRL_CMN_LCPLL_LOCK_A_MASK, SEQ_CNTRL_CMN_LCPLL_LOCK_A_OFFSET, pll_lock));
//     return AW_ERR_CODE_NONE;
// }

int aw_pmd_4ln_pll_lock_max_set(mss_access_t *mss, uint32_t val){
    CHECK(pmd_write_field(mss, LCPLL_CHECK_ADDR, LCPLL_CHECK_MAX_NT_MASK, LCPLL_CHECK_MAX_NT_OFFSET, val));
    return AW_ERR_CODE_NONE;
}

int aw_pmd_4ln_pll_lock_min_set(mss_access_t *mss, uint32_t val){
    CHECK(pmd_write_field(mss, LCPLL_CHECK_ADDR, LCPLL_CHECK_MIN_NT_MASK, LCPLL_CHECK_MIN_NT_OFFSET, val));
    return AW_ERR_CODE_NONE;
}

int aw_pmd_4ln_pll_lock_get(mss_access_t *mss, uint32_t *pll_lock ,uint32_t check_en, uint32_t expected_val){
    int poll_result;
    CHECK(pmd_write_field(mss, LCPLL_CHECK_ADDR, LCPLL_CHECK_START_A_MASK, LCPLL_CHECK_START_A_OFFSET, 1));
    poll_result = pmd_poll_field(mss, LCPLL_CHECK_RDREG_ADDR, LCPLL_CHECK_RDREG_DONE_A_MASK, LCPLL_CHECK_RDREG_DONE_A_OFFSET, 1, 20);

    if (poll_result == -1) {
        USR_PRINTF("ERROR: Polling for LC PLL Lock check done\n");
        return AW_ERR_CODE_POLL_TIMEOUT;
    } else {
        USR_PRINTF("LC PLL Lock check done\n");
        if (check_en) {
            CHECK(pmd_read_check_field(mss, LCPLL_CHECK_RDREG_ADDR, LCPLL_CHECK_RDREG_STAT_NT_MASK, LCPLL_CHECK_RDREG_STAT_NT_OFFSET, RD_EQ, pll_lock, expected_val, 0 /*NULL*/));
        } else {
            CHECK(pmd_read_field(mss, LCPLL_CHECK_RDREG_ADDR, LCPLL_CHECK_RDREG_STAT_NT_MASK, LCPLL_CHECK_RDREG_STAT_NT_OFFSET, pll_lock));
        }
    }
    CHECK(pmd_write_field(mss, LCPLL_CHECK_ADDR, LCPLL_CHECK_START_A_MASK, LCPLL_CHECK_START_A_OFFSET, 0));
    return AW_ERR_CODE_NONE;
}

/**
 * This dump function will print all the diagnostic information about
 * the Cores Master FSM. This includes all pointer locations where all
 * the FSM routines are stored. This API uses a macro that is to be
 * user defined.
 */
int aw_pmd_4ln_uc_diag_reg_dump(mss_access_t *mss, aw_uc_diag_regs_t *uc_diag){

    // RX
    CHECK(pmd_read_field(mss, RXMFSM_STAT_ADDR, RXMFSM_STAT_RXMFSM_RATE_CUR_MASK, RXMFSM_STAT_RXMFSM_RATE_CUR_OFFSET, &(uc_diag->rxmfsm_rate_cur) ));
    CHECK(pmd_read_field(mss, RXMFSM_STAT_ADDR, RXMFSM_STAT_RXMFSM_RATE_NEW_MASK, RXMFSM_STAT_RXMFSM_RATE_NEW_OFFSET, &(uc_diag->rxmfsm_rate_new) ));
    CHECK(pmd_read_field(mss, RXMFSM_STAT_ADDR, RXMFSM_STAT_RXMFSM_WIDTH_CUR_MASK, RXMFSM_STAT_RXMFSM_WIDTH_CUR_OFFSET, &(uc_diag->rxmfsm_width_cur) ));
    CHECK(pmd_read_field(mss, RXMFSM_STAT_ADDR, RXMFSM_STAT_RXMFSM_WIDTH_NEW_MASK, RXMFSM_STAT_RXMFSM_WIDTH_NEW_OFFSET, &(uc_diag->rxmfsm_width_new) ));
    CHECK(pmd_read_field(mss, RXMFSM_STAT_ADDR, RXMFSM_STAT_RXMFSM_RXDISABLE_MASK, RXMFSM_STAT_RXMFSM_RXDISABLE_OFFSET, &(uc_diag->rxmfsm_rxdisable) ));
    CHECK(pmd_read_field(mss, RXMFSM_STAT_ADDR, RXMFSM_STAT_RXMFSM_REQ_MASK, RXMFSM_STAT_RXMFSM_REQ_OFFSET, &(uc_diag->rxmfsm_req) ));
    CHECK(pmd_read_field(mss, RXMFSM_STAT_ADDR, RXMFSM_STAT_RXMFSM_POWER_CUR_MASK, RXMFSM_STAT_RXMFSM_POWER_CUR_OFFSET, &(uc_diag->rxmfsm_power_cur) ));
    CHECK(pmd_read_field(mss, RXMFSM_STAT_ADDR, RXMFSM_STAT_RXMFSM_POWER_NEW_MASK, RXMFSM_STAT_RXMFSM_POWER_NEW_OFFSET, &(uc_diag->rxmfsm_power_new) ));
    CHECK(pmd_read_field(mss, RXMFSM_STAT_ADDR, RXMFSM_STAT_RXMFSM_PAM_CUR_MASK, RXMFSM_STAT_RXMFSM_PAM_CUR_OFFSET, &(uc_diag->rxmfsm_pam_cur) ));
    CHECK(pmd_read_field(mss, RXMFSM_STAT_ADDR, RXMFSM_STAT_RXMFSM_PAM_NEW_MASK, RXMFSM_STAT_RXMFSM_PAM_NEW_OFFSET, &(uc_diag->rxmfsm_pam_new) ));
    CHECK(pmd_read_field(mss, RXMFSM_STAT_ADDR, RXMFSM_STAT_RXMFSM_PAM_CTRL_CUR_MASK, RXMFSM_STAT_RXMFSM_PAM_CTRL_CUR_OFFSET, &(uc_diag->rxmfsm_pam_ctrl_cur) ));
    CHECK(pmd_read_field(mss, RXMFSM_STAT_ADDR, RXMFSM_STAT_RXMFSM_PAM_CTRL_NEW_MASK, RXMFSM_STAT_RXMFSM_PAM_CTRL_NEW_OFFSET, &(uc_diag->rxmfsm_pam_ctrl_new) ));
    CHECK(pmd_read_field(mss, RXMFSM_STAT_ADDR, RXMFSM_STAT_RXMFSM_INSTR_NUM_MASK, RXMFSM_STAT_RXMFSM_INSTR_NUM_OFFSET, &(uc_diag->rxmfsm_instr_num) ));
    CHECK(pmd_read_field(mss, RXMFSM_STATE_ADDR, RXMFSM_STATE_RXMFSM_STATE_MASK, RXMFSM_STATE_RXMFSM_STATE_OFFSET, &(uc_diag->rxmfsm_state) ));
    CHECK(pmd_read_field(mss, RXIFFSM_STAT_ADDR, RXIFFSM_STAT_RXIFFSM_STATE_MASK, RXIFFSM_STAT_RXIFFSM_STATE_OFFSET, &(uc_diag->rxiffsm_state) ));
    CHECK(pmd_read_field(mss, RXMFSM_LOG_RDREG1_ADDR, RXMFSM_LOG_RDREG1_LOG0_MASK, RXMFSM_LOG_RDREG1_LOG0_OFFSET, &(uc_diag->rx_log0) ));
    CHECK(pmd_read_field(mss, RXMFSM_LOG_RDREG1_ADDR, RXMFSM_LOG_RDREG1_LOG1_MASK, RXMFSM_LOG_RDREG1_LOG1_OFFSET, &(uc_diag->rx_log1) ));
    CHECK(pmd_read_field(mss, RXMFSM_LOG_RDREG2_ADDR, RXMFSM_LOG_RDREG2_LOG2_MASK, RXMFSM_LOG_RDREG2_LOG2_OFFSET, &(uc_diag->rx_log2) ));
    CHECK(pmd_read_field(mss, RXMFSM_LOG_RDREG2_ADDR, RXMFSM_LOG_RDREG2_LOG3_MASK, RXMFSM_LOG_RDREG2_LOG3_OFFSET, &(uc_diag->rx_log3) ));
    CHECK(pmd_read_field(mss, RXMFSM_LOG_RDREG3_ADDR, RXMFSM_LOG_RDREG3_LOG4_MASK, RXMFSM_LOG_RDREG3_LOG4_OFFSET, &(uc_diag->rx_log4) ));
    CHECK(pmd_read_field(mss, RXMFSM_LOG_RDREG3_ADDR, RXMFSM_LOG_RDREG3_LOG5_MASK, RXMFSM_LOG_RDREG3_LOG5_OFFSET, &(uc_diag->rx_log5) ));
    CHECK(pmd_read_field(mss, RXMFSM_LOG_RDREG4_ADDR, RXMFSM_LOG_RDREG4_LOG6_MASK, RXMFSM_LOG_RDREG4_LOG6_OFFSET, &(uc_diag->rx_log6) ));
    CHECK(pmd_read_field(mss, RXMFSM_LOG_RDREG4_ADDR, RXMFSM_LOG_RDREG4_LOG7_MASK, RXMFSM_LOG_RDREG4_LOG7_OFFSET, &(uc_diag->rx_log7) ));
    CHECK(pmd_read_field(mss, RXMFSM_LOG_RDREG5_ADDR, RXMFSM_LOG_RDREG5_LOG8_MASK, RXMFSM_LOG_RDREG5_LOG8_OFFSET, &(uc_diag->rx_log8) ));
    // TX
    CHECK(pmd_read_field(mss, TXMFSM_STAT_ADDR, TXMFSM_STAT_TXMFSM_RATE_CUR_MASK, TXMFSM_STAT_TXMFSM_RATE_CUR_OFFSET, &(uc_diag->txmfsm_rate_cur) ));
    CHECK(pmd_read_field(mss, TXMFSM_STAT_ADDR, TXMFSM_STAT_TXMFSM_RATE_NEW_MASK, TXMFSM_STAT_TXMFSM_RATE_NEW_OFFSET, &(uc_diag->txmfsm_rate_new) ));
    CHECK(pmd_read_field(mss, TXMFSM_STAT_ADDR, TXMFSM_STAT_TXMFSM_WIDTH_CUR_MASK, TXMFSM_STAT_TXMFSM_WIDTH_CUR_OFFSET, &(uc_diag->txmfsm_width_cur) ));
    CHECK(pmd_read_field(mss, TXMFSM_STAT_ADDR, TXMFSM_STAT_TXMFSM_WIDTH_NEW_MASK, TXMFSM_STAT_TXMFSM_WIDTH_NEW_OFFSET, &(uc_diag->txmfsm_width_new) ));
    CHECK(pmd_read_field(mss, TXMFSM_STAT_ADDR, TXMFSM_STAT_TXMFSM_POWER_CUR_MASK, TXMFSM_STAT_TXMFSM_POWER_CUR_OFFSET, &(uc_diag->txmfsm_power_cur) ));
    CHECK(pmd_read_field(mss, TXMFSM_STAT_ADDR, TXMFSM_STAT_TXMFSM_POWER_NEW_MASK, TXMFSM_STAT_TXMFSM_POWER_NEW_OFFSET, &(uc_diag->txmfsm_power_new) ));
    CHECK(pmd_read_field(mss, TXMFSM_STAT_ADDR, TXMFSM_STAT_TXMFSM_PAM_CUR_MASK, TXMFSM_STAT_TXMFSM_PAM_CUR_OFFSET, &(uc_diag->txmfsm_pam_cur) ));
    CHECK(pmd_read_field(mss, TXMFSM_STAT_ADDR, TXMFSM_STAT_TXMFSM_PAM_NEW_MASK, TXMFSM_STAT_TXMFSM_PAM_NEW_OFFSET, &(uc_diag->txmfsm_pam_new) ));
    CHECK(pmd_read_field(mss, TXMFSM_STAT_ADDR, TXMFSM_STAT_TXMFSM_INSTR_NUM_MASK, TXMFSM_STAT_TXMFSM_INSTR_NUM_OFFSET, &(uc_diag->txmfsm_instr_num) ));
    CHECK(pmd_read_field(mss, TXMFSM_STAT_ADDR, TXMFSM_STAT_TXMFSM_REQ_MASK, TXMFSM_STAT_TXMFSM_REQ_OFFSET, &(uc_diag->txmfsm_req) ));
    CHECK(pmd_read_field(mss, TXMFSM_STATE_ADDR, TXMFSM_STATE_TXMFSM_STATE_MASK, TXMFSM_STATE_TXMFSM_STATE_OFFSET, &(uc_diag->txmfsm_state) ));
    CHECK(pmd_read_field(mss, TXIFFSM_STAT_ADDR, TXIFFSM_STAT_TXIFFSM_STATE_MASK, TXIFFSM_STAT_TXIFFSM_STATE_OFFSET, &(uc_diag->txiffsm_state) ));
    CHECK(pmd_read_field(mss, TXMFSM_LOG_RDREG1_ADDR, TXMFSM_LOG_RDREG1_LOG0_MASK, TXMFSM_LOG_RDREG1_LOG0_OFFSET, &(uc_diag->tx_log0) ));
    CHECK(pmd_read_field(mss, TXMFSM_LOG_RDREG1_ADDR, TXMFSM_LOG_RDREG1_LOG1_MASK, TXMFSM_LOG_RDREG1_LOG1_OFFSET, &(uc_diag->tx_log1) ));
    CHECK(pmd_read_field(mss, TXMFSM_LOG_RDREG2_ADDR, TXMFSM_LOG_RDREG2_LOG2_MASK, TXMFSM_LOG_RDREG2_LOG2_OFFSET, &(uc_diag->tx_log2) ));
    CHECK(pmd_read_field(mss, TXMFSM_LOG_RDREG2_ADDR, TXMFSM_LOG_RDREG2_LOG3_MASK, TXMFSM_LOG_RDREG2_LOG3_OFFSET, &(uc_diag->tx_log3) ));
    CHECK(pmd_read_field(mss, TXMFSM_LOG_RDREG3_ADDR, TXMFSM_LOG_RDREG3_LOG4_MASK, TXMFSM_LOG_RDREG3_LOG4_OFFSET, &(uc_diag->tx_log4) ));
    CHECK(pmd_read_field(mss, TXMFSM_LOG_RDREG3_ADDR, TXMFSM_LOG_RDREG3_LOG5_MASK, TXMFSM_LOG_RDREG3_LOG5_OFFSET, &(uc_diag->tx_log5) ));
    CHECK(pmd_read_field(mss, TXMFSM_LOG_RDREG4_ADDR, TXMFSM_LOG_RDREG4_LOG6_MASK, TXMFSM_LOG_RDREG4_LOG6_OFFSET, &(uc_diag->tx_log6) ));
    CHECK(pmd_read_field(mss, TXMFSM_LOG_RDREG4_ADDR, TXMFSM_LOG_RDREG4_LOG7_MASK, TXMFSM_LOG_RDREG4_LOG7_OFFSET, &(uc_diag->tx_log7) ));
    CHECK(pmd_read_field(mss, TXMFSM_LOG_RDREG5_ADDR, TXMFSM_LOG_RDREG5_LOG8_MASK, TXMFSM_LOG_RDREG5_LOG8_OFFSET, &(uc_diag->tx_log8) ));
    CHECK(pmd_read_field(mss, TXMFSM_LOG_RDREG5_ADDR, TXMFSM_LOG_RDREG5_LOG9_MASK, TXMFSM_LOG_RDREG5_LOG9_OFFSET, &(uc_diag->tx_log9) ));
    // CMN
    CHECK(pmd_read_field(mss, CMNMFSM_STAT_ADDR, CMNMFSM_STAT_CMNMFSM_RATE_CUR_MASK, CMNMFSM_STAT_CMNMFSM_RATE_CUR_OFFSET, &(uc_diag->cmnmfsm_rate_cur) ));
    CHECK(pmd_read_field(mss, CMNMFSM_STAT_ADDR, CMNMFSM_STAT_CMNMFSM_INSTR_NUM_MASK, CMNMFSM_STAT_CMNMFSM_INSTR_NUM_OFFSET, &(uc_diag->cmnmfsm_instr_num) ));
    CHECK(pmd_read_field(mss, CMNMFSM_STAT_ADDR, CMNMFSM_STAT_CMNMFSM_POWER_CUR_MASK, CMNMFSM_STAT_CMNMFSM_POWER_CUR_OFFSET, &(uc_diag->cmnmfsm_power_cur) ));
    CHECK(pmd_read_field(mss, CMNMFSM_STAT_ADDR, CMNMFSM_STAT_CMNMFSM_POWER_NEW_MASK, CMNMFSM_STAT_CMNMFSM_POWER_NEW_OFFSET, &(uc_diag->cmnmfsm_power_new) ));
    CHECK(pmd_read_field(mss, CMNMFSM_STAT_ADDR, CMNMFSM_STAT_CMNMFSM_RATE_NEW_MASK, CMNMFSM_STAT_CMNMFSM_RATE_NEW_OFFSET, &(uc_diag->cmnmfsm_rate_new) ));
    CHECK(pmd_read_field(mss, CMNMFSM_STAT_ADDR, CMNMFSM_STAT_CMNMFSM_REQ_MASK, CMNMFSM_STAT_CMNMFSM_REQ_OFFSET, &(uc_diag->cmnmfsm_req) ));
    CHECK(pmd_read_field(mss, CMNMFSM_STATE_ADDR, CMNMFSM_STATE_CMNMFSM_STATE_MASK, CMNMFSM_STATE_CMNMFSM_STATE_OFFSET, &(uc_diag->cmnmfsm_state) ));
    CHECK(pmd_read_field(mss, CMNIFFSM_STAT_ADDR, CMNIFFSM_STAT_CMNIFFSM_STATE_MASK, CMNIFFSM_STAT_CMNIFFSM_STATE_OFFSET, &(uc_diag->cmniffsm_state) ));
    CHECK(pmd_read_field(mss, CMNMFSM_LOG_RDREG1_ADDR, CMNMFSM_LOG_RDREG1_LOG0_MASK, CMNMFSM_LOG_RDREG1_LOG0_OFFSET, &(uc_diag->cmn_log0) ));
    CHECK(pmd_read_field(mss, CMNMFSM_LOG_RDREG1_ADDR, CMNMFSM_LOG_RDREG1_LOG1_MASK, CMNMFSM_LOG_RDREG1_LOG1_OFFSET, &(uc_diag->cmn_log1) ));
    CHECK(pmd_read_field(mss, CMNMFSM_LOG_RDREG2_ADDR, CMNMFSM_LOG_RDREG2_LOG2_MASK, CMNMFSM_LOG_RDREG2_LOG2_OFFSET, &(uc_diag->cmn_log2) ));
    CHECK(pmd_read_field(mss, CMNMFSM_LOG_RDREG2_ADDR, CMNMFSM_LOG_RDREG2_LOG3_MASK, CMNMFSM_LOG_RDREG2_LOG3_OFFSET, &(uc_diag->cmn_log3) ));
    CHECK(pmd_read_field(mss, CMNMFSM_LOG_RDREG3_ADDR, CMNMFSM_LOG_RDREG3_LOG4_MASK, CMNMFSM_LOG_RDREG3_LOG4_OFFSET, &(uc_diag->cmn_log4) ));
    CHECK(pmd_read_field(mss, CMNMFSM_LOG_RDREG3_ADDR, CMNMFSM_LOG_RDREG3_LOG5_MASK, CMNMFSM_LOG_RDREG3_LOG5_OFFSET, &(uc_diag->cmn_log5) ));
    CHECK(pmd_read_field(mss, CMNMFSM_LOG_RDREG4_ADDR, CMNMFSM_LOG_RDREG4_LOG6_MASK, CMNMFSM_LOG_RDREG4_LOG6_OFFSET, &(uc_diag->cmn_log6) ));
    CHECK(pmd_read_field(mss, CMNMFSM_LOG_RDREG4_ADDR, CMNMFSM_LOG_RDREG4_LOG7_MASK, CMNMFSM_LOG_RDREG4_LOG7_OFFSET, &(uc_diag->cmn_log7) ));
    CHECK(pmd_read_field(mss, CMNMFSM_LOG_RDREG5_ADDR, CMNMFSM_LOG_RDREG5_LOG8_MASK, CMNMFSM_LOG_RDREG5_LOG8_OFFSET, &(uc_diag->cmn_log8) ));
    CHECK(pmd_read_field(mss, CMNMFSM_LOG_RDREG5_ADDR, CMNMFSM_LOG_RDREG5_LOG9_MASK, CMNMFSM_LOG_RDREG5_LOG9_OFFSET, &(uc_diag->cmn_log9) ));

    USR_PRINTF("CMN MFSM DEBUG INFO:\n");
    USR_PRINTF("CMN_CMNMFSM_STAT_CMNMFSM_REQ: 0x%x", uc_diag->cmnmfsm_req);
    USR_PRINTF("CMN_CMNMFSM_STAT_CMNMFSM_POWER_CUR: 0x%x", uc_diag->cmnmfsm_power_cur);
    USR_PRINTF("CMN_CMNMFSM_STAT_CMNMFSM_POWER_NEW: 0x%x", uc_diag->cmnmfsm_power_new);
    USR_PRINTF("CMN_CMNMFSM_STAT_CMNMFSM_RATE_CUR: 0x%x", uc_diag->cmnmfsm_rate_cur);
    USR_PRINTF("CMN_CMNMFSM_STAT_CMNMFSM_RATE_NEW: 0x%x", uc_diag->cmnmfsm_rate_new);
    USR_PRINTF("CMN_CMNMFSM_STATE_CMNMFSM_STATE: 0x%x", uc_diag->cmnmfsm_state);
    USR_PRINTF("CMN_CMNIFFSM_STAT_CMNIFFSM_STATE: 0x%x", uc_diag->cmniffsm_state);
    USR_PRINTF("CMN_CMNMFSM_STAT_CMNMFSM_INSTR_NUM: 0x%x", uc_diag->cmnmfsm_instr_num);
    USR_PRINTF("CMN_CMNMFSM_LOG_RDREG1_LOG0: 0x%x", uc_diag->cmn_log0);
    USR_PRINTF("CMN_CMNMFSM_LOG_RDREG1_LOG1: 0x%x", uc_diag->cmn_log1);
    USR_PRINTF("CMN_CMNMFSM_LOG_RDREG2_LOG2: 0x%x", uc_diag->cmn_log2);
    USR_PRINTF("CMN_CMNMFSM_LOG_RDREG2_LOG3: 0x%x", uc_diag->cmn_log3);
    USR_PRINTF("CMN_CMNMFSM_LOG_RDREG3_LOG4: 0x%x", uc_diag->cmn_log4);
    USR_PRINTF("CMN_CMNMFSM_LOG_RDREG3_LOG5: 0x%x", uc_diag->cmn_log5);
    USR_PRINTF("CMN_CMNMFSM_LOG_RDREG4_LOG6: 0x%x", uc_diag->cmn_log6);
    USR_PRINTF("CMN_CMNMFSM_LOG_RDREG4_LOG7: 0x%x", uc_diag->cmn_log7);
    USR_PRINTF("CMN_CMNMFSM_LOG_RDREG5_LOG8: 0x%x", uc_diag->cmn_log8);
    USR_PRINTF("CMN_CMNMFSM_LOG_RDREG5_LOG9: 0x%x", uc_diag->cmn_log9);

    USR_PRINTF("TX MFSM DEBUG INFO:\n");
    USR_PRINTF("TXMFSM_STAT_TXMFSM_REQ: 0x%x", uc_diag->txmfsm_req);
    USR_PRINTF("TXMFSM_STAT_TXMFSM_POWER_CUR: 0x%x", uc_diag->txmfsm_power_cur);
    USR_PRINTF("TXMFSM_STAT_TXMFSM_POWER_NEW: 0x%x", uc_diag->txmfsm_power_new);
    USR_PRINTF("TXMFSM_STAT_TXMFSM_RATE_CUR: 0x%x", uc_diag->txmfsm_rate_cur);
    USR_PRINTF("TXMFSM_STAT_TXMFSM_RATE_NEW: 0x%x", uc_diag->txmfsm_rate_new);
    USR_PRINTF("TXMFSM_STAT_TXMFSM_WIDTH_CUR: 0x%x", uc_diag->txmfsm_width_cur);
    USR_PRINTF("TXMFSM_STAT_TXMFSM_WIDTH_NEW: 0x%x", uc_diag->txmfsm_width_new);
    USR_PRINTF("TXMFSM_STAT_TXMFSM_PAM_CUR: 0x%x", uc_diag->txmfsm_pam_cur);
    USR_PRINTF("TXMFSM_STAT_TXMFSM_PAM_NEW: 0x%x", uc_diag->txmfsm_pam_new);
    USR_PRINTF("TXMFSM_STATE_TXMFSM_STATE: 0x%x", uc_diag->txmfsm_state);
    USR_PRINTF("TXIFFSM_STAT_TXIFFSM_STATE: 0x%x", uc_diag->txiffsm_state);
    USR_PRINTF("TXMFSM_STAT_TXMFSM_INSTR_NUM: 0x%x", uc_diag->txmfsm_instr_num);
    USR_PRINTF("TXMFSM_LOG_RDREG1_LOG0: 0x%x", uc_diag->tx_log0);
    USR_PRINTF("TXMFSM_LOG_RDREG1_LOG1: 0x%x", uc_diag->tx_log1);
    USR_PRINTF("TXMFSM_LOG_RDREG2_LOG2: 0x%x", uc_diag->tx_log2);
    USR_PRINTF("TXMFSM_LOG_RDREG2_LOG3: 0x%x", uc_diag->tx_log3);
    USR_PRINTF("TXMFSM_LOG_RDREG3_LOG4: 0x%x", uc_diag->tx_log4);
    USR_PRINTF("TXMFSM_LOG_RDREG3_LOG5: 0x%x", uc_diag->tx_log5);
    USR_PRINTF("TXMFSM_LOG_RDREG4_LOG6: 0x%x", uc_diag->tx_log6);
    USR_PRINTF("TXMFSM_LOG_RDREG4_LOG7: 0x%x", uc_diag->tx_log7);
    USR_PRINTF("TXMFSM_LOG_RDREG5_LOG8: 0x%x", uc_diag->tx_log8);
    USR_PRINTF("TXMFSM_LOG_RDREG5_LOG9: 0x%x", uc_diag->tx_log9);

    USR_PRINTF("RX MFSM DEBUG INFO:\n");
    USR_PRINTF("RXMFSM_STAT_RXMFSM_REQ: 0x%x\n", uc_diag->rxmfsm_req);
    USR_PRINTF("RXMFSM_STAT_RXMFSM_POWER_CUR: 0x%x\n", uc_diag->rxmfsm_power_cur);
    USR_PRINTF("RXMFSM_STAT_RXMFSM_POWER_NEW: 0x%x\n", uc_diag->rxmfsm_power_new);
    USR_PRINTF("RXMFSM_STAT_RXMFSM_RATE_CUR: 0x%x\n", uc_diag->rxmfsm_rate_cur);
    USR_PRINTF("RXMFSM_STAT_RXMFSM_RATE_NEW: 0x%x\n", uc_diag->rxmfsm_rate_new);
    USR_PRINTF("RXMFSM_STAT_RXMFSM_WIDTH_CUR: 0x%x\n", uc_diag->rxmfsm_width_cur);
    USR_PRINTF("RXMFSM_STAT_RXMFSM_WIDTH_NEW: 0x%x\n", uc_diag->rxmfsm_width_new);
    USR_PRINTF("RXMFSM_STAT_RXMFSM_PAM_CUR: 0x%x\n", uc_diag->rxmfsm_pam_cur);
    USR_PRINTF("RXMFSM_STAT_RXMFSM_PAM_NEW: 0x%x\n", uc_diag->rxmfsm_pam_new);
    USR_PRINTF("RXMFSM_STATE_RXMFSM_STATE: 0x%x\n", uc_diag->rxmfsm_state);
    USR_PRINTF("RXIFFSM_STAT_RXIFFSM_STATE: 0x%x\n", uc_diag->rxiffsm_state);
    USR_PRINTF("RXMFSM_STAT_RXMFSM_INSTR_NUM: 0x%x\n", uc_diag->rxmfsm_instr_num);
    USR_PRINTF("RXMFSM_STAT_RXMFSM_RXDISABLE: 0x%x\n", uc_diag->rxmfsm_rxdisable);
    USR_PRINTF("RXMFSM_STAT_RXMFSM_PAM_CTRL_CUR: 0x%x\n", uc_diag->rxmfsm_pam_ctrl_cur);
    USR_PRINTF("RXMFSM_STAT_RXMFSM_PAM_CTRL_NEW: 0x%x\n", uc_diag->rxmfsm_pam_ctrl_new);
    USR_PRINTF("RXMFSM_LOG_RDREG1_LOG0: 0x%x\n", uc_diag->rx_log0);
    USR_PRINTF("RXMFSM_LOG_RDREG1_LOG1: 0x%x\n", uc_diag->rx_log1);
    USR_PRINTF("RXMFSM_LOG_RDREG2_LOG2: 0x%x\n", uc_diag->rx_log2);
    USR_PRINTF("RXMFSM_LOG_RDREG2_LOG3: 0x%x\n", uc_diag->rx_log3);
    USR_PRINTF("RXMFSM_LOG_RDREG3_LOG4: 0x%x\n", uc_diag->rx_log4);
    USR_PRINTF("RXMFSM_LOG_RDREG3_LOG5: 0x%x\n", uc_diag->rx_log5);
    USR_PRINTF("RXMFSM_LOG_RDREG4_LOG6: 0x%x\n", uc_diag->rx_log6);
    USR_PRINTF("RXMFSM_LOG_RDREG4_LOG7: 0x%x\n", uc_diag->rx_log7);
    USR_PRINTF("RXMFSM_LOG_RDREG5_LOG8: 0x%x\n", uc_diag->rx_log8);

    return AW_ERR_CODE_NONE;
}

int aw_pmd_4ln_uc_diag_logging_en_set(mss_access_t *mss, uint32_t uc_log_cmn_en, uint32_t uc_log_tx_en, uint32_t uc_log_rx_en){
    CHECK(pmd_write_field(mss, CMNMFSM_LOG_CTRL_ADDR, CMNMFSM_LOG_CTRL_EN_MASK, CMNMFSM_LOG_CTRL_EN_OFFSET, uc_log_cmn_en));
    CHECK(pmd_write_field(mss, TXMFSM_LOG_CTRL_ADDR, TXMFSM_LOG_CTRL_EN_MASK, TXMFSM_LOG_CTRL_EN_OFFSET, uc_log_tx_en));
    CHECK(pmd_write_field(mss, RXMFSM_LOG_CTRL_ADDR, RXMFSM_LOG_CTRL_EN_MASK, RXMFSM_LOG_CTRL_EN_OFFSET, uc_log_rx_en));
    return AW_ERR_CODE_NONE;
}

int aw_pmd_4ln_uc_diag_logging_en_get(mss_access_t *mss, uint32_t *uc_log_cmn_en, uint32_t *uc_log_tx_en, uint32_t *uc_log_rx_en){
    CHECK(pmd_read_field(mss, CMNMFSM_LOG_CTRL_ADDR, CMNMFSM_LOG_CTRL_EN_MASK, CMNMFSM_LOG_CTRL_EN_OFFSET, uc_log_cmn_en));
    CHECK(pmd_read_field(mss, TXMFSM_LOG_CTRL_ADDR, TXMFSM_LOG_CTRL_EN_MASK, TXMFSM_LOG_CTRL_EN_OFFSET, uc_log_tx_en));
    CHECK(pmd_read_field(mss, RXMFSM_LOG_CTRL_ADDR, RXMFSM_LOG_CTRL_EN_MASK, RXMFSM_LOG_CTRL_EN_OFFSET, uc_log_rx_en));
    return AW_ERR_CODE_NONE;
}

int aw_pmd_4ln_iso_ref_ls_en_set(mss_access_t *mss, uint32_t value){
    CHECK(pmd_write_field(mss, DIG_SOC_CMN_OVRD_ADDR, DIG_SOC_CMN_OVRD_ICTL_REF_LS_ENA_A_MASK, DIG_SOC_CMN_OVRD_ICTL_REF_LS_ENA_A_OFFSET, value));
    return AW_ERR_CODE_NONE;
}

int aw_pmd_4ln_iso_cmn_pstate_set(mss_access_t *mss, uint32_t value){
    CHECK(pmd_write_field(mss, DIG_SOC_CMN_OVRD_ADDR, DIG_SOC_CMN_OVRD_ICTL_PCLK_STATE_POWER_A_MASK, DIG_SOC_CMN_OVRD_ICTL_PCLK_STATE_POWER_A_OFFSET, value));
    return AW_ERR_CODE_NONE;
}

int aw_pmd_4ln_iso_cmn_pstate_get(mss_access_t *mss, uint32_t *value){
    CHECK(pmd_read_field(mss, DIG_SOC_CMN_OVRD_ADDR, DIG_SOC_CMN_OVRD_ICTL_PCLK_STATE_POWER_A_MASK, DIG_SOC_CMN_OVRD_ICTL_PCLK_STATE_POWER_A_OFFSET, value));
    return AW_ERR_CODE_NONE;
}

int aw_pmd_4ln_iso_cmn_state_req_set(mss_access_t *mss, uint32_t value){
    CHECK(pmd_write_field(mss, DIG_SOC_CMN_OVRD_ADDR, DIG_SOC_CMN_OVRD_ICTL_PCLK_STATE_REQ_A_MASK, DIG_SOC_CMN_OVRD_ICTL_PCLK_STATE_REQ_A_OFFSET, value));
    return AW_ERR_CODE_NONE;
}

int aw_pmd_4ln_iso_cmn_state_ack_get(mss_access_t *mss, uint32_t *cmn_state_ack){
    CHECK(pmd_read_field(mss, DIG_SOC_CMN_STAT_ADDR, DIG_SOC_CMN_STAT_OCTL_PCLK_STATE_ACK_MASK,  DIG_SOC_CMN_STAT_OCTL_PCLK_STATE_ACK_OFFSET, cmn_state_ack));
    return AW_ERR_CODE_NONE;
}

int aw_pmd_4ln_iso_tx_reset_set(mss_access_t *mss, uint32_t value) {
    CHECK(bf_pmd_4ln_write_field(mss, DIG_SOC_LANE_OVRD_REG4_ADDR, DIG_SOC_LANE_OVRD_REG4_IRST_TX_BA_MASK, DIG_SOC_LANE_OVRD_REG4_IRST_TX_BA_OFFSET, value));
    return AW_ERR_CODE_NONE;
}

int aw_pmd_4ln_iso_tx_reset_get(mss_access_t *mss, uint32_t *value) {
    CHECK(bf_pmd_4ln_read_field(mss, DIG_SOC_LANE_OVRD_REG4_ADDR, DIG_SOC_LANE_OVRD_REG4_IRST_TX_BA_MASK, DIG_SOC_LANE_OVRD_REG4_IRST_TX_BA_OFFSET, value));
    return AW_ERR_CODE_NONE;
}

int aw_pmd_4ln_iso_rx_reset_set(mss_access_t *mss, uint32_t value) {
    CHECK(bf_pmd_4ln_write_field(mss, DIG_SOC_LANE_OVRD_REG4_ADDR, DIG_SOC_LANE_OVRD_REG4_IRST_RX_BA_MASK, DIG_SOC_LANE_OVRD_REG4_IRST_RX_BA_OFFSET, value));
    return AW_ERR_CODE_NONE;
}

int aw_pmd_4ln_iso_rx_reset_get(mss_access_t *mss, uint32_t *value) {
    CHECK(bf_pmd_4ln_read_field(mss, DIG_SOC_LANE_OVRD_REG4_ADDR, DIG_SOC_LANE_OVRD_REG4_IRST_RX_BA_MASK, DIG_SOC_LANE_OVRD_REG4_IRST_RX_BA_OFFSET, value));
    return AW_ERR_CODE_NONE;
}

int aw_pmd_4ln_iso_tx_rate_set(mss_access_t *mss, uint32_t value) {
    CHECK(bf_pmd_4ln_write_field(mss, DIG_SOC_LANE_OVRD_REG2_ADDR, DIG_SOC_LANE_OVRD_REG2_ICTL_TX_STATE_RATE_A_MASK, DIG_SOC_LANE_OVRD_REG2_ICTL_TX_STATE_RATE_A_OFFSET, value));
    return AW_ERR_CODE_NONE;
}

int aw_pmd_4ln_iso_tx_rate_get(mss_access_t *mss, uint32_t *value) {
    CHECK(bf_pmd_4ln_read_field(mss, DIG_SOC_LANE_OVRD_REG2_ADDR, DIG_SOC_LANE_OVRD_REG2_ICTL_TX_STATE_RATE_A_MASK, DIG_SOC_LANE_OVRD_REG2_ICTL_TX_STATE_RATE_A_OFFSET, value));
    return AW_ERR_CODE_NONE;
}

int aw_pmd_4ln_iso_rx_rate_set(mss_access_t *mss, uint32_t value) {
    CHECK(bf_pmd_4ln_write_field(mss, DIG_SOC_LANE_OVRD_REG2_ADDR, DIG_SOC_LANE_OVRD_REG2_ICTL_RX_STATE_RATE_A_MASK, DIG_SOC_LANE_OVRD_REG2_ICTL_RX_STATE_RATE_A_OFFSET, value));
    return AW_ERR_CODE_NONE;
}

int aw_pmd_4ln_iso_rx_rate_get(mss_access_t *mss, uint32_t *value) {
    CHECK(bf_pmd_4ln_read_field(mss, DIG_SOC_LANE_OVRD_REG2_ADDR, DIG_SOC_LANE_OVRD_REG2_ICTL_RX_STATE_RATE_A_MASK, DIG_SOC_LANE_OVRD_REG2_ICTL_RX_STATE_RATE_A_OFFSET, value));
    return AW_ERR_CODE_NONE;
}

int aw_pmd_4ln_iso_tx_pstate_set(mss_access_t *mss, uint32_t value) {
    CHECK(bf_pmd_4ln_write_field(mss, DIG_SOC_LANE_OVRD_REG3_ADDR, DIG_SOC_LANE_OVRD_REG3_ICTL_TX_STATE_POWER_A_MASK, DIG_SOC_LANE_OVRD_REG3_ICTL_TX_STATE_POWER_A_OFFSET, value));
    return AW_ERR_CODE_NONE;
}

int aw_pmd_4ln_iso_tx_pstate_get(mss_access_t *mss, uint32_t *value) {
    CHECK(bf_pmd_4ln_read_field(mss, DIG_SOC_LANE_OVRD_REG3_ADDR, DIG_SOC_LANE_OVRD_REG3_ICTL_TX_STATE_POWER_A_MASK, DIG_SOC_LANE_OVRD_REG3_ICTL_TX_STATE_POWER_A_OFFSET, value));
    return AW_ERR_CODE_NONE;
}

int aw_pmd_4ln_iso_rx_pstate_set(mss_access_t *mss, uint32_t value) {
    CHECK(bf_pmd_4ln_write_field(mss, DIG_SOC_LANE_OVRD_REG3_ADDR, DIG_SOC_LANE_OVRD_REG3_ICTL_RX_STATE_POWER_A_MASK, DIG_SOC_LANE_OVRD_REG3_ICTL_RX_STATE_POWER_A_OFFSET, value));
    return AW_ERR_CODE_NONE;
}

int aw_pmd_4ln_iso_rx_pstate_get(mss_access_t *mss, uint32_t *value) {
    CHECK(bf_pmd_4ln_read_field(mss, DIG_SOC_LANE_OVRD_REG3_ADDR, DIG_SOC_LANE_OVRD_REG3_ICTL_RX_STATE_POWER_A_MASK, DIG_SOC_LANE_OVRD_REG3_ICTL_RX_STATE_POWER_A_OFFSET, value));
    return AW_ERR_CODE_NONE;
}

int aw_pmd_4ln_iso_tx_width_set(mss_access_t *mss, uint32_t value) {
    CHECK(bf_pmd_4ln_write_field(mss, DIG_SOC_LANE_OVRD_REG1_ADDR, DIG_SOC_LANE_OVRD_REG1_ICTL_TX_STATE_WIDTH_A_MASK, DIG_SOC_LANE_OVRD_REG1_ICTL_TX_STATE_WIDTH_A_OFFSET, value));
    return AW_ERR_CODE_NONE;
}

int aw_pmd_4ln_iso_tx_width_get(mss_access_t *mss, uint32_t *value) {
    CHECK(bf_pmd_4ln_read_field(mss, DIG_SOC_LANE_OVRD_REG1_ADDR, DIG_SOC_LANE_OVRD_REG1_ICTL_TX_STATE_WIDTH_A_MASK, DIG_SOC_LANE_OVRD_REG1_ICTL_TX_STATE_WIDTH_A_OFFSET, value));
    return AW_ERR_CODE_NONE;
}

int aw_pmd_4ln_iso_rx_width_set(mss_access_t *mss, uint32_t value) {
    CHECK(bf_pmd_4ln_write_field(mss, DIG_SOC_LANE_OVRD_REG2_ADDR, DIG_SOC_LANE_OVRD_REG2_ICTL_RX_STATE_WIDTH_A_MASK, DIG_SOC_LANE_OVRD_REG2_ICTL_RX_STATE_WIDTH_A_OFFSET, value));
    return AW_ERR_CODE_NONE;
}

int aw_pmd_4ln_iso_rx_width_get(mss_access_t *mss, uint32_t *value) {
    CHECK(bf_pmd_4ln_read_field(mss, DIG_SOC_LANE_OVRD_REG2_ADDR, DIG_SOC_LANE_OVRD_REG2_ICTL_RX_STATE_WIDTH_A_MASK, DIG_SOC_LANE_OVRD_REG2_ICTL_RX_STATE_WIDTH_A_OFFSET, value));
    return AW_ERR_CODE_NONE;
}

int aw_pmd_4ln_iso_tx_state_req_set(mss_access_t *mss, uint32_t value) {
    CHECK(bf_pmd_4ln_write_field(mss, DIG_SOC_LANE_OVRD_REG3_ADDR, DIG_SOC_LANE_OVRD_REG3_ICTL_TX_STATE_REQ_A_MASK, DIG_SOC_LANE_OVRD_REG3_ICTL_TX_STATE_REQ_A_OFFSET, value));
    return AW_ERR_CODE_NONE;
}

int aw_pmd_4ln_iso_rx_state_req_set(mss_access_t *mss, uint32_t value) {
    CHECK(bf_pmd_4ln_write_field(mss, DIG_SOC_LANE_OVRD_REG3_ADDR, DIG_SOC_LANE_OVRD_REG3_ICTL_RX_STATE_REQ_A_MASK, DIG_SOC_LANE_OVRD_REG3_ICTL_RX_STATE_REQ_A_OFFSET, value));
    return AW_ERR_CODE_NONE;
}

int aw_pmd_4ln_iso_tx_state_ack_get(mss_access_t *mss, uint32_t *tx_state_ack) {
    CHECK(bf_pmd_4ln_read_field(mss, DIG_SOC_LANE_STAT_REG1_ADDR, DIG_SOC_LANE_STAT_REG1_OCTL_TX_STATE_ACK_MASK, DIG_SOC_LANE_STAT_REG1_OCTL_TX_STATE_ACK_OFFSET, tx_state_ack));
    return AW_ERR_CODE_NONE;
}

int aw_pmd_4ln_iso_rx_state_ack_get(mss_access_t *mss, uint32_t *rx_state_ack) {
    CHECK(bf_pmd_4ln_read_field(mss, DIG_SOC_LANE_STAT_REG1_ADDR, DIG_SOC_LANE_STAT_REG1_OCTL_RX_STATE_ACK_MASK, DIG_SOC_LANE_STAT_REG1_OCTL_RX_STATE_ACK_OFFSET, rx_state_ack));
    return AW_ERR_CODE_NONE;
}

int aw_pmd_4ln_isolate_cmn_set(mss_access_t *mss, uint32_t en) {
    CHECK(pmd_write_field(mss, DIG_SOC_CMN_OVRD_ADDR, DIG_SOC_CMN_OVRD_CMN_OVRD_EN_A_MASK, DIG_SOC_CMN_OVRD_CMN_OVRD_EN_A_OFFSET, en));
    CHECK(pmd_write_field(mss, DIG_SOC_CMN_OVRD_ADDR, DIG_SOC_CMN_OVRD_CTRL_CLK_A_MASK, DIG_SOC_CMN_OVRD_CTRL_CLK_A_OFFSET, en));
    return AW_ERR_CODE_NONE;
}

int aw_pmd_4ln_isolate_cmn_get(mss_access_t *mss, uint32_t *en) {
    CHECK(pmd_read_field(mss, DIG_SOC_CMN_OVRD_ADDR, DIG_SOC_CMN_OVRD_CMN_OVRD_EN_A_MASK, DIG_SOC_CMN_OVRD_CMN_OVRD_EN_A_OFFSET, en));
    return AW_ERR_CODE_NONE;
}

int aw_pmd_4ln_isolate_lane_set(mss_access_t *mss, uint32_t en) {
#if 0
    //Calling the isolate lane api's for TX, RX and TXRX 
    aw_pmd_4ln_isolate_lane_tx_set(mss, en);
    aw_pmd_4ln_isolate_lane_rx_set(mss, en);
    aw_pmd_4ln_isolate_lane_txrx_set(mss, en);
#else
    if (en) {
      CHECK(bf_pmd_4ln_write_field(mss, DIG_SOC_LANE_OVRD_REG5_ADDR, DIG_SOC_LANE_OVRD_REG5_LANE_DME_OVRD_EN_A_MASK, DIG_SOC_LANE_OVRD_REG5_LANE_DME_OVRD_EN_A_OFFSET, 0));
    CHECK(bf_pmd_4ln_write_field(mss, DIG_SOC_LANE_OVRD_REG5_ADDR, DIG_SOC_LANE_OVRD_REG5_LANE_RXTERM_OVRD_EN_A_MASK, DIG_SOC_LANE_OVRD_REG5_LANE_RXTERM_OVRD_EN_A_OFFSET, en));
    CHECK(bf_pmd_4ln_write_field(mss, DIG_SOC_LANE_OVRD_REG5_ADDR, DIG_SOC_LANE_OVRD_REG5_LANE_RESET_OVRD_EN_A_MASK, DIG_SOC_LANE_OVRD_REG5_LANE_RESET_OVRD_EN_A_OFFSET, en));
    CHECK(bf_pmd_4ln_write_field(mss, DIG_SOC_LANE_OVRD_REG5_ADDR, DIG_SOC_LANE_OVRD_REG5_LANE_TXSTATE_OVRD_EN_A_MASK, DIG_SOC_LANE_OVRD_REG5_LANE_TXSTATE_OVRD_EN_A_OFFSET, en));
    CHECK(bf_pmd_4ln_write_field(mss, DIG_SOC_LANE_OVRD_REG5_ADDR, DIG_SOC_LANE_OVRD_REG5_LANE_RXSTATE_OVRD_EN_A_MASK, DIG_SOC_LANE_OVRD_REG5_LANE_RXSTATE_OVRD_EN_A_OFFSET, en));
    CHECK(bf_pmd_4ln_write_field(mss, DIG_SOC_LANE_OVRD_REG5_ADDR, DIG_SOC_LANE_OVRD_REG5_LANE_RXLINKEVAL_OVRD_EN_A_MASK, DIG_SOC_LANE_OVRD_REG5_LANE_RXLINKEVAL_OVRD_EN_A_OFFSET, en));
      } else {
      CHECK(bf_pmd_4ln_write_field(mss, DIG_SOC_LANE_OVRD_REG5_ADDR, DIG_SOC_LANE_OVRD_REG5_LANE_TX_BLOCK_DATA_ENA_OVRD_EN_A_MASK, DIG_SOC_LANE_OVRD_REG5_LANE_TX_BLOCK_DATA_ENA_OVRD_EN_A_OFFSET, en));
    CHECK(bf_pmd_4ln_write_field(mss, DIG_SOC_LANE_OVRD_REG1_ADDR, DIG_SOC_LANE_OVRD_REG1_ICTL_CK_TX_BLOCK_DATA_ENA_A_MASK, DIG_SOC_LANE_OVRD_REG1_ICTL_CK_TX_BLOCK_DATA_ENA_A_OFFSET, en));
    CHECK(bf_pmd_4ln_write_field(mss, DIG_SOC_LANE_OVRD_REG5_ADDR, DIG_SOC_LANE_OVRD_REG5_CTRL_CLK_A_MASK, DIG_SOC_LANE_OVRD_REG5_CTRL_CLK_A_OFFSET, en));
    CHECK(bf_pmd_4ln_write_field(mss, DIG_SOC_LANE_OVRD_REG5_ADDR, DIG_SOC_LANE_OVRD_REG5_TXSOC_CLK_A_MASK, DIG_SOC_LANE_OVRD_REG5_TXSOC_CLK_A_OFFSET, en));
    CHECK(bf_pmd_4ln_write_field(mss, DIG_SOC_LANE_OVRD_REG5_ADDR, DIG_SOC_LANE_OVRD_REG5_LANE_DME_OVRD_EN_A_MASK, DIG_SOC_LANE_OVRD_REG5_LANE_DME_OVRD_EN_A_OFFSET, en));
    CHECK(bf_pmd_4ln_write_field(mss, DIG_SOC_LANE_OVRD_REG5_ADDR, DIG_SOC_LANE_OVRD_REG5_LANE_TXBEACON_OVRD_EN_A_MASK, DIG_SOC_LANE_OVRD_REG5_LANE_TXBEACON_OVRD_EN_A_OFFSET, en));
    CHECK(bf_pmd_4ln_write_field(mss, DIG_SOC_LANE_OVRD_REG5_ADDR, DIG_SOC_LANE_OVRD_REG5_LANE_RXTERM_OVRD_EN_A_MASK, DIG_SOC_LANE_OVRD_REG5_LANE_RXTERM_OVRD_EN_A_OFFSET, en));
    CHECK(bf_pmd_4ln_write_field(mss, DIG_SOC_LANE_OVRD_REG5_ADDR, DIG_SOC_LANE_OVRD_REG5_LANE_RXDISABLE_OVRD_EN_A_MASK, DIG_SOC_LANE_OVRD_REG5_LANE_RXDISABLE_OVRD_EN_A_OFFSET, en));
    CHECK(bf_pmd_4ln_write_field(mss, DIG_SOC_LANE_OVRD_REG5_ADDR, DIG_SOC_LANE_OVRD_REG5_LANE_PCIEL1_OVRD_EN_A_MASK, DIG_SOC_LANE_OVRD_REG5_LANE_PCIEL1_OVRD_EN_A_OFFSET, en));
    CHECK(bf_pmd_4ln_write_field(mss, DIG_SOC_LANE_OVRD_REG5_ADDR, DIG_SOC_LANE_OVRD_REG5_LANE_BYPASS_OVRD_EN_A_MASK, DIG_SOC_LANE_OVRD_REG5_LANE_BYPASS_OVRD_EN_A_OFFSET, en));
    CHECK(bf_pmd_4ln_write_field(mss, DIG_SOC_LANE_OVRD_REG5_ADDR, DIG_SOC_LANE_OVRD_REG5_LANE_RESET_OVRD_EN_A_MASK, DIG_SOC_LANE_OVRD_REG5_LANE_RESET_OVRD_EN_A_OFFSET, en));
    CHECK(bf_pmd_4ln_write_field(mss, DIG_SOC_LANE_OVRD_REG5_ADDR, DIG_SOC_LANE_OVRD_REG5_LANE_TXLEVEL_OVRD_EN_A_MASK, DIG_SOC_LANE_OVRD_REG5_LANE_TXLEVEL_OVRD_EN_A_OFFSET, en));
    CHECK(bf_pmd_4ln_write_field(mss, DIG_SOC_LANE_OVRD_REG5_ADDR, DIG_SOC_LANE_OVRD_REG5_LANE_TXFIR_OVRD_EN_A_MASK, DIG_SOC_LANE_OVRD_REG5_LANE_TXFIR_OVRD_EN_A_OFFSET, en));
    CHECK(bf_pmd_4ln_write_field(mss, DIG_SOC_LANE_OVRD_REG5_ADDR, DIG_SOC_LANE_OVRD_REG5_LANE_TXSTATE_OVRD_EN_A_MASK, DIG_SOC_LANE_OVRD_REG5_LANE_TXSTATE_OVRD_EN_A_OFFSET, en));
    CHECK(bf_pmd_4ln_write_field(mss, DIG_SOC_LANE_OVRD_REG5_ADDR, DIG_SOC_LANE_OVRD_REG5_LANE_RXSTATE_OVRD_EN_A_MASK, DIG_SOC_LANE_OVRD_REG5_LANE_RXSTATE_OVRD_EN_A_OFFSET, en));
    CHECK(bf_pmd_4ln_write_field(mss, DIG_SOC_LANE_OVRD_REG5_ADDR, DIG_SOC_LANE_OVRD_REG5_LANE_RXLINKEVAL_OVRD_EN_A_MASK, DIG_SOC_LANE_OVRD_REG5_LANE_RXLINKEVAL_OVRD_EN_A_OFFSET, en));
    CHECK(bf_pmd_4ln_write_field(mss, DIG_SOC_LANE_OVRD_REG5_ADDR, DIG_SOC_LANE_OVRD_REG5_LANE_TXRXDET_OVRD_EN_A_MASK, DIG_SOC_LANE_OVRD_REG5_LANE_TXRXDET_OVRD_EN_A_OFFSET, en));
    CHECK(bf_pmd_4ln_write_field(mss, DIG_SOC_LANE_OVRD_REG5_ADDR, DIG_SOC_LANE_OVRD_REG5_LANE_RXMARGIN_OVRD_EN_A_MASK, DIG_SOC_LANE_OVRD_REG5_LANE_RXMARGIN_OVRD_EN_A_OFFSET, en));
    CHECK(bf_pmd_4ln_write_field(mss, DIG_SOC_LANE_OVRD_REG5_ADDR, DIG_SOC_LANE_OVRD_REG5_LANE_LOOPBACK_OVRD_EN_A_MASK, DIG_SOC_LANE_OVRD_REG5_LANE_LOOPBACK_OVRD_EN_A_OFFSET, en));
    CHECK(bf_pmd_4ln_write_field(mss, DIG_SOC_LANE_OVRD_REG5_ADDR, DIG_SOC_LANE_OVRD_REG5_LANE_TXDATA_OVRD_EN_A_MASK, DIG_SOC_LANE_OVRD_REG5_LANE_TXDATA_OVRD_EN_A_OFFSET, en));
    CHECK(bf_pmd_4ln_write_field(mss, DIG_SOC_LANE_OVRD_REG5_ADDR, DIG_SOC_LANE_OVRD_REG5_LANE_ETH_AN_CTRL_OVRD_EN_A_MASK, DIG_SOC_LANE_OVRD_REG5_LANE_ETH_AN_CTRL_OVRD_EN_A_OFFSET, en));
    }
#endif
    return AW_ERR_CODE_NONE;
}

int aw_pmd_4ln_isolate_lane_tx_set(mss_access_t *mss, uint32_t en) {
    CHECK(bf_pmd_4ln_write_field(mss, DIG_SOC_LANE_OVRD_REG5_ADDR, DIG_SOC_LANE_OVRD_REG5_LANE_TX_BLOCK_DATA_ENA_OVRD_EN_A_MASK, DIG_SOC_LANE_OVRD_REG5_LANE_TX_BLOCK_DATA_ENA_OVRD_EN_A_OFFSET, en));
    CHECK(bf_pmd_4ln_write_field(mss, DIG_SOC_LANE_OVRD_REG1_ADDR, DIG_SOC_LANE_OVRD_REG1_ICTL_CK_TX_BLOCK_DATA_ENA_A_MASK, DIG_SOC_LANE_OVRD_REG1_ICTL_CK_TX_BLOCK_DATA_ENA_A_OFFSET, en));
    CHECK(bf_pmd_4ln_write_field(mss, DIG_SOC_LANE_OVRD_REG5_ADDR, DIG_SOC_LANE_OVRD_REG5_TXSOC_CLK_A_MASK, DIG_SOC_LANE_OVRD_REG5_TXSOC_CLK_A_OFFSET, en));
    CHECK(bf_pmd_4ln_write_field(mss, DIG_SOC_LANE_OVRD_REG5_ADDR, DIG_SOC_LANE_OVRD_REG5_LANE_TXBEACON_OVRD_EN_A_MASK, DIG_SOC_LANE_OVRD_REG5_LANE_TXBEACON_OVRD_EN_A_OFFSET, en));
    CHECK(bf_pmd_4ln_write_field(mss, DIG_SOC_LANE_OVRD_REG5_ADDR, DIG_SOC_LANE_OVRD_REG5_LANE_TXLEVEL_OVRD_EN_A_MASK, DIG_SOC_LANE_OVRD_REG5_LANE_TXLEVEL_OVRD_EN_A_OFFSET, en));
    CHECK(bf_pmd_4ln_write_field(mss, DIG_SOC_LANE_OVRD_REG5_ADDR, DIG_SOC_LANE_OVRD_REG5_LANE_TXFIR_OVRD_EN_A_MASK, DIG_SOC_LANE_OVRD_REG5_LANE_TXFIR_OVRD_EN_A_OFFSET, en));
    CHECK(bf_pmd_4ln_write_field(mss, DIG_SOC_LANE_OVRD_REG5_ADDR, DIG_SOC_LANE_OVRD_REG5_LANE_TXSTATE_OVRD_EN_A_MASK, DIG_SOC_LANE_OVRD_REG5_LANE_TXSTATE_OVRD_EN_A_OFFSET, en));
    CHECK(bf_pmd_4ln_write_field(mss, DIG_SOC_LANE_OVRD_REG5_ADDR, DIG_SOC_LANE_OVRD_REG5_LANE_TXRXDET_OVRD_EN_A_MASK, DIG_SOC_LANE_OVRD_REG5_LANE_TXRXDET_OVRD_EN_A_OFFSET, en));
    CHECK(bf_pmd_4ln_write_field(mss, DIG_SOC_LANE_OVRD_REG5_ADDR, DIG_SOC_LANE_OVRD_REG5_LANE_TXDATA_OVRD_EN_A_MASK, DIG_SOC_LANE_OVRD_REG5_LANE_TXDATA_OVRD_EN_A_OFFSET, en));

    return AW_ERR_CODE_NONE;
}

int aw_pmd_4ln_isolate_lane_rx_set(mss_access_t *mss, uint32_t en) {
    CHECK(bf_pmd_4ln_write_field(mss, DIG_SOC_LANE_OVRD_REG5_ADDR, DIG_SOC_LANE_OVRD_REG5_LANE_RXTERM_OVRD_EN_A_MASK, DIG_SOC_LANE_OVRD_REG5_LANE_RXTERM_OVRD_EN_A_OFFSET, en));
    CHECK(bf_pmd_4ln_write_field(mss, DIG_SOC_LANE_OVRD_REG5_ADDR, DIG_SOC_LANE_OVRD_REG5_LANE_RXDISABLE_OVRD_EN_A_MASK, DIG_SOC_LANE_OVRD_REG5_LANE_RXDISABLE_OVRD_EN_A_OFFSET, en));
    CHECK(bf_pmd_4ln_write_field(mss, DIG_SOC_LANE_OVRD_REG5_ADDR, DIG_SOC_LANE_OVRD_REG5_LANE_RXSTATE_OVRD_EN_A_MASK, DIG_SOC_LANE_OVRD_REG5_LANE_RXSTATE_OVRD_EN_A_OFFSET, en));
    CHECK(bf_pmd_4ln_write_field(mss, DIG_SOC_LANE_OVRD_REG5_ADDR, DIG_SOC_LANE_OVRD_REG5_LANE_RXLINKEVAL_OVRD_EN_A_MASK, DIG_SOC_LANE_OVRD_REG5_LANE_RXLINKEVAL_OVRD_EN_A_OFFSET, en));
    CHECK(bf_pmd_4ln_write_field(mss, DIG_SOC_LANE_OVRD_REG5_ADDR, DIG_SOC_LANE_OVRD_REG5_LANE_RXMARGIN_OVRD_EN_A_MASK, DIG_SOC_LANE_OVRD_REG5_LANE_RXMARGIN_OVRD_EN_A_OFFSET, en));

    return AW_ERR_CODE_NONE;
}

int aw_pmd_4ln_isolate_lane_txrx_set(mss_access_t *mss, uint32_t en) {
    CHECK(bf_pmd_4ln_write_field(mss, DIG_SOC_LANE_OVRD_REG5_ADDR, DIG_SOC_LANE_OVRD_REG5_CTRL_CLK_A_MASK, DIG_SOC_LANE_OVRD_REG5_CTRL_CLK_A_OFFSET, en));
    CHECK(bf_pmd_4ln_write_field(mss, DIG_SOC_LANE_OVRD_REG5_ADDR, DIG_SOC_LANE_OVRD_REG5_LANE_DME_OVRD_EN_A_MASK, DIG_SOC_LANE_OVRD_REG5_LANE_DME_OVRD_EN_A_OFFSET, en));
    CHECK(bf_pmd_4ln_write_field(mss, DIG_SOC_LANE_OVRD_REG5_ADDR, DIG_SOC_LANE_OVRD_REG5_LANE_PCIEL1_OVRD_EN_A_MASK, DIG_SOC_LANE_OVRD_REG5_LANE_PCIEL1_OVRD_EN_A_OFFSET, en));
    CHECK(bf_pmd_4ln_write_field(mss, DIG_SOC_LANE_OVRD_REG5_ADDR, DIG_SOC_LANE_OVRD_REG5_LANE_BYPASS_OVRD_EN_A_MASK, DIG_SOC_LANE_OVRD_REG5_LANE_BYPASS_OVRD_EN_A_OFFSET, en));
    CHECK(bf_pmd_4ln_write_field(mss, DIG_SOC_LANE_OVRD_REG5_ADDR, DIG_SOC_LANE_OVRD_REG5_LANE_RESET_OVRD_EN_A_MASK, DIG_SOC_LANE_OVRD_REG5_LANE_RESET_OVRD_EN_A_OFFSET, en));
    CHECK(bf_pmd_4ln_write_field(mss, DIG_SOC_LANE_OVRD_REG5_ADDR, DIG_SOC_LANE_OVRD_REG5_LANE_LOOPBACK_OVRD_EN_A_MASK, DIG_SOC_LANE_OVRD_REG5_LANE_LOOPBACK_OVRD_EN_A_OFFSET, en));
    CHECK(bf_pmd_4ln_write_field(mss, DIG_SOC_LANE_OVRD_REG5_ADDR, DIG_SOC_LANE_OVRD_REG5_LANE_ETH_AN_CTRL_OVRD_EN_A_MASK, DIG_SOC_LANE_OVRD_REG5_LANE_ETH_AN_CTRL_OVRD_EN_A_OFFSET, en));

    return AW_ERR_CODE_NONE;
}





int aw_pmd_4ln_isolate_lane_get(mss_access_t *mss, uint32_t *en) {
    CHECK(bf_pmd_4ln_read_field(mss, DIG_SOC_LANE_OVRD_REG5_ADDR, DIG_SOC_LANE_OVRD_REG5_LANE_TX_BLOCK_DATA_ENA_OVRD_EN_A_MASK, DIG_SOC_LANE_OVRD_REG5_LANE_TX_BLOCK_DATA_ENA_OVRD_EN_A_OFFSET, en));
    return AW_ERR_CODE_NONE;
}

int aw_pmd_4ln_cmn_r2l_hsref_sel_set(mss_access_t *mss, uint32_t sel) {
    CHECK(pmd_write_field(mss, CMN_REFCLK_ADDR, CMN_REFCLK_R2L_HSREF_SELECT_NT_MASK,CMN_REFCLK_R2L_HSREF_SELECT_NT_OFFSET , sel));
    return AW_ERR_CODE_NONE;
}

int aw_pmd_4ln_cmn_r2l0_lsref_sel_set(mss_access_t *mss, uint32_t sel) {
/* Applicable for all variants with refclksel_ports=1 */
    CHECK(pmd_write_field(mss, CMN_REFCLK_ADDR, CMN_REFCLK_R2L0_LSREF_SELECT_NT_MASK,CMN_REFCLK_R2L0_LSREF_SELECT_NT_OFFSET , sel));
    return AW_ERR_CODE_NONE;
}

int aw_pmd_4ln_cmn_r2l1_lsref_sel_set(mss_access_t *mss, uint32_t sel) {
/* Applicable for all variants with refclksel_ports=1 */
    CHECK(pmd_write_field(mss, CMN_REFCLK_ADDR, CMN_REFCLK_R2L1_LSREF_SELECT_NT_MASK,CMN_REFCLK_R2L1_LSREF_SELECT_NT_OFFSET , sel));
    return AW_ERR_CODE_NONE;
}

int aw_pmd_4ln_cmn_l2r_hsref_sel_set(mss_access_t *mss, uint32_t sel) {
    CHECK(pmd_write_field(mss, CMN_REFCLK_ADDR, CMN_REFCLK_L2R_HSREF_SELECT_NT_MASK,CMN_REFCLK_L2R_HSREF_SELECT_NT_OFFSET , sel));
    return AW_ERR_CODE_NONE;
}

int aw_pmd_4ln_cmn_l2r0_lsref_sel_set(mss_access_t *mss, uint32_t sel) {
/* Applicable for all variants with refclksel_ports=1 */
    CHECK(pmd_write_field(mss, CMN_REFCLK_ADDR, CMN_REFCLK_L2R0_LSREF_SELECT_NT_MASK,CMN_REFCLK_L2R0_LSREF_SELECT_NT_OFFSET , sel));
    return AW_ERR_CODE_NONE;
}

int aw_pmd_4ln_cmn_l2r1_lsref_sel_set(mss_access_t *mss, uint32_t sel) {
/* Applicable for all variants with refclksel_ports=1 */
    CHECK(pmd_write_field(mss, CMN_REFCLK_ADDR, CMN_REFCLK_L2R1_LSREF_SELECT_NT_MASK,CMN_REFCLK_L2R1_LSREF_SELECT_NT_OFFSET , sel));
    return AW_ERR_CODE_NONE;
}

int aw_pmd_4ln_cmn_r2l_hsref_sel_get(mss_access_t *mss, uint32_t *sel) {
    CHECK(pmd_read_field(mss, CMN_REFCLK_ADDR, CMN_REFCLK_R2L_HSREF_SELECT_NT_MASK,CMN_REFCLK_R2L_HSREF_SELECT_NT_OFFSET , sel));
    return AW_ERR_CODE_NONE;
}

int aw_pmd_4ln_cmn_r2l0_lsref_sel_get(mss_access_t *mss, uint32_t *sel) {
/* Applicable for all variants with refclksel_ports=1 */
    CHECK(pmd_read_field(mss, CMN_REFCLK_ADDR, CMN_REFCLK_R2L0_LSREF_SELECT_NT_MASK,CMN_REFCLK_R2L0_LSREF_SELECT_NT_OFFSET , sel));
    return AW_ERR_CODE_NONE;
}

int aw_pmd_4ln_cmn_r2l1_lsref_sel_get(mss_access_t *mss, uint32_t *sel) {
/* Applicable for all variants with refclksel_ports=1 */
    CHECK(pmd_read_field(mss, CMN_REFCLK_ADDR, CMN_REFCLK_R2L1_LSREF_SELECT_NT_MASK,CMN_REFCLK_R2L1_LSREF_SELECT_NT_OFFSET , sel));
    return AW_ERR_CODE_NONE;
}

int aw_pmd_4ln_cmn_l2r_hsref_sel_get(mss_access_t *mss, uint32_t *sel) {
    CHECK(pmd_read_field(mss, CMN_REFCLK_ADDR, CMN_REFCLK_L2R_HSREF_SELECT_NT_MASK,CMN_REFCLK_L2R_HSREF_SELECT_NT_OFFSET , sel));
    return AW_ERR_CODE_NONE;
}

int aw_pmd_4ln_cmn_l2r0_lsref_sel_get(mss_access_t *mss, uint32_t *sel) {
/* Applicable for all variants with refclksel_ports=1 */
    CHECK(pmd_read_field(mss, CMN_REFCLK_ADDR, CMN_REFCLK_L2R0_LSREF_SELECT_NT_MASK,CMN_REFCLK_L2R0_LSREF_SELECT_NT_OFFSET , sel));
    return AW_ERR_CODE_NONE;
}

int aw_pmd_4ln_cmn_l2r1_lsref_sel_get(mss_access_t *mss, uint32_t *sel) {
/* Applicable for all variants with refclksel_ports=1 */
    CHECK(pmd_read_field(mss, CMN_REFCLK_ADDR, CMN_REFCLK_L2R1_LSREF_SELECT_NT_MASK,CMN_REFCLK_L2R1_LSREF_SELECT_NT_OFFSET , sel));
    return AW_ERR_CODE_NONE;
}

int aw_pmd_4ln_cmn_lsref_sel_set(mss_access_t *mss, uint32_t ref_sel) {
/* Applicable for all variants with refclksel_ports=1 */
    CHECK(pmd_write_field(mss, CMN_SYNTH_ADDR, CMN_SYNTH_LSREF_SELECT_NT_MASK, CMN_SYNTH_LSREF_SELECT_NT_OFFSET, ref_sel));
    return AW_ERR_CODE_NONE;
}

int aw_pmd_4ln_cmn_lsref_sel_get(mss_access_t *mss, uint32_t *ref_sel) {
/* Applicable for all variants with refclksel_ports=1 */
    CHECK(pmd_read_field(mss, CMN_SYNTH_ADDR, CMN_SYNTH_LSREF_SELECT_NT_MASK, CMN_SYNTH_LSREF_SELECT_NT_OFFSET, ref_sel));
    return AW_ERR_CODE_NONE;
}

int aw_pmd_4ln_pcie_cmn_lsref_25m_set(mss_access_t *mss, uint32_t lsref_25m) {
    CHECK(pmd_write_field(mss, SWITCHCLK_DBE_CMN_ADDR, SWITCHCLK_DBE_CMN_REFCLK_SEL_NT_MASK, SWITCHCLK_DBE_CMN_REFCLK_SEL_NT_OFFSET, lsref_25m));
    return AW_ERR_CODE_NONE;
}

int aw_pmd_4ln_pcie_cmn_lsref_25m_get(mss_access_t *mss, uint32_t *lsref_25m) {
    CHECK(pmd_read_field(mss, SWITCHCLK_DBE_CMN_ADDR, SWITCHCLK_DBE_CMN_REFCLK_SEL_NT_MASK, SWITCHCLK_DBE_CMN_REFCLK_SEL_NT_OFFSET, lsref_25m));
    return AW_ERR_CODE_NONE;
}

int aw_pmd_4ln_gen_tx_en_set(mss_access_t *mss, uint32_t value) {
    CHECK(pmd_write_field(mss, TX_DATAPATH_REG1_ADDR, TX_DATAPATH_REG1_BIST_ENABLE_A_MASK, TX_DATAPATH_REG1_BIST_ENABLE_A_OFFSET, value));
    return AW_ERR_CODE_NONE;
}

int aw_pmd_4ln_gen_tx_en_get(mss_access_t *mss, uint32_t *value) {
    CHECK(pmd_read_field(mss, TX_DATAPATH_REG1_ADDR, TX_DATAPATH_REG1_BIST_ENABLE_A_MASK, TX_DATAPATH_REG1_BIST_ENABLE_A_OFFSET, value));
    return AW_ERR_CODE_NONE;
}

int aw_pmd_4ln_rx_error_cnt_done_get(mss_access_t *mss, uint32_t *err_count_done) {
    CHECK(pmd_read_field(mss, RX_DATABIST_TOP_RDREG1_ADDR, RX_DATABIST_TOP_RDREG1_ERROR_CNT_DONE_NT_MASK, RX_DATABIST_TOP_RDREG1_ERROR_CNT_DONE_NT_OFFSET, err_count_done));
    return AW_ERR_CODE_NONE;
}

int aw_pmd_4ln_iso_request_cmn_state_change(mss_access_t *mss, aw_cmn_pstate_t cmn_pstate, uint32_t timeout_us) {
    int poll_result = 0;
    aw_pmd_4ln_iso_cmn_pstate_set(mss, cmn_pstate);
    aw_pmd_4ln_iso_cmn_state_req_set(mss, 1);
    // Needed for AWPRJ-8705
    USR_SLEEP(1000);
    poll_result = pmd_poll_field(mss, DIG_SOC_CMN_STAT_ADDR, DIG_SOC_CMN_STAT_OCTL_PCLK_STATE_ACK_MASK,  DIG_SOC_CMN_STAT_OCTL_PCLK_STATE_ACK_OFFSET, 1, timeout_us);

    if (poll_result == -1) {
        USR_PRINTF("ERROR: polling for CMN state ack\n");
    } else {
        USR_PRINTF("CMN state ack received\n");
    }
    aw_pmd_4ln_iso_cmn_state_req_set(mss, 0);
    if (poll_result == -1) {
        return AW_ERR_CODE_POLL_TIMEOUT;
    } else {
        return AW_ERR_CODE_NONE;
    }
}

int aw_pmd_4ln_iso_request_tx_state_change(mss_access_t *mss, aw_pstate_t tx_pstate, uint32_t tx_rate, uint32_t tx_width, uint32_t timeout_us) {
    int poll_result;
    USR_PRINTF("Setting TX rate/width/pstate\n");
    aw_pmd_4ln_iso_tx_reset_set(mss,1);
    aw_pmd_4ln_iso_tx_rate_set(mss,tx_rate);
    aw_pmd_4ln_iso_tx_width_set(mss,tx_width);
    aw_pmd_4ln_iso_tx_pstate_set(mss,tx_pstate);

    aw_pmd_4ln_iso_tx_state_req_set(mss,1);

    poll_result = pmd_poll_field(mss, DIG_SOC_LANE_STAT_REG1_ADDR, DIG_SOC_LANE_STAT_REG1_OCTL_TX_STATE_ACK_MASK, DIG_SOC_LANE_STAT_REG1_OCTL_TX_STATE_ACK_OFFSET, 1, timeout_us);

    if (poll_result == -1) {
        USR_PRINTF("ERROR: polling for TX state ack\n");
    } else {
        USR_PRINTF("TX state ack received\n");
    }

    aw_pmd_4ln_iso_tx_state_req_set(mss,0);

    if (poll_result == -1) {
        return AW_ERR_CODE_POLL_TIMEOUT;
    } else {
        return AW_ERR_CODE_NONE;
    }
}

int aw_pmd_4ln_iso_request_rx_state_change(mss_access_t *mss, aw_pstate_t rx_pstate, uint32_t rx_rate, uint32_t rx_width, uint32_t timeout_us) {
    int poll_result;
    USR_PRINTF("Setting RX rate/width/pstate\n");
    aw_pmd_4ln_iso_rx_reset_set(mss,1);
    aw_pmd_4ln_iso_rx_rate_set(mss,rx_rate);
    aw_pmd_4ln_iso_rx_width_set(mss,rx_width);
    aw_pmd_4ln_iso_rx_pstate_set(mss,rx_pstate);

    aw_pmd_4ln_iso_rx_state_req_set(mss,1);

    poll_result = pmd_poll_field(mss, DIG_SOC_LANE_STAT_REG1_ADDR, DIG_SOC_LANE_STAT_REG1_OCTL_RX_STATE_ACK_MASK, DIG_SOC_LANE_STAT_REG1_OCTL_RX_STATE_ACK_OFFSET, 1, timeout_us);

    if (poll_result == -1) {
        USR_PRINTF("ERROR: polling for RX state ack\n");
    } else {
        USR_PRINTF("RX state ack received\n");
    }

    aw_pmd_4ln_iso_rx_state_req_set(mss,0);

    if (poll_result == -1) {
        return AW_ERR_CODE_POLL_TIMEOUT;
    } else {
        return AW_ERR_CODE_NONE;
    }
}

int aw_pmd_4ln_rx_check_cdr_lock(mss_access_t *mss, uint32_t timeout_us) {
    int poll_result;
    poll_result = pmd_poll_field(mss, DIG_SOC_LANE_STAT_REG1_ADDR, DIG_SOC_LANE_STAT_REG1_OCTL_RX_DATA_VLD_MASK, DIG_SOC_LANE_STAT_REG1_OCTL_RX_DATA_VLD_OFFSET, 1, timeout_us);

    if (poll_result == -1) {
        USR_PRINTF("ERROR: RX CDR timed out waiting for lock\n");
        return AW_ERR_CODE_POLL_TIMEOUT;

    } else {
        USR_PRINTF("RX CDR is locked\n");
        return AW_ERR_CODE_NONE;
    }
}

int aw_pmd_4ln_rx_check_bist(mss_access_t *mss, aw_bist_mode_t bist_mode, uint32_t timer_threshold, uint32_t rx_width, uint32_t timeout_us, int32_t expected_errors) {
    uint32_t err_count_overflow, err_cnt_55_32, err_cnt_31_0;
    uint64_t err_count;
    uint32_t width;
    double ber;
    int poll_result;
    if (bist_mode == AW_TIMER) {
        //Timer mode
        poll_result = pmd_poll_field(mss, RX_DATABIST_TOP_RDREG1_ADDR, RX_DATABIST_TOP_RDREG1_ERROR_CNT_DONE_NT_MASK, RX_DATABIST_TOP_RDREG1_ERROR_CNT_DONE_NT_OFFSET, 1, timeout_us);

        if (poll_result == -1) {
            USR_PRINTF("ERROR: Timed out waiting for error_cnt_done\n");
            return AW_ERR_CODE_POLL_TIMEOUT;

        } else {
            if (expected_errors == -1) {
                CHECK(pmd_read_field(mss, RX_DATABIST_TOP_RDREG3_ADDR, RX_DATABIST_TOP_RDREG3_ERROR_CNT_55T32_NT_MASK, RX_DATABIST_TOP_RDREG3_ERROR_CNT_55T32_NT_OFFSET, &err_cnt_55_32));
                CHECK(pmd_read_field(mss, RX_DATABIST_TOP_RDREG2_ADDR, RX_DATABIST_TOP_RDREG2_ERROR_CNT_NT_MASK, RX_DATABIST_TOP_RDREG2_ERROR_CNT_NT_OFFSET, &err_cnt_31_0));
            } else {
                CHECK(pmd_read_check_field(mss, RX_DATABIST_TOP_RDREG3_ADDR, RX_DATABIST_TOP_RDREG3_ERROR_CNT_55T32_NT_MASK, RX_DATABIST_TOP_RDREG3_ERROR_CNT_55T32_NT_OFFSET, RD_EQ, &err_cnt_55_32, 0, 0 /*NULL*/));
                CHECK(pmd_read_check_field(mss, RX_DATABIST_TOP_RDREG2_ADDR, RX_DATABIST_TOP_RDREG2_ERROR_CNT_NT_MASK, RX_DATABIST_TOP_RDREG2_ERROR_CNT_NT_OFFSET, RD_EQ, &err_cnt_31_0, (uint32_t)expected_errors, 0 /*NULL*/));
            }
            //CHECK(pmd_read_field(mss, RX_DATABIST_TOP_REG2_ADDR, RX_DATABIST_TOP_REG2_TIMER_THRESHOLD_NT_MASK, RX_DATABIST_TOP_REG2_TIMER_THRESHOLD_NT_OFFSET, &timer_threshold));
            //CHECK(pmd_read_field(mss, RX_DATABIST_TOP_REG3_ADDR, RX_DATABIST_TOP_REG3_DATA_WIDTH_NT_MASK, RX_DATABIST_TOP_REG3_DATA_WIDTH_NT_OFFSET, &width_encoded));
            err_count = (uint64_t) err_cnt_55_32 << 32 | (uint64_t) err_cnt_31_0;
            (void)err_count;
            CHECK(pmd_read_check_field(mss, RX_DATABIST_TOP_RDREG1_ADDR, RX_DATABIST_TOP_RDREG1_ERROR_CNT_OVERFLOW_NT_MASK, RX_DATABIST_TOP_RDREG1_ERROR_CNT_OVERFLOW_NT_OFFSET, RD_EQ, &err_count_overflow, 0, 0 /*NULL*/));
            width = aw_width_decoder(rx_width);
            ber = (double) err_count / (width * timer_threshold);
            (void)ber;
            USR_PRINTF("err_count = %" PRIu64 "\n", err_count);
            USR_PRINTF("err_count_overflow = %d\n", err_count_overflow);
            USR_PRINTF("ber = %e\n", ber);

            return AW_ERR_CODE_NONE;
        }
    }
    //Should not get here
    return AW_ERR_CODE_FUNC_FAILURE;
}

int aw_pmd_4ln_rx_prefec_clear(mss_access_t *mss) {
    CHECK(pmd_write_field(mss, RX_DATABIST_TOP_REG10_ADDR, RX_DATABIST_TOP_REG10_PREFEC_CLR_A_MASK, RX_DATABIST_TOP_REG10_PREFEC_CLR_A_OFFSET, 1));
    CHECK(pmd_write_field(mss, RX_DATABIST_TOP_REG10_ADDR, RX_DATABIST_TOP_REG10_PREFEC_CLR_A_MASK, RX_DATABIST_TOP_REG10_PREFEC_CLR_A_OFFSET, 0));
    return AW_ERR_CODE_NONE;
}

int aw_pmd_4ln_rx_prefec_enable_get(mss_access_t *mss, uint32_t *enable) {
    CHECK(pmd_read_field(mss, RX_DATABIST_TOP_REG10_ADDR, RX_DATABIST_TOP_REG10_PREFEC_ENABLE_A_MASK, RX_DATABIST_TOP_REG10_PREFEC_ENABLE_A_OFFSET, enable));
    return AW_ERR_CODE_NONE;
}

int aw_pmd_4ln_rx_prefec_enable_set(mss_access_t *mss, uint32_t enable) {
    CHECK(pmd_write_field(mss, RX_DATABIST_TOP_REG10_ADDR, RX_DATABIST_TOP_REG10_PREFEC_ENABLE_A_MASK, RX_DATABIST_TOP_REG10_PREFEC_ENABLE_A_OFFSET, enable));
    return AW_ERR_CODE_NONE;
}

int aw_pmd_4ln_rx_prefec_poll(mss_access_t *mss, uint32_t timeout_us) {
    int poll_result;
    poll_result = pmd_poll_field(mss, RX_DATABIST_TOP_RDREG46_ADDR, RX_DATABIST_TOP_RDREG46_PREFEC_DONE_NT_MASK, RX_DATABIST_TOP_RDREG46_PREFEC_DONE_NT_OFFSET, 1, timeout_us);

    if (poll_result == -1) {
        USR_PRINTF("ERROR: Timed out waiting for rx prefec ack\n");
        return AW_ERR_CODE_POLL_TIMEOUT;
    } else {
        return AW_ERR_CODE_NONE;
    }
}

int aw_pmd_4ln_rx_prefec_config_get(mss_access_t *mss, uint32_t *corr_num_syms, uint32_t *symbol_size, uint32_t *wall_mode, uint32_t *sym_per_cw, uint32_t *skip_syms, uint32_t *timer_num_cw) {
    CHECK(pmd_read_field(mss, RX_DATABIST_TOP_REG10_ADDR, RX_DATABIST_TOP_REG10_PREFEC_UNCORR_SYMERR_NT_MASK,       RX_DATABIST_TOP_REG10_PREFEC_UNCORR_SYMERR_NT_OFFSET,    corr_num_syms));
    CHECK(pmd_read_field(mss, RX_DATABIST_TOP_REG10_ADDR, RX_DATABIST_TOP_REG10_PREFEC_SYMBOL_SIZE_NT_MASK,         RX_DATABIST_TOP_REG10_PREFEC_SYMBOL_SIZE_NT_OFFSET,      symbol_size));
    CHECK(pmd_read_field(mss, RX_DATABIST_TOP_REG10_ADDR, RX_DATABIST_TOP_REG10_PREFEC_MODE_A_MASK,                 RX_DATABIST_TOP_REG10_PREFEC_MODE_A_OFFSET,              wall_mode));
    CHECK(pmd_read_field(mss, RX_DATABIST_TOP_REG11_ADDR, RX_DATABIST_TOP_REG11_PREFEC_SYMBOLS_PER_CW_NT_MASK,      RX_DATABIST_TOP_REG11_PREFEC_SYMBOLS_PER_CW_NT_OFFSET,   sym_per_cw));
    CHECK(pmd_read_field(mss, RX_DATABIST_TOP_REG12_ADDR, RX_DATABIST_TOP_REG12_PREFEC_SKIP_BITS_NT_MASK,           RX_DATABIST_TOP_REG12_PREFEC_SKIP_BITS_NT_OFFSET,        skip_syms));
    CHECK(pmd_read_field(mss, RX_DATABIST_TOP_REG13_ADDR, RX_DATABIST_TOP_REG13_PREFEC_TIMER_NUM_CW_NT_MASK,        RX_DATABIST_TOP_REG13_PREFEC_TIMER_NUM_CW_NT_OFFSET,     timer_num_cw));
    return AW_ERR_CODE_NONE;
}

int aw_pmd_4ln_rx_prefec_config_set(mss_access_t *mss, uint32_t corr_num_syms, uint32_t symbol_size, uint32_t wall_mode, uint32_t sym_per_cw, uint32_t skip_syms, uint32_t timer_num_cw) {
    uint32_t search_buffer = (symbol_size==8) ? 20 : 16;
    CHECK(pmd_write_field(mss, RX_DATABIST_TOP_REG10_ADDR, RX_DATABIST_TOP_REG10_PREFEC_UNCORR_SYMERR_NT_MASK,       RX_DATABIST_TOP_REG10_PREFEC_UNCORR_SYMERR_NT_OFFSET,      corr_num_syms));
    CHECK(pmd_write_field(mss, RX_DATABIST_TOP_REG10_ADDR, RX_DATABIST_TOP_REG10_PREFEC_SEARCHBUF_SYMBOLS_NT_MASK,  RX_DATABIST_TOP_REG10_PREFEC_SEARCHBUF_SYMBOLS_NT_OFFSET,   search_buffer));
    CHECK(pmd_write_field(mss, RX_DATABIST_TOP_REG10_ADDR, RX_DATABIST_TOP_REG10_PREFEC_SYMBOL_SIZE_NT_MASK,         RX_DATABIST_TOP_REG10_PREFEC_SYMBOL_SIZE_NT_OFFSET,        symbol_size));
    CHECK(pmd_write_field(mss, RX_DATABIST_TOP_REG10_ADDR, RX_DATABIST_TOP_REG10_PREFEC_MODE_A_MASK,                 RX_DATABIST_TOP_REG10_PREFEC_MODE_A_OFFSET,                wall_mode));
    CHECK(pmd_write_field(mss, RX_DATABIST_TOP_REG11_ADDR, RX_DATABIST_TOP_REG11_PREFEC_SYMBOLS_PER_CW_NT_MASK,      RX_DATABIST_TOP_REG11_PREFEC_SYMBOLS_PER_CW_NT_OFFSET,     sym_per_cw));
    CHECK(pmd_write_field(mss, RX_DATABIST_TOP_REG12_ADDR, RX_DATABIST_TOP_REG12_PREFEC_SKIP_BITS_NT_MASK,           RX_DATABIST_TOP_REG12_PREFEC_SKIP_BITS_NT_OFFSET,          skip_syms));
    CHECK(pmd_write_field(mss, RX_DATABIST_TOP_REG13_ADDR, RX_DATABIST_TOP_REG13_PREFEC_TIMER_NUM_CW_NT_MASK,        RX_DATABIST_TOP_REG13_PREFEC_TIMER_NUM_CW_NT_OFFSET,       timer_num_cw));
    return AW_ERR_CODE_NONE;
}

int aw_pmd_4ln_rx_prefec_get_results(mss_access_t *mss, uint32_t *hist) {
    CHECK(pmd_read_field(mss, RX_DATABIST_TOP_RDREG13_ADDR, RX_DATABIST_TOP_RDREG13_PREFEC_HIST0_NT_MASK,  RX_DATABIST_TOP_RDREG13_PREFEC_HIST0_NT_OFFSET,  &hist[0]));
    CHECK(pmd_read_field(mss, RX_DATABIST_TOP_RDREG14_ADDR, RX_DATABIST_TOP_RDREG14_PREFEC_HIST1_NT_MASK,  RX_DATABIST_TOP_RDREG14_PREFEC_HIST1_NT_OFFSET,  &hist[1]));
    CHECK(pmd_read_field(mss, RX_DATABIST_TOP_RDREG15_ADDR, RX_DATABIST_TOP_RDREG15_PREFEC_HIST2_NT_MASK,  RX_DATABIST_TOP_RDREG15_PREFEC_HIST2_NT_OFFSET,  &hist[2]));
    CHECK(pmd_read_field(mss, RX_DATABIST_TOP_RDREG16_ADDR, RX_DATABIST_TOP_RDREG16_PREFEC_HIST3_NT_MASK,  RX_DATABIST_TOP_RDREG16_PREFEC_HIST3_NT_OFFSET,  &hist[3]));
    CHECK(pmd_read_field(mss, RX_DATABIST_TOP_RDREG17_ADDR, RX_DATABIST_TOP_RDREG17_PREFEC_HIST4_NT_MASK,  RX_DATABIST_TOP_RDREG17_PREFEC_HIST4_NT_OFFSET,  &hist[4]));
    CHECK(pmd_read_field(mss, RX_DATABIST_TOP_RDREG18_ADDR, RX_DATABIST_TOP_RDREG18_PREFEC_HIST5_NT_MASK,  RX_DATABIST_TOP_RDREG18_PREFEC_HIST5_NT_OFFSET,  &hist[5]));
    CHECK(pmd_read_field(mss, RX_DATABIST_TOP_RDREG19_ADDR, RX_DATABIST_TOP_RDREG19_PREFEC_HIST6_NT_MASK,  RX_DATABIST_TOP_RDREG19_PREFEC_HIST6_NT_OFFSET,  &hist[6]));
    CHECK(pmd_read_field(mss, RX_DATABIST_TOP_RDREG20_ADDR, RX_DATABIST_TOP_RDREG20_PREFEC_HIST7_NT_MASK,  RX_DATABIST_TOP_RDREG20_PREFEC_HIST7_NT_OFFSET,  &hist[7]));
    CHECK(pmd_read_field(mss, RX_DATABIST_TOP_RDREG21_ADDR, RX_DATABIST_TOP_RDREG21_PREFEC_HIST8_NT_MASK,  RX_DATABIST_TOP_RDREG21_PREFEC_HIST8_NT_OFFSET,  &hist[8]));
    CHECK(pmd_read_field(mss, RX_DATABIST_TOP_RDREG22_ADDR, RX_DATABIST_TOP_RDREG22_PREFEC_HIST9_NT_MASK,  RX_DATABIST_TOP_RDREG22_PREFEC_HIST9_NT_OFFSET,  &hist[9]));
    CHECK(pmd_read_field(mss, RX_DATABIST_TOP_RDREG23_ADDR, RX_DATABIST_TOP_RDREG23_PREFEC_HIST10_NT_MASK, RX_DATABIST_TOP_RDREG23_PREFEC_HIST10_NT_OFFSET, &hist[10]));
    CHECK(pmd_read_field(mss, RX_DATABIST_TOP_RDREG24_ADDR, RX_DATABIST_TOP_RDREG24_PREFEC_HIST11_NT_MASK, RX_DATABIST_TOP_RDREG24_PREFEC_HIST11_NT_OFFSET, &hist[11]));
    CHECK(pmd_read_field(mss, RX_DATABIST_TOP_RDREG25_ADDR, RX_DATABIST_TOP_RDREG25_PREFEC_HIST12_NT_MASK, RX_DATABIST_TOP_RDREG25_PREFEC_HIST12_NT_OFFSET, &hist[12]));
    CHECK(pmd_read_field(mss, RX_DATABIST_TOP_RDREG26_ADDR, RX_DATABIST_TOP_RDREG26_PREFEC_HIST13_NT_MASK, RX_DATABIST_TOP_RDREG26_PREFEC_HIST13_NT_OFFSET, &hist[13]));
    CHECK(pmd_read_field(mss, RX_DATABIST_TOP_RDREG27_ADDR, RX_DATABIST_TOP_RDREG27_PREFEC_HIST14_NT_MASK, RX_DATABIST_TOP_RDREG27_PREFEC_HIST14_NT_OFFSET, &hist[14]));
    CHECK(pmd_read_field(mss, RX_DATABIST_TOP_RDREG28_ADDR, RX_DATABIST_TOP_RDREG28_PREFEC_HIST15_NT_MASK, RX_DATABIST_TOP_RDREG28_PREFEC_HIST15_NT_OFFSET, &hist[15]));
    CHECK(pmd_read_field(mss, RX_DATABIST_TOP_RDREG29_ADDR, RX_DATABIST_TOP_RDREG29_PREFEC_HIST16_NT_MASK, RX_DATABIST_TOP_RDREG29_PREFEC_HIST16_NT_OFFSET, &hist[16]));
    CHECK(pmd_read_field(mss, RX_DATABIST_TOP_RDREG30_ADDR, RX_DATABIST_TOP_RDREG30_PREFEC_HIST17_NT_MASK, RX_DATABIST_TOP_RDREG30_PREFEC_HIST17_NT_OFFSET, &hist[17]));
    CHECK(pmd_read_field(mss, RX_DATABIST_TOP_RDREG31_ADDR, RX_DATABIST_TOP_RDREG31_PREFEC_HIST18_NT_MASK, RX_DATABIST_TOP_RDREG31_PREFEC_HIST18_NT_OFFSET, &hist[18]));
    CHECK(pmd_read_field(mss, RX_DATABIST_TOP_RDREG32_ADDR, RX_DATABIST_TOP_RDREG32_PREFEC_HIST19_NT_MASK, RX_DATABIST_TOP_RDREG32_PREFEC_HIST19_NT_OFFSET, &hist[19]));
    CHECK(pmd_read_field(mss, RX_DATABIST_TOP_RDREG33_ADDR, RX_DATABIST_TOP_RDREG33_PREFEC_HIST20_NT_MASK, RX_DATABIST_TOP_RDREG33_PREFEC_HIST20_NT_OFFSET, &hist[20]));
    CHECK(pmd_read_field(mss, RX_DATABIST_TOP_RDREG34_ADDR, RX_DATABIST_TOP_RDREG34_PREFEC_HIST21_NT_MASK, RX_DATABIST_TOP_RDREG34_PREFEC_HIST21_NT_OFFSET, &hist[21]));
    CHECK(pmd_read_field(mss, RX_DATABIST_TOP_RDREG35_ADDR, RX_DATABIST_TOP_RDREG35_PREFEC_HIST22_NT_MASK, RX_DATABIST_TOP_RDREG35_PREFEC_HIST22_NT_OFFSET, &hist[22]));
    CHECK(pmd_read_field(mss, RX_DATABIST_TOP_RDREG36_ADDR, RX_DATABIST_TOP_RDREG36_PREFEC_HIST23_NT_MASK, RX_DATABIST_TOP_RDREG36_PREFEC_HIST23_NT_OFFSET, &hist[23]));
    CHECK(pmd_read_field(mss, RX_DATABIST_TOP_RDREG37_ADDR, RX_DATABIST_TOP_RDREG37_PREFEC_HIST24_NT_MASK, RX_DATABIST_TOP_RDREG37_PREFEC_HIST24_NT_OFFSET, &hist[24]));
    CHECK(pmd_read_field(mss, RX_DATABIST_TOP_RDREG38_ADDR, RX_DATABIST_TOP_RDREG38_PREFEC_HIST25_NT_MASK, RX_DATABIST_TOP_RDREG38_PREFEC_HIST25_NT_OFFSET, &hist[25]));
    CHECK(pmd_read_field(mss, RX_DATABIST_TOP_RDREG39_ADDR, RX_DATABIST_TOP_RDREG39_PREFEC_HIST26_NT_MASK, RX_DATABIST_TOP_RDREG39_PREFEC_HIST26_NT_OFFSET, &hist[26]));
    CHECK(pmd_read_field(mss, RX_DATABIST_TOP_RDREG40_ADDR, RX_DATABIST_TOP_RDREG40_PREFEC_HIST27_NT_MASK, RX_DATABIST_TOP_RDREG40_PREFEC_HIST27_NT_OFFSET, &hist[27]));
    CHECK(pmd_read_field(mss, RX_DATABIST_TOP_RDREG41_ADDR, RX_DATABIST_TOP_RDREG41_PREFEC_HIST28_NT_MASK, RX_DATABIST_TOP_RDREG41_PREFEC_HIST28_NT_OFFSET, &hist[28]));
    CHECK(pmd_read_field(mss, RX_DATABIST_TOP_RDREG42_ADDR, RX_DATABIST_TOP_RDREG42_PREFEC_HIST29_NT_MASK, RX_DATABIST_TOP_RDREG42_PREFEC_HIST29_NT_OFFSET, &hist[29]));
    CHECK(pmd_read_field(mss, RX_DATABIST_TOP_RDREG43_ADDR, RX_DATABIST_TOP_RDREG43_PREFEC_HIST30_NT_MASK, RX_DATABIST_TOP_RDREG43_PREFEC_HIST30_NT_OFFSET, &hist[30]));
    CHECK(pmd_read_field(mss, RX_DATABIST_TOP_RDREG44_ADDR, RX_DATABIST_TOP_RDREG44_PREFEC_HIST31_NT_MASK, RX_DATABIST_TOP_RDREG44_PREFEC_HIST31_NT_OFFSET, &hist[31]));
    return AW_ERR_CODE_NONE;
}

int aw_pmd_4ln_eqeval_type_set(mss_access_t *mss, uint32_t eq_type) {
    CHECK(bf_pmd_4ln_write_field(mss, DIG_SOC_LANE_OVRD_REG2_ADDR, DIG_SOC_LANE_OVRD_REG2_ICTL_RX_LINKEVAL_TYPE_A_MASK, DIG_SOC_LANE_OVRD_REG2_ICTL_RX_LINKEVAL_TYPE_A_OFFSET, eq_type));
    return AW_ERR_CODE_NONE;
}

int aw_pmd_4ln_eqeval_req_set(mss_access_t *mss, uint32_t value) {
    CHECK(bf_pmd_4ln_write_field(mss, DIG_SOC_LANE_OVRD_REG2_ADDR, DIG_SOC_LANE_OVRD_REG2_ICTL_RX_LINKEVAL_REQ_A_MASK, DIG_SOC_LANE_OVRD_REG2_ICTL_RX_LINKEVAL_REQ_A_OFFSET, value));
    return AW_ERR_CODE_NONE;
}

int aw_pmd_4ln_eqeval_ack_get(mss_access_t *mss, uint32_t *eqeval_ack) {
    CHECK(bf_pmd_4ln_read_field(mss, DIG_SOC_LANE_STAT_REG1_ADDR, DIG_SOC_LANE_STAT_REG1_OCTL_RX_LINKEVAL_ACK_MASK, DIG_SOC_LANE_STAT_REG1_OCTL_RX_LINKEVAL_ACK_OFFSET, eqeval_ack));
    return AW_ERR_CODE_NONE;
}

int aw_pmd_4ln_eqeval_incdec_get(mss_access_t *mss, uint32_t *incdec) {
    CHECK(bf_pmd_4ln_read_field(mss, DIG_SOC_LANE_STAT_REG1_ADDR, DIG_SOC_LANE_STAT_REG1_OCTL_RX_LINKEVAL_DIR_MASK, DIG_SOC_LANE_STAT_REG1_OCTL_RX_LINKEVAL_DIR_OFFSET, incdec));

    return AW_ERR_CODE_NONE;
}

int aw_pmd_4ln_rx_equalize(mss_access_t *mss, aw_eq_type_t eq_type, uint32_t timeout_us) {
    int poll_result;
    uint32_t incdec;

    aw_pmd_4ln_eqeval_type_set(mss, eq_type);
    aw_pmd_4ln_eqeval_req_set(mss,1);
    poll_result = pmd_poll_field(mss, DIG_SOC_LANE_STAT_REG1_ADDR, DIG_SOC_LANE_STAT_REG1_OCTL_RX_LINKEVAL_ACK_MASK, DIG_SOC_LANE_STAT_REG1_OCTL_RX_LINKEVAL_ACK_OFFSET, 1, timeout_us);
    if (poll_result == -1) {
        USR_PRINTF("ERROR: Timed out waiting for asserting rx linkeval ack\n");
        return AW_ERR_CODE_POLL_TIMEOUT;
    }
    aw_pmd_4ln_eqeval_req_set(mss,0);

    poll_result = pmd_poll_field(mss, DIG_SOC_LANE_STAT_REG1_ADDR, DIG_SOC_LANE_STAT_REG1_OCTL_RX_LINKEVAL_ACK_MASK, DIG_SOC_LANE_STAT_REG1_OCTL_RX_LINKEVAL_ACK_OFFSET, 0, 10);

    if (poll_result == -1) {
        USR_PRINTF("ERROR: Timed out waiting for rx linkeval ack\n");
        return AW_ERR_CODE_POLL_TIMEOUT;
    } else {
        USR_PRINTF("Received RXEQ EVAL Ack\n");
        aw_pmd_4ln_eqeval_incdec_get(mss,&incdec);
        USR_PRINTF("EqEval incdec = 0x%X\n",incdec);
        return AW_ERR_CODE_NONE;
    }
}

int aw_pmd_4ln_tx_rxdet_req_set(mss_access_t *mss, uint32_t value) {
    CHECK(bf_pmd_4ln_write_field(mss, DIG_SOC_LANE_OVRD_REG3_ADDR, DIG_SOC_LANE_OVRD_REG3_ICTL_PCIE_TX_RXDET_REQ_A_MASK, DIG_SOC_LANE_OVRD_REG3_ICTL_PCIE_TX_RXDET_REQ_A_OFFSET, value));
    return AW_ERR_CODE_NONE;
}

int aw_pmd_4ln_tx_rxdet(mss_access_t *mss, uint32_t rxdet_expected, uint32_t timeout_us) {
    int poll_result;
    uint32_t rxdet_result;

    aw_pmd_4ln_tx_rxdet_req_set(mss,1);
    poll_result = pmd_poll_field(mss, DIG_SOC_LANE_STAT_REG1_ADDR, DIG_SOC_LANE_STAT_REG1_OCTL_PCIE_TX_RXDET_ACK_MASK, DIG_SOC_LANE_STAT_REG1_OCTL_PCIE_TX_RXDET_ACK_OFFSET, 1, timeout_us);

    aw_pmd_4ln_tx_rxdet_req_set(mss,0);

    if (poll_result == -1) {
        USR_PRINTF("ERROR: Timed out waiting for tx_rxdet ack\n");
        return AW_ERR_CODE_POLL_TIMEOUT;
    } else {
        USR_PRINTF("Received TX RXDET Ack\n");
        CHECK(pmd_read_check_field(mss, DIG_SOC_LANE_STAT_REG1_ADDR, DIG_SOC_LANE_STAT_REG1_OCTL_PCIE_TX_RXDET_RESULT_MASK, DIG_SOC_LANE_STAT_REG1_OCTL_PCIE_TX_RXDET_RESULT_OFFSET, RD_EQ, &rxdet_result, rxdet_expected, 0 /*NULL*/));
        if (rxdet_result != rxdet_expected) {
           USR_PRINTF("ERROR: Expected rxdet_result = %d, Actual rxdet_result = %d\n",rxdet_expected, rxdet_result);
           return AW_ERR_CODE_CHECK_FAILURE;
        } else {
            USR_PRINTF("Expected rxdet_result = %d, Actual rxdet_result = %d\n",rxdet_expected, rxdet_result);
            return AW_ERR_CODE_NONE;
        }
    }
}

int aw_pmd_4ln_tx_beacon_en_set(mss_access_t *mss, uint32_t value) {
    CHECK(bf_pmd_4ln_write_field(mss, DIG_SOC_LANE_OVRD_REG2_ADDR, DIG_SOC_LANE_OVRD_REG2_ICTL_TX_BEACON_ENA_A_MASK, DIG_SOC_LANE_OVRD_REG2_ICTL_TX_BEACON_ENA_A_OFFSET, value));
    return AW_ERR_CODE_NONE;
}

int aw_pmd_4ln_tx_beacon_en_get(mss_access_t *mss, uint32_t *value) {
    CHECK(bf_pmd_4ln_read_field(mss, DIG_SOC_LANE_OVRD_REG2_ADDR, DIG_SOC_LANE_OVRD_REG2_ICTL_TX_BEACON_ENA_A_MASK, DIG_SOC_LANE_OVRD_REG2_ICTL_TX_BEACON_ENA_A_OFFSET, value));
    return AW_ERR_CODE_NONE;
}

int aw_pmd_4ln_tx_pll_fine_code_get(mss_access_t *mss, uint32_t * tx_pll_fine_code, uint32_t center_code, int tolerance) {
    CHECK(pmd_write_field(mss, TX_SSCM_DLPF_REG3_ADDR, TX_SSCM_DLPF_REG3_RO_CSR_CAPTURE_A_MASK, TX_SSCM_DLPF_REG3_RO_CSR_CAPTURE_A_OFFSET, 0));
    CHECK(pmd_write_field(mss, TX_SSCM_DLPF_REG3_ADDR, TX_SSCM_DLPF_REG3_RO_CSR_CAPTURE_A_MASK, TX_SSCM_DLPF_REG3_RO_CSR_CAPTURE_A_OFFSET, 1));
    if (tolerance == -1) {
        //Read back fine code without check
        CHECK(pmd_read_field(mss, TX_SSCM_DLPF_RDREG2_ADDR, TX_SSCM_DLPF_RDREG2_CTL_AFE_DS_OSC_INT_NT_MASK, TX_SSCM_DLPF_RDREG2_CTL_AFE_DS_OSC_INT_NT_OFFSET, tx_pll_fine_code));
    } else {
        //Read back fine code with range check
        CHECK(pmd_read_check_field(mss, TX_SSCM_DLPF_RDREG2_ADDR, TX_SSCM_DLPF_RDREG2_CTL_AFE_DS_OSC_INT_NT_MASK, TX_SSCM_DLPF_RDREG2_CTL_AFE_DS_OSC_INT_NT_OFFSET, RD_RANGE, tx_pll_fine_code, center_code-tolerance, center_code+tolerance));
    }
    return AW_ERR_CODE_NONE;
}

int aw_pmd_4ln_tx_pll_coarse_code_get(mss_access_t *mss, uint32_t * tx_pll_coarse_code, uint32_t center_code, int tolerance) {
    if (tolerance == -1) {
        //Read back coarse code without check
        CHECK(pmd_read_field(mss, TX_VCO_ADAPT_RDREG2_ADDR, TX_VCO_ADAPT_RDREG2_DS_OSC_CAL_MASK, TX_VCO_ADAPT_RDREG2_DS_OSC_CAL_OFFSET, tx_pll_coarse_code));
    } else {
        //Read back coarse code without check
        CHECK(pmd_read_check_field(mss, TX_VCO_ADAPT_RDREG2_ADDR, TX_VCO_ADAPT_RDREG2_DS_OSC_CAL_MASK, TX_VCO_ADAPT_RDREG2_DS_OSC_CAL_OFFSET, RD_RANGE, tx_pll_coarse_code, center_code-tolerance, center_code+tolerance));
    }
    return AW_ERR_CODE_NONE;
}

int aw_pmd_4ln_rx_pll_fine_code_get(mss_access_t *mss, uint32_t * rx_pll_fine_code, uint32_t center_code, int tolerance) {
    CHECK(pmd_write_field(mss, RX_SSCM_DLPF_REG3_ADDR, RX_SSCM_DLPF_REG3_RO_CSR_CAPTURE_A_MASK, RX_SSCM_DLPF_REG3_RO_CSR_CAPTURE_A_OFFSET, 0));
    CHECK(pmd_write_field(mss, RX_SSCM_DLPF_REG3_ADDR, RX_SSCM_DLPF_REG3_RO_CSR_CAPTURE_A_MASK, RX_SSCM_DLPF_REG3_RO_CSR_CAPTURE_A_OFFSET, 1));
    if (tolerance == -1) {
        //Read back fine code without check
        CHECK(pmd_read_field(mss, RX_SSCM_DLPF_RDREG2_ADDR, RX_SSCM_DLPF_RDREG2_CTL_AFE_DS_OSC_INT_NT_MASK, RX_SSCM_DLPF_RDREG2_CTL_AFE_DS_OSC_INT_NT_OFFSET, rx_pll_fine_code));
    } else {
        //Read back fine code with range check
        CHECK(pmd_read_check_field(mss, RX_SSCM_DLPF_RDREG2_ADDR, RX_SSCM_DLPF_RDREG2_CTL_AFE_DS_OSC_INT_NT_MASK, RX_SSCM_DLPF_RDREG2_CTL_AFE_DS_OSC_INT_NT_OFFSET, RD_RANGE, rx_pll_fine_code, center_code-tolerance, center_code+tolerance));
    }
    return AW_ERR_CODE_NONE;
}

int aw_pmd_4ln_rx_pll_coarse_code_get(mss_access_t *mss, uint32_t * rx_pll_coarse_code, uint32_t center_code, int tolerance) {
    if (tolerance == -1) {
        //Read back coarse code without check
        CHECK(pmd_read_field(mss, RX_VCO_ADAPT_RDREG2_ADDR, RX_VCO_ADAPT_RDREG2_DS_OSC_CAL_MASK, RX_VCO_ADAPT_RDREG2_DS_OSC_CAL_OFFSET, rx_pll_coarse_code));
    } else {
        //Read back coarse code without check
        CHECK(pmd_read_check_field(mss, RX_VCO_ADAPT_RDREG2_ADDR, RX_VCO_ADAPT_RDREG2_DS_OSC_CAL_MASK, RX_VCO_ADAPT_RDREG2_DS_OSC_CAL_OFFSET, RD_RANGE, rx_pll_coarse_code, center_code-tolerance, center_code+tolerance));
    }
    return AW_ERR_CODE_NONE;
}

int aw_pmd_4ln_cmn_pll_fine_code_get(mss_access_t *mss, uint32_t * cmn_pll_fine_code, uint32_t center_code, int tolerance) {
    CHECK(pmd_write_field(mss, AFE_CMN_LCPLL_OSC_REG3_ADDR, AFE_CMN_LCPLL_OSC_REG3_RO_CSR_CAPTURE_A_MASK, AFE_CMN_LCPLL_OSC_REG3_RO_CSR_CAPTURE_A_OFFSET, 0));
    CHECK(pmd_write_field(mss, AFE_CMN_LCPLL_OSC_REG3_ADDR, AFE_CMN_LCPLL_OSC_REG3_RO_CSR_CAPTURE_A_MASK, AFE_CMN_LCPLL_OSC_REG3_RO_CSR_CAPTURE_A_OFFSET, 1));
    if (tolerance == -1) {
        //Read back fine code without check
        CHECK(pmd_read_field(mss, AFE_CMN_LCPLL_OSC_RDREG1_ADDR, AFE_CMN_LCPLL_OSC_RDREG1_CLKGEN_OSC_NT_MASK, AFE_CMN_LCPLL_OSC_RDREG1_CLKGEN_OSC_NT_OFFSET, cmn_pll_fine_code));
    } else {
        //Read back fine code with range check
        CHECK(pmd_read_check_field(mss, AFE_CMN_LCPLL_OSC_RDREG1_ADDR, AFE_CMN_LCPLL_OSC_RDREG1_CLKGEN_OSC_NT_MASK, AFE_CMN_LCPLL_OSC_RDREG1_CLKGEN_OSC_NT_OFFSET, RD_RANGE, cmn_pll_fine_code, center_code-tolerance, center_code+tolerance));
    }
    return AW_ERR_CODE_NONE;
}

int aw_pmd_4ln_cmn_pll_coarse_code_get(mss_access_t *mss, uint32_t * cmn_pll_coarse_code , uint32_t center_code, int tolerance) {
    if (tolerance == -1) {
        //Read back coarse code without check
        CHECK(pmd_read_field(mss, AFE_CMN_VCO_ADAPT_RDREG3_ADDR, AFE_CMN_VCO_ADAPT_RDREG3_CLKGEN_OSC_CAL_MASK, AFE_CMN_VCO_ADAPT_RDREG3_CLKGEN_OSC_CAL_OFFSET, cmn_pll_coarse_code));
    } else {
        //Read back coarse code without check
        CHECK(pmd_read_check_field(mss, AFE_CMN_VCO_ADAPT_RDREG3_ADDR, AFE_CMN_VCO_ADAPT_RDREG3_CLKGEN_OSC_CAL_MASK, AFE_CMN_VCO_ADAPT_RDREG3_CLKGEN_OSC_CAL_OFFSET, RD_RANGE, cmn_pll_coarse_code, center_code-tolerance, center_code+tolerance));
    }
    return AW_ERR_CODE_NONE;
}

int aw_pmd_4ln_rd_data_pipeline_stages_set(mss_access_t *mss, uint32_t stages) {
    CHECK(pmd_write_field(mss, SRAM0_CFG_ADDR, SRAM0_CFG_RD_DATA_PIPELINE_STAGES_A_MASK, SRAM0_CFG_RD_DATA_PIPELINE_STAGES_A_OFFSET, stages));
#if AW_NUM_LANES == 16
    CHECK(pmd_write_field(mss, SRAM1_CFG_ADDR, SRAM1_CFG_RD_DATA_PIPELINE_STAGES_A_MASK, SRAM1_CFG_RD_DATA_PIPELINE_STAGES_A_OFFSET, stages));
#endif
    return AW_ERR_CODE_NONE;
}

int aw_pmd_4ln_rd_data_pipeline_stages_get(mss_access_t *mss, uint32_t* stages) {

    CHECK(pmd_read_field(mss, SRAM0_CFG_ADDR, SRAM0_CFG_RD_DATA_PIPELINE_STAGES_A_MASK, SRAM0_CFG_RD_DATA_PIPELINE_STAGES_A_OFFSET, stages));
    return AW_ERR_CODE_NONE;

}

int aw_pmd_4ln_measure_pmon(mss_access_t *mss, uint32_t pmon_sel, uint32_t pvt_measure_timing_window, uint32_t timeout_us, uint32_t * pvt_measure_result) {
    int poll_result;

    CHECK(pmd_write_field(mss, PD_AFE_CMN_ADDR, PD_AFE_CMN_ATEST_PMON_BA_MASK, PD_AFE_CMN_ATEST_PMON_BA_OFFSET, 1));
    CHECK(pmd_write_field(mss, RST_AFE_CMN_ADDR, RST_AFE_CMN_ATEST_PMON_BA_MASK, RST_AFE_CMN_ATEST_PMON_BA_OFFSET, 1));

    CHECK(pmd_write_field(mss, AFE_CMN_ATEST_ADDR, AFE_CMN_ATEST_BYPASS_ENA_A_MASK, AFE_CMN_ATEST_BYPASS_ENA_A_OFFSET, 1));
    CHECK(pmd_write_field(mss, AFE_CMN_ATEST_ADDR, AFE_CMN_ATEST_PMONSEL_A_MASK, AFE_CMN_ATEST_PMONSEL_A_OFFSET, pmon_sel));

    CHECK(pmd_write_field(mss, PVT_MEASURE_ADDR, PVT_MEASURE_TIMING_WINDOW_NT_MASK,PVT_MEASURE_TIMING_WINDOW_NT_OFFSET, pvt_measure_timing_window));
    CHECK(pmd_write_field(mss, PVT_MEASURE_ADDR, PVT_MEASURE_FREQ_TARGET_NT_MASK,PVT_MEASURE_FREQ_TARGET_NT_OFFSET, 0));

    CHECK(pmd_write_field(mss, PVT_MEASURE_ADDR, PVT_MEASURE_REQ_A_MASK, PVT_MEASURE_REQ_A_OFFSET, 1));

    poll_result = pmd_poll_field(mss, PVT_MEASURE_RDREG_ADDR, PVT_MEASURE_RDREG_ACK_A_MASK, PVT_MEASURE_RDREG_ACK_A_OFFSET, 1, timeout_us);

    CHECK(pmd_write_field(mss, PVT_MEASURE_ADDR, PVT_MEASURE_REQ_A_MASK, PVT_MEASURE_REQ_A_OFFSET, 0));

    if (poll_result == -1) {
        USR_PRINTF("ERROR: Timed out waiting for pvt_measure_ack\n");
        return AW_ERR_CODE_POLL_TIMEOUT;
    } else {
        USR_PRINTF("Received pvt_measure_ack\n");
        CHECK(pmd_read_field(mss, PVT_MEASURE_RDREG_ADDR, PVT_MEASURE_RDREG_RESULT_MASK, PVT_MEASURE_RDREG_RESULT_OFFSET, pvt_measure_result));
        USR_PRINTF("pmon_sel = %d. pvt_measure_result = %d\n", pmon_sel, *pvt_measure_result);
        return AW_ERR_CODE_NONE;
    }
}

int aw_pmd_4ln_atest_en(mss_access_t *mss, uint32_t en) {

    CHECK(pmd_write_field(mss, AFE_CMN_ATEST_ADDR, AFE_CMN_ATEST_BYPASS_ENA_A_MASK, AFE_CMN_ATEST_BYPASS_ENA_A_OFFSET, en));
    CHECK(pmd_write_field(mss, PD_AFE_CMN_ADDR, PD_AFE_CMN_ATEST_PMON_BA_MASK, PD_AFE_CMN_ATEST_PMON_BA_OFFSET, en));
    CHECK(pmd_write_field(mss, PD_AFE_CMN_ADDR, PD_AFE_CMN_ATEST_ADC_BA_MASK, PD_AFE_CMN_ATEST_ADC_BA_OFFSET, en));
    CHECK(pmd_write_field(mss, RST_AFE_CMN_ADDR, RST_AFE_CMN_ATEST_ADC_BA_MASK, RST_AFE_CMN_ATEST_ADC_BA_OFFSET, en));
    CHECK(pmd_write_field(mss, RST_AFE_CMN_ADDR, RST_AFE_CMN_ATEST_PMON_BA_MASK, RST_AFE_CMN_ATEST_PMON_BA_OFFSET, en));

    return AW_ERR_CODE_NONE;
}

int aw_pmd_4ln_atest_cmn_capture(mss_access_t *mss, uint32_t atest_addr, uint32_t atest_term, uint32_t * atest_adc_val) {
    CHECK(pmd_write_field(mss, AFE_CMN_ATEST_ADDR, AFE_CMN_ATEST_MEAS_TERM_NT_MASK, AFE_CMN_ATEST_MEAS_TERM_NT_OFFSET, atest_term));
    CHECK(pmd_write_field(mss, CMN_ATEST_ADDR, CMN_ATEST_ADDR_BIN_NT_MASK, CMN_ATEST_ADDR_BIN_NT_OFFSET, atest_addr));
    CHECK(pmd_write_field(mss, AFE_CMN_ATEST_ADDR, AFE_CMN_ATEST_CAPTURE_ENA_NT_MASK, AFE_CMN_ATEST_CAPTURE_ENA_NT_OFFSET, 1));
    USR_SLEEP(5);
    CHECK(pmd_read_field(mss, AFE_CMN_ATEST_RDREG_ADDR, AFE_CMN_ATEST_RDREG_ADC_NT_MASK, AFE_CMN_ATEST_RDREG_ADC_NT_OFFSET, atest_adc_val));
    CHECK(pmd_write_field(mss, AFE_CMN_ATEST_ADDR, AFE_CMN_ATEST_CAPTURE_ENA_NT_MASK, AFE_CMN_ATEST_CAPTURE_ENA_NT_OFFSET, 0));
    CHECK(pmd_write_field(mss, CMN_ATEST_ADDR, CMN_ATEST_ADDR_BIN_NT_MASK, CMN_ATEST_ADDR_BIN_NT_OFFSET, 0));
    return AW_ERR_CODE_NONE;
}

int aw_pmd_4ln_atest_tx_capture(mss_access_t *mss, uint32_t atest_addr, uint32_t atest_term, uint32_t * atest_adc_val) {
    CHECK(pmd_write_field(mss, AFE_CMN_ATEST_ADDR, AFE_CMN_ATEST_MEAS_TERM_NT_MASK, AFE_CMN_ATEST_MEAS_TERM_NT_OFFSET, atest_term));
    CHECK(pmd_write_field(mss, TX_ATEST_ADDR, TX_ATEST_ADDR_BIN_NT_MASK, TX_ATEST_ADDR_BIN_NT_OFFSET, atest_addr));
    CHECK(pmd_write_field(mss, AFE_CMN_ATEST_ADDR, AFE_CMN_ATEST_CAPTURE_ENA_NT_MASK, AFE_CMN_ATEST_CAPTURE_ENA_NT_OFFSET, 1));
    USR_SLEEP(5);
    CHECK(pmd_read_field(mss, AFE_CMN_ATEST_RDREG_ADDR, AFE_CMN_ATEST_RDREG_ADC_NT_MASK, AFE_CMN_ATEST_RDREG_ADC_NT_OFFSET, atest_adc_val));
    CHECK(pmd_write_field(mss, AFE_CMN_ATEST_ADDR, AFE_CMN_ATEST_CAPTURE_ENA_NT_MASK, AFE_CMN_ATEST_CAPTURE_ENA_NT_OFFSET, 1));
    CHECK(pmd_write_field(mss, TX_ATEST_ADDR, TX_ATEST_ADDR_BIN_NT_MASK, TX_ATEST_ADDR_BIN_NT_OFFSET, 0));
    return AW_ERR_CODE_NONE;
}

int aw_pmd_4ln_atest_rx_a_capture(mss_access_t *mss, uint32_t atest_addr, uint32_t atest_term, uint32_t * atest_adc_val) {
    CHECK(pmd_write_field(mss, AFE_CMN_ATEST_ADDR, AFE_CMN_ATEST_MEAS_TERM_NT_MASK, AFE_CMN_ATEST_MEAS_TERM_NT_OFFSET, atest_term));
    CHECK(pmd_write_field(mss, RX_ATEST_ADDR, RX_ATEST_A_ADDR_BIN_NT_MASK, RX_ATEST_A_ADDR_BIN_NT_OFFSET, atest_addr));
    CHECK(pmd_write_field(mss, AFE_CMN_ATEST_ADDR, AFE_CMN_ATEST_CAPTURE_ENA_NT_MASK, AFE_CMN_ATEST_CAPTURE_ENA_NT_OFFSET, 1));
    USR_SLEEP(5);
    CHECK(pmd_read_field(mss, AFE_CMN_ATEST_RDREG_ADDR, AFE_CMN_ATEST_RDREG_ADC_NT_MASK, AFE_CMN_ATEST_RDREG_ADC_NT_OFFSET, atest_adc_val));
    CHECK(pmd_write_field(mss, AFE_CMN_ATEST_ADDR, AFE_CMN_ATEST_CAPTURE_ENA_NT_MASK, AFE_CMN_ATEST_CAPTURE_ENA_NT_OFFSET, 1));
    CHECK(pmd_write_field(mss, RX_ATEST_ADDR, RX_ATEST_A_ADDR_BIN_NT_MASK, RX_ATEST_A_ADDR_BIN_NT_OFFSET, 0));
    return AW_ERR_CODE_NONE;
}

int aw_pmd_4ln_atest_rx_b_capture(mss_access_t *mss, uint32_t atest_addr, uint32_t atest_term, uint32_t * atest_adc_val) {
    CHECK(pmd_write_field(mss, AFE_CMN_ATEST_ADDR, AFE_CMN_ATEST_MEAS_TERM_NT_MASK, AFE_CMN_ATEST_MEAS_TERM_NT_OFFSET, atest_term));
    CHECK(pmd_write_field(mss, RX_ATEST_ADDR, RX_ATEST_B_ADDR_BIN_NT_MASK, RX_ATEST_B_ADDR_BIN_NT_OFFSET, atest_addr));
    CHECK(pmd_write_field(mss, AFE_CMN_ATEST_ADDR, AFE_CMN_ATEST_CAPTURE_ENA_NT_MASK, AFE_CMN_ATEST_CAPTURE_ENA_NT_OFFSET, 1));
    USR_SLEEP(5);
    CHECK(pmd_read_field(mss, AFE_CMN_ATEST_RDREG_ADDR, AFE_CMN_ATEST_RDREG_ADC_NT_MASK, AFE_CMN_ATEST_RDREG_ADC_NT_OFFSET, atest_adc_val));
    CHECK(pmd_write_field(mss, AFE_CMN_ATEST_ADDR, AFE_CMN_ATEST_CAPTURE_ENA_NT_MASK, AFE_CMN_ATEST_CAPTURE_ENA_NT_OFFSET, 1));
    CHECK(pmd_write_field(mss, RX_ATEST_ADDR, RX_ATEST_B_ADDR_BIN_NT_MASK, RX_ATEST_B_ADDR_BIN_NT_OFFSET, 0));
    return AW_ERR_CODE_NONE;
}

int aw_pmd_4ln_atest_adc_power(mss_access_t *mss, uint32_t en) {
    if (en == 0) {
        /* Power down the ADC */
        CHECK(pmd_write_field(mss, AFE_CMN_ATEST_ADDR, AFE_CMN_ATEST_BYPASS_ENA_A_MASK  , AFE_CMN_ATEST_BYPASS_ENA_A_OFFSET  , 0));
        CHECK(pmd_write_field(mss, PD_AFE_CMN_ADDR   , PD_AFE_CMN_ATEST_ADC_BA_MASK     , PD_AFE_CMN_ATEST_ADC_BA_OFFSET     , 0));
        CHECK(pmd_write_field(mss, RST_AFE_CMN_ADDR  , RST_AFE_CMN_ATEST_ADC_BA_MASK    , RST_AFE_CMN_ATEST_ADC_BA_OFFSET    , 0));
        CHECK(pmd_write_field(mss, AFE_CMN_ATEST_ADDR, AFE_CMN_ATEST_DIV_NT_MASK        , AFE_CMN_ATEST_DIV_NT_OFFSET        , 0));
    }
    else  {
        /* Power up the ADC */
        CHECK(pmd_write_field(mss, AFE_CMN_ATEST_ADDR, AFE_CMN_ATEST_BYPASS_ENA_A_MASK  , AFE_CMN_ATEST_BYPASS_ENA_A_OFFSET  , 1));
        CHECK(pmd_write_field(mss, PD_AFE_CMN_ADDR   , PD_AFE_CMN_ATEST_ADC_BA_MASK     , PD_AFE_CMN_ATEST_ADC_BA_OFFSET     , 1));
        CHECK(pmd_write_field(mss, RST_AFE_CMN_ADDR  , RST_AFE_CMN_ATEST_ADC_BA_MASK    , RST_AFE_CMN_ATEST_ADC_BA_OFFSET    , 1));
        CHECK(pmd_write_field(mss, AFE_CMN_ATEST_ADDR, AFE_CMN_ATEST_DIV_NT_MASK        , AFE_CMN_ATEST_DIV_NT_OFFSET        , 0));
    }

    return AW_ERR_CODE_NONE;
}

int aw_pmd_4ln_atest_adc_temp_capture(mss_access_t *mss, uint32_t iterations, aw_adc_temp_data_t *mean_data) {
    uint32_t current_iteration;
    uint32_t atest_adc_running_sum;
    uint32_t atest_adc_read_val;

    NULL_CHECK(mean_data);

    /* VC */
    CHECK(pmd_write_field(mss, AFE_CMN_ATEST_ADDR, AFE_CMN_ATEST_MEAS_TERM_NT_MASK, AFE_CMN_ATEST_MEAS_TERM_NT_OFFSET, AW_ATEST_CMN_BANDGAP_VC_TERM));
    CHECK(pmd_write_field(mss, CMN_ATEST_ADDR    , CMN_ATEST_ADDR_BIN_NT_MASK     , CMN_ATEST_ADDR_BIN_NT_OFFSET     , AW_ATEST_CMN_BANDGAP_VC_ADDR));

    atest_adc_running_sum = 0;
    for (current_iteration = 0; current_iteration < iterations; current_iteration++) {
        CHECK(pmd_write_field(mss, AFE_CMN_ATEST_ADDR      , AFE_CMN_ATEST_CAPTURE_ENA_NT_MASK, AFE_CMN_ATEST_CAPTURE_ENA_NT_OFFSET, 1));
        USR_SLEEP(5);
        CHECK(pmd_read_field (mss, AFE_CMN_ATEST_RDREG_ADDR, AFE_CMN_ATEST_RDREG_ADC_NT_MASK  , AFE_CMN_ATEST_RDREG_ADC_NT_OFFSET  , &atest_adc_read_val));
        CHECK(pmd_write_field(mss, AFE_CMN_ATEST_ADDR      , AFE_CMN_ATEST_CAPTURE_ENA_NT_MASK, AFE_CMN_ATEST_CAPTURE_ENA_NT_OFFSET, 0));

        atest_adc_running_sum += atest_adc_read_val;
    }
    mean_data->vc = ((float) atest_adc_running_sum)/iterations;

    /* VB */
    CHECK(pmd_write_field(mss, AFE_CMN_ATEST_ADDR, AFE_CMN_ATEST_MEAS_TERM_NT_MASK, AFE_CMN_ATEST_MEAS_TERM_NT_OFFSET, AW_ATEST_CMN_BANDGAP_VB_TERM));
    CHECK(pmd_write_field(mss, CMN_ATEST_ADDR    , CMN_ATEST_ADDR_BIN_NT_MASK     , CMN_ATEST_ADDR_BIN_NT_OFFSET     , AW_ATEST_CMN_BANDGAP_VB_ADDR));

    atest_adc_running_sum = 0;
    for (current_iteration = 0; current_iteration < iterations; current_iteration++) {
        CHECK(pmd_write_field(mss, AFE_CMN_ATEST_ADDR      , AFE_CMN_ATEST_CAPTURE_ENA_NT_MASK, AFE_CMN_ATEST_CAPTURE_ENA_NT_OFFSET, 1));
        USR_SLEEP(5);
        CHECK(pmd_read_field (mss, AFE_CMN_ATEST_RDREG_ADDR, AFE_CMN_ATEST_RDREG_ADC_NT_MASK  , AFE_CMN_ATEST_RDREG_ADC_NT_OFFSET  , &atest_adc_read_val));
        CHECK(pmd_write_field(mss, AFE_CMN_ATEST_ADDR      , AFE_CMN_ATEST_CAPTURE_ENA_NT_MASK, AFE_CMN_ATEST_CAPTURE_ENA_NT_OFFSET, 0));

        atest_adc_running_sum += atest_adc_read_val;
    }
    mean_data->vb = ((float) atest_adc_running_sum)/iterations;
    
    /* BG1 */
    CHECK(pmd_write_field(mss, AFE_CMN_ATEST_ADDR, AFE_CMN_ATEST_MEAS_TERM_NT_MASK, AFE_CMN_ATEST_MEAS_TERM_NT_OFFSET, AW_ATEST_CMN_BANDGAP_VREF_TERM));
    CHECK(pmd_write_field(mss, CMN_ATEST_ADDR    , CMN_ATEST_ADDR_BIN_NT_MASK     , CMN_ATEST_ADDR_BIN_NT_OFFSET     , AW_ATEST_CMN_BANDGAP_VREF_ADDR));

    atest_adc_running_sum = 0;
    for (current_iteration = 0; current_iteration < iterations; current_iteration++) {
        CHECK(pmd_write_field(mss, AFE_CMN_ATEST_ADDR      , AFE_CMN_ATEST_CAPTURE_ENA_NT_MASK, AFE_CMN_ATEST_CAPTURE_ENA_NT_OFFSET, 1));
        USR_SLEEP(5);
        CHECK(pmd_read_field (mss, AFE_CMN_ATEST_RDREG_ADDR, AFE_CMN_ATEST_RDREG_ADC_NT_MASK  , AFE_CMN_ATEST_RDREG_ADC_NT_OFFSET  , &atest_adc_read_val));
        CHECK(pmd_write_field(mss, AFE_CMN_ATEST_ADDR      , AFE_CMN_ATEST_CAPTURE_ENA_NT_MASK, AFE_CMN_ATEST_CAPTURE_ENA_NT_OFFSET, 0));

        atest_adc_running_sum += atest_adc_read_val;
    }
    mean_data->bg1 = ((float) atest_adc_running_sum)/iterations;

    /* BG2 */
    CHECK(pmd_write_field(mss, AFE_CMN_ATEST_ADDR, AFE_CMN_ATEST_MEAS_TERM_NT_MASK, AFE_CMN_ATEST_MEAS_TERM_NT_OFFSET, AW_ATEST_CMN_BANDGAP_VREF2_TERM));
    CHECK(pmd_write_field(mss, CMN_ATEST_ADDR    , CMN_ATEST_ADDR_BIN_NT_MASK     , CMN_ATEST_ADDR_BIN_NT_OFFSET     , AW_ATEST_CMN_BANDGAP_VREF2_ADDR));

    atest_adc_running_sum = 0;
    for (current_iteration = 0; current_iteration < iterations; current_iteration++) {
        CHECK(pmd_write_field(mss, AFE_CMN_ATEST_ADDR      , AFE_CMN_ATEST_CAPTURE_ENA_NT_MASK, AFE_CMN_ATEST_CAPTURE_ENA_NT_OFFSET, 1));
        USR_SLEEP(5);
        CHECK(pmd_read_field (mss, AFE_CMN_ATEST_RDREG_ADDR, AFE_CMN_ATEST_RDREG_ADC_NT_MASK  , AFE_CMN_ATEST_RDREG_ADC_NT_OFFSET  , &atest_adc_read_val));
        CHECK(pmd_write_field(mss, AFE_CMN_ATEST_ADDR      , AFE_CMN_ATEST_CAPTURE_ENA_NT_MASK, AFE_CMN_ATEST_CAPTURE_ENA_NT_OFFSET, 0));

        atest_adc_running_sum += atest_adc_read_val;
    }
    mean_data->bg2 = ((float) atest_adc_running_sum)/iterations;

    return AW_ERR_CODE_NONE;
}

int aw_pmd_4ln_atest_adc_temp_get(mss_access_t               *mss, 
                              aw_adc_temp_calibration_t  *calibration,
                              aw_adc_temp_method_t        method, 
                              uint32_t                    iterations,
                              aw_adc_temp_data_t         *measured_data,
                              float                      *result_temp) {

    NULL_CHECK(result_temp);
    NULL_CHECK(calibration);
    NULL_CHECK(measured_data);

    CHECK(aw_pmd_4ln_atest_adc_temp_capture(mss, iterations, measured_data));

    switch (method) {
        case AW_ATEST_ADC_1_POINT_SUPPLY_SENSITIVE_METHOD:
            *result_temp = calibration->temp_1 + AW_ATEST_ADC_VREF2_CONSTANT/AW_ATEST_ADC_MTEMP_CONSTANT * (calibration->data_1.vb - calibration->data_1.vc - measured_data->vb + measured_data->vc)/(measured_data->bg2-measured_data->bg1);
            break;
        case AW_ATEST_ADC_1_POINT_SUPPLY_INSENSITIVE_METHOD:
            *result_temp = calibration->temp_1 + AW_ATEST_ADC_VREF2_CONSTANT/AW_ATEST_ADC_MTEMP_CONSTANT * ( ((measured_data->vb - measured_data->vc)/(measured_data->bg1 - measured_data->bg2)) - ((calibration->data_1.vb - calibration->data_1.vc)/(calibration->data_1.bg1-calibration->data_1.bg2)) );
            break;
        case AW_ATEST_ADC_2_POINT_SUPPLY_SENSITIVE_METHOD:
            *result_temp = calibration->temp_1 + ((calibration->temp_2-calibration->temp_1) * (measured_data->vb - measured_data->vc - calibration->data_1.vb + calibration->data_1.vc)/(calibration->data_2.vb - calibration->data_2.vc-calibration->data_1.vb + calibration->data_1.vc));
            break;
        case AW_ATEST_ADC_2_POINT_SUPPLY_INSENSITIVE_METHOD:
            *result_temp = calibration->temp_1 + ((calibration->temp_1-calibration->temp_2) * (((measured_data->vb-measured_data->vc)/(measured_data->bg1-measured_data->bg2)) - ((calibration->data_1.vb-calibration->data_1.vc)/(calibration->data_1.bg1-calibration->data_1.bg2))) / (((calibration->data_1.vb-calibration->data_1.vc)/(calibration->data_1.bg1-calibration->data_1.bg2)) - ((calibration->data_2.vb-calibration->data_2.vc)/(calibration->data_2.bg1-calibration->data_2.bg2))) );
            break;
        default:
            return AW_ERR_CODE_INVALID_ARG_VALUE;
    }

    return AW_ERR_CODE_NONE;
}

int aw_pmd_4ln_rx_cdr_lock_get(mss_access_t *mss, uint32_t *rx_cdr_lock) {
    CHECK(bf_pmd_4ln_read_field(mss, DIG_SOC_LANE_STAT_REG1_ADDR, DIG_SOC_LANE_STAT_REG1_OCTL_RX_DATA_VLD_MASK, DIG_SOC_LANE_STAT_REG1_OCTL_RX_DATA_VLD_OFFSET, rx_cdr_lock));
    return AW_ERR_CODE_NONE;
}

int aw_pmd_4ln_lcpll_vco_counter_get(mss_access_t *mss, uint32_t timing_window, double *lcpll_ppm, double *vco_freq, double refclk_freq){
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

static char * aw_bist_pattern_decoder (uint32_t bist_pattern_encoded) {
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

int aw_pmd_4ln_rx_cdr_offset_get(mss_access_t *mss, uint32_t *use_custom_cdr_offset, uint32_t *cdr_offset, uint32_t *cdr_dir) {
    uint32_t cdr_offset_dir;

    CHECK(pmd_read_field(mss, CDR_OFFSET_CFG_ADDR, CDR_OFFSET_CFG_MASK, CDR_OFFSET_CFG_OFFSET, &cdr_offset_dir));
    CHECK(pmd_read_field(mss, USE_CUSTOM_CDR_OFFSET_ADDR, USE_CUSTOM_CDR_OFFSET_MASK, USE_CUSTOM_CDR_OFFSET_OFFSET, use_custom_cdr_offset));
    *cdr_offset = (cdr_offset_dir & 0x00ff);
    *cdr_dir = (cdr_offset_dir & 0x0100) >> 8;
    return AW_ERR_CODE_NONE;
}

int aw_pmd_4ln_read_status(mss_access_t *mss, int branch) {
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

int aw_pmd_4ln_read_status2(mss_access_t *mss, int branch) {
  bf_pmd_4ln_dump_csv_header(branch);
  return bf_pmd_4ln_read_status(mss, branch);
}

// needed for ffe, dfe, slicer thresholds and targets
int aw_pmd_4ln_pause_background(mss_access_t *mss, uint32_t pause_enable) {
    CHECK(pmd_write_field(mss, EQBK_HOLD_REQ_ADDR, EQBK_HOLD_REQ_MASK, EQBK_HOLD_REQ_OFFSET, pause_enable));
    int poll_result;
    uint32_t timeout_us = 100;

    poll_result = pmd_poll_field(mss, EQBK_HOLD_ACK_ADDR, EQBK_HOLD_ACK_MASK, EQBK_HOLD_ACK_OFFSET, pause_enable, timeout_us);
    if (poll_result == -1) {
        return AW_ERR_CODE_POLL_TIMEOUT;
    }
    return AW_ERR_CODE_NONE;
}

int aw_pmd_4ln_tracebuffer_config_enable(mss_access_t *mss, uint32_t clk_sel, uint32_t enable){
    // DFX Clock Select
    // 0 - Disabled
    // 1 - Internal RX clock
    // 2 - External RX Clock
    // 3 - RX refdiv
    // 4 - RX synthdiv
    // 5 - Internal TX clock
    // 6 - External TX clock
    // 7 - TX refdiv
    // 8 - TX synthdiv
    CHECK(pmd_write_field(mss, RST_DBE_RX_ADDR, RST_DBE_RX_DFX_BA_MASK, RST_DBE_RX_DFX_BA_OFFSET, enable));
    CHECK(pmd_write_field(mss, RX_CCG_ADDR, RX_CCG_DFX_ENA_A_MASK, RX_CCG_DFX_ENA_A_OFFSET, enable));
    CHECK(pmd_write_field(mss, CTL_DBE_OCLA_ADDR, CTL_DBE_OCLA_CLKSEL_A_MASK, CTL_DBE_OCLA_CLKSEL_A_OFFSET, clk_sel));
    return AW_ERR_CODE_NONE;
}

int aw_pmd_4ln_tracebuffer_config_get(mss_access_t *mss, aw_pmd_tracebuffer_mode_t *tbmode, uint32_t *samples_per_cycle) {
    uint32_t read_value;

    CHECK(pmd_read_field(mss, CTL_DBE_TRACE_BUFFER_ADDR, CTL_DBE_TRACE_BUFFER_SAMPLES_PER_CYCLE_A_MASK, CTL_DBE_TRACE_BUFFER_SAMPLES_PER_CYCLE_A_OFFSET, samples_per_cycle));
    CHECK(pmd_read_field(mss, CTL_DBE_TRACE_BUFFER_ADDR, CTL_DBE_TRACE_BUFFER_MODE_A_MASK, CTL_DBE_TRACE_BUFFER_MODE_A_OFFSET, &read_value));

    *tbmode = (aw_pmd_tracebuffer_mode_t) read_value;

    return AW_ERR_CODE_NONE;
}

int aw_pmd_4ln_tracebuffer_config_set(mss_access_t *mss, aw_pmd_tracebuffer_mode_t tbmode, uint32_t samples_per_cycle) {
    CHECK(pmd_write_field(mss, CTL_DBE_OCLA_TRIGGER_ADDR, CTL_DBE_OCLA_TRIGGER_FIRE_A_MASK, CTL_DBE_OCLA_TRIGGER_FIRE_A_OFFSET, 0));
    CHECK(pmd_write_field(mss, CTL_DBE_TRACE_BUFFER_ADDR, CTL_DBE_TRACE_BUFFER_SAMPLES_PER_CYCLE_A_MASK, CTL_DBE_TRACE_BUFFER_SAMPLES_PER_CYCLE_A_OFFSET, samples_per_cycle));
    CHECK(pmd_write_field(mss, CTL_DBE_TRACE_BUFFER_ADDR, CTL_DBE_TRACE_BUFFER_MODE_A_MASK, CTL_DBE_TRACE_BUFFER_MODE_A_OFFSET, (uint32_t) tbmode));
    return AW_ERR_CODE_NONE;
}

// will read the entire tracebuffer and store values into an array
int aw_pmd_4ln_read_tracebuffer(mss_access_t *mss, uint32_t *tb_data, int tb_size) {
    // TODO: optimize by using aw_pmd_4ln_rx_active_branches_get to read only active
    //       branches, will require updates to all calling funcs

    // trigger the tracebuffer and poll for it to fill
    CHECK(pmd_write_field(mss, CTL_DBE_OCLA_TRIGGER_ADDR, CTL_DBE_OCLA_TRIGGER_FIRE_A_MASK, CTL_DBE_OCLA_TRIGGER_FIRE_A_OFFSET, 0));
    CHECK(pmd_write_field(mss, CTL_DBE_OCLA_TRIGGER_ADDR, CTL_DBE_OCLA_TRIGGER_FIRE_A_MASK, CTL_DBE_OCLA_TRIGGER_FIRE_A_OFFSET, 1));
    int poll_result = pmd_poll_field(mss, STS_DBE_TRACE_BUFFER_ADDR, STS_DBE_TRACE_BUFFER_FULL_MASK, STS_DBE_TRACE_BUFFER_FULL_OFFSET, 1, 100); // TODO: need to sim how long it takes for TB to fill up, worst cast 56 block clocks? leaving at 100us for now
    if (poll_result == -1) {
        USR_PRINTF("ERROR: Timed out while waiting for tracebuffer to fill. Please ensure DBE is initialized.");
        return AW_ERR_CODE_POLL_TIMEOUT;
    }

    for ( int addr = 0; addr < tb_size; addr++ ) {
        CHECK(pmd_write_field(mss, CTL_DBE_TRACE_BUFFER_ADDR, CTL_DBE_TRACE_BUFFER_RDADDR_A_MASK, CTL_DBE_TRACE_BUFFER_RDADDR_A_OFFSET, addr));
        CHECK(pmd_read_field(mss, DAT_DBE_TRACE_BUFFER_ADDR, DAT_DBE_TRACE_BUFFER_RDDATA_MASK, DAT_DBE_TRACE_BUFFER_RDDATA_OFFSET, &tb_data[addr] ));
        USR_PRINTF("Got tracebuffer data, addr: %d, data: %d\n", addr, tb_data[addr]);
    }
    // set fire back to 0
    CHECK(pmd_write_field(mss, CTL_DBE_OCLA_TRIGGER_ADDR, CTL_DBE_OCLA_TRIGGER_FIRE_A_MASK, CTL_DBE_OCLA_TRIGGER_FIRE_A_OFFSET, 0));

    return AW_ERR_CODE_NONE;
}

int aw_pmd_4ln_convert_data_signed(uint32_t data) {
    if (data<256){
        return (int)data;
    } else {
        return -1 * (512-data);
    }
}

int aw_pmd_4ln_read_tracebuffer_adc(mss_access_t *mss, int num_samples, int adc_data[][AW_NUM_BRANCHES]) {
    // return ADC tracebuffer data as a 2D array, where the first index is the
    // "sample" number which is really a "sample block" since each "sample" is
    // a collection of samples for all 64 ADCs. The second index represents the
    // value of that particular ADC

    // floor the number of samples to collect to ensure that we always are
    // allocating values within the array.
    // Note: int division in C always truncates (aka floor)
    int tb_iters = num_samples / AW_TB_ADC_FULL_CAPTURES_PER_TRIGGER;

    aw_pmd_4ln_tracebuffer_config_enable(mss, 1, 1);

    // Get the AGC clk/rst state
    uint32_t agc_ccg;
    uint32_t agc_rst;
    CHECK(pmd_read_field(mss, RX_CCG_ADDR, RX_CCG_AGC_ADAPT_ENA_A_MASK, RX_CCG_AGC_ADAPT_ENA_A_OFFSET, &agc_ccg));
    CHECK(pmd_read_field(mss, RST_DBE_RX_ADDR, RST_DBE_RX_AGC_ADAPT_BA_MASK, RST_DBE_RX_AGC_ADAPT_BA_OFFSET, &agc_rst));
    // Override clk/rst
    CHECK(pmd_write_field(mss, RX_CCG_ADDR, RX_CCG_AGC_ADAPT_ENA_A_MASK, RX_CCG_AGC_ADAPT_ENA_A_OFFSET, 1));
    CHECK(pmd_write_field(mss, RST_DBE_RX_ADDR, RST_DBE_RX_AGC_ADAPT_BA_MASK, RST_DBE_RX_AGC_ADAPT_BA_OFFSET, 1));

    uint32_t tb_data[AW_TB_DEPTH];

    USR_PRINTF("Setting up tracebuffer for ADC Capture. TB_MODE: %d, Blocks per TB Capture: %d\n", AW_TB_RAWADC, AW_TB_ADC_NUM_BLOCKS_PER_CAPTURE);
    aw_pmd_4ln_tracebuffer_config_set(mss, AW_TB_RAWADC, AW_TB_ADC_NUM_BLOCKS_PER_CAPTURE);

    int i = 0;
    for (int sample_iter = 0; sample_iter < tb_iters; sample_iter++) {

        // trigger and fill the tracebuffer with samples
        aw_pmd_4ln_read_tracebuffer(mss, tb_data, AW_TB_DEPTH);

        int adc_0_start = (AW_FFE_NUM_TAPS-1) % AW_TB_ADC_SAMPLES_PER_BLOCK; // example is (18-1) % 3 = 2
        int block_addr_start = (int) ceil( (float)(AW_FFE_NUM_TAPS-1)/AW_TB_ADC_SAMPLES_PER_BLOCK ) - 1; // prevent truncation, cast float then cast back to int
        // setup the loop through of TB data, remember that it is 9 bit signed
        // magnitude (2's compliment), in 32 bit little endian registers per
        // TB block
        USR_PRINTF("TB DEBUG Block Addr Start: %d, ADC0 Start: %d\n", block_addr_start, adc_0_start);
        for ( int capture = 0; capture < AW_TB_ADC_FULL_CAPTURES_PER_TRIGGER; capture++ ) {
            USR_PRINTF("Iteration: %d\n", i);
            int adc_num = 0; // reset adc number conuter back to 0 for each new capture
            for ( int block_addr = block_addr_start; block_addr < AW_TB_ADC_NUM_BLOCKS_PER_CAPTURE; block_addr++ ) {
                // get the 3 potential samples of data, but might not use all 3
                // depending on the case logic below
                uint32_t data1 = 0x1FF & tb_data[block_addr+capture*AW_TB_ADC_NUM_BLOCKS_PER_CAPTURE];
                uint32_t data2 = 0x1FF & (tb_data[block_addr+capture*AW_TB_ADC_NUM_BLOCKS_PER_CAPTURE] >> 9);
                uint32_t data3 = 0x1FF & (tb_data[block_addr+capture*AW_TB_ADC_NUM_BLOCKS_PER_CAPTURE] >> 18);
                USR_PRINTF("Depth: %d, ADC Num Start: %d, Block Addr: %d, Data3: %x, Data2: %x, Data1: %x\n", capture, adc_num, block_addr+capture*AW_TB_ADC_NUM_BLOCKS_PER_CAPTURE, data3, data2, data1);
                // if we are in the first block, make sure that we don't count preamble data
                if (block_addr == block_addr_start) {
                    // check where the first sample is stored in the block. Using an
                    // 18 TAP FFE, the first sample would be in the 6th block (block5)
                    // in the last position (index 2 if we are counting from 0)
                    if (adc_0_start == 0) {
                        adc_data[i][adc_num] = aw_pmd_4ln_convert_data_signed(data1);
                        adc_num++;
                        adc_data[i][adc_num] = aw_pmd_4ln_convert_data_signed(data2);
                        adc_num++;
                        adc_data[i][adc_num] = aw_pmd_4ln_convert_data_signed(data3);
                        adc_num++;
                    }
                    if (adc_0_start == 1) {
                        adc_data[i][adc_num] = aw_pmd_4ln_convert_data_signed(data2);
                        adc_num++;
                        adc_data[i][adc_num] = aw_pmd_4ln_convert_data_signed(data3);
                        adc_num++;
                    }
                    if (adc_0_start == 2) {
                        adc_data[i][adc_num] = aw_pmd_4ln_convert_data_signed(data3);
                        adc_num++;
                    }
                } else {
                    // we are now in blocks that are just all new ADC samples, so
                    // we pull them out 1 by 1 and put them in the data structure
                    adc_data[i][adc_num] = aw_pmd_4ln_convert_data_signed(data1);
                    adc_num++;
                    adc_data[i][adc_num] = aw_pmd_4ln_convert_data_signed(data2);
                    adc_num++;
                    adc_data[i][adc_num] = aw_pmd_4ln_convert_data_signed(data3);
                    adc_num++;
                }
            }
            i++; // increment the sample block index
        }
    }

    // restore AGC clk/rst state
    CHECK(pmd_write_field(mss, RX_CCG_ADDR, RX_CCG_AGC_ADAPT_ENA_A_MASK, RX_CCG_AGC_ADAPT_ENA_A_OFFSET, agc_ccg));
    CHECK(pmd_write_field(mss, RST_DBE_RX_ADDR, RST_DBE_RX_AGC_ADAPT_BA_MASK, RST_DBE_RX_AGC_ADAPT_BA_OFFSET, agc_rst));
    aw_pmd_4ln_tracebuffer_config_enable(mss, 0, 0);
    return AW_ERR_CODE_NONE;
}

int aw_pmd_4ln_tbus_client_config_get(mss_access_t *mss, uint32_t *block_id, uint32_t *signal_id, uint32_t *trigger, uint32_t *continuous_sample){
    // note design only has 1 client, and likely will only have 1 client for
    // the foreseeable future
    CHECK(pmd_read_field(mss, CTL_DBE_TBUS_CLIENT0_ADDR, CTL_DBE_TBUS_CLIENT0_BLOCK_ID_A_MASK, CTL_DBE_TBUS_CLIENT0_BLOCK_ID_A_OFFSET, block_id));
    CHECK(pmd_read_field(mss, CTL_DBE_TBUS_CLIENT0_ADDR, CTL_DBE_TBUS_CLIENT0_SIGNAL_ID_A_MASK, CTL_DBE_TBUS_CLIENT0_SIGNAL_ID_A_OFFSET, signal_id));
    CHECK(pmd_read_field(mss, CTL_DBE_TBUS_CLIENT0_ADDR, CTL_DBE_TBUS_CLIENT0_TRIGGER_A_MASK, CTL_DBE_TBUS_CLIENT0_TRIGGER_A_OFFSET, trigger));
    CHECK(pmd_read_field(mss, CTL_DBE_TBUS_CLIENT0_ADDR, CTL_DBE_TBUS_CLIENT0_CONTINUOUS_SAMPLE_A_MASK, CTL_DBE_TBUS_CLIENT0_CONTINUOUS_SAMPLE_A_OFFSET, continuous_sample));
    return AW_ERR_CODE_NONE;
}

int aw_pmd_4ln_tbus_client_config_set(mss_access_t *mss, uint32_t block_id, uint32_t signal_id, uint32_t trigger, uint32_t continuous_sample) {
    // note design only has 1 client, and likely will only have 1 client for
    // the foreseeable future
    CHECK(pmd_write_field(mss, CTL_DBE_TBUS_CLIENT0_ADDR, CTL_DBE_TBUS_CLIENT0_BLOCK_ID_A_MASK, CTL_DBE_TBUS_CLIENT0_BLOCK_ID_A_OFFSET, block_id));
    CHECK(pmd_write_field(mss, CTL_DBE_TBUS_CLIENT0_ADDR, CTL_DBE_TBUS_CLIENT0_SIGNAL_ID_A_MASK, CTL_DBE_TBUS_CLIENT0_SIGNAL_ID_A_OFFSET, signal_id));
    CHECK(pmd_write_field(mss, CTL_DBE_TBUS_CLIENT0_ADDR, CTL_DBE_TBUS_CLIENT0_TRIGGER_A_MASK, CTL_DBE_TBUS_CLIENT0_TRIGGER_A_OFFSET, trigger));
    CHECK(pmd_write_field(mss, CTL_DBE_TBUS_CLIENT0_ADDR, CTL_DBE_TBUS_CLIENT0_CONTINUOUS_SAMPLE_A_MASK, CTL_DBE_TBUS_CLIENT0_CONTINUOUS_SAMPLE_A_OFFSET, continuous_sample));
    return AW_ERR_CODE_NONE;
}

int aw_pmd_4ln_read_tracebuffer_demapper(mss_access_t *mss, int32_t demapper_data[2][AW_TBUS_NUM_SAMPLES], uint32_t branch_id, uint32_t is_ffe) {
    // in FFE mode, signal id offset is 0, in quantizer error mode, offset is equal to AW_NUM_BRANCHES/2
    uint32_t signal_id_offset = 0;
    if (!is_ffe){
        signal_id_offset = AW_NUM_BRANCHES/2;
    }
    // demapper_data is assumed to be a 2D Array where the first index is the
    // branch and the second index is the sample number, which is inherently
    // tied to the size of the tracebuffer. For most serdes' the size of the
    // tracebuffer is num_blocks * tracebuffer depth (which is 2)
    // At very minimum the array must be allocated for:
    //                uint32_t demapper_data[2][AW_TBUS_NUM_SAMPLES];

    // branch_id goes from 0 -> (AW_NUM_BRANCHES/2 - 1), because two branches are pulled per ID
    // branch_id = AW_NUM_BRANCHES/2 -> (AW_NUM_BRANCHES - 1) is quantizer ffe value (quantizer is the error
    // between the ffe value and the closest target value), this likely isn't
    // needed for debug

    // branches are read in pairs, so ID = 0 means reading branch 0 + 1,
    // ID = 1 means reading branch 2+3, etc
    // uint32_t tbus_client_id = 0; // only have 1 client so default to client0
    aw_pmd_4ln_tracebuffer_config_set(mss, AW_TB_TBUS, 1); // in TBUS mode, samples per cycle always is 1
    uint32_t tbus_block_id = 11; // 11 is demapper
    aw_pmd_4ln_tbus_client_config_set(mss, tbus_block_id, branch_id+signal_id_offset, 1, 1);

    uint32_t tb_data[AW_TBUS_NUM_SAMPLES];
    aw_pmd_4ln_read_tracebuffer(mss, tb_data, AW_TBUS_NUM_SAMPLES);

    for (int i = 0; i < AW_TBUS_NUM_SAMPLES; i++){
        uint32_t first_branch = 0x1FF & tb_data[i];
        uint32_t second_branch = 0x1FF & (tb_data[i] >> 16);
        // convert to signed data
        demapper_data[0][i] = aw_pmd_4ln_convert_data_signed(first_branch);
        demapper_data[1][i] = aw_pmd_4ln_convert_data_signed(second_branch);
    }

    return AW_ERR_CODE_NONE;
}

int aw_pmd_4ln_read_tracebuffer_general(mss_access_t *mss, int32_t tb_data_out[AW_TBUS_NUM_SAMPLES], int tbus_block_id, uint32_t signal_id, int fp_lsb, int fp_msb, int fp_si) {
    // calculate custom bit mask and msb/lsb for data conversion
    int fp_nbits = fp_msb-fp_lsb+1;
    int fp_mask  = (2<<(fp_nbits-1))-1;
    int fp_max = 0;
    if (fp_si){
        fp_max  = 2<<(fp_nbits-2);
    } else {
        fp_max = 2<<(fp_nbits-1);
    }

    // only have 1 client so default to client0
    aw_pmd_4ln_tracebuffer_config_set(mss, AW_TB_TBUS, 1); // in TBUS mode, samples per cycle always is 1
    aw_pmd_4ln_tbus_client_config_set(mss, tbus_block_id, signal_id, 0, 1);

    uint32_t tb_data[AW_TBUS_NUM_SAMPLES];
    int data;
    aw_pmd_4ln_read_tracebuffer(mss, tb_data, AW_TBUS_NUM_SAMPLES);

    for (int i = 0; i < AW_TBUS_NUM_SAMPLES; i++){
        data = (tb_data[i]>>fp_lsb) & fp_mask;
        if(data >= fp_max){
            data = data - (fp_max<<1);
        }
        tb_data_out[i] = data;
    }
    aw_pmd_4ln_tbus_client_config_set(mss, 0, 0, 0, 0); // disable tbus/trigger

    return AW_ERR_CODE_NONE;
}

int aw_pmd_4ln_read_tracebuffer_ffe(mss_access_t *mss, int num_samples, int32_t *ffe_data, int branch_id) {

    if (num_samples < AW_TBUS_NUM_SAMPLES){
        USR_PRINTF("Warning: Number of samples provided is less than FFE buffer size of: %d, valid samples will be discarded.\n", AW_TBUS_NUM_SAMPLES);
    }
    if ((num_samples % AW_TBUS_NUM_SAMPLES)!=0){
        USR_PRINTF("Warning: Number of samples provided is not a multiple of: %d, valid samples will be discarded.\n", AW_TBUS_NUM_SAMPLES);
    }
    if (branch_id > 31){
        USR_PRINTF("Error: Invalid branch_id: %d. Branch ID may be between 0-31 for FFE.\n", branch_id);
        return AW_ERR_CODE_INVALID_ARG_VALUE;
    }
    USR_PRINTF("Collecting FFE sample data for branches %d and %d\n", branch_id, branch_id+1);

    int num_tb_iters = (int)ceil((float)num_samples / (float)AW_TBUS_NUM_SAMPLES);
    aw_pmd_4ln_tracebuffer_config_enable(mss, 1, 1);
    int32_t tb_data[2][AW_TBUS_NUM_SAMPLES];

    int samples_collected=0;
    int sample_num = 0; // used to keep track of the samples for a branch pair
    for (int j=0;j<num_tb_iters;j++){
        // data from the tracebuffer for the FFE is returned in a 2D array where
        // [ffe_branch][sample_number], and sample number is a number from
        // 0-(AW_TBUS_NUM_SAMPLES-1)
        aw_pmd_4ln_read_tracebuffer_demapper(mss, tb_data, branch_id, 1);
        for (int i=0;i<AW_TBUS_NUM_SAMPLES;i++){
            if (sample_num == num_samples){
                // remember sample_num starts from 0, so when sample_num is
                // equal to num_samples, we have filled up everything we
                // need and we are done
                samples_collected = 1;
                break;
            }
            USR_PRINTF("Branch_ID: %d, TB_Iter: %d, Sample_Num: %d, Total_Sample_Num: %d, num_tb_iters: %d\n", branch_id, j, i, sample_num, num_tb_iters);
            USR_PRINTF("ffe_data[%d][%d] = %d\n", 0, sample_num, tb_data[0][i]);
            USR_PRINTF("ffe_data[%d][%d] = %d\n", 1, sample_num, tb_data[1][i]);
            ffe_data[0 * num_samples + sample_num] = tb_data[0][i];
            ffe_data[1 * num_samples + sample_num] = tb_data[1][i];
            sample_num++;
        }
        if (samples_collected){
            break;
        }
    }
    USR_PRINTF("FFE Sample collection complete.\n");
    aw_pmd_4ln_tracebuffer_config_enable(mss, 0, 0);

    return AW_ERR_CODE_NONE;
}

int aw_pmd_4ln_read_tracebuffer_itr_dlpf_int(mss_access_t *mss, int num_samples, int32_t *itr_dlpf_int) {

    if (num_samples < AW_TBUS_NUM_SAMPLES){
        USR_PRINTF("Warning: Number of samples provided is less than buffer size of: %d, valid samples will be discarded.\n", AW_TBUS_NUM_SAMPLES);
    }
    if ((num_samples % AW_TBUS_NUM_SAMPLES)!=0){
        USR_PRINTF("Warning: Number of samples provided is not a multiple of: %d, valid samples will be discarded.\n", AW_TBUS_NUM_SAMPLES);
    }
    USR_PRINTF("Collecting ITR DLPF sample data\n");

    int num_tb_iters = (int)ceil((float)num_samples / (float)AW_TBUS_NUM_SAMPLES);
    aw_pmd_4ln_tracebuffer_config_enable(mss, 1, 1);
    int32_t tb_data[AW_TBUS_NUM_SAMPLES];

    int samples_collected=0;
    int sample_num = 0; // used to keep track of the samples for a branch pair
    for (int j=0;j<num_tb_iters;j++){
        // data from the tracebuffer for the ITR DLPF Int. code is returned
        // as a 26 bit signed number where 1 value takes up 1 buffer slot
        aw_pmd_4ln_read_tracebuffer_general(mss, tb_data, 0, 1, 0, 25, 1);
        for (int i=0;i<AW_TBUS_NUM_SAMPLES;i++){
            if (sample_num == num_samples){
                // remember sample_num starts from 0, so when sample_num is
                // equal to num_samples, we have filled up everything we
                // need and we are done
                samples_collected = 1;
                break;
            }
            USR_PRINTF("itr_dlpf_int[%d] = %d\n", sample_num, tb_data[i]);
            itr_dlpf_int[sample_num] = tb_data[i];
            sample_num++;
        }
        if (samples_collected){
            break;
        }
    }
    USR_PRINTF("ITR DLPF Sample collection complete.\n");
    aw_pmd_4ln_tracebuffer_config_enable(mss, 0, 0);

    return AW_ERR_CODE_NONE;
}
int aw_pmd_4ln_read_tracebuffer_quantizer_err(mss_access_t *mss, int num_samples, int32_t *qztr_err_data, int branch_id) {

    if (num_samples < AW_TBUS_NUM_SAMPLES){
        USR_PRINTF("Warning: Number of samples provided is less than QUANTIZER ERR buffer size of: %d, valid samples will be discarded.\n", AW_TBUS_NUM_SAMPLES);
    }
    if ((num_samples % AW_TBUS_NUM_SAMPLES)!=0){
        USR_PRINTF("Warning: Number of samples provided is not a multiple of: %d, valid samples will be discarded.\n", AW_TBUS_NUM_SAMPLES);
    }
    if (branch_id > 31){
        USR_PRINTF("Error: Invalid branch_id: %d. Branch ID may be between 0-31 for QUANTIZER ERR.\n", branch_id);
        return AW_ERR_CODE_INVALID_ARG_VALUE;
    }
    USR_PRINTF("Collecting QUANTIZER ERR sample data for branches %d and %d\n", branch_id, branch_id+1);

    int num_tb_iters = (int)ceil((float)num_samples / (float)AW_TBUS_NUM_SAMPLES);
    aw_pmd_4ln_tracebuffer_config_enable(mss, 1, 1);
    int32_t tb_data[2][AW_TBUS_NUM_SAMPLES];

    int samples_collected=0;
    int sample_num = 0; // used to keep track of the samples for a branch pair
    for (int j=0;j<num_tb_iters;j++){
        // data from the tracebuffer for the quantizer error is returned in a 2D array where
        // [branch][sample_number], and sample number is a number from
        // 0-(AW_TBUS_NUM_SAMPLES-1)
        aw_pmd_4ln_read_tracebuffer_demapper(mss, tb_data, branch_id, 0);
        for (int i=0;i<AW_TBUS_NUM_SAMPLES;i++){
            if (sample_num == num_samples){
                // remember sample_num starts from 0, so when sample_num is
                // equal to num_samples, we have filled up everything we
                // need and we are done
                samples_collected = 1;
                break;
            }
            USR_PRINTF("Branch_ID: %d, TB_Iter: %d, Sample_Num: %d, Total_Sample_Num: %d, num_tb_iters: %d\n", branch_id, j, i, sample_num, num_tb_iters);
            USR_PRINTF("qztr_err_data[%d][%d] = %d\n", 0, sample_num, tb_data[0][i]);
            USR_PRINTF("qztr_err_data[%d][%d] = %d\n", 1, sample_num, tb_data[1][i]);
            qztr_err_data[0 * num_samples + sample_num] = tb_data[0][i];
            qztr_err_data[1 * num_samples + sample_num] = tb_data[1][i];
            sample_num++;
        }
        if (samples_collected){
            break;
        }
    }
    USR_PRINTF("QUANTIZER ERR Sample collection complete.\n");
    aw_pmd_4ln_tracebuffer_config_enable(mss, 0, 0);

    return AW_ERR_CODE_NONE;
}

int aw_pmd_4ln_cmn_ssc_config(mss_access_t *mss, double lsref_mhz, double lcpll_mhz, int ppm_downspread){
    const double ssc_mhz = 0.03; // 30 KHz modulation
    const double fbdiv_int = floor(lcpll_mhz / (lsref_mhz * 4));

    double F_ssc_max_mhz = lcpll_mhz;
    double F_ssc_min_mhz = F_ssc_max_mhz*(1-ppm_downspread/1e6);
    double frac_code_max = pow(2, 31) - 1;
    double frac_code_min = frac_code_max + ( ( ( F_ssc_min_mhz / lsref_mhz / 4 ) - fbdiv_int ) / fbdiv_int ) * pow(2, 31);
    double half_cycle_count = lsref_mhz / ssc_mhz / 2;
    double ssc_step_size = floor( (frac_code_max - frac_code_min) / half_cycle_count );
    double ssc_max_value = frac_code_max;
    double ssc_min_value = ssc_max_value - ( ssc_step_size * half_cycle_count );

    // Enable FracN and set value to max (we assume that we are operating in integer mode)
    CHECK(pmd_write_field(mss, AFE_CMN_LCPLL_FRACDIV_REG1_ADDR, AFE_CMN_LCPLL_FRACDIV_REG1_FRACN_VALUE_NT_MASK, AFE_CMN_LCPLL_FRACDIV_REG1_FRACN_VALUE_NT_OFFSET, (uint32_t)ssc_max_value));
    CHECK(pmd_write_field(mss, AFE_CMN_LCPLL_FRACDIV_REG2_ADDR, AFE_CMN_LCPLL_FRACDIV_REG2_FRACN_ENABLE_A_MASK, AFE_CMN_LCPLL_FRACDIV_REG2_FRACN_ENABLE_A_OFFSET, 1));

    CHECK(pmd_write_field(mss, AFE_CMN_LCPLL_FRACDIV_REG4_ADDR, AFE_CMN_LCPLL_FRACDIV_REG4_SSC_MAX_VALUE_NT_MASK, AFE_CMN_LCPLL_FRACDIV_REG4_SSC_MAX_VALUE_NT_OFFSET, (uint32_t)ssc_max_value));
    CHECK(pmd_write_field(mss, AFE_CMN_LCPLL_FRACDIV_REG5_ADDR, AFE_CMN_LCPLL_FRACDIV_REG5_SSC_MIN_VALUE_NT_MASK, AFE_CMN_LCPLL_FRACDIV_REG5_SSC_MIN_VALUE_NT_OFFSET, (uint32_t)ssc_min_value));
    CHECK(pmd_write_field(mss, AFE_CMN_LCPLL_FRACDIV_REG6_ADDR, AFE_CMN_LCPLL_FRACDIV_REG6_SSC_STEP_VALUE_NT_MASK, AFE_CMN_LCPLL_FRACDIV_REG6_SSC_STEP_VALUE_NT_OFFSET, (uint32_t)ssc_step_size));

    return AW_ERR_CODE_NONE;
}

int aw_pmd_4ln_cmn_ssc_en_set(mss_access_t *mss, uint32_t en){

    CHECK(pmd_write_field(mss, AFE_CMN_CLKGEN_REG2_ADDR, AFE_CMN_CLKGEN_REG2_CMN_SRIS_ENA_NT_MASK, AFE_CMN_CLKGEN_REG2_CMN_SRIS_ENA_NT_OFFSET, en));

    return AW_ERR_CODE_NONE;
}

int aw_pmd_4ln_sram_clk_div_set(mss_access_t *mss, uint32_t en){

    CHECK(pmd_write_field(mss, FAST_SRAM_CLK_ADDR, FAST_SRAM_CLK_MASK, FAST_SRAM_CLK_OFFSET, en));

    return AW_ERR_CODE_NONE;

}

int aw_pmd_4ln_sram_clk_div_get(mss_access_t *mss, uint32_t *en) {

    CHECK(pmd_read_field(mss, FAST_SRAM_CLK_ADDR, FAST_SRAM_CLK_MASK, FAST_SRAM_CLK_OFFSET, en));

    return AW_ERR_CODE_NONE;
}

int aw_pmd_4ln_rx_burst_mode_config_set(mss_access_t *mss, uint32_t pam_mode, uint32_t burst_threshold, uint32_t burst_mode){

    CHECK(pmd_write_field(mss, RX_DATABIST_TOP_REG1_ADDR, RX_DATABIST_TOP_REG1_BURST_ERR_SYMBOL_MODE_NT_MASK, RX_DATABIST_TOP_REG1_BURST_ERR_SYMBOL_MODE_NT_OFFSET, pam_mode));
    CHECK(pmd_write_field(mss, RX_DATABIST_TOP_REG1_ADDR, RX_DATABIST_TOP_REG1_BURST_MODE_NT_MASK, RX_DATABIST_TOP_REG1_BURST_MODE_NT_OFFSET, burst_mode));
    CHECK(pmd_write_field(mss, RX_DATABIST_TOP_REG14_ADDR, RX_DATABIST_TOP_REG14_BURST_ERROR_BITS_THRESHOLD_NT_MASK, RX_DATABIST_TOP_REG14_BURST_ERROR_BITS_THRESHOLD_NT_OFFSET, burst_threshold));


    return AW_ERR_CODE_NONE;
}

int aw_pmd_4ln_rx_burst_mode_config_get(mss_access_t *mss, uint32_t *pam_mode, uint32_t *burst_threshold, uint32_t *burst_mode){

    CHECK(pmd_read_field(mss, RX_DATABIST_TOP_REG1_ADDR, RX_DATABIST_TOP_REG1_BURST_ERR_SYMBOL_MODE_NT_MASK, RX_DATABIST_TOP_REG1_BURST_ERR_SYMBOL_MODE_NT_OFFSET, pam_mode));
    CHECK(pmd_read_field(mss, RX_DATABIST_TOP_REG1_ADDR, RX_DATABIST_TOP_REG1_BURST_MODE_NT_MASK, RX_DATABIST_TOP_REG1_BURST_MODE_NT_OFFSET, burst_mode));
    CHECK(pmd_read_field(mss, RX_DATABIST_TOP_REG14_ADDR, RX_DATABIST_TOP_REG14_BURST_ERROR_BITS_THRESHOLD_NT_MASK, RX_DATABIST_TOP_REG14_BURST_ERROR_BITS_THRESHOLD_NT_OFFSET, burst_threshold));


    return AW_ERR_CODE_NONE;
}

int aw_pmd_4ln_rx_burst_err_cnt_get(mss_access_t *mss , uint32_t *burst_err_cnt){

    CHECK(pmd_read_field(mss, RX_DATABIST_TOP_RDREG12_ADDR, RX_DATABIST_TOP_RDREG12_BURST_ERR_FOUND_CNT_NT_MASK, RX_DATABIST_TOP_RDREG12_BURST_ERR_FOUND_CNT_NT_OFFSET, burst_err_cnt));

    return AW_ERR_CODE_NONE;

}

int aw_pmd_4ln_rx_burst_mode_stats_get(mss_access_t *mss, uint32_t exp_data[], uint32_t rec_data[], uint32_t xor_data[]){

    CHECK(pmd_read_field(mss, RX_DATABIST_TOP_RDREG4_ADDR, RX_DATABIST_TOP_RDREG4_EXPECT_DATA0_NT_MASK, RX_DATABIST_TOP_RDREG4_EXPECT_DATA0_NT_OFFSET, &exp_data[0]));
    CHECK(pmd_read_field(mss, RX_DATABIST_TOP_RDREG5_ADDR, RX_DATABIST_TOP_RDREG5_EXPECT_DATA1_NT_MASK, RX_DATABIST_TOP_RDREG5_EXPECT_DATA1_NT_OFFSET, &exp_data[1]));
    CHECK(pmd_read_field(mss, RX_DATABIST_TOP_RDREG6_ADDR, RX_DATABIST_TOP_RDREG6_EXPECT_DATA2_NT_MASK, RX_DATABIST_TOP_RDREG6_EXPECT_DATA2_NT_OFFSET, &exp_data[2]));
    CHECK(pmd_read_field(mss, RX_DATABIST_TOP_RDREG7_ADDR, RX_DATABIST_TOP_RDREG7_EXPECT_DATA3_NT_MASK, RX_DATABIST_TOP_RDREG7_EXPECT_DATA3_NT_OFFSET, &exp_data[3]));

    CHECK(pmd_read_field(mss, RX_DATABIST_TOP_RDREG8_ADDR, RX_DATABIST_TOP_RDREG8_REC_DATA0_NT_MASK, RX_DATABIST_TOP_RDREG8_REC_DATA0_NT_OFFSET, &rec_data[0]));
    CHECK(pmd_read_field(mss, RX_DATABIST_TOP_RDREG9_ADDR, RX_DATABIST_TOP_RDREG9_REC_DATA1_NT_MASK, RX_DATABIST_TOP_RDREG9_REC_DATA1_NT_OFFSET, &rec_data[1]));
    CHECK(pmd_read_field(mss, RX_DATABIST_TOP_RDREG10_ADDR, RX_DATABIST_TOP_RDREG10_REC_DATA2_NT_MASK, RX_DATABIST_TOP_RDREG10_REC_DATA2_NT_OFFSET, &rec_data[2]));
    CHECK(pmd_read_field(mss, RX_DATABIST_TOP_RDREG11_ADDR, RX_DATABIST_TOP_RDREG11_REC_DATA3_NT_MASK, RX_DATABIST_TOP_RDREG11_REC_DATA3_NT_OFFSET, &rec_data[3]));

    xor_data[0] =  (exp_data[0])^(rec_data[0]);
    xor_data[1] =  (exp_data[1])^(rec_data[1]);
    xor_data[2] =  (exp_data[2])^(rec_data[2]);
    xor_data[3] =  (exp_data[3])^(rec_data[3]);

    return AW_ERR_CODE_NONE;
}

int aw_pmd_4ln_rx_gray_code_mapping_get(mss_access_t *mss, uint8_t *gray_code_map) {
    uint32_t el3 = 0;
    uint32_t el1 = 0;
    uint32_t eh3 = 0;
    uint32_t eh1 = 0;

    CHECK(pmd_read_field(mss, RX_DEMAPPER_ADDR, RX_DEMAPPER_EL3_VAL_NT_MASK, RX_DEMAPPER_EL3_VAL_NT_OFFSET, &el3));
    CHECK(pmd_read_field(mss, RX_DEMAPPER_ADDR, RX_DEMAPPER_EL1_VAL_NT_MASK, RX_DEMAPPER_EL1_VAL_NT_OFFSET, &el1));
    CHECK(pmd_read_field(mss, RX_DEMAPPER_ADDR, RX_DEMAPPER_EH3_VAL_NT_MASK, RX_DEMAPPER_EH3_VAL_NT_OFFSET, &eh3));
    CHECK(pmd_read_field(mss, RX_DEMAPPER_ADDR, RX_DEMAPPER_EH1_VAL_NT_MASK, RX_DEMAPPER_EH1_VAL_NT_OFFSET, &eh1));

    *gray_code_map = ((eh3 & 0x3) << 6) | ((eh1 & 0x3) << 4) | ((el1 & 0x3) << 2) | (el3 & 0x3);

    return AW_ERR_CODE_NONE;
}

int aw_pmd_4ln_rx_gray_code_mapping_set(mss_access_t *mss, uint8_t gray_code_map) {
    uint8_t el3 = gray_code_map & 0x3;
    uint8_t el1 = (gray_code_map >> 2) & 0x3;
    uint8_t eh1 = (gray_code_map >> 4) & 0x3;
    uint8_t eh3 = (gray_code_map >> 6) & 0x3;

    CHECK(pmd_write_field(mss, RX_DEMAPPER_ADDR, RX_DEMAPPER_EL3_VAL_NT_MASK, RX_DEMAPPER_EL3_VAL_NT_OFFSET, el3));
    CHECK(pmd_write_field(mss, RX_DEMAPPER_ADDR, RX_DEMAPPER_EL1_VAL_NT_MASK, RX_DEMAPPER_EL1_VAL_NT_OFFSET, el1));
    CHECK(pmd_write_field(mss, RX_DEMAPPER_ADDR, RX_DEMAPPER_EH1_VAL_NT_MASK, RX_DEMAPPER_EH1_VAL_NT_OFFSET, eh1));
    CHECK(pmd_write_field(mss, RX_DEMAPPER_ADDR, RX_DEMAPPER_EH3_VAL_NT_MASK, RX_DEMAPPER_EH3_VAL_NT_OFFSET, eh3));

    return AW_ERR_CODE_NONE;
}

int aw_pmd_4ln_tx_gray_code_mapping_get(mss_access_t *mss, uint8_t *gray_code_map){

    uint32_t mapping = 0;
    CHECK(pmd_read_field(mss, TX_DATAPATH_REG1_ADDR, TX_DATAPATH_REG1_GRAY_CODE_MAPPING_NT_MASK, TX_DATAPATH_REG1_GRAY_CODE_MAPPING_NT_OFFSET, &mapping));
    *gray_code_map = mapping;
    return AW_ERR_CODE_NONE;
}

int aw_pmd_4ln_tx_gray_code_mapping_set(mss_access_t *mss, uint8_t gray_code_map){

    CHECK(pmd_write_field(mss, TX_DATAPATH_REG1_ADDR, TX_DATAPATH_REG1_GRAY_CODE_MAPPING_NT_MASK, TX_DATAPATH_REG1_GRAY_CODE_MAPPING_NT_OFFSET, gray_code_map));

    return AW_ERR_CODE_NONE;
}

int aw_pmd_4ln_tx_perf_settings(mss_access_t *mss, uint8_t perf_mode){

    if (perf_mode != 2){
        CHECK(pmd_write_field(mss, TX_VREG_ADDR, TX_VREG_BITCK_NT_MASK, TX_VREG_BITCK_NT_OFFSET, 6));
    }
    return AW_ERR_CODE_NONE;
}

int aw_pmd_4ln_cmn_bias_bandgap_force_startup(mss_access_t *mss){

    CHECK(pmd_write_field(mss, CMN_BIAS_ADDR, CMN_BIAS_STARTUP_FORCE_NT_MASK, CMN_BIAS_STARTUP_FORCE_NT_OFFSET, 1));
    CHECK(pmd_write_field(mss, CMN_BIAS_ADDR, CMN_BIAS_STARTUP_FORCE_NT_MASK, CMN_BIAS_STARTUP_FORCE_NT_OFFSET, 0));
    return AW_ERR_CODE_NONE;

}


int aw_pmd_4ln_rx_active_branches_get(mss_access_t *mss, uint32_t * active_branches) {
    CHECK(pmd_read_field(mss, SEQ_CNTRL_ADAPT_ADDR, SEQ_CNTRL_RX_ACTIVE_BRANCH_NT_MASK, SEQ_CNTRL_RX_ACTIVE_BRANCH_NT_OFFSET, active_branches));
    switch (*active_branches) {
        case 0:
            *active_branches = 4;
            break;
        case 1:
            *active_branches = 8;
            break;
        case 2:
            *active_branches = 16;
            break;
        case 3:
            *active_branches = 32;
            break;
        case 4:
            *active_branches = 64;
            break;
        default:
            return AW_ERR_CODE_FUNC_FAILURE;
    }
    return AW_ERR_CODE_NONE;
}

/**
 * This function will fill the passed structure with current versioning information
 * and then print it
 */
int aw_pmd_4ln_fw_version_get(mss_access_t *mss, aw_version_t *version_st) {
    uint32_t version_raw;
    //read_csr(0x80000000, &version_raw);
    aw_pmd_4ln_rd_csr(mss, 0x80000000, &version_raw);

    version_st->version_major = (version_raw >> 10) & 0x3;
    version_st->version_minor = (version_raw >> 6) & 0xF;
    version_st->version_patch = version_raw & 0x3F;
    //USR_PRINTF("INFO: Version MAJOR = %d Version MINOR = %d Version PATCH = %d\n", version_st->version_major, version_st->version_minor, version_st->version_patch);
    return AW_ERR_CODE_NONE;
}

int aw_pmd_4ln_rx_dfe_ratio_set(mss_access_t *mss, uint32_t enable, uint32_t use_auto_lookup_dfe_ratio, double custom_dfe_ratio) {
    CHECK(pmd_write_field(mss, ENABLE_DFE_RATIO_ADDR, ENABLE_DFE_RATIO_MASK, ENABLE_DFE_RATIO_OFFSET, enable));
    CHECK(pmd_write_field(mss, USE_AUTO_LOOKUP_DFE_RATIO_ADDR, USE_AUTO_LOOKUP_DFE_RATIO_MASK, USE_AUTO_LOOKUP_DFE_RATIO_OFFSET, use_auto_lookup_dfe_ratio));

    // convert to 2.14 format
    int32_t val = custom_dfe_ratio * (1 << 14); // custom_dfe_ratio * 2^14
    int32_t val2 = val & 0xFFFF;
    CHECK(pmd_write_field(mss, CUSTOM_DFE_RATIO_ADDR, CUSTOM_DFE_RATIO_MASK, CUSTOM_DFE_RATIO_OFFSET, val2));
    return AW_ERR_CODE_NONE;
}

int aw_pmd_4ln_rx_dfe_ratio_get(mss_access_t *mss, uint32_t* enable, uint32_t* use_auto_lookup_dfe_ratio, double* custom_dfe_ratio) {

    CHECK(pmd_read_field(mss, ENABLE_DFE_RATIO_ADDR, ENABLE_DFE_RATIO_MASK, ENABLE_DFE_RATIO_OFFSET, enable));
    CHECK(pmd_read_field(mss, USE_AUTO_LOOKUP_DFE_RATIO_ADDR, USE_AUTO_LOOKUP_DFE_RATIO_MASK, USE_AUTO_LOOKUP_DFE_RATIO_OFFSET, use_auto_lookup_dfe_ratio));

    uint32_t dfe_ratio_fw = 0;
    CHECK(pmd_read_field(mss, CUSTOM_DFE_RATIO_ADDR, CUSTOM_DFE_RATIO_MASK, CUSTOM_DFE_RATIO_OFFSET, &dfe_ratio_fw));

    double dfe_ratio_decimal = 0;
    if (dfe_ratio_fw > 0x7FFF) {
        uint32_t twos_complement = (1<<16) - dfe_ratio_fw;
        dfe_ratio_decimal = -(twos_complement / (double) (1<<14));
    }
    else 
        dfe_ratio_decimal = dfe_ratio_fw / (double) (1<<14);

    *custom_dfe_ratio = dfe_ratio_decimal;

    return AW_ERR_CODE_NONE;
}

int aw_pmd_4ln_sris_set(mss_access_t *mss, uint32_t en) {
    CHECK(pmd_write_field(mss, CMN_SRIS_ENABLE_ADDR, CMN_SRIS_ENABLE_MASK, CMN_SRIS_ENABLE_OFFSET, en));
    CHECK(pmd_write_field(mss, RX_SRIS_ENABLE_ADDR, RX_SRIS_ENABLE_MASK, RX_SRIS_ENABLE_OFFSET, en));

    return AW_ERR_CODE_NONE;
}

int aw_pmd_4ln_sris_get(mss_access_t *mss, uint32_t* en) {
    uint32_t is_cmn_sris_enabled = 0;
    uint32_t is_rx_sris_enabled = 0;

    // Since SRIS relates to refclk, and is global, we expect that is_cmn_sris_enabled == is_rx_sris_enabled
    CHECK(pmd_read_field(mss, CMN_SRIS_ENABLE_ADDR, CMN_SRIS_ENABLE_MASK, CMN_SRIS_ENABLE_OFFSET, &is_cmn_sris_enabled));
    CHECK(pmd_read_field(mss, RX_SRIS_ENABLE_ADDR, RX_SRIS_ENABLE_MASK, RX_SRIS_ENABLE_OFFSET, &is_rx_sris_enabled));

    *en = is_cmn_sris_enabled & is_rx_sris_enabled;

    return AW_ERR_CODE_NONE;
}

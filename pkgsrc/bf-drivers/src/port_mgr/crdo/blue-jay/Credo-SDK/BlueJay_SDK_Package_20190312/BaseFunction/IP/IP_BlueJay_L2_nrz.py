import time, re
from BaseFunction.common import *
from credo import *
import math
permMode.setFilename(__file__)
permMode.setHivename('PAM4')

def rx_checker_nrz(lane_obj, status = None):
    if status==None:
        return lane_obj.RX_PRBS_CHECK_EN
    else:
        lane_obj.RX_PRBS_CHECK_EN = status

def rx_prbs_mode_nrz(lane_obj, pat=None):
    nrz_prbs_pat  = ['PRBS9-NRZ', 'PRBS15-NRZ', 'PRBS23-NRZ', 'PRBS31-NRZ']
    if pat == None:
        checker = rx_checker_nrz(lane_obj)
        pat = lane_obj.RX_PRBS_MODE
        pat_sel = nrz_prbs_pat[pat]
        return checker, pat_sel
    elif type(pat) == int:
        rx_checker_nrz(lane_obj, 1)
        lane_obj.RX_PRBS_MODE = pat
    elif type(pat) == str:
        val = nrz_prbs_pat.index(pat)
        rx_checker_nrz(lane_obj, 1)
        lane_obj.RX_PRBS_MODE = val

def prbs_rst_nrz(lane_obj):
    wait = 0.001
    lane_obj.RX_PRBS_COUNT_RESET = 0    
    time.sleep(wait)
    lane_obj.RX_PRBS_COUNT_RESET = 1   
    time.sleep(wait)
    lane_obj.RX_PRBS_COUNT_RESET = 0    

def ready_nrz(lane_obj):
    sd = lane_obj.RX_SIG_DET   
    rdy = lane_obj.RX_NRZ_PHY_READY   
    return sd, rdy

def lane_reset(lane_obj):
    lane_obj.NRZ_SM_RESET = 1
    time.sleep(0.001)
    lane_obj.NRZ_SM_RESET = 0

def lr_nrz(lane_obj):
    lane_obj.TARGET_CTRL = 0x100     
    lane_obj.NRZ_SM_RESET = 0x1 
    time.sleep(.050)        
    lane_obj.NRZ_SM_RESET = 0x0 
    lane_obj.TARGET_CTRL = 0x002   
    
def sm_cont_nrz(lane_obj):
    lane_obj.NRZ_SM_CONT = 1
    time.sleep(0.001)
    lane_obj.NRZ_SM_CONT = 0

def tx_pol_flip_nrz(lane_obj):
    val = lane_obj.TX_ANA_OUT_FLIP
    lane_obj.TX_ANA_OUT_FLIP = 1-val

def tx_pol_nrz(lane_obj, val = None):
    if val == None:
        return lane_obj.TX_ANA_OUT_FLIP
    else:
        lane_obj.TX_ANA_OUT_FLIP = val

def rx_pol_nrz(lane_obj, val = None):
    if val == None:
        return lane_obj.RX_POL_FLIP
    else:
        lane_obj.RX_POL_FLIP = val

def rx_pol_flip_nrz(lane_obj):
    val = lane_obj.RX_POL_FLIP
    lane_obj.RX_POL_FLIP = 1-val

def bp1(lane_obj, en=None, st=0):
    if en == None:
        en_v = lane_obj.NRZ_BP1_EN
        st_v = lane_obj.NRZ_BP1_STATE
        reached = lane_obj.NRZ_BP1_REACHED
        return en_v, st_v, reached
    else:
        lane_obj.NRZ_BP1_STATE = st
        lane_obj.NRZ_BP1_EN = en

def bp2(lane_obj, en=None, st=0):
    if en == None:
        en_v = lane_obj.NRZ_BP2_EN
        st_v = lane_obj.NRZ_BP2_STATE
        reached = lane_obj.NRZ_BP2_REACHED
        return en_v, st_v, reached
    else:
        lane_obj.NRZ_BP2_STATE = st
        lane_obj.NRZ_BP2_EN = en

def ctle_map_nrz(lane_obj, sel = None, val1 = None, val2 = None):
    if None in (sel, val1, val2):
        agc_0 = lane_obj.Reg0100_0076_15_10
        agc_1 = lane_obj.Reg0100_0076_9_4
        agc_2 = lane_obj.Reg0100_0076_3_0S
        agc_3 = lane_obj.Reg0100_0077_13_8
        agc_4 = lane_obj.Reg0100_0077_7_2
        agc_5 = lane_obj.Reg0100_0077_1_0S
        agc_6 = lane_obj.Reg0100_0078_11_6
        agc_7 = lane_obj.Reg0100_0078_5_0
        agc = { 0: [agc_0 >> 3, agc_0 & 0x7],
                1: [agc_1 >> 3, agc_1 & 0x7],
                2: [agc_2 >> 3, agc_2 & 0x7],
                3: [agc_3 >> 3, agc_3 & 0x7],
                4: [agc_4 >> 3, agc_4 & 0x7],
                5: [agc_5 >> 3, agc_5 & 0x7],
                6: [agc_6 >> 3, agc_6 & 0x7],
                7: [agc_7 >> 3, agc_7 & 0x7],
                }
        if sel == None:
            for key in agc.keys():
                print key, agc[key]
        else:
            return agc[sel]
    else:
        val = (val1 << 3) + val2
        if sel == 0:
            lane_obj.Reg0100_0076_15_10 = val
        elif sel == 1:
            lane_obj.Reg0100_0076_9_4 = val
        elif sel == 2:
            lane_obj.Reg0100_0076_3_0S = val
        elif sel == 3:
            lane_obj.Reg0100_0077_13_8 = val
        elif sel == 4:
            lane_obj.Reg0100_0077_7_2 = val
        elif sel == 5:
            lane_obj.Reg0100_0077_1_0S = val
        elif sel == 6:
            lane_obj.Reg0100_0078_11_6 = val
        else:
            lane_obj.Reg0100_0078_5_0 = val

def state_nrz(lane_obj):
    return lane_obj.NRZ_CURRENT_STATE

def dfe_nrz(lane_obj):
    #f1 = twos_to_int(lane_obj.RX_DFE_TAP_1_CURR_VAL, 7)
    f1 = lane_obj.RX_DFE_TAP_1_CURR_VAL
    #f2 = twos_to_int(lane_obj.RX_DFE_TAP_2_CURR_VAL, 7)
    f2 = lane_obj.RX_DFE_TAP_2_CURR_VAL
    #f3 = twos_to_int(lane_obj.RX_DFE_TAP_3_CURR_VAL, 7)
    f3 = lane_obj.RX_DFE_TAP_3_CURR_VAL
    return f1, f2, f3

def get_err_nrz(lane_obj):
    checker = lane_obj.RX_PRBS_CHECK_EN 
    rdy = sum(ready_nrz(lane_obj))
    if checker == 0:
        print('***Lane {} PRBS checker is off***'.format(regFuncInit.getNameList()[lane_obj._laneno]))
        return -1
    else:
        return long(lane_obj.RX_NRZ_PRBS_READ_ERR_HIGH_RX_NRZ_PRBS_READ_ERR_LOW)
    
def prbs_rst(lane_obj):
    lane_obj.RX_PRBS_COUNT_RESET = 0
    time.sleep(0.001)
    lane_obj.RX_PRBS_COUNT_RESET = 1
    time.sleep(0.001)
    lane_obj.RX_PRBS_COUNT_RESET = 0

def ber_nrz(lane_obj, rst = 1, t = 5):  
    if(rst == 1): 
        prbs_rst_nrz(lane_obj)
    err_cnt_pre = get_err_nrz(lane_obj)
    time.sleep(t)
    err_cnt_post = get_err_nrz(lane_obj)
    DataRate = get_lane_pll(lane_obj)[0][0] 
    print DataRate
    if err_cnt_pre == -1:
        return -1
    else:
        err = err_cnt_post - err_cnt_pre
        return float(err) / (t * DataRate * pow(10, 9))
        
def bb_nrz(lane_obj,en=None):
    if en != None:
        lane_obj.RX_25G_EN = en
        lane_obj.Reg0100_0001_14 = 1-en
    else:
        bb = lane_obj.RX_25G_EN
        delta_en = lane_obj.Reg0100_0001_14
        return bb, delta_en

def agcgain(lane_obj, agcgain1 = None, agcgain2 = None):
    if agcgain1 == agcgain2 == None:
        agcgain1_v = Gray_Bin(lane_obj.Reg01D4_15_9)    
        agcgain2_v = Gray_Bin(lane_obj.Reg01D4_8_4)   
        return agcgain1_v, agcgain2_v
    else:
        if agcgain1 != None:
            lane_obj.Reg01D4_15_9 = Bin_Gray(agcgain1) 
        if agcgain2 != None:
            lane_obj.Reg01D4_8_4 = Bin_Gray(agcgain2)       

def Gray_Bin(gg=0): # up to 7-bit Gray Number
    bb1 = (gg & 0x40)
    bb2 = (gg ^ (bb1 >> 1)) & (0x20)
    bb3 = (gg ^ (bb2 >> 1)) & (0x10)
    bb4 = (gg ^ (bb3 >> 1)) & (0x8)
    bb5 = (gg ^ (bb4 >> 1)) & (0x4)
    bb6 = (gg ^ (bb5 >> 1)) & (0x2)
    bb7 = (gg ^ (bb6 >> 1)) & (0x1)
    bb = bb1+bb2+bb3+bb4+bb5+bb6+bb7
    return bb

def tx_prbs_en_nrz(lane_obj, en = None):
    if en == None:
        prbs_en = lane_obj.TX_PRBS_GEN_EN 
        test_patt_en = lane_obj.TX_TEST_EN 
        prbs_clk_en = lane_obj.TX_PRBS_CLK_EN 
        test_patt_sc = lane_obj.TX_TEST_DATA_SRC 
        val = prbs_en & test_patt_en & prbs_clk_en & test_patt_sc
        return val
    else:
        lane_obj.TX_PRBS_GEN_EN = en
        lane_obj.TX_TEST_EN = en
        lane_obj.TX_PRBS_CLK_EN = en
        lane_obj.TX_TEST_DATA_SRC = en

def tx_prbs_mode_nrz(lane_obj, pat = None):
    nrz_prbs_pat  = ['PRBS9', 'PRBS15', 'PRBS23', 'PRBS31'] 
    if pat == None:
        en = tx_prbs_en_nrz(lane_obj)
        val = lane_obj.TX_PRBS_MODE
        pat_sel = nrz_prbs_pat[val]
        return (en, pat_sel)
    elif type(pat) == int:
        tx_prbs_en_nrz(lane_obj,0)
        lane_obj.TX_PRBS_MODE = pat
        tx_prbs_en_nrz(lane_obj,1)
    elif type(pat) == str:
        pat_ind = nrz_prbs_pat.index(pat)
        tx_prbs_en_nrz(lane_obj,0)
        lane_obj.TX_PRBS_MODE = pat_ind
        tx_prbs_en_nrz(lane_obj,1)

def err_inject(lane_obj):
    lane_obj.TX_PRBS_GEN_ERR = 0
    time.sleep(0.001)
    lane_obj.TX_PRBS_GEN_ERR = 1
    time.sleep(0.001)
    lane_obj.TX_PRBS_GEN_ERR = 0

def tx_taps(lane_obj, tap1 = None, tap2 = None, tap3 = None, tap4 = None, tap5 = None, tap6 = None, tap7 = None, tap8 = None, tap9 = None, tap10 = None, tap11 = None):
    result = []
    if tap1 == tap2 == tap3 == tap4 == tap5 == tap6 == tap7 == tap8 == tap9 == tap10 == tap11 == None:
        tap1_v  = twos_to_int(lane_obj.TX_PRE_2,  8)   
        tap2_v  = twos_to_int(lane_obj.TX_PRE_1,  8)   
        tap3_v  = twos_to_int(lane_obj.TX_MAIN,  8)   
        tap4_v  = twos_to_int(lane_obj.TX_POST_1,  8)   
        tap5_v  = twos_to_int(lane_obj.TX_POST_2,  8)   
        tap6_v  = twos_to_int(lane_obj.Reg00B1_15_12,  4)   
        tap7_v  = twos_to_int(lane_obj.Reg00B1_11_8,  4)   
        tap8_v  = twos_to_int(lane_obj.Reg00B1_7_4,  4)   
        tap9_v  = twos_to_int(lane_obj.Reg00B1_3_0,  4)   
        tap10_v = twos_to_int(lane_obj.Reg00B2_15_12, 4)   
        tap11_v = twos_to_int(lane_obj.Reg00B2_11_8, 4)   
        a = [tap1_v, tap2_v, tap3_v, tap4_v, tap5_v, tap6_v, tap7_v, tap8_v, tap9_v, tap10_v, tap11_v]
        result.extend(a)
    else:
        if tap1  != None: lane_obj.TX_PRE_2  = int_to_twos(tap1, 8)    
        if tap2  != None: lane_obj.TX_PRE_1  = int_to_twos(tap2, 8)    
        if tap3  != None: lane_obj.TX_MAIN  = int_to_twos(tap3, 8)    
        if tap4  != None: lane_obj.TX_POST_1  = int_to_twos(tap4, 8)    
        if tap5  != None: lane_obj.TX_POST_2  = int_to_twos(tap5, 8)    
        if tap6  != None: lane_obj.Reg00B1_15_12  = int_to_twos(tap6, 4)    
        if tap7  != None: lane_obj.Reg00B1_11_8  = int_to_twos(tap7, 4)   
        if tap8  != None: lane_obj.Reg00B1_7_4  = int_to_twos(tap8, 4)    
        if tap9  != None: lane_obj.Reg00B1_3_0  = int_to_twos(tap9, 4)    
        if tap10 != None: lane_obj.Reg00B2_15_12 = int_to_twos(tap10,4)  
        if tap11 != None: lane_obj.Reg00B2_11_8 = int_to_twos(tap11,4)    
    if result != []:
        return result

def gc(lane_obj, tx_gc = None, rx_gc = None, print_en = None):
    if (tx_gc != None and rx_gc == None):
        lane_obj.TX_GRAYCODE_EN = tx_gc    
        lane_obj.RX_GRAYCODE_EN = tx_gc 
    elif (tx_gc != None and rx_gc != None):
        lane_obj.TX_GRAYCODE_EN = tx_gc    
        lane_obj.RX_GRAYCODE_EN = rx_gc 
    else:
        if print_en == None: print_en = 0
    tx_gc_this_lane = lane_obj.TX_GRAYCODE_EN  
    rx_gc_this_lane = lane_obj.RX_GRAYCODE_EN   
    if print_en:
        print("\nLane %s (%4s) GrayCode: TX: %d -- RX: %d"%(regFuncInit.getNameList()[lane_obj._laneno], (lane_obj._hive)._gname, tx_gc_this_lane, rx_gc_this_lane)),
    else:
        return tx_gc_this_lane, rx_gc_this_lane

def pc(lane_obj, tx_pc = None, rx_pc = None, print_en = None):
    if (tx_pc != None and rx_pc == None):
        lane_obj.TX_PRECODE_EN = tx_pc   
        lane_obj.RX_PRECODE_EN = tx_pc 
    elif (tx_pc != None and rx_pc != None):
        lane_obj.TX_PRECODE_EN = tx_pc    
        lane_obj.RX_PRECODE_EN = rx_pc 
    else:
        if print_en == None: print_en = 0
    tx_pc_this_lane = lane_obj.TX_PRECODE_EN  
    rx_pc_this_lane = lane_obj.RX_PRECODE_EN   
    if print_en:
        print("\nLane %s (%4s) Precoder: TX: %d -- RX: %d"%(regFuncInit.getNameList()[lane_obj._laneno], (lane_obj._hive)._gname, tx_pc_this_lane, rx_pc_this_lane)),
    else:
        return tx_pc_this_lane, rx_pc_this_lane

def msblsb(lane_obj, tx_msblsb = None, rx_msblsb = None, print_en = None):
    if (tx_msblsb != None and rx_msblsb == None):
        lane_obj.TX_SWAP_MSB_LSB = tx_msblsb       
        lane_obj.RX_SWAP_MSB_LSB = tx_msblsb    
    elif (tx_msblsb != None and rx_msblsb != None):
        lane_obj.TX_SWAP_MSB_LSB = tx_msblsb       
        lane_obj.RX_SWAP_MSB_LSB = rx_msblsb    
    else:
        if print_en == None: print_en = 0
    tx_msblsb_this_lane = lane_obj.TX_SWAP_MSB_LSB 
    rx_msblsb_this_lane = lane_obj.RX_SWAP_MSB_LSB  
    if print_en:
        print("\nLane %s (%4s) MSB-LSB Swap: TX: %d -- RX: %d"%(regFuncInit.getNameList()[lane_obj._laneno], (lane_obj._hive)._gname, tx_msblsb_this_lane, rx_msblsb_this_lane)),
    else:
        return tx_msblsb_this_lane, rx_msblsb_this_lane

def tx_gray(lane_obj, en = None):
    if en == None:
        return lane_obj.TX_GRAYCODE_EN
    else:
        lane_obj.TX_GRAYCODE_EN = en

def tx_precoder(lane_obj, en = None):
    if en == None:
        return lane_obj.TX_PRECODE_EN
    else:
        lane_obj.TX_PRECODE_EN = en

def tx(lane_obj):
    test_patt_en, mode = tx_test_patt(lane_obj)
    prbs_en, prbs_sel = tx_prbs_mode(lane_obj)
    taps = tx_taps(lane_obj)
    taps_sh = tx_taps_sh(lane_obj)
    taps = [taps[i] if taps_sh[i] == 0 else float(taps[i])/2.0 for i in range(len(taps))]
    sum = tx_taps_rule_check(lane_obj)[-1]
    print "PRBS enable: %s %s, test pattern enable: %s, taps: %s, %d"%(bool(prbs_en), prbs_sel, bool(test_patt_en), ','.join((map(str,taps))), sum)

def tx_test_patt_nrz(lane_obj, en=None, tx_test_patt4_val=0x0000,tx_test_patt3_val=0x0000,tx_test_patt2_val=0x0000,tx_test_patt1_val=0x0000):
    if en == None:
        prbs_en = lane_obj.TX_PRBS_GEN_EN
        test_patt_en = lane_obj.TX_TEST_EN
        prbs_clk_en = lane_obj.TX_PRBS_CLK_EN
        test_patt_sc = lane_obj.TX_TEST_DATA_SRC
        val = prbs_en & test_patt_en & prbs_clk_en & (1-test_patt_sc)
        m = lane_obj.PAM4_TEST_PAT_MODE
        return val, m
    elif en == 1:
        lane_obj.TX_TEST_EN = 1
        lane_obj.TX_TEST_DATA_SRC = 0
        lane_obj.TX_TEST_PAT_3_TX_TEST_PAT_2 = tx_test_patt4_val << 48
        lane_obj.TX_TEST_PAT_2_TX_TEST_PAT_1 = tx_test_patt3_val << 32
        lane_obj.TX_TEST_PAT_1_TX_TEST_PAT_0 = (tx_test_patt2_val << 16) + tx_test_patt1_val
    else:
        lane_obj.TX_TEST_EN = 0

def tx_pu(lane_obj, en = None):
    if en == None:
        return lane_obj.Reg00EB_13
    else:
        if en == 0:
            lane_obj.Reg00EB_13 = 0
        else:
            lane_obj.Reg00EB_13 = 1
            lane_obj.TX_PRBS_GEN_EN = 0
            lane_obj.TX_PRBS_GEN_EN = 1

def sig_det_nrz(lane_obj):
    sd_bit = lane_obj.RX_SIG_DET   
    return sd_bit

def ffegain(lane_obj, ffegain1 = None, ffegain2 = None):
    if ffegain1 == ffegain2 == None:
        ffegain1_v = Gray_Bin(lane_obj.DEGENMAIN_SUM4) 
        ffegain2_v = Gray_Bin(lane_obj.DEGENSUM_SUM4)   
        return ffegain1_v, ffegain2_v
    else:
        if ffegain1 != None:
            lane_obj.DEGENMAIN_SUM4 = Bin_Gray(ffegain1)   
        if ffegain2 != None:
            lane_obj.DEGENSUM_SUM4 = Bin_Gray(ffegain2) 

def dc_gain(lane_obj, agcgain1 = None, agcgain2 = None, ffegain1 = None, ffegain2 = None):    
    if agcgain1 == agcgain2 == ffegain1 == ffegain2 == None:
        agcgain_val = agcgain(lane_obj)
        ffegain_val = ffegain(lane_obj)
        if (agcgain_val != None) and (ffegain_val != None):
            return agcgain_val[0], agcgain_val[1], ffegain_val[0], ffegain_val[1]
    else:
        if agcgain1 != None or agcgain2 != None:
            agcgain(lane_obj, agcgain1, agcgain2)
        if ffegain1 != None or ffegain2 != None:
            ffegain(lane_obj, ffegain1, ffegain2)

def skef(lane_obj, en = None, val = None):
    if en == None:
        en_v = lane_obj.SKEF_EN 
        v = lane_obj.SKEF_VAL 
        return en_v, v
    else:
        lane_obj.SKEF_VAL = val   
        lane_obj.SKEF_EN = en   

def delta_nrz(lane_obj, val = None):
    if val == None:
        return lane_obj.DELTA_VAL    
    else:
        lane_obj.Reg0100_005F_6_0 = val  
        lane_obj.Reg0100_000A_12 = 0x1    

def ctle_nrz(lane_obj, val = None):
    if val == None:
        return lane_obj.RX_NRZ_CTLE_OVER_VAL 
    else:
        lane_obj.RX_NRZ_CTLE_OVER_VAL = val    
        lane_obj.RX_NRZ_CTLE_OVER_EN = 0x1  

def dac_nrz(lane_obj, en = None, val = None):
    if en == None:
        return lane_obj.DAC_SEL 
    else:
        lane_obj.Reg0100_004F_15_12 = val    
        lane_obj.Reg0100_004C_7 = en   

def edge(lane_obj, edge1 = None, edge2 = None, edge3 = None, edge4 = None):
    if edge1 == edge2 == edge3 == edge4 == None:
        return lane_obj.EDGE1, lane_obj.EDGE2, lane_obj.EDGE3, lane_obj.EDGE4
    else:
        if edge1 != None: lane_obj.EDGE1 = edge1
        if edge2 != None: lane_obj.EDGE2 = edge2
        if edge3 != None: lane_obj.EDGE3 = edge3
        if edge4 != None: lane_obj.EDGE4 = edge4

def eye_nrz(lane_obj):
    x = 200
    dac_sel_reg = dac_nrz(lane_obj)
    for i in range(50):
        rdy = sum(ready_nrz(lane_obj))
        if rdy == 2:
            cnt = 0
            while(1):
                eye_reg_1 = lane_obj.RX_READ_EM
                eye_reg_2 = lane_obj.RX_READ_EM
                if abs(eye_reg_1 - eye_reg_2) < 50: break
                if cnt > 1000: break
                cnt += 1
            if cnt > 1000: eye_reg_2 = 0.0
            em = (float(eye_reg_2) / 2048.0) * (x + (50.0 * float(dac_sel_reg)))
            break
        else:
            em = 0.0
        time.sleep(0.001)
    else:
        em = -1
    return dac_sel_reg, em

def get_lane_pll (lane_obj):
    pll_params = {}
    
    ref_clk = 156.25
    tx_div4_en = lane_obj.TX_REFCLK_DIV4_EN
    tx_div2_bypass = lane_obj.Reg00FF_1
    tx_pll_n = lane_obj.TX_PLL_N
    tx_pll_cap = lane_obj.TX_PLL_VCO_RANGE
    tx_10g_mode_en = lane_obj.TX_HALF_RATE_EN
    #tx_pll_frac_n = lane_obj.Reg00D9_3_0S

    tx_pll_frac_n = lane_obj.Reg00D9_3_0S
    tx_pll_frac_order = lane_obj.Reg00D7_15_14
    tx_pll_frac_en = lane_obj.Reg00D7_13

    rx_div4_en = lane_obj.RX_REFCLK_DIV4_EN
    rx_div2_bypass = lane_obj.Reg01F5_8
    rx_pll_n = lane_obj.RX_PLL_N
    rx_pll_cap = lane_obj.RX_PLL_VCO_RANGE

    rx_pll_frac_n = lane_obj.Reg01F1_15_0
    rx_pll_frac_order = lane_obj.Reg01F0_15_14
    rx_pll_frac_en = lane_obj.Reg01F0_13

    tx_div_by_4 = 1.0 if tx_div4_en == 0     else 4.0
    rx_div_by_4 = 1.0 if rx_div4_en == 0     else 4.0
    tx_mul_by_2 = 1.0 if tx_div2_bypass == 1 else 2.0
    rx_mul_by_2 = 1.0 if rx_div2_bypass == 1 else 2.0

    pam4_mode_en = 1 if (lane_obj.TX_NRZ_MODE == 0 and lane_obj.PAM4_EN == 1) else 0

    if pam4_mode_en: 
        data_rate_to_fvco_ratio = 2.0
    else: 
        if tx_10g_mode_en==0: 
            #print "enter NRZ25 mode"
            data_rate_to_fvco_ratio = 1.0
        else:                 
            data_rate_to_fvco_ratio = 0.5
    #print 'data_rate_to_fvco_ratio', data_rate_to_fvco_ratio
    tx_pll_n_float = float(tx_pll_n) + float(tx_pll_frac_n/1048575.0) if tx_pll_frac_en else float(tx_pll_n)
    
    rx_pll_n_float = float(rx_pll_n) + float(rx_pll_frac_n/65535.0) if rx_pll_frac_en else float(rx_pll_n)

    tx_fvco = (ref_clk * tx_pll_n_float * 2.0 * tx_mul_by_2) / tx_div_by_4 / 1000.0  
    rx_fvco = (ref_clk * rx_pll_n_float * 2.0 * rx_mul_by_2) / rx_div_by_4 / 1000.0

    tx_half_rate = lane_obj.HALF_RATE_SPEED_MODE
    rx_sub_rate = lane_obj.RX_SUB_RATE_MODE

    tx_data_rate = tx_fvco * data_rate_to_fvco_ratio/(2**tx_half_rate)
    rx_data_rate = rx_fvco * data_rate_to_fvco_ratio/(2**rx_sub_rate)

    #print 'ref_clk = ', ref_clk
    tx_pll_params = tx_data_rate, tx_fvco, tx_pll_cap, tx_pll_n_float, tx_div4_en, tx_div2_bypass, ref_clk, tx_pll_frac_en, tx_pll_frac_n
    rx_pll_params = rx_data_rate, rx_fvco, rx_pll_cap, rx_pll_n_float, rx_div4_en, rx_div2_bypass, ref_clk, rx_pll_frac_en, rx_pll_frac_n

    pll_params = [list(tx_pll_params), list(rx_pll_params)]
        
    return pll_params   

def set_tx_lane(lane_obj, on=1):
    if on == 0:
        lane_obj.TX_TEST_DATA_SRC = 0
        lane_obj.TX_TEST_PAT_3_TX_TEST_PAT_2 = 0x0
        lane_obj.TX_TEST_PAT_2_TX_TEST_PAT_1 = 0x0
        lane_obj.TX_TEST_PAT_1_TX_TEST_PAT_0 = 0x0
    else:
        lane_obj.TX_TEST_DATA_SRC = 1

def set_rx_lane(lane_obj, on=1):
    if on == 0:
        lane_obj.NRZ_SM_RESET = 0
    else:
        lane_obj.NRZ_SM_RESET = 0

def err_inject_nrz(lane_obj):
    lane_obj.TX_NRZ_PRBS_GERR_EN = 0
    time.sleep(0.001)
    lane_obj.TX_NRZ_PRBS_GERR_EN = 1
    time.sleep(0.01)
    lane_obj.TX_NRZ_PRBS_GERR_EN = 0

def dec2bin(x):
    x -= int(x)
    bins = []
    for i in range(8):
        x *= 2
        bins.append(1 if x >= 1. else 0)
        x -= int(x)
        # print bins
    value = 0
    for a in range(8):
        value = value + bins[7 - a] * pow(2, a)
    return value

def tx_sj_ampl(lane_obj, A=3):
    phase_value = [math.cos(0), math.cos((math.pi)/8), math.cos((math.pi)/4), math.cos(3*(math.pi)/8)]
    for i in range(4):
        value = A*phase_value[i]
        int_A = int(value)
        float_A = dec2bin(value-int_A)
        if i == 0:
            lane_obj.Reg00F9_15_5 = ((int_A << 8) & 0x700) + (float_A & 0xff)
        elif i == 1:
            lane_obj.Reg00F8_15_5 = ((int_A << 8) & 0x700) + (float_A & 0xff)
        elif i == 2:
            lane_obj.Reg00F7_15_5 = ((int_A << 8) & 0x700) + (float_A & 0xff)
        elif i == 3:
            lane_obj.Reg00F6_15_5 = ((int_A << 8) & 0x700) + (float_A & 0xff)

def tx_sj_freq(lane_obj, f=250):
    data_f = int(round((f/3.2), 0))
    lane_obj.Reg00F5_15_2 = data_f   #sj_freq value

def tx_sj_en(lane_obj, en=1):
    if en:
        lane_obj.TX_PI_EN = 0      # disable TRF before enabling injecting SJ
        lane_obj.ROTR_DIS = 0      # enable tx_rotator to inject SJ
    else: # Disable SJ
        lane_obj.ROTR_DIS = 1      # disable tx_rotator
        tx_sj_ampl(lane_obj, A=0)  # sj_ampl_0123  = 0
        tx_sj_freq(lane_obj, f=0)  # sj_freq    = 0
    return en

def tx_sj_range(lane_obj, A=3):
    tx_sj_en(lane_obj, en=1)
    tx_sj_ampl(lane_obj, A)
    lane_obj.Reg00F5_15_2 = 0x3FFF    # sj_freq = max


def tx_sj(lane_obj, A=0.3, f=2500):
    result = {}
    if (A != None and A > 0.0):
        sj_en = 1
    elif (A != None):
        sj_en = 0
    if A != None:
        if sj_en:
            tx_sj_en(lane_obj, en=1)
            tx_sj_ampl(lane_obj, A)
            tx_sj_freq(lane_obj, f)
        else:  # Disable SJ
            tx_sj_en(lane_obj, en=0)

    sj_en_status = 'dis' if lane_obj.ROTR_DIS == 1 else 'en'
    sj_amp = float((lane_obj.Reg00F9_15_5 & 0x700) >> 8) + (float(lane_obj.Reg00F9_15_5 & 0xff) / 256.0)
    sj_freq = int(float(lane_obj.Reg00F5_15_2) * 3.2)
    result[lane_obj] = sj_en_status, sj_amp, sj_freq

    return result

def twos_to_int(twos_val, bitWidth):
    '''
    return a signed decimal number
    '''
    mask = 1 << (bitWidth - 1)
    return -(twos_val & mask) + (twos_val & ~mask)

def Bin_Gray(bb=0):
    gg = bb ^ (bb >> 1)
    return gg







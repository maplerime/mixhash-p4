import time, re
from BaseFunction.common import *
from credo import *
permMode.setFilename(__file__)
permMode.setHivename('PAM4')

def rx_checker_pam4(lane_obj, status = None):
    if status==None:
        return lane_obj.PU_PRBS_SYNC_CHKR
    else:
        lane_obj.PU_PRBS_SYNC_CHKR = status
        lane_obj.PU_PRBS_CHKR = status
        lane_obj.RX_PRBS_AUTO_SYNC_EN = status

def sm_cont_pam4(lane_obj):
    lane_obj.PAM4_SM_CONT = 1
    time.sleep(0.001)
    lane_obj.PAM4_SM_CONT = 0

def bp1(lane_obj, en=None, st=0):
    if en == None:
        en_v = lane_obj.PAM4_BP1_EN
        st_v = lane_obj.PAM4_BP1_STATE
        reached = lane_obj.PAM4_BP1_REACHED
        return en_v, st_v, reached
    else:
        lane_obj.PAM4_BP1_STATE = st
        lane_obj.PAM4_BP1_EN = en

def bp2(lane_obj, en=None, st=0):
    if en == None:
        en_v = lane_obj.PAM4_BP2_EN
        st_v = lane_obj.PAM4_BP2_STATE
        reached = lane_obj.PAM4_BP2_REACHED
        return en_v, st_v, reached
    else:
        lane_obj.PAM4_BP2_STATE = st
        lane_obj.PAM4_BP2_EN = en

def lr_pam4(lane_obj):
    super_cal = lane_obj.MU_OFFSET_OW 
    if super_cal != 0:
        lane_obj.MU_OFFSET_OWEN = 1           
        lane_obj.MU_OFFSET_OW = 0 
    updn  = (lane_obj.THETA2_UPDATE_MODE << 2)
    updn += (lane_obj.THETA3_UPDATE_MODE << 1)
    updn += (lane_obj.THETA4_UPDATE_MODE)
    if updn != 0:
        lane_obj.THETA2_UPDATE_MODE = 0
        lane_obj.THETA3_UPDATE_MODE = 0
        lane_obj.THETA4_UPDATE_MODE = 0
    lane_obj.Reg01D5_8 = 0
    lane_obj.THETA2_UPDATE_MODE = 0
    lane_obj.THETA3_UPDATE_MODE = 0
    lane_obj.THETA4_UPDATE_MODE = 0
    lane_obj.PAM4_SM_RESET = 0x1 
    time.sleep(.050)        
    lane_obj.PAM4_SM_RESET = 0x0 
            
    if updn != 0: 
        lane_obj.THETA2_UPDATE_MODE = (updn >> 2)
        lane_obj.THETA3_UPDATE_MODE = (updn >> 1) & 0x1
        lane_obj.THETA4_UPDATE_MODE = (updn & 0x1)
    lane_obj.MU_OFFSET_OWEN = 1   
    if super_cal != 0: 
        lane_obj.MU_OFFSET_OW = super_cal         
    lane_obj.BLWC_EN = 1
    
def eye_pam4(lane_obj):
    DACQ_reg = lane_obj.READ_DAC_SEL  
    eye_margin = []
    for eye_index in range(0, 3):
        result1 = 0xffff
        for y in range(0, 4):
            sel = 3 * y + eye_index
            lane_obj.MINUS_MARGIN_SEL = sel  
            lane_obj.PLUS_MARGIN_SEL = sel  
            plus_margin = lane_obj.READ_PLUS_MARGIN_2C    
            if (plus_margin > 0x7ff):
                plus_margin = plus_margin - 0x1000
            minus_margin = lane_obj.READ_MINUS_MARGIN_2C  # This is for two register name combined
            if (minus_margin > 0x7ff):
                minus_margin = minus_margin - 0x1000
            diff = plus_margin - minus_margin
            if (diff < result1):
                result1 = diff
            else:
                result1 = result1
        eye_margin.append((result1))
    em0, em1, em2 = map(lambda em: float(em) / 2048.0 * (100.0 + 50.0 * float(DACQ_reg)), eye_margin)
    return em0, em1, em2

####################################################################
## EYE Margin, by Firmware
####################################################################
def fw_eye_pam4(lane = None):

    #fw_eye_en = fw_loaded(print_en=0) and fw_date(print_en=0)>=18015 and fw_reg_rd(128)!=0 # FW Date 20190429 or later
    # if fw_eye_en == False:
        # print("\n*** FW Not Loaded, or "),
        # print("\n*** FW Eye Margin function Not Available In This Release (Need DateCode 20190429 or later), or"),
        # print("\n*** FW Background Functions are Disabled (FW REG 128 = 0 instead of 0xFFFF)\n"),
        # return -1, -1, -1
        
    lanes = get_lane_list(lane)
    result = {}
    for ln in lanes:    
        get_lane_mode(ln)
        line_encoding = lane_mode_list[ln].lower()
        c = Pam4Reg if line_encoding == 'pam4' else NrzReg
        x = 100 if line_encoding == 'pam4' else 200
        rdy = sum(ready(ln)[ln])
        em=[-1,-1,-1]
        ##### PAM4 EYE
        if line_encoding == 'pam4' and rdy == 3:
            fw_debug_cmd(section=10, index=5, lane=ln)
            em = [rreg(0x5000+eye_index) for eye_index in range(3)]
        ##### NRZ EYE
        elif line_encoding == 'nrz' and rdy == 3: # NRZ EYE
            dac_val = dac(lane=ln)[ln]
            eye_reg_val = rreg(c.rx_em_addr, ln)
            em[0] = (float(eye_reg_val) / 2048.0) * (x + (50.0 * float(dac_val)))            
            em[1] = 0
            em[2] = 0
        result[ln] = int(em[0]),int(em[1]),int(em[2])                
    return result
    
def ber_pam4(lane_obj, rst=1, t=5):
    if (rst == 1):
        prbs_rst_pam4(lane_obj)
    err_cnt_pre = get_err_pam4(lane_obj)
    time.sleep(t)
    err_cnt_post = get_err_pam4(lane_obj)
    DataRate = get_lane_pll(lane_obj)[0][0]  # 25.78125
    # print DataRate
    if err_cnt_pre == -1:
        return -1
    else:
        err = err_cnt_post - err_cnt_pre
        return float(err) / (t * DataRate * pow(10, 9))

def get_err_pam4(lane_obj):
    # check if prbs checker is on
    checker = lane_obj.PU_PRBS_SYNC_CHKR  # rreg(c.rx_prbs_checker_pu_addr, lane)
    rdy = sum(ready_pam4(lane_obj))
    if checker == 0:
        print('***Lane {} PRBS checker is off***'.format(regFuncInit.getNameList()[lane_obj._laneno]))
        return -1
    else:
        # long(rreg(c.rx_err_cntr_msb_addr, lane)<<16) + rreg(c.rx_err_cntr_lsb_addr, lane)
        return long(lane_obj.PRBS_READ_SYNC_ERR_CNTR)  # This is for two register name combined

def ctle_map_pam4(lane_obj, sel = None, val1 = None, val2 = None):
    if None in (sel, val1, val2):
        agc_0 = lane_obj.PAM4_CTLE_TABLE_0
        agc_1 = lane_obj.PAM4_CTLE_TABLE_1
        agc_2 = lane_obj.PAM4_CTLE_TABLE_2
        agc_3 = lane_obj.PAM4_CTLE_TABLE_3
        agc_4 = lane_obj.PAM4_CTLE_TABLE_4
        agc_5 = lane_obj.PAM4_CTLE_TABLE_5
        agc_6 = lane_obj.PAM4_CTLE_TABLE_6
        agc_7 = lane_obj.PAM4_CTLE_TABLE_7
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
            lane_obj.PAM4_CTLE_TABLE_0 = val
        elif sel == 1:
            lane_obj.PAM4_CTLE_TABLE_1 = val
        elif sel == 2:
            lane_obj.PAM4_CTLE_TABLE_2 = val
        elif sel == 3:
            lane_obj.PAM4_CTLE_TABLE_3 = val
        elif sel == 4:
            lane_obj.PAM4_CTLE_TABLE_4 = val
        elif sel == 5:
            lane_obj.PAM4_CTLE_TABLE_5 = val
        elif sel == 6:
            lane_obj.PAM4_CTLE_TABLE_6 = val
        else:
            lane_obj.PAM4_CTLE_TABLE_7 = val

def ffe_taps(lane_obj, k1 = None, k2 = None, k3 = None, k4 = None, s1 = None, s2 = None):
    if k1 != None:
        pol1 = 1 if k1 < 0 else 0
        k11 = abs(k1)
        rx_ffe_k1_gray_msb = Bin_Gray(k11 >> 4)
        rx_ffe_k1_gray_lsb = Bin_Gray(k11 & 0xf)
        rx_ffe_k1_gray = ((rx_ffe_k1_gray_msb << 4) + rx_ffe_k1_gray_lsb) & 0xFF
        lane_obj.DEGENMAIN1_SUM3 = rx_ffe_k1_gray_msb   # wreg(c.rx_ffe_k1_msb_addr, rx_ffe_k1_gray_msb, lane)
        lane_obj.DEGENMAIN0_SUM3 = rx_ffe_k1_gray_lsb   # wreg(c.rx_ffe_k1_lsb_addr, rx_ffe_k1_gray_lsb, lane)
        lane_obj.POL_MUX1 = pol1    # wreg(c.rx_ffe_pol1_addr, pol1, lane)
    if k2 != None:
        pol2 = 1 if k2 < 0 else 0
        k22 = abs(k2)
        rx_ffe_k2_gray_msb = Bin_Gray(k22 >> 4)
        rx_ffe_k2_gray_lsb = Bin_Gray(k22 & 0xf)
        rx_ffe_k2_gray = ((rx_ffe_k2_gray_msb << 4) + rx_ffe_k2_gray_lsb) & 0xFF
        lane_obj.DEGENMAIN1_SUM2 = rx_ffe_k2_gray_msb   # wreg(c.rx_ffe_k2_msb_addr, rx_ffe_k2_gray_msb, lane)
        lane_obj.DEGENMAIN0_SUM2 = rx_ffe_k2_gray_lsb   # wreg(c.rx_ffe_k2_lsb_addr, rx_ffe_k2_gray_lsb, lane)
        lane_obj.POL_MUX2 = pol2    # wreg(c.rx_ffe_pol2_addr, pol2, lane)
    if k3 != None:
        pol3 = 1 if k3 < 0 else 0
        k33 = abs(k3)
        rx_ffe_k3_gray_lsb = Bin_Gray(k33 & 0xf)
        rx_ffe_k3_gray_msb = Bin_Gray(k33 >> 4)
        rx_ffe_k3_gray = ((rx_ffe_k3_gray_msb << 4) + rx_ffe_k3_gray_lsb) & 0xFF
        lane_obj.DEGENMAIN1_SUM1 = rx_ffe_k3_gray_msb   # wreg(c.rx_ffe_k3_msb_addr, rx_ffe_k3_gray_msb, lane)
        lane_obj.DEGENMAIN0_SUM1 = rx_ffe_k3_gray_lsb   # wreg(c.rx_ffe_k3_lsb_addr, rx_ffe_k3_gray_lsb, lane)
        lane_obj.POL_MUX3 = pol3    # wreg(c.rx_ffe_pol3_addr, pol3, lane)
    if k4 != None:
        pol4 = 1 if k4<0 else 0
        k44 = abs(k4)
        rx_ffe_k4_gray_lsb = Bin_Gray(k44 & 0xf)
        rx_ffe_k4_gray_msb = Bin_Gray(k44 >> 4)
        rx_ffe_k4_gray = ((rx_ffe_k4_gray_msb << 4) + rx_ffe_k4_gray_lsb) & 0xFF
        lane_obj.DEGENSUM1_SUM1 = rx_ffe_k4_gray_msb    # wreg(c.rx_ffe_k4_msb_addr, rx_ffe_k4_gray_msb, lane)
        lane_obj.DEGENSUM0_SUM1 = rx_ffe_k4_gray_lsb    # wreg(c.rx_ffe_k4_lsb_addr, rx_ffe_k4_gray_lsb, lane)
        lane_obj.POL_MUX4 = pol4    # wreg(c.rx_ffe_pol4_addr, pol4, lane)
    if s2 != None:
        rx_ffe_s2_gray_lsb = Bin_Gray(s2 & 0x0f)
        rx_ffe_s2_gray_msb = Bin_Gray((s2 >> 4) & 0x0f)
        rx_ffe_s2_gray = ((rx_ffe_s2_gray_msb << 4) + rx_ffe_s2_gray_lsb) & 0xFF
        lane_obj.DEGENSUM1_SUM2 = rx_ffe_s2_gray_msb    # wreg(c.rx_ffe_s2_msb_addr, rx_ffe_s2_gray_msb, lane)
        lane_obj.DEGENSUM0_SUM2 = rx_ffe_s2_gray_lsb    # wreg(c.rx_ffe_s2_lsb_addr, rx_ffe_s2_gray_lsb, lane)
    if s1 != None:
        rx_ffe_s1_gray_lsb = Bin_Gray(s1 & 0x0f)
        rx_ffe_s1_gray_msb = Bin_Gray((s1 >> 4) & 0x0f)
        rx_ffe_s1_gray = ((rx_ffe_s1_gray_msb << 4) + rx_ffe_s1_gray_lsb) & 0xFF
        lane_obj.DEGENSUM1_SUM3 = rx_ffe_s1_gray_msb    # wreg(c.rx_ffe_s1_msb_addr, rx_ffe_s1_gray_msb, lane)
        lane_obj.DEGENSUM0_SUM3 = rx_ffe_s1_gray_lsb    # wreg(c.rx_ffe_s1_lsb_addr, rx_ffe_s1_gray_lsb, lane)
    if k1 == None and k2 == None and k3 == None and k4 == None and s1 == None and s2 == None:
        pol1 = lane_obj.POL_MUX1    # rreg(c.rx_ffe_pol1_addr, lane)
        rx_ffe_k1_msb = Gray_Bin(lane_obj.DEGENMAIN1_SUM3)  #rreg(c.rx_ffe_k1_msb_addr, lane)
        rx_ffe_k1_lsb = Gray_Bin(lane_obj.DEGENMAIN0_SUM3)  # rreg(c.rx_ffe_k1_lsb_addr, lane)
        rx_ffe_k1_bin = (1 - 2 * pol1) * (((rx_ffe_k1_msb << 4) + rx_ffe_k1_lsb) & 0xFF)
        
        pol2 = lane_obj.POL_MUX2    # rreg(c.rx_ffe_pol2_addr, lane)
        rx_ffe_k2_msb = Gray_Bin(lane_obj.DEGENMAIN1_SUM2)  # rreg(c.rx_ffe_k2_msb_addr,  lane)
        rx_ffe_k2_lsb = Gray_Bin(lane_obj.DEGENMAIN0_SUM2)  # rreg(c.rx_ffe_k2_lsb_addr,  lane)
        rx_ffe_k2_bin = (1 - 2 * pol2) * (((rx_ffe_k2_msb << 4) + rx_ffe_k2_lsb) & 0xFF)

        pol3 = lane_obj.POL_MUX3    # rreg(c.rx_ffe_pol3_addr, lane)
        rx_ffe_k3_msb = Gray_Bin(lane_obj.DEGENMAIN1_SUM1)  # rreg(c.rx_ffe_k3_msb_addr,  lane)
        rx_ffe_k3_lsb = Gray_Bin(lane_obj.DEGENMAIN0_SUM1)  # rreg(c.rx_ffe_k3_lsb_addr,  lane)
        rx_ffe_k3_bin = (1 - 2 * pol3) * (((rx_ffe_k3_msb << 4) + rx_ffe_k3_lsb) & 0xFF)
            
        pol4 = lane_obj.POL_MUX4    # rreg(c.rx_ffe_pol4_addr, lane)
        rx_ffe_k4_msb = Gray_Bin(lane_obj.DEGENSUM1_SUM1)   # rreg(c.rx_ffe_k4_msb_addr, lane)
        rx_ffe_k4_lsb = Gray_Bin(lane_obj.DEGENSUM0_SUM1)   # rreg(c.rx_ffe_k4_lsb_addr, lane)
        rx_ffe_k4_bin = (1 - 2 * pol4) * (((rx_ffe_k4_msb << 4) + rx_ffe_k4_lsb) & 0xFF)
            
        rx_ffe_s1_msb = Gray_Bin(lane_obj.DEGENSUM1_SUM3)   # rreg(c.rx_ffe_s1_msb_addr,  lane)
        rx_ffe_s1_lsb = Gray_Bin(lane_obj.DEGENSUM0_SUM3)   # rreg(c.rx_ffe_s1_lsb_addr,  lane)
        rx_ffe_s1_bin = ((rx_ffe_s1_msb << 4) + rx_ffe_s1_lsb) & 0xFF
            
        rx_ffe_s2_msb = Gray_Bin(lane_obj.DEGENSUM1_SUM2)   # rreg(c.rx_ffe_s2_msb_addr,  lane)
        rx_ffe_s2_lsb = Gray_Bin(lane_obj.DEGENSUM0_SUM2)   # rreg(c.rx_ffe_s2_lsb_addr,  lane)
        rx_ffe_s2_bin = ((rx_ffe_s2_msb << 4) + rx_ffe_s2_lsb) & 0xFF  
        return rx_ffe_k1_bin, rx_ffe_k2_bin, rx_ffe_k3_bin, rx_ffe_k4_bin, rx_ffe_s1_bin, rx_ffe_s2_bin 

def state_pam4(lane_obj):
    return lane_obj.PAM4_CURRENT_STATE
    
def prbs_rst_pam4(lane_obj):
    wait = 0.001
    lane_obj.PRBS_SYNC_CNTR_RESET = 0  
    time.sleep(wait)
    lane_obj.PRBS_SYNC_CNTR_RESET = 1  
    time.sleep(wait)
    lane_obj.PRBS_SYNC_CNTR_RESET = 0  

def agcgain(lane_obj, agcgain1 = None, agcgain2 = None):
    if agcgain1 == agcgain2 == None:
        agcgain1_v = Gray_Bin(lane_obj.CTLE_GAIN_1)    
        agcgain2_v = Gray_Bin(lane_obj.CTLE_GAIN_2)   
        return agcgain1_v, agcgain2_v
    else:
        if agcgain1 != None:
            lane_obj.CTLE_GAIN_1 = Bin_Gray(agcgain1)  
        if agcgain2 != None:
            lane_obj.CTLE_GAIN_2 = Bin_Gray(agcgain2)  
            
            
def Gray_Bin(gg=0): # up to 7-bit Gray Number
    bb1=(gg&0x40)
    bb2=(gg^(bb1>>1))&(0x20)
    bb3=(gg^(bb2>>1))&(0x10)
    bb4=(gg^(bb3>>1))&(0x8)
    bb5=(gg^(bb4>>1))&(0x4)
    bb6=(gg^(bb5>>1))&(0x2)
    bb7=(gg^(bb6>>1))&(0x1)
    bb=bb1+bb2+bb3+bb4+bb5+bb6+bb7
    return bb

def Bin_Gray(bb=0):
    gg=bb^(bb>>1)
    return gg
    
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
        tap6_v  = twos_to_int(lane_obj.TX_POST_3,  4)  
        tap7_v  = twos_to_int(lane_obj.TX_POST_4,  4)   
        tap8_v  = twos_to_int(lane_obj.TX_POST_5,  4)  
        tap9_v  = twos_to_int(lane_obj.TX_POST_6,  4)   
        tap10_v = twos_to_int(lane_obj.TX_POST_7, 4)   
        tap11_v = twos_to_int(lane_obj.TX_POST_8, 4)   
        a = [tap1_v, tap2_v, tap3_v, tap4_v, tap5_v, tap6_v, tap7_v, tap8_v, tap9_v, tap10_v, tap11_v]
        result.extend(a)
    else:
        if tap1  != None: lane_obj.TX_PRE_2  = int_to_twos(tap1, 8)    
        if tap2  != None: lane_obj.TX_PRE_1  = int_to_twos(tap2, 8)    
        if tap3  != None: lane_obj.TX_MAIN  = int_to_twos(tap3, 8)    
        if tap4  != None: lane_obj.TX_POST_1  = int_to_twos(tap4, 8)    
        if tap5  != None: lane_obj.TX_POST_2  = int_to_twos(tap5, 8)    
        if tap6  != None: lane_obj.TX_POST_3  = int_to_twos(tap6, 4)   
        if tap7  != None: lane_obj.TX_POST_4  = int_to_twos(tap7, 4)    
        if tap8  != None: lane_obj.TX_POST_5  = int_to_twos(tap8, 4)   
        if tap9  != None: lane_obj.TX_POST_6  = int_to_twos(tap9, 4)    
        if tap10 != None: lane_obj.TX_POST_7 = int_to_twos(tap10,4)    
        if tap11 != None: lane_obj.TX_POST_8 = int_to_twos(tap11,4)    
    if result != []:
        return result

def tx_taps_sh(lane_obj, sh5 = None, sh4 = None, sh3 = None, sh2 = None, sh1 = None):
    if sh1 == sh2 == sh3 == sh4 == sh5 == None:
        sh1 = lane_obj.TX_PRE_2_sh
        sh2 = lane_obj.TX_PRE_1_sh
        sh3 = lane_obj.TX_MAIN_sh
        sh4 = lane_obj.TX_POST_1_sh
        sh5 = lane_obj.TX_POST_2_sh
        return sh5, sh4, sh3, sh2, sh1
    else:
        if sh1 != None: lane_obj.TX_PRE_2_sh = sh1
        if sh2 != None: lane_obj.TX_PRE_1_sh = sh2
        if sh3 != None: lane_obj.TX_MAIN_sh = sh3
        if sh4 != None: lane_obj.TX_POST_1_sh = sh4
        if sh5 != None: lane_obj.TX_POST_2_sh = sh5

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


def get_lane_pll(lane_obj):
    pll_params = {}

    ref_clk = 156.25
    tx_div4_en = lane_obj.TX_REFCLK_DIV4_EN
    tx_div2_bypass = lane_obj.TX_PLL_BYPASS_DIV2
    tx_pll_n = lane_obj.TX_PLL_N
    tx_pll_cap = lane_obj.TX_PLL_VCO_RANGE
    tx_10g_mode_en = lane_obj.TX_HALF_RATE_EN
    #tx_pll_frac_n = lane_obj.Reg00D9_3_0S

    tx_pll_frac_n = lane_obj.TX_PLL_N_FRAC
    tx_pll_frac_order = lane_obj.TX_PLL_N_FRAC_CTRL
    tx_pll_frac_en = lane_obj.TX_PLL_N_FRAC_EN

    rx_div4_en = lane_obj.RX_REFCLK_DIV4_EN
    rx_div2_bypass = lane_obj.RX_PLL_BYPASS_DIV2
    rx_pll_n = lane_obj.RX_PLL_N
    rx_pll_cap = lane_obj.RX_PLL_VCO_RANGE

    rx_pll_frac_n = lane_obj.RX_PLL_N_FRAC
    rx_pll_frac_order = lane_obj.RX_PLL_N_FRAC_CTRL
    rx_pll_frac_en = lane_obj.RX_PLL_N_FRAC_EN
    
    tx_div_by_4 = 1.0 if tx_div4_en == 0 else 4.0
    rx_div_by_4 = 1.0 if rx_div4_en == 0 else 4.0
    tx_mul_by_2 = 1.0 if tx_div2_bypass == 1 else 2.0
    rx_mul_by_2 = 1.0 if rx_div2_bypass == 1 else 2.0

    pam4_mode_en = 1 if (lane_obj.TX_NRZ_MODE == 0 and lane_obj.PAM4_EN == 1) else 0
    #print 'pam4_mode_en',pam4_mode_en
    if pam4_mode_en:
        data_rate_to_fvco_ratio = 2.0
    else:
        if tx_10g_mode_en == 0:
            #print "enter NRZ25 mode"
            data_rate_to_fvco_ratio = 1.0
        else:
            data_rate_to_fvco_ratio = 0.5
    #print 'data_rate_to_fvco_ratio', data_rate_to_fvco_ratio
    #print 'rx_pll_n', rx_pll_n
    #print 'rx_pll_frac_n', rx_pll_frac_n
    tx_pll_n_float = float(tx_pll_n) + float(tx_pll_frac_n / 1048575.0) if tx_pll_frac_en else float(tx_pll_n)

    rx_pll_n_float = float(rx_pll_n) + float(rx_pll_frac_n / 65535.0) if rx_pll_frac_en else float(rx_pll_n)
    #print 'rx_pll_n_float', rx_pll_n_float
    #print 'rx_div_by_4', rx_div_by_4
    #print 'rx_mul_by_2', rx_mul_by_2
    tx_fvco = (ref_clk * tx_pll_n_float * 2.0 * tx_mul_by_2) / tx_div_by_4 / 1000.0
    rx_fvco = (ref_clk * rx_pll_n_float * 2.0 * rx_mul_by_2) / rx_div_by_4 / 1000.0

    tx_half_rate = lane_obj.HALF_RATE_SPEED_MODE
    rx_sub_rate = lane_obj.RX_SUB_RATE_MODE

    tx_data_rate = tx_fvco * data_rate_to_fvco_ratio/(2**tx_half_rate)
    rx_data_rate = rx_fvco * data_rate_to_fvco_ratio/(2**rx_sub_rate)

    tx_pll_params = tx_data_rate, tx_fvco, tx_pll_cap, tx_pll_n_float, tx_div4_en, tx_div2_bypass, ref_clk, tx_pll_frac_en, tx_pll_frac_n
    rx_pll_params = rx_data_rate, rx_fvco, rx_pll_cap, rx_pll_n_float, rx_div4_en, rx_div2_bypass, ref_clk, rx_pll_frac_en, rx_pll_frac_n

    pll_params = [list(tx_pll_params), list(rx_pll_params)]

    return pll_params

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

def tx_test_patt_pam4(lane_obj, en=None, tx_test_patt4_val=0x0000,tx_test_patt3_val=0x0000,tx_test_patt2_val=0x0000,tx_test_patt1_val=0x0000):
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
        lane_obj.TX_TEST_PAT_3 = tx_test_patt4_val
        lane_obj.TX_TEST_PAT_2 = tx_test_patt3_val
        lane_obj.TX_TEST_PAT_1 = tx_test_patt2_val
        lane_obj.TX_TEST_PAT_0 = tx_test_patt1_val
    else:
        lane_obj.TX_TEST_EN = 0

def tx_prbs_en_pam4(lane_obj, en = None):
    if en == None:
        prbs_en = lane_obj.TX_PRBS_GEN_EN 
        test_patt_en = lane_obj.TX_TEST_EN 
        prbs_clk_en = lane_obj.TX_PRBS_CLK_EN 
        test_patt_sc = lane_obj.TX_TEST_DATA_SRC 
        val = prbs_en & test_patt_en & prbs_clk_en & test_patt_sc
        return val
    else:
        lane_obj.TX_TEST_EN = en
        lane_obj.TX_PRBS_CLK_EN = en
        lane_obj.TX_TEST_DATA_SRC = en
        lane_obj.TX_PRBS_GEN_EN = en

def tx_prbs_mode_pam4(lane_obj, pat = None):
    pam4_prbs_pat = ['PRBS9', 'PRBS13', 'PRBS15', 'PRBS31']
    if pat == None:
        en = tx_prbs_en_pam4(lane_obj)
        val = lane_obj.TX_PRBS_MODE
        pat_sel = pam4_prbs_pat[val]
        return (en, pat_sel)
    elif type(pat) == int:
        #tx_prbs_en_pam4(lane_obj,0)
        lane_obj.TX_PRBS_MODE = pat
        tx_prbs_en_pam4(lane_obj,1)
    elif type(pat) == str:
        pat_ind = pam4_prbs_pat.index(pat)
        #tx_prbs_en_pam4(lane_obj,0)
        lane_obj.TX_PRBS_MODE = pat_ind
        tx_prbs_en_pam4(lane_obj,1)

def get_err_pam4(lane_obj):
    checker = lane_obj.PU_PRBS_SYNC_CHKR 
    rdy = sum(ready_pam4(lane_obj))
    if checker == 0:
        print('***Lane {} PRBS checker is off***'.format(regFuncInit.getNameList()[lane_obj._laneno]))
        return -1
    else:
        latch_data_pam4(lane_obj)
        cnt = long(lane_obj.PRBS_READ_SYNC_ERR_CNTR) # This is for two register name combined
        latch_data_release_pam4(lane_obj)
        return cnt
        #return long(lane_obj.PRBS_READ_SYNC_ERR_CNTR_MSB_PRBS_READ_SYNC_ERR_CNTR_LSB)

def rx_prbs_mode_pam4(lane_obj, pat = None):
    pam4_prbs_pat = ['PRBS9', 'PRBS13', 'PRBS15', 'PRBS31']
    if pat == None:
        checker = rx_checker_pam4(lane_obj)
        patt_v = lane_obj.PRBS_MODE_SEL 
        pat_sel = pam4_prbs_pat[patt_v]
        return checker, pat_sel
    elif type(pat) == int:
        rx_checker_pam4(lane_obj, 1)
        lane_obj.PRBS_MODE_SEL = pat
    elif type(pat) == str:
        val = pam4_prbs_pat.index(pat)
        rx_checker_pam4(lane_obj, 1)
        lane_obj.PRBS_MODE_SEL = val 

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

def edge(lane_obj, edge1 = None, edge2 = None, edge3 = None, edge4 = None):
    if edge1 == edge2 == edge3 == edge4 == None:
        return lane_obj.EDGE1, lane_obj.EDGE2, lane_obj.EDGE3, lane_obj.EDGE4
    else:
        if edge1 != None: lane_obj.EDGE1 = edge1
        if edge2 != None: lane_obj.EDGE2 = edge2
        if edge3 != None: lane_obj.EDGE3 = edge3
        if edge4 != None: lane_obj.EDGE4 = edge4
        
def f13(lane_obj, val = None):
    if type(val)==str and val.upper()=='ALL': 
        val = None
    if val != None: 
        val_to_write = (val < 0) and (val + 0x80) or val
        lane_obj.DFE_INIT_VAL = val_to_write   
    f1o3 = lane_obj.DFE_INIT_VAL   
    return (f1o3 > 0x40) and (f1o3 - 0x80) or f1o3

def ctle_pam4(lane_obj, val = None):
    if val == None:
        return lane_obj.RX_PAM4_CTLE_OVER_VAL    
    else:
        lane_obj.RX_PAM4_CTLE_OVER_VAL = val 
        lane_obj.RX_PAM4_CTLE_OVER_EN = 0x1   

def delta_pam4(lane_obj, val = None):
    if val == None:
        return lane_obj.READ_DELTA_2C   
    else:
        lane_obj.DELTA_OW = val   
        lane_obj.DELTA_OWEN = 0x1 

def delta_ph_pam4(lane_obj, val = None):
    if val != None:
        delta_to_write= ((val < 0) and (val + 0x80) or val)
        if delta_to_write >= 64:
            delta_to_write -= 128
        lane_obj.DELTA_OWEN = 1
        lane_obj.DELTA_OW = delta_to_write
        delta_val= lane_obj.DELTA_OW
    return (delta_val > 0x40) and (delta_val - 0x80) or delta_val

def get_pam4_dfe(lane_obj, print_en = 0):
    ths_sel_list = [x for x in range(12)]
    ths_list = []
    wait = 0.01
    for val in ths_sel_list:
        lane_obj.THS_SEL = val
        time.sleep(wait)
        readout = lane_obj.READ_THS_2C
        readout2 = lane_obj.READ_THS_2C
        if readout == readout2:
            result = ((float(readout) / 2048.0) + 1.0) % 2.0 - 1.0
            if print_en: print("\n%2d 0x%04X %6.3f"%(val, readout, result)),
        else:
            readout = lane_obj.READ_THS_2C
            result = ((float(readout) / 2048.0) + 1.0) % 2.0 - 1.0
            if print_en: print("\n*%2d 0x%04X %6.3f"%(val, readout, result)),
        ths_list.append(result)
    f0 = (-3.0 / 16) * ((ths_list[0] - ths_list[2]) + (ths_list[3] - ths_list[5]) + (ths_list[6] - ths_list[8]) + (ths_list[9] - ths_list[11]))
    f1 = (-3.0 / 20) * ((ths_list[0] + ths_list[1] + ths_list[2] - ths_list[9] - ths_list[10] - ths_list[11]) + (1 / 3) * (ths_list[3] + ths_list[4] + ths_list[5] - ths_list[6] - ths_list[7] - ths_list[8]))
    if print_en: print("\n")
    try:
        ratio = f1/ f0
    except ZeroDivisionError:
        ratio = 0
    return f0, f1, ratio

def dac_pam4(lane_obj, en = None, val = None):
    if en == None:
        return lane_obj.READ_DAC_SEL 
    else:
        lane_obj.DAC_OW_SEL = val 
        lane_obj.DAC_OWEN_SEL = en    

def ready_pam4(lane_obj):
    sd = lane_obj.READ_SIG_DET    
    rdy = lane_obj.RX_READ_PHY_READY  
    return sd, rdy

def sig_det_pam4(lane_obj):
    sd_bit = lane_obj.READ_SIG_DET    
    return sd_bit

def phy_rdy_pam4(lane_obj):
    return lane_obj.RX_READ_PHY_READY  

def tx_pol_flip_pam4(lane_obj):
    val = lane_obj.TX_ANA_OUT_FLIP
    lane_obj.TX_ANA_OUT_FLIP = 1-val

def tx_pol_pam4(lane_obj, val = None):
    if val == None:
        return lane_obj.TX_ANA_OUT_FLIP
    else:
        lane_obj.TX_ANA_OUT_FLIP = val

def rx_pol_pam4(lane_obj, val = None):
    if val == None:
        return lane_obj.RX_DATA_FLIP
    else:
        lane_obj.RX_DATA_FLIP = val

def rx_pol_flip_pam4(lane_obj):
    val = lane_obj.RX_DATA_FLIP   
    lane_obj.RX_DATA_FLIP = 1 - val    

def set_tx_lane(lane_obj, on=1):
    if on == 0:
        lane_obj.TX_TEST_DATA_SRC = 0
        lane_obj.TX_TEST_PAT_3 = 0x0
        lane_obj.TX_TEST_PAT_2 = 0x0
        lane_obj.TX_TEST_PAT_1 = 0x0
        lane_obj.TX_TEST_PAT_0 = 0x0
    else:
        lane_obj.TX_TEST_DATA_SRC = 1

def set_rx_lane(lane_obj, on=1):
    if on == 0:
        lane_obj.NRZ_SM_RESET = 0
    else:
        lane_obj.NRZ_SM_RESET = 0

def latch_data_pam4(lane_obj):
    lane_obj.READOUT_SYNC_EN = 0
    lane_obj.READOUT_CAPTURE = 1
    lane_obj.READOUT_SYNC_EN = 1

def latch_data_release_pam4(lane_obj):
    lane_obj.READOUT_SYNC_EN = 0
    lane_obj.READOUT_CAPTURE = 0
    lane_obj.PAM4_PRBS_CHK_PHASE_EN = 0






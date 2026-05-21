import sys
import time, re, datetime
import math

sys.path.append('../')

from Device.CredoUsbDongle import Mdio_Lib 
from RegisterControl.regmap import load_regmap, load_Regs, saveAllRegsToFile
from BaseFunction.Project.BlueJay_L2_nrz import reload_L2 #L2
chip_name = 'BlueJay'

from BaseFunction.Project.BlueJay_L2_pam4 import load_basefunction_pam4
from Algorithm.Project.BlueJay_L3_pam4 import load_algorithm_pam4
from BaseFunction.Project.BlueJay_L2_nrz import load_basefunction_nrz
from Algorithm.Project.BlueJay_L3_nrz import load_algorithm_nrz
from BlueJay_PAM4_init import PAM4_init

global gLane;                 gLane = range(8)
global gDevice;               gDevice = 0
global gChanEst;              gChanEst = []
global gEncodingMode;         gEncodingMode = []

gEncodingMode.append([]); gEncodingMode[0] = [['pam4', 53.125]] * 8
gEncodingMode.append([]); gEncodingMode[1] = [['pam4', 53.125]] * 8
gEncodingMode.append([]); gEncodingMode[2] = [['nrz', 25.78125]] * 4 + [['pam4', 53.125]]*3 + [['nrz', 25.78125]]
gEncodingMode.append([]); gEncodingMode[3] = [['nrz', 25.78125]] * 2 + [['pam4', 53.125]]*3 + [['nrz', 10.3125]] * 3
gEncodingMode.append([]); gEncodingMode[4] = [['nrz', 25.78125]] * 8
gEncodingMode.append([]); gEncodingMode[5] = [['pam4', 53.125]] * 8
gEncodingMode.append([]); gEncodingMode[6] = [['pam4', 53.125]] * 8
gEncodingMode.append([]); gEncodingMode[7] = [['nrz', 25.78125]] * 8
gEncodingMode.append([]); gEncodingMode[8] = [['nrz', 25.78125]] * 2 + [['nrz', 1.25]] * 1 + [['nrz', 10.3125]] * 5

global TxPeerMap;
TxPeerMap = [];
TxPeerMap.append([]);         TxPeerMap = [0, 1, 2, 3, 4, 5, 6, 7]
global gFecThresh;            gFecThresh=15
global TxPolarityMap;         TxPolarityMap = []
TxPolarityMap.append([]);     TxPolarityMap = [[1, 1, 1, 1, 1, 1, 1, 1], [0, 0, 0, 0, 0, 0, 0, 0], [1, 1, 1, 1, 1, 1, 1, 1], [1, 1, 1, 1, 1, 1, 1, 1], [1, 1, 1, 1, 1, 1, 1, 1], [1, 1, 1, 1, 1, 1, 1, 1], [1, 1, 1, 1, 1, 1, 1, 1], [1, 1, 1, 1, 1, 1, 1, 1], [1, 0, 1, 1, 1, 1, 1, 1]]

global RxPolarityMap;         RxPolarityMap = []
RxPolarityMap.append([]);     RxPolarityMap = [[0, 0, 0, 0, 0, 0, 0, 0], [0, 0, 0, 0, 0, 0, 0, 0], [1, 1, 1, 1, 1, 1, 1, 1], [1, 1, 1, 1, 1, 1, 1, 1], [1, 1, 1, 1, 1, 1, 1, 1], [1, 1, 1, 1, 1, 1, 1, 1], [1, 1, 1, 1, 1, 1, 1, 1], [1, 1, 1, 1, 1, 1, 1, 1], [0, 1, 0, 0, 0, 0, 0, 0]]

gChanEst.append([]);          gChanEst[0] = [[0.0, 0, 0]]*8

lane_name_list = ['A0',   'A1',   'A2',   'A3',  'A4',  'A5',   'A6',   'A7']

lane_mode_list = ['nrz'] * 8

try:
    gLane
except:
    gLane = [0]

global gLaneStats; gLaneStats=[]

gLaneStats.append([]);
gLaneStats = [[0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0]]*8

chip = Mdio_Lib()

time.sleep(0.1)

load_Regs(chip_name, chip, textFilePath=chip_name + '_regs.txt')

load_basefunction_pam4(chip)
load_basefunction_nrz(chip)
load_algorithm_pam4(chip)
load_algorithm_nrz(chip)

#############################################################################################################################################################################################################
def get_lane_list(lane=None):

    if lane == None: lane = gLane
    if type(lane) == int:       lanes = [lane]
    elif type(lane) == list:    lanes = lane
    elif type(lane) == str and lane.upper() == 'ALL':
        lanes = range(0, len(lane_name_list))

    return lanes

############################################################################################################################################################################################################
def serdes_params(group=None, ln=None, ber_en=0):
    chip.setPhyAddr(group)
    line_separator= "\n#+------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+"
    print line_separator,
    print ("\n#|   |     |    |Line | Data  |      TX Taps       |  CHANNEL  |      CTLE     |   |   |         |         | EYE MARGIN  |         |        DFE        | TIMING  |          FFE Taps       |"),
    print ("\n#|Dev|Group|Lane| Enc | Rate  |Ln(-2, -1, M,+1,+2) | Est,OF,HF |Peaking, G1,G2 |SK |DAC|agc_gain1|agc_gain2|  1   2   3  |   BER   | F0 , F1 ,F1/F0,F13|Del,Edge | K1 , K2 , K3 , K4 ,S1,S2|"),
    print line_separator,
    if ln == None:
        lanes = get_lane_list(lane=None)
    else:
        lanes = gLane

    for ln in lanes:
        line_encoding = gEncodingMode[group][ln][0]
        lane_speed = gEncodingMode[group][ln][1]
        if gEncodingMode[group][ln][0].upper() != 'PAM4':
            tt = chip.NRZ25[TxPeerMap[ln]].tx_taps()
        else:
            tt = chip.PAM50[TxPeerMap[ln]].tx_taps()
        tx_peer_marker = '<' if TxPeerMap[ln] == ln else '/'
        chan_est, of, hf = gChanEst[gDevice][ln]
        if gEncodingMode[group][ln][0].upper() != 'PAM4':
            ctle_val = chip.NRZ25[ln].ctle_nrz(); ctle_1 = chip.NRZ25[ln].ctle_map_nrz(ctle_val)[0]; ctle_2 = chip.NRZ25[ln].ctle_map_nrz(ctle_val)[1]
            dac_val = chip.NRZ25[ln].dac_nrz()
            delta_val = chip.NRZ25[ln].delta_ph_nrz()
            eyes = chip.NRZ25[ln].eye_nrz()
            if ber_en == 1:
                BER = chip.NRZ25[ln].ber_nrz()
            else:
                BER = 'NA'
            #BER = 0
            delta_val = chip.NRZ25[0].delta_nrz()
            edge1, edge2, edge3, edge4 = chip.NRZ25[ln].edge()
            f1, f2, f3 = chip.NRZ25[ln].dfe_nrz()
            agc_gain1 = chip.NRZ25[ln].agcgain()[0]
            agc_gain2 = chip.NRZ25[ln].agcgain()[1]
            skef_val = chip.NRZ25[ln].skef()[1]
        else:
            ctle_val = chip.PAM50[ln].ctle_pam4(); ctle_1 = chip.PAM50[ln].ctle_map_pam4(ctle_val)[0]; ctle_2 = chip.PAM50[ln].ctle_map_pam4(ctle_val)[1]
            dac_val = chip.PAM50[ln].dac_pam4()
            #delta_val = chip.PAM50[ln].delta_ph_pam4()
            eyes = chip.PAM50[ln].eye_pam4()
            if ber_en == 1:
                BER = chip.PAM50[ln].ber_pam4()
            else:
                BER = 'NA'
            skef_val = chip.PAM50[ln].skef()[1]
            agc_gain1 = chip.PAM50[ln].agcgain()[0]
            agc_gain2 = chip.PAM50[ln].agcgain()[1]
            delta_val = chip.PAM50[ln].delta_ph_pam4()
            edge1, edge2, edge3, edge4 = chip.PAM50[ln].edge()
            f0, f1, f1f0_ratio = chip.PAM50[ln].get_pam4_dfe()
            f13_val = chip.PAM50[ln].f13()
            [ffe_k1_bin, ffe_k2_bin, ffe_k3_bin, ffe_k4_bin, ffe_s1_bin, ffe_s2_bin] = chip.PAM50[ln].ffe_taps()

        print ("\n#| %d |  %d  | %s" % (gDevice, group, lane_name_list[ln])),
        print ("|%4s |%6.3f" % (line_encoding, lane_speed)),
        print ("|%2s(%2d,%3d,%2d,%2d,%2d)" % (lane_name_list[TxPeerMap[ln]], tt[0], tt[1], tt[2], tt[3], tt[4])),
        print ("|%4.2f,%2d,%2d" % (abs(chan_est), of, hf)),
        print ("| %d   (%d,%d)    " % (ctle_val, ctle_1, ctle_2)),
        print ("| %d |%2d" % (skef_val, dac_val)),
        print ("|   %3d   |   %2d   " % (agc_gain1, agc_gain2)),
        if gEncodingMode[group][ln][0].upper() != 'PAM4':
            print ("|    %4.0f    " % (eyes[1])),
            if ber_en == 1:
                print ("|%8.1e "%(BER)),
            else:
                print ("|   %s   "%(BER)),
            print ("|%4d,%4d,%4d    " % (f1, f2, f3)),
            print ("|%3d,%X%X%X%X" % (delta_val, edge1, edge2, edge3, edge4)),
            print('|                         |'),
        else:
            print ("|%4.0f,%3.0f,%3.0f" % (eyes)),
            if ber_en == 1:
                print ("|%8.1e " % (BER)),
            else:
                print ("|   %s   "%(BER)),
            print ("|%4.2f,%4.2f,%5.2f,%2d" % (abs(f0), abs(f1), abs(f1f0_ratio), f13_val)),
            print ("|%3d,%X%X%X%X" % (delta_val, edge1, edge2, edge3, edge4)),
            print('|%4d,%4d,%4d,%4d,%02X,%02X|' % (ffe_k1_bin, ffe_k2_bin, ffe_k3_bin, ffe_k4_bin, ffe_s1_bin, ffe_s2_bin)),


        if (ln<lanes[-1] and ln==7) or (ln==lanes[-1]):
            print line_separator,

############################################################################################################################################################################################################
def opt_lane_nrz(datarate=None, input_mode='ac',ln=None):
    chip.NRZ25[ln].ctle_map_nrz(0, 1, 3)
    chip.NRZ25[ln].ctle_map_nrz(1, 1, 5)
    chip.NRZ25[ln].ctle_map_nrz(2, 1, 7)
    chip.NRZ25[ln].ctle_map_nrz(3, 2, 7)
    chip.NRZ25[ln].ctle_map_nrz(4, 3, 7)
    chip.NRZ25[ln].ctle_map_nrz(5, 4, 7)
    chip.NRZ25[ln].ctle_map_nrz(6, 5, 7)
    chip.NRZ25[ln].ctle_map_nrz(7, 7, 7)

    chip.NRZ25[ln].dc_gain_fast_search_nrz_full(target=15, which=0)
    chip.NRZ25[ln].sm_cont_nrz()
    time.sleep(0.1)
    chip.NRZ25[ln].Reg0100_0061_8=0
    agcgain_result = chip.NRZ25[ln].agcgain()

    of, hf, chan_est = chip.NRZ25[ln].channel_analyzer_nrz()
    chip.NRZ25[ln].Reg0100_0061_8=1
    chip.NRZ25[ln].tx_taps_table_nrz(chan_est=chan_est)
    gChanEst[gDevice][ln] = [chan_est, of, hf]
    if chan_est == 0.0:
        print ("\nDevice %d Lane %s (NRZ) Channel Analyzer: ChanEst: %6.3f <<< FAILED"%(gDevice, lane_name_list[ln],chan_est)),
    else:
        print ("\nDevice %d Lane %s (NRZ) Channel Analyzer: ChanEst : %6.3f, OF: %2d, HF: %2d"%(gDevice,lane_name_list[ln],chan_est, of, hf)),

    if chan_est == 0.0:
        chip.NRZ25[ln].agcgain(1, 1)
    elif chan_est < 1.20:
        chip.NRZ25[ln].ctle_nrz(7)
        chip.NRZ25[ln].agcgain(1, 28)
    elif chan_est < 1.40:
        chip.NRZ25[ln].ctle_nrz(6)
        chip.NRZ25[ln].agcgain(5, 31)
    elif chan_est < 1.55:
        chip.NRZ25[ln].ctle_nrz(5)
        chip.NRZ25[ln].agcgain(10, 31)
    elif chan_est < 1.80:
        chip.NRZ25[ln].ctle_nrz(4)
        chip.NRZ25[ln].agcgain(15, 31)
    elif chan_est < 2.09:
        chip.NRZ25[ln].ctle_nrz(3)
        chip.NRZ25[ln].agcgain(25, 31)
    elif chan_est < 2.70:
        chip.NRZ25[ln].ctle_nrz(2)
        chip.NRZ25[ln].agcgain(40, 31)
    elif chan_est < 3.50:
        chip.NRZ25[ln].ctle_nrz(1)
        chip.NRZ25[ln].agcgain(60, 31)
        chip.NRZ25[ln].edge(9, 9, 9, 9)
    else:
        chip.NRZ25[ln].ctle_map_nrz(0, 1, 2)
        chip.NRZ25[ln].ctle_nrz(0)
        chip.NRZ25[ln].agcgain(90, 31)
        chip.NRZ25[ln].edge(15, 15, 15, 15)

    if chan_est != 0:
        chip.NRZ25[ln].lr_nrz()
        chip.NRZ25[ln].ctle_search_nrz(t=0.02)
        chip.NRZ25[ln].dc_gain_search_nrz(target_dac_val=14)
        chip.NRZ25[ln].dc_gain_fast_search_nrz_full(target=14, which=1)
        chip.NRZ25[ln].sm_cont_nrz()
        chip.NRZ25[ln].lr_nrz()

    chip.NRZ25[ln].Gettapvalue_nrz()
############################################################################################################################################################################################################
def opt_lane_pam4(datarate=None, input_mode='ac',ln=None):
    chip.PAM50[ln].tx_taps(+5, -14, 21, 0, 0)
    chip.PAM50[ln].dc_gain(22, 31, 8, 1)
    chip.PAM50[ln].delta_ph_pam4(-1)
    chip.PAM50[ln].f13(3)
    chip.PAM50[ln].lr_pam4()
    opt_agc1, opt_agc2 = chip.PAM50[ln].dc_gain_search_pam4(target_dac_val=10)
    of, hf, chan_est = chip.PAM50[ln].channel_analyzer_pam4(gain1_val=opt_agc1, gain2_val=opt_agc2)
    chan_est = chan_est * 1.0
    print 'chan_est_1', chan_est
    if chan_est < 1.6:
        chip.PAM50[ln].tx_taps(+2, -8, 17, 0, 0)
    elif chan_est < 1.71:
        chip.PAM50[ln].tx_taps(+3, -14, 21, 0, 0)
    elif chan_est < 1.75:
        chip.PAM50[ln].tx_taps(+3, -14, 21, 0, 0)
    elif chan_est < 1.78:
        chip.PAM50[ln].tx_taps(+3, -14, 21, 0, 0)
    elif chan_est < 1.9:
        chip.PAM50[ln].tx_taps(+4, -15, 20, 0, 0)
    elif chan_est < 2.0:
        chip.PAM50[ln].tx_taps(+4, -15, 20, 0, 0)
    else:
        chip.PAM50[ln].tx_taps(+4, -17, 19, 0, 0)

    of, hf, chan_est = chip.PAM50[ln].channel_analyzer_pam4(gain1_val=opt_agc1, gain2_val=opt_agc2)
    print 'chan_est', chan_est
    gChanEst[gDevice][ln] = [chan_est, of, hf]
    if chan_est == 0.0:
        print ("\nDevice %d Lane %s (PAM4) Channel Analyzer: ChanEst FAILED <<<"%(gDevice, lane_name_list[ln])),
    else:
        print ("\nDevice %d Lane %s (PAM4) Channel Analyzer: ChanEst: %5.3f, OF: %2d, HF: %2d"%(gDevice, lane_name_list[ln],chan_est, of, hf)),
    print("chan_est is", chan_est)

    if chan_est < 1.80:
        chip.PAM50[ln].ctle_map_pam4(0, 7, 1)
        chip.PAM50[ln].ctle_map_pam4(1, 3, 4)
        chip.PAM50[ln].ctle_map_pam4(2, 3, 5)
        chip.PAM50[ln].ctle_map_pam4(3, 6, 3)
        chip.PAM50[ln].ctle_map_pam4(4, 4, 6)
        chip.PAM50[ln].ctle_map_pam4(5, 7, 5)
        chip.PAM50[ln].ctle_map_pam4(6, 6, 7)
        chip.PAM50[ln].ctle_map_pam4(7, 7, 7)

    else:
        chip.PAM50[ln].ctle_map_pam4(0, 2, 1)
        chip.PAM50[ln].ctle_map_pam4(1, 3, 1)
        chip.PAM50[ln].ctle_map_pam4(2, 2, 2)
        chip.PAM50[ln].ctle_map_pam4(3, 4, 1)
        chip.PAM50[ln].ctle_map_pam4(4, 5, 1)
        chip.PAM50[ln].ctle_map_pam4(5, 6, 1)
        chip.PAM50[ln].ctle_map_pam4(6, 7, 1)
        chip.PAM50[ln].ctle_map_pam4(7, 3, 4)

    if chan_est == 0.0:
        chip.PAM50[ln].agcgain(1, 1)
    elif chan_est < 1.25:
        chip.PAM50[ln].ctle_pam4(7)
        chip.PAM50[ln].delta_ph_pam4(4)
        chip.PAM50[ln].f13(1)
        chip.PAM50[ln].edge(4, 4, 4, 4)
        chip.PAM50[ln].skef(1, 3)
        chip.PAM50[ln].dc_gain(1, 1, 8, 8)
        chip.PAM50[ln].ffe_taps(0x66, 0x11, -0x11, 0x11, 0x01, 0x01)
    elif chan_est < 1.36:
        chip.PAM50[ln].ctle_pam4(7)
        chip.PAM50[ln].delta_ph_pam4(2)
        chip.PAM50[ln].f13(3)
        chip.PAM50[ln].edge(4, 4, 4, 4)
        chip.PAM50[ln].skef(1, 4)
        chip.PAM50[ln].dc_gain(1, 10, 8, 8)
        chip.PAM50[ln].ffe_taps(0x33, 0x11, -0x11, 0x12, 0x01, 0x01)
    elif chan_est < 1.47:
        chip.PAM50[ln].ctle_pam4(6)
        chip.PAM50[ln].delta_ph_pam4(-6)
        chip.PAM50[ln].f13(4)
        chip.PAM50[ln].edge(6, 6, 6, 6)
        chip.PAM50[ln].skef(1, 5)
        chip.PAM50[ln].dc_gain(1, 10, 8, 8)
        chip.PAM50[ln].ffe_taps(0x44, 0x00, 0x00, -0x11, 0x01, 0x01)
    elif chan_est < 1.55:
        chip.PAM50[ln].ctle_pam4(5)
        chip.PAM50[ln].delta_ph_pam4(-6)
        chip.PAM50[ln].f13(4)
        chip.PAM50[ln].edge(7, 7, 7, 7)
        chip.PAM50[ln].skef(1, 5)
        chip.PAM50[ln].dc_gain(1, 25, 8, 8)
        chip.PAM50[ln].ffe_taps(0x77, 0x01, 0x11, -0x55, 0x01, 0x01)
    elif chan_est < 1.59:
        chip.PAM50[ln].ctle_pam4(4)
        chip.PAM50[ln].delta_ph_pam4(-6)
        chip.PAM50[ln].f13(4)
        chip.PAM50[ln].edge(8, 8, 8, 8)
        chip.PAM50[ln].skef(1, 5)
        chip.PAM50[ln].dc_gain(8, 31, 8, 8)
        chip.PAM50[ln].ffe_taps(0x77, 0x01, 0x11, -0x55, 0x01, 0x01)
    elif chan_est < 1.68:
        chip.PAM50[ln].ctle_pam4(3)
        chip.PAM50[ln].delta_ph_pam4(-6)
        chip.PAM50[ln].f13(5)
        chip.PAM50[ln].edge(9, 9, 9, 9)
        chip.PAM50[ln].skef(1, 6)
        chip.PAM50[ln].dc_gain(10, 31, 8, 8)
        chip.PAM50[ln].ffe_taps(0x77, 0x01, 0x11, -0x55, 0x01, 0x01)
    elif chan_est < 1.80:
        chip.PAM50[ln].ctle_pam4(2)
        chip.PAM50[ln].delta_ph_pam4(-8)
        chip.PAM50[ln].f13(5)
        chip.PAM50[ln].edge(10, 10, 10, 10)
        chip.PAM50[ln].skef(1, 6)
        chip.PAM50[ln].dc_gain(30, 31, 8, 8)
        chip.PAM50[ln].ffe_taps(0x77, 0x11, 0x11, -0x55, 0x01, 0x01)

    ################# NEW CTLE TABLE USED ####################### after this condition > 1.80
    elif chan_est < 1.95:
        chip.PAM50[ln].ctle_pam4(6)
        chip.PAM50[ln].delta_ph_pam4(-5)
        chip.PAM50[ln].f13(6)
        chip.PAM50[ln].edge(10,10,10,10)
        chip.PAM50[ln].skef(1,7)
        chip.PAM50[ln].dc_gain(30,31,8,8)
        chip.PAM50[ln].ffe_taps(0x77,0x11,0x11,-0x55,0x04,0x04)
    elif chan_est < 2.05:
        chip.PAM50[ln].ctle_pam4(5,)
        chip.PAM50[ln].delta_ph_pam4(-5)
        chip.PAM50[ln].f13(6)
        chip.PAM50[ln].edge(12, 12, 12, 12)
        chip.PAM50[ln].skef(1, 7)
        chip.PAM50[ln].dc_gain(20, 31, 8, 8)
        chip.PAM50[ln].ffe_taps(0x77, 0x01, 0x11, -0x55, 0x04, 0x04)
    elif chan_est < 2.25:
        chip.PAM50[ln].ctle_pam4(4)
        chip.PAM50[ln].delta_ph_pam4(-6)
        chip.PAM50[ln].f13(6)
        chip.PAM50[ln].edge(11, 11, 11, 11)
        chip.PAM50[ln].skef(1, 7)
        chip.PAM50[ln].dc_gain(25, 31, 8, 2)
        chip.PAM50[ln].ffe_taps(0x77, 0x01, 0x11, -0x55, 0x04, 0x04)
    elif chan_est < 2.45:
        chip.PAM50[ln].ctle_pam4(3)
        chip.PAM50[ln].delta_ph_pam4(-7)
        chip.PAM50[ln].f13(7)
        chip.PAM50[ln].edge(12, 12, 12, 12)
        chip.PAM50[ln].skef(1, 7)
        chip.PAM50[ln].dc_gain(30, 31, 8, 2)
        chip.PAM50[ln].ffe_taps(0x77, 0x01, 0x11, -0x55, 0x04, 0x04)
    elif chan_est < 2.7:
        chip.PAM50[ln].ctle_pam4(2)
        chip.PAM50[ln].delta_ph_pam4(-8)
        chip.PAM50[ln].f13(8)
        chip.PAM50[ln].edge(13, 13, 13, 13)
        chip.PAM50[ln].skef(1, 7)
        chip.PAM50[ln].dc_gain(50, 31, 8, 2)
        chip.PAM50[ln].ffe_taps(0x77, 0x01, 0x11, 0x11, 0x04, 0x04)
    elif chan_est < 3.25:
        chip.PAM50[ln].ctle_pam4(1)
        chip.PAM50[ln].delta_ph_pam4(-14)
        chip.PAM50[ln].f13(9)
        chip.PAM50[ln].edge(13, 13, 13, 13)
        chip.PAM50[ln].skef(1, 7)
        chip.PAM50[ln].dc_gain(70, 31, 15, 1)
        chip.PAM50[ln].ffe_taps(0x11, 0x11, 0x11, 0x11, 0x32, 0x30)
    else:
        chip.PAM50[ln].ctle_pam4(0)
        chip.PAM50[ln].delta_ph_pam4(-14)
        chip.PAM50[ln].f13(9)
        chip.PAM50[ln].edge(13, 13, 13, 13)
        chip.PAM50[ln].skef(1, 7)
        chip.PAM50[ln].dc_gain(90, 31, 15, 1)
        chip.PAM50[ln].ffe_taps(0x11, 0x55, 0x01, 0x11, 0x04, 0x04)

    print ("\Lane %s (PAM4) CTLE selection in EQ1: %d"%(lane_name_list[ln],chip.PAM50[ln].ctle2_pam4()))

    if chan_est != 0.0:
        chip.PAM50[ln].lr_pam4()
        time.sleep(1)
        chip.PAM50[ln].dis_updn()
        chip.PAM50[ln].delta_search(print_en=0)
        chip.PAM50[ln].ctle_fine_search()
        if chan_est > 1.6:
            chip.PAM50[ln].ctle_fine_search()
        chip.PAM50[ln].dc_gain_search_pam4()
        chip.PAM50[ln].lr_pam4()
        time.sleep(1)
        print ("\nDevice %d Lane %s (PAM4) CTLE selection in ctle_search: %d"%(gDevice, lane_name_list[ln],chip.PAM50[ln].ctle2_pam4()))

        chip.PAM50[ln].en_updn()
        time.sleep(.1)
        chip.PAM50[ln].delta_search(print_en=0)
        chip.PAM50[ln].f13_table()
        chip.PAM50[ln].lr_pam4()
        time.sleep(.5)
        i = 0
        list = [1, 1, 1, -1, -1, -1]
        while i < 5:
            if chip.PAM50[ln].eye_check() !=  0:
                i += 1
                chip.PAM50[ln].f13(val=(chip.PAM50[ln].f13()+list[i]))
                chip.PAM50[ln].lane_reset_fast_pam4()
                time.sleep(.3)
            else:
                break

        print("\nDevice %d Lane %s final f13 value: %d"%(gDevice, lane_name_list[ln],chip.PAM50[ln].f13())),
        if chan_est >= 1.47:
            chip.PAM50[ln].ffe_search_a1_orig(print_en=0)

        chip.PAM50[ln].lr_pam4()
        time.sleep(.5)
        chip.PAM50[ln].ffe_adapt()
        chip.PAM50[ln].background_cal(enable='en')
        time.sleep(.5)
    chip.PAM50[ln].Gettapvalue_pam4()

def get_pll_cal(group=None, lane = None):
    chip.setPhyAddr(group)
    chip.PAM50[lane].pll_cal(lane_name=lane)

def sw_opt_lane(group=None, datarate=None, input_mode='ac',ln=None):
    if group != None:
        chip.setPhyAddr(group)
    if gEncodingMode[group][ln][0].upper() != 'PAM4':
        opt_lane_nrz(ln=ln)
    else:
        opt_lane_pam4(ln=ln)

def Tx_Taps(group=None,ln=None,tap1=None,tap2=None,tap3=None,tap4=None,tap5=None,tap6=None,tap7=None,tap8=None,tap9=None,tap10=None,tap11=None):
    if group != None:
        chip.setPhyAddr(group)
    if gEncodingMode[group][ln][0].upper() != 'PAM4':
        chip.NRZ25[ln].tx_taps(tap1=tap1, tap2=tap2, tap3=tap3, tap4=tap4, tap5=tap5, tap6=tap6, tap7=tap7, tap8=tap8, tap9=tap9, tap10=tap10, tap11=tap11)
    else:
        chip.PAM50[ln].tx_taps(tap1=tap1, tap2=tap2, tap3=tap3, tap4=tap4, tap5=tap5, tap6=tap6, tap7=tap7, tap8=tap8, tap9=tap9, tap10=tap10, tap11=tap11)

def save_setup(filename=None, group=None, lane=None):
    if group != None:
        chip.setPhyAddr(group)
    global gSetupFileName
    global gChipName
    gChipName = 'BlueJay'
    if lane == None:
        lanes = range(0, len(lane_name_list))  # Save all lanes' settings by default
    else:
        lanes = get_lane_list(lane)

    try:
        f = open(filename, 'r')
        script = f.read()
        f.close()
        print ("\n*** A file with the same name already exists. Choose another file name!  <<<<"),
        return
    except IOError:
        print (" ")

    if len(lanes) == 1:
        lanes_str = '_Device%d_Lane%d_%s_' % (0, lanes[0], gEncodingMode[group][lanes[0]][0])
    else:
        lanes_str = '_Device%d_Lanes%d-%d_%s_' % (0, lanes[0], lanes[-1], gEncodingMode[group][lanes[0]][0])

    timestr = time.strftime("%Y%m%d_%H%M%S")
    if filename == None:
        filename = gChipName + lanes_str + timestr + '.txt'
    gSetupFileName = filename
    log = open(filename, 'w')
    log.write('\n#---------------------------------------------'),
    log.write('\n# File: %s' % filename),
    log.write("\n# %s Registers" % (gChipName)),
    log.write("\n# %s" % time.asctime())
    log.write('\n#---------------------------------------------\n'),
    log.close()

    ##### Save FW SERDES PARAMS
    log_file = open(filename, 'a+')
    temp_stdout = sys.stdout
    sys.stdout = log_file
    if fw_loaded():
        fw_serdes_params(group=group)
    else:
        serdes_params(group=group, ln=lane)
    sys.stdout = temp_stdout
    log_file.close()

    if lanes == range(0, len(lane_name_list)):
        reg_group_dump(0x4800, range(0x00, 0x59, 1), 'TOP Registers', filename)
        reg_group_dump(0x4D00, range(0x00, 0x16, 1), 'TOP PLL Registers', filename)
    for lane in lanes:
        reg_group_dump(0x0000 + 0x800 * lane, range(0x000, 0x1FF + 1, 1), 'Per Lane Register', filename)
        reg_group_dump(0x0000 + 0x800 * lane, range(0x0C0, 0x0FF + 1, 1), 'ANA_Reg', filename)
        reg_group_dump(0x0500 + 0x800 * lane, range(0x000, 0x00B + 1, 1), 'Aneg_lt Registers', filename)
        reg_group_dump(0x01C0 + 0x800 * lane, range(0x000, 0x00D + 1, 1), 'FEC Analyzer Registers', filename)
        reg_group_dump(0x0200 + 0x800 * lane, range(0x000, 0x00C + 1, 1), 'LANE_SLICE', filename)
        reg_group_dump(0x0400 + 0x800 * lane, range(0x000, 0x06C + 1, 1), 'Training Registers', filename)
        reg_group_dump(0x4000, range(0x000, 0x0EF, 1), 'GROUP8 Registers', filename)
        reg_group_dump(0x4B00, range(0x03F, 0x0FF, 1), 'TSensor, VSensor Registers', filename)

    ##### Save FW Register values
    log_file = open(filename, 'a+')
    temp_stdout = sys.stdout
    sys.stdout = log_file
    fw_reg()
    sys.stdout = temp_stdout
    log_file.close()

    lanes_str = 'Group%d, Lanes %d-%d,' % (group, lanes[0], lanes[-1])
    print ("...Saved Slice %s Registers to Setup File: %s" % (lanes_str, filename))

    ##############################################################################
    # Similar to the function in main script
    # changed the order of addresses saved to be sequential from 0x7000 to 0x8FFF
    ##############################################################################
def reg_group_dump(base_addr, addr_range, addr_name, filename):
    log = open(filename, "a+")
    log.write('\n\n#---------------------------------------------')
    log.write('\n#%s (R%04X to R%04X)' % (addr_name, base_addr + addr_range[0], base_addr + addr_range[-1]))
    log.write('\n#Addr Value')
    log.write('\n#---------------------------------------------\n'),

    for i in addr_range:
        for j in range(1):
            addr = base_addr + i
            val = chip.MdioRd(addr)
            if addr == 0x9501 or addr == 0x9601:
                val_01 = val
                val = val & 0xfffb
            if addr == 0x9512 or addr == 0x9612:
                val_12 = val
                val = val & 0x7fff
            log.write("%04X %04X\n" % (addr, val))
    if base_addr == 0x9500 or base_addr == 0x9600:
        log.write("%04X %04X # TOP PLL POR TX [15] toggle\n" % (base_addr + 0x12, val_12))
    if base_addr == 0x9500 or base_addr == 0x9600:
        log.write("%04X %04X # TOP PLL PU [2] toggle\n" % (base_addr + 0x01, val_01))
    log.write("\n")
    log.close()


def save_setup_debug_mode(group=None, filename=None):
    '''
    saves the same address values for all lanes in one row, for quick comparison and debugging.
    '''
    if group != None:
        chip.setPhyAddr(group)
    global gChipName
    gChipName = 'BlueJay'
    timestr = time.strftime("%Y%m%d_%H%M%S")
    if filename == None:
        filename = gChipName + '_Reg_Setup_Debug_Mode' + timestr + '.txt'
    log = open(filename, 'w')
    log.write('\n----------------------------------------------------------------------'),
    log.write('\n File: %s' % filename),
    log.write('\n %s Rev %2.1f' % (gChipName, 1)),
    log.write("\n %s" % time.asctime())
    log.write('\n----------------------------------------------------------------------\n\n'),
    log.close()

    reg_group_dump_debug_mode(range(0x0000, 0x3800 + 1, 0x800), range(0, 0x1FF + 1, 1), "SerDes Lane Registers", filename)
    reg_group_dump_debug_mode(range(0x0500, 0x3D00 + 1, 0x800), range(0, 0x00F + 1, 1), "Link Training Registers", filename)
    reg_group_dump_debug_mode(range(0x4D00, 0x4D00 + 1, 0x100), range(0, 0x003 + 1, 1), "Top PLL Registers", filename)
    ##### Save FW Serdes Params and FW Regiters
    log_file = open(filename, 'a+')
    temp_stdout = sys.stdout
    sys.stdout = log_file
    if fw_loaded():
        fw_serdes_params(group=group)
    else:
        serdes_params(group=group, ln=range(8))  #####
    fw_reg()  #####
    sys.stdout = temp_stdout
    log_file.close()

    print ("\n...Saved Registers as Debug Setup File: %s" % filename)

def reg_group_dump_debug_mode(base_addr_range, lane_addr_range, addr_name, filename=None):
    if filename != None:
        log_file = open(filename, 'a+')
        temp_stdout = sys.stdout
        sys.stdout = log_file

    separator = '\n-----'
    for lane_base_addr in base_addr_range:
        separator += '------'

    ### header
    print('\n %s   (Addr: 0x%04X to 0x%04X)' % (
    addr_name, base_addr_range[0], base_addr_range[-1] + lane_addr_range[-1])),
    print('%s' % separator),
    print("\n     "),
    for lane in lane_name_list:
        print("%4s " % (lane)),
    print("\nAddr:"),
    for lane_base_addr in base_addr_range:
        print("%04X " % (lane_base_addr)),
    print('%s' % separator),

    ### Data
    for pre_lane_addr in lane_addr_range:
        print("\n %03X:" % (pre_lane_addr)),
        prev_val = 0xeeeee
        for base_addr in base_addr_range:
            val = chip.MdioRd(base_addr + pre_lane_addr)
            diff = '<' if (base_addr != base_addr_range[0] and val != prev_val) else ' '
            print("%04X%s" % (val, diff)),
            prev_val = val

    print('%s' % separator),
    print("\n"),

    if filename != None:
        sys.stdout = temp_stdout
        log_file.close()

def hard_reset():
    print "To do a hardware reset here"

def soft_reset():
    chip.GROUP8_TOP[0].Reg0013_11_0 = 0x888
    chip.GROUP8_TOP[0].Reg0013_11_0 = 0x0

def logic_reset():
    chip.GROUP8_TOP[0].Reg0013_11_0 = 0x777
    chip.GROUP8_TOP[0].Reg0013_11_0 = 0x0

def group_reset(lane=None,t=0.1):
    soft_reset()
    time.sleep(t)

    logic_reset()
    time.sleep(t)

    #print"lane = ", lane
    chip.ANLT_TOP[lane].Tr_Reg0000_1 = 0
    chip.ANLT_TOP[lane].Tr_Reg0000_0 = 0

def per_lane_reset(group=None, lane=None, t=0.1):
    if group != None:
        chip.setPhyAddr(group)
    if lane == None:
        lanes = range(8)
    elif type(lane) == int:
        lanes = [lane]
    elif type(lane) == list:
        lanes = lane
    for ln in lanes:
        wregBits(0x40E0, [0], 1)
        time.sleep(t)
        wregBits(0x400B, range(8)[::-1], 1 << ln)
        time.sleep(t)
        wregBits(0x400B, range(8)[::-1], 0 << ln)

def sw_init_lane_pam4():
    for ln in range(8):
        PAM4_init(chip, lane=ln)

def init_lane_for_fw(input_mode='ac', lane=None, group = None, TX_pat=3, RX_pat=3):
    if group != None:
        chip.setPhyAddr(group)
    set_lane_mode(group=group, lane=lane)
    ####################### put lane in PAM4 mode
    if (gEncodingMode[group][lane][0].upper() == 'PAM4'):
        chip.PAM50[lane].TX_POST2_SCALE = 0
        chip.PAM50[lane].TX_POST1_SCALE = 0
        chip.PAM50[lane].TX_MAIN_SCALE = 1
        chip.PAM50[lane].TX_PRE1_SCALE = 0
        chip.PAM50[lane].TX_PRE2_SCALE = 0

        chip.PAM50[lane].tx_taps(2, -8, 17, 0, 0)

        chip.PAM50[lane].gc(1, 1)
        chip.PAM50[lane].pc(0, 0)
        chip.PAM50[lane].msblsb(0, 0)

        prbs_mode_select(group=group, lane=lane, tx_prbs_mode='prbs', rx_prbs_mode='prbs', tx_pat=TX_pat, rx_pat=RX_pat)

    ####################### put lane in NRZ mode
    if (gEncodingMode[group][lane][0].upper() == 'NRZ'):
        #print 'enter NRZ init'
        chip.PAM50[lane].TX_POST2_SCALE = 0
        chip.PAM50[lane].TX_POST1_SCALE = 0
        chip.PAM50[lane].TX_MAIN_SCALE = 1
        chip.PAM50[lane].TX_PRE1_SCALE = 0
        chip.PAM50[lane].TX_PRE2_SCALE = 0

        chip.NRZ25[lane].tx_taps(0, -8, 17, 0, 0)

        chip.PAM50[lane].gc(0, 0)
        chip.PAM50[lane].pc(0, 0)
        chip.PAM50[lane].msblsb(0, 0)

        prbs_mode_select(group=group, lane=lane, tx_prbs_mode='prbs', rx_prbs_mode='prbs', tx_pat=TX_pat, rx_pat=RX_pat)

    chip.PAM50[lane].Reg007B_3 = 1
    chip.NRZ25[lane].Reg0100_007C_15 = 0

    if gEncodingMode[group][lane][0].upper() == 'NRZ':
        chip.NRZ25[lane].RX_AC_COUPLE_EN = 1
        chip.NRZ25[lane].tx_pol_nrz(val=TxPolarityMap[group][lane])
        chip.NRZ25[lane].rx_pol_nrz(val=RxPolarityMap[group][lane])
    elif gEncodingMode[group][lane][0].upper() == 'PAM4':
        chip.PAM50[lane].RX_AC_COUPLE_EN = 1
        chip.PAM50[lane].tx_pol_pam4(val=TxPolarityMap[group][lane])
        chip.PAM50[lane].rx_pol_pam4(val=RxPolarityMap[group][lane])

def prbs_mode_select(group=None, lane=None, tx_prbs_mode='prbs', rx_prbs_mode='prbs', tx_pat=None, rx_pat=None):
    if group != None:
        chip.setPhyAddr(group)
    if gEncodingMode[group][lane][0].upper() == 'NRZ':
        if tx_prbs_mode == 'prbs':
            chip.NRZ25[lane].tx_prbs_en_nrz(en=1)
            chip.NRZ25[lane].tx_prbs_mode_nrz(pat=tx_pat)
        elif tx_prbs_mode == 'functional':
            chip.NRZ25[lane].tx_prbs_en_nrz(en=0)
            chip.NRZ25[lane].tx_prbs_mode_nrz(pat=None)
        if rx_prbs_mode == 'prbs':
            chip.NRZ25[lane].rx_prbs_mode_nrz(pat=rx_pat)
        elif rx_prbs_mode == 'functional':
            chip.NRZ25[lane].rx_prbs_mode_nrz(pat=None)
            chip.NRZ25[lane].RX_PRBS_CHECK_EN = 0
    else:
        if tx_prbs_mode == 'prbs':
            chip.PAM50[lane].tx_prbs_en_pam4(en=1)
            chip.PAM50[lane].tx_prbs_mode_pam4(pat=tx_pat)
        elif tx_prbs_mode == 'functional':
            chip.PAM50[lane].tx_prbs_en_pam4(en=0)
            chip.PAM50[lane].tx_prbs_mode_pam4(pat=None)
        if rx_prbs_mode == 'prbs':
            chip.PAM50[lane].rx_prbs_mode_pam4(pat=rx_pat)
        elif rx_prbs_mode == 'functional':
            chip.PAM50[lane].rx_prbs_mode_pam4(pat=None)

def set_lane_mode(group=None, lane=None):
    if group != None:
        chip.setPhyAddr(group)
    if gEncodingMode[group][lane][0].upper() !='PAM4':
        #print 'enter set lane nrz'
        chip.PAM50[lane].PAM4_EN = 0
        chip.PAM50[lane].TX_PRBS_CLK_EN = 0
        chip.NRZ25[lane].TX_NRZ_MODE = 1
        chip.NRZ25[lane].TX_NRZ_PRBS_GEN_EN = 1
    else:
        chip.NRZ25[lane].TX_NRZ_MODE = 0
        chip.NRZ25[lane].TX_NRZ_PRBS_GEN_EN = 0
        chip.PAM50[lane].PAM4_EN = 1
        chip.PAM50[lane].TX_PRBS_CLK_EN = 1

def read_plus_minus_margin_nrz(lane=None):
    lanes = get_lane_list(lane)
    result = {}
    for lane in lanes:
        plus_0 = chip.NRZ25[lane].Reg0100_001A_15_4
        plus_1 = chip.NRZ25[lane].Reg0100_001A_3_0S
        plus_2 = chip.NRZ25[lane].Reg0100_001B_7_0S
        plus_3 = chip.NRZ25[lane].Reg0100_001C_11_0
        plus_4 = chip.NRZ25[lane].Reg0100_001D_15_4
        plus_5 = chip.NRZ25[lane].Reg0100_001D_3_0S
        plus_6 = chip.NRZ25[lane].Reg0100_001E_7_0S
        plus_7 = chip.NRZ25[lane].Reg0100_001F_11_0
        if plus_0 > 2047: plus_0 = plus_0 - 4096
        if plus_1 > 2047: plus_1 = plus_1 - 4096
        if plus_2 > 2047: plus_2 = plus_2 - 4096
        if plus_3 > 2047: plus_3 = plus_3 - 4096
        if plus_4 > 2047: plus_4 = plus_4 - 4096
        if plus_5 > 2047: plus_5 = plus_5 - 4096
        if plus_6 > 2047: plus_6 = plus_6 - 4096
        if plus_7 > 2047: plus_7 = plus_7 - 4096
        plus_margin = [plus_0, plus_1, plus_2, plus_3, plus_4, plus_5, plus_6, plus_7]
        minus_0 = chip.NRZ25[lane].Reg0100_0020_15_4
        minus_1 = chip.NRZ25[lane].Reg0100_0020_3_0S
        minus_2 = chip.NRZ25[lane].Reg0100_0021_7_0S
        minus_3 = chip.NRZ25[lane].Reg0100_0022_11_0
        minus_4 = chip.NRZ25[lane].Reg0100_0023_15_4
        minus_5 = chip.NRZ25[lane].Reg0100_0023_3_0S
        minus_6 = chip.NRZ25[lane].Reg0100_0024_7_0S
        minus_7 = chip.NRZ25[lane].Reg0100_0025_11_0
        if minus_0 > 2047: minus_0 = minus_0 - 4096
        if minus_1 > 2047: minus_1 = minus_1 - 4096
        if minus_2 > 2047: minus_2 = minus_2 - 4096
        if minus_3 > 2047: minus_3 = minus_3 - 4096
        if minus_4 > 2047: minus_4 = minus_4 - 4096
        if minus_5 > 2047: minus_5 = minus_5 - 4096
        if minus_6 > 2047: minus_6 = minus_6 - 4096
        if minus_7 > 2047: minus_7 = minus_7 - 4096
        minus_margin = [minus_0, minus_1, minus_2, minus_3, minus_4, minus_5, minus_6, minus_7]
        result[lane] = plus_margin, minus_margin
    else:
        if result != {}: return result

####################################################################################################
# Read On-Chip Temperature Sensor
#
####################################################################################################
def temp_sensor(auto=0):
    chip.setPhyAddr(9)
    temp_sensor_start(auto)
    value1 = temp_sensor_read(auto)
    print('Slice TempSensor: %3.1f C' % (value1)),

####################################################################################################
# Set up On-Chip Temperature Sensor (to be read later)
#
####################################################################################################
def temp_sensor_start(auto=0):
    base = 0x4B00
    chip.MdioWr(0x4d00, 0x5d81)
    if(auto):
        chip.MdioWr(base+0x3a, 0x3f)
    else:
        chip.MdioWr(base+0x3a, 0x7)
    chip.MdioWr(base + 0x3e, 0x0054)  # set clock
    time.sleep(1)
    chip.MdioWr(base + 0x37, 0x0)  # reset sensor
    time.sleep(1)

####################################################################################################
# Read back On-Chip Temperature Sensor (after it's been set up aready)
#
####################################################################################################
def temp_sensor_read(auto=0):
    base = 0x4B00
    Yds = 237.7
    Kds = 79.925
    time1 = time.time()
    time2 = time.time()
    if auto == 1:
        rdy = 0
        while (rdy == 0):  # wait for rdy
            value = chip.MdioRd(0x4859)
            rdy = value >> 12
            time2 = time.time()
            if (time2-time1) >= 5:
                print 'AutoReadTsensor test timeout2...'
        realVal = (value&0x0fff) * Yds / 4096 - Kds
        print('tempsensor%d: %d,realVal:%f' % (0, value, realVal))
    else:
        addr = [base + 0x39, base + 0x3a, base + 0x3b, base + 0x3c]
        chip.MdioWr(base + 0x37, 0xc)  # set no ack
        rdy = chip.MdioRd(base + 0x38) >> 8
        while (rdy == 0):  # wait for rdy
            rdy = chip.MdioRd(base + 0x38) >> 8
            time2 = time.time()
            if (time2 - time1) >= 5:
                print 'ReadTSensor test timeout.2..'
                break
        value = chip.MdioRd(addr[0])
        realVal = value * Yds / 4096 - Kds
        print('tempsensor%d: %d,realVal:%f' % (0, value, realVal))
    return realVal

def wreg(addr, val, lane = None):

    #write to a register address, register offset or register field

    #global chip
    if lane == None:
        lane = gLane[0]
    if type(addr) == int:
        if (addr & 0xf000) == 0:
            addr += 0x800 * lane
        elif (addr & 0xff00) == 0x4100:
            addr += 0x20 * lane
        chip.MdioWr(addr, val)
    elif type(addr) == list:
        addr_1 = addr[0]
        if (addr_1 & 0xf000) == 0:
            addr_1 += 0x800 * lane
        elif (addr_1 & 0xff00) == 0x4100:
            addr_1 += 0x20 * lane
        val_old = chip.MdioRd(addr_1)
        mask = sum([1 << bit for bit in range(addr[1][0], addr[1][-1]-1, -1)])
        val_new = (val_old & ~mask) + (val << addr[1][-1] & mask)
        chip.MdioWr(addr_1, val_new)
    else:
        print("\n***Error writing register***")

def wregBits(addr, bits, val, lane = None):
    '''
    write to a register field
    '''

    addr = [addr, bits]
    wreg(addr, val, lane)

def rreg(addr, lane=None):

    #read from a register address, register offset or register field

    global chip
    if lane == None:
        lane = gLane[0]
    if type(addr) == int:
        if (addr & 0xf000) == 0:
            addr += 0x800 * lane
        elif (addr & 0xff00) == 0x4100:
            addr += 0x20 * lane
        return hex(chip.MdioRd(addr))
    elif type(addr) == list:
        val = 0
        i = 0
        while (i < (len(addr) - 1)):
            addr_1 = addr[i]
            if (addr[i] & 0xf000) == 0:
                addr_1 += 0x800 * lane
            elif (addr[i] & 0xff00) == 0x4100:
                addr_1 += 0x20 * lane
            val_tmp = chip.MdioRd(addr_1)
            i += 1
            mask = sum([1 << bit for bit in range(addr[i][0], addr[i][-1] - 1, -1)])
            val_tmp = (val_tmp & mask) >> addr[i][-1]
            val = (val << (addr[i][0] - addr[i][-1] + 1)) + val_tmp
            val = hex(val)
            i += 1
        return val
    else:
        print("\n***Error reading register***")
        return -1

def rregBits(addr, bits, lane = None):
    '''
    read from a register field
    '''
    if lane == None: lane = gLane[0]
    addr = [addr, bits]
    return rreg(addr, lane)

def reg(addr, val=None, lane=None, group=None):
    if (group != None):
        chip.setPhyAddr(group)
    if (lane == None):
        lane_list = range(8)
    elif (type(lane) == int):
        lane_list = [lane]
    elif (type(lane) == list):
        lane_list = lane

    if (type(addr) == int):
        addr_list = [addr]
    elif (type(addr) == list):
        addr_list = addr
    # chip.MdioWr(0x40C0, 0xFFF0)
    if (val != None):
        for addr in addr_list:
            for lane in lane_list:
                top_addr = 0x800 * lane + addr
                chip.MdioWr(top_addr, val)
    print ("LANE NUM   : "),
    for lane in lane_list:
        print (" %2d " % (lane)),
    for addr in addr_list:
        print ("\nADDR(%4X) : " % (addr)),
        for lane in lane_list:
            top_addr = 0x800 * lane + addr
            val = chip.MdioRd(top_addr)
            print("%4X" % (val)),

def rx_tx_serial_loopback(group=None, lane=0, enable=1, TX_pat=3, RX_pat=3):
    if group != None:
        chip.setPhyAddr(group)
    if gEncodingMode[group][lane][0].upper() == 'PAM4':
        # In PAM4 mode we need to use divide by 2 for TX PLL
        if enable == 1:
            chip.PAM50[lane].TX_PLL_N = 42
            chip.PAM50[lane].Reg00FF_1 = 0
            chip.PAM50[lane].RX_REFCLK_DIV4_EN = 0
            chip.PAM50[lane].Reg00D7_13 = 1
            chip.PAM50[lane].Reg00D7_15_14 = 2
            chip.PAM50[lane].Reg00D9_3_0S = 0x80000
        else:
            chip.PAM50[lane].TX_PLL_N = 85
            chip.PAM50[lane].Reg00FF_1 = 1
            chip.PAM50[lane].RX_REFCLK_DIV4_EN = 0
            chip.PAM50[lane].Reg00D7_13 = 0
            chip.PAM50[lane].Reg00D7_15_14 = 0
            chip.PAM50[lane].Reg00D9_3_0S = 0
    # Disable Link Training of the lane
    chip.MdioWr(0x400 + 0x800 * lane, 0x0000)
    # Disable AutoNeg of the lane
    chip.MdioWr(0x600 + 0x800 * lane, 0xC000)
    if gEncodingMode[group][lane][0].upper() == 'PAM4':
        if enable == 0:
            chip.PAM50[lane].TX_PI_EN = enable
        chip.PAM50[lane].PH_ROTR_OW = 0x0
        chip.PAM50[lane].PH_ROTR_OWEN = enable
        chip.PAM50[lane].TX_PH_ROTR_FLIP = 0
    else:
        if enable == 0:
            chip.NRZ25[lane].TX_PI_EN = enable
        chip.NRZ25[lane].PH_ROTR_OW = 0x0
        chip.NRZ25[lane].PH_ROTR_OWEN = enable
        chip.NRZ25[lane].TX_PH_ROTR_FLIP = 0

    if enable == 1:
        prbs_mode_select(group=group, lane=lane, tx_prbs_mode='functional', rx_prbs_mode='prbs', tx_pat=None, rx_pat=None)
    else:
        prbs_mode_select(group=group, lane=lane, tx_prbs_mode='prbs', rx_prbs_mode='prbs', tx_pat=TX_pat, rx_pat=RX_pat)

    chip.LANE_TOP[lane].RX_TO_TX_LPBK = enable
    if gEncodingMode[group][lane][0] == 'pam4':
        if enable == 1:
        # Reg0079
            chip.PAM50[lane].Reg0079_14 = 0
            chip.PAM50[lane].Reg0079_13 = 1
            chip.PAM50[lane].LOCK_PH_FLIP = 1
            chip.PAM50[lane].SPD_SEL2 = 0
            chip.PAM50[lane].SPD_SEL1 = 0
            chip.PAM50[lane].FREQ_MUP = 0x2
            chip.PAM50[lane].FREQ_MULTIPLIER = 0x0
            chip.PAM50[lane].FREQ_SHIFT_SEL = 0x4
        else:
            chip.PAM50[lane].Reg0079_14 = 0
            chip.PAM50[lane].Reg0079_13 = 0
            chip.PAM50[lane].LOCK_PH_FLIP = 0
            chip.PAM50[lane].SPD_SEL2 = 0
            chip.PAM50[lane].SPD_SEL1 = 0
            chip.PAM50[lane].FREQ_MUP = 0x5
            chip.PAM50[lane].FREQ_MULTIPLIER = 0x0
            chip.PAM50[lane].FREQ_SHIFT_SEL = 0x4
    else:
        if enable == 1:
            chip.NRZ25[lane].Reg0100_007A_12 = 1
            chip.NRZ25[lane].Reg0100_007A_11_10 = 0
            chip.NRZ25[lane].Reg0100_007A_9_8 = 0
            chip.NRZ25[lane].Reg0100_007A_7_5 = 0x4
            chip.NRZ25[lane].Reg0100_007A_4_3 = 0x0
            chip.NRZ25[lane].Reg0100_007A_2_0 = 0x4

        else:
            chip.NRZ25[lane].Reg0100_007A_12 = 0
            chip.NRZ25[lane].Reg0100_007A_11_10 = 0
            chip.NRZ25[lane].Reg0100_007A_9_8 = 0
            chip.NRZ25[lane].Reg0100_007A_7_5 = 0x5
            chip.NRZ25[lane].Reg0100_007A_4_3 = 0x0
            chip.NRZ25[lane].Reg0100_007A_2_0 = 0x4

    if enable == 1:
        chip.NRZ25[lane].TX_PI_EN = enable

def rx_tx_external_normal_loopback(group=None, lane=0, enable=1, TX_pat=3, RX_pat=3):
    if group != None:
        chip.setPhyAddr(group)
    if gEncodingMode[group][lane][0].upper() == 'PAM4':
        # In PAM4 mode we need to use divide by 2 for TX PLL
        if enable == 1:
            chip.PAM50[lane].TX_PLL_N = 42
            chip.PAM50[lane].Reg00FF_1 = 0
            chip.PAM50[lane].RX_REFCLK_DIV4_EN = 0
            chip.PAM50[lane].Reg00D7_13 = 1
            chip.PAM50[lane].Reg00D7_15_14 = 2
            chip.PAM50[lane].Reg00D9_3_0S = 0x80000
        else:
            chip.PAM50[lane].TX_PLL_N = 85
            chip.PAM50[lane].Reg00FF_1 = 1
            chip.PAM50[lane].RX_REFCLK_DIV4_EN = 0
            chip.PAM50[lane].Reg00D7_13 = 0
            chip.PAM50[lane].Reg00D7_15_14 = 0
            chip.PAM50[lane].Reg00D9_3_0S = 0
    # Disable Link Training of the lane
    chip.MdioWr(0x400 + 0x800 * lane, 0x0000)
    # Disable AutoNeg of the lane
    chip.MdioWr(0x600 + 0x800 * lane, 0xC000)
    if gEncodingMode[group][lane][0].upper() == 'PAM4':
        if enable == 0:
            chip.PAM50[lane].TX_PI_EN = enable   #TX PLL refclk source
        chip.PAM50[lane].PH_ROTR_OW = 0x0        #[15]->1, TX phase rotator value
        chip.PAM50[lane].PH_ROTR_OWEN = enable   #[15]->1, RX phase rotator enable
        chip.PAM50[lane].TX_PH_ROTR_FLIP = 0     #TRF pol
    else:
        if enable == 0:
            chip.NRZ25[lane].TX_PI_EN = enable
        chip.NRZ25[lane].PH_ROTR_OW = 0x0
        chip.NRZ25[lane].PH_ROTR_OWEN = enable
        chip.NRZ25[lane].TX_PH_ROTR_FLIP = 0

    if enable == 1:
        prbs_mode_select(group=group, lane=lane, tx_prbs_mode='functional', rx_prbs_mode='prbs', tx_pat=None, rx_pat=None)
    else:
        prbs_mode_select(group=group, lane=lane, tx_prbs_mode='prbs', rx_prbs_mode='prbs', tx_pat=TX_pat, rx_pat=RX_pat)

    if gEncodingMode[group][lane][0] == 'pam4':
        if enable == 1:
            # Reg0079 PAM4 TRF
            chip.PAM50[lane].Reg0079_14 = 0
            chip.PAM50[lane].Reg0079_13 = 1
            chip.PAM50[lane].LOCK_PH_FLIP = 1
            chip.PAM50[lane].SPD_SEL2 = 0
            chip.PAM50[lane].SPD_SEL1 = 0
            chip.PAM50[lane].FREQ_MUP = 0x2
            chip.PAM50[lane].FREQ_MULTIPLIER = 0x0
            chip.PAM50[lane].FREQ_SHIFT_SEL = 0x4
        else:
            chip.PAM50[lane].Reg0079_14 = 0
            chip.PAM50[lane].Reg0079_13 = 0
            chip.PAM50[lane].LOCK_PH_FLIP = 0
            chip.PAM50[lane].SPD_SEL2 = 0
            chip.PAM50[lane].SPD_SEL1 = 0
            chip.PAM50[lane].FREQ_MUP = 0x5
            chip.PAM50[lane].FREQ_MULTIPLIER = 0x0
            chip.PAM50[lane].FREQ_SHIFT_SEL = 0x4
    else:
        if enable == 1:
            chip.NRZ25[lane].Reg0100_007A_12 = 1
            chip.NRZ25[lane].Reg0100_007A_11_10 = 0
            chip.NRZ25[lane].Reg0100_007A_9_8 = 0
            chip.NRZ25[lane].Reg0100_007A_7_5 = 0x4
            chip.NRZ25[lane].Reg0100_007A_4_3 = 0x0
            chip.NRZ25[lane].Reg0100_007A_2_0 = 0x4

        else:
            chip.NRZ25[lane].Reg0100_007A_12 = 0
            chip.NRZ25[lane].Reg0100_007A_11_10 = 0
            chip.NRZ25[lane].Reg0100_007A_9_8 = 0
            chip.NRZ25[lane].Reg0100_007A_7_5 = 0x5
            chip.NRZ25[lane].Reg0100_007A_4_3 = 0x0
            chip.NRZ25[lane].Reg0100_007A_2_0 = 0x4

    if enable == 1:
        chip.NRZ25[lane].TX_PI_EN = enable   #TX PLL refclk source 0: crystal 1:RX_PLL recovered clock

#######################################################################################################################
# if you want to enable tx_rx loopback, you need to connect another lane's rx to test lane's tx as a termination      #
#######################################################################################################################
def tx_rx_serial_loopback(group=None, lane=0, enable=1):
    if group != None:
        chip.setPhyAddr(group)
    if gEncodingMode[group][lane][0].upper() == 'NRZ':
        if enable == 1:
            chip.NRZ25[lane].tx_taps(0, 0, 15, -4, 0)
            chip.NRZ25[lane].Reg01E7_13 = 1
            chip.NRZ25[lane].Reg00FF_3 = 1
            chip.NRZ25[lane].Reg0100_0001_14 = 0
            time.sleep(3)
            fw_reg(addr=0x8, data=0xffff-2**lane) # inactive FW
            chip.MdioWr(0x10b + 0x800 * lane, 0x0)
            chip.NRZ25[lane].NRZ_SM_CONT = 0
            chip.NRZ25[lane].NRZ_SM_CONT = 1
            chip.NRZ25[lane].agcgain(0, 0)
            chip.NRZ25[lane].ctle_nrz(7)
            time.sleep(0.1)
            chip.NRZ25[lane].lane_reset()

        else:
            ########################################################################################################################
            # After you enable the loopback, if you want to active the FW, you need to set the enable=0 to active the FW
            ########################################################################################################################
            chip.NRZ25[lane].Reg01E7_13 = 0
            chip.NRZ25[lane].Reg00FF_3 = 0
            chip.NRZ25[lane].Reg0100_0001_14 = 1
            chip.GROUP8_TOP[0].fw_reg(chip, addr=0x8, data=0xffff) # active FW
            chip.NRZ25[lane].tx_taps(0, -8, 17, 0, 0)
    else:
        print "\n>>>> TX-to-RX Serial Loopback feature is available in NRZ mode Only!\n"

def rx_tx_external_dft_loopback(group=None, lane=None, t=0.1, pat_gen=3, pat_chk=3, en=1):
    if group != None:
        chip.setPhyAddr(group)
    if en == 1:
        if gEncodingMode[group][lane][0].upper() == 'PAM4':
            # In PAM4 mode we need to use divide by 2 for TX PLL
            chip.PAM50[lane].TX_PLL_N = 42
            chip.PAM50[lane].Reg00FF_1 = 0
            chip.PAM50[lane].RX_REFCLK_DIV4_EN = 0
            chip.PAM50[lane].Reg00D7_13 = 1
            chip.PAM50[lane].Reg00D7_15_14 = 2
            chip.PAM50[lane].Reg00D9_3_0S = 0x80000
        chip.MdioWr(0x4107 + lane * 0x20, (0x1A00 & 0xFCFF) | (pat_gen << 8))  #PRBS patt gen
        chip.MdioWr(0x410C + lane * 0x20, (0x3201 & 0xE7FF) | (pat_chk << 11)) #PRBS patt check
        chip.MdioWr(0x4107 + lane * 0x20, (0x1E00 & 0xFCFF) | (pat_gen << 8))  #turn on PRBS gen
        time.sleep(t)
        chip.MdioWr(0x410C + lane * 0x20, (0x3001 & 0xE7FF) | (pat_chk << 11)) #turn off PRBS checker
        chip.MdioWr(0x4107 + lane * 0x20, (0x0A00 & 0xFCFF) | (pat_gen << 8))  #turn off PRBS gen
    else:
        if gEncodingMode[group][lane][0].upper() == 'PAM4':
            chip.PAM50[lane].TX_PLL_N = 85
            chip.PAM50[lane].Reg00FF_1 = 1
            chip.PAM50[lane].RX_REFCLK_DIV4_EN = 0
            chip.PAM50[lane].Reg00D7_13 = 0
            chip.PAM50[lane].Reg00D7_15_14 = 0
            chip.PAM50[lane].Reg00D9_3_0S = 0


def check_rx_tx_external_dft_loopback(group=None, lane=None):
    if group != None:
        chip.setPhyAddr(group)
    PRBS_check = chip.MdioRd(0x4102 + lane * 0x20)
    if PRBS_check != 0x8FFF:
        print 'PRBS_check = ', hex(PRBS_check)
        print 'External loopback with fault'
    err_cycle_1 = chip.MdioRd(0x4113 + lane * 0x20)
    err_cycle_2 = chip.MdioRd(0x4114 + lane * 0x20)
    if (err_cycle_1 != 0) or (err_cycle_2 != 0):
        print 'Cycle number of last error is', hex((err_cycle_1 << 16) + err_cycle_2)

def broadcast_mode(Group=range(8), en=1):
    for group in Group:
        chip.setPhyAddr(group)
        if en == 1:
            chip.MdioWr(0x4014, 0x8888)
        else:
            chip.MdioWr(0x4014, 0x0000)

def fw_load_broadcast_mode(group=None, fw_file_name=None, broadcast_en=0):
    broadcast_mode(Group=group, en=broadcast_en)
    if broadcast_en == 1:
        fw_load(fw_file_name=fw_file_name, broadcast_mode=broadcast_en)
    else:
        for grp in group:
            chip.setPhyAddr(grp)
            fw_load(group=grp, fw_file_name=fw_file_name, broadcast_mode=broadcast_en)
    broadcast_mode(Group=group, en=0)

def Group_Reset(Grp_list=None):
    for group in Grp_list:
        chip.setPhyAddr(group)
        for lane in range(8):
            group_reset(lane=lane)

def fec_analyzer_init(group=None, lane=None, delay=.1, err_type=0, T=15, M=10, N=5440, print_en=1):
    if group != None:
        chip.setPhyAddr(group)
    lanes = get_lane_list(lane)

    for ln in lanes:

        if gEncodingMode[group][ln][0].upper() != 'PAM4':
            if T > 7: T = 7
        # err_type: tei ctrl teo ctrl
        chip.FECANA[ln].PRBS_CHK_CNTR_RESET = 0
        chip.FECANA[ln].RX_PRBS_FORCE_RELOAD = 1
        chip.FECANA[ln].RX_PRBS_AUTO_SYNC_EN = 1
        chip.FECANA[ln].PRBS_MISMATCH_THR = 0x10
        chip.FECANA[ln].PRBS_SYNC_THR = 0x2
        chip.FECANA[ln].PRBS_MODE = 0x3
        chip.FECANA[ln].SYM_SIZE = M
        chip.FECANA[ln].FRAM_SIZE = N
        chip.FECANA[ln].CORR_SIZE = T
        chip.FECANA[ln].TH_SIZE = T
        chip.FECANA[ln].CNT_CLR = 1
        chip.FECANA[ln].CNT_FREEZE = 0
        chip.FECANA[ln].FEC_CLK_EN = 1
        chip.FECANA[ln].FEC_ANA_EN = 1
        chip.FECANA[ln].CNT_CLR = 0
        chip.FECANA[ln].CTRL_TEO = err_type
        chip.FECANA[ln].CTRL_TEI = err_type
        if print_en: print '\n....Lane %s: FEC Analyzer Initialized' % (lane_name_list[ln]),

    time.sleep(delay)

def rx_monitor_clear(group=None, lane=None):
    global gLaneStats  # [per Slice][per lane], [PrbsCount, PrbsCount-1,PrbsCount-2, PrbsRstTime, PrbsLastReadoutTime]
    if group != None:
        chip.setPhyAddr(group)
    lanes = get_lane_list(lane)
    get_lane_mode(group=group, lane=lanes)  # updates gEncodingMode real time based on the actual setting of the chip
    for ln in lanes:
        ###### 1. Initialize FEC Analyzer for this lane
        if gEncodingMode[group][ln][0].upper() == 'PAM4':
            fec_analyzer_init(group=group, lane=ln, delay=.1, err_type=0, T=15, M=10, N=5440, print_en=0)
            ###### 2. Clear Rx PRBS Counter for this lane
            chip.PAM50[ln].prbs_rst_pam4()
        else:  # Lane is in NRZ mode
            fec_analyzer_init(group=group, lane=ln, delay=.1, err_type=0, T=7, M=10, N=5280, print_en=0)
            chip.NRZ25[ln].prbs_rst_nrz()
        ###### 3. Capture Time Stamp for Clearing FEC and PRBS Counters. Used for Calculating BER
        prbs_reset_time = time.time()  # get the time-stamp for the counter clearing

        ###### 4. Clear Stats for this lane
        #                     prbs1/2/3                               eye123     fec1,2
        gLaneStats[ln] = [0, 0, 0, prbs_reset_time, prbs_reset_time, 0, 0, 0, 'CLR', 0, 0]


def fec_analyzer_tei(lane=None):
    chip.FECANA[lane].READ_SEL = 4
    tei_l = chip.FECANA[lane].READ_DATA       # read data
    chip.FECANA[lane].READ_SEL = 5            # set reading data of TEi high 16 bit
    tei_h = chip.FECANA[lane].READ_DATA       # read data
    tei = tei_h * 65536 + tei_l               # combinate the data
    return tei

def fec_analyzer_teo(lane=None):
    chip.FECANA[lane].READ_SEL = 6       #set reading data of TEo low 16 bit
    teo_l = chip.FECANA[lane].READ_DATA  #read data
    chip.FECANA[lane].READ_SEL = 7       #set reading data of TEo high 16 bit
    teo_h = chip.FECANA[lane].READ_DATA  #read data
    teo = teo_h*65536+teo_l              #combinate the data
    return teo

def rx_monitor_capture(group=None, lane=None):
    global gLaneStats  
    chip.setPhyAddr(group)
    lanes = get_lane_list(lane)
    get_lane_mode(group=group, lane=lanes)
    for ln in lanes:
        ###### 1. Capture FEC Analyzer Data for this lane
        tei = fec_analyzer_tei(lane=ln)
        teo = fec_analyzer_teo(lane=ln)
        ###### 2. Capture PRBS Counter for this lane
        if gEncodingMode[group][ln][0].upper() == 'NRZ':
            #'enter nrz err count'
            cnt = long(chip.NRZ25[ln].RX_NRZ_PRBS_READ_ERR_HIGH_RX_NRZ_PRBS_READ_ERR_LOW)
            cnt1 = long(chip.NRZ25[ln].RX_NRZ_PRBS_READ_ERR_HIGH_RX_NRZ_PRBS_READ_ERR_LOW)
            if cnt1 < cnt:
                cnt = cnt1
        else:
            chip.PAM50[ln].latch_data_pam4()
            cnt = long(chip.PAM50[ln].PRBS_READ_SYNC_ERR_CNTR_MSB_PRBS_READ_SYNC_ERR_CNTR_LSB)
            chip.PAM50[ln].latch_data_release_pam4()
        ###### 3. Capture Time Stamp for the FEC and PRBS Counters. Used for Calculating BER
        cnt_time = time.time()  # PrbsReadoutTime = get the time stamp ASAP for valid prbs count
        if gEncodingMode[group][ln][0].upper() == 'NRZ':
            sd, rdy = chip.NRZ25[ln].ready_nrz()
        else:
            sd, rdy = chip.PAM50[ln].ready_pam4()
        cnt_n1 = gLaneStats[ln][0]  # PrevPrbsCount-1
        cnt_n2 = gLaneStats[ln][1]  # PrevPrbsCount-2
        if gLaneStats[ln][3] != 0:  # if the PRBS count was cleared at least once before, use the time of last clear
            prbs_reset_time = gLaneStats[ln][3]
        else:
            prbs_reset_time = cnt_time  # if the PRBS counter was never cleared before, use the current time as the new 'clear time'
        link_status = 'RDY'
        if (sd == 0 or rdy == 0):  # check for PHY_RDY first
            tei = 0xEEEEEEEEL
            teo = 0xEEEEEEEEL
            cnt = 0xEEEEEEEEL  # return an artificially large number if RDY=0
            cnt_n1 = 0  # PrevPrbsCount-1
            cnt_n2 = 0  # PrevPrbsCount-2
            link_status = 'NOT_RDY'
            eyes = [-1, -1, -1]
        else:  # RDY=1, then consider the PRBS count value
            if gEncodingMode[group][ln][0].upper() == 'NRZ':
                eyes = [chip.NRZ25[ln].eye_nrz()[1], 0, 0]
            else:
                eyes = chip.PAM50[ln].eye_pam4()
            if (cnt < cnt_n1):  # check for PRBS counter wrap-around
                cnt = 0xFFFFFFFFL  # return an artificially large number if counter rolls over
                cnt_n1 = 0  # PrevPrbsCount-1
                cnt_n2 = 0  # PrevPrbsCount-2
        gLaneStats[ln] = [cnt, cnt_n1, cnt_n2, prbs_reset_time, cnt_time, eyes[0], eyes[1], eyes[2],
                                  link_status, tei, teo]

def rx_monitor_print(group=None, lane=None, lc=0, ph=0):
    if group != None:
        chip.setPhyAddr(group)
    global gLaneStats  
    myber = 99e-1

    lanes = get_lane_list(lane)

    get_lane_mode(group=group, lane=lanes)

    print("\n-------------"),
    for ln in lanes:
        if ln == 8:
            print("|-------"),
        else:
            print("--------"),

    #    print("\n       Slice"),
    print("\n LC,Phy,Group"),
    for ln in lanes:  print("   %d,%d,%d" % (lc, ph, group)),
    print("\n         Lane"),
    for ln in lanes:  print("%8s" % (lane_name_list[ln])),

    print("\n     Encoding"),
    for ln in lanes:  print("%8s" % (gEncodingMode[group][ln][0].upper())),
    print("\nDataRate Gbps"),
    for ln in lanes:  print("%8.4f" % (gEncodingMode[group][ln][1])),

    print("\n-------------"),
    for ln in lanes:
        if ln == 8:
            print("|-------"),
        else:
            print("--------"),
    print("\n  Link Status"),
    for ln in lanes:  print("%8s" % (gLaneStats[ln][8])),
    print("\n    Eye1 (mV)"),
    for ln in lanes:
        if (gLaneStats[ln][8] == 'RDY'):
            print ("%8.0f" % (gLaneStats[ln][5])),
        else:
            print("       -"),
    print("\n    Eye2 (mV)"),
    for ln in lanes:
        if (gEncodingMode[group][ln][0].upper() == 'PAM4' and gLaneStats[ln][8] == 'RDY'):
            print ("%8.0f" % (gLaneStats[ln][6])),
        else:
            print("       -"),
    print("\n    Eye3 (mV)"),
    for ln in lanes:
        if (gEncodingMode[group][ln][0].upper() == 'PAM4' and gLaneStats[ln][8] == 'RDY'):
            print ("%8.0f" % (gLaneStats[ln][7])),
        else:
            print("       -"),

    print("\n-------------"),
    for ln in lanes:
        if ln == 8:
            print("|-------"),
        else:
            print("--------"),
    print("\n Elapsed Time"),
    for ln in lanes:
        if (gLaneStats[ln][8] == 'RDY'):
            elapsed_time = gLaneStats[ln][4] - gLaneStats[ln][3]
            if elapsed_time < 1000.0:
                print("%6.1f s" % (elapsed_time)),
            else:
                print("%6.0f s" % (elapsed_time)),
        else:
            print("       -"),

    # print("\nPrev PRBS    "),
    # for ln in lanes:  print("%8X " % (gLaneStats[gSlice][ln][1]) ),
    # print("\nCurr PRBS    "),
    print("\n     PRBS Cnt"),
    for ln in lanes:
        if (gLaneStats[ln][8] == 'RDY'):
            print("%8X" % (gLaneStats[ln][0])),
        else:
            print("       -"),
    # print("\nDeltaPRBS    "),
    # for ln in lanes:  print("%8X " % (gLaneStats[gSlice][ln][0] - gLaneStats[gSlice][ln][1]) ),

    print("\n     PRBS BER"),
    for ln in lanes:
        curr_prbs_cnt = float(gLaneStats[ln][0])
        prbs_accum_time = gLaneStats[ln][4] - gLaneStats[ln][3]
        data_rate = gEncodingMode[group][ln][1]
        bits_transferred = float((prbs_accum_time * data_rate * pow(10, 9)))
        if curr_prbs_cnt == 0 or bits_transferred == 0:
            ber_val = 0
            print("%8d" % (ber_val)),
            myber = ((ber_val))
        else:
            ber_val = curr_prbs_cnt / bits_transferred
            if (gLaneStats[ln][8] == 'RDY'):
                print("%8.1e" % (ber_val)),
                myber = ((ber_val))
            else:
                print("       -"),

    print("\n-------------"),
    for ln in lanes:
        if ln == 8:
            print("|-------"),
        else:
            print("--------"),
    # print "\n  User has set the FEC Threshold to less than max"
    if (gEncodingMode[group][ln][0].upper() == 'PAM4' and gFecThresh < 15) or \
            (gEncodingMode[group][ln][0].upper() != 'PAM4' and gFecThresh < 7):
        print("\n   FEC Thresh"),
        for ln in lanes:
            print("%8d" % (gFecThresh)),

    print("\n  pre-FEC Cnt"),
    for ln in lanes:
        if (gLaneStats[ln][8] == 'RDY'):
            print("%8X" % (gLaneStats[ln][9])),
        else:
            print("       -"),

    print("\n Post-FEC Cnt"),
    for ln in lanes:
        if (gLaneStats[ln][8] == 'RDY'):
            print("%8X" % (gLaneStats[ln][10])),
        else:
            print("       -"),
    print("\n  pre-FEC BER"),
    for ln in lanes:
        curr_prbs_cnt = float(gLaneStats[ln][9])
        prbs_accum_time = gLaneStats[ln][4] - gLaneStats[ln][3]
        data_rate = gEncodingMode[group][ln][1]
        bits_transferred = float((prbs_accum_time * data_rate * pow(10, 9)))
        if curr_prbs_cnt == 0 or bits_transferred == 0:
            ber_val = 0
            print("%8d" % (ber_val)),
        else:
            ber_val = curr_prbs_cnt / bits_transferred
            if (gLaneStats[ln][8] == 'RDY'):
                print("%8.1e" % (ber_val)),
            else:

                print("       -"),
    print("\n Post-FEC BER"),
    for ln in lanes:
        curr_prbs_cnt = float(gLaneStats[ln][10])
        prbs_accum_time = gLaneStats[ln][4] - gLaneStats[ln][3]
        data_rate = gEncodingMode[group][ln][1]
        bits_transferred = float((prbs_accum_time * data_rate * pow(10, 9)))
        if curr_prbs_cnt == 0 or bits_transferred == 0:
            ber_val = 0
            print("%8d" % (ber_val)),
        else:
            ber_val = curr_prbs_cnt / bits_transferred
            if (gLaneStats[ln][8] == 'RDY'):
                print("%8.1e" % (ber_val)),
            else:
                print("       -"),

    print("\n-------------"),
    for ln in lanes:
        if ln == 8:
            print("|-------"),
        else:
            print("--------"),
    print("\n")

    return myber

def rx_monitor(lane=None, rst=0, lc=0, ph=0, group=0, print_en=1, returnon=0):
    if group != None:
        chip.setPhyAddr(group)
    if lane == None and group != 8:
        lanes = get_lane_list(lane)
    elif lane == None and group == 8:
        lanes = range(4)
    elif type(lane) == int:
        lanes = [lane]
    elif type(lane) == list:
        lanes = lane
    #print 'gLaneStats_1', gLaneStats
    if (rst == 1):
        rx_monitor_clear(group=group, lane=lanes)
        # time.sleep(0.9)
    else:  # rst=0
        # in case any of the lanes' PRBS not cleared or FEC Analyzer not initialized, clear its Stats first
        for ln in lanes:
            if gLaneStats[ln][3] == 0:
                rx_monitor_clear(group=group, lane=ln)
                print("\nGroup %d Lane %s RX_MONITOR Initialized First!" % (group, lane_name_list[ln])),

    rx_monitor_capture(group=group, lane=lane)
    if print_en:
        mydata = rx_monitor_print(lane=lane, lc=lc, ph=ph, group=group)
    if returnon == 1: return mydata


def get_lane_mode(group=None, lane=None):
    if group != None:
        chip.setPhyAddr(group)
    if lane == None and group != 8:
        lanes = get_lane_list(lane)
    elif lane == None and group == 8:
        lanes = range(4)
    elif type(lane) == int:
        lanes = [lane]
    elif type(lane) == list:
        lanes = lane
    global gEncodingMode

    for ln in lanes:

        if chip.NRZ25[ln].PU_TX_BG == 0 or chip.NRZ25[ln].PU_RX_BG == 0:  # Lane's bandgap is OFF
            # print ("\n Slice %d lane %2d is OFF"%(gSlice,ln)),
            data_rate = 1.0
            gEncodingMode[group][ln] = ['off', data_rate]
            lane_mode_list[ln] = 'off'

        elif chip.NRZ25[ln].TX_NRZ_MODE == 0 and chip.PAM50[ln].PAM4_EN == 1:
            # print ("\n Slice %d lane %2d is PAM4"%(gSlice,ln)),
            data_rate = chip.LANE[ln].get_lane_pll()[0][0]
            gEncodingMode[group][ln] = ['pam4', data_rate]
            lane_mode_list[ln] = 'pam4'
        else:
            # print ("\n Slice %d lane %2d is NRZ"%(gSlice,ln)),
            data_rate = chip.LANE[ln].get_lane_pll()[0][0]
            gEncodingMode[group][ln] = ['nrz', data_rate]
            lane_mode_list[ln] = 'nrz'

    return gEncodingMode

def static_power_down(group=None, lane=None, rx_off=1, tx_off=1, rx_bg_off=0 , tx_bg_off=0):
    if group != None:
        chip.setPhyAddr(group)
    if lane == None:
        lanes = get_lane_list(lane=None)
    elif type(lane) == int:
        lanes = [lane]
    elif type(lane) == list:
        lanes = lane
    for ln in lanes:
        if rx_bg_off == 1:
            chip.NRZ25[ln].PU_RX_BG = 0
        else:
            chip.NRZ25[ln].PU_RX_BG = 1
        if tx_bg_off == 1:
            chip.NRZ25[ln].PU_TX_BG = 0
        else:
            chip.NRZ25[ln].PU_TX_BG = 1
        if rx_off == 1:
            chip.NRZ25[ln].RX_VBG = 0
            chip.NRZ25[ln].PU_RX_RVDD = 0
            chip.NRZ25[ln].PU_RX_PLL = 0
            chip.NRZ25[ln].Reg01FF_5 = 0    #pd_agc_ma
            chip.NRZ25[ln].PU_AGC_1_MASTER = 0
            chip.NRZ25[ln].PU_AGCDL_MASTER = 0

            chip.NRZ25[ln].Reg01F8_15 = 0   #pd_adc_,a
            chip.NRZ25[ln].Reg01F8_13 = 0   #pd_adc_1_ma
            chip.NRZ25[ln].PU_RX_AGC_LN = 0
            chip.NRZ25[ln].PU_AGCDL = 0

            chip.NRZ25[ln].PU_RX_PLL_INTP = 0
            chip.NRZ25[ln].Reg01FF_7 = 0    #pd_rvddloop_rx
            chip.NRZ25[ln].Reg01F8_14 = 0   #pd_clkcompreg_ma
            chip.NRZ25[ln].Reg01F8_9 = 0    #pd_intp_ma
            chip.NRZ25[ln].Reg01FC_6_4 = 0  #vrvdd_rx
            chip.NRZ25[ln].Reg01FC_3_1 = 0  #vrvdd2_rx
            chip.NRZ25[ln].PU_DEGENMAIN_SUM1_MSB = 0
            chip.NRZ25[ln].PU_DEGENMAIN_SUM1_LSB = 0
            chip.NRZ25[ln].PU_DEGENMAIN_SUM2_MSB = 0
            chip.NRZ25[ln].PU_DEGENMAIN_SUM2_LSB = 0
            chip.NRZ25[ln].PU_DEGENMAIN_SUM3_MSB = 0
            chip.NRZ25[ln].PU_DEGENMAIN_SUM3_LSB = 0
            chip.NRZ25[ln].PU_INTP = 0
            chip.FECANA[ln].FEC_ANA_EN = 0
        else:
            chip.NRZ25[ln].RX_VBG = 0x7
            chip.NRZ25[ln].PU_RX_RVDD = 1
            chip.NRZ25[ln].PU_RX_PLL = 1
            chip.NRZ25[ln].Reg01FF_5 = 1
            chip.NRZ25[ln].PU_AGC_1_MASTER = 1
            chip.NRZ25[ln].PU_AGCDL_MASTER = 1
            chip.NRZ25[ln].Reg01F8_15 = 1
            chip.NRZ25[ln].Reg01F8_13 = 1
            chip.NRZ25[ln].PU_RX_AGC_LN = 1
            if gEncodingMode[group][ln][0].upper() == 'NRZ':
                chip.NRZ25[ln].PU_AGCDL = 0
            elif gEncodingMode[group][ln][0].upper() == 'PAM4':
                chip.NRZ25[ln].PU_AGCDL = 1
            chip.NRZ25[ln].PU_RX_PLL_INTP = 1
            chip.NRZ25[ln].Reg01FF_7 = 1        # pu_rvddloop_rx
            chip.NRZ25[ln].Reg01F8_14 = 1       # pu_clkcompreg_ma
            chip.NRZ25[ln].Reg01F8_9 = 1        # pu_intp_ma
            chip.NRZ25[ln].Reg01FC_6_4 = 0x4  # vrvdd_rx
            chip.NRZ25[ln].Reg01FC_3_1 = 0x4  # vrvdd2_rx

            chip.NRZ25[ln].PU_DEGENMAIN_SUM1_MSB = 1
            chip.NRZ25[ln].PU_DEGENMAIN_SUM1_LSB = 1
            chip.NRZ25[ln].PU_DEGENMAIN_SUM2_MSB = 1
            chip.NRZ25[ln].PU_DEGENMAIN_SUM2_LSB = 1
            chip.NRZ25[ln].PU_DEGENMAIN_SUM3_MSB = 1
            chip.NRZ25[ln].PU_DEGENMAIN_SUM3_LSB = 1
            chip.NRZ25[ln].PU_INTP = 1
            #chip.FECANA[ln].FEC_ANA_EN = 1

        if tx_off == 1:
            chip.NRZ25[ln].TX_VBG = 0
            chip.NRZ25[ln].Reg00FF_11 = 0   #pu_rvdd_tx
            chip.NRZ25[ln].PU_TX_PLL = 0
            chip.NRZ25[ln].PU_VDRV_MA = 0
            chip.NRZ25[ln].Reg00EB_13 = 0   #pd vdrv
            chip.NRZ25[ln].PU_HIMODE_VDDR = 0
            chip.NRZ25[ln].Reg00F1_2 = 0    #pd_adc
            chip.NRZ25[ln].Reg00FF_7 = 0    #pd_rvddloop_tx
            chip.NRZ25[ln].Reg00FD_6_4 = 0  #vrdd_tx default is 011
            chip.NRZ25[ln].Reg00FD_3_1 = 0  #vpllpmp1_tx default is 011
            chip.NRZ25[ln].Reg00F1_1 = 0    #pd_clkcomp
            chip.NRZ25[ln].Reg00F1_0 = 0    #pd_clkcompreg
            #chip.NRZ25[ln].PU_ACJTAG = 0    #pd_acjtag
        else:
            chip.NRZ25[ln].TX_VBG = 0x7
            chip.NRZ25[ln].Reg00FF_11 = 1   #pu_rvdd_tx
            chip.NRZ25[ln].PU_TX_PLL = 1
            chip.NRZ25[ln].PU_VDRV_MA = 1
            chip.NRZ25[ln].Reg00EB_13 = 1
            chip.NRZ25[ln].PU_HIMODE_VDDR = 0
            chip.NRZ25[ln].Reg00F1_2 = 1
            chip.NRZ25[ln].Reg00FF_7 = 1      # pu_rvddloop_tx
            chip.NRZ25[ln].Reg00FD_6_4 = 0x3  # vrdd_tx default is 011
            chip.NRZ25[ln].Reg00FD_3_1 = 0x3  # vpllpmp1_tx default is 011
            chip.NRZ25[ln].Reg00F1_1 = 1      # pu_clkcomp
            chip.NRZ25[ln].Reg00F1_0 = 1      # pu_clkcompreg
            #chip.NRZ25[ln].PU_ACJTAG = 1      # pu_acjtag

def static_power_down_broadcast_mode(group=None, lanes =None):
    broadcast_mode(Group=group, en=1)
    for ln in lanes:
        chip.MdioWr(0x1FF + 0x800*ln, 0x1511)
        chip.MdioWr(0x1FE + 0x800*ln, 0x0)
        chip.MdioWr(0x1FD + 0x800*ln, 0x2934)
        chip.MdioWr(0x1FC + 0x800*ln, 0x1200)
        chip.MdioWr(0x1F8 + 0x800*ln, 0x0)
        chip.MdioWr(0x1F3 + 0x800*ln, 0xB440)
        chip.MdioWr(0x1E7 + 0x800*ln, 0xB6C)
        chip.MdioWr(0x1E0 + 0x800*ln, 0x0)
        chip.MdioWr(0x1DD + 0x800*ln, 0x8040)
        chip.MdioWr(0x0FF + 0x800*ln, 0x1576)
        chip.MdioWr(0x0FE + 0x800*ln, 0x2934)
        chip.MdioWr(0x0FD + 0x800*ln, 0x1200)
        chip.MdioWr(0x0FA + 0x800*ln, 0x10)
        chip.MdioWr(0x0F1 + 0x800*ln, 0x8)
        chip.MdioWr(0x0EB + 0x800*ln, 0x436C)


###########################################################################################################
def fw_reg(addr=None, data=None, print_en=1):
    if addr == None:
        addr_list = range(201)  # read all FW registers
        data = None  # Make sure not to write more than one address at a time. Just read FW registers and exit!
    elif type(addr) == int:
        addr_list = [addr]  # read single FW register
    elif type(addr) == list:
        addr_list = addr  # read list of FW registers
        data = None  # Make sure not to write more than one address at a time. Just read FW registers and exit!
    # print 'addr_list', addr_list
    result = {}
    str = ""
    #### FW Reg Write if data-to-write is not given
    if data != None:
        chip.MdioWr(0x40C2, addr_list[0])  # fw_cmd_detail_addr = 0x9807
        chip.MdioWr(0x40C4, data)  # fw_cmd_status_addr = 0x98C7
        cmd_status = fw_cmd(0xe020)  # fw_cmd_addr = 0x9806
        if cmd_status != 0x000e:
            print("*** FW Register write error: Addr %d Code=0x%04x" % (addr, cmd_status))
            return False

    line_separator = "\n#+-------------------------+"
    title = "\n#+ FWReg |      Value      |"
    str += line_separator + title + line_separator
    #### FW Reg Read
    # print 'addr_list', addr_list
    for reg_addr in addr_list:
        # print 'reg_addr', reg_addr
        chip.MdioWr(0x40C2, reg_addr)  # fw_cmd_detail_addr = 0x9807
        cmd_status = fw_cmd(0xe010)  # fw_cmd_addr = 0x9806
        # time.sleep(0.1)
        if cmd_status != 0x000e:
            # print 'b'
            # Undefined FW register address or the read of a "defined" FW register failed
            print("*** FW Register read error: Addr %d Code=0x%04x" % (reg_addr, cmd_status))
            # break
        else:
            result[reg_addr] = chip.MdioRd(0x40C4)
            str += ("\n#|  %3d  | 0x%04x  (%-5d) |" % (reg_addr, result[reg_addr], result[reg_addr]))

    str += line_separator
    if print_en == 1:
        print str
    else:
        return result


import struct


def fw_load(group=None, fw_file_name=None, broadcast_mode=0, wait=0.001):
    # set top pll
    fw_file_ptr = open(fw_file_name, 'rb')
    fw_data = fw_file_ptr.read()
    start = 4096
    file_hash_code = struct.unpack_from('>I', fw_data[start:start + 4])[0]
    file_crc_code = struct.unpack_from('>H', fw_data[start + 4:start + 6])[0]
    file_date_code = struct.unpack_from('>H', fw_data[start + 6:start + 8])[0]
    entryPoint = struct.unpack_from('>I', fw_data[start + 8:start + 12])[0]
    length = struct.unpack_from('>I', fw_data[start + 12:start + 16])[0]
    ramAddr = struct.unpack_from('>I', fw_data[start + 16:start + 20])[0]
    data = fw_data[start + 20:]

    d = datetime.date(1970, 1, 1) + datetime.timedelta(file_date_code)

    print "fw_load Hash Code : 0x%06x" % file_hash_code
    print "fw_load Date Code : 0x%02x (%04d-%02d-%02d)" % (file_date_code, d.year, d.month, d.day)
    print "fw_load  CRC Code : 0x%04x" % file_crc_code
    print "fw_load    Length : %d" % length
    print "fw_load     Entry : 0x%08x" % entryPoint
    print "fw_load       RAM : 0x%08x" % ramAddr

    dataPtr = 0
    sections = (length + 23) / 24
    # ===========Firmware unload start===========
    chip.MdioWr(0x40C0, 0xFFF0)
    time.sleep(.1)
    chip.MdioWr(0x4013, 0x0AAA)
    time.sleep(.1)
    chip.MdioWr(0x4013, 0x0000)
    time.sleep(.1)

    if broadcast_mode == 1:  # broadcast download mode, use fixed delay
        time.sleep(0.1)
    else:
        start_time = time.time()
        checkTime = 0
        status = chip.MdioRd(0x40C0)
        while status != 0:
            status = chip.MdioRd(0x40C0)
            checkTime += 1
            if checkTime > 100000:
                print '\n...FW LOAD ERROR: : Wait for 0x40C0=0 Timed Out! FW2 = 0x%X' % status,  # Wait for 0x40c0=0: 0.000432 sec
                break
        stop_time = time.time()
    chip.MdioWr(0x40C0, 0x0000)
    # ===========Firmware unload finish==========
    if group == 8:
        chip.setPhyAddr(9)
    i = 0
    while i < sections:
        checkSum = 0x800c
        if i == 0: print checkSum
        chip.MdioWr(0x5000 + 12, ramAddr >> 16)
        chip.MdioWr(0x5000 + 13, (ramAddr & 0xFFFF))
        checkSum += (ramAddr >> 16) + (ramAddr & 0xFFFF)
        for j in range(12):
            if (dataPtr > length):
                mdioData = 0x0000
            else:
                mdioData = struct.unpack_from('>H', data[dataPtr:dataPtr + 2])[0]
            chip.MdioWr(0x5000 + j, mdioData)
            checkSum += mdioData
            dataPtr += 2
            ramAddr += 2

        chip.MdioWr(0x5000 + 14, (~checkSum + 1) & 0xFFFF)
        chip.MdioWr(0x5000 + 15, 0x800c)

        if broadcast_mode == 1:
            time.sleep(wait)
        else:
            checkTime = 0
            status = chip.MdioRd(0x5000 + 15)
            while status == 0x800c:
                status = chip.MdioRd(0x5000 + 15)
                checkTime += 1
                if checkTime > 1000:
                    print '\n...FW LOAD ERROR: Write to Ram Timed Out! 0x5000 = %x' % status,
                    break
        i += 1

    chip.MdioWr(0x5000 + 12, entryPoint >> 16)
    chip.MdioWr(0x5000 + 13, (entryPoint & 0xFFFF))
    checkSum = (entryPoint >> 16) + (entryPoint & 0xFFFF) + 0x4000
    chip.MdioWr(0x5000 + 14, (~checkSum + 1) & 0xFFFF)
    chip.MdioWr(0x5000 + 15, 0x4000)
    fw_file_ptr.close()
    if group == 8:
        chip.setPhyAddr(8)
    print("Done!"),
    time.sleep(.5)


def fw_unload():
    chip.MdioWr(0x40C0, 0xFFF0)
    time.sleep(.1)
    chip.MdioWr(0x4013, 0x0AAA)
    time.sleep(.1)
    chip.MdioWr(0x4013, 0x0000)
    time.sleep(.1)

    start_time = time.time()
    checkTime = 0
    status = chip.MdioRd(0x40C0)
    while status != 0:
        status = chip.MdioRd(0x40C0)
        checkTime += 1
        if checkTime > 100000:
            print '\n...FW LOAD ERROR: : Wait for 0x40C0=0 Timed Out! FW2 = 0x%X' % status,
            break
    stop_time = time.time()
    chip.MdioWr(0x40C0, 0x0000)


def fw_cmd(cmd=0x0000, print_en=False):
    if print_en: print("Writing command : %d" % cmd)
    chip.MdioWr(0x40C1, cmd)
    loop_cnt = 0
    while (chip.MdioRd(0x40C1) == cmd):
        loop_cnt += 1
        if (loop_cnt > 1000):
            break
        continue
    return (chip.MdioRd(0x40C1) >> 8)


##############################################################################
# This function reads VERSION for the FW already loaded in Serdes
#
##############################################################################
def fw_ver(print_en=False):
    date_code = fw_date()
    fw_cmd(cmd=0xF003)
    high_word = chip.MdioRd(0x40C1)
    low_word = chip.MdioRd(0x40C2)
    if date_code == low_word:
        d = datetime.date(1970, 1, 1) + datetime.timedelta(date_code)
        ver_code = ((d.month) << 8) + d.day
    else:
        ver_code = (high_word << 16) + low_word
    if print_en: print(
            "\n...FW Version : %02d.%02d.%02d\n" % (ver_code >> 8 & 0xFF, ver_code >> 8 & 0xFF, ver_code & 0xFF))

    return ver_code


##############################################################################
# This function reads HASH CODE for the FW already loaded in Serdes
#
##############################################################################
def fw_hash(print_en=False):
    chip.MdioWr(0x40C2, 0x0)
    fw_cmd(cmd=0xF000)
    high_word = chip.MdioRd(0x40C1) & 0xFF  # upper byte, only 8 bits are valid
    low_word = chip.MdioRd(0x40C2)  # lower word
    hash_code = (high_word << 16) + low_word
    if print_en: print("\n...FW Hash Code : 0x%06X\n" % (hash_code))
    return hash_code


##############################################################################
# This function reads CRC CODE for the FW already loaded in Serdes
#
##############################################################################
def fw_crc(print_en=False):
    fw_cmd(cmd=0xF001)
    crc_code = chip.MdioRd(0x40C2)
    if print_en: print("\n...FW CRC Code : 0x%06X\n" % (crc_code))
    return crc_code


##############################################################################
# This function reads MAGIC for the FW already loaded in Serdes
#
##############################################################################
def fw_magic(print_en=False):
    magic_word = chip.MdioRd(0x40C0)
    if print_en: print("\n...FW Magic Word : 0x%04X\n" % (magic_word))
    return magic_word


##############################################################################
# This function reads DATE CODE for the FW already loaded in Serdes
#
##############################################################################
def fw_date(print_en=False):
    fw_cmd(cmd=0xF002)
    datecode = chip.MdioRd(0x40C2)
    d = datetime.date(1970, 1, 1) + datetime.timedelta(datecode)
    if print_en: print("\n...FW Date Code : %04d-%02d-%02d\n" % (d.year, d.month, d.day))
    return datecode


##############################################################################
# This function reads Info for the FW already loaded in Serdes
#
##############################################################################
def fw_info(print_en=False):
    global gFwFileName
    global gFwFileNameLastLoaded

    try:
        gFwFileNameLastLoaded
    except NameError:
        fw_bin_filename = 'FW_FILENAME_UNKNOWN'
    else:
        fw_bin_filename = gFwFileNameLastLoaded

    ver_code = fw_ver()
    date_code = fw_date()
    hash_code = fw_hash()
    crc_code = fw_crc()
    magic_code = fw_magic()
    d = datetime.date(1970, 1, 1) + datetime.timedelta(date_code)

    ver_str = "VER_%02d.%02d.%02d" % (ver_code >> 16 & 0xFF, ver_code >> 8 & 0xFF, ver_code & 0xFF)
    date_str = "DATE_%04d%02d%02d" % (d.year, d.month, d.day)
    hash_str = "HASH_0x%06X" % (hash_code)
    crc_str = "CRC_0x%04X" % (crc_code)
    magic_str = "MAGIC_0x%04X" % (magic_code)

    if print_en:
        print("\n... Credo FW Info: Device : "),
        print("'%s'," % (fw_bin_filename)),
        print("%s," % (ver_str)),
        print("%s," % (date_str)),
        print("%s," % (hash_str)),
        print("%s," % (crc_str)),
        print("%s \n" % (magic_str))

    else:
        return fw_bin_filename, ver_str, date_str, hash_str, crc_str, magic_str


##############################################################################

# This function check if a FW is loaded
#
# Return 0 : if FW is not loaded
# Return 1 : if FW is loaded
#
##############################################################################
def fw_loaded_new(fw_file_name=None, print_en=False):
    fw_file_ptr = open(fw_file_name, 'rb')
    fw_data = fw_file_ptr.read()
    start = 4096
    file_hash_code = struct.unpack_from('>I', fw_data[start:start + 4])[0]
    file_crc_code = struct.unpack_from('>H', fw_data[start + 4:start + 6])[0]

    val = fw_hash()
    crc = fw_crc()
    if val != file_hash_code or crc != file_crc_code:
        fw_loaded_stat = 0
        if print_en:
            print("\n...Device has no FW Loaded!")
    else:
        fw_loaded_stat = 1
        if print_en:
            print("\n...Device has FW Loaded!")
    return fw_loaded_stat


def fw_loaded(print_en=False):
    val = fw_hash()
    if val == 0:
        fw_loaded_stat = 0
        if print_en:
            print("\n...Device has no FW Loaded!")
    else:
        fw_loaded_stat = 1
        if print_en:
            print("\n...Device has FW Loaded!")
    return fw_loaded_stat


##############################################################################
# This function reads WATCHDOG counter value
#
##############################################################################
def fw_watchdog(count=None, print_en=False):
    if count != None:
        chip.MdioWr(0x40C3, 0x0000)
    watchdog_count = chip.MdioRd(0x40C3)
    return watchdog_count


def fw_lane_speed(lane=None):
    # [ 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,  0x08,  0x09, 0x0A ]
    speed_list = ['OFF', '10G', '20G', '25G', '26G', '28G', '50G', '07?', '53G', '53G', '1G']
    mode_list = ['off', 'nrz', 'nrz', 'nrz', 'nrz', 'nrz', 'pam4', '07?', 'pam4', 'pam4', 'nrz']
    result = {}
    speed_index = (fw_debug_cmd(section=0, index=4, lane=lane) & 0xf)
    # speed_index = 3
    result = [mode_list[speed_index], speed_list[speed_index]]
    # result[ln] = speed_index
    return result


def fw_config_lane(mode=None, datarate=None, lane=None):
    if not fw_loaded(print_en=0):
        print("\n*** No FW Loaded. Skipping fw_config_lane().\n")
        return

    lanes = get_lane_list(lane)
    curr_fw_lane_mode = fw_lane_mode(lanes)  # learn which mode (nrz/pam4/off) each lane is in now

    speed_str_list = ['10', '20', '25', '26', '28', '51', '50', '53', '56']  # speed as part of mode argument
    speed_code_list = [0x11, 0x22, 0x33, 0x44, 0x55, 0x88, 0x99, 0x99, 0xAA]  # speed codes to be written to 0x9807[7:0]

    if mode == None:  # no arguments? then just print current fw_modes of the lanes and exit
        print""
        for ln in lanes:
            print(" %4s" % (lane_name_list[ln])),
        for idx in [1, 2]:  # show both fw modes [0]:set_mode [1]: opt_mode [2]: adapt_done flag
            print""
            for ln in lanes:
                print(" %4s" % curr_fw_lane_mode[ln][idx]),
    else:  # configure (either activate or turn off) the lanes
        if 'NRZ' in mode.upper():
            mode_code_cmd = 0x80C0  # command to activate lane in NRZ
            speed_code_cmd = 0x33  # default NRZ speed is 25G
            if datarate != None:  mode = mode + str(
                int(round((datarate - 1.0) / 5.0) * 5.0))  # add datarate to mode so we can find it in the speed codes
            for i in range(len(speed_str_list)):  # if NRZ speed is specified, take it
                if speed_str_list[i] in mode: speed_code_cmd = speed_code_list[i]

        elif 'PAM4' in mode.upper():
            mode_code_cmd = 0x80D0  # command to activate lane in PAM4
            speed_code_cmd = 0x99  # default PAM4 speed is 53G
            for i in range(len(speed_str_list)):  # if PAM4 speed is specified, take it
                if speed_str_list[i] in mode: speed_code_cmd = speed_code_list[i]

        elif 'OFF' in mode.upper():  # command to deactivate lane (Turn it OFF)
            speed_code_cmd = 0x00  # For deactivating the lane, speed code does not matter
            mode_code_cmd = 0x90D0 if (curr_fw_lane_mode[lanes[0]][1] == 'PAM4') else 0x90C0

        else:
            for ln in lanes:
                print("\n***Device %d Lane %s FW Config: selected mode is invalid  => '%s'" % (
                    gDevice, lane_name_list[ln], mode.upper())),
            return

        ################ Destroy the target lanes before programming them to target mode
        for ln in lanes:
            off_cmd = 0x90D0 + ln if (curr_fw_lane_mode[ln][0] == 'PAM4') else 0x90C0 + ln
            result = fw_config_cmd(config_cmd=off_cmd, config_detail=0x0000)
            if (result != c.fw_config_lane_status):  # fw_config_lane_status=0x800
                print("\n***Device %d Lane %s: FW could not free up lane before reconfiguring it. (Error Code 0x%04x)" % (gDevice, lane_name_list[ln], result)),

        ############# Now, configure the lane per user's target mode (either activate or deactivate)
        for ln in lanes:
            result = fw_config_cmd(config_cmd=mode_code_cmd + ln, config_detail=speed_code_cmd)
            if (result != c.fw_config_lane_status):  # fw_config_lane_status=0x800
                print(
                        "\n***Device %d Lane %s FW_CONFIG_LANE to Active Mode Failed. (SpeedCode 0x9807=0x%04X, ActiveCode 0x9806=0x%04X, ExpectedStatus:0x%0X, ActualStatus=0x%04x)" % (
                    gDevice, lane_name_list[ln], speed_code_cmd, mode_code_cmd + ln, c.fw_config_lane_status, result)),


##############################################################################
# show current mode assigned to the lane by the FW
#
# index of the mode:
# 0: set_mode before opt
# 1: opt_mode
#
# mode:
# 0: lane in OFF
# 1: lane is in NRZ mode
# 2: lane is in PAM4 mode
##############################################################################
def fw_lane_mode(lane=None):
    lanes = get_lane_list(lane)
    result = {}
    # print current fw_modes of the lanes
    dbg_mode = 0
    dbg_cmd = 0xb000
    mode_def = ['OFF', 'NRZ', 'PAM4']
    mode_idx = [0, 1]
    opt_status_def = ['-', 'OPT']
    opt_status_bit = [0, 1]

    for ln in lanes:
        for idx in range(2):  # get both fw modes [0]:set_mode [1]: opt_mode
            mode_idx[idx] = fw_debug_cmd(section=0, index=idx, lane=ln)  # for index=1 ===> 0: OFF, 1: NRZ, 2: PAM4
        opt_status_bit = (chip.MdioRd(0x40C7) >> ln) & 0x0001
        result[ln] = (mode_def[mode_idx[0]], mode_def[mode_idx[1]], opt_status_def[opt_status_bit])
    return result


def fw_config_cmd(config_cmd=0x8090, config_detail=0x0000):
    chip.MdioWr(0x40C2, config_detail)
    status = fw_cmd(config_cmd)
    if (status == 0x302):
        print("FW Config CMD 0x%04X, Config Detail: 0x%04X, Failed with Status 0x%04X. RETRYING..." % (
            config_cmd, config_detail, status))
        status = fw_cmd(config_cmd)

    if (status == 0x302):
        print("FW Config CMD 0x%04X, Config Detail: 0x%04X, Failed with Status 0x%04X. AFTER RETRY" % (
            config_cmd, config_detail, status))

    return status


def fw_adapt_cnt(group=None, lane=None):
    if not fw_loaded(print_en=0):
        result = -1

    if gEncodingMode[group][lane][0].upper == 'NRZ':
        result = fw_debug_cmd(section=1, index=10, lane=lane)
    else:
        result = fw_debug_cmd(section=2, index=7, lane=lane)

    return result


##############################################################################
#
# "Re-adaptation" counter for SerDes PHY. Clears on read, saturates to 0xFFFF.
# This increments with fw_adapt_cnt (FW does a lane-restart)
#
##############################################################################
def fw_readapt_cnt(lane=None):
    if not fw_loaded(print_en=0):
        result = -1
    result = fw_debug_cmd(section=8, index=1, lane=lane)

    return result


##############################################################################
#
# "Link lost" counter for SerDes PHY. Clears on read, saturates to 0xFFFF.
#
##############################################################################
def fw_link_lost_cnt(lane=None):
    if not fw_loaded(print_en=0):
        result = -1
    result = fw_debug_cmd(section=8, index=0, lane=lane)

    return result


def debug_cmd(mode=0, index=0):
    chip.MdioWr(0x40C2, index)
    chip.MdioWr(0x40C1, 0xB000 + (mode << 4))
    loop_cnt = 0
    while (chip.MdioRd(0x40C1) == cmd):
        loop_cnt += 1
        if (loop_cnt > 1000):
            break
        continue
    if (loop_cnt == 1000): print ("debug_cmd failed")
    return (chip.MdioRd(0x40C2))


def twos_to_int(twos_val, bitWidth):
    '''
    return a signed decimal number
    '''
    mask = 1 << (bitWidth - 1)
    return -(twos_val & mask) + (twos_val & ~mask)


def Bin_Gray(bb=0):
    gg = bb ^ (bb >> 1)
    return gg

def fw_serdes_params(group=None, lanes=None, print_en=False):
    chip.setPhyAddr(group)
    if lanes == None:
        lanes = get_lane_list(lane=None)
    if not fw_loaded(print_en=False):
        print ("\n######## FW is Not Loaded ! #########")

    line_separator = "\n#+-----------------------------------------------------------------------------------------------------------------------------------------------------------------+"
    print line_separator,
    print (
        "\n#|   |     |    |     |    COUNTERS     |SD,Rdy,| FRQ |  CHANNEL   |      CTLE     |   |   | EYE MARGIN  |         DFE       | TIMING  |           FFE Taps      |"),
    print (
        "\n#|Dev|Group|Lane| Mode| Adp ,ReAdp,LLost|AdpDone| PPM | Est ,OF,HF |Peaking, G1,G2 |SK |DAC|  1 , 2 , 3  | F0 , F1 ,F1/F0,F13|Del,Edge | K1 , K2 , K3 , K4 ,S1,S2|"),
    print line_separator,
    for ln in lanes:
        if fw_loaded(print_en=False):
            [lane_mode, lane_speed] = fw_lane_speed(ln)
        else:
            get_lane_mode(group=group, lane=ln)
            [lane_mode, lane_speed] = gEncodingMode[group][ln]
        if lane_mode.upper() == 'OFF':  # Lane is OFF
            print ("\n#| %d |  %d  | %s | %3s" % (0, group, lane_mode, lane_speed)),
            print (
                "|                 |       |     |       |            |               |   |   |             |                   |         |                         |"),

        else:  # Lane is active, in PAM4 or NRZ mode
            adapt_cnt = fw_adapt_cnt(group=group, lane=ln)
            readapt_cnt = fw_readapt_cnt(ln)
            linklost_cnt = fw_link_lost_cnt(ln)
            if gEncodingMode[group][ln][0] == 'nrz':
                sd = chip.NRZ25[ln].RX_SIG_DET
                rdy = chip.NRZ25[ln].RX_NRZ_PHY_READY
            else:
                sd = chip.PAM50[ln].Reg0030_0
                rdy = chip.PAM50[ln].Reg0030_1
            adapt_done = (chip.MdioRd(0x40C7) >> ln) & 1
            sd_flag = '*' if (sd != 1) else ' '
            rdy_flag = '*' if (rdy != 1 or adapt_done != 1) else ' '
            if gEncodingMode[group][ln][0] == 'nrz':
                ppm = chip.MdioRd(0x173 + ln * 0x800) & 0x7FF
                of = fw_debug(ln, 1, 4)
                hf = fw_debug(ln, 1, 5)
            else:
                ppm = chip.MdioRd(0x073 + ln * 0x800) & 0x7FF
                of = fw_debug(ln, 2, 4)
                hf = fw_debug(ln, 2, 5)
            if (hf > 0):
                chan_est = float(float(of) / float(hf))
            else:
                chan_est = 0
            if gEncodingMode[group][ln][0] == 'nrz':
                ctle_val = chip.NRZ25[ln].RX_NRZ_CTLE_OVER_VAL
                map0 = chip.MdioRd(0x176 + ln * 0x800)
                map1 = chip.MdioRd(0x177 + ln * 0x800)
                map2 = chip.MdioRd(0x178 + ln * 0x800)
                ctle_map_0 = chip.NRZ25[ln].ctle_map_nrz(ctle_val, ln)[0]
                ctle_map_1 = chip.NRZ25[ln].ctle_map_nrz(ctle_val, ln)[1]
            else:
                ctle_val = chip.PAM50[ln].RX_PAM4_CTLE_OVER_VAL
                map0 = chip.MdioRd(0x048 + ln * 0x800)
                map1 = chip.MdioRd(0x049 + ln * 0x800)
                map2 = chip.MdioRd(0x04A + ln * 0x800)
                ctle_map_0 = chip.PAM50[ln].ctle_map_pam4(ctle_val, ln)[0]
                ctle_map_1 = chip.PAM50[ln].ctle_map_pam4(ctle_val, ln)[1]
            agc = {0: [map0 >> 13, (map0 >> 10) & 0x7],
                   1: [(map0 >> 7) & 0x7, (map0 >> 4) & 0x7],
                   2: [(map0 >> 1) & 0x7, ((map0 & 0x1) << 2) + (map1 >> 14)],
                   3: [(map1 >> 11) & 0x7, (map1 >> 8) & 0x7],
                   4: [(map1 >> 5) & 0x7, (map1 >> 2) & 0x7],
                   5: [((map1 & 0x3) << 1) + (map2 >> 15), (map2 >> 12) & 0x7],
                   6: [(map2 >> 9) & 0x7, (map2 >> 6) & 0x7],
                   7: [(map2 >> 3) & 0x7, map2 & 0x7]
                   }
            ctle_1_bit4 = int(rreg([0x1d7, [3]], ln), 16)
            ctle_2_bit4 = int(rreg([0x1d7, [2]], ln), 16)
            ctle_1 = ctle_map_0 + (ctle_1_bit4 * 8)
            ctle_2 = ctle_map_1 + (ctle_2_bit4 * 8)
            dc_gain_1, dc_gain_2 = chip.NRZ25[ln].agcgain()
            if gEncodingMode[group][ln][0] == 'nrz':
                skef_val = chip.NRZ25[ln].SKEF_VAL
                dac_val = chip.NRZ25[ln].DAC_SEL
                eye = (float(chip.NRZ25[ln].RX_READ_EM) / 2048.0) * (200 + (50.0 * float(dac_val)))
                delta_val = chip.NRZ25[ln].delta_nrz()
                edge1, edge2, edge3, edge4 = chip.NRZ25[ln].edge()
                f1, f2, f3 = chip.NRZ25[ln].dfe_nrz()
            else:
                skef_val = chip.PAM50[ln].SKEF_VAL
                dac_val = chip.PAM50[ln].READ_DAC_SEL
                eyes = chip.PAM50[ln].eye_pam4()
                if (ctle_1_bit4 == 1 or ctle_2_bit4 == 1 or chip.PAM50[ln].ctle_map_pam4(7)[0] == 7): ctle_val += 8
                delta_val = chip.PAM50[ln].delta_ph_pam4()
                edge1, edge2, edge3, edge4 = chip.PAM50[ln].edge()
                f0, f1, f1f0_ratio = chip.PAM50[ln].get_pam4_dfe()
                f13_val = chip.PAM50[ln].f13()
                [ffe_k1_bin, ffe_k2_bin, ffe_k3_bin, ffe_k4_bin, ffe_s1_bin, ffe_s2_bin] = chip.PAM50[ln].ffe_taps()
            print ("\n#| %d |  %d  | %2d | %3s |%5d,%5d,%4d |%s%d,%d,%d%s|%4d |" % (
            0, group, ln, lane_speed, adapt_cnt, readapt_cnt, linklost_cnt, sd_flag, sd, rdy, adapt_done, rdy_flag, ppm)),
            print ("%4.2f,%2d,%2d" % (chan_est, of, hf)),
            print ("| %d(%d,%d),%3d,%2d" % (ctle_val, ctle_1, ctle_2, dc_gain_1, dc_gain_2)),
            print ("| %d |%2d" % (skef_val, dac_val)),
            if gEncodingMode[group][ln][0] == 'nrz':
                print ("|   %4.0f     " % (eye)),
                print ("|%4d,%4d,%4d    " % (f1, f2, f3)),
                print ('|%3d,%X%X%X%X |' % (delta_val, edge1, edge2, edge3, edge4)),
                print ('                        | '),
            else:
                print ("|%4.0f,%3.0f,%3.0f" % (eyes[0], eyes[1], eyes[2])),
                print ("|%4.2f,%4.2f,%5.2f,%2d" % (f0, f1, f1f0_ratio, f13_val)),
                print ("|%3d,%X%X%X%X |" % (delta_val, edge1, edge2, edge3, edge4)),
                print ('\b%4d,%4d,%4d,%4d,%02X,%02X| ' % (
                ffe_k1_bin, ffe_k2_bin, ffe_k3_bin, ffe_k4_bin, ffe_s1_bin, ffe_s2_bin)),
    print line_separator,  # Line separator between A lines and B lines


##############################################################################
# This function checks if a FW is loaded and is running?
#
# Returns 0: if FW is not running or not loaded
# Returns 1: if FW is running
#
##############################################################################
def fw_running():
    val0 = fw_watchdog()  # check the watchdog counter to see if FW is incrementing it
    if val0 == 0:  # A quick check for zero value
        fw_running_stat = 0  # watchdog counter is zero, FW is not loaded
    else:
        time.sleep(1.8)  # wait more than one second
        val1 = fw_watchdog()  # check the watchdog counter once more
        if val1 > val0:  # watchdog counter is moving, then FW is loaded and running
            # wreg(fw_load_magic_word_addr, fw_load_magic_word) # if FW is loaded but magic word is corrupted, correct it
            fw_running_stat = 1  # watchdog counter is moving, then FW is loaded and running
        else:
            fw_running_stat = 0  # watchdog counter is not moving, then FW is not loaded or halted
            fw_watchdog(0)  # clear the counter so the next fw_running check goes faster

    return fw_running_stat


##############################################################################
# BE debug functions
#
##############################################################################
def fw_debug(lane, mode, index, print_en=False):
    chip.MdioWr(0x40C2, index)
    cmd = 0xB000 + ((mode & 0xf) << 4) + lane
    status = fw_cmd(cmd)
    result = chip.MdioRd(0x40C1)
    if (result != 0x0b00 + mode):
        print("Debug command failed with code %04x" % result)
        # raise IOError("Debug command failed with code %04x" % result)

    return chip.MdioRd(0x40C2)


##############################################################################
# Get FW Debug Information
##############################################################################
def fw_debug_cmd(section=2, index=7, lane=0):
    timeout = 0.2
    result = 0
    cmd = 0xB000 + ((section & 0xf) << 4) + lane
    chip.MdioWr(0x40C2, index)  # fw_cmd_detail_addr = 0x40C2
    status = fw_cmd(cmd)  # fw_cmd_addr = 0x40C1
    if status != 0xB:
        # print("FW Debug CMD Section %d, Index %d, for Lane %s failed with code 0x%04x" %(section, index, lane_name_list[lane],status))
        result = -1
    else:
        result = chip.MdioRd(0x40C2)  # fw_cmd_detail_addr = 0x40C2

    return result


##############################################################################
# Get FW Debug Information
##############################################################################
def fw_debug_info(section=2, index=2, lane=None):
    if lane == None:
        lanes = range(8)
    elif type(lane) == int:
        lanes = [lane]
    elif type(lane) == list:
        lanes = lane
    result = {}
    timeout = 0.2
    for ln in lanes:
        result[ln] = fw_debug_cmd(section, index, ln)

    return result


######################################## Added by Jeff
def fw_debugs( lane, mode, index, print_en=False):
    return [fw_debug(lane, mode, i) for i in index]


def fw_debug32(lane, mode, index, print_en=False):
    h = fw_debug(lane, mode, index)
    l = fw_debug(lane, mode, index + 1)
    return (h << 16) + l


def fw_debug_signed(mode, index, print_en=False):
    value = fw_debug(chip_obj, chip, lane, mode, index)
    return value if value < 0x8000 else value - 0x10000


def fw_debug32_signed(lane, mode, index, print_en=False):
    value = fw_debug32(lane, mode, index)
    return value if value < 0x80000000 else value - 0x100000000


def fw_test():
    print('fw_test 123 %d' % (chip.MdioRd(0x40C2)))
    print('fw_test 123 %d' % (chip.MdioRd(0x40C2)))
    print('fw_test 123 0x%04X\n' % (chip.MdioRd(0x40C0)))

    print('fw_test 125 %d' % (chip.NRZ25[0].NRZ_SM_RESET))
    chip.NRZ25[0].NRZ_SM_RESET = 0
    print('fw_test 126 %d' % (chip.NRZ25[0].NRZ_SM_RESET))
    chip.NRZ25[0].NRZ_SM_RESET = 1
    print('fw_test 127 %d' % (chip.NRZ25[0].NRZ_SM_RESET))

    print('fw_test 128 0x%04X' % (chip.MdioRd(0x0181)))

    print('fw_test 125 %d' % (chip.NRZ25[1].NRZ_SM_RESET))
    chip.NRZ25[1].NRZ_SM_RESET = 0
    print('fw_test 128 0x%04X' % (chip.MdioRd(0x0981)))
    print('fw_test 126 %d' % (chip.NRZ25[1].NRZ_SM_RESET))
    chip.NRZ25[1].NRZ_SM_RESET = 1
    print('fw_test 127 %d' % (chip.NRZ25[1].NRZ_SM_RESET))
    print('fw_test 128 0x%04X' % (chip.MdioRd(0x0981)))

    #######
    # BG_TX
    print('fw_test BG_TX lane_0 0x%04X' % (chip.MdioRd(0x00FF)))  # 0xFF [12]
    print('fw_test BG_TX lane_0 0x%04X' % (chip.PAM50[0].PU_TX_BG))
    # BG_RX
    print('fw_test BG_RX lane_0 0x%04X' % (chip.MdioRd(0x01FF)))  # 0xFF [12]
    print('fw_test BG_RX lane_0 0x%04X' % (chip.PAM50[0].PU_RX_BG))

    # BG_TX
    print('fw_test BG_TX lane_1 0x%04X' % (chip.MdioRd(0x08FF)))  # 0xFF [12]
    print('fw_test BG_TX lane_1 0x%04X' % (chip.PAM50[1].PU_TX_BG))
    # BG_RX
    print('fw_test BG_RX lane_1 0x%04X' % (chip.MdioRd(0x09FF)))  # 0xFF [12]
    print('fw_test BG_RX lane_1 0x%04X' % (chip.PAM50[1].PU_RX_BG))

    chip.PAM50[1].PU_TX_BG = 0
    chip.PAM50[1].PU_RX_BG = 0
    print('fw_test BG_TX lane_1 0x%04X' % (chip.MdioRd(0x08FF)))
    print('fw_test BG_RX lane_1 0x%04X' % (chip.MdioRd(0x09FF)))

    if chip.PAM50[0].PU_TX_BG == 0 or chip.PAM50[0].PU_TX_BG == 0:
        print(" ok \n")
    else:
        print(" ng \n")


def fw_pause(fw_mode=None, group=None, lane=None, print_en=1):
    chip.setPhyAddr(group)
    if not fw_loaded(print_en=0):
        print("\n*** No FW Loaded. Skipping fw_pause() \n")
        return

    serdes_fw_addr = 0
    ffe_fw_addr = 128

    off_def = ['OFF', 'DIS', 'PAUSE', 'STOP']
    mode_def = ['off', 'ON']

    lanes = get_lane_list(lane)
    result = {}

    if fw_mode != None:  # Write FW Registers to pause/restart FW
        en = 0 if any(i in fw_mode.upper() for i in off_def) else 1
        if en == 0:
            print "\n...PAUSED FW on Lanes: " + str(lanes)
        else:
            print "\n...Restarted FW on Lanes: " + str(lanes)

        for ln in lanes:
            val1 = fw_reg(addr=serdes_fw_addr, print_en=0)[serdes_fw_addr]
            val2 = fw_reg(addr=ffe_fw_addr, print_en=0)[ffe_fw_addr]
            ####### Proper sequence to Pause FW
            if en == 0:
                ### Pause FFE FW
                ### if Pausing Serdes FW, Pause FFE FW first
                if any(i in fw_mode.upper() for i in ['SERDES', 'PHY', 'FFE', 'TRK', 'TRACKING', 'ALL']):
                    val = (val2 & ~(1 << ln)) | (en << ln)
                    fw_reg(addr=ffe_fw_addr, data=val, print_en=0)
                ### Pause Serdes FW
                if any(i in fw_mode.upper() for i in ['SERDES', 'PHY', 'ALL']):
                    val = (val1 & ~(1 << ln)) | (en << ln)
                    fw_reg(addr=serdes_fw_addr, data=val, print_en=0)
                    ### Other preprations for "No-Serdes FW" mode
                    if gEncodingMode[group][ln][0].upper() == 'NRZ':
                        chip.NRZ25[ln].bp1(0, 0)  # Clear Breakpoint 1
                        chip.NRZ25[ln].bp2(0, 0)  # Clear Breakpoint 2
                        chip.NRZ25[ln].sm_cont_nrz()
                    else:
                        chip.PAM50[ln].bp1(0, 0)  # Clear Breakpoint 1
                        chip.PAM50[ln].bp2(0, 0)  # Clear Breakpoint 2
                        chip.PAM50[ln].sm_cont_pam4()
                    #sm_cont(lane=ln)  # Continue state machine



            ####### Proper sequence to Restart FW
            else:
                ### Restart Serdes FW
                ### if Restarting Serdes FW, Start FFE FW "after" Serdes FW
                if any(i in fw_mode.upper() for i in ['SERDES', 'PHY', 'ALL']):
                    val = (val1 & ~(1 << ln)) | (en << ln)
                    fw_reg(addr=serdes_fw_addr, data=val, print_en=0)
                ### Restart FFE FW
                if any(i in fw_mode.upper() for i in ['SERDES', 'PHY', 'FFE', 'TRK', 'TRACKING', 'ALL']):
                    val = (val2 & ~(1 << ln)) | (en << ln)
                    fw_reg(addr=ffe_fw_addr, data=val, print_en=0)

    ### Read the updated values for the FW Registers
    for ln in range(8):
        status1 = (fw_reg(addr=serdes_fw_addr, print_en=0)[serdes_fw_addr] >> ln) & 1
        status2 = (fw_reg(addr=ffe_fw_addr, print_en=0)[ffe_fw_addr] >> ln) & 1
        result[ln] = status1, status2

    ### Print FW Pause/Active Status for all lanes
    if print_en:
        print("\n-----------------------"),
        print("  FW Pause/Off or Active/ON Status Per Lane"),
        print("  -----------------------"),
        print("\n...     Lane:"),
        for ln in range(8):
            print(" %3s" % (lane_name_list[ln])),
        print("\n...Serdes FW:"),
        for ln in range(8):
            print(" %3s" % (mode_def[result[ln][0]])),
        print("\n...   FFE FW:"),
        for ln in range(8):
            print(" %3s" % (mode_def[result[ln][1]])),
    else:
        return result

def set_logical_physical_map(group=None, tx_val0=0x0, tx_val1=0x1, tx_val2=0x2, tx_val3=0x3, tx_val4=0x4, tx_val5=0x5, tx_val6=0x6, tx_val7=0x7, rx_val0=0x0, rx_val1=0x1, rx_val2=0x2, rx_val3=0x3, rx_val4=0x4, rx_val5=0x5, rx_val6=0x6, rx_val7=0x7):
    if group != None:
        chip.setPhyAddr(group)
    wregBits(0x4000, [14, 13, 12], tx_val0)
    wregBits(0x4000, [10, 9, 8], tx_val1)
    wregBits(0x4000, [6, 5, 4], tx_val2)
    wregBits(0x4000, [2, 1, 0], tx_val3)
    wregBits(0x4001, [14, 13, 12], tx_val4)
    wregBits(0x4001, [10, 9, 8], tx_val5)
    wregBits(0x4001, [6, 5, 4], tx_val6)
    wregBits(0x4001, [2, 1, 0], tx_val7)
    wregBits(0x4002, [14, 13, 12], rx_val0)
    wregBits(0x4002, [10, 9, 8], rx_val1)
    wregBits(0x4002, [6, 5, 4], rx_val2)
    wregBits(0x4002, [2, 1, 0], rx_val3)
    wregBits(0x4003, [14, 13, 12], rx_val4)
    wregBits(0x4003, [10, 9, 8], rx_val5)
    wregBits(0x4003, [6, 5, 4], rx_val6)
    wregBits(0x4003, [2, 1, 0], rx_val7)

def check_logical_physical_map(group=None):
    if group != None:
        chip.setPhyAddr(group)
    L0_TX = rregBits(0x4000, [14, 13, 12])
    L1_TX = rregBits(0x4000, [10, 9, 8])
    L2_TX = rregBits(0x4000, [6, 5, 4])
    L3_TX = rregBits(0x4000, [2, 1, 0])
    L4_TX = rregBits(0x4001, [14, 13, 12])
    L5_TX = rregBits(0x4001, [10, 9, 8])
    L6_TX = rregBits(0x4001, [6, 5, 4])
    L7_TX = rregBits(0x4001, [2, 1, 0])
    L0_RX = rregBits(0x4002, [14, 13, 12])
    L1_RX = rregBits(0x4002, [10, 9, 8])
    L2_RX = rregBits(0x4002, [6, 5, 4])
    L3_RX = rregBits(0x4002, [2, 1, 0])
    L4_RX = rregBits(0x4003, [14, 13, 12])
    L5_RX = rregBits(0x4003, [10, 9, 8])
    L6_RX = rregBits(0x4003, [6, 5, 4])
    L7_RX = rregBits(0x4003, [2, 1, 0])
    phy_lane_tx = [L0_TX, L1_TX, L2_TX, L3_TX, L4_TX, L5_TX, L6_TX, L7_TX]
    phy_lane_rx = [L0_RX, L1_RX, L2_RX, L3_RX, L4_RX, L5_RX, L6_RX, L7_RX]
    if len(phy_lane_tx) != len(set(phy_lane_tx)):
        print '\n Multiple physical Tx are mapping to one logical Tx'
    if len(phy_lane_rx) != len(set(phy_lane_rx)):
        print '\n Multiple physical Rx are mapping to one logical Rx'
    print ('\n+-------------'),
    print ('Group %d Physical to Logical Map' % group),
    print ('-------------+'),
    #print("\n+----------------------------------------------------------+"),
    print '\n|  Phy Ln  ',
    for ln in range(8):
        print ('| %2d ' % (ln)),
    print ('|'),
    print '\n|   Lg Tx  ',
    for ln in range(8):
        print ('| %2s ' % (int(phy_lane_tx[ln], 16))),
    print ('|'),
    print '\n|   Lg Rx  ',
    for ln in range(8):
        print ('| %2s ' % (int(phy_lane_rx[ln], 16))),
    print ('|'),
    print("\n+-----------------------------------------------------------+"),

def Config_BlueJay_GROUP0_7(Group=range(1), datarate=None, input_mode='ac', TX_patt=3, RX_patt=3, broadcast_set=0):
    fw_name = "bluejay.fw.1.00.05.bin"

    hard_reset()
    lanes = get_lane_list(lane=None)
    Group_Reset(Grp_list=Group)
    fw_load_broadcast_mode(group=Group, fw_file_name=fw_name, broadcast_en=broadcast_set)
    if not fw_loaded_new(fw_file_name=fw_name):
        fw_load_broadcast_mode(group=Group, fw_file_name=fw_name, broadcast_en=broadcast_set)
    start_time = time.time()
    for group in Group:
        chip.setPhyAddr(group)
        for ln in lanes:
            nrz_sel = 0x0
            if gEncodingMode[group][ln][1] == 10.3125:
                nrz_sel = 0x1
            elif gEncodingMode[group][ln][1] == 1.25:
                nrz_sel = 0xA
            if gEncodingMode[group][ln][0].upper() == 'NRZ':
                init_lane_for_fw(input_mode='ac', lane=ln, group=group, TX_pat=TX_patt, RX_pat=RX_patt)
                fw_config_cmd(config_cmd=0x80C0+ln, config_detail=nrz_sel)  # set to NRZ 10G mode, if you want to change it back to 25G, you need to set config_detail=0x0000
            else:
                init_lane_for_fw(input_mode='ac', lane=ln, group=group, TX_pat=TX_patt, RX_pat=RX_patt)
                fw_config_cmd(config_cmd=0x80D0 + ln, config_detail=0x0000)

    for group in Group:
        chip.setPhyAddr(group)
        for ln in lanes:
            cnt = 0
            adapt_done = (chip.MdioRd(0x40C7) >> ln) & 1
            while adapt_done != 1 and cnt < 100:
                time.sleep(0.1)
                cnt += 1
                adapt_done = (chip.MdioRd(0x40C7) >> ln) & 1
    stop_time = time.time()
    adapt_time = stop_time - start_time


def Config_BlueJay_GROUP8(Group=[8], datarate=None, input_mode='ac', TX_patt=3, RX_patt=3):
    fw_name ="bluejay_nrz.fw.1.00.05.bin"
    hard_reset()
    lanes = range(4)
    Group_Reset(Grp_list=Group)
    #chip.setPhyAddr(8)
    fw_load_broadcast_mode(group=Group, fw_file_name=fw_name, broadcast_en=0)
    if not fw_loaded_new(fw_file_name=fw_name):
        fw_load_broadcast_mode(group=Group, fw_file_name=fw_name, broadcast_en=0)
    for group in Group:
        start_time = time.time()
        for ln in lanes:
            nrz_sel = 0x0
            if gEncodingMode[group][ln][1] == 10.3125:
                nrz_sel = 0x1
            elif gEncodingMode[group][ln][1] == 1.25:
                nrz_sel = 0xA
            if gEncodingMode[group][ln][0].upper() == 'NRZ':
                init_lane_for_fw(input_mode='ac', lane=ln, group=8, TX_pat=TX_patt, RX_pat=RX_patt)
                fw_config_cmd(config_cmd=0x80C0+ln, config_detail=nrz_sel)  # set to NRZ 10G mode, if you want to change it back to 25G, you need to set config_detail=0x0000
            else:
                print 'Group8 do not support PAM4 mode'
    for ln in lanes:
        cnt = 0
        adapt_done = (chip.MdioRd(0x40C7) >> ln) & 1
        while adapt_done != 1 and cnt < 100:
            time.sleep(0.1)
            cnt += 1
            adapt_done = (chip.MdioRd(0x40C7) >> ln) & 1
    stop_time = time.time()
    adapt_time = stop_time - start_time

def reload_all():
    reload_L2()
    import BaseFunction.Project.BlueJay_L2_nrz as L2
    reload(L2)
    load_basefunction(chip)
    print("Test reload_all\n")

####################################################################################################
#Main Script
####################################################################################################

chip.connect(mdio=False,mode=3)
#lanes = get_lane_list(lane=None)
static_power_down_broadcast_mode(group=range(9), lanes=range(8))  # Broadcast function only for powering down without powering up option
time.sleep(1)

Config_BlueJay_GROUP0_7(Group=range(8), datarate=None, input_mode='ac', TX_patt=3, RX_patt=3, broadcast_set=1)
Config_BlueJay_GROUP8(Group=[8], datarate=None, input_mode='ac', TX_patt=3, RX_patt=3)

for grp in range(9):
    rx_monitor(lane=None, rst=1, lc=0, ph=0, group=grp, print_en=1, returnon=0)   #if you set lane=None or range(8), you will get the result for all the lanes

time.sleep(1)
for group in range(9):
    chip.setPhyAddr(group)
    fw_serdes_params(group=group)




import sys
import time, re, datetime
import math

sys.path.append('../')


#####################################################
# BFN
import struct
import math

# default encoding mode to create ports in
default_mode = 'pam4'
#default_mode = 'nrz'

# default Tx EQ settings (change based on cable/peer)
default_pre2  = 0x0200
default_pre1  = 0xF600
default_main  = 0x1400
default_post1 = 0x0000
default_post2 = 0x0000

# actual settings to use (defaults unless changed)
TX_EQ_pre2  = default_pre2
TX_EQ_pre1  = default_pre1
TX_EQ_main  = default_main
TX_EQ_post1 = default_post1
TX_EQ_post2 = default_post2

test_tiles = [0,1,2,3]
test_grps  = range(8)
test_lns   = range(8)

#test_tiles = [1,2] # for Spirent
#test_grps  = [0,1]
#test_tiles = [0]  # for functional mode loopback
#test_grps  = [6,7]
#test_lns   = range(8)

#test_tiles = [1]  # for Molex DD - 2xQSFP cables
#test_grps  = [0]
#test_lns   = [0]

tile_min   = 0
tile_max   = 4

default_tile_list = range(tile_min, tile_max)

bfn_working_tiles = []
grp_0_7_fw_name = "bluejay.fw.1.00.05.bin"
grp_8_fw_name   = "bluejay_nrz.fw.1.00.05.bin"

bfn_fw_file = grp_0_7_fw_name

DEFAULT_PRBS_CNTR_DWELL         = 0.01
cur_dwell_time                  = DEFAULT_PRBS_CNTR_DWELL
tx_to_rx_loopback_mode          = False
die_to_die_test                 = False #True
die_to_die_test_error_injection = False #True
temp_and_pwr_test               = False #True
#####################################################



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

"""
gEncodingMode.append([]); gEncodingMode[0] = [['pam4', 53.125]] * 8
gEncodingMode.append([]); gEncodingMode[1] = [['pam4', 53.125]] * 8
gEncodingMode.append([]); gEncodingMode[2] = [['nrz', 25.78125]] * 4 + [['pam4', 53.125]]*3 + [['nrz', 25.78125]]
gEncodingMode.append([]); gEncodingMode[3] = [['nrz', 25.78125]] * 2 + [['pam4', 53.125]]*3 + [['nrz', 10.3125]] * 3
gEncodingMode.append([]); gEncodingMode[4] = [['nrz', 25.78125]] * 8
gEncodingMode.append([]); gEncodingMode[5] = [['pam4', 53.125]] * 8
gEncodingMode.append([]); gEncodingMode[6] = [['pam4', 53.125]] * 8
gEncodingMode.append([]); gEncodingMode[7] = [['nrz', 25.78125]] * 8
gEncodingMode.append([]); gEncodingMode[8] = [['nrz', 25.78125]] * 2 + [['nrz', 1.25]] * 1 + [['nrz', 10.3125]] * 5
"""
gEncodingMode.append([]); gEncodingMode[0] = [['pam4', 53.125]] * 8
gEncodingMode.append([]); gEncodingMode[1] = [['pam4', 53.125]] * 8
gEncodingMode.append([]); gEncodingMode[2] = [['pam4', 53.125]] * 8
gEncodingMode.append([]); gEncodingMode[3] = [['pam4', 53.125]] * 8
gEncodingMode.append([]); gEncodingMode[4] = [['pam4', 53.125]] * 8
gEncodingMode.append([]); gEncodingMode[5] = [['pam4', 53.125]] * 8
gEncodingMode.append([]); gEncodingMode[6] = [['pam4', 53.125]] * 8
gEncodingMode.append([]); gEncodingMode[7] = [['pam4', 53.125]] * 8
gEncodingMode.append([]); gEncodingMode[8] = [['pam4', 53.125]] * 8
"""

gEncodingMode.append([]); gEncodingMode[0] = [['nrz', 25.78125]] * 8
gEncodingMode.append([]); gEncodingMode[1] = [['nrz', 25.78125]] * 8
gEncodingMode.append([]); gEncodingMode[2] = [['nrz', 25.78125]] * 8
gEncodingMode.append([]); gEncodingMode[3] = [['nrz', 25.78125]] * 8
gEncodingMode.append([]); gEncodingMode[4] = [['nrz', 25.78125]] * 8
gEncodingMode.append([]); gEncodingMode[5] = [['nrz', 25.78125]] * 8
gEncodingMode.append([]); gEncodingMode[6] = [['nrz', 25.78125]] * 8
gEncodingMode.append([]); gEncodingMode[7] = [['nrz', 25.78125]] * 8
gEncodingMode.append([]); gEncodingMode[8] = [['nrz', 25.78125]] * 8
"""

global TxPeerMap;
TxPeerMap = [];
TxPeerMap.append([]);         TxPeerMap = [0, 1, 2, 3, 4, 5, 6, 7]
global gFecThresh;            gFecThresh=15

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
        #chip.NRZ25[lane].tx_pol_nrz(val=TxPolarityMap[group][lane])
        #chip.NRZ25[lane].rx_pol_nrz(val=RxPolarityMap[group][lane])
    elif gEncodingMode[group][lane][0].upper() == 'PAM4':
        chip.PAM50[lane].RX_AC_COUPLE_EN = 1
        #chip.PAM50[lane].tx_pol_pam4(val=TxPolarityMap[group][lane])
        #chip.PAM50[lane].rx_pol_pam4(val=RxPolarityMap[group][lane])

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
# Set up On-Chip Temperature Sensor (to be read later)
#
####################################################################################################
def bfn_temp_sensor_read():
    set_grp(9)
    base = 0x4B00
    chip.MdioWr(0x4b3e, 0x10d)  # set clock
    time.sleep(0.1)
    chip.MdioWr(0x4b37, 0x0)  # reset sensor
    time.sleep(0.1)
    chip.MdioWr(0x4b37, 0x0C)  # cfg sensor
    time.sleep(0.1)
    rdy = chip.MdioRd(0x4b38)
    while (rdy & 0x100) == 0:
      temp = chip.MdioRd(0x4b39)
      print("sts=" + hex(rdy) + " temp=" + hex(temp))
      time.sleep(1)
      rdy = chip.MdioRd(0x4b38)
    temp = chip.MdioRd(0x4b39)
    print("temp (raw) = " + hex(temp))

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
        #prbs_mode_select(group=group, lane=lane, tx_prbs_mode='functional', rx_prbs_mode='functional', tx_pat=None, rx_pat=None)
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

def rx_tx_external_normal_loopback_new(group=0, lanes=0, enable=1, TX_pat=3, RX_pat=3):
    chip.setPhyAddr(group)
    if lanes == None:
        lanes = range(8)
    elif type(lanes) == int:
        lanes = [lanes]
    elif type(lanes) == list:
        lanes = lanes
    for lane in lanes:
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

        #BFN hack
        print("grp" + str(group) + " ln" + str(lane) + " : bit-rate: " + str(gEncodingMode[group][lane][1]))
        if gEncodingMode[group][lane][1] == 53.125:
            wregBits(0x400D, [15 - (2 * lane), 14 - (2 * lane)], 0x1)
            wregBits(0x400C, [15 - (2 * lane), 14 - (2 * lane)], 0x1)
        elif gEncodingMode[group][lane][1] == 25.78125:
            wregBits(0x400D, [15 - (2 * lane), 14 - (2 * lane)], 0x0)
            wregBits(0x400C, [15 - (2 * lane), 14 - (2 * lane)], 0x0)
        elif gEncodingMode[group][lane][1] == 10.3125:
            wregBits(0x400D, [15 - (2 * lane), 14 - (2 * lane)], 0x3)
            wregBits(0x400C, [15 - (2 * lane), 14 - (2 * lane)], 0x3)
        elif gEncodingMode[group][lane][1] == 1.25:
            wregBits(0x400D, [15 - (2 * lane), 14 - (2 * lane)], 0x2)
            wregBits(0x400C, [15 - (2 * lane), 14 - (2 * lane)], 0x2)
        base_addr = lane * 0x800
        chip.MdioWr(base_addr+0x200, 0x000F)
        chip.MdioWr(base_addr+0x200, 0x0000)

        if enable == 1:
            prbs_mode_select(group=group, lane=lane, tx_prbs_mode='functional', rx_prbs_mode='prbs', tx_pat=None, rx_pat=None)
            #prbs_mode_select(group=group, lane=lane, tx_prbs_mode='functional', rx_prbs_mode='functional', tx_pat=None, rx_pat=None)
        else:
            prbs_mode_select(group=group, lane=lane, tx_prbs_mode='prbs', rx_prbs_mode='prbs', tx_pat=TX_pat, rx_pat=RX_pat)


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

def rx_monitor(lane=None, rst=0, lc=0, ph=0, group=0, print_en=1, returnon=0, capture_en=1):
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

    if capture_en == 1:
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
            #print ("\n Slice %d lane %2d is OFF"%(gSlice,ln)),
            data_rate = 1.0
            gEncodingMode[group][ln] = ['off', data_rate]
            lane_mode_list[ln] = 'off'

        elif chip.NRZ25[ln].TX_NRZ_MODE == 0 and chip.PAM50[ln].PAM4_EN == 1:
            #print ("\n Slice %d lane %2d is PAM4"%(gSlice,ln)),
            data_rate = chip.LANE[ln].get_lane_pll()[0][0]
            gEncodingMode[group][ln] = ['pam4', data_rate]
            lane_mode_list[ln] = 'pam4'
        else:
            #print ("\n Slice %d lane %2d is NRZ"%(gSlice,ln)),
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
            #BFN
            result[reg_addr] = 0
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
        print("... Credo FW Info: Device : "),
        print("'%s'," % (fw_bin_filename)),
        print("%s," % (ver_str)),
        print("%s," % (date_str)),
        print("%s," % (hash_str)),
        print("%s," % (crc_str)),
        print("%s" % (magic_str))

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
    #if not fw_loaded(print_en=False):
    if not fw_loaded(print_en=True):
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
                #BFN
                #print("ctle_val=" + str(ctle_val) + " ln=" + str(ln))
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
"""
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

"""



#############
# BFN defs
#############
import datetime
from parse_board_file import *

g_cur_tile = 0
g_cur_grp = 0

#######################################################################################################################
# print_date_time
#
# Print date and time string
#
def print_date_time():
  print datetime.datetime.now().strftime("%a, %d %B %Y %H:%M:%S")

#######################################################################################################################
# lanes_in_group
#
# Return list of valid lanes in a given group
#
# Group 0-7 have 8 lanes each (is capable of (10/25G NRZ and 50G PAM4)
# Group 8 has only 4 lanes (is capable of 1G and 10/25g NRZ)
#
def lanes_in_group(g):
  if g == 8:
    group_range = [0,1,2,3]
  else:
    group_range = [0,1,2,3,4,5,6,7]
  return group_range

#######################################################################################################################
# set_tile
#
# Set tile # in jtag access
#
def set_tile(tile):
  global g_cur_tile
  chip.setTileAddr(tile)
  g_cur_tile = tile

#######################################################################################################################
# set_grp
#
# Set group # in jtag access
#
def set_grp(g):
  global g_cur_grp
  chip.setPhyAddr(g)
  g_cur_grp = g

#######################################################################################################################
# reg_rd
#
# Read a tile register via jtag/mdio
#
def reg_rd(reg):
  data = chip.MdioRd(reg)
  return data

#######################################################################################################################
# reg_wr
#
# Write a tile register via jtag/mdio
#
def reg_wr(reg, data):
  chip.MdioWr(reg, data)

#######################################################################################################################
# run_post
#
# Run Power-On Self-Test (POST) to verify accessibility of all tiles, groups, and lanes
#
def run_post(tile, g):
  failed = False
  ln_list = lanes_in_group(g)
  set_grp(g)

  for ln in ln_list:
    post_reg = 0xA1 + (ln<<11)
    # Use 0xA1 as a test register. It normally contains 0xAAAA
    data = reg_rd(post_reg)
    if (data != 0xAAAA):
        print(str(hex(post_reg)) + " : " + str(hex(data)) + " : re-setting to 0xAAAA")
        reg_wr(post_reg, 0xAAAA)
        data = reg_rd(0xA1)
        if (data != 0xAAAA):
            print(str(hex(post_reg)) + " : " + str(hex(data)) + " : Still not 0xAAAA")
            failed = True
            #return -1
    for attempt in range(1,10):
        reg_wr(post_reg, attempt)
        data = reg_rd(post_reg)
        if (data != attempt):
            print(str(hex(post_reg)) + " : " + str(hex(data)) + " : attempt : " + str(attempt) + " : read-back failed : " + str(hex(data)))
            failed = True
    if failed:
      print("Tile " + str(tile) + " grp" + str(g) + " ln" + str(ln) + ": ** FAILED **")
    else:
      print("Tile " + str(tile) + " grp" + str(g) + " ln" + str(ln) + ":    PASSED")
    # reset to default value
    reg_wr(post_reg, 0xAAAA)

  return failed


####################################################################################################
# bfn_post
#
# Run POST on requested tiles. All passing tiles are added to bfn_working_tiles list for
# subsequent enumerations
#
def bfn_post(tile_list):
  for tile in tile_list:
    set_tile(tile)
    failed_post = False
    for g in range(9):
      failed = run_post(tile, g)
      if failed:
        failed_post = True

    if not failed_post:
      bfn_working_tiles.append(tile)

####################################################################################################
# bfn_prbs_mode_select
#
# BFN copy of prbs_mode_select that doesn't depend on an global variables
#
def bfn_prbs_mode_select(mode='pam4', group=None, lane=None, tx_prbs_mode='prbs', rx_prbs_mode='prbs', tx_pat=None, rx_pat=None):
    set_grp(group)
    #if mode == 'NRZ':
    if mode.upper() == 'NRZ':
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

####################################################################################################
# bfn_set_lane_mode
#
# BFN copy of set_lane_mode that doesn't depend on an global variables
#
def bfn_set_lane_mode(mode='pam4', g=None, ln=None):
    set_grp(g)
    if mode.upper() !='PAM4':
        chip.PAM50[ln].PAM4_EN = 0
        chip.PAM50[ln].TX_PRBS_CLK_EN = 0
        chip.NRZ25[ln].TX_NRZ_MODE = 1
        chip.NRZ25[ln].TX_NRZ_PRBS_GEN_EN = 1
    else:
        chip.NRZ25[ln].TX_NRZ_MODE = 0
        chip.NRZ25[ln].TX_NRZ_PRBS_GEN_EN = 0
        chip.PAM50[ln].PAM4_EN = 1
        chip.PAM50[ln].TX_PRBS_CLK_EN = 1

####################################################################################################
# bfn_get_lane_mode
#
# BFN copy of set_lane_mode that doesn't depend on an global variables
#
def bfn_get_lane_mode(tile, g, ln):
  set_tile(tile)
  set_grp(g)
  pam4_en = chip.PAM50[ln].PAM4_EN
  if pam4_en == 1: return 'pam4'
  return 'nrz' 


####################################################################################################
# bfn_init_lane_for_fw
#
# BFN copy of init_lane_for_fw that doesn't depend on an global variables
#
def bfn_init_lane_for_fw(mode='pam4', input_mode='ac', lane=None, group = None, TX_pat=3, RX_pat=3):
    set_grp(group)

    bfn_set_lane_mode(mode=mode, g=group, ln=lane)

    ####################### put lane in PAM4 mode
    if (mode.upper() == 'PAM4'):
        chip.PAM50[lane].TX_POST2_SCALE = 0
        chip.PAM50[lane].TX_POST1_SCALE = 0
        chip.PAM50[lane].TX_MAIN_SCALE = 1
        chip.PAM50[lane].TX_PRE1_SCALE = 0
        chip.PAM50[lane].TX_PRE2_SCALE = 0

        chip.PAM50[lane].tx_taps(2, -8, 17, 0, 0)

        chip.PAM50[lane].gc(1, 1)
        chip.PAM50[lane].pc(0, 0)
        chip.PAM50[lane].msblsb(0, 0)

        bfn_prbs_mode_select(mode, group=group, lane=lane, tx_prbs_mode='prbs', rx_prbs_mode='prbs', tx_pat=TX_pat, rx_pat=RX_pat)

    ####################### put lane in NRZ mode
    else:
        chip.PAM50[lane].TX_POST2_SCALE = 0
        chip.PAM50[lane].TX_POST1_SCALE = 0
        chip.PAM50[lane].TX_MAIN_SCALE = 1
        chip.PAM50[lane].TX_PRE1_SCALE = 0
        chip.PAM50[lane].TX_PRE2_SCALE = 0

        chip.NRZ25[lane].tx_taps(0, -8, 17, 0, 0)

        chip.PAM50[lane].gc(0, 0)
        chip.PAM50[lane].pc(0, 0)
        chip.PAM50[lane].msblsb(0, 0)

        bfn_prbs_mode_select(mode, group=group, lane=lane, tx_prbs_mode='prbs', rx_prbs_mode='prbs', tx_pat=TX_pat, rx_pat=RX_pat)

    chip.PAM50[lane].Reg007B_3 = 1
    chip.NRZ25[lane].Reg0100_007C_15 = 0

    if mode == 'NRZ':
        chip.NRZ25[lane].RX_AC_COUPLE_EN = 1
    else:
        chip.PAM50[lane].RX_AC_COUPLE_EN = 1

#######################################################################################################################
# if you want to enable tx_rx loopback, you need to connect another lane's rx to test lane's tx as a termination      #
#######################################################################################################################
def bfn_tx_rx_serial_loopback(group=None, lane=0, enable=1, phase=0):
    if group != None:
        chip.setPhyAddr(group)
    if gEncodingMode[group][lane][0].upper() == 'NRZ':
        if enable == 1:
          if phase == 0:
            chip.NRZ25[lane].tx_taps(0, 0, 15, -4, 0)
            chip.NRZ25[lane].Reg01E7_13 = 1
            chip.NRZ25[lane].Reg00FF_3 = 1
            chip.NRZ25[lane].Reg0100_0001_14 = 0
            #time.sleep(3)
          elif phase == 1:
            fw_reg(addr=0x8, data=0xffff-2**lane) # inactive FW
            chip.MdioWr(0x10b + 0x800 * lane, 0x0)
            chip.NRZ25[lane].NRZ_SM_CONT = 0
            chip.NRZ25[lane].NRZ_SM_CONT = 1
            chip.NRZ25[lane].agcgain(0, 0)
            chip.NRZ25[lane].ctle_nrz(7)
            #time.sleep(0.1)
          else:
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


####################################################################################################
# bfn_group_reset_all
#
# Reset all groups in the requested tiles (at once)
#
def bfn_group_reset_all(working_tiles):
    for seq in range(3):
      for tile in working_tiles:
        set_tile(tile)
        for g in range(9):
          set_grp(g)
          if seq == 0:
            soft_reset()
          elif seq == 1:
            logic_reset()
          else:
            ln_list = lanes_in_group(g)
            for ln in ln_list:
              chip.ANLT_TOP[ln].Tr_Reg0000_1 = 0
              chip.ANLT_TOP[ln].Tr_Reg0000_0 = 0
      if seq < 2:
        time.sleep(0.1)

####################################################################################################
# bfn_set_lane_map
#
# Configure the (swizzled) lane map registers
#
def bfn_lane_map_set(working_tiles):
  for tile in bfn_working_tiles:
    set_tile(tile)
    if tile == 0:
      num_grp = 9
    else:
      num_grp = 8
    for g in range(num_grp):
        set_grp(g)
        r80000,r80001,r80002,r80003 = lane_map_get(tile, g)
        reg_wr(0x4000, r80000)
        reg_wr(0x4001, r80001)
        reg_wr(0x4002, r80002)
        reg_wr(0x4003, r80003)
        #print("Tile " + str(tile) + " grp" + str(g) + " : " + hex(r80000) + " : " + hex(r80001) + " : " + hex(r80002) + " : " + hex(r80003))

  bfn_check_all_lane_map(working_tiles, range(9))
  print("")

####################################################################################################
# bfn_fw_load_all_groups
#
# Load FW on all groups on the current tile
#
def bfn_is_fw_loaded_all_groups(tile, fw_file, file_hash_code, file_crc_code, optional = False):
  set_tile(tile)
  force = not optional
  load_reqd = False
  for g in range(8):
    set_grp(g)
    running_hash = fw_hash()
    running_crc = fw_crc()

    if (running_hash != file_hash_code) or (running_crc != file_crc_code):
      print("grp" + str(g) + " Hash Code : 0x%06x" % running_hash + " : CRC Code : 0x%04x" % running_crc + " : FW Load reqd")
      load_reqd = True
      break
  if force != 0:
    load_reqd = True # force reload
  if not load_reqd:
    print("Tile " + str(tile) + ": All groups already running : " + fw_file)
    return True
  return False

####################################################################################################
# bfn_fw_load_all_tiles
#
# Load FW on all requested tiles
#
def bfn_fw_load_tile(tile):
  set_tile(tile)
  print("Tile " + str(tile) + ":")
  fw_load_broadcast_mode(group=range(8), fw_file_name=grp_0_7_fw_name, broadcast_en=1)
  print("")
  fw_load_broadcast_mode(group=[8], fw_file_name=grp_8_fw_name, broadcast_en=0)
  print("")

####################################################################################################
# bfn_fw_load_all_tiles
#
# Load FW on all requested tiles
#
def bfn_fw_load_all_tiles(working_tiles, fw_file, optional=1):
    fw_file_ptr = open(fw_file, 'rb')
    fw_data = fw_file_ptr.read()
    start = 4096
    file_hash_code = struct.unpack_from('>I', fw_data[start   :start+4 ])[0]
    file_crc_code  = struct.unpack_from('>H', fw_data[start+4 :start+6 ])[0]
    file_date_code = struct.unpack_from('>H', fw_data[start+6 :start+8 ])[0]
    entryPoint     = struct.unpack_from('>I', fw_data[start+8 :start+12])[0]
    length         = struct.unpack_from('>I', fw_data[start+12:start+16])[0]
    ramAddr        = struct.unpack_from('>I', fw_data[start+16:start+20])[0]
    data           = fw_data[start+20:]

    d=datetime.date(1970, 1, 1) + datetime.timedelta(file_date_code)

    print "fw_load Hash Code : 0x%06x" % file_hash_code
    print "fw_load Date Code : 0x%02x (%04d-%02d-%02d)" % (file_date_code, d.year, d.month, d.day)
    print "fw_load  CRC Code : 0x%04x" % file_crc_code
    print "fw_load    Length : %d" % length
    print "fw_load     Entry : 0x%08x" % entryPoint
    print "fw_load       RAM : 0x%08x" % ramAddr
    fw_file_ptr.close()

    for tile in bfn_working_tiles:
      set_tile(tile)
      ok = bfn_is_fw_loaded_all_groups(tile, fw_file, file_hash_code, file_crc_code, optional)
      if not ok:
        bfn_fw_load_tile(tile)
      if (tile == 0): # load CPU port
        print("Tile " + str(tile) + ": load group8")
        #fw_load_broadcast_mode(group=[8], fw_file_name=grp_8_fw_name, broadcast_en=0)
        print("")

####################################################################################################
# bfn_dump_bist
#
# Dump BIST results
#
def bfn_bist_results_get():
  r402f = reg_rd(0x402f)
  r4030 = reg_rd(0x4030)
  r4031 = reg_rd(0x4031)
  r4032 = reg_rd(0x4032)
  return r402f, r4030, r4031, r4032

####################################################################################################
# bfn_rx_tx_external_dft_loopback
#
# 
#
def bfn_rx_tx_external_dft_loopback(chip, group=None, lane=None, t=0.1, pat_gen=3, pat_chk=3, en=1):
    set_grp(group)
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
        #time.sleep(t)
    else:
        if gEncodingMode[group][lane][0].upper() == 'PAM4':
            chip.MdioWr(0x410C + lane * 0x20, (0x3001 & 0xE7FF) | (pat_chk << 11)) #turn off PRBS checker
            chip.MdioWr(0x4107 + lane * 0x20, (0x0A00 & 0xFCFF) | (pat_gen << 8))  #turn off PRBS gen
            chip.PAM50[lane].TX_PLL_N = 85
            chip.PAM50[lane].Reg00FF_1 = 1
            chip.PAM50[lane].RX_REFCLK_DIV4_EN = 0
            chip.PAM50[lane].Reg00D7_13 = 0
            chip.PAM50[lane].Reg00D7_15_14 = 0
            chip.PAM50[lane].Reg00D9_3_0S = 0

####################################################################################################
# bfn_check_rx_tx_external_dft_loopback
#
# 
#
def bfn_check_rx_tx_external_dft_loopback(chip, group=None, lane=None):
    set_grp(group)
    PRBS_check = reg_rd(0x4102 + lane * 0x20)
    if PRBS_check != 0x8FFF:
        print 'PRBS_check = ', hex(PRBS_check)
        print 'External loopback with fault'
    err_cycle_1 = reg_rd(0x4113 + lane * 0x20)
    err_cycle_2 = reg_rd(0x4114 + lane * 0x20)
    if (err_cycle_1 != 0) or (err_cycle_2 != 0):
        print 'Cycle number of last error is', hex((err_cycle_1 << 16) + err_cycle_2)



#######################################################################################################################
# set_external_digital_loopback_checker
#
# Set up or tear down the external digital loopback checker.
# phase 0: set up
# phase 1: tear down
#
def set_external_digital_loopback_checker(tile, group=None, lane=None, t=0.1, pat_gen=3, pat_chk=3, phase=0):
    set_tile(tile)
    set_grp(group)
    if phase == 0:
      #reg_wr(0x4107+lane*0x20, (0x1A00&0xFCFF)|(pat_gen<<8))
      reg_wr(0x410C+lane*0x20, (0x3201&0xE7FF)|(pat_chk<<11))
      #reg_wr(0x4107+lane*0x20, (0x1E00&0xFCFF)|(pat_gen<<8))  #turn on PRBS gen
      #time.sleep(t)
    elif phase == 1:
      reg_wr(0x410C+lane*0x20, (0x3001&0xE7FF)|(pat_chk<<11))  #turn off PRBS checker
      #reg_wr(0x4107+lane*0x20, (0x0A00&0xFCFF)|(pat_gen<<8))  #turn off PRBS gen

"""
#######################################################################################################################
# set_external_digital_loopback
#
# Set up or tear down the external digital loopback.
# phase 0: set up
# phase 1: tear down
#
def set_external_digital_loopback(chip, group=None, lane=None, t=0.1, pat_gen=3, pat_chk=3, phase=0):
    chip.setPhyAddr(group)
    if phase == 0:
      reg_wr(0x4107+lane*0x20, (0x1A00&0xFCFF)|(pat_gen<<8))
      reg_wr(0x410C+lane*0x20, (0x3201&0xE7FF)|(pat_chk<<11))
      reg_wr(0x4107+lane*0x20, (0x1E00&0xFCFF)|(pat_gen<<8))  #turn on PRBS gen
      #time.sleep(t)
    elif phase == 1:
      reg_wr(0x410C+lane*0x20, (0x3001&0xE7FF)|(pat_chk<<11))  #turn off PRBS checker
      reg_wr(0x4107+lane*0x20, (0x0A00&0xFCFF)|(pat_gen<<8))  #turn off PRBS gen
"""

#######################################################################################################################
# check_external_digital_loopback
#
# Get and print status of an external digital loopback test.
#
def check_external_digital_loopback(chip, group=None, lane=None):
    chip.setPhyAddr(group)
    PRBS_check0 = reg_rd(0x4100+lane*0x20)
    PRBS_check1 = reg_rd(0x4101+lane*0x20)
    PRBS_check2 = reg_rd(0x4102+lane*0x20)
    err_cycle_1 = reg_rd(0x4113+lane*0x20)
    err_cycle_2 = reg_rd(0x4114+lane*0x20)
    if (err_cycle_1 != 0) or (err_cycle_2 != 0):
      if PRBS_check2 != 0x8FFF:
          print 'PRBS_check = ', hex(PRBS_check2),
          print 'External loopback with fault',
      print 'Cycle number of last error is', hex((err_cycle_1<<16) + err_cycle_2)
    return PRBS_check0, PRBS_check1, PRBS_check2, err_cycle_1, err_cycle_2

#######################################################################################################################
# bfn_quick_die2die_loopback_set
#
# Run an external digital loopback test on tile0 grp0 ln0
#
def bfn_quick_die2die_loopback_set(working_tiles):
  banner("Die-to-Die Loopback Setup")
  tile = 0
  chip.setTileAddr(tile)
  g = 0
  chip.setPhyAddr(g)
  ln = 0

  set_external_digital_loopback(chip, g, ln, t=0.1, pat_gen=3, pat_chk=3,phase=0)
  #time.sleep(0.1)
  time.sleep(10)
  set_external_digital_loopback(chip, g, ln, t=0.1, pat_gen=3, pat_chk=3,phase=1)

  banner("Die-to-Die Loopback Results")
  sts, e1, e2 = check_external_digital_loopback(chip, g, ln)
  if (e1 != 0) or (e2 != 0):
    print("Tile " + str(tile) + ": grp" + str(g) + ": ln" + str(ln) + ": "),
    print("PRBS_check=" + str(hex(sts)) + " : err_cycle_1=" + str(hex(e1)) + " : err_cycle_2=" + str(hex(e2)))
    no_errors = False
  else:
    print("Tile " + str(tile) + ": grp" + str(g) + ": ln" + str(ln) + ": "),
    print("PRBS_check=" + str(hex(sts)) + " : err_cycle_1=" + str(hex(e1)) + " : err_cycle_2=" + str(hex(e2)))

#######################################################################################################################
# inject_error_in_external_digital_loopback
#
# Inject a bit error in one of the three ranges
#
def inject_error_in_external_digital_loopback(tile, g, ln, fld):
  chip.setTileAddr(tile)
  chip.setPhyAddr(g)
  if fld == 0:
    inj_reg = 0x4107+ln*0x20
  elif fld == 1:
    inj_reg = 0x4108+ln*0x20
  elif fld == 2:
    inj_reg = 0x4109+ln*0x20

  errval = reg_rd(inj_reg)
  errval |= 1
  reg_wr(inj_reg, errval)
  print(str(hex(inj_reg)) + " = " + str(hex(errval)))
  errval &= ~1
  reg_wr(inj_reg, errval)
  print(str(hex(inj_reg)) + " = " + str(hex(errval)))

#######################################################################################################################
# inject_error_in_external_digital_loopback
#
# Inject a bit error in one of the three ranges
#
def inject_multibit_error_in_external_digital_loopback(tile, g, ln, fld, mask):
  chip.setTileAddr(tile)
  chip.setPhyAddr(g)
  if fld == 0:
    inj_reg = 0x4107+ln*0x20
  elif fld == 1:
    inj_reg = 0x4108+ln*0x20
  elif fld == 2:
    inj_reg = 0x4109+ln*0x20

  errval = reg_rd(inj_reg)
  errval |= mask
  reg_wr(inj_reg, errval)
  print(str(hex(inj_reg)) + " = " + str(hex(errval)))
  errval &= ~mask
  reg_wr(inj_reg, errval)
  print(str(hex(inj_reg)) + " = " + str(hex(errval)))

#######################################################################################################################
# bfn_die2die_loopback_set
#
# Run an external digital loopback test on all lanes of all working tiles
#
def bfn_die2die_loopback_set(working_tiles, dwell_time=1.0, pat_gen=3, pat_chk=3):
  banner("Die-to-Die Loopback Setup")
  for tile in working_tiles:
    set_tile(tile)
    for g in range(0,8):
      set_grp(g)
      for ln in range(0,8):
        bfn_rx_tx_external_dft_loopback(chip, group=g, lane=ln, t=0.1, pat_gen=pat_gen, pat_chk=pat_chk, en=1)

  if die_to_die_test_error_injection:
    print("Force a multi-bit  error on Tile 0, grp0 ln0")
    inject_multibit_error_in_external_digital_loopback(0, 0, 0, 0, 3)

    #print("Force an error on Tile 0, grp0 ln0")
    #inject_error_in_external_digital_loopback(0, 0, 0, 0)
    #print("Force an error on Tile 0, grp0 ln0 (in different bits)")
    #inject_error_in_external_digital_loopback(0, 0, 0, 1)
    print("Force an error on Tile 1, grp2 ln3")
    inject_error_in_external_digital_loopback(1, 2, 3, 1)
    print("Force an error on Tile 2, grp2 ln2")
    inject_error_in_external_digital_loopback(2, 2, 2, 2)

  #print("Wait " + str(dwell_time) + " seconds for errors..")
  #time.sleep(dwell_time)
  print("=========================================") 
  print_date_time()
  print("Collecting data..:")
  inp = raw_input("Enter key to continue> ")
  print_date_time()

  for tile in working_tiles:
    chip.setTileAddr(tile)
    for g in range(0,8):
      chip.setPhyAddr(g)
      for ln in range(0,8):
        bfn_rx_tx_external_dft_loopback(chip, group=g, lane=ln, t=0.1, pat_gen=pat_gen, pat_chk=pat_chk, en=0)

  banner("Die-to-Die Loopback Results")
  for tile in working_tiles:
    chip.setTileAddr(tile)
    for g in range(0,8):
      chip.setPhyAddr(g)
      for ln in range(0,8):
        sts0, sts1, sts2, e1, e2 = check_external_digital_loopback(chip, g, ln)
        if (e1 != 0) or (e2 != 0):
          print("Tile " + str(tile) + ": grp" + str(g) + ": ln" + str(ln) + ": "),
          print("PRBS_check=" + str(hex(sts0)) + " : " + str(hex(sts1)) + " : " + str(hex(sts2)) + " : err_cycle_1=" + str(hex(e1)) + " : err_cycle_2=" + str(hex(e2)))
          no_errors = False
        else:
          print("Tile " + str(tile) + ": grp" + str(g) + ": ln" + str(ln) + ": "),
          print("PRBS_check=" + str(hex(sts0)) + " : " + str(hex(sts1)) + " : " + str(hex(sts2)) + " : err_cycle_1=" + str(hex(e1)) + " : err_cycle_2=" + str(hex(e2)))


#######################################################################################################################
# dump_die2die_loopback_status
#
# Run an external digital loopback test on all lanes of all working tiles
#
def dump_die2die_loopback_status(tile_list, grp_list):
  banner("Die-to-Die Loopback Results")
  for tile in tile_list:
    set_tile(tile)
    for g in grp_list:
      set_grp(g)
      for ln in range(0,8):
        sts0, sts1, sts2, e1, e2 = check_external_digital_loopback(chip, g, ln)
        if (e1 != 0) or (e2 != 0):
          print("Tile " + str(tile) + ": grp" + str(g) + ": ln" + str(ln) + ": "),
          print("PRBS_check=" + str(hex(sts0)) + " : " + str(hex(sts1)) + " : " + str(hex(sts2)) + " : err_cycle_1=" + str(hex(e1)) + " : err_cycle_2=" + str(hex(e2)))
          no_errors = False
        else:
          print("Tile " + str(tile) + ": grp" + str(g) + ": ln" + str(ln) + ": "),
          print("PRBS_check=" + str(hex(sts0)) + " : " + str(hex(sts1)) + " : " + str(hex(sts2)) + " : err_cycle_1=" + str(hex(e1)) + " : err_cycle_2=" + str(hex(e2)))

#######################################################################################################################
# bfn_configure_die2die_checker
#
# Run an external digital loopback test on all lanes of all working tiles
#
def bfn_configure_die2die_checker(tile_list, grp_list, phase=0, pat_gen=3, pat_chk=3):
  banner("Die-to-Die Loopback Checker Enable")
  for tile in tile_list:
    set_tile(tile)
    for g in grp_list:
      set_grp(g)
      for ln in range(0,8):
        set_external_digital_loopback_checker(tile, g, ln, 0.1, pat_gen, pat_chk, phase)

#######################################################################################################################
# bfn_tx_eq_set_2_on_lanes
#
# Set all lanes to a specific set of PRE/MAIN/POST values
#
def bfn_tx_eq_set_2_on_lanes(tile, g, ln_list, pre2, pre, main, post, post2):
  set_tile(tile)
  set_grp(g)
  for ln in ln_list:
    reg_wr(0xA5 | (ln<<11), pre2)  #PRE2
    reg_wr(0xA7 | (ln<<11), pre)   #PRE1
    reg_wr(0xA9 | (ln<<11), main)  #MAIN
    reg_wr(0xAB | (ln<<11), post)  #POST1
    reg_wr(0xAD | (ln<<11), post2) #POST1

    bfn_lane_reset(tile, g, ln)

#######################################################################################################################
# bfn_tx_eq_set_2_on_grps
#
# Set all lanes to a specific set of PRE/MAIN/POST values
#
def bfn_tx_eq_set_2_on_grps(tile, grp_list, pre2, pre, main, post, post2):
  for g in grp_list:
    ln_list = lanes_in_group(g)
    bfn_tx_eq_set_2_on_lanes(tile, g, ln_list, pre2, pre, main, post, post2)

#######################################################################################################################
# bfn_tx_eq_set_2
#
# Set all lanes to a specific set of PRE/MAIN/POST values
#
def bfn_tx_eq_set_2(tile_list, pre2, pre, main, post, post2):
  for tile in tile_list:
    bfn_tx_eq_set_2_on_grps(tile, range(8), pre2, pre, main, post, post2)

#######################################################################################################################
# bfn_tx_eq_set
#
# Set all lanes to a specific set of PRE/MAIN/POST values
#
def bfn_tx_eq_set(tile_list, pre, main, post):
  bfn_tx_eq_set_2(tile_list, 0, pre, main, post, 0)

#######################################################################################################################
# bfn_tx_eq_get
#
# Return current settings of PRE/MAIN/POST
#
def bfn_tx_eq_get(tile, g, ln):
  chip.setTileAddr(tile)
  chip.setPhyAddr(g)
  pre = reg_rd(0xA7 | (ln<<11)) #PRE1
  main = reg_rd(0xA9 | (ln<<11)) #MAIN
  post = reg_rd(0xAB | (ln<<11)) #POST1
  return pre, main, post

#######################################################################################################################
# bfn_tx_eq_settings_dump
#
# Dump PRE2/PRE1/MAIN/POST1/POST2, default and actual (usu the same) 
#
def bfn_tx_eq_settings_dump():
  print("PRE2  : " + hex(TX_EQ_pre2) + " <default: " + hex(default_pre2) + ">")
  print("PRE1  : " + hex(TX_EQ_pre1) + " <default: " + hex(default_pre1) + ">")
  print("MAIN  : " + hex(TX_EQ_main) + " <default: " + hex(default_main) + ">")
  print("POST1 : " + hex(TX_EQ_post1) + " <default: " + hex(default_post1) + ">")
  print("POST2 : " + hex(TX_EQ_post2) + " <default: " + hex(default_post2) + ">")

#######################################################################################################################
# bfn_tx_eq_dump
#
# Dump PRE/MAIN/POST values of all lanes
#
def bfn_tx_eq_dump(tile_list, grp_list, ln_list):
  for tile in tile_list:
    print("Tile " + str(tile) + ":")
    print("+------+-----------------+----------------+----------------+----------------+----------------+----------------+----------------+----------------+")
    for g in grp_list:
      print("| grp" + str(g) + " | "),
      for ln in ln_list:
        pre, main, post = bfn_tx_eq_get(tile, g, ln)
        print("%04x"%pre + "-%04x"%main + "-%04x |"%post),
      print("")
  print("+------+-----------------+----------------+----------------+----------------+----------------+----------------+----------------+----------------+")

#######################################################################################################################
# bfn_pn_swap_set
#
# Apply board settings
#
def bfn_pn_swap_set(working_tiles):
  for tile in working_tiles:
    set_tile(tile)
    for g in range(0,8):
      set_grp(g)
      for ln in range(0,8):
        tx_pn, rx_pn = lane_PN_swap_get(tile, g, ln)
        ln_mode = bfn_get_lane_mode(tile, g, ln)
        if ln_mode == 'pam4':
          tx_pn = 1 - tx_pn # 1 == NO INVERT
          chip.PAM50[ln].tx_pol_pam4(val=tx_pn)
          chip.PAM50[ln].rx_pol_pam4(val=rx_pn)
        else:
          tx_pn = 1 - tx_pn # 1 == NO INVERT
          chip.NRZ25[ln].tx_pol_nrz(val=tx_pn)
          chip.NRZ25[ln].rx_pol_nrz(val=rx_pn)

#######################################################################################################################
# bfn_external_loopback_set
#
# Configure die-to-die loopback 
#
def bfn_external_loopback_set():
  banner("Die-to-Die Loopback Setup")
  for tile in bfn_working_tiles:
    chip.setTileAddr(tile)
    for g in range(0,8):
      chip.setPhyAddr(g)
      for ln in range(0,8):
        set_external_digital_loopback(chip, g, ln, t=0.1, pat_gen=3, pat_chk=3,phase=0)


FW_CMD = 0x40C1
FW_CMD_DETAIL = 0x40C2
CMD_DEBUG_INFO = 0xB
CMD_DESTROY = 0x9
CMD_CONFIG = 0x8
MODE_NRZ = 0xC
MODE_PAM4 = 0xD

DEBUG_INFO_MODE_TOP = 0
DEBUG_INFO_MODE_NRZ = 1
DEBUG_INFO_MODE_PAM4 = 2
DEBUG_INFO_MODE_TOP_INFO =7

fw_cmd_dbg = 0


def bfn_fw_info(tile, g):
  set_tile(tile)
  set_grp(g)
  print("tile " + str(tile) + " grp" + str(g) + " : "),
  fw_info(print_en=True)

def bfn_all_fw_info(tile_list, grp_list):
  for tile in tile_list:
    for g in grp_list:
      bfn_fw_info(tile, g)

def group_reset_all(chip, tile_min, tile_max):
    for seq in range(0,3):
      for t in range(tile_min, tile_max):
        set_tile(t)
        for g in range(0, 8):
          set_grp(g)
          if seq == 0:
            soft_reset()
          elif seq == 1:
            logic_reset()
          else:
            for lane in range(8):
              chip.ANLT_TOP[lane].Tr_Reg0000_1 = 0
              chip.ANLT_TOP[lane].Tr_Reg0000_0 = 0
      if seq < 2:
        time.sleep(0.1)

#######################################################################################################################
# bfn_config_ln_nrz
#
# Configure NRZ on the specified tile, group, and ln
#
def bfn_config_ln_nrz(tile, g, ln, prbs_mode='PRBS31-NRZ', speed=25):
  set_tile(tile)
  set_grp(g)

  if speed == 25:
    gEncodingMode[g][ln] = ['nrz', 25.78125]
  elif speed == 10:
    gEncodingMode[g][ln] = ['nrz', 10.3125]
  else:
    gEncodingMode[g][ln] = ['nrz', 1.25]

  bfn_init_lane_for_fw(mode='nrz', input_mode='ac', lane=ln, group = g, TX_pat=3, RX_pat=3)
  if speed == 25:
    fw_config_cmd(config_cmd=0x80C0+ln, config_detail=0x0)  # set to 0x1 for NRZ 10G mode, if you want to change it back to 25G, you need to set config_detail=0x0000
  if speed == 10:
    fw_config_cmd(config_cmd=0x80C0+ln, config_detail=0x1)  # set to 0x1 for NRZ 10G mode, if you want to change it back to 25G, you need to set config_detail=0x0000
  else:
    fw_config_cmd(config_cmd=0x80C0+ln, config_detail=0xA)  # set to 0x1 for NRZ 10G mode, if you want to change it back to 25G, you need to set config_detail=0x0000
  if prbs_mode=='functional':
      bfn_prbs_mode_select(mode='nrz', group=g, lane=ln, tx_prbs_mode='functional', rx_prbs_mode='functional', tx_pat=None, rx_pat=None)
  else:
      bfn_prbs_mode_select(mode='nrz', group=g, lane=ln, tx_prbs_mode='prbs', rx_prbs_mode='prbs', tx_pat=prbs_mode, rx_pat=prbs_mode)

#######################################################################################################################
# bfn_config_lanes_nrz
#
# Configure NRZ on all lanes of the tile and group
#
def bfn_config_lanes_nrz(tile, g, ln_list, prbs_mode='PRBS31', speed=25):
  for ln in ln_list:
    bfn_config_ln_nrz(tile, g, ln, prbs_mode, speed)

#######################################################################################################################
# bfn_config_grps_nrz
#
# Configure NRZ on all lanes of the tile and group list
#
def bfn_config_grps_nrz(tile, grp_list, prbs_mode='PRBS31', speed=25):
  for g in grp_list:
    bfn_config_lanes_nrz(tile, g, range(8), prbs_mode, speed)

#######################################################################################################################
# bfn_config_all_lanes_nrz
#
# Configure NRZ on all groups/lanes of the tile list
#
def bfn_config_all_lanes_nrz(tile_list, prbs_mode='PRBS31', speed=25):
  for tile in tile_list:
    bfn_config_grps_nrz(tile, range(8), prbs_mode, speed)

#######################################################################################################################
# bfn_config_ln_pam4
#
# Configure PAM4 on the specified tile, group, and ln
#
def bfn_config_ln_pam4(tile, g, ln, prbs_mode='QPRBS13'):
  set_tile(tile)
  set_grp(g)

  gEncodingMode[g][ln] = ['pam4', 53.125]

  bfn_init_lane_for_fw(mode='pam4', input_mode='ac', lane=ln, group = g, TX_pat=3, RX_pat=3)
  fw_config_cmd(config_cmd=0x80D0 + ln, config_detail=0x0000)

  if prbs_mode=='functional':
      #bfn_prbs_mode_select(mode='pam4', group=g, lane=ln, tx_prbs_mode='functional', rx_prbs_mode='functional', tx_pat=None, rx_pat=None)
      bfn_prbs_mode_select(mode='pam4', group=g, lane=ln, tx_prbs_mode='functional', rx_prbs_mode='prbs', tx_pat=None, rx_pat=None)
  else:
      bfn_prbs_mode_select(mode='pam4', group=g, lane=ln, tx_prbs_mode='prbs', rx_prbs_mode='prbs', tx_pat=prbs_mode, rx_pat=prbs_mode)

#######################################################################################################################
# bfn_config_lanes_pam4
#
# Configure PAM4 on all lanes of the tile and group
#
def bfn_config_lanes_pam4(tile, g, ln_list, prbs_mode='QPRBS13'):
  for ln in ln_list:
    bfn_config_ln_pam4(tile, g, ln, prbs_mode)

#######################################################################################################################
# bfn_config_grps_pam4
#
# Configure PAM4 on all lanes of the tile and group list
#
def bfn_config_grps_pam4(tile, grp_list, prbs_mode='QPRBS13'):
  for g in grp_list:
    bfn_config_lanes_pam4(tile, g, range(8), prbs_mode)

#######################################################################################################################
# bfn_config_all_lanes_pam4
#
# Configure PAM4 on all groups/lanes of the tile list
#
def bfn_config_all_lanes_pam4(tile_list, prbs_mode='QPRBS13'):
  for tile in tile_list:
    bfn_config_grps_pam4(tile, range(8), prbs_mode)


FW_CMD = 0x40C1
FW_CMD_DETAIL = 0x40C2
CMD_DEBUG_INFO = 0xB
CMD_DESTROY = 0x9
CMD_CONFIG = 0x8
MODE_NRZ = 0xC
MODE_PAM4 = 0xD

DEBUG_INFO_MODE_TOP = 0
DEBUG_INFO_MODE_NRZ = 1
DEBUG_INFO_MODE_PAM4 = 2
DEBUG_INFO_MODE_TOP_INFO =7 

fw_cmd_dbg = 0

def bfn_fw_cmd(chip, cmd, mode, ln, detail):
    if fw_cmd_dbg: print("FW_CMD: cmd=" + str(hex(cmd)) + " : mode=" + str(hex(mode)) + " : ln=" + str(hex(ln)) + " : detail: " + str(hex(detail)))
    reg_wr(FW_CMD_DETAIL, detail)
    in_progress = (cmd << 12) | (mode << 4) | ln
    reg_wr(FW_CMD, in_progress)
    sts = reg_rd(FW_CMD)
    wait = 0
    while sts == in_progress:
        sts = reg_rd(FW_CMD)
        wait += 1
        if (wait > 100):
            break;
    detail = reg_rd(FW_CMD_DETAIL)
    if (((sts >> 8) & 0xF) == 3):
        result = "Failed"
    elif (((sts >> 8) & 0xF) == cmd):
        result = "Ok"
    else:
        result = "--"
    if fw_cmd_dbg: print("FW_CMD: sts" + str(hex(sts)) + " : detail: " + str(hex(detail)) + " : " + result + " : wait=" + str(wait))
    return sts, detail

def dbg_info_get(chip, ln):
    # see what debug info is
    #print("Get top level debug info")
    rc, detail = bfn_fw_cmd(chip, CMD_DEBUG_INFO, DEBUG_INFO_MODE_TOP, 0, 0x1)
    print("Opt mode   : " + str(detail))
    rc, detail = bfn_fw_cmd(chip, CMD_DEBUG_INFO, DEBUG_INFO_MODE_TOP, 0, 0x4)
    print("Speed      : " + str(detail))
    rc, detail = bfn_fw_cmd(chip, CMD_DEBUG_INFO, DEBUG_INFO_MODE_TOP, 0, 0x5)
    print("Vcocap Rx  : " + str(detail))
    rc, detail = bfn_fw_cmd(chip, CMD_DEBUG_INFO, DEBUG_INFO_MODE_TOP, 0, 0x6)
    print("Vcocap Tx  : " + str(detail))
    rc, detail = bfn_fw_cmd(chip, CMD_DEBUG_INFO, DEBUG_INFO_MODE_TOP_INFO, 0, 0x0)
    print("Link lost  : " + str(detail))
    rc, detail = bfn_fw_cmd(chip, CMD_DEBUG_INFO, DEBUG_INFO_MODE_TOP_INFO, 0, 0x1)
    print("Readapt cnt: " + str(detail))
    rc, detail = bfn_fw_cmd(chip, CMD_DEBUG_INFO, DEBUG_INFO_MODE_NRZ, 0, 0x0)
    print("NRZ debug info: " + str(hex(detail)))
    print("")

def reset_tx_eq(chip, ln):
    for r in range(0xa5,0xae):
      if r == 0xab:
        reg_wr(r,0x1e00) # fixed POST1?
      elif r == 0xa9:
        reg_wr(r,0x1100) # fixed MAIN
      else:
        reg_wr(r,0)

def sig_info(chip, ln):
    org_103 = reg_rd(0x103 | (ln<<11))
    thresh = org_103 & 0x7FF
    org_104 = reg_rd(0x104 | (ln<<11))
    cycles = org_104 >> 12
    xover  = org_104 & 0xFFF
    sd,rdy = chip.NRZ25[ln].ready_nrz()
    print("<sig info>-----------------------------------------")
    print("THR=" + str(hex(thresh)) + " CYCLES=" + str(hex(cycles)) + " XOVR=" + str(hex(xover)))
    print("Signal (" + str(sd) + ") : PHY ready (" + str(rdy) + ")")
    rc, detail = bfn_fw_cmd(chip, CMD_DEBUG_INFO, DEBUG_INFO_MODE_TOP_INFO, 0, 0x0)
    print("Link lost  : " + str(detail))
    rc, detail = bfn_fw_cmd(chip, CMD_DEBUG_INFO, DEBUG_INFO_MODE_TOP_INFO, 0, 0x1)
    print("Readapt cnt: " + str(detail))

def tx_eq_info(chip, ln):
    pre2 = reg_rd(0xA5 )
    pre1 = reg_rd(0xA7 )
    main = reg_rd(0xA9 )
    post1 = reg_rd(0xAB)
    post2 = reg_rd(0xAD)
    pre2_3x = reg_rd(0xA6 )
    pre1_3x = reg_rd(0xA8 )
    main_3x = reg_rd(0xAA )
    post1_3x = reg_rd(0xAC)
    post2_3x = reg_rd(0xAE)    
    scale = reg_rd(0xAF)
    if (scale & 0xF800) != 0xF800:
      scale |= 0xF800
      reg_wr(0xAF, scale)
      
    print("<tx-eq info>----------------------------------------------")
    print(": PRE2=    "),
    print("%04x : "%pre2),
    print(": PRE1=    "),
    print("%04x : "%pre1),
    print(": MAIN=    "),
    print("%04x : "%main),
    print(": POST1=   "),
    print("%04x : "%post1),
    print(": POST2=   "),
    print("%04x : "%post2)
    print(": PRE2_3X= "),
    print("%04x : "%pre2_3x),
    print(": PRE1_3X= "),
    print("%04x : "%pre1_3x),
    print(": MAIN_3X= "),
    print("%04x : "%main_3x),
    print(": POST1_3X="),
    print("%04x : "%post1_3x),
    print(": POST2_3X="),
    print("%04x : "%post2_3x)
    print(": AUTO_SEL_SCALE= %04x : "%scale)


def sig_level_get(chip, grp, ln):
    #reg_wr(0x103 | (ln<<16), 0x333)
    #time.sleep(0.9)
    org_103 = reg_rd(0x103 | (ln<<11))
    if (org_103 >> 11) != 0x1b:
      reg_wr(0x103, (0x1b << 11) | 0x333)
      new_103 = reg_rd(0x103)
      if (new_103 != 0xdb33):
         print("Warning: Cant reset SD THR or rsvd bits: " + str(hex(new_103)))

    #thresh = org_103 & 0x7FF
    #org_104 = reg_rd(0x104 | (ln<<16))
    #cycles = org_104 >> 12
    #xover  = org_104 & 0xFFF
    #print("Original: THR=" + str(hex(thresh)) + " CYCLES=" + str(hex(cycles)) + " XOVR=" + str(hex(xover)))
    #org_a9 = reg_rd(0xA9 | (ln<<16))
    #main = org_a9 >> 8
    #main_comp = org_a9 & 0xFF
    #print("Original: MAIN=" + str(main) + " : MAIN_COMP=" + str(main_comp))
    #tx_eq_info(chip, ln)

    sd,rdy = chip.NRZ25[ln].ready_nrz()
    if ((sd == 0) or (rdy == 0)):
      sig_info(chip, ln)
      return

    for thresh in range(1,0x7ff):
      new_103 = (org_103 & ~0x7FF) | thresh
      reg_wr(0x103 | (ln<<11), new_103)

      #sig_info(chip, ln)
      #dbg_info_get(chip, ln)
      #tx_eq_info(chip, ln)

      sd,rdy = chip.NRZ25[ln].ready_nrz()
      if ((sd == 0) or (rdy == 0)):
        sig_info(chip, ln)
        dbg_info_get(chip, ln)
        tx_eq_info(chip, ln)
        lev_x10 = thresh*4
        print("grp=" + str(grp) + " : ln=" + str(ln) + " SIG-LEV = " + str(lev_x10/10) + "mV")
        reg_wr(0x103 | (ln<<11), 0x333)
        return
    # restore
    lev_x10 = thresh*4
    print("grp=" + str(grp) + " : ln=" + str(ln) + " SIG-LEV > " + str(lev_x10/10) + "mV")
    reg_wr(0x103 | (ln<<11), 0x333)

def dump_regs(chip,ln):
  for r in range(0,0x220):
    v = reg_rd(r)
  
    if (r % 16) == 0:
      print("")
      print("%04x : "%r),
    print("%04x "%v),
  print("")

def dump_regs_in_setup_format(chip,ln):
  for r in range(0,0x200):
    ln_r = r | (ln << 11)
    v = reg_rd(ln_r)
    print("%04x "%ln_r),
    print("%04x"%v)
  print("")


def adapt_done(chip,ln):
  v = reg_rd(0x40C7)
  if ((v >> ln) & 1):
    return 1
  return 0

def tx_drvr_set(tile, g, ln, en):
  if en == True:
    st = 1
  else:
    st = 0
  set_tile(tile)
  set_grp(g)
  data = reg_rd(0xFF | (ln<<11))
  data = data & 0xF7FF
  data = data | (st << 11)
  reg_wr((0xFF | (ln<<11)), data)

def banner(hdr):
  print("#############################################")
  print(hdr)
  print("#############################################")

def wait_sig_detect_nrz(chip,g):
    chip.setPhyAddr(g)
    for ln in range(0,8):
      sd,rdy = chip.NRZ25[ln].ready_nrz()
      if (sd == 0) or (rdy == 0):
        return False
    return True

def wait_all_sig_detect_nrz(chip):
    for g in range(0,8):
      all_good = wait_sig_detect_nrz(chip,g)
      if not all_good:
          return False
    return True

def dump_sig_detect_nrz(chip,g):
    chip.setPhyAddr(g)
    print("grp" + str(g) + " |"),
    for ln in range(0,8):
      sd,rdy = chip.NRZ25[ln].ready_nrz()
      if (sd == 0) and (rdy == 0):
        char = "-"
      if (sd == 1) and (rdy == 0):
        char = "."
      if (sd == 0) and (rdy == 1):
        char = "?"
      if (sd == 1) and (rdy == 1):
        char = "*"
      print(char + "|"),

def dump_all_sig_detect_nrz(chip):
    print("----------------------------------")
    for g in range(0,8):
      dump_sig_detect_nrz(chip,g)
      print("")
    print("----------------------------------")

def wait_adapt_done_nrz(chip,g):
    chip.setPhyAddr(g)
    for ln in range(0,8):
      ad = adapt_done(chip,ln)
      if (ad == 0):
        #print("Grp" + str(g) + " ln" + str(ln) + "..")
        return False
    return True

def wait_all_adapt_done_nrz(chip):
    for g in range(0,8):
      for retry in range(0,2):
        done = wait_adapt_done_nrz(chip,g)
        if done:
          break
        else:
          time.sleep(1.0)

#######################################################################################################################
# bfn_save_setup_on_lanes
#
# 
#
def bfn_save_setup_on_lanes(filename, tile, g, ln_list):
  set_tile(tile)
  set_grp(g)
  if len(ln_list) == 1:
    lanes_str = '_Device%d_Lane%d_%s_' % (0, ln_list[0], gEncodingMode[g][ln_list[0]][0])
  else:
    lanes_str = '_Device%d_Lanes%d-%d_%s_' % (0, ln_list[0], ln_list[-1], gEncodingMode[g][ln_list[0]][0])

  timestr = time.strftime("%Y%m%d_%H%M%S")
  filename = "save_setup" + "_tile" + str(tile) + "_grp" + str(g) + lanes_str + timestr + '.txt'
  save_setup(filename, g, ln_list)

#######################################################################################################################
# bfn_save_setup_on_groups
#
# 
#
def bfn_save_setup_on_groups(filename, tile, grp_list, ln_list):
  for g in grp_list:
    bfn_save_setup_on_lanes(filename, tile, g, ln_list) 

#######################################################################################################################
# bfn_save_setup
#
# 
#
def bfn_save_setup(filename, tile_list, grp_list, ln_list):
  for tile in tile_list:
    set_tile(tile)
    bfn_save_setup_on_groups(filename, tile, grp_list, ln_list)

#######################################################################################################################
# bfn_dump_plls_pam4
#
# 
#
def bfn_dump_plls_pam4(tile_list, grp_list, ln_list):
  for tile in tile_list:
    for g in grp_list:
      set_grp(g)
      for ln in ln_list:
        ln_mode = bfn_get_lane_mode(tile, g, ln)
        if ln_mode == 'pam4':
          plls = chip.PAM50[0].get_lane_pll()
        else:
          plls = chip.NRZ25[0].get_lane_pll()
        print plls

#######################################################################################################################
# bfn_invert_tx_polarity_on_lanes
#
# 
#
def bfn_invert_tx_polarity_on_lanes(tile, g, ln_list):
  set_tile(tile)
  set_grp(g)
  for ln in ln_list:
    ln_mode = bfn_get_lane_mode(tile, g, ln)
    if ln_mode == 'pam4':
      tx_pn = chip.PAM50[ln].tx_pol_pam4()
      chip.PAM50[ln].tx_pol_pam4(val=1-tx_pn)
    else:
      tx_pn = chip.NRZ25[ln].tx_pol_nrz()
      chip.NRZ25[ln].tx_pol_nrz(val=1-tx_pn)

#######################################################################################################################
# bfn_invert_tx_polarity_on_groups
#
# 
#
def bfn_invert_tx_polarity_on_groups(tile, grp_list, ln_list):
  for g in grp_list:
    bfn_invert_tx_polarity_on_lanes(tile, g, ln_list)

#######################################################################################################################
# bfn_invert_tx_polarity
#
# 
#
def bfn_invert_tx_polarity(tile_list, grp_list, ln_list):
  for tile in tile_list:
    bfn_invert_tx_polarity_on_groups(tile, grp_list, ln_list)

#######################################################################################################################
# bfn_invert_rx_polarity_on_lanes
#
# 
#
def bfn_invert_rx_polarity_on_lanes(tile, g, ln_list):
  set_tile(tile)
  set_grp(g)
  for ln in ln_list:
    ln_mode = bfn_get_lane_mode(tile, g, ln)
    if ln_mode == 'pam4':
      rx_pn = chip.PAM50[ln].rx_pol_pam4()
      chip.PAM50[ln].rx_pol_pam4(val=1-rx_pn)
    else:
      rx_pn = chip.NRZ25[ln].rx_pol_nrz()
      chip.NRZ25[ln].rx_pol_nrz(val=1-rx_pn)

#######################################################################################################################
# bfn_invert_rx_polarity_on_groups
#
# 
#
def bfn_invert_rx_polarity_on_groups(tile, grp_list, ln_list):
  for g in grp_list:
    bfn_invert_rx_polarity_on_lanes(tile, g, ln_list)

#######################################################################################################################
# bfn_invert_rx_polarity
#
# 
#
def bfn_invert_rx_polarity(tile_list, grp_list, ln_list):
  for tile in tile_list:
    bfn_invert_rx_polarity_on_groups(tile, grp_list, ln_list)

#######################################################################################################################
# bfn_invert_tx_msblsb_swap_on_lanes_pam4
#
# 
#
def bfn_invert_tx_msblsb_swap_on_lanes_pam4(tile, g, ln_list):
  set_tile(tile)
  set_grp(g)
  for ln in ln_list:
    tx_swp, rx_swp = chip.PAM50[ln].msblsb(print_en=0)
    chip.PAM50[ln].msblsb(1-tx_swp, rx_swp)

#######################################################################################################################
# bfn_invert_tx_msblsb_swap_on_groups_pam4
#
# 
#
def bfn_invert_tx_msblsb_swap_on_groups_pam4(tile, grp_list, ln_list):
  for g in grp_list:
    bfn_invert_tx_msblsb_swap_on_lanes_pam4(tile, g, ln_list)

#######################################################################################################################
# bfn_invert_tx_msblsb_swap_pam4
#
# 
#
def bfn_invert_tx_msblsb_swap_pam4(tile_list, grp_list, ln_list):
  for tile in tile_list:
    bfn_invert_tx_msblsb_swap_on_groups_pam4(tile, grp_list, ln_list)

#######################################################################################################################
# bfn_invert_rx_msblsb_swap_on_lanes_pam4
#
# 
#
def bfn_invert_rx_msblsb_swap_on_lanes_pam4(tile, g, ln_list):
  set_tile(tile)
  set_grp(g)
  for ln in ln_list:
    tx_swp, rx_swp = chip.PAM50[ln].msblsb(print_en=0)
    chip.PAM50[ln].msblsb(tx_swp, 1-rx_swp)

#######################################################################################################################
# bfn_invert_rx_msblsb_swap_on_groups_pam4
#
# 
#
def bfn_invert_rx_msblsb_swap_on_groups_pam4(tile, grp_list, ln_list):
  for g in grp_list:
    bfn_invert_rx_msblsb_swap_on_lanes_pam4(tile, g, ln_list)

#######################################################################################################################
# bfn_invert_rx_msblsb_swap_pam4
#
# 
#
def bfn_invert_rx_msblsb_swap_pam4(tile_list, grp_list, ln_list):
  for tile in tile_list:
    bfn_invert_rx_msblsb_swap_on_groups_pam4(tile, grp_list, ln_list)

#######################################################################################################################
# bfn_invert_rx_greycode_on_lanes_pam4
#
# 
#
def bfn_invert_rx_greycode_on_lanes_pam4(tile, g, ln_list):
  set_tile(tile)
  set_grp(g)
  for ln in ln_list:
    tx_, rx_ = chip.PAM50[ln].gc(print_en=0)
    chip.PAM50[ln].gc(tx_, 1-rx_)
    print("Tile " + str(tile) + " : grp" + str(g) + " : ln" + str(ln) + "gc(tx=" + str(tx_) + ", rx=" + str(1-rx_) + ")")

#######################################################################################################################
# bfn_invert_rx_greycode_on_groups_pam4
#
# 
#
def bfn_invert_rx_greycode_on_groups_pam4(tile, grp_list, ln_list):
  for g in grp_list:
    bfn_invert_rx_greycode_on_lanes_pam4(tile, g, ln_list)

#######################################################################################################################
# bfn_invert_rx_greycode_pam4
#
# 
#
def bfn_invert_rx_greycode_pam4(tile_list, grp_list, ln_list):
  for tile in tile_list:
    bfn_invert_rx_greycode_on_groups_pam4(tile, grp_list, ln_list)

#######################################################################################################################
# bfn_invert_tx_greycode_on_lanes_pam4
#
# 
#
def bfn_invert_tx_greycode_on_lanes_pam4(tile, g, ln_list):
  set_tile(tile)
  set_grp(g)
  for ln in ln_list:
    tx_, rx_ = chip.PAM50[ln].gc(print_en=0)
    chip.PAM50[ln].gc(1-tx_, rx_)
    print("Tile " + str(tile) + " : grp" + str(g) + " : ln" + str(ln) + "gc(tx=" + str(1-tx_) + ", rx=" + str(rx_) + ")")

#######################################################################################################################
# bfn_invert_tx_greycode_on_groups_pam4
#
# 
#
def bfn_invert_tx_greycode_on_groups_pam4(tile, grp_list, ln_list):
  for g in grp_list:
    bfn_invert_tx_greycode_on_lanes_pam4(tile, g, ln_list)

#######################################################################################################################
# bfn_invert_tx_greycode_pam4
#
# 
#
def bfn_invert_tx_greycode_pam4(tile_list, grp_list, ln_list):
  for tile in tile_list:
    bfn_invert_tx_greycode_on_groups_pam4(tile, grp_list, ln_list)

#######################################################################################################################
# bfn_invert_rx_precode_on_lanes_pam4
#
# 
#
def bfn_invert_rx_precode_on_lanes_pam4(tile, g, ln_list):
  set_tile(tile)
  set_grp(g)
  for ln in ln_list:
    tx_, rx_ = chip.PAM50[ln].pc(print_en=0)
    chip.PAM50[ln].pc(tx_, 1-rx_)
    print("Tile " + str(tile) + " : grp" + str(g) + " : ln" + str(ln) + "pc(tx=" + str(tx_) + ", rx=" + str(1-rx_) + ")")

#######################################################################################################################
# bfn_invert_rx_precode_on_groups_pam4
#
# 
#
def bfn_invert_rx_precode_on_groups_pam4(tile, grp_list, ln_list):
  for g in grp_list:
    bfn_invert_rx_precode_on_lanes_pam4(tile, g, ln_list)

#######################################################################################################################
# bfn_invert_rx_precode_pam4
#
# 
#
def bfn_invert_rx_precode_pam4(tile_list, grp_list, ln_list):
  for tile in tile_list:
    bfn_invert_rx_precode_on_groups_pam4(tile, grp_list, ln_list)

#######################################################################################################################
# bfn_invert_tx_precode_on_lanes_pam4
#
# 
#
def bfn_invert_tx_precode_on_lanes_pam4(tile, g, ln_list):
  set_tile(tile)
  set_grp(g)
  for ln in ln_list:
    tx_, rx_ = chip.PAM50[ln].pc(print_en=0)
    chip.PAM50[ln].pc(1-tx_, rx_)
    print("Tile " + str(tile) + " : grp" + str(g) + " : ln" + str(ln) + "pc(tx=" + str(1-tx_) + ", rx=" + str(rx_) + ")")

#######################################################################################################################
# bfn_invert_tx_precode_on_groups_pam4
#
# 
#
def bfn_invert_tx_precode_on_groups_pam4(tile, grp_list, ln_list):
  for g in grp_list:
    bfn_invert_tx_precode_on_lanes_pam4(tile, g, ln_list)

#######################################################################################################################
# bfn_invert_tx_precode_pam4
#
# 
#
def bfn_invert_tx_precode_pam4(tile_list, grp_list, ln_list):
  for tile in tile_list:
    bfn_invert_tx_precode_on_groups_pam4(tile, grp_list, ln_list)

#######################################################################################################################
# bfn_lane_reset
#
# Apply a lane reset
#
def bfn_lane_reset(tile, g, ln):
  set_tile(tile)
  set_grp(g)
  ln_mode = bfn_get_lane_mode(tile, g, ln)
  if ln_mode == 'pam4':
    chip.PAM50[ln].lane_reset_fast_pam4()
  else:
    chip.NRZ25[ln].lane_reset()

#######################################################################################################################
# bfn_lane_reset_lanes
#
# Apply a lane reset
#
def bfn_lane_reset_lanes(tile, g, ln_list):
  for ln in ln_list:
    bfn_lane_reset(tile, g, ln)

#######################################################################################################################
# bfn_lane_reset_groups
#
# Apply a lane reset
#
def bfn_lane_reset_groups(tile, grp_list, ln_list):
  for g in grp_list:
    bfn_lane_reset_lanes(tile, g, ln_list)

#######################################################################################################################
# bfn_lane_reset
#
# Apply a lane reset
#
def bfn_lane_reset_all(tile_list, grp_list, ln_list):
  for tile in tile_list:
    bfn_lane_reset_groups(tile, grp_list, ln_list)

#######################################################################################################################
# bfn_rx_monitor_lanes
#
# 
#
def bfn_rx_monitor_lanes(tile, g, ln_list, dwell=0.1):
  set_tile(tile)
  set_grp(g)

  for ln in ln_list:
    rx_monitor(lane=ln, rst=1, lc=0, ph=0, group=g, print_en=0, returnon=0, capture_en=0)
    time.sleep(dwell)
    rx_monitor(lane=ln, rst=0, lc=0, ph=0, group=g, print_en=0, returnon=0, capture_en=1)
  print("Tile " + str(tile) + " Grp" + str(g) + ":")
  rx_monitor(lane=ln_list, rst=0, lc=0, ph=0, group=g, print_en=1, returnon=0, capture_en=0)
    
#######################################################################################################################
# bfn_rx_monitor_groups
#
# 
#
def bfn_rx_monitor_groups(tile, grp_list, ln_list):
  for g in grp_list:
    bfn_rx_monitor_lanes(tile, g, ln_list)

#######################################################################################################################
# bfn_rx_monitor
#
# 
#
def bfn_rx_monitor(tile_list, grp_list, ln_list):
  for tile in tile_list:
    bfn_rx_monitor_groups(tile, grp_list, ln_list)

#######################################################################################################################
# bfn_fw_sd_params_lanes
#
# 
#
def bfn_fw_sd_params_lanes(tile, g, ln_list):
  set_tile(tile)
  set_grp(g)
  print("tile " + str(tile) + "grp" + str(g) + " :")
  fw_serdes_params(g, ln_list, True)
  print("")

#######################################################################################################################
# bfn_fw_sd_params_groups
#
# 
#
def bfn_fw_sd_params_groups(tile, grp_list, ln_list):
  for g in grp_list:
    bfn_fw_sd_params_lanes(tile, g, ln_list)

#######################################################################################################################
# bfn_fw_sd_params
#
# 
#
def bfn_fw_sd_params(tile_list, grp_list, ln_list):
  for tile in tile_list:
    bfn_fw_sd_params_groups(tile, grp_list, ln_list)
  if 0 in tile_list:
    if not 8 in grp_list:
      bfn_fw_sd_params_groups(0, [8], range(4))
       
#######################################################################################################################
# wait_sig_detect
#
# 
#
def wait_sig_detect(chip,g):
    set_grp(g)
    for ln in range(0,8):
      ln_mode = bfn_get_lane_mode(tile, g, ln)
      if ln_mode == 'pam4':
        sd,rdy = chip.PAM50[ln].ready_pam4()
      else:
        sd,rdy = chip.NRZ25[ln].ready_nrz()
      if (sd == 0) or (rdy == 0):
        return False
    return True

#######################################################################################################################
# bfn_wait_lane_sig_detect
#
# 
#
def bfn_wait_lane_sig_detect(tile, g, ln):
  set_tile(tile)
  set_grp(g)
  ln_mode = bfn_get_lane_mode(tile, g, ln)
  if ln_mode == 'pam4':
    sd,rdy = chip.PAM50[ln].ready_pam4()
  else:
    sd,rdy = chip.NRZ25[ln].ready_nrz()
  return sd, rdy

#######################################################################################################################
# bfn_wait_all_lanes_sig_detect
#
# 
#
def bfn_wait_all_lanes_sig_detect(tile, g, ln_list):
  for ln in ln_list:
    sd, rdy = bfn_wait_lane_sig_detect(tile, g, ln)
    if (sd == 0) or (rdy == 0):
      return sd, rdy, tile, g, ln
  return 1, 1, tile, g, ln

#######################################################################################################################
# wait_all_groups_sig_detect
#
# 
#
def wait_all_groups_sig_detect(tile, grp_list, ln_list):
  for g in grp_list:
    sd, rdy, wait_tile, wait_grp, wait_ln = bfn_wait_all_lanes_sig_detect(tile, g, ln_list)
  return sd, rdy, wait_tile, wait_grp, wait_ln

#######################################################################################################################
# bfn_wait_all_sig_detect
#
# 
#
def bfn_wait_all_sig_detect(tile_list, grp_list, ln_list, tries=10, dwell=0.5, print_en=1):
  for attempt in range(tries):
    for tile in tile_list:
      sd, rdy, wait_tile, wait_grp, wait_ln = wait_all_groups_sig_detect(tile, grp_list, ln_list)
      if (not sd) or (not rdy):
        if print_en:
          print("Tile " + str(wait_tile) + " grp" + str(wait_grp) + " ln" + str(wait_ln) + " sd=" + str(sd) + " : rdy=" + str(rdy))
        attempt += 1
        break
    return True
  return False

#######################################################################################################################
# dump_lanes_prbs_errors
#
# 
#
def dump_lanes_prbs_errors(tile, g, dwell_time):
  set_tile(tile)
  set_grp(g)
  errs = []
  for ln in range(8):
    ln_mode = bfn_get_lane_mode(tile, g, ln)

    if dwell_time != 0.0:
      if ln_mode == 'pam4':
        chip.PAM50[ln].prbs_rst_pam4()
      else:
        chip.NRZ25[ln].prbs_rst_nrz()
      time.sleep(dwell_time)

    # read error counts
    if ln_mode == 'pam4':
      error_cnt = chip.PAM50[ln].get_err_pam4()
    else:
      error_cnt = chip.NRZ25[ln].get_err_nrz()
    errs.append(error_cnt)
  for ln in range(8):
    print("%12d"%errs[ln] + "|"),

#######################################################################################################################
# dump_groups_prbs_errors
#
# 
#
def dump_groups_prbs_errors(tile, grp_list, dwell_time):
    print("Tile " + str(tile) + " : dwell " + str(dwell_time) + "s")
    print("-----+--------------+-------------+-------------+-------------+-------------+-------------+-------------+-------------+")
    print("lane |       0      |      1      |      2      |      3      |      4      |      5      |      6      |       7     |")
    print("-----+--------------+-------------+-------------+-------------+-------------+-------------+-------------+-------------+")
    for g in grp_list:
      print("grp" + str(g) + " : "),
      dump_lanes_prbs_errors(tile, g, dwell_time)
      print("")
    print("-----+--------------+-------------+-------------+-------------+-------------+-------------+-------------+-------------+")

#######################################################################################################################
# dump_all_prbs_errors
#
# 
#
def dump_all_prbs_errors(tile_list, grp_list, dwell_time = cur_dwell_time):
  for tile in tile_list:
    dump_groups_prbs_errors(tile, grp_list, dwell_time)

def bfn_ln_inject_errors(tile, g, ln):
  set_tile(tile)
  set_grp(g)
  ln_mode = bfn_get_lane_mode(tile, g, ln)
  if ln_mode == 'pam4':
    for e in range(ln+1):
      chip.PAM50[ln].err_inject()
    print("tile " + str(tile) + " grp" + str(g) + " ln" + str(ln) + " PAM4: inject errors")
  elif ln_mode == 'nrz':
    for e in range(ln+1):
      chip.NRZ25[ln].err_inject_nrz()
    print("tile " + str(tile) + " grp" + str(g) + " ln" + str(ln) + " NRZ: inject errors")

def bfn_lanes_inject_errors(tile, g, ln_list):
  for ln in ln_list:
    bfn_ln_inject_errors(tile, g, ln)

def bfn_grps_inject_errors(tile, grp_list, ln_list):
  for g in grp_list:
    bfn_lanes_inject_errors(tile, g, ln_list)

def bfn_all_inject_errors(tile_list, grp_list, ln_list):
  for tile in tile_list:
    bfn_grps_inject_errors(tile, grp_list, ln_list)

#######################################################################################################################
# bfn_invert_pol
#
# 
#
def bfn_invert_pol():
  t_or_r = raw_input("Enter t=Tx or r=Rx : ")
  tile   = raw_input("Enter Tile  number : ")
  g      = raw_input("Enter group number : ")
  ln     = raw_input("Enter lane  number : ")
  if t_or_r == 't':
    bfn_invert_tx_polarity_on_lanes(int(tile), int(g), [int(ln)])
  elif t_or_r == 'r':
    bfn_invert_rx_polarity_on_lanes(int(tile), int(g), [int(ln)])
  else:
    print("invalid arg")

#######################################################################################################################
# bfn_inject_errors
#
# 
#
def bfn_inject_errors():
  tile   = raw_input("Enter Tile  number : ")
  g      = raw_input("Enter group number : ")
  ln     = raw_input("Enter lane  number : ")
  bfn_ln_inject_errors(int(tile), int(g), int(ln))

#######################################################################################################################
# bfn_access_reg
#
# 
#
def bfn_access_reg():
  r_or_w = raw_input("Enter 'r'=read, 'w'=write           : ")
  tile   = raw_input("Enter Tile  number                  : ")
  g      = raw_input("Enter Group number                  : ")
  reg    = raw_input("Enter Register address (incl lane #): ")
  set_tile(int(tile))
  set_grp(int(g))
  reg = int(reg,base=16)
  if r_or_w == 'r':
    data = reg_rd(reg)
    print("Rd: [" + hex(reg) + "] = " + hex(data))
  elif r_or_w == 'w':
    data = raw_input("Enter data : ")
    reg_wr(reg, int(data, base=16))
    print("Wr: [" + hex(reg) + "] = " + hex(data))
  else:
    print("Follow instructions dummy")

#######################################################################################################################
# bfn_command_menu_help
#
# 
#
def bfn_command_menu_help():
    print("S: save setup")
    print("s: Dump Signal detect and PHY ready")
    print("L: Dump PLLs")
    print("l: Lane Reset")
    print("m: Rx Monitor")
    print("e: Inject Errors on all lanes")
    print("E: Inject Errors on a single lane")
    print("d: Toggle Dwell Time")
    print("f: FW Serdes Params")
    print("a: Rx EQ Adaptation Done")
    print("t: Invert Tx Polarity")
    print("r: Invert Rx Polarity")
    print("W: Invert Tx MSB/LSB swap")
    print("w: Invert Rx MSB/LSB swap")
    print("G: Invert Tx Greycode En")
    print("g: Invert Rx Greycode En")
    print("P: Invert Tx Precode En")
    print("p: Invert Rx Precode En")
    print("i: Invert Polarity (Tx or Rx) on a single lane")
    print("A: Access a register")

    print("q: Quit")

#######################################################################################################################
# bfn_command_menu
#
# 
#
def bfn_command_menu(inp):
    global cur_dwell_time

    if inp == 'S':
      bfn_save_setup(filename=None, tile_list=test_tiles, grp_list=test_grps, ln_list=test_lns)
    elif inp == 'L':
      bfn_dump_plls_pam4(tile_list=test_tiles, grp_list=test_grps, ln_list=test_lns)
    elif inp == 's':
      print("Signal detect and PHY ready")
      dump_all_sig_detect(test_tiles, test_grps, test_lns)
    elif inp == 'a':
      print("Adaptation done")
      dump_all_adapt_done(tile_list=test_tiles, grp_list=test_grps, ln_list=test_lns)
    elif inp == 't':
      print("invert_tx_polarity_on_group")
      bfn_invert_tx_polarity(test_tiles, test_grps, test_lns)
    elif inp == 'r':
      print("invert_rx_polarity_on_group")
      bfn_invert_rx_polarity(test_tiles, test_grps, test_lns)
    elif inp == 'W':
      print("invert_tx_msblsb_swap_on_group_pam4")
      bfn_invert_tx_msblsb_swap_pam4(test_tiles, test_grps, test_lns)
    elif inp == 'w':
      print("invert_rx_msblsb_swap_on_group_pam4")
      bfn_invert_rx_msblsb_swap_pam4(test_tiles, test_grps, test_lns)
    elif inp == 'G':
      print("invert_tx_greycode_on_group_pam4")
      bfn_invert_tx_greycode_pam4(test_tiles, test_grps, test_lns)
    elif inp == 'g':
      print("invert_rx_greycode_on_group_pam4")
      bfn_invert_rx_greycode_pam4(test_tiles, test_grps, test_lns)
    elif inp == 'P':
      print("invert_tx_precode_on_group_pam4")
      bfn_invert_tx_precode_pam4(test_tiles, test_grps, test_lns)
    elif inp == 'p':
      print("invert_rx_precode_on_group_pam4")
      bfn_invert_rx_precode_pam4(test_tiles, test_grps, test_lns)
    elif inp == 'l':
      print("Lane reset all")
      bfn_lane_reset_all(test_tiles, test_grps, test_lns)
    elif inp == 'm':
      print("Rx Eye Monitor")
      bfn_rx_monitor(test_tiles, test_grps, test_lns)
    elif inp == 'f':
      print("fw_serdes_params")
      bfn_fw_sd_params(test_tiles, test_grps, test_lns)
    elif inp == 'e':
      print("inject errors")
      bfn_all_inject_errors(test_tiles, test_grps, test_lns)
    elif inp == 'E':
      print("inject errors on given lane")
      bfn_inject_errors()
    elif inp == 'd':
      if cur_dwell_time == 0.0:
        cur_dwell_time = DEFAULT_PRBS_CNTR_DWELL
        print("Resume PRBS count reset")
      else:
        cur_dwell_time = 0.0
        print("No PRBS count reset")
    elif (inp == 'i'):
      bfn_invert_pol()
    elif (inp == 'A'):
      bfn_access_reg()
    elif (inp == 'q'):
      exit()
    else:
      bfn_command_menu_help()

#######################################################################################################################
# TEST_Credo_Init
#
# 
#
def TEST_Credo_Init():
  banner("Credo Init")
  set_tile(3)
  Config_BlueJay_GROUP0_7(Group=range(8), datarate=None, input_mode='ac', TX_patt=3, RX_patt=3, broadcast_set=1)
  for g in range(8):
    for ln in range(8):
      prbs_mode_select(g, lane=ln, tx_prbs_mode='prbs', rx_prbs_mode='prbs', tx_pat=3, rx_pat=3)

  banner("Apply PN swaps")
  bfn_pn_swap_set(bfn_working_tiles)

  banner("Tx EQ")
  #bfn_tx_eq_set(bfn_working_tiles, 0xfe00, 0x1300, 0x0000)
  #bfn_tx_eq_set(bfn_working_tiles, 0xfe00, 0x1900, 0x0000)
  bfn_tx_eq_set_2(bfn_working_tiles, 0x0200, 0xf600, 0x1400, 0x0000, 0x0000)
  bfn_tx_eq_dump(bfn_working_tiles, range(8), range(8))

  """
  banner("Configure Rx-Tx (remote) Loopback")
  for tile in bfn_working_tiles:
    chip.setTileAddr(tile)
    for g in range(0,8):
      chip.setPhyAddr(g)
      for ln in range(0,8):
        rx_tx_serial_loopback(g, lane=ln, enable=1, TX_pat=None, RX_pat=None)
  """

  inp = 's'
  while inp != 'q':
    dump_all_prbs_errors(test_tiles, test_grps, cur_dwell_time)

    print("=========================================")
    inp = raw_input("Enter key to continue> ")
    bfn_command_menu(inp)


#######################################################################################################################
# TEST_all_lanes_functional
#
# 
#
def TEST_all_lanes_functional():
  #banner("Configure PAM4 Functional mode")
  #bfn_config_all_lanes_pam4(bfn_working_tiles, prbs_mode='functional')

  if default_mode == 'nrz':
    banner("Configure NRZ PRBS31 mode")
    bfn_config_all_lanes_nrz(bfn_working_tiles, prbs_mode='functional')
    #bfn_config_all_lanes_nrz(bfn_working_tiles, prbs_mode='PRBS31')
  else:
    banner("Configure PAM4 PRBS31 mode")
    bfn_config_all_lanes_pam4(bfn_working_tiles, prbs_mode='functional')
    #bfn_config_all_lanes_pam4(bfn_working_tiles, prbs_mode='PRBS31')

  for tile in bfn_working_tiles:
    set_tile(tile)
    for g in range(8):
      set_grp(g)
      rx_tx_external_normal_loopback_new(group=g, lanes=range(8), enable=1, TX_pat=0, RX_pat=3)

  banner("Apply PN swaps")
  bfn_pn_swap_set(bfn_working_tiles)

  banner("Tx EQ")
  #bfn_tx_eq_set(bfn_working_tiles, 0xfe00, 0x1300, 0x0000)
  #bfn_tx_eq_set(bfn_working_tiles, 0xfe00, 0x1900, 0x0000)
  #bfn_tx_eq_set(bfn_working_tiles, 0xfa00, 0x0f00, 0x0000)
  #bfn_tx_eq_set(bfn_working_tiles, 0xf300, 0x0f00, 0x0000)
  bfn_tx_eq_set(bfn_working_tiles, 0xf600, 0x1400, 0x0000)
  #bfn_tx_eq_set(bfn_working_tiles, 0xf600, 0x1100, 0x0000)
 
  bfn_tx_eq_dump(bfn_working_tiles, range(8), range(8))

  banner("Wait for Rx Signal-Detect and PHY-Ready")
  bfn_wait_all_sig_detect(test_tiles, test_grps, test_lns, 10, 0.5, True)

  banner("Check SIGNAL-DETECT and PHY-READY")
  dump_all_sig_detect(tile_list=test_tiles, grp_list=test_grps, ln_list=range(8))

  banner("Wait Rx EQ adaptation Complete")
  wait_all_adapt_done(test_tiles, test_grps, test_lns, tries=16, dwell=1.0, quiet=False)
  dump_all_adapt_done(tile_list=test_tiles, grp_list=test_grps, ln_list=test_lns)

  inp = 's'
  while inp != 'q':
    print_date_time()
    dump_all_prbs_errors(test_tiles, test_grps, cur_dwell_time)

    #banner("Configure Parallel side checker")
    #bfn_configure_die2die_checker(test_tiles, test_grps, phase=0, pat_gen=0, pat_chk=3)
    #time.sleep(0.1)
    #bfn_configure_die2die_checker(test_tiles, test_grps, phase=1, pat_gen=0, pat_chk=3)
    #dump_die2die_loopback_status(test_tiles, test_grps)

    print("=========================================")
    inp = raw_input("Enter key to continue> ")
    if inp == 'q':
      break
    bfn_command_menu(inp)

#######################################################################################################################
# TEST_1_grp_functional
#
# 
#
def TEST_1_grp_functional():
  # set fixed tile/grp for this test as it involves external cabling
  test_tiles = [0]
  test_grps = [6] # the grp to be put in external-normal loopback

  if default_mode == 'nrz':
    banner("Configure NRZ PRBS31 mode")
    bfn_config_all_lanes_nrz(bfn_working_tiles, prbs_mode='PRBS31')
  else:
    banner("Configure PAM4 PRBS31 mode")
    bfn_config_all_lanes_pam4(bfn_working_tiles, prbs_mode='PRBS31')
    #banner("Configure " + str(test_tiles[0]) + " " + str([7]) + " PAM4 PRBS31 mode")
    #bfn_config_grps_pam4(test_tiles[0], [7], prbs_mode='PRBS31')

  #banner("Configure " + str(test_tiles[0]) + " " + str(test_grps) + " PAM4 'external normal'  mode")
  #bfn_config_grps_pam4(test_tiles[0], test_grps, prbs_mode='functional')
  banner("Configure tile 0 grp 6 in 'external normal'  mode")
  set_tile(0)
  g = 6
  set_grp(g)
  #rx_tx_external_normal_loopback_new(group=g, lanes=range(8), enable=1, TX_pat=3, RX_pat=3)
  rx_tx_external_normal_loopback_new(group=g, lanes=range(8), enable=1, TX_pat=0, RX_pat=3)

  #bfn_rx_tx_external_dft_loopback(chip, group=g, lane=0, t=0.1, pat_gen=3, pat_chk=3, en=1)

  banner("Apply PN swaps")
  bfn_pn_swap_set(bfn_working_tiles)

  banner("Tx EQ")
  #bfn_tx_eq_set(bfn_working_tiles, 0xfe00, 0x1300, 0x0000)
  #bfn_tx_eq_set(bfn_working_tiles, 0xfe00, 0x1900, 0x0000)
  #bfn_tx_eq_set(bfn_working_tiles, 0xfa00, 0x0f00, 0x0000)
  #bfn_tx_eq_set(bfn_working_tiles, 0xf300, 0x0f00, 0x0000)
  bfn_tx_eq_set(bfn_working_tiles, 0xf600, 0x1400, 0x0000)
  #bfn_tx_eq_set(bfn_working_tiles, 0xf600, 0x1100, 0x0000)

  bfn_tx_eq_dump(bfn_working_tiles, range(8), range(8))

  # now, update test_grps for debug displays
  test_grps = [6,7]

  banner("Wait for Rx Signal-Detect and PHY-Ready")
  bfn_wait_all_sig_detect(test_tiles, test_grps, test_lns, 20, 0.5, True)

  banner("Check SIGNAL-DETECT and PHY-READY")
  dump_all_sig_detect(tile_list=test_tiles, grp_list=test_grps, ln_list=range(8))

  banner("Wait Rx EQ adaptation Complete")
  wait_all_adapt_done(test_tiles, test_grps, test_lns, tries=16, dwell=1.0, quiet=False)
  dump_all_adapt_done(tile_list=test_tiles, grp_list=test_grps, ln_list=test_lns)

  inp = 's'
  while inp != 'q':
    #banner("Configure Parallel side checker")
    #bfn_configure_die2die_checker(test_tiles, [6], phase=0, pat_gen=3, pat_chk=3)
    #bfn_rx_tx_external_dft_loopback(chip, group=6, lane=0, t=0.1, pat_gen=3, pat_chk=3, en=1)

    print_date_time()
    dump_all_prbs_errors(test_tiles, test_grps, cur_dwell_time)

    #time.sleep(0.1)
    #bfn_configure_die2die_checker(test_tiles, [6], phase=1, pat_gen=3, pat_chk=3)
    #dump_die2die_loopback_status(test_tiles, [6])


    print("=========================================")
    inp = raw_input("Enter key to continue> ")
    if inp == 'q':
      break
    bfn_command_menu(inp)

#######################################################################################################################
# compute_ber
#
# 
#
def compute_ber(ln):
    # pre-FEC BER
    curr_prbs_cnt = float(gLaneStats[ln][9])
    prbs_accum_time = gLaneStats[ln][4] - gLaneStats[ln][3]
    data_rate = 53.125
    bits_transferred = float((prbs_accum_time * data_rate * pow(10, 9)))
    if curr_prbs_cnt == 0 or bits_transferred == 0:
        ber_val = 0
    else:
        ber_val = curr_prbs_cnt / bits_transferred
    pre_fec_ber = ber_val

    # Post-FEC BER
    curr_prbs_cnt = float(gLaneStats[ln][10])
    prbs_accum_time = gLaneStats[ln][4] - gLaneStats[ln][3]
    data_rate = 53.125
    bits_transferred = float((prbs_accum_time * data_rate * pow(10, 9)))
    if curr_prbs_cnt == 0 or bits_transferred == 0:
        ber_val = 0
    else:
        ber_val = curr_prbs_cnt / bits_transferred
    post_fec_ber = ber_val
    return pre_fec_ber, post_fec_ber

def print_tx_eq_eyes(tile, grp):
  global gLaneStats
  chip.setTileAddr(tile)
  chip.setPhyAddr(grp)
  print("Tile: " + str(tile) + " : grp" + str(grp) + ":")
  for ln in range(8):
    rx_monitor(lane=[ln], rst=1, lc=0, ph=0, group=grp, print_en=0, returnon=0)
    time.sleep(0.01)
    rx_monitor(lane=[ln], rst=0, lc=0, ph=0, group=grp, print_en=0, returnon=0)
    print("ln" + str(ln) + " Eyes (mV)"),
    if (gLaneStats[ln][8] == 'RDY'):
      print ("%8.0f " % (gLaneStats[ln][5])),
    else:
      print("       - "),
    if (gEncodingMode[ln][0].upper() == 'PAM4' and gLaneStats[ln][8] == 'RDY'):
      print ("%8.0f " % (gLaneStats[ln][6])),
    else:
      print("       - "),
    if (gEncodingMode[ln][0].upper() == 'PAM4' and gLaneStats[ln][8] == 'RDY'):
      print ("%8.0f " % (gLaneStats[ln][7])),
    else:
      print("       - "),
    print("")

def dump_in_csv_fmt(tile1, grp1, tile2, grp2, pre, main, post): 
  global gLaneStats
  chip.setTileAddr(tile1)
  chip.setPhyAddr(grp1)
  for ln in range(8):
    rx_monitor(lane=[ln], rst=1, lc=0, ph=0, group=grp1, print_en=0, returnon=0)
    time.sleep(0.01)
    rx_monitor(lane=[ln], rst=0, lc=0, ph=0, group=grp1, print_en=0, returnon=0)
    print(hex(pre) + ", " + hex(main) + ", " + hex(post) + ", "),
    print(str(tile1) + ", " + str(grp1) + ", " + str(ln) + ", "),
    
    if (gLaneStats[ln][8] == 'RDY'):
      print ("%8.0f, " % (gLaneStats[ln][5])),
    else:
      print("     0.0, "),
    if (gEncodingMode[ln][0].upper() == 'PAM4' and gLaneStats[ln][8] == 'RDY'):
      print ("%8.0f, " % (gLaneStats[ln][6])),
    else:
      print("     0.0, "),
    if (gEncodingMode[ln][0].upper() == 'PAM4' and gLaneStats[ln][8] == 'RDY'):
      print ("%8.0f, " % (gLaneStats[ln][7])),
    else:
      print("     0.0, "),

    pre_fec_ber, post_fec_ber = compute_ber(ln)
    if pre_fec_ber == 0:
      print("       0, "),
    else:
      print("%8.1e, " % (pre_fec_ber)),
    if post_fec_ber == 0:
      print("       0, "),
    else:
      print("%8.1e, " % (post_fec_ber)),

    chip.setTileAddr(tile2)
    chip.setPhyAddr(grp2)
    rx_monitor(lane=[ln], rst=1, lc=0, ph=0, group=grp2, print_en=0, returnon=0)
    time.sleep(0.01)
    rx_monitor(lane=[ln], rst=0, lc=0, ph=0, group=grp2, print_en=0, returnon=0)
    print(str(tile2) + ", " + str(grp2) + ", " + str(ln) + ", "),
    if (gLaneStats[ln][8] == 'RDY'):
      print ("%8.0f, " % (gLaneStats[ln][5])),
    else:
      print("     0.0, "),
    if (gEncodingMode[ln][0].upper() == 'PAM4' and gLaneStats[ln][8] == 'RDY'):
      print ("%8.0f, " % (gLaneStats[ln][6])),
    else:
      print("     0.0, "),
    if (gEncodingMode[ln][0].upper() == 'PAM4' and gLaneStats[ln][8] == 'RDY'):
      print ("%8.0f, " % (gLaneStats[ln][7])),
    else:
      print("     0.0, "),

    pre_fec_ber, post_fec_ber = compute_ber(ln)
    if pre_fec_ber == 0:
      print("       0, "),
    else:
      print("%8.1e, " % (pre_fec_ber)),
    if post_fec_ber == 0:
      print("       0, "),
    else:
      print("%8.1e, " % (post_fec_ber)),

    print("")


#######################################################################################################################
# TEST_two_group_tx_eq
#
# 
#
def TEST_two_group_tx_eq(tile1,grp1,tile2,grp2, in_csv_fmt=False):
  banner("Configure PAM4 PRBS31 mode")
  bfn_config_all_lanes_pam4(bfn_working_tiles, prbs_mode='PRBS31')

  if in_csv_fmt:
    print("Pre, Main, Post, Tile1, Group1, Ln, Eye1, Eye2, Eye3, Pre-FEC-BER, Post-FEC-BER, Tile2, Group2, Ln, Eye1, Eye2, Eye3, Pre-FEC-BER, Post-FEC-BER")

  #post_list = [0x0000, 0x0001, 0x0002, 0x0003]
  post_list = [0x0000]
  pre_list = [0xff00, 0xfe00, 0xfd00, 0xfc00, 0xfb00, 0xfa00, 0xf900, 0xf800, 0xf700, 0xf600, 0xf500, 0xf400, 0xf300, 0xf200, 0xf100, 0xf000]
  main_list = [0x0f00, 0x1000, 0x1100, 0x1200, 0x1300, 0x1400, 0x1500, 0x1600, 0x1700, 0x1800]
  for post in post_list:
    for main in main_list:
      for pre in pre_list:
        if not in_csv_fmt:
          print("New Tx EQ: " + hex(pre) + " : " + hex(main) + " : " + hex(post))
        bfn_tx_eq_set(bfn_working_tiles, pre, main, post)
        bfn_lane_reset_all([tile1], [grp1], range(8))
        bfn_lane_reset_all([tile2], [grp2], range(8))

        chip.setTileAddr(tile1)
        chip.setPhyAddr(grp1)
        retry = 0
        all_good = False
        while (not all_good) and (retry < 4):
          all_good = wait_sig_detect_pam4(chip,grp1)
          if not all_good:
            time.sleep(0.5)
            retry += 1
        if not all_good: continue 

        chip.setTileAddr(tile2)
        chip.setPhyAddr(grp2)
        retry = 0
        all_good = False
        while (not all_good) and (retry < 4):
          all_good = wait_sig_detect_pam4(chip,grp2)
          if not all_good:
            time.sleep(0.5)
            retry += 1
        if not all_good: continue 

        if not in_csv_fmt:
          print_tx_eq_eyes(tile1, grp1)
          print_tx_eq_eyes(tile2, grp2)
        else:
          dump_in_csv_fmt(tile1, grp1, tile2, grp2, pre, main, post)
  exit()


#######################################################################################################################
# dump_margin_in_csv_fmt
#
# 
#
margins = dict()
def dump_margin_in_csv_fmt(iteration, tile, grp):
  global gLaneStats
  set_tile(tile)
  set_grp(grp)
  for ln in range(8):
    failed = False
    rx_monitor(lane=[ln], rst=1, lc=0, ph=0, group=grp, print_en=0, returnon=0)
    time.sleep(0.01)
    rx_monitor(lane=[ln], rst=0, lc=0, ph=0, group=grp, print_en=0, returnon=0)
    print(str(iteration) + ", " + str(tile) + ", " + str(grp) + ", " + str(ln) + ", "),
    
    if (gLaneStats[ln][8] == 'RDY'):
      print ("%8.0f, " % (gLaneStats[ln][5])),
    else:
      failed = True
      print("     0.0, "),
    if (gEncodingMode[grp][ln][0].upper() == 'PAM4' and gLaneStats[ln][8] == 'RDY'):
      print ("%8.0f, " % (gLaneStats[ln][6])),
    else:
      failed = True
      print("     0.0, "),
    if (gEncodingMode[grp][ln][0].upper() == 'PAM4' and gLaneStats[ln][8] == 'RDY'):
      print ("%8.0f, " % (gLaneStats[ln][7])),
    else:
      failed = True
      print("     0.0, "),
    
    pre_fec_ber, post_fec_ber = compute_ber(ln)
    if pre_fec_ber == 0:
      print("       0, "),
    else:
      print("%8.1e, " % (pre_fec_ber)),
    if post_fec_ber == 0:
      print("       0, "),
    else:
      print("%8.1e, " % (post_fec_ber)),

    print("")

    key = (tile, grp, ln)
    try:
      value = margins[key]
    except:
      value = { 'min_post_fec_ber':post_fec_ber, 'max_post_fec_ber':post_fec_ber, 'sum_post_fec_ber':post_fec_ber, 'min_pre_fec_ber':pre_fec_ber, 'max_pre_fec_ber':pre_fec_ber, 'sum_pre_fec_ber':pre_fec_ber, 'num_passes':1, 'pre_fec_ber_list':[pre_fec_ber], 'post_fec_ber_list':[post_fec_ber], 'failed':failed }
    else:
      min_post_fec_ber = value['min_post_fec_ber']
      if post_fec_ber < min_post_fec_ber:
        value['min_post_fec_ber'] = post_fec_ber
      max_post_fec_ber = value['max_post_fec_ber']
      if post_fec_ber > max_post_fec_ber:
        value['max_post_fec_ber'] = post_fec_ber
      value['sum_post_fec_ber'] = value['sum_post_fec_ber'] + post_fec_ber
      min_pre_fec_ber = value['min_pre_fec_ber']
      if pre_fec_ber < min_pre_fec_ber:
        value['min_pre_fec_ber'] = pre_fec_ber
      max_pre_fec_ber = value['max_pre_fec_ber']
      if pre_fec_ber > max_pre_fec_ber:
        value['max_pre_fec_ber'] = pre_fec_ber
      value['sum_pre_fec_ber'] = value['sum_pre_fec_ber'] + pre_fec_ber
      value['num_passes'] += 1
      value['pre_fec_ber_list'].append(pre_fec_ber)
      value['post_fec_ber_list'].append(post_fec_ber)
      if not value['failed']:
        if failed:
          value['failed'] = True 
    margins[key] = value  # replace?

#######################################################################################################################
# margin_summary
#
# 
#
def margin_summary(tile_list, grp_list, ln_list):
  for tile in tile_list:
    for grp in grp_list:
      for ln in ln_list:
        value = margins[ (tile, grp, ln) ]
        # compute standard deviation
        n = value['num_passes']
        for i in range(n):
          # first compute avg
          avg_post_ber = value['sum_post_fec_ber'] / n
          avg_pre_ber  = value['sum_pre_fec_ber'] / n
        pre_diff_sum = 0
        post_diff_sum = 0
        for i in range(n):
          pre_diff_sum += (value['pre_fec_ber_list'][i] - avg_pre_ber)**2
          post_diff_sum += (value['post_fec_ber_list'][i] - avg_post_ber)**2
        pre_dev = math.sqrt(pre_diff_sum/n)
        post_dev = math.sqrt(post_diff_sum/n)
        print(str(tile) + ", " + str(grp) + ", " + str(ln) + " |  min: " + "%8.1e, " % (value['min_post_fec_ber']) + "max: " +  "%8.1e, " % (value['max_post_fec_ber']) + "avg: " + "%8.1e, " % (value['sum_post_fec_ber'] / value['num_passes']) + "std dev= " + "%8.1e, " % (post_dev) + "  |   min: " + "%8.1e, " % (value['min_pre_fec_ber']) + "max: " +  "%8.1e, " % (value['max_pre_fec_ber']) + "avg: " + "%8.1e, " % (value['sum_pre_fec_ber'] / value['num_passes']) + "std dev= " + "%8.1e, " % (pre_dev)),
        if value['failed']:
          print(" *** failed")
        elif ((value['sum_pre_fec_ber'] / value['num_passes']) > 1.0e-7):
          print(" *** high BER")
        else:
          print("")

#######################################################################################################################
# margin_summary_front_port_based
#
# 
#
def margin_summary_front_port_based(fp_range):
  for fp in fp_range:
    for ln in range(8):
        tile, grp = front_port_to_tile_grp(fp, ln)
        value = margins[ (tile, grp, ln) ]
        # compute standard deviation
        n = value['num_passes']
        for i in range(n):
          # first compute avg
          avg_post_ber = value['sum_post_fec_ber'] / n
          avg_pre_ber  = value['sum_pre_fec_ber'] / n
        pre_diff_sum = 0
        post_diff_sum = 0
        for i in range(n):
          pre_diff_sum += (value['pre_fec_ber_list'][i] - avg_pre_ber)**2
          post_diff_sum += (value['post_fec_ber_list'][i] - avg_post_ber)**2
        pre_dev = math.sqrt(pre_diff_sum/n)
        post_dev = math.sqrt(post_diff_sum/n)
        print(str(fp) + "/" + str(ln) + ", " + " |  min: " + "%8.1e, " % (value['min_post_fec_ber']) + "max: " +  "%8.1e, " % (value['max_post_fec_ber']) + "avg: " + "%8.1e, " % (value['sum_post_fec_ber'] / value['num_passes']) + "std dev= " + "%8.1e, " % (post_dev) + "  |   min: " + "%8.1e, " % (value['min_pre_fec_ber']) + "max: " +  "%8.1e, " % (value['max_pre_fec_ber']) + "avg: " + "%8.1e, " % (value['sum_pre_fec_ber'] / value['num_passes']) + "std dev= " + "%8.1e, " % (pre_dev)),
        if value['failed']:
          print(" *** failed")
        elif ((value['sum_pre_fec_ber'] / value['num_passes']) > 1.0e-7):
          print(" *** high BER")
        else:
          print("")

#######################################################################################################################
# bfn_dump_grps_margin_info_in_csv_fmt
#
# 
#
def bfn_dump_grps_margin_info_in_csv_fmt(iteration, tile, grp_list, ln_list):
  for g in grp_list:
    dump_margin_in_csv_fmt(iteration, tile, g)

#######################################################################################################################
# bfn_dump_all_margin_info_in_csv_fmt
#
# 
#
def bfn_dump_all_margin_info_in_csv_fmt(iteration, tile_list, grp_list, ln_list):
  for tile in tile_list:
    bfn_dump_grps_margin_info_in_csv_fmt(iteration, tile, grp_list, ln_list)

#######################################################################################################################
# bfn_pause_all_grps_fw
#
# 
#
def bfn_pause_all_grps_fw(tile, grp_list, pause=True):
  set_tile(tile)
  for g in grp_list:
    set_grp(g)
    if pause:
      fw_pause(fw_mode='all off', group=g)
    else:
      print("Tile: " + str(tile) + " grp: " + str(g))
      fw_pause(fw_mode='all on', group=g, print_en=False)

#######################################################################################################################
# bfn_pause_all_fw
#
# 
#
def bfn_pause_all_fw(tile_list, grp_list, pause=True):
  for tile in tile_list:
    bfn_pause_all_grps_fw(tile, grp_list, pause)
  print("")

def dump_all_lanes_lane_margin_in_csv_fmt(n, tile, g, ln_list):
  for ln in ln_list:
    dump_lane_margin_in_csv_fmt(n, tile, g, ln)

def dump_all_grps_lane_margin_in_csv_fmt(n, tile, grp_list, ln_list):
  for g in grp_list:
    dump_all_lanes_lane_margin_in_csv_fmt(n, tile, g, ln_list)

def dump_all_lane_margin_in_csv_fmt(n, tile_list, grp_list, ln_list):
  for tile in tile_list:
    dump_all_grps_lane_margin_in_csv_fmt(n, tile, grp_list, ln_list)

#######################################################################################################################
# TEST_Rx_EQ_repeatability
#
# 
#
def TEST_Rx_EQ_repeatability(repeat_count, in_csv_fmt, pause_fw = False):
  banner("Configure PAM4 PRBS31")
  bfn_config_all_lanes_pam4(test_tiles, prbs_mode='PRBS31')

  banner("Apply PN swaps")
  bfn_pn_swap_set(test_tiles)

  banner("TX EQ Settings")
  #bfn_tx_eq_set_2(bfn_working_tiles, 0x0200, 0xf600, 0x1400, 0x0000, 0x0000)
  bfn_tx_eq_set_2(test_tiles, TX_EQ_pre2, TX_EQ_pre1, TX_EQ_main, TX_EQ_post1, TX_EQ_post2)

  #bfn_tx_eq_dump(test_tiles, test_grps, test_lns)
  bfn_wait_all_sig_det_and_phy_rdy(test_tiles, test_grps, test_lns, tries=16, quiet=True)

  wait_all_adapt_done(test_tiles, test_grps, test_lns, tries=16, dwell=1.0, quiet=False)

  if pause_fw:
    bfn_pause_all_fw(test_tiles, test_grps, pause=True)

  for i in range(repeat_count):

    if in_csv_fmt:
      #print("#,Tile,group,lane, eye1, eye2, eye3, pre-FEC BER, post-FEC BER")
      print("#,tile,grp,ln,      eye1,      eye2,      eye3,  pre-FEC BER,     post-FEC BER, k1,   k2,  k3, k4, s1,s2,  Peak,G1,G2,  F0,   F1    ,F1/F0,F13")

    #bfn_dump_all_margin_info_in_csv_fmt(i, test_tiles, test_grps, test_lns)
    dump_all_lane_margin_in_csv_fmt(i, test_tiles, test_grps, test_lns)

    bfn_lane_reset_all(test_tiles, test_grps, test_lns)
    bfn_wait_all_sig_det_and_phy_rdy(test_tiles, test_grps, test_lns, tries=16, quiet=True)
    wait_all_adapt_done(test_tiles, test_grps, test_lns, tries=16, dwell=1.0, quiet=True)
    print("")

  margin_summary(test_tiles, test_grps, test_lns)

  #margin_summary(test_tiles, test_grps, test_lns)
  #margin_summary_front_port_based(range(1,33))
  exit()

#######################################################################################################################
# dump_iane_margin_in_csv_fmt
#
# 
#
def dump_lane_margin_in_csv_fmt(iteration, tile, grp, ln):
  global gLaneStats
  set_tile(tile)
  set_grp(grp)
  failed = False
  rx_monitor(lane=[ln], rst=1, lc=0, ph=0, group=grp, print_en=0, returnon=0)
  time.sleep(0.01)
  rx_monitor(lane=[ln], rst=0, lc=0, ph=0, group=grp, print_en=0, returnon=0)
  print(str(iteration) + ", " + str(tile) + ",   " + str(grp) + ", " + str(ln) + ", "),

  if (gLaneStats[ln][8] == 'RDY'):
    print ("%8.0f, " % (gLaneStats[ln][5])),
  else:
    failed = True
    print("     0.0, "),
  if (gLaneStats[ln][8] == 'RDY'):
    print ("%8.0f, " % (gLaneStats[ln][6])),
  else:
    failed = True
    print("     0.0, "),
  if (gLaneStats[ln][8] == 'RDY'):
    print ("%8.0f, " % (gLaneStats[ln][7])),
  else:
    failed = True
    print("     0.0, "),

  pre_fec_ber, post_fec_ber = compute_ber(ln)
  if pre_fec_ber == 0:
    print("       0, "),
  else:
    print("%8.1e, " % (pre_fec_ber)),
  if post_fec_ber == 0:
    print("       0, "),
  else:
    print("%8.1e, " % (post_fec_ber)),
  
  ffe_k1_bin, ffe_k2_bin, ffe_k3_bin, ffe_k4_bin, ffe_s1_bin, ffe_s2_bin = chip.PAM50[ln].ffe_taps()

  if ffe_k1_bin < 60:
    ffe_k1_bin, ffe_k2_bin, ffe_k3_bin, ffe_k4_bin, ffe_s1_bin, ffe_s2_bin = chip.PAM50[ln].ffe_taps()
    print('*******  %4d,%4d,%4d,%4d,%02X,%02X' %(ffe_k1_bin,ffe_k2_bin,ffe_k3_bin,ffe_k4_bin,ffe_s1_bin,ffe_s2_bin)),
    print("")
    ffe_k1_bin, ffe_k2_bin, ffe_k3_bin, ffe_k4_bin, ffe_s1_bin, ffe_s2_bin = chip.PAM50[ln].ffe_taps()
    print('         %4d,%4d,%4d,%4d,%02X,%02X' %(ffe_k1_bin,ffe_k2_bin,ffe_k3_bin,ffe_k4_bin,ffe_s1_bin,ffe_s2_bin)),
    print("")

  print('         %4d,%4d,%4d,%4d,%02X,%02X' %(ffe_k1_bin,ffe_k2_bin,ffe_k3_bin,ffe_k4_bin,ffe_s1_bin,ffe_s2_bin)),

  ctle_val = chip.PAM50[ln].ctle_pam4(); ctle_1 = chip.PAM50[ln].ctle_map_pam4(ctle_val)[0]; ctle_2 = chip.PAM50[ln].ctle_map_pam4(ctle_val)[1]
  print (", %d,   %d, %d" %(ctle_val,ctle_1,ctle_2)),
  f0, f1, f1f0_ratio = chip.PAM50[ln].get_pam4_dfe()
  f13_val = chip.PAM50[ln].f13()
  print (",  %4.2f, %4.2f, %5.2f, %2d" %(abs(f0),abs(f1),abs(f1f0_ratio),f13_val))


  key = (tile, grp, ln)
  try:
    value = margins[key]
  except:
    value = { 'min_post_fec_ber':post_fec_ber, 'max_post_fec_ber':post_fec_ber, 'sum_post_fec_ber':post_fec_ber, 'min_pre_fec_ber':pre_fec_ber, 'max_pre_fec_ber':pre_fec_ber, 'sum_pre_fec_ber':pre_fec_ber, 'num_passes':1, 'pre_fec_ber_list':[pre_fec_ber], 'post_fec_ber_list':[post_fec_ber], 'failed':failed }
  else:
    min_post_fec_ber = value['min_post_fec_ber']
    if post_fec_ber < min_post_fec_ber:
      value['min_post_fec_ber'] = post_fec_ber
    max_post_fec_ber = value['max_post_fec_ber']
    if post_fec_ber > max_post_fec_ber:
      value['max_post_fec_ber'] = post_fec_ber
    value['sum_post_fec_ber'] = value['sum_post_fec_ber'] + post_fec_ber
    min_pre_fec_ber = value['min_pre_fec_ber']
    if pre_fec_ber < min_pre_fec_ber:
      value['min_pre_fec_ber'] = pre_fec_ber
    max_pre_fec_ber = value['max_pre_fec_ber']
    if pre_fec_ber > max_pre_fec_ber:
      value['max_pre_fec_ber'] = pre_fec_ber
    value['sum_pre_fec_ber'] = value['sum_pre_fec_ber'] + pre_fec_ber
    value['num_passes'] += 1
    value['pre_fec_ber_list'].append(pre_fec_ber)
    value['post_fec_ber_list'].append(post_fec_ber)
    if not value['failed']:
      if failed:
        value['failed'] = True
  margins[key] = value  # replace?

#######################################################################################################################
# bfn_power_dn_all
#
# 
#
def bfn_power_dn_all():
  for tile in bfn_working_tiles:
    set_tile(tile)
    static_power_down_broadcast_mode(group=range(8), lanes=range(8))

#######################################################################################################################
# TEST_xtalk
#
# 
#
def TEST_xtalk(repeat_count, in_csv_fmt):
  banner("Cross-Talk Test")

  print("note: This test requires a logical lane with tx and rx mapped the same")
  test_tiles = [2]
  test_grps = [0]
  test_lns = [4]
  tile = test_tiles[0]
  g = test_grps[0]
  ln = test_lns[0]
  print("Currently using tile " + str(tile) + " grp" + str(g) + " ln" + str(ln))

  banner("Power-off ALL lanes")
  for tile in bfn_working_tiles:
    set_tile(tile)
    for g in range(8):
      set_grp(g)
      for ln in range(8):
        if ln == test_lns[0]: continue
        # power down all other lanes
        static_power_down(group=g, lane=[ln], rx_off=1, tx_off=1, rx_bg_off=0 , tx_bg_off=0)
  
  for tile in test_tiles:
    set_tile(tile)
    for g in test_grps:
      set_grp(g)
      for ln in test_lns:
        print("Tile " + str(tile) + " : grp" + str(g) + " : ln" + str(ln) + "..")
        # turn off all serdes in group
        for other_ln in range(8):
          if other_ln == ln:
            # power up DUT lane
            static_power_down(group=g, lane=[other_ln], rx_off=0, tx_off=0, rx_bg_off=0 , tx_bg_off=0)
          else:
            # power down all other lanes
            static_power_down(group=g, lane=[other_ln], rx_off=1, tx_off=1, rx_bg_off=0 , tx_bg_off=0)

        # config only 1 lane
        print("config ln in PAM4")
        bfn_config_ln_pam4(tile, g, ln, 'PRBS31')

        banner("Apply PN swaps")
        bfn_pn_swap_set([tile])

        # apply Tx EQ settings
        pre2 = 0x0300
        pre1 = 0xf100
        main = 0x1500
        post1 = 0x0000
        post2 = 0x0000
        print("pre2=" + hex(pre2) + " pre1=" + hex(pre1) + " main=" + hex(main) + " post1=" + hex(post1) + " post2=" + hex(post2))

        reg_wr(0xA5 | (ln<<11), pre2) #PRE2
        reg_wr(0xA7 | (ln<<11), pre1) #PRE1
        reg_wr(0xA9 | (ln<<11), main) #MAIN
        reg_wr(0xAB | (ln<<11), post1) #POST1
        reg_wr(0xAD | (ln<<11), post2) #POST1

        banner("Check SIGNAL-DETECT and PHY-READY")
        dump_all_sig_detect(tile_list=test_tiles, grp_list=test_grps, ln_list=range(8))

        banner("Wait Rx EQ adaptation Complete")
        wait_all_adapt_done([tile], [g], [ln], tries=16, dwell=1.0, quiet=False)
        dump_all_adapt_done(tile_list=test_tiles, grp_list=test_grps, ln_list=test_lns)

        print("By itself ..")

        if in_csv_fmt:
          print("#,tile,grp,ln,      eye1,      eye2,      eye3,  pre-FEC BER,     post-FEC BER, k1,   k2,  k3, k4, s1,s2,  Peak,G1,G2,  F0,   F1    ,F1/F0,F13")

        for cnt in range(repeat_count):
          # reset to restart Adaptation
          chip.PAM50[ln].lane_reset_fast_pam4()
          bfn_wait_all_sig_det_and_phy_rdy([tile], [g], [ln], tries=16, quiet=True)
          wait_all_adapt_done([tile], [g], [ln], tries=16, dwell=1.0, quiet=True)
          #for n in range(32):
          #  rx_eq_dump_quick(n, tile, g, ln)

          dump_lane_margin_in_csv_fmt(cnt, tile, g, ln)
        margin_summary([tile], [g], [ln])

        for l2 in range(8):
          if l2 == ln: continue 
          print("With new aggressor: ln" + str(l2))

          if in_csv_fmt:
            print("#,tile,grp,ln,      eye1,      eye2,      eye3,  pre-FEC BER,     post-FEC BER, k1,   k2,  k3, k4, s1,s2,  Peak,G1,G2,  F0,   F1    ,F1/F0,F13")

          static_power_down(group=g, lane=[l2], rx_off=0, tx_off=0, rx_bg_off=0 , tx_bg_off=0)
          time.sleep(1.0)
          # config all lanes
          bfn_config_ln_pam4(tile, g, l2, 'PRBS31')
          reg_wr(0xA5 | (l2<<11), pre2) #PRE2
          reg_wr(0xA7 | (l2<<11), pre1) #PRE1
          reg_wr(0xA9 | (l2<<11), main) #MAIN
          reg_wr(0xAB | (l2<<11), post1) #POST1
          reg_wr(0xAD | (l2<<11), post2) #POST1

          for cnt in range(repeat_count):
            # reset to restart Adaptation
            chip.PAM50[ln].lane_reset_fast_pam4()
            # wait for sig-detect and phy-ready
            bfn_wait_all_sig_det_and_phy_rdy([tile], [g], [ln], tries=16, quiet=True)
            #dump_all_sig_detect([tile], [g], range(8))
            wait_all_adapt_done([tile], [g], [ln], tries=16, dwell=1.0, quiet=True)
            #dump_all_adapt_done([tile], [g], range(8))
            dump_lane_margin_in_csv_fmt(cnt, tile, g, ln)
          margin_summary([tile], [g], [ln])
  exit()

#######################################################################################################################
# rx_eq_dump_quick
#
# 
#
def rx_eq_dump_quick(iteration, tile, grp, ln):
  rx_monitor(lane=[ln], rst=1, lc=0, ph=0, group=grp, print_en=0, returnon=0)
  rx_monitor(lane=[ln], rst=0, lc=0, ph=0, group=grp, print_en=0, returnon=0)
  compute_ber(ln)

  print('%4d, '%iteration + ", " + str(tile) + ",   " + str(grp) + ",  " + str(ln) + ", "),
  sd, rdy = chip.PAM50[ln].ready_pam4()
  ad = adapt_done(chip,ln)
  print(str(sd) + ",  " + str(rdy) + ",   " +  str(ad) + ", "),
  ffe_k1_bin, ffe_k2_bin, ffe_k3_bin, ffe_k4_bin, ffe_s1_bin, ffe_s2_bin = chip.PAM50[ln].ffe_taps()
  print('         %4d,%4d,%4d,%4d,%02X,%02X' %(ffe_k1_bin,ffe_k2_bin,ffe_k3_bin,ffe_k4_bin,ffe_s1_bin,ffe_s2_bin)),


  ctle_val = chip.PAM50[ln].ctle_pam4(); ctle_1 = chip.PAM50[ln].ctle_map_pam4(ctle_val)[0]; ctle_2 = chip.PAM50[ln].ctle_map_pam4(ctle_val)[1]
  print (", %d,   %d, %d" %(ctle_val,ctle_1,ctle_2)),
  f0, f1, f1f0_ratio = chip.PAM50[ln].get_pam4_dfe()
  f13_val = chip.PAM50[ln].f13()
  print (",  %4.2f, %4.2f, %5.2f, %2d" %(abs(f0),abs(f1),abs(f1f0_ratio),f13_val))

  #if ffe_k1_bin < 60: exit()

#######################################################################################################################
# TEST_ffe_analyzer
#
# 
#
def TEST_ffe_analyzer():
  banner("FFE tap analyzer Test")

  print("note: This test requires a logical lane with tx and rx mapped the same")
  tile = 3
  g = 0
  ln = 6
  print("Currently using tile " + str(tile) + " grp" + str(g) + " ln" + str(ln))

  set_tile(tile)
  set_grp(g)

  # apply Tx EQ settings
  pre2 = 0x0200
  pre1 = 0xf600
  main = 0x1400
  post1 = 0x0000
  post2 = 0x0000
  print("pre2=" + hex(pre2) + " pre1=" + hex(pre1) + " main=" + hex(main) + " post1=" + hex(post1) + " post2=" + hex(post2))

  # turn off all serdes in group
  for l2 in range(8):
    if l2 == ln:
      # power up DUT lane
      print("Tile " + str(tile) + " : grp" + str(g) + " : ln" + str(l2) + " Power-up")
      static_power_down(group=g, lane=[l2], rx_off=0, tx_off=0, rx_bg_off=0 , tx_bg_off=0)
    else:
      # power down all other lanes
      print("Tile " + str(tile) + " : grp" + str(g) + " : ln" + str(l2) + " Power-dn")
      static_power_down(group=g, lane=[l2], rx_off=1, tx_off=1, rx_bg_off=0 , tx_bg_off=0)

  # config only 1 lane
  banner("config ln in PAM4")
  bfn_config_ln_pam4(tile, g, ln, 'PRBS31')

  banner("Apply PN swaps")
  bfn_pn_swap_set([tile])

  reg_wr(0xA5 | (ln<<11), pre2) #PRE2
  reg_wr(0xA7 | (ln<<11), pre1) #PRE1
  reg_wr(0xA9 | (ln<<11), main) #MAIN
  reg_wr(0xAB | (ln<<11), post1) #POST1
  reg_wr(0xAD | (ln<<11), post2) #POST1

  # fire up adjacent lanes
  #for l2 in range(8):
  #  if l2 == ln: continue
  #  bfn_config_ln_pam4(tile, g, l2, 'PRBS31')

  # wait for sig-detect and phy-ready
  bfn_wait_all_sig_det_and_phy_rdy([tile], [g], [ln], tries=16, quiet=True)
  wait_all_adapt_done([tile], [g], [ln], tries=16, dwell=1.0, quiet=True)

  banner("Lane reset DUT lane")
  # reset to restart Adaptation
  print("tile=" + str(tile) + " g=" + str(g) + " ln=" + str(ln))
  chip.PAM50[ln].lane_reset_fast_pam4()

  for _pass in range(1):
    for section in range(5):
      print_date_time()
      print("pass, tile, grp, ln, sd, rdy, ad,           k1,   k2,  k3, k4, s1,s2,  Peak,G1,G2,  F0,   F1    ,F1/F0,F13")
      for iteration in range(20):
        rx_eq_dump_quick(_pass*20*5 + section*20 + iteration, tile, g, ln)
      print("")
    dump_lane_margin_in_csv_fmt(_pass, tile, g, ln)
    print("")

  exit()

#######################################################################################################################
# TEST_all_lanes_remote_loopback
#
# 
#
def TEST_all_lanes_remote_loopback():
  if default_mode == 'nrz':
    banner("Configure NRZ PRBS31 mode")
    bfn_config_all_lanes_nrz(bfn_working_tiles, prbs_mode='functional')
  else:
    banner("Configure PAM4 PRBS31 mode")
    bfn_config_all_lanes_pam4(bfn_working_tiles, prbs_mode='functional')

  banner("Tx EQ")
  #bfn_tx_eq_set(bfn_working_tiles, 0xfe00, 0x1300, 0x0000)
  #bfn_tx_eq_set(bfn_working_tiles, 0xfe00, 0x1900, 0x0000)
  #bfn_tx_eq_set(bfn_working_tiles, 0xfa00, 0x0f00, 0x0000)
  #working?
  #bfn_tx_eq_set(bfn_working_tiles, 0xf600, 0x1400, 0x0000)  #Spirent w/1 or 2m amphenol
  #bfn_tx_eq_set(bfn_working_tiles, 0xf300, 0x0f00, 0x0000)
  #bfn_tx_eq_set(bfn_working_tiles, 0xf600, 0x1100, 0x0000)
  #bfn_tx_eq_set(bfn_working_tiles, 0xf600, 0x1000, 0x0000) # Credo active cable
  bfn_tx_eq_set_2(bfn_working_tiles, TX_EQ_pre2, TX_EQ_pre1, TX_EQ_main, TX_EQ_post1, TX_EQ_post2)

  bfn_tx_eq_dump(bfn_working_tiles, range(8), range(8))

  banner("Configure Rx-Tx (remote) Serial Loopback")
  for tile in bfn_working_tiles:
    set_tile(tile)
    for g in range(0,8):
      set_grp(g)
      for ln in range(0,8):
        rx_tx_serial_loopback(g, lane=ln, enable=1, TX_pat=None, RX_pat=None)

  banner("Apply PN swaps")
  bfn_pn_swap_set([tile])

  banner("Wait for Rx Signal-Detect and PHY-Ready")
  bfn_wait_all_sig_detect(test_tiles, test_grps, test_lns, 10, 0.5, True)
  dump_all_sig_detect(test_tiles, test_grps, test_lns)

  banner("Wait Rx EQ adaptation Complete")
  wait_all_adapt_done(test_tiles, test_grps, test_lns, tries=16, dwell=1.0, quiet=False)
  dump_all_adapt_done(test_tiles, test_grps, test_lns)

  inp = 's'
  while inp != 'q':
    print_date_time()
    dump_all_prbs_errors(test_tiles, test_grps, cur_dwell_time)

    print("=========================================")
    inp = raw_input("Enter key to continue> ")
    bfn_command_menu(inp)
  exit()


#######################################################################################################################
# bfn_wait_lanes_sig_det_and_phy_rdy
#
# 
#
def bfn_wait_lanes_sig_det_and_phy_rdy(tile, g, ln_list, retries=10, dwell=1.0):
  set_tile(tile)
  set_grp(g)

  for ln in ln_list:
    ln_mode = bfn_get_lane_mode(tile, g, ln)
    if ln_mode == 'pam4':
      sd, rdy = chip.PAM50[ln].ready_pam4()
    else:
      sd, rdy = chip.NRZ25[ln].ready_nrz()
    if (not sd) or (not rdy):
      return sd, rdy, ln
  return sd, rdy, ln

#######################################################################################################################
# bfn_wait_grps_sig_det_and_phy_rdy
#
# 
#
def bfn_wait_grps_sig_det_and_phy_rdy(tile, grp_list, ln_list, retries=10, dwell=1.0):
  for g in grp_list:
    sd, rdy, ln = bfn_wait_lanes_sig_det_and_phy_rdy(tile, g, ln_list, retries, dwell)
    if (not sd) or (not rdy):
      return sd, rdy, ln, g
  return sd, rdy, ln, g

#######################################################################################################################
# bfn_wait_sig_det_and_phy_rdy
#
# 
#
def bfn_wait_sig_det_and_phy_rdy(tile_list, grp_list, ln_list, retries=10, dwell=1.0):
  for tile in tile_list:
    sd, rdy, ln, g = bfn_wait_grps_sig_det_and_phy_rdy(tile, grp_list, ln_list, retries, dwell)
    if (not sd) or (not rdy):
      return sd, rdy, ln, g, tile
  return sd, rdy, ln, g, tile

#######################################################################################################################
# bfn_wait_all_sig_det_and_phy_rdy
#
# 
#
def bfn_wait_all_sig_det_and_phy_rdy(tile_list, grp_list, ln_list, tries=16, dwell=1.0, quiet=False):
  for attempt in range(tries):
    sd, rdy, ln, g, tile = bfn_wait_sig_det_and_phy_rdy(tile_list, grp_list, ln_list, 1, 0.0)
    if (not sd) or (not rdy):
      if not quiet:
        print("Waiting on tile " + str(tile) + " grp" + str(g) + " ln" + str(ln) + " : sig-det=" + str(sd) + " phy-rdy=" + str(rdy))
    time.sleep(dwell)

#######################################################################################################################
# dump_ln_sig_detect
#
# 
#
def dump_ln_sig_detect(tile, g, ln):
  set_tile(tile)
  set_grp(g)

  ln_mode = bfn_get_lane_mode(tile, g, ln)
  if ln_mode == 'pam4':
    sd,rdy = chip.PAM50[ln].ready_pam4()
  else:
    sd, rdy = chip.NRZ25[ln].ready_nrz()

  if (sd == 0) and (rdy == 0):
    char = "-"
  if (sd == 1) and (rdy == 0):
    char = "."
  if (sd == 0) and (rdy == 1):
    char = "?"
  if (sd == 1) and (rdy == 1):
    char = "*"
  print(char + "|"),

#######################################################################################################################
# dump_all_lanes_sig_detect
#
# 
#
def dump_all_lanes_sig_detect(tile, g, ln_list):
  for ln in ln_list:
    dump_ln_sig_detect(tile, g, ln)

#######################################################################################################################
# dump_all_grps_sig_detect
#
# 
#
def dump_all_grps_sig_detect(tile, grp_list, ln_list):
  for g in grp_list:
    print("grp" + str(g) + " |"),
    dump_all_lanes_sig_detect(tile, g, ln_list)
    print("")

#######################################################################################################################
# dump_all_sig_detect
#
# 
#
def dump_all_sig_detect(tile_list, grp_list, ln_list):
  for tile in tile_list:
    print("----------------------------------")
    dump_all_grps_sig_detect(tile, grp_list, ln_list)
    print("----------------------------------")

#######################################################################################################################
# dump_ln_adapt_done
#
# 
#
def dump_ln_adapt_done(tile, g, ln):
  set_tile(tile)
  set_grp(g)
  ad = adapt_done(chip,ln)
  if (ad == 0):
    char = "-"
  if (ad == 1):
    char = "*"
  print(char + "|"),

#######################################################################################################################
# dump_all_adapt_done
#
# 
#
def dump_all_adapt_done(tile_list, grp_list, ln_list):
  for tile in tile_list:
    print("----------------------------------")
    for g in grp_list:
      print("grp" + str(g) + " |"),
      for ln in ln_list:
        dump_ln_adapt_done(tile, g, ln)
      print("")
    print("----------------------------------")

#######################################################################################################################
# wait_lanes_adapt_done
#
# 
#
def wait_lanes_adapt_done(tile, g, ln_list, tries=16, dwell=1.0, quiet=False):
    set_tile(tile)
    set_grp(g)
    ad = False
    for ln in ln_list:
      ad = adapt_done(chip,ln)
      if not ad:
        return ad, ln
    return ad, ln

#######################################################################################################################
# wait_grps_adapt_done
#
# 
#
def wait_grps_adapt_done(tile, grp_list, ln_list, tries=16, dwell=1.0, quiet=False):
  for g in grp_list:
    ad, ln = wait_lanes_adapt_done(tile, g, ln_list, tries, dwell, quiet)
    if not ad:
      return ad, ln, g
  return ad, ln, g
  
#######################################################################################################################
# wait_all_adapt_done
#
# 
#
def wait_all_adapt_done(tile_list, grp_list, ln_list, tries=16, dwell=1.0, quiet=False):
  for attempt in range(tries):
    for tile in tile_list:
      ad, ln, g = wait_grps_adapt_done(tile, grp_list, ln_list, tries, dwell, quiet)
      if not ad:
        if not quiet:
          print("Waiting on tile " + str(tile) + " grp" + str(g) + " ln" + str(ln) + " : adapt-done=" + str(ad))
          time.sleep(dwell)
          break

#######################################################################################################################
# clear_all_lanes_prbs_error_counts
#
# 
#
def clear_all_lanes_prbs_error_counts(tile, g, ln_list):
  set_tile(tile)
  set_grp(g)
  for ln in ln_list:
    ln_mode = bfn_get_lane_mode(tile, g, ln)
    if ln_mode == 'pam4':
      chip.PAM50[ln].prbs_rst_pam4()
    else:
      chip.NRZ25[ln].prbs_rst_nrz()

#######################################################################################################################
# clear_all_grps_prbs_error_counts
#
# 
#
def clear_all_grps_prbs_error_counts(tile, grp_list, ln_list):
  for g in grp_list:
    clear_all_lanes_prbs_error_counts(tile, g, ln_list)

#######################################################################################################################
# clear_all_prbs_error_counts
#
# 
#
def clear_all_prbs_error_counts(tile_list, grp_list, ln_list):
  for tile in tile_list:
    clear_all_grps_prbs_error_counts(tile, grp_list, ln_list)

#######################################################################################################################
# bfn_check_lane_map
#
# 
#
def bfn_check_lane_map(tile, g):
  set_tile(tile)
  set_grp(g)
  check_logical_physical_map(group=g)

#######################################################################################################################
# bfn_check_grps_lane_map
#
# 
#
def bfn_check_grps_lane_map(tile, grp_list):
  for g in grp_list:
    bfn_check_lane_map(tile, g)

#######################################################################################################################
# bfn_check_all_lane_map
#
# 
#
def bfn_check_all_lane_map(tile_list, grp_list):
  for tile in tile_list:
    bfn_check_grps_lane_map(tile, grp_list)

#######################################################################################################################
# wait_for_ln_ffe_to_stabilize
#
# 
#
def wait_for_ln_ffe_to_stabilize(tile, g, ln, tries=60, wait=1.0, print_en=1):
  set_tile(tile)
  set_grp(g)
  # skip lanes that failed to even adapt
  ad = adapt_done(chip,ln)
  if not ad:
    if print_en:
        print(str(tile) + ":" + str(g) + ":" + str(ln) + " : Failed to adapt. skip..")
    return
  prev_ffe_k1_bin = 0
  prev_ffe_k2_bin = 0
  prev_ffe_k3_bin = 0
  prev_ffe_k4_bin = 0
  prev_ffe_s1_bin = 0
  prev_ffe_s2_bin = 0

  for t in range(tries):
    stable = True 
    ffe_k1_bin, ffe_k2_bin, ffe_k3_bin, ffe_k4_bin, ffe_s1_bin, ffe_s2_bin = chip.PAM50[ln].ffe_taps()
    if ffe_k1_bin != prev_ffe_k1_bin: stable = False
    if ffe_k2_bin != prev_ffe_k2_bin: stable = False
    if ffe_k3_bin != prev_ffe_k3_bin: stable = False
    if ffe_k4_bin != prev_ffe_k4_bin: stable = False
    if ffe_s1_bin != prev_ffe_s1_bin: stable = False
    if ffe_s2_bin != prev_ffe_s2_bin: stable = False
    if not stable: 
      prev_ffe_k1_bin = ffe_k1_bin
      prev_ffe_k2_bin = ffe_k2_bin
      prev_ffe_k3_bin = ffe_k3_bin
      prev_ffe_k4_bin = ffe_k4_bin
      prev_ffe_s1_bin = ffe_s1_bin
      prev_ffe_s2_bin = ffe_s2_bin
      if print_en:
        print(str(tile) + ":" + str(g) + ":" + str(ln) + " : k1=" + str(ffe_k1_bin) + " : k2=" + str(ffe_k2_bin) + " : k3=" + str(ffe_k3_bin) + " : k4=" + str(ffe_k4_bin) + " : s1=" + str(ffe_s1_bin) + " : s2=" + str(ffe_s2_bin))
      time.sleep(wait)
    else:
      return

#######################################################################################################################
# wait_for_lanes_ffe_to_stabilize
#
# 
#
def wait_for_lanes_ffe_to_stabilize(tile, g, ln_list, tries=60, wait=1.0, print_en=1):
  for ln in ln_list:
     wait_for_ln_ffe_to_stabilize(tile, g, ln, tries, wait, print_en)

#######################################################################################################################
# wait_for_grps_ffe_to_stabilize
#
# 
#
def wait_for_grps_ffe_to_stabilize(tile, grp_list, ln_list, tries=60, wait=1.0, print_en=1):
  for g in grp_list:
    wait_for_lanes_ffe_to_stabilize(tile, g, ln_list, tries, wait, print_en)

#######################################################################################################################
# wait_for_ffe_to_stabilize
#
# 
#
def wait_for_ffe_to_stabilize(tile_list, grp_list, ln_list, tries=60, wait=1.0, print_en=1):
  for tile in tile_list:
    wait_for_grps_ffe_to_stabilize(tile, grp_list, ln_list, tries, wait, print_en)


#######################################################################################################################
# TEST_all_lanes_prbs
#
# 
#
def TEST_all_lanes_prbs():
  if default_mode.upper() == 'PAM4':
    banner("Configure PAM4 PRBS31")
    bfn_config_all_lanes_pam4(bfn_working_tiles, prbs_mode='PRBS31')
  else:
    banner("Configure NRZ PRBS31")
    bfn_config_all_lanes_nrz(bfn_working_tiles, prbs_mode='PRBS31')

  banner("Apply PN swaps")
  bfn_pn_swap_set(bfn_working_tiles)

  banner("TX EQ Settings")
  #bfn_tx_eq_set(bfn_working_tiles, 0xfe00, 0x1300, 0x0000)
  #bfn_tx_eq_set(bfn_working_tiles, 0xfe00, 0x1900, 0x0000)
  #bfn_tx_eq_set(bfn_working_tiles, 0xf800, 0x1100, 0x0000)
  #bfn_tx_eq_set(bfn_working_tiles, 0xf800, 0x0f00, 0x0000)
  #bfn_tx_eq_set(bfn_working_tiles, 0xfa00, 0x0f00, 0x0000)
  #bfn_tx_eq_set(bfn_working_tiles, 0xf600, 0x1400, 0x0000)
  #bfn_tx_eq_set_2(bfn_working_tiles, 0x0200, 0xf600, 0x1400, 0x0000, 0x0000)
  bfn_tx_eq_set_2(bfn_working_tiles, TX_EQ_pre2, TX_EQ_pre1, TX_EQ_main, TX_EQ_post1, TX_EQ_post2)
  bfn_tx_eq_settings_dump()

  banner("Wait for Rx Signal-Detect and PHY-Ready")
  bfn_wait_all_sig_det_and_phy_rdy(bfn_working_tiles, range(8), range(8), tries=16, dwell=1.0, quiet=False)
  dump_all_sig_detect(bfn_working_tiles, range(9), range(8))

  banner("Wait Rx EQ adaptation Complete")
  wait_all_adapt_done(bfn_working_tiles, range(8), range(8), tries=16, dwell=1.0, quiet=False)
  dump_all_adapt_done(bfn_working_tiles, range(9), range(8))

  while True:
    print_date_time()
    dump_all_prbs_errors(test_tiles, test_grps, cur_dwell_time)

    print("=========================================")
    inp = raw_input("Enter key to continue> ")
    bfn_command_menu(inp)

#######################################################################################################################
# TEST_all_lanes_prbs_both_enc_modes
#
# 
#
def TEST_all_lanes_prbs_both_enc_modes():
 while True:

  if default_mode.upper() == 'PAM4':
    banner("Configure PAM4 PRBS31")
    bfn_config_all_lanes_pam4(bfn_working_tiles, prbs_mode='PRBS31')
  else:
    banner("Configure NRZ PRBS31")
    bfn_config_all_lanes_nrz(bfn_working_tiles, prbs_mode='PRBS31')

  banner("Apply PN swaps")
  bfn_pn_swap_set(bfn_working_tiles)

  banner("TX EQ Settings")
  #bfn_tx_eq_set(bfn_working_tiles, 0xfe00, 0x1300, 0x0000)
  #bfn_tx_eq_set(bfn_working_tiles, 0xfe00, 0x1900, 0x0000)
  #bfn_tx_eq_set(bfn_working_tiles, 0xf800, 0x1100, 0x0000)
  #bfn_tx_eq_set(bfn_working_tiles, 0xf800, 0x0f00, 0x0000)
  #bfn_tx_eq_set(bfn_working_tiles, 0xfa00, 0x0f00, 0x0000)
  #bfn_tx_eq_set(bfn_working_tiles, 0xf600, 0x1400, 0x0000)
  #bfn_tx_eq_set_2(bfn_working_tiles, 0x0200, 0xf600, 0x1400, 0x0000, 0x0000)
  bfn_tx_eq_set_2(bfn_working_tiles, TX_EQ_pre2, TX_EQ_pre1, TX_EQ_main, TX_EQ_post1, TX_EQ_post2)
  bfn_tx_eq_settings_dump()

  banner("Wait for Rx Signal-Detect and PHY-Ready")
  bfn_wait_all_sig_det_and_phy_rdy(bfn_working_tiles, range(8), range(8), tries=16, dwell=1.0, quiet=False)
  dump_all_sig_detect(bfn_working_tiles, range(9), range(8))

  banner("Wait Rx EQ adaptation Complete")
  wait_all_adapt_done(bfn_working_tiles, range(8), range(8), tries=16, dwell=1.0, quiet=False)
  dump_all_adapt_done(bfn_working_tiles, range(9), range(8))

  while True:
    print_date_time()
    dump_all_prbs_errors(test_tiles, test_grps, cur_dwell_time)

    print("=========================================")
    inp = raw_input("Enter key to continue> ")
    if inp == 'c': break
    bfn_command_menu(inp)

  banner("Reset all Groups")
  bfn_group_reset_all(bfn_working_tiles)

  banner("Set Lane Maps")  
  bfn_lane_map_set(bfn_working_tiles)

  if default_mode.upper() != 'PAM4':
    banner("Configure PAM4 PRBS31")
    bfn_config_all_lanes_pam4(bfn_working_tiles, prbs_mode='PRBS31')
  else:
    banner("Configure NRZ PRBS31")
    bfn_config_all_lanes_nrz(bfn_working_tiles, prbs_mode='PRBS31')

  banner("Apply PN swaps")
  bfn_pn_swap_set(bfn_working_tiles)

  banner("TX EQ Settings")
  #bfn_tx_eq_set(bfn_working_tiles, 0xfe00, 0x1300, 0x0000)
  #bfn_tx_eq_set(bfn_working_tiles, 0xfe00, 0x1900, 0x0000)
  #bfn_tx_eq_set(bfn_working_tiles, 0xf800, 0x1100, 0x0000)
  #bfn_tx_eq_set(bfn_working_tiles, 0xf800, 0x0f00, 0x0000)
  #bfn_tx_eq_set(bfn_working_tiles, 0xfa00, 0x0f00, 0x0000)
  #bfn_tx_eq_set(bfn_working_tiles, 0xf600, 0x1400, 0x0000)
  #bfn_tx_eq_set_2(bfn_working_tiles, 0x0200, 0xf600, 0x1400, 0x0000, 0x0000)
  bfn_tx_eq_set_2(bfn_working_tiles, TX_EQ_pre2, TX_EQ_pre1, TX_EQ_main, TX_EQ_post1, TX_EQ_post2)
  bfn_tx_eq_settings_dump()

  banner("Wait for Rx Signal-Detect and PHY-Ready")
  bfn_wait_all_sig_det_and_phy_rdy(bfn_working_tiles, range(8), range(8), tries=16, dwell=1.0, quiet=False)
  dump_all_sig_detect(bfn_working_tiles, range(9), range(8))

  banner("Wait Rx EQ adaptation Complete")
  wait_all_adapt_done(bfn_working_tiles, range(8), range(8), tries=16, dwell=1.0, quiet=False)
  dump_all_adapt_done(bfn_working_tiles, range(9), range(8))

  while True:
    print_date_time()
    dump_all_prbs_errors(test_tiles, test_grps, cur_dwell_time)

    print("=========================================")
    inp = raw_input("Enter key to continue> ")
    if inp == 'c': break
    bfn_command_menu(inp)

  banner("Reset all Groups")
  bfn_group_reset_all(bfn_working_tiles)

  banner("Set Lane Maps")  
  bfn_lane_map_set(bfn_working_tiles)


#######################################################################################################################
# TEST_all_lanes_prbs_1g
#
# 
#
#######################################################################################################################
# if you want to enable tx_rx loopback, you need to connect another lane's rx to test lane's tx as a termination      #
#######################################################################################################################
#######################################################################################################################
# bfn_lane_internal_loopback
#
# 
#
def bfn_lane_internal_loopback(tile, g, ln, phase):
  set_tile(tile)
  set_grp(g)
  bfn_tx_rx_serial_loopback(g, ln, enable=1, phase=phase)

#######################################################################################################################
# bfn_all_lanes_internal_loopback
#
# 
#
def bfn_all_lanes_internal_loopback(tile, g, ln_list, phase):
  for ln in ln_list:
    bfn_lane_internal_loopback(tile, g, ln, phase)

#######################################################################################################################
# bfn_all_grps_internal_loopback
#
# 
#
def bfn_all_grps_internal_loopback(tile, grp_list, ln_list, phase):
  for g in grp_list:
    set_grp(g)
    if phase == 0:
      set_logical_physical_map(group=g, tx_val0=0x0, tx_val1=0x1, tx_val2=0x2, tx_val3=0x3, tx_val4=0x4, tx_val5=0x5, tx_val6=0x6, tx_val7=0x7, rx_val0=0x0, rx_val1=0x1, rx_val2=0x2, rx_val3=0x3, rx_val4=0x4, rx_val5=0x5, rx_val6=0x6, rx_val7=0x7)
    
    bfn_all_lanes_internal_loopback(tile, g, ln_list, phase)

#######################################################################################################################
# bfn_all_internal_loopback
#
# 
#
def bfn_all_internal_loopback(tile_list, grp_list, ln_list):
  for tile in tile_list:
    bfn_all_grps_internal_loopback(tile, grp_list, ln_list, phase=0)
  time.sleep(3)
  for tile in tile_list:
    bfn_all_grps_internal_loopback(tile, grp_list, ln_list, phase=1)
  time.sleep(0.1)
  for tile in tile_list:
    bfn_all_grps_internal_loopback(tile, grp_list, ln_list, phase=2)

#######################################################################################################################
# TEST_all_lanes_prbs_internal_loopback
#
# 
#
def TEST_all_lanes_prbs_internal_loopback():
  banner("Configure 1G NRZ PRBS31")
  bfn_config_all_lanes_nrz(bfn_working_tiles, prbs_mode='PRBS31', speed=1)

  #banner("Apply PN swaps")
  #bfn_pn_swap_set(bfn_working_tiles)

  banner("Set TX-RX (internal) Serial loopback")
  bfn_all_internal_loopback(bfn_working_tiles, range(9), range(8))

  banner("TX EQ Settings")
  #bfn_tx_eq_set(bfn_working_tiles, 0xfe00, 0x1300, 0x0000)
  #bfn_tx_eq_set(bfn_working_tiles, 0xfe00, 0x1900, 0x0000)
  #bfn_tx_eq_set(bfn_working_tiles, 0xf800, 0x1100, 0x0000)
  #bfn_tx_eq_set(bfn_working_tiles, 0xf800, 0x0f00, 0x0000)
  #bfn_tx_eq_set(bfn_working_tiles, 0xfa00, 0x0f00, 0x0000)
  #bfn_tx_eq_set(bfn_working_tiles, 0xf600, 0x1400, 0x0000)
  #bfn_tx_eq_set_2(bfn_working_tiles, 0x0200, 0xf600, 0x1400, 0x0000, 0x0000)
  bfn_tx_eq_set_2(bfn_working_tiles, TX_EQ_pre2, TX_EQ_pre1, TX_EQ_main, TX_EQ_post1, TX_EQ_post2)
  bfn_tx_eq_settings_dump()

  banner("Wait for Rx Signal-Detect and PHY-Ready")
  bfn_wait_all_sig_det_and_phy_rdy(bfn_working_tiles, range(8), range(8), tries=16, dwell=1.0, quiet=False)
  dump_all_sig_detect(bfn_working_tiles, range(9), range(8))

  banner("Wait Rx EQ adaptation Complete")
  wait_all_adapt_done(bfn_working_tiles, range(8), range(8), tries=16, dwell=1.0, quiet=False)
  dump_all_adapt_done(bfn_working_tiles, range(9), range(8))

  while True:
    print_date_time()
    dump_all_prbs_errors(test_tiles, test_grps, cur_dwell_time)

    print("=========================================")
    inp = raw_input("Enter key to continue> ")
    bfn_command_menu(inp)

#######################################################################################################################
# TEST_all_lanes_prbs_1g
#
# 
#
def TEST_all_lanes_prbs_1g():
  banner("Configure 1G NRZ PRBS31")
  bfn_config_all_lanes_nrz(bfn_working_tiles, prbs_mode='PRBS31', speed=1)

  banner("Apply PN swaps")
  bfn_pn_swap_set(bfn_working_tiles)

  banner("TX EQ Settings")
  #bfn_tx_eq_set(bfn_working_tiles, 0xfe00, 0x1300, 0x0000)
  #bfn_tx_eq_set(bfn_working_tiles, 0xfe00, 0x1900, 0x0000)
  #bfn_tx_eq_set(bfn_working_tiles, 0xf800, 0x1100, 0x0000)
  #bfn_tx_eq_set(bfn_working_tiles, 0xf800, 0x0f00, 0x0000)
  #bfn_tx_eq_set(bfn_working_tiles, 0xfa00, 0x0f00, 0x0000)
  #bfn_tx_eq_set(bfn_working_tiles, 0xf600, 0x1400, 0x0000)
  #bfn_tx_eq_set_2(bfn_working_tiles, 0x0200, 0xf600, 0x1400, 0x0000, 0x0000)
  bfn_tx_eq_set_2(bfn_working_tiles, TX_EQ_pre2, TX_EQ_pre1, TX_EQ_main, TX_EQ_post1, TX_EQ_post2)
  bfn_tx_eq_settings_dump()

  banner("Wait for Rx Signal-Detect and PHY-Ready")
  bfn_wait_all_sig_det_and_phy_rdy(bfn_working_tiles, range(8), range(8), tries=16, dwell=1.0, quiet=False)
  dump_all_sig_detect(bfn_working_tiles, range(9), range(8))

  banner("Wait Rx EQ adaptation Complete")
  wait_all_adapt_done(bfn_working_tiles, range(8), range(8), tries=16, dwell=1.0, quiet=False)
  dump_all_adapt_done(bfn_working_tiles, range(9), range(8))

  while True:
    print_date_time()
    dump_all_prbs_errors(test_tiles, test_grps, cur_dwell_time)

    print("=========================================")
    inp = raw_input("Enter key to continue> ")
    bfn_command_menu(inp)

#######################################################################################################################
# bfn_get_ln_adapt_cnts
#
# 
#
def bfn_get_ln_adapt_cnts(tile, g, ln):
  set_tile(tile)
  set_grp(g)
  adapt_cnt = fw_adapt_cnt(group=g, lane=ln)
  readapt_cnt = fw_readapt_cnt(ln)
  linklost_cnt = fw_link_lost_cnt(ln)
  return adapt_cnt, readapt_cnt, linklost_cnt

#######################################################################################################################
# TEST_all_lanes_prbs_stability
#
# 
#
def TEST_all_lanes_prbs_stability(pause_even_grps=False):
  if default_mode.upper() == 'PAM4':
    banner("Configure PAM4 PRBS31")
    bfn_config_all_lanes_pam4(bfn_working_tiles, prbs_mode='PRBS31')
  else:
    banner("Configure NRZ PRBS31")
    bfn_config_all_lanes_nrz(bfn_working_tiles, prbs_mode='PRBS31')

  banner("Apply PN swaps")
  bfn_pn_swap_set(bfn_working_tiles)

  banner("TX EQ Settings")
  #bfn_tx_eq_set(bfn_working_tiles, 0xfe00, 0x1300, 0x0000)
  #bfn_tx_eq_set(bfn_working_tiles, 0xfe00, 0x1900, 0x0000)
  #bfn_tx_eq_set(bfn_working_tiles, 0xf800, 0x1100, 0x0000)
  #bfn_tx_eq_set(bfn_working_tiles, 0xf800, 0x0f00, 0x0000)
  #bfn_tx_eq_set(bfn_working_tiles, 0xfa00, 0x0f00, 0x0000)
  #bfn_tx_eq_set(bfn_working_tiles, 0xf600, 0x1400, 0x0000)
  #bfn_tx_eq_set_2(bfn_working_tiles, 0x0200, 0xf600, 0x1400, 0x0000, 0x0000)
  bfn_tx_eq_set_2(bfn_working_tiles, TX_EQ_pre2, TX_EQ_pre1, TX_EQ_main, TX_EQ_post1, TX_EQ_post2)
  bfn_tx_eq_settings_dump()

  banner("Wait for Rx Signal-Detect and PHY-Ready")
  bfn_wait_all_sig_det_and_phy_rdy(bfn_working_tiles, range(8), range(8), tries=16, dwell=1.0, quiet=False)
  dump_all_sig_detect(bfn_working_tiles, range(9), range(8))

  banner("Wait Rx EQ adaptation Complete")
  wait_all_adapt_done(bfn_working_tiles, range(8), range(8), tries=16, dwell=1.0, quiet=False)
  dump_all_adapt_done(bfn_working_tiles, range(9), range(8))

  if pause_even_grps:
    for t in test_tiles:
      print("Pause FW on: tile " + str(t) + " grps: " + str([0,2,4,6]))
      bfn_pause_all_grps_fw(t, [0,2,4,6], pause=True)

  for tile in test_tiles:
    for g in test_grps:
      for ln in test_lns:
        bfn_get_ln_adapt_cnts(tile, g, ln) # clears readapt cnt, should be 2, 2, 1

  while True:
    print_date_time()
    dump_all_prbs_errors(test_tiles, test_grps, cur_dwell_time)
    time.sleep(60.0)

    for tile in test_tiles:
      for g in test_grps:
        for ln in test_lns:
          adapt_cnt, readapt_cnt, linklost_cnt = bfn_get_ln_adapt_cnts(tile, g, ln) # clears readapt cnt, should be 2, 2, 1
          if linklost_cnt or readapt_cnt:
            print("<<<< LINK LOST : tile" + str(tile) + " grp" + str(g) + " ln" + str(ln) + " : lnklst=" + str(linklost_cnt) + " : readapt=" + str(readapt_cnt) + " >>>>")
            print_date_time()
            bfn_rx_monitor([tile], [g], [ln])
            bfn_fw_sd_params([tile], [g], [ln])

            bfn_rx_monitor(test_tiles, test_grps, test_lns)
            bfn_fw_sd_params(test_tiles, test_grps, test_lns)

            while True:
              print("=========================================")
              inp = raw_input("Enter key to continue> ")
              bfn_command_menu(inp)
          
    bfn_rx_monitor(test_tiles, test_grps, test_lns)
    bfn_fw_sd_params(test_tiles, test_grps, test_lns)
    
  print("=========================================")
  inp = raw_input("Enter key to continue> ")
  bfn_command_menu(inp)


#######################################################################################################################
# TEST_die_to_die_interface
#
# 
#
def TEST_die_to_die_interface(test_time = 60.0):
  banner("Configure PAM4 PRBS31")
  #bfn_config_all_lanes_pam4(bfn_working_tiles, prbs_mode='PRBS31')
  bfn_config_all_lanes_pam4(bfn_working_tiles, prbs_mode='functional')

  banner("Apply PN swaps")
  bfn_pn_swap_set(bfn_working_tiles)

  banner("Tx EQ")
  #bfn_tx_eq_dump(bfn_working_tiles, range(8), range(8))
  bfn_tx_eq_dump(test_tiles, test_grps, test_lns)

  banner("Wait for Rx Signal-Detect and PHY-Ready")
  #bfn_wait_all_sig_det_and_phy_rdy(bfn_working_tiles, range(8), range(8), tries=16, dwell=1.0, quiet=False)
  #dump_all_sig_detect(bfn_working_tiles, range(9), range(8))
  bfn_wait_all_sig_det_and_phy_rdy(test_tiles, test_grps, test_lns, tries=16, dwell=1.0, quiet=False)
  dump_all_sig_detect(test_tiles, test_grps, test_lns)

  banner("Wait Rx EQ adaptation Complete")
  #wait_all_adapt_done(bfn_working_tiles, range(8), range(8), tries=16, dwell=1.0, quiet=False)
  #dump_all_adapt_done(bfn_working_tiles, range(9), range(8))
  wait_all_adapt_done(test_tiles, test_grps, test_lns, tries=16, dwell=1.0, quiet=False)
  dump_all_adapt_done(test_tiles, test_grps, test_lns)

  banner("Configure Die to Die Loopback")
  bfn_die2die_loopback_set(test_tiles, test_time, 3, 3)
  exit()

#######################################################################################################################
# TEST_sensors
#
#
def TEST_sensors():
  """
  banner("Temp sensor read (Auto=1)")
  temp_sensor(auto=1)
  print("")
  """

  banner("Temp sensor read (Auto=0)")
  temp_sensor(auto=0)
  print("")
  exit()

  for tile in bfn_working_tiles:
    set_tile(tile)
    static_power_down_broadcast_mode(group=range(8), lanes =range(8))

  for check in range(5):
    print("Power-dn + " + str(check) + " seconds..")
    temp_sensor(auto=0)
    print("")
    temp_sensor(auto=1)
    print("")

  print("Power-Up erdes again..")
  for tile in bfn_working_tiles:
    set_tile(tile)
    for g in range(8):
      set_grp(g)
      # power back up
      static_power_down(group=g, lane=range(8), rx_off=0, tx_off=0, rx_bg_off=0 , tx_bg_off=0)

#######################################################################################################################
# TEST_mixed_mode
#
#
def TEST_mixed_mode():
  tile = test_tiles[0]
  g = test_grps[0]
  gEncodingMode[1] = [['nrz', 10.3125], ['nrz', 25.78125], ['pam4', 53.125], ['nrz', 25.78125], ['nrz', 10.3125], ['pam4', 53.125], ['nrz', 25.78125], ['nrz', 1.25]]

  bfn_config_ln_nrz(tile, g, 0, 'PRBS31', 10)
  bfn_config_ln_nrz(tile, g, 1, 'PRBS31', 25)
  bfn_config_ln_pam4(tile, g, 2, 'PRBS31')
  bfn_config_ln_nrz(tile, g, 3, 'PRBS31', 25)
  bfn_config_ln_nrz(tile, g, 4, 'PRBS31', 10)
  bfn_config_ln_pam4(tile, g, 5, 'PRBS31')
  bfn_config_ln_nrz(tile, g, 6, 'PRBS31', 25)
  bfn_config_ln_nrz(tile, g, 7, 'PRBS31', 1)

  banner("Re-Apply PN swaps (after lanes are configured)")
  bfn_pn_swap_set(bfn_working_tiles)

  bfn_tx_eq_set_2(test_tiles, 0x0200, 0xf600, 0x1400, 0x0000, 0x0000)

  wait_all_adapt_done(test_tiles, test_grps, test_lns, tries=16, dwell=1.0, quiet=False)
  dump_all_adapt_done(test_tiles, test_grps, test_lns)

  gLaneStats = [[0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0]]*8
  rx_monitor(lane=test_lns, rst=1, lc=0, ph=0, group=g, print_en=1, returnon=0)
  bfn_fw_sd_params(test_tiles, test_grps, test_lns)

  while True:
    print_date_time()
    dump_all_prbs_errors(test_tiles, test_grps, cur_dwell_time)

    print("=========================================")
    inp = raw_input("Enter key to continue> ")
    bfn_command_menu(inp)

  exit()

#######################################################################################################################
# TEST_pre_existing_conditions
#
#
def TEST_pre_existing_conditions():

  #banner("Re-Apply PN swaps (after lanes are configured)")
  #bfn_pn_swap_set(bfn_working_tiles)

  #wait_all_adapt_done(test_tiles, test_grps, test_lns, tries=16, dwell=1.0, quiet=False)
  #dump_all_adapt_done(test_tiles, test_grps, test_lns)

  while True:
    print_date_time()
    dump_all_prbs_errors(test_tiles, test_grps, cur_dwell_time)

    print("=========================================")
    inp = raw_input("Enter key to continue> ")
    bfn_command_menu(inp)




def twos_to_int(twos_val, bitWidth):
    return twos_val - int((twos_val << 1) & 2**bitWidth)

def int_to_twos(value, bitWidth):
    #return value - int((value << 1) & 2**bitWidth)
    return (2**bitWidth + value) & (2**bitWidth - 1)

def test_test():
  for v in range(-16,16):
    print("v = " + str(v) + " width=5 : int_to_twos= " +  str(int_to_twos(v,5)) )

def tile_chip_init():
  banner("Parse Board Map File")
  parse_board_map(print_en=1)

  banner("POST")
  bfn_post( default_tile_list )

  banner("Reset all Groups")
  bfn_group_reset_all(bfn_working_tiles)

  banner("Set Lane Maps")  
  bfn_lane_map_set(bfn_working_tiles)

  banner("Un-Pause all FW (in case leftover from prior test)")
  bfn_pause_all_fw(bfn_working_tiles, range(8), pause=False)

  banner("FW Download")
  bfn_fw_load_all_tiles(bfn_working_tiles, grp_0_7_fw_name, optional=True)

####################################################################################################
#Main Script
####################################################################################################

chip.connect(mdio=False,mode=3)

# init tile and group pointers
set_tile(0)
set_grp(0)

print(" 'i' = init chip, else skip chip init")
print("=========================================")
inp = raw_input("Enter key to continue> ")
if inp == "i":
  tile_chip_init()
else:
  bfn_working_tiles = default_tile_list

print("")
print("BFN working tiles : " + str(bfn_working_tiles))
print("Test tiles : " + str(test_tiles))
print("Test groups: " + str(test_grps))
print("Test lanes : " + str(test_lns))
print("")

inp = '-'
while inp != 'q':
  print("q = quit")
  print("f = force load FW")
  print("i = (re-) Init the device")
  print("")
  print("0.  BFN temp sensor read")
  print("1.  TEST_mixed_mode")
  print("2.  TEST_sensors")
  print("3.  TEST_ffe_analyzer")
  print("4.  TEST_xtalk(repeat_count=10, in_csv_fmt=True)")
  print("5.  TEST_Rx_EQ_repeatability(repeat_count=10, in_csv_fmt=True, pause_fw=True)")
  print("6.  TEST_two_group_tx_eq(2,6,2,7, in_csv_fmt=True)")
  print("7.  TEST_1_grp_functional")
  print("8.  TEST_Credo_Init")
  print("9.  TEST_die_to_die_interface(test_time = 60.0)")
  print("10. TEST_all_lanes_remote_loopback")
  print("11. TEST_all_lanes_prbs_internal_loopback")
  print("12. TEST_all_lanes_prbs_1g")
  print("13. TEST_all_lanes_prbs_stability")
  print("14. TEST_all_lanes_prbs")
  print("15. TEST_all_lanes_functional")
  print("16. TEST_pre_existing_conditions")
  print("17. TEST_all_lanes_prbs_both_enc_modes")
  print("=========================================")
  inp = raw_input("Enter test number : ")

  if inp == 'q': exit()
  if inp == 'f':
    banner("FW Download (force)")
    bfn_fw_load_all_tiles(bfn_working_tiles, grp_0_7_fw_name, optional=False)
    continue
  if inp == 'i':
    tile_chip_init()
    continue

  if inp == '0':
    bfn_temp_sensor_read()
  elif inp == '1':
    TEST_mixed_mode()
  elif inp == '2':
    TEST_sensors()
  elif inp == '3':
    TEST_ffe_analyzer()
  elif inp == '4':
    TEST_xtalk(repeat_count=10, in_csv_fmt=True)
  elif inp == '5':
    TEST_Rx_EQ_repeatability(repeat_count=10, in_csv_fmt=True, pause_fw=True)
  elif inp == '6':
    TEST_two_group_tx_eq(2,6,2,7, in_csv_fmt=True)
  elif inp == '7':
    TEST_1_grp_functional()
  elif inp == '8':
    TEST_Credo_Init()
  elif inp == '9':
    TEST_die_to_die_interface(test_time = 60.0)
  elif inp == '10':
    TEST_all_lanes_remote_loopback()
  elif inp == '11':
    TEST_all_lanes_prbs_internal_loopback()
  elif inp == '12':
    TEST_all_lanes_prbs_1g()
  elif inp == '13':
    TEST_all_lanes_prbs_stability(pause_even_grps=True)
  elif inp == '14':
    TEST_all_lanes_prbs()
  elif inp == '15':
    TEST_all_lanes_functional()
  elif inp == '16':
    TEST_pre_existing_conditions()
  elif inp == '17':
    TEST_all_lanes_prbs_both_enc_modes()

exit()


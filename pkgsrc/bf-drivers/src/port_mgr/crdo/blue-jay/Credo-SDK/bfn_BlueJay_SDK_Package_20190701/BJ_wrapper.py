import os
import sys
import time
import argparse
from collections import OrderedDict
import datetime
import pdb
import copy
# import functools
import math
import texttable

# Credo SDK related
from credoSdk import _SDK_path, Chip_name, _REG_path, _FW_path, _FW8_path
from Device.CredoUsbDongle import Mdio_Lib
from credoSock import Mdio_sock, Mdio_pcie

if sys.platform.startswith('win'):
    from JBay_Portmap import get_QSFP2JB, get_JB2QSFP_tx, get_JB2QSFP_rx
else:
    print "Loading pickle file..."
    import pickle
    with open('NP_portmap.pkl', 'rb') as fp:
        obj = pickle.load(fp)
        def get_QSFP2JB( port, lane):
            return obj['QSFP2JB'][(port, lane)]
        def get_JB2QSFP_tx( tile, octal, lane):
            return obj['JBTX2QSFP'][ (tile, octal, lane) ]
        def get_JB2QSFP_rx( tile, octal, lane):
            return obj['JBRX2QSFP'][ (tile, octal, lane) ]

# Credo
import Device.CredoUsbDongle
import RegisterControl.regmap
import BaseFunction.Project.BlueJay_L2_pam4
import Algorithm.Project.BlueJay_L3_pam4
import BaseFunction.Project.BlueJay_L2_nrz
import Algorithm.Project.BlueJay_L3_nrz


def deco_AllLanes(func):
    #place 'lane' parameter at 1st position

    def do_all(*args, **kw):
        rtn_all = list()

        _self = args[0]
        _args = list(args)
        if len(args)==1:
            # all parameters are passed as keyword argument
            lane = kw['lane']
        else:
            # lane parameter is passed as position argument
            lane = args[1]

        if 'tile' in kw.keys():
            tile = kw['tile']
            _self.set_tile(tile)

        if lane in ['all_group', 'all_groups', 'tile']:
            grp = BlueJay.Groups

        elif lane in ['all_lane', 'all_lanes', 'all']:
            if 'group' in kw.keys():
                grp = kw['group'] if type(kw['group'])==list else [kw['group']]
            else:
                grp = [_self._get_cur_group()]

        elif type(lane)==tuple: # Newport (port, lane)
            pass
        else:
            # lane is int type
            if 'group' in kw.keys():
                _self.chip.setPhyAddr(kw['group'])
            return func(*args, **kw)

        _kw = copy.copy(kw)
        for g in grp:
            _self.chip.setPhyAddr(g)
            for l in range(BlueJay.Lanes[g]):
                if len(_args)>=2:
                    _args[1] = l
                else:
                    _kw['lane']=l
                rtn = func(*_args, **_kw)
                rtn = 0 if rtn==None else rtn
                rtn_all.append(rtn)
        return rtn_all

    return do_all



# BlueJay
class BlueJay():

    Groups  = range(8)
    Lanes   = {0:8, 1:8, 2:8, 3:8,
               4:8, 5:8, 6:8, 7:8,
               8:4}


    # # group: polarity setting
    # TxPolarityMap = {
                # 0:[1]*8, 1:[0]*8, 2:[0]*8, 3:[0]*8,
                # 4:[0]*8, 5:[0]*8, 6:[0]*8, 7:[0]*8,
                # 8:[1]*4}

    # # group: polarity setting
    # RxPolarityMap = {
                # 0:[0]*8, 1:[0]*8, 2:[0]*8, 3:[0]*8,
                # 4:[0]*8, 5:[0]*8, 6:[0]*8, 7:[0]*8,
                # 8:[0]*4}
    @staticmethod
    def message( msg, level=0 ):
        """
        level: 0=Info, 1=Warning, 2=error
        """
        print "BJ {:8s}: {}".format( {0:'Info', 1:'Warning', 2:'Error'}[level], msg)

    def __init__(self, mode='nrz', datarate='10', conn={'mode':'socket','host':'10.11.20.50','port':8000}, tile=0):
        """
        conn: {'mode':'socket','host':'10.11.20.50','port':8000}
              {'mode': 'usb'}
        """
        self.mode = mode.lower() # pam4, nrz
        self.lane_mode = [[self.mode,]*8]*8
        if conn['mode'] == 'socket':
            self.chip = Mdio_sock(mode='socket', host=conn['host'], port=conn['port'], tile=tile)
            self.host = conn['host']
            self.port = conn['port']
            # self.tile = tile
        elif conn['mode'] == 'usb':
            self.chip = Mdio_sock(mode='usb')
            # self.tile = 0
        elif conn['mode'] == 'pcie':
            self.chip = Mdio_pcie(mode='pcie')
            # self.tile = 0

        self.datarate = datarate

        RegisterControl.regmap.load_Regs(Chip_name, self.chip, textFilePath = _REG_path )

        BaseFunction.Project.BlueJay_L2_pam4.load_basefunction_pam4(self.chip)
        BaseFunction.Project.BlueJay_L2_nrz.load_basefunction_nrz(self.chip)
        Algorithm.Project.BlueJay_L3_pam4.load_algorithm_pam4(self.chip)
        Algorithm.Project.BlueJay_L3_nrz.load_algorithm_nrz(self.chip)

        try:
            self.connect()
            time.sleep(0.1)
            if 'resetMdio' in self.__dict__.keys():
                self.chip.resetMdio()
        except:
            print "Error occurred during init"
            if self.chip.connected:
                self.disconnect()

        self.laneStats = dict()
        for tile in range(4):
            self.laneStats[tile] = dict()
            for group in BlueJay.Groups:
                self.laneStats[tile][group] = [[0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0]]*BlueJay.Lanes[group]
        # self.get_lane_mode(0, group=0)

    def connect(self):
        # self.chip.connect(mdio=False, mode=3)
        self.chip.connect()

    def disconnect(self):
        self.chip.disconnect()
        print "disconnected"

    def _get_cur_group(self):
        # return self.chip.usb.cr_phy_addr
        return self.chip.group

    def set_tile(self, tile):
        self.chip.setTileAddr(tile)

    def set_group(self, grp):
        self.chip.setPhyAddr(grp)

    def load_init(self, group, filename):
        """Legacy function"""
        regfile = open(filename, 'r')
        lines = regfile.readlines()
        lines = map( lambda x: x.strip(), lines)
        regfile.close()
        self.set_group(group)
        print "Loading {} into to group {}".format( filename, group )
        for l in lines:
            # if l.strip().startswith('#') or len(l)==0:
            if l.find('#')>=0:
                _l = l[:l.find('#')].strip()
            else:
                _l = l.strip()

            if len(_l) == 0:
                continue

            addr, data = _l.split()
            addr = int(addr, 16)
            data = int(data, 16)
            if group == 8:
                if addr > 0x2000:
                    continue

            self.mdioWr(addr, data)
            # print "{:04x}, {:04x}".format(addr, data)

    def set_top_div4(self, div4=True):
        """Legacy function"""
        self.set_group(9)
        if div4:
            print "Enable div4"
            self.chip.MdioWr(0x4d02, 0xB6E0)
        else:
            print "Disable div4"
            self.chip.MdioWr(0x4d02, 0xA6E0)


    def broadcast_mode(self, groups=range(8), en=1):
        for group in groups:
            self.chip.setPhyAddr(group)
            if en == 1:
                self.chip.MdioWr(0x4014, 0x8888)
            else:
                self.chip.MdioWr(0x4014, 0x0000)

    def fw_load_broadcast_mode(self, groups=None, fw_file_name=None, broadcast_en=0):
        self.broadcast_mode(groups=groups, en=broadcast_en)
        if broadcast_en == 1:
            self.chip.GROUP8_TOP[0].fw_load(self.chip, fw_file_name=fw_file_name, broadcast_mode=broadcast_en)
        else:
            for grp in groups:
                self.chip.setPhyAddr(grp)
                self.chip.GROUP8_TOP[0].fw_load(self.chip, group=grp, fw_file_name=fw_file_name, broadcast_mode=broadcast_en)
        self.broadcast_mode(groups=groups, en=0)

    def config_blueJay(self, groups):
        if groups == 'all':
            groups = range(9)
        elif type(groups) != list:
            groups = [groups]
        groups.sort()

        print "Config BlueJay..."

        # # reset
        # for grp in groups:
            # print "Resetting group {}...".format(grp)
            # self.chip.setPhyAddr(grp)
            # self.reset_group()
            # self.reset_lane(lane='all')

        # Load FW in broadcasting mode
        _last_grp = len(groups)
        if groups[-1]==8:
            fw_name = _FW8_path
            _fw = os.path.split(fw_name)[1]
            print
            print "Loading {} to group 8...".format(_fw)
            self.fw_load_broadcast_mode(groups=[8], fw_file_name=fw_name, broadcast_en=0)
            _last_grp -= 1

        if groups[0]!=8:
            fw_name = _FW_path
            _fw = os.path.split(fw_name)[1]
            print
            print "Loading {} to group {}-{}...".format(_fw, groups[0], groups[_last_grp-1])
            self.fw_load_broadcast_mode(groups=groups[:_last_grp], fw_file_name=fw_name, broadcast_en=1)

        time.sleep(0.5)
        self.chip.GROUP8_TOP[0].fw_loaded(self.chip)


        # FW cmd
        for grp in groups:
            print "Initializing group {}".format(grp)
            self.chip.setPhyAddr(grp)
            self._init_lane_for_fw(lane='all')

        for grp in groups:
            self.chip.setPhyAddr(grp)
            self.message("Waiting for adapt done ({},{})".format(self.chip.tile, self.chip.group))

            cnt = 0 # no need to wait for each lane's timeout
            for lane in range(BlueJay.Lanes[grp]):
                while True:
                    adapt_done = (self.chip.MdioRd(0x40C7) >> lane) & 1
                    if adapt_done == 1:
                        self.message('done ({})'.format(lane))
                        break
                    elif cnt > 100:
                        self.message('timeout ({})'.format(lane))
                        break
                    time.sleep(0.1)
                    cnt += 1


        # self.get_lane_mode(0)
    #
    # Newport specific
    #
    # def bfn_lane_map_set(self):
        # """bfn_set_lane_map
        # Configure the (swizzled) lane map registers
        # """
        # for g in range(0,8):
            # self.chip.setPhyAddr(g)
            # r80000,r80001,r80002,r80003 = lane_map_gset(self.tile, g)
            # self.chip.MdioWr(0x4000, r80000)
            # self.chip.MdioWr(0x4001, r80001)
            # self.chip.MdioWr(0x4002, r80002)
            # self.chip.MdioWr(0x4003, r80003)

    # def bfn_configure_PN_swaps(self):
        # """bfn_configure_PN_swaps
        # Configure the Tx and Rx polarity swaps required by the board map
        # """
        # for g in range(0,8):
            # self.chip.setPhyAddr(g)
            # for ln in range(0,8):
                # tx_pn, rx_pn = lane_PN_swap_get(self.tile, g, ln)
                # tx_pn = 1 - tx_pn # Note: 1=NO INVERT
                # self.chip.NRZ25[ln].tx_pol_nrz(val=tx_pn)
                # self.chip.NRZ25[ln].rx_pol_nrz(val=rx_pn)

    #
    # Util functions
    #
    def save_setup(self, group=None, tile=None, filedir=None):

        if tile==None:
            tile = self.chip.tile
        else:
            self.chip.setTileAddr(tile)

        if group==None:
            group = self._get_cur_group()
        else:
            self.chip.setPhyAddr(group)

        print "Dumping Tile={}, Group {}...".format(tile, group)

        lanes = BlueJay.Lanes[group]
        timefmt = datetime.datetime.now().strftime("%Y%m%d_%H%M%S")
        filename = "BlueJay_tile{}_grp{}_{}.txt".format(tile, group, timefmt)

        if filedir:
            filename = os.path.join(filedir, filename)

        log = open(filename, 'w')
        log.write('\n#---------------------------------------------'),
        log.write('\n# File: %s' % filename),
        log.write("\n# %s Registers" % (Chip_name)),
        log.write("\n# %s" % time.asctime())
        log.write('\n#---------------------------------------------\n'),

        ##### Save FW SERDES PARAMS
        for lane in range(lanes):
            params = self.get_serdes_params(lane, group)
            for k,v in params.iteritems():
                log.write("\n---- Lane {} -----\n".format(lane))
                log.write("{} : {}\n".format(k,v))
        else:
            log.close()

        self.reg_group_dump(0x4800, range(0x00, 0x59, 1), 'TOP Registers', filename)
        self.reg_group_dump(0x4D00, range(0x00, 0x16, 1), 'TOP PLL Registers', filename)
        for lane in range(lanes):
            self.reg_group_dump(0x0000 + 0x800 * lane, range(0x000, 0x1FF + 1, 1), 'Per Lane Register', filename)
            self.reg_group_dump(0x0000 + 0x800 * lane, range(0x0C0, 0x0FF + 1, 1), 'ANA_Reg', filename)
            self.reg_group_dump(0x0500 + 0x800 * lane, range(0x000, 0x00B + 1, 1), 'Aneg_lt Registers', filename)
            self.reg_group_dump(0x01C0 + 0x800 * lane, range(0x000, 0x00D + 1, 1), 'FEC Analyzer Registers', filename)
            self.reg_group_dump(0x0200 + 0x800 * lane, range(0x000, 0x00C + 1, 1), 'LANE_SLICE', filename)
            self.reg_group_dump(0x0400 + 0x800 * lane, range(0x000, 0x06C + 1, 1), 'Training Registers', filename)
            self.reg_group_dump(0x0300 + 0x800 * lane, range(0x000, 0x0FF + 1, 1), 'Aneg_ieee Registers', filename)
            self.reg_group_dump(0x4B00, range(0x03F, 0x0FF, 1), 'TSensor, VSensor Registers', filename)

        ##### Save FW Register values
        log_file = open(filename, 'a+')
        temp_stdout = sys.stdout
        sys.stdout = log_file
        self.chip.GROUP8_TOP[0].fw_reg(self.chip)
        sys.stdout = temp_stdout
        log_file.close()

        print ("...Saved Registers to %s" % (filename))

        ##############################################################################
        # Similar to the function in main script
        # changed the order of addresses saved to be sequential from 0x7000 to 0x8FFF
        ##############################################################################
    def reg_group_dump(self, base_addr, addr_range, addr_name, filename):
        log = open(filename, "a+")
        log.write('\n\n#---------------------------------------------')
        log.write('\n#%s (R%04X to R%04X)' % (addr_name, base_addr + addr_range[0], base_addr + addr_range[-1]))
        log.write('\n#Addr Value')
        log.write('\n#---------------------------------------------\n'),

        for i in addr_range:
            for j in range(1):
                addr = base_addr + i
                val = self.chip.MdioRd(addr)
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


    def check_mdio(self, group, tile):
        """Check if MDIO communication is healthy
        return True if failed
        """

        failed = False
        self.chip.setTileAddr(tile)
        self.chip.setPhyAddr(group)
        # Use 0xA1 as a test register. It normally contains 0xAAAA
        data = self.chip.MdioRd(0xA1)
        if (data != 0xAAAA):
            print("0xA1 : " + str(hex(data)) + " : re-setting to 0xAAAA")
            self.chip.MdioWr(0xA1, 0xAAAA)
            data = self.chip.MdioRd(0xA1)
            if (data != 0xAAAA):
                print("0xA1 : " + str(hex(data)) + " : Still not 0xAAAA")
                failed = True
                #return -1
        for attempt in range(1,10):
            self.chip.MdioWr(0xA1, attempt)
            data = self.chip.MdioRd(0xA1)
            if (data != attempt):
                print("0xA1 : " + str(hex(data)) + " : attempt : " + str(attempt) + " : read-back failed : " + str(hex(data)))
        if failed:
          print("Tile " + str(tile) + " grp" + str(group) + ": ** FAILED **")
        else:
          print("Tile " + str(tile) + " grp" + str(group) + ":    PASSED")
        # reset to default value
        self.chip.MdioWr(0xA1, 0xAAAA)

        return failed

    #
    # Reset
    #
    def soft_reset(self, group=None, tile=None):
        self.chip.GROUP8_TOP[0].Reg0013_11_0 = 0x888
        time.sleep(0.01)
        self.chip.GROUP8_TOP[0].Reg0013_11_0 = 0x0

    def logic_reset(self, group=None, tile=None):
        self.chip.GROUP8_TOP[0].Reg0013_11_0 = 0x777
        time.sleep(0.01)
        self.chip.GROUP8_TOP[0].Reg0013_11_0 = 0x0

    def reset_group(self, t=0.1, group=None, tile=None):

        if group!=None:
            self.set_group(group)
        if tile!=None:
            self.set_tile(tile)

        self.soft_reset(group=group, tile=tile)
        time.sleep(t)

        self.logic_reset(group=group, tile=tile)
        time.sleep(t)

    @deco_AllLanes
    def reset_lane(self, lane, **kw):
        # print"lane = ", lane
        self.chip.ANLT_TOP[lane].Tr_Reg0000_1 = 0
        time.sleep(0.01)
        self.chip.ANLT_TOP[lane].Tr_Reg0000_0 = 0


    #
    # Device Initialization
    #
    @deco_AllLanes
    def set_lane_mode(self, lane, **kw):
        """set lane mode to either nrz or pam4 mode"""
        if self.mode=='nrz':
            self.chip.PAM50[lane].PAM4_EN = 0
            self.chip.PAM50[lane].TX_PRBS_CLK_EN = 0
            self.chip.NRZ25[lane].TX_NRZ_MODE = 1
            self.chip.NRZ25[lane].TX_NRZ_PRBS_GEN_EN = 1
        elif self.mode=='pam4':
            self.chip.NRZ25[lane].TX_NRZ_MODE = 0
            self.chip.NRZ25[lane].TX_NRZ_PRBS_GEN_EN = 0
            self.chip.PAM50[lane].PAM4_EN = 1
            self.chip.PAM50[lane].TX_PRBS_CLK_EN = 1

    @deco_AllLanes
    def _init_lane_for_fw(self, lane, **kw):
        """ This initialize lanes for BJ SMK board"""

        self.set_lane_mode(lane=lane, **kw)
        ####################### put lane in PAM4 mode
        if (self.mode == 'pam4'):
            self.chip.PAM50[lane].TX_POST2_SCALE = 0
            self.chip.PAM50[lane].TX_POST1_SCALE = 0
            self.chip.PAM50[lane].TX_MAIN_SCALE = 1
            self.chip.PAM50[lane].TX_PRE1_SCALE = 0
            self.chip.PAM50[lane].TX_PRE2_SCALE = 0

            self.chip.PAM50[lane].tx_taps(2, -8, 17, 0, 0)

            self.chip.PAM50[lane].gc(1, 1)
            self.chip.PAM50[lane].pc(0, 0)
            self.chip.PAM50[lane].msblsb(0, 0)

            # self.set_prbs(lane=lane, tx_prbs_mode='functional', rx_prbs_mode='functional')
            # time.sleep(0.1)
            self.set_prbs(lane=lane, tx_prbs_mode='prbs', rx_prbs_mode='prbs', tx_pat='QPRBS13', rx_pat='QPRBS13')

        ####################### put lane in NRZ mode
        if (self.mode == 'nrz'):
            self.chip.PAM50[lane].TX_POST2_SCALE = 0
            self.chip.PAM50[lane].TX_POST1_SCALE = 0
            self.chip.PAM50[lane].TX_MAIN_SCALE = 1
            self.chip.PAM50[lane].TX_PRE1_SCALE = 0
            self.chip.PAM50[lane].TX_PRE2_SCALE = 0

            self.chip.NRZ25[lane].tx_taps(0, -8, 17, 0, 0)

            self.chip.PAM50[lane].gc(0, 0)
            self.chip.PAM50[lane].pc(0, 0)
            self.chip.PAM50[lane].msblsb(0, 0)

            self.set_prbs(lane=lane, tx_prbs_mode='prbs', rx_prbs_mode='prbs', tx_pat='PRBS31', rx_pat='PRBS31')

        self.chip.PAM50[lane].Reg007B_3 = 1
        self.chip.NRZ25[lane].Reg0100_007C_15 = 0

        if self.mode == 'nrz':
            self.chip.NRZ25[lane].RX_AC_COUPLE_EN = 1
            # self.chip.NRZ25[lane].tx_pol_nrz(val=BlueJay.TxPolarityMap[grp][lane])
            # self.chip.NRZ25[lane].rx_pol_nrz(val=BlueJay.RxPolarityMap[grp][lane])
        elif self.mode == 'pam4':
            self.chip.PAM50[lane].RX_AC_COUPLE_EN = 1
            # self.chip.PAM50[lane].tx_pol_pam4(val=BlueJay.TxPolarityMap[grp][lane])
            # self.chip.PAM50[lane].rx_pol_pam4(val=BlueJay.RxPolarityMap[grp][lane])

        # Start Initialization
        if self.mode == 'nrz':
            if int(self.datarate) == 10:
                self.chip.GROUP8_TOP[0].fw_config_cmd(self.chip, config_cmd=0x80C0+lane, config_detail=0x0001)
            else:
                self.chip.GROUP8_TOP[0].fw_config_cmd(self.chip, config_cmd=0x80C0+lane, config_detail=0x0000)
        elif self.mode == 'pam4':
            self.chip.GROUP8_TOP[0].fw_config_cmd(self.chip, config_cmd=0x80D0 + lane, config_detail=0x0000)




    #
    # Firmware commnads
    #
    def fw_runCmd(self, lane, cmd, mode=0, detail=0, wait=1.0, **kw):
        """ execute FW command
        * Command (FW_CMD[15:12])
        0x8 : fw_config_mode-> config target lane to target mode/speed
        0x9 : fw_destroy_mode-> config target lane to quiet mode
        0xB : fw_debug_info_mode-> retrieve debug information from firmware
        0xE : fw_reg_mode-> read/write FW internal registers
        * Status (Read only) (FW_CMD[11:8])
        0x3 : Command failed
        0xn : Command successful if n equal to the command issued
        * Mode (FW_CMD[7:4])
        Defines specific mode for the command. Each FW command has its specific list of modes. i.e.
            NRZ or PAM4
        * Lane Number (FW_CMD[3:0]
        Defines target lane to apply the FW command to (lane range: 0 to 7)
        * Command detail (FW_CMD_DETAIL[15:0])
        Define additional detail for an specific mode, i.e. lane speed
        """
        # register address for FW command
        FW_CMD       = 0x40C1
        FW_CMD_DETAIL = 0x40C2

        self.chip.MdioWr(FW_CMD_DETAIL, detail)
        fw_cmd = (cmd << 12) | (0<<8) | (mode << 4) | (lane)
        self.chip.MdioWr(FW_CMD, fw_cmd)
        start_t = time.time()
        while True:
            time.sleep(0.01)
            status = (self.chip.MdioRd(FW_CMD) >> 8) & 0xF
            if status == cmd:
                # command successful
                return 0
            elif status == 0x3:
                print "FW command filed"
                return -1
            elif status == 0:
                if (time.time() - start_t > wait):
                    print "FW command timeout"
                    return -2
            else:
                print "FW in unexpected status"
                return -3

    def fw_modeConfig(self, lane, mode, speed, **kw):
        """Config mode
        mode: 'nrz', 'pam4'
        speed:  0x1: 10.3125G
                0x2: 20.625G
                0x3: 25.78125G
                0x4: 26.5625G
                0x8: 51.5626G
                0x9: 53.125G
        """
        _mode = {'nrz': 0xc, 'pam4': 0xd}[mode]
        return self.fw_runCmd(lane, cmd=0x8, mode=_mode, detail=speed)

    def fw_destroy(self, lane, **kw):
        """Unload FW. Do this before reload FW"""
        return self.fw_runCmd(lane, cmd=0x9, mode=0, detail=0)


    def fw_regRd(self, lane, addr):
        """Read from FW register"""
        FW_REG_VALUE = 0x40C4
        r = self.fw_runCmd(lane, cmd=0xE, mode=0x1, detail=addr)
        if r < 0:
            return r
        else:
            return self.chip.MdioRd(FW_REG_VALUE)

    def fw_regWr(self, lane, addr, value):
        """Write to FW register"""
        FW_REG_VALUE = 0x40C4
        self.chip.MdioWr(FW_REG_VALUE, value)
        r = self.fw_runCmd(lane, cmd=0xE, mode=0x2, detail=addr)
        if r < 0:
            return r

    def fw_isLoaded(self):
        if self.fw_runCmd(lane=0, cmd=0xF, mode=0, detail=0, wait=2.0) == 0:
            high_word = self.chip.MdioRd(0x40C1)&0xFF # upper byte, only 8 bits are valid
            low_word  = self.chip.MdioRd(0x40C2) # lower word
            hash_code = (high_word <<16) + low_word
            return hash_code
        else:
            return -1


    #
    # Utils
    #
    def print_params(self, params):
        """
        params: dict or list of same dicts
        """
        pass
        if type(params)==[dict, OrderedDict]:
            dict_lst = [params]
        elif type(params)==list:
            dict_lst = params

        keys=dict_lst[0].keys()
        lines = [keys]

        for i, param in enumerate(dict_lst):
            line = list()
            for key in keys:
                line.append(param[key])
            lines.append(line)

        t = texttable.Texttable()
        t.set_cols_align( ['c']*len(keys))
        # t.set_cols_width( map(lambda x: max(len(x),5), keys) )
        t.add_rows( lines, header=True )
        print t.draw()


    #
    # Tx / Rx basic functions
    #
    @deco_AllLanes
    def set_tx_pol(self, lane, inv=None, **kw):
        """
        """
        if inv==None:
            if self.mode=='nrz':
                inv = self.chip.NRZ25[lane].tx_pol_nrz()
            elif self.mode == 'pam4':
                inv = self.chip.PAM50[lane].tx_pol_pam4()
            return 1-inv
        else:
            if self.mode=='nrz':
                self.chip.NRZ25[lane].tx_pol_nrz(val=1-inv)
            elif self.mode == 'pam4':
                self.chip.PAM50[lane].tx_pol_pam4(val=1-inv)

    @deco_AllLanes
    def set_rx_pol(self, lane, inv=None, **kw):
        """
        """
        if inv==None:
            if self.mode=='nrz':
                inv = self.chip.NRZ25[lane].rx_pol_nrz()
            elif self.mode == 'pam4':
                inv = self.chip.PAM50[lane].rx_pol_pam4()
            return inv
        else:
            if self.mode=='nrz':
                self.chip.NRZ25[lane].rx_pol_nrz(val=inv)
            elif self.mode == 'pam4':
                self.chip.PAM50[lane].rx_pol_pam4(val=inv)

    @deco_AllLanes
    def set_tx_eq(self, lane, pre2=None, pre1=None, main=None, post1=None, post2=None, **kw):
        """update tx eq settings
            For our pre and post cursor, the sum total of all cursors is 31. This 31 sum corresponds to 500mV.
            For the pre and post cursors, we turn on a divide_by_2 bit that makes each increment a half step.
            For the main cursor we use full steps.
            As a result for pre and post cursor increments: 500/31/2 = ~8mV per increment
            For main cursor: 500/31 = ~16mV
        """

        self.message('setting TX EQ')
        tx_eq = [pre2, pre1, main, post1, post2]
        if self.mode=='nrz':
            tx_taps = self.chip.NRZ25[lane].tx_taps()
            for i, txp in enumerate(tx_eq):
                if txp!=None: tx_taps[i] = txp
            self.chip.NRZ25[lane].tx_taps( *tx_taps )
        elif self.mode=='pam4':
            tx_taps = self.chip.PAM50[lane].tx_taps()
            for i, txp in enumerate(tx_eq):
                if txp!=None: tx_taps[i] = txp
            self.chip.PAM50[lane].tx_taps( *tx_taps )

        return tx_taps

    @deco_AllLanes
    def set_rx_ctle(self, lane, mode, **kw):
        pass

    @deco_AllLanes
    def set_rx_dfe(self, lane, **kw):
        pass

    @deco_AllLanes
    def set_rx_acgain(self, lane, acgain1, acgain2, **kw):
        pass

    @deco_AllLanes
    def set_rx_ffegain(self, lane, ffegain1, ffegain2, **kw):
        pass

        # ffegain


    # skef?
    # tx_pu - Reg00EB_13


    #
    # PLL
    #
    @deco_AllLanes
    def get_pll_param(self, lane):
        pass

    #
    # PRBS gen/chk
    #
    @deco_AllLanes
    def set_prbs(self, lane, tx_prbs_mode='prbs', rx_prbs_mode='prbs',
                                                    tx_pat=None, rx_pat=None, **kw):
        """ Set data pattern
        Pattern can be PRBS9/15/23/31, QPRBS13/31
        """

        _prbs_str = 'prbs'
        _functional_str = 'functional'
        _custom_str = 'custom' # only for tx
        _qprbs13_str = 'QPRBS13'
        _qprbs31_str = 'QPRBS31'

        if self.mode == 'nrz':
            nrz_prbs_pat  = ['PRBS9', 'PRBS15', 'PRBS23', 'PRBS31']
            if tx_prbs_mode == _prbs_str:
                self.chip.NRZ25[lane].tx_prbs_en_nrz(en=1)
                if type(tx_pat)==str:
                    tx_pat_idx = nrz_prbs_pat.index(tx_pat)
                else:
                    tx_pat_idx = tx_pat
                self.chip.NRZ25[lane].tx_prbs_mode_nrz(pat=tx_pat_idx)

            elif tx_prbs_mode == _custom_str:
                self.chip.NRZ25[lane].tx_prbs_en_nrz(en=0)
                self.chip.NRZ25[lane].tx_prbs_mode_nrz(pat=None)
                self.chip.NRZ25[lane].tx_test_patt_nrz(en=1, mode=0,
                        tx_test_patt4_val=tx_pat, # 16-bits
                        tx_test_patt3_val=tx_pat,
                        tx_test_patt2_val=tx_pat,
                        tx_test_patt1_val=tx_pat)

            elif tx_prbs_mode == _functional_str:
                self.chip.NRZ25[lane].tx_prbs_en_nrz(en=0)
                self.chip.NRZ25[lane].tx_prbs_mode_nrz(pat=None)

            if rx_prbs_mode == _prbs_str:
                if type(rx_pat)==str:
                    rx_pat_idx = nrz_prbs_pat.index(rx_pat)
                else:
                    rx_pat_idx = rx_pat
                self.chip.NRZ25[lane].rx_prbs_mode_nrz(pat=rx_pat_idx)
            elif rx_prbs_mode == _functional_str:
                self.chip.NRZ25[lane].rx_prbs_mode_nrz(pat=None)

        elif self.mode=='pam4':
            pam4_prbs_pat = {'PRBS9':(0,0), 'PRBS13':(1,0), 'PRBS15':(2,0), 'PRBS31':(3,0),
                             'QPRBS9':(0,1), 'QPRBS13':(1,1), 'QPRBS15':(2,1), 'QPRBS31':(3,1),}
            if tx_prbs_mode == _prbs_str:
                self.chip.PAM50[lane].tx_prbs_en_pam4(en=1)
                if type(tx_pat)==str:
                    tx_pat_idx = pam4_prbs_pat[tx_pat][0]
                    tx_gc = pam4_prbs_pat[tx_pat][1]
                else:
                    tx_pat_idx = tx_pat
                    tx_gc = 1 #always QPRBS :-)
                self.chip.PAM50[lane].tx_prbs_mode_pam4(pat=tx_pat_idx)
                self.chip.PAM50[lane].TX_GRAYCODE_EN = tx_gc

            elif tx_prbs_mode == _custom_str:
                self.chip.PAM50[lane].tx_test_patt(en=1, mode=0,
                        tx_test_patt4_val=tx_pat, # 16-bits
                        tx_test_patt3_val=tx_pat,
                        tx_test_patt2_val=tx_pat,
                        tx_test_patt1_val=tx_pat)

            elif tx_prbs_mode == _functional_str:
                self.chip.PAM50[lane].tx_prbs_en_pam4(en=0)
                self.chip.PAM50[lane].tx_prbs_mode_pam4(pat=None)

            if rx_prbs_mode == _prbs_str:
                if type(rx_pat)==str:
                    rx_pat_idx = pam4_prbs_pat[rx_pat][0]
                    rx_gc = pam4_prbs_pat[rx_pat][1]
                else:
                    rx_pat_idx = rx_pat
                    rx_gc = 1
                self.chip.PAM50[lane].rx_prbs_mode_pam4(pat=rx_pat_idx)
                self.chip.PAM50[lane].RX_PRECODE_EN = rx_gc

            elif rx_prbs_mode == 'functional':
                self.chip.PAM50[lane].rx_prbs_mode_pam4(pat=None)


    @deco_AllLanes
    def enable_prbs_err_cntr(self, lane, en=1, **kw):
        """just is just to assert or deassert counter reset"""

        if self.mode=='pam4':
            self.chip.PAM50[lane].PRBS_SYNC_CNTR_RESET = 1 if en==0 else 0
        elif self.mode=='nrz':
            self.chip.NRZ25[lane].RX_PRBS_COUNT_RESET = 1 if en==0 else 0

    @deco_AllLanes
    def get_prbs_err_cntr(self, lane, **kw):

        if self.mode=='pam4':
            error_cnt = self.chip.PAM50[lane].get_err_pam4()
        elif self.mode=='nrz':
            error_cnt = self.chip.NRZ25[lane].get_err_nrz()

        return error_cnt

    @deco_AllLanes
    def reset_prbs_err_cntr(self, lane, **kw):
        if self.mode=='pam4':
            self.chip.PAM50[lane].prbs_rst_pam4()
        elif self.mode == 'nrz':
            self.chip.NRZ25[lane].prbs_rst_nrz()


    @deco_AllLanes
    def inject_prbs_err(self, lane, **kw):
        """Inject tx error"""
        if self.mode=='pam4':
            self.chip.PAM50[lane].err_inject()
        elif self.mode == 'nrz':
            self.chip.NRZ25[lane].err_inject_nrz()


    @deco_AllLanes
    def inject_prbs_err_para(self, lane, wait=0.001, **kw):
        """Inject errors on Parallel data path
        2/6/19 Alex
        Error injection
        0x4107[1:0] injects errors in data bits[33:32]. If you set any bit x to 1 it will generate one error in data[x]
        0x4108[15:0] injects errors in data bits[31:16]. If you set any bit x to 1 it will generate one error in data[x]
        0x4109[15:0] injects errors in data bits[15:0]. If you set any bit x to 1 it will generate one error in data[x]
        """
        # err_inject_nrz -- TX_NRZ_PRBS_GERR_EN
        self.chip.NRZ25[lane].TX_NRZ_PRBS_GERR_EN = 0
        self.chip.NRZ25[lane].TX_NRZ_PRBS_GERR_EN = 1
        time.sleep(0.001)
        self.chip.NRZ25[lane].TX_NRZ_PRBS_GERR_EN = 0

    def dump_prbs_errors_nrz(self, group=None):
        if group==None:
            group = self._get_cur_group()
        self.chip.setPhyAddr(group)
        # read error counts
        error_cnt = self.get_prbs_err_cntr(lane='all')
        self.reset_prbs_err_cntr(lane='all')
        lane_err = '| '.join(map(lambda s: "{:08x}".format(s), error_cnt))
        print("grp" + str(group) + " : " + lane_err + '|'),

    def dump_all_prbs_errors_nrz(self):
        print("-----+----------+---------+---------+---------+---------+---------+---------+---------+")
        print("lane |     0    |    1    |    2    |    3    |    4    |    5    |    6    |     7   |")
        print("-----+----------+---------+---------+---------+---------+---------+---------+---------+")
        for g in BlueJay.Groups:
            self.dump_prbs_errors_nrz(g)
            print("")
        print("-----+----------+---------+---------+---------+---------+---------+---------+---------+")



    #
    # Status
    #
    @deco_AllLanes
    def get_signal_detect(self, lane, timeout=1.0, **kw):
        """get_signal_detect
        returns Signal Detect, Phy Ready signal
        lanes: int or [int, int, ...]
        return: {lane:(sd, rdy), ...}
        """
        start_time = time.time()
        while True:
            nosig = False
            sd,rdy = self.chip.NRZ25[lane].ready_nrz()
            status = (sd,rdy)

            if status==(1,1):
                return status
            elif time.time() - start_time > timeout:
                return status
            else:
                time.sleep(0.1)


    @deco_AllLanes
    def get_adapt_done(self, lane, timeout=1.0, **kw):
        """returns rx adaptation done status"""
        start_time = time.time()
        while True:
            done = True
            # status=dict()
            # for ln in lanes:
            v = self.chip.MdioRd(0x40C7)
            done = (v >> lane) & 1
            if done==True:
                return done
            elif time.time() - start_time > timeout:
                return done
            else:
                time.sleep(0.1)


    @deco_AllLanes
    def get_serdes_params(self, lane, ber_en=0, **kw ):
        """returns eye
        :param lane: all_group', 'all_groups', 'tile', 'all_lanes', 'all'
        :param group: (optional) int
        :param ber_en: 0/1
        """
        
        """
        CTLE value written when CTLE override is enabled.
        0h:  22.9 dB
        1h:  20.4 dB
        2h:  17.4 dB
        3h:  15.1 dB
        4h:  13.0 dB
        5h:  10.6 dB
        6h:  7.5 dB
        7h:  2.5 dB        
        """
        
        if self.mode=='nrz':
            txtaps = self.chip.NRZ25[lane].tx_taps()
            ctle_val = self.chip.NRZ25[lane].ctle_nrz(); ctle_1 = self.chip.NRZ25[lane].ctle_map_nrz(ctle_val)[0]; ctle_2 = self.chip.NRZ25[lane].ctle_map_nrz(ctle_val)[1]
            dac_val = self.chip.NRZ25[lane].dac_nrz()
            # delta_val = self.chip.NRZ25[lane].delta_ph_nrz()
            eyes = self.chip.NRZ25[lane].eye_nrz()
            if ber_en:
                BER = self.chip.NRZ25[lane].ber_nrz()
            # edge1,edge2,edge3,edge4 = self.chip.NRZ25[lane].edge()
            agc_gain1 = self.chip.NRZ25[lane].agcgain()[0]
            agc_gain2 = self.chip.NRZ25[lane].agcgain()[1]
            skef_val = self.chip.NRZ25[lane].skef()[1]
            # NRZ_F1 =  0x12B[6:0] , NRZ_F2 =  0x12C[14:8], NRZ_F3 =  0x12C[6:0]
            dfe_f1 = self.chip.MdioBitRd(0x12b + (lane<<11), (6,0))
            dfe_f2 = self.chip.MdioBitRd(0x12c + (lane<<11), (14,8))
            dfe_f3 = self.chip.MdioBitRd(0x12c + (lane<<11), (6,0))

        else:
            txtaps = self.chip.PAM50[lane].tx_taps()
            ctle_val = self.chip.PAM50[lane].ctle_pam4()
            ctle_1 = self.chip.PAM50[lane].ctle_map_pam4(ctle_val)[0]
            ctle_2 = self.chip.PAM50[lane].ctle_map_pam4(ctle_val)[1]
            dac_val = self.chip.PAM50[lane].dac_pam4()
            delta_val = self.chip.PAM50[lane].delta_ph_pam4()
            eyes = self.chip.PAM50[lane].eye_pam4()
            if ber_en:
                BER = self.chip.PAM50[lane].ber_pam4()
            edge1,edge2,edge3,edge4 = self.chip.PAM50[lane].edge()
            agc_gain1 = self.chip.PAM50[lane].agcgain()[0]
            agc_gain2 = self.chip.PAM50[lane].agcgain()[1]
            skef_val = self.chip.PAM50[lane].skef()[1]
            # delta_val = self.chip.PAM50[lane].delta_ph_pam4()
            # edge1,edge2,edge3,edge4 = self.chip.PAM50[lane].edge()
            f0, f1, f1f0_ratio = self.chip.PAM50[lane].get_pam4_dfe()
            f13_val = self.chip.PAM50[lane].f13()
            (ffe_k1_bin, ffe_k2_bin, ffe_k3_bin, ffe_k4_bin, ffe_s1_bin, ffe_s2_bin) = self.chip.PAM50[lane].ffe_taps()

        params = OrderedDict()
        params['lane'] = lane
        params['txtaps'] = tuple(map(int, txtaps[0:5]))
        params['ctle'] = tuple(map(int, [ctle_val, ctle_1, ctle_2]))
        params['dac']  = dac_val
        if ber_en:
            params['ber']  = BER
        params['agc_gain1'] = agc_gain1
        params['agc_gain2'] = agc_gain2
        params['skef'] = skef_val

        if self.mode=='nrz':
            params['eye']  = eyes[1]
            params['dfe'] = {'f1':dfe_f1, 'f2':dfe_f2, 'f3':dfe_f3}
        elif self.mode=='pam4':
            params['eye']   = eyes
            params['delta'] = delta_val
            params['dfe']   = {'f0':f0, 'f1':f1, 'f1/f0': f1f0_ratio, 'f13':f13_val}
            params['edge']  = [edge1,edge2,edge3,edge4]
            params['ffe']   = {'k1':ffe_k1_bin, 'k2':ffe_k2_bin, 'k3':ffe_k3_bin,
                               'k4': ffe_k4_bin, 's1': ffe_s1_bin, 's2': ffe_s2_bin}

        return params

    def _get_param(self, lane, func):
        if self.mode == 'nrz':
            f = eval( 'self.chip.NRZ25[{}].{}'.format(lane, func) )
        elif self.mode == 'pam4':
            f = eval( 'self.chip.PAM50[{}].{}'.format(lane, func) )

        rtn = f()
        return rtn

    @deco_AllLanes
    def get_eye(self, lane, **kw):
        """returns eye
        :param lane: all_group', 'all_groups', 'tile', 'all_lanes', 'all'
        """
        if self.mode=='nrz':
            eye = self._get_param(lane, 'eye_nrz')
        elif self.mode=='pam4':
            eye = self._get_param(lane, 'eye_pam4')

        print '{},{}: {}'.format(self.chip.group, lane, str(eye))

        return eye

    #
    # FEC analyzer
    #
    @deco_AllLanes
    def fec_analyzer_init(self, lane, delay=.1, err_type=0, T=15, M=10, N=5440, print_en=1, **kw):

        if self.mode != 'pam4':
            if T > 7:
                T = 7

        # err_type: tei ctrl teo ctrl
        self.chip.FECANA[lane].PRBS_CHK_CNTR_RESET = 0
        self.chip.FECANA[lane].RX_PRBS_FORCE_RELOAD = 1
        self.chip.FECANA[lane].RX_PRBS_AUTO_SYNC_EN = 1
        self.chip.FECANA[lane].PRBS_MISMATCH_THR = 0x10
        self.chip.FECANA[lane].PRBS_SYNC_THR = 0x2
        self.chip.FECANA[lane].PRBS_MODE = 0x3
        self.chip.FECANA[lane].SYM_SIZE = M
        self.chip.FECANA[lane].FRAM_SIZE = N
        self.chip.FECANA[lane].CORR_SIZE = T
        self.chip.FECANA[lane].TH_SIZE = T
        self.chip.FECANA[lane].CNT_CLR = 1
        self.chip.FECANA[lane].CNT_FREEZE = 0
        self.chip.FECANA[lane].FEC_CLK_EN = 1
        self.chip.FECANA[lane].FEC_ANA_EN = 1
        self.chip.FECANA[lane].CNT_CLR = 0
        self.chip.FECANA[lane].CTRL_TEO = err_type
        self.chip.FECANA[lane].CTRL_TEI = err_type
        if print_en:
            print '\n....Lane {}: FEC Analyzer Initialized'.format(lane),

        time.sleep(delay)

    @deco_AllLanes
    def rx_monitor_clear(self, lane, **kw):

        ###### 1. Initialize FEC Analyzer for this lane
        if self.mode == 'pam4':
            self.fec_analyzer_init(lane=lane, delay=.1, err_type=0, T=15, M=10, N=5440, print_en=0)
            ###### 2. Clear Rx PRBS Counter for this lane
            self.chip.PAM50[lane].prbs_rst_pam4()
        else:  # Lane is in NRZ mode
            self.fec_analyzer_init(lane=lane, delay=.1, err_type=0, T=7, M=10, N=5280, print_en=0)
            self.chip.NRZ25[lane].prbs_rst_nrz()

        ###### 3. Capture Time Stamp for Clearing FEC and PRBS Counters. Used for Calculating BER
        prbs_reset_time = time.time()  # get the time-stamp for the counter clearing

        ###### 4. Clear Stats for this lane
        #                     prbs1/2/3                               eye123     fec1,2

        t = self.chip.tile
        g = self.chip.group
        self.laneStats[t][g][lane] = [0, 0, 0, prbs_reset_time, prbs_reset_time, 0, 0, 0, 'CLR', 0, 0]


    @deco_AllLanes
    def fec_analyzer_tei(self, lane, **kw):
        self.chip.FECANA[lane].READ_SEL = 4
        tei_l = self.chip.FECANA[lane].READ_DATA       # read data
        self.chip.FECANA[lane].READ_SEL = 5       # set reading data of TEi high 16 bit
        tei_h = self.chip.FECANA[lane].READ_DATA       # read data
        tei = tei_h * 65536 + tei_l  # combinate the data
        # print '\n....Lane %s: TEi counter: %d' %(lane_name_list[lane],tei),
        return tei

    @deco_AllLanes
    def fec_analyzer_teo(self, lane, **kw):
        self.chip.FECANA[lane].READ_SEL = 6   #set reading data of TEo low 16 bit
        teo_l = self.chip.FECANA[lane].READ_DATA  #read data
        self.chip.FECANA[lane].READ_SEL = 7  #set reading data of TEo high 16 bit
        teo_h = self.chip.FECANA[lane].READ_DATA  #read data
        teo = teo_h*65536+teo_l#combinate the data
        #print '\n....Lane %s: TEo counter: %d' %(lane_name_list[lane],teo),
        return teo

    @deco_AllLanes
    def rx_monitor_capture(self, lane, **kw):

        t = self.chip.tile
        g = self.chip.group
        _lanestats = self.laneStats[t][g][lane]

        ###### 1. Capture FEC Analyzer Data for this lane
        tei = self.fec_analyzer_tei(lane)
        teo = self.fec_analyzer_teo(lane)
        # sei = fec_analyzer_sei(lane=lane)
        # bei = fec_analyzer_bei(lane=lane)

        ###### 2. Capture PRBS Counter for this lane
        if self.mode == 'nrz':
            cnt = long(self.chip.NRZ25[lane].RX_NRZ_PRBS_READ_ERR_HIGH_RX_NRZ_PRBS_READ_ERR_LOW)
            cnt1 = long(self.chip.NRZ25[lane].RX_NRZ_PRBS_READ_ERR_HIGH_RX_NRZ_PRBS_READ_ERR_LOW)
            if cnt1 < cnt:
                cnt = cnt1
        else:
            self.chip.PAM50[lane].latch_data_pam4()
            cnt = long(self.chip.PAM50[lane].PRBS_READ_SYNC_ERR_CNTR_MSB_PRBS_READ_SYNC_ERR_CNTR_LSB)
            self.chip.PAM50[lane].latch_data_release_pam4()
        ###### 3. Capture Time Stamp for the FEC and PRBS Counters. Used for Calculating BER
        cnt_time = time.time()  # PrbsReadoutTime = get the time stamp ASAP for valid prbs count

        if self.mode == 'nrz':
            sd, rdy = self.chip.NRZ25[lane].ready_nrz()
        else:
            sd, rdy = self.chip.PAM50[lane].ready_pam4()
        cnt_n1 = _lanestats[0]  # PrevPrbsCount-1
        cnt_n2 = _lanestats[1]  # PrevPrbsCount-2
        if _lanestats[3] != 0:  # if the PRBS count was cleared at least once before, use the time of last clear
            prbs_reset_time = _lanestats[3]
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
            if self.mode == 'nrz':
                eyes = [self.chip.NRZ25[lane].eye_nrz()[1], 0, 0]
            else:
                eyes = self.chip.PAM50[lane].eye_pam4()
            if (cnt < cnt_n1):  # check for PRBS counter wrap-around
                cnt = 0xFFFFFFFFL  # return an artificially large number if counter rolls over
                cnt_n1 = 0  # PrevPrbsCount-1
                cnt_n2 = 0  # PrevPrbsCount-2

        # if RDY=1 and PRBS count value is considered a valid count
        _lanestats = [cnt, cnt_n1, cnt_n2, prbs_reset_time, cnt_time, eyes[0], eyes[1], eyes[2],
                                  link_status, tei, teo]

        # BER
        def _getber(c,t):
            if c == 0 or t == 0:
                _ber = 0
            else:
                _ber = c / t
                if (_lanestats[8] != 'RDY'):
                    _ber = 'INV'
            return _ber

        prbs_accum_time = _lanestats[4] - _lanestats[3]
        bits_transferred = float((prbs_accum_time * self.datarate * pow(10, 9)))
        cur_cnt     = float(_lanestats[0])
        prefec_cnt  = float(_lanestats[9])
        postfec_cnt = float(_lanestats[10])
        ber_val     = _getber(cur_cnt, bits_transferred)
        prefec_ber  = _getber(cur_cnt, bits_transferred)
        postfec_ber = _getber(cur_cnt, bits_transferred)

        rtn = OrderedDict()
        # rtn['tei'] = tei
        # rtn['teo'] = teo
        rtn['lane'] = lane
        rtn['eye1'] = "{:3.1f}".format(eyes[0])
        rtn['eye2'] = "{:3.1f}".format(eyes[1])
        rtn['eye3'] = "{:3.1f}".format(eyes[2])
        rtn['link_status'] = link_status
        rtn['ber'] = ber_val
        rtn['preFEC'] = tei
        rtn['postFEC'] = teo
        rtn['preFEC_ber'] = prefec_ber
        rtn['postFEC_ber'] = postfec_ber
        return rtn


    def rx_monitor_print(self, lane):
        """lane: int, list of int, 'all'"""
        myber = 99e-1
        gFecThresh = 15

        t = self.chip.tile
        g = self.chip.group
        _lanestats = self.laneStats[t][g][lane]

        group = self._get_cur_group()
        if lane != list:
            lanes = [lane]
        elif type(lane)==str:
            if lane=='all':
                n = BlueJay.Lanes[group]
                lanes = range(n)

        #Slice = gSlice
        #get_lane_mode(lanes)
        #if sl == None:  # if slice is not defined used gSlice
            #sl = gSlice

        print("\n-------------"),
        for lane in lanes:
            if lane == 8:
                print("|-------"),
            else:
                print("--------"),

        #    print("\n       Slice"),
        print("\n        Group"),
        for lane in lanes:  print("       %d" % (group)),
        print("\n         Lane"),
        for lane in lanes:  print("{}".format(lane)),

        print("\n     Encoding"),
        for lane in lanes:  print("%8s" % (self.mode.upper())),
        print("\nDataRate Gbps"),
        for lane in lanes:  print("%8.4f" % (self.datarate)),

        print("\n-------------"),
        for lane in lanes:
            if lane == 8:
                print("|-------"),
            else:
                print("--------"),
        print("\n  Link Status"),
        for lane in lanes:  print("%8s" % (_lanestats[8])),
        print("\n    Eye1 (mV)"),
        for lane in lanes:
            if (_lanestats[8] == 'RDY'):
                print ("%8.0f" % (_lanestats[5])),
            else:
                print("       -"),
        print("\n    Eye2 (mV)"),
        for lane in lanes:
            if (self.mode == 'pam4' and _lanestats[8] == 'RDY'):
                print ("%8.0f" % (_lanestats[6])),
            else:
                print("       -"),
        print("\n    Eye3 (mV)"),
        for lane in lanes:
            if (self.mode.upper() == 'PAM4' and _lanestats[8] == 'RDY'):
                print ("%8.0f" % (_lanestats[7])),
            else:
                print("       -"),

        print("\n-------------"),
        for lane in lanes:
            if lane == 8:
                print("|-------"),
            else:
                print("--------"),
        print("\n Elapsed Time"),
        for lane in lanes:
            if (_lanestats[8] == 'RDY'):
                elapsed_time = _lanestats[4] - _lanestats[3]
                if elapsed_time < 1000.0:
                    print("%6.1f s" % (elapsed_time)),
                else:
                    print("%6.0f s" % (elapsed_time)),
            else:
                print("       -"),

        # print("\nPrev PRBS    "),
        # for lane in lanes:  print("%8X " % (self.laneStats[gSlice][lane][1]) ),
        # print("\nCurr PRBS    "),
        print("\n     PRBS Cnt"),
        for lane in lanes:
            if (_lanestats[8] == 'RDY'):
                print("%8X" % (_lanestats[0])),
            else:
                print("       -"),
        # print("\nDeltaPRBS    "),
        # for lane in lanes:  print("%8X " % (self.laneStats[gSlice][lane][0] - self.laneStats[gSlice][lane][1]) ),

        print("\n     PRBS BER"),
        for lane in lanes:
            curr_prbs_cnt = float(_lanestats[0])
            prbs_accum_time = _lanestats[4] - _lanestats[3]
            # data_rate = self.datarate
            bits_transferred = float((prbs_accum_time * self.datarate * pow(10, 9)))
            if curr_prbs_cnt == 0 or bits_transferred == 0:
                ber_val = 0
                print("%8d" % (ber_val)),
                myber = ((ber_val))
            else:
                ber_val = curr_prbs_cnt / bits_transferred
                if (_lanestats[8] == 'RDY'):
                    print("%8.1e" % (ber_val)),
                    myber = ((ber_val))
                else:
                    print("       -"),

        print("\n-------------"),
        for lane in lanes:
            if lane == 8:
                print("|-------"),
            else:
                print("--------"),
        # print "\n  User has set the FEC Threshold to less than max"
        if (self.mode == 'pam4' and gFecThresh < 15) or \
                (self.mode != 'pam4' and gFecThresh < 7):
            print("\n   FEC Thresh"),
            for lane in lanes:
                print("%8d" % (gFecThresh)),

        print("\n  pre-FEC Cnt"),
        for lane in lanes:
            if (_lanestats[8] == 'RDY'):
                print("%8X" % (_lanestats[9])),
            else:
                print("       -"),

        print("\n Post-FEC Cnt"),
        for lane in lanes:
            if (_lanestats[8] == 'RDY'):
                print("%8X" % (_lanestats[10])),
            else:
                print("       -"),
        print("\n  pre-FEC BER"),
        for lane in lanes:
            curr_prbs_cnt = float(_lanestats[9])
            prbs_accum_time = _lanestats[4] - _lanestats[3]
            # data_rate = gEncodingMode[lane][1]
            bits_transferred = float((prbs_accum_time * self.datarate * pow(10, 9)))
            if curr_prbs_cnt == 0 or bits_transferred == 0:
                ber_val = 0
                print("%8d" % (ber_val)),
            else:
                ber_val = curr_prbs_cnt / bits_transferred
                if (_lanestats[8] == 'RDY'):
                    print("%8.1e" % (ber_val)),
                else:

                    print("       -"),
        print("\n Post-FEC BER"),
        for lane in lanes:
            curr_prbs_cnt = float(_lanestats[10])
            prbs_accum_time = _lanestats[4] - _lanestats[3]
            # data_rate = gEncodingMode[lane][1]
            bits_transferred = float((prbs_accum_time * self.datarate * pow(10, 9)))
            if curr_prbs_cnt == 0 or bits_transferred == 0:
                ber_val = 0
                print("%8d" % (ber_val)),
            else:
                ber_val = curr_prbs_cnt / bits_transferred
                if (_lanestats[8] == 'RDY'):
                    print("%8.1e" % (ber_val)),
                else:
                    print("       -"),

        print("\n-------------"),
        for lane in lanes:
            if lane == 8:
                print("|-------"),
            else:
                print("--------"),
        print("\n")

        return myber

    @deco_AllLanes
    def rx_monitor(self, lane, rst=0, **kw):

        tile = self.chip.tile
        group = self._get_cur_group()
        if (rst == 1):
            self.rx_monitor_clear(lane)
            # time.sleep(0.9)
        else:  # rst=0
            # in case any of the lanes' PRBS not cleared or FEC Analyzer not initialized, clear its Stats first
            if self.laneStats[tile][group][lane][3] == 0:
                self.rx_monitor_clear(lane)
                print("\nGroup %d Lane %d RX_MONITOR Initialized First!" % (group, lane)),

        rtn = self.rx_monitor_capture(lane, **kw)
        # if print_en:
            # mydata = self.rx_monitor_print(lane, **kw)
        return rtn

    @deco_AllLanes
    def get_lane_mode(self, lane, **kw):

        if self.chip.NRZ25[lane].PU_TX_BG == 0 or self.chip.NRZ25[lane].PU_RX_BG == 0:  # Lane's bandgap is OFF
            # print ("\n Slice %d lane %2d is OFF"%(gSlice,lane)),
            self.datarate = 1.0
            self.mode = 'off'
            # self.lane_mode_list[lane] = 'off'

        elif self.chip.NRZ25[lane].TX_NRZ_MODE == 0 and self.chip.PAM50[lane].PAM4_EN == 1:
            # print ("\n Slice %d lane %2d is PAM4"%(gSlice,lane)),
            data_rate = self.chip.PAM50[lane].get_lane_pll()[0][0]
            self.mode = 'pam4'
            self.datarate = data_rate
            # self.lane_mode_list[lane] = 'pam4'
        else:
            # print ("\n Slice %d lane %2d is NRZ"%(gSlice,lane)),
            data_rate = self.chip.NRZ25[lane].get_lane_pll()[0][0]
            self.mode = 'nrz'
            self.datarate = data_rate
            # self.lane_mode_list[lane] = 'nrz'

        return (self.mode, self.datarate)

    @deco_AllLanes
    def static_power_down(self, lane, rx_off=1, tx_off=1, rx_bg_off=0 , tx_bg_off=0, **kw):

        if rx_bg_off == 1:
            self.chip.NRZ25[lane].PU_RX_BG = 0
        else:
            self.chip.NRZ25[lane].PU_RX_BG = 1
        if tx_bg_off == 1:
            self.chip.NRZ25[lane].PU_TX_BG = 0
        else:
            self.chip.NRZ25[lane].PU_TX_BG = 1
        if rx_off == 1:
            self.chip.NRZ25[lane].RX_VBG = 0
            self.chip.NRZ25[lane].PU_RX_RVDD = 0
            self.chip.NRZ25[lane].PU_RX_PLL = 0
            self.chip.NRZ25[lane].Reg01FF_5 = 0    #pd_agc_ma
            self.chip.NRZ25[lane].PU_AGC_1_MASTER = 0
            self.chip.NRZ25[lane].PU_AGCDL_MASTER = 0
            self.chip.NRZ25[lane].Reg01F8_15 = 0   #pd_adc_,a
            self.chip.NRZ25[lane].Reg01F8_13 = 0   #pd_adc_1_ma
            self.chip.NRZ25[lane].PU_RX_AGC_LN = 0
            self.chip.NRZ25[lane].PU_AGCDL = 0
            self.chip.NRZ25[lane].PU_RX_PLL_INTP = 0
            self.chip.NRZ25[lane].Reg01FF_7 = 0    #pd_rvddloop_rx
            self.chip.NRZ25[lane].Reg01F8_14 = 0   #pd_clkcompreg_ma
            self.chip.NRZ25[lane].Reg01F8_9 = 0    #pd_intp_ma
            self.chip.NRZ25[lane].Reg01FC_6_4 = 0  #vrvdd_rx
            self.chip.NRZ25[lane].Reg01FC_3_1 = 0  #vrvdd2_rx
            self.chip.NRZ25[lane].PU_DEGENMAIN_SUM1_MSB = 0
            self.chip.NRZ25[lane].PU_DEGENMAIN_SUM1_LSB = 0
            self.chip.NRZ25[lane].PU_DEGENMAIN_SUM2_MSB = 0
            self.chip.NRZ25[lane].PU_DEGENMAIN_SUM2_LSB = 0
            self.chip.NRZ25[lane].PU_DEGENMAIN_SUM3_MSB = 0
            self.chip.NRZ25[lane].PU_DEGENMAIN_SUM3_LSB = 0
            self.chip.NRZ25[lane].PU_INTP = 0
            self.chip.FECANA[lane].FEC_ANA_EN = 0
        else:
            self.chip.NRZ25[lane].RX_VBG = 0x7
            self.chip.NRZ25[lane].PU_RX_RVDD = 1
            self.chip.NRZ25[lane].PU_RX_PLL = 1
            self.chip.NRZ25[lane].Reg01FF_5 = 1
            self.chip.NRZ25[lane].PU_AGC_1_MASTER = 1
            self.chip.NRZ25[lane].PU_AGCDL_MASTER = 1
            self.chip.NRZ25[lane].Reg01F8_15 = 1
            self.chip.NRZ25[lane].Reg01F8_13 = 1
            self.chip.NRZ25[lane].PU_RX_AGC_LN = 1
            if self.mode=='nrz':
                self.chip.NRZ25[lane].PU_AGCDL = 0
            elif self.mode=='pam4':
                self.chip.NRZ25[lane].PU_AGCDL = 1
            self.chip.NRZ25[lane].PU_RX_PLL_INTP = 1
            self.chip.NRZ25[lane].Reg01FF_7 = 1        # pu_rvddloop_rx
            self.chip.NRZ25[lane].Reg01F8_14 = 1       # pu_clkcompreg_ma
            self.chip.NRZ25[lane].Reg01F8_9 = 1        # pu_intp_ma
            self.chip.NRZ25[lane].Reg01FC_6_4 = 0x4  # vrvdd_rx
            self.chip.NRZ25[lane].Reg01FC_3_1 = 0x4  # vrvdd2_rx
            self.chip.NRZ25[lane].PU_DEGENMAIN_SUM1_MSB = 1
            self.chip.NRZ25[lane].PU_DEGENMAIN_SUM1_LSB = 1
            self.chip.NRZ25[lane].PU_DEGENMAIN_SUM2_MSB = 1
            self.chip.NRZ25[lane].PU_DEGENMAIN_SUM2_LSB = 1
            self.chip.NRZ25[lane].PU_DEGENMAIN_SUM3_MSB = 1
            self.chip.NRZ25[lane].PU_DEGENMAIN_SUM3_LSB = 1
            self.chip.NRZ25[lane].PU_INTP = 1
            self.chip.FECANA[lane].FEC_ANA_EN = 1
        if tx_off == 1:
            self.chip.NRZ25[lane].TX_VBG = 0
            self.chip.NRZ25[lane].Reg00FF_11 = 0   #pu_rvdd_tx
            self.chip.NRZ25[lane].PU_TX_PLL = 0
            self.chip.NRZ25[lane].PU_VDRV_MA = 0
            self.chip.NRZ25[lane].Reg00EB_13 = 0   #pd vdrv
            self.chip.NRZ25[lane].PU_HIMODE_VDDR = 0
            self.chip.NRZ25[lane].Reg00F1_2 = 0    #pd_adc
            self.chip.NRZ25[lane].Reg00FF_7 = 0    #pd_rvddloop_tx
            self.chip.NRZ25[lane].Reg00FD_6_4 = 0  #vrdd_tx default is 011
            self.chip.NRZ25[lane].Reg00FD_3_1 = 0  #vpllpmp1_tx default is 011
            self.chip.NRZ25[lane].Reg00F1_1 = 0    #pd_clkcomp
            self.chip.NRZ25[lane].Reg00F1_0 = 0    #pd_clkcompreg
            #self.chip.NRZ25[lane].PU_ACJTAG = 0    #pd_acjtag
        else:
            self.chip.NRZ25[lane].TX_VBG = 0x7
            self.chip.NRZ25[lane].Reg00FF_11 = 1   #pu_rvdd_tx
            self.chip.NRZ25[lane].PU_TX_PLL = 1
            self.chip.NRZ25[lane].PU_VDRV_MA = 1
            self.chip.NRZ25[lane].Reg00EB_13 = 1
            self.chip.NRZ25[lane].PU_HIMODE_VDDR = 0
            self.chip.NRZ25[lane].Reg00F1_2 = 1
            self.chip.NRZ25[lane].Reg00FF_7 = 1      # pu_rvddloop_tx
            self.chip.NRZ25[lane].Reg00FD_6_4 = 0x3  # vrdd_tx default is 011
            self.chip.NRZ25[lane].Reg00FD_3_1 = 0x3  # vpllpmp1_tx default is 011
            self.chip.NRZ25[lane].Reg00F1_1 = 1      # pu_clkcomp
            self.chip.NRZ25[lane].Reg00F1_0 = 1      # pu_clkcompreg
            #self.chip.NRZ25[lane].PU_ACJTAG = 1      # pu_acjtag

    @deco_AllLanes
    def print_powerdownbits(self, lane, **kw):

        print "---- Lane {} ----".format(lane)
        print
        power_down = {
        "PU_RX_BG": self.chip.NRZ25[lane].PU_RX_BG,
        "PU_TX_BG": self.chip.NRZ25[lane].PU_TX_BG,
        "RX_VBG": self.chip.NRZ25[lane].RX_VBG,
        "PU_RX_RVDD": self.chip.NRZ25[lane].PU_RX_RVDD,
        "PU_RX_PLL": self.chip.NRZ25[lane].PU_RX_PLL,
        "Reg01FF_5": self.chip.NRZ25[lane].Reg01FF_5,
        "PU_AGC_1_MASTER": self.chip.NRZ25[lane].PU_AGC_1_MASTER,
        "PU_AGCDL_MASTER": self.chip.NRZ25[lane].PU_AGCDL_MASTER,
        "Reg01F8_15": self.chip.NRZ25[lane].Reg01F8_15,
        "Reg01F8_13": self.chip.NRZ25[lane].Reg01F8_13,
        "PU_RX_AGC_LN": self.chip.NRZ25[lane].PU_RX_AGC_LN,
        "PU_AGCDL": self.chip.NRZ25[lane].PU_AGCDL,
        "PU_RX_PLL_INTP": self.chip.NRZ25[lane].PU_RX_PLL_INTP,
        "Reg01FF_7": self.chip.NRZ25[lane].Reg01FF_7,
        "Reg01F8_14": self.chip.NRZ25[lane].Reg01F8_14,
        "Reg01F8_9": self.chip.NRZ25[lane].Reg01F8_9,
        "Reg01FC_6_4": self.chip.NRZ25[lane].Reg01FC_6_4,
        "Reg01FC_3_1": self.chip.NRZ25[lane].Reg01FC_3_1,
        "PU_DEGENMAIN_SUM1_MSB": self.chip.NRZ25[lane].PU_DEGENMAIN_SUM1_MSB,
        "PU_DEGENMAIN_SUM1_LSB": self.chip.NRZ25[lane].PU_DEGENMAIN_SUM1_LSB,
        "PU_DEGENMAIN_SUM2_MSB": self.chip.NRZ25[lane].PU_DEGENMAIN_SUM2_MSB,
        "PU_DEGENMAIN_SUM2_LSB": self.chip.NRZ25[lane].PU_DEGENMAIN_SUM2_LSB,
        "PU_DEGENMAIN_SUM3_MSB": self.chip.NRZ25[lane].PU_DEGENMAIN_SUM3_MSB,
        "PU_DEGENMAIN_SUM3_LSB": self.chip.NRZ25[lane].PU_DEGENMAIN_SUM3_LSB,
        "PU_INTP": self.chip.NRZ25[lane].PU_INTP,
        ".FEC_ANA_EN": self.chip.FECANA[lane].FEC_ANA_EN,
        "TX_VBG": self.chip.NRZ25[lane].TX_VBG,
        "Reg00FF_11": self.chip.NRZ25[lane].Reg00FF_11,
        "PU_TX_PLL": self.chip.NRZ25[lane].PU_TX_PLL,
        "PU_VDRV_MA": self.chip.NRZ25[lane].PU_VDRV_MA,
        "Reg00EB_13": self.chip.NRZ25[lane].Reg00EB_13,
        "PU_HIMODE_VDDR": self.chip.NRZ25[lane].PU_HIMODE_VDDR,
        "Reg00F1_2": self.chip.NRZ25[lane].Reg00F1_2,
        "Reg00FF_7": self.chip.NRZ25[lane].Reg00FF_7,
        "Reg00FD_6_4": self.chip.NRZ25[lane].Reg00FD_6_4,
        "Reg00FD_3_1": self.chip.NRZ25[lane].Reg00FD_3_1,
        "Reg00F1_1": self.chip.NRZ25[lane].Reg00F1_1,
        "Reg00F1_0": self.chip.NRZ25[lane].Reg00F1_0,}

        for reg in power_down.keys():
            print "{:24s} {}".format(reg, power_down[reg])


    @deco_AllLanes
    def static_power_down_broadcast_mode(self, lane, **kw):
        """ Boardcasting available """
        # if 'group' in kw.keys():
            # group = kw['group']
            # if type(group) != type:
                # group=[group]
        # else:
            # group = [self._get_cur_group()]

        # self.broadcast_mode( groups=group, en=1)

        self.chip.MdioWr(0x1FF + 0x800*lane, 0x1511)
        self.chip.MdioWr(0x1FE + 0x800*lane, 0x0)
        self.chip.MdioWr(0x1FD + 0x800*lane, 0x2934)
        self.chip.MdioWr(0x1FC + 0x800*lane, 0x1200)
        self.chip.MdioWr(0x1F8 + 0x800*lane, 0x0)
        self.chip.MdioWr(0x1F3 + 0x800*lane, 0xB440)
        self.chip.MdioWr(0x1E7 + 0x800*lane, 0xB6C)
        self.chip.MdioWr(0x1E0 + 0x800*lane, 0x0)
        self.chip.MdioWr(0x1DD + 0x800*lane, 0x8040)
        self.chip.MdioWr(0x0FF + 0x800*lane, 0x1576)
        self.chip.MdioWr(0x0FE + 0x800*lane, 0x2934)
        self.chip.MdioWr(0x0FD + 0x800*lane, 0x1200)
        self.chip.MdioWr(0x0FA + 0x800*lane, 0x10)
        self.chip.MdioWr(0x0F1 + 0x800*lane, 0x8)
        self.chip.MdioWr(0x0EB + 0x800*lane, 0x436C)


    @deco_AllLanes
    def read_plus_minus_margin_nrz(self, lane):
        """returns (plus_margin, minus_margin)"""
        plus_0 = self.chip.NRZ25[lane].Reg0100_001A_15_4
        plus_1 = self.chip.NRZ25[lane].Reg0100_001A_3_0S
        plus_2 = self.chip.NRZ25[lane].Reg0100_001B_7_0S
        plus_3 = self.chip.NRZ25[lane].Reg0100_001C_11_0
        plus_4 = self.chip.NRZ25[lane].Reg0100_001D_15_4
        plus_5 = self.chip.NRZ25[lane].Reg0100_001D_3_0S
        plus_6 = self.chip.NRZ25[lane].Reg0100_001E_7_0S
        plus_7 = self.chip.NRZ25[lane].Reg0100_001F_11_0
        if plus_0 > 2047: plus_0 = plus_0 - 4096
        if plus_1 > 2047: plus_1 = plus_1 - 4096
        if plus_2 > 2047: plus_2 = plus_2 - 4096
        if plus_3 > 2047: plus_3 = plus_3 - 4096
        if plus_4 > 2047: plus_4 = plus_4 - 4096
        if plus_5 > 2047: plus_5 = plus_5 - 4096
        if plus_6 > 2047: plus_6 = plus_6 - 4096
        if plus_7 > 2047: plus_7 = plus_7 - 4096
        plus_margin = [plus_0,plus_1,plus_2,plus_3,plus_4,plus_5,plus_6,plus_7]
        minus_0 = self.chip.NRZ25[lane].Reg0100_0020_15_4
        minus_1 = self.chip.NRZ25[lane].Reg0100_0020_3_0S
        minus_2 = self.chip.NRZ25[lane].Reg0100_0021_7_0S
        minus_3 = self.chip.NRZ25[lane].Reg0100_0022_11_0
        minus_4 = self.chip.NRZ25[lane].Reg0100_0023_15_4
        minus_5 = self.chip.NRZ25[lane].Reg0100_0023_3_0S
        minus_6 = self.chip.NRZ25[lane].Reg0100_0024_7_0S
        minus_7 = self.chip.NRZ25[lane].Reg0100_0025_11_0
        if minus_0 > 2047: minus_0 = minus_0 - 4096
        if minus_1 > 2047: minus_1 = minus_1 - 4096
        if minus_2 > 2047: minus_2 = minus_2 - 4096
        if minus_3 > 2047: minus_3 = minus_3 - 4096
        if minus_4 > 2047: minus_4 = minus_4 - 4096
        if minus_5 > 2047: minus_5 = minus_5 - 4096
        if minus_6 > 2047: minus_6 = minus_6 - 4096
        if minus_7 > 2047: minus_7 = minus_7 - 4096
        minus_margin = [minus_0,minus_1,minus_2,minus_3,minus_4,minus_5,minus_6,minus_7]
        result = (plus_margin, minus_margin)

        return result


    #
    # Read On-Chip Temperature Sensor
    #
    def temp_sensor(self, auto=0):
        self.chip.setPhyAddr(9)
        self._temp_sensor_start(auto)
        value1 = self._temp_sensor_read(auto)
        print('Slice TempSensor: %3.1f C' % (value1))
        return value1

    def _temp_sensor_start(self, auto=0):
        """ Set up On-Chip Temperature Sensor (to be read later) """
        base = 0x4B00
        self.chip.MdioWr(0x4d00, 0x5d81)
        if(auto):
            self.chip.MdioWr(base+0x3a, 0x3f)
        else:
            self.chip.MdioWr(base+0x3a, 0x7)
        self.chip.MdioWr(base + 0x3e, 0x0054)  # set clock
        time.sleep(1)
        self.chip.MdioWr(base + 0x37, 0x0)  # reset sensor
        time.sleep(1)

    def _temp_sensor_read(self, auto=0):
        """ Read back On-Chip Temperature Sensor (after it's been set up already) """
        base = 0x4B00
        Yds = 237.7
        Kds = 79.925
        time1 = time.time()
        time2 = time.time()
        if auto == 1:
            rdy = 0
            while (rdy == 0):  # wait for rdy
                value = self.chip.MdioRd(0x4859)
                rdy = value>>12
                time2 = time.time()
                if (time2-time1)>=5:
                    print 'AutoReadTsensor test timeout2...'
            realVal = (value&0x0fff) * Yds / 4096 - Kds
            print('tempsensor%d: %d,realVal:%f' % (0, value, realVal))
        else:
            addr = [base + 0x39, base + 0x3a, base + 0x3b, base + 0x3c]
            self.chip.MdioWr(base + 0x37, 0xc)  # set no ack
            rdy = self.chip.MdioRd(base + 0x38) >> 8
            while (rdy == 0):  # wait for rdy
                rdy = self.chip.MdioRd(base + 0x38) >> 8
                time2 = time.time()
                if (time2 - time1) >= 5:
                    print 'ReadTSensor test timeout.2..'
                    break
            value = self.chip.MdioRd(addr[0])
            realVal = value * Yds / 4096 - Kds
            print('tempsensor%d: %d,realVal:%f' % (0, value, realVal))
        return realVal

    #
    # Loopback functions
    #
    @deco_AllLanes
    def rx_tx_serial_loopback(self, lane, enable=1, TX_pat=3, RX_pat=3, **kw):
        #if enable == 1:
        if self.mode == 'pam4':
            if enable == 1:
            # In PAM4 mode we need to use divide by 2 for TX PLL
                self.chip.PAM50[lane].TX_PLL_N = 42
                self.chip.PAM50[lane].Reg00FF_1 = 0
                self.chip.PAM50[lane].RX_REFCLK_DIV4_EN = 0
                self.chip.PAM50[lane].Reg00D7_13 = 1
                self.chip.PAM50[lane].Reg00D7_15_14 = 2
                self.chip.PAM50[lane].Reg00D9_3_0S = 0x80000
            else:
                self.chip.PAM50[lane].TX_PLL_N = 85
                self.chip.PAM50[lane].Reg00FF_1 = 1
                self.chip.PAM50[lane].RX_REFCLK_DIV4_EN = 0
                self.chip.PAM50[lane].Reg00D7_13 = 0
                self.chip.PAM50[lane].Reg00D7_15_14 = 0
                self.chip.PAM50[lane].Reg00D9_3_0S = 0

        # Disable Link Training of the lane
        self.chip.MdioWr(0x400 + 0x800 * lane, 0x0000)
        # Disable AutoNeg of the lane
        self.chip.MdioWr(0x600 + 0x800 * lane, 0xC000)
        if self.mode == 'pam4':
            if enable == 0:
                self.chip.PAM50[lane].TX_PI_EN = enable
            self.chip.PAM50[lane].PH_ROTR_OW = 0x0
            self.chip.PAM50[lane].PH_ROTR_OWEN = enable
            self.chip.PAM50[lane].TX_PH_ROTR_FLIP = 0
        else:
            if enable == 0:
                self.chip.NRZ25[lane].TX_PI_EN = enable
            self.chip.NRZ25[lane].PH_ROTR_OW = 0x0
            self.chip.NRZ25[lane].PH_ROTR_OWEN = enable
            self.chip.NRZ25[lane].TX_PH_ROTR_FLIP = 0

        if enable == 1:
            prbs_mode_select(lane=lane, tx_prbs_mode='functional', rx_prbs_mode='prbs', tx_pat=None, rx_pat=None)
        else:
            prbs_mode_select(lane=lane, tx_prbs_mode='prbs', rx_prbs_mode='prbs', tx_pat=TX_pat, rx_pat=RX_pat)

        self.chip.LANE_TOP[lane].RX_TO_TX_LPBK = enable
        if self.mode == 'pam4':
            if enable == 1:
                self.chip.PAM50[lane].Reg0079_14 = 0
                self.chip.PAM50[lane].Reg0079_13 = 1
                self.chip.PAM50[lane].LOCK_PH_FLIP = 1
                self.chip.PAM50[lane].SPD_SEL2 = 0
                self.chip.PAM50[lane].SPD_SEL1 = 0
                self.chip.PAM50[lane].FREQ_MUP = 0x4
                self.chip.PAM50[lane].FREQ_MULTIPLIER = 0x0
                self.chip.PAM50[lane].FREQ_SHIFT_SEL = 0x4

            else:
                self.chip.PAM50[lane].Reg0079_14 = 0
                self.chip.PAM50[lane].Reg0079_13 = 0
                self.chip.PAM50[lane].LOCK_PH_FLIP = 0
                self.chip.PAM50[lane].SPD_SEL2 = 0
                self.chip.PAM50[lane].SPD_SEL1 = 0
                self.chip.PAM50[lane].FREQ_MUP = 0x5
                self.chip.PAM50[lane].FREQ_MULTIPLIER = 0x0
                self.chip.PAM50[lane].FREQ_SHIFT_SEL = 0x4

        else:
            if enable == 1:
                self.chip.NRZ25[lane].Reg0100_007A_12 = 1
                self.chip.NRZ25[lane].Reg0100_007A_11_10 = 0
                self.chip.NRZ25[lane].Reg0100_007A_9_8 = 0
                self.chip.NRZ25[lane].Reg0100_007A_7_5 = 0x4
                self.chip.NRZ25[lane].Reg0100_007A_4_3 = 0x0
                self.chip.NRZ25[lane].Reg0100_007A_2_0 = 0x4

            else:
                self.chip.NRZ25[lane].Reg0100_007A_12 = 0
                self.chip.NRZ25[lane].Reg0100_007A_11_10 = 0
                self.chip.NRZ25[lane].Reg0100_007A_9_8 = 0
                self.chip.NRZ25[lane].Reg0100_007A_7_5 = 0x5
                self.chip.NRZ25[lane].Reg0100_007A_4_3 = 0x0
                self.chip.NRZ25[lane].Reg0100_007A_2_0 = 0x4

        if enable == 1:
            self.chip.NRZ25[lane].TX_PI_EN = enable

    @deco_AllLanes
    def tx_rx_serial_loopback(self, lane, enable=1, **kw):
        """ if you want to enable tx_rx loopback, you need to connect another lane's rx to
            test lane's tx as a termination
        """
        if self.mode == 'nrz':
            if enable == 1:
                self.chip.NRZ25[lane].tx_taps(0, 0, 15, -4, 0)
                self.chip.NRZ25[lane].Reg01E7_13 = 1
                self.chip.NRZ25[lane].Reg00FF_3 = 1
                self.chip.NRZ25[lane].Reg0100_0001_14 = 0
                time.sleep(3)
                self.chip.GROUP8_TOP[0].fw_reg(self.chip, addr=0x8, data=0xffff-2**lane) # inactive FW
                self.chip.MdioWr(0x10b + 0x800 * lane, 0x0)
                self.chip.NRZ25[lane].NRZ_SM_CONT = 0
                self.chip.NRZ25[lane].NRZ_SM_CONT = 1
                self.chip.NRZ25[lane].agcgain(0, 0)
                self.chip.NRZ25[lane].ctle_nrz(7)
                time.sleep(0.1)
                self.chip.NRZ25[lane].lane_reset()

            else:
                ########################################################################################################################
                # After you enable the loopback, if you want to active the FW, you need to set the enable=0 to active the FW
                ########################################################################################################################
                self.chip.NRZ25[lane].Reg01E7_13 = 0
                self.chip.NRZ25[lane].Reg00FF_3 = 0
                self.chip.NRZ25[lane].Reg0100_0001_14 = 1
                self.chip.GROUP8_TOP[0].fw_reg(self.chip, addr=0x8, data=0xffff) # active FW
                self.chip.NRZ25[lane].tx_taps(0, -8, 17, 0, 0)
        else:
            print "\n>>>> TX-to-RX Serial Loopback feature is available in NRZ mode Only!\n"

    @deco_AllLanes
    def set_external_digital_loopback(self, lane, t=0.1, pat_gen=3, pat_chk=3, **kw):

        if self.mode == 'pam4':
            # In PAM4 mode we need to use divide by 2 for TX PLL
            self.chip.PAM50[lane].TX_PLL_N = 42
            self.chip.PAM50[lane].Reg00FF_1 = 0
            self.chip.PAM50[lane].RX_REFCLK_DIV4_EN = 0
            self.chip.PAM50[lane].Reg00D7_13 = 1
            self.chip.PAM50[lane].Reg00D7_15_14 = 2
            self.chip.PAM50[lane].Reg00D9_3_0S = 0x80000

        self.chip.MdioWr(0x4107+lane*0x20, (0x1A00&0xFCFF)|(pat_gen<<8))
        self.chip.MdioWr(0x410C+lane*0x20, (0x3201&0xE7FF)|(pat_chk<<11))
        self.chip.MdioWr(0x4107+lane*0x20, (0x1E00&0xFCFF)|(pat_gen<<8))  #turn on PRBS gen
        time.sleep(t)
        self.chip.MdioWr(0x410C+lane*0x20, (0x3001&0xE7FF)|(pat_chk<<11))  #turn off PRBS checker
        self.chip.MdioWr(0x4107+lane*0x20, (0x0A00&0xFCFF)|(pat_gen<<8))  #turn off PRBS gen

    @deco_AllLanes
    def check_external_digital_loopback(self, lane, **kw):
        PRBS_check = self.chip.MdioRd(0x4102+lane*0x20)
        if PRBS_check != 0x8FFF:
            print 'PRBS_check = ', hex(PRBS_check)
            print 'External loopback with fault'
        err_cycle_1 = self.chip.MdioRd(0x4113+lane*0x20)
        err_cycle_2 = self.chip.MdioRd(0x4114+lane*0x20)
        if (err_cycle_1 != 0) or (err_cycle_2 != 0):
            print 'Cycle number of last error is', hex((err_cycle_1<<16) + err_cycle_2)

    #######################################################################################################################
    # bfn_configure_group4_tx_rx_serial_loopback
    #
    # Configure all lanes, in all groups, in all tiles in TX->RX serial loopback mode
    #
    def bfn_configure_group4_tx_rx_serial_loopback(working_tiles):
        for tile in working_tiles:
            chip.setTileAddr(tile)
            for g in range(8,9):
                chip.setPhyAddr(g)
                for ln in range(0,4):
                    bfn_tx_rx_serial_loopback(mode='nrz', lane=ln, enable=1, phase=0)

        time.sleep(3.0)

        for tile in working_tiles:
            chip.setTileAddr(tile)
            for g in range(8,9):
                chip.setPhyAddr(g)
                for ln in range(0,4):
                    bfn_tx_rx_serial_loopback(mode='nrz', lane=ln, enable=1, phase=1)

        time.sleep(0.1)

        for tile in working_tiles:
            chip.setTileAddr(tile)
            for g in range(8,9):
                chip.setPhyAddr(g)
                for ln in range(0,4):
                    bfn_tx_rx_serial_loopback(mode='nrz', lane=ln, enable=1, phase=2)


    #
    # Power on/off, Drive on/off etc...
    #
    @deco_AllLanes
    def enable_tx_driver(self, lane, en=1, **kw):
        """
        en: 1(enable) or 0
        """
        data = self.chip.MdioRd(0xFF | (lane<<11) )
        data = data & 0xF7FF
        data = data | (en << 11)
        self.chip.MdioWr((0xFF | (lane<<11)), data)


    #
    # SJ inject (experimental)
    #
    def dec2bin(self, x):
        """decimal to int"""
        x -= int(x)
        bins = []
        for i in range(8):
            x *= 2
            bins.append(1 if x>=1. else 0)
            x -= int(x)
            # print bins
        value = 0
        for a in range(8):
            value = value+ bins[7-a]*pow(2,a)
        return value

    def _mdiobitwr_lane(self, lane, reg, val, field):
        return self.chip.MdioBitWr( reg + (lane<<11), val, field)

    @deco_AllLanes
    def inject_sj_en(self, lane, en=1, **kw):
        if en:
            self._mdiobitwr_lane(lane,0xf3,0,( 5,5)) # enable tx_rotator to inject SJ
        else:
            self._mdiobitwr_lane(lane,0xf3,1,( 5,5)) # disable tx_rotator
            self._mdiobitwr_lane(lane,0xf9,0,(15,5)) # sj_ampl_0  = 0
            self._mdiobitwr_lane(lane,0xf8,0,(15,5)) # sj_ampl_1  = 0
            self._mdiobitwr_lane(lane,0xf7,0,(15,5)) # sj_ampl_2  = 0
            self._mdiobitwr_lane(lane,0xf6,0,(15,5)) # sj_ampl_3  = 0
            self._mdiobitwr_lane(lane,0xf5,0,(15,2)) # sj_freq    = 0

    @deco_AllLanes
    def inject_sj_ampl( self, lane, A = 3, **kw ):
        """SJ amp
        A:
        """
        addr = [0xf9,0xf8,0xf7,0xf6] # sj_ampl_0/1/2/3
        #           sj_ampl_cos_0,  sj_ampl_cos_1,         sj_ampl_cos_2,         sj_ampl_cos_3
        phase_value =[math.cos(0), math.cos((math.pi)/8), math.cos((math.pi)/4), math.cos(3*(math.pi)/8)]
        for i in range(4):
            value = A*phase_value[i]
            int_A = int(value)
            float_A = self.dec2bin(value - int_A)
            self._mdiobitwr_lane(lane,addr[i],  int_A,(15,13))
            self._mdiobitwr_lane(lane,addr[i],float_A,(12, 5))

    @deco_AllLanes
    def inject_sj_freq( self, lane, f = 250, **kw):
        """SJ freq
        f: KHz
        """
        data_f = int(round((f/3.2),0))
        self._mdiobitwr_lane(lane, 0xf5, data_f, (15,2)) # sj_freq value

    @deco_AllLanes
    def inject_sj(self,lane, A = 0.3, f = 2500, **kw):#kHz
        self.inject_sj_en(lane, en=1, **kw)
        self.inject_sj_ampl(lane, A, **kw)
        self.inject_sj_freq(lane, f, **kw)

    def remap_tx_lane(self, lane, lane_map, group=None):
        """remap Tx physical lane to logical lane"""
        if group!=None:
            self.set_group(group)

        reg = { 0: {'reg':0x4000, 'field': (15, 12)},
                1: {'reg':0x4000, 'field': (11,  8)},
                2: {'reg':0x4000, 'field': ( 7,  4)},
                3: {'reg':0x4000, 'field': ( 3,  0)},
                4: {'reg':0x4001, 'field': (15, 12)},
                5: {'reg':0x4001, 'field': (11,  8)},
                6: {'reg':0x4001, 'field': ( 7,  4)},
                7: {'reg':0x4001, 'field': ( 3,  0)},}
        self.chip.MdioBitWr( reg[lane]['reg'], lane_map, reg[lane]['field'] )


    def remap_rx_lane(self, lane, lane_map, group=None):
        """remap Rx physical lane to logical lane"""
        if group!=None:
            self.set_group(group)

        reg = { 0: {'reg':0x4002, 'field': (15, 12)},
                1: {'reg':0x4002, 'field': (11,  8)},
                2: {'reg':0x4002, 'field': ( 7,  4)},
                3: {'reg':0x4002, 'field': ( 3,  0)},
                4: {'reg':0x4003, 'field': (15, 12)},
                5: {'reg':0x4003, 'field': (11,  8)},
                6: {'reg':0x4003, 'field': ( 7,  4)},
                7: {'reg':0x4003, 'field': ( 3,  0)},}
        self.chip.MdioBitWr( reg[lane]['reg'], lane_map, reg[lane]['field'] )

    def remap_newport(self):
        _tiles = range(4)
        _groups  = BlueJay.Groups
        _lanes = BlueJay.Lanes

        for tile in _tiles:
            self.set_tile(tile)
            for group in _groups:
                self.set_group(group)
                txmap = 0
                rxmap = 0
                for lane in range(_lanes[group]): # logical lane
                    tx = get_JB2QSFP_tx(tile, group, lane)
                    rx = get_JB2QSFP_rx(tile, group, lane)
                    # map physical lane to logical lane slot
                    txmap = (txmap << 4) | tx['lane']
                    rxmap = (rxmap << 4) | rx['lane']

                self.chip.MdioWr(0x4000, txmap>>16)
                self.chip.MdioWr(0x4001, txmap&0xffff)
                self.chip.MdioWr(0x4002, rxmap>>16)
                self.chip.MdioWr(0x4003, rxmap&0xffff)

                for lane in range(_lanes[group]): # logical lane
                    tx = get_JB2QSFP_tx(tile, group, lane)
                    rx = get_JB2QSFP_rx(tile, group, lane)
                    self.set_tx_pol(lane, inv=1 if tx['tx_inv']=='Y' else 0)
                    self.set_rx_pol(lane, inv=1 if rx['rx_inv']=='Y' else 0)

    def print_newport(self, port):

        if port not in range(1, 33):
            print "port must be between 1 to 32"
            return

        for lane in range(8):
            jb = get_QSFP2JB(port, lane)
            print "Port/lane={}/{}, Tile/group/Tx/Rx={}/{}/{}".format(port, lane,
                                                        jb['tile'], jb['oct'], jb['tx'], jb['rx'])


if __name__ == '__main__':

    parser = argparse.ArgumentParser()
    parser.add_argument("-m", "--mode",
                            help='pam4 or nrz',
                            type=str, default='nrz', choices=['nrz', 'pam4'])
    parser.add_argument("-n", "--conn", help='socket or usb', type=str, default='socket')
    parser.add_argument("-i", "--ip", help='IP address', type=str, default='10.11.20.66')
    parser.add_argument("-r", "--datarate", help='10, 25, 50', type=int, default=25)
    parser.add_argument("-c", "--config", help='BJ Config',action='store_true')
    args = parser.parse_args()


    group = range(9)
    print 'mode={}'.format(args.mode)
    print 'config={}'.format('True' if args.config else 'False')

    if args.conn=='socket':
        bj = BlueJay(mode=args.mode, datarate=args.datarate, conn={'mode':'socket', 'host':args.ip,
                                        'port':8000}, tile=0)
        if args.config:
            group = range(9)
            for i in range(4):
                bj.set_tile(i)
                for grp in group:
                    bj.reset_group(group=group)
                bj.config_blueJay( groups=group )

    elif args.conn=='usb':
        bj = BlueJay(mode=args.mode, datarate=args.datarate, conn={'mode':'usb'})
        if args.config:
            group = range(9)
            for grp in group:
                bj.reset_group(group=group)
            bj.config_blueJay( groups=group )

    elif args.conn=='pcie':
        bj = BlueJay(mode=args.mode, datarate=args.datarate, conn={'mode':'pcie'})
        if args.config:
            group = range(9)
            for grp in group:
                bj.reset_group(group=group)
            bj.config_blueJay( groups=group )

    def disconnect_all(bj_obj):
        if type(bj_obj)==list:
            for _bj in bj_obj:
                _bj.disconnect()
        else:
            bj_obj.disconnect()

    # # TEST
    # if args.config:
        # bj.config_blueJay(groups=[8])

    if args.mode=='pam4':
        bj.set_prbs(lane='tile', tx_pat='QPRBS13', rx_pat='QPRBS13')
    elif args.mode=='nrz':
        bj.set_prbs(lane='tile', tx_pat='PRBS31', rx_pat='PRBS31')
        # bj.set_prbs(lane='tile', tx_pat='PRBS9', rx_pat='PRBS9')
    bj.set_tx_eq(lane='tile', main=20, post1=0, pre1=-10)

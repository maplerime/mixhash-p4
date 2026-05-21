import platform
import os
import ctypes as c
import random

do_print = 0
last_rtnd_val = 0 
cur_grp = 0

import socket

HOST = '127.0.0.1'  # The server's hostname or IP address
#HOST = '10.12.80.76'  # The server's hostname or IP address
PORT =  9001        # The port used by the server

s = 0

def connect_to_fpga_server():
    s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    s.connect((HOST, PORT))
    return s
#global wr_cnt; wr_cnt = 0
#global rd_cnt; rd_cnt = 0

class CredoUsbDongle:
    maxDeviceCount = 4

    def __init__(self, lib_name_linux, lib_name_window, phy_addr=0, dev_addr=1, shared_lib_path=""):
        self.cr_phy_addr = phy_addr
        self.cr_dev_addr = dev_addr

        if platform.system() == 'Linux':
            shared_lib_name = lib_name_linux
        elif platform.system() == 'Windows':
            shared_lib_name = lib_name_window
        else:
            print 'Your platform is unsupported!'
            exit(-1)

        shared_lib_path = os.path.abspath(shared_lib_path)
        self.lib_path_name = os.path.join(shared_lib_path, shared_lib_name)

        self.lib = c.CDLL(self.lib_path_name)

    #mdio, 0-mdio, 1-I2C
    def CredoUsbDongleOpen(self, usb_sel, phy_addr = None, dev_addr = None, mdio = 0):
        if phy_addr is None:
            phy_addr = self.cr_phy_addr
        if dev_addr is None:
            dev_addr = self.cr_dev_addr
        for i in range(5):
            ret = self.lib.cr_mdio_init(c.c_ubyte(phy_addr), c.c_ubyte(dev_addr), c.c_int(usb_sel), c.c_int(mdio))
            if ret != 0:
                self.lib.cr_mdio_close()
            else:
                port_index = usb_sel
                return port_index
            self.lib.cr_wait(10)
        raise IOError("MDIO Open Error!")

    def CredoUsbDogleOpenFirstValid(self, phy_addr = None, dev_addr = None, mdio = 0):
        if phy_addr is None:
            phy_addr = self.cr_phy_addr
        if dev_addr is None:
            dev_addr = self.cr_dev_addr

        deviceCounts = self.lib.cr_mdio_get_device_number()
        if deviceCounts > self.maxDeviceCount:
            deviceCounts = self.maxDeviceCount

        for i in range (3):
            for j in range(deviceCounts):
                ret = self.lib.cr_mdio_init(c.c_ubyte(phy_addr), c.c_ubyte(dev_addr), c.c_int(j), c.c_int(mdio))
                if ret == 0:
                    port_index = j
                    return port_index
            self.lib.cr_wait(10)
        raise IOError("MDIO Open Error!")

    def CredoUsbDongleGetDeviceNumber(self):
        deviceCounts = self.lib.cr_mdio_get_device_number()
        if deviceCounts > self.maxDeviceCount:
            deviceCounts = self.maxDeviceCount
        return deviceCounts


    # def CredoUsbDongleOpen(self, usb_sel, phy_addr = None, dev_addr = None):
    #     if phy_addr is None:
    #         phy_addr = self.cr_phy_addr
    #     if dev_addr is None:
    #         dev_addr = self.cr_dev_addr
    #     for i in range(5):
    #         ret = self.lib.cr_mdio_init(c.c_ubyte(phy_addr), c.c_ubyte(dev_addr), c.c_int(usb_sel))
    #         if ret != 0:
    #             self.lib.cr_mdio_close()
    #             ret = self.lib.cr_mdio_init(c.c_ubyte(phy_addr), c.c_ubyte(dev_addr), c.c_int(1))
    #             if ret != 0:
    #                 self.lib.cr_mdio_close()
    #             else:
    #                 port_index = 1
    #                 return port_index
    #         else:
    #             port_index = 0
    #             return port_index
    #         self.lib.cr_wait(10)
    #     raise IOError("MDIO Open Error!")

    # def listDeviceWithLocationId(self):
    #     locId = c.c_long * 16
    #     locId_result = locId()
    #     self.lib.cr_mdio_listDevice(locId_result)
    #     A = map(lambda x: x, locId_result)
    #     print A
    #     return A

    def isUsbDongleValid(self):
        nValid = self.lib.isDeviceHadleValid()
        if nValid == 1:
            return True
        return False

    def CredoUsbDongleMdioRead(self, reg):
        data = self.lib.cr_mdio_read(c.c_ushort(reg))
        return data  # FIXME: may have issue

    def CredoUsbDongleMdioWrite(self, reg, data):
        ret = self.lib.cr_mdio_write(c.c_ushort(reg), c.c_ushort(data))
        if ret != 0:
            raise IOError("MDIO Write Error!")

    def CredoUsbDongleClose(self):
        ret = self.lib.cr_mdio_close()
        if ret != 0:
            raise IOError("MDIO Close Error!")

class CredoNdUsbDongle:
    def __init__(self, lib_name_linux, lib_name_window, phy_addr=0, dev_addr=1, shared_lib_path=""):
        self.cr_phy_addr = phy_addr
        self.cr_dev_addr = dev_addr

        if platform.system() == 'Linux':
            shared_lib_name = lib_name_linux
        elif platform.system() == 'Windows':
            shared_lib_name = lib_name_window
        else:
            print 'Your platform is unsupported!'
            exit(-1)

        shared_lib_path = os.path.abspath(shared_lib_path)

        self.lib = c.CDLL(os.path.join(shared_lib_path, shared_lib_name))

    def CredoNdUsbDongleOpen(self):
        ret = self.lib.CredoNdUsbDongleOpen(c.c_ubyte(self.cr_phy_addr), c.c_ubyte(self.cr_dev_addr))
        if ret != 0:
            raise Exception("Open Failed")  # XXX

    def CredoNdUsbDongleMdioRead(self, reg):
        data = self.lib.CredoNdUsbDongleMdioRead(c.c_ushort(reg))
        return data  # FIXME: may have issue

    def CredoNdUsbDongleMdioWrite(self, reg, data):
        ret = self.lib.CredoNdUsbDongleMdioWrite(c.c_ushort(reg), c.c_ushort(data))
        if ret != 0:
            raise Exception("Write Failed")  # XXX

    def CredoNdUsbDongleClose(self):
        self.lib.CredoNdUsbDongleClose()

class LibBase(object):
    lib_name_linux = ""
    lib_name_window = ""

    def __init__(self, shared_lib_path=""):
        # TODO: support 2 usb dongle
        if platform.system() == 'Linux' and len(self.lib_name_linux) <= 0:
            print("no lib path") #raise Exception("Need correct lib path")
        elif platform.system() == 'Windows' and len(self.lib_name_window) <= 0:
            raise Exception("Need correct lib path")
        #self.usb = CredoUsbDongle(self.lib_name_linux, self.lib_name_window,
        #        shared_lib_path=shared_lib_path)

        # Firmware communicate default registers
        self.reg_cmd = 0x9815
        self.reg_cmd_detail = 0x9816
        self.reg_reg_value = 0x9812

    def getLibPathName(self):
        return self.usb.lib_path_name

    def isDebug(self):
        result = self.usb.lib.isDebug()
        if result == 1:
            return True
        else:
            return False

    # def listDeviceWithLocationId(self):
    #     return self.usb.listDeviceWithLocationId()

    # def connectFirstValid(self):
    #     self.connected = False
    #     port_index = self.usb.CredoUsbDogleOpenFirstValid()
    #     self.connected = True
    #     return port_index
    #
    # #mdio, True--mdio, False- I2C
    # #I2C, 1-I2C, 2-MCU
    # def connect(self, phy_addr = None, dev_addr = None, usb_sel = 0, mdio = True, I2C = 1):
    #     self.connected = False
    #     if mdio:
    #         port_index = self.usb.CredoUsbDongleOpen(usb_sel, phy_addr, dev_addr, 0)
    #     else:
    #         port_index = self.usb.CredoUsbDongleOpen(usb_sel, phy_addr, dev_addr, I2C)
    #     self.connected = True
    #     return port_index

            # mode: 0-mdio, 1-I2C, 2-MCU, 3-jtag

    def connectFirstValid(self, mode=0):
        self.connected = False
        port_index = self.usb.CredoUsbDogleOpenFirstValid(mode=mode)
        self.connected = True
        return port_index

    # mode: 0-mdio, 1-I2C, 2-MCU, 3-jtag,
    # mdio: True, mdio, False, other mode 1,2,3
    def connect(self, phy_addr=None, dev_addr=None, usb_sel=0, mdio=True, mode=1):
        self.connected = False
        #if mdio:
        #    port_index = self.usb.CredoUsbDongleOpen(usb_sel, phy_addr, dev_addr, 0)
        #else:
        #    print("connect mode: " + str(mode)) #port_index = self.usb.CredoUsbDongleOpen(usb_sel, phy_addr, dev_addr, mode)
        #    port_index = 1 # added
        port_index = 1 # added

        print("*** Connecting *** ")
        s = connect_to_fpga_server()
        self.s = s
        print("*** Connected  *** ")

        self.connected = True

        return port_index

    def disconnect(self):
        self.connected = False
        #self.usb.CredoUsbDongleClose()

    def isConnected(self):
        return self.connected

    def getDeviceNumber(self):
        return self.usb.CredoUsbDongleGetDeviceNumber()

    def isDeviceValid(self):
        return self.usb.isUsbDongleValid()

    def MdioRd(self, reg):
        global last_rtnd_val

        reg_str = "{0:#0{1}x}".format(reg,6)
        string = "R:" + reg_str + ":0x0000"
        #print("string= " + string)
        #print("<enc> = " + str(string.encode('ascii')))
        self.s.send(string.encode('ascii'))
        rtn_bytes = self.s.recv(15)
        rtn_string = rtn_bytes.decode('ascii')
        #print("rtn_string= %s\n", rtn_string)
        ret_list = rtn_string.split(':',3)
        #print("ret_list=" + ret_list[0] + " : " + ret_list[1] + " : " + ret_list[2] )
        ret_val = int(ret_list[2],base=16)

        return ret_val

    def MdioWr(self, reg, val):
        # return self.usb.CredoNdUsbDongleMdioWrite(reg, val)
        #return self.usb.CredoUsbDongleMdioWrite(reg, val)
        if do_print: print("MdioWr: reg= " + str(hex(reg)) + " : " + str(hex(val)))

        reg_str = "{0:#0{1}x}".format(reg,6)
        data_str = "{0:#0{1}x}".format(val,6)
        string = "W:" + reg_str + ":" + data_str
        #print("string= " + string)
        #print("<enc> = " + str(string.encode('ascii')))
        self.s.send(string.encode('ascii'))
        rtn_bytes = self.s.recv(15)
        rtn_string = rtn_bytes.decode('ascii')
        #print("%s\n", rtn_string)
        return 0

    def setPhyAddr(self, phyAddr):
        if do_print: print("setPhyAddr: grp= " + str(hex(phyAddr)))

        cur_grp = phyAddr

        grp_str = "{0:#0{1}x}".format(phyAddr,6)
        string = "G:" + grp_str + ":0x0000"
        self.s.send(string.encode('ascii'))
        rtn_bytes = self.s.recv(15)
        rtn_string = rtn_bytes.decode('ascii')
        #print("%s\n", rtn_string)

    def getPhyAddr(self):
        return cur_grp

    def setTileAddr(self, tileAddr):
        if do_print: print("setTileAddr: tile= " + str(hex(tileAddr)))

        tile_str = "{0:#0{1}x}".format(tileAddr,6)
        string = "T:" + tile_str + ":0x0000"
        self.s.send(string.encode('ascii'))
        rtn_bytes = self.s.recv(15)
        rtn_string = rtn_bytes.decode('ascii')
        #print("%s\n", rtn_string)

    def setClkDivMdio(self, clkDivMdio):
        self.usb.lib.setClkDivMdio(c.c_int64(clkDivMdio))
        
    def rreg(addr, lane = None, lane_offset=None, lane_name_list=None):
    #read from a register address, register offset or register field
        #global chip
        if lane==None: lane=gLane
                        
        #lane_mode_list
        if type(addr) == int:
            if (addr & 0xf000) == 0: addr += lane_offset[lane_name_list[lane]]
            return self.MdioRd(addr)
        elif type(addr) == list:
            val = 0
            i = 0
            while(i  < (len(addr)-1)):
                addr_1 = addr[i]
                if (addr[i] & 0xf000) == 0: addr_1 += lane_offset[lane_name_list[lane]]            
                val_tmp = self.MdioRd(addr_1)
                i += 1
                mask = sum([1<<bit for bit in range(addr[i][0], addr[i][-1]-1, -1)])
                val_tmp = (val_tmp & mask)>>addr[i][-1]
                val = (val<<(addr[i][0]-addr[i][-1]+1)) + val_tmp
                i += 1
            return val
        else:
            print("\n***Error reading register***")
            return -1

## socket stuff

class Mdio_Lib(LibBase):
    def __init__(self, lib_name_window='../Device/mdio.dll'):
        Mdio_Lib.lib_name_window = lib_name_window

        super(Mdio_Lib, self).__init__()
        

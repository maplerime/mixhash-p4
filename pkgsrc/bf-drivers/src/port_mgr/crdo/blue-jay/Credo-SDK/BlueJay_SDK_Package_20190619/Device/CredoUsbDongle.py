import platform
import os
import ctypes as c

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
            raise Exception("Need correct lib path")
        elif platform.system() == 'Windows' and len(self.lib_name_window) <= 0:
            raise Exception("Need correct lib path")
        self.usb = CredoUsbDongle(self.lib_name_linux, self.lib_name_window,
                shared_lib_path=shared_lib_path)

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
        if mdio:
            port_index = self.usb.CredoUsbDongleOpen(usb_sel, phy_addr, dev_addr, 0)
        else:
            port_index = self.usb.CredoUsbDongleOpen(usb_sel, phy_addr, dev_addr, mode)
        self.connected = True
        return port_index

    def disconnect(self):
        self.connected = False
        self.usb.CredoUsbDongleClose()

    def isConnected(self):
        return self.connected

    def getDeviceNumber(self):
        return self.usb.CredoUsbDongleGetDeviceNumber()

    def isDeviceValid(self):
        return self.usb.isUsbDongleValid()

    def MdioRd(self, reg):
        # return self.usb.CredoNdUsbDongleMdioRead(reg)
        return self.usb.CredoUsbDongleMdioRead(reg)

    def MdioWr(self, reg, val):
        # return self.usb.CredoNdUsbDongleMdioWrite(reg, val)
        return self.usb.CredoUsbDongleMdioWrite(reg, val)

    def setPhyAddr(self, phyAddr):
        self.phyAddr = phyAddr
        self.usb.lib.cr_mdio_setPhyAddr(c.c_ubyte(phyAddr))

    def getPhyAddr(self):
        return self.phyAddr

    def setClkDivMdio(self, clkDivMdio):
        self.usb.lib.setClkDivMdio(c.c_int64(clkDivMdio))


class Mdio_Lib(LibBase):
    def __init__(self, lib_name_window='../Device/mdio.dll'):
        Mdio_Lib.lib_name_window = lib_name_window
        super(Mdio_Lib, self).__init__()

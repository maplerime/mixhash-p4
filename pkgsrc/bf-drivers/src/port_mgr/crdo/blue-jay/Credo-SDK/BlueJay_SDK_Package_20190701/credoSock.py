import sys,os
import socket
import time

from Device.CredoUsbDongle import Mdio_Lib 
from credoSdk import _SDK_path
import newportPCIeI2C

BufSize = 1024
class Mdio_sock(Mdio_Lib):
    """Implement Socket client interface"""
 
    def __init__(self, mode, host=None, port=None, tile=None):
        """Create connection
        mode: 'socket', 'usb'
        """
        self.mode = mode
        if mode == 'usb':
            _mdio_dll = os.path.join( _SDK_path, 'Device', 'mdio.dll')
            super(Mdio_sock, self).__init__(lib_name_window=_mdio_dll)
            self.tile = 0
        elif mode == 'socket':
            self.host = host
            self.port = port
            self.tile = tile
            print 'host={}, port={}, tile={}'.format(self.host, self.port, self.tile)
        else:
            self.tile = 0
        
        # initialize internal variable
        self.group = 0
        
        
    def connect(self):
        if self.mode=='socket':
            self.client_sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
            self.client_sock.connect((self.host, self.port))
            self.client_sock.settimeout(3.0)
            self.connected = True
            return 0
        elif self.mode=='usb':
            return super(Mdio_sock, self).connect(mdio=False,mode=3)
    
    def disconnect(self):
        if self.mode=='socket':
            self.client_sock.sendall('bye')
            self.client_sock.recv(BufSize)
            self.client_sock.close()
            self.client_sock = None
            self.connected = False
        elif self.mode=='usb':
            super(Mdio_sock, self).disconnect() 
    
    def MdioRd(self, reg, tile=None):
        if self.mode=='socket':
            if tile!=None:
                self.tile=tile
            cmd = 'rd,{:d},{:d}'.format(reg,self.tile)
            self.client_sock.sendall(cmd)
            data = self.client_sock.recv(BufSize)
            if data.isdigit():
                data = int(data, 10)
            return data
        elif self.mode=='usb':
            return super(Mdio_sock, self).MdioRd(reg) 
    
    def MdioWr(self, reg, val, tile=None):
        if self.mode=='socket':
            if tile!=None:
                self.tile=tile
            cmd = 'wr,{:d},{:d},{:d}'.format(reg,val,self.tile)
            # time_start = time.time()
            self.client_sock.sendall(cmd)
            # time_sent = time.time()
            data = self.client_sock.recv(BufSize)
            # time_recv = time.time()
            # print "sent={:1.5f}, recv={:1.5f}".format(time_sent-time_start, time_recv-time_sent) 
            if data.isdigit():
                data = int(data, 10)
            return data
        elif self.mode=='usb':
            return super(Mdio_sock, self).MdioWr(reg,val) 
            
            
    def MdioBitWr(self, reg, val, field, tile=None):
        """Perform read-modify-write
        field: (msb, lsb)
        """
        if type(field)==int:
            field=(field, field)
        msb = field[0]
        lsb = field[1]
        
        mask = (1<<(msb+1))-(1<<lsb)
        mask_b = 0xffff-mask
        rd = self.MdioRd(reg, tile) & mask_b
        val = ((val<<lsb) & mask) + rd
        return self.MdioWr(reg, val, tile)
        
    def MdioBitRd(self, reg, field, tile=None):
        """bit-field read
        field: (msb, lsb)
        """
        if type(field)==int:
            field=(field, field)
        msb = field[0]
        lsb = field[1]
        
        mask = (1<<(msb+1))-(1<<lsb)
        rd = self.MdioRd(reg, tile) & mask
        rd >>= lsb
        return rd
        
    
    def setPhyAddr(self, phyAddr=None, tile=None):
        if phyAddr == None:
            return self.group
            
        self.group = phyAddr
        if self.mode=='socket':
            if tile!=None:
                self.tile = tile
            cmd = 'group,{:d},{:d}'.format(phyAddr,self.tile)
            self.client_sock.sendall(cmd)
            data = self.client_sock.recv(BufSize)
            if data.isdigit():
                data = int(data, 10)
            return data
        elif self.mode=='usb':
            return super(Mdio_sock, self).setPhyAddr(phyAddr) 
            
    def resetMdio(self, tile=None):
        if tile!=None:
            self.tile = tile
        cmd = 'reset_mdio,{:d}'.format(self.tile)
        self.client_sock.sendall(cmd)
        data = self.client_sock.recv(BufSize)
    
    def resetJB(self, tile=None):
        if tile!=None:
            self.tile = tile
        cmd = 'reset_jb,{:d}'.format(self.tile)
        self.client_sock.sendall(cmd)
        data = self.client_sock.recv(BufSize)
    
    def setTileAddr(self, tile):
        self.tile = tile


class Mdio_pcie(Mdio_sock):
    """Implement Socket client interface"""
 
    def __init__(self, mode, host=None, port=None, tile=None):
        """Create connection
        mode: 'socket', 'usb'
        """
        
        super(Mdio_pcie, self).__init__(mode=mode, host=host, port=port, tile=tile)
        if mode == 'pcie':
            self.mode = mode
            newportPCIeI2C.init()
            self.bj = newportPCIeI2C.BlueJay()
        else:
            print "Only PCIe-FPGA mode is supported by this class"
        
    def connect(self):
        self.bj.update_mdio_clkdiv(30, tile=0)
        self.bj.update_mdio_clkdiv(30, tile=1)
        self.bj.update_mdio_clkdiv(30, tile=2)
        self.bj.update_mdio_clkdiv(30, tile=3)
        return 0
        
    def disconnect(self):
        return 0
    
    def MdioRd(self, reg, tile=None):
        if tile!=None:
            self.tile = tile
        return self.bj.mdio_read(reg, tile=self.tile)
    
    def MdioWr(self, reg, val, tile=None):
        if tile!=None:
            self.tile = tile
        return self.bj.mdio_write(reg, val, tile=self.tile) 
            
    def MdioBitWr(self, reg, val, field, tile=None):
        """Perform read-modify-write
        field: (msb, lsb)
        """
        if type(field)==int:
            field=(field, field)
        msb = field[0]
        lsb = field[1]
        
        mask = (1<<(msb+1))-(1<<lsb)
        mask_b = 0xffff-mask
        rd = self.MdioRd(reg, tile) & mask_b
        val = ((val<<lsb) & mask) + rd
        return self.MdioWr(reg, val, tile)
        
    def MdioBitRd(self, reg, field, tile=None):
        """bit-field read
        field: (msb, lsb)
        """
        if type(field)==int:
            field=(field, field)
        msb = field[0]
        lsb = field[1]
        
        mask = (1<<(msb+1))-(1<<lsb)
        rd = self.MdioRd(reg, tile) & mask
        rd >>= lsb
        return rd
    
    def setPhyAddr(self, phyAddr=None):
        if phyAddr == None:
            return self.group
            
        self.group = phyAddr
        self.bj.set_group(phyAddr, self.tile)
        
    def resetMdio(self, tile=None):
        if tile!=None:
            self.tile = tile
        self.bj.mdio_reset(self.tile)
    
    def resetJB(self, tile=None):
        if tile!=None:
            self.tile = tile
        self.bj.jb_reset(self.tile)
    
    def setTileAddr(self, tile):
        self.tile = tile



   
if __name__ == '__main__':
    mdio = Mdio_sock( '10.11.20.50', 8000, 0 )
    
        
    

import os
import mmap
import subprocess
import shlex
import time

# _VendorID = "1D1C"
# _DeviceID = "01F0"
# _PCIeResource = r"./pcimem /sys/bus/pci/devices/0000\:06\:00.0/resource0"

# 06:00.0 Memory controller: Device 1d1c:01f0
        # Subsystem: Device 1d1c:01f0
        # Control: I/O- Mem+ BusMaster- SpecCycle- MemWINV- VGASnoop- ParErr- Stepping- SERR- FastB2B- DisINTx-
        # Status: Cap+ 66MHz- UDF- FastB2B- ParErr- DEVSEL=fast >TAbort- <TAbort- <MAbort- >SERR- <PERR- INTx-
        # Interrupt: pin A routed to IRQ 11
        # Region 0: Memory at fbc00000 (32-bit, non-prefetchable) [size=256K]
        # Capabilities: <access denied>
        
_DEBUG_PCIEMEM= False

class PcieAccess():
    def __init__(self, dev='1d1c'):
        self.memp = self._pcie_open(dev)
    
    def get_pcie_offset(self, dev):
        cmd = "lspci -vvv -d {:s}:".format(dev)
        cmd = shlex.split(cmd)
        p = subprocess.Popen(cmd, stdout=subprocess.PIPE, stderr=subprocess.PIPE, shell=False )
        o,e = p.communicate()
        # print o
        
        words = o.replace('\n', ' ').replace('\t', ' ').split()
        mem_words = ["Region", "0:", "Memory", "at"]
        offset = ''
        for i, w in enumerate(words):
            if w == mem_words[0]:
                if words[i+1:i+4] == mem_words[1:4]: 
                    offset = words[i+4]
                    print "PCIe offset={}".format(offset)
                    return int(offset,16)
        else:
            return -1
            
    def _pcie_open(self, dev='1d1c'):
        f = os.open('/dev/mem', os.O_RDWR | os.O_SYNC )
        offset = self.get_pcie_offset(dev)
        m = mmap.mmap(f, 256*1024, prot=mmap.PROT_READ | mmap.PROT_WRITE, offset=offset)
        return m

    def close(self):
        self.memp.close()
        
    def read(self, addr):    
        if _DEBUG_PCIEMEM:
            start_time = time.time()
        
        self.memp.seek(addr)
        rd = self.memp.read(4)
        rd_i = 0
        for i in range(4):
            rd_i += (ord(rd[i]) << (i*8))
        
        if _DEBUG_PCIEMEM:
            etime = time.time() - start_time
            print "elapsed time: {:1.5f} s".format(etime)
        
        return rd_i

    def write(self, addr, val):
        if _DEBUG_PCIEMEM:
            start_time = time.time()
        self.memp.seek(addr)
        v = [val&0xff, (val>>8)&0xff, (val>>16)&0xff, (val>>24)&0xff]
        self.memp.write(chr(v[0])+chr(v[1])+chr(v[2])+chr(v[3]) )
        # self.memp.flush()
        if _DEBUG_PCIEMEM:
            etime = time.time() - start_time
            print "elapsed time: {:1.5f} s".format(etime)
        return 0
        
PciMem = PcieAccess()

if __name__ == '__main__':
    def read(port, addr):
        rd = PciMem.read( (port << 12) + addr )
        return rd
    
    def write(port, addr, val):
        return PciMem.write( (port << 12) + addr, val )
    import os
import mmap
import subprocess
import shlex
import time

# _VendorID = "1D1C"
# _DeviceID = "01F0"
# _PCIeResource = r"./pcimem /sys/bus/pci/devices/0000\:06\:00.0/resource0"

# 06:00.0 Memory controller: Device 1d1c:01f0
        # Subsystem: Device 1d1c:01f0
        # Control: I/O- Mem+ BusMaster- SpecCycle- MemWINV- VGASnoop- ParErr- Stepping- SERR- FastB2B- DisINTx-
        # Status: Cap+ 66MHz- UDF- FastB2B- ParErr- DEVSEL=fast >TAbort- <TAbort- <MAbort- >SERR- <PERR- INTx-
        # Interrupt: pin A routed to IRQ 11
        # Region 0: Memory at fbc00000 (32-bit, non-prefetchable) [size=256K]
        # Capabilities: <access denied>
        
_DEBUG_PCIEMEM= False

class PcieAccess():
    def __init__(self, dev='1d1c'):
        self.memp = self._pcie_open(dev)
    
    def get_pcie_offset(self, dev):
        cmd = "lspci -vvv -d {:s}:".format(dev)
        cmd = shlex.split(cmd)
        p = subprocess.Popen(cmd, stdout=subprocess.PIPE, stderr=subprocess.PIPE, shell=False )
        o,e = p.communicate()
        # print o
        
        words = o.replace('\n', ' ').replace('\t', ' ').split()
        mem_words = ["Region", "0:", "Memory", "at"]
        offset = ''
        for i, w in enumerate(words):
            if w == mem_words[0]:
                if words[i+1:i+4] == mem_words[1:4]: 
                    offset = words[i+4]
                    print "PCIe offset={}".format(offset)
                    return int(offset,16)
        else:
            return -1
            
    def _pcie_open(self, dev='1d1c'):
        f = os.open('/dev/mem', os.O_RDWR | os.O_SYNC )
        offset = self.get_pcie_offset(dev)
        m = mmap.mmap(f, 256*1024, prot=mmap.PROT_READ | mmap.PROT_WRITE, offset=offset)
        return m

    def close(self):
        self.memp.close()
        
    def read(self, addr):    
        if _DEBUG_PCIEMEM:
            start_time = time.time()
        
        self.memp.seek(addr)
        rd = self.memp.read(4)
        rd_i = 0
        for i in range(4):
            rd_i += (ord(rd[i]) << (i*8))
        
        if _DEBUG_PCIEMEM:
            etime = time.time() - start_time
            print "elapsed time: {:1.5f} s".format(etime)
        
        return rd_i

    def write(self, addr, val):
        if _DEBUG_PCIEMEM:
            start_time = time.time()
        self.memp.seek(addr)
        v = [val&0xff, (val>>8)&0xff, (val>>16)&0xff, (val>>24)&0xff]
        self.memp.write(chr(v[0])+chr(v[1])+chr(v[2])+chr(v[3]) )
        # self.memp.flush()
        if _DEBUG_PCIEMEM:
            etime = time.time() - start_time
            print "elapsed time: {:1.5f} s".format(etime)
        return 0
        
PciMem = PcieAccess()

if __name__ == '__main__':
    def read(port, addr):
        rd = PciMem.read( (port << 12) + addr )
        return rd
    
    def write(port, addr, val):
        return PciMem.write( (port << 12) + addr, val )
    

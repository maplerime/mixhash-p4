"""
Utility tool to communicate with Xilinx FPGA through PCIe

Note:
PCIe register access requires admin privilege. Run python as Super User.


--- THIS IS NO LONGER NEEDED ---
How to use:
1. Run python with sudo
    > sudo python -i newportPCIeI2C.py -init
    Type admin password (admin right is needed to access PCIe driver)
    "-init" option is needed only first time run
2. You are ready to access QSFP modules through I2C bus.

Or when you want to run only one command, type command at shell prompt like below
> sudo python newportPCIeI2C.py -c "qsfp_read_all(0)"
 - This command reads register 0 from all ports
 - '-c' option takes a function that is defined in this file

Functions:
Hit <Tab> key at prompt like "qsfp_". You will see functions
>> qsfp_<TAB>

Examples:
To read QSFP 5 (0-31) register 0x12, page=3
>> qsfp_read(5, 0x12, page=3)

To write QSFP 5 (0-31) register 0x12, page=3, value=0xAB
>> qsfp_read(5, 0x12, 0xAB, page=3)

To read QSFP status "rxloss" of port 3
>> qsfp_status_read(3, 'rxloss')
1st parameter can be 'present_n', 'rxloss', 'lpmode', 'reset_n'

To write QSFP control "lpmode" of port 3 to 1
>> qsfp_ctrl_write(3, 'lpmode', 1)
1st parameter can be 'lpmode', 'reset_n'

"""
import os
import subprocess
import shlex
import time
import socket
import threading
import copy

from pcimem import PciMem

# Tab auto completion
import rlcompleter, readline
readline.parse_and_bind('tab: complete')

_DEBUG = True

_VendorID = "1D1C"
_DeviceID = "01F0"
_PCIeResource = r"./pcimem /sys/bus/pci/devices/0000\:06\:00.0/resource0"

#
# Command line foundation
#
def init():
    """ Enable PCIe device for read/write """
    cmd = "setpci -s 6:00.0 COMMAND=2"
    _runCmd( cmd )
    print "Init successful"

def _runCmd( cmd ):
    cmd_s = shlex.split(cmd)
    proc = subprocess.Popen( cmd_s, stdout=subprocess.PIPE, stderr=subprocess.PIPE, shell=False )
    (output, err) = proc.communicate()
    # print output
    return output

def findFPGA(raw=False):
    cmd = "lspci -vvv -d {}:".format(_VendorID)
    r = _runCmd( cmd )
    pcieKey = "06:00.0 Memory controller: Device 1d1c:01f0"
    if r.find( pcieKey ) >= 0:
        print "FPGA PCIe link is found!!"
        if raw==False:
            print pcieKey
        else:
            print r
        return True
    else:
        print "PCIe could not be found"
        return False
#
# Low level read/write access to PCIe device
#

def readMem( offset):
    rd = PciMem.read( offset<<2 )
    rd = ['', hex(rd)]
    return rd


def writeMem( offset, val):
    return PciMem.write( offset<<2, val )

# PCIe - I2C
#
#
class _I2CCMD:
    ADDR_WRITE = 0b000
    ADDR_READ  = 0b001
    ADDR       = 0b010
    READ       = 0b011
    WRITE_READ = 0b100
    RESET      = 0b101
    SPEED      = 0 #  0=100Khz, 1=400Khz, 2=1MHz

class _I2CADDR:
    Ctrl    = 0
    Status  = 1
    Command = 4
    BigData = 128

class _NEWPORT:
    I2CMUX      = [0x70, 0x71, 0x72, 0x73]
    QSFP0_7     = range(0, 8)
    QSFP8_15    = range(8, 16)
    QSFP16_23   = range(16, 24)
    QSFP24_31   = range(24, 32)
    I2C_MAIN    = 32

#
# i2c commands
#
def _get_addr(port, offset):
    if port < 64 and offset < 1024:
        # return port << 12
        return (port << 10) + offset
    else:
        return None

def i2c_set_ctrl(port, val):
    writeMem( _get_addr(port, _I2CADDR.Ctrl), val)

def i2c_get_status(port):
    st = readMem( _get_addr(port, _I2CADDR.Status) )
    st = int(st[1],16)
    return st

def i2c_clr_status(port):
    writeMem( _get_addr(port, _I2CADDR.Status), 0 )

def i2c_start(port, wait=1.0):
    i2c_stop(port, wait)
    #clear status
    i2c_clr_status( port )
    i2c_set_ctrl(port, (_I2CCMD.SPEED << 16) + 1)
    # print _I2CCMD.SPEED

def i2c_stop(port, wait=1.0):
    i2c_set_ctrl(port, 0)
    for i in range( int(wait/0.1) + 1 ):
        status = i2c_get_status(port)
        if (status & 0x1) == 0: # Stopped
            break
        else:
            time.sleep(0.1)
    else:
        print "I2C didn't stop within wait time"

def i2c_get_cmdStatus(port):
    st = readMem( _get_addr(port, _I2CADDR.Status) )
    st = int(st[1],16)
    return st

def i2c_wait_done( port, wait=1.0 ):
    # wait for all commands are executed at least one time
    for i in range( int(wait/0.1)+1 ):
        status = i2c_get_status(port)
        if (status & 0x02) != 0:
            # print 'Status', status
            break
        time.sleep(0.1)
    else:
        print "Timeout"
        return -1
    return 0

def _i2c_read_data(port, id):
    """ read data from data memory """
    addr = _get_addr( port, id)
    rd0 = readMem(addr)
    rd0 = int(rd0[1],16)
    cmd = (rd0 >> 26) & 0xF

    # Does command contain read?
    if cmd in [_I2CCMD.ADDR_READ, _I2CCMD.READ, _I2CCMD.WRITE_READ]:
        addr = _get_addr( port, id + 1)
        rd1 = readMem(addr)
        rd1 = int(rd1[1],16)
        numByte = rd1 & 0xFF

        addr = _get_addr( port, id + 2)
        rd2 = readMem(addr)
        rd2 = int(rd2[1],16)
        datAdr = rd2 & 0xFF
    else:
        return None

    # did command complete successfully?
    addr = _get_addr( port, id)
    _start = time.time()
    while True:
        time.sleep(0.01)
        rd0 = readMem(addr)
        rd0 = int(rd0[1],16)
        sts = rd0 & 0x3f
        if (sts & 0b11) == 0b10: # [1] complete, [0]: running
            break

        elif (sts & 0b11) != 0b10: # [1] complete, [0]: running
            if time.time() - _start > 0.02*numByte:
                print "command could not completed"
                return None # error

    if (sts & 0b111100) != 0:
        if (sts >> 5 & 1): print "ERROR: command timeout"
        if (sts >> 4 & 1): print "ERROR: NACK after write"
        if (sts >> 3 & 1): print "ERROR: NACK after register address"
        if (sts >> 2 & 1): print "ERROR: NACK after device address"
        return None # error

    rdData = list()
    numDW = (numByte-1)/4

    if numByte <=8:
        adrStart = _get_addr(port, id + 2)
    else:
        adrStart = _get_addr(port, datAdr)

    for i in range(numDW+1):
        rd = readMem(adrStart + i)
        rdData.append(rd[1].strip())

    return rdData

def i2c_reset( port, id, i2cCmd=_I2CCMD.RESET):
    """reset i2c by toggling scl line"""

    # stop controller
    i2c_stop(port)

    # clear status
    i2c_clr_status(port)

    cmdAdr = _get_addr(port, id)
    cmd0  = (1<<31)  | ( i2cCmd << 26)
    cmd1  = 0
    writeMem( cmdAdr,   cmd0 )
    writeMem( cmdAdr+1, cmd1 )

    i2c_start(port)

def i2c_read( port, id, devAddr, regAddr, numByte=1, dataLoc=0x80, i2cCmd=_I2CCMD.ADDR_READ):
    """
    id: sequential command id (0,1,2...). Address is computed in this function.
    """
    # _start = time.time()

    cmdAdr = _get_addr(port, id)
    cmd0  = (1<<31)  | (i2cCmd << 26)
    cmd1  = (devAddr << 24) | (regAddr << 16) | numByte
    writeMem( cmdAdr,   cmd0 )
    writeMem( cmdAdr+1, cmd1 )
    if numByte>8:
        cmd2  = dataLoc
        writeMem( cmdAdr+2, cmd2)

    # Start command
    i2c_start(port)
    i2c_wait_done(port)
    rd = _i2c_read_data(port, id)
    # elapsetime = time.time() - _start
    # print "Elapsed time = {:1.3f} sec".format(elapsetime)
    return rd

def i2c_write( port, id, devAddr, regAddr, data, dataLoc=0x80):
    """data is list of 32-bit value"""

    if type(data) != list:
        data = [data]

    cmdAdr = _get_addr(port, id)
    cmd0  = (1<<31)  | (_I2CCMD.ADDR_WRITE << 26)
    cmd1  = (devAddr << 24) | (regAddr << 16) | (len(data) << 8)
    writeMem( cmdAdr,   cmd0 )
    writeMem( cmdAdr+1, cmd1 )

    # Write data
    if len(data) <= 8:
        for i, d in enumerate(data):
            writeMem( cmdAdr+2+i, d)
    else:
        for i, d in enumerate(data):
            writeMem( dataLoc+i, d)

    # Start command
    i2c_start(port)
    i2c_wait_done(port)

def i2c_writeAddr( port, id, devAddr, regAddr, i2cCmd=_I2CCMD.ADDR):
    cmdAdr = _get_addr(port, id )
    cmd0  = (1<<31)  | (i2cCmd << 26)
    cmd1  = (devAddr << 24) | (regAddr << 16)
    writeMem( cmdAdr,   cmd0 )
    writeMem( cmdAdr+1, cmd1 )

    # Start command
    i2c_start(port)
    i2c_wait_done(port)


def i2c_readData( port, id, devAddr, numByte=1):
    cmdAdr = _get_addr(port, id )
    cmd0  = (1<<31)  | (_I2CCMD.READ << 26)
    cmd1  = (devAddr << 24) | numByte
    writeMem( cmdAdr,   cmd0 )
    writeMem( cmdAdr+1, cmd1 )

    # Start command
    i2c_start(port)
    rd = _i2c_read_data(port, id)
    return rd

def clear_mem(port):
    """Clear all PCIe space memory"""
    i2c_stop(port)
    adr = _get_addr(port, 0)
    for i in range(adr, adr+1020): #1024-4
        writeMem(i, 0)

def get_fpga_rev(port=0):
    adr = _get_addr(port, 2)
    rd = readMem(adr)
    rd = rd[1].strip()
    
    #ddddd_MMMM_yyyyyy_hhhhh_mmmmmm_ssssss
    rev_format = ["ddddd","MMMM","yyyyyy","hhhhh","mmmmmm","ssssss"]
    rev_format.reverse()
    rd_i = int(rd, 16)
    ts=dict()
    for i in rev_format:
        mask = (2**len(i)) - 1
        t = rd_i & mask
        ts[i[0]] = t
        rd_i >>= len(i)
    print "20{:02d}-{:02d}-{:02d} {:02d}:{:02d}:{:02d}".format(ts['y'], ts['M'], ts['d'], 
                                     ts['h'], ts['m'], ts['s'])    
    return rd


#
# QSFP
#
def deco_QSFP(func):
    # "port" parameter must be at first position
    def do_all(*args, **kwargs):
        if len(args)==0:
            port = kwargs['port']
        else:
            port = args[0]

        if port in ['all']:
            ports = range(32)
        elif type(port)==list:
            ports = port
        elif type(port)==int:
            ports = [port]

        rtns = list()
        for p in ports:
            if len(args)==0:
                kwargs['port']=p
            else:
                _args = list(args)
                _args[0] = p
            # print args, _args, kwargs
            rtn = func(*_args, **kwargs)
            rtns.append(rtn)

        if type(port)==int:
            return rtns[0]
        else:
            return rtns
    return do_all


class _QSFP:
    DEVID       = 0x50
    PAGESEL     = 0x7F # Page select register address
    I2CMUXADDR  = 0x74
    I2CMUXPORT  = 32

@deco_QSFP
def qsfp_write(port, regAddr, value, page=None, id=4):
    """Write to QSFP module's register.
    port:0-31
    regAddr:8-bits
    value: 8-bits
    id: i2c controller memory ID. Default=4
    """
    if page:
        i2c_write(port, id, _QSFP.DEVID, _QSFP.PAGESEL, page)
    i2c_write(port, id, _QSFP.DEVID, regAddr, value)

@deco_QSFP
def qsfp_read(port, regAddr, numByte=1, page=None, id=4, dataLoc=0x80, i2cCmd=_I2CCMD.ADDR_READ):
    """Read from QSFP module's register.
    port:0-31
    regAddr:8-bits
    numByte: 1-127. If larger than 4, please specify data location id (default=0x80)
    id: i2c controller memory ID. Default=4
    """
    if page:
        i2c_write(port, id, _QSFP.DEVID, _QSFP.PAGESEL, page)
    rd = i2c_read(port, id, _QSFP.DEVID, regAddr, numByte, dataLoc, i2cCmd=i2cCmd)
    return rd

def qsfp_read_all(regAddr, numByte, page=None, label=None):
    """Read from all QSFP of 32 ports"""
    rd_lst = list()
    for i in range(32):
        rd = qsfp_read(i, regAddr, numByte=numByte, page=page)
        rd_lst.append(rd)

    print label
    for i in range(32):
        print "{:02d}: {}".format(i, ','.join(rd_lst[i]))

def qsfp_write_all(regAddr, value, page=None, label=None):
    """Write to all QSFP of 32 ports"""
    for i in range(32):
        rd = qsfp_write(i, regAddr, value, page=page)

@deco_QSFP
def qsfp_dd_lpbk_read_temp(port, temp=3, i2cCmd=_I2CCMD.ADDR_READ):
    """Read QSFP DD temperature"""
    temp_sense = {
        1: (None, 24),
        2: (3,   152),
        3: (None, 14),
        4: (3,   154)}[temp]

    rd = qsfp_read(port, temp_sense[1], 2, page=temp_sense[0], i2cCmd=i2cCmd)
    val = int(rd[0], 16)
    t = ((val & 0xff)<<8) + ((val & 0xff00)>>8)
    if (t & 0x8000) != 0:
        t = (0x10000 - t) * (-1)
    t = t * (1.0/256)
    return t

@deco_QSFP
def qsfp_dd_power(port, rate=None):
    """ rate: 0-100 (%)"""
    if rate==None:
        pwr = list()
        for i in [135, 136, 137, 138]:
            p = qsfp_read(port, i, page=3)
            pwr.append('{:1.2f}'.format(int(p[0], 16)*0.1))
        else:
            print ','.join(pwr) + ' W'

        return pwr

    elif rate >= 0 and rate <= 100:
        # p1 = int( 4.84 * (rate*0.1) )
        # p2 = int( 3.20 * (rate*0.1) )
        p1 = p2 = int( 255 * rate * 0.01 )
        qsfp_write(port, 135, p1, page=3)
        qsfp_write(port, 136, p2, page=3)
        qsfp_write(port, 137, p2, page=3)
        qsfp_write(port, 138, p2, page=3)
    else:
        print "Error: rate must be 0-100"

#------------------------
# NEWPORT Status/Control
#------------------------
def _np_get_iomap():
    iomap = {
     'lpmode': [{'sel':0x01, 'i2c': 0x20, 'p0':[ 0, 1, 2, 3, 4, 5, 6, 7],'p1': [ 9, 8,11,10,13,12,15,14],'io':'o'},
                {'sel':0x02, 'i2c': 0x21, 'p0':[17,16,19,18,21,20,23,22],'p1': [24,25,26,27,29,28,31,30],'io':'o'}],
  'present_n': [{'sel':0x04, 'i2c': 0x22, 'p0':[ 0, 1, 2, 3, 4, 5, 6, 7],'p1': [ 9, 8,11,10,13,12,15,14],'io':'i' },
                {'sel':0x08, 'i2c': 0x23, 'p0':[17,16,19,18,21,20,23,22],'p1': [24,25,26,27,29,28,31,30],'io':'i'}],
     'rxloss': [{'sel':0x10, 'i2c': 0x24, 'p0':[ 0, 1, 2, 3, 4, 5, 6, 7],'p1': [ 9, 8,11,10,13,12,15,14],'io':'i'},
                {'sel':0x20, 'i2c': 0x25, 'p0':[17,16,19,18,21,20,23,22],'p1': [24,25,26,27,29,28,31,30],'io':'i'}],
    'reset_n': [{'sel':0x40, 'i2c': 0x26, 'p0':[ 0, 1, 2, 3, 4, 5, 6, 7],'p1': [ 9, 8,11,10,13,12,15,14],'io':'o'},
                {'sel':0x80, 'i2c': 0x27, 'p0':[17,16,19,18,21,20,23,22],'p1': [24,25,26,27,29,28,31,30],'io':'o'}],
    }

    status = dict()
    ports = ['p0', 'p1']
    for s in iomap.keys():
        for io in iomap[s]:
            for p in ports:
                for pi in io[p]:
                    status[(pi,s)] = {'sel': io['sel'], 'i2c': io['i2c'], 'io': io['io'], 'p': p, 'bit':io[p].index(pi)}
    return status


def qsfp_init():
    """
    Initialize IO port configuration of PCA19535A devices

    pcal9535a
    0x00: Input port 0 (0-7)
    0x01: Input port 1 (8-15)
    0x02: Output port 0
    0x03: Output port 1
    0x04: Polarity inversion port 0 (1: inversion for input, default=0)
    0x05: Polarity inversion port 1
    0x06: Configuration port 0 (1: Input, 0: output, default=1)
    0x07: Configuration port 1
    0x44: Input latch enable port 0 (0: not latch, generates interrupt, read input clears interrupt. 1: enable latch)
    0x45: Input latch enable port 1 (0: not latch, generates interrupt, read input clears interrupt. 1: enable latch)
    0x4A: Interrupt mask port 0 (1: mask, default=1)
    0x4B: Interrupt mask port 1
    0x4C: Interrupt status port 0
    0x4D: Interrupt status port 1
    """
    status = ['lpmode', 'present_n', 'rxloss', 'reset_n']
    for st in status:
        for i in range(32):
            ioconf = statusIOs[(i, st)]
            i2c_writeAddr( _QSFP.I2CMUXPORT, 4, _QSFP.I2CMUXADDR, ioconf['sel'])
            # if _DEBUG:
                # rd = i2c_readData( _QSFP.I2CMUXPORT, 4, _QSFP.I2CMUXADDR, 1 )
                # print "MUX: sel={:02x}, read={:02x}".format(ioconf['sel'], int(rd[0],16))
            if ioconf['p'] == 'p0':
                ofst=0
            else:
                ofst=1
            i2c_write( _QSFP.I2CMUXPORT, 4, ioconf['i2c'], 0x04+ofst, 0x00)
            i2c_write( _QSFP.I2CMUXPORT, 4, ioconf['i2c'], 0x06+ofst, {'i':0xff, 'o':0x00}[ioconf['io']] )
            i2c_write( _QSFP.I2CMUXPORT, 4, ioconf['i2c'], 0x44+ofst, 0x00 )

            # if _DEBUG:
                # addr = [0x4, 0x6, 0x44]
                # for a in addr:
                    # rd = i2c_read( _QSFP.I2CMUXPORT, 4, ioconf['i2c'], a+ofst )
                    # print "{}: i2c=x{:x}, addr=x{:x}, read=x{:02x}".format(st, ioconf['i2c'], a+ofst, int(rd[0],16))

@deco_QSFP
def qsfp_status_read(port, signal, **kw):
    """ signal:  'present_n', 'rxloss', 'lpmode', 'reset_n', 'all',
        port: 0-31
    """
    status = ['present_n', 'rxloss', 'lpmode', 'reset_n', 'all']
    if signal not in status:
        print "wrong parameter: signal should be one of ['present_n', 'rxloss', 'lpmode', 'reset_n']"
        return

    if signal == 'all':
        status_rd = dict()
        for s in status[:-1]:
            rd = qsfp_status_read(port, s)
            status_rd[s] = rd
        return status_rd

    ioconf = statusIOs[(port, signal)]
    i2c_writeAddr( _QSFP.I2CMUXPORT, 4, _QSFP.I2CMUXADDR, ioconf['sel'])
    regaddr = 0 if ioconf['p']=='p0' else 1
    rd = i2c_read( _QSFP.I2CMUXPORT, 4, ioconf['i2c'], regaddr, numByte=1)
    rd = (int(rd[0], 16) >> ioconf['bit']) & 0x01
    return rd


def qsfp_ctrl_write(port, signal, val):
    """read or write to LPMODE or RESETn"""
    status = ['lpmode', 'reset_n']
    if signal not in status:
        print "wrong parameter: signal should be one of '{}'".format(','.join(status))
        return

    if val != 0:
        val = 1

    ioconf = statusIOs[(port,signal)]
    i2c_writeAddr( _QSFP.I2CMUXPORT, 4, _QSFP.I2CMUXADDR, ioconf['sel'])
    regaddr = 2 if ioconf['p']=='p0' else 3
    rd = i2c_read( _QSFP.I2CMUXPORT, 4, ioconf['i2c'], regaddr, numByte=1)
    rd = int(rd[0], 16) & (0xff - (1 << (ioconf['bit'])))
    wd = rd | (val << (ioconf['bit']))
    i2c_write( _QSFP.I2CMUXPORT, 4, ioconf['i2c'], regaddr, wd)
    # return wd


#
# BlueJay JTAG Mdio
#
class _MDIO_ERR:
    SUCCESS = 0
    TIMEOUT = 1

class _MDIOCMD:
    NOP    = 0b000
    WRITE  = 0b001
    READ   = 0b010
    RESET  = 0b011
    CLKDIV = 0b100

class _MDIOADDR:
    CMD     = 0 # [1:0]
    STATUS  = 0 # [31:16]
    GROUP   = 1 #4
    ADDR    = 2 # 8
    DATA    = 3 # 12
    JBRST   = 0xf

    PORT    = 34


#
# BlueJays
#

# Decorator for BlueJay class methods
def deco_BJ(func):
    def do_all(*args, **kw):
    
        rtn_all = list()
        _self = args[0] # first arg is alway "self"
        _args = list(args)
        _kw = copy.copy(kw)
        if len(args)==1: # all parameters are passed as keyword argument (only 'self')
            tile = kw['tile']
        else:   # tile parameter is passed as position argument. tile is at the end of argument list
            tile = args[-1]

        if tile in ['all', 'all_tile', 'all_tiles']:
            tiles = range(4)
        else:
            # tile is int type
            return func(*args, **kw)

        for t in tiles:
            print 'tile {}'.format(t)
            if 'tile' in _kw.keys():
                _kw['tile']=t
            else:
                _args[-1] = t

            rtn = func(*_args, **_kw)
            rtn = 0 if rtn==None else rtn
            rtn_all.append(rtn)
        return rtn_all

    return do_all

class BlueJay:
    def __init__(self, group=0):
        self._init_shadow()
        self.set_group(group)

    def _init_shadow(self, tile=None):
        if tile:
            self._shadow[tile] = {'group':-1, 'addr':-1} # tile0
        else:
            self._shadow = dict()
            for i in range(4):
                self._shadow[i] = {'group':-1, 'addr':-1} # tile0

    def _update_shadow(self, tile, group=None, addr=None):
        if group != None:
            if self._shadow[tile]['group'] == group:
                return False
            else:
                self._shadow[tile]['group'] = group
                return True
        if addr != None:
            if self._shadow[tile]['addr'] == addr:
                return False
            else:
                self._shadow[tile]['addr'] = addr
                return True
        return None

    def _get_mdio_base(self,tile = 0):
        tile_mem_base = ((_MDIOADDR.PORT + tile) << 10)
        return tile_mem_base

    def _mdio_write(self, addr, value, tile):
        tile_mem_base = self._get_mdio_base(tile)
        writeMem( tile_mem_base + addr, value )

    def _mdio_read(self, addr, tile):
        tile_mem_base = self._get_mdio_base(tile)
        rd = readMem( tile_mem_base + addr )
        rd = int(rd[1], 16)
        return rd

    def set_group(self, group=0, tile=0):
        if not self._update_shadow( tile, group=group):
            return 0

        tile_mem_base = ((_MDIOADDR.PORT+tile) << 10)
        self._mdio_write( _MDIOADDR.GROUP, group, tile )

    def _mdio_poll_status(self, tile):
        start = time.time()
        while True:
            reg0 = self._mdio_read(_MDIOADDR.STATUS, tile)
            sts = (reg0 >> 16)
            if ((reg0 & 0xffff)==0x0000 and (sts & 0x03)!=0):
                # print "{:1.5f}".format(time.time() - start)
                return 0
            if time.time() - start > 1.0:
                print "Time out"
                return _MDIO_ERR.TIMEOUT

    @deco_BJ
    def dbg_mem_dump(self, tile=0):
        print "Tile {} memory".format(tile)
        for addr in range(0, 4):
            rd = self._mdio_read(addr, tile)
            print "Tile={:d} Addr={:2d}: {:08x}".format(tile, addr, rd)

    @deco_BJ
    def mdio_reset(self, tile=0):
        self._init_shadow(tile)
        tile_mem_base = self._get_mdio_base(tile)
        self._mdio_write( _MDIOADDR.CMD, _MDIOCMD.RESET, tile )
        for i in range(10):
            rtn = self._mdio_read(_MDIOADDR.CMD, tile) & 0xff
            if rtn == 0:
                print "Reset done"
                return 0
            else:
                time.sleep(0.2)
        else:
            print "timeout error"
            return -1

    @deco_BJ
    def mdio_write(self, addr, value, tile=0):
        tile_mem_base = self._get_mdio_base(tile)
        if self._update_shadow(tile, addr=addr):
            self._mdio_write( _MDIOADDR.ADDR, addr, tile )
        self._mdio_write( _MDIOADDR.DATA, value, tile )
        self._mdio_write( _MDIOADDR.CMD, _MDIOCMD.WRITE, tile )

        rtn = self._mdio_poll_status(tile)
        return rtn

    @deco_BJ
    def mdio_read(self, addr, tile=0):
        tile_mem_base = self._get_mdio_base(tile)
        if self._update_shadow(tile, addr=addr):
            self._mdio_write( _MDIOADDR.ADDR, addr, tile )
        # self._mdio_write( _MDIOADDR.DATA, 0, tile )
        self._mdio_write( _MDIOADDR.CMD, _MDIOCMD.READ, tile )

        rtn = self._mdio_poll_status(tile)
        if rtn != 0:
            return -1
        rd = self._mdio_read( _MDIOADDR.DATA, tile )
        return rd

    @deco_BJ
    def update_mdio_clkdiv(self, div, tile=0):
        tile_mem_base = self._get_mdio_base(tile)
        if self._update_shadow(tile):
            self._mdio_write( _MDIOADDR.ADDR, 0, tile )
        self._mdio_write( _MDIOADDR.DATA, div, tile )
        self._mdio_write( _MDIOADDR.CMD, _MDIOCMD.CLKDIV, tile )

        rtn = self._mdio_poll_status(tile)
        return rtn

    @deco_BJ
    def jb_reset(self, tile):
        self._init_shadow(tile)
        self._mdio_write( _MDIOADDR.JBRST, 0x0, tile )
        time.sleep(0.1)
        self._mdio_write( _MDIOADDR.JBRST, 0x1, tile )
        return 0

    def mdio_read_all(self, addr):
        rds = list()
        for i in range(4):
            rd = self.mdio_read(addr, i)
            rds.append(rd)
            print "Tile{}: 0x{:04x}".format(i, rd)
        else:
            return rds


#----------------------------------------------------------+
# Socket interface
#----------------------------------------------------------+
# import socket
TcpPort = 8000
BufSize = 1024
class _ThreadedServer(object):

    def __init__(self, host, port=TcpPort):
        self.host = host
        self.port = port
        self.sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        # self.sock.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
        self.sock.bind((self.host, self.port))
        # self.tile=0
        # self.group=0
        self.bye = False
        self.bj = BlueJay()


    def listen(self):
        self.sock.listen(10)
        self.sock.settimeout(3.0)

        while True:
            print "Waiting for connection..."
            print "Host={}, Port={}".format(self.host, self.port)
            try:
                client, address = self.sock.accept()
                print "connected to client..."
                client.settimeout(1.0)
                threading.Thread(target=self.listenToClient, args=(client,address)).start()
                self.bye = False
            except KeyboardInterrupt:
                print "\nBye!\n"
                self.bye = True
                self.sock.close()
                return
            except:
                print "Ctrl+C to exit..."

            print "Number of client = {}".format(threading.activeCount())

    def parseRecvData(self, data):
        cmds = ['reset_mdio', 'reset_jb', 'rd', 'wr', 'group', 'tile']
        cmd = dict()
        tokens = data.split(',')
        if len(tokens)==0:
            return None
        elif tokens[0] not in cmds:
            return None

        cmd['cmd'] = tokens[0]
        if   tokens[0] == cmds[0]: # reset mdio
            cmd['tile'] = int(tokens[1], 16) if tokens[1].startswith('0x') else int(tokens[1], 10)
        elif tokens[0] == cmds[1]: # reset jbay
            cmd['tile'] = int(tokens[1], 16) if tokens[1].startswith('0x') else int(tokens[1], 10)
        elif tokens[0] == cmds[2]: # rd
            cmd['addr'] = int(tokens[1], 16) if tokens[1].startswith('0x') else int(tokens[1], 10)
            cmd['tile'] = int(tokens[2], 16) if tokens[2].startswith('0x') else int(tokens[2], 10)
        elif tokens[0] == cmds[3]: # wr
            cmd['addr'] = int(tokens[1], 16) if tokens[1].startswith('0x') else int(tokens[1], 10)
            cmd['val']  = int(tokens[2], 16) if tokens[2].startswith('0x') else int(tokens[2], 10)
            cmd['tile'] = int(tokens[3], 16) if tokens[3].startswith('0x') else int(tokens[3], 10)
        elif tokens[0] == cmds[4]: # grp
            self.group = int(tokens[1], 10)
            cmd['group'] = self.group
            cmd['tile'] = int(tokens[2], 16) if tokens[2].startswith('0x') else int(tokens[2], 10)
        elif tokens[0] == cmds[5]:
            cmd['tile'] = int(tokens[1], 10)
        else:
            return None

        return cmd

    def listenToClient(self, client, address):
        # tile = None
        print "Client thread started..."
        while True:
            try:
                data = client.recv(BufSize)

                if data:
                    if data.lower() == 'bye':
                        print "Bye!"
                        client.sendall(data)
                        client.close()
                        return
                    # else:
                        # client.sendall(data)
                        # continue

                    cmd = self.parseRecvData(data)
                    if cmd == None:
                        print "Command error"
                        response = "error"
                    else:
                        # print cmd['cmd']
                        if cmd['cmd'] == 'reset_mdio':
                            response = self.bj.mdio_reset(cmd['tile'])
                        elif cmd['cmd'] == 'reset_jb':
                            response = self.bj.jb_reset(cmd['tile'])
                        elif cmd['cmd'] == 'rd':
                            response = self.bj.mdio_read(cmd['addr'], tile=cmd['tile'])
                        elif cmd['cmd'] == 'wr':
                            response = self.bj.mdio_write(cmd['addr'], cmd['val'], tile=cmd['tile'])
                        elif cmd['cmd'] == 'group':
                            response = self.bj.set_group(cmd['group'], tile=cmd['tile'])
                        # elif cmd['cmd'] == 'tile':
                            # tile = cmd['tile']
                            # response = 0

                    # Set the response to echo back the received data
                    if type(response)==int:
                        client.sendall('{:d}'.format(response))
                    else:
                        client.sendall('{}'.format(response))

                else:
                    raise error('Client disconnected')

            except socket.timeout:
                if self.bye:
                    client.close()
                    print "Client close..."
                    return
            except:
                if self.sock == None:
                    client.close()
                    print "Client close..."
                    return

def start_mdio(host=None, port=TcpPort):
    if host == None:
        # f = os.popen('ifconfig enp2s0 | grep "inet\ addr" | cut -d: -f2 | cut -d" " -f1')
        # host = f.read().strip()
        # https://stackoverflow.com/questions/24196932/how-can-i-get-the-ip-address-of-eth0-in-python
        host = os.popen('ip addr show enp2s0').read().split("inet ")[1].split("/")[0]

    print "Host={}".format(host)
    srvr = None
    try:
        srvr = _ThreadedServer(host, port)
        print "Created socket"
        srvr.listen()
    except:
        if srvr:
            srvr.sock.close()
            srvr.sock = None
            print "Socket closed"
        else:
            print "Failed to create server thread"


#----------------------------------------------------------+
# Arguments
#----------------------------------------------------------+


init()
bj = BlueJay()
bj.update_mdio_clkdiv(30, 0)
bj.update_mdio_clkdiv(30, 1)
bj.update_mdio_clkdiv(30, 2)
bj.update_mdio_clkdiv(30, 3)

if __name__ == "__main__":

    import argparse

    parser = argparse.ArgumentParser()
    parser.add_argument("-i", "--init", help='Init QSFP Mux', action="store_true")
    parser.add_argument("-c", "--cmd",  help='Run command', type=str)

    # Initialization
    # init()
    statusIOs = _np_get_iomap()
    args = parser.parse_args()
    if args.init:
        print "Initializing QSFP MUX"
        qsfp_init()
    if args.cmd:
        # qsfp_read_all(0, 1, label='ID')
        eval(args.cmd)

    def testReset(n=1):
        bj = BlueJay()

        bj.update_mdio_clkdiv(30, 0)
        bj.update_mdio_clkdiv(30, 1)
        bj.update_mdio_clkdiv(30, 2)
        bj.update_mdio_clkdiv(30, 3)

        time.sleep(1)

        for test in range(n):
            print
            print "Test {}".format(test)
            for i in range(4):
                bj.jb_reset(i)
                bj.mdio_reset(i)
                addr = 0x0
                rd = bj.mdio_read(addr, i)
                if rd != 0x906b:
                    print "Tile{}, Error, {:x}, {:x}".format(i, addr, rd)
                addr = 0xa1
                rd = bj.mdio_read(addr, i)
                if rd != 0xaaaa:
                    print "Tile{}, Error, {:x}, {:x}".format(i, addr, rd)

    # testReset(10)

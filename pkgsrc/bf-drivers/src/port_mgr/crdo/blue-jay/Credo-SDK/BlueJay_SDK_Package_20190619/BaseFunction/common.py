import os, time, datetime
from credo import *
from regfunc import regFuncInit
permMode.setFilename(__file__)
permMode.setHivename('TOP')

def twos_to_int(twos_val, bitWidth):
    return twos_val - int((twos_val << 1) & 2**bitWidth)

def int_to_twos(value, bitWidth):
    #return value - int((value << 1) & 2**bitWidth)
    return (2**bitWidth + value) & (2**bitWidth - 1)

def bin_gray(bb = 0):
    gg = bb ^ (bb >> 1)
    return gg
    
def gray_bin(num = 0):
    mask = num
    while(mask != 0):
        mask = mask >> 1
        num = num ^ mask;
    return num

@public()
def soft_reset(lane_obj):
    #lane_obj.reg_cpuc_div = 0
    #lane_obj.reg_regc_div = 0
    lane_obj.sw_rstb_magic = 0x888
    lane_obj.sw_rstb_magic = 0

@public()
def reg_reset(lane_obj):
    #lane_obj.reg_cpuc_div = 0
    #lane_obj.reg_regc_div = 0
    lane_obj.sw_rstb_magic = 0x999
    lane_obj.sw_rstb_magic = 0

@public()
def logic_reset(lane_obj):
    #lane_obj.reg_cpuc_div = 0
    #lane_obj.reg_regc_div = 0
    lane_obj.sw_rstb_magic = 0x777
    lane_obj.sw_rstb_magic = 0 # Logic Reset after new FW downloaded

def cpu_reset(lane_obj):
    lane_obj.sw_rstb_magic = 0xaaa
    lane_obj.sw_rstb_magic = 0

@public()
def fw_hash(lane_obj, print_en = True):
    lane_obj.reserved_reg2 = 0xf000
    cnt=0
    while(lane_obj.reserved_reg2 & 0x0f00 != 0x0f00):
        cnt += 1
        if (cnt > 100): break
        pass
    high_word = (lane_obj.reserved_reg2 & 0xff)  # upper byte
    low_word = (lane_obj.reserved_reg3 & 0xffff) # lower bytes
    hash_code = (high_word <<16) + low_word
    if print_en: print("\n....FW Hash Code: 0x%06X" %(hash_code))
    return hash_code
################################################################################

@public()
def fw_crc(lane_obj, print_en = True):
    lane_obj.reserved_reg2 = 0xf001
    cnt = 0
    while(lane_obj.reserved_reg2 & 0x0f00 != 0x0f00):
        cnt += 1
        if (cnt > 100): break
        pass
    checksum_code  = lane_obj.reserved_reg3
    if print_en: print("\n....FW CRC Checksum: 0x%04X\n" %(checksum_code)) 
    return checksum_code
###############################################################################

@public()
def fw_unload(lane_obj):
    lane_obj.reserved_reg1 = 0xfff0
    cpu_reset(lane_obj)
    time.sleep(0.1)
    lane_obj.reserved_reg1 = 0x0
    print("....FW Unloaded")

####################################################################
permMode.setHivename('PAM4')
@public()
def fw_load_info(lane_obj,expected_hash_code=None):
    fw_load_info_code=0
    actual_magic_word = lane_obj.reserved_reg0 # Validate FW magic word
    actual_hash_code = fw_hash(lane_obj)
    actual_crc_code  = fw_crc(lane_obj)

    if expected_hash_code==None: expected_hash_code=fw_file_hash_code
    
    if (actual_magic_word == fw_load_magic_word): # Firmware Downloaded and magic word is correct
            fw_load_info_code = fw_load_info_code | 1
    if (actual_hash_code == expected_hash_code): # Firmware Downloaded and hash code is correct
            fw_load_info_code = fw_load_info_code | 2
    if (actual_crc_code == fw_file_crc_code): # Firmware Downloaded and crc code is correct
            fw_load_info_code = fw_load_info_code | 4
            
    return fw_load_info_code, actual_magic_word, actual_hash_code, actual_crc_code

def is_fw_loaded(lane_obj, expected_hash_code=None):
    global fw_load_magic_word
    
    fw_load_info_code, actual_magic_word, actual_hash_code, actual_crc_code = fw_load_info(lane_obj)

    if expected_hash_code==None: expected_hash_code=fw_file_hash_code

    if (actual_hash_code != expected_hash_code): # Firmware Downloaded but Hash code is wrong
        fw_running_stat=0
        return fw_running_stat
    # if (actual_crc_code != fw_file_crc_code): # Firmware Downloaded but CRC code is wrong
        # fw_running_stat=0
        # return fw_running_stat
        
    # The correct FW is downloaded, now check if the magic word is not corrupted
    if (actual_magic_word == fw_load_magic_word): # Firmware Downloaded and magic word is correct
        fw_running_stat=1
    else: # Firmware loaded but the magic word is corrupted
        val0 = lane_obj.SPI_CTRL1 # check the watchdog counter to see if FW is incrementing it
        time.sleep(1.8) # wait more than one second
        val1 = lane_obj.SPI_CTRL1 # check the watchdog counter once more
        if val1 != val0: # watchdog counter is moving, then FW is loaded and running
            lane_obj.reserved_reg0 = fw_load_magic_word # if FW is loaded but magic word is corrupted, correct it
            fw_running_stat=2  # watchdog counter is moving, then FW is loaded and running
        else:
            fw_running_stat=0 # watchdog counter is not moving, then FW is not loaded or halted            

    return fw_running_stat 

def is_fw_running(lane_obj):
    #global fw_load_magic_word
    
    val0 = lane_obj.SPI_CTRL1 # check the watchdog counter to see if FW is incrementing it
    time.sleep(1.8) # wait more than one second
    val1 = lane_obj.SPI_CTRL1 # check the watchdog counter once more
    if val1 != val0: # watchdog counter is moving, then FW is loaded and running
        #lane_obj.reserved_reg0 = fw_load_magic_word # if FW is loaded but magic word is corrupted, correct it
        fw_running_stat=1  # watchdog counter is moving, then FW is loaded and running
    else:
        fw_running_stat=0 # watchdog counter is not moving, then FW is not loaded or halted            

    return fw_running_stat 

def get_mdio_status(chip):
    MDIO_CONNECTED = 0
    MDIO_DISCONNECTED =  1
    MDIO_LOST_CONNECTION = 2  
    mdio_status = MDIO_LOST_CONNECTION
    val0 = chip.MdioRd(0)
    val1 = chip.MdioRd(1)
    if ((val0==0x0001 or val0==0xffff or val0==0x7fff) and val1==val0):
        mdio_status = MDIO_DISCONNECTED
    
    elif (val0!=0x0000 and val0!=0x0001 and val0!=0xffff and val1!=val0): # Already connected and reading valid values
        mdio_status = MDIO_CONNECTED
     
    return mdio_status


########################### 2018-1-22 for baldeagle
def mdio(chip, connect = None, dev = 0):
    '''
    get mdio status, connect mdio, disconnect mdio
    '''
    val0 = chip.MdioRd(0x0)
    val1 = chip.MdioRd(0x1)  
    if (connect == None):
        mdio_status = get_mdio_status()
        if (mdio_status == 0): # check MDIO connection to the chip
            print '\n...MDIO Already Disconnected!'
        else:
            print '\n...MDIO Already Connected!'    
    elif connect == 1: 
        if (val0!=0x0000 and val0!=0x0001 and val0!=0xffff and val1!=val0): # Already connected and reading valid values
            print '\n...MDIO Already Connected!'
        else: # Not connected yet, so issue connect() command
            chip.connect(phy_addr = dev)
            val0 = chip.MdioRd(0x0)
            if (val0!=0x0000 and val0!=0x0001 and val0!=0xffff): 
                print '\n...MDIO Connected Successfully!'
            else:
                print ('\n***MDIO Connection Issue***')
                print ('***Reading back 0x%04x***' % val0)        
    elif connect == 0: 
        chip.disconnect()
        print '\n...MDIO Disconnected!'
    
def load_setup(chip, filename):
    '''
    generic function to load register and value pairs
    '''
    try:
        f=open(filename, 'r')
        script = f.read()
        f.close()
    except IOError:
        print ("\n***Error: Can't Find Register Setup File: %s***\n" %filename)
        return
    insts = re.findall("^[\da-fA-F]{4}[ \t]+[\da-fA-F]{4}", script, flags = re.MULTILINE)
    insts = map(lambda t: (int(t[0:4], 16), int(t[5:], 16)), insts) # convert extracted texts to 16-bit integers addr and data
    for inst in insts:
        chip.MdioWr(inst[0], inst[1])
    print ("...Successfully Loaded Register Setup File: %s" %filename)

def fw_load(lane_obj, chip, file_name, magic_word = 0x5b5b): ## lane_obj -> top
    if not os.path.exists(file_name):
        print("\n***Error Opening FW File: %s" % file_name)
        return
    print("\n...Downloding FW: %s" % file_name)
    file_ptr = open(file_name, 'rb')
    bin_data = file_ptr.read()
    chip.bin2mdio(bin_data,1)
    time.sleep(.5)
    magic_word = lane_obj.reserved_reg1 #reg_register_reset_ctrl_s
    if (magic_word == 0x5b5b): ### incorrect magic word ???
        print("...Firmware Downloaded Successfully: 0x%04X" % magic_word)
        fw_hash(lane_obj)
        logic_reset(lane_obj)
    else:
        print("\n***Error (0x%04X) Downloading SerDes Firmware File: %s" % (magic_word,file_name))
    file_ptr.close()

def wregBits(lane_obj, addr,bits, val): ## chip_obj
    val_ori = lane_obj.MdioRd(addr)
    val1 = rregBits(lane_obj,addr,bits)
    val_edit = val_ori - (val1<<bits[-1])
    val_final = val_edit + ((val<<bits[-1])&(2**(bits[0]-bits[-1]+1)-1))
    lane_obj.MdioWr(addr,val)

def rregBits(lane_obj, addr, bits): ## chip_obj
    val_ori = lane_obj.MdioRd(addr)
    val = (val_ori>>bits[-1])&(2**(bits[0]-bits[-1]+1)-1)
    return val

def read_fw_reg(lane_obj, fw_reg_addr = 20):
    lane_obj.reserved_reg3 = fw_reg_addr #chip.MdioWr(0x9816, fw_reg_addr)
    lane_obj.reserved_reg2 = 0xE010 #chip.MdioWr(0x9815, 0xE010)    
    if lane_obj.reserved_reg2 & 0x0f00 == 0x300:
        print "Wrong index..."
    elif lane_obj.reserved_reg2 & 0x0f00 != 0x0e00:
        print "Firmware register read error..."
    else:
        return lane_obj.SPI_DATA #chip.MdioRd(0x9812)
    
def write_fw_reg(lane_obj, fw_reg_addr = 20, val = 0xE):
    lane_obj.reserved_reg3 = fw_reg_addr #chip.MdioWr(0x9816, fw_reg_addr)
    lane_obj.SPI_DATA = val #chip.MdioWr(0x9812, val)
    lane_obj.reserved_reg2 = 0xE020 #chip.MdioWr(0x9815, 0xE020)
    if lane_obj.reserved_reg2 & 0x0f00 == 0x300:
        print "Wrong index..."
    if lane_obj.reserved_reg2 & 0x0f00 != 0x0e00:
        print "Firmware register write error..."

def fw_halt(lane_obj):
    lane_obj.reserved_reg2 = 0xd000
    return not is_fw_running(lane_obj)

def fw_continue(lane_obj):
    lane_obj.reserved_reg2 = 0xd001
    return is_fw_running(lane_obj)
##############################################################################
# 
# Gray Code to Binary Conversion
##############################################################################
def Bin_Gray(bb):
    gg=bb ^ (bb >> 1)
    return gg

##############################################################################
def Gray_Bin(gg): # up to 7-bit Gray Number
    bb1 = (gg & 0x40)
    bb2 = (gg ^ (bb1 >> 1)) & (0x20)
    bb3 = (gg ^ (bb2 >> 1)) & (0x10)
    bb4 = (gg ^ (bb3 >> 1)) & (0x8)
    bb5 = (gg ^ (bb4 >> 1)) & (0x4)
    bb6 = (gg ^ (bb5 >> 1)) & (0x2)
    bb7 = (gg ^ (bb6 >> 1)) & (0x1)
    bb  = bb1 + bb2 + bb3 + bb4 + bb5 + bb6 + bb7
    return bb

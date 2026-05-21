"""Define Credo BlueJay SDK path"""

import os
import sys

Chip_name='BlueJay'
if sys.platform.startswith('linux'):
    _SDK_path = r'/home/admin12/toshi/sdk/BlueJay_SDK_Package_20190305'
elif sys.platform.startswith('win'):
    _SDK_path = r'C:\myData\design\python\system\BlueJay_SDK_Package_20190305'

_REG_path = os.path.join(_SDK_path, 'TestScript', r'BlueJay_regs.txt')
# _FW_path  = os.path.join(_SDK_path, 'TestScript', r'bluejay.fw.2019_01_18_2.bin')
# _FW_path  = os.path.join(_SDK_path, 'TestScript', r'bluejay.fw.bin')
_FW_path  = os.path.join(_SDK_path, 'TestScript', r'bluejay.fw.group0_7.1.00.03.bin')
_FW8_path  = os.path.join(_SDK_path, 'TestScript', r'bluejay.fw.group8.1.00.03.bin')


if _SDK_path not in sys.path:
    sys.path.append(_SDK_path)

if sys.platform.startswith('win'):
    _Tofino2_path = r"C:\myData\design\python\app\tofino2"
    if _Tofino2_path not in sys.path:
        sys.path.append(_Tofino2_path)

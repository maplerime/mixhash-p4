import os

"""
Note: This change is temporary. Scapy and bf_pktpy will only be available 
      during the transition period. After this period, the default 
      tool will be bf-pktpy.
"""
pktpy_tool = (os.environ.get("PKTPY", "true")).lower()
if pktpy_tool == "false":
    from .dtel_utils_scapy import *
else:
    from .dtel_utils_pktpy import *


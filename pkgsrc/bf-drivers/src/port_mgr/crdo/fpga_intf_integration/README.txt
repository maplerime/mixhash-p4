#
# This program is used to access an FPGA over PCIe. The FPGA provides an
# interface to the Credo serdes tile (PCIe -> JTAG -> MDIO).
# This code provides a conduit from the Credo Python SDK (or the driver)
# via a TCP/IP socket.
#
# SDK or ->  TCP/IP -> fpga_if.c -> PCIe -> JTAG -> MDIO -> Tile Chip
# driver 
#
# On a newly booted system, of the kernel driver is not loaded, one needsto 
# enable the BAR mem space once before pcimem.c is used.  I just discovered.
# Pl. remember (and preserve this email) when you start using fpga board.
#
# To build FPGA interfce program
#
gcc fpga_if.c
#
#
# e.g., do once like this
sudo setpci -d 1d1c:0010 command=102
#
# To run FPGA interface program
#
# TBD
#
sudo gdb ./a.out  
gdb> run some-file
gdb> run /sys/bus/pci/devices/0000\:06\:00.0/resource0
#
# In a separate terminal/shell window, start the python SDK,
#
python ./test_BlueJay_release_package_120618.py
#
#
# Note:
# For some reason the Credo Algorithm/ directory does not get added to git. You will need to copy
# it (recursively) from somewhere else.


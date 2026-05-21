#
# Following can be used to (re-)generate the access .c and .h files
#
python ./aw-xml-to-code.py CSR_full.xml
#
# After generation, move them to src/port_mgr/port_mgr_tof3/
#
# Alphawave accessros
cp aw_access.h ../port_mgr_tof3
cp aw_access.c ../port_mgr_tof3
cp aw_fld_access.h ../port_mgr_tof3
cp aw_fld_access.c ../port_mgr_tof3
cp aw_def.h ../port_mgr_tof3
cp bf_ll_aw_if.c ../port_mgr_tof3
cp bf_ll_aw_if.h ../../../include/port_mgr


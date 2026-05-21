#
# Following can be used to (re-)generate the access .c and .h files
#
python ./umac3-xml-to-code.py ../../../src/comira/umac3c4-u1244.xml
python ./umac4-xml-to-code.py ../../../src/comira/umac4c8.xml 
python ./csr-gen.py ./eth400g_pcs_rspec.csr eth400g_pcs_rspec "eth400g_p1.eth400g_pcs" "<no-rotation-diff>"
python ./csr-gen.py ./eth400g_mac_rspec.csr eth400g_mac_rspec "eth400g_p1.eth400g_mac" "<no-rotation-diff>"
python ./csr-gen.py ./eth100g_reg_rspec.csr eth100g_reg_rspec "eth100g_regs.eth100g_reg" "eth100g_regs_rot.eth100g_reg"
#
# After generation, move them to src/port_mgr/port_mgr_tof2/
#
# UMAC4
cp umac4c8_access.h ../port_mgr_tof2
cp umac4c8_access.c ../port_mgr_tof2
cp umac4c8_fld_access.h ../port_mgr_tof2
cp umac4c8_fld_access.c ../port_mgr_tof2
cp umac4c8_def.h ../port_mgr_tof2
cp bf_ll_umac4_if.c ..
cp bf_ll_umac4_if.h ../../../include/port_mgr
#
# UMAC3
#
cp umac3c4_def.h ../port_mgr_tof2
cp umac3c4_access.h ../port_mgr_tof2
cp umac3c4_access.c ../port_mgr_tof2
cp umac3c4_fld_access.h ../port_mgr_tof2
cp umac3c4_fld_access.c ../port_mgr_tof2
cp bf_ll_umac3_if.c ..
cp bf_ll_umac3_if.h ../../../include/port_mgr
#
# CSRs
#
cp eth400g_pcs_rspec_access.h ../port_mgr_tof2
cp eth400g_pcs_rspec_access.c ../port_mgr_tof2
cp eth400g_mac_rspec_access.h ../port_mgr_tof2
cp eth400g_mac_rspec_access.c ../port_mgr_tof2
cp eth100g_reg_rspec_access.h ../port_mgr_tof2
cp eth100g_reg_rspec_access.c ../port_mgr_tof2
#
# BF low-level APIs
#
cp bf_ll_eth100g_reg_rspec_if.h ../../../include/port_mgr
cp bf_ll_eth400g_mac_rspec_if.h ../../../include/port_mgr
cp bf_ll_eth400g_pcs_rspec_if.h ../../../include/port_mgr
cp bf_ll_eth100g_reg_rspec_if.c ../port_mgr_tof2
cp bf_ll_eth400g_mac_rspec_if.c ../port_mgr_tof2
cp bf_ll_eth400g_pcs_rspec_if.c ../port_mgr_tof2


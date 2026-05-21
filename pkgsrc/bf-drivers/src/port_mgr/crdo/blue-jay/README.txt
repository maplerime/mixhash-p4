# README.txt
#
# Credo API auto-generation
#
# The credo_csv_parse.py python file takes a .csv file generated from the Credo xlsx file.
#
# It outputs several products:
#
# Credo serdes tile C APIs (get/set) for every defined serdes attribute:
# credo_sd_api.h
# credo_sd_api.c
#
# Python APIs (get/set/show) for every defined serdes attribute:
# credo_sd_menu.py
# credo_sd_api.py
#
#
python ./credo_csv_parse.py ./bluejay-regs-0614.csv
#
# After re-generating the above files they should be copied to their
# destination directories as follows:
#
cp credo_sd_access.h ../../port_mgr_tof2/
cp credo_sd_access.c ../../port_mgr_tof2/
cp bf_ll_serdes_if.h ../../../../include/port_mgr/
cp bf_ll_serdes_if.c ../../port_mgr_tof2/
#
# Not sure where the generated python APIs will live yet
#

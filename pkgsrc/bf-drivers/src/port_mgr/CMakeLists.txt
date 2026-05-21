project(libport_mgr VERSION 0.1 LANGUAGES C)

add_library(bfport_mgr_o OBJECT
bf_port_if.c
bf_map.c
port_mgr.c
port_mgr_ha.c
port_mgr_dev.c
port_mgr_port.c
port_mgr_mac_stats.c
port_mgr_map.c
port_mgr_umac_access.c
bf_ll_umac3_if.c
bf_ll_umac4_if.c
port_mgr_tof1/port_mgr_tof1.c
port_mgr_tof1/port_mgr_tof1_dev.c
port_mgr_tof1/port_mgr_tof1_ha.c
port_mgr_tof1/port_mgr_tof1_port.c
port_mgr_tof1/port_mgr_tof1_map.c
port_mgr_tof1/port_mgr_ucli.c
port_mgr_tof1/bf_fsm_if.c
port_mgr_tof1/bf_fsm_hdlrs.c
port_mgr_tof1/port_mgr_port_diag.c
port_mgr_tof1/port_mgr_mac.c
port_mgr_tof1/port_mgr_serdes.c
port_mgr_tof1/port_mgr_serdes_sbus_map.c
port_mgr_tof1/port_mgr_serdes_diag.c
port_mgr_tof1/port_mgr_av_sd.c
port_mgr_tof1/port_mgr_av_sd_an.c
port_mgr_tof1/comira_reg_access_autogen.c
port_mgr_tof1/comira_reg_access_autogen.h
port_mgr_tof1/bf_serdes_if.c
port_mgr_tof1/comira_reg_def_autogen.h
port_mgr_tof2/bf_tof2_serdes_if.c
port_mgr_tof2/credo_sd_access.c
port_mgr_tof2/umac3c4_fld_access.c
port_mgr_tof2/umac3c4_access.c
port_mgr_tof2/umac4c8_fld_access.c
port_mgr_tof2/umac4c8_access.c
port_mgr_tof2/eth400g_mac_rspec_access.c
port_mgr_tof2/eth400g_pcs_rspec_access.c
port_mgr_tof2/eth100g_reg_rspec_access.c
port_mgr_tof2/umac4_ctrs.c
port_mgr_tof2/umac4_ctrs_str.c
port_mgr_tof2/port_mgr_tof2.c
port_mgr_tof2/port_mgr_tof2_ha.c
port_mgr_tof2/port_mgr_tof2_map.c
port_mgr_tof2/port_mgr_tof2_dev.c
port_mgr_tof2/port_mgr_tof2_umac.c
port_mgr_tof2/port_mgr_tof2_umac3.c
port_mgr_tof2/port_mgr_tof2_umac4.c
port_mgr_tof2/port_mgr_tof2_microp.c
port_mgr_tof2/port_mgr_tof2_gpio.c
port_mgr_tof2/port_mgr_tof2_serdes.c
port_mgr_tof2/port_mgr_tof2_port.c
port_mgr_tof2/bf_ll_serdes_if.c
port_mgr_tof2/bf_ll_eth100g_reg_rspec_if.c
port_mgr_tof2/bf_ll_eth400g_mac_rspec_if.c
port_mgr_tof2/bf_ll_eth400g_pcs_rspec_if.c
port_mgr_tof3/bf_ll_tof3_eth400g_app_rspec_if.c
port_mgr_tof3/bf_ll_tof3_eth400g_app_rspec_if.h
port_mgr_tof3/bf_ll_tof3_eth400g_mac_rspec_if.c
port_mgr_tof3/bf_ll_tof3_eth400g_mac_rspec_if.h
port_mgr_tof3/bf_ll_tof3_eth400g_sys_rspec_if.c
port_mgr_tof3/bf_ll_tof3_eth400g_sys_rspec_if.h
port_mgr_tof3/port_mgr_tof3.c
port_mgr_tof3/port_mgr_tof3_dev.c
port_mgr_tof3/port_mgr_tof3_dev.h
port_mgr_tof3/port_mgr_tof3.h
port_mgr_tof3/port_mgr_tof3_map.c
port_mgr_tof3/port_mgr_tof3_map.h
port_mgr_tof3/port_mgr_tof3_physical_dev.h
port_mgr_tof3/port_mgr_tof3_port.c
port_mgr_tof3/port_mgr_tof3_port.h
port_mgr_tof3/port_mgr_tof3_serdes.c
port_mgr_tof3/port_mgr_tof3_serdes.h
port_mgr_tof3/port_mgr_tof3_tmac.c
port_mgr_tof3/port_mgr_tof3_tmac.h
port_mgr_tof3/tmac_access.c
port_mgr_tof3/tmac_access.h
port_mgr_tof3/tof3-autogen-required-headers.h
port_mgr_tof3/tof3_eth400g_app_rspec_access.c
port_mgr_tof3/tof3_eth400g_app_rspec_access.h
port_mgr_tof3/tof3_eth400g_mac_rspec_access.c
port_mgr_tof3/tof3_eth400g_mac_rspec_access.h
port_mgr_tof3/tof3_eth400g_sys_rspec_access.c
port_mgr_tof3/tof3_eth400g_sys_rspec_access.h
port_mgr_tof3/port_mgr_tof3_microp.c
port_mgr_tof3/bf_tof3_serdes_if.c
port_mgr_tof3/bf_tof3_serdes_utils.c
port_mgr_tof3/port_mgr_tof3_serdes_map.c
port_mgr_tof3/bf_aw_pmd.c
port_mgr_tof3/bf_aw_vfld_pmd.c
port_mgr_tof3/aw_io.c
port_mgr_tof3/aw_driver_sim.c
port_mgr_tof3/svdpi.c
port_mgr_tof3/aw_reg_dbg.c
port_mgr_tof3/aw_16ln/aw_alphacore.c
port_mgr_tof3/aw_16ln/aw_pmd_base.c
port_mgr_tof3/aw_16ln/aw_pmd_rx_dsp_get.c
#port_mgr_tof3/aw_16ln/aw_pmd_sw_load_lane_cfg.c
port_mgr_tof3/aw_16ln/aw_16ln_vapi.c
port_mgr_tof3/aw_4ln/aw_alphacore.c
port_mgr_tof3/aw_4ln/aw_pmd_base.c
port_mgr_tof3/aw_4ln/aw_pmd_rx_dsp_get.c
#port_mgr_tof3/aw_4ln/aw_pmd_sw_load_lane_cfg.c
port_mgr_tof3/aw_4ln/aw_4ln_vapi.c
)
target_compile_options(bfport_mgr_o PRIVATE -Wno-unused-parameter -Wno-maybe-uninitialized -Wno-implicit-fallthrough)
add_library(bfport_mgr SHARED EXCLUDE_FROM_ALL $<TARGET_OBJECTS:bfport_mgr_o>)
target_link_libraries(bfport_mgr lld)

# Building port manager doxygen
find_package(Doxygen)
if(DOXYGEN_FOUND)
  set(srcdir ${CMAKE_CURRENT_SOURCE_DIR}/docs)
  set(doxyfile_in ${CMAKE_CURRENT_SOURCE_DIR}/docs/Doxyfile.in)
  set(doxyfile ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile)
  configure_file(${doxyfile_in} ${doxyfile} @ONLY)
  add_custom_target(doc-driver-port-mgr
                    COMMAND ${DOXYGEN_EXECUTABLE} ${doxyfile}
                    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
                    COMMENT "Generating Port Manager Doxygen"
                    VERBATIM)
  set_target_properties(doc-driver-port-mgr PROPERTIES EXCLUDE_FROM_ALL TRUE)
  install(DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/html DESTINATION
    share/doc/bf-drivers/port_mgr OPTIONAL)
  install(DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/latex DESTINATION
    share/doc/bf-drivers/port_mgr OPTIONAL)
else()
  message(WARNING "Doxygen is needed to build the documentation. Please install doxygen.")
endif()

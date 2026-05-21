include_directories(.)

add_subdirectory(dvm)
add_subdirectory(lld)
add_subdirectory(avago)
add_subdirectory(mc_mgr)
add_subdirectory(port_mgr)
add_subdirectory(traffic_mgr)
add_subdirectory(pkt_mgr)
add_subdirectory(pipe_mgr)
add_subdirectory(knet_mgr)
if (BFRT)
  add_subdirectory(bf_rt)
endif()
if(TDI)
  add_subdirectory(tdi_tofino)
endif()
if (PI)
  add_subdirectory(pi)
endif()
if (P4RT)
  add_subdirectory(p4_rt)
endif()

add_subdirectory(ctx_json)
add_subdirectory(pdfixed)
add_subdirectory(bf_pm)
add_subdirectory(dru_sim)
add_subdirectory(diag)
add_subdirectory(perf)

# copy the files to install. this happens at cmake configuration time
file(COPY
  ${CMAKE_CURRENT_SOURCE_DIR}/avago/firmware/sbus_master.0x101D_2001.rom
  ${CMAKE_CURRENT_SOURCE_DIR}/avago/firmware/pcie_serdes.0x1066_2147.rom
  ${CMAKE_CURRENT_SOURCE_DIR}/avago/firmware/serdes.0x106F_204D.rom
  ${CMAKE_CURRENT_SOURCE_DIR}/avago/firmware/serdes.0x1091_244D.rom
  DESTINATION ${CMAKE_INSTALL_PREFIX}/share/tofino_sds_fw/avago/firmware)
file(COPY
  ${CMAKE_CURRENT_SOURCE_DIR}/credo/firmware/bluejay_nrz.fw.1.2.28.bin
  ${CMAKE_CURRENT_SOURCE_DIR}/credo/firmware/bluejay.fw.1.2.28.bin
  DESTINATION ${CMAKE_INSTALL_PREFIX}/share/tofino_sds_fw/credo/firmware)
file(COPY
  microp/tof2/fw/tv80-code.bin
  DESTINATION ${CMAKE_INSTALL_PREFIX}/share/microp_fw/microp/tof2/fw)
file(COPY
  ${CMAKE_CURRENT_SOURCE_DIR}/alphawave/firmware/fw-16ln-eth-v1p1p3.hex
  ${CMAKE_CURRENT_SOURCE_DIR}/alphawave/firmware/fw-4ln-eth-v1p1p2.hex
  ${CMAKE_CURRENT_SOURCE_DIR}/alphawave/firmware/fw-4ln-pcie-090122.hex
  DESTINATION ${CMAKE_INSTALL_PREFIX}/share/tofino_sds_fw/alphawave/firmware)

# make will automatically install symlinks. no need to invoke "make firmware_install"
add_custom_target(firmware_install ALL)
add_custom_command(
  TARGET firmware_install POST_BUILD
  WORKING_DIRECTORY ${CMAKE_INSTALL_PREFIX}/share/tofino_sds_fw/avago/firmware
  COMMAND ln -sf sbus_master.0x101D_2001.rom sbus_master.rom
  COMMAND ln -sf pcie_serdes.0x1066_2147.rom pcie_serdes.rom
  COMMAND ln -sf serdes.0x106F_204D.rom serdes_A0.rom
  COMMAND ln -sf serdes.0x1091_244D.rom serdes_B0.rom
)
add_custom_command(
  TARGET firmware_install POST_BUILD
  WORKING_DIRECTORY ${CMAKE_INSTALL_PREFIX}/share/tofino_sds_fw/credo/firmware
  COMMAND ln -sf bluejay.fw.1.2.28.bin tof2_A0_grp_0_7_serdes.bin
  COMMAND ln -sf bluejay_nrz.fw.1.2.28.bin tof2_A0_grp_8_serdes.bin
)
add_custom_command(
  TARGET firmware_install POST_BUILD
  WORKING_DIRECTORY ${CMAKE_INSTALL_PREFIX}/share/microp_fw/microp/tof2/fw
  COMMAND ln -sf tv80-code.bin microp_fw.bin
)
add_custom_command(
  TARGET firmware_install POST_BUILD
  WORKING_DIRECTORY ${CMAKE_INSTALL_PREFIX}/share/tofino_sds_fw/alphawave/firmware
  COMMAND ln -sf fw-16ln-eth-v1p1p3.hex tof3_serdes_1_32_fw.hex
  COMMAND ln -sf fw-4ln-eth-v1p1p2.hex  tof3_serdes_0_fw.hex
  COMMAND ln -sf fw-4ln-pcie-090122.hex  tof3_serdes_0_pcie_fw.hex
)

/*******************************************************************************
 *  INTEL CONFIDENTIAL
 *
 *  Copyright (c) 2021 Intel Corporation
 *  All Rights Reserved.
 *
 *  This software and the related documents are Intel copyrighted materials,
 *  and your use of them is governed by the express license under which they
 *  were provided to you ("License"). Unless the License provides otherwise,
 *  you may not use, modify, copy, publish, distribute, disclose or transmit
 *  this software or the related documents without Intel's prior written
 *  permission.
 *
 *  This software and the related documents are provided as is, with no express
 *  or implied warranties, other than those that are expressly stated in the
 *  License.
 ******************************************************************************/

#ifndef lld_tof2_addr_conversion_h
#define lld_tof2_addr_conversion_h

static inline uint64_t tof2_dir_to_indir_dev_sel(uint32_t daddr) {
  // Indirect Address Format for Device Select
  // 41:39 == PCIe 26:24 == 0
  // 38:34 == PCIe 23:19 == 5 bit Dev_sel type
  // 33:19 == 0 for registers, non-zero for memories
  // 18:0  == PCIe 18:0
  uint64_t x = daddr;
  x = ((x & UINT64_C(0xF80000)) << 15) | (x & UINT64_C(0x7FFFF));
  return x;
}

static inline uint64_t tof2_dir_to_indir_pipe_reg(uint32_t daddr) {
  // Indirect Address format for pipe registers
  // 41    == PCIe 26 == 1
  // 40:39 == 25:24 == Pipe ID
  // 38:34 == 32:19 == Stage ID
  // 33:19 == 0 for registers, non-zero for memories
  // 18:0  == PCIe 18:0
  uint64_t x = daddr;
  x = ((x & UINT64_C(0x7F80000)) << 15) | (x & UINT64_C(0x7FFFF));
  return x;
}

static inline uint64_t tof2_dir_to_indir_mac_reg(uint32_t daddr) {
  // Indirect Address format for mac registers
  // 41:39 == 26:24 == 010b (MAC address space)
  // 38    == 0
  // 37:32 == 23:18 == MAC ID
  // 31:18 == 0 for registers, non-zero for memories
  // 17:0  == PCIe 17:0
  uint64_t x = daddr;
  x = ((UINT64_C(0x1) << 40) | (x & UINT64_C(0xFC0000)) << 14) |
      (x & UINT64_C(0x3FFFF));
  return x;
}
#endif

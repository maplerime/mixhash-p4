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

#ifndef lld_tof_addr_conversion_h
#define lld_tof_addr_conversion_h

/* Allow the use in C++ code.  */
#ifdef __cplusplus
extern "C" {
#endif

static inline uint64_t tof_dir_to_indir_dev_sel(uint32_t daddr) {
  // Indirect Address Format for Device Select
  // 41:39 == PCIe 25:23 == 0
  // 38:34 == PCIe 22:18 == 5 bit Dev_sel type
  // 33:18 == 0 for registers, non-zero for memories
  // 17:0  == PCIe 17:0
  uint64_t x = daddr;
  x = ((x & UINT64_C(0x7C0000)) << 16) | (x & UINT64_C(0x3FFFF));
  return x;
}

static inline uint64_t tof_dir_to_indir_mac(uint32_t daddr) {
  // Indirect Address Format for MAC
  // 41:39 == 010b
  // 38:32 == MAC [6:0]
  // 31:17 == 0 for registers, non-zero for memories
  // 16:0  == PCIe [16:0]
  uint64_t x = daddr;
  x = ((x & UINT64_C(0xFE0000)) << 15) | (x & UINT64_C(0x1FFFF)) |
      0x10000000000;
  return x;
}

static inline uint64_t tof_dir_to_indir_pipe(uint32_t daddr) {
  // Indirect Address Format for pipe
  // 41:40 == 10b (pipe)
  // 39:37 == PipeID
  // 36:33 == StageID
  // 32:19 == 0
  // 18:0  == PCIe 18:0
  uint64_t x = daddr;
  x = ((x & UINT64_C(0x1F80000)) << 15) | (x & UINT64_C(0x7FFFF)) |
      0x20000000000;
  return x;
}

static inline uint32_t tof_indir_to_dir_dev_sel(uint64_t iaddr) {
  // Indirect Address Format for Device Select
  // 41:39 == PCIe 25:23 == 0
  // 38:34 == PCIe 22:18 == 5 bit Dev_sel type
  // 33:18 == 0 for registers, non-zero for memories
  // 17:0  == PCIe 17:0
  uint64_t x = iaddr;
  x = ((x >> 16) & 0x7C0000) | (x & 0x3FFFF);
  return x & 0xFFFFFFFF;
}

#ifdef __cplusplus
}
#endif /* C++ */

#endif

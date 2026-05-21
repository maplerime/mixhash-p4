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

#ifndef lld_tof3_addr_conversion_h
#define lld_tof3_addr_conversion_h

static inline uint64_t tof3_dir_to_indir_dev_sel(uint32_t daddr) {
  // Indirect Address Format for Device Select
  // 41:39 == PCIe 26:24 == 0
  // 38:34 == PCIe 23:19 == 5 bit Dev_sel type
  // 33:19 == 0 for registers, non-zero for memories
  // 18:0  == PCIe 18:0
  uint64_t x = daddr;
  x = ((x & UINT64_C(0xF80000)) << 15) | (x & UINT64_C(0x7FFFF));
  return x;
}

#endif

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

#ifndef lld_bits_h
#define lld_bits_h

/* Allow the use in C++ code.  */
#ifdef __cplusplus
extern "C" {
#endif

/** BITS64:
 *  Simple access macro to extract a bit field from a 64 bit value.
 * This is useful when processing the DRs. Added here as it is used
 * in several sections below.
 */
#define BITS64(x, hi, lo) ((x << (63LL - hi)) >> (63LL - hi + lo))

#define extract_bit_fld_128(wd1, wd0, hi, lo)                             \
  (hi < 64)                                                               \
      ? ((wd0 << (63 - hi)) >> (63 - hi + lo))                            \
      : (lo > 63)                                                         \
            ? ((wd1 << (63 - (hi - 64))) >> (63 - (hi - 64) + (lo - 64))) \
            : ((wd0 >> lo) |                                              \
               (wd1 << (63 - (hi - 64))) >> ((63 - (hi - 64)) - (64 - lo)))

#define FLD_MSK(hi, lo) (((-1ULL << (63 - hi)) >> (63 - hi + lo)) << lo)
#define insert_bit_fld_128(wd1, wd0, hi, lo, fld)                           \
  {                                                                         \
    if (hi < 64) {                                                          \
      wd0 = ((wd0 & ~(FLD_MSK(hi, lo)))) | ((fld << lo) & FLD_MSK(hi, lo)); \
    }                                                                       \
  }

#ifdef __cplusplus
}
#endif /* C++ */

#endif

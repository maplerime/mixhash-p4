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

/*
 * Helper code and macros used in parsing mau context json.
 */

#ifndef __PIPE_MGR_PARSE_CTX_JSON_H__
#define __PIPE_MGR_PARSE_CTX_JSON_H__

#include <stdint.h>
#include <inttypes.h>
#include <target-utils/bit_utils/bit_utils.h>

static inline uint32_t bob_jenkin_hash_one_at_a_time(int table_depth,
                                                     uint32_t key1,
                                                     uint8_t key2,
                                                     uint8_t key3) {
  /* http://www.burtleburtle.net/bob/hash/doobs.html */
  /* This code is public domain as per the URL */
  /* modified to match key usage of this program */
  uint32_t hash, i;

  for (hash = 0, i = 0; i < 4; ++i) {
    hash += (key1 >> (i * 8)) & 0xff;
    hash += (hash << 10);
    hash ^= (hash >> 6);
  }
  hash += (hash << 3);
  hash ^= (hash >> 11);
  hash += (hash << 15);

  for (i = 0; i < 1; ++i) {
    hash += key2;
    hash += (hash << 10);
    hash ^= (hash >> 6);
  }
  hash += (hash << 3);
  hash ^= (hash >> 11);
  hash += (hash << 15);

  for (i = 0; i < 1; ++i) {
    hash += key3;
    hash += (hash << 10);
    hash ^= (hash >> 6);
  }
  hash += (hash << 3);
  hash ^= (hash >> 11);
  hash += (hash << 15);

  return (hash & (table_depth - 1));
}

#define PIPEMGR_JSON_GETINT(item, key) \
  ((cJSON_GetObjectItem((item), key))->valueint)
#define PIPEMGR_JSON_GETDOUBLE(item, key) \
  ((cJSON_GetObjectItem((item), key))->valuedouble)
#define PIPEMGR_JSON_GETSTR(item, key) \
  ((cJSON_GetObjectItem((item), key))->valuestring)
#define PIPEMGR_JSON_GETBOOL(item, key) \
  (((cJSON_GetObjectItem((item), key))->type == cJSON_True) ? true : false)
#define PIPEMGR_JSON_GETOBJ(item, key) (cJSON_GetObjectItem((item), key))
#define PIPEMGR_JSON_IS_FIELD_STR(item, key) \
  (((cJSON_GetObjectItem((item), key))->type) == cJSON_String)

/* Used by Hash and Entry LUTs */
#define PIPEMGR_TBL_PKG_2POWER(val) (1 << (log2_uint32_ceil(val)))

#endif

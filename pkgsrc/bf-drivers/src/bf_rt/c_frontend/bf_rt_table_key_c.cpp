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

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include <bf_rt/bf_rt_table_key.h>

#ifdef __cplusplus
}
#endif

#include <bf_rt/bf_rt_table_key.hpp>
#include <bf_rt_common/bf_rt_table_key_impl.hpp>

/** Set */
/* Exact */
bf_status_t bf_rt_key_field_set_value(bf_rt_table_key_hdl *key_hdl,
                                      const bf_rt_id_t field_id,
                                      const uint64_t value) {
  auto key = reinterpret_cast<bfrt::BfRtTableKey *>(key_hdl);
  return key->setValue(field_id, value);
}

bf_status_t bf_rt_key_field_set_value_ptr(bf_rt_table_key_hdl *key_hdl,
                                          const bf_rt_id_t field_id,
                                          const uint8_t *value,
                                          const size_t size) {
  auto key = reinterpret_cast<bfrt::BfRtTableKey *>(key_hdl);
  return key->setValue(field_id, value, size);
}

bf_status_t bf_rt_key_field_set_value_string(bf_rt_table_key_hdl *key_hdl,
                                             const bf_rt_id_t field_id,
                                             const char *value) {
  auto key = reinterpret_cast<bfrt::BfRtTableKey *>(key_hdl);
  return key->setValue(field_id, std::string(value));
}

/* Ternary */
bf_status_t bf_rt_key_field_set_value_and_mask(bf_rt_table_key_hdl *key_hdl,
                                               const bf_rt_id_t field_id,
                                               const uint64_t value,
                                               const uint64_t mask) {
  auto key = reinterpret_cast<bfrt::BfRtTableKey *>(key_hdl);
  return key->setValueandMask(field_id, value, mask);
}

bf_status_t bf_rt_key_field_set_value_and_mask_ptr(bf_rt_table_key_hdl *key_hdl,
                                                   const bf_rt_id_t field_id,
                                                   const uint8_t *value1,
                                                   const uint8_t *mask,
                                                   const size_t size) {
  auto key = reinterpret_cast<bfrt::BfRtTableKey *>(key_hdl);
  return key->setValueandMask(field_id, value1, mask, size);
}

/* Range */
bf_status_t bf_rt_key_field_set_value_range(bf_rt_table_key_hdl *key_hdl,
                                            const bf_rt_id_t field_id,
                                            const uint64_t start,
                                            const uint64_t end) {
  auto key = reinterpret_cast<bfrt::BfRtTableKey *>(key_hdl);
  return key->setValueRange(field_id, start, end);
}

bf_status_t bf_rt_key_field_set_value_range_ptr(bf_rt_table_key_hdl *key_hdl,
                                                const bf_rt_id_t field_id,
                                                const uint8_t *start,
                                                const uint8_t *end,
                                                const size_t size) {
  auto key = reinterpret_cast<bfrt::BfRtTableKey *>(key_hdl);
  return key->setValueRange(field_id, start, end, size);
}

/* LPM */
bf_status_t bf_rt_key_field_set_value_lpm(bf_rt_table_key_hdl *key_hdl,
                                          const bf_rt_id_t field_id,
                                          const uint64_t value1,
                                          const uint16_t p_length) {
  auto key = reinterpret_cast<bfrt::BfRtTableKey *>(key_hdl);
  return key->setValueLpm(field_id, value1, p_length);
}

bf_status_t bf_rt_key_field_set_value_lpm_ptr(bf_rt_table_key_hdl *key_hdl,
                                              const bf_rt_id_t field_id,
                                              const uint8_t *value1,
                                              const uint16_t p_length,
                                              const size_t size) {
  auto key = reinterpret_cast<bfrt::BfRtTableKey *>(key_hdl);
  return key->setValueLpm(field_id, value1, p_length, size);
}

/* Optional */
bf_status_t bf_rt_key_field_set_value_optional(bf_rt_table_key_hdl *key_hdl,
                                               const bf_rt_id_t field_id,
                                               const uint64_t value1,
                                               const bool is_valid) {
  auto key = reinterpret_cast<bfrt::BfRtTableKey *>(key_hdl);
  return key->setValueOptional(field_id, value1, is_valid);
}

bf_status_t bf_rt_key_field_set_value_optional_ptr(bf_rt_table_key_hdl *key_hdl,
                                                   const bf_rt_id_t field_id,
                                                   const uint8_t *value1,
                                                   const bool is_valid,
                                                   const size_t size) {
  auto key = reinterpret_cast<bfrt::BfRtTableKey *>(key_hdl);
  return key->setValueOptional(field_id, value1, is_valid, size);
}

/** Get */
/* Exact */
bf_status_t bf_rt_key_field_get_value(const bf_rt_table_key_hdl *key_hdl,
                                      const bf_rt_id_t field_id,
                                      uint64_t *value) {
  auto key = reinterpret_cast<const bfrt::BfRtTableKey *>(key_hdl);
  return key->getValue(field_id, value);
}

bf_status_t bf_rt_key_field_get_value_ptr(const bf_rt_table_key_hdl *key_hdl,
                                          const bf_rt_id_t field_id,
                                          const size_t size,
                                          uint8_t *value) {
  auto key = reinterpret_cast<const bfrt::BfRtTableKey *>(key_hdl);
  return key->getValue(field_id, size, value);
}

bf_status_t bf_rt_key_field_get_value_string_size(
    const bf_rt_table_key_hdl *key_hdl,
    const bf_rt_id_t field_id,
    uint32_t *str_size) {
  auto key = reinterpret_cast<const bfrt::BfRtTableKey *>(key_hdl);
  std::string str;
  bf_status_t status = key->getValue(field_id, &str);
  *str_size = str.size();
  return status;
}

bf_status_t bf_rt_key_field_get_value_string(const bf_rt_table_key_hdl *key_hdl,
                                             const bf_rt_id_t field_id,
                                             char *value) {
  auto key = reinterpret_cast<const bfrt::BfRtTableKey *>(key_hdl);
  std::string str;
  bf_status_t status = key->getValue(field_id, &str);
  strncpy(value, str.c_str(), str.size());
  return status;
}

/* Ternary */
bf_status_t bf_rt_key_field_get_value_and_mask(
    const bf_rt_table_key_hdl *key_hdl,
    const bf_rt_id_t field_id,
    uint64_t *value1,
    uint64_t *value2) {
  auto key = reinterpret_cast<const bfrt::BfRtTableKey *>(key_hdl);
  return key->getValueandMask(field_id, value1, value2);
}

bf_status_t bf_rt_key_field_get_value_and_mask_ptr(
    const bf_rt_table_key_hdl *key_hdl,
    const bf_rt_id_t field_id,
    const size_t size,
    uint8_t *value1,
    uint8_t *value2) {
  auto key = reinterpret_cast<const bfrt::BfRtTableKey *>(key_hdl);
  return key->getValueandMask(field_id, size, value1, value2);
}

/* Range */
bf_status_t bf_rt_key_field_get_value_range(const bf_rt_table_key_hdl *key_hdl,
                                            const bf_rt_id_t field_id,
                                            uint64_t *start,
                                            uint64_t *end) {
  auto key = reinterpret_cast<const bfrt::BfRtTableKey *>(key_hdl);
  return key->getValueRange(field_id, start, end);
}

bf_status_t bf_rt_key_field_get_value_range_ptr(
    const bf_rt_table_key_hdl *key_hdl,
    const bf_rt_id_t field_id,
    const size_t size,
    uint8_t *start,
    uint8_t *end) {
  auto key = reinterpret_cast<const bfrt::BfRtTableKey *>(key_hdl);
  return key->getValueRange(field_id, size, start, end);
}

/* LPM */
bf_status_t bf_rt_key_field_get_value_lpm(const bf_rt_table_key_hdl *key_hdl,
                                          const bf_rt_id_t field_id,
                                          uint64_t *start,
                                          uint16_t *p_length) {
  auto key = reinterpret_cast<const bfrt::BfRtTableKey *>(key_hdl);
  return key->getValueLpm(field_id, start, p_length);
}

bf_status_t bf_rt_key_field_get_value_lpm_ptr(
    const bf_rt_table_key_hdl *key_hdl,
    const bf_rt_id_t field_id,
    const size_t size,
    uint8_t *value1,
    uint16_t *p_length) {
  auto key = reinterpret_cast<const bfrt::BfRtTableKey *>(key_hdl);
  return key->getValueLpm(field_id, size, value1, p_length);
}

/* OPTIONAL */
bf_status_t bf_rt_key_field_get_value_optional(
    const bf_rt_table_key_hdl *key_hdl,
    const bf_rt_id_t field_id,
    uint64_t *value,
    bool *is_valid) {
  auto key = reinterpret_cast<const bfrt::BfRtTableKey *>(key_hdl);
  return key->getValueOptional(field_id, value, is_valid);
}

bf_status_t bf_rt_key_field_get_value_optional_ptr(
    const bf_rt_table_key_hdl *key_hdl,
    const bf_rt_id_t field_id,
    const size_t size,
    uint8_t *value1,
    bool *is_valid) {
  auto key = reinterpret_cast<const bfrt::BfRtTableKey *>(key_hdl);
  return key->getValueOptional(field_id, size, value1, is_valid);
}

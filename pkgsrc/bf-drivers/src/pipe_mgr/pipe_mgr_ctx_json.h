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

/**
 * @file pipe_mgr_ctx_json.h
 *
 * Macro and definitions that are helpful within pipe_mgr for parsing the
 * Context JSON.
 */

#ifndef __PIPE_MGR_CTX_JSON__
#define __PIPE_MGR_CTX_JSON__

#include <stddef.h>
#include <target-utils/third-party/cJSON/cJSON.h>
#include <ctx_json/ctx_json_utils.h>
#include <pipe_mgr/pipe_mgr_err.h>
#include "pipe_mgr_int.h"

#define CHECK_ERR_INTERNAL(err, rc, label, file, line, func)            \
  if ((err)) {                                                          \
    LOG_ERROR(                                                          \
        "%s:%d: An error has occurred while parsing the Context JSON. " \
        "Function %s cannot continue.",                                 \
        (file),                                                         \
        (line),                                                         \
        (func));                                                        \
    rc = PIPE_INVALID_ARG;                                              \
    goto label;                                                         \
  }

#define CHECK_RC_INTERNAL(rc, label, file, line, func)                      \
  if ((rc) != PIPE_SUCCESS) {                                               \
    LOG_ERROR("%s:%d: An error has occurred. Function %s cannot continue.", \
              (file),                                                       \
              (line),                                                       \
              (func));                                                      \
    goto label;                                                             \
  }

#define CHECK_ERR(err, rc, label) \
  CHECK_ERR_INTERNAL(err, rc, label, __FILE__, __LINE__, __func__)
#define CHECK_RC(rc, label) \
  CHECK_RC_INTERNAL(rc, label, __FILE__, __LINE__, __func__)

/*
 * Entry points to context json parsing, entry format, hashes, parser and dkm
 * parsing routines.  */
rmt_dev_tbl_info_t *pipe_mgr_ctx_json_parse(rmt_dev_info_t *dev_info,
                                            profile_id_t prof_id,
                                            const char *config_file_path,
                                            bool virtual_device);
pipe_status_t ctx_json_parse_entry_format(bf_dev_id_t devid,
                                          profile_id_t prof_id,
                                          cJSON *root);
pipe_status_t ctx_json_parse_hashes(bf_dev_id_t devid,
                                    profile_id_t prof_id,
                                    cJSON *root);
pipe_status_t ctx_json_parse_parser(bf_dev_id_t devid,
                                    profile_id_t prof_id,
                                    cJSON *root,
                                    bool virtual_device);
pipe_status_t ctx_json_parse_dkm(rmt_dev_info_t *dev_info,
                                 profile_id_t prof_id,
                                 cJSON *root,
                                 rmt_dev_tbl_info_t *tbl_info);

#endif  // __PIPE_MGR_CTX_JSON__

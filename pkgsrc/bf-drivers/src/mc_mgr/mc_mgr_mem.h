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

bf_status_t mc_mgr_get_bkup_port_reg(bf_dev_id_t dev,
                                     int ver,
                                     int port_bit_idx,
                                     int *bkup_bit_idx);
bf_status_t mc_mgr_set_bkup_port_wrl(
    int sid, bf_dev_id_t dev, int ver, int port_bit_idx, int bkup_bit_idx);

bf_status_t mc_mgr_get_lit_np_reg(
    bf_dev_id_t dev, int ver, int id, int *l_cnt, int *r_cnt);
bf_status_t mc_mgr_set_lit_np_wrl(int sid, bf_dev_id_t dev, int ver, int id);

bf_status_t mc_mgr_get_lit_seg_reg(
    bf_dev_id_t dev, int ver, int id, int seg, bf_bitset_t *val);
bf_status_t mc_mgr_set_lit_wrl(int sid, bf_dev_id_t dev, int ver, int lag_id);

bf_status_t mc_mgr_get_pmt_seg_reg(
    bf_dev_id_t dev, int ver, int yid, int seg, bf_bitset_t *val);
bf_status_t mc_mgr_set_pmt_wrl(int sid, bf_dev_id_t dev, int ver, int yid);

bf_status_t mc_mgr_get_mit_row_reg(bf_dev_id_t dev,
                                   int pipe,
                                   int row,
                                   uint32_t *mit0,
                                   uint32_t *mit1,
                                   uint32_t *mit2,
                                   uint32_t *mit3);
bf_status_t mc_mgr_set_mit_wrl(int sid, bf_dev_id_t dev, int pipe, int mgid);

bf_status_t mc_mgr_get_rdm_reg(
    int sid, bf_dev_id_t dev, int line, uint64_t *hi, uint64_t *lo);
bf_status_t mc_mgr_set_rdm_wrl(
    int sid, bf_dev_id_t dev, int line, uint64_t hi, uint64_t lo);

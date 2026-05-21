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
        pipe_manager pkt_path (pp) thrift file
*/

namespace py pipemgr_api_rpc
namespace cpp pipemgr_api_rpc
namespace c_glib pipemgr_api_rpc

typedef i32 pp_dev_t
typedef i32 pp_pipe_t
typedef i32 pp_bf_status_t
typedef i32 pp_bf_dev_id_t
typedef i32 pp_port_t


exception InvalidPpOperation {
  1:i32 code
}

/*
 *                  IBUF Counters per PIPE per Port
 */

struct pp_bf_ibp_drop_cntrs {
  1:  i64 total_pkts_drop
  2:  i64 total_pkts_disc
  3:  i64 total_recirc_pkt_disc
  4:  i64 total_prsr_pkt_disc

}

/*
 *                  IPRSR Counters per PIPE per Port
 */

struct pp_bf_iprsr_drop_cntrs {
  1:  i64 fcs_err_count
  2:  i64 csum_err_count
  3:  i64 tcam_parity_err_count
  4:  i64 total_pkts_drop

}

/*
 *                  IDPRSR Counters per PIPE per Port
 */

struct pp_bf_idprsr_drop_cntrs {
  1:  i64 pkts_disc
  2:  i64 pkts_disc_at_tm
  3:  i64 err_pkts_to_tm
  4:  i64 err_pkts_to_ictm
  5:  list<i64> crc_err

}

/*
 *                  EPRSR Counters per PIPE per Port
 */

struct pp_bf_eprsr_drop_cntrs {
  1:  i64 fcs_err_count
  2:  i64 csum_err_count
  3:  i64 tcam_parity_err_count
  4:  i64 total_pkts_drop

}

/*
 *                  EDPRSR Counters per PIPE
 */

struct pp_bf_edprsr_drop_cntrs {
  1:  i64 pkts_disc
  2:  i64 err_pkts_to_ebuf
  3:  i64 err_pkts_to_ectm
  4:  list<i64> crc_err

}

service pipemgr_api {
    # Pipe Maganger Packet Path Get Drops, Errors, Discards
    pp_bf_ibp_drop_cntrs    pp_ibp_drop_cntr_get(1:pp_dev_t dev, 2:pp_port_t port) throws (1:InvalidPpOperation ouch);
    pp_bf_iprsr_drop_cntrs    pp_iprsr_drop_cntr_get(1:pp_dev_t dev, 2:pp_port_t port) throws (1:InvalidPpOperation ouch);
    pp_bf_idprsr_drop_cntrs   pp_idprsr_drop_cntr_get(1:pp_dev_t dev, 2:pp_pipe_t pipe) throws (1:InvalidPpOperation ouch);
    pp_bf_eprsr_drop_cntrs    pp_eprsr_drop_cntr_get(1:pp_dev_t dev, 2:pp_port_t port) throws (1:InvalidPpOperation ouch);
    pp_bf_edprsr_drop_cntrs   pp_edprsr_drop_cntr_get(1:pp_dev_t dev, 2:pp_pipe_t pipe) throws (1:InvalidPpOperation ouch);

}

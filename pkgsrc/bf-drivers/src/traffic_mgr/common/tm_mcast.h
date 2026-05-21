/*!
 * @fixme Standard Barefoot Networks Copyright Notice
 */

/*
 *    This file contains all data strcutures and parameters
 *    related to port
 */

#ifndef __TM_MCAST_H__
#define __TM_MCAST_H__

#include <stdint.h>
#include <stdbool.h>
#include "tm_ctx.h"

typedef struct _bf_tm_mcast {
  uint8_t fifo;      // Identfies mcast fifo in TM
  uint8_t l_pipe;    // logical pipe
  uint8_t phy_pipe;  // pipe to which fifo belongs to.
  bool arb_mode;     // True = strict prio, False = wrr
  uint8_t weight;
  uint8_t icos_bmap;
  uint16_t size;  // fifo depth/size in units of 8 cells
} bf_tm_mcast_t;

bf_status_t bf_tm_mcast_set_fifo_arb_mode(bf_dev_id_t, uint8_t, int, bool);
bf_status_t bf_tm_mcast_get_fifo_arb_mode(
    bf_dev_id_t, bf_dev_pipe_t, int, bool *, bool *);
bf_status_t bf_tm_mcast_set_fifo_wrr_weight(bf_dev_id_t, uint8_t, int, uint8_t);
bf_status_t bf_tm_mcast_get_fifo_wrr_weight(
    bf_dev_id_t, bf_dev_pipe_t, int, uint8_t *, uint8_t *);
bf_status_t bf_tm_mcast_set_fifo_icos_mapping(bf_dev_id_t,
                                              uint8_t,
                                              int,
                                              uint8_t);
bf_status_t bf_tm_mcast_get_fifo_icos_mapping(
    bf_dev_id_t, bf_dev_pipe_t, int, uint8_t *, uint8_t *);
bf_status_t bf_tm_mcast_set_fifo_depth(bf_dev_id_t, uint8_t, int, uint16_t);
bf_status_t bf_tm_mcast_get_fifo_depth(
    bf_dev_id_t, bf_dev_pipe_t, int, uint16_t *, uint16_t *);

typedef struct _bf_tm_mcast_fifo {
  uint8_t fifo;
  bf_dev_pipe_t phy_pipe;
  uint32_t arb_mode;
  uint32_t weight;
  uint32_t icos_bmap;
  uint32_t size;
} bf_tm_mcast_fifo_t;

/*
 * Function pointers to program HW/ASIC.
 * Depending on ASIC version/type populate correct read/write fptr.
 */
typedef bf_tm_status_t (*bf_tm_mcast_wr_fptr)(bf_dev_id_t,
                                              bf_tm_mcast_fifo_t *);
typedef bf_tm_status_t (*bf_tm_mcast_rd_fptr)(bf_dev_id_t,
                                              bf_tm_mcast_fifo_t *);

typedef struct _bf_tm_mcast_hw_funcs {
  bf_tm_mcast_wr_fptr fifo_prio_wr_fptr;
  bf_tm_mcast_rd_fptr fifo_prio_rd_fptr;
  bf_tm_mcast_wr_fptr fifo_weight_wr_fptr;
  bf_tm_mcast_rd_fptr fifo_weight_rd_fptr;
  bf_tm_mcast_wr_fptr fifo_icosmap_wr_fptr;
  bf_tm_mcast_rd_fptr fifo_icosmap_rd_fptr;
  bf_tm_mcast_wr_fptr fifo_depth_wr_fptr;
  bf_tm_mcast_rd_fptr fifo_depth_rd_fptr;

} bf_tm_mcast_hw_funcs_tbl;

#endif

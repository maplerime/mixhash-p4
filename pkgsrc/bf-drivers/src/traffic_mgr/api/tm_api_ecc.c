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

#include <traffic_mgr/traffic_mgr_types.h>
#include <tof2_regs/tof2_mem_addr.h>
#include <tof3_regs/tof3_mem_addr.h>
#include "traffic_mgr/common/tm_ctx.h"
#include "traffic_mgr/common/tm_hw_access.h"

bf_status_t bf_tm_ecc_correct_addr(bf_dev_id_t dev, uint64_t addr) {
  bf_status_t rc;
  uint64_t hi = 0, lo = 0;

  BF_TM_INVALID_ARG(TM_IS_DEV_INVALID(dev));
  TM_LOCK(dev, g_tm_ctx[dev]->lock);
  bf_tm_complete_ops(dev);
  rc = bf_tm_read_memory(dev, addr, &hi, &lo);
  if (rc != BF_SUCCESS) {
    TM_UNLOCK(dev, g_tm_ctx[dev]->lock);
    return rc;
  }
  rc = bf_tm_write_memory(dev, addr, 16, hi, lo);
  if (rc != BF_SUCCESS) {
    TM_UNLOCK(dev, g_tm_ctx[dev]->lock);
    return rc;
  }
  TM_UNLOCK_AND_FLUSH(dev);
  return (rc);
}

bf_status_t bf_tm_ecc_correct_wac_ppg_map(bf_dev_id_t dev,
                                          bf_dev_pipe_t pipe,
                                          uint32_t l_addr) {
  uint64_t addr =
      tof2_mem_tm_tm_wac_wac_pipe_mem_csr_memory_wac_port_ppg_mapping(pipe,
                                                                      l_addr);
  return bf_tm_ecc_correct_addr(dev, addr);
}

bf_status_t bf_tm_ecc_correct_wac_qid_map(bf_dev_id_t dev,
                                          bf_dev_pipe_t pipe,
                                          uint32_t l_addr) {
  uint64_t addr =
      tof2_mem_tm_tm_wac_wac_pipe_mem_csr_memory_wac_qid_map(pipe, l_addr);
  return bf_tm_ecc_correct_addr(dev, addr);
}

bf_status_t bf_tm_ecc_correct_qac_qid_map(bf_dev_id_t dev,
                                          bf_dev_pipe_t pipe,
                                          uint32_t l_addr) {
  uint64_t addr =
      tof2_mem_tm_tm_qac_qac_pipe_mem_csr_memory_qac_qid_mapping(pipe, l_addr);
  return bf_tm_ecc_correct_addr(dev, addr);
}

bf_status_t bf_tm_ecc_correct_sch_q_minrate(bf_dev_id_t dev,
                                            bf_dev_pipe_t pipe,
                                            bool dyn,
                                            uint32_t l_addr) {
  uint64_t addr;
  if (pipe == 0 || pipe == 1) {
    if (dyn) {
      addr =
          tof2_mem_tm_tm_scha_sch_pipe_mem_q_min_lb_dynamic_mem(pipe, l_addr);
    } else {
      addr = tof2_mem_tm_tm_scha_sch_pipe_mem_q_min_lb_static_mem(pipe, l_addr);
    }
  } else if (pipe == 2 || pipe == 3) {
    if (dyn) {
      addr =
          tof2_mem_tm_tm_schb_sch_pipe_mem_q_min_lb_dynamic_mem(pipe, l_addr);
    } else {
      addr = tof2_mem_tm_tm_schb_sch_pipe_mem_q_min_lb_static_mem(pipe, l_addr);
    }
  } else {
    return BF_INVALID_ARG;
  }
  return bf_tm_ecc_correct_addr(dev, addr);
}

bf_status_t bf_tm_ecc_correct_sch_q_excrate(bf_dev_id_t dev,
                                            bf_dev_pipe_t pipe,
                                            bool dyn,
                                            uint32_t l_addr) {
  uint64_t addr;
  if (pipe == 0 || pipe == 1) {
    if (dyn) {
      addr = tof2_mem_tm_tm_scha_sch_pipe_mem_q_exc_dynamic_mem(pipe, l_addr);
    } else {
      addr = tof2_mem_tm_tm_scha_sch_pipe_mem_q_exc_static_mem(pipe, l_addr);
    }
  } else if (pipe == 2 || pipe == 3) {
    if (dyn) {
      addr = tof2_mem_tm_tm_schb_sch_pipe_mem_q_exc_dynamic_mem(pipe, l_addr);
    } else {
      addr = tof2_mem_tm_tm_schb_sch_pipe_mem_q_exc_static_mem(pipe, l_addr);
    }
  } else {
    return BF_INVALID_ARG;
  }
  return bf_tm_ecc_correct_addr(dev, addr);
}

bf_status_t bf_tm_ecc_correct_sch_q_maxrate(bf_dev_id_t dev,
                                            bf_dev_pipe_t pipe,
                                            bool dyn,
                                            uint32_t l_addr) {
  uint64_t addr;
  if (pipe == 0 || pipe == 1) {
    if (dyn) {
      addr =
          tof2_mem_tm_tm_scha_sch_pipe_mem_q_max_lb_dynamic_mem(pipe, l_addr);
    } else {
      addr = tof2_mem_tm_tm_scha_sch_pipe_mem_q_max_lb_static_mem(pipe, l_addr);
    }
  } else if (pipe == 2 || pipe == 3) {
    if (dyn) {
      addr =
          tof2_mem_tm_tm_schb_sch_pipe_mem_q_max_lb_dynamic_mem(pipe, l_addr);
    } else {
      addr = tof2_mem_tm_tm_schb_sch_pipe_mem_q_max_lb_static_mem(pipe, l_addr);
    }
  } else {
    return BF_INVALID_ARG;
  }
  return bf_tm_ecc_correct_addr(dev, addr);
}

bf_status_t bf_tm_ecc_correct_sch_l1_minrate(bf_dev_id_t dev,
                                             bf_dev_pipe_t pipe,
                                             bool dyn,
                                             uint32_t l_addr) {
  uint64_t addr;
  if (pipe == 0 || pipe == 1) {
    if (dyn) {
      addr =
          tof2_mem_tm_tm_scha_sch_pipe_mem_l1_min_lb_dynamic_mem(pipe, l_addr);
    } else {
      addr =
          tof2_mem_tm_tm_scha_sch_pipe_mem_l1_min_lb_static_mem(pipe, l_addr);
    }
  } else if (pipe == 2 || pipe == 3) {
    if (dyn) {
      addr =
          tof2_mem_tm_tm_schb_sch_pipe_mem_l1_min_lb_dynamic_mem(pipe, l_addr);
    } else {
      addr =
          tof2_mem_tm_tm_schb_sch_pipe_mem_l1_min_lb_static_mem(pipe, l_addr);
    }
  } else {
    return BF_INVALID_ARG;
  }
  return bf_tm_ecc_correct_addr(dev, addr);
}

bf_status_t bf_tm_ecc_correct_sch_l1_excrate(bf_dev_id_t dev,
                                             bf_dev_pipe_t pipe,
                                             bool dyn,
                                             uint32_t l_addr) {
  uint64_t addr;
  if (pipe == 0 || pipe == 1) {
    if (dyn) {
      addr = tof2_mem_tm_tm_scha_sch_pipe_mem_l1_exc_dynamic_mem(pipe, l_addr);
    } else {
      addr = tof2_mem_tm_tm_scha_sch_pipe_mem_l1_exc_static_mem(pipe, l_addr);
    }
  } else if (pipe == 2 || pipe == 3) {
    if (dyn) {
      addr = tof2_mem_tm_tm_schb_sch_pipe_mem_l1_exc_dynamic_mem(pipe, l_addr);
    } else {
      addr = tof2_mem_tm_tm_schb_sch_pipe_mem_l1_exc_static_mem(pipe, l_addr);
    }
  } else {
    return BF_INVALID_ARG;
  }
  return bf_tm_ecc_correct_addr(dev, addr);
}

bf_status_t bf_tm_ecc_correct_sch_l1_maxrate(bf_dev_id_t dev,
                                             bf_dev_pipe_t pipe,
                                             bool dyn,
                                             uint32_t l_addr) {
  uint64_t addr;
  if (pipe == 0 || pipe == 1) {
    if (dyn) {
      addr =
          tof2_mem_tm_tm_scha_sch_pipe_mem_l1_max_lb_dynamic_mem(pipe, l_addr);
    } else {
      addr =
          tof2_mem_tm_tm_scha_sch_pipe_mem_l1_max_lb_static_mem(pipe, l_addr);
    }
  } else if (pipe == 2 || pipe == 3) {
    if (dyn) {
      addr =
          tof2_mem_tm_tm_schb_sch_pipe_mem_l1_max_lb_dynamic_mem(pipe, l_addr);
    } else {
      addr =
          tof2_mem_tm_tm_schb_sch_pipe_mem_l1_max_lb_static_mem(pipe, l_addr);
    }
  } else {
    return BF_INVALID_ARG;
  }
  return bf_tm_ecc_correct_addr(dev, addr);
}

bf_status_t bf_tm_ecc_correct_sch_p_maxrate(bf_dev_id_t dev,
                                            bf_dev_pipe_t pipe,
                                            bool dyn,
                                            uint32_t l_addr) {
  uint64_t addr;
  if (pipe == 0 || pipe == 1) {
    if (dyn) {
      addr = tof2_mem_tm_tm_scha_sch_pipe_mem_port_max_lb_dynamic_mem(pipe,
                                                                      l_addr);
    } else {
      addr =
          tof2_mem_tm_tm_scha_sch_pipe_mem_port_max_lb_static_mem(pipe, l_addr);
    }
  } else if (pipe == 2 || pipe == 3) {
    if (dyn) {
      addr = tof2_mem_tm_tm_schb_sch_pipe_mem_port_max_lb_dynamic_mem(pipe,
                                                                      l_addr);
    } else {
      addr =
          tof2_mem_tm_tm_schb_sch_pipe_mem_port_max_lb_static_mem(pipe, l_addr);
    }
  } else {
    return BF_INVALID_ARG;
  }
  return bf_tm_ecc_correct_addr(dev, addr);
}


#define TILE_SIM
#include <assert.h>
#include <byteswap.h>
#include <stdarg.h>

#define bf_dev_port_t uint32_t
#define bf_dev_id_t uint32_t
#define bf_status_t uint32_t
#define port_mgr_err_t uint32_t
#define bf_clkobs_pad_t uint32_t
#define bf_sds_clkobs_clksel_t uint32_t
#define BF_SUCCESS 0
#define BF_INVALID_ARG 3
#define BF_HW_COMM_FAIL 99
#define port_mgr_log fpga_printf
#define bf_sys_usleep usleep
#define PORT_MGR_OK 0
#define port_mgr_tof2_prbs_mode_t uint32_t

//(((pipe) << 7) | (port))
#define MAKE_DEV_PORT(pipe_id, port_id) (pipe_id * 72 + port_id)

typedef enum {
  BF_SERDES_ENC_MODE_NONE = 0,
  BF_SERDES_ENC_MODE_NRZ,
  BF_SERDES_ENC_MODE_PAM4,
} bf_serdes_encoding_mode_t;

typedef enum bf_port_prbs_mode_e {
  BF_PORT_PRBS_MODE_31 = 0,
  BF_PORT_PRBS_MODE_23,
  BF_PORT_PRBS_MODE_15,
  BF_PORT_PRBS_MODE_13,
  BF_PORT_PRBS_MODE_11,
  BF_PORT_PRBS_MODE_9,
  BF_PORT_PRBS_MODE_7,
  BF_PORT_PRBS_MODE_NONE,  // "mission mode"
  BF_PORT_PRBS_MODE_MAX
} bf_port_prbs_mode_t;

typedef enum bf_port_speed_e {
  BF_SPEED_NONE = 0,
  BF_SPEED_1G = (1 << 0),
  BF_SPEED_10G = (1 << 1),
  BF_SPEED_25G = (1 << 2),
  BF_SPEED_40G = (1 << 3),
  BF_SPEED_50G = (1 << 5),
  BF_SPEED_100G = (1 << 6),

  // Tof2 speeds
  BF_SPEED_200G = (1 << 7),
  BF_SPEED_400G = (1 << 8),
  BF_SPEED_40G_R2 = (1 << 9),    /* 40G 2x NRZ, Non-standard speed */
  BF_SPEED_50G_CONS = (1 << 10), /* 50G 2x25G NRZ, Consortium mode */

} bf_port_speed_t;

int fpga_printf(const char *fmt, ...) {
  va_list args;
  va_start(args, fmt);
  vprintf(fmt, args);
  printf("%s", "\n");
  va_end(args);
}

extern void tile_sim_rd(uint32_t mac_stn_id, uint32_t addr, uint32_t *data);
extern void tile_sim_wr(uint32_t mac_stn_id, uint32_t addr, uint32_t data);

void lld_read_register(bf_dev_id_t dev_id,
                       uint32_t bfn_addr,
                       uint32_t *reg_val) {
  // bfn_addr = (3 << 24) | (mac_stn_id << 18) | bfn_ofs;
  // printf("lld_read_register: dev_id=%d : bfn_addr = %08x\n", dev_id,
  // bfn_addr);
  uint32_t mac = (bfn_addr >> 18) & 0x3f;
  uint32_t addr = (bfn_addr >> 2) & 0xffff;
  tile_sim_rd(mac, addr, reg_val);
}

void lld_write_register(bf_dev_id_t dev_id,
                        uint32_t bfn_addr,
                        uint32_t reg_val) {
  // printf("lld_write_register: dev_id=%d : bfn_addr = %08x : reg_val =
  // %08x\n", dev_id, bfn_addr, reg_val);
  uint32_t mac = (bfn_addr >> 18) & 0x3f;
  uint32_t addr = (bfn_addr >> 2) & 0xffff;
  tile_sim_wr(mac, addr, reg_val);
}

bf_status_t port_mgr_tof2_map_dev_port_to_all(bf_dev_id_t dev_id,
                                              bf_dev_port_t dev_port,
                                              uint32_t *pipe_id,
                                              uint32_t *port_id,
                                              uint32_t *umac,
                                              uint32_t *ch,
                                              bool *is_cpu_port) {
  if (pipe_id) *pipe_id = dev_port >> 8;  // fixme
  if (port_id) *port_id = dev_port % 8;
  if (umac) *umac = dev_port / 8;
  if (ch) *ch = dev_port % 8;
  if (is_cpu_port) *is_cpu_port = (*umac == 0);

  return 0;
}

typedef struct port_mgr_tof2_serdes_t {
  // Tx EQ parameters
  // (note: values are signed)
  int32_t pre2;
  int32_t pre1;
  int32_t main;
  int32_t post1;
  int32_t post2;

  // Tx polarity
  bool tx_inv;

  // Rx polarity
  bool rx_inv;
} port_mgr_tof2_serdes_t;
port_mgr_tof2_serdes_t default_sd[33 * 8 + 8];

port_mgr_tof2_serdes_t *port_mgr_tof2_map_dev_port_lane_to_serdes(
    bf_dev_id_t dev_id, bf_dev_port_t dev_port, uint32_t ln) {
  return &default_sd[dev_port + ln];
}

typedef struct port_mgr_umac4_t {
  // serdes lane map (i.e. swizzling)
  uint32_t phys_tx_ln[8];
  uint32_t phys_rx_ln[8];
} port_mgr_umac4_t;

typedef struct port_mgr_tof2_pdev_t {
  port_mgr_umac4_t umac4[33];
} port_mgr_tof2_pdev_t;

port_mgr_tof2_pdev_t default_dev[1];

port_mgr_tof2_pdev_t *port_mgr_dev_physical_dev_tof2_get(bf_dev_id_t dev_id) {
  return &default_dev[dev_id];
}

bf_status_t bf_serdes_encoding_mode_get(bf_port_speed_t speed,
                                        uint32_t n_lanes,
                                        bf_serdes_encoding_mode_t *enc_mode) {
  *enc_mode = BF_SERDES_ENC_MODE_NONE;

  switch (speed) {
    case BF_SPEED_400G:
      *enc_mode = BF_SERDES_ENC_MODE_PAM4;
      return BF_SUCCESS;
      break;
    case BF_SPEED_200G:
      if (n_lanes == 8) {
        *enc_mode = BF_SERDES_ENC_MODE_NRZ;
        return BF_SUCCESS;
      } else if (n_lanes == 4) {
        *enc_mode = BF_SERDES_ENC_MODE_PAM4;
        return BF_SUCCESS;
      }
      break;
    case BF_SPEED_100G:
      if (n_lanes == 2) {
        *enc_mode = BF_SERDES_ENC_MODE_PAM4;
        return BF_SUCCESS;
      } else if (n_lanes == 4) {
        *enc_mode = BF_SERDES_ENC_MODE_NRZ;
        return BF_SUCCESS;
      }
      break;
    case BF_SPEED_50G:
      if (n_lanes == 1) {
        *enc_mode = BF_SERDES_ENC_MODE_PAM4;
        return BF_SUCCESS;
      } else if (n_lanes == 2) {
        *enc_mode = BF_SERDES_ENC_MODE_NRZ;
        return BF_SUCCESS;
      }
      break;
    case BF_SPEED_40G_R2:
    case BF_SPEED_40G:
    case BF_SPEED_25G:
    case BF_SPEED_10G:
    case BF_SPEED_1G:
      *enc_mode = BF_SERDES_ENC_MODE_NRZ;
      return BF_SUCCESS;
    case BF_SPEED_NONE:
      return BF_SUCCESS;
  }
  return BF_INVALID_ARG;
}

#include "../../port_mgr_tof2/port_mgr_tof2_serdes_defs.h"
#include "../../port_mgr_tof2/credo_sd_access.c"
#include "../../port_mgr_tof2/port_mgr_tof2_serdes.c"
#include "../../port_mgr_tof2/bf_tof2_serdes_if.c"

bf_status_t port_mgr_tof2_port_lane_map_set(bf_dev_id_t dev_id,
                                            bf_dev_port_t dev_port,
                                            uint32_t phys_tx_ln[8],
                                            uint32_t phys_rx_ln[8]) {
  port_mgr_tof2_pdev_t *dev_p = port_mgr_dev_physical_dev_tof2_get(dev_id);
  int ln;
  uint32_t umac;
  bf_status_t rc;

  port_mgr_tof2_map_dev_port_to_all(
      dev_id, dev_port, NULL, NULL, &umac, NULL, NULL);

  for (ln = 0; ln < 8; ln++) {
    dev_p->umac4[umac].phys_tx_ln[ln] = phys_tx_ln[ln];
    dev_p->umac4[umac].phys_rx_ln[ln] = phys_rx_ln[ln];
  }

  // set lane map in serdes
  rc = port_mgr_tof2_serdes_lane_map_set(
      dev_id, dev_port, phys_tx_ln, phys_rx_ln);
  return rc;
}

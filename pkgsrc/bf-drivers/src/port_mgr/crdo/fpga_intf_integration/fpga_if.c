/* clang-format off */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <signal.h>
#include <fcntl.h>
#include <ctype.h>
#include <termios.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <time.h>

#include <inttypes.h>  //strlen
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <arpa/inet.h>  //inet_addr
#include <unistd.h>     //write
#include <stddef.h>
#include <stdbool.h>

#include "driver_files.h"
#include "newport_board_map.h"
#include <sys/time.h>

void log_timeval(struct timeval *tm) {
  char tbuf[256] = {0};
  char ubuf[256] = {0};
  struct tm *loctime;

  loctime = localtime(&tm->tv_sec);

  strftime(tbuf, sizeof(tbuf), "%a %b %d", loctime);
  printf("%s ", tbuf);

  strftime(ubuf, sizeof(ubuf), "%T\n", loctime);
  ubuf[strlen(ubuf) - 1] = 0;  // remove CR
  printf("%s.%06d : ", ubuf, (int)tm->tv_usec);
}

void get_time(struct timeval *tv) {
  gettimeofday(tv, NULL);
}

char *_log_time(void) {
  struct timeval tv;
  get_time(&tv);
  log_timeval(&tv);
}

char *fw_file_name = "../../crdo/blue-jay/Credo-SDK/bfn_BlueJay_SDK_Package_20190504/TestScript/bluejay.fw.1.00.08.bin";
char *fw_grp8_file_name = "../../crdo/blue-jay/Credo-SDK/bfn_BlueJay_SDK_Package_20190504/TestScript/bluejay_nrz.fw.1.00.08.bin";

bool force_fw_dnld = false;

//#define TILE_INIT_MODE (0 /*NRZ*/)
#define TILE_INIT_MODE (1 /*PAM4*/)
bool tile_init_mode_nrz(void) {
  return (TILE_INIT_MODE == 0);
}

#define DEFAULT_PAM4_SPEED BF_SPEED_400G
#define DEFAULT_NRZ_SPEED BF_SPEED_100G
//#define DEFAULT_PAM4_SPEED BF_SPEED_NONE
//#define DEFAULT_NRZ_SPEED  BF_SPEED_NONE

uint32_t dwell_us = 10000;
 
#define GROUP9__TILE_0_MAC 35
#define GROUP9__TILE_1_MAC 36
#define GROUP9__TILE_2_MAC 37
#define GROUP9__TILE_3_MAC 38

//define default Tx EQ settings (change based on cable/peer)
#define default_pre2    2
#define default_pre1  -10
#define default_main   20
#define default_post1   0
#define default_post2   0

#define ERR_LOG \
	do { \
		fprintf(stderr, "Error at line %d, file %s (%d) [%s]\n", \
		__LINE__, __FILE__, errno, strerror(errno)); exit(1); \
	} while(0)

void tile_sim_map_mac_to_tile_and_grp(uint32_t mac_stn_id, uint32_t *tile, uint32_t *grp);

int reg_chnl = 0;
int dbg_print = 1;
int detailed_dbg_print = 0;
void *map_base, *virt_addr;
uint8_t our_fake_address_space[64*1024*2] = {0};
int proc_server(void);

int log_timestamps = 0;

void log_time(char *why) {
  struct timespec ts;
  clock_gettime(CLOCK_MONOTONIC, &ts);
  printf("%lld.%.9ld : %s", (long long)ts.tv_sec, ts.tv_nsec, why);
}

#define FPGA_SIGNATURE 0x0008

#define FPGA_TILE_0 0x22000
#define FPGA_TILE_1 0x23000
#define FPGA_TILE_2 0x24000
#define FPGA_TILE_3 0x25000

uint32_t tile_offset[4] = { FPGA_TILE_0,
                            FPGA_TILE_1,
                            FPGA_TILE_2,
                            FPGA_TILE_3 };
int active_tile = 0;
int active_group = 0;

typedef enum {
  FPGA_CMD_WRITE= 1,
  FPGA_CMD_READ = 2,
  FPGA_CMD_RST  = 3,
  FPGA_CMD_TCK  = 4, // update jtag clock (speed)
} fpga_ctrlr_cmd_e;

#define FPGA_STATUS_DONE  (1 << 16)
#define FPGA_STATUS_ERROR (2 << 16)

typedef struct {
  uint32_t r0_ctrl_status; // [15:0] cmd, [31:16] status
  uint32_t r1_group; // 0-8, 9=top
  uint32_t r2_addr;  // [15:0] offset 
  uint32_t r3_data;  // [15:0] data to write/read
  uint32_t r15_tile_reset;  // [15:0] data to write/read
} fpga_jtag_to_mdio_ctrl_t;

uintptr_t fpga_ctrlr_base_address = 0;
fpga_jtag_to_mdio_ctrl_t *fpga_ctrlr;

fpga_jtag_to_mdio_ctrl_t *fpga_ctrlr_set(void) {
  fpga_ctrlr = (fpga_jtag_to_mdio_ctrl_t *)(uintptr_t)(fpga_ctrlr_base_address + tile_offset[ active_tile ]);
  return fpga_ctrlr;
}

void fpga_signature(void) {
  uintptr_t sig_p = (uintptr_t)(fpga_ctrlr_base_address + FPGA_SIGNATURE);
  uint32_t sig = *((uint32_t *)sig_p);

  printf("FPGA signature: %08x (%u)\n", sig, sig);
}

void fpga_active_tile_set(int tile) {
  if (tile < 4) {
    active_tile = tile;
  } else {
    active_tile = 0;
  }

  fpga_ctrlr_set(); // update ptr
}

int fpga_wait_done(int max_tmout) {
  int tmout = max_tmout;
  uint32_t sts;
  uint32_t raw_sts;

  do {
    raw_sts = (volatile uint32_t)fpga_ctrlr->r0_ctrl_status;

    sts = raw_sts & 0xFFFF0000;
  } while ((sts == 0) && (tmout-- > 0));

  //printf("fpga_ctrlr=%p : status=0x%08x\n", fpga_ctrlr, raw_sts);

  if (sts == FPGA_STATUS_DONE) {
    return 0;
  } else if (sts == FPGA_STATUS_ERROR) {
    printf("..error.. ");
    return -1;
  } else if (tmout <= 0) {
    printf("..timeout..[%d] ", max_tmout);
    return -2;
  }
  printf("..ERROR..[sts=%08x, tmout=%d] ", raw_sts, tmout);
  exit(1);
  //return -3;
}

#define MAX_TILE 4
void fpga_ctrlr_reset(void) {
  int tmout = 100000;
  int tile;

  for (tile = 0; tile < MAX_TILE; tile++) {
    fpga_active_tile_set(tile);
    printf("fpga_ctrlr reset tile %d..\n", tile);
    fpga_ctrlr->r15_tile_reset = 0x0;
  }
  sleep(1);
  for (tile = 0; tile < MAX_TILE; tile++) {
    fpga_active_tile_set(tile);
    printf("fpga_ctrlr un-reset tile ..\n");
    fpga_ctrlr->r15_tile_reset = 0xf;
  }
  sleep(1);
  for (tile = 0; tile < MAX_TILE; tile++) {
    fpga_active_tile_set(tile);
    fpga_ctrlr->r0_ctrl_status = FPGA_CMD_RST;
  }
  sleep(1);
  printf("Set MDIO speed to 2Mhz (divider=30)\n");
  for (tile = 0; tile < MAX_TILE; tile++) {
    fpga_active_tile_set(tile);
    fpga_ctrlr->r3_data = 30; // 2Mhz (62.5Mhz/(div+2)
    fpga_ctrlr->r0_ctrl_status = FPGA_CMD_TCK;
  }
  sleep(1);
  fpga_signature();
}

void banner(char* str) {
  printf("#############################################\n");
  printf("# %s\n", str);
  printf("#############################################\n");
}

void fw_serdes_params(uint32_t fp) {
  uint32_t tile, grp, ln;
  bf_status_t rc;
  bool loaded;
  bf_dev_id_t dev_id;
  bf_dev_port_t dev_port;
  char *line_separator;

  dev_id = 0;
  dev_port = (fp*8);
  tile_sim_map_mac_to_tile_and_grp(fp, &tile, &grp);

  rc = port_mgr_tof2_serdes_fw_loaded_get(dev_id, dev_port, &loaded);
  if (rc || !loaded) {
    printf("\n######## FW is Not Loaded ! #########\n");
    return;
  }
  line_separator = "\n#+-------------------------------------------------------------------------------------------------------------------------------------------------------------------+";
  printf("%s", line_separator);
  printf(
        "\n#|   |   |    |     |     |    COUNTERS     |SD,Rdy,| FRQ |  CHANNEL   |      CTLE     |   |   | EYE MARGIN  |         DFE       | TIMING  |           FFE Taps      |");
  printf(
        "\n#|Dev|Grp|Lane| Mode|Speed| Adp ,ReAdp,LLost|AdpDone| PPM | Est ,OF,HF |Peaking, G1,G2 |SK |DAC|  1 , 2 , 3  | F0 , F1 ,F1/F0,F13|Del,Edge | K1 , K2 , K3 , K4 ,S1,S2|");
  printf("%s", line_separator);

  for (ln = 0; ln < 8; ln++) {
    uint32_t G, adapt, readapt, link_lost, ppm, of, hf;
    uint32_t skef_val, dac_val;
    uint32_t ctle_over_val, ctle_map_0, ctle_map_1, ctle_1, ctle_2, dc_gain_1, dc_gain_2;
    uint32_t delta, edge1, edge2, edge3, edge4;
    uint32_t tap1, tap2, tap3, f13_val;
    int signed_ppm, signed7_delta;
    bf_serdes_encoding_mode_t enc_mode;
    bool sig_detect, phy_ready, adapt_done;
    char *mode_str[] = {"NONE", "NRZ ", "PAM4"};
    float chan_est, eye_1, eye_2, eye_3;
    float f0, f1, ratio;
    char *sd_flag;

    tile_sim_map_mac_to_tile_and_grp(fp, &tile, &grp);

    rc = port_mgr_tof2_serdes_fw_lane_speed_get(dev_id, dev_port, ln, &G, &enc_mode);
    if (rc) {
      printf("port_mgr_tof2_serdes_fw_lane_speed_get: rtnd: %d\n", rc);
      return;
    }
    if (G == 0) { // OFF
      printf("\n#| %d | %d |  %d |%5s| %3dG|", tile, grp, ln, mode_str[enc_mode], G);
      //printf("|                 |       |     |       |            |               |   |   |             |                   |         |                         |");
      continue;
    }
    rc = port_mgr_tof2_serdes_fw_adapt_cnt_get(dev_id, dev_port, ln, &adapt);
    if (rc != BF_SUCCESS) continue;
    rc = port_mgr_tof2_serdes_fw_readapt_cnt_get(dev_id, dev_port, ln, &readapt);
    if (rc != BF_SUCCESS) continue;
    rc = port_mgr_tof2_serdes_fw_link_lost_cnt_get(dev_id, dev_port, ln, &link_lost);
    if (rc != BF_SUCCESS) continue;
    rc = port_mgr_tof2_serdes_sig_detect_get(dev_id, dev_port, ln, &sig_detect, &phy_ready);
    if (rc != BF_SUCCESS) continue;
    sd_flag = sig_detect ? " ":"*";
    rc = port_mgr_tof2_serdes_adapt_done_get(dev_id, dev_port, ln, &adapt_done);
    if (rc != BF_SUCCESS) continue;

    rc = port_mgr_tof2_serdes_ppm_get(dev_id, dev_port, ln, &ppm);
    if (ppm & (1<<10)) {
      signed_ppm = (int)ppm - 2048;
    } else {
      signed_ppm = ppm;
    }
    if (rc != BF_SUCCESS) continue;
    printf("\n#| %d | %d |  %d |%5s| %3dG|%5d,%5d,%4d |%s%d,%d,%d%s|%4d |",
            tile, grp, ln, mode_str[enc_mode], G, adapt, readapt, link_lost, sd_flag, sig_detect, phy_ready, adapt_done, phy_ready ? " ":"*", signed_ppm);

    rc = port_mgr_tof2_serdes_of_get(dev_id, dev_port, ln, &of);
    if (rc != BF_SUCCESS) continue;
    rc = port_mgr_tof2_serdes_hf_get(dev_id, dev_port, ln, &hf);
    if (rc != BF_SUCCESS) continue;
    chan_est = (float)(float)of / (float)hf;
    printf(" %4.2f,%2d,%2d ", chan_est, of, hf);

    port_mgr_tof2_serdes_ctle_over_val_get(dev_id, dev_port, ln, &ctle_over_val);
    port_mgr_tof2_serdes_ctle_val_get(dev_id, dev_port, ln, ctle_over_val, &ctle_map_0, &ctle_map_1);
    ctle_1 = ctle_map_0;
    ctle_2 = ctle_map_1;

    if (enc_mode == 2) { //PAM4
      uint32_t ctle_map_7_0, ctle_map_7_1;
      //if (ctle_1_bit4 == 1 or ctle_2_bit4 == 1 or chip.PAM50[ln].ctle_map_pam4(7)[0] == 7): ctle_val += 8
      port_mgr_tof2_serdes_ctle_val_get(dev_id, dev_port, ln, 7, &ctle_map_7_0, &ctle_map_7_1);
      if ((ctle_1 & 0x8) || (ctle_2 & 0x8) || (ctle_map_7_0 == 7)) {
        ctle_over_val += 8;
      }
    }

    port_mgr_tof2_serdes_ctle_gain_get(dev_id, dev_port, ln, &dc_gain_1, &dc_gain_2);
    printf("| %2d(%d,%d),%3d,%2d", ctle_over_val, ctle_1, ctle_2, dc_gain_1, dc_gain_2);

    rc = port_mgr_tof2_serdes_skef_val_get(dev_id, dev_port, ln, &skef_val);
    if (rc != BF_SUCCESS) continue;
    rc = port_mgr_tof2_serdes_dac_val_get(dev_id, dev_port, ln, &dac_val);
    if (rc != BF_SUCCESS) continue;

    printf("| %d | %2d", skef_val, dac_val);

    rc = port_mgr_tof2_serdes_eye_get(dev_id, dev_port, ln, &eye_1, &eye_2, &eye_3);
    if (enc_mode == 1) { //NRZ
      printf("|    %4.0f     ", eye_1);
    } else {
      printf("|%4.0f,%3.0f,%3.0f ", eye_1, eye_2, eye_3);
    }
    port_mgr_tof2_serdes_delta_get(dev_id, dev_port, ln, &delta);
    if (delta & (1<<6)) {
      signed7_delta = 0 - (128 - delta);
    } else {
      signed7_delta = delta;
    }
    port_mgr_tof2_serdes_edge_get(dev_id, dev_port, ln, &edge1, &edge2, &edge3, &edge4);
    if (enc_mode == 1) { //NRZ
      port_mgr_tof2_serdes_dfe_nrz_get(dev_id, dev_port, ln, &tap1, &tap2, &tap3);
      printf("| %4d,%4d,%4d    ", tap1, tap2, tap3);
    } else {
      port_mgr_tof2_serdes_dfe_pam4_get(dev_id, dev_port, ln, &f0, &f1, &ratio);
      port_mgr_tof2_serdes_f13_val_pam4_get(dev_id, dev_port, ln, &f13_val);
      printf("|%4.2f,%4.2f,%5.2f,%2d ", f0, f1, ratio, f13_val);
    }
    printf("|%3d,%X%X%X%X |", signed7_delta, edge1, edge2, edge3, edge4);

    if (enc_mode == 2) { //PAM4
      int32_t k1, k2, k3, k4, s1, s2;
      port_mgr_tof2_serdes_ffe_taps_pam4_get(dev_id, dev_port, ln, &k1, &k2, &k3, &k4, &s1, &s2);
      printf("\b%4d,%4d,%4d,%4d,%02X,%02X| ", k1, k2, k3, k4, s1, s2);
    }
  }  
}

typedef struct rx_mon_st  {
  uint32_t cnt;
  uint32_t cnt_n1;
  uint32_t cnt_n2;
  struct timeval prbs_cnt_time;
  struct timeval prbs_reset_time;
  uint32_t cnt_time;
  float    eyes_0;
  float    eyes_1;
  float    eyes_2;
  uint32_t link_status;
  uint32_t tei;
  uint32_t teo;
} rx_mon_st;

rx_mon_st rx_mon[33][8] = {{0}}; // indexed by {front_port, lane}

static uint32_t fec_analyzer_tei(uint32_t fp, uint32_t ln) {
  uint32_t dev_id, dev_port, tei;

  dev_id = 0;
  dev_port = (fp*8);

  port_mgr_tof2_serdes_fec_analyzer_tei_get(dev_id, dev_port, ln, &tei);
  return tei;
}

static uint32_t fec_analyzer_teo(uint32_t fp, uint32_t ln) {
  uint32_t dev_id, dev_port, teo;

  dev_id = 0;
  dev_port = (fp*8);

  port_mgr_tof2_serdes_fec_analyzer_teo_get(dev_id, dev_port, ln, &teo);
  return teo;
}

static void rx_monitor_clear(uint32_t fp, uint32_t ln) {
  bf_serdes_encoding_mode_t enc_mode;
  uint32_t G;
  uint32_t dev_id, dev_port;

  dev_id = 0;
  dev_port = (fp*8);

  memset((char*)&rx_mon[fp][ln], 0, sizeof(rx_mon[fp][ln]));

  port_mgr_tof2_serdes_fw_lane_speed_get(dev_id, dev_port, ln, &G, &enc_mode);
  if (enc_mode == 2) { //PAM4
    port_mgr_tof2_serdes_fec_analyzer_init_set(dev_id, dev_port, ln, 0, 15 /*T*/, 10 /*M*/, 5440 /*N*/);
  } else {
    port_mgr_tof2_serdes_fec_analyzer_init_set(dev_id, dev_port, ln, 0, 7 /*T*/, 10 /*M*/, 5280 /*N*/);
  }
  usleep(100000);
  port_mgr_tof2_serdes_prbs_rst_set(dev_id, dev_port, ln);
  get_time(&rx_mon[fp][ln].prbs_reset_time);
}

static void rx_monitor_capture(uint32_t fp, uint32_t ln) {
  uint32_t dev_id, dev_port;
  uint32_t cnt, cnt1;
  bool sd, phy_rdy, rdy;

  dev_id = 0;
  dev_port = (fp*8);

  //###### 1. Capture FEC Analyzer Data for this lane
  rx_mon[fp][ln].tei = fec_analyzer_tei(fp, ln);
  rx_mon[fp][ln].teo = fec_analyzer_teo(fp, ln);
  //###### 2. Capture PRBS Counter for this lane
  port_mgr_tof2_serdes_rx_prbs_err_get(dev_id, dev_port, ln, &cnt);
  cnt1 = cnt; // can we assume this?
  get_time(&rx_mon[fp][ln].prbs_cnt_time);
  port_mgr_tof2_serdes_sig_detect_get(dev_id, dev_port, ln, &sd, &phy_rdy);
  rdy = sd && phy_rdy;
  rx_mon[fp][ln].link_status = rdy;
  if ((rx_mon[fp][ln].prbs_reset_time.tv_sec == 0) && 
      (rx_mon[fp][ln].prbs_reset_time.tv_usec == 0)) {
    rx_mon[fp][ln].prbs_reset_time = rx_mon[fp][ln].prbs_cnt_time;
  }
  if (!rdy) {
    rx_mon[fp][ln].tei = 0xEEEEEEEE;
    rx_mon[fp][ln].teo = 0xEEEEEEEE;
    rx_mon[fp][ln].cnt = 0xEEEEEEEE; //  # return an artificially large number if RDY=0
    rx_mon[fp][ln].cnt_n1 = 0; //  # PrevPrbsCount-1
    rx_mon[fp][ln].cnt_n2 = 0; //  # PrevPrbsCount-2
    rx_mon[fp][ln].eyes_0 = -1;
    rx_mon[fp][ln].eyes_1 = -1;
    rx_mon[fp][ln].eyes_2 = -1;
  } else {
    port_mgr_tof2_serdes_eye_get(dev_id, dev_port, ln,
                          &rx_mon[fp][ln].eyes_0,
                          &rx_mon[fp][ln].eyes_1,
                          &rx_mon[fp][ln].eyes_2);
  }
  rx_mon[fp][ln].cnt_n2 = rx_mon[fp][ln].cnt_n1;
  rx_mon[fp][ln].cnt_n1 = rx_mon[fp][ln].cnt;
  rx_mon[fp][ln].cnt    = cnt;
}

static void rx_monitor_print(uint32_t fp, uint32_t ln) {
  uint32_t tile, grp, G;
  bf_serdes_encoding_mode_t enc_mode;
  bf_dev_id_t dev_id = 0;
  bf_dev_port_t dev_port = (fp*8);
  char *lane_name_list[] = {"A0",   "A1",   "A2",   "A3",  "A4",  "A5",   "A6",   "A7"};
  char *mode_str[] = {"NONE", "NRZ ", "PAM4"};

  tile_sim_map_mac_to_tile_and_grp(fp, &tile, &grp);

  printf("\n-------------");
  for (ln = 0; ln < 8; ln++) {
    printf("--------");
  }

  printf("\n Tile,Grp,Ln ");
  for (ln = 0; ln < 8; ln++) {
    printf("   %d,%d,%d", tile, grp, ln);
  }
  printf("\n         Lane");
  for (ln = 0; ln < 8; ln++) {
    printf("%8s", lane_name_list[ln]);
  }
  printf("\n     Encoding");
  for (ln = 0; ln < 8; ln++) {
    port_mgr_tof2_serdes_fw_lane_speed_get(dev_id, dev_port, ln, &G, &enc_mode);
    printf("%8s", mode_str[ enc_mode ]);
  }
  printf("\nDataRate Gbps");
  for (ln = 0; ln < 8; ln++) {
    float data_rate;

    port_mgr_tof2_serdes_fw_lane_speed_get(dev_id, dev_port, ln, &G, &enc_mode);
    if (G == 53) {
      data_rate = 53.125;
    } else if (G == 25) {
      data_rate = 25.78125;
    } else if (G == 10) {
      data_rate = 10.3125;
    } else if (G == 1) {
      data_rate = 1.25;
    } else {
      data_rate == 0.0;
    }
    printf("%8.4f", data_rate);
  }
  printf("\n-------------");
  for (ln = 0; ln < 8; ln++) {
    printf("--------");
  }
  printf("\n  Link Status");
  for (ln = 0; ln < 8; ln++) {
    printf("%8s", rx_mon[fp][ln].link_status ? "RDY":"NOT RDY");
  }
  printf("\n    Eye1 (mV)");
  for (ln = 0; ln < 8; ln++) {
    if (rx_mon[fp][ln].link_status) {
      printf("%8.0f", rx_mon[fp][ln].eyes_0);
    } else {
      printf("       -");
    }
  }
  printf("\n    Eye2 (mV)");
  for (ln = 0; ln < 8; ln++) {
    if ((enc_mode == 2) && rx_mon[fp][ln].link_status) {
      printf("%8.0f", rx_mon[fp][ln].eyes_1);
    } else {
      printf("       -");
    }
  }
  printf("\n    Eye3 (mV)");
  for (ln = 0; ln < 8; ln++) {
    if ((enc_mode == 2) && rx_mon[fp][ln].link_status) {
      printf("%8.0f", rx_mon[fp][ln].eyes_2);
    } else {
      printf("       -");
    }
  }
  printf("\n-------------");
  for (ln = 0; ln < 8; ln++) {
    printf("--------");
  }
  printf("\n Elapsed Time");
  for (ln = 0; ln < 8; ln++) {
    if (rx_mon[fp][ln].link_status) {
      struct timeval res;
      uint64_t elapsed_time;
      float f_elapsed_time;

      timersub(&rx_mon[fp][ln].prbs_cnt_time, &rx_mon[fp][ln].prbs_reset_time, &res);
      elapsed_time = res.tv_sec*1000000 + res.tv_usec;
      f_elapsed_time = (float)elapsed_time / 1000000;
      printf("%6.1f s", f_elapsed_time);
    } else {
      printf("       -");
    }
  }
  printf("\n     PRBS Cnt");
  for (ln = 0; ln < 8; ln++) {
    if (rx_mon[fp][ln].link_status) {
      printf("%8X", rx_mon[fp][ln].cnt);
    } else {
      printf("       -");
    }
  }
  printf("\n     PRBS BER");
  for (ln = 0; ln < 8; ln++) {
    if (rx_mon[fp][ln].link_status) {
      struct timeval res;
      uint64_t elapsed_time, bits_transferred;
      float f_elapsed_time, data_rate;

      timersub(&rx_mon[fp][ln].prbs_cnt_time, &rx_mon[fp][ln].prbs_reset_time, &res);
      elapsed_time = res.tv_sec*1000000 + res.tv_usec;
      f_elapsed_time = (float)elapsed_time / 1000000;
      if (G == 53) {
        data_rate = 53.125;
      } else if (G == 25) {
        data_rate = 25.78125;
      } else if (G == 10) {
        data_rate = 10.3125;
      } else if (G == 1) {
        data_rate = 1.25;
      } else {
        data_rate == 0.0;
      }
      bits_transferred = (float)((f_elapsed_time * data_rate * (float)1000000000.0));
      if ((rx_mon[fp][ln].cnt == 0) || (bits_transferred == 0)) {
        printf("%8.1e", 0.0);
      } else {
        printf("%8.1e", (float)rx_mon[fp][ln].cnt / (float)bits_transferred);
      }
    } else {
      printf("       -");
    }
  }
  printf("\n-------------");
  for (ln = 0; ln < 8; ln++) {
    printf("--------");
  }
  printf("\n  pre-FEC Cnt");
  for (ln = 0; ln < 8; ln++) {
    if (rx_mon[fp][ln].link_status) {
      printf("%8X", rx_mon[fp][ln].tei);
    } else {
      printf("       -");
    }
  }
  printf("\n Post-FEC Cnt");
  for (ln = 0; ln < 8; ln++) {
    if (rx_mon[fp][ln].link_status) {
      printf("%8X", rx_mon[fp][ln].teo);
    } else {
      printf("       -");
    }
  }
  printf("\n  pre-FEC BER");
  for (ln = 0; ln < 8; ln++) {
    if (rx_mon[fp][ln].link_status) {
      struct timeval res;
      uint64_t elapsed_time, bits_transferred;
      float f_elapsed_time, data_rate;

      timersub(&rx_mon[fp][ln].prbs_cnt_time, &rx_mon[fp][ln].prbs_reset_time, &res);
      elapsed_time = res.tv_sec*1000000 + res.tv_usec;
      f_elapsed_time = (float)elapsed_time / 1000000;
      if (G == 53) {
        data_rate = 53.125;
      } else if (G == 25) {
        data_rate = 25.78125;
      } else if (G == 10) {
        data_rate = 10.3125;
      } else if (G == 1) {
        data_rate = 1.25;
      } else {
        data_rate == 0.0;
      }
      bits_transferred = (float)((f_elapsed_time * data_rate * (float)1000000000.0));
      if ((rx_mon[fp][ln].tei == 0) || (bits_transferred == 0)) {
        printf("%8.1e", 0.0);
      } else {
        printf("%8.1e", (float)rx_mon[fp][ln].tei / (float)bits_transferred);
      }
    } else {
      printf("       -");
    }
  }
  printf("\n Post-FEC BER");
  for (ln = 0; ln < 8; ln++) {
    if (rx_mon[fp][ln].link_status) {
      struct timeval res;
      uint64_t elapsed_time, bits_transferred;
      float f_elapsed_time, data_rate;
  
      timersub(&rx_mon[fp][ln].prbs_cnt_time, &rx_mon[fp][ln].prbs_reset_time, &res);
      elapsed_time = res.tv_sec*1000000 + res.tv_usec;
      f_elapsed_time = (float)elapsed_time / 1000000;
      if (G == 53) {
        data_rate = 53.125;
      } else if (G == 25) {
        data_rate = 25.78125;
      } else if (G == 10) {
        data_rate = 10.3125;
      } else if (G == 1) {
        data_rate = 1.25;
      } else {
        data_rate == 0.0;
      }
      bits_transferred = (float)((f_elapsed_time * data_rate * (float)1000000000.0));
      if ((rx_mon[fp][ln].teo == 0) || (bits_transferred == 0)) {
        printf("%8d", 0);
      } else {
        printf("%8.1e", (float)rx_mon[fp][ln].teo / (float)bits_transferred);
      }
    } else {
      printf("       -");
    }
  }
  printf("\n-------------");
  for (ln = 0; ln < 8; ln++) {
    printf("--------");
  }
  printf("\n");
}

void rx_monitor(uint32_t fp, uint32_t ln,
                bool rst, bool print_en, bool capture_en) {
  uint32_t dev_id, dev_port;

  dev_id = 0;
  dev_port = (fp*8);
  if (rst) {
    rx_monitor_clear(fp, ln);
  } else if ((rx_mon[fp][ln].prbs_reset_time.tv_sec == 0) &&
             (rx_mon[fp][ln].prbs_reset_time.tv_usec == 0)) {
    rx_monitor_clear(fp, ln);
  }
  if (capture_en) {
    rx_monitor_capture(fp, ln);
  }
  if (print_en) {
    rx_monitor_print(fp, ln);
  }
}

/*
typedef struct tile_brd_map_t {
  uint32_t fp;
  uint32_t ln;
  uint32_t tile;
  uint32_t grp;
  uint32_t tx_phy;
  uint32_t tx_pn;
  uint32_t rx_phy;
  uint32_t rx_pn;
} tile_brd_map_t;

#define Y 1
#define N 0

tile_brd_map_t newport_brd
*/

void tile_lane_map_set() {
  uint32_t phys_tx_ln[8];
  uint32_t phys_rx_ln[8];
  tile_brd_map_t *map = newport_brd;
  uint32_t fp, ln, e;
  uint32_t dev_port, dev_id;

  for (fp = 1; fp <= 32; fp++) {
    dev_port = (fp*8);
    for (ln = 0; ln < 8; ln++) {
      // find entry
      for (e = 0; e < sizeof(newport_brd)/sizeof(newport_brd[0]); e++) {
        if ((map[e].fp == fp) && (map[e].ln == ln)) break;
      }
      phys_tx_ln[ln] = map[e].tx_phy;
      phys_rx_ln[ln] = map[e].rx_phy;
    }
    port_mgr_tof2_serdes_lane_map_set(dev_id, dev_port, phys_tx_ln, phys_rx_ln);
  }
  fp = 33;
  dev_port = 0; // CPU port
  for (ln = 0; ln < 4; ln++) {
    // find entry
    for (e = 0; e < sizeof(newport_brd)/sizeof(newport_brd[0]); e++) {
      if ((map[e].fp == fp) && (map[e].ln == ln)) break;
    }
    phys_tx_ln[ln] = map[e].tx_phy;
    phys_rx_ln[ln] = map[e].rx_phy;
  }
  for (ln = 4; ln < 8; ln++) {
    phys_tx_ln[ln] = ln;
    phys_rx_ln[ln] = ln;
  }
  port_mgr_tof2_serdes_lane_map_set(dev_id, dev_port, phys_tx_ln, phys_rx_ln);
}

void tile_pn_swap_set(bool apply) {
  tile_brd_map_t *map = newport_brd;
  uint32_t fp, ln, e;
  uint32_t dev_port, dev_id;

  for (fp = 1; fp <= 32; fp++) {
    dev_port = (fp*8);
    for (ln = 0; ln < 8; ln++) {
      // find entry
      for (e = 0; e < sizeof(newport_brd)/sizeof(newport_brd[0]); e++) {
        if ((map[e].fp == fp) && (map[e].ln == ln)) break;
      }
      bf_tof2_serdes_tx_polarity_set(dev_id, dev_port, ln, map[e].tx_pn, apply);
      bf_tof2_serdes_rx_polarity_set(dev_id, dev_port, ln, map[e].rx_pn, apply);
    }
  }
  fp = 33;
  dev_port = 0;
  for (ln = 0; ln < 4; ln++) {
    // find entry
    for (e = 0; e < sizeof(newport_brd)/sizeof(newport_brd[0]); e++) {
      if ((map[e].fp == fp) && (map[e].ln == ln)) break;
    }
    bf_tof2_serdes_tx_polarity_set(dev_id, dev_port, ln, map[e].tx_pn, apply);
    bf_tof2_serdes_rx_polarity_set(dev_id, dev_port, ln, map[e].rx_pn, apply);
  }
}

void temp_sensor_check(void) {
  int tile;
  uint32_t dev_id = 0, dev_port;
  int n, n_samples = 4;
  uint32_t rc, phase;
  float temp;
  float sample[4][4];

  for (n = 0; n < 4; n++) {
    for (tile = 0; tile < 4; tile++) {
      dev_port = tile*8*8;

      // read temp (auto)
      port_mgr_tof2_serdes_temperature_start_set(dev_id, dev_port, true);
    }
  
    for (tile = 0; tile < 4; tile++) {
      dev_port = tile*8*8;
      rc = port_mgr_tof2_serdes_temperature_get(dev_id, dev_port, true, &temp);
      if (rc == BF_SUCCESS) {
        sample[tile][n] = temp;
      } else {
        printf("Tile %d: Temperature(auto) : <timeout>\n", tile);
        sample[tile][n] = 0.0;
      }
    }
  }
  printf("Temperature(auto) :\n");
  for (tile = 0; tile < 4; tile++) {
    printf("  Tile %d: %2.1f C : %2.1f C : %2.1f C : %2.1f C\n",
           tile, sample[tile][0], sample[tile][1], sample[tile][2], sample[tile][3]);
  }
}

void tile_lane_reset_all(void) {
  uint32_t fp, ln, dev_id, dev_port;

  for (fp = 0; fp <= 32; fp++) {
    for (ln = 0; ln < 8; ln++) {
      dev_port = (fp*8);
      port_mgr_tof2_serdes_lane_reset_set(dev_id, dev_port, ln);
    }
  }
}

int tile_wait_all_signal(void) {
  int fp, dn_ports = 0;
  uint32_t dev_id = 0, dev_port, ln;
  int tries = 10;

  do {
    dn_ports = 0;
    for (fp = 1; fp <= 32; fp++) {
      for (ln = 0; ln < 8; ln++) {
        dev_port = (fp*8);
        bool sig_detect, phy_ready;
        bf_status_t rc;
        rc = port_mgr_tof2_serdes_sig_detect_get(dev_id, dev_port, ln, &sig_detect, &phy_ready);
        if (rc != BF_SUCCESS) {
          printf("rc=%d from port_mgr_tof2_serdes_sig_detect_get, dev_port=%d, ln=%d, fp=%d\n", rc, dev_port, ln, fp);
        }
        if ((!sig_detect) || (!phy_ready)) {
          dn_ports++;
        }
      }
    }
    printf("%d ports dn\n", dn_ports);
    if (dn_ports > 0) {
      usleep(1000000);
    }
  } while ((--tries >= 0) && (dn_ports > 0));
  return dn_ports;
}

int tile_wait_all_adapt_done(void) {
  int fp, not_dn_ports = 0;
  uint32_t dev_id = 0, dev_port, ln;
  int tries = 16;

  do {
    not_dn_ports = 0;
    for (fp = 1; fp <= 32; fp++) {
      for (ln = 0; ln < 8; ln++) {
        dev_port = (fp*8);
        bool adapt_done;
        bf_status_t rc;
        rc = port_mgr_tof2_serdes_adapt_done_get(dev_id, dev_port, ln, &adapt_done);
        if (rc != BF_SUCCESS) {
          printf("rc=%d from port_mgr_tof2_serdes_adapt_done_get, dev_port=%d, ln=%d, fp=%d\n", rc, dev_port, ln, fp);
        }
        if (adapt_done) {
          not_dn_ports++;
        }
      }
    }
    printf("%d ports not done\n", not_dn_ports);
    if (not_dn_ports > 0) {
      usleep(1000000);
    }
  } while ((--tries >= 0) && (not_dn_ports > 0));
  return not_dn_ports;
}

void tile_dump_all_ppm(void) {
  int fp, ln;
  uint32_t dev_id = 0, dev_port;

  printf("PPM:\n");
  printf("FP |Tile|Grp|   Ln7   |   Ln6   |   Ln5   |   Ln4   |   Ln3   |   Ln2   |   Ln1   |   Ln0   |\n");
  printf("---+----+---+---------+---------+---------+---------+---------+---------+---------+---------+\n");
  
  for (fp = 0; fp <= 32; fp++) {
    uint32_t tile, grp;
    uint32_t cnt;
    dev_port = (fp*8);
    bf_status_t rc;

    tile_sim_map_mac_to_tile_and_grp(fp, &tile, &grp);

    printf("%2d |  %d | %d |", fp, tile, grp);
    for (ln = 7; ln >= 0; ln--) {
      port_mgr_tof2_serdes_ppm_get(dev_id, dev_port, ln, &cnt);
      printf(" %7d |", cnt);
    }
    printf("\n");
  }
  printf("---+----+---+---------+---------+---------+---------+---------+---------+---------+---------+\n");
}

void tile_dump_all_chan_est(void) {
  int fp, ln;
  uint32_t dev_id = 0, dev_port;

  printf("Channel Est.:\n");
  printf("FP |Tile|Grp|   Ln7   |   Ln6   |   Ln5   |   Ln4   |   Ln3   |   Ln2   |   Ln1   |   Ln0   |\n");
  printf("---+----+---+---------+---------+---------+---------+---------+---------+---------+---------+\n");
  for (fp = 0; fp <= 32; fp++) {
    uint32_t tile, grp;
    uint32_t cnt, _of[8] = {0}, _hf[8] = {0};
    dev_port = (fp*8);
    bf_status_t rc;

    tile_sim_map_mac_to_tile_and_grp(fp, &tile, &grp);

    printf("%2d |  %d | %d |", fp, tile, grp);
    for (ln = 7; ln >= 0; ln--) {
      port_mgr_tof2_serdes_of_get(dev_id, dev_port, ln, &cnt);
      _of[ln] = cnt;
      printf(" %7d |", cnt);
    }
    printf("\n");

    printf("   |    |   |");
    for (ln = 7; ln >= 0; ln--) {
      port_mgr_tof2_serdes_hf_get(dev_id, dev_port, ln, &cnt);
      _hf[ln] = cnt;
      printf(" %7d |", cnt);
    }
    printf("\n");

    printf("   |    |   |");
    for (ln = 7; ln >= 0; ln--) {
      printf(" %7.2f |", ((float)(_of[ln]) / ((float)(_hf[ln]))));
    }
    printf("\n");
    printf("---+----+---+---------+---------+---------+---------+---------+---------+---------+---------+\n");
  }
}

void tile_dump_all_adapt_counts(void) {
  int fp, ln;
  uint32_t dev_id = 0, dev_port;

  printf("Adaptation counts:\n");
  printf("FP |Tile|Grp|   Ln7   |   Ln6   |   Ln5   |   Ln4   |   Ln3   |   Ln2   |   Ln1   |   Ln0   |\n");
  printf("---+----+---+---------+---------+---------+---------+---------+---------+---------+---------+\n");
  for (fp = 0; fp <= 32; fp++) {
    uint32_t tile, grp;
    uint32_t cnt;
    dev_port = (fp*8);
    bf_status_t rc;

    tile_sim_map_mac_to_tile_and_grp(fp, &tile, &grp);

    printf("%2d |  %d | %d |", fp, tile, grp);
    for (ln = 7; ln >= 0; ln--) {
      port_mgr_tof2_serdes_fw_adapt_cnt_get(dev_id, dev_port, ln, &cnt);
      printf(" %7d |", cnt);
    }
    printf("\n");

    printf("   |    |   |");
    for (ln = 7; ln >= 0; ln--) {
      port_mgr_tof2_serdes_fw_readapt_cnt_get(dev_id, dev_port, ln, &cnt);
      printf(" %7d |", cnt);
    }
    printf("\n");

    printf("   |    |   |");
    for (ln = 7; ln >= 0; ln--) {
      port_mgr_tof2_serdes_fw_link_lost_cnt_get(dev_id, dev_port, ln, &cnt);
      printf(" %7d |", cnt);
    }
    printf("\n");
    printf("---+----+---+---------+---------+---------+---------+---------+---------+---------+---------+\n");
  }
}

void tile_dump_all_signal(void) {
  int fp, ln;
  uint32_t dev_id = 0, dev_port;

  printf("* = sig-ok phy-rdy\n");
  printf(". = sig-ok\n");
  printf("---+----+-----+---+---+---+---+---+---+---+---+\n");
  printf("FP |Tile| Grp | 7 | 6 | 5 | 4 | 3 | 2 | 1 | 0 |\n");
  printf("---+----+-----+---+---+---+---+---+---+---+---+\n");
  for (fp = 0; fp <= 32; fp++) {
    uint32_t tile, grp;
    
    tile_sim_map_mac_to_tile_and_grp(fp, &tile, &grp);
    printf("%2d |  %d |  %d  |", fp, tile, grp);
    for (ln = 7; ln >= 0; ln--) {
      bool sig_detect, phy_ready;
      bf_status_t rc;
      dev_port = (fp*8);
      char *chr;

      port_mgr_tof2_serdes_sig_detect_get(dev_id, dev_port, ln, &sig_detect, &phy_ready);

      if (sig_detect && phy_ready) {
        chr = "*";
      } else if (sig_detect) {
        chr = ".";
      } else {
        chr = " ";
      }
      printf(" %s |", chr);
    }
    printf("\n");
  }
}

void tile_dump_all_adapt_done(void) {
  int fp, ln;
  uint32_t dev_id = 0, dev_port;

  printf("* = adapt_done\n");
  printf("---+----+-----+---+---+---+---+---+---+---+---+\n");
  printf("FP |Tile| Grp | 7 | 6 | 5 | 4 | 3 | 2 | 1 | 0 |\n");
  printf("---+----+-----+---+---+---+---+---+---+---+---+\n");
  for (fp = 0; fp <= 32; fp++) {
    uint32_t tile, grp;

    tile_sim_map_mac_to_tile_and_grp(fp, &tile, &grp);
    printf("%2d |  %d |  %d  |", fp, tile, grp);
    for (ln = 7; ln >= 0; ln--) {
      bool sig_detect, phy_ready, adapt_done;
      bf_status_t rc;
      dev_port = (fp*8);
      char *chr;

      port_mgr_tof2_serdes_adapt_done_get(dev_id, dev_port, ln, &adapt_done);

      if (adapt_done) {
        chr = "*";
      } else {
        chr = " ";
      }
      printf(" %s |", chr);
    }
    printf("\n");
  }
}

void tile_dump_all_prbs_errs(void) {
  int fp, ln;
  uint32_t dev_id = 0, dev_port;

  printf("Dwell: %d.%03d ms\n", dwell_us/1000, dwell_us % 1000);
  printf("---+----+---+--------------+-------------+-------------+-------------+-------------+-------------+-------------+-------------+\n");
  printf("FP |Tile|Grp|       7      |      6      |      5      |      4      |      3      |      2      |      1      |       0     |\n");
  printf("---+----+---+--------------+-------------+-------------+-------------+-------------+-------------+-------------+-------------+\n");
  for (fp = 0; fp <= 32; fp++) {
    uint32_t tile, grp;
    dev_port = (fp*8);

    tile_sim_map_mac_to_tile_and_grp(fp, &tile, &grp);
    printf("%2d |  %d | %d | ", fp, tile, grp);

    for (ln = 7; ln >= 0; ln--) {
      uint32_t err_cnt;
      bf_status_t rc;

      // reset counts 
      port_mgr_tof2_serdes_prbs_rst_set(dev_id, dev_port, ln);
      usleep(dwell_us);
      rc = port_mgr_tof2_serdes_rx_prbs_err_get(dev_id, dev_port, ln, &err_cnt);
      if (rc == BF_SUCCESS) {
        printf("%12u |", err_cnt);
      } else {
        printf(" ------------ |");
      }
    }
    printf("\n");
  }
}

void tile_all_power_up_dn(bool rx_off, bool tx_off, bool rx_bg_off, bool tx_bg_off) {
  int fp, ln;
  uint32_t dev_id = 0, dev_port;
  bf_status_t rc;

  for (fp = 0; fp <= 32; fp++) {
    dev_port = (fp*8);
    int max_ln;

    if (dev_port == 0) { //cpu port
      max_ln = 4;
    } else {
      max_ln = 8;
    }
    for (ln = 0; ln < 8; ln++) {
      bf_tof2_serdes_power_dn_set(dev_id, dev_port, ln, rx_off, tx_off, rx_bg_off, tx_bg_off);
    }
  }
}

void tile_dump_all_config(void) {
  int fp, ln;
  uint32_t dev_id = 0, dev_port;
  bf_status_t rc;
  char *mode_str[] = {"NONE", "NRZ ", "PAM4"};


  for (fp = 0; fp <= 32; fp++) {
    uint32_t tile, grp;
    uint32_t phys_tx_ln[8], phys_rx_ln[8];
    dev_port = (fp*8);

    printf("---+-----+--------+----+---+------+------+------+-----+-------+------+------+------+------+------+-------+-------+\n");
    printf("FP |devid|dev port|Tile|Grp|log ln|phy tx|phy rx| enc | speed | pre2 | pre1 | main | post1| post2| tx pol| rx pol|\n");
    printf("---+-----+--------+----+---+------+------+------+-----+-------+------+------+------+------+------+-------+-------+\n");

    tile_sim_map_mac_to_tile_and_grp(fp, &tile, &grp);
    port_mgr_tof2_serdes_lane_map_get(dev_id, dev_port, phys_tx_ln, phys_rx_ln);

    for (ln = 0; ln < 8; ln++) {
      bf_serdes_encoding_mode_t enc_mode;
      uint32_t G;
      int32_t pre2, pre1, main, post1, post2;
      bool tx_inv, rx_inv;

      printf("%2d | %3d |   %3d  | %2d | %d |   %d  |", fp, dev_id, dev_port, tile, grp, ln);
      printf("   %d  |   %d  |", phys_tx_ln[ln], phys_rx_ln[ln]);

      port_mgr_tof2_serdes_fw_lane_speed_get(dev_id, dev_port, ln, &G, &enc_mode);
      printf("%5s|  %3dG |", mode_str[enc_mode], G);

      bf_tof2_serdes_tx_taps_get(dev_id, dev_port, ln, &pre2, &pre1, &main, &post1, &post2);
      printf(" %5d| %5d| %5d| %5d| %5d|", pre2, pre1, main, post1, post2);
      bf_tof2_serdes_tx_polarity_get(dev_id, dev_port, ln, &tx_inv);
      bf_tof2_serdes_rx_polarity_get(dev_id, dev_port, ln, &rx_inv);
      printf("   %d   |   %d   |\n", tx_inv, rx_inv);
    }
  }
}

void squelch_all_tx(void) {
  int fp, ln;
  uint32_t dev_id = 0, dev_port;
  bf_status_t rc;

  for (fp = 0; fp <= 32; fp++) {
    dev_port = (fp*8);

    for (ln = 0; ln < 8; ln++) {
      if ((fp == 0) && (ln >= 4)) continue;
      bf_tof2_serdes_tx_squelch_set(dev_id, dev_port, ln);
    }
  }
}

void tile_dump_all_fw_info(void) {
  int fp, ln;
  uint32_t dev_id = 0, dev_port;
  bf_status_t rc;
  char *mode_str[] = {"NONE", "NRZ ", "PAM4"};


  for (fp = 0; fp <= 32; fp++) {
    uint32_t tile, grp;
    dev_port = (fp*8);
    char *fw_file_nm;
    uint8_t *fw_buffer_p;
    uint32_t fw_len;
    uint32_t fw_hash_code;
    uint32_t fw_crc;
    uint32_t running_hash_code;
    uint32_t running_crc;

    fw_file_name = fw_file_name; // default

    if (fp == 0) { // CPU port
      fw_file_nm = fw_grp8_file_name;

      printf("---+-----+--------+----+---+--------+---------+--------+------------+-----------+\n");
      printf("FP |devid|dev port|Tile|Grp|file len|file hash|file crc|running hash|running crc|\n");
      printf("---+-----+--------+----+---+--------+---------+--------+------------+-----------+\n");
    } else if (((fp - 1) % 8) == 0) {
      printf("---+-----+--------+----+---+--------+---------+--------+------------+-----------+\n");
      printf("FP |devid|dev port|Tile|Grp|file len|file hash|file crc|running hash|running crc|\n");
      printf("---+-----+--------+----+---+--------+---------+--------+------------+-----------+\n");
    }

    tile_sim_map_mac_to_tile_and_grp(fp, &tile, &grp);
    
    bf_tof2_serdes_fw_ver_get(dev_id,
                              dev_port,
                              fw_file_name,
                              &fw_buffer_p,
                              &fw_len,
                              &fw_hash_code,
                              &fw_crc,
                              &running_hash_code,
                              &running_crc);
    printf("%2d | %3d |   %3d  | %2d | %d |", fp, dev_id, dev_port, tile, grp);
    printf("%7d | %06x  |  %04x  |   %06x   |   %04x    |\n", fw_len, fw_hash_code, fw_crc, running_hash_code, running_crc);
  }
}



void timing_test(void) {
  uint32_t err_cnt, dev_id = 0, dev_port = 8, ln = 0;
  int i;

  detailed_dbg_print = 1;
  for (i = 0; i < 10; i++) {
      port_mgr_tof2_serdes_prbs_rst_set(dev_id, dev_port, ln);
      usleep(10000);
      port_mgr_tof2_serdes_rx_prbs_err_get(dev_id, dev_port, ln, &err_cnt);
  }
  detailed_dbg_print = 0;
  exit(0);
}

void tile_all_lane_reset(void) {
}

void tile_chip_init(void) {
#include "../../port_mgr_tof2/port_mgr_tof2_serdes.h"
  int front_port;
  bf_status_t rc;
  uint32_t dev_id = 0, dev_port;
  int phase;

  uint8_t *fw_buffer_p;
  uint32_t fw_len;
  uint32_t fw_hash_code;
  uint32_t fw_crc;
  uint8_t *fw_grp8_buffer_p;
  uint32_t fw_grp8_len;
  uint32_t fw_grp8_hash_code;
  uint32_t fw_grp8_crc;

  bool skip_group_reset = false;
  bool skip_post = false;
  bool force_fw_dnld = false;
  bool skip_power_dn = false;

  banner("Init Tile Chip");

  bf_tof2_serdes_tile_init(dev_id,
                           fw_file_name,
                           fw_grp8_file_name,
                           skip_group_reset,
                           skip_post,
                           force_fw_dnld,
                           skip_power_dn); 

  banner("Set Lane Map");
  tile_lane_map_set();

  banner("FW Info");
  tile_dump_all_fw_info();

  banner("Let chip cool dn (10s)");
  sleep(10);

  banner("Temperature Sensors");
  temp_sensor_check();

  banner("Power-up all lanes");
  tile_all_power_up_dn(false, false, false, false);

  banner("Lane Config");
  tile_dump_all_config();

  banner("Set PN Swaps");
  tile_pn_swap_set(false);

  for (front_port = 1; front_port <= 32; front_port++) {
    uint32_t dev_id = 0, dev_port = (front_port*8);
    int ln, n_lanes;
    bf_port_speed_t speed;
    
    for (ln = 0; ln < 8; ln++) {
      if (tile_init_mode_nrz()) {
        speed = DEFAULT_NRZ_SPEED;
        n_lanes = 4;
      } else {
        speed = DEFAULT_PAM4_SPEED;
        n_lanes = 8;
      }
      bf_tof2_serdes_tx_taps_set(dev_id, dev_port, ln, 2, -10, 20, 0, 0, false);
      rc = bf_tof2_serdes_config_ln(dev_id,
                               dev_port,
                               ln,
                               speed,
                               n_lanes,
                               BF_PORT_PRBS_MODE_31,
                               BF_PORT_PRBS_MODE_31);
      if (rc != BF_SUCCESS) {
        printf("Error: %d : from bf_tof2_serdes_config_ln\n", rc);
        //exit(0);
      }
    }
  }

  // CPU Port
  for (front_port = 0; front_port < 1; front_port++) {
    uint32_t dev_id = 0, dev_port = (front_port*8);
    int ln, n_lanes;
    bf_port_speed_t speed = BF_SPEED_1G;

    for (ln = 0; ln < 4; ln++) {
      n_lanes = 4;
      bf_tof2_serdes_tx_taps_set(dev_id, dev_port, ln, 2, -10, 20, 0, 0, false);
      rc = bf_tof2_serdes_config_ln(dev_id,
                               dev_port,
                               ln,
                               speed,
                               n_lanes,
                               BF_PORT_PRBS_MODE_31,
                               BF_PORT_PRBS_MODE_31);
      if (rc != BF_SUCCESS) {
        printf("Error: %d : from bf_tof2_serdes_config_ln\n", rc);
        //exit(0);
      }
    }
    for (ln = 0; ln < 4; ln++) {
      for (int phase = 0; phase < 3; phase++) {
        rc = port_mgr_tof2_serdes_tx_rx_serial_loopback_nrz(dev_id, dev_port, ln, 1, phase);
        if (rc != BF_SUCCESS) {
          printf("%d/%d: Error configuring loopback: %d\n", front_port, ln, rc);
          //exit(1);
        }
        if (phase == 0) {
          usleep(3*1000*1000); // yep, 3 seconds 
        } else if (phase == 1) {
          usleep(10000);
        }
      }
    }
  }

  banner("Lane Config");
  tile_dump_all_config();

  banner("Wait for SIGNAL");
  int dn_ports = 0;
  dn_ports = tile_wait_all_signal();
  if (dn_ports >0) {
    printf("%d ports not seeing SIGNAL\n", dn_ports);
  }
  tile_dump_all_signal();

  banner("Lane Reset");
  tile_lane_reset_all();

  banner("Wait for ADAPT DONE");
  tile_wait_all_adapt_done();
  tile_dump_all_adapt_done();
  tile_dump_all_adapt_counts();
  tile_dump_all_prbs_errs();

  tile_dump_all_ppm(); 

  for (front_port = 1; front_port < 33; front_port++) {
    fw_serdes_params(front_port);
  }
  fw_serdes_params(0); // CPU port (NRZ)

#if 0
  //rst +
  //capture
  for (front_port = 1; front_port < 33; front_port++) {
    for (int ln = 0; ln < 8; ln++) {
      rx_monitor(front_port, ln, true, false, false);
      usleep(100000);
      rx_monitor(front_port, ln, false, false, true);
    }
  }
  // cpu port
  for (int ln = 0; ln < 8; ln++) {
    rx_monitor(0, ln, true, false, false);
    usleep(100000);
    rx_monitor(0, ln, false, false, true);
  }
  //print
  for (front_port = 1; front_port < 33; front_port++) {
    rx_monitor(front_port, 0, false, true, false);
  }
  // cpu port
  rx_monitor(0, 0, false, true, false);

  printf("\n");

  banner("Temperature Sensors");
  temp_sensor_check();

  banner("Squelch Tx Output");
  squelch_all_tx();

  banner("SIGNAL DETECT");
  tile_dump_all_signal();

  banner("ADAPT DONE");
  tile_dump_all_adapt_done();
  tile_dump_all_adapt_counts();
  tile_dump_all_prbs_errs();

  for (front_port = 1; front_port < 33; front_port++) {
    fw_serdes_params(front_port);
  }
  fw_serdes_params(0); // CPU port (NRZ)

  //rst +
  //capture
  for (front_port = 1; front_port < 33; front_port++) {
    for (int ln = 0; ln < 8; ln++) {
      rx_monitor(front_port, ln, true, false, false);
      usleep(100000);
      rx_monitor(front_port, ln, false, false, true);
    }
  }
  // cpu port
  for (int ln = 0; ln < 8; ln++) {
    rx_monitor(0, ln, true, false, false);
    usleep(100000);
    rx_monitor(0, ln, false, false, true);
  }
  //print
  for (front_port = 1; front_port < 33; front_port++) {
    rx_monitor(front_port, 0, false, true, false);
  }
  // cpu port
  rx_monitor(0, 0, false, true, false);

  printf("\n");
#endif

}


void fpga_ctrlr_rd(uint32_t addr, uint32_t *data) {
  if (fpga_wait_done(1000) != 0) {
    printf("Previous operation not done (before read)\n");
    *data = 0x00000bad;
    //fpga_ctrlr_init(fpga_ctrlr_base_address);
  }
  if (log_timestamps) log_time("Read start: \n");
  fpga_ctrlr->r1_group = active_group;
  fpga_ctrlr->r2_addr  = addr;
  fpga_ctrlr->r0_ctrl_status = FPGA_CMD_READ;

  if (fpga_wait_done(1000) != 0) {
    printf("Read failed\n");
    *data = 0x00001bad;
    return;
  }
  *data = fpga_ctrlr->r3_data;
  if (log_timestamps) log_time("Read done :\n");
}

void fpga_ctrlr_wr(uint32_t addr, uint32_t data) {
  if (fpga_wait_done(1000) != 0) {
    printf("Previous operation not done (before write)\n");
    //fpga_ctrlr_init(fpga_ctrlr_base_address);
  }
  if (log_timestamps) log_time("Write start:\n");
  fpga_ctrlr->r1_group = active_group;
  fpga_ctrlr->r2_addr  = addr;
  fpga_ctrlr->r3_data  = data;
  fpga_ctrlr->r0_ctrl_status = FPGA_CMD_WRITE;

  if (fpga_wait_done(1000) != 0) {
    printf("Write failed\n");
    return;
  }
  if (log_timestamps) log_time("Write done :\n");
}

uint32_t tiles[] = {0,
                    1, 1, 1, 1, 1, 1, 1, 1,
                    2, 2, 2, 2, 2, 2, 2, 2,
                    3, 3, 3, 3, 3, 3, 3, 3, 
                    0, 0, 0, 0, 0, 0, 0, 0}; // mac
uint32_t grps[] = { 8,
                    0, 1, 2, 3, 4, 5, 6, 7,
                    0, 1, 2, 3, 4, 5, 6, 7,
                    1, 0, 3, 2, 5, 4, 7, 6,
                    1, 0, 3, 2, 5, 4, 7, 6}; //mac

void tile_sim_map_mac_to_tile_and_grp(uint32_t mac_stn_id, uint32_t *tile, uint32_t *grp) {
  if (mac_stn_id == 0) {
    *tile = 0; // cpu port
    *grp  = 8;
  } else if (mac_stn_id == 35) {
    *tile = 0;
    *grp = 9;
  } else if (mac_stn_id == 36) {
    *tile = 1;
    *grp = 9;
  } else if (mac_stn_id == 37) {
    *tile = 2;
    *grp = 9;
  } else if (mac_stn_id == 38) {
    *tile = 3;
    *grp = 9;
  } else {
    *tile = tiles[mac_stn_id];
    *grp  = grps[mac_stn_id];
  }
}

void tile_sim_rd(uint32_t mac_stn_id, uint32_t addr, uint32_t *data) {
  uint32_t tile, grp;

  tile_sim_map_mac_to_tile_and_grp(mac_stn_id, &tile, &grp);
  fpga_active_tile_set(tile);
  active_group = grp;

  // special check for group8 FW load. Must set group=9
  if ((addr >= 0x5000) && (addr <=0x500F) && (grp == 8)) {
    active_group = 9;
  } else if (((addr >> 11) & 0xF) == 9) {
    active_group = 9;
  }
  
  if (0 && dbg_print) printf("Rd(1): mac_stn_id=%d : addr=%04x : data=na : tile=%d : grp=%d\n", mac_stn_id, addr, tile, grp);
  fpga_ctrlr_rd(addr, data);
  if (0 && dbg_print) printf("Rd(2): mac_stn_id=%d : addr=%04x : data=%04x : tile=%d : grp=%d\n", mac_stn_id, addr, *data, tile, grp);
  {
    if (detailed_dbg_print && dbg_print) {
      _log_time();
      printf("Tile: %d : Group: %d : %s : Addr: %04x : Data : %04x\n",
             active_tile,
             active_group,
             "Read ",
             addr,
             *data);
    }
    fflush(stdout);
  }
}

void tile_sim_wr(uint32_t mac_stn_id, uint32_t addr, uint32_t data) {
  uint32_t tile, grp;

  tile_sim_map_mac_to_tile_and_grp(mac_stn_id, &tile, &grp);
  if (0 && dbg_print) printf("Wr   : mac_stn_id=%d : addr=%04x : data=%04x : tile=%d : grp=%d\n", mac_stn_id, addr, data, tile, grp);
  fpga_active_tile_set(tile);
  active_group = grp;

  // special check for group8 FW load. Must set group=9
  if ((addr >= 0x5000) && (addr <=0x500F) && (grp == 8)) {
    active_group = 9;
  } else if (((addr >> 11) & 0xF) == 9) {
    active_group = 9;
  }

  fpga_ctrlr_wr(addr, data);
  {
    if (detailed_dbg_print && dbg_print) {
      _log_time();
      printf("Tile: %d : Group: %d : %s : Addr: %04x : Data : %04x\n",
             active_tile,
             active_group,
             "Write",
             addr,
             data);
    }
    fflush(stdout);
  }
}

/*********************************************************************
* create_server
*********************************************************************/
int create_server(int listen_port, bool is_local_only) {
  int socket_desc, client_sock, c;
  struct sockaddr_in server, client;
  int so_reuseaddr = 1;

  // Create socket
  socket_desc = socket(AF_INET, SOCK_STREAM, 0);
  if (socket_desc == -1) {
    printf("ERROR: fpga_intf could not create socket");
    return -1;
  }
  printf("fpga_intf: listen socket created\n");

  // Prepare the sockaddr_in structure
  server.sin_family = AF_INET;
  server.sin_addr.s_addr =  is_local_only ? htonl(INADDR_LOOPBACK) : htonl(INADDR_ANY);
  server.sin_port = htons(listen_port);

  setsockopt(socket_desc,
             SOL_SOCKET,
             SO_REUSEADDR,
             &so_reuseaddr,
             sizeof so_reuseaddr);

  // Bind
  if (bind(socket_desc, (struct sockaddr *)&server, sizeof(server)) < 0) {
    perror("bind failed. Error");
    close(socket_desc);
    return -1;
  }
  printf("fpga_intf: bind done on port %d, listening...\n", listen_port);
  do {
    // Listen
    puts("fpga_intf: listening for incoming connections...");
    listen(socket_desc, 1);


    // accept connection from an incoming client
    c = sizeof(struct sockaddr_in);
    client_sock =
        accept(socket_desc, (struct sockaddr *)&client, (socklen_t *)&c);
    if (client_sock < 0) {
      printf("accept failed");
      //close(socket_desc);
      continue;
    }
    reg_chnl = client_sock;

    printf("fpga_intf: <CONNECT> connection accepted on client sock: %d\n", client_sock);
    proc_server(); // start processing
    printf("fpga_intf: <DISCONNECT> connection terminated on client sock: %d\n", client_sock);

    close(client_sock);
  } while (true);

  return client_sock;
}

/*********************************************************************
* write_to_socket
*********************************************************************/
int write_to_socket(int sock, uint8_t *buf, int len) {
  if (send(sock, buf, len, 0) < 0) {
    printf("write_to_socket failed");
    return -3;
  }
  return 0;
}

/*********************************************************************
* read_from_socket
*********************************************************************/
int read_from_socket(int sock, uint8_t *buf, int len) {
  int n_read = 0, n_read_this_time = 0, i = 1;

  // Receive a message from client
  do {
    n_read_this_time =
        recv(sock, (buf + n_read), (len - n_read), MSG_WAITALL);
        //recv(sock, (buf + n_read), (len - n_read), 0 /*MSG_WAITALL*/);
    if (n_read_this_time > 0) {
      n_read += n_read_this_time;
      if (n_read < len) {
        printf("Partial recv: %d of %d so far..\n", n_read, len);
      }
    } else {
      return n_read;
    }
    setsockopt(sock, IPPROTO_TCP, TCP_QUICKACK, (void *)&i, sizeof(i));

  } while (n_read < len);
  return n_read;
}

uint32_t proc_so_read(uint32_t offset) {
  uint32_t data = 0;
  uint16_t u16;

  fpga_ctrlr_rd(offset, &data);
  //u16 = *((uint16_t *) (virt_addr + offset));
  //data = (uint32_t)u16;
  return data;
}

void proc_so_write(uint32_t offset, uint32_t data) {
  uint16_t u16 = (uint16_t)data & 0xffff;

  fpga_ctrlr_wr(offset, data);
  //*((uint16_t *) (virt_addr + offset)) = u16;
  return;
}

void start_server(bool is_local_only) {
  reg_chnl = create_server(9001, is_local_only);
  if (-1 == reg_chnl) {
    /* Indicates bind/accept error in create server */
    printf("Socket already in use. Exiting the application\n");
    exit(1);
  }
}

/*****************************************************************
* proc_server
*
* This option provides a faster way to process I2C requests in
* support of the Credo eval boards.
* Credo supplies python scripts to configure their eval boards.
* We run these scripts on a switch CPU and connect the CP2112
* of that switch to the I2C pins of the eval board (using the
* CPU port QSFP channel).
*
* Because the driver is in python, the protocol is string based,
* using colons as separators.
*
* The command protocol is as follows:
*   Read: "r:<address>:<unused_data>"
*  Write: "w:<address>:<data>"
*
* E.g.
*   "r:0x9816:0x0000"
*   "w:0x9818:0x1234"
*
*****************************************************************/
#define CMD_LEN 15
int proc_server(void) {
  fpga_ctrlr_reset();

extern void tile_chip_init(void); 
  tile_chip_init();

  while (true) {
    char cmd[CMD_LEN+1] = {0};
    uint32_t address, data;
    int rc, n_read, is_tile = false, is_grp = false, is_read = true;

    memset(cmd, 0, sizeof(cmd));
    n_read = read_from_socket(reg_chnl, (uint8_t *)cmd, CMD_LEN);
    if (0 && dbg_print) {
      int i;
      printf("\nsocket (%d bytes) => ", n_read);
      for (i = 0; i < n_read; i++) {
        printf("%02x ", cmd[i]);
      }
      printf("\n");
    }
    if (n_read != 15) return -1;

    cmd[8] = 0;
    address = strtoul(&cmd[2], NULL, 16);
    cmd[CMD_LEN] = 0;
    data = strtoul(&cmd[9], NULL, 16);
    if ((cmd[0] == 'w') || (cmd[0] == 'W')) is_read = false;
    else if ((cmd[0] == 'r') || (cmd[0] == 'R')) is_read = true;
    else if ((cmd[0] == 'g') || (cmd[0] == 'G')) {is_read = false; is_grp = true;}
    else if ((cmd[0] == 't') || (cmd[0] == 'T')) {is_read = false; is_tile = true;}
    else return -2;

    if (is_tile) {
      fpga_active_tile_set(address);
      //data = 0;
      data = address;
    } else if (is_grp) {
      active_group = address;
      //data = 0;
      data = active_group;
    } else if (is_read) {
      data = proc_so_read(address);
      snprintf(&cmd[9], 7, "0x%04x", data); // return data
    } else {
      uint32_t wr_data = data;
      proc_so_write(address, data);
      //data = 0;
      data = wr_data;
    }
    if (data > 0xffff) {
      printf(" Warn: data= %x\n", data);
      int i;
      printf("\nsocket (%d bytes) => ", n_read);
      for (i = 0; i < n_read; i++) {
        printf("%02x ", cmd[i]);
      }
      printf("\n");
    }
    // put back colon
    cmd[8] = ':';
    cmd[15] = 0;
    if (1 && dbg_print) {
      _log_time();
      printf("Tile: %d : Group: %d : %s : Addr: %04x : Data : %04x\n",
             active_tile,
             active_group,
             is_tile ? " Tile" : 
             is_grp  ? "Group" : 
             is_read ? " Read" : "Write",
             address,
             data);
    }
    fflush(stdout);
    rc = write_to_socket(reg_chnl, (uint8_t *)cmd, CMD_LEN);
    if (rc != 0) return -4;
  }
  return reg_chnl;
}

int main(int argc, char **argv) {
    int fd;
    char *filename;
    off_t target, target_base;
    //int map_size = 4096UL;
    int map_size = (256ul*1024ul);

    if(argc < 1) {
        // pcimem /sys/bus/pci/devices/0001\:00\:07.0/resource0 0x100 w 0x00
        // argv[0]  [1]                                         [2]   [3] [4]
        fprintf(stderr, "\nUsage:\t%s <sysfile>\n"
            "\tsys file: sysfs file for the pci resource to act on\n",
	    argv[0]);
        exit(1);
    }
    //filename = argv[1];
    filename = "/sys/bus/pci/devices/0000:06:00.0/resource0";
    target = 0;

//int on_hw = false;
int on_hw = true;
if (on_hw) {

    if((fd = open(filename, O_RDWR | O_SYNC)) == -1) ERR_LOG;
    printf("%s opened.\n", filename);
    printf("Target offset is 0x%x, page size is %ld\n", (int) target, sysconf(_SC_PAGE_SIZE));
    fflush(stdout);

    //target_base = target & ~(sysconf(_SC_PAGE_SIZE)-1);
    target_base = 0;
    //if (target + 4 - target_base > map_size)
    // map_size = target + 4 - target_base;

    /* Map one page */
    printf("mmap(%d, %d, 0x%x, 0x%x, %d, 0x%x)\n", 0, map_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, (int) target);

    //map_base = mmap(0, map_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, target_base);
    map_base = mmap(0, 256*1024, PROT_READ | PROT_WRITE, MAP_SHARED, fd, target_base);
    if(map_base == (void *) -1) ERR_LOG;
    printf("PCI Memory mapped to address 0x%08lx.\n", (unsigned long) map_base);
    fflush(stdout);
} else {
    target_base = (off_t)0x0000;
    map_base = (void*)our_fake_address_space;
}

    // set virtual address
    virt_addr = map_base + target + target_base;
    fpga_ctrlr_base_address = (uintptr_t)virt_addr;
    fpga_ctrlr_set();
    fpga_ctrlr_reset();

    // process socket requests (forever)
    start_server(false /*is_local_only*/);

    fflush(stdout);

    if(munmap(map_base, map_size) == -1) ERR_LOG;
    close(fd);
    return 0;
}


/* **************************************************************** */
/*                                                                  */
/* ASIC and ASSP Programming Layer (AAPL)                           */
/* Copyright (c) 2014-2016 Avago Technologies. All rights reserved. */
/*                                                                  */
/* **************************************************************** */
/* AAPL Revision: 2.4.3                                        */
/* AAPL (ASIC and ASSP Programming Layer) support for configuring */
/* and initiating AN (16nm only). */

/** Doxygen File Header */
/** @file */
/** @brief Functions related to the AN. */

#define AAPL_ENABLE_INTERNAL_FUNCTIONS
#include <avago/avago_aapl.h>
#include <port_mgr/port_mgr_log.h>

#if AAPL_ENABLE_SERDES_AUTO_NEG

/** @brief   Configures serdes to 25gbase_krcr_s. */
/** @details Configures serdes to 25.7812G rate with 40-bit width. */
/** @return  void */
static void setup_25gbase_krcr_s(Aapl_t *aapl, uint addr) {
  BOOL tx_ready, rx_ready;

  avago_serdes_set_tx_rx_enable(
      aapl, addr, FALSE, FALSE, FALSE); /* Disable serdes */
  avago_spico_int(
      aapl, addr, 0x05, 0x80A5); /* set serdes bit/ref ratio = 25.7812G */
  avago_serdes_set_tx_rx_width(aapl, addr, 40, 40); /* 40bit width */
  avago_serdes_set_tx_rx_enable(
      aapl, addr, TRUE, TRUE, TRUE); /* enable serdes for next operations */
  avago_serdes_get_tx_rx_ready(aapl, addr, &tx_ready, &rx_ready);
  if ((tx_ready != 1) || (rx_ready != 1))
    port_mgr_log(
        "Tx or Rx is not ready. Tx bit %d, Rx bit %d ", tx_ready, rx_ready);
}

/** @brief   Configures serdes to 25gbase_krcr. */
/** @details Configures serdes to 25.7812G rate with 40-bit width. */
/** @return  void */
static void setup_25gbase_krcr(Aapl_t *aapl, uint addr) {
  BOOL tx_ready, rx_ready;

  avago_serdes_set_tx_rx_enable(
      aapl, addr, FALSE, FALSE, FALSE); /* Disable serdes */
  avago_spico_int(
      aapl, addr, 0x05, 0x80A5); /* set serdes bit/ref ratio = 25.7812G */
  avago_serdes_set_tx_rx_width(aapl, addr, 40, 40); /* 40bit width */
  avago_serdes_set_tx_rx_enable(
      aapl, addr, TRUE, TRUE, TRUE); /* enable serdes for next operations */
  avago_serdes_get_tx_rx_ready(aapl, addr, &tx_ready, &rx_ready);
  if ((tx_ready != 1) || (rx_ready != 1))
    port_mgr_log(
        "Tx or Rx is not ready. Tx bit %d, Rx bit %d ", tx_ready, rx_ready);
}

/** @brief   Configures serdes to 1000base_kx. */
/** @details Configures serdes to 1.25G rate with 10-bit width. */
/** @return  void */
static void setup_1000base_kx(Aapl_t *aapl, uint addr) {
  BOOL tx_ready, rx_ready;

  avago_serdes_set_tx_rx_enable(
      aapl, addr, FALSE, FALSE, FALSE); /* Disable serdes */
  avago_spico_int(
      aapl, addr, 0x05, 0x8008); /* set serdes bit/ref ratio = 1.25G */
  avago_serdes_set_tx_rx_width(aapl, addr, 10, 10); /* 10bit width */
  avago_serdes_set_tx_rx_enable(
      aapl, addr, TRUE, TRUE, TRUE); /* enable serdes for next operations */
  avago_serdes_get_tx_rx_ready(aapl, addr, &tx_ready, &rx_ready);
  if ((tx_ready != 1) || (rx_ready != 1))
    port_mgr_log(
        "Tx or Rx is not ready. Tx bit %d, Rx bit %d ", tx_ready, rx_ready);
}

/** @brief   Configures serdes to 10gbase_kx4. */
/** @details Configures 3.125G serdes rate with 10-bit width. */
/** @return  void */
static void setup_10gbase_kx4(Aapl_t *aapl, uint addr) {
  BOOL tx_ready, rx_ready;

  avago_serdes_set_tx_rx_enable(
      aapl, addr, FALSE, FALSE, FALSE); /* Disable serdes */
  avago_spico_int(
      aapl, addr, 0x05, 0x8014); /* set serdes bit/ref ratio = 3.125G */
  avago_serdes_set_tx_rx_width(aapl, addr, 10, 10); /* 10bit width */
  avago_serdes_set_tx_rx_enable(
      aapl, addr, TRUE, TRUE, TRUE); /* enable serdes for next operations */
  avago_serdes_get_tx_rx_ready(aapl, addr, &tx_ready, &rx_ready);
  if ((tx_ready != 1) || (rx_ready != 1))
    port_mgr_log(
        "Tx or Rx is not ready. Tx bit %d, Rx bit %d ", tx_ready, rx_ready);
}

/** @brief   Configures serdes to 10gbase_kr. */
/** @details Configures 10.3125G serdes rate with 20-bit width and performs KR
 * training. */
/** @return  void */
static void setup_10gbase_kr(Aapl_t *aapl, uint addr) {
  BOOL tx_ready, rx_ready;

  avago_serdes_set_tx_rx_enable(
      aapl, addr, FALSE, FALSE, FALSE); /* Disable serdes */
  avago_spico_int(
      aapl, addr, 0x05, 0x8042); /* set serdes bit/ref ratio = 10G */
  avago_serdes_set_tx_rx_width(aapl, addr, 20, 20); /* 20bit width */
  avago_serdes_set_tx_rx_enable(
      aapl, addr, TRUE, TRUE, FALSE);        /* Enable Tx and Rx */
  avago_spico_int(aapl, addr, 0x04, 0x0002); /* launch KR training */
  avago_serdes_set_tx_rx_enable(
      aapl, addr, TRUE, TRUE, TRUE); /* enable serdes for next operations */
  avago_serdes_get_tx_rx_ready(aapl, addr, &tx_ready, &rx_ready);
  if ((tx_ready != 1) || (rx_ready != 1))
    port_mgr_log(
        "Tx or Rx is not ready. Tx bit %d, Rx bit %d ", tx_ready, rx_ready);
}

/** @brief   Configures serdes to 40gbase_kr4. */
/** @details Configures 10.3125G serdes rate with 20-bit width and performs KR
 * training. */
/** @return  void */
static void setup_40gbase_kr4(Aapl_t *aapl, uint addr) {
  BOOL tx_ready, rx_ready;

  avago_serdes_set_tx_rx_enable(
      aapl, addr, FALSE, FALSE, FALSE); /* Disable serdes */
  avago_spico_int(
      aapl, addr, 0x05, 0x8042); /* set serdes bit/ref ratio = 10G */
  avago_serdes_set_tx_rx_width(aapl, addr, 20, 20); /* 20bit width */
  avago_serdes_set_tx_rx_enable(
      aapl, addr, TRUE, TRUE, FALSE);        /* Enable Tx and Rx */
  avago_spico_int(aapl, addr, 0x04, 0x0002); /* launch KR training */
  avago_serdes_set_tx_rx_enable(
      aapl, addr, TRUE, TRUE, TRUE); /* enable serdes for next operations */
  avago_serdes_get_tx_rx_ready(aapl, addr, &tx_ready, &rx_ready);
  if ((tx_ready != 1) || (rx_ready != 1))
    port_mgr_log(
        "Tx or Rx is not ready. Tx bit %d, Rx bit %d ", tx_ready, rx_ready);
}

/** @brief   Configures serdes to 40gbase_cr4. */
/** @details Configures 10.3125G serdes rate with 20-bit width and performs KR
 * training. */
/** @return  void */
static void setup_40gbase_cr4(Aapl_t *aapl, uint addr) {
  BOOL tx_ready, rx_ready;

  avago_serdes_set_tx_rx_enable(
      aapl, addr, FALSE, FALSE, FALSE); /* Disable serdes */
  avago_spico_int(
      aapl, addr, 0x05, 0x8042); /* set serdes bit/ref ratio = 10G */
  avago_serdes_set_tx_rx_width(aapl, addr, 20, 20); /* 20bit width */
  avago_serdes_set_tx_rx_enable(
      aapl, addr, TRUE, TRUE, FALSE);        /* Enable Tx and Rx */
  avago_spico_int(aapl, addr, 0x04, 0x0002); /* launch KR training */
  avago_serdes_set_tx_rx_enable(
      aapl, addr, TRUE, TRUE, TRUE); /* enable serdes for next operations */
  avago_serdes_get_tx_rx_ready(aapl, addr, &tx_ready, &rx_ready);
  if ((tx_ready != 1) || (rx_ready != 1))
    port_mgr_log(
        "Tx or Rx is not ready. Tx bit %d, Rx bit %d ", tx_ready, rx_ready);
}

/** @brief   Configures serdes to 100gbase_cr10. */
/** @details Configures 25.7812G serdes rate with 40-bit width and performs KR
 * training. */
/** @return  void */
static void setup_100gbase_cr10(Aapl_t *aapl, uint addr) {
  BOOL tx_ready, rx_ready;

  avago_serdes_set_tx_rx_enable(
      aapl, addr, FALSE, FALSE, FALSE); /* Disable serdes */
  avago_spico_int(
      aapl, addr, 0x05, 0x80A5); /* set serdes bit/ref ratio = 25G */
  avago_serdes_set_tx_rx_width(aapl, addr, 40, 40); /* 40bit width */
  avago_serdes_set_tx_rx_enable(
      aapl, addr, TRUE, TRUE, FALSE);        /* Enable Tx and Rx */
  avago_spico_int(aapl, addr, 0x04, 0x0002); /* launch KR training */
  avago_serdes_set_tx_rx_enable(
      aapl, addr, TRUE, TRUE, TRUE); /* enable serdes for next operations */
  avago_serdes_get_tx_rx_ready(aapl, addr, &tx_ready, &rx_ready);
  if ((tx_ready != 1) || (rx_ready != 1))
    port_mgr_log(
        "Tx or Rx is not ready. Tx bit %d, Rx bit %d ", tx_ready, rx_ready);
}

/** @brief   Configures serdes to 100gbase_kp4. */
/** @details Configures 13.59375G serdes rate with 80-bit width PAM4. */
/** @return  void */
static void setup_100gbase_kp4(Aapl_t *aapl, uint addr) {
  BOOL tx_ready, rx_ready;

  avago_serdes_set_tx_rx_enable(
      aapl, addr, FALSE, FALSE, FALSE); /* Disable serdes */
  avago_spico_int(
      aapl, addr, 0x05, 0x8057); /* set serdes bit/ref ratio = 13.59G */
  avago_serdes_set_tx_rx_width_pam(
      aapl, addr, 80, 80, AVAGO_SERDES_PAM4, AVAGO_SERDES_PAM4); /*PAM4 80b */
  avago_serdes_set_tx_rx_enable(
      aapl, addr, TRUE, TRUE, TRUE); /* enable serdes for next operations */
  avago_serdes_get_tx_rx_ready(aapl, addr, &tx_ready, &rx_ready);
  if ((tx_ready != 1) || (rx_ready != 1))
    port_mgr_log(
        "Tx or Rx is not ready. Tx bit %d, Rx bit %d ", tx_ready, rx_ready);
}

/** @brief   Configures serdes to 100gbase_kr4. */
/** @details Configures 25.7812G serdes rate with 40-bit width and performs KR
 * training. */
/** @return  void */
static void setup_100gbase_kr4(Aapl_t *aapl, uint addr) {
  BOOL tx_ready, rx_ready;

  avago_serdes_set_tx_rx_enable(
      aapl, addr, FALSE, FALSE, FALSE); /* Disable serdes */
  avago_spico_int(
      aapl, addr, 0x05, 0x80A5); /* set serdes bit/ref ratio = 25G */
  avago_serdes_set_tx_rx_width(aapl, addr, 40, 40); /* 40bit width */
  avago_serdes_set_tx_rx_enable(
      aapl, addr, TRUE, TRUE, FALSE);        /* Enable Tx and Rx */
  avago_spico_int(aapl, addr, 0x04, 0x0002); /* launch KR training */
  avago_serdes_set_tx_rx_enable(
      aapl, addr, TRUE, TRUE, TRUE); /* enable serdes for next operations */
  avago_serdes_get_tx_rx_ready(aapl, addr, &tx_ready, &rx_ready);
  if ((tx_ready != 1) || (rx_ready != 1))
    port_mgr_log(
        "Tx or Rx is not ready. Tx bit %d, Rx bit %d ", tx_ready, rx_ready);
}

/** @brief   Configures serdes to 100gbase_cr10. */
/** @details Configures 25.7812G serdes rate with 40-bit width and performs KR
 * training. */
/** @return  void */
static void setup_100gbase_cr4(Aapl_t *aapl, uint addr) {
  BOOL tx_ready, rx_ready;

  avago_serdes_set_tx_rx_enable(
      aapl, addr, FALSE, FALSE, FALSE); /* Disable serdes */
  avago_spico_int(
      aapl, addr, 0x05, 0x80A5); /* set serdes bit/ref ratio = 25G */
  avago_serdes_set_tx_rx_width(aapl, addr, 40, 40); /* 40bit width */
  avago_serdes_set_tx_rx_enable(
      aapl, addr, TRUE, TRUE, FALSE);        /* Enable Tx and Rx */
  avago_spico_int(aapl, addr, 0x04, 0x0002); /* launch KR training */
  avago_serdes_set_tx_rx_enable(
      aapl, addr, TRUE, TRUE, TRUE); /* enable serdes for next operations */
  avago_serdes_get_tx_rx_ready(aapl, addr, &tx_ready, &rx_ready);
  if ((tx_ready != 1) || (rx_ready != 1))
    port_mgr_log(
        "Tx or Rx is not ready. Tx bit %d, Rx bit %d ", tx_ready, rx_ready);
}

/** @brief   Constructs an auto-negotiation structure. */
/** @details Allocates and initializes memory for configuring AN. */
/** @return  Allocated structure.  Call port_mgr_av_sd_an_config_destruct() to
 * free it. */
Avago_serdes_an_config_t *port_mgr_av_sd_an_config_construct(Aapl_t *aapl) {
  size_t bytes = sizeof(Avago_serdes_an_config_t);
  Avago_serdes_an_config_t *config;

  if (!(config =
            (Avago_serdes_an_config_t *)aapl_malloc(aapl, bytes, __func__)))
    return NULL;
  memset(config, 0, sizeof(*config)); /* set all bytes to zero */
  return config;
}

/** @brief   Releases the resources associated with config. */
/** @see     port_mgr_av_sd_an_config_construct(). */
void port_mgr_av_sd_an_config_destruct(Aapl_t *aapl,
                                       Avago_serdes_an_config_t *config) {
  aapl_free(aapl, config, "Avago_serdes_an_config_t struct");
}

/** @brief   Starts Auto Negotiation. */
/** @details Configures AN and load Base page using Serdes Interrupt 0x29. */
/** @return  On success return 0. */
/** @see     port_mgr_av_sd_an_config_construct(),
 * port_mgr_av_sd_an_config_destruct(). */
uint port_mgr_av_sd_an_start(
    Aapl_t *aapl,                    /**< [in] Pointer to Aapl_t structure. */
    uint addr,                       /**< [in] Serdes address */
    Avago_serdes_an_config_t *config /**< [in] The input configuration struct */
) {
  int AN_input = 0x001; /* Enable AN */
  uint cap;
  uint fec_request;
  uint np;
  uint ret;

  cap = (config->user_cap & 0x7ff) << 5;
  np = (config->np_enable & 0x1) << 15;
  fec_request = (((config->fec_ability & 0x1) << 14) |
                 ((config->fec_request & 0x1) << 15) |
                 ((config->fec_request & 0x2) << 11) |
                 ((config->fec_request & 0x4) << 11));
  /* Base page bit definition: */
  /* D[4:0] Selector field                                           bit[15:0]
   * configured through Int 0x0029 */
  /* D[9:5] contains the Echoed Nonce field */
  /* D[12:10] contains capability bits to advertise capabilities */
  /* D[15:13] contains the RF, Ack, and NP bits                      bit[16:31]
   * configured through Int 0x0229 */
  /* D[20:16] contains the Transmitted Nonce field */
  /* D[45:21] contains the Technology Ability field                  bit[47:32]
   * configured through Int 0x0129 */
  /* D[47:46] contains FEC capability */

  /* Load base page using Serdes Interrupt 0x29 */
  avago_spico_int(aapl, addr, 0x0029, 0x0001 | np);
  avago_spico_int(
      aapl, addr, 0x0129, (config->nonce_user_pattern & 0x1f) | cap);
  avago_spico_int(aapl, addr, 0x0229, 0x000 | fec_request);

  // port_mgr_log("aapl: base-pg: %04x_%04x_%04x",
  //             0x000 | fec_request,
  //             (config->nonce_user_pattern & 0x1f) | cap,
  //             0x0001 | np);

  /* Configure Auto KR */
  if (config->auto_kr) {
    avago_spico_int(aapl, addr, 0x0907, 0x01);
    avago_spico_int(aapl, addr, 0x0A07, config->pmd_config);
  }

  /* auto load device's next pages to null */
  if (config->np_continuous_load == 1)
    avago_spico_int(aapl, addr, 0x0507, 0x02);

  /* No change, same as base page */
  if (config->nonce_user_pattern > 0) config->nonce_pattern_sel = 0x3;

  /* Set user settings from the config struct and enable Auto Negotiation */
  AN_input |= (config->an_clk & 0x1) << 1;
  AN_input |= (config->disable_link_inhibit_timer & 0x1) << 2;
  AN_input |= (config->ignore_nonce_match & 0x1) << 3;
  AN_input |= (config->nonce_pattern_sel & 0x3) << 4;

  /* Final call to configure and execute Auto Negotiation */
  ret = avago_spico_int(aapl, addr, 0x0007, 0x0001 | AN_input);
  // if (ret == 0x7)
  // port_mgr_log("Addr 0x%02x, AN_interrupt_input=0x%x, user_cap=0x%x, ret=%u",
  //             addr,
  //             (AN_input | 0x0001),
  //             config->user_cap,
  //             ret);

  return ret;
}

/** @brief   Reads various status of AN. */
/** @details Call this to read various AN status. */
/** @return  On success, returns actual status. */
/** @return  On error, returns -1. */
int port_mgr_av_sd_an_read_an_status(
    Aapl_t *aapl, /**< [in] Pointer to Aapl_t structure */
    uint addr,    /**< [in] Serdes address */
    Avago_serdes_an_status_t
        status /**< structure defining various AN serdes status */
) {
  uint ret;
  switch (status) {
    case AVAGO_SERDES_AN_READ_HCD:
      ret = (avago_spico_int(aapl, addr, 0x0707, 0x0001)) & 0x0f;
      return ret;
    case AVAGO_SERDES_AN_READ_FEC_ENABLE:
      ret = (avago_spico_int(aapl, addr, 0x0707, 0x0001)) & 0x0100;
      return (ret >> 8);
    case AVAGO_SERDES_AN_BASE_PAGE_RX:
      ret = (avago_spico_int(aapl, addr, 0x0707, 0x0001)) & 0x0200;
      return (ret >> 9);
    case AVAGO_SERDES_AN_NEXT_PAGE_RX:
      ret = ((avago_serdes_mem_rd(aapl, addr, AVAGO_LSB, 0x69)) & 0x0002) >> 1;
      return ret;
    /*ret = (avago_spico_int(aapl, addr, 0x0707, 0x0001)) & 0x0400 ; */
    /*return (ret >> 10); */
    case AVAGO_SERDES_AN_LP_AN_ABLE:
      ret = (avago_spico_int(aapl, addr, 0x0707, 0x0001)) & 0x1000;
      return (ret >> 12);
    case AVAGO_SERDES_AN_COMPLETE:
      ret = ((avago_serdes_mem_rd(aapl, addr, AVAGO_LSB, 0x69)) & 0x0008) >> 3;
      return ret;
    case AVAGO_SERDES_AN_GOOD:
      ret = ((avago_serdes_mem_rd(aapl, addr, AVAGO_LSB, 0x69)) & 0x0010) >> 4;
      return ret;
    default:
      return -1;
  }
}

/** @brief   Loads next page data and assert next page loaded bit. */
/** @details #data_buf contains the next page information to send */
/** @return  On success, returns 0. */
int port_mgr_av_sd_an_next_page_transmit(
    Aapl_t *aapl,        /**< [in] Pointer to Aapl_t structure */
    uint addr,           /**< [in] Serdes address */
    const char *data_buf /**< [in] 48-bit data buffer */
) {
  uint i, j = 0, index = 0;
  long np_data[4] = {0};
  char temp[16];

  /* Convert 48-bit data into chunks of 15bits hex value */
  for (i = 0; i < 14; i++) {
    temp[j++] = data_buf[i];
    if ((data_buf[i] == '_') || (data_buf[i] == ',') || (i == 13)) {
      temp[j] = '\0';
      j = 0;
      if (index >= 4) {
        port_mgr_log_warn(
            "failed to convert 48-bit data into chunks of 15bits hex value");
        return 1;
      }
      np_data[index++] = aapl_strtol(temp, NULL, 16);
    }
  }

  /* Next page bit definition: */
  /* D[10:0]  Message code field */
  /* D[15:11] NP, Ack, MP, Ack2, Toggle bit */
  /* D[47:16] Unformatted Code Field */
  /* Load next page using Serdes Interrupt 0x29 */
  avago_spico_int(aapl, addr, 0x0329, np_data[2] & 0xffff); /* bit[15:0] */
  avago_spico_int(aapl, addr, 0x0429, np_data[1] & 0xffff); /* bit [31:16] */
  avago_spico_int(aapl, addr, 0x0529, np_data[0] & 0xffff); /* bit [47:32] */

  /* allowing next page data transmission and reception to begin */
  avago_spico_int(aapl, addr, 0x0507, 0x1);

  return 0;
}

/** @brief   Monitors o_next_page_rx and validate the received data. */
/** @details #data_buf contains the transmitted data which is compared with the
 * received data */
/** @return  On success, returns 0. */
/** @return  On error, returns -1. */
int port_mgr_av_sd_an_next_page_receive(
    Aapl_t *aapl,        /**< [in] Pointer to Aapl_t structure */
    uint addr,           /**< [in] Serdes address */
    const char *data_buf /**< [in] 48-bit data buffer */
) {
  uint status = 0, i, j = 0, index = 0;
  int wait_time = 0;
  long np_data[4] = {0};
  char temp[16];
  long np_15_0, np_31_16, np_47_32;

  /* monitor o_base_page_rx */
  while ((status == 0) && (wait_time <= 200)) {
    status =
        avago_serdes_read_an_status(aapl, addr, AVAGO_SERDES_AN_NEXT_PAGE_RX);
    ms_sleep(10);
    wait_time += 10;
  }
  if (status != 1)
    return aapl_fail(
        aapl, __func__, __LINE__, "Fail to receive next page on 0x%02x ", addr);

  port_mgr_log("Received next page on 0x%02x", addr);

  /* verify next page data received */
  np_15_0 = avago_spico_int(
      aapl, addr, 0x0729, 0x0); /* Reads bit[15:0] of next page */
  np_31_16 = avago_spico_int(
      aapl, addr, 0x0729, 0x1); /* Reads bit[31:16] of next page */
  np_47_32 = avago_spico_int(
      aapl, addr, 0x0729, 0x2); /* Reads bit[47:32] of next page */

  for (i = 0; i < 14; i++) {
    temp[j++] = data_buf[i];
    if ((data_buf[i] == '_') || (data_buf[i] == ',') || (i == 13)) {
      temp[j] = '\0';
      j = 0;
      if (index >= 4) {
        port_mgr_log_warn(
            "failed to convert 48-bit data into chunks of 15bits hex value");
        return 1;
      }
      np_data[index++] = aapl_strtol(temp, NULL, 16);
    }
  }
  /* Compare read data with the received data */
  if (((np_15_0 & 0x7ff) == (np_data[2] & 0x7ff)) && (np_31_16 == np_data[1]) &&
      (np_47_32 == np_data[0]))
    port_mgr_log("Received correct next page information on 0x%02x", addr);
  else
    return aapl_fail(aapl,
                     __func__,
                     __LINE__,
                     "Received incorrect next page data on 0x%02x ",
                     addr);

  return 0;
}
/** @brief   Monitors an_link_good till HCD technology gets selected. */
/**          Configures serdes to the selected new rate. */
/**          Asserts link and waits for an_complete. */
/** @return  On success, returns 0. */
/** @return  On error, returns -1. */
int port_mgr_av_sd_an_assert_link_status(
    Aapl_t *aapl,                    /**< [in] Pointer to Aapl_t structure */
    uint addr,                       /**< [in] Serdes address */
    Avago_serdes_an_config_t *config /**< [in] The input configuration struct */
) {
  uint status = 0;
  uint an_hcd;
  int wait_time = 0;

  /* wait for an_good which indicate valid HCD has been selected */
  while ((status == 0) && (wait_time <= 200)) {
    status = avago_serdes_read_an_status(aapl, addr, AVAGO_SERDES_AN_GOOD);
    ms_sleep(10);
    wait_time += 10;
  }
  if (status != 1)
    return aapl_fail(
        aapl, __func__, __LINE__, "Fail to achieve AN good on 0x%02x", addr);

  port_mgr_log("Achieved AN link state on 0x%02x", addr);

  /* Read HCD */
  an_hcd = avago_serdes_read_an_status(aapl, addr, AVAGO_SERDES_AN_READ_HCD);
  switch (an_hcd) {
    case 0x00:
      port_mgr_log("HCD Technology selected on serdes 0x%02x: 1000BASE-KX",
                   addr);
      /* configure serdes to the selected rate */
      setup_1000base_kx(aapl, addr);
      avago_spico_int(aapl, addr, 0x0107, 0x01); /* assert link status */
      break;

    case 0x01:
      port_mgr_log("HCD Technology selected on serdes 0x%02x: 10GBASE-KX4",
                   addr);
      /* configure serdes to the selected rate */
      setup_10gbase_kx4(aapl, addr);
      avago_spico_int(aapl, addr, 0x0107, 0x02); /* assert link status */
      break;

    case 0x02:
      port_mgr_log("HCD Technology selected on serdes 0x%02x: 10GBASE-KR",
                   addr);
      /* configure serdes to the selected rate */
      setup_10gbase_kr(aapl, addr);
      avago_spico_int(aapl, addr, 0x0107, 0x04); /* assert link status */
      break;

    case 0x03:
      port_mgr_log("HCD Technology selected on serdes 0x%02x: 40GBASE-KR4",
                   addr);
      /* configure serdes to the selected rate */
      setup_40gbase_kr4(aapl, addr);
      avago_spico_int(aapl, addr, 0x0107, 0x08); /* assert link status */
      break;
    case 0x04:
      port_mgr_log("HCD Technology selected on serdes 0x%02x: 40GBASE-CR4",
                   addr);
      /* configure serdes to the selected rate */
      setup_40gbase_cr4(aapl, addr);
      avago_spico_int(aapl, addr, 0x0107, 0x0010); /* assert link status */
      break;
    case 0x05:
      port_mgr_log("HCD Technology selected on serdes 0x%02x: 100GBASE-CR10",
                   addr);
      /* configure serdes to the selected rate */
      setup_100gbase_cr10(aapl, addr);
      avago_spico_int(aapl, addr, 0x0107, 0x0020); /* assert link status */
      break;
    case 0x06:
      port_mgr_log("HCD Technology selected on serdes 0x%02x: 100GBASE-KP4",
                   addr);
      /* configure serdes to the selected rate */
      setup_100gbase_kp4(aapl, addr);
      avago_spico_int(aapl, addr, 0x0107, 0x0040); /* assert link status */
      break;
    case 0x08:
      port_mgr_log("HCD Technology selected on serdes 0x%02x: 100GBASE-KR4",
                   addr);
      /* configure serdes to the selected rate */
      setup_100gbase_kr4(aapl, addr);
      avago_spico_int(aapl, addr, 0x0107, 0x0080); /* assert link status */
      break;
    case 0x09:
      port_mgr_log("HCD Technology selected on serdes 0x%02x: 100GBASE-CR4",
                   addr);
      /* configure serdes to the selected rate */
      setup_100gbase_cr4(aapl, addr);
      avago_spico_int(aapl, addr, 0x0107, 0x0100); /* assert link status */
      break;
    case 0xa:
      port_mgr_log("HCD Technology selected on serdes 0x%02x: 25GBASE-KRCR-S",
                   addr);
      /* configure serdes to the selected rate */
      setup_25gbase_krcr_s(aapl, addr);
      avago_spico_int(aapl, addr, 0x0107, 0x0200); /* assert link status */
      break;
    case 0xb:
      port_mgr_log("HCD Technology selected on serdes 0x%02x: 25GBASE-KRCR",
                   addr);
      /* configure serdes to the selected rate */
      setup_25gbase_krcr(aapl, addr);
      avago_spico_int(aapl, addr, 0x0107, 0x0400); /* assert link status */
      break;
    case 0x0f:
      return aapl_fail(aapl,
                       __func__,
                       __LINE__,
                       "No HCD Technology selected on serdes 0x%02x ",
                       addr);
  }
  if ((config->fec_ability == 1) || (config->fec_request)) {
    if (an_hcd == 0x00 || an_hcd == 0x01)
      return aapl_fail(aapl,
                       __func__,
                       __LINE__,
                       "Selected HCD 0x%02x does not support FEC enable  ",
                       an_hcd);
    else {
      status = avago_serdes_read_an_status(
          aapl, addr, AVAGO_SERDES_AN_READ_FEC_ENABLE);
      if (status != 1)
        return aapl_fail(aapl,
                         __func__,
                         __LINE__,
                         "Fail to assert FEC Enable. Both device should "
                         "advertise FEC ability and atlease one device should "
                         "request FEC ");
      port_mgr_log("FEC enable bit asserted ");
    }
  }

  wait_time = 0;
  status = 0;
  /* wait for AN complete */
  while ((status == 0) && (wait_time <= 1000)) {
    status = avago_serdes_read_an_status(aapl, addr, AVAGO_SERDES_AN_COMPLETE);
    ms_sleep(10);
    wait_time += 10;
  }
  if (status != 1)
    return aapl_fail(aapl,
                     __func__,
                     __LINE__,
                     "Fail to achieve AN Complete on 0x%02x",
                     addr);

  port_mgr_log("Auto-Negotiation completed on serdes on 0x%02x", addr);
  return 0;
}

/** @brief   Monitors an_link_good till HCD technology gets selected. */
/**          Configures serdes to the selected new rate. */
/**          Asserts link and waits for an_complete. */
/** @return  On success, returns 0. */
/** @return  On error, returns -1. */
uint32_t port_mgr_av_sd_an_assert_link_status_after_an_good(
    Aapl_t *aapl, /**< [in] Pointer to Aapl_t structure */
    uint addr,    /**< [in] Serdes address */
    uint an_hcd) {
#if 1
  uint cur_hcd;

  /* Read HCD */
  cur_hcd = avago_serdes_read_an_status(aapl, addr, AVAGO_SERDES_AN_READ_HCD);
  if (cur_hcd != an_hcd) {
    port_mgr_log("Training failed (AN restarted) on addr=%x (hcd=%d : cur=%d)",
                 addr,
                 an_hcd,
                 cur_hcd);
    return -1;
  }
#endif
  switch (an_hcd) {
    case 0x00:
      port_mgr_log(
          "AAPL: HCD Technology selected on serdes 0x%02x: 1000BASE-KX", addr);
      avago_spico_int(aapl, addr, 0x0107, 0x01); /* assert link status */
      break;
    case 0x01:
      port_mgr_log(
          "AAPL: HCD Technology selected on serdes 0x%02x: 10GBASE-KX4", addr);
      avago_spico_int(aapl, addr, 0x0107, 0x02); /* assert link status */
      break;
    case 0x02:
      port_mgr_log("AAPL: HCD Technology selected on serdes 0x%02x: 10GBASE-KR",
                   addr);
      avago_spico_int(aapl, addr, 0x0107, 0x04); /* assert link status */
      break;
    case 0x03:
      port_mgr_log(
          "AAPL: HCD Technology selected on serdes 0x%02x: 40GBASE-KR4", addr);
      avago_spico_int(aapl, addr, 0x0107, 0x08); /* assert link status */
      break;
    case 0x04:
      port_mgr_log(
          "AAPL: HCD Technology selected on serdes 0x%02x: 40GBASE-CR4", addr);
      avago_spico_int(aapl, addr, 0x0107, 0x0010); /* assert link status */
      break;
    case 0x05:
      port_mgr_log(
          "AAPL: HCD Technology selected on serdes 0x%02x: 100GBASE-CR10",
          addr);
      avago_spico_int(aapl, addr, 0x0107, 0x0020); /* assert link status */
      break;
    case 0x06:
      port_mgr_log(
          "AAPL: HCD Technology selected on serdes 0x%02x: 100GBASE-KP4", addr);
      avago_spico_int(aapl, addr, 0x0107, 0x0040); /* assert link status */
      break;
    case 0x08:
      port_mgr_log(
          "AAPL: HCD Technology selected on serdes 0x%02x: 100GBASE-KR4", addr);
      avago_spico_int(aapl, addr, 0x0107, 0x0080); /* assert link status */
      break;
    case 0x09:
      port_mgr_log(
          "AAPL: HCD Technology selected on serdes 0x%02x: 100GBASE-CR4", addr);
      avago_spico_int(aapl, addr, 0x0107, 0x0100); /* assert link status */
      break;
    case 0xa:
      port_mgr_log(
          "AAPL: HCD Technology selected on serdes 0x%02x: 25GBASE-KRCR-S",
          addr);
      avago_spico_int(aapl, addr, 0x0107, 0x0200); /* assert link status */
      break;
    case 0xb:
      port_mgr_log(
          "AAPL: HCD Technology selected on serdes 0x%02x: 25GBASE-KRCR", addr);
      avago_spico_int(aapl, addr, 0x0107, 0x0400); /* assert link status */
      break;
    case 0x0f:
      return aapl_fail(aapl,
                       __func__,
                       __LINE__,
                       "No HCD Technology selected on serdes 0x%02x ",
                       addr);
  }
  return 0;
}

#endif /* AAPL_ENABLE_SERDES_AUTO_NEG */

/* **************************************************************** */
/*                                                                  */
/* ASIC and ASSP Programming Layer (AAPL)                           */
/* Copyright (c) 2014-2017 Avago Technologies. All rights reserved. */
/*                                                                  */
/* **************************************************************** */
/* AAPL Revision: 2.5.0                                        */
/* AAPL (ASIC and ASSP Programming Layer) generic handling of SerDes */
/* (Serializer/Deserializer) slices on ASIC SBus rings; all functions exported */
/* from this directory. */

/** Doxygen File Header */
/** @file */
/** @brief Declarations for SerDes AN functions. */

#ifndef AVAGO_SERDES_AN_H_
#define AVAGO_SERDES_AN_H_

#if AAPL_ENABLE_SERDES_AUTO_NEG

/**@brief Configuration setting to perform AN. */
typedef struct
{
    uint an_clk;                     /**< Defines what speed AN will occur at: */
                                     /**< - 0x00 = 1.25GB/s */
                                     /**< - 0x01 = 3.125 GB/s */
    BOOL disable_link_inhibit_timer; /**< - 0x01 = disable timer */
                                     /**< - 0x00 = enable timer */
    BOOL ignore_nonce_match;         /**< - 0x00 = perform nonce match */
                                     /**< - 0x01 = ignore nonce pattern match */
    uint nonce_pattern_sel;          /**< Whether AN will use: */
                                     /**< - 0 : PRBS 7 */
                                     /**< - 1 : PRBS 9 */
                                     /**< - 3 : user defined nonce pattern */
    uint nonce_user_pattern;         /**< 5 bit user defined nonce pattern */
    uint user_cap;                   /**< The user's capability list. Value as follows for different capabilities */
                                     /**< 0x1=1000Bkx 0x2=10GBkx4 0x4=10GBkr 0x8=40GBkr4 0x10=40GBcr4 0x20=100GBcr10 0x40=100GBkp4 */
                                     /**< 0x80=100GBkr4 0x100=100GBcr4 0x200=25GBikrcr_s 0x400=25GBkrcr */
    BOOL auto_kr;                    /**<  Whether or not Auto KR training should be perfromed after AN */
                                     /**< - 0 = Disable 1 = enable */
    uint pmd_config;                 /**< PMD core interrupt data  for Int 0x04 which is enabled by auto_kr */
    BOOL pause;                      /**< Symmetric PAUSE ability */
                                     /**< - 0 = Disable 1 = enable */
    BOOL asm_dir;                    /**< Asymmetric PAUSE ability */
                                     /**< - 0 = Disable 1 = enable */
    BOOL np_enable;                  /**< 0x0 next page transmission disables */
                                     /**< 0x1 there is next page to send */
    BOOL fec_ability;                /**< Selected FEC ability */
    uint fec_request;                /**< The FEC capability list. Value as follows for different capabilities */
                                     /**< 0x01=10Gb/s FEC requested    - F1 */
                                     /**< 0x02=25G/bs RS-FEC requested - F2 */
                                     /**< 0x04=25G/bs FEC requested    - F3 */
    BOOL np_continuous_load;         /**< Set to 1 when there is no next pages to send. It will auto load next pages with null */

} Avago_serdes_an_config_t;

/**@brief Read different AN status. */
typedef enum
{
    AVAGO_SERDES_AN_READ_HCD = 0,
    AVAGO_SERDES_AN_READ_FEC_ENABLE = 1,
    AVAGO_SERDES_AN_BASE_PAGE_RX = 2,
    AVAGO_SERDES_AN_NEXT_PAGE_RX = 3,
    AVAGO_SERDES_AN_LP_AN_ABLE = 4,
    AVAGO_SERDES_AN_COMPLETE = 5,
    AVAGO_SERDES_AN_GOOD = 6,
    AVAGO_SERDES_AN_READ_RSFEC_ENABLE = 7
} Avago_serdes_an_status_t;

EXT Avago_serdes_an_config_t *avago_serdes_an_config_construct(Aapl_t *aapl);
EXT void avago_serdes_an_config_destruct(Aapl_t *aapl, Avago_serdes_an_config_t *config);
EXT uint avago_serdes_an_start(Aapl_t *aapl, uint sbus_address, Avago_serdes_an_config_t *config);
EXT int avago_serdes_an_configure_to_hcd(Aapl_t *aapl, Avago_addr_t *addr_list, Avago_serdes_an_config_t *config, Avago_serdes_pmd_config_t *pmd_config);
EXT int avago_serdes_an_complete(Aapl_t *aapl, uint addr, uint an_hcd, Avago_serdes_an_config_t *config);
EXT int avago_serdes_an_assert_link_status(Aapl_t *aapl, uint addr, uint an_hcd);
EXT int avago_serdes_read_an_status(Aapl_t *aapl, uint addr, Avago_serdes_an_status_t status);
EXT int avago_serdes_an_next_page_transmit(Aapl_t *aapl, uint addr, const char *data_buf);
EXT int avago_serdes_an_next_page_receive(Aapl_t *aapl, uint addr, const char *data_buf);

#endif /* AAPL_ENABLE_SERDES_AUTO_NEG */

#endif /* AVAGO_SERDES_AN_H_ */





/* **************************************************************** */
/*                                                                  */
/* ASIC and ASSP Programming Layer (AAPL)                           */
/* Copyright (c) 2014-2017 Avago Technologies. All rights reserved. */
/*                                                                  */
/* **************************************************************** */
/* AAPL Revision: 2.5.0                                        */
/* AAPL (ASIC and ASSP Programming Layer) generic handling of SerDes */
/* (Serializer/Deserializer) slices on ASIC SBus rings. */

/** Doxygen File Header */
/** @file */
/** @brief Function and type definitions for generic SerDes devices. */

#ifndef AVAGO_SERDES_H_
#define AVAGO_SERDES_H_

/** @brief  Defines for the TX_EQ values that have special features or requirements. */
typedef enum
{
    AVAGO_TX_EQ_FEATURE_NONE,
    AVAGO_TX_EQ_FEATURE_EVEN_VALUES_ONLY
} Avago_serdes_tx_eq_limits_feature_t;
/** @brief  The minimum and maximum supported values for the transmitter */
/**         equalization settings. */
/**         Note that values are device type specific. */
typedef struct
{
    int pre_min;        /**< Minimum pre-cursor value. */
    int pre_step;       /**< Step size for pre-cursor values. */
    int pre_max;        /**< Maximum pre-cursor value. */
    int t2_min;         /**< Minimum t2 value. */
    int t2_max;         /**< Maximum t2 value. */
    Avago_serdes_tx_eq_limits_feature_t pre_feature;    /**< Any special features pertain, 0-no special features, 1-even values only */
    int pre2_min;       /**< Minimum pre2-cursor value. */
    int pre2_max;       /**< Maximum pre2-cursor value. */
    int pre3_min;       /**< Minimum pre3-cursor value. */
    int pre3_max;       /**< Maximum pre3-cursor value. */
    int atten_min;      /**< Minimum attenuator value. */
    int atten_step;     /**< Step size for attenuator values. */
    int atten_max;      /**< Maximum attenuator value. */
    int post_min;       /**< Minimum post-cursor value. */
    int post_step;      /**< Step size for post-cursor values. */
    int post_max;       /**< Maximum post-cursor value. */
    Avago_serdes_tx_eq_limits_feature_t post_feature;   /**< Any special features pertain, 0-no special features, 1-even values only */
    int vert_min;       /**< Minimum vertical skew value. */
    int vert_max;       /**< Maximum vertical skew value. */
    int amp_min;        /**< Minimum amp value. */
    int amp_max;        /**< Maximum amp value. */
    int slew_min;       /**< Minimum slew value. */
    int slew_max;       /**< Maximum slew value. */
    int total_eq;       /**< Maximum total equalization allowed: sum(abs(pre),abs(atten),abs(post)) */
} Avago_serdes_tx_eq_limits_t;
EXT int avago_serdes_get_tx_eq_limits(Aapl_t *aapl, uint addr,
                                      Avago_serdes_tx_eq_limits_t *tx_eq_limits);

/** @brief Validate the Tx equalization value. */
EXT int avago_serdes_validate_tx_eq(Aapl_t *aapl, uint addr,
                                      Avago_serdes_tx_eq_t *tx_eq);

/** @brief Data qualification filter for purposes of eye measurement. */
typedef enum
{
    /* Note: Partial eyes are listed first and ordered to match */
    /*   vert_alpha[] array elements [0..5]. */
    /*  Note: See the data_compare_config section of the serdes firmware doc for */
    /*  other valid values: */
    /*    Bits [13:11]: 000 ALL, 100 PREV0, 101 PREV1 */
    /*    Bits [15:14]: 00 ALL, 01 EVEN, 10 ODD */
    AVAGO_SERDES_RX_DATA_QUAL_UNQUAL= 0,     /**< Select all bits */
    AVAGO_SERDES_RX_DATA_QUAL_DEFAULT=0x0100,/**< prev0 + prev1 */
    AVAGO_SERDES_RX_DATA_QUAL_UNLOCK =0x0101,/**< Release counter resource, reset to prev_bits. */
    AVAGO_SERDES_RX_DATA_QUAL_EVEN  = 0x8000,/**< Select only even bits */
    AVAGO_SERDES_RX_DATA_QUAL_ODD   = 0x4000,/**< Select only odd bits */
    AVAGO_SERDES_RX_DATA_QUAL_PREV0 = 0x2000,/**< Only select bits following a 0. */
    AVAGO_SERDES_RX_DATA_QUAL_PREV1 = 0x2800,/**< Only select bits following a 1. */
    AVAGO_SERDES_RX_DATA_QUAL_PREV0E= 0xa000,/**< Only select bits following a 0 from even channel. */
    AVAGO_SERDES_RX_DATA_QUAL_PREV1E= 0xa800,/**< Only select bits following a 1 from even channel. */
    AVAGO_SERDES_RX_DATA_QUAL_PREV0O= 0x6000,/**< Only select bits following a 0 from odd channel. */
    AVAGO_SERDES_RX_DATA_QUAL_PREV1O= 0x6800 /**< Only select bits following a 1 from odd channel. */
} Avago_serdes_rx_data_qual_t;

EXT Avago_serdes_rx_data_qual_t avago_serdes_get_rx_data_qual(Aapl_t *aapl, uint sbus);
EXT int                         avago_serdes_set_rx_data_qual(Aapl_t *aapl, uint sbus,
                                                              Avago_serdes_rx_data_qual_t qual);

typedef struct
{
    Avago_serdes_rx_data_qual_t d6_data_qual;
    unsigned short cmp_data_compare_en[4];  /**< Full data_qual selections, NRZ uses [7:0] of [0]. */
    char  pam;              /**< TRUE if pam pattern */
    char  select;           /**< [1:0] Mask. 0-all,1-even,2-odd. */
                            /**< [3:2] Counter input select. 0-all,1-lsb,2-msb. */
    char  cmp_valid_pre;    /**< [3] Enable, [2:0] Pre, NRZ use 0 or 7. */
    char  cmp_valid_post;   /**< [3] Enable, [2:0] Post, NRZ use 0 or 7. */
} Avago_serdes_data_qual_t;

EXT void avago_serdes_data_qual_init(Avago_serdes_data_qual_t *ext_qual);
EXT Avago_serdes_data_qual_t avago_serdes_get_data_qual(Aapl_t *aapl, uint sbus);
EXT int                      avago_serdes_set_data_qual(Aapl_t *aapl, uint sbus, Avago_serdes_data_qual_t qual);

#ifdef AAPL_ENABLE_INTERNAL_FUNCTIONS
EXT void aapl_construct_d6_data_qual(Avago_serdes_data_qual_t *dq);
EXT int aapl_get_pcs_architecture(Aapl_t *aapl, uint addr);
#endif

/** @brief Select the PCS FIFO clock source. */
typedef enum
{
    AVAGO_SERDES_PCS_FIFO_F66     = 0x0000, /**< default */
    AVAGO_SERDES_PCS_FIFO_F50     = 0x0800, /**< 28nm design only */
    AVAGO_SERDES_PCS_FIFO_F60     = 0x0900, /**< 28nm design only */
    AVAGO_SERDES_PCS_FIFO_F70     = 0x0a00, /**< 28nm design only */
    AVAGO_SERDES_PCS_FIFO_F80     = 0x0b00, /**< 28nm design only */
    AVAGO_SERDES_PCS_FIFO_F90     = 0x0c00, /**< 28nm design only */
    AVAGO_SERDES_PCS_FIFO_F100    = 0x0d00, /**< 28nm design only */
    AVAGO_SERDES_PCS_FIFO_F110    = 0x0e00, /**< 28nm design only */
    AVAGO_SERDES_PCS_FIFO_F120    = 0x0f00, /**< 28nm design only */
    AVAGO_SERDES_PCS_FIFO_RX_DIVX = 0x0100, /**< 16nm design only */
    AVAGO_SERDES_PCS_FIFO_F68     = 0x0300, /**< 16nm design only */
    AVAGO_SERDES_PCS_FIFO_F33     = 0x0400, /**< 16nm design only */
    AVAGO_SERDES_PCS_FIFO_GND     = 0x1000  /**< P1 only valid value. */
} Avago_serdes_pcs_fifo_clk_t;

EXT Avago_serdes_pcs_fifo_clk_t avago_serdes_get_pcs_fifo_clk_div(Aapl_t *aapl, uint sbus_addr);
EXT int                         avago_serdes_set_pcs_fifo_clk_div(Aapl_t *aapl, uint sbus_addr,
                                                                  Avago_serdes_pcs_fifo_clk_t div);

typedef struct {
    BOOL info;   /* print information when running */
    BOOL dfe_tune; /* enable DFE tuning */
    BOOL eye;    /* gather eye information when complete */

    /* SerDes A: */
    Aapl_t *aapl_a;
    uint addr_a; /* Address for SerDes A */
    Avago_serdes_init_config_t *serdes_config_a; /* config struct for SerDes A */
    Avago_serdes_dfe_state_t *serdes_dfe_config_a;

    /* SerDes B: */
    Aapl_t *aapl_b;
    uint addr_b; /* Address for SerDes B */
    Avago_serdes_init_config_t *serdes_config_b; /* config struct for SerDes B */
    Avago_serdes_dfe_state_t *serdes_dfe_config_b;
} Avago_serdes_link_init_config_t;

EXT Avago_serdes_link_init_config_t *avago_serdes_link_init_config_construct(Aapl_t *aapl_a, uint addr_a, Aapl_t *aapl_b, uint addr_b);
EXT void avago_serdes_link_init_config_destruct(Aapl_t *aapl, Avago_serdes_link_init_config_t *config);
EXT BOOL avago_serdes_link_init(Avago_serdes_link_init_config_t *config);
EXT BOOL avago_serdes_link_init_quick(Aapl_t *aapl_a, uint addr_a, uint div_a, Aapl_t *aapl_b, uint addr_b, uint div_b);


EXT BOOL avago_serdes_is_valid_divider(Aapl_t *aapl, uint sbus_addr, uint divider);


typedef enum
{
    AVAGO_AN_HCD_1000BASE_KX,
    AVAGO_AN_HCD_10GBASE_KX4,
    AVAGO_AN_HCD_10GBASE_KR,
    AVAGO_AN_HCD_40GBASE_KR4,
    AVAGO_AN_HCD_40GBASE_CR4,
    AVAGO_AN_HCD_100GBASE_CR10,
    AVAGO_AN_HCD_100GBASE_KP4,
    AVAGO_AN_HCD_0X07,
    AVAGO_AN_HCD_100GBASE_KR4,
    AVAGO_AN_HCD_100GBASE_CR4,
    AVAGO_AN_HCD_0X0A,
    AVAGO_AN_HCD_0X0B,
    AVAGO_AN_HCD_0X0C,
    AVAGO_AN_HCD_0X0D,
    AVAGO_AN_HCD_0X0E,
    AVAGO_AN_HCD_NONE
} Avago_an_hcd_t;

typedef struct
{
    uint ip_rev;
    BOOL enabled;
    Avago_an_hcd_t hcd;

} Avago_an_info_t;

EXT BOOL avago_an_get_info(Aapl_t *aapl, uint sbus_addr, Avago_an_info_t *an_info);

#if 0 /* Unsupported in AAPL. */
/* Joins SerDes to supplied group broadcast address. */
/* SerDes can be joined to 2 group broadcast addresses (given by addr_sel, which should be 1 or 2) */
/* When calling these functions, this will overwrite any previous group broadcast address applied. */
EXT void avago_serdes_set_group_broadcast_list( Aapl_t *aapl, int broadcast_addr, int addr_sel, int n, ...);
EXT void avago_serdes_set_group_broadcast_array(Aapl_t *aapl, int broadcast_addr, int addr_sel, int n, int *sbus_addr_array);
#endif

#endif /* AVAGO_SERDES_H_ */


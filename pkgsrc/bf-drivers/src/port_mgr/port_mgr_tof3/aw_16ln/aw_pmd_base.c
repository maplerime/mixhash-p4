/*
** ? 2020 Alphawave IP Inc.
*/

#include <string.h>

#include "aw_alphacore.h"

// TODO: we should consider adding these to the C API library
//Default timeouts (slow sim)
typedef enum aw_pmd_timeouts_e {
    CMN_ACK_TIMEOUT_US                   = 50,
    TX_ACK_TIMEOUT_US                    = 1200,
    TX_ACK_P1_TIMEOUT_US                 = 1100,
    TX_ACK_P2_TIMEOUT_US                 = 900,
    RX_ACK_TIMEOUT_US                    = 500,
    RX_ACK_P2_TIMEOUT_US                 = 100,
    RX_CDR_TIMEOUT_US                    = 150,
    RX_BIST_TIMEOUT_US                   = 200,
    RX_LINKEVAL_FULL_TIMEOUT_US          = 1800,
    TX_RXDET_TIMEOUT_US                  = 550,
} aw_pmd_timeouts_t;

typedef enum aw_pmd_par_data_width_e {
    AW_WIDTH128 = 0x7,
    AW_WIDTH64  = 0x6,
    AW_WIDTH40  = 0x5,
    AW_WIDTH32  = 0x4,
    AW_WIDTH20  = 0x3,
    AW_WIDTH16  = 0x2,
} aw_pmd_par_data_width_t;

typedef enum aw_rate_preset_e {
    AW_RATE0 = 0x0,
    AW_RATE1 = 0x1,
    AW_RATE2 = 0x2,
    AW_RATE3 = 0x3,
    AW_RATE4 = 0x4,
    AW_RATE5 = 0x5,
    AW_RATE6 = 0x6,
    AW_RATE7 = 0x7,
} aw_rate_preset_t;


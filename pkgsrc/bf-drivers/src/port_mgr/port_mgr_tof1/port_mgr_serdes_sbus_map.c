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

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <unistd.h>

#include <tofino_regs/tofino.h>
#include <bf_types/bf_types.h>
#include <lld/lld_err.h>
#include <lld/lld_sku.h>
#include <dvm/bf_drv_intf.h>
#include <port_mgr/port_mgr_intf.h>
#include <port_mgr/port_mgr_ha.h>
#include <port_mgr/port_mgr.h>
#include <port_mgr/port_mgr_map.h>
#include <port_mgr/port_mgr_log.h>
#include "port_mgr_tof1_map.h"
#include <port_mgr/port_mgr_serdes_sbus_map.h>

// for aim_printf
#include <target-utils/uCli/ucli.h>
#include <target-utils/uCli/ucli_argparse.h>
#include <target-utils/uCli/ucli_handler_macros.h>

port_mgr_sbus_node_map_t tofino_sbus_ring0_node_map[] = {
    {/* 00 */ IP_UNPOPULATED, 0, 0, "unpopulated"},
    {/* 01 */ IP_TYPE_PCIE_PLL, 0, 0, "PCIe PLL"},
    {/* 02 */ IP_TYPE_CORE_PLL, 0, 0, "Core PLL"},

    {/* 03 */ IP_TYPE_PCIE_PMA, 0, 0, "PCIe PMA"},
    {/* 04 */ IP_TYPE_PCIE_PCS, 0, 0, "PCIe PCS"},
    {/* 05 */ IP_TYPE_PCIE_PMA, 0, 1, "PCIe PMA"},
#ifdef AVAGO_EVAL_BOARD
    /* If we are running against the eval board, which
     * only has a few serdes nodes (6-9), pretned those
     * serdes correspond to mac[14-15] lanes 0-1.
     * This allows us to "bring up",
     *    6: bf_dev_port(0,0)
     *    8: bf_dev_port(0,1)
     *    7: bf_dev_port(1,0)
     *    9: bf_dev_port(1,1)
     *
     * including the serdes initialization.
     * On the eval board we have connected,
     *    6 <--> 7
     *    8 <--> 9
     *
     * This makes the following bf_dev_ports link-partners,
     *    bf_dev_port(0,0 <--> bf_dev_port(1,0)
     *    bf_dev_port(1,0 <--> bf_dev_port(1,1)
     *
     * With this we can bring up mac[14] and mac[15] at
     * the following speeds (1 or 2 lane speeds),
     *    1g
     *   10g
     *   25g
     *   40g mlg
     *   50g
     */
    {/* 06 */ IP_TYPE_ETH_PMA, 0, 0, "Eth PMA"},
    {/* 07 */ IP_TYPE_ETH_PMA, 1, 0, "Eth PMA"},
    {/* 08 */ IP_TYPE_ETH_PMA, 0, 1, "Eth PMA"},
    {/* 09 */ IP_TYPE_ETH_PMA, 1, 1, "Eth PMA"},
#else
    {/* 06 */ IP_TYPE_PCIE_PCS, 0, 1, "PCIe PCS"},
    {/* 07 */ IP_TYPE_PCIE_PMA, 0, 2, "PCIe PMA"},
    {/* 08 */ IP_TYPE_PCIE_PCS, 0, 2, "PCIe PCS"},
    {/* 09 */ IP_TYPE_PCIE_PMA, 0, 3, "PCIe PMA"},
#endif  // AVAGO_EVAL_BOARD
    {/* 10 */ IP_TYPE_PCIE_PCS, 0, 3, "PCIe PCS"},

    {/* 11 */ IP_TYPE_ETH_PMA, 0, 0, "Eth PMA"},
    {/* 12 */ IP_TYPE_ETH_PMA, 0, 1, "Eth PMA"},
    {/* 13 */ IP_TYPE_ETH_PMA, 0, 2, "Eth PMA"},
    {/* 14 */ IP_TYPE_ETH_PMA, 0, 3, "Eth PMA"},
    {/* 15 */ IP_TYPE_ETH_PMA, 1, 0, "Eth PMA"},
    {/* 16 */ IP_TYPE_ETH_PMA, 1, 1, "Eth PMA"},
    {/* 17 */ IP_TYPE_ETH_PMA, 1, 2, "Eth PMA"},
    {/* 18 */ IP_TYPE_ETH_PMA, 1, 3, "Eth PMA"},
    {/* 19 */ IP_TYPE_ETH_PMA, 2, 0, "Eth PMA"},
    {/* 20 */ IP_TYPE_ETH_PMA, 2, 1, "Eth PMA"},
    {/* 21 */ IP_TYPE_ETH_PMA, 2, 2, "Eth PMA"},
    {/* 22 */ IP_TYPE_ETH_PMA, 2, 3, "Eth PMA"},
    {/* 23 */ IP_TYPE_ETH_PMA, 3, 0, "Eth PMA"},
    {/* 24 */ IP_TYPE_ETH_PMA, 3, 1, "Eth PMA"},
    {/* 25 */ IP_TYPE_ETH_PMA, 3, 2, "Eth PMA"},
    {/* 26 */ IP_TYPE_ETH_PMA, 3, 3, "Eth PMA"},
    {/* 27 */ IP_TYPE_ETH_PMA, 4, 0, "Eth PMA"},
    {/* 28 */ IP_TYPE_ETH_PMA, 4, 1, "Eth PMA"},
    {/* 29 */ IP_TYPE_ETH_PMA, 4, 2, "Eth PMA"},
    {/* 30 */ IP_TYPE_ETH_PMA, 4, 3, "Eth PMA"},
    {/* 31 */ IP_TYPE_ETH_PMA, 5, 0, "Eth PMA"},
    {/* 32 */ IP_TYPE_ETH_PMA, 5, 1, "Eth PMA"},
    {/* 33 */ IP_TYPE_ETH_PMA, 5, 2, "Eth PMA"},
    {/* 34 */ IP_TYPE_ETH_PMA, 5, 3, "Eth PMA"},
    {/* 35 */ IP_TYPE_ETH_PMA, 6, 0, "Eth PMA"},
    {/* 36 */ IP_TYPE_ETH_PMA, 6, 1, "Eth PMA"},
    {/* 37 */ IP_TYPE_ETH_PMA, 6, 2, "Eth PMA"},
    {/* 38 */ IP_TYPE_ETH_PMA, 6, 3, "Eth PMA"},
    {/* 39 */ IP_TYPE_ETH_PMA, 7, 0, "Eth PMA"},
    {/* 40 */ IP_TYPE_ETH_PMA, 7, 1, "Eth PMA"},
    {/* 41 */ IP_TYPE_ETH_PMA, 7, 2, "Eth PMA"},
    {/* 42 */ IP_TYPE_ETH_PMA, 7, 3, "Eth PMA"},

    {/* 43 */ IP_TYPE_MAC_PLL, 0, 0, "MAC PLL"},

    {/* 44 */ IP_TYPE_ETH_PMA, 8, 0, "Eth PMA"},
    {/* 45 */ IP_TYPE_ETH_PMA, 8, 1, "Eth PMA"},
    {/* 46 */ IP_TYPE_ETH_PMA, 8, 2, "Eth PMA"},
    {/* 47 */ IP_TYPE_ETH_PMA, 8, 3, "Eth PMA"},
    {/* 48 */ IP_TYPE_ETH_PMA, 9, 0, "Eth PMA"},
    {/* 49 */ IP_TYPE_ETH_PMA, 9, 1, "Eth PMA"},
    {/* 50 */ IP_TYPE_ETH_PMA, 9, 2, "Eth PMA"},
    {/* 51 */ IP_TYPE_ETH_PMA, 9, 3, "Eth PMA"},
    {/* 52 */ IP_TYPE_ETH_PMA, 10, 0, "Eth PMA"},
    {/* 53 */ IP_TYPE_ETH_PMA, 10, 1, "Eth PMA"},
    {/* 54 */ IP_TYPE_ETH_PMA, 10, 2, "Eth PMA"},
    {/* 55 */ IP_TYPE_ETH_PMA, 10, 3, "Eth PMA"},
    {/* 56 */ IP_TYPE_ETH_PMA, 11, 0, "Eth PMA"},
    {/* 57 */ IP_TYPE_ETH_PMA, 11, 1, "Eth PMA"},
    {/* 58 */ IP_TYPE_ETH_PMA, 11, 2, "Eth PMA"},
    {/* 59 */ IP_TYPE_ETH_PMA, 11, 3, "Eth PMA"},
    {/* 60 */ IP_TYPE_ETH_PMA, 12, 0, "Eth PMA"},
    {/* 61 */ IP_TYPE_ETH_PMA, 12, 1, "Eth PMA"},
    {/* 62 */ IP_TYPE_ETH_PMA, 12, 2, "Eth PMA"},
    {/* 63 */ IP_TYPE_ETH_PMA, 12, 3, "Eth PMA"},
    {/* 64 */ IP_TYPE_ETH_PMA, 13, 0, "Eth PMA"},
    {/* 65 */ IP_TYPE_ETH_PMA, 13, 1, "Eth PMA"},
    {/* 66 */ IP_TYPE_ETH_PMA, 13, 2, "Eth PMA"},
    {/* 67 */ IP_TYPE_ETH_PMA, 13, 3, "Eth PMA"},
    {/* 68 */ IP_TYPE_ETH_PMA, 14, 0, "Eth PMA"},
    {/* 69 */ IP_TYPE_ETH_PMA, 14, 1, "Eth PMA"},
    {/* 70 */ IP_TYPE_ETH_PMA, 14, 2, "Eth PMA"},
    {/* 71 */ IP_TYPE_ETH_PMA, 14, 3, "Eth PMA"},
    {/* 72 */ IP_TYPE_ETH_PMA, 15, 0, "Eth PMA"},
    {/* 73 */ IP_TYPE_ETH_PMA, 15, 1, "Eth PMA"},
    {/* 74 */ IP_TYPE_ETH_PMA, 15, 2, "Eth PMA"},
    {/* 75 */ IP_TYPE_ETH_PMA, 15, 3, "Eth PMA"},
    {/* 76 */ IP_TYPE_ETH_PMA, 16, 0, "Eth PMA"},
    {/* 77 */ IP_TYPE_ETH_PMA, 16, 1, "Eth PMA"},
    {/* 78 */ IP_TYPE_ETH_PMA, 16, 2, "Eth PMA"},
    {/* 79 */ IP_TYPE_ETH_PMA, 16, 3, "Eth PMA"},
    {/* 80 */ IP_TYPE_ETH_PMA, 17, 0, "Eth PMA"},
    {/* 81 */ IP_TYPE_ETH_PMA, 17, 1, "Eth PMA"},
    {/* 82 */ IP_TYPE_ETH_PMA, 17, 2, "Eth PMA"},
    {/* 83 */ IP_TYPE_ETH_PMA, 17, 3, "Eth PMA"},
    {/* 84 */ IP_TYPE_ETH_PMA, 18, 0, "Eth PMA"},
    {/* 85 */ IP_TYPE_ETH_PMA, 18, 1, "Eth PMA"},
    {/* 86 */ IP_TYPE_ETH_PMA, 18, 2, "Eth PMA"},
    {/* 87 */ IP_TYPE_ETH_PMA, 18, 3, "Eth PMA"},
    {/* 88 */ IP_TYPE_ETH_PMA, 19, 0, "Eth PMA"},
    {/* 89 */ IP_TYPE_ETH_PMA, 19, 1, "Eth PMA"},
    {/* 90 */ IP_TYPE_ETH_PMA, 19, 2, "Eth PMA"},
    {/* 91 */ IP_TYPE_ETH_PMA, 19, 3, "Eth PMA"},
    {/* 92 */ IP_TYPE_ETH_PMA, 20, 0, "Eth PMA"},
    {/* 93 */ IP_TYPE_ETH_PMA, 20, 1, "Eth PMA"},
    {/* 94 */ IP_TYPE_ETH_PMA, 20, 2, "Eth PMA"},
    {/* 95 */ IP_TYPE_ETH_PMA, 20, 3, "Eth PMA"},
    {/* 96 */ IP_TYPE_ETH_PMA, 21, 0, "Eth PMA"},
    {/* 97 */ IP_TYPE_ETH_PMA, 21, 1, "Eth PMA"},
    {/* 98 */ IP_TYPE_ETH_PMA, 21, 2, "Eth PMA"},
    {/* 99 */ IP_TYPE_ETH_PMA, 21, 3, "Eth PMA"},
    {/*100 */ IP_TYPE_ETH_PMA, 22, 0, "Eth PMA"},
    {/*101 */ IP_TYPE_ETH_PMA, 22, 1, "Eth PMA"},
    {/*102 */ IP_TYPE_ETH_PMA, 22, 2, "Eth PMA"},
    {/*103 */ IP_TYPE_ETH_PMA, 22, 3, "Eth PMA"},
    {/*104 */ IP_TYPE_ETH_PMA, 23, 0, "Eth PMA"},
    {/*105 */ IP_TYPE_ETH_PMA, 23, 1, "Eth PMA"},
    {/*106 */ IP_TYPE_ETH_PMA, 23, 2, "Eth PMA"},
    {/*107 */ IP_TYPE_ETH_PMA, 23, 3, "Eth PMA"},
    {/*108 */ IP_TYPE_ETH_PMA, 24, 0, "Eth PMA"},
    {/*109 */ IP_TYPE_ETH_PMA, 24, 1, "Eth PMA"},
    {/*110 */ IP_TYPE_ETH_PMA, 24, 2, "Eth PMA"},
    {/*111 */ IP_TYPE_ETH_PMA, 24, 3, "Eth PMA"},
    {/*112 */ IP_TYPE_ETH_PMA, 25, 0, "Eth PMA"},
    {/*113 */ IP_TYPE_ETH_PMA, 25, 1, "Eth PMA"},
    {/*114 */ IP_TYPE_ETH_PMA, 25, 2, "Eth PMA"},
    {/*115 */ IP_TYPE_ETH_PMA, 25, 3, "Eth PMA"},
    {/*116 */ IP_TYPE_ETH_PMA, 26, 0, "Eth PMA"},
    {/*117 */ IP_TYPE_ETH_PMA, 26, 1, "Eth PMA"},
    {/*118 */ IP_TYPE_ETH_PMA, 26, 2, "Eth PMA"},
    {/*119 */ IP_TYPE_ETH_PMA, 26, 3, "Eth PMA"},
    {/*120 */ IP_TYPE_ETH_PMA, 27, 0, "Eth PMA"},
    {/*121 */ IP_TYPE_ETH_PMA, 27, 1, "Eth PMA"},
    {/*122 */ IP_TYPE_ETH_PMA, 27, 2, "Eth PMA"},
    {/*123 */ IP_TYPE_ETH_PMA, 27, 3, "Eth PMA"},
    {/*124 */ IP_TYPE_ETH_PMA, 28, 0, "Eth PMA"},
    {/*125 */ IP_TYPE_ETH_PMA, 28, 1, "Eth PMA"},
    {/*126 */ IP_TYPE_ETH_PMA, 28, 2, "Eth PMA"},
    {/*127 */ IP_TYPE_ETH_PMA, 28, 3, "Eth PMA"},
    {/*128 */ IP_TYPE_ETH_PMA, 29, 0, "Eth PMA"},
    {/*129 */ IP_TYPE_ETH_PMA, 29, 1, "Eth PMA"},
    {/*130 */ IP_TYPE_ETH_PMA, 29, 2, "Eth PMA"},
    {/*131 */ IP_TYPE_ETH_PMA, 29, 3, "Eth PMA"},
    {/*132 */ IP_TYPE_ETH_PMA, 30, 0, "Eth PMA"},
    {/*133 */ IP_TYPE_ETH_PMA, 30, 1, "Eth PMA"},
    {/*134 */ IP_TYPE_ETH_PMA, 30, 2, "Eth PMA"},
    {/*135 */ IP_TYPE_ETH_PMA, 30, 3, "Eth PMA"},
    {/*136 */ IP_TYPE_ETH_PMA, 31, 0, "Eth PMA"},
    {/*137 */ IP_TYPE_ETH_PMA, 31, 1, "Eth PMA"},
    {/*138 */ IP_TYPE_ETH_PMA, 31, 2, "Eth PMA"},
    {/*139 */ IP_TYPE_ETH_PMA, 31, 3, "Eth PMA"},
    {/*140 */ IP_TYPE_ETH_PMA, 64, 0, "Eth PMA"},
    {/*141 */ IP_TYPE_ETH_PMA, 64, 1, "Eth PMA"},
    {/*142 */ IP_TYPE_ETH_PMA, 64, 2, "Eth PMA"},
    {/*143 */ IP_TYPE_ETH_PMA, 64, 3, "Eth PMA"},
};

port_mgr_sbus_map_t tofino_sbus_ring0_map = {144, tofino_sbus_ring0_node_map};

port_mgr_sbus_node_map_t tofino_sbus_ring1_node_map[] = {
    {/* 00 */ IP_UNPOPULATED, 0, 0, "unpopulated"},

    {/*  1 */ IP_TYPE_ETH_PMA, 32, 0, "Eth PMA"},
    {/*  2 */ IP_TYPE_ETH_PMA, 32, 1, "Eth PMA"},
    {/*  3 */ IP_TYPE_ETH_PMA, 32, 2, "Eth PMA"},
    {/*  4 */ IP_TYPE_ETH_PMA, 32, 3, "Eth PMA"},
    {/*  5 */ IP_TYPE_ETH_PMA, 33, 0, "Eth PMA"},
    {/*  6 */ IP_TYPE_ETH_PMA, 33, 1, "Eth PMA"},
    {/*  7 */ IP_TYPE_ETH_PMA, 33, 2, "Eth PMA"},
    {/*  8 */ IP_TYPE_ETH_PMA, 33, 3, "Eth PMA"},
    {/*  9 */ IP_TYPE_ETH_PMA, 34, 0, "Eth PMA"},
    {/* 10 */ IP_TYPE_ETH_PMA, 34, 1, "Eth PMA"},
    {/* 11 */ IP_TYPE_ETH_PMA, 34, 2, "Eth PMA"},
    {/* 12 */ IP_TYPE_ETH_PMA, 34, 3, "Eth PMA"},
    {/* 13 */ IP_TYPE_ETH_PMA, 35, 0, "Eth PMA"},
    {/* 14 */ IP_TYPE_ETH_PMA, 35, 1, "Eth PMA"},
    {/* 15 */ IP_TYPE_ETH_PMA, 35, 2, "Eth PMA"},
    {/* 16 */ IP_TYPE_ETH_PMA, 35, 3, "Eth PMA"},
    {/* 17 */ IP_TYPE_ETH_PMA, 36, 0, "Eth PMA"},
    {/* 18 */ IP_TYPE_ETH_PMA, 36, 1, "Eth PMA"},
    {/* 19 */ IP_TYPE_ETH_PMA, 36, 2, "Eth PMA"},
    {/* 20 */ IP_TYPE_ETH_PMA, 36, 3, "Eth PMA"},
    {/* 21 */ IP_TYPE_ETH_PMA, 37, 0, "Eth PMA"},
    {/* 22 */ IP_TYPE_ETH_PMA, 37, 1, "Eth PMA"},
    {/* 23 */ IP_TYPE_ETH_PMA, 37, 2, "Eth PMA"},
    {/* 24 */ IP_TYPE_ETH_PMA, 37, 3, "Eth PMA"},
    {/* 25 */ IP_TYPE_ETH_PMA, 38, 0, "Eth PMA"},
    {/* 26 */ IP_TYPE_ETH_PMA, 38, 1, "Eth PMA"},
    {/* 27 */ IP_TYPE_ETH_PMA, 38, 2, "Eth PMA"},
    {/* 28 */ IP_TYPE_ETH_PMA, 38, 3, "Eth PMA"},
    {/* 29 */ IP_TYPE_ETH_PMA, 39, 0, "Eth PMA"},
    {/* 30 */ IP_TYPE_ETH_PMA, 39, 1, "Eth PMA"},
    {/* 31 */ IP_TYPE_ETH_PMA, 39, 2, "Eth PMA"},
    {/* 32 */ IP_TYPE_ETH_PMA, 39, 3, "Eth PMA"},

    {/* 33 */ IP_TYPE_TEMP_SENSOR, 0, 0, "Temp Snsr"},

    {/* 34 */ IP_TYPE_ETH_PMA, 40, 0, "Eth PMA"},
    {/* 35 */ IP_TYPE_ETH_PMA, 40, 1, "Eth PMA"},
    {/* 36 */ IP_TYPE_ETH_PMA, 40, 2, "Eth PMA"},
    {/* 37 */ IP_TYPE_ETH_PMA, 40, 3, "Eth PMA"},
    {/* 38 */ IP_TYPE_ETH_PMA, 41, 0, "Eth PMA"},
    {/* 39 */ IP_TYPE_ETH_PMA, 41, 1, "Eth PMA"},
    {/* 40 */ IP_TYPE_ETH_PMA, 41, 2, "Eth PMA"},
    {/* 41 */ IP_TYPE_ETH_PMA, 41, 3, "Eth PMA"},
    {/* 42 */ IP_TYPE_ETH_PMA, 42, 0, "Eth PMA"},
    {/* 43 */ IP_TYPE_ETH_PMA, 42, 1, "Eth PMA"},
    {/* 44 */ IP_TYPE_ETH_PMA, 42, 2, "Eth PMA"},
    {/* 45 */ IP_TYPE_ETH_PMA, 42, 3, "Eth PMA"},
    {/* 46 */ IP_TYPE_ETH_PMA, 43, 0, "Eth PMA"},
    {/* 47 */ IP_TYPE_ETH_PMA, 43, 1, "Eth PMA"},
    {/* 48 */ IP_TYPE_ETH_PMA, 43, 2, "Eth PMA"},
    {/* 49 */ IP_TYPE_ETH_PMA, 43, 3, "Eth PMA"},
    {/* 50 */ IP_TYPE_ETH_PMA, 44, 0, "Eth PMA"},
    {/* 51 */ IP_TYPE_ETH_PMA, 44, 1, "Eth PMA"},
    {/* 52 */ IP_TYPE_ETH_PMA, 44, 2, "Eth PMA"},
    {/* 53 */ IP_TYPE_ETH_PMA, 44, 3, "Eth PMA"},
    {/* 54 */ IP_TYPE_ETH_PMA, 45, 0, "Eth PMA"},
    {/* 55 */ IP_TYPE_ETH_PMA, 45, 1, "Eth PMA"},
    {/* 56 */ IP_TYPE_ETH_PMA, 45, 2, "Eth PMA"},
    {/* 57 */ IP_TYPE_ETH_PMA, 45, 3, "Eth PMA"},
    {/* 58 */ IP_TYPE_ETH_PMA, 46, 0, "Eth PMA"},
    {/* 59 */ IP_TYPE_ETH_PMA, 46, 1, "Eth PMA"},
    {/* 60 */ IP_TYPE_ETH_PMA, 46, 2, "Eth PMA"},
    {/* 61 */ IP_TYPE_ETH_PMA, 46, 3, "Eth PMA"},
    {/* 62 */ IP_TYPE_ETH_PMA, 47, 0, "Eth PMA"},
    {/* 63 */ IP_TYPE_ETH_PMA, 47, 1, "Eth PMA"},
    {/* 64 */ IP_TYPE_ETH_PMA, 47, 2, "Eth PMA"},
    {/* 65 */ IP_TYPE_ETH_PMA, 47, 3, "Eth PMA"},
    {/* 66 */ IP_TYPE_ETH_PMA, 48, 0, "Eth PMA"},
    {/* 67 */ IP_TYPE_ETH_PMA, 48, 1, "Eth PMA"},
    {/* 68 */ IP_TYPE_ETH_PMA, 48, 2, "Eth PMA"},
    {/* 69 */ IP_TYPE_ETH_PMA, 48, 3, "Eth PMA"},
    {/* 70 */ IP_TYPE_ETH_PMA, 49, 0, "Eth PMA"},
    {/* 71 */ IP_TYPE_ETH_PMA, 49, 1, "Eth PMA"},
    {/* 72 */ IP_TYPE_ETH_PMA, 49, 2, "Eth PMA"},
    {/* 73 */ IP_TYPE_ETH_PMA, 49, 3, "Eth PMA"},
    {/* 74 */ IP_TYPE_ETH_PMA, 50, 0, "Eth PMA"},
    {/* 75 */ IP_TYPE_ETH_PMA, 50, 1, "Eth PMA"},
    {/* 76 */ IP_TYPE_ETH_PMA, 50, 2, "Eth PMA"},
    {/* 77 */ IP_TYPE_ETH_PMA, 50, 3, "Eth PMA"},
    {/* 78 */ IP_TYPE_ETH_PMA, 51, 0, "Eth PMA"},
    {/* 79 */ IP_TYPE_ETH_PMA, 51, 1, "Eth PMA"},
    {/* 80 */ IP_TYPE_ETH_PMA, 51, 2, "Eth PMA"},
    {/* 81 */ IP_TYPE_ETH_PMA, 51, 3, "Eth PMA"},
    {/* 82 */ IP_TYPE_ETH_PMA, 52, 0, "Eth PMA"},
    {/* 83 */ IP_TYPE_ETH_PMA, 52, 1, "Eth PMA"},
    {/* 84 */ IP_TYPE_ETH_PMA, 52, 2, "Eth PMA"},
    {/* 85 */ IP_TYPE_ETH_PMA, 52, 3, "Eth PMA"},
    {/* 86 */ IP_TYPE_ETH_PMA, 53, 0, "Eth PMA"},
    {/* 87 */ IP_TYPE_ETH_PMA, 53, 1, "Eth PMA"},
    {/* 88 */ IP_TYPE_ETH_PMA, 53, 2, "Eth PMA"},
    {/* 89 */ IP_TYPE_ETH_PMA, 53, 3, "Eth PMA"},
    {/* 90 */ IP_TYPE_ETH_PMA, 54, 0, "Eth PMA"},
    {/* 91 */ IP_TYPE_ETH_PMA, 54, 1, "Eth PMA"},
    {/* 92 */ IP_TYPE_ETH_PMA, 54, 2, "Eth PMA"},
    {/* 93 */ IP_TYPE_ETH_PMA, 54, 3, "Eth PMA"},
    {/* 94 */ IP_TYPE_ETH_PMA, 55, 0, "Eth PMA"},
    {/* 95 */ IP_TYPE_ETH_PMA, 55, 1, "Eth PMA"},
    {/* 96 */ IP_TYPE_ETH_PMA, 55, 2, "Eth PMA"},
    {/* 97 */ IP_TYPE_ETH_PMA, 55, 3, "Eth PMA"},
    {/* 98 */ IP_TYPE_ETH_PMA, 56, 0, "Eth PMA"},
    {/* 99 */ IP_TYPE_ETH_PMA, 56, 1, "Eth PMA"},
    {/*100 */ IP_TYPE_ETH_PMA, 56, 2, "Eth PMA"},
    {/*101 */ IP_TYPE_ETH_PMA, 56, 3, "Eth PMA"},
    {/*102 */ IP_TYPE_ETH_PMA, 57, 0, "Eth PMA"},
    {/*103 */ IP_TYPE_ETH_PMA, 57, 1, "Eth PMA"},
    {/*104 */ IP_TYPE_ETH_PMA, 57, 2, "Eth PMA"},
    {/*105 */ IP_TYPE_ETH_PMA, 57, 3, "Eth PMA"},
    {/*106 */ IP_TYPE_ETH_PMA, 58, 0, "Eth PMA"},
    {/*107 */ IP_TYPE_ETH_PMA, 58, 1, "Eth PMA"},
    {/*108 */ IP_TYPE_ETH_PMA, 58, 2, "Eth PMA"},
    {/*109 */ IP_TYPE_ETH_PMA, 58, 3, "Eth PMA"},
    {/*110 */ IP_TYPE_ETH_PMA, 59, 0, "Eth PMA"},
    {/*111 */ IP_TYPE_ETH_PMA, 59, 1, "Eth PMA"},
    {/*112 */ IP_TYPE_ETH_PMA, 59, 2, "Eth PMA"},
    {/*113 */ IP_TYPE_ETH_PMA, 59, 3, "Eth PMA"},
    {/*114 */ IP_TYPE_ETH_PMA, 60, 0, "Eth PMA"},
    {/*115 */ IP_TYPE_ETH_PMA, 60, 1, "Eth PMA"},
    {/*116 */ IP_TYPE_ETH_PMA, 60, 2, "Eth PMA"},
    {/*117 */ IP_TYPE_ETH_PMA, 60, 3, "Eth PMA"},
    {/*118 */ IP_TYPE_ETH_PMA, 61, 0, "Eth PMA"},
    {/*119 */ IP_TYPE_ETH_PMA, 61, 1, "Eth PMA"},
    {/*120 */ IP_TYPE_ETH_PMA, 61, 2, "Eth PMA"},
    {/*121 */ IP_TYPE_ETH_PMA, 61, 3, "Eth PMA"},
    {/*122 */ IP_TYPE_ETH_PMA, 62, 0, "Eth PMA"},
    {/*123 */ IP_TYPE_ETH_PMA, 62, 1, "Eth PMA"},
    {/*124 */ IP_TYPE_ETH_PMA, 62, 2, "Eth PMA"},
    {/*125 */ IP_TYPE_ETH_PMA, 62, 3, "Eth PMA"},
    {/*126 */ IP_TYPE_ETH_PMA, 63, 0, "Eth PMA"},
    {/*127 */ IP_TYPE_ETH_PMA, 63, 1, "Eth PMA"},
    {/*128 */ IP_TYPE_ETH_PMA, 63, 2, "Eth PMA"},
    {/*129 */ IP_TYPE_ETH_PMA, 63, 3, "Eth PMA"},
};

port_mgr_sbus_map_t tofino_sbus_ring1_map = {130, tofino_sbus_ring1_node_map};

port_mgr_sbus_ring_map_t tofino_sbus_ring_map = {
    2, {&tofino_sbus_ring0_map, &tofino_sbus_ring1_map}};

port_mgr_sbus_node_map_t trestles_sbus_ring0_node_map[] = {
    {/* 00 */ IP_UNPOPULATED, 0, 0, "unpopulated"},
};

port_mgr_sbus_map_t trestles_sbus_ring0_map = {1, trestles_sbus_ring0_node_map};

port_mgr_sbus_node_map_t trestles_sbus_ring1_node_map[] = {
    {/* 00 */ IP_UNPOPULATED, 0, 0, "unpopulated"},
};

port_mgr_sbus_map_t trestles_sbus_ring1_map = {1, trestles_sbus_ring1_node_map};

port_mgr_sbus_ring_map_t trestles_sbus_ring_map = {
    2, {&trestles_sbus_ring0_map, &trestles_sbus_ring1_map}};

/*****************************************************************************
 *
 *****************************************************************************/
int port_mgr_dump_sbus_adrr_map(ucli_context_t *uc, bf_dev_id_t dev_id) {
  bf_chip_family_e chip_family = bf_chip_tofino;
  port_mgr_sbus_ring_map_t *ring_map;
  int r, n;

  // get chip type from sku...when necessary
  // sku = lld_sku_get_sku(dev_id);

  if (chip_family == bf_chip_tofino) {
    ring_map = &tofino_sbus_ring_map;
  } else if (chip_family == bf_chip_trestles) {
    ring_map = &trestles_sbus_ring_map;
  } else {
    aim_printf(
        &uc->pvs, "Error identifying chip family for dev_id=%d\n", dev_id);
    return -3;
  }

  for (r = 0; r < ring_map->num_rings; r++) {
    port_mgr_sbus_map_t *sbus_map = ring_map->sbus_map[r];
    port_mgr_sbus_node_map_t *node_map = sbus_map->node_map;
    int num_nodes = sbus_map->num_nodes;

    for (n = 0; n < num_nodes; n++) {
      aim_printf(
          &uc->pvs,
          " %d : %d : %d : ip type=%-10s : %d : %d : %s\n",
          dev_id,
          r,
          n,
          node_map->ip_type == IP_UNPOPULATED
              ? "na"
              : node_map->ip_type == IP_TYPE_PCIE_PLL
                    ? "Pcie PLL"
                    : node_map->ip_type == IP_TYPE_CORE_PLL
                          ? "Core PLL"
                          : node_map->ip_type == IP_TYPE_MAC_PLL
                                ? "Mac PLL"
                                : node_map->ip_type == IP_TYPE_PCIE_PMA
                                      ? "Pcie Pma"
                                      : node_map->ip_type == IP_TYPE_PCIE_PCS
                                            ? "Pcie Pcs"
                                            : node_map->ip_type ==
                                                      IP_TYPE_ETH_PMA
                                                  ? "Eth Pma"
                                                  : node_map->ip_type ==
                                                            IP_TYPE_TEMP_SENSOR
                                                        ? "Temp Snsr"
                                                        : "??",
          node_map->inst,
          node_map->sub_inst,
          node_map->desc);
      node_map++;
    }
  }
  return 0;
}

int port_mgr_find_addr_for(bf_dev_id_t dev_id,
                           port_mgr_sbus_ip_type_e ip_type,
                           int inst,
                           int sub_inst,
                           int *ring,
                           int *sd) {
  bf_chip_family_e chip_family = bf_chip_tofino;
  port_mgr_sbus_ring_map_t *ring_map;
  int r, n;

  if (ring) *ring = -1;
  if (sd) *sd = -1;

  // get chip type from sku...when necessary
  // sku = lld_sku_get_sku(dev_id);

  if (chip_family == bf_chip_tofino) {
    ring_map = &tofino_sbus_ring_map;
  } else if (chip_family == bf_chip_trestles) {
    ring_map = &trestles_sbus_ring_map;
  } else {
    port_mgr_log("Error identifying chip family for dev_id=%d", dev_id);
    return -3;
  }

  for (r = 0; r < ring_map->num_rings; r++) {
    port_mgr_sbus_map_t *sbus_map = ring_map->sbus_map[r];
    port_mgr_sbus_node_map_t *node_map = sbus_map->node_map;
    int num_nodes = sbus_map->num_nodes;

    for (n = 0; n < num_nodes; n++) {
      if ((node_map->ip_type == ip_type) &&
          ((node_map->inst == inst) && (node_map->sub_inst == sub_inst))) {
        if (ring) *ring = r;
        if (sd) *sd = n;
        return 0;
      }
      node_map++;
    }
  }
  return -1;  // not found
}

/*********************************************************************
 * Given a dev_id, ring, and sd, return the IP info we have about it
 * (mac-block, mac-channel, ip-type
 *
 * note: only expected to be called for serdes nodes.
 */
int port_mgr_find_mac_info_for(bf_dev_id_t dev_id,
                               int ring,
                               int sd,
                               port_mgr_sbus_ip_type_e *ip_type,
                               int *inst,
                               int *sub_inst) {
  port_mgr_sbus_ring_map_t *ring_map;
  port_mgr_sbus_map_t *sbus_map;
  port_mgr_sbus_node_map_t *node_map;

  (void)dev_id;
  ring_map = &tofino_sbus_ring_map;
  sbus_map = ring_map->sbus_map[ring];
  node_map = &sbus_map->node_map[sd];

  if (ip_type) *ip_type = node_map->ip_type;
  if (inst) *inst = node_map->inst;
  if (sub_inst) *sub_inst = node_map->sub_inst;

  return 0;
}

/*********************************************************************
 * Given a dev_id, ring, and an ip type, return the list of nodes
 * matching the ip type.
 */
void port_mgr_get_nodes_of_type(bf_dev_id_t dev_id,
                                int ring,
                                port_mgr_sbus_ip_type_e ip_type,
                                int *ip_nodes,
                                int *n_nodes) {
  port_mgr_sbus_ring_map_t *ring_map = &tofino_sbus_ring_map;
  (void)dev_id;

  if ((uint)ring >=
      sizeof(ring_map->sbus_map) / sizeof(ring_map->sbus_map[0])) {
    port_mgr_log_warn("Unexpected value: ring = %d", ring);
    return;
  }

  port_mgr_sbus_map_t *sbus_map = ring_map->sbus_map[ring];
  port_mgr_sbus_node_map_t *node_map = sbus_map->node_map;
  int n, num_nodes_rtnd = 0, num_nodes_in_map = sbus_map->num_nodes;

  for (n = 0; n < num_nodes_in_map; n++) {
    if (node_map->ip_type == ip_type) {
      ip_nodes[num_nodes_rtnd] = n;
      num_nodes_rtnd++;
    }
    node_map++;
  }
  *n_nodes = num_nodes_rtnd;
  return;
}

/************************************************************************
 * port_mgr_configure_sbus_map
 *
 * Initializes the port_mgr_serdes_t fields,
 *        ring
 *        tx_sd
 *        rx_sd
 *
 * These are just default values, assuming no swapping due to board
 * layout. They must later be corrected via bf_port_lane_map_set() to
 * account for any "swizzling".
 ************************************************************************/
void port_mgr_configure_sbus_map(bf_dev_id_t dev_id) {
  uint32_t pipe, port;
  port_mgr_mac_block_t *mac_block_p;
  port_mgr_serdes_t *serdes_p;
  uint32_t num_subdev = 0;

  lld_sku_get_num_subdev(dev_id, &num_subdev, NULL);
  // determine sbus map associated with chip type
  // then fill in all serdes addresses
  for (pipe = 0; pipe < (BF_SUBDEV_PIPE_COUNT * num_subdev); pipe++) {
    for (port = (uint32_t)lld_get_min_fp_port(dev_id);
         port <= (uint32_t)lld_get_max_fp_port(dev_id);
         port++) {
      bf_dev_port_t dev_port = MAKE_DEV_PORT(pipe, port);

      serdes_p = port_mgr_tof1_map_port_lane_to_serdes_allow_unassigned(
          dev_id, dev_port, 0);
      if (serdes_p != NULL) {
        int mac_block, ch, ring, sd;
        port_mgr_err_t err;

        // mac_block=inst, ch=sub_inst in the maps
        err = port_mgr_map_dev_port_to_all(
            dev_id, dev_port, NULL, NULL, &mac_block, &ch, NULL);
        if (err != 0) {
          port_mgr_log(
              "Error (%d) converting dev_port (%x) to mac/ch\n", err, dev_port);
          return;
        }

        // look up sbus addr
        port_mgr_find_addr_for(
            dev_id, IP_TYPE_ETH_PMA, mac_block, ch, &ring, &sd);

        // set connected serdes node address (determined by chip design)
        // not board layout. This is to set up the node addresses of the
        // four serdes associated with this mac block. The actual mapping
        // will be done later by remapping these addresses to different
        // mac channels based on the board map
        mac_block_p = port_mgr_tof1_map_idx_to_mac_block(dev_id, mac_block);
        if (!mac_block_p) {
          port_mgr_log("Error on finding mac block for dev id %x\n", dev_id);
          return;
        }
        mac_block_p->sds_node[ch] = sd;

        serdes_p->ring = ring;
        serdes_p->tx_sd = sd;
        serdes_p->rx_sd = sd;
      }
    }
  }
  // and then the cpu port ..
  pipe = 0;
  for (port = (uint32_t)lld_get_min_cpu_port(dev_id);
       port <= (uint32_t)lld_get_max_cpu_port(dev_id);
       port++) {
    bf_dev_port_t dev_port = MAKE_DEV_PORT(pipe, port);

    serdes_p = port_mgr_tof1_map_port_lane_to_serdes_allow_unassigned(
        dev_id, dev_port, 0);
    if (serdes_p != NULL) {
      int mac_block, ch, ring, sd;
      port_mgr_err_t err;

      // mac_block=inst, ch=sub_inst in the maps
      err = port_mgr_map_dev_port_to_all(
          dev_id, dev_port, NULL, NULL, &mac_block, &ch, NULL);
      if (err != 0) {
        port_mgr_log(
            "Error (%d) converting dev_port (%x) to mac/ch\n", err, dev_port);
        return;
      }
      // look up sbus addr
      port_mgr_find_addr_for(
          dev_id, IP_TYPE_ETH_PMA, mac_block, ch, &ring, &sd);

      // set connected serdes node address (determined by chip design)
      // not board layout. This is to set up the node addresses of the
      // four serdes associated with this mac block. The actual mapping
      // will be done later by remapping these addresses to different
      // mac channels based on the board map
      mac_block_p = port_mgr_tof1_map_idx_to_mac_block(dev_id, mac_block);
      if (!mac_block_p) {
        port_mgr_log("Error on finding mac block for dev id %x\n", dev_id);
        return;
      }

      mac_block_p->sds_node[ch] = sd;

      serdes_p->ring = ring;
      serdes_p->tx_sd = sd;
      serdes_p->rx_sd = sd;
    }
  }
  return;
}

/*************************************************************
 * port_mgr_num_sbus_rings_get
 *
 *************************************************************/
int port_mgr_num_sbus_rings_get(bf_dev_id_t dev_id) {
  (void)dev_id;
#ifdef AVAGO_EVAL_BOARD
  return 1;
#else
  return tofino_sbus_ring_map.num_rings;  // 2 rings on tofino
#endif
}

/*************************************************************
 * port_mgr_num_sbus_nodes_get
 *
 *************************************************************/
int port_mgr_num_sbus_nodes_get(bf_dev_id_t dev_id, int ring) {
#ifdef AVAGO_EVAL_BOARD
  (void)dev_id;
  (void)ring;
  return 22;
#else
  (void)dev_id;
  return tofino_sbus_ring_map.sbus_map[ring]->num_nodes;
#endif
}

/*************************************************************
 * port_mgr_is_valid_sbus_address
 *
 *************************************************************/
bool port_mgr_is_valid_sbus_address(bf_dev_id_t dev_id, int ring, int sd) {
  if (ring < port_mgr_num_sbus_rings_get(dev_id)) {
    if (sd < port_mgr_num_sbus_nodes_get(dev_id, ring)) {
      return true;
    }
  }
  return false;
}

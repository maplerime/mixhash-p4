/**
 * Example Driver header file for interfacing with the Alphawave C APIs
 */

#ifndef __aw_driver_sim
#define __aw_driver_sim

#include <stdint.h>
#include <stdio.h>
#include "svdpi.h"
#include "aw_mss.h"
//#include "aw_alphacore_csr_defines.h"

// NOTE:
//   * To access registers in different lanes
//     please use the lane offsets below.
//   * LANE_BROADCAST is write only and does not
//     work for reading registers.
// -------------------------------------
// LANE OFFSETS
#define CMN 0x00000000
#define LANE_OFFSET 0x02000000
#define LANE_BROADCAST 0x40000000

// this should ideally be a vector API
//#define LANE_MAX 15

#define LANE0_OFFSET 0x02000000 // this can be derived from aw_alphacore_csr_defines.h

#define USR_SLEEP(x) \
    delay_us(x);

extern int suppress_aw_prints;
void suppress_aw_prints_set(int val);
uint32_t suppress_aw_prints_get(void);

#define USR_PRINTF(...) \
  if (!suppress_aw_prints) { \
    printf(__VA_ARGS__); \
  } \

typedef enum aw_rd_opcode_e {
    RD_EQ    = 0, //Read equal check
    RD_GT    = 1, //Read greater than check
    RD_GTE   = 2, //Read greater than or equal check
    RD_LT    = 3, //Read less than check
    RD_LTE   = 4, //Read less than or equal check
    RD_RANGE = 5  //Read range check
} aw_rd_opcode_t;

int serdes_init(mss_access_t *mss, uint32_t lane_offset, uint32_t phy_offset);
void mss_reset_evb(mss_access_t *mss);
void write_csr(uint32_t addr, uint32_t wdata);
void read_csr(uint32_t addr, uint32_t *rdata);
int delay_us(int x);
int c_test_api_write(void);
int c_test_api_read(void);
int pmd_set_lane(mss_access_t *mss, uint32_t lane);
int pmd_write_addr(mss_access_t *mss, uint32_t addr, uint32_t value);
int pmd_read_addr(mss_access_t *mss, uint32_t addr, uint32_t *rdval);
int pmd_write_field(mss_access_t *mss, uint32_t addr, uint32_t fld_mask, uint32_t fld_offset, uint32_t wval);
int pmd_read_field(mss_access_t *mss, uint32_t addr, uint32_t fld_mask, uint32_t fld_offset, uint32_t *rdval);
int pmd_read_check_field(mss_access_t *mss, uint32_t addr, uint32_t fld_mask, uint32_t fld_offset, aw_rd_opcode_t rd_opcode, uint32_t *rdval, uint32_t rdcheck_val1, uint32_t rdcheck_val2);
int pmd_poll_field(mss_access_t *mss, uint32_t addr, uint32_t fld_mask, uint32_t fld_offset, uint32_t poll_val, uint32_t timeout_us);
int pmd_ate_vec_comment(char comment[]);

#endif // __aw_driver_sim

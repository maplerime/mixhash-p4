/**
 * Example Driver header file for interfacing with the Alphawave C APIs
 */

#include <stdint.h>
#include <stdio.h>
#include "svdpi.h"
//#include "aw_alphacore_csr_defines.h"
#include "aw_driver_sim.h"

// This is the max() of any AW IP. It ideally would be an API
#define LANE_MAX 15

int dbg_mode(void) {return 0;}
int suppress_aw_prints = 0;

void suppress_aw_prints_set(int val) {suppress_aw_prints = val;}
uint32_t suppress_aw_prints_get() {return suppress_aw_prints;}

/**
 * lane_offset_set
 *
 * Tx and Rx lanes can be independently mapped on the board (swizzled). So we
 * need to track different lane_offsets for TX and RX registers. This is not
 * trivial. There are 4 blocks in the IP, TX, RX, ANLT, DFX (in additioon to CMN)
 * For addresses in the TX and RX blocks the mapping is clear. However, for the
 * DFX and ANLT blocks the registers will have to be checked one-by-one to
 * determines whether they affect the TX or RX path and set the lane offset for
 * the appropriate physical lane used for that path. 
 */
void lane_offset_set(mss_access_t *mss, uint32_t addr) {
  uint32_t blk = (addr >> 12) & 0x3;
  uint32_t is_sram = ((addr >> 28) == 0xa) ||
	             ((addr >> 28) == 0x8);

  if (is_sram) {
    mss->lane_offset = 0;
    return;
  }

  if (addr < LANE0_OFFSET) { // CMN
    mss->lane_offset = 0;
    return;
  }
  switch (blk) {
  case 0: // RX
    mss->lane_offset = mss->rx_lane_offset;
    return;
  case 1: // TX
    mss->lane_offset = mss->tx_lane_offset;
    return;
  case 2: // ANLT
    //TBD: Add special-case checks here
    //default to Tx side for AN
    mss->lane_offset = mss->tx_lane_offset;
    break;
  case 3: // DFX
    mss->lane_offset = mss->derived_from_name_offset;
    //TBD: Add special-case checks here
    return;
  default:
    break;
  }
  return;
}

/**
 * A general init function for the mss_access struct. This is just a placehlder
 * as it should have more checking, report an error if any of the values are
 * invalid, etc.
 * 
 */
int serdes_init(mss_access_t *mss, uint32_t lane_offset, uint32_t phy_offset) {
    mss->lane_offset = lane_offset;
    mss->phy_offset = phy_offset;
    return 0;
}

/**
 * Wrap misc SV behaviour away from the core DPI function calls
 * 
 */
void mss_reset_evb(mss_access_t *mss) {
    sv_mss_reset(mss->phy_offset);
}

int delay_us(int x) {

    sv_delay_us(x);

    return 0;
}

void io_evb_write_csr(uint32_t dev_id, uint32_t subdev_id, uint32_t addr, uint32_t wdata, uint32_t phy_offset_used) {
  sv_write_csr(addr, wdata);
}

void io_evb_read_csr(uint32_t dev_id, uint32_t subdev_id, uint32_t addr, uint32_t *rdata, uint32_t phy_offset_used) {
  sv_read_csr(addr, rdata);
}


/**
 * General Notes:
 *   - for the core driver functions below, if any addr is less than the lane0
 *     offset, it is cmn lane and we do not apply lane offset in the mss_access
 *     struct
 *
 */

int pmd_set_lane(mss_access_t *mss, uint32_t lane){
    // if broadcast bit is set, lane bits are dont care
    if (lane == 99) {
        mss->lane_offset = LANE_BROADCAST;
        return 0;
    } else if (lane > LANE_MAX) { 
        printf("[pmd_set_lane]: Lane number %d does not exist in the design\n", lane);
        return 1;
    } else {
        mss->lane_offset = (lane) * LANE0_OFFSET;
        return 0;
    }
}

int pmd_write_addr(mss_access_t *mss, uint32_t addr, uint32_t value){
    uint32_t final_addr;

    //bfn
    lane_offset_set(mss, addr);

    if (addr < LANE0_OFFSET ||  addr > LANE_BROADCAST) {
        final_addr = addr + mss->phy_offset;
    } else {
        final_addr = addr + mss->lane_offset + mss->phy_offset;
    }
    mss->io.write_csr(mss->dev_id, mss->subdev_id, final_addr, value, mss->phy_offset);


    return 0;
}

int pmd_read_addr(mss_access_t *mss, uint32_t addr, uint32_t *rdval){
    uint32_t final_addr;

    //bfn
    lane_offset_set(mss, addr);

    if (addr < LANE0_OFFSET) {
        final_addr = addr + mss->phy_offset;
    } else {
        final_addr = addr + mss->lane_offset + mss->phy_offset;
    }
    if (0 && final_addr >= LANE_BROADCAST) {
        printf("[pmd_read_addr]: Cannot read register while mss.lane_offset has lane broadcast set.\n");
        return 1;
    }
    mss->io.read_csr(mss->dev_id, mss->subdev_id, final_addr, rdval, mss->phy_offset);
    return 0;
}

int pmd_write_field(mss_access_t *mss, uint32_t addr, uint32_t fld_mask, uint32_t fld_offset, uint32_t wval){
    uint32_t final_addr;

    //bfn
    lane_offset_set(mss, addr);

    if (addr < LANE0_OFFSET) {
        final_addr = addr + mss->phy_offset;
    } else if (mss->lane_offset == LANE_BROADCAST) {
        final_addr = addr + mss->lane_offset - LANE0_OFFSET + mss->phy_offset;
    } else {
        final_addr = addr + mss->lane_offset + mss->phy_offset;
    }

    uint32_t reg_read;
    wval = wval << fld_offset;
    mss->io.read_csr(mss->dev_id, mss->subdev_id, final_addr, &reg_read, mss->phy_offset);

    wval = (wval & fld_mask) | (reg_read & ~fld_mask);
    mss->io.write_csr(mss->dev_id, mss->subdev_id, final_addr, wval, mss->phy_offset);

    return 0;
}

int pmd_read_field(mss_access_t *mss, uint32_t addr, uint32_t fld_mask, uint32_t fld_offset, uint32_t *rdval){
    uint32_t final_addr;

    //bfn
    lane_offset_set(mss, addr);

    if (addr < LANE0_OFFSET) {
        final_addr = addr + mss->phy_offset;
    }
    else {
        final_addr = addr + mss->lane_offset + mss->phy_offset;
    }
    if (final_addr >= LANE_BROADCAST) {
        printf("[pmd_read_check_field]: Cannot read register while mss.lane_offset has lane broadcast set.\n");
        return 1;
    }

    uint32_t rddata;
    mss->io.read_csr(mss->dev_id, mss->subdev_id, final_addr, &rddata, mss->phy_offset);
    uint32_t fld_val = ((rddata & fld_mask) >> fld_offset);
    *rdval = fld_val;

    return 0;
}

int pmd_read_check_field(mss_access_t *mss, uint32_t addr, uint32_t fld_mask, uint32_t fld_offset, aw_rd_opcode_t rd_opcode, uint32_t *rdval, uint32_t rdcheck_val1, uint32_t rdcheck_val2){
    uint32_t final_addr;

    //bfn
    lane_offset_set(mss, addr);

    if (addr < LANE0_OFFSET) { // common lane
        final_addr = addr + mss->phy_offset;
    } 
    else {
        final_addr = addr + mss->lane_offset + mss->phy_offset;
    }
    if (final_addr >= LANE_BROADCAST) {
        printf("[pmd_read_check_field]: Cannot read register while mss.lane_offset has lane broadcast set.\n");
        return 1;
    }

    uint32_t rddata;
    mss->io.read_csr(mss->dev_id, mss->subdev_id, final_addr, &rddata, mss->phy_offset);
    uint32_t fld_val = ((rddata & fld_mask) >> fld_offset);
    *rdval = fld_val;



    if (rd_opcode == RD_EQ) {
        if (fld_val == rdcheck_val1) {
            if (dbg_mode())
              printf("[pmd_read_check_field]: Register check passed. Expected value = 0x%X. Field value = 0x%X\n", rdcheck_val1, fld_val);
            return 0;
        } else {
            //printf("[pmd_read_check_field]: ERROR(1). Register check failed. Expected value = 0x%X. Field value = 0x%X\n", rdcheck_val1, fld_val);
            return -1;
        }
    } else if (rd_opcode == RD_GT) {
        if (fld_val > rdcheck_val1) {
            if (dbg_mode())
              printf("[pmd_read_check_field]: Register check passed. Expected value > 0x%X. Field value = 0x%X\n", rdcheck_val1, fld_val);
            return 0;
        } else {
            printf("[pmd_read_check_field]: ERROR(2). Register check failed. Expected > 0x%X. Field value = 0x%X\n", rdcheck_val1, fld_val);
            return -1;
        }
    } else if (rd_opcode == RD_GTE) {
        if (fld_val >= rdcheck_val1) {
            if (dbg_mode())
              printf("[pmd_read_check_field]: Register check passed. Expected value >= 0x%X. Field value = 0x%X\n", rdcheck_val1, fld_val);
            return 0;
        } else {
            printf("[pmd_read_check_field]: ERROR(3). Register check failed. Expected value >= 0x%X. Field value = 0x%X\n", rdcheck_val1, fld_val);
            return -1;
        }
    } else if (rd_opcode == RD_LT) {
        if (fld_val < rdcheck_val1) {
            if (dbg_mode())
              printf("[pmd_read_check_field]: Register check passed. Expected value < 0x%X. Field value = 0x%X\n", rdcheck_val1, fld_val);
            return 0;
        } else {
            printf("[pmd_read_check_field]: ERROR(4). Register check failed. Expected < 0x%X. Field value = 0x%X\n", rdcheck_val1, fld_val);
            return -1;
        }
    } else if (rd_opcode == RD_LTE) {
        if (fld_val <= rdcheck_val1) {
            if (dbg_mode())
              printf("[pmd_read_check_field]: Register check passed. Expected value <= 0x%X. Field value = 0x%X\n", rdcheck_val1, fld_val);
            return 0;
        } else {
            printf("[pmd_read_check_field]: ERROR(5). Register check failed. Expected <= 0x%X. Field value = 0x%X\n", rdcheck_val1, fld_val);
            return -1;
        }
    } else if (rd_opcode == RD_RANGE) {
        if (fld_val >= rdcheck_val1 && fld_val <= rdcheck_val2) {
            if (dbg_mode())
              printf("[pmd_read_check_field]: Register check passed. Expected range = 0x%X -> 0x%X. Field value = 0x%X\n", rdcheck_val1, rdcheck_val2, fld_val);
            return 0;
        } else {
            printf("[pmd_read_check_field]: ERROR(6). Register check failed. Expected range = 0x%X -> 0x%X. Field value = 0x%X\n", rdcheck_val1, rdcheck_val2, fld_val);
            return -1;
        }

    } else {
        printf("[pmd_read_check_field]: ERROR. Invalid read opcode");
        return -1;
    }

}


//Use this in simulations
int pmd_poll_field(mss_access_t *mss, uint32_t addr, uint32_t fld_mask, uint32_t fld_offset, uint32_t poll_val, uint32_t timeout_us){
    uint32_t final_addr;

    //bfn
    lane_offset_set(mss, addr);

    if (addr < LANE0_OFFSET) {
        final_addr = addr + mss->phy_offset;
    } else {
        final_addr = addr + mss->lane_offset + mss->phy_offset;
    }
    if (final_addr >= LANE_BROADCAST) {
        printf("[pmd_read_check_field]: Cannot read register while mss.lane_offset has lane broadcast set.\n");
        return 1;
    }

    uint32_t rddata = 0;
    uint32_t fld_val = 0;
    uint32_t i = 0;
    while (i < timeout_us) {
        i++;
        USR_SLEEP(1);
        mss->io.read_csr(mss->dev_id, mss->subdev_id, final_addr, &rddata, mss->phy_offset);
        //printf("[pmd_poll_field]: Read value: 0x%08X\n", rddata);
        fld_val = ((rddata & fld_mask) >> fld_offset);
        //printf("[pmd_poll_field]: Field value: 0x%X\n", fld_val);
        if (fld_val == poll_val) {
            break;
        }
    }
    if (fld_val == poll_val) {
        //printf("[pmd_poll_field]: Polling successful after %d us\n", i);
        return 0;
    } else {
        USR_PRINTF("[pmd_poll_field]: Polling timed out after %d us\n", timeout_us);
        return -1;
    }
}

//Use this to add comments to ATE vector files
int pmd_ate_vec_comment(char comment[]) {
    printf("[pmd_ate_vec_comment]: %s\n", comment);
    return 0;
}

#include <stdint.h>
#include <stdio.h>
#include "svdpi.h"

extern void evb_mss_reset(uint32_t macro_base_addr);
extern void evb_write_csr(uint32_t addr, uint32_t data);
extern void evb_read_csr(uint32_t addr, uint32_t *data);
#define USR_PRINTF(...) printf(__VA_ARGS__)
void bf_sys_usleep(uint32_t);


/*
* The EVB contains a chip that is different from eith the 4ln or 16ln
* IP we will use on CB. The interface to the EVB uses a "compressed"
* form of the CSR/SRAM address to accomodate a 16b APB bus on-chip.
*/

uint32_t svdpi_convert_32to16_addr(uint32_t addr_32);

// SV
void sv_mss_reset(uint32_t macro_base_addr) {
  evb_mss_reset(macro_base_addr);
}

void sv_write_csr(uint32_t addr, uint32_t wdata) {
  uint32_t evb_addr = svdpi_convert_32to16_addr(addr);
  evb_write_csr(evb_addr, wdata);
}

void sv_read_csr(uint32_t addr, uint32_t *rdata) {
  uint32_t evb_addr = svdpi_convert_32to16_addr(addr);
  evb_read_csr(evb_addr, rdata);
}

void sv_delay_us(int delay_us) {
  bf_sys_usleep(delay_us);
}

/*
    def convert_32to16_addr(self, addr_32):
        reg_offset_32   = addr_32 & 0xFFF
        sram_offset_32  = addr_32 & 0xFFFF
        is_broadcast = 1 if (((addr_32 >> 30) & 0x3) == 0x1) else 0
        is_sram = 1 if (((addr_32 >> 31) & 0x1) == 0x1) else 0
        is_sram1_select = 1 if (((addr_32 >> 30) & 0x3) == 0x3) else 0

        block_select  = (addr_32 >> 12) & 0x3
        lane_select   = (addr_32 >> 25) & 0x1F
        sram_select   = (addr_32 >> 31) & 0x1

        if is_broadcast == 1:
            raise Exception("ERROR: Broadcast is not supported with 16-bit addr = " +  addr_32)

        if is_sram == 1:
            if is_sram1_select == 1:
                raise Exception("ERROR: SRAM1 select is not supported with 16-bit addr = " +  addr_32)

            if ((sram_offset_32 >> 14) & 0x3 != 0x00):
                raise Exception("ERROR: Only SRAM addr of 14-bits is supported with 16-bit addr = " +  addr_32)

        reg_offset_16 = reg_offset_32 >> 1
        sram_offset_16 = sram_offset_32 >> 1

        if sram_select == 1:
            addr_16 = (0x3 << 14) | (0x0 << 13) | sram_offset_16
        else:
            if lane_select == 0:
              device_select = 0
            elif (lane_select >= 1 and lane_select <= 4):
                lane_num = lane_select - 1
                device_select = ((lane_num * 4) + 1) + block_select
            else:
                raise Exception("ERROR: Only lanes 0-3 are supported with 16-bit addr:  lane_select = " +  lane_select)

            addr_16 = (device_select << 11) | reg_offset_16

        return addr_16
*/
uint32_t svdpi_convert_32to16_addr(uint32_t addr_32) {
  uint32_t addr_16, device_select, lane_num;
  uint32_t reg_offset_32   = addr_32 & 0xFFF;
  uint32_t sram_offset_32  = addr_32 & 0xFFFF;
  uint32_t is_broadcast = (((addr_32 >> 30) & 0x3) == 0x1) ? 1 : 0;
  uint32_t is_sram = (((addr_32 >> 31) & 0x1) == 0x1) ? 1 : 0;
  uint32_t is_sram1_select = (((addr_32 >> 30) & 0x3) == 0x3) ? 1 : 0;

  uint32_t block_select  = (addr_32 >> 12) & 0x3;
  uint32_t lane_select   = (addr_32 >> 25) & 0x1F;
  uint32_t sram_select   = (addr_32 >> 31) & 0x1;

  if (is_broadcast == 1) {
      USR_PRINTF("ERROR: Broadcast is not supported with 16-bit addr = %08x",  addr_32);
  }
  if (is_sram == 1) {
      if (is_sram1_select == 1) {
          USR_PRINTF("ERROR: SRAM1 select is not supported with 16-bit addr = %08x",  addr_32);
      }
      if (((sram_offset_32 >> 14) & 0x3) != 0x00) {
          USR_PRINTF("ERROR: Only SRAM addr of 14-bits is supported with 16-bit addr = %08x",  addr_32);
      }
  }

  uint32_t reg_offset_16 = reg_offset_32 >> 1;
  uint32_t sram_offset_16 = sram_offset_32 >> 1;

  if (sram_select == 1) {
      addr_16 = (0x3 << 14) | (0x0 << 13) | sram_offset_16;
  } else {
      if (lane_select == 0) {
        device_select = 0;
      } else if ((lane_select >= 1) && (lane_select <= 4)) {
          lane_num = lane_select - 1;
          device_select = ((lane_num * 4) + 1) + block_select;
      } else{
          USR_PRINTF("ERROR: Only lanes 0-3 are supported with 16-bit addr:  lane_select = %08x\n",  lane_select);
          //bfn hack
          USR_PRINTF("ERROR: addr_32 = %08x\n", addr_32);
          USR_PRINTF("ERROR: lane_select = %08x\n", lane_select);
          return 0xffffffff;
      }
      addr_16 = (device_select << 11) | reg_offset_16;
  }
  return addr_16;
}


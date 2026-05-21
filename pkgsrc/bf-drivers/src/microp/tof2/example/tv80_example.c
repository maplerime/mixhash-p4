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

/* example z80 C program
 *
 * compile with sdcc compiler (http://sdcc.sourceforge.net/)
 *
 * sdcc -mz80 --no-std-crt0 --code-loc 1024 tv80_example.c
 *
 * or, for vc style error handling,
 *
 * sdcc -mz80 --no-std-crt0 --vc --code-loc 1024 --data-loc 2048 tv80_example.c
 *
 * the linker produces the code which starts from address 1024 (0x400)
 *
 * the output is in the form of intel hex with above command line.
 * convert it to binary payload before loading it in tv80 mem space.
 *
 * convert using objcopy command like:
 * objcopy -I ihex -O binary <ihex file> <bin file>
 *
 * load the binary to TV80 memory space starting from location 1024
 * using APIs in bf-drivers/src/port_mgr/
 * Application start address and maximum program size must match the values
 * set in the boot code, tv80_boot.asm, i.e., LOC_APP_MAIN and LEN_APP_MAIN.
 *
 * no printf/scanf or no libc calls, to conserve space
 *
 * work with 8/16 bit-only variables and addresses. compiler is not good
 * at dealing with 32 bit variables.
 *
 */

/* some well known addresses in tv80 space as declared in tv80_boot.asm */

#define LOC_APP_MAIN 1024L /* application program start location */
#define LOC_DBG_LOG 15360L /* last 1K bytes for debug log */
/* aplication prog max size */
#define LEN_APP_MAIN (LOC_DBG_LOG - LOC_APP_MAIN)

static unsigned char test_chr[64]; /* may not breate a BSS segment!!! */

/* return value would be discarded by calling function (in boot code) */
int main() {
  /* copy first 64 bytes of log  buffer to a global trest buffer */
  unsigned char cnt;
  unsigned char *s_addr = (unsigned char *)LOC_DBG_LOG;
  unsigned char *d_addr = (unsigned char *)test_chr;

  for (cnt = 0; cnt < 64; cnt++) {
    *d_addr++ = *s_addr++;
  }

  return 0;
}

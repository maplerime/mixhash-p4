#################################################################################
#  INTEL CONFIDENTIAL
#
#  Copyright (c) 2022 Intel Corporation
#  All Rights Reserved.
#
#  This software and the related documents are Intel copyrighted materials,
#  and your use of them is governed by the express license under which they
#  were provided to you ("License"). Unless the License provides otherwise,
#  you may not use, modify, copy, publish, distribute, disclose or transmit this
#  software or the related documents without Intel's prior written permission.
#
#  This software and the related documents are provided as is, with no express or
#  implied warranties, other than those that are expressly stated in the License.
#################################################################################

# This exemplary script can be run in switchd bfrt_python environment.
# To do this, use the following command:
# %run -i run_all_tests.py

from perfCli import *

if __name__ == "__main__":
    perf = CPerf(dev_id=0)

    perf.env()

    # run sram_dma test all currently supported configurations
    perf.sram_dma.run(pipes=1, maus=1, rows=1, cols=1)
    perf.sram_dma.run(pipes=2, maus=1, rows=1, cols=1)
    perf.sram_dma.run(pipes=1, maus=1, rows=8, cols=10)
    perf.sram_dma.run(pipes=1, maus=12, rows=8, cols=10)
    perf.sram_dma.run(pipes=2, maus=1, rows=8, cols=10)

    # run tcam_dma test all currently supported configurations
    perf.tcam_dma.run(pipes=1, maus=1, rows=1, cols=1)
    perf.tcam_dma.run(pipes=2, maus=1, rows=1, cols=1)
    perf.tcam_dma.run(pipes=1, maus=1, rows=12, cols=2)
    perf.tcam_dma.run(pipes=1, maus=12, rows=12, cols=2)
    perf.tcam_dma.run(pipes=2, maus=1, rows=12, cols=2)

    # run interrupts test all currently supported configurations
    perf.interrupts.run(bus='PBUS', iterations=1)
    perf.interrupts.run(bus='MBUS', iterations=300)
    perf.interrupts.run(bus='CBUS', iterations=300)
    perf.interrupts.run(bus='HOSTIF', iterations=300)

    # run reg_dir test all currently supported configurations
    perf.reg_dir.run(bus='PBUS')
    perf.reg_dir.run(bus='MBUS')
    perf.reg_dir.run(bus='CBUS')
    perf.reg_dir.run(bus='HOSTIF')

    # run reg_indir test all currently supported configurations
    perf.reg_indir.run(bus='PBUS')
    perf.reg_indir.run(bus='MBUS')
    perf.reg_indir.run(bus='CBUS')
    perf.reg_indir.run(bus='HOSTIF')


#include <stdint.h>
#include <stdbool.h>

#include "../aw_types.h"
#include "../aw_driver_sim.h"
#include "../aw_reg_dbg.h"

#include "aw_alphacore_csr_defines.h"
#include "aw_alphacore_vfield_defines.h"
#include "aw_alphacore_ip_defines.h"


// BFN defined fns
int aw_pmd_16ln_mss_reset(mss_access_t *mss);
int aw_pmd_16ln_fw_load(mss_access_t *mss, char *path);
int aw_pmd_16ln_one_time_pgm(mss_access_t *mss);
int aw_pmd_16ln_cur_rate_width_pstate(mss_access_t *mss,
                                      uint32_t *tx_rate,
                                      uint32_t *tx_width,
                                      uint32_t *tx_pstate,
                                      uint32_t *rx_rate,
                                      uint32_t *rx_width,
                                      uint32_t *rx_pstate);
int aw_pmd_16ln_rxmfsm_eq_check_rxdisable_set(mss_access_t *mss, uint32_t val);
int aw_pmd_16ln_lt_info_get(mss_access_t *mss, uint32_t * lt_fsm_st, uint32_t * frame_lock);
int aw_pmd_16ln_rd_csr(mss_access_t *mss, uint32_t addr, uint32_t *rdata);
int aw_pmd_16ln_wr_csr(mss_access_t *mss, uint32_t addr, uint32_t wdata);
int aw_pmd_16ln_reg_defs_get(aw_reg_defs_t **regs,
                            aw_fld_defs_t **flds,
                            char **cmnts,
                            aw_reg_defs_t **vregs,
                            aw_fld_defs_t **vflds,
                            char **vcmnts);
int aw_pmd_16ln_speed_to_rate_and_width(mss_access_t *mss, uint32_t serdes_speed, bool is_pam4, uint32_t *rate, uint32_t *width);
int aw_pmd_16ln_anlt_width_set(mss_access_t *mss);
int aw_pmd_16ln_anlt_ms_per_clk_set(mss_access_t *mss);
int aw_pmd_16ln_rx_dig_pwr_det_threshold_get(mss_access_t *mss, uint32_t *adc_valid_thresh_nt, uint32_t *adc_invalid_thresh_nt );
int aw_pmd_16ln_rx_dig_pwr_det_threshold_set(mss_access_t *mss, uint32_t adc_valid_thresh_nt, uint32_t adc_invalid_thresh_nt );


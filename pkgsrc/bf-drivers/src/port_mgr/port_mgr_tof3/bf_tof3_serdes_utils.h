#ifndef BF_TOF3_SERDES_UTILS_INCLUDED
#define BF_TOF3_SERDES_UTILS_INCLUDED

#include <stdint.h>
#include <bf_types/bf_types.h>
#include "aw_if.h"

bf_status_t bf_tof3_serdes_lane_install(bf_dev_id_t dev_id,
                                        bf_dev_port_t dev_port,
                                        uint32_t logical_ln, uint32_t tx_ln,
                                        uint32_t rx_ln, uint32_t is_4ln);

bf_status_t bf_tof3_serdes_csr_def_get(uint32_t dev_id, uint32_t dev_port,
                                       uint32_t ln, uint32_t csr_addr,
                                       char **name, uint32_t *num_flds,
                                       uint32_t *fld_num_base, char **comment);

bf_status_t bf_tof3_serdes_vreg_def_get(uint32_t dev_id, uint32_t dev_port,
                                        uint32_t ln, uint32_t csr_addr,
                                        char **name, uint32_t *num_flds,
                                        uint32_t *fld_num_base, char **comment);

bf_status_t bf_tof3_serdes_csr_def_get_next(uint32_t dev_id, uint32_t dev_port,
                                            uint32_t ln, uint32_t csr_addr,
                                            uint32_t *next_csr_addr,
                                            char **name, uint32_t *num_flds,
                                            uint32_t *fld_num_base,
                                            char **comment);

bf_status_t bf_tof3_serdes_csr_fld_def_get(
    uint32_t dev_id, uint32_t dev_port, uint32_t ln, uint32_t csr_addr,
    uint32_t fld_num, char **name, uint32_t *lo_bit, uint32_t *width,
    uint32_t *mask, uint32_t *access, uint32_t *reset_value, char **comment);
int bf_tof3_serdes_read_status(uint32_t dev_id, uint32_t dev_port, uint32_t ln,
                               uint32_t br);
int bf_tof3_serdes_read_status2(uint32_t dev_id, uint32_t dev_port, uint32_t ln,
                                uint32_t br);
char *bf_tof3_serdes_prbs_mode_str(uint32_t mode);
char *bf_tof3_serdes_term_mode_str(uint32_t mode);
bf_status_t bf_tof3_bf_to_aw_prbs_pat(bf_port_prbs_mode_t bf_pat,
                                      aw_bist_pattern_t *aw_pat);
bf_status_t bf_tof3_sweep(bf_dev_id_t dev_id, bf_dev_port_t tx_dev_port,
                          bf_dev_port_t rx_dev_port, uint32_t ln,
                          // tx settings
                          uint32_t cm3, uint32_t cm2, uint32_t cm1, uint32_t c0,
                          uint32_t c1,
                          // rx settings
                          uint32_t ctle_adapt_en, uint32_t ctle_adapt_boost,
                          uint32_t vga_cap,
                          // return vals
                          uint32_t *eq_ack, uint32_t *cdr_lock,
                          uint32_t *bist_lock, double *ber);
bf_status_t bf_tof3_run_lt(bf_dev_id_t dev_id, bf_dev_port_t dev_port,
                           uint32_t ln, uint32_t *lt_status);
bf_status_t bf_tof3_run_lt2(bf_dev_id_t dev_id, bf_dev_port_t dev_port,
                            uint32_t ln, uint32_t width, uint32_t clause);

char *bf_tof3_serdes_tech_ability_to_str(uint32_t tech_ability);
char *bf_tof3_serdes_loopback_mode_to_str(uint32_t loopback_mode);
uint32_t bf_tof3_serdes_str_to_loopback_mode(char *str);

#endif // BF_TOF3_SERDES_UTILS_INCLUDED

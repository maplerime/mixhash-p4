#ifndef SV_DPI_INCLUDED
#define SV_DPI_INCLUDED

// SV
extern void sv_mss_reset(uint32_t macro_base_addr);
extern void sv_write_csr(uint32_t addr, uint32_t wdata);
extern void sv_read_csr(uint32_t addr, uint32_t *rdata);
extern void sv_delay_us(int delay_us);

#endif // SV_DPI_INCLUDED


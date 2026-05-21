#ifndef AW_IO_INCLUDED
#define AW_IO_INCLUDED

typedef void (*aw_write_csr)(uint32_t dev_id,
                             uint32_t subdev_id,
                             uint32_t addr,
                             uint32_t wdata,
                             uint32_t phy_offset_used);
typedef void (*aw_read_csr)(uint32_t dev_id,
                            uint32_t subdev_id,
                            uint32_t addr,
                            uint32_t *rdata,
                            uint32_t phy_offset_used);

typedef struct aw_hw_io_t {
  aw_write_csr write_csr;
  aw_read_csr read_csr;
} aw_hw_io_t;

extern aw_hw_io_t aw_evb_sim_io;
extern aw_hw_io_t aw_evb_io;
extern aw_hw_io_t aw_4ln_io;
extern aw_hw_io_t aw_16ln_io;

#endif  // AW_IO_INCLUDED

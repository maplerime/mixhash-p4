#ifndef _P4_PD_HELPER_H_
#define _P4_PD_HELPER_H_

//#include <stdint.h>
//#include <bf_types/bf_types.h>

#ifdef __cplusplus
extern "C" {
#endif

inline bf_dev_pipe_t i16_to_bf_pipe(int16_t pipe) {
  return (pipe == -1) ? BF_DEV_PIPE_ALL : (bf_dev_pipe_t)pipe;
}

#ifdef __cplusplus
}
#endif

#endif /* _P4_PD_HELPER_H_ */

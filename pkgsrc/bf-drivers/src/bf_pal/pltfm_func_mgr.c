#include <stdio.h>
#include <string.h>
#include <stddef.h>
#include <tofino/bf_pal/pltfm_func_mgr.h>

pltfm_func_s pltfm_func;

bf_status_t bf_pal_pltfm_fn_reg(pltfm_func_s *param) {
  if (param == NULL) {
    return BF_INVALID_ARG;
  }

  memcpy((char *)&pltfm_func, (char *)param, sizeof(pltfm_func_s));
  return BF_SUCCESS;
}

bf_status_t bf_pal_pltfm_fn_get(pltfm_func_s *ptr) {
  if (ptr == NULL) {
    return BF_INVALID_ARG;
  }

  memcpy((char *)ptr, (char *)&pltfm_func, sizeof(pltfm_func_s));
  return BF_SUCCESS;
}

bf_status_t bf_pal_pltfm_type_get_fn_get(
    bf_pal_pltfm_type_get_fn *pltfm_type_get_fn) {
  if (pltfm_func.pltfm_type_get == NULL) {
    return BF_NOT_IMPLEMENTED;
  }

  *pltfm_type_get_fn = pltfm_func.pltfm_type_get;

  return BF_SUCCESS;
}

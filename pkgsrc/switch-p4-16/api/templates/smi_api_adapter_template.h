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

#ifndef _SMI_API_ADAPTER_H_
#define _SMI_API_ADAPTER_H_

#include "bf_switch/bf_switch_types.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

//:: import template_generator as template
//:: attribute_iterator = template.attribute_generator(_context, True)
/******************************************************************************
 * ENUMS
 ******************************************************************************/
//:: for enums, key, objectname in attribute_iterator:
typedef enum switch_${objectname}_${key}_s {
//::          for enum in enums:
   SWITCH_${objectname}__${key}_${enum},
//::          #endfor
} switch_${objectname}_${key}_t;

//:: #endfor
//:: attribute_iterator = template.attribute_generator(_context, False)
//:: counter = -1
//:: oldobjectname = ""
/******************************************************************************
 * STRUCTS
 ******************************************************************************/
//:: for type, key, _, _, _, _, objectname in attribute_iterator:
//::   if oldobjectname != objectname:
//::      if counter !=-1:
} switch_api_${oldobjectname}_t;

//::      #endif
//::      counter = 1
//::      oldobjectname = objectname
typedef struct switch_api_${objectname}_s {
//::    #endif
     ${type} ${key};
//::  counter +=1
//:: #endfor
} switch_api_${objectname}_t;

//:: object_iterator = template.object_generator(_context)
//:: for objectname, counter in object_iterator:
//::   extension = ""
//::   if counter != 0:
//::      extension = ", switch_api_" + objectname + "_t *structobject"
//::   #endif
switch_status_t switch_api_${objectname}_create(switch_object_id_t *const object_handle${extension});

switch_status_t switch_api_${objectname}_delete(switch_object_id_t const object_handle);

//:: #endfor



//:: attribute_iterator = template.attribute_generator(_context, False)
//:: for type, key, _, _, _, _, objectname in attribute_iterator:

switch_status_t switch_api_${objectname}_set_${key}(
        switch_object_id_t const object_handle, ${type} ${key});

switch_status_t switch_api_${objectname}_get_${key}(
        switch_object_id_t const object_handle, ${type} *${key});
//:: #endfor

#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif /* header guard */

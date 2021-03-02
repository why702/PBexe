/*
 * Copyright (c) 2020, Precise Biometrics AB
 * All rights reserved.
 *
 */

#ifndef PB_IMAGE_SIGNAL_PROCESSORI_H
#define PB_IMAGE_SIGNAL_PROCESSORI_H

#include <stdint.h>

#include "pb_image_signal_processor.h"
#include "pb_session.h"
#include "pb_errors.h"

#ifdef __cplusplus
extern "C" {
#endif


/** Creates an Image Signal Processor (ISP) module object.
  * 
  * The implementation shall call pb_image_signal_processor_create with the appropriate
  * parameters for the specific ISP. 
  *
  * @param[in] session is the session object.
  * @param[out] isp is the returned created ISP object. The caller is responsible for 
  *             deleting the returned object by calling pb_image_signal_processor_delete
  *
  * @return PB_RC_OK if successful, otherwise an error code.
  */
typedef pb_rc_t pb_image_signal_processorI_create_fn_t (pb_session_t* session, pb_image_signal_processor_t** isp);

/** Interface for the ISP module, used for writing different implementations for specific sensors. */
typedef struct {
    /** Creates an ISP module object. */
    pb_image_signal_processorI_create_fn_t* create;
} pb_image_signal_processorI;

#ifdef __cplusplus
}
#endif

#endif /* PB_IMAGE_SIGNAL_PROCESSORI_H */

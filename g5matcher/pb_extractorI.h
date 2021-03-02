/*
 * Copyright (c) 2008 - 2013, Precise Biometrics AB
 * All rights reserved.
 *
 */

#ifndef PB_EXTRACTORI_H
#define PB_EXTRACTORI_H

#include "pb_errors.h"
#include "pb_session.h"
#include "pb_image.h"
#include "pb_finger.h"
#include "pb_template.h"

#ifdef __cplusplus
extern "C" {
#endif

/** Extracts a template from the image, if the specified template
  * type is supported by the extractor.
  *
  * @param[in] session is the session object.
  * @param[in] image is the image to extract information from
  *     into a template.
  * @param[in] finger is the finger associated with the image.
  * @param[in] template_type is the template type to be extracted.
  *     If that type is not supported by the extractor, 
  *     PB_RC_NOT_SUPPORTED is returned.
  * @param[out] template_ is the returned extracted template. The 
  *     caller is responsible for deleting the template.
  *
  * @return PB_RC_OK if successful, or an error code. 
  */

typedef pb_rc_t pb_extractorI_extract_template_fn_t (pb_session_t* session,
                                                     const pb_image_t* image, 
                                                     const pb_finger_t* finger,
                                                     pb_template_type_t template_type,
                                                     pb_template_t** template_);

/// Template extractor interface
/** A fingerprint feature extractor implements the extractor interface.
  * 
  * The purpose of an extractor implementation is to create a fingerprint
  * template from a fingerprint image.
  * 
  * Different implementations may support different kinds of templates.
  * An application needs to be configured to use the extractor for the
  * intended purpose. Some implementations may be targeted to embedded
  * systems while others are more resource demanding.
  * 
  * Some extractors may offer much more customization parameters
  * offered by the extractor interface. Such parameters can be passed
  * via the session object.
  */
typedef struct {
    /// Extractor function pointer
    pb_extractorI_extract_template_fn_t* extract_template;
} pb_extractorI;

#ifdef __cplusplus
}
#endif

#endif /* PB_EXTRACTORI_H */

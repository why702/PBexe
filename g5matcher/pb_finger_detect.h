/*
 * Copyright (c) 2017, Precise Biometrics AB
 * All rights reserved.
 *
 */

#ifndef PB_FINGER_DETECT_H
#define PB_FINGER_DETECT_H

#include <stdint.h>

#include "pb_image.h"
#include "pb_errors.h"
#include "pb_session.h"
#include "pb_types.h"
#include "pb_image_t.h"

#ifdef __cplusplus
extern "C" {
#endif

/** Returns a score indicating the fingerprintness of the source image using a
  * generic model.
  *
  * @param[in]  |session| is the session object (must not be null). Can be
  *             provided like this from current algorithm object:
  *               pb_session_t* session;
  *               session = pb_algorithm_get_session(pb_algorithm_t* algorithm);
  * @param[in]  |image| is the image object.
  * @param[out] |score| is a value indicating the fingerprintness of the
  *             provided image. The more the image resembles a fingerprint, the
  *             higher the score. A normal image would get a score around 20000,
  *             non fingerprint images will get lower values. It is recommended
  *             to elaborate on a set of known images to find a suitable
  *             threshold to reject images with too low score.
  */
pb_rc_t
pb_finger_detect_get_fingerprint_score(pb_session_t* session,
                                       pb_image_t* image,
                                       uint16_t* score);

#ifdef __cplusplus
}
#endif

#endif /* PB_FINGER_DETECT_H */

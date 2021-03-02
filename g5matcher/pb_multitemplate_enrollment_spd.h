/*
 * Copyright (c) 2008 - 2018, Precise Biometrics AB
 * All rights reserved.
 *
 */

#ifndef PB_MULTITEMPLATE_ENROLLMENT_SPD_H
#define PB_MULTITEMPLATE_ENROLLMENT_SPD_H

#include "pb_multitemplate_enrollment.h"
#include "pb_static_pattern_detect.h"
#include "pb_errors.h"

#ifdef __cplusplus
extern "C" {
#endif
/** See pb_multitemplate_enrollment.h for a full description of the multitemplate
  * enrollment controller. */

/** This file describes the API:s when using the SPD object.
  *
  * The SPD is connected to the MTE by calling pb_multitemplate_enroll_set_spd()
  *
  * Note: The SPD will be retained when calling get or set, so it will need to
  *       be deleted by the caller.
  */

typedef struct pb_multitemplate_enroll_st pb_multitemplate_enroll_t;

/** Set the multitemplate enrollment object to use the provided SPD.
  * Any existing SPD will be replaced.
  *
  * Note: The MTE will retain the SPD meaning that the SPD still needs to be
  *       deleted by the caller.
  *
  * @param[in] mte is the multitemplate enrollment object.
  * @param[in] spd is the static pattern detect object
  */
pb_rc_t
pb_multitemplate_enroll_set_spd(pb_multitemplate_enroll_t* mte,
                                pb_static_pattern_detect_t* spd);

#ifdef __cplusplus
}
#endif

#endif /* PB_MULTITEMPLATE_ENROLLMENT_SPD_H */

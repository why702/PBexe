/*
 * Copyright (c) 2008 - 2018, Precise Biometrics AB
 * All rights reserved.
 *
 */

#ifndef PB_ALGORITHM_SPECTRAL_H
#define PB_ALGORITHM_SPECTRAL_H

#include "pb_algorithmI.h"
#include "pb_types.h"

#ifdef __cplusplus
extern "C" {
#endif


/** Algorithm used for extra large square sensors, approximately 10x10 mm 
  * (200x200 pixels @ 500 dpi). */
extern pbif_const pb_algorithmI gen_192x192_spectral_algorithm;

#ifdef __cplusplus
}
#endif

#endif /* PB_ALGORITHM_SPECTRAL_H */

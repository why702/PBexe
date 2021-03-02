/*
 * Copyright (c) 2008 - 2015, Precise Biometrics AB
 * All rights reserved.
 *
 */

#ifndef PB_ALGORITHM_MINUTIAE_H
#define PB_ALGORITHM_MINUTIAE_H

#include "pb_algorithmI.h"
#include "pb_preprocessorI.h"
#include "pb_types.h"

#ifdef __cplusplus
extern "C" {
#endif

/** Preprocessor to be used for the mobile minutiae algorithm. */
extern pbif_const pb_preprocessorI pb_minutiae_mobile_preprocessor;

/** Algorithm used for extra large square sensors, approximately 10x10 mm 
  * (200x200 pixels @ 500 dpi). Also used for full size sensors. */
extern pbif_const pb_algorithmI gen_192x192_minutiae_algorithm;
/* Same as above, but with extractor optimized for speed and memory. */
extern pbif_const pb_algorithmI gen_192x192_minutiae_speed_mem_algorithm;

/** Generic minutiae algorithm, optimized for mobile. */
extern pbif_const pb_algorithmI gen_0x0_minutiae_mobile_algorithm;

#ifdef __cplusplus
}
#endif

#endif /* PB_ALGORITHM_MINUTIAE_H */

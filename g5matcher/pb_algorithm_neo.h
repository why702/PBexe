/*
 * Copyright (c) 2016, Precise Biometrics AB
 * All rights reserved.
 *
 */

#ifndef PB_ALGORITHM_NEO_H
#define PB_ALGORITHM_NEO_H

#include "pb_algorithmI.h"
#include "pb_types.h"

#ifdef __cplusplus
extern "C" {
#endif

/** Algorithm used for medium square sensors, approximately 6.5x6.5 mm 
  * (130x130 pixels @ 500 dpi). */
extern pbif_const pb_algorithmI gen_130x130_neo_algorithm;
/** Same as above, but with extractor optimized for speed. */
extern pbif_const pb_algorithmI gen_130x130_neo_speed_algorithm;

#ifdef __cplusplus
}
#endif

#endif /* PB_ALGORITHM_NEO_H */

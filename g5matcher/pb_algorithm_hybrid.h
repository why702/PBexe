/*
 * Copyright (c) 2008 - 2018, Precise Biometrics AB
 * All rights reserved.
 *
 */

#ifndef PB_ALGORITHM_HYBRID_H
#define PB_ALGORITHM_HYBRID_H

#include "pb_algorithmI.h"
#include "pb_types.h"

#ifdef __cplusplus
extern "C" {
#endif


/** Algorithm used for extra large square sensors, approximately 10x10 mm
    * (200x200 pixels @ 500 dpi). */
extern pbif_const pb_algorithmI gen_192x192_hybrid_algorithm;

/* For 160x160 pixels at 500 dpi. */
extern pbif_const pb_algorithmI gen_160x160_hybrid_algorithm;


/* Same as above, but with extractor optimized for speed and memory. */
extern pbif_const pb_algorithmI gen_10x10_hybrid_embedded_254dpi_algorithm;

/* For 160x160 pixels at 500 dpi. */
extern pbif_const pb_algorithmI gen_8x8_hybrid_embedded_254dpi_algorithm;

/* For 80x80 pixels at 250 dpi. */
extern pbif_const pb_algorithmI gen_8x8_hybrid_plus_embedded_254dpi_algorithm;

/** Algorithm used for medium rectangular sensors, approximately 10x4 mm
  * (200x80 pixels @ 500 dpi). */
extern pbif_const pb_algorithmI gen_192x80_hybridhr_algorithm;

/* Same as above, but optimized for limited platforms. */
extern pbif_const pb_algorithmI gen_192x80_pro_speed_mem_algorithm;

extern pbif_const pb_algorithmI gen_10x4_hybrid_embedded_254dpi_algorithm;


#ifdef __cplusplus
}
#endif

#endif /* PB_ALGORITHM_HYBRID_H */

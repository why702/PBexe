/*
 * Copyright (c) 2016, Precise Biometrics AB
 * All rights reserved.
 *
 */

#ifndef PB_ALGORITHM_CARDO_H
#define PB_ALGORITHM_CARDO_H

#include "pb_algorithmI.h"
#include "pb_preprocessorI.h"
#include "pb_types.h"

#ifdef __cplusplus
extern "C" {
#endif


/** The algorithms below uses the Cardo extractor in combination
  * with the Cardo verifier and Cardo templates.
  *
  * The various alternatives are targeted towards mobile platforms.
  *
  * Choose the algorithm that best matches the size of the sensor to
  * be used.
  */

/** Cardo embedded pre-processor, for low-quality images */
extern pbif_const pb_preprocessorI pb_cardo_embedded_lowq_preprocessor;

/** Cardo embedded pre-processor, for high-quality images */
extern pbif_const pb_preprocessorI pb_cardo_embedded_highq_preprocessor;

/** Preprocessor to be used for Cardo. */
extern pbif_const pb_preprocessorI pb_cardo_preprocessor;

/** Denoiser (row and column noise) used by the Cardo Evaluation algorithm.
 *  Reduces row and column noise. */
extern pbif_const pb_preprocessorI pb_cardo_denoiser;

/** 
  * Algorithm used for evaluating biometric performance.
  *
  * The algorithm supports raw runs to generate a DET curve.
  * The DET curve can be used to compare how different changes on
  * sensor side impact the matcher.
  *
  * For running in PerfEval use the following command:
  * PerfEval.exe -algo=gen_0x0_eval_cardo -enr=XofX -div=Y -ver_type=raw <indexfile>
  * where X is the number of templates to enroll in the multitemplate and Y=X+1
  * If static pattern detection should be used, also add -static_pattern_detection
  *
  * Please note that this algorithm is currently just supported for use in PerfEval.
  */
extern pbif_const pb_algorithmI gen_0x0_eval_cardo_algorithm;

/**
 * In order to improve biometric performance in the cardo_speed_mem algorithms
 * below it is possible to enable using the image mask during extraction of
 * descriptors. Enabling this feature is done by setting the session parameter
 * pb.cardo.extract.mask_descriptors to 1. Doing so will have an negative effect
 * on extraction latency.
 *
 * Note: If used without suppling a mask with the image there will be no change in
 * bio-performance.
 */

/** Algorithm used for full sized sensors, approximately 13x18 mm
 * (256x360 pixels @ 500 dpi). Latency and memory optimized for embedded use. */
extern pbif_const pb_algorithmI gen_fullsize_cardo_embedded_254dpi_algorithm;

/** Algorithm used for extra large square sensors, approximately 10x10 mm
 * (192x192 pixels @ 500 dpi). Latency and memory optimized for embedded use. */
extern pbif_const pb_algorithmI gen_10x10_cardo_embedded_363dpi_algorithm;

/** Algorithm used for large square sensors, approximately 8x8 mm
 * (160x160 pixels @ 500 dpi). Latency and memory optimized for embedded use. */
extern pbif_const pb_algorithmI gen_8x8_cardo_embedded_363dpi_algorithm;

/** Algorithm used for small square sensors, approximately 5.8x5.8 mm
 * (115x115 pixels @ 500 dpi). Latency and memory optimized for embedded use. */
extern pbif_const pb_algorithmI gen_6x6_cardo_embedded_363dpi_algorithm;
/** Algorithm used for small square sensors, approximately 5.8x5.8 mm
Same as above but used for different input resolution. */
extern pbif_const pb_algorithmI gen_6x6_cardo_embedded_508dpi_algorithm;

/** Algorithm used for small capacitive sensors. */
extern pbif_const pb_algorithmI gen_80x64_cardo_capacitive_algorithm;

#ifdef __cplusplus
}
#endif

#endif /* PB_ALGORITHM_CARDO_H */

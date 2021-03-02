/*
 * Copyright (c) 2017, Precise Biometrics AB
 * All rights reserved.
 *
 */

#ifndef PB_PAD_DUMMY_H
#define PB_PAD_DUMMY_H

#include "pb_padI.h"
#include "pb_types.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Dummy PAD classifier that returns a liveness score of 500. This classifier
 * is NOT intended for production code.
 * 
 * Use the pb_dummy_pad classifier to evaluate how the added security of
 * Precise BioMatch presentation attack detection impacts the system resources
 * and application latency for sensors of different dimensions.
 * 
 * Using this model will run the PAD algoritihms in the same manner as a trained
 * production ready classifier. The only difference is that the calculated liveness
 * score for live and spoof images always will be 500 with this classifier. 
 * 
 */
extern pbif_const pb_padI pb_dummy_pad;

#ifdef __cplusplus
}
#endif

#endif /* PB_PAD_DUMMY_H */

/*
 * Copyright (c) 2018, Precise Biometrics AB
 * All rights reserved.
 *
 */

#ifndef PB_IMAGE_CONVERT_H
#define PB_IMAGE_CONVERT_H

#include <stdint.h>

#include "pb_rawimage_converterI.h"
#include "pb_image_t.h"
#include "pb_types.h"

/** 
 * Converts a RAW 8-bit image buffer into a 8-bit pb_image_t 
 * Basically this simplest form of converter is no converter at all,
 * but simply creates a pb_image_t.
 */
pb_image_t* 
pb_image_convert_from_8bit (const uint8_t* pixels, 
                            uint16_t rows, 
                            uint16_t cols, 
                            uint16_t resolution);

extern pbif_const pb_rawimage_converterI gen_8bit_converter;

#ifdef __cplusplus
extern "C" {
#endif


#ifdef __cplusplus
}
#endif

#endif /* PB_IMAGE_CONVERT_H */

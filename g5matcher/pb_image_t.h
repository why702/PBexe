/*
 * Copyright (c) 2008 - 2017, Precise Biometrics AB
 * All rights reserved.
 *
 */

#ifndef PB_IMAGE_T_H
#define PB_IMAGE_T_H

#include "pb_types.h"

#ifdef __cplusplus
extern "C" {
#endif

/** A image object holds a fingerprint image. The fingerprint image
  * is an 8-bit grey scale image with a specified resolution,
  * width and height. The image object also offers some basic
  * image transformations to get alternate representations of the image
  * like a scaled down version suitable for presentation in a GUI. 
  */
typedef struct pb_image_st pb_image_t;

/** The relation between a full image and an sub image cropped from the full image. */
typedef struct pb_image_relation_st {
    /** The number of rows in the full image. */
    uint16_t full_rows;
    /** The number of columns in the full image. */
    uint16_t full_cols;

    /** The number of rows in the sub image. */
    uint16_t sub_rows;
    /** The number of columns in the sub image. */
    uint16_t sub_cols;
    /** The offset in rows of the sub image within the full image. */
    int16_t sub_offset_rows;
    /** The offset in columns of the sub image within the full image. */
    int16_t sub_offset_cols;
} pb_image_relation_t;

#ifdef __cplusplus
}
#endif

#endif /* PM_IMAGE_T_H */

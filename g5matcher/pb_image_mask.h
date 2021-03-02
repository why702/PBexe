/*
 * Copyright (c) 2008 - 2018, Precise Biometrics AB
 * All rights reserved.
 *
 */

#ifndef PB_IMAGE_MASK_H
#define PB_IMAGE_MASK_H

#include <stddef.h>
#include <stdint.h>

#include "pb_types.h"
#include "pb_errors.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Note! It is the callers responsibility to make sure that 
 * all image mask input parameters are valid, i.e. not null (0). 
 * Passing a null image mask parameter to a function will result 
 * in a runtime error!
 * 
 * Due to the bitmask nature of this enum, the internal representation needs
 * to be an uint8_t.
 *
 */

/** Image mask types. A mask pixel with value 0 is considered fingerprint. */
enum pb_image_mask_type_e {
    /** Background, i.e. non-fingerprint. */
    PB_IMAGE_MASK_TYPE_BACKGROUND               = 0x10,
    /** Possible static pattern mask type, i.e. these pixels may contain static pattern */
    PB_IMAGE_MASK_TYPE_POSSIBLE_STATIC_PATTERN  = 0x20,
    /** Static pattern mask type, i.e. these pixels contain static pattern. */
    PB_IMAGE_MASK_TYPE_STATIC_PATTERN           = 0x40,

    /** Backgrounds as set by internal segmentation of the image. For internal use only. */
    /** Static segmentation background, doesn't change over time. */
    PB_IMAGE_MASK_TYPE_SEGMENTATION_BACKGROUND_STATIC  = 0x01,
    /** Dynamic segmentation background, may change over time, e.g. due to 
      * preprocessing of the image. */
    PB_IMAGE_MASK_TYPE_SEGMENTATION_BACKGROUND_DYNAMIC = 0x02,
};
typedef uint8_t pb_image_mask_type_t;


/** An image mask object contains additional information of each 
  * pixel in the image, e.g. if the pixel is background, fingerprint etcs. 
  * The image mask is a vector with rows * columns values, where each index
  * is computed as r * cols + c for a pixel at row r and column c. */
typedef struct pb_image_mask_st pb_image_mask_t;


/** Allocates an image mask object and sets the reference counter to 1. Initially
  * sets all pixels to 0, i.e. no mask. 
  * 
  * @param[in] rows is the number of rows in the corresponding image. 
  * @param[in] cols is the number of columns in the corresponding image. 
  *
  * @return the allocated image mask object if successful, else 0. 
  */
pb_image_mask_t*
pb_image_mask_create (uint16_t rows,
                      uint16_t cols);

/** Retains the object, which means that the reference counter for the object
  * will increase by 1. The caller must make sure that _delete is being called
  * when the object is not needed anymore. Retaining a null pointer has
  * no effect. */
pb_image_mask_t*
pb_image_mask_retain (pb_image_mask_t* image_mask);

/** Decreases the reference counter for the object by 1. If the reference 
  * counter reaches 0 then the object will also be deleted. 
  * When deleted, it will also call _delete on the user object, since _retain 
  * was called on the user object when the image mask was created.
  * Deleting a null pointer has no effect. */
void
pb_image_mask_delete (pb_image_mask_t* image_mask);

/** Creates a copy of the image_mask. */
pb_image_mask_t* 
pb_image_mask_copy (const pb_image_mask_t* image_mask);

/** Tells if image_mask 1 is a copy of image_mask 2. */
int
pb_image_mask_is_copy (const pb_image_mask_t* image_mask1, 
                       const pb_image_mask_t* image_mask2);

/** Returns the number of rows for the mask. */
uint16_t
pb_image_mask_get_rows (const pb_image_mask_t* image_mask);

/** Returns the number of columns for the mask. */
uint16_t
pb_image_mask_get_cols (const pb_image_mask_t* image_mask);

/** Scales the image mask.
  * 
  * @param[in] image_mask is the image mask to be scaled.
  * @param[in] rows is the number of rows to be scaled to. 
  * @param[in] cols is the number of columns to be scaled to. 
  *
  * @return a scaled image mask. The caller is responsible for deleting the
  *         returned image mask. 
  */
pb_image_mask_t*
pb_image_mask_scale (pb_image_mask_t* image_mask, 
                     uint16_t rows,
                     uint16_t cols);

/** Flips the image mask.
  * 
  * @param[in] image_mask is the image mask to be flipped.
  * @param[in] flip is the type of flip to be performed. 
  *
  * @return a flipped image mask. The caller is responsible for deleting the
  *         returned image mask. 
  */
pb_image_mask_t*
pb_image_mask_flip (pb_image_mask_t* image_mask,
                    pb_flip_t flip);

/** Rotates the image mask.
  * 
  * @param[in] image_mask is the image mask to be rotated.
  * @param[in] rotation is the rotation amount to be performed, in binary radians. 
  *
  * @return a rotated image mask. The caller is responsible for deleting the
  *         returned image mask. 
  */
pb_image_mask_t*
pb_image_mask_rotate (pb_image_mask_t* image_mask,
                      uint8_t rotation);

/** Crops the image mask.
  * 
  * @param[in] image_mask is the image mask to be cropped.
  * @param[in] rows is the number of rows to be cropped to. 
  * @param[in] cols is the number of columns to be cropped to. 
  * @param[in] offset_rows is the number of rows that the crop is offset from. 
  * @param[in] offset_cols is the number of columns that the crop is offset from. 
  *
  * @return a cropped image mask. The caller is responsible for deleting the
  *         returned image mask. 
  */

/** Transforms an image mask to a power series on radius as in pb_image_lens_correction()
  * 
  * @param[in] mask is the image mask to be transformed
  * @param[in] a is the r^3 coefficient
  * @param[in] b is the r^2 coefficient
  * @param[in] c is the r^1 coefficient
  * @param[out] out_image_mask transformed image mask. The caller is responsible of deleting the returned image mask.
  * 
  * @return the status
  */
pb_rc_t
pb_image_mask_lens_correction (const pb_image_mask_t *mask, float a, float b, float c, pb_image_mask_t** out_image_mask);

pb_image_mask_t*
pb_image_mask_crop (pb_image_mask_t* image_mask, 
                    uint16_t rows,
                    uint16_t cols,
                    int32_t offset_rows,
                    int32_t offset_cols);

/** Expands the image mask.
  * 
  * @param[in] image_mask is the image mask to be expanded.
  * @param[in] rows is the number of rows to be expanded to. 
  * @param[in] cols is the number of columns to be expanded to. 
  *
  * @return an expanded image mask. The caller is responsible for deleting the
  *         returned image mask. 
  */
pb_image_mask_t*
pb_image_mask_expand (pb_image_mask_t* image_mask, 
                      uint16_t rows,
                      uint16_t cols);

/** Draws the image_mask2 on top of image_mask1.
  *
  * @param[in] image_mask1 is the original image mask.
  * @param[in] image_mask2 is the image mask to draw on top of the original
  *            image mask.
  * @param[in] offset_x is the horizontal offset of image_mask2 within
  *            image_mask1.
  * @param[in] offset_y is the vertical offset of image_mask2 within image_mask1.
  *
  * @return the new image mask object if successful, else 0. The size of the
  *         new image mask will be equal to the size of image_mask1. The caller
  *         is responsible of deleting the new image mask.
  */
pb_image_mask_t*
pb_image_mask_draw (const pb_image_mask_t* image_mask1,
                    const pb_image_mask_t* image_mask2,
                    int32_t offset_x,
                    int32_t offset_y);

/** Adds mask of type to all pixels where type_mask is TRUE. 
  *
  * @param[in] image_mask is the image mask.
  * @param[in] type is the image mask type to be set for the pixels in type_mask
  *            that is "true" (> 0).
  * @param[in] type_mask is a mask with "true" (> 0) values for all pixels that
  *            shall set the mask type.
  *            Pixels with "false" (0) values will not be affected.
  */
void
pb_image_mask_add_type_mask (pb_image_mask_t* image_mask,
                             pb_image_mask_type_t type,
                             uint8_t* type_mask);

/** Adds mask of type to all pixels where type_mask is FALSE. */
void
pb_image_mask_add_type_mask_invert (pb_image_mask_t* image_mask,
                                    pb_image_mask_type_t type,
                                    uint8_t* type_mask);

/** Sets mask to type for all pixels where type_mask is TRUE. 
  *
  * @param[in] image_mask is the image mask. 
  * @param[in] type is the image mask type to be set for the pixels in type_mask
  *            that is "true" (> 0). 
  * @param[in] type_mask is a mask with "true" (> 0) values for all pixels that 
  *            shall set the mask type. Pixels with "false" (0) values will invalidate
  *            any previous set mask types of the specified type. 
  */
void
pb_image_mask_set_type_mask (pb_image_mask_t* image_mask,
                             pb_image_mask_type_t type,
                             uint8_t* type_mask);

/** Sets mask to type for all pixels where type_mask is FALSE. */
void
pb_image_mask_set_type_mask_invert (pb_image_mask_t* image_mask,
                                    pb_image_mask_type_t type,
                                    uint8_t* type_mask);

/** Adds mask of _BACKGROUND to all pixels where type_mask is TRUE. */
void
pb_image_mask_add_background (pb_image_mask_t* image_mask,
                              uint8_t* background_mask);

/** Sets mask to _BACKGROUND for all pixels where type_mask is TRUE. */
void
pb_image_mask_set_background (pb_image_mask_t* image_mask,
                              uint8_t* background_mask);

/** Invalidates all set mask types of the specified type. */
void
pb_image_mask_reset_type (pb_image_mask_t* image_mask,
                          pb_image_mask_type_t type);

/** Returns a mask of the specified type.
  *
  * @param[in] image_mask is the image mask. 
  * @param[in] type is the image mask type to be returned. 
  * @param[out] type_mask is the returned mask with "true" (> 0) values for all pixels 
  *             with the specified mask type.  
  */
void
pb_image_mask_get_type_mask (pb_image_mask_t* image_mask,
                             pb_image_mask_type_t type,
                             uint8_t* type_mask);

/** Sets a mask type for a specific pixel in the image mask. 
  *
  * @param[in] image_mask is the image mask. 
  * @param[in] type is the image mask type to be set for the specified pixel. 
  * @param[in] pixel_index is the index of the pixel to be set. 
  */
void
pb_image_mask_set_type_for_pixel (pb_image_mask_t* image_mask,
                                  pb_image_mask_type_t type,
                                  uint32_t pixel_index);

/** Tells if the specified pixel is of a certain type. */
int
pb_image_mask_pixel_is_type (const pb_image_mask_t* image_mask,
                             pb_image_mask_type_t type,
                             uint32_t pixel_index);

/** Tells if the specified pixel is background. */
int
pb_image_mask_pixel_is_background (const pb_image_mask_t* image_mask,
                                   uint32_t pixel_index);

/** Tells if the specified pixel is fingerprint. */
int
pb_image_mask_pixel_is_fingerprint (const pb_image_mask_t* image_mask,
                                    uint32_t pixel_index);

/** Returns a mask where all valid (non-backround and non-static-pattern) pixels are set to TRUE.
  *
  * @param[in] image_mask is the image mask.
  * @param[out] valid_mask is the returned valid mask. The valid_mask must have the same
  *             dimensions as the image_mask.
  */
pb_rc_t
pb_image_mask_get_valid_pixels (const pb_image_mask_t* image_mask,
                                uint8_t* valid_mask);

/** Returns a mask where all fingerprint (non-backround) pixels are set to TRUE.
  *
  * @param[in] image_mask is the image mask.
  * @param[out] fingerprint_mask is the returned fingerprint mask. The fingerprint_mask must have 
  *             the same dimensions as the image_mask.
  */
pb_rc_t
pb_image_mask_get_fingerprint_pixels (const pb_image_mask_t* image_mask,
                                      uint8_t* fingerprint_mask);

/** Computes the coverage for a specific type, in percent of the
  * total image area. */
int
pb_image_mask_type_coverage (const pb_image_mask_t* image_mask,
                             pb_image_mask_type_t type);

/** Computes the coverage for a specific type, in percent of the total active image area, 
  * i.e. the image area not set as PB_IMAGE_MASK_TYPE_SEGMENTATION_BACKGROUND_STATIC. */
int
pb_image_mask_type_coverage_of_active (const pb_image_mask_t* image_mask,
                                       pb_image_mask_type_t type);

/** Computes the valid coverage, i.e. how much that is not background or
  * static pattern, in percent of the total image area. */
int
pb_image_mask_valid_coverage (const pb_image_mask_t* image_mask);

/** Generates an elliptical shaped mask, defined by radius 
  * a (vertically) and b (horizontally) by setting everything 
  * outside the ellipse to 0. */
void
pb_image_mask_generate_elliptical (uint8_t* mask,
                                   uint16_t rows, uint16_t cols,
                                   uint16_t a, uint16_t b);

/** Returns a debug image (pixels) of the image mask. */
void
pb_image_mask_get_debug_image (const pb_image_mask_t* image_mask,
                               uint8_t* debug_pixels);

/** Encodes the image mask into a data buffer. */
size_t
lib_codec_encode_image_mask (uint8_t* data,
                             const pb_image_mask_t* mask);

/** Decodes an image mask from a data buffer. */
size_t
lib_codec_decode_image_mask (const uint8_t* data,
                             pb_image_mask_t** image_mask);

#ifdef __cplusplus
}
#endif

#endif /* PB_IMAGE_MASK_H */

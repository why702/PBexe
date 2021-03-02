/*
 * Copyright (c) 2008 - 2017, Precise Biometrics AB
 * All rights reserved.
 *
 */

#ifndef PB_ALIGNMENT_H
#define PB_ALIGNMENT_H

#include <stdint.h>

#include "pb_types.h"
#include "pb_errors.h"

#ifdef __cplusplus
extern "C" {
#endif

/** An alignment object describes translation and rotation alignment.
  * As such an alignment is used as part of fingerprint match 
  * operations. The alignment can be expressed as a matrix multitplication:
  *
  *     V = T*R*E
  *
  * Where E and V are coordinates. The relation can also be expressed as:
  *
  *     Vx = Ex * cos(angle) - Ey * sin(angle) + dx
  *     Vy = Ex * sin(angle) + Ey * cos(angle) + dy
  *
  * Another way to understand alignment is to, starting at origo or the top left
  * corner of V, move template E (dx, dy) pixels, then rotate E a number of degrees
  * equal to angle.
  */
typedef struct pb_alignment_st pb_alignment_t;

/** Allocates an alignment object and sets the reference counter to 1.
  *
  * @param dx is the translation in pixels along the x-axis, in 500dpi. 
  * @param dy is the translation in pixels along the y-axis, in 500dpi.
  * @param rotation is the rotation in binary radians [0,255]. 
  *
  * @return the allocated alignment object if successful, else 0.
  */
pb_alignment_t*
pb_alignment_create (int16_t dx, 
                     int16_t dy, 
                     uint8_t rotation);

/** Similar to pb_alignment_create but also takes a dpi argument as input to control
  * the dpi at which the alignment was measured. This simply scales the translation
  * part of the alignment.
  * 
  */
pb_alignment_t*
pb_alignment_create_ex (int16_t dx, 
                        int16_t dy, 
                        uint8_t rotation,
                        int16_t dpi);

/** Retains the object, which means that the reference counter for the object
  * will increase by 1. The caller must make sure that _delete is being called
  * when the object is not needed anymore. Retaining a null pointer has
  * no effect. 
  */
pb_alignment_t*
pb_alignment_retain (pb_alignment_t* alignment);

/** Decreases the reference counter for the object by 1. If the reference 
  * counter reaches 0 then the object will also be deleted. Deleting a null
  * pointer has no effect. 
  */
void
pb_alignment_delete (pb_alignment_t* alignment);

/** Creates a copy of the alignment object. If input alignment is null, a new
    empty alignment with unspecified values is created. */
pb_alignment_t* 
pb_alignment_copy (const pb_alignment_t* alignment);

/** 
 * Returns true if the two alignments are equal, measured at 500dpi. A NULL alignment
 * is not equal to another alignment, including another NULL alignment.
 */
int
pb_alignment_is_equal(const pb_alignment_t* alignment1, const pb_alignment_t* alignment2);

/** Returns the translation in pixels (x-wise) of the alignment, in 500dpi. */
int16_t
pb_alignment_get_dx (const pb_alignment_t* alignment);

/** Returns the translation  in pixels (y-wise) of the alignment, in 500dpi. */
int16_t
pb_alignment_get_dy (const pb_alignment_t* alignment);

/** Returns the rotation in binary radians [0,255]. */
uint8_t
pb_alignment_get_rotation (const pb_alignment_t* alignment);

/** Returns the translation and rotation of the alignment.
  *
  * Unwanted output can be skipped by providing a null pointer.
  *
  * Alignment data is created at 500 dpi by default.
  *
  */
void
pb_alignment_get (const pb_alignment_t* alignment, 
                  int16_t* dx, 
                  int16_t* dy, 
                  uint8_t* rotation,
                  int32_t dpi);

/** Combines two alignments into one.
  * 
  * Returns a new alignment given by the matrix A = A1 * A2.
  * 
  */
pb_alignment_t*
pb_alignment_multiply(const pb_alignment_t* A1,
                      const pb_alignment_t* A2);

/** Returns the inverse of alignment.
  * 
  * If I is the unit alignment (zero translation, zero rotation), then
  * this function returns A so that A * alignment = I.
  * 
  * Alignment is always invertible, so this function fails only if memory
  * can not be allocated.
  *
  */
pb_alignment_t*
pb_alignment_inverse(const pb_alignment_t* alignment);

/** Transforms the point (x,y) to a new point (x',y') using the affine 
  * transformation defined by the alignment.
  * 
  */
void
pb_alignment_transform_point (const pb_alignment_t* alignment,
                              int16_t x, int16_t y,
                              int16_t* x_prim, int16_t* y_prim);

/** Transforms the point (x,y) to a new point (x',y') using the inverse of the
  * affine transformation defined by the alignment.
  *
  * If x = A * x', this function returns x'.
  *
  */
void
pb_alignment_transform_point_inverse (const pb_alignment_t* alignment,
                                      int16_t x, int16_t y,
                                      int16_t* x_prim, int16_t* y_prim);

/** Combines two alignments into one.
  * 
  * Returns a new alignment given by the matrix A = A2 * A1.
  * 
  */
pb_alignment_t*
pb_alignment_combine (const pb_alignment_t* alignment1,
                      const pb_alignment_t* alignment2);

/** Assuming two templates V1 and V2 with alignments A1 and A2 both aligned
  * against the same template E:
  *     V1 = A1 * E
  *     V2 = A2 * E
  *
  * This function then computes the alignment A so that V1 = A * V2:
  *
  *           -1
  *     E = A1  * V1
  *           -1                =>
  *     E = A2  * V2
  *
  *       -1         -1
  *     A1  * V1 = A2  * V2     =>
  *
  *                 -1
  *     V1 = A1 * A2  * V2      =>
  *
  *                -1
  *     A = A1 * A2
  */
pb_alignment_t*
pb_alignment_difference (const pb_alignment_t* alignment1,
                         const pb_alignment_t* alignment2);

/** Assuming two alignments A1 and A2 of the same template this function returns
  * the distance between them. For each corner of the rectangle we calculate the
  * distance between them and then return the average of all four.
  *
  * Returns -1 if a distance is not feasible, e.g., if we have null pointers in.
  */
int16_t
pb_alignment_distance (const pb_alignment_t* alignment1,
                       const pb_alignment_t* alignment2,
                       int16_t rectangle_width, int16_t rectangle_height);

/**
 * Computes the overlap in percentage between two rectangles (templates/images) 
 * extracted from images with the specified width/height.
 * 
 * @param[in] alignment is the alignment
 * @param[in] image_height is the height of the image in pixels
 * @param[in] image_width is the width of the image in pixels
 * @param[in] dpi is the image resolution.
 * 
 * @returns the overlap, or -1 if an error occured. If there is no
 *          overlap, 0 is returned.
 */
int
pb_alignment_compute_overlap(const pb_alignment_t* alignment,
                             int image_height, 
                             int image_width,
                             int32_t dpi);

/** DEPRECATED
  * Compares two alignments. If they are within the limits then
  * the function returns 1, otherwise 0. 
  */
int 
pb_alignment_within_limits (const pb_alignment_t* alignment1,
                            const pb_alignment_t* alignment2,
                            uint16_t translation_limit,
                            uint8_t rotation_limit);

/** DEPRECATED
  * Checks an alignment to see if it is with the specified limits. If they are 
  * then the function returns 1, otherwise 0. 
  */
int 
pb_alignment_within_limits_single (const pb_alignment_t* alignment,
                                   uint16_t translation_limit,
                                   uint8_t rotation_limit);

/** DEPRECATED
  * Returns the transformed translation in pixels (x-wise) of the alignment, in 
  * 500dpi. The transformed translation dx' = dx * cos(-a) - dy * sin(-a). 
  */
int16_t
pb_alignment_get_dx_prim (const pb_alignment_t* alignment);

/** DEPRECATED
  * Returns the transformed translation in pixels (y-wise) of the alignment, in 
  * 500dpi. The transformed translation dy' = dx * sin(-a) + dy * cos(-a). 
  */
int16_t
pb_alignment_get_dy_prim (const pb_alignment_t* alignment);

/** DEPRECATED
  * Returns the extreme values for a rectangle positioned in (0, 0) and with a 
  * given size, transformed according to the alignment. 
  */
void
pb_alignment_get_rectangle_extremes (const pb_alignment_t* alignment,
                                     int16_t rectangle_width, int16_t rectangle_height,
                                     int16_t* min_x, int16_t* max_x,
                                     int16_t* min_y, int16_t* max_y);

/** Sets if one of the templates is inverted relative the other (1), or not (0). */
void
pb_alignment_set_polarity_inverted (pb_alignment_t* alignment,
                                    int polarity_inverted);

/** Tells if the two fingerprint images have different polarities, i.e. if one of the 
  * images are inverted relative the other (1), or not (0). */
int
pb_alignment_is_polarity_inverted (const pb_alignment_t* alignment);

/** Switches the polarity (i.e. if one of the images are inverted relative the other) of the alignment. */
void
pb_alignment_switch_polarity (pb_alignment_t* alignment);

#ifdef __cplusplus
}
#endif

#endif /* PB_ALIGNMENT_H */

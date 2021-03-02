/*
 * Copyright (c) 2008 - 2015, Precise Biometrics AB
 * All rights reserved.
 *
 */

#ifndef PB_IMAGE_H
#define PB_IMAGE_H

#include <stddef.h>
#include <stdint.h>

#include "pb_types.h"
#include "pb_errors.h"
#include "pb_image_t.h"
#include "pb_image_mask.h"
#include "pb_preprocessorI.h"
#include "pb_template.h"
#include "pb_finger.h"
#include "pb_session.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Note! It is the callers responsibility to make sure that 
 * all image input parameters are valid, i.e. not null (0). 
 * Passing a null image parameter to a function will result 
 * in a runtime error! */

/** Impression type codes. */
typedef enum pb_impression_type_e {
    PB_IMPRESSION_TYPE_LIVE_SCAN_PLAIN     = 0,
    PB_IMPRESSION_TYPE_LIVE_SCAN_ROLLED    = 1,
    PB_IMPRESSION_TYPE_NONLIVE_SCAN_PLAIN  = 2,
    PB_IMPRESSION_TYPE_NONLIVE_SCAN_ROLLED = 3,
    PB_IMPRESSION_TYPE_SWIPE               = 8,
    PB_IMPRESSION_TYPE_UNKNOWN             = 29
} pb_impression_type_t;

/** Pseudo image types for enrollment. */
typedef enum pb_pseudo_image_type_e {
    PB_PSEUDO_IMAGE_NONE   = 0,
    PB_PSEUDO_IMAGE_SCALED = 1,
    PB_PSEUDO_IMAGE_COLD   = 2
} pb_pseudo_image_type_t;

/** Allocates an image object, with gray-level 8-bit pixel values, and 
  * sets the reference counter to 1.
  *
  * @param[in] rows is the number of rows (height) of the image. 
  * @param[in] cols is the number of columns (width) of the image. 
  * @param[in] vertical_resolution is the vertical resolution of the
  *     image, in dpi. 
  * @param[in] horizontal_resolution is the horizontal resolution of 
  *     the image, in dpi. 
  * @param[in] pixels is the pixel buffer of the image. The pixels 
  *     are ordered so that pixels[r * cols + c] returns 
  *     the c:th pixel on the r:th row. The pixels have 
  *     values ranging from 0 (black) to 255 (white). If 0,
  *     then a completely white image is created.
  * @param[in] impression_type is the impression type of the image, 
  *     see PB_IMPRESSION_TYPE_X. 
  *
  * @return the allocated image object if successful, else 0.
  */
pb_image_t* 
pb_image_create (uint16_t rows,
                 uint16_t cols,
                 uint16_t vertical_resolution,
                 uint16_t horizontal_resolution,
                 const uint8_t* pixels,
                 pb_impression_type_t impression_type);


/** Allocates an image object, with gray-level 8-bit pixel values. This
  * create function also adds a mask to the image. 
  *
  * @param[in] rows is the number of rows (height) of the image. 
  * @param[in] cols is the number of columns (width) of the image. 
  * @param[in] vertical_resolution is the vertical resolution of the
  *     image, in dpi. 
  * @param[in] horizontal_resolution is the horizontal resolution of 
  *     the image, in dpi. 
  * @param[in] pixels is the pixel buffer of the image. The pixels 
  *     are ordered so that pixels[r * cols + c] returns 
  *     the c:th pixel on the r:th row. The pixels have 
  *     values ranging from 0 (black) to 255 (white). If 0,
  *     then a completely white image is created.
  * @param[in] impression_type is the impression type of the image, 
  *     see PB_IMPRESSION_TYPE_X. 
  * @param[in] mask is the image mask. The image mask will be retained
  *            by this function so it is ok to delete the mask after 
  *            the call. 
  *
  * @return the allocated image object if successful, else 0. 
  */
pb_image_t* 
pb_image_create_mask (uint16_t rows,
                      uint16_t cols,
                      uint16_t vertical_resolution,
                      uint16_t horizontal_resolution,
                      const uint8_t* pixels,
                      pb_impression_type_t impression_type,
                      pb_image_mask_t* mask);

/** Sets a mask to the current image. 
  * 
  * The image mask will be retained by this function so it is ok to delete 
  * the mask after the call. 
  */
void
pb_image_set_mask (pb_image_t *image, pb_image_mask_t* mask);

/** Set a segmentation to the image mask. If no image mask exists one 
  * will be created. 
  * 
  * @param[in] image is the image to set segmentation for. 
  * @param[in] segmentation is the segmentation where "0" indicates backround
  *            and ">0" indicates fingerprint (foreground). The segmentation
  *            must have the same size (rows, columns) as the image. 
  * @param[in] dynamic tells if the segmentation is dynamic (1) or static (0).
  *            A static segmentation will be kept forever, while a dynamic may
  *            be replaced during certain circumstances. 
  *
  * @return PB_RC_OK if successful, otherwise an error code.
  */
pb_rc_t
pb_image_set_segmentation (pb_image_t* image, 
                           uint8_t* segmentation,
                           int dynamic);

/** Intersect image mask with segmentation. If no image mask exists one 
  * will be created.
  * 
  * @param[in] image is the image to set segmentation for. 
  * @param[in] segmentation is the segmentation where "0" indicates backround
  *            and ">0" indicates fingerprint (foreground). The segmentation
  *            must have the same size (rows, columns) as the image.
  *            Fingerprint will be clear where segmentation indicates background.
  *            Fingerprint will be unchanged where segmentation indicates foreground.
  * @param[in] dynamic tells if the segmentation is dynamic (1) or static (0).
  *            A static segmentation will be kept forever, while a dynamic may
  *            be replaced during certain circumstances. 
  *
  * @return PB_RC_OK if successful, otherwise an error code.
  */
pb_rc_t
pb_image_intersect_segmentation (pb_image_t* image, 
                                 uint8_t* segmentation,
                                 int dynamic);

/** Computes the amount of background in the image mask, in percent of 
  * the total active image area, i.e. the image area not set as 
  * PB_IMAGE_MASK_TYPE_SEGMENTATION_BACKGROUND_STATIC. */
int
pb_image_background_coverage (const pb_image_t* image);

/** Computes the amount of static pattern in the image mask, in percent of 
  * the total active image area, i.e. the image area not set as
  * PB_IMAGE_MASK_TYPE_SEGMENTATION_BACKGROUND_STATIC. */
int
pb_image_static_pattern_coverage (const pb_image_t* image);

/** Computes the amount valid pixels, i.e. non-background and non-static pattern, 
  * in the image mask, in percent of the total image area. */
int
pb_image_valid_coverage (const pb_image_t* image);

/** Allocates an image object, with gray-level 8-bit
  * pixel values. The pixels will also be flipped according
  * to the flip parameter. This ensures that all images are
  * in an upright position, e.g. when comparing images from 
  * different sensors. An upright position is equal to the
  * orientation that a fingerprint would have when being
  * rolled on a paper (i.e. mirrored horizontally compared
  * with the physical fingerprint).
  *
  * @param[in] rows is the number of rows (height) of the image. 
  * @param[in] cols is the number of columns (width) of the image. 
  * @param[in] vertical_resolution is the vertical resolution of the
  *     image, in dpi. 
  * @param[in] horizontal_resolution is the horizontal resolution of 
  *     the image, in dpi. 
  * @param[in] pixels is the pixel buffer of the image. The image
  *     object creates a copy of the input pixels. The pixels are 
  *     ordered so that pixels[r * cols + c] returns the c:th pixel 
  *     on the r:th row. The pixels have values ranging from 0 
  *     (black) to 255 (white). If 0, then a completely white image 
  *     is created.
  * @param[in] impression_type is the impression type of the image, 
  *     see PB_IMPRESSION_TYPE_X. 
  * @param[in] flip tells how the input pixels shall be flipped to
  *     receive a fingerprint in an upright position, see PB_FLIP_X.
  *     Setting flip to PB_FLIP_NONE will give the same result as
  *     calling pb_image_create.
  *
  * @return the allocated image object if successful, else 0.
  */
pb_image_t* 
pb_image_create_flip (uint16_t rows,
                      uint16_t cols,
                      uint16_t vertical_resolution,
                      uint16_t horizontal_resolution,
                      const uint8_t* pixels,
                      pb_impression_type_t impression_type,
                      pb_flip_t flip);

/** Allocates an image object without allocating and copying pixel
  * data and instead use direct memory referencing, mr.  The
  * use-case for this alternative create function is in systems with
  * low RAM but comes with some additional complexity for an
  * application to consider such that the object and data may
  * be considered non-const despite of the declaration.
  * 
  * The pixel blob must remain valid and intact throughout the existence
  * of the image object up to its release of the pixels. A complicated
  * matter is that different parts an application may retain an object
  * for later use.
  *
  * In most simple applications there is however a bounded lifetime,
  * image data is captured, used to extract/verify and then operation
  * is ended. Hence the following scheme should be followed.
  *
  *   Capture_Sesnsor_ImageData();
  *   pb_image_t* image = pb_image_create_mr(192, 192, 500, 500,
  *                                          ImageData, _SCAN_PLAIN)
  *   extractor->extract_template(image, ....)
  *   assert(pb_image_get_num_refs(image) == 1));
  *   pb_image_detete(image); 
  *
  * See also the extended function pb_image_create_mre(), calling
  * the extended function with all extra parameters set to 0 is
  * equivalent. The extended function e.g. allows wrapping a pixel
  * blob allocated on the heap, this may in turn be usefull together
  * with the _mr_soft(), _mr_weak(), _mr_weakify() and mr_release()
  * functions.
  *
  * NOTE! The default property mr_const is 0 (false) which means
  *       that e.g. the pb_image_crop() function is allowed
  *       to retain the source image and use memory references instead
  *       of allocating a new pixel blob when the conditions allows it.
  *       If this behavior is not wanted use the _create_mre() function
  *       or change the property using _set_mr_const().
  *
  *       Also an exctractor that operates on a non-const image
  *       may release pixels early if they have been marked as weak
  *
  * @return the allocated image object if successful, else 0.
  */
pb_image_t* 
pb_image_create_mr (uint16_t rows,
                    uint16_t cols,
                    uint16_t vertical_resolution,
                    uint16_t horizontal_resolution,
                    const uint8_t* pixels,
                    pb_impression_type_t impression_type);

/** Extended version of the _create_mr() function with additional
  * functionality. Calling this function will all extra parameters
  * set to 0 is equivalent of using the _create_mr() function.
  *
  * The extended function adds a callback release function which
  * purpose is to allows using another another BMF object or heap
  * allocated memory as data source which in turn will be deleted
  * when the image object is deleted. This factious example shows
  * the idea.
  *
  *   image = SomeImage();
  *   pb_image_retain(image); // Protect from beein deleted
  *   pixels = pb_image_get_pixels(image); // Internal data
  *   slice = pb_image_create_mre(
  *               pb_image_get_rows(image),
  *               pb_image_get_cols(image),
  *               500, 500, pixels, _TYPE_UNKNOWN, 0, 0,
  *               (pb_memref_release_fn_t*) pb_image_delete, image);
  *   pb_image_delete(image); // safe as previously retained.
  *   DoOperation(slice);
  *   pb_image_delete(slice); // Will also cascade delete image
  *
  * The same mechanism can be used to prevent double allocation
  * when using heap memory that should be freed:
  *
  *   pixels = malloc(rows*cols);
  *   CaptureImage(pixels);
  *   image = pb_image_create_mre(
  *               rows, cols, 500, 500,
  *               pixels, _TYPE_UNKNOWN, 0, 0,
  *               free, pixels);
  *   pb_image_delete(image); // will free(pixels) also
  *
  * This also hooks into the early release of pixels which then may
  * lower total RAM requirements. To make this work the release
  * function must be the heap manager free() function just as in the
  * previous example.
  *
  * Note that a non-const image still must keep pixel data valid
  * (not modified) with the exception of the early release semantics.
  *
  * @param[in] rows is the number of rows (height) of the image. 
  * @param[in] cols is the number of columns (width) of the image. 
  * @param[in] vertical_resolution is the vertical resolution of the
  *                    image, in dpi. 
  * @param[in] horizontal_resolution is the horizontal resolution of 
  *                    the image, in dpi. 
  * @param[in] pixels is the pixel buffer of the image. The image
  *                    object creates a copy of the input pixels. The pixels are 
  *                    ordered so that pixels[r * cols + c] returns the c:th pixel 
  *                    on the r:th row. The pixels have values ranging from 0 
  *                    (black) to 255 (white). If 0, then a completely white image 
  *                    is created.
  * @param[in] impression_type is the impression type of the image, 
  *                    see PB_IMPRESSION_TYPE_X. 
  * @param[in] mr_size If not 0 this specifies the actual memory size
  *                    of the pixel buffer and may be larger than
  *                    the size of the pixel blob. When it is larger
  *                    an implementation may take advantage of this
  *                    if the pixel data is also not write protected.
  * @param[in] mr_const If 0 code may cast away const to the image object
  *                    itself e.g. to retain it or make an early release
  *                    release of pixels, yet pixels themself must not
  *                    be changed. 
  * @param[in] mr_release Optional release callback, called when the
  *                    last reference to the image object is deleted.
  *                    On application level it is better to follow
  *                    the scheme described earlier by checking
  *                    the reference counter value. For library
  *                    development it may used to build a chain of
  *                    BMF objects.
  * @param[in] mr_release_obj Callback value passed to the optional
  *                    release function. This may e.g. be same as pixels,
  *                    but may point to a different object related to
  *                    the release of pixels.
  * 
  * @return the allocated image object if successful, else 0.
  */
pb_image_t* 
pb_image_create_mre (uint16_t rows,
                     uint16_t cols,
                     uint16_t vertical_resolution,
                     uint16_t horizontal_resolution,
                     const uint8_t* pixels,
                     pb_impression_type_t impression_type,
                     uint32_t mr_size,
                     uint32_t mr_const,
                     pb_memref_release_fn_t* mr_release,
                     void*    mr_release_obj);

/** Retains the object, which means that the reference counter for the object
  * will increase by 1. The caller must make sure that _delete is being called
  * when the object is not needed anymore. Retaining a null pointer has
  * no effect. */
pb_image_t*
pb_image_retain (pb_image_t* image);

/** Decreases the reference counter for the object by 1. If the reference 
  * counter reaches 0 then the object will also be deleted. Deleting a null
  * pointer has no effect. */
void
pb_image_delete (pb_image_t* image);

/** Returns the number of references to this object. */
uint32_t pb_image_get_num_refs (const pb_image_t* image);

/** Creates a copy of the image.
 * Note that copying consumes far more resources than just retaining the
 * object. When copying a renderer backed image object a copy does
 * not clone the renderer but instead triggers a complete rendering of the
 * pixel blob.
 */
pb_image_t* 
pb_image_copy (const pb_image_t* image);

/** Returns the number of rows (height) of the image. */
uint16_t
pb_image_get_rows (const pb_image_t* image);

/** Returns the number of columns (width) of the image. */
uint16_t
pb_image_get_cols (const pb_image_t* image);

/** Returns the total number of pixels (rows * cols) of 
  * the image. */
uint32_t
pb_image_get_nbr_of_pixels (const pb_image_t* image);

/** Returns the vertical resolution of the image, in dpi. */
uint16_t
pb_image_get_vertical_resolution (const pb_image_t* image);

/** Returns the horizontal resolution of the image, in dpi. */
uint16_t
pb_image_get_horizontal_resolution (const pb_image_t* image);

/** Returns the resolution of the image, in dpi.
 *
 * If vertical and horizontal resolution is different or the function
 * otherwise fails to run, 0 is returned.
 */
uint16_t
pb_image_get_resolution (const pb_image_t* image);

/** Changes the resolution of the image, in dpi.
 *
 * If vertical and horizontal resolution is different or the function
 * otherwise fails to run, 0 is returned.
 */
void
pb_image_set_resolution (pb_image_t* image,
                         uint16_t resolution);

/** Returns the impression type of the image, see 
  * PB_IMPRESSION_TYPE_X. */
pb_impression_type_t
pb_image_get_impression_type (const pb_image_t* image);

/** Returns a reference to the pixels of the image.
  * In case of a weak image object 0 may be returned if the
  * pixels has been released. */
const uint8_t*
pb_image_get_pixels (const pb_image_t* image);

/** Retrieves the mask for this image.
  *
  * Not all images have a computed mask, and in that case, NULL is returned.
  */
pb_image_mask_t*
pb_image_get_mask (const pb_image_t* image);

/** Retrieves the mask for this image. If no mask exists for the image a new one
  * will be created and returned. */
pb_image_mask_t*
pb_image_get_mask_create_if_na (pb_image_t* image);

/** Retrieves all valid pixels for the image.
  *
  * Underlying functionality uses the image mask. If not set or defined, the
  * map of valid pixels will still return filled with 255 (max validity).
  * 
  * Caller is responsible for deallocating the returned surface.
  * Returned surface has the same dimensions as the image.
  */
pb_rc_t
pb_image_get_valid_pixels (const pb_image_t* image,
                           uint8_t** valid_pixels);

/** Flags the image as being similar to the previous captured image, e.g. if 
  * two images are captured within a short time frame in which the user has not 
  * moved the finger. This will improve e.g. the static pattern detection which 
  * otherwise could have a hard time to distinguish between two similar images 
  * and true static patterns. 
  * Use this functionality to mark images if the verification process sometimes
  * captures more than one image, e.g. if the verification process involves a 
  * first quick capture and if this first image doesn't match, another capture
  * is performed. */
void
pb_image_set_similar_image (pb_image_t* image,
                            int similar);
/** Tells if the image is similar to another image being captured very recently. */
int
pb_image_get_similar_image (const pb_image_t* image);

/** Flags the image as being valid for update (1) or not (0). An image marked 
  * as not valid for update will not be used for dynamic update of the enrolled
  * template. */
void
pb_image_set_valid_for_update (pb_image_t* image,
                               int valid_for_update);
int
pb_image_get_valid_for_update (const pb_image_t* image);

/** Scales an image to the specified resolution. 
  * 
  * @param[in] image is the original image.
  * @param[in] vertical_resolution is the new vertical resolution
  *     of the scaled image.
  * @param[in] horizontal_resolution is the new horizontal resolution
  *     of the scaled image.
  *
  * @return a new scaled image.
  */
pb_image_t*
pb_image_scale_res (const pb_image_t* image, 
                    uint16_t vertical_resolution,
                    uint16_t horizontal_resolution);

/** Scales an image to the specified size. 
  * 
  * @param[in] image is the original image.
  * @param[in] rows is the new number of rows (height) of the 
  *     scaled image.
  * @param[in] cols is the new number of columns (width) of the 
  *     scaled image.
  *
  * @return a new scaled image.
  */
pb_image_t*
pb_image_scale_size (const pb_image_t* image, 
                     uint16_t rows,
                     uint16_t cols);

/** Like pb_image_scale_size but sets the given resolution on
  * the scaled image.
  * 
  * @param[in] image is the original image.
  * @param[in] rows is the new number of rows (height) of the 
  *     scaled image.
  * @param[in] cols is the new number of columns (width) of the 
  *     scaled image.
  * @param[in] vres is the vertical resolution to encode.
  * @param[in] hres is the horizontal resolution to encode.
  *
  * @return a new scaled image.
  */
pb_image_t*
pb_image_scale_size_res (const pb_image_t* image,
                         uint16_t rows,
                         uint16_t cols,
                         uint16_t vres,
                         uint16_t hres);

/** Crops an image to the specified size and offset.  It is possible
  * to crop to a larger image or set the crop offset in such a way
  * that the original image does not cover all parts of the cropped
  * image. In those cases, the uncovered parts will get pixel values
  * of 255 (white).
  *
  * If the mr_const property of the image is 0 (false) and the cropped
  * image lies inside image and same number of cols a mr-image is
  * created without allocating a copy of the pixels and the source
  * image will be retained until safe to be deleted. Such created
  * image slice will have property mr_const 1 (true).
  *
  * The cropped image is automatically related to the original image.
  *
  * @param image is the original image.
  * @param[in] rows is the new number of rows (height) of the 
  *     cropped image.
  * @param[in] cols is the new number of columns (width) of the 
  *     cropped image.
  * @param[in] offset_rows is the row offset for the crop.
  * @param[in] offset_cols is the column offset for the crop.
  *
  * @return a new cropped image.
  */
pb_image_t*
pb_image_crop (const pb_image_t* image, 
               uint16_t rows,
               uint16_t cols,
               int32_t offset_rows,
               int32_t offset_cols);

/** Crops an image to the specified size, centered.
  * It is possible to crop to a larger image. In that 
  * case, the extended parts will get pixel values 
  * of 255 (white). 
  *
  * The cropped image is automatically related to the original image.
  * 
  * @param[in] image is the original image.
  * @param[in] rows is the new number of rows (height) of the 
  *     cropped image.
  * @param[in] cols is the new number of columns (width) of the 
  *     cropped image.
  *
  * @return a new cropped image.
  */
pb_image_t*
pb_image_crop_centered (const pb_image_t* image, 
                        uint16_t rows,
                        uint16_t cols);

/** As above, but returns the actual crop offset used. 
  *
  * @param[out] offset_rows the returned offset rows of the crop. 
  *             May be set to 0 if not needed.
  * @param[out] offset_cols the returned offset columns of the crop. 
  *             May be set to 0 if not needed.
  */
pb_image_t*
pb_image_crop_centered_ex (const pb_image_t* image, 
                           uint16_t rows,
                           uint16_t cols,
                           int16_t* offset_rows,
                           int16_t* offset_cols);

/** As pb_image_crop_centered but the center is computed as the center of 
  * the fingerprint in the image. Uses local variance to determine where the 
  * fingerprint is located. 
  *
  * @param[in] variance_threshold is the threshold to be used to evaluate if the
  *            computed local variance is seen as fingerprint (> threshold) or 
  *            background (< threshold). 
  * @param[out] offset_rows the returned offset rows of the crop. 
  *             May be set to 0 if not needed.
  * @param[out] offset_cols the returned offset columns of the crop. 
  *             May be set to 0 if not needed.
  */
pb_image_t* 
pb_image_crop_centered_on_finger (const pb_image_t* image,
                                  uint16_t rows, 
                                  uint16_t cols,
                                  int variance_threshold,
                                  int16_t* offset_rows,
                                  int16_t* offset_cols);

/* Expands the image to larger size. The expanded pixels will be filled with an 
 * average of the neighbourhood. */
pb_image_t*
pb_image_expand (const pb_image_t* image, 
                 uint16_t rows,
                 uint16_t cols);

/** Struct for defining splitting of an image. */
typedef struct pb_image_split_st {
    /** The number of rows/columns to initially crop the image to, centered on the fingerprint. */
    int finger_crop_rows;
    int finger_crop_cols;

    /** The variance threshold used to segment image into background and fingerprint. */
    int variance_threshold;

    /** The number of rows/columns to split the original image into. */
    int split_rows;
    int split_cols;

    /** The number of rows/columns for each sub-image, in the resolution of the original image. */
    int sub_image_rows;
    int sub_image_cols;

    /** Tells if to include an additional center cropped sub-image. */
    int include_center_cropped;

} pb_image_split_t;

/** Computes the number of sub-images from a defined image split. */
int
pb_image_split_get_nbr_of_sub_images (const pb_image_split_t* image_split);

/** Tells if a split of the image split is applicable, given the specified split settings
  * and a maximum overlap (in percentage). The maximum overlap may be used to avoid splitting
  * up images into too similar parts with too much overlap. */
int
pb_image_split_is_applicable (const pb_image_t* image, 
                              const pb_image_split_t* image_split,
                              uint8_t max_overlap);

/**
 * Splits an image into evenly distributed sub-images. 
 *
 * Example of splitting an image of size W into x (=3) sub-images with size Ws, where x * Ws >= W (in one dimension).
 *   |---------------------------- W -----------------------------| original image rows/cols
 *   |---------- Ws ----------|          |- O --|                   sub-image 1
 *                     |---------- Ws ----------|                   sub-image 2 
 *                     |- O --|          |---------- Ws ----------| sub-image 3
 * , where O = (x * Ws - W) / (x - 1)
 *
 * In case that the image is larger than the sub-images together the sub-images will have no overlap
 * and will cover the center of the image.
 *
 * Example of splitting an image of size W into x (=3) sub-images with size Ws, where x * Ws < W (in one dimension).
 *   |---------------------------- W -----------------------------| original image rows/cols
 *        |------ Ws ------|                                        sub-image 1
 *                         |------ Ws ------|                       sub-image 2 
 *                                          |------ Ws ------|      sub-image 3
 *
 * The sub images are automatically related to the larger image.
 *
 * @param[in] image is the original image.
 * @param[in] split tells how the image shall be splitted.
 * @param[out] sub_images is the returned sub-images. The caller is responsible
 *             for deallocating the returned sub-images. 
 *
 * @return PB_RC_OK if successful, otherwise an error code.
 */
pb_rc_t
pb_image_split(const pb_image_t* image,
               const pb_image_split_t* split,
               pb_image_t* sub_images[]);

/**
 * Relates a cropped sub image to the full original image/sensor.
 * Note: pb_image_crop(_x) and pb_image_split will automatically relate the cropped sub images.
 * 
 * @param[in/out] sub_image is the sub image to relate to the full image. 
 * @param[in] sub_offset_rows is the offset rows for the cropped sub image within the full image.
 * @param[in] sub_offset_cols is the offset columns for the cropped sub image within the full image.
 * @param[in] full_rows is the number of rows in the full image. 
 * @param[in] full_cols is the number of columns in the full image. 
 *
 * @return PB_RC_OK if successful, otherwise an error code.
 */
pb_rc_t 
pb_image_relate(pb_image_t* sub_image, 
                int16_t sub_offset_rows, int16_t sub_offset_cols,
                uint16_t full_rows, uint16_t full_cols);

/** Unrelates a sub image from the full original image/sensor.
  * 
  * @param[in/out] sub_image is the sub image to unrelate from the full image. 
  *
  * @return PB_RC_OK if successful, otherwise an error code.
  */
void 
pb_image_unrelate (pb_image_t* sub_image);

/** Returns the relation between the sub image and the full image. */
const pb_image_relation_t*
pb_image_get_relation (const pb_image_t* sub_image);

/** Returns the full image size. Returns the actual image size if no relation exists. */
uint16_t
pb_image_get_full_rows (const pb_image_t* sub_image);
uint16_t
pb_image_get_full_cols (const pb_image_t* sub_image);

/** Returns the offset of the sub image relative the full image. Returns 0 if no relation exists. */
int16_t
pb_image_get_offset_rows (const pb_image_t* sub_image);
int16_t
pb_image_get_offset_cols (const pb_image_t* sub_image);

/** Associates the image with a finger. */
void
pb_image_set_finger (pb_image_t* image,
                     pb_finger_t* finger);

/* Returns the finger associated with this image. Returns PB_FINGER_ANONYMOUS if 
 * no finger has been associated. */
pb_finger_t*
pb_image_get_finger (const pb_image_t* image);

/** Flips the image according to the flip parameter. 
  *
  * @param[in] image is the image to flip. 
  * @param[in] flip tells how the input pixels shall be flipped to
  *     receive a fingerprint in an upright position, see PB_FLIP_X.
  *     Setting flip to PB_FLIP_NONE will give the same result as
  *     calling pb_image_create.
  *
  * @return the flipped image object if successful, else 0. The caller is 
  *         responsible of deleting the flipped image. 
  */
pb_image_t*
pb_image_flip (const pb_image_t* image, 
               pb_flip_t flip);

/** Blends an image with another image by taking the average between the two 
  * images. 
  * 
  * Pixels with values equal to 255 are considered background and will not be 
  * blended but instead replaced with the pixel value of the other image. If 
  * all pixels shall be blended, make sure that to set any 255 values to 254 
  * before calling this function. 
  *
  * @param[in] image1 is the original image.
  * @param[in] image2 is the image to blend with the original image.
  * @param[in] offset_x is the horizontal offset of image2 within image1.
  * @param[in] offset_y is the vertical offset of image2 within image1.
  *
  * @return the blended image object if successful, else 0. The size of the
  *         blended image will be equal to the size of image1. The caller is 
  *         responsible of deleting the blended image.
  */
pb_image_t*
pb_image_blend (const pb_image_t* image1, 
                const pb_image_t* image2, 
                int32_t offset_x, 
                int32_t offset_y);

/** Draws the image2 on top of image1.
  *
  * Pixels with values equal to 255 are considered background and will not be
  * drawn. If all pixels shall be drawn, use pb_image_draw_ignore_255_mask().
  *
  * @param[in] image1 is the original image.
  * @param[in] image2 is the image to draw on top of the original image.
  * @param[in] offset_x is the horizontal offset of image2 within image1.
  * @param[in] offset_y is the vertical offset of image2 within image1.
  *
  * @return the new image object if successful, else 0. The size of the
  *         new image will be equal to the size of image1. The caller is
  *         responsible of deleting the new image.
  */
pb_image_t*
pb_image_draw (const pb_image_t* image1, 
               const pb_image_t* image2, 
               int32_t offset_x, 
               int32_t offset_y);

/** Rotates an image around it's origo, counter-clockwise. The new rotated image
  * will likely be larger and will receive a new origo.
  *
  * @param[in] image is the image to be rotated.
  * @param[in] rotation is the rotation of the image around it's origo, 
  *            counter-clockwise.
  * @param[out] new_origo_x is the returend x-coordinate of the new origo.
  * @param[out] new_origo_y is the returend y-coordinate of the new origo.
  *
  * @return the rotated image object if successful, else 0. The caller is 
  *         responsible of deleting the rotated image.
  */
pb_image_t* 
pb_image_rotate (const pb_image_t* image, 
                 uint8_t rotation,
                 int32_t* new_origo_x,
                 int32_t* new_origo_y);

/** Inverts an image, i.e. for each pixel changing its graylevel xi to 255 - xi.
  *
  * @param[in] image is the image to be inverted.
  *
  * @return the inverted image. The caller is responsible of deleting the 
  *         inverted image.
  */
pb_image_t* 
pb_image_invert (const pb_image_t* image);

/** Transforms an image to a power series on radius:
  * 
  * R = ( a * r^3  +  b * r^2  +  c * r  +  d ) * r  , where d = 1-a-b-c
  * 
  * resulting in e.g. Barrel, Pincushion or Mustache distortion
  * 
  * @param[in] image is the image to be transformed
  * @param[in] a is the r^3 coefficient
  * @param[in] b is the r^2 coefficient
  * @param[in] c is the r^1 coefficient
  * @param[out] out_corrected is the corrected image
  * 
  * @return the status
  */
pb_rc_t
pb_image_lens_correction (const pb_image_t *image, float a, float b, float c, pb_image_t** out_corrected);

/** Sets a certain part of the image to white (255) to simulate low fingerprint coverage. 
  * This function is mainly aimed for evaluating the effect low fingerprint coverage for
  * different algorithms. 
  *
  * @param[in] image is the input image.
  * @param[in] part is the amount of the image to set to white, in percentage (0-100).
  * 
  * @return a new image with a certain part of the image set to white. The caller is 
  *         responsible for deleting the returned image.
  */
pb_image_t*
pb_image_set_part_to_white (const pb_image_t* image, 
                            uint8_t part);

/** Enhances the quality of the image, by trying to increase the contrast and 
  * remove noise. The enhanced image may be used for visualization of the
  * image for the user. 
  * Note: The enhanced image should not be used for further image processing, 
  * e.g. template extraction, since vital information may be removed during
  * the enhancement. */
pb_image_t*
pb_image_enhance_quality (const pb_image_t* image);

/** 
   Generate the a mask for the fingerprint optimized for being used in 
   memory and CPU constained environments
  * @param[in] image is to mbe used.
  *
  * @return PB_RC_OK if successful, otherwise an error code.
 **/
pb_rc_t
pb_image_embedded_mask(pb_image_t* image);

/** Enhances the quality of the image, by trying to increase the contrast and 
  * remove noise. The enhanced image may be used for visualization of the
  * image for the user. 
  *
  * This function is a more advanced version of the pb_image_enhance_quality
  * function. It is much slower but produces a much more noise reduced image.
  *
  * It is strongly recommended that the rows and columns of the images used are
  * multiples of 8!
  *
  * Note: The enhanced image should not be used for further image processing, 
  * e.g. template extraction, since vital information may be removed during
  * the enhancement. */
pb_image_t*
pb_image_enhance_quality_advanced (const pb_image_t* image);

/** Enhances the image and reduces noise by the use of a preprocessor implementation.
  *
  * @param[in] image is the image to be enhanced. 
  * @param[in] session is the session object.
  * @param[in] preprocessor is the specified preprocessor. 
  *
  * @return a new enhanced image. 
  */
pb_image_t*
pb_image_enhance_by_preprocessor (const pb_image_t* image, 
                                  pb_session_t* session,
                                  const pb_preprocessorI* preprocessor);

/** Encodes the image, without mask information, into a template. Note that any
  * mask information will be lost by this operation.
  *
  * @param[in] image is the image to be encode.
  * @param[out] template_ is the encoded template.
  *
  * @return PB_RC_OK if successful, otherwise an error code.
  */
pb_rc_t
pb_image_encode (const pb_image_t* image,
                 pb_template_t** template_);

/**
 * Decodes the image from the template. Note that mask information is not encoded,
 * and therefore not decoded.
 * 
 * @param[in] template_ is the template which contains the image.
 * @param[out] image the newly decoded image.
 * 
 * @return PB_RC_OK if successful, otherwise an error code
 */
pb_rc_t
pb_image_decode (const pb_template_t* template_,
                 pb_image_t** image);

/** Returns a previously computed PAD score of the 
  * fingerprint image. If no value has been calculated
  * PB_PAD_SCORE_UNDEFINED is returned. The value can 
  * be cross-referenced with the PAD-model threshold 
  * table to determine whether the fingerprint image comes 
  * from a live or a spoof finger. Note that the cached 
  * score value is not automatically updated if the PAD 
  * model is changed at run-time. */
int16_t
pb_image_get_liveness_score(const pb_image_t* image);

/** Sets the liveness score value for the image. This value
  * can be retrieved with pb_image_get_liveness_score() */
void 
pb_image_set_liveness_score(pb_image_t* image,
                            int16_t score);

/** Image quality values. */

/** The value returned when a quality value could not be computed. */
#define PB_IMAGE_QUALITY_UNKNOWN    -1

/** Returns the fingerprint quality of the image, computed as the ridge
  * direction coherence of the image, ranging from 0 (worst quality) to 
  * 100 (best quality). 
  * This value indicates how clear and continous the ridges of the 
  * fingerprint are. 
  * Note: The fingerprint quality will be low for fingerprints with e.g. 
  * a lot of scars or for non-continous ridges. However, such fingerprints 
  * could still be very unique and easily recognisable. */
int
pb_image_get_fingerprint_quality (pb_image_t* image);

/** Returns the area of the fingerprint in the image, in percentage
  * of the total image area. */
int 
pb_image_get_fingerprint_area_percentage (pb_image_t* image);

/** Returns the area of the fingerprint in the image, in mm^2. */
int
pb_image_get_fingerprint_area (pb_image_t* image);

/** Returns the condition of the fingerprint in the image, computed as 
  * the amount of dark pixels compared to the amount of bright pixels, 
  * ranging from 0 (soaked) to 100 (bone dry). */
int
pb_image_get_fingerprint_condition (pb_image_t* image);

/** Returns the signal-to-noise ratio (SNR) of the image. The signal is 
  * computed as the grey level intensity difference between ridges and 
  * valleys and the noise as the standard deviation of the gaussian noise. 
  * 
  * @param[in] image is the image.
  * @param[out] signal is the computed fingerprint signal in the image.
  *             May be set to 0 if not needed.
  * @param[out] noise is the computed noise in the image. May be set to
  *             0 if not needed. 
  *
  * @return the SNR leftshifted 3 steps for improved resolution if 
  *         successful, otherwise PB_IMAGE_QUALITY_UNKNOWN. */
int
pb_image_get_signal_to_noise_ratio_3 (pb_image_t* image,
                                      int* signal,
                                      int* noise);

/** Returns the dynamic range of the image, computed as the difference 
  * between the maximum intensity and the minimum intensity, while 
  * ignoring the 1% highest and lowest pixel values. */
int
pb_image_get_dynamic_range (pb_image_t* image);

/** Returns the sharpness of the image, computed as the standard deviation 
  * of the laplacian of the image. The value is leftshifted 2 steps for 
  * improved resolution. */
int
pb_image_get_sharpness_2 (pb_image_t* image);

/** Computes the difference between two images, as the percentage of pixels with 
  * an intensity difference (between the two images) higher than the specified 
  * threshold. 
  *
  * @param[in] image1 is the first image.
  * @param[in] image2 is the second image.
  * @param[in] intensity_difference_threshold is the threshold when comparing
  *            the intensity of the corresponding pixels. 
  * @param[in] reduce_noise_before_comparing tells if noise reduction should be 
  *            performed prior to the comparison. Enabling this increases latency
  *            but improves the result for noisy images. 
  * @param[out] difference is the returned difference between the two images. 
  *
  * @return PB_RC_OK if successful, otherwise an error code. 
  */
pb_rc_t
pb_image_difference (const pb_image_t* image1,
                     const pb_image_t* image2,
                     uint8_t intensity_difference_threshold,
                     int reduce_noise_before_comparing,
                     uint8_t* difference);

/** Computes the dominant frequency component of the fingerprint image.
  *
  * @param[in] image is the image.
  * @param[out] dominant frequency of image.
  */
pb_rc_t
pb_image_get_dominant_frequency(const pb_image_t *image, uint32_t *frequency);


/** Generate a pseudo image based on the given image.
  *
  * @param[in] image is the image.
  * @param[out] pseudo_image - the generated pseudo image.
  * @param[in] pseudo_type - type of pseudo image to generate.
  * @param[in] pseudo_param - the tunable parameter for pseudo image generation.
  */
pb_rc_t
pb_image_generate_pseudo_image(const pb_image_t *image, 
                               pb_image_t **pseudo_image,
                               pb_pseudo_image_type_t pseudo_type,
                               int pseudo_param);

/** Converts an image width and height to the best corresponding sensor size. */
pb_sensor_size_t
pb_image_size_to_sensor_size (uint16_t image_rows, 
                              uint16_t image_cols,
                              uint16_t vertical_resolution,
                              uint16_t horizontal_resolution);

/** Computes the corresponding sensor size. */
pb_sensor_size_t
pb_image_get_sensor_size (const pb_image_t* image);

/** Returns the size of the memory-reference mr_size attribute or
 * if 0 the actual size of the pixel blob (rows*cols). */
uint32_t pb_image_get_mr_size(const pb_image_t* image);

/** Returns the state of the memory-reference mr_const attribute.
 * If 0 then the image object itself and in particular the pixel
 * blob may be treated as writable. */
uint32_t pb_image_get_mr_const(const pb_image_t* image);

/** Change the state of  memory-reference mr_const attribute.
  * If 0 then the image object itself and in particular the pixel
  * blob may be treated as writable. */
void pb_image_set_mr_const(pb_image_t* image, uint32_t mr_const);

/** Makes a hard image pixel reference soft, if not marked as hard this
  * function has no effect. A soft reference has in it self no effect
  * but can be weakened via the _mr_weakify() function. The design
  * pattern of using soft reference is to enable a controller to
  * release pixels early while not disturbing those controllers who ar
  * unaware of this pattern.
  */
void pb_image_mr_soft(pb_image_t* image);

/** Makes a soft pixel reference weak, if not marked as soft this
  * function has no effect. When pixels are made weak it is like
  * calling _mr_weak().  The design pattern for this function is to be
  * used in aware controllers that can release pixels early yet it
  * depends on the caller application to have enabled it by making
  * pixles soft (since main application may want the pixels to
  * remain).
  */
void pb_image_mr_weakify(pb_image_t* image);

/** Makes pixel references weak, unconditionally. Weakly referenced
  * pixels may be released at earliest convenience in the code that
  * follows. The _mr_release() function can only succeed when the
  * pixel reference is in this state but there are more criterias.
  * The design pattern is that only a few main components make use of
  * early release such as an extractor which then may have a chance of
  * lowering total RAM usage. Functions that do early release should
  * describe this.
  */
void pb_image_mr_weak(pb_image_t* image);

/** Check if image has a weak pixel reference. */
int pb_image_is_mr_weak(const pb_image_t* image);

/** Release the image pixels, if possible. To be released the pixels
  * must be weakly referenced but this is not the only criteria, The
  * image object itself will remain and must still be deleted but the
  * image pixel reference will become 0 if the object was actually released.
  *
  * The intended usage is is from inside an extractor where the image
  * pixels could be reused or released early. The caller must specify
  * the weak attribute of the image just prior to calling the
  * extractor and then delete the image object right after. See
  * the _mr_soft(), _mr_weakify() and _mr_weak() functions.
  *
  * This function may also request taking ownership of the pixels by
  * specify the intended release function, if ownership is granted the
  * pixels will be seen as released from the image object point of
  * view while the caller will be responsible to release the pixels
  * eventually. Getting ownership of pixels also grants the right to
  * modify the pixels before they are finally released. So if e.g. the
  * pixels has been allocated with malloc() then the free() function
  * would be used to get ownership - this is also the intended major
  * use-case.
  *
  * As a consequence of taking ownership it requires the caller to
  * first request and store the image pixels pointer.
  *
  * This function will not release unless the image != 0, image has
  * already been released (pixels==0), the image is not maked as weak,
  * the reference counter is > 1 or there is no release function
  * (possible e.g. when using create_mr()).
  *
  * In addition no ownership transfer is make if the release function
  * is not compatible or the image is renderer backed.
  *
  * The function will return
  *
  *     0 - If nothing was actually released or ownership transfer
  *         This may happen for a number of cases such as if there
  *         are several retains on the image, there is no registered
  *         release function, active renderer backed image or if
  *         an ownership take-over request is not granted.
  *
  *     1 - Pixels where released by some means or ownership transfer
  *         In the case of ownership request the pixels are now
  *         owned by the caller.
  *         If not ownership was requested it means that something was
  *         released but not necessarily finally back to the system
  *         heap as there may be a chain of releases that is not yet
  *         fully unraveled.
  *
  * NOTE: In cases where the image object is a const image this
  *       function can still be used if the object in turn has been
  *       marked as non-const using _set_mr_const() or similar which
  *       allows breaking const for this purpose.
  *
  * @param image   The image to release pixels for.
  * @param release The release function if requesting ownership transfer,
  *                else 0 should be specified.
  *
  * @returns as described above 0 is returned if nothing was released
  *          and otherwise 1 is returned.
  */
int
pb_image_mr_release (pb_image_t* image, pb_memref_release_fn_t* release);
/** Enhances the image and reduces noise by the use of a preprocessor implementation.
  * Used when a limited amount of memory is available.
  *
  * @param[in/out] image is the image to be enhanced. 
  * @param[in] session is the session object.
  * @param[in] preprocessors is the specified preprocessors. 
  *
  * @return PB_RC_OK if successful, otherwise an error code.
*/
pb_rc_t
pb_image_enhance_by_embedded_preprocessor(pb_image_t* image,
                                          pb_session_t* session,
                                          const pb_preprocessorI* const* preprocessors);

/** Image class functionality. */

/** An image can be assigned a class which can then be used e.g. for deciding the minimum
  * number of images for a specific class that the multimplate will contain, see 
  * pb_algorithm_set_min_templates_in_class. The class of the image will be copied to any
  * template extracted from that image. 
  *
  * There is currently a restriction of only using class id 0-31. This restriction will be
  * removed in the future allowing the full 256 class ids. */

/** The maximum number of classes supported, i.e. class id 0-31 is allowed. */
#define PB_IMAGE_MAX_NBR_OF_CLASSES 32

/** The default image class. Will be set for any image not receiving a specific class. */
#define PB_IMAGE_DEFAULT_CLASS 0

/** Sets the class id for an image.
  *
  * @return PB_RC_OK if successful, otherwise an error code.
  */
pb_rc_t
pb_image_set_class_id (pb_image_t* image, 
                       uint8_t class_id);

/** Returns the class id of the image. */
uint8_t
pb_image_get_class_id (const pb_image_t* image);

/** Encodes the image into a data buffer. */
size_t
lib_codec_encode_image (uint8_t* data, 
                        const pb_image_t* image);

/** Decodes an image from a data buffer. */
size_t
lib_codec_decode_image (const uint8_t* data,
                        pb_image_t** image);

#define PB_IMAGE_ID_MAX_LENGTH (64)

/**
 * Attaches a short string to an image. This string can be used to identify the image or
 * keep meta data about the image around.
 * 
 * String must not be longer than PB_IMAGE_ID_MAX_LENGTH and if longer, and error message
 * is returned and the string is not set to the image.
 * 
 * The string is internally copied, and may be released afterwards.
 * 
 * A zero length string or a NULL pointer removes the image_id.
 */
pb_rc_t
pb_image_set_image_id(pb_image_t* image,
                      const char* image_id);

/** Returns the image id previously set by set_image_id, or a NULL pointer if no image id is set. */
const char*
pb_image_get_image_id(const pb_image_t* image);

/**
* Append the image ID with an index.
* Only adds an index if an ID already exists,
* otherwise nothing is done and the function returns OK.
* Indexes from 0-99 is supported.
*/
pb_rc_t
pb_image_append_image_id(pb_image_t* image,
                         int index);

#ifdef __cplusplus
}
#endif

#endif /* PM_IMAGE_H */

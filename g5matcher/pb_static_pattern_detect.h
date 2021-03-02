/*
 * Copyright (c) 2018, Precise Biometrics AB
 * All rights reserved.
 *
 */

#ifndef PB_STATIC_PATTERN_DETECT_H
#define PB_STATIC_PATTERN_DETECT_H

#include <stdint.h>

#include "pb_image.h"
#include "pb_session.h"
#include "pb_errors.h"
#include "pb_image_t.h"

#ifdef __cplusplus
extern "C" {
#endif

/// The Static Pattern Detect (SPD) module
/** The Static Pattern Detect (SPD) Module detects static patterns in
  * consecutive images and generates a pb_image_mask_t which in turn is
  * associated with a pb_image_t. This prevents the static pattern from
  * poisoning the template which could increase the risk to accept an impostor.
  *
  * The SPD Module will be able to detect static patterns as soon as it has been
  * updated with at least two images. Nothing can be determined to be truly
  * static from only one still image.
  *
  * The SPD object will need to exist perpetually and is easy to store as a blob.
  *
  * The caller is responsible for deleting the SPD object with the provided
  * delete function.
  *
  * pb_static_pattern_detect_update() must be run on an image before it is sent
  * to verification or template extraction.
  *
  *     Example:
  * @code
  *         pb_static_pattern_detect_t* spd = 0
  *         pb_image_t* image = get_image_function();
  *         pb_static_pattern_detect_update(image, session, &spd);
  *         save_data_function(spd->data, spd->data_size);
  *         pb_static_pattern_detect_delete(spd);
  * @endcode
  *
  * pb_static_pattern_detect_allocate() is used to allocate data to make sure
  * that it is compatible with the internal free function.
  *
  *     Example:
  * @code
  *         pb_static_pattern_detect_t* spd = 0
  *         uint32_t filesize = get_data_size("filename");
  *         pb_static_pattern_detect_allocate(&spd, filesize);
  *         load_data_function("filename", spd->data, spd->data_size);
  * @endcode
  *
  * pb_static_pattern_detect_enroll() updates the SPD with several images more
  * efficiently than multiple calls to pb_static_pattern_detect_update().
  *
  *     Example:
  * @code
  *         pb_static_pattern_detect_t* spd = 0
  *         pb_image_t* images[NBR_OF_IMAGES]
  *         for (int i=0; i<NBR_OF_IMAGES; i++)
  *             images[i] = get_image_function();
  *         pb_static_pattern_detect_enroll(images, NBR_OF_IMAGES, session, &spd);
  * @endcode
  *
  * SPD and large area sensors.
  * The SPD can handle smaller images (cropped) from a large area sensor if the 
  * images are related to the larger sensor area. To relate an image to the sensor,
  * use pb_image_relate to set the offset and the size of the sensor.
  *
  * Similar images.
  * Sometimes multiple images are captures without the user moving the finger between 
  * the captures, e.g. if the verification process involves a first quick capture and
  * if that image fails to match, another capture is performed. In these cases the SPD 
  * will have a hard time distinguish between two very similar images and actual static
  * patterns. To improve the SP detection for these cases, any succeeding images 
  * captured within a short time frame from the first image, shall be marked as similar, 
  * using the pb_image_set_similar_image function. 
  *
  * NOTE:
  * As mentioned, pb_static_pattern_detect_update() will create a mask for the
  * image object. If the image object already has a mask, for example created
  * based on known artifacts from the sensor, pb_static_pattern_detect_update()
  * will use original mask as a base. This is also the recommended way if using
  * both manually created mask and SPD mask; create manual mask first, then
  * update mask using pb_static_pattern_detect_update().
  * If a manual mask is created after pb_static_pattern_detect_update() it will
  * overwrite the SPD mask. The existing mask can be retrieved and edited with
  * pb_image_get_mask(), but it is highly recommended to just run SPD after the
  * manual mask is created so that changes made by the SPD will not risk being
  * overwritten.
  */
typedef struct {
    /// SPD data
    uint8_t* data;
    /// Size of *data
    uint32_t data_size;
    /// Allocation reference counter
    int reference_counter;
} pb_static_pattern_detect_t;

/** Create an SPD object
  *
  * @param[out] spd is the created SPD object
  *
  * Note: The caller is responsible for deleting the SPD object.
  */
pb_rc_t
pb_static_pattern_detect_create(pb_static_pattern_detect_t** spd);

/** Create an SPD object and allocate data of a given size
  *
  *
  * @param[out] spd is the created SPD object
  * @param[in] data_size is how many bytes to allocate
  *
  * Note: The caller is responsible for deleting the SPD object.
  */
pb_rc_t
pb_static_pattern_detect_allocate(pb_static_pattern_detect_t** spd,
                                  uint32_t data_size);

/** Enrolls several images to the SPD object. Call this function instead of calling
  * _update repeatedly for enrollment to optimize the detection of static patterns
  * for the very first images.
  *
  * @param[in,out] images is the images to update the SPD with.
  *                A static pattern mask will be attached to each of these images.
  * @param[in]     nbr_of_images is the number of images to update the SPD with.
  * @param[in]     session is the session object.
  * @param[in,out] spd is the SPD object
  *                A new SPD will be created if *spd == NULL.
  *
  * Note: The caller is responsible for deleting the SPD object.
  */
pb_rc_t
pb_static_pattern_detect_enroll (pb_image_t** images,
                                 int nbr_of_images,
                                 pb_session_t* session,
                                 pb_static_pattern_detect_t** spd);

/** Update the SPD object with an image.
  *
  * @param[in,out] image is the current image to update the SPD with.
  *                The static pattern mask will be attached to this image.
  * @param[in]     session is the session object.
  * @param[in,out] spd is the SPD object
  *                A new SPD will be created if *spd == NULL.
  *
  * Note: The caller is responsible for deleting the SPD object.
  */
pb_rc_t
pb_static_pattern_detect_update(pb_image_t* image,
                                pb_session_t* session,
                                pb_static_pattern_detect_t** spd);

/** Retains the SPD, which means that the reference counter for the object
  * will increase by 1. The caller must make sure that _delete is being called
  * when the object is not needed anymore. Retaining a null pointer has
  * no effect.
  *
  * @param[in] spd is the SPD object
  * @return the retained SPD object
  *
  * Note: The caller is responsible for deleting the retained SPD object.
  */
pb_static_pattern_detect_t*
pb_static_pattern_detect_retain(pb_static_pattern_detect_t* spd);

/** Resets the SPD to the initial state.
  *
  * @param[in] spd is the SPD object.
  */
pb_rc_t
pb_static_pattern_detect_reset(pb_static_pattern_detect_t* spd);

/** Decreases the reference counter for the SPD by 1. If the reference
  * counter reaches 0 then the object will also be deleted. Deleting a null
  * pointer has no effect.
  *
  * @param[in] spd is the SPD object
  */
void
pb_static_pattern_detect_delete(pb_static_pattern_detect_t* spd);

#ifdef __cplusplus
}
#endif

#endif /* PB_STATIC_PATTERN_DETECT_H */

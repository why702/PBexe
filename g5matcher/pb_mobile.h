/*
 * Copyright (c) 2018, Precise Biometrics AB
 * All rights reserved.
 *
 */

#ifndef PB_MOBILE_H
#define PB_MOBILE_H

#include <stdint.h>

#include "pb_algorithm.h"
#include "pb_match_result.h"
#include "pb_errors.h"
#include "pb_session.h"
#include "pb_template.h"
#include "pb_types.h"
#include "pb_image_t.h"
#include "pb_pad.h"
#include "pb_verifier_defs.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct pb_mobile_st pb_mobile_t;

typedef struct pb_mobile_settings_st {
    /** Tells if SPD is disabled. Default value is 0, meaning that SPD is enabled by default. */
    int disable_spd; 

    /**
     * Set the penalty hit in FRR to have finger detection.
     * Note that this setting requires that the algorithm is configured to have a finger detect function.
     */
    pb_frr_t finger_detect_enrollment_frr_penalty;
    pb_frr_t finger_detect_verification_frr_penalty;
    pb_frr_t finger_detect_dynamic_update_frr_penalty;

    /** Tells whether or not the dynamic update should use similar images, i.e.
      * images that are similar to the previous image, to replace any subtemplates
      * with.
      * Default value is 0, meaning that similar images will not be used for
      * replacing any subtemplates. */
    int replace_with_similar_images;
} pb_mobile_settings_t;

/** Retrieves default settings. */
pb_mobile_settings_t
pb_mobile_get_default_settings();

/**
 * Creates an empty pb_mobile object.
 *
 * @return an error code or PB_RC_OK if successful.
 */
pb_rc_t
pb_mobile_create(const pb_mobile_settings_t* settings,
                 pb_algorithm_t* algorithm,
                 pb_mobile_t** mobile);

/**
 * Retains the mobile object, which means that the reference counter for the object
 * will increase by 1. The caller must make sure that _delete is being called
 * when the object is not needed anymore.
 * 
 * Retaining a null pointer has no effect. 
 */
pb_mobile_t*
pb_mobile_retain(pb_mobile_t* mobile);

/**
 * Decreases the reference counter for the object by 1. If the reference counter
 * reaches 0 then the object will also be deleted.
 * 
 * Deleting a null pointer has no effect. 
 */
void
pb_mobile_delete(pb_mobile_t* mobile);

/**
 * Encodes a pb_mobile_t object.
 *
 * @param[in] mobile is the pb_mobile_t object
 * @param[out] data is the encoded data
 * @param[out] data_size is the encoded data size
 * 
 * @return an error code or PB_RC_OK if successful.
 */
pb_rc_t
pb_mobile_encode(pb_mobile_t* mobile,
                 uint8_t** data,
                 uint32_t* data_size);

/**
 * Decodes a pb_mobile_t object
 *
 * @param[in] data is the encoded data
 * @param[in] data_size is the encoded data size
 * @param[in] algorithm is the algorithm to use for this mobile object
 * @param[out] mobile is the decoded pb_mobile_t object
 * 
 * @return an error code or PB_RC_OK if successful.
 */
pb_rc_t
pb_mobile_decode(const uint8_t* data,
                 uint32_t data_size,
                 pb_algorithm_t* algorithm,
                 pb_mobile_t** mobile);

/** 
 * Enrollment process. 
 * 
 * The enrollment process is divided in three steps:
 *   1. Screening. (optional)
 *   2. Adding images.
 *   3. Finalizing.
 *
 * 1. Screening step may be used to decide whether or not an image 
 *    should be used for enrollment, by using the quality or geometry
 *    information returned. Should be called once for each captured
 *    image. This step is optional. 
 *
 * 2. Adding an image tells the mobile api that this image shall be 
 *    part of the enrolled template. Should be called once for each 
 *    image chosen for enrollment.
 *
 * 3. In the finalizing step the actual template to be enrolled is 
 *    created from all the added images. 
 *
 * An example of a simple enrollment process:
 *
 *  pb_rc_t status = PB_RC_OK;
 *  int nbr_of_captures = 0;
 *  
 *  // Get the mobile object.
 *  pb_mobile_t* mobile = 0;
 *  status = pb_mobile_create/decode (..., &mobile);
 *
 *  while (status == PB_RC_OK) {
 *      // Capture an image.
 *      pb_image_t* image = external_capture_image (...);
 *      nbr_of_captures++;
 *
 *      // Screen the image.
 *      pb_enroll_quality_t quality;
 *      pb_enroll_geometry_t geometry;
 *      status = pb_mobile_enroll_screen (mobile, image, &quality, &geometry);
 *      if (status != PB_RC_OK) break;
 *      
 *      // Add the image.
 *      uint8_t coverage;
 *      int nbr_of_added_images = 0;
 *      if (external_image_shall_be_added (image, quality, geometry)) {
 *          status = pb_mobile_enroll_add (mobile, &image, 1, &nbr_of_added_images, &coverage);
 *          if (status != PB_RC_OK) break;
 *      }
 *      pb_image_delete (image);
 *
 *      // Decide if the enrollment is done.
 *      if (external_enrollment_done (nbr_of_captures, nbr_of_added_images, coverage)) break;
 *  }
 *
 *  // Finalize enrollment.
 *  pb_template_t* template = 0;
 *  if (status == PB_RC_OK) {
 *      status = pb_mobile_enroll_finalize (mobile, &template);
 *  }
 *
 *  // Store template.
 *  if (status == PB_RC_OK) {
 *      external_store_template (template);
 *  }
 *
 *
 * An example of single (few) tap enrollment for larger sensors, assuming that a full 
 * image from the full sensor is captured for enrollment:
 *
 *  ...
 *
 *  pb_image_t* sub_images[PB_MOBILE_SPLIT_MAX_SUB_IMAGES] = { 0 };
 *  int nbr_of_sub_images = 0;
 *
 *  // Split image into sub images.
 *  status = pb_mobile_split_image (mobile, image, sub_images, &nbr_of_sub_images);
 *  if (status != PB_RC_OK) break;
 * 
 *  // Train with each sub image.
 *  for (i = 0; i < nbr_of_sub_images; i++) {
 *      // Screen the image.
 *      pb_enroll_quality_t quality;
 *      pb_enroll_geometry_t geometry;
 *      status = pb_mobile_enroll_screen (mobile, image, &quality, &geometry);
 *      if (status != PB_RC_OK) break;
 *      
 *      // Add the image.
 *      uint8_t coverage;
 *      int nbr_of_added_images = 0;
 *      if (external_image_shall_be_added (image, quality, geometry)) {
 *          status = pb_mobile_enroll_add (mobile, &image, 1, &nbr_of_added_images, &coverage);
 *          if (status != PB_RC_OK) break;
 *      }
 *  }
 *
 *  // Delete the sub images. 
 *  for (i = 0; i < nbr_of_sub_images; i++) {
 *      pb_image_delete (sub_images[i]);
 *  }
 *
 * ...
 *
 */

/** The quality of a single enrollment image. Use this information to
  * decide whether or not an image should be used for enrollment. */
typedef struct pb_enroll_quality_st {
    /** The area of the fingerprint in percentage (0-100) of the total 
      * image area. An area of 70 or more is recommended for enrollment,
      * but note that the value is very dependent on the image size. */
    uint8_t fingerprint_area;
    /** The fingerprint quality (ridge direction coherence) ranging from 
      * 0 (worse) to 100 (best). A quality of 20 or above is recommended
      * for enrollment. */
    uint8_t fingerprint_quality;
    /** The signal-to-noise ratio (SNR) of the image, shifted 3 bits. An
      * SNR of 5 (5 << 3) or above is recommended for enrollment. */
    uint8_t signal_to_noise_ratio_3;
    /** Indicates if finger detect has identified this as a finger (1) or not (0).
      * If finger detect is disabled this will always be set to finger (1).*/
    int is_finger;
} pb_enroll_quality_t;

/** The enrollment geometry computed from all the enrolled images. Use 
  * this information to decide whether or not an image should be used
  * for enrollment. */
typedef struct pb_enroll_geometry_st {
    /** The maximum overlap against any of the previously added images,
      * in percentage of the total image area. A maximum overlap of 90
      * or lower is recommended to ensure a good spread of the 
      * enrollment images. */
    uint8_t maximum_overlap;
    /** The overlap against the last added image. An overlap of 80 or
      * lower is recommended to ensure a good spread of the 
      * enrollment images. */
    uint8_t overlap_against_last_added_image;
    /** The best match score against the previously added images. */
    uint16_t match_score;
} pb_enroll_geometry_t;

/**
 * The screening part of the enrollment. Computes the quality of the 
 * image and the geometries of the enrollment. 
 *
 * @param[in] mobile is the pb_mobile_t object to use.
 * @param[in] image is the image to use.
 * @param[out] quality is the quality metrics of the image. May be set
 *             to 0 if not needed. 
 * @param[out] geometry is the geometry metrics of the enrollment. May 
 *             be set to 0 if not needed.
 *
 * @return an error code or PB_RC_OK if successful.
 * 
 * Images sent to this function may be altered by the call, adding information
 * such as segmentation and quality metrics. If the original image must be kept,
 * a copy must be made before calling this function.
 */
pb_rc_t
pb_mobile_enroll_screen (pb_mobile_t* mobile,
                         pb_image_t* image,
                         pb_enroll_quality_t* quality,
                         pb_enroll_geometry_t* geometry);

/** Room for 5x4 sub images including a center image. */
#define PB_MOBILE_SPLIT_MAX_SUB_IMAGES    21

/**
 * Splits an image into several sub images, based on the split configuration
 * set for the algorithm.
 *
 * @param[in] mobile is the pb_mobile_t object to use.
 * @param[in] image is the image to use.
 * @param[out] sub_images is the list of sub images cropped from the original 
 *             image. The caller is responsible of deleting the sub images.
 * @param[out] nbr_of_sub_images is the number of sub images. 
 *
 * @return an error code or PB_RC_OK if successful.
 */
pb_rc_t
pb_mobile_split_image (pb_mobile_t* mobile,
                       pb_image_t* image,
                       pb_image_t* sub_images[PB_MOBILE_SPLIT_MAX_SUB_IMAGES],
                       int* nbr_of_sub_images);

/** 
 * Adds one or more images for enrollment. 
 *
 * @param[in] mobile is the pb_mobile_t object to use.
 * @param[in] images is the list of images to use.
 * @param[in] nbr_of_images is the number of templates to be enrolled.
 * @param[out] nbr_of_added_images is the total number of added images including
 *             the ones added in the current call. May be set to 0 if not needed.
 * @param[out] coverage is the fingerprint coverage, in percentage of a 
 *             "default fingerprint" with an estimated size of 12.5x17.5mm, 
 *             computed from all added images. A coverage of 75 or more is 
 *             recommended for enrollment. May be set to 0 if not needed.
 *
 * @return an error code or PB_RC_OK if successful.
 */
pb_rc_t
pb_mobile_enroll_add (pb_mobile_t* mobile, 
                      pb_image_t* images[], 
                      int nbr_of_images,
                      int* nbr_of_added_images,
                      uint8_t* coverage);

/** 
 * Completes the enrollment by creating the enrollment template from the 
 * added images.
 *
 * @param[in] mobile is the pb_mobile_t object to use.
 * @param[out] template_ is the returned enrollment template. The
 *             caller is responsible for deleting the template.
 *
 * @return an error code or PB_RC_OK if successful.
 */
pb_rc_t
pb_mobile_enroll_finalize (pb_mobile_t* mobile,
                           pb_template_t** template_);

/** 
 * Cancels the enrollment process and returns the enrollment to it's initial
 * state. All added images will be removed. 
 *
 * @return an error code or PB_RC_OK if successful.
 */
pb_rc_t
pb_mobile_enroll_cancel (pb_mobile_t* mobile);

/** The specific settings for the verification. */
typedef struct pb_mobile_verify_settings_st {

    /**
     * Adjusts the latency for the verification. Allows for a decrease or increase of
     * the latency for a verification in relation to the default latency. The latency 
     * adjustment is given as a percentage of the default latency, with negative values 
     * indicating a decrease of latency, with -100 as the minimum value allowed. 
     * Default value is 0, meaning no increase or decrease of latency.
     *
     * Adjusting the latency affects the biometric performance, the shorter the 
     * verification time, the worse FRR. 
     *
     * The default latency is dependent on how the algorithm has been setup. A change of
     * algorithm settings may therefore require a change of the latency adjustments.
     *
     * Note that this setting only affects the verification part, excluding such things
     * as static pattern detection, image processing and feature extraction.
     */
    int latency_adjustment;

    /**
     * Tells which subtemplates to match against, depending on the class id of the
     * subtemplates. All subtemplates with a class id >= match_against_class_id will
     * be matched against. Other subtemplates will be ignored. 
     *
     * Make sure that some classes are not starved out by others during dynamic
     * update, by setting a minimum number of templates for the higher class ids.
     *
     * Default value is 0, meaning that all classes will be matched against (minimum
     * class id is 0).
     */
    uint8_t match_against_class_id;

    /**
     * Set the penalty hit in FRR to have Presentation Attack Detection (PAD).
     * 
     * Note that this setting only affects algorithms with PAD.
     */
    pb_frr_t pad_frr_penalty;

    /**
     * Set the penalty hit in FRR to have Latent Detection.
     *
     * Note that this setting only affects algorithms with Latent Dection.
     */
    pb_frr_t latent_frr_penalty;

    /**
     * Enabling this, should improve dry finger biometric performance, but adds
     * latency to the verification.
     */
    uint32_t improve_dry_finger_performance;

} pb_mobile_verify_settings_t;

/** Retrieves default settings. */
pb_mobile_verify_settings_t
pb_mobile_verify_get_default_settings ();

/**
 * Verifies an image against a list of previously enrolled templates.
 * 
 * If this function successfully completes, match_result contains information
 * about the verification; if it was successful, and which template in the list
 * that was matched against. See pb_match_result.h for APIs to get information
 * from the match_result object, e.g.,
 *   pb_match_result_get_decision(...)
 *   pb_match_result_get_matching_template_index(...)
 *
 * The verify functionality is divided into two functions, this one and the 
 * pb_mobile_verify_finalize function that must be called regardless of the match
 * result. This allows the caller to unlock the phone as fast as possible without
 * having to wait for e.g. the dynamic update process that could be done in the 
 * background. 
 * 
 * @param[in] mobile is the pb_mobile_t object.
 * @param[in] enrolled_templates is the list of enrolled templates.
 * @param[in] nbr_of_enrolled_templates is the number of enrolled templates in the list.
 * @param[in] verification_image is the image to use for verification.
 * @param[in] false_accept_rate is the requested false accept rate. 
 * @param[in] verify_settings is the specific settings for this verification. May be set
 *            to 0 if not needed, in which case the default settings will be used.
 * @param[out] match_result is the returned result of the verification. The caller is 
 *             responsible for deleting the returned match_result. 
 * 
 * @return an error code or PB_RC_OK if successful.
 * 
 * The verification image is updated by calling this function. Updates may include
 * additional segmentation and quality metrics. If the original image needs to be
 * stored, a copy must be created.
 * 
 */
pb_rc_t
pb_mobile_verify(pb_mobile_t* mobile,
                 pb_template_t* enrolled_templates[], int nbr_of_enrolled_templates,
                 pb_image_t* verification_image,
                 pb_far_t false_accept_rate,
                 const pb_mobile_verify_settings_t* verify_settings,
                 pb_match_result_t** match_result);

/**
 * After a verification has completed without error, this function will finalize any remaining 
 * work, such as updating an enrolled template.
 * 
 * This function must be called regardless of the match result from the pb_mobile_verify call.
 * 
 * @param[in] mobile is the pb_mobile_t object.
 * @param[out] updated_enrolled_template is the returned updated template. May be 0 if no enrolled 
 *             template needs to be updated. The caller is responsible for deleting the returned 
 *             enrolled template.
 * 
 * @return an error code or PB_RC_OK if successful.
 */
pb_rc_t
pb_mobile_verify_finalize(pb_mobile_t* mobile,
                          pb_template_t** updated_enrolled_template);

/**
 * Adds an image as a calibration image. If possible, call this function whenever 
 * the image characteristics has changed, e.g. if a re-calibration of the sensor 
 * has been made. Calling this function helps e.g. the static pattern detection 
 * to adopt quicker to changes in the sensor settings. 
 * 
 * Note that the image shall be empty from fingerprint data.
 * 
 * @param[in] mobile is the pb_mobile_t object to use.
 * @param[in] images is the list of images to use.
 * @param[in] nbr_of_images is the number of images in the list.
 *
 * @return an error code or PB_RC_OK if successful.
 * 
 */
pb_rc_t
pb_mobile_calibrate(pb_mobile_t* mobile,
                    pb_image_t* images[],
                    int nbr_of_images);

/** Resets the history for the mobile object, including a reset of the SPD history. 
  *
  * @param[in] mobile is the pb_mobile_t object to use.
  *
  * @return an error code or PB_RC_OK if successful.
  */
pb_rc_t
pb_mobile_reset_history (pb_mobile_t* mobile);



/** 
  * TEE + SE verification.
  *
  * For increased security, it is possible to split the verification between the Trusted Execution 
  * Environment (TEE) and the Secure Element (SE). The TEE performs the initial verification and 
  * the SE performs the final (if TEE accepted). 
  *
  * The call flow is as shown below:
  *
  *   ----------------------------------------------------------------------------------
  *
  *   // Verify begin in TEE:
  *
  *   pb_match_result_t* match_result = 0;
  *   pb_template_t* verification_template_SE = 0;
  *   pb_template_t* match_data_SE = 0;
  *
  *   // Perform initial verification in TEE, returns the match result as well as a verification 
  *   // template and match data to be sent to the SE where the final verification is done. 
  *   pb_mobile_verify_TEE (..., &match_result, &verification_template_SE, &match_data_SE);
  *
  *   if (pb_match_result_get_decision (match_result) == PB_DECISION_MATCH) {
  *       // The TEE accepted the verification, trigger verification in SE to confirm accept. 
  *       ... // Implemented by caller. 
  *   }
  *
  *   pb_match_result_delete (match_result);
  *   pb_template_delete (verification_template_SE);
  *   pb_template_delete (match_data_SE);
  *
  *   ----------------------------------------------------------------------------------
  *
  *   // Verify in SE:
  *   
  *   pb_match_result_SE_t match_result_SE = { 0 };
  *   pb_template_t* updated_enrolled_template_SE = 0;
  *   
  *   // Get enrolled template corresponding to the matched enrolled template in the TEE verification.
  *   pb_template_t* enrolled_template_SE = ... // Implemented by caller. 
  *   // Get verification template passed from TEE.
  *   pb_template_t* verification_template_SE = ... // Implemented by caller.
  *   // Get match data passed from TEE.
  *   pb_template_t* match_data_SE = ... // Implemented by caller.
  *
  *   // Verify.
  *   pb_mobile_verify_SE (enrolled_template_SE, verification_template_SE, match_data_SE, 
  *                        1, &match_result_SE, &updated_enrolled_template_SE);
  *
  *   // Check if SE confirmed accept from TEE. 
  *   if (match_result_SE->decision == PB_MATCH_DECISION) {
  *       // Trigger unlock of phone. 
  *       ... // Implemented by caller. 
  *   }
  *
  *   // Update enrolled template in SE, if applicable. 
  *   if (updated_enrolled_template_SE) {
  *       ... // Implemented by caller. 
  *   }
  *
  *   ----------------------------------------------------------------------------------
  *
  *   // Verify end in TEE:
  *
  *   pb_template_t* updated_enrolled_template = 0;
  *   // Get match result passed from SE.
  *   pb_match_result_SE_t match_result_SE = ... // Implemented by caller.
  *
  *   // Finalize verification. 
  *   pb_mobile_verify_finalize_TEE (..., match_result_SE, &updated_enrolled_template);
  *
  *   // Update enrolled template in TEE, if applicable.
  *   if (updated_enrolled_template) {
  *       ... // Implemented by caller.
  *   }
  *
  *   ----------------------------------------------------------------------------------
  *
  * TEE + SE Enrollment.
  *
  * Enrollment is very similar to the TEE only solution. However, the following needs to be considered:
  *
  *  - The limitation of the enrolled templates must be specified as maximum number of subtemplates, maximum
  *    template size (in bytes) is not supported. The reason for this is that the templates are stored both 
  *    in the TEE and the SE and the same subtemplate needs to be replaced in both locations at dynamic 
  *    update, hence the need for a well defined limitation.
  *       pb_algorithm_config_t* algorithm_config = pb_algorithm_get_config (algorithm);
  *       algorithm_config->max_nbr_of_subtemplates = N;
  *
  *  - The enrolled template to be stored in the SE must be created and passed to the SE after enrollment is
  *    complete. Use the pb_mobile_get_enrolled_template_SE function to create the enrolled template for the SE.
  *
  */

/** Returns the part of the enrolled template (in the TEE) to be stored in the
  * secure element (SE). May be called after an enrollment to pass the SE template
  * to the SE. */
pb_rc_t
pb_mobile_get_enrolled_template_SE (const pb_template_t* enrolled_template,
                                    pb_template_t** enrolled_template_SE);

/** Performs the initial part of the TEE + SE verification. Should be called in the TEE. 
  *
  * @param[in] mobile is the pb_mobile_t object.
  * @param[in] enrolled_templates is the list of enrolled templates.
  * @param[in] nbr_of_enrolled_templates is the number of enrolled templates in the list.
  * @param[in] verification_image is the image to use for verification.
  * @param[in] false_accept_rate is the requested false accept rate.
  * @param[in] verify_settings is the specific settings for this verification. May be set
  *            to 0 if not needed, in which case the default settings will be used.
  * @param[out] match_result is the returned result of the verification. The caller is
  *             responsible for deleting the returned match_result.
  * @param[out] verification_template_SE is the verification template to be passed
  *             to the SE to perform the final verification in the SE.
  * @param[out] match_data_SE is some extra data needed for the SE verification to be 
  *             passed to the SE to perform the final verification in the SE.
  *
  * @return an error code or PB_RC_OK if successful.
  */
pb_rc_t
pb_mobile_verify_TEE (pb_mobile_t* mobile,
                      pb_template_t* enrolled_templates[], int nbr_of_enrolled_templates,
                      pb_image_t* verification_image,
                      pb_far_t false_accept_rate,
                      const pb_mobile_verify_settings_t* verify_settings,
                      pb_match_result_t** match_result,
                      pb_template_t** verification_template_SE,
                      pb_template_t** match_data_SE);

/** Match result for the verification in SE. */
typedef struct pb_match_result_SE_st {
    /** Decision from the verification, PB_DECISION_MATCH if match was successful. */
    int decision;
    /** The tag of the subtemplate that was replaced in the SE, -1 if not applicable.
      * The SE will decide which subtemplate to replace and send this information back to
      * the TEE so that the corresponding subtemplate can be replaced in the TEE as well. */
    int32_t replaced_subtemplate_tag;
    /** The tag of the new subtemplate. */
    int32_t new_subtemplate_tag;
} pb_match_result_SE_t;

/** Performs the final part of the TEE + SE verification. Should be called in the SE. 
  *
  * @param[in] algorithm is the algorithm to be used. Must be the same algorithm as used in the TEE. 
  * @param[in/out] enrolled_template_SE is the enrolled template in the SE corresponding to the
  *            finger matched by the TEE verification, see pb_match_result_get_matching_template_index.
  * @param[in] verification_template_SE is the verification template returned by the
  *            pb_mobile_verify_TEE function.
  * @param[in] match_data_SE is the verification template returned by the
  *            pb_mobile_verify_TEE function.
  * @param[in] allow_update tells if the SE should perform an update of the enrolled template
  *            in the case of an accept.
  * @param[out] match_result_SE is the match result of the verification.
  * 
  * @note Data will be written to the template passed in as enrolled_template_SE. If this is not desired,
  *       a copy will have to be made before calling this function.
  *
  * @return an error code or PB_RC_OK if successful.
  */
pb_rc_t
pb_mobile_verify_SE (pb_algorithm_t* algorithm,
                     pb_template_t* enrolled_template_SE,
                     pb_template_t* verification_template_SE,
                     pb_template_t* match_data_SE,
                     int allow_update,
                     pb_match_result_SE_t* match_result_SE);

/** Finalizes the verification. Should be called in the TEE. 
  *
  * @param[in] mobile is the pb_mobile_t object.
  * @param[in] match_result_SE is the match result from the verification in the SE.
  * @param[out] updated_enrolled_template is the returned updated template. May be 0 if no enrolled
  *             template needs to be updated. The caller is responsible for deleting the returned
  *             enrolled template.
  *
  * @return an error code or PB_RC_OK if successful.
  */
pb_rc_t
pb_mobile_verify_finalize_TEE (pb_mobile_t* mobile,
                               pb_match_result_SE_t* match_result_SE,
                               pb_template_t** updated_enrolled_template);


#ifdef __cplusplus
}
#endif

#endif // PB_MOBILE_H

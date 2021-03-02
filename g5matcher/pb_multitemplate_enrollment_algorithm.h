/*
 * Copyright (c) 2008 - 2015, Precise Biometrics AB
 * All rights reserved.
 *
 */

#ifndef PB_MULTITEMPLATE_ENROLLMENT_ALGORITHM_H
#define PB_MULTITEMPLATE_ENROLLMENT_ALGORITHM_H

#include "pb_multitemplate_enrollment.h"
#include "pb_algorithm.h"

#ifdef __cplusplus
extern "C" {
#endif

/** See pb_multitemplate_enrollment.h for a full description of the multitemplate
  * enrollment controller. */

/** This file describes the API:s when using the algorithm object.
  *
  * There are two main versions of the controller.
  *
  *  MPU - The most advanced and suited for MPU environments with a bit
  *        more resources. The controller supports rich user feedback,
  *        template selection and creates templates that can be
  *        continuously updated.
  *
  *  MCU - A simpler controller using less resources and suited for MCU
  *        environments. Still provides some user feedback, template
  *        selections. Default is to create templates that allows incremental
  *        update until max capacity is reached, but can be altered to
  *        use normal continuously updatable templates.
  *        The incremental format only grows/changes at the end, hence
  *        a Flash memory sector need not be erased at each update.
  *
  * To run the enrollment process, follow these steps:
  *     1. Start the enrollment by creating the multitemplate enrollment object.
  *             pb_multitemplate_enroll_t* mte = pb_multitemplate_enroll_create_algorithm (...);
  *     2. Continously push images to the enrollment until all templates have
  *        been collected.
  *             pb_multitemplate_enroll_run (mte, image, 0, &coverage);
  *        The _run function returns the coverage of a thought area (250x350) of
  *        the merged fingerprint.
  *     3. When all captures have been collected, call pb_multitemplate_enroll_finalize_template_algorithm
  *        to create the multitemplate from the collected templates.
  *     4. Delete the mte object
  *             pb_multitemplate_enroll_delete (mte);
  */

/** Creates the multitemplate enroll object used for image driven
  * multitemplate enrollment.
  *
  * @param[in] algorithm is the algorithm to be used.
  * @param[in] finger is the finger to be enrolled.
  * @param[in] gui is an implementation of the gui callback interface.
  * @param[in] synthetic_image is an image to be used for displaying which
  *            parts of the finger that has been captured during enrollment. If set to 0,
  *            no synthetic image will be created and the function display_synthetic_image
  *            will never be called. The recommended usage is to set it to 0 and instead
  *            listen to the display_islands function that will then be used to give
  *            information about the captured parts of the finger. By setting it to 0 the
  *            RAM usage will also decrease significantly.
  *            An example image can be found in pb_synthetic_image.h.
  *
  * @return the multitemplate enrollment object.
  *
  * See pb_multitemplate_enroll_create for more information. */
pb_multitemplate_enroll_t*
pb_multitemplate_enroll_create_algorithm (pb_algorithm_t* algorithm,
                                          const pb_finger_t* finger,
                                          const pb_mte_guiI* gui,
                                          pb_image_t* synthetic_image);

/** Creates a simpler less resource demanding version of the enrollment controller,
  * more suited for MCU. This controller gives less user feedback and has less
  * sophisticated sample selection than the MPU version and by default creates
  * templates that can be incrementally updated until max capacity is reached.
  * Such templates only change by growing at the end. Normal, continuous update,
  * mode can be selected via the option parameter, such templates may change any
  * part of the template at each update.
  *
  * With a low value of max_template_size, enrollment may end sooner
  * than expected as there is no point in collecting more samples. In
  * this case it is normal and expected that the _run() function ends
  * with PB_RC_CAPACITY.
  *
  * Some differences compared to the fully featured controller:
  *
  *   - gui is optional, may be null.
  *   - can be configured to limit number of checks against accepted
  *     samples, default is all but may be lowered when CPU speed requires it.
  *   - prevent multiple fingers setting is a bit more strict; it will reject
  *     samples that do not match with any of the already enrolled samples.
  *   - Supports a more limited but to some extent more practical template format
  *     that can be incrementally updated until max capacity is reached. Max
  *     capacity is either max number of subtemplates or max template size.
  *   - Can be configured to use continuous update templates, see options.
  *   - the incremental nature of the template update allows update in Flash
  *     without erase (typically).
  *   - may limit max_nbr_captures if max template size is reached first,
  *     there is no point in collecting more than this many samples with current
  *     implementation (may change in future). In this case enrollment may end with
  *     PB_RC_CAPACITY, while not an error a reconfig may be relevant.
  *   - will not not give synthetic_image or display_island feedback
  *   - will not give calculate image coverage, 0 reported until last then 100.
  *   - will give basic feedback of required finger movements
  *   - will reject too similar samples, as tuned by the limit in number of
  *     verifications done.
  *
  * Note: The configuration of the controller can be tuned via the algorithms
  *       configuration pb_algorithm_config_t. Even though that pb_algorithm_config_t
  *       always contains settings for the full MPU controller, only the settings found
  *       in the MCU configuration (pb_multitemplate_enrollment_mcu_config_t) will
  *       affect the controller created here.
  *
  * @param[in] algorithm is the algorithm to be used.
  * @param[in] finger is the finger to be enrolled.
  * @param[in] gui is an implementation of the gui callback interface - may be NULL (0).
  * @param[in] max_verifications - limit the number of verifcation agains
  *            already accepted samples. Default 0 means verify all and is
  *            best, but when CPU is too slow lower to a suited number that
  *            gives good user experience.
  * @param[in] options Unused bits must be given as 0.
  *            Bit 0: Choose incremental (0) or normal (1) templates.
  *
  */
pb_multitemplate_enroll_t*
pb_multitemplate_enroll_mcu_create_algorithm (pb_algorithm_t* algorithm,
                                              const pb_finger_t* finger,
                                              const pb_mte_guiI* gui,
                                              uint8_t max_verifications,
                                              int options);

/** Creates a simpler less resource demanding version of the enrollment controller,
  * more suited for MCU. This controller gives less user feedback and has less
  * sophisticated sample selection than the MPU version and by default creates
  * templates that can be incrementally updated until max capacity is reached.
  * Such templates only change by growing at the end. Normal, continuous update,
  * mode can be selected via the option parameter, such templates may change any
  * part of the template at each update.
  *
  * With a low value of max_template_size, enrollment may end sooner
  * than expected as there is no point in collecting more samples. In
  * this case it is normal and expected that the _run() function ends
  * with PB_RC_CAPACITY.
  *
  * Some differences compared to the fully featured controller:
  *
  *   - gui is optional, may be null.
  *   - can be configured to limit number of checks against accepted
  *     samples, default is all but may be lowered when CPU speed requires it.
  *   - prevent multiple fingers setting is a bit more strict; it will reject
  *     samples that do not match with any of the already enrolled samples.
  *   - Supports a more limited but to some extent more practical template format
  *     that can be incrementally updated until max capacity is reached. Max
  *     capacity is either max number of subtemplates or max template size.
  *   - Can be configured to use continuous update templates, see options.
  *   - the incremental nature of the template update allows update in Flash
  *     without erase (typically).
  *   - may limit max_nbr_captures if max template size is reached first,
  *     there is no point in collecting more than this many samples with current
  *     implementation (may change in future). In this case enrollment may end with
  *     PB_RC_CAPACITY, while not an error a reconfig may be relevant.
  *   - will not not give synthetic_image or display_island feedback
  *   - will not give calculate image coverage, 0 reported until last then 100.
  *   - will give basic feedback of required finger movements
  *   - will reject too similar samples, as tuned by the limit in number of
  *     verifications done.
  *
  * Note: The configuration of the controller can be tuned via the algorithms
  *       configuration pb_algorithm_config_t. Even though that pb_algorithm_config_t
  *       always contains settings for the full MPU controller, only the settings found
  *       in the MCU configuration (pb_multitemplate_enrollment_mcu_config_t) will
  *       affect the controller created here.
  *
  * @param[in] algorithm is the algorithm to be used.
  * @param[in] finger is the finger to be enrolled.
  * @param[in] gui is an implementation of the gui callback interface - may be NULL (0).
  * @param[in] max_verifications - limit the number of verifcation agains
  *            already accepted samples. Default 0 means verify all and is
  *            best, but when CPU is too slow lower to a suited number that
  *            gives good user experience.
  * @param[in] storage Pointer to pb_storageI structure (see pb_storageI.h for more information),
  *            this parameter is optional and may be null.
  * @param[in] storage_state Pointer to storage state, related to pb_storageI (see pb_storageI.h for more information),
  *            this parameter is optional and may be null only if storage is also null.
  */
pb_multitemplate_enroll_t*
pb_multitemplate_enroll_storage_create_algorithm(pb_algorithm_t* algorithm,
    const pb_finger_t* finger,
    const pb_mte_guiI* gui,
    uint8_t max_verifications,
    const pb_storageI* storage,
    void* storage_state);

/** Allows the user to determine the polarity (normal or inverted) of the enrollment set and pass
  * this information on to the multitemplate enrollment object. The multitemplate enrollment 
  * controller only finds relative polarity between the enrolled images, and sets the base polarity
  * (default normal) to the polarity that the majority of enrolled images have. Call this function
  * to change this default polarity (base_polarity_is_inverted = 1). 
  *
  * Note that this function needs to be called prior to calling _finalize_template.
  *
  * This functionality only applies to sensors that under certain circumstances captures inverted 
  * images. Sensor vendors that always capture images with the same polarity can ignore this function.
  *
  * @param mte is the multimteplate enrollment object.
  * @param base_polarity_is_inverted tells if the default base polarity shall be set as inverted (1)
  *        or normal (0). Default 0.
  */
void
pb_multitemplate_enroll_set_base_polarity (pb_multitemplate_enroll_t* mte,
                                           int base_polarity_is_inverted);

/** Creates the multitemplate from the enrolled templates when
  * the enrollment process is completed.
  *
  * @param[in] algorithm is the algorithm to be used.
  * @param[in] mte is the multitemplate enrollment object.
  * @param[out] multitemplate is the returned multitemplate. The caller
  *             is responsible for deleting the returned multitemplate.
  *
  * @return PB_RC_OK if successful, or an error code.
  *
  * See pb_multitemplate_enroll_finalize_template for more information. */
pb_rc_t
pb_multitemplate_enroll_finalize_template_algorithm (pb_algorithm_t* algorithm,
                                                     pb_multitemplate_enroll_t* mte,
                                                     pb_template_t** multitemplate);

/** Starts and runs the complete enrollment process, including the capturing
  * of images from a sensor module (through the reader object).
  *
  * @param[in] algorithm is the algorithm to be used.
  * @param[in] finger is the finger to be enrolled.
  * @param[in] reader is the reader to capture images from.
  * @param[in] quality is the quality object.
  * @param[in] gui is an implementation of the gui callback interface.
  * @param[in] synthetic_image is an image to be used for displaying which
  *            parts of the finger that has been captured during enrollment. If set to 0,
  *            no synthetic image will be created and the function display_synthetic_image
  *            will never be called. The recommended usage is to set it to 0 and instead
  *            listen to the display_islands function that will then be used to give
  *            information about the captured parts of the finger. By setting it to 0 the
  *            RAM usage will also decrease significantly.
  *            An example image can be found in pb_synthetic_image.h.
  * @param[out] multitemplate is the returned multitemplate. The caller
  *             is responsible for deleting the returned multitemplate.
  *
  * @return PB_RC_OK if successful, or an error code.
  *
  * See pb_multitemplate_enroll for more information.
  */
pb_rc_t
pb_multitemplate_enroll_algorithm (pb_algorithm_t* algorithm,
                                   pb_finger_t* finger,
                                   pb_reader_t* reader,
                                   const pb_mte_guiI* gui,
                                   pb_image_t* synthetic_image,
                                   pb_template_t** multitemplate);

/** Cancels the enrollment process.
  * See pb_multitemplate_cancel_enroll for more information. */
pb_rc_t
pb_multitemplate_cancel_enroll_algorithm (pb_algorithm_t* algorithm,
                                          pb_reader_t* reader);

#ifdef __cplusplus
}
#endif

#endif /* PB_MULTITEMPLATE_ENROLLMENT_ALGORITHM_H */

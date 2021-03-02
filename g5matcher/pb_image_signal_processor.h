/*
 * Copyright (c) 2020, Precise Biometrics AB
 * All rights reserved.
 *
 */

#ifndef PB_IMAGE_SIGNAL_PROCESSOR_H
#define PB_IMAGE_SIGNAL_PROCESSOR_H

#include <stdint.h>

#include "pb_image.h"
#include "pb_session.h"
#include "pb_errors.h"
#include "pb_image_t.h"

#ifdef __cplusplus
extern "C" {
#endif

/** Image Signal Processor (ISP). 
  * 
  * Note: This API is subject to change. 
  *
  * The ISP converts a 16-bit raw image into a processed 8-bit image. The goal is to remove the noise
  * (e.g. low and high frequency noise and static noise) and to enhance the ridge patterns of the raw 
  * image. Call pb_image_signal_processor_process for each captured raw 16-bit image to convert them
  * to 8-bit images. 
  *
  *  - Initialization of the ISP - 
  * The ISP needs to be initialized for optimal initial performance, which can be done in two ways. 
  * Preferably, images without fingerprint information should be used with the pb_image_signal_processor_calibrate
  * function. If such images are not available the other option is to initialize the ISP with fingerprint
  * images using the pb_image_signal_processor_init, e.g. during the very first enrollment. If no initialization
  * at all is used then the first x images will not have any static noise reduction, where x is equal to 
  * the specified value of static_noise_reduce_window. If static noise reduction is disabled, there is no
  * need to initialize the ISP. 
  *
  *  - Configuration of the ISP - 
  * The ISP module itself is generic but specific implementations will be configured and created by Precise 
  * to fit each customer's specific sensor. The customer may still change some of the ISP settings, see 
  * pb_image_signal_processor_set_static_noise_settings and pb_image_signal_processor_set_noise_filter_settings.
  *
  *  - ISP and PerfEval - 
  * ISP is automatically built into PerfEval and can be run with algorithms that has an ISP attached
  * and if the index file is setup for .raw images (## itemType=raw/<name>/<width>x<height>). The 
  * image capture identifier may be specified in the index files as ' : capture_id=<value>'. If the image
  * capture identifer is not specified for an image the ISP will use the first image capture identifier. 
  * The ISP may be calibrated with a calibration image by specifying those in the header as: 
  * ' ## calibrationImage=<filename> : capture_id=<value> : person=<value>'. Again, if no capture identifier 
  * is specified the first identifier will be used. The person parameter may be used to specify a certain
  * calibration for a person e.g. if the dataset was collected from more than one device. If no person is 
  * specified, the calibration image will be used for all persons. 
  * To get debug images of the processed images, specify -debug_images=image. 
  *
  *  - ISP flash memory usage - 
  * The ISP will require extensive flash memory if static pattern reduction is enabled. The amount of 
  * memory used for encode will be approximately 3 * C * W * H byte, where C is the number of image 
  * captures specified, W is the width of the full sensor area and H is the height of the full sensor area.
  */

/** The Image Signal Processor (ISP) object. */
typedef struct pb_image_signal_processor_st pb_image_signal_processor_t;

/** Image capture settings. */
typedef struct pb_image_capture_st {
    /** The exposure time. */
    int exposure_time;
    /** The intensity range for a good quality image in normal condition, containing ~99% of the 
      * total pixel values. This range determines the range to be converted into 8-bit representation. */
    uint16_t intensity_range;
    /** Fixed lower boundary intensity for dark outlier detection. Defines the minimum dark outlier 
      * boundary. Boundary may be set higher by the automatic outlier detection. Default disabled (0). */
    uint16_t lower_boundary_intensity;
    /** Fixed upper boundary intensity for bright outlier detection. Defines the maximum bright outlier 
      * boundary. Boundary may be set lower by the automatic outlier detection. Default disabled (0). */
    uint16_t upper_boundary_intensity;
    /** Tells if static noise reduction shall be disabled for this image capture. */
    int disable_static_noise_reduction;
    /** Defines the lens reflection radius, if available in the images. Default disabled (0). */
    uint16_t lens_reflection_radius;
    /** The image capture identifier. */
    uint8_t identifier;
} pb_image_capture_t;

typedef struct pb_image_16bit_st {
    /** The image pixels. */
    uint16_t* pixels;
    /** The number of rows in the image. */
    uint16_t rows;
    /** The number of columns in the image. */
    uint16_t cols;
    /** The resolution of the image, in dpi. */
    uint16_t resolution;

    /** Relation of the sub image within the full image (sensor). Offset is (0, 0) for an
      * image that has the same size as the sensor size. */

    /** The offset in rows of the sub image within the full image. */
    int16_t offset_rows;
    /** The offset in columns of the sub image within the full image. */
    int16_t offset_cols;

    /** Tells if the image is similar to an image captured very recently. An image
     * marked as similar will e.g. not be used for backround estimation. */
    int similar;

    // TODO: Consider adding full_rows, full_cols for better control of users correctly relating images.
} pb_image_16bit_t;

/** Calibrates the ISP with one or more non-fingerprint images. It is important that these
  * images only contain static noise from the sensor, since no filtering will be made to 
  * remove fingerprint signal on these images. 
  * One calibration image should be sufficient, but the ISP may benefit from receiving several. 
  * Note that for large area sensors the ISP needs to be calibrated with one or more image 
  * covering the full sensor area for optimal performance. 
  * Will reset the current history of the ISP, including the static noise history and replace
  * with new data from the calibration images. 
  *
  * @param[in] isp is the image signal processor object.
  * @param[in] non_fingerprint_images_16bit is the non-fingerprint images to be used for calibration.
  * @param[in] nbr_of_images is the number of non-fingerprint images. 
  * @param[in] image_capture_identifer is the identifier of the image capture. Must be one of the
  *            image captures specified at _create. 
  *
  * @return PB_RC_OK if succesful, otherwise an error code.
  */
pb_rc_t
pb_image_signal_processor_calibrate (pb_image_signal_processor_t* isp,
                                     pb_image_16bit_t** non_fingerprint_images_16bit,
                                     int nbr_of_images,
                                     uint8_t image_capture_identifier);

/** Calibrates the ISP with one or more images. The images may contain fingerprint signal which will
  * be filtered out by the ISP. If non-fingerprint images is available use the _calibrate function instead.
  * The amount of images to use for initialization should match the static_noise_reduce_window specfied 
  * when creating the ISP. 
  * Will reset the current history of the ISP, including the static noise history and replace
  * with new data from the calibration images.
  *
  * @param[in] isp is the image signal processor object.
  * @param[in] images_16bit is the images to be used for calibration.
  * @param[in] nbr_of_images is the number of non-fingerprint images.
  * @param[in] image_capture_identifer is the identifier of the image capture. Must be one of the
  *            image captures specified at _create.
  *
  * @return PB_RC_OK if succesful, otherwise an error code.
  */
pb_rc_t
pb_image_signal_processor_init (pb_image_signal_processor_t* isp,
                                pb_image_16bit_t** images_16bit,
                                int nbr_of_images,
                                uint8_t image_capture_identifier);

/** Process an image, i.e. reduces noise, enhances ridges and converts to 8-bit. 
  *
  * @param[in] isp is the image signal processor object.
  * @param[in] image_16bit is the 16-bit raw image to be processed.
  * @param[in] image_capture_identifer is the identifier of the image capture. Must be one of the
  *            image captures specified at _create. 
  * @param[out] image_8bit is the returned processed 8-bit image.  
  *
  * @return PB_RC_OK if succesful, otherwise an error code.
  */
pb_rc_t
pb_image_signal_processor_process (pb_image_signal_processor_t* isp,
                                   const pb_image_16bit_t* image_16bit,
                                   uint8_t image_capture_identifier,
                                   pb_image_t** image_8bit);

typedef struct pb_isp_static_noise_settings_st {
    /** Tells if the static noise reduction is enabled for the ISP. */
    int enabled;
    /** The number of images to average the static noise over. */
    uint8_t average_window;
    /** The number of images required before applying static noise reduction. */
    uint8_t reduce_window;
} pb_isp_static_noise_settings_t;

/** Retrieves the static noise reduction settings of the ISP. 
  * 
  * @param[in] isp is the image signal processor object.
  * @param[out] settings is the returned static noise settings from the ISP. 
  *
  * @return PB_RC_OK if succesful, otherwise an error code.
  */
pb_rc_t
pb_image_signal_processor_get_static_noise_settings (pb_image_signal_processor_t* isp,
                                                     pb_isp_static_noise_settings_t* settings);

/** Changes the static noise reduction settings of the ISP.
  *
  * @param[in] isp is the image signal processor object.
  * @param[in] settings is the static noise settings for the ISP.
  *
  * @return PB_RC_OK if succesful, otherwise an error code.
  */
pb_rc_t
pb_image_signal_processor_set_static_noise_settings (pb_image_signal_processor_t* isp,
                                                     pb_isp_static_noise_settings_t* settings);

typedef struct pb_isp_noise_filter_settings_st {
    /** The highpass cutoff frequency for what is to be considered low frequency noise, in lines/mm
      * and shifted 8 bits. Anything below this frequency will be seen as noise. Default 0.5 lines/mm. */
    int highpass_cutoff_frequency;
    /** The lowpass cutoff frequency for what is to be considered high frequency noise, in lines/mm
      * and shifted 8 bits. Anything above this frequency will be seen as noise. Default 6 lines/mm. */
    int lowpass_cutoff_frequency;
} pb_isp_noise_filter_settings_t;

/** Retrieves the noise filter settings of the ISP.
  *
  * @param[in] isp is the image signal processor object.
  * @param[out] settings is the returned noise filter settings from the ISP.
  *
  * @return PB_RC_OK if succesful, otherwise an error code.
  */
pb_rc_t
pb_image_signal_processor_get_noise_filter_settings (pb_image_signal_processor_t* isp,
                                                     pb_isp_noise_filter_settings_t* settings);

/** Changes the noise filter settings of the ISP.
  *
  * @param[in] isp is the image signal processor object.
  * @param[in] settings is the noise filter settings for the ISP.
  *
  * @return PB_RC_OK if succesful, otherwise an error code.
  */
pb_rc_t
pb_image_signal_processor_set_noise_filter_settings (pb_image_signal_processor_t* isp,
                                                     pb_isp_noise_filter_settings_t* settings);

/** Returns the number of image captures. */
int
pb_image_signal_processor_get_nbr_of_image_captures (const pb_image_signal_processor_t* isp);

/** Returns the image capture identifier for the specified image capture index. */
uint8_t
pb_image_signal_processor_get_image_capture_identifier (const pb_image_signal_processor_t* isp,
                                                        int image_capture_index);

/** Returns the image capture settings for the specified image capture identifier. The returned 
  * settings structure may be modified to change the configuration of the ISP.*/
pb_image_capture_t*
pb_image_signal_processor_get_image_capture (pb_image_signal_processor_t* isp,
                                             uint8_t image_capture_identifer);

/** Retains the ISP, which means that the reference counter for the object
  * will increase by 1. The caller must make sure that _delete is being called
  * when the object is not needed anymore. Retaining a null pointer has
  * no effect.
  *
  * @param[in] isp is the ISP object
  * @return the retained ISP object
  *
  * Note: The caller is responsible for deleting the retained ISP object.
  */
pb_image_signal_processor_t*
pb_image_signal_processor_retain(pb_image_signal_processor_t* isp);

/** Decreases the reference counter for the ISP by 1. If the reference
  * counter reaches 0 then the object will also be deleted. Deleting a null
  * pointer has no effect.
  *
  * @param[in] isp is the ISP object
  */
void
pb_image_signal_processor_delete(pb_image_signal_processor_t* isp);


/**
  * Encodes a pb_image_signal_processor_t object.
  *
  * @param[in] isp is the pb_image_signal_processor_t object
  * @param[out] data is the encoded data
  * @param[out] data_size is the encoded data size
  *
  * @return PB_RC_OK if succesful, otherwise an error code.
  */
pb_rc_t
pb_image_signal_processor_encode (const pb_image_signal_processor_t* isp,
                                  uint8_t** data,
                                  uint32_t* data_size);

/**
  * Decodes a pb_image_signal_processor_t object
  *
  * @param[in] data is the encoded data
  * @param[in] data_size is the encoded data size
  * @param[out] isp is the decoded pb_image_signal_processor_t object
  *
  * @return PB_RC_OK if succesful, otherwise an error code.
  */
pb_rc_t
pb_image_signal_processor_decode (const uint8_t* data,
                                  uint32_t data_size,
                                  pb_image_signal_processor_t** isp);

typedef void pb_isp_process_start_listener_fn_t (void* context);
typedef void pb_isp_process_end_listener_fn_t (void* context, pb_rc_t status);

/** Registers listeners for the process function. May e.g. be used to measure the time spent in the
  * process function. */
pb_rc_t
pb_image_signal_processor_register_listeners (pb_image_signal_processor_t* isp,
                                              pb_isp_process_start_listener_fn_t* start,
                                              pb_isp_process_end_listener_fn_t* end,
                                              void* context);

void
pb_image_16bit_init (pb_image_16bit_t* image_16bit,
                     uint16_t* pixels,
                     uint16_t rows, uint16_t cols,
                     uint16_t resolution);

void
pb_image_16bit_relate (pb_image_16bit_t* image_16bit,
                       uint16_t full_rows, uint16_t full_cols,
                       uint16_t offset_rows, uint16_t offset_cols);

/** Swaps the bytes of a 16bit-image */
void
pb_image_16bit_convert_from_big_endian(pb_image_16bit_t* image_16bit);


void
pb_image_16bit_set_similar_image (pb_image_16bit_t* image_16bit,
                                  int similar);

#ifdef __cplusplus
}
#endif

#endif /* PB_IMAGE_SIGNAL_PROCESSOR_H */

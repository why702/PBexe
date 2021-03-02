/*
 * Copyright (c) 2008 - 2018, Precise Biometrics AB
 * All rights reserved.
 *
 */

#ifndef PB_IMAGE_PROCESSING_H
#define PB_IMAGE_PROCESSING_H

#include <stdint.h>

#include "pb_types.h"
#include "pb_errors.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Precise external Image Processing library.
 *
 * This file contains multiple image processing filters that can be used 
 * to enhance image quality and reduce noise. 
 *
 * This library should be seen as a toolbox from which an external 
 * customer may experiment with different filters in order to achieve
 * optimal biometric performance, see below, for a specific sensor 
 * as a short term solution. 
 * The long term solution is for Precise Biometrics to design a 
 * specific preprocessor targeting the specific sensor. In this way 
 * Precise Biometrics can optimize the performance both in terms of 
 * latency and biometric performance. 
 *
 * Evaluating optimal biometric performance when changing preprocessing
 * filters could be hard when evaluating using a trained algorithm, 
 * since the training has been made on a fixed set of filters and hence
 * applying a different set of filters may cause unwanted behavior. 
 * This could lead to worse biometric performance even though the new 
 * set of filters would actually have better biometric performance when 
 * trained specifically for them.
 * Therefore, we suggest using the verification type 'compare' when evaluating
 * using PerfEval. The same principal can be done in the customer integration
 * code and for that we refer to pb_match_result.h for usage.
 */

typedef enum pb_ip_normalization_e {
    /** Simple tretching of the image histogram so that 98% of the values are within 0 and 255. */
    PB_IP_NORMALIZATION_STRETCH = 0,
    /** Stretching of the image histogram while ignoring extreme pixel values . */
    PB_IP_NORMALIZATION_ADVANCED_STRETCH,
    /** Successive Mean Quantization Transform (SMQT) normalization. */
    PB_IP_NORMALIZATION_SMQT,
    /** Normalization approach used by Hong, Wan and Jain (1998). */
    PB_IP_NORMALIZATION_HWJ,
    /** Normalization approach used by Hong, Wan and Jain (1998), applied locally. */
    PB_IP_NORMALIZATION_LOCAL_HWJ
} pb_ip_normalization_t;

/** Normalizes the image, i.e. improves the dynamic range of the pixel intensity values. 
  *
  * @param[in] image is the image to be normalized.
  * @param[in] rows is the number of rows in the image.
  * @param[in] cols is the number of columns in the image.
  * @param[in] resolution is the resolution of the image, in dpi.  
  * @param[in] type is the type of normalization method to be applied.
  * @param[out] normalized_image is the returned normalized image. The caller
  *             is responsible of allocating the normalized image. The 
  *             normalized_image and image may reference the same image.
  *
  * @return PB_RC_OK if successful, otherwise an error code. 
  */
pb_rc_t
pb_ip_normalize_image (const uint8_t* image, 
                       uint16_t rows, 
                       uint16_t cols,
                       uint16_t resolution, 
                       pb_ip_normalization_t type,
                       uint8_t* normalized_image);

typedef enum pb_ip_noise_e {
    /** White (pixel) noise. */
    PB_IP_NOISE_WHITE = 0,
    /** Impulse noise, i.e. additative noise that occurs for a specific 
      * number of pixels in a row. */
    PB_IP_NOISE_IMPULSE,
    /** Row noise, i.e. additative noise present for a complete row. */
    PB_IP_NOISE_ROW,
    /** Column noise, i.e. additative noise present for a complete column. */
    PB_IP_NOISE_COLUMN,
    /** Moir� noise, i.e. high frequency Moir� patterns. */
    PB_IP_NOISE_MOIRE,
} pb_ip_noise_t;

/** Denoises the image, i.e. removes noise from the image. 
  *
  * @param[in] image is the image to be denoised.
  * @param[in] rows is the number of rows in the image.
  * @param[in] cols is the number of columns in the image.
  * @param[in] resolution is the resolution of the image, in dpi.  
  * @param[in] type is the type of noise that should be removed.
  * @param[in] x is dependent on type.
  *            For type = PB_IP_NOISE_IMPULSE x is the length of the 
  *            impulse noise. 
  *            Otherwise, x will not be used. 
  * @param[out] denoised_image is the returned denoised image. The caller
  *             is responsible of allocating the denoised image. The 
  *             denoised_image and image may reference the same image.
  *
  * @return PB_RC_OK if successful, otherwise an error code. 
  */
pb_rc_t
pb_ip_denoise_image (const uint8_t* image, 
                     uint16_t rows, 
                     uint16_t cols, 
                     uint16_t resolution, 
                     pb_ip_noise_t type,
                     int x,
                     uint8_t* denoised_image);

/** Filters the image using a bandpass filter, thus removing high and low 
  * frequency noise. 
  *
  * @param[in] image is the image to be filtered.
  * @param[in] rows is the number of rows in the image.
  * @param[in] cols is the number of columns in the image.
  * @param[in] resolution is the resolution of the image, in dpi.  
  * @param[out] filtered_image is the returned filtered image. The caller
  *             is responsible of allocating the filtered image. The 
  *             filtered_image and image may reference the same image.
  *
  * @return PB_RC_OK if successful, otherwise an error code. 
  */
pb_rc_t
pb_ip_bandpass_filter_image (const uint8_t* image,
                             uint16_t rows, 
                             uint16_t cols, 
                             uint16_t resolution, 
                             uint8_t* filtered_image);

typedef enum pb_ip_lowpass_e {
    /** Blur image using the kernel [1 1 1; 1 1 1; 1 1 1] / 9. */
    PB_IP_LOWPASS_SIMPLE_BLUR = 0,
    /** Blur image using the kernel [1 2 1; 2 4 2; 1 2 1] / 16. Approximately 
      * a small gaussian blur. */
    PB_IP_LOWPASS_WEIGHTED_BLUR,
    /** Gaussian blur. 
      * NOTE! NOT IMPLEMENTED YET. */
    PB_IP_GAUSSIAN_BLUR,
    /** Wiener filtering. */
    PB_IP_LOWPASS_WIENER,
    /** Lowpass cutoff frequency filtering. Removes all frequencies above a 
      * certain cutoff frequency. */
    PB_IP_LOWPASS_CUTOFF_FREQUENCY,
} pb_ip_lowpass_t;

/** Filters the image using a lowpass filter to reduce high freqency noise or
  * blur the image.
  *
  * @param[in] image is the image to be filtered.
  * @param[in] rows is the number of rows in the image.
  * @param[in] cols is the number of columns in the image.
  * @param[in] resolution is the resolution of the image, in dpi.  
  * @param[in] type is the type of lowpass filter to be applied. 
  * @param[in] x is dependent on type. 
  *            For type = PB_IP_LOWPASS_WIENER it defines the neighbourhood 
  *            size for the wiener filter, as neighbourhood = (2 * x + 1).
  *            E.g. x = 1 results in a 3x3 neighbourhood, 2 in a 5x5. 
  *            For type = PB_IP_LOWPASS_CUTOFF_FREQUENCY it defines the cutoff
  *            frequency to be used in lines/mm, shifted 8 bits. The fingerprint 
  *            information are primarily between 1.5 and 5 lines/mm. 
  *            Otherwise, x will not be used. 
  * @param[out] filtered_image is the returned filtered image. The caller
  *             is responsible of allocating the filtered image. The 
  *             filtered_image and image may reference the same image.
  *
  * @return PB_RC_OK if successful, otherwise an error code. 
  */
pb_rc_t 
pb_ip_lowpass_filter_image (const uint8_t* image,
                            uint16_t rows, 
                            uint16_t cols, 
                            uint16_t resolution, 
                            pb_ip_lowpass_t type,
                            int x,
                            uint8_t *filtered_image);

typedef enum pb_ip_highpass_e {
    /** Sharpens the image according to O = I + k * (I - blur(I)). */
    PB_IP_HIGHPASS_SHARPEN = 0,
    /** Highpass cutoff frequency filtering. Removes all frequencies below a 
      * certain cutoff frequency using a sharp sinus transition. 
      * Note: For low cutoff frequencies this could lead to ringings, in that 
      * case use the PB_IP_HIGHPASS_CUTOFF_FREQUENCY_GAUSS instead. */
    PB_IP_HIGHPASS_CUTOFF_FREQUENCY,    
    /** Highpass cutoff frequency filtering. Removes all frequencies below a 
      * certain cutoff frequency using a gaussian transition. */
    PB_IP_HIGHPASS_CUTOFF_FREQUENCY_GAUSS,
} pb_ip_highpass_t;

/** Filters the image using a highpass filter to reduce low freqency noise or
  * sharpen the image.
  *
  * @param[in] image is the image to be filtered.
  * @param[in] rows is the number of rows in the image.
  * @param[in] cols is the number of columns in the image.
  * @param[in] resolution is the resolution of the image, in dpi.  
  * @param[in] type is the type of highpass filter to be applied. 
  * @param[in] x is dependent on type. 
  *            For type = PB_IP_HIGHPASS_SHARPEN it controls the amount of 
  *            sharpening that will occur, corresponding to the k parameter 
  *            above. A lower value results in less sharpening. 16 is a good 
  *            default value to use. x can be in the range of [0, 255]. 
  *            For type = PB_IP_HIGHPASS_CUTOFF_FREQUENCY<_GAUSS> it defines the 
  *            cutoff frequency to be used in lines/mm, shifted 8 bits. The  
  *            fingerprint information are primarily between 1.5 and 5 lines/mm. 
  *            Otherwise, x will not be used. 
  * @param[out] filtered_image is the returned filtered image. The caller
  *             is responsible of allocating the filtered image. The 
  *             filtered_image and image may reference the same image.
  *
  * @return PB_RC_OK if successful, otherwise an error code. 
  */
pb_rc_t 
pb_ip_highpass_filter_image (const uint8_t* image,
                             uint16_t rows, 
                             uint16_t cols, 
                             uint16_t resolution, 
                             pb_ip_highpass_t type,
                             int x,
                             uint8_t *filtered_image);

#ifdef __cplusplus
}
#endif

#endif /* PB_IMAGE_PROCESSING_H */

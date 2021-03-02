#ifndef PB_EMBEDDED_IMAGE_PROCESSING_H
#define PB_EMBEDDED_IMAGE_PROCESSING_H

#include "pb_errors.h"
#include "pb_types.h"
#include "pb_preprocessorI.h"
#ifdef __cplusplus
extern "C" {
#endif

/**
  * Embedded Image Processing Module
  * 
  * Contains light weight (computation speed, memory) optimized image processing functionality
  *
  * Image downscaling example (heap memory) without memory fragmentation:
  *     uint16_t rows = 160;
  *     uint16_t cols = 160;
  *     uint16_t resolution = 500;
  *
  *     uint16_t scaled_rows = 0;
  *     uint16_t scaled_cols = 0;
  *     uint16_t scaled_resolution = 250;
  *
  *     uint16_t scaled_size = 0;
  *
  *     pb_embedded_ip_compute_downscaled_image_size(rows, cols, resolution, scaled_resolution, &scaled_size);
  *
  *     // The parameter buffer_size can be chosen to be greater or equal to the
  *     // output value scaled_size from pb_embedded_ip_compute_downscaled_image_size
  *     uint16_t buffer_size = scaled_size;
  *
  *     // Located at bottom of heap
  *     uint8_t* scaled_pixels = malloc(buffer_size);
  *
  *     // Located after scaled_pixels as we want to free this memory without causing fragmentation
  *     uint8_t* pixels = malloc(rows * cols);
  *
  *     // Perform image capture into pixels
  *     image_capture(pixels); // Pseudo-code function name
  *
  *     pb_rc_t res = pb_embedded_ip_downscale_image(pixels, resolution, rows, cols,
  *                                scaled_pixels, scaled_resolution, scaled_rows, scaled_cols, buffer_size);
  *
  *     // Free the original pixel buffer
  *     free(pixels);
  *     pixels = NULL;
  *
  *     if (res == PB_RC_OK) {
  *         // Do extraction on scaled_pixels, some pseudo-code
  *         pb_template_t* template == NULL;
  *         pb_algorithm_extract_embedded(..., scaled_pixels, rows, cols, scaled_resolution, ..., &template);
  *         // ...
  *     }
  *
  *     free(scaled_pixels);
  *     scaled_pixels = NULL;
  */

/** 
  * Bandpasses the image to remove noise outside frequency domain of the fingerprint
  *
  * @param[in] pixels is the pixels of the image to be enhanced.
  * @param[in] rows is the number of rows in the image.
  * @param[in] cols is the number of columns in the image.
  * @param[in] resolution is the resolution of the image, in dpi.
  * @param[out] filtered_pixels is the returned enhanced pixels. 
  *
  * @return PB_RC_OK if successful, otherwise an error code.
 */
pb_rc_t
pb_embedded_ip_bandpass(const uint8_t* pixels,
                        uint16_t rows,
                        uint16_t cols,
                        uint16_t resolution,
                        uint8_t* filtered_pixels);

/**
  * Preprocessor interface that can be used for above image processing for example from PerfEval or as
  * setting to pb_algorithm.
*/
extern pbif_const pb_preprocessorI pb_embedded_ip_bandpass_preprocessor;


/**
  * Blur the image. This function can be called multiple times on the same
  * pixel area in order to apply a stronger blur.
  *
  * @param[in] pixels is the pixels of the image to be enhanced.
  * @param[in] rows is the number of rows in the image.
  * @param[in] cols is the number of columns in the image.
  * @param[in] resolution is the resolution of the image, in dpi.
  * @param[out] filtered_pixels is the returned enhanced pixels. .
  *
  * Note: filtered_pixels may point to the same pixel area as the pointer 'pixels'
  *       if in-place filtering is required in order to reduce memory footprint.
  * 
  * @return PB_RC_OK if successful, otherwise an error code.
 */
pb_rc_t
pb_embedded_ip_blur(const uint8_t* pixels,
                    uint16_t rows,
                    uint16_t cols,
                    uint16_t resolution,
                    uint8_t* filtered_pixels);

/**
  * Preprocessor interface that can be used for above image processing for example from PerfEval or as
  * setting to pb_algorithm.
*/
extern pbif_const pb_preprocessorI pb_embedded_ip_blur_preprocessor;

/**
  * Local image normalization.
  *
  * @param[in] pixels is the pixels of the image to be enhanced.
  * @param[in] rows is the number of rows in the image.
  * @param[in] cols is the number of columns in the image.
  * @param[in] resolution is the resolution of the image, in dpi.
  * @param[out] filtered_pixels is the returned enhanced pixels. The caller is
  *             responsible for allocating the memory for the filtered_pixels
  *             and it must be the same size as pixels.
  * 
  * Note: filtered_pixels may point to the same pixel area as the pointer 'pixels'
  *       if in-place filtering is required in order to reduce memory footprint.
  *
  * @return PB_RC_OK if successful, otherwise an error code.
 */
pb_rc_t
pb_embedded_ip_normalize(const uint8_t* pixels,
                         uint16_t rows,
                         uint16_t cols,
                         uint16_t resolution,
                         uint8_t* filtered_pixels);


/**
  * Preprocessor interface that can be used for above image processing for example from PerfEval or as
  * setting to pb_algorithm.
*/
extern pbif_const pb_preprocessorI pb_embedded_ip_normalize_preprocessor;

/**
  * Impulse noise removal.
  *
  * @param[in] pixels is the pixels of the image to be enhanced.
  * @param[in] rows is the number of rows in the image.
  * @param[in] cols is the number of columns in the image.
  * @param[in] resolution is the resolution of the image, in dpi.
  * @param[in] filter_type is the type of impulse filtering to be applied, the
  *            following options are available:
  *            0 - Additive noise that occur every 'impulse_length' pixel
  *            1 - Additive noise in rows
  *            2 - Additive noise in columns
  *            3 - High frequency moire patterns
  * @param[in] impulse_length is the impulse length for the filter, only applicable
  *            for type 0 filtering
  * @param[out] filtered_pixels is the returned enhanced pixels. The caller is
  *             responsible for allocating the memory for the filtered_pixels
  *             and it must be the same size as pixels.
  *
  * Note: filtered_pixels may point to the same pixel area as the pointer 'pixels'
  *       if in-place filtering is required in order to reduce memory footprint.
  *
  * @return PB_RC_OK if successful, otherwise an error code.
 */
pb_rc_t
pb_embedded_ip_denoise_impulse(const uint8_t* pixels,
                               uint16_t rows,
                               uint16_t cols,
                               uint16_t resolution,
                               uint16_t filter_type,
                               uint16_t impulse_length,
                               uint8_t* filtered_pixels);

/**
  * Preprocessor interface that can be used for above image processing for example from PerfEval or as
  * setting to pb_algorithm.
  Session settings that can be used:
      "pb.preprocessor.denoise.filter_type"     filter_type default 0
      "pb.preprocessor.denoise.impulse_length"  impulse_length deafault 1
*/
extern pbif_const pb_preprocessorI pb_embedded_ip_impulse_preprocessor;

/**
  * White noise removal.
  *
  * @param[in] pixels is the pixels of the image to be enhanced.
  * @param[in] rows is the number of rows in the image.
  * @param[in] cols is the number of columns in the image.
  * @param[in] resolution is the resolution of the image, in dpi.
  * @param[out] filtered_pixels is the returned enhanced pixels. The caller is
  *             responsible for allocating the memory for the filtered_pixels
  *             and it must be the same size as pixels.
  *
  * Note: filtered_pixels may point to the same pixel area as the pointer 'pixels'
  *       if in-place filtering is required in order to reduce memory footprint.
  * 
  * @return PB_RC_OK if successful, otherwise an error code.
 */
pb_rc_t
pb_embedded_ip_denoise_white(const uint8_t* pixels,
                             uint16_t rows,
                             uint16_t cols,
                             uint16_t resolution,
                             uint8_t* filtered_pixels);

/**
  * Preprocessor interface that can be used for above image processing for example from PerfEval or as
  * setting to pb_algorithm.
*/
extern pbif_const pb_preprocessorI pb_embedded_ip_whitenoise_preprocessor;

/**
  * Pre-compute the size of a scaled image
  *
  * @param[in] rows is the number of rows in the image to be scaled.
  * @param[in] cols is the number of columns in the image to be scaled.
  * @param[in] resolution resolution of image to be scaled (in DPI).
  * @param[in] scaled_resolution resolution of the resulting scaled image (in DPI).
  * @param[out] scaled_size required size of the resulting scaled image pixel buffer.
  *
  * @return PB_RC_OK if successful, otherwise an error code.
 */
pb_rc_t
pb_embedded_ip_compute_downscaled_image_size(uint16_t rows,
                                             uint16_t cols,
                                             uint16_t resolution,
                                             uint16_t scaled_resolution,
                                             uint32_t* scaled_size);

/**
  * Downscale image into a new resolution, given scaling parameters. The parameter
  * buffer_size should be set to the pre-computed value 'scaled_size' from calling
  * pb_embedded_ip_compute_downscaled_image_size() at minimum. The caller is responsible
  * for allocating both pixels and scaled_pixels.
  * 
  * Note: In order to reduce memory consumption the scaled_pixels should be located
  *       at the bottom of the memory used (allocated earliest of the two). See example
  *       at the top of this file for more details on how to do this.
  *
  * @param[in] pixels is the pixels of the image to be scaled.
  * @param[in] resolution resolution of image to be scaled (in DPI).
  * @param[in] rows is the number of rows in the image.
  * @param[in] cols is the number of columns in the image.
  * @param[out] scaled_pixels is the returned scaled pixels.
  * @param[in] scaled_resolution resolution of resulting scaled image (in DPI).
  * @param[out] scaled_rows is the number of rows in the resulting scaled image.
  * @param[out] scaled_cols is the number of columns in the resulting scaled image.
  * @param[in] buffer_size is the size of the scaled image buffer to store the 
  *            resulting scaled image in.
  *
  * @return PB_RC_OK if successful, otherwise an error code.
 */
pb_rc_t
pb_embedded_ip_downscale_image(const uint8_t* pixels,
                               uint16_t resolution,
                               uint16_t rows,
                               uint16_t cols,
                               uint8_t* scaled_pixels,
                               uint16_t scaled_resolution,
                               uint16_t* scaled_rows,
                               uint16_t* scaled_cols,
                               uint32_t buffer_size);

/**
  * Compute fingerprint quality statistics: quality, area and condition.
  *
  * @param[in] pixels is the pixels of the image to compute statistics of.
  * @param[in] rows is the number of rows in the image.
  * @param[in] cols is the number of columns in the image.
  * @param[in] quality is the returned quality of the image. The
  *            value range from 0 (low quality) to 100 (high quality).
  * @param[in] area is the returned area of the fingerprint, in
  *            pixels.
  * @param[in] condition is the returned condition of the fingerprint,
  *            ranging from 0 (soaked) to 100 (bone dry).
  * 
  * @return PB_RC_OK if successful, otherwise an error code.
 */
pb_rc_t pb_embedded_ip_image_quality(const uint8_t* pixels,
                                     uint16_t rows,
                                     uint16_t cols,
                                     uint32_t* quality,
                                     uint32_t* area,
                                     uint32_t* condition);

/**
  * Compute fingerprint signal to noise ratio.
  *
  * @param[in] pixels is the pixels of the image to compute statistics of.
  * @param[in] rows is the number of rows in the image.
  * @param[in] cols is the number of columns in the image.
  * @param[in] resolution is the resolution of the image, in dpi.
  * @param[in] signal_to_noise_ratio is the signal to noise ratio, normalized
  *            to fractional UQ5.3 representation.
  * 
  * @return PB_RC_OK if successful, otherwise an error code.
 */
pb_rc_t pb_embedded_ip_signal_to_noise_ratio(const uint8_t* pixels,
                                             uint16_t rows,
                                             uint16_t cols,
                                             uint16_t resolution,
                                             uint32_t* signal_to_noise_ratio);


#ifdef __cplusplus
}
#endif

#endif /* PB_EMBEDDED_IMAGE_PROCESSING_H */

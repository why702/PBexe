/*
 * Copyright (c) 2008 - 2013, Precise Biometrics AB
 * All rights reserved.
 *
 */

#ifndef PB_RAWIMAGE_CONVERTERI_H
#define PB_RAWIMAGE_CONVERTERI_H

#include "pb_errors.h"
#include "pb_session.h"
#include "pb_image_t.h"

#ifdef __cplusplus
extern "C" {
#endif

/// Converts a raw image representation into a pb_image object.
/** The format of the raw data is described by each implementation.
  * The BMF natural image format is raw 8-bit grayscale and is the
  * representation in the pb_image_t object. Hence special conversion
  * is only needed in cases such as 16-bit pixels.
  *
  * A converter that finds the data corrupt, e.g. to little or to much
  * data given the parameters should return with an error code.
  *
  * The image resolution should be passed on to pb_image rather than to
  * be used in the conversion process.
  *
  * @param[in] session is the session object.
  * @param[in] data is the raw data/pixels of the image to be converted.
  *            Note that input data is only required to be byte aligned even
  *            if the native pixel size is wider.
  * @param[in] data_size is the amount of pixeldata in of the image to be converted.
  * @param[in] rows is the number of rows in the image.
  * @param[in] cols is the number of columns in the image.
  * @param[in] resolution is the resolution of the image, in dpi.
  * @param[out] image is the converted image.
  *
  * @return PB_RC_OK if successful, or an error code. 
  */
typedef pb_rc_t pb_rawimage_converterI_fn_t (pb_session_t* session,
                                             const uint8_t* data,
                                             uint32_t data_size,
                                             uint16_t rows, 
                                             uint16_t cols,
                                             uint16_t resolution,
                                             pb_image_t** image);
    

/// Raw image converter interface
/** 
  * An implementation of image conversion may address specific
  * conversion issues in the data format or e.g. how to reduce 16-bit
  * wide dynamic range into 8-bit while preserving the best fingerpint
  * representation.
  *
  * Note that the conversion may include some image processing, but
  * major processing should typically be done in a pb_preprocessorI
  * implementation which can be applied after conversion.
  *
  */
typedef struct {
    /// Se definition of pb_rawimage_converterI_fn_t()
    pb_rawimage_converterI_fn_t* convert_image;
} pb_rawimage_converterI;

#ifdef __cplusplus
}
#endif

#endif /* PB_RAWIMAGE_CONVERTERI_H */

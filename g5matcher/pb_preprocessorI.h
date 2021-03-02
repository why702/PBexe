/*
 * Copyright (c) 2008 - 2013, Precise Biometrics AB
 * All rights reserved.
 *
 */

#ifndef PB_PREPROCESSORI_H
#define PB_PREPROCESSORI_H

#include "pb_errors.h"
#include "pb_session.h"
#include "pb_image_t.h"

#ifdef __cplusplus
extern "C" {
#endif

/** Segmentation defines. */
#define PB_PREPROCESSOR_FINGERPRINT 255
#define PB_PREPROCESSOR_BACKGROUND 0

/** Enhances the raw image by improving image quality and reducing
  * noise. 
  *
  * @param[in] session is the session object.
  * @param[in] pixels is the pixels of the image to be enhanced.
  * @param[in] segmentation is the input segmentation of the image to be enhanced.
  *            background (0) and fingerprint (> 0).
  *            segmentation may be Null if no input segmentation is available.
  * @param[in] rows is the number of rows in the image. 
  * @param[in] cols is the number of columns in the image. 
  * @param[in] resolution is the resolution of the image, in dpi.
  * @param[in] relation is the relation to the full image, in case the image was
  *            cropped. 
  * @param[out] enhanced_pixels is the returned enhanced pixels. The caller is 
  *             responsible for allocating the memory for the enhanced_pixels 
  *             and it must be the same size as pixels. 
  * @param[out] enhanced_segmentation is the returned segmentation of the image into 
  *             fingerprint (> 0) and background (0) for the image. 
  *             Returning a segmentation is optional for the implementation, i.e.
  *             the implementation may only enhance the image, without computing
  *             a segmentation. 
  *             The caller is responsible for deallocating the returned 
  *             segmentation, if any. 
  * @param[out] image_analysis value representing some image characteristic (optional)
  *
  * @return PB_RC_OK if successful, or an error code. 
  */
typedef pb_rc_t
pb_preprocessorI_enhance_image_fn_t (pb_session_t* session,
                                     const uint8_t* pixels,
                                     const uint8_t* segmentation,
                                     uint16_t rows,
                                     uint16_t cols,
                                     uint16_t resolution,
                                     const pb_image_relation_t* relation,
                                     uint8_t* enhanced_pixels,
                                     uint8_t** enhanced_segmentation,
                                     uint32_t* image_analysis);

/// Fingerprint image preprocessor interface
/** 
  * The purpose of a preprocessor implementation is to preprocess the raw
  * fingerprint image to enhance image quality and reduce noise. The 
  * enhancement shall focus on things that are specific to a certain sensor. 
  * More generic image enhancement shall be taken care of by the extractor 
  * module. 
  *
  * Typical noise that could be handled by a preprocessor module are things 
  * like:
  *  - Striped patterns, e.g. if certain rows/columns is a bit darker or
  *    brighter than other rows/columns. 
  *  - Histogram stretching, e.g. if images from a certain sensors have 
  *    really poor dynamic range.
  *  - Removal of dead pixels, e.g. if certain pixels always gets the same
  *    value due to e.g. hardware issues. 
  *  - Removal of high and low frequency noise. 
  * 
  * Since different sensors produce different images in terms of image quality 
  * and noise, a preprocessing step might be required for optimal performance.
  */
typedef struct {
    /// Se definition of pb_preprocessorI_enhance_image_fn_t()
    pb_preprocessorI_enhance_image_fn_t* enhance_image;
} pb_preprocessorI;

#ifdef __cplusplus
}
#endif

#endif /* PB_PREPROCESSORI_H */

/*
 * Copyright (c) 2008 - 2016, Precise Biometrics AB
 * All rights reserved.
 *
 */

#ifndef PB_TEMPLATE_H
#define PB_TEMPLATE_H

#include <stddef.h>
#include <stdint.h>

#include "pb_types.h"
#include "pb_image_t.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Note! It is the callers responsibility to make sure that
 * all template input parameters are valid, i.e. not null (0).
 * Passing a null template parameter to a function will result
 * in a runtime error! */

/* Template types. The values are split into four categories:
 *
 * 1-63     Standardized templates.
 * 64-127   Precise proprietary templates.
 * 128-191  RFU, Reserved for Future Use.
 * 192-255  Application specific templates, not defined.
 *
 * There are 4 already defined external application specific template types.
 * If these are not enough it is possible to use the other values 196-255 as
 * well.
 *
 * Note: The already defined values MUST NOT CHANGE. Only new values may be
 * added!
 */
typedef enum pb_template_type_e {
    PB_TEMPLATE_UNKNOWN             = 0,

    /** The ISO 19794-2 Record Format (v3) from the 2011 standard. */
    PB_TEMPLATE_ISO                 = 1,
    /** The ISO 19794-2 Record Format (v2) from the 2005 standard
     * Requests for extraction of V2 templates will result in a template
     * type of PB_TEMPLATE_ISO. */
    PB_TEMPLATE_ISO_V2              = 4,

    /** The ISO 19794-2 Compact Card Format from the 2011 standard */
    PB_TEMPLATE_ISO_COMPACTCARD     = 2,
    /** The ISO 19794-2 Compact Card Format from the 2005 standard
     * as interpreted by Precise. This format should be considered
     * depricated unless backwards compatibility requires it.
     * Requests for extraction of V2 templates will result in a template
     * type of PB_TEMPLATE_ISO_COMPACTCARD. */
    PB_TEMPLATE_ISO_COMPACTCARD_V2  = 5,

    /** The ANSI-378 Record Format (v3) from the 2009 standard. */
    PB_TEMPLATE_ANSI                = 3,
    /** The ANSI-378 Record Format (v2) from the 2004 standard.
     * Requests for extraction of V2 templates will result in a template
     * type of PB_TEMPLATE_ANSI. */
    PB_TEMPLATE_ANSI_V2             = 6,

    /** DEPRECATED, Use BIOMATCH3 types instead.
      * The Precise Flex Record Format. */
    PB_TEMPLATE_PB_FLEX             = 64,
    /** The Precise Turbine Format, based on ISO. */
    PB_TEMPLATE_PB_TURBINE          = 65,
    /** The template for spectral (direction and frequency) data. */
    PB_TEMPLATE_PB_SPECTRAL         = 66,
    /** The format for the Embedded Hybrid Matcher version 2, based on
      * an ISO_COMPACTCARD template extended with SPECTRAL data. */
    PB_TEMPLATE_PB_EHM              = 67,
    /** The Precise BioMatch 3 enrollment template. The template extracted for
      * enrollment purpose. */
    PB_TEMPLATE_PB_BIOMATCH3_ENROLLMENT = 68,
    /** The Precise BioMatch 3 verification template. The template extracted for
      * verification purpose. */
    PB_TEMPLATE_PB_BIOMATCH3_VERIFICATION = 69,
    /** The Precise BioMatch 3 header template. Contains only limited data used
      * for alignment purposes by the extractor. */
    PB_TEMPLATE_PB_BIOMATCH3_HEADER = 70,
    /** The Precise image template, consisting of an image (see pb_image.h for
      * encoding and decoding functions). */
    PB_TEMPLATE_PB_IMAGE            = 71,
    /** The Precise fusion template, consisting of an ISO_COMPACTCARD template,
      * a SPECTRAL template and an IMAGE template. */
    PB_TEMPLATE_PB_FUSION           = 72,
    /** The legacy proprietary format for the Embedded Hybrid Matcher.
     * This format is depricated, instead use the PB_TEMPLATE_PB_EHM format.
     * The two formats are compatible and requesting a V1 template for
     * extraction will result in a template type of PB_TEMPLATE_PB_EHM. */
    PB_TEMPLATE_PB_EHM_V1           = 73,
    /** A high resolution version of the EHM format. This produces larger
     * templates but may increase biometric performance for tiny sensors.
     * Requesting this template type will result in a template type of
     * PB_TEMPLATE_PB_EHM. */
    PB_TEMPLATE_PB_EHM_HR           =  74,
    /** A high resolution version of the FUSION format corresponding to
     * the EHM_HR variant. Requesting this template type will result in a
     * template type of PB_TEMPLATE_PB_FUSION. */
    PB_TEMPLATE_PB_FUSION_HR        =  75,
    /** The format for the Hybrid MOC Matcher, based on an
      * ISO_COMPACTCARD template extended with SPECTRAL data. It
      * contains the same information as a EHM v2 template, with the
      * spectral template converted to the spectral matcher data
      * format. */
    PB_TEMPLATE_PB_HMOC             = 76,
    /** The template for Spectral HR data. */
    PB_TEMPLATE_PB_SPECTRAL_HR      = 77,
    /** The template for Neo data. */
    PB_TEMPLATE_PB_NEO              = 78,
    /** The template for Cardo data. */
    PB_TEMPLATE_PB_CARDO            = 79,
    /** Format for template container that can hold multiple templates of other
      * formats. */
    PB_TEMPLATE_PB_MULTIPLE         = 80,

    /** The template for ridge curvature. */
    PB_TEMPLATE_PB_RIDGE_CURVATURE  = 83,

    /* Generic proprietary tag-length-value format. Undefined contents.
     * This is the template type to use for verification. Any other type is
     * incomplete w.r.t. matching and dynamic update.
     */
    PB_TEMPLATE_PB_GENERIC          = 84,

    /** The template for alignment vectors. */
    PB_TEMPLATE_PB_ALIGNMENT_VECTOR = 85,

    /** The template for generic meta data. */
    PB_TEMPLATE_PB_METADATA         = 86,

    /** The template for Liveness data. */
    PB_TEMPLATE_PB_LIVENESS         = 87,

    /** Storage template for multiple subtemplates. */
    PB_TEMPLATE_PB_STORAGE          = 88,

    /** Storage template for multiple subtemplates. */
    PB_TEMPLATE_PB_MATCHER_1        = 89,

    /** External application specific template types. */
    PB_TEMPLATE_EXTERNAL1           = 192,
    PB_TEMPLATE_EXTERNAL2           = 193,
    PB_TEMPLATE_EXTERNAL3           = 194,
    PB_TEMPLATE_EXTERNAL4           = 195,

    PB_TEMPLATE_UPPER_GUARD         = 0x7FFFFFFF, // Ensure at least 32bits
    PB_TEMPLATE_LOWER_GUARD         = -0x7FFFFFFF, // Ensure handles negative numbers
} pb_template_type_t;

/** A template object holds a fingerprint template. A template is a
  * compact representation of a fingerprint and is the result of a
  * fingerprint feature extraction and can be used to match fingerprints
  * using a fingerprint matcher or be stored in a database along with
  * user and finger identification for later use.
  *
  * Templates can be of different types. There are standardized template
  * types, proprietary types and application specific types. A certain
  * extractor may e.g. be able to create templates of only one type while
  * others may be configurable to create different template types for
  * various purposes, like Math-on-Card.
  *
  * Likewise a matcher may e.g. only accept templates of a certain type
  * while other matchers may accept a range of different template types
  * and internally transform these during the matching process.
  */
typedef struct pb_template_st pb_template_t;

/** Allocates a template object and sets the reference counter to 1.
  *
  * @param[in] type is the template type, see PB_TEMPLATE_X.
  *     Note that some types are virtual and will be handled
  *     as if the corresponding base type was given, see
  *     _get_type().
  * @param[in] data is the template data encoded in the format
  *     associated with the template type. The template object
  *     creates a copy of the input data. It is assumed that
  *     the creator of the template knows how to encode the
  *     template and the user of the template knows how to
  *     decode it.
  * @param[in] data_size is the size of the data, in bytes.
  *
  * @return the allocated template object if successful, else 0.
  */
pb_template_t*
pb_template_create (pb_template_type_t type,
                    const uint8_t* data,
                    uint32_t data_size);

/** Allocates a template object without allocating and copying template
  * data and instead use direct memory referencing, mr.  The
  * use-case for this alternative create function is in systems with
  * low RAM but comes with some additional complexity for an
  * application to consider such that the object and data may
  * be considered non-const despite of the declaration.
  *
  * The template data must remain valid throughout the existence of
  * the template object. A complicated matter is that different parts
  * an application may retain an object for later use.
  *
  * In most simple applications there is however a bounded lifetime,
  * enrollment template data is e.g. used during verification and then
  * operation is ended. Hence the following scheme should be followed.
  *
  * @code
  *   Fetch_Template_Data(); // Or just get pointer in Flash.
  *   enr = pb_template_create_mr(_PB_EHM, TemplateData, TemplateSize);
  *   extractor->extract_template(image, &ver, ...)
  *   verifier->verify_templaye_template(enr, ver, ....)
  *   assert(pb_template_get_num_refs(enr) == 1));
  *   pb_template_detete(enr);
  * @endcode
  *
  * See also the extended function pb_template_create_mre(), calling
  * the extended function with all extra parameters set to 0 is
  * equivalent.
  *
  * NOTE! The default property mr_const is 0 (false) which means
  *       that pb_multitemplate_decode() and pb_multitemplate_encode()
  *       are allowed to use memory references instead of allocating new
  *       data blobs. If this behavior is not wanted use the _create_mre()
  *       function or change the property using _set_mr_const().
  *
  * @return the allocated template object if successful, else 0.
  */
pb_template_t*
pb_template_create_mr (pb_template_type_t type,
                       const uint8_t* data,
                       uint32_t data_size);

/** Extended version of the _create_mr() function with additional
  * functionality. Calling this function will all extra parameters
  * set to 0 is equivalent of using the _create_mr() function.
  *
  * The extended function adds a callback release function which
  * purpose is to allows using another another BMF object or heap
  * allocated memory as data source which in turn will be deleted
  * when the template object is deleted. This ficticious example
  * shows the idea.
  *
  * @code
  *   enr_template = ReadTemplate(); // Could also be mr-based
  *   pb_template_retain(enr_template); // Protect from beeing deleted
  *   data = pb_template_get_data(enr_template); // Internal data
  *   subtemplate = pb_template_create_mre(
  *              type, data + 100, 200, 0, 0
  *              (pb_memref_release_fn_t*) pb_template_delete, enr_template);
  *
  *   pb_template_delete(enr_template); // safe as previously retained.
  *   DoOperation(subtemplate);
  *   pb_template_delete(subtemplate); // Will also cascade delete enr_template
  * @endcode
  *
  * The same mechanism can be used to prevent double allocation
  * when using heap memory that should be freed:
  *
  * @code
  *   data = malloc(size);
  *   ReadTemplate(data);
  *   template = pb_template_create_mre(
  *              type, data, size, 0, 0
  *              free, template);
  *   pb_template_delete(template); // will free(data) also
  * @endcode
  *
  * @param[in] type is the template type, see PB_TEMPLATE_X.
  *                     Note that some types are virtual and will be handled
  *                     as if the corresponding base type was given, see
  *                     _get_type().
  * @param[in] data is the template data encoded in the format
  *                     associated with the template type. The template object
  *                     creates a copy of the input data. It is assumed that
  *                     the creator of the template knows how to encode the
  *                     template and the user of the template knows how to
  *                     decode it.
  * @param[in] data_size is the size of the data, in bytes.
  * @param[in] mr_const If 0 then it is allowed to break the const
  *                     of a template object. The template data
  *                     will never be modified. This is needed to allow
  *                     efficient unpacking of multitemplates in a
  *                     situation where only a const object reference
  *                     is available. Note that using this e.g. prevents
  *                     sharing the template between threads.
  * @param[in] mr_release Optional release callback, called when the
  *                     last reference to the template object is deleted.
  *                     On application level it is better to follow
  *                     the scheme described earlier by checking
  *                     the reference counter value. For library
  *                     development it may used to build a chain of
  *                     BMF objects.
  * @param[in] mr_release_obj Callback value passed to the optional
  *                     release function.
  *
  * @return the allocated template object if successful, else 0.
  */
pb_template_t*
pb_template_create_mre (pb_template_type_t type,
                        const uint8_t* data,
                        uint32_t data_size,
                        uint32_t mr_const,
                        pb_memref_release_fn_t* mr_release,
                        void* mr_release_obj);

/** Retains the object, which means that the reference counter for the object
  * will increase by 1. The caller must make sure that _delete is being called
  * when the object is not needed anymore. Retaining a null pointer has
  * no effect. */
pb_template_t*
pb_template_retain (pb_template_t* template_);

/** Decreases the reference counter for the object by 1. If the reference
  * counter reaches 0 then the object will also be deleted. Deleting a null
  * pointer has no effect. */
void
pb_template_delete (pb_template_t* template_);

/** Returns the number of references to this object. */
uint32_t pb_template_get_num_refs (const pb_template_t* template_);

/** Creates a copy of the template. */
pb_template_t*
pb_template_copy (const pb_template_t* template_);

/** Tells if a template is a copy of another template. */
int
pb_template_is_copy (const pb_template_t* template1,
                     const pb_template_t* template2);

/** Returns the template type. The template type defines
  * how the template is encoded, see PB_TEMPLATE_X.
  * In case the template was created using a template subtype
  * (virtual type), its corresponding base type is returned; e.g. if
  * created as PB_TEMPLATE_ISO_V2 then PB_TEMPLATE_ISO is returned.
  */
pb_template_type_t
pb_template_get_type (const pb_template_t* template_);

/** Sets a new template type for the template. Use this function with care
  * since the template type is often the only way of knowing which type
  * of template that is encoded in the template data. */
void
pb_template_set_type (pb_template_t* template_,
                      pb_template_type_t type);

/** Returns a reference to the template data. Note that it is
  * assumed that the caller knows how to decode the template. */
const uint8_t*
pb_template_get_data (const pb_template_t* template_);

/** Returns the template data size. */
uint32_t
pb_template_get_data_size (const pb_template_t* template_);

/** Sets an image associated with the template. If an image has already
  * been set, the old image will be deleted and replaced with this one.
  * This function will retain the image so the caller may safely delete
  * the image after this call. */
void
pb_template_set_image (pb_template_t* template_,
                       pb_image_t* image);

/** Returns the image associated with the template. */
pb_image_t*
pb_template_get_image (const pb_template_t* template_);

/** Returns the state of the memory-reference mr_cont attribute.
 * If 0 then a const reference to the object may be casted away
 * so that it may be retained. It is not allowed to change the
 * content itself. */
uint32_t pb_template_get_mr_const(const pb_template_t* template_);

/** Change the state of  memory-reference mr_cont attribute.
 * If 0 then a const reference to the object may be casted away
 * so that it may be retained. It is not allowed to change the
 * content itself. */
void pb_template_set_mr_const(pb_template_t* template_, uint32_t mr_const);

/** Returns the class id of the template. */
uint8_t
pb_template_get_class_id (pb_template_t* template_);

/** Sets the class id of the template. The original template is untouched 
  * and new template is returned with the new class id. 
  * The caller is responsible of deleting the returned template. */
pb_template_t*
pb_template_set_class_id (const pb_template_t* template_,
                          uint8_t class_id);

/** Sets the tag of the template, e.g. from the multitemplate structure.
  * Note that the tag is not encoded into the template. */
void
pb_template_set_tag (pb_template_t* template_,
                     int32_t tag);
/** Returns the tag of the template, or -1 if no tag has been set for 
  * the template. */
int32_t
pb_template_get_tag (const pb_template_t* template_);

/** Encodes the template into a data buffer. */
size_t
lib_codec_encode_template (uint8_t* data_encode,
                           const pb_template_t* template_);

/** Decodes a template from a data buffer. */
size_t
lib_codec_decode_template (const uint8_t* data_decode,
                           pb_template_t** template_);

#ifdef __cplusplus
}
#endif

#endif /* PB_TEMPLATE_H */

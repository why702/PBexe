/*
 * Copyright (c) 2017, Precise Biometrics AB
 * All rights reserved.
 *
 */

#ifndef PB_MATCH_RESULT_H
#define PB_MATCH_RESULT_H

#include <stdint.h>

#include "pb_types.h"
#include "pb_alignment.h"
#include "pb_template.h"
#include "pb_verifier_defs.h"
#include "pb_errors.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct pb_match_result_st pb_match_result_t;

/** Releases this match result reference. */
void
pb_match_result_delete(pb_match_result_t* match_result);

/** Retains this match result reference. */
pb_match_result_t*
pb_match_result_retain(pb_match_result_t* match_result);

/**
 * Retrieves the alignment from a match result object.
 * 
 * Alignment is between the verification template and the best matching
 * subtemplate from the best matching multitemplate.
 * 
 * If the match result was not a match, alignment is also not reliable and
 * may not be available.
 */
const pb_alignment_t* 
pb_match_result_get_alignment(const pb_match_result_t* match_result);

/**
 * Retrieves the absolute alignment from a match result object.
 *
 * The absolute alignment is the alignment of the verification template relative the full multitemplate.
 *
 * If the match result was not a match, alignment is also not reliable and
 * may not be available.
 */
const pb_alignment_t*
pb_match_result_get_absolute_alignment (const pb_match_result_t* match_result);

/**
 * Tells if the verification image was inverted, relative to the base polarity
 * of the multitemplate. 
 *
 * If the match result was not a match, this information may not be reliable. 
 */
int
pb_match_result_is_inverted(const pb_match_result_t* match_result);

/** 
 * Retrieves the decision from a match result object.
 * 
 * Decision requires the match result to be run on a decision based match. See
 * PB_DECISION_X.
 * 
 * If the image is not classified as a live finger, this function will always
 * return PB_DECISION_NON_MATCH.
 */
int
pb_match_result_get_decision(const pb_match_result_t* match_result);

/** 
 * Retrieves the decision from a match result object at a specific FAR.
 * 
 * Decision requires the match result to be run on a decision based match. See
 * PB_DECISION_X.
 * 
 * This function returns PB_DECISION_MATCH if the match would have been 
 * successful at the requested FAR.
 * 
 * Note that this function is not guaranteed to return correct results when
 * requesting a FAR at higher security level than what was used by the
 * verification.
 * 
 * If the image is not classified as a live finger, this function will always
 * return PB_DECISION_NON_MATCH.
 */
int
pb_match_result_get_decision_at(const pb_match_result_t* match_result,
                                pb_far_t false_accept_rate);

/** Tells if the image used for verification was from a latent print (1) or a real finger (0), 
  * based on the result from the latent detection module. */
int
pb_match_result_is_latent (const pb_match_result_t* match_result);

/** Tells if the image used for verification was from a spoof (1) or a real finger (0), based 
  * on the result from the presentation attack detection (PAD) module. */
int
pb_match_result_is_spoof (const pb_match_result_t* match_result);

/** Tells if the image used for verification was from a non-fingerprint (1) or a real finger (0),
  * based on the result from the finger detect module. */
int
pb_match_result_is_non_finger (const pb_match_result_t* match_result);

/**
 * Returns the index of the subtemplate that matched the verification template.
 *
 * If the match resulted in a reject, this field is set to zero.
 */
int
pb_match_result_get_matching_subtemplate_index(const pb_match_result_t* match_result);

/**
 * Returns the tag of the subtemplate that matched the verification template. Opposite to the
 * subtemplate index, the tag is unique and may be referenced forever. Each new subtemplate 
 * added to a multitemplate will get a new unique tag. 
 *
 * If the match resulted in a reject, this field is set to zero.
 */
int32_t
pb_match_result_get_matching_subtemplate_tag(const pb_match_result_t* match_result);

/**
 * Returns the index of the template that best matched the verification template
 * 
 * This refers to the index of the multitemplate in the list of multitemplates
 * used for matching.
 */
int
pb_match_result_get_matching_template_index(const pb_match_result_t* match_result);

/**
 * Returns the overlap against the best matching subtemplate, in percentage of the total image area. 
 */
int
pb_match_result_get_overlap_area(const pb_match_result_t* match_result);

/** Image class functionality. */

/**
 * Returns the class id of the subtemplate that matched the verification
 * template.
 */
uint8_t
pb_match_result_get_matching_subtemplate_class_id(const pb_match_result_t* match_result);

/** Sets the class id of the verification template. */
pb_rc_t
pb_match_result_set_verification_image_class_id(pb_match_result_t* match_result, 
                                                uint8_t class_id);

#ifdef __cplusplus
}
#endif

#endif /* PB_MATCH_RESULT_H */

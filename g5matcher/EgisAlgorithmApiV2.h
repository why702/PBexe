/******************************************************************************\
|*                                                                            *|
|*  EgisAlgorithmAPI.h                                                        *|
|*  Version: 3.0.1.51                                                         *|
|*  Revise Date: 2021/01/26                                                   *|
|*  Copyright (C) 2007-2020 Egis Technology Inc.                              *|
|*                                                                            *|
\******************************************************************************/

#ifndef EGIS_ALGORITHM_API_H
#define EGIS_ALGORITHM_API_H

#define FP_OK 0
#define FP_MATCHOK 101

#define FP_ERR -1000
#define FP_TIMEOUT -1001
#define FP_BADIMAGE -1002
#define FP_FEATURELOW -1003
#define FP_DUPLICATE -1004
#define FP_NOTDUPLICATE -1005
#define FP_MATCHFAIL -1006
#define FP_NULL_DATA -1007
#define FP_INVALID_FEATURE_LEN -1008
#define FP_ALLOC_MEM_FAIL -1009
#define FP_INVALID_FORMAT -1010
#define FP_TUNING_FAIL -1011
#define FP_INVALID_BUFFER_SIZE -1012
#define FP_IO_ACCESS_FAIL -1013
#define FP_NOT_TUNING -1014
#define FP_CONNECT_SENSOR_FAIL -1015
#define FP_CALL_FAILED -1016
#define FP_NOT_CONNECT_SENSOR -1017
#define FP_NULL_ENROLL_DATA -1018
#define FP_NULL_FEATURE -1019
#define FP_GETIMAGEFAIL -1020
#define FP_DECRYPT_FAIL -1021
#define FP_STATE_ERR -1022
#define FP_INVALID_SENSOR_MODE -1023
#define FP_PARAMETER_NOT_VALID -1888
#define FP_PARAMETER_UNACCEPTABLE -1889
#define FP_MATCH_TAMPERED -2000         // Returned from TEE/SE verification when TEE/SE results mismatch

// op code for common usage
#define FP_OP_GET_VERSION_V2 0x385
#define FP_OP_MAX_ENROLL_COUNT 0x4BF
#define FP_OP_ENROLL_REDUNDANT_LEVEL 0x4CD
#define FP_OP_ENROLL_QUALITY_REJECT_LEVEL 0x4CF
/**
 * Op code for setting a reject level for latent fingerprints during enrollment. 
 * Allowed values are 0 to LATENT_REJECT_LEVEL_ACCEPT_ALL, where 0 will reject all
 * images and LATENT_REJECT_LEVEL_ACCEPT_ALL will allow all images.
 */
#define FP_OP_ENROLL_LATENT_REJECT_LEVEL 0x4D0
#define FP_OP_RESOLUTION 0x5E6
#define FP_OP_CENTROID_X 0x5E7
#define FP_OP_CENTROID_Y 0x5E8
#define FP_OP_RADIUS 0x5E9
#define FP_OP_DYN_MASK_THRESHOLD 0x5EA
#define FP_OP_DYN_MASK_RADIUS 0x5EB

/** Maximum value for latent reject threshold. Will disable latent finger rejection. */
#define LATENT_REJECT_LEVEL_ACCEPT_ALL (0)

/* op code for vendor specific define */
#define FP_OP_VENDOR_BASE 0x1000

/* op code for precise biometrics */
#define FP_OP_VENDOR_PRECISE_BIOMETRICS_BASE 0x1000
#define FP_OP_ENABLE_SPD (FP_OP_VENDOR_PRECISE_BIOMETRICS_BASE + 1)

/**
 * 0:   NO lower FAR.
 * 1~N: first N verification with lower FAR
 * 
 */
#define FP_OP_SET_FIRST_N_LOWER_FAR (FP_OP_VENDOR_PRECISE_BIOMETRICS_BASE + 2)

/* op code for Static Pattern Detect threshold
 * corresponding SPD threshold values from 0 to 255.
 * 0 - No SPD
 * 1 - Minimum SPD
 * ...
 * 255 - Maximum SPD
 */
#define FP_OP_SPD_THRESHOLD (FP_OP_VENDOR_PRECISE_BIOMETRICS_BASE + 3)

// For FP_OP_ENROLL_REDUNDANT_IMAGE_POLICY
#define FP_DISABLE_SPD 0
#define FP_ENABLE_SPD 1

/**
 * Disable lens correction (if available) for the current algorithm.
 * If disabled, the function can not be resored.
 */
#define FP_OP_DISABLE_LENS_CORRECTION (FP_OP_VENDOR_PRECISE_BIOMETRICS_BASE + 4)

// For FP_OP_GET_VERSION_V2
#define FP_ALGO_VERSION_LEN 100

// FP_MERGE_ENROLL_ERROR_CODE
#define FP_ENROLL_IMAGE_OK                   1
#define FP_ENROLL_FINISH                     2
#define FP_ENROLL_IMAGE_HIGHLY_SIMILARITY    3
#define FP_ENROLL_REDUNDANT_ACCEPT           4
#define FP_ENROLL_BAD_IMAGE                 -1
#define FP_ENROLL_FEATURE_LOW               -2
#define FP_ENROLL_FAIL                      -3
#define FP_ENROLL_OUT_OF_MEMORY             -4
#define FP_ENROLL_UNKNOWN_FAIL              -5
#define FP_ENROLL_IRREGULAR_CONTEXT         -6
#define FP_ENROLL_DUPLICATE_IMAGE_REMOVED   -7
#define FP_ENROLL_REDUNDANT_INPUT           -8

#ifdef __cplusplus
extern "C" {
#endif

enum algo_api_sensor_type {
    FP_ALGOAPI_MODE_UNKNOWN = 0,

    /*FP ALGORITHM API MODE for CAPACITOR SENSORS(0~0x400)*/
    FP_ALGOAPI_MODE_EGIS_ET528 =                        0x30528,

    /*FP ALGORITHM API MODE for OPTICAL SENSORS(0x40000~0xF0000)*/
    FP_ALGOAPI_MODE_EGIS_ET713_2Px =                    0x40000,        /* 200 X 200 support ET713_2PA */
    FP_ALGOAPI_MODE_EGIS_ET713_2PA =                    0x40001,        /* 200 X 200 552DPI */
    FP_ALGOAPI_MODE_EGIS_ET713_2PA_S2PA4 =              0x40002,        /* 200 X 200 552DPI */
    FP_ALGOAPI_MODE_EGIS_ET713_2PA_NEW =                0x40003,        /* 200 X 200 552DPI */
    FP_ALGOAPI_MODE_EGIS_ET713S_2PB =                   0x40004,        /* 200 X 200 758DPI */

    FP_ALGOAPI_MODE_EGIS_ET713_3Px =                    0x40100,        /* 200 X 200 support ET713_3PC, ET713_3PD */
    FP_ALGOAPI_MODE_EGIS_ET713_3PC =                    0x40101,        /* 200 X 200 565DPI */
    FP_ALGOAPI_MODE_EGIS_ET713_3PD =                    0x40102,        /* 200 X 200 500DPI (TabS)*/
    FP_ALGOAPI_MODE_EGIS_ET713_3PG_S3PG1 =              0x40103,        /* 200 X 200 705DPI */
    FP_ALGOAPI_MODE_EGIS_ET713_3PG_S3PG2 =              0x40104,        /* 200 X 200 705DPI */
    FP_ALGOAPI_MODE_EGIS_ET713_3PG_S3PG3 =              0x40105,        /* 200 X 200 720DPI */
    FP_ALGOAPI_MODE_EGIS_ET713_3PG_S3PG3_Latency =      0x40108,        /* 200 X 200 720DPI */
    FP_ALGOAPI_MODE_EGIS_ET713_3PG_S3PG4 =              0x40106,        /* 200 X 200 705DPI */
    FP_ALGOAPI_MODE_EGIS_ET713_3PG_S3PG5 =              0x40107,        /* 200 X 200 705DPI */
    FP_ALGOAPI_MODE_EGIS_ET713_3PG_S3PG6 =              0x40109,        /* 200 X 200 705DPI */
    FP_ALGOAPI_MODE_EGIS_ET713_3PG_S3PG7 =              0x40110,        /* 200 X 200 730DPI */
    FP_ALGOAPI_MODE_EGIS_ET713_3PG_S3PG7_NEW =          0x40111,        /* 200 X 200 730DPI */
    FP_ALGOAPI_MODE_EGIS_ET713_3PG_S3PG6_EVO =          0x40112,        /* 200 X 200 730DPI */
    FP_ALGOAPI_MODE_EGIS_ET713_3PG_S3PG7_EVO =          0x40113,        /* 200 X 200 730DPI */
    FP_ALGOAPI_MODE_EGIS_ET713_3PG_S3PG8 =              0x40114,        /* 200 X 200 743DPI */
    FP_ALGOAPI_MODE_EGIS_ET713_3PG_S3PG8_new =          0x40115,        /* 200 X 200 743DPI */

    FP_ALGOAPI_MODE_EGIS_ET715S_2PB_S2PB1 =             0x40201,        /* 200 X 200 745DPI */

    FP_ALGOAPI_MODE_EGIS_ET713_3PCLA_CH1LA =            0x42101,        /* 200 X 200 530DPI */
    FP_ALGOAPI_MODE_EGIS_ET713_3PCLB_CH1SEA =           0x42102,        /* 200 X 200 */
    FP_ALGOAPI_MODE_EGIS_ET713_3PCLA_CH1LA_NEW =        0x42103,        /* 200 X 200 530DPI */
    FP_ALGOAPI_MODE_EGIS_ET713_3PG_CH1E_SV =            0x42104,        /* 200 X 200 724DPI SOFT*/
    FP_ALGOAPI_MODE_EGIS_ET713_3PG_CH1E_H =             0x42105,        /* 200 X 200 686DPI HARD*/
    FP_ALGOAPI_MODE_EGIS_ET713_3PG_CH1J_SB =            0x42106,        /* 200 X 200 743DPI SOFT*/
    FP_ALGOAPI_MODE_EGIS_ET713_3PG_CH1E_SB =            0x42107,        /* 200 X 200 700DPI SOFT*/
    FP_ALGOAPI_MODE_EGIS_ET713_3PG_CH1B_H =             0x42108,        /* 200 X 200 686DPI */
    FP_ALGOAPI_MODE_EGIS_ET720_2PB_CH1M30 =             0x42109,        /* 200 X 200 725DPI */
    FP_ALGOAPI_MODE_EGIS_ET713_3PG_CH1JSC_H =           0x42110,        /* 200 X 200 705DPI */
    FP_ALGOAPI_MODE_EGIS_ET713_3PG_CH1ABY =             0x42111,        /* 200 X 200 715DPI */
    FP_ALGOAPI_MODE_EGIS_ET713_3PG_CH1AJA =             0x42112,        /* 200 X 200 720DPI */
    FP_ALGOAPI_MODE_EGIS_ET713_3PG_CH1AJB =             0x42113,        /* 200 X 200 720DPI */
    FP_ALGOAPI_MODE_EGIS_ET713_3PG_CH1AJA_demorie =     0x42114,        /* 200 X 200 720DPI */
    FP_ALGOAPI_MODE_EGIS_ET713_3PG_CH1AJB_demorie =     0x42115,        /* 200 X 200 720DPI */

    FP_ALGOAPI_MODE_EGIS_ET713_3PC_CL1MH2 =             0x43101,        /* 200 X 200 550DPI */
    FP_ALGOAPI_MODE_EGIS_ET713_3PC_CL1MH2_CLT3 =        0x43102,        /* 200 X 200 550DPI */
    FP_ALGOAPI_MODE_EGIS_ET713_3PC_CL1MH2V =            0x43103,        /* 200 X 200 550DPI */

    FP_ALGOAPI_MODE_EGIS_ET713_3PCLC_CO1D151 =          0x44100,        /* 200 X 200 475DPI */
    FP_ALGOAPI_MODE_EGIS_ET713_3PCLD_CO1A118 =          0x44101,        /* 200 X 200 510DPI */
    FP_ALGOAPI_MODE_EGIS_ET713_3PG_CO1A118 =            0x44102,        /* 200 X 200 686DPI */

    FP_ALGOAPI_MODE_EGIS_ET713_3PC_CS3ZE2 =             0x45100,        /* 200 X 200 553DPI */

    FP_ALGOAPI_MODE_EGIS_ET713_3PC_CV1CPD1960 =         0x46100,        /* 200 X 200 510DPI */
    FP_ALGOAPI_MODE_EGIS_ET713_3PG_CV1CPD1960 =         0x46101,        /* 200 X 200 650DPI */
    FP_ALGOAPI_MODE_EGIS_ET713_3PG_CV1CTD2041 =         0x46102,        /* 200 X 200 697DPI */

    FP_ALGOAPI_MODE_EGIS_ET713_3PG_CH2NTH =             0x47100,        /* 200 X 200 720DPI */
    FP_ALGOAPI_MODE_EGIS_ET713_3PG_CH2NTH_B =           0x47101,        /* 200 X 200 720DPI */
    FP_ALGOAPI_MODE_EGIS_ET713_3PG_CH2NTH_V =           0x47102,        /* 200 X 200 720DPI */

    FP_ALGOAPI_MODE_EGIS_ET715_3Px =                    0x50100,        /* 193 X 193 */
    FP_ALGOAPI_MODE_EGIS_ET715_3PA =                    0x50101,        /* 193 X 193 576DPI */
    FP_ALGOAPI_MODE_EGIS_ET715_3PE =                    0x50102,        /* 193 X 193 518DPI */
    FP_ALGOAPI_MODE_EGIS_ET715_3PF =                    0x50103,        /* 193 X 193 700DPI */
    FP_ALGOAPI_MODE_EGIS_ET715_3PF_S3PF5 =              0x50104,        /* 193 X 193 720DPI */
    FP_ALGOAPI_MODE_EGIS_ET715_3PF_S3PF2 =              0x50105,        /* 193 X 193 740DPI */
    FP_ALGOAPI_MODE_EGIS_ET715_3PA_S3PA2 =              0x50106,        /* 193 X 193 596DPI */
    FP_ALGOAPI_MODE_EGIS_ET715_3PA_S3PA2_latency =      0x50107,        /* 193 X 193 596DPI Latency */
    FP_ALGOAPI_MODE_EGIS_ET715_3PG =                    0x50108,        /* 193 X 193 576DPI*/

    FP_ALGOAPI_MODE_EGIS_ET715_3PF_CL1TIME =            0x53101,        /* 193 X 193 764DPI */
    FP_ALGOAPI_MODE_EGIS_ET715_3PF_CL1CAY =             0x53102,        /* 193 X 193 764DPI */

    FP_ALGOAPI_MODE_EGIS_EL721_3PI =                    0x60000,        /* 215 X 175 643DPI */
    FP_ALGOAPI_MODE_EGIS_EL721_3PI_CV1CTD2052 =         0x60001,        /* 215 X 175 600DPI */
    FP_ALGOAPI_MODE_EGIS_EL721_3PI_CV1CTD2052_EVO =     0x60002,        /* 215 X 175 600DPI */
    FP_ALGOAPI_MODE_EGIS_EL721_3PI_S3PI1 =              0x60100,        /* 215 X 175 630DPI */
    FP_ALGOAPI_MODE_EGIS_EL721_3PI_S3PI1_EVO =          0x60101,        /* 215 X 175 630DPI */

    FP_ALGOAPI_MODE_EGIS_ET702 =                        0x70000,        /* 134 x 188 508DPI */
    FP_ALGOAPI_MODE_EGIS_ET702_INV =                    0x70001,        /* 134 x 188 508DPI invert image */	
    FP_ALGOAPI_MODE_EGIS_ET702_SXC210 =                 0x70002,        /* 134 x 188 508DPI */

    FP_ALGOAPI_MODE_EGIS_ET702_CH1M30 =                 0x72101,        /* 134 X 188 508DPI */
    FP_ALGOAPI_MODE_EGIS_ET702_CH1M30_INV =             0x72102,        /* 134 X 188 508DPI invert image */

    FP_ALGOAPI_MODE_EGIS_ET702_CL1MH2 =                 0x73101,        /* 134 X 188 508DPI */
    FP_ALGOAPI_MODE_EGIS_ET702_CL1MH2_INV =             0x73102,        /* 134 X 188 508DPI invert image */
    FP_ALGOAPI_MODE_EGIS_ET702_CL1MH2_C230 =            0x73103,        /* 134 X 188 508DPI */
    FP_ALGOAPI_MODE_EGIS_ET702_CL1WING =                0x73104,        /* 134 x 188 508DPI */
    FP_ALGOAPI_MODE_EGIS_ET702_CL1WING_Latency =        0x73105,        /* 134 x 188 508DPI */
    FP_ALGOAPI_MODE_EGIS_ET702_CL1WING_RESIZE =         0x73106,        /* 118 x 172 508DPI */

    FP_ALGOAPI_MODE_EGIS_ET760 =                        0x80000,        /* 200 x 200 363DPI */
    FP_ALGOAPI_MODE_EGIS_ET760_CROP =                   0x80001,        /* 142 x 142 363DPI */
    FP_ALGOAPI_MODE_EGIS_ET760_CROP2 =                  0x80002,        /* 150 x 104 363DPI */
    FP_ALGOAPI_MODE_EGIS_ET760_2 =                      0x80003,        /* 200 x 200 363DPI */
    FP_ALGOAPI_MODE_EGIS_ET760_2_IPP61e =               0x80004,        /* 200 x 200 363DPI */
    FP_ALGOAPI_MODE_EGIS_ET760_3 =                      0x80005,        /* 200 x 200 363DPI */

    FP_ALGOAPI_MODE_EGIS_ET901 =                        0x90000,        /* 150 x 150 363DPI */
    FP_ALGOAPI_MODE_EGIS_EF9002 =                       0x90001,        /* 175 x 175 318DPI */

    FP_ALGOAPI_MODE_EGIS_ET901_CL1V60 =                 0x93100,        /* 150 x 150 363DPI */
};

enum image_class_type {
    FP_IMAGE_TYPE_NORMAL = 0x0,
    FP_IMAGE_TYPE_ENROLL = 0xf,
    FP_IMAGE_TYPE_DRY    = 0x10,
};

/** Image mask flags. */
enum image_mask_flag {
    /** The pixel belongs to fingerprint. */
    FP_IMAGE_MASK_FINGERPRINT = 0x00,
    /** The pixel belongs to background (non-fingerprint). */
    FP_IMAGE_MASK_BACKGROUND = 0x01,
    /** The pixel belongs to flares (non-fingerprint). */
    FP_IMAGE_MASK_FLARE = 0x02,
    /** 0x04, 0x08, ... reserved for future use. */
};

/** An image. */
struct image_v2 {
    /** The pixels of the image. */
    unsigned char *pixels;
    /** The width of the image. */
    int width;
    /** The height of the image. */
    int height;
    /** The class of the image, see image_class_type. */
    unsigned int class;
    /** The resolution of the image, in dpi. */
    int resolution;
    /** The mask of the image, see image_mask_flag. May be 0 if all pixels belongs to fingerprint. */
    unsigned char* mask;

    /** The offset x for the cropped sub image within the full image (sensor). */
    int offset_x;
    /** The offset y for the cropped sub image within the full image (sensor). */
    int offset_y;
    /** The full width of the image (sensor). */
    int full_width;
    /** The full height of the image (sensor). */
    int full_height;
};

/** An image in 16-bit format. */
struct image_16bit {
    /** The pixels of the image. */
    unsigned short *pixels;
    /** The width of the image. */
    int width;
    /** The height of the image. */
    int height;
    /** The class of the image, see image_class_type. */
    unsigned int class;
    /** The resolution of the image, in dpi. */
    int resolution;
    /** The mask of the image, see image_mask_flag. May be 0 if all pixels belongs to fingerprint. */
    unsigned char* mask;

    /** The offset x for the cropped sub image within the full image (sensor). */
    int offset_x;
    /** The offset y for the cropped sub image within the full image (sensor). */
    int offset_y;
    /** The full width of the image (sensor). */
    int full_width;
    /** The full height of the image (sensor). */
    int full_height;
};

/** Initialization */
int 
algorithm_initialization_v2(void **ctx,
                            unsigned char *decision_data,
                            int decision_data_len,
                            int sensor_type);

/** Uninitialize */
int
algorithm_uninitialization_v2(void *ctx,
                              unsigned char **decision_data,
                              int *decision_data_len);

/** Resets the history of the algorithm, including a reset of the static pattern history. */
int
algorithm_reset_v2(void* ctx);

struct verify_init_v2 {
    unsigned char **enroll_temp_array;
    int *enroll_temp_size_array;
    int enroll_temp_number;
};

// Enrollment
int
enroll_init_v2(void *ctx,
               int template_size);

int 
enroll_duplicate_check_v2(void *ctx, 
                          struct verify_init_v2 *verify_init);

struct image_quality_values_v2 {
    int fingerprint_quality;
    int fingerprint_area;        // Fingerprint area in percentage of the total image area.
    int signal_to_noise_ratio;
    int signal;
    int noise;
    int static_pattern_area;     // Static pattern area in percentage of the total image area.
    int background_pattern_area; // The background pattern area in percent of the total active image area. 
};

struct enroll_info_v2 {
    unsigned char *image;
    int width;
    int height;
    unsigned int image_class;
    int percentage;
    int count;
    struct image_quality_values_v2 image_quality_values;
    int latent_score;   // See get_latent_score for documentation.
    /** The fingerprint coverage, in percentage of a "default fingerprint" with an estimated size 
      * of 12.5x17.5mm, computed from all added images. */
    int finger_coverage;
    /** The static pattern detection coverage on the sensor during enroll. */
    int spd_coverage;
    /** The match score of the added image against the enrolled (previously added) images. */
    int match_score;
    /** The image similar to previously added image will also be kept in enroll template. */
    int keep_redundant;
    /** The number of redundant images, added to the template. */
    int redundant_count;
};

struct enroll_info_v2_1 {
    struct image_v2 image;
    int percentage;
    int count;
    struct image_quality_values_v2 image_quality_values;
    int latent_score;   // See get_latent_score for documentation.
    /** The fingerprint coverage, in percentage of a "default fingerprint" with an estimated size
      * of 12.5x17.5mm, computed from all added images. */
    int finger_coverage;
    /** The static pattern detection coverage on the sensor during enroll. */
    int spd_coverage;
    /** The match score of the added image against the enrolled (previously added) images. */
    int match_score;
    /** The image similar to previously added image will also be kept in enroll template. */
    int keep_redundant;
    /** The number of redundant images, added to the template. */
    int redundant_count;
};

int
enroll_v2 (void *ctx,
           struct enroll_info_v2 *enroll_info);

int
enroll_v2_1 (void *ctx,
             struct enroll_info_v2_1 *enroll_info);

/**
 * Returns a data pointer to the latest enrolled template.
 * 
 * Memory is not transferred to the caller, and must not be deallocated. Memory reference is lost after
 * calling enroll_uninit_v2.
 * 
 * NOTE: Memory management works differently than get_enroll_template_SE. Please see that function for a
 * description of the difference.
 */
int
get_enroll_template_v2 (void *ctx,
                        unsigned char **enroll_temp,
                        int *enroll_temp_size);

/**
 * Stops the enrollment process.
 * 
 * Pointers returned through get_enroll_template_v2 are made invalid by calling this function. Please see
 * get_enroll_template_v2 and get_enrolled_template_SE for description of the different behaviors of these
 * functions.
 * 
 */
int
enroll_uninit_v2 (void *ctx);

/** Finish enroll before MAX_ENROLL_COUNT is reached.
  * Note: call this function only when there is not enough images to be enrolled to  
  * prohibit bad images to be enrolled if reject level is lower down during enroll.
  */
int
enroll_finish_v2(void *ctx);

int
get_template_version_v2(void *ctx,
                        unsigned char *template_data,
                        int template_data_size,
                        int *major_version,
                        int *minor_version);


/**
 * Template integrity checking
 * 
 * Returns FP_OK if templates check out, or an error code depending on the type of error
 * detected in the template.
 * 
 * Note: Template checking can only implement basic tests. Errors may still occur.
 * 
 */
int
get_template_integrity(void *ctx,
                       unsigned char *template_data,
                       int template_data_size);


// Verification

int
verify_init_v2 (void *ctx,
                struct verify_init_v2 *verify_init);

/** An alignment object describes translation and rotation alignment.
  * As such an alignment is used as part of fingerprint match
  * operations. The alignment can be expressed as a matrix multitplication:
  *
  *     V = T*R*E
  *
  * Where E and V are coordinates. The relation can also be expressed as:
  *
  *     Vx = Ex * cos(angle) - Ey * sin(angle) + dx
  *     Vy = Ex * sin(angle) + Ey * cos(angle) + dy
  *
  * Another way to understand alignment is to, starting at origo or the top left
  * corner of V, move template E (dx, dy) pixels, then rotate E a number of degrees
  * equal to angle.
  */
struct alignment_v2 {
    /** The translation in pixels along the x-axis, in 500dpi. */
    int dx;
    /** The translation in pixels along the y-axis, in 500dpi. */
    int dy;
    /** The rotation in binary radians [0, 255]. */
    unsigned char rotation;
    /** The overlap between the images, in percentage of the total image area. */
    int overlap;
};

struct verify_info_v2 {
    int try_match_count;
    unsigned char *image;
    int width;
    int height;
    unsigned int image_class;
    unsigned char *feat;
    int len;
    int match_score;
    int match_threshold;
    unsigned int match_image_class;
    int	far_min;
    int match_index;
    /** Store the matching score for all scaling
     *  size - (1 + sizeof(improve_dry_finger_performance)) * NBR_OF_ENROLLED_FINGER
     *  the first row stores the matching scores for default DPI,
     *  the coming rows store the matching scores for corresponding DPIs in improve_dry_finger_performance[]
     */
    int *match_score_array;
    int is_learning_update;
    unsigned char *enroll_temp;
    int enroll_temp_size;
    int latency_adjustment;
    /** Tells the matcher to perform an additional verification using a different dpi if
      * the first verification fails. This is done to improve performance for dry/cold
      * use cases when the finger may shrink.
      * 0 - Disabled.
      * 1 - Enabled and Automatic (i.e. the matcher decides when and how much to scale and
      *     which image that is used for dynamic update.
      * X (> 1) - Enabled and scaling is done to X% of original resolution and the original
      *           (non-scaled) image will be used for dynamic update. May not be used in
      *           combination with the TEE + SE verification!
      * It is possible to set up to 4 different percentages, e.g. {92, 1, 0, 0} will result
      * in a normal verification, a 92% verification and an "auto" verification. */
    unsigned char improve_dry_finger_performance[4];
    /** Tells whether the match failed (0), matched with the original image (1) or matched 
      * with the scaled image (2). */
    int match_result;
    /** The subtemplate matched with the verification templates, init to -1, if rejected, set to zero. */
    int matching_subtemplate_tag;
    /** The subtemplate added when updating the multitemplate, or -1 if none was added. */
    int added_subtemplate_tag;
    /** The subtemplate replaced when updating the multitemplate, or -1 if none was replaced. */
    int replaced_subtemplate_tag;
    /** The alignment between the verification image and the best matching subtemplate. */
    struct alignment_v2 match_alignment;
    struct image_quality_values_v2 image_quality_values;
};

struct verify_info_v2_1 {
    int try_match_count;
    struct image_v2 image;
    unsigned char *feat;
    int len;
    int match_score;
    int match_threshold;
    unsigned int match_image_class;
    int	far_min;
    int match_index;
    /** Store the matching score for all scaling
     *  size - (1 + sizeof(improve_dry_finger_performance)) * NBR_OF_ENROLLED_FINGER
     *  the first row stores the matching scores for default DPI,
     *  the coming rows store the matching scores for corresponding DPIs in improve_dry_finger_performance[]
     */
    int *match_score_array;
    int is_learning_update;
    unsigned char *enroll_temp;
    int enroll_temp_size;
    int latency_adjustment;
    /** Tells the matcher to perform an additional verification using a different dpi if
      * the first verification fails. This is done to improve performance for dry/cold
      * use cases when the finger may shrink. 
      * 0 - Disabled.
      * 1 - Enabled and Automatic (i.e. the matcher decides when and how much to scale and 
      *     which image that is used for dynamic update. 
      * X (> 1) - Enabled and scaling is done to X% of original resolution and the original
      *           (non-scaled) image will be used for dynamic update. May not be used in 
      *           combination with the TEE + SE verification!
      * It is possible to set up to 4 different percentages, e.g. {92, 1, 0, 0} will result
      * in a normal verification, a 92% verification and an "auto" verification. */
    unsigned char improve_dry_finger_performance[4];
    /** Tells whether the match failed (0), matched with the original image (1) or matched
      * with the scaled image (2). */
    int match_result;
    /** The subtemplate matched with the verification templates, init to -1, if rejected, set to zero. */
    int matching_subtemplate_tag;
    /** The subtemplate added when updating the multitemplate, or -1 if none was added. */
    int added_subtemplate_tag;
    /** The subtemplate replaced when updating the multitemplate, or -1 if none was replaced. */
    int replaced_subtemplate_tag;
    /** The alignment between the verification image and the best matching subtemplate. */
    struct alignment_v2 match_alignment;
    struct image_quality_values_v2 image_quality_values;
};

/**
 * Verify, return the result and train finger (if needed) with the new data.
 * 
 * Returns FP_MATCHOK, FP_MATCHFAIL or an error code. The function will never return FP_OK.
 * 
 * This function is fully equivalent to calling verify_begin_v2, verify_end_v2 and returning
 * the match result (unless an error occurs, in which case the error is returned instead).
 * 
 */
int
verify_v2 (void *ctx,
           struct verify_info_v2 *verify_info);

int
verify_v2_1 (void *ctx,
             struct verify_info_v2_1 *verify_info);

/** Returns the template extracted from the verification image from the call to verify_v2. 
  * This function must be called after verify(_begin)_v2 but before verify_uninit_v2. It 
  * may be called before verify_end_v2. The memory referenced will be deleted at 
  * verify_uninit_v2. 
  * Note: If only verify_begin_v2 has been called prior to this, this function will perform
  * the same updating as the verify_end_v2 does. */
int
get_verify_template_v2 (void *ctx,
                        const unsigned char **verify_temp,
                        int *verify_temp_size);

/** Verifies a subtemplate temp_2 against an another subtemplate temp_1. This function can
  * be called separately, i.e. no specific calls to verify_init_v2 and verify_uninit_v2 is
  * required. */
int
verify_template_v2 (void* ctx,
                    const unsigned char* temp_1,
                    int temp_1_size,
                    const unsigned char* temp_2,
                    int temp_2_size,
                    struct verify_info_v2* verify_info);

int
verify_template_v2_1 (void* ctx,
                      const unsigned char* temp_1,
                      int temp_1_size,
                      const unsigned char* temp_2,
                      int temp_2_size,
                      struct verify_info_v2_1* verify_info);

/**
 * Begins a verification process and returns a match result
 * 
 * Some parts of verification_info is not fully filled out by this function. Final results are
 * available from verify_end_v2, which must be called after this function regardless of MATCHFAIL
 * or MATCHOK.
 * 
 * This function can return FP_MATCHFAIL or FP_MATCHOK. If an error occurs, it is returned. This function
 * will never return FP_OK.
 * 
 */
int
verify_begin_v2 (void *ctx,
                 struct verify_info_v2 *verify_info);

int
verify_begin_v2_1 (void *ctx,
                   struct verify_info_v2_1 *verify_info);

#define NBR_OF_LATENT_FEATURES 23

/**
 * Latent detect
 *
 * Returns a score indicating the probability that the last verified image (from verify_begin_v2) is a latent 
 * fingerprint. This function needs to be called after the verify_begin_v2 but before the verify_end_v2.
 * 
 * Higher score means is "better", indicating that the image is a real fingerprint.
 *
 * This function also returns the latent features used to generate this score. The caller can choose to use the
 * latent score itself or to compute a score on their own using the the returned latent features as well as additional
 * external latent features. Note: These features are subject to change in later releases. 
 *
 * Scores are related to reject rate of real fingerprints. Adding and subtracting 2048 means doubling or halving
 * the reject rate. 32768 is tuned to give ~1% reject rate of real fingerprints.
 *
 * This gives the following table:
 *
 *    FRR           score
 * -------------------------
 *   0.01%          19161
 *   0.05%          23917
 *    0.1%          25965
 *    0.5%          30720
 *      1%          32768
 *      5%          37523
 *     10%          39571
 *
 * These are measured from reference databases, and actual scores may vary with the database.
 *
 * If an error occurs, an error code (negative number) is returned. Any positive return value indicates success.
 *
 */
int
get_latent_score (void* ctx,
                  int latent_features[NBR_OF_LATENT_FEATURES]);

/**
 * Ends the verification process, trains the matcher
 * 
 * This function will take care of all updating that was not made in verify_begin_v2, so that as much
 * time consuming processing as possible is placed in this function call.
 * 
 * Before a new verification can be made, this function must be called. Some parts of verification_info
 * is not fully filled out until this function has been called.
 * 
 * This function does not return MATCHOK or MATCHFAIL, only an error code or FP_OK.
 * 
 */
int
verify_end_v2 (void *ctx,
               struct verify_info_v2 *verify_info);

int
verify_end_v2_1 (void *ctx,
                 struct verify_info_v2_1 *verify_info);

int
verify_uninit_v2 (void *ctx);

int
set_accuracy_level_v2 (void *ctx,
                       int far_ratio);

int
extract_feature_v2 (void *ctx,
                    const unsigned char *image,
                    int width,
                    int height,
                    unsigned int image_class,
                    unsigned char **feature,
                    int *feat_size);

int
extract_feature_v2_1 (void *ctx,
                      const struct image_v2* image,
                      unsigned char **feature,
                      int *feat_size,
                      struct image_quality_values_v2* image_quality_values);

int
update_enroll_template_v2 (void *ctx,
                           const unsigned char *enroll_temp,
                           int enroll_temp_size,
                           const unsigned char *feature,
                           int feat_size,
                           unsigned char **new_enroll_temp,
                           int *new_enroll_temp_size);

/** Removes the N newest (most recently added) templates from the enrolled template. */
int
remove_newest_templates (void *ctx,
                         const unsigned char *enroll_temp,
                         int enroll_temp_size,
                         int N,
                         unsigned char **new_enroll_temp,
                         int *new_enroll_temp_size);

/** 
 * Change max template size in bytes or max number of subtemplate
 *
 * If non-positive value is input, algorithm will keep the default or previous value. 
 */
int
change_max_template_size(void *ctx,
                         const unsigned char *enroll_temp,
                         int enroll_temp_size,
                         int max_temp_size,
                         int max_nbr_of_subtemplates,
                         unsigned char **new_enroll_temp,
                         int *new_enroll_temp_size);

void
free_data (unsigned char* data,
           int data_size);

// Other
int
set_algo_config_v2 (void *ctx,
                    int param,
                    int value);
int
get_algo_config_v2 (void *ctx,
                    int param,
                    int *value);

int
algorithm_do_other_v2 (void *ctx,
                       int op_code,
                       unsigned char *in_data,
                       int in_data_size, 
                       unsigned char *out_data,
                       int *out_data_size);

/** Image class functionality. 
  *
  * SDK can use up to 32 classes (0-31), with the default class being 0 (will be assigned to any images not 
  * receiving a specific class). The class will be stored in the subtemplate of the multitemplate but the 
  * required minimum of subtemplates for each class is currently not stored. This means that you need to set 
  * the required minimum each time after you initialize the algorithm. If not, it will default back to no 
  * requirements at all, in which case all classes can replace each other. 
  * 
  * An image can be assigned a class using the pb_image_set_class_id function. The class id is copied
  * to all templates extracted from the image. Each extracted (sub)template may then be enrolled or updated
  * into the enrolled multitemplate. By specifying the required minimum number of subtemplates for different
  * classes the dynamic update functionality can then control which subtemplates to replace. 
  *
  * A subtemplate of class A may replace another subtemplate of class A or any other subtemplate of a different
  * class without any requirements on the minimum number of enrolled subtemplates. It may also replace any 
  * other subtemplate of a different class even if it has requirements on the minimum number of subtemplates 
  * given that the quota for that class is larger than the specified minimum. 
  *
  * Example:
  * Assume three classes A, B and C, with a required minimum nbr of subtemplates of 0, 2 and 4 respectively and 
  * the actual number of subtemplates enrolled being 4, 4 and 4 respectively, with a maximum of 12 subtemplates. 
  *   A (min: 0, current: 4)
  *   B (min: 2, current: 4)
  *   C (min: 4, current: 4)
  *
  * A new subtemplate of class A may then replace any of the 4 enrolled subtemplates of class A, since they are
  * from the same class, any of the 4 enrolled subtemplates of class B, since they have a higher quota (4) than 
  * required (2), but none of the 4 enrolled subtemplates of class C.
  *   - A may replace A (same class) or B (quota exceeded). 
  *
  * A new subtemplate of class B may then replace any of the 4 enrolled subtemplates of class A, since A has no
  * requirement on the minimum of subtemplates enrolled, any of the 4 enrolled subtemplates of class B since they
  * are from the same class, but none of the 4 enrolled subtemplates of class C. 
  *   - B may replace A (no min requirement) or B (same class)
  *
  * A new subtemplate of class C may then replace any of the 4 enrolled subtemplates of class A, since A has no
  * requirement on the minimum of subtemplates enrolled, any of the 4 enrolled subtemplates of class B since they 
  * have a higher quota (4) than required (2), and any of the 4 enrolled subtemplates of class C since they are 
  * from the same class. 
  *   - C may replace A (no min requirement) or B (quota exceeded) or C (same class). 
  */

int
set_required_minimum_nbr_of_subtemplates_v2 (void* ctx,
                                             unsigned int image_class,
                                             unsigned int minimum_nbr_of_subtemplates);

int
get_required_minimum_nbr_of_subtemplates_v2 (void* ctx,
                                             unsigned int image_class,
                                             int* minimum_nbr_of_subtemplates);

int
get_nbr_of_subtemplates_from_image_class (void* ctx,
                                          const unsigned char *enroll_temp,
                                          int enroll_temp_size,
                                          unsigned int image_class,
                                          int* nbr_of_subtemplates);

int
remove_subtemplates_from_image_class (void* ctx,
                                      unsigned char *enroll_temp,
                                      int enroll_temp_size,
                                      unsigned int image_class,
                                      int nbr_of_subtemplates_to_remove,
                                      unsigned char **new_enroll_temp,
                                      int *new_enroll_temp_size);

/**
 * Fake finger detect
 * 
 * Returns a score indicating how much the input image looks like a fingerprint. Higher scores means that the
 * image looks more like a real fingerprint.
 * 
 * Scores are related to reject rate of real fingerprints. Adding and subtracting 2048 means doubling or halving
 * the reject rate. 32768 is tuned to give ~1% reject rate of real fingerprints.
 * 
 * This gives the following table:
 * 
 *    FRR           score
 * -------------------------
 *   0.01%          19161
 *   0.05%          23917
 *    0.1%          25965
 *    0.5%          30720
 *      1%          32768
 *      5%          37523
 *     10%          39571
 * 
 * 
 * These are measured from reference databases, and actual scores may vary with the database.
 * 
 * If an error occurs, an error code (negative number) is returned. Any positive return value indicates success.
 *
 */
int
get_finger_score(void* ctx,
                 const unsigned char* pixels,
                 const int width,
                 const int height);

/**
 * Phone cover detect
 * 
 * Returns a score (range 0 to 255) indicating if there is a phone cover present.
 * High score (typically above 175) suggests that there is a (silicon) phone cover case present.
 *
 * If an error occurs, an error code is returned as a negative number. Any positive return value indicates success.
 */
int
get_cover_score(void* ctx,
                const unsigned char* pixels,
                const int width,
                const int height);

/**
 * Phone cover detect for 16bit raw images. ONLY supported for CL1WING algo.
 * 
 * Returns a score (range 0 to 255) indicating if there is a phone cover present.
 * High score (typically above 175) suggests that there is a (silicon) phone cover case present.
 *
 * If an error occurs, an error code is returned as a negative number. Any positive return value indicates success.
 */
int
get_cover_score_16b(void* ctx,
                    const unsigned short* pixels,
                    const int width,
                    const int height);

/**
 * Input two images to get the spd score 
 * 
 * This function will return the spd score for the two images if succesful. 
 * Otherwise an error code will be returned. 
 * 
 * Assumes zero alignment for these two templates.
*/
int
get_spd_score_for_two_images(void *ctx,
                             const struct image_v2* image1,
                             const struct image_v2* image2,
                             int resolution);

/** Detects straws of hair in the image. 
  * 
  * @param[in] image is the 16-bit image in which to detect hair. The static noise (background) shall have 
  *            been removed from this image, but no other processing should be made. 
  * @param[in] intensity_range is the intensity range of a normal fingerprint [0, UINT16_MAX], i.e. the range covering 
  *            approximately 99% of the fingerprint information. 
  * @param[in] outlier_threshold is the threshold used to detect the hair [0, UINT32_MAX]. A higher threshold will detect less hair
  *            while a lower will falsely detect more ridges as hair. This threshold should be tuned for each sensor. 
  * @param[out] hair_mask is the mask specifying which pixels that was detected as hair (0) and which as something
  *             else (255).
  */
int
detect_hair (void *ctx,
             const struct image_16bit* image,
             unsigned int intensity_range,
             unsigned int outlier_threshold,
             unsigned char* hair_mask);

/** 
  * TEE + SE verification.
  *
  * For increased security, it is possible to split the verification between the TEE and the SE. The TEE
  * performs the initial verification and the SE performs the final (if TEE accepted).
  *
  * The call flow is as shown below:
  *
  *   ----------------------------------------------------------------------------------
  *
  *   // Verify begin in TEE:
  *
  *   unsigned char* verification_template_SE = 0;
  *   int verification_template_size_SE = 0;
  *   unsigned char* match_data_SE = 0;
  *   int match_data_size_SE = 0;
  *
  *   // Perform initial verification in TEE, returns the match result as well as a verification template 
  *   // and match data to be sent to the SE where the final verification is done.
  *   int result = verify_begin_TEE (..., &verification_template_SE, &verification_template_size_SE,
  *                                       &match_data_SE, &match_data_size_SE);
  *
  *   if (result == FP_MATCHOK) {
  *       // The TEE accepted the verification, trigger verification in SE to confirm accept.
  *       ... // Implemented by caller.
  *   }
  *
  *   ----------------------------------------------------------------------------------
  *
  *   // Verify in SE:
  *
  *   struct verify_info_SE verify_info_SE = { 0 };
  *   unsigned char* updated_enrolled_template_SE = 0;
  *   int updated_enrolled_template_size_SE = 0;
  *
  *   // Get enrolled template corresponding to the matched enrolled template in the TEE verification.
  *   unsigned char* enrolled_template_SE = ... // Implemented by caller.
  *   int enrolled_template_size_SE = ... // Implemented by caller.
  *   // Get verification template passed from TEE.
  *   unsigned char* verification_template_SE = ... // Implemented by caller.
  *   int verification_template_size_SE = ... // Implemented by caller.
  *
  *   int allow_update = 1;
  *
  *   // Verify.
  *   int result = verify_SE (enrolled_template_SE, enrolled_template_size_SE, verification_template_SE, verification_template_size_SE, 
  *                           allow_update, &verify_info_SE, &updated_enrolled_template_SE, &updated_enrolled_template_size_SE);
  *
  *   // Check if SE confirmed accept from TEE.
  *   if (result == FP_MATCHOK) {
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
  *   // Get verify info passed from SE.
  *   struct verify_info_SE verify_info_SE = ... // Implemented by caller.
  *
  *   // Finalize verification.
  *   verify_end_TEE (..., verify_info_SE, ...);
  *
  *   // Update enrolled template in TEE, if applicable.
  *   if (verify_info->enroll_temp) {
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
  *    template size (in bytes) is not supported. Use the enroll_init_TEE_SE instead of enroll_init_v2.
  *    The reason for this is that the templates are stored both in the TEE and the SE and the same subtemplate
  *    needs to be replaced in both locations at dynamic update, hence the need for a well defined limitation. 
  *
  *  - The enrolled template to be stored in the SE must be created and passed to the SE after enrollment is 
  *    complete. Use the get_enrolled_template_SE function to create the enrolled template for the SE.
  *
  */

/* Initializes the enrollment to be used for TEE + SE solution. The limitation of the enrolled template needs 
 * to be specified as maximum number of subtemplates. */
int
enroll_init_TEE_SE (void *ctx,
                    int max_nbr_of_subtemplates);

/**
 * Returns the part of the enrolled template (in the TEE) to be stored in the 
 * secure element (SE). May be called after an enrollment to pass the SE template
 * to the SE.
 * 
 * NOTE: Works differently than get_enroll_template_v2. This function creates a copy of the enrolled template and
 * the copy must be destroyed by calling free_data to prevent memory leaks.
 * 
 */
int
get_enrolled_template_SE (unsigned char *enrolled_template_TEE,
                          int enrolled_template_size_TEE,
                          unsigned char **enrolled_template_SE,
                          int *enrolled_template_size_SE);

/**
 * Initial part of the TEE + SE verification. Should be called in the TEE.
 * 
 * @param[out] verification_template_SE is the verification template to be passed
 *             to the SE to perform the final verification in the SE. Must be deallocated
 *             by the caller.
 * @param[out] verification_template_size_SE is the size of the verification_template_SE.
 * @param[out] match_data_SE is the match data to be passed to the SE to perform the 
 *             final verification in the SE. Must be deallocated by the caller.
 * @param[out] match_data_size_SE is the size of the match_data_SE.
 * 
 */
int
verify_begin_TEE (void *ctx,
                  struct verify_info_v2_1 *verify_info,
                  unsigned char **verification_template_SE,
                  int *verification_template_size_SE,
                  unsigned char **match_data_SE,
                  int *match_data_size_SE);

/** Verify info for the verification in SE. */
struct verify_info_SE {
    /** Decision from the verification, != 0 if match was successful. */
    int decision;
    /** The tag of the subtemplate that was replaced in the SE, -1 if not applicable.
      * The SE will decide which subtemplate to replace and send this information back to 
      * the TEE so that the corresponding subtemplate can be replaced in the TEE as well. */
    int replaced_subtemplate_tag;
    /** The tag of the new subtemplate. */
    int new_subtemplate_tag;
};

/**
 * Final part of the TEE + SE verification. Should be called in the SE.
 *
 * @param[in] sensor_type is the sensor type, see algo_api_sensor_type. 
 * @param[in/out] enrolled_template_SE is the enrolled template in the SE corresponding to the 
 *            finger matched by the TEE verification, see match_index from the verify_info
 *            in the call to verify_begin_TEE.
 * @param[in] enrolled_template_size_SE is the size of the enrolled_template_SE.
 * @param[in] verification_template_SE is the verification template returned by the 
 *            verify_begin_TEE function. 
 * @param[in] verification_template_size_SE is the size of the verification_template_SE.
 * @param[in] match_data_SE is the match data returned by the verify_begin_TEE function. 
 * @param[in] match_data_size_SE is the size of the match_data_SE.
 * @param[in] allow_update tells if the SE should perform an update of the enrolled template
 *            in the case of an accept. 
 * @param[out] verify_info_SE is the verify info of the verification.
 * 
 * @note enrolled_template_SE will be written to by this operation. Memory is still owned
 *       by the caller. If the original contents is needed, a copy must be made before
 *       calling this function.
 * 
 */
int
verify_SE (int sensor_type,
           unsigned char *enrolled_template_SE,
           int enrolled_template_size_SE,
           unsigned char *verification_template_SE,
           int verification_template_size_SE,
           unsigned char *match_data_SE,
           int match_data_size_SE,
           int allow_update,
           struct verify_info_SE* verify_info_SE);

/**
 * Finalizes the verification. Should be called in the TEE. 
 *
 * @param[in] verify_info_SE is the verify info from the verification in the SE. 
 * 
 * @return Return code will be either FP_OK or an error code.
 */
int
verify_end_TEE (void *ctx,
                struct verify_info_SE* verify_info_SE,
                struct verify_info_v2_1 *verify_info);
;

/** Helper functions. */
void
verify_info_v2_1_to_v2 (const struct verify_info_v2_1 *verify_info,
                        struct verify_info_v2 *verify_info_v2);

void
verify_info_v2_to_v2_1 (const struct verify_info_v2 *verify_info,
                        unsigned char* mask,
                        int resolution,
                        struct verify_info_v2_1 *verify_info_v2_1);

/** Egistec defined APIs. */
typedef struct {
   unsigned char *decision_data;
   int decision_data_len;
} algo_backup_t;

/** create an unique bkdata. */
int
algorithm_backup_data_create (void *ctx, algo_backup_t **bkdata);

/** backup current decision data, maybe only SPD data in the future. */
int
algorithm_backup_v2 (void *ctx, algo_backup_t *bkdata);

/** restore the bkdata into the current context. */
int
algorithm_restore_by_sensor_type_v2 (void **ctx, algo_backup_t *bkdata, int sensor_type);

/** release bkdata, corresponding to algorithm_backup_data_create(). */
void
algorithm_backup_data_destory(void *ctx, algo_backup_t *bkdata);

/**
 * Extra integration API. These functions need to be defined. An empty definition is included below inside
 * a dead code block for ease of integration.
 * 
 */
 
/** Returns the number of microseconds elapsed as measured by clock(). */
extern int
egistec_clock();

/** Returns the cpu id of the cpu on which the current thread is running. */
extern int
egistec_cpu_id();


/** Dummy implementation of the integration functions. If possible, use a proper implementation. */
#if 0
int egistec_clock() { return -1; }

int egistec_cpu_id() { return -1; }
#endif

#ifdef __cplusplus
}
#endif

#endif

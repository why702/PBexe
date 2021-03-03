///******************************************************************************\
//|*                                                                            *|
//|*  main.c *|
//|*  Version: v0.9.9.9_2018_1129 *|
//|*  Date: 2017/06/30 *|
//|*  Revise Date: 2018/11/30 *|
//|*  Copyright (C) 2007-2018 Egis Technology Inc. *|
//|* *|
//\******************************************************************************/

typedef unsigned char BYTE;
#include "g5_match.h"

#include <stdio.h>
#include <stdlib.h>

#include "EgisAlgorithmApiV2.h"
//

#ifndef plat_alloc
#define plat_alloc(fmt) malloc(fmt)
#endif

#ifndef PLAT_FREE
#define PLAT_FREE(x) \
    if (x != NULL) { \
        free(x);     \
        x = NULL;    \
    }
#endif

#ifndef plat_free
#define plat_free(x) free(x)
#endif

#ifndef FALSE
#define FALSE 0
#endif

#ifndef TRUE
#define TRUE 1
#endif

#define BAUTH_MAX_TEMPLATE_INDEXS 4
#define MATCHER_API_MAX_DPI_VERIFY_NUM 5

void* pb_malloc(size_t size) {
    return malloc(size);
}
void pb_free(void* ptr) {
    free(ptr);
}
#define FAR_RATIO 100 * 1000  // 100K
static char g_imgfmt[16] = "*.png";
static int g_max_enroll_count = 17;
static int g_max_dry_count = 5;
static int g_enroll_redundant_level = 0;
static int g_enroll_quality_reject_level = 15;
static int g_enroll_latent_reject_level = LATENT_REJECT_LEVEL_ACCEPT_ALL;
static int g_verify_accuracy_level = FAR_RATIO;
static int g_latency_adjustment = 0;
static int g_resolution = 705;
static int g_resolution_v2 = 705;
static int g_first_n_lower_far = 0;
static int g_spd = FP_ENABLE_SPD;
static int g_centroid_X = 100;
static int g_centroid_Y = 100;
static int g_radius = 120;
static int g_dyn_mask_threshold = 0;
static int g_dyn_mask_radius = 0;
static void* g_ctx;                  // new
static BYTE* g_decision_data = 0;    // new
static int g_decision_data_len = 0;  // new
static enum algo_api_sensor_type g_sensor_type = FP_ALGOAPI_MODE_EGIS_ET713_3PG_S3PG6;

typedef struct _model_setting {
    void* g_ctx;
    int g_enroll_template_size;
    int g_enroll_redundant_level;
    int g_enroll_quality_reject_level;
    int g_enroll_latent_reject_level;  // Pb design latent solution not currenlty
                                       // in use
    int g_normal_far_ratio;
    int g_wash_hand_far_ratio;
    int g_easy_mode_2_far_ratio;
    int g_easy_mode_3_far_ratio;
    int g_latency_adjustment;  // Pb design latent solution not currenlty in use
    int g_resolution;
    int g_resolution_v2;
    int g_boost;  // Not sure what is this not in use
    int g_spd;
    int g_matcher_g5_spd_th;
    int g_mask_enable;  // Use Mask during verification
    int g_centroid_X;
    int g_centroid_Y;
    int g_radius;
    int g_dyn_mask_threshold;   // Not in used CS1 don't use dyn mask
    int g_dyn_mask_radius;      // Not in used CS1 don't use dyn mask
    int g_latent_finger_check;  // Not yet impliment
    int g_skip_failimage_learn;
    int g_update_learn_by_filename;
    int g_dry_finger_mode;
    enum algo_api_sensor_type g_sensor_type;
    enum model_type phone_model_type;
    enum lens_type phone_lens_type;
    enum fp_type phone_sensor_type;
} model_setting;
model_setting g_session;

static void set_image_class_type_num(void* ctx) {
    set_required_minimum_nbr_of_subtemplates_v2(ctx, FP_IMAGE_TYPE_ENROLL, g_max_enroll_count);
    set_required_minimum_nbr_of_subtemplates_v2(ctx, FP_IMAGE_TYPE_DRY, g_max_dry_count);
}

int algorithm_initialization() {
    g_session.g_ctx = g_ctx;
    // g_session.g_enroll_template_size = g_enroll_template_size;
    g_session.g_enroll_redundant_level = g_enroll_redundant_level;
    g_session.g_enroll_quality_reject_level = g_enroll_quality_reject_level;
    g_session.g_enroll_latent_reject_level = g_enroll_latent_reject_level;
    g_session.g_normal_far_ratio = g_verify_accuracy_level;
    // g_session.g_wash_hand_far_ratio = g_wash_hand_far_ratio;
    // g_session.g_easy_mode_2_far_ratio = g_easy_mode_2_far_ratio;
    // g_session.g_easy_mode_3_far_ratio = g_easy_mode_3_far_ratio;
    g_session.g_latency_adjustment = g_latency_adjustment;
    g_session.g_resolution = g_resolution;
    g_session.g_resolution_v2 = g_resolution_v2;
    // g_session.g_boost = g_boost;
    g_session.g_spd = g_spd;
    // g_session.g_matcher_g5_spd_th = g_matcher_g5_spd_th;
    // g_session.g_mask_enable = g_mask_enable;
    g_session.g_centroid_X = g_centroid_X;
    g_session.g_centroid_Y = g_centroid_Y;
    g_session.g_radius = g_radius;
    g_session.g_dyn_mask_threshold = g_dyn_mask_threshold;
    g_session.g_dyn_mask_radius = g_dyn_mask_radius;
    // g_session.g_latent_finger_check = g_latent_finger_check;
    // g_session.g_skip_failimage_learn = g_skip_failimage_learn;
    // g_session.g_update_learn_by_filename = g_update_learn_by_filename;
    // g_session.g_dry_finger_mode = g_dry_finger_mode;
    g_session.g_sensor_type = g_sensor_type;
    // g_session.phone_model_type = phone_model_type;
    // g_session.phone_lens_type = phone_lens_type;
    // g_session.phone_sensor_type = phone_sensor_type;

    int ret;
    ret = algorithm_initialization_v2(&g_session.g_ctx, g_decision_data, g_decision_data_len,
                                      g_session.g_sensor_type);
    // General config
    set_image_class_type_num(g_session.g_ctx);
    ret = set_algo_config_v2(g_session.g_ctx, FP_OP_ENABLE_SPD, g_session.g_spd);
    ret = set_algo_config_v2(g_session.g_ctx, FP_OP_RESOLUTION, g_session.g_resolution);
    if (g_session.g_centroid_X > 0 && g_session.g_centroid_Y > 0 && g_session.g_radius > 0) {
        ret = set_algo_config_v2(g_session.g_ctx, FP_OP_CENTROID_X, g_session.g_centroid_X);
        ret = set_algo_config_v2(g_session.g_ctx, FP_OP_CENTROID_Y, g_session.g_centroid_Y);
        ret = set_algo_config_v2(g_session.g_ctx, FP_OP_RADIUS, g_session.g_radius);
    }
    if (g_session.g_dyn_mask_threshold > 0 && g_session.g_dyn_mask_radius > 0) {
        ret = set_algo_config_v2(g_session.g_ctx, FP_OP_DYN_MASK_THRESHOLD,
                                 g_session.g_dyn_mask_threshold);
        ret =
            set_algo_config_v2(g_session.g_ctx, FP_OP_DYN_MASK_RADIUS, g_session.g_dyn_mask_radius);
    }
    // Enroll config
    ret = set_algo_config_v2(g_session.g_ctx, FP_OP_MAX_ENROLL_COUNT, g_max_enroll_count);
    ret = set_algo_config_v2(g_session.g_ctx, FP_OP_ENROLL_REDUNDANT_LEVEL,
                             g_session.g_enroll_redundant_level);
    ret = set_algo_config_v2(g_session.g_ctx, FP_OP_ENROLL_QUALITY_REJECT_LEVEL,
                             g_session.g_enroll_quality_reject_level);
    ret = set_algo_config_v2(g_session.g_ctx, FP_OP_ENROLL_LATENT_REJECT_LEVEL,
                             g_session.g_enroll_latent_reject_level);
    // Verify config
    ret = set_accuracy_level_v2(g_session.g_ctx, g_session.g_normal_far_ratio);
    ret = set_algo_config_v2(g_session.g_ctx, FP_OP_SET_FIRST_N_LOWER_FAR, g_first_n_lower_far);
    return ret;
}
unsigned char g_algo_ver[FP_ALGO_VERSION_LEN];
void get_version() {
    int algo_ver_len = FP_ALGO_VERSION_LEN;
    algorithm_initialization_v2(&g_ctx, g_decision_data, g_decision_data_len, g_sensor_type);

    // algorithm_do_other(FP_ALGOAPI_GET_VERSION, NULL, (BYTE*)&g_algo_api_info);
    algorithm_do_other_v2(g_ctx, FP_OP_GET_VERSION_V2, NULL, 0, g_algo_ver, &algo_ver_len);
    printf("G5 version : matcher(%s) \n", g_algo_ver);
    // algorithm_uninitialization(NULL, DECISION_DATA_LEN);
    algorithm_uninitialization_v2(g_ctx, &g_decision_data, &g_decision_data_len);  // new
}

void images_compare_(unsigned char** raw1, unsigned char** raw2, int w, int h, int* match_score,
                     int* rot, int* dx, int* dy) {
    if (raw1 == NULL || raw2 == NULL) {
        printf("Load image file fail\r\n");
        return;
    }

    // init
    int status = 0;
    int nbr_of_fingers_to_enroll = 1;  // new

    algorithm_initialization();
    set_algo_config_v2(g_session.g_ctx, FP_OP_MAX_ENROLL_COUNT, 1);
    BYTE* extract_finger_temp1 = NULL;
    int extract_finger_temp1_size = 0;
    BYTE* extract_finger_temp2 = NULL;
    int extract_finger_temp2_size = 0;

    // get version
    get_version();

    // extract feature
    extract_feature_v2(g_session.g_ctx, raw1[0], w, h, FP_IMAGE_TYPE_NORMAL, &extract_finger_temp1,
                       &extract_finger_temp1_size);

    extract_feature_v2(g_session.g_ctx, raw2[0], w, h, FP_IMAGE_TYPE_NORMAL, &extract_finger_temp2,
                       &extract_finger_temp2_size);

    struct verify_init_v2 verify_init = {0};  // new
    verify_init.enroll_temp_array =
        (BYTE**)plat_alloc(nbr_of_fingers_to_enroll * sizeof(BYTE*));  // new
    if (verify_init.enroll_temp_array == NULL) {
        return;
    }
    verify_init.enroll_temp_size_array =
        (int*)plat_alloc(nbr_of_fingers_to_enroll * sizeof(int));  // new
    if (verify_init.enroll_temp_size_array == NULL) {
        PLAT_FREE(verify_init.enroll_temp_array);
        return;
    }
    verify_init.enroll_temp_number = nbr_of_fingers_to_enroll;  // new
    verify_init.enroll_temp_array[0] = (BYTE*)plat_alloc(extract_finger_temp1_size * sizeof(BYTE));
    verify_init.enroll_temp_array[0] = extract_finger_temp1;
    verify_init.enroll_temp_size_array[0] = extract_finger_temp1_size;
    status = verify_init_v2(g_session.g_ctx, &verify_init);

    struct verify_info_v2 verify_info_data = {0};
    verify_info_data.try_match_count = 0;
    verify_info_data.image = raw2[0];
    verify_info_data.width = w;
    verify_info_data.height = h;
    verify_info_data.match_score = 0;
    verify_info_data.match_index = -1;
    verify_info_data.image_class = FP_IMAGE_TYPE_NORMAL;
    verify_info_data.latency_adjustment = g_latency_adjustment;
    verify_info_data.match_score_array = (int*)plat_alloc(sizeof(int) * 1);
    verify_info_data.is_learning_update = 0;
    verify_info_data.enroll_temp_size = 0;

    // matching
    status = verify_template_v2(g_session.g_ctx, extract_finger_temp1, extract_finger_temp1_size,
                                extract_finger_temp2, extract_finger_temp2_size, &verify_info_data);

    int result = verify_info_data.match_result;
    *match_score = verify_info_data.match_score;
    *rot = (float)verify_info_data.match_alignment.rotation / 255 * 360;
    if (*rot > 180) {
        *rot = *rot - 360;
    }
    *dx = verify_info_data.match_alignment.dx;
    *dy = verify_info_data.match_alignment.dy;

    status = verify_uninit_v2(g_session.g_ctx);

    plat_free(verify_init.enroll_temp_array);
    plat_free(verify_init.enroll_temp_size_array);
    plat_free(verify_info_data.match_score_array);

    plat_free(extract_finger_temp1);
    plat_free(extract_finger_temp2);
    algorithm_uninitialization_v2(g_session.g_ctx, &g_decision_data,
                                  &g_decision_data_len);  // new
    return;
}

// void images_compare_1(BYTE **raw1, BYTE **raw2, unsigned char *mask1,
//                      unsigned char *mask2, int w, int h, int dpi,
//                      int *match_score, int *rot, int *dx, int *dy) {
//  if (raw1 == NULL || raw2 == NULL) {
//    printf("Load image file fail\r\n");
//    return;
//  }
//
//  // init
//  int status = 0;
//  int nbr_of_fingers_to_enroll = 1; // new
//
//  algorithm_initialization();
//  set_algo_config_v2(g_session.g_ctx, FP_OP_MAX_ENROLL_COUNT, 1);
//  BYTE *extract_finger_temp1 = NULL;
//  int extract_finger_temp1_size = 0;
//  BYTE *extract_finger_temp2 = NULL;
//  int extract_finger_temp2_size = 0;
//
//  // extract feature
//  struct image_v2 image1;
//  image1.pixels = raw1[0];
//  image1.width = w;
//  image1.height = h;
//  image1.full_width = w;
//  image1.full_height = h;
//  image1.mask = mask1;
//  image1.class = FP_IMAGE_TYPE_NORMAL;
//  image1.resolution = g_resolution_v2;
//  image1.offset_x = 0;
//  image1.offset_y = 0;
//
//  struct image_v2 image2;
//  image2.pixels = raw2[0];
//  image2.width = w;
//  image2.height = h;
//  image2.full_width = w;
//  image2.full_height = h;
//  image2.mask = mask2;
//  image2.class = FP_IMAGE_TYPE_NORMAL;
//  image2.resolution = g_resolution_v2;
//  image2.offset_x = 0;
//  image2.offset_y = 0;
//
//  struct image_quality_values_v2 image_quality_values1, image_quality_values2;
//
//  extract_feature_v2_1(g_session.g_ctx, &image1, &extract_finger_temp1,
//                       &extract_finger_temp1_size, &image_quality_values1);
//
//  extract_feature_v2_1(g_session.g_ctx, &image2, &extract_finger_temp2,
//                       &extract_finger_temp2_size, &image_quality_values2);
//
//  static struct verify_init_v2 verify_init;
//  verify_init.enroll_temp_array = (unsigned char **)plat_alloc(
//      nbr_of_fingers_to_enroll * sizeof(unsigned char *)); // new
//  verify_init.enroll_temp_size_array =
//      (int *)plat_alloc(nbr_of_fingers_to_enroll * sizeof(int)); // new
//  verify_init.enroll_temp_number = nbr_of_fingers_to_enroll;     // new
//  verify_init.enroll_temp_array[0] = extract_finger_temp1;
//  verify_init.enroll_temp_size_array[0] = extract_finger_temp1_size;
//
//  verify_init_v2(g_session.g_ctx, &verify_init);
//
//  struct verify_info_v2_1 verify_info_data = {0};
//  verify_info_data.try_match_count = 0;
//  verify_info_data.image.pixels = raw2[0];
//  verify_info_data.image.width = w;
//  verify_info_data.image.height = h;
//  verify_info_data.image.class = FP_IMAGE_TYPE_NORMAL;
//  verify_info_data.feat = NULL;
//  verify_info_data.len = 0;
//  verify_info_data.match_score = 0;
//  verify_info_data.match_index = -1;
//  verify_info_data.is_learning_update = FALSE;
//  verify_info_data.enroll_temp = NULL;
//  verify_info_data.enroll_temp_size = 0;
//  verify_info_data.match_threshold = 0;
//  verify_info_data.image.mask = mask2;
//  verify_info_data.latency_adjustment = 0;
//  verify_info_data.match_score_array =
//      (int *)plat_alloc(sizeof(int) * BAUTH_MAX_TEMPLATE_INDEXS *
//                        MATCHER_API_MAX_DPI_VERIFY_NUM); // new
//  memset(verify_info_data.match_score_array, 0,
//         sizeof(int) * BAUTH_MAX_TEMPLATE_INDEXS *
//             MATCHER_API_MAX_DPI_VERIFY_NUM); // new
//  verify_info_data.match_alignment.dx = 0;
//  verify_info_data.match_alignment.dy = 0;
//  verify_info_data.match_alignment.rotation = 0;
//  if (dpi < 0) {
//    verify_info_data.improve_dry_finger_performance[0] = 94;
//    verify_info_data.improve_dry_finger_performance[2] = 1;
//  } else {
//    verify_info_data.improve_dry_finger_performance[0] = dpi;
//  }
//
//  // matching
//  status = verify_template_v2_1(g_session.g_ctx, extract_finger_temp1,
//                                extract_finger_temp1_size, extract_finger_temp2,
//                                extract_finger_temp2_size, &verify_info_data);
//
//  //printf("matching score = %d ,%d ,%d \n",
//  //       verify_info_data.match_score_array[0],
//  //       verify_info_data.match_score_array[1],
//  //       verify_info_data.match_score_array[2]);
//
//  int result = verify_info_data.match_result;
//  *match_score = verify_info_data.match_score;
//  *rot = (float)verify_info_data.match_alignment.rotation / 255 * 360;
//  if (*rot > 180) {
//    *rot = *rot - 360;
//  }
//  *dx = verify_info_data.match_alignment.dx;
//  *dy = verify_info_data.match_alignment.dy;
//
//  status = verify_end_v2_1(g_session.g_ctx, &verify_info_data);
//
//  plat_free(verify_init.enroll_temp_array);
//  plat_free(verify_init.enroll_temp_size_array);
//  plat_free(verify_info_data.match_score_array);
//
//  plat_free(extract_finger_temp1);
//  plat_free(extract_finger_temp2);
//  algorithm_uninitialization_v2(g_session.g_ctx, &g_decision_data,
//                                &g_decision_data_len); // new
//  return;
//}

void images_compare_by_algo(unsigned char** raw1, unsigned char** raw2, int w, int h,
                            int* match_score, int* rot, int* dx, int* dy,
                            struct algo_info* algo_info) {
    g_sensor_type = algo_info->sensor_type;
    g_resolution = algo_info->resolution;
    g_radius = algo_info->radius;
    images_compare_(raw1, raw2, w, h, match_score, rot, dx, dy);
    return;
}

// void images_compare_1_by_algo(BYTE** raw1, BYTE** raw2, unsigned char* mask1, unsigned char*
// mask2, int w, int h, int dpi, int* match_score, int* rot, int* dx, int* dy,
//                              struct algo_info* algo_info) {
//    g_sensor_type = algo_info->sensor_type;
//    g_resolution = algo_info->resolution;
//    g_radius = algo_info->radius;
//    images_compare_1(raw1, raw2, mask1, mask2, w, h, dpi, match_score, rot, dx, dy);
//	return;
//	}
//

///******************************************************************************\
//|*                                                                            *|
//|*  main.c                                                                    *|
//|*  Version: v0.9.9.9_2018_1129                                               *|
//|*  Date: 2017/06/30                                                          *|
//|*  Revise Date: 2018/11/30                                                   *|
//|*  Copyright (C) 2007-2018 Egis Technology Inc.                              *|
//|*                                                                            *|
//\******************************************************************************/

#include "g5_match.h"
#include "EgisAlgorithmAPI.h"
#include <stdio.h>
#include <stdlib.h>
//
typedef unsigned char       BYTE;
#define plat_alloc malloc
#define plat_free free
#define FAR_RATIO 100 * 1000 //100K 
static char g_imgfmt[16] = "*.png";
static int g_max_enroll_count = 17;
static int g_enroll_redundant_level = 0;
static int g_enroll_quality_reject_level = 15;
static int g_enroll_latent_reject_level = LATENT_REJECT_LEVEL_ACCEPT_ALL;
static int g_verify_accuracy_level = FAR_RATIO;
static int g_latency_adjustment = 0;
static int g_resolution = 552;
static int g_resolution_v2 = 500;
static int g_first_n_lower_far = 0;
static int g_spd = FP_ENABLE_SPD;
static int g_centroid_X = 0;
static int g_centroid_Y = 0;
static int g_radius = 0;
static int g_dyn_mask_threshold = 0;
static int g_dyn_mask_radius = 0;
static void *g_ctx;                        // new
static BYTE *g_decision_data = 0; // new
static int g_decision_data_len = 0;        // new
static enum algo_api_sensor_type g_sensor_type = FP_ALGOAPI_MODE_EGIS_ET713_2Px;

static void algorithm_initialization() {
  algorithm_initialization_v2(&g_ctx, g_decision_data, g_decision_data_len,
                              g_sensor_type);
  // General config
  set_image_class_type_num(g_ctx);
  set_algo_config_v2(g_ctx, FP_OP_ENABLE_SPD, g_spd);
  set_algo_config_v2(g_ctx, FP_OP_RESOLUTION, g_resolution_v2);
  if (g_centroid_X > 0 && g_centroid_Y > 0 && g_radius > 0) {
    set_algo_config_v2(g_ctx, FP_OP_CENTROID_X, g_centroid_X);
    set_algo_config_v2(g_ctx, FP_OP_CENTROID_Y, g_centroid_Y);
    set_algo_config_v2(g_ctx, FP_OP_RADIUS, g_radius);
  }
  if (g_dyn_mask_threshold > 0 && g_dyn_mask_radius > 0) {
    set_algo_config_v2(g_ctx, FP_OP_DYN_MASK_THRESHOLD, g_dyn_mask_threshold);
    set_algo_config_v2(g_ctx, FP_OP_DYN_MASK_RADIUS, g_dyn_mask_radius);
  }
  // Enroll config
  set_algo_config_v2(g_ctx, FP_OP_MAX_ENROLL_COUNT, g_max_enroll_count);
  set_algo_config_v2(g_ctx, FP_OP_ENROLL_REDUNDANT_LEVEL,
                     g_enroll_redundant_level);
  set_algo_config_v2(g_ctx, FP_OP_ENROLL_QUALITY_REJECT_LEVEL,
                     g_enroll_quality_reject_level);
  set_algo_config_v2(g_ctx, FP_OP_ENROLL_LATENT_REJECT_LEVEL,
                     g_enroll_latent_reject_level);
  // Verify config
  set_accuracy_level_v2(g_ctx, g_verify_accuracy_level);
  set_algo_config_v2(g_ctx, FP_OP_SET_FIRST_N_LOWER_FAR, g_first_n_lower_far);
}

void images_compare_(BYTE **raw1, BYTE **raw2, int w, int h) {
	if (raw1 == NULL || raw2 == NULL) {
		printf("Load image file fail\r\n");
		return;
	}

  // init
  int status = 0;
  int nbr_of_fingers_to_enroll = 1; // new

  algorithm_initialization();
  set_algo_config_v2(g_ctx, FP_OP_MAX_ENROLL_COUNT, 1);
  BYTE *extract_finger_temp1 = NULL;
  int extract_finger_temp1_size = 0;
  BYTE *extract_finger_temp2 = NULL;
  int extract_finger_temp2_size = 0;

  // extract feature
  extract_feature_v2(g_ctx, raw1[0], w, h, FP_IMAGE_TYPE_NORMAL,
                     &extract_finger_temp1, &extract_finger_temp1_size);

  extract_feature_v2(g_ctx, raw2[0], w, h, FP_IMAGE_TYPE_NORMAL,
                     &extract_finger_temp2, &extract_finger_temp2_size);

  struct verify_init_v2 verify_init = { 0 }; // new
  verify_init.enroll_temp_array = (BYTE **)plat_alloc(
	  nbr_of_fingers_to_enroll * sizeof(BYTE *)); // new
  verify_init.enroll_temp_size_array =
	  (int *)plat_alloc(nbr_of_fingers_to_enroll * sizeof(int)); // new
  verify_init.enroll_temp_number = nbr_of_fingers_to_enroll;     // new
  verify_init.enroll_temp_array[0] = (BYTE *)plat_alloc(extract_finger_temp1_size * sizeof(BYTE));
  verify_init.enroll_temp_array[0] = extract_finger_temp1;
  verify_init.enroll_temp_size_array[0] = extract_finger_temp1_size;
  status = verify_init_v2(g_ctx, &verify_init);

  struct verify_info_v2 verify_info_data = { 0 };
  verify_info_data.try_match_count = 0;
  verify_info_data.image = raw1[0];
  verify_info_data.width = w;
  verify_info_data.height = h;
  verify_info_data.match_score = 0;
  verify_info_data.match_index = -1;
  verify_info_data.image_class = FP_IMAGE_TYPE_NORMAL;
  verify_info_data.latency_adjustment = g_latency_adjustment;
  verify_info_data.match_score_array = (int *)plat_alloc(sizeof(int) * 1);
  verify_info_data.is_learning_update = 0;
  verify_info_data.enroll_temp_size = 0;

  // matching
  status = verify_template_v2(g_ctx, extract_finger_temp1,
                              extract_finger_temp1_size, extract_finger_temp2,
                              extract_finger_temp2_size, &verify_info_data);

  status = verify_uninit_v2(g_ctx);

  plat_free(verify_init.enroll_temp_array);
  plat_free(verify_init.enroll_temp_size_array);
  plat_free(verify_info_data.match_score_array);

  plat_free((void **)raw1);
  plat_free((void **)raw2);
  plat_free(extract_finger_temp1);
  plat_free(extract_finger_temp2);
  algorithm_uninitialization_v2(g_ctx, &g_decision_data,
                                &g_decision_data_len); // new
  return;
}

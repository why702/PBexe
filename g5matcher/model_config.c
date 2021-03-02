#include "EgisAlgorithmAPI.h"
#include "model_config.h"
#include "mlq_db/mlq_db_a50.h"
#include "mlq_db/mlq_db_a50s.h"
#include "mlq_db/mlq_db_tabs6.h"
#include "mlq_db/mlq_db_a90.h"
#include "mlq_db/mlq_db_a91.h"
#include "mlq_db/mlq_db_a30s.h"
#include "mlq_db/mlq_db_a51.h"
#include <stdio.h>

threshold_manager_t threshold_manager;

static void init_th_by_model_lens_sensor(enum model_type this_model_type,
										 enum lens_type this_lens_type,
										 enum fp_type this_sensor_type)
{
	switch (this_model_type)
	{
	case MODEL_A70S:
		INIT_TH_BY_MODEL(MODEL_A70S);
		break;
	case MODEL_A70:
		INIT_TH_BY_MODEL(MODEL_A70);
		break;
	case MODEL_A50:
		INIT_TH_BY_MODEL(MODEL_A50);
		break;
	case MODEL_A80:
		INIT_TH_BY_MODEL(MODEL_A80);
		break;
	case MODEL_A50S:
		INIT_TH_BY_MODEL(MODEL_A50S);
		break;
	case MODEL_TAB_S6:
		INIT_TH_BY_MODEL(MODEL_TAB_S6);
		break;
	case MODEL_A51:
		INIT_TH_BY_MODEL(MODEL_A51);
		break;
	case MODEL_A90:
		INIT_TH_BY_MODEL(MODEL_A90);
		break;
	case MODEL_A91:
		INIT_TH_BY_MODEL(MODEL_A91);
		break;
	case MODEL_A30S_LENS_3PA:
	case MODEL_A30S_LENS_3PF:
		if (this_lens_type == LENS_3PA)
		{
			INIT_TH_BY_MODEL(MODEL_A30S_LENS_3PA);
		}
		else if (this_lens_type == LENS_3PF)
		{
			INIT_TH_BY_MODEL(MODEL_A30S_LENS_3PF);
		}
		else
		{
			INIT_TH_BY_MODEL(MODEL_DEFAULT);
		}
		break;
	case MODEL_A71:
		INIT_TH_BY_MODEL(MODEL_A71);
		break;
	case MODEL_A_NOTE:
		INIT_TH_BY_MODEL(MODEL_A_NOTE);
		break;
	case MODEL_A31:
		INIT_TH_BY_MODEL(MODEL_A31);
		break;
	case MODEL_A41:
		INIT_TH_BY_MODEL(MODEL_A41);
		break;
	case MODEL_A51_5G:
		INIT_TH_BY_MODEL(MODEL_A51_5G);
		break;
	case MODEL_A71_5G:
		INIT_TH_BY_MODEL(MODEL_A71_5G);
		break;
	case MODEL_A71_G7_ET713_3PG:
	case MODEL_A71_G7_ET715S_2PB:
		if (this_sensor_type == FP_ET713)
		{
			INIT_TH_BY_MODEL(MODEL_A71_G7_ET713_3PG);
		}
		else if (this_sensor_type == FP_ET715S)
		{
			INIT_TH_BY_MODEL(MODEL_A71_G7_ET715S_2PB);
		}
		else
		{
			INIT_TH_BY_MODEL(MODEL_DEFAULT);
		}
		break;
	case MODEL_A42_5G:
		INIT_TH_BY_MODEL(MODEL_A42_5G);
		break;
	default:
		INIT_TH_BY_MODEL(MODEL_DEFAULT);
		break;
	}
}

// Need to be in consistant with enum model_type
const char *PHONE_MODEL[] = {
	"MODEL_A50",
	"MODEL_A70",
	"MODEL_A80",
	"MODEL_TAB_S6",
	"MODEL_A90",
	"MODEL_A30S_LENS_3PA",
	"MODEL_A30S_LENS_3PF",
	"MODEL_A50S",
	"MODEL_A70S",
	"MODEL_A51",
	"MODEL_A71",
	"MODEL_A91",
	"MODEL_A_NOTE",
	"MODEL_A31",
	"MODEL_A41",
	"MODEL_A51_5G",
	"MODEL_A71_G7_ET713_3PG",
	"MODEL_A71_G7_ET715S_2PB",
	"MODEL_A_71_G7",
	"MODEL_A42_5G" };

int get_phone_model(const char *MODEL_NAME, enum model_type *model_type, enum lens_type *lens_type, enum fp_type *sensor_type)
{
	if (_stricmp(MODEL_NAME, PHONE_MODEL[MODEL_A50]) == 0)
	{
		*model_type = MODEL_A50;
		*lens_type = LENS_2PA;
		*sensor_type = FP_ET713;
		return EGIS_OK;
	}
	else if (_stricmp(MODEL_NAME, PHONE_MODEL[MODEL_A70]) == 0)
	{
		*model_type = MODEL_A70;
		*lens_type = LENS_2PA;
		*sensor_type = FP_ET713;
		return EGIS_OK;
	}
	else if (_stricmp(MODEL_NAME, PHONE_MODEL[MODEL_A80]) == 0)
	{
		*model_type = MODEL_A80;
		*lens_type = LENS_2PA;
		*sensor_type = FP_ET713;
		return EGIS_OK;
	}
	else if (_stricmp(MODEL_NAME, PHONE_MODEL[MODEL_TAB_S6]) == 0)
	{
		*model_type = MODEL_TAB_S6;
		*lens_type = LENS_3PD;
		*sensor_type = FP_ET713;
		return EGIS_OK;
	}
	else if (_stricmp(MODEL_NAME, PHONE_MODEL[MODEL_A90]) == 0)
	{
		*model_type = MODEL_A90;
		*lens_type = LENS_3PF;
		*sensor_type = FP_ET715;
		return EGIS_OK;
	}
	else if (_stricmp(MODEL_NAME, PHONE_MODEL[MODEL_A30S_LENS_3PA]) == 0)
	{
		*model_type = MODEL_A30S_LENS_3PA;
		*lens_type = LENS_3PA;
		*sensor_type = FP_ET715;
		return EGIS_OK;
	}
	else if (_stricmp(MODEL_NAME, PHONE_MODEL[MODEL_A30S_LENS_3PF]) == 0)
	{
		*model_type = MODEL_A30S_LENS_3PF;
		*lens_type = LENS_3PF;
		*sensor_type = FP_ET715;
		return EGIS_OK;
	}
	else if (_stricmp(MODEL_NAME, PHONE_MODEL[MODEL_A50S]) == 0)
	{
		*model_type = MODEL_A50S;
		*lens_type = LENS_3PC;
		*sensor_type = FP_ET713;
		return EGIS_OK;
	}
	else if (_stricmp(MODEL_NAME, PHONE_MODEL[MODEL_A70S]) == 0)
	{
		*model_type = MODEL_A70S;
		*lens_type = LENS_2PA;
		*sensor_type = FP_ET713;
		return EGIS_OK;
	}
	else if (_stricmp(MODEL_NAME, PHONE_MODEL[MODEL_A51]) == 0)
	{
		*model_type = MODEL_A51;
		*lens_type = LENS_3PG;
		*sensor_type = FP_ET713;
		return EGIS_OK;
	}
	else if (_stricmp(MODEL_NAME, PHONE_MODEL[MODEL_A71]) == 0)
	{
		*model_type = MODEL_A71;
		*lens_type = LENS_3PG;
		*sensor_type = FP_ET713;
		return EGIS_OK;
	}
	else if (_stricmp(MODEL_NAME, PHONE_MODEL[MODEL_A91]) == 0)
	{
		*model_type = MODEL_A91;
		*lens_type = LENS_3PF;
		*sensor_type = FP_ET715;
		return EGIS_OK;
	}
	else if (_stricmp(MODEL_NAME, PHONE_MODEL[MODEL_A_NOTE]) == 0)
	{
		*model_type = MODEL_A_NOTE;
		*lens_type = LENS_3PG_LCE;
		*sensor_type = FP_ET713;
		return EGIS_OK;
	}
	else if (_stricmp(MODEL_NAME, PHONE_MODEL[MODEL_A31]) == 0)
	{
		*model_type = MODEL_A31;
		*lens_type = LENS_3PG;
		*sensor_type = FP_ET713;
		return EGIS_OK;
	}
	else if (_stricmp(MODEL_NAME, PHONE_MODEL[MODEL_A41]) == 0)
	{
		*model_type = MODEL_A41;
		*lens_type = LENS_3PA;
		*sensor_type = FP_ET715;
		return EGIS_OK;
	}
	else if (_stricmp(MODEL_NAME, PHONE_MODEL[MODEL_A51_5G]) == 0)
	{
		*model_type = MODEL_A51_5G;
		*lens_type = LENS_3PG;
		*sensor_type = FP_ET713;
		return EGIS_OK;
	}
	else if (_stricmp(MODEL_NAME, PHONE_MODEL[MODEL_A71_5G]) == 0)
	{
		*model_type = MODEL_A71_5G;
		*lens_type = LENS_3PG;
		*sensor_type = FP_ET713;
		return EGIS_OK;
	}
	else if (_stricmp(MODEL_NAME, PHONE_MODEL[MODEL_A71_G7_ET713_3PG]) == 0)
	{
		*model_type = MODEL_A71_G7_ET713_3PG;
		*lens_type = LENS_3PG;
		*sensor_type = FP_ET713;
		return EGIS_OK;
	}
	else if (_stricmp(MODEL_NAME, PHONE_MODEL[MODEL_A71_G7_ET715S_2PB]) == 0)
	{
		*model_type = MODEL_A71_G7_ET715S_2PB;
		*lens_type = LENS_2PB;
		*sensor_type = FP_ET715S;
		return EGIS_OK;
	}
	else if (_stricmp(MODEL_NAME, PHONE_MODEL[MODEL_A42_5G]) == 0)
	{
	*model_type = MODEL_A42_5G;
	*lens_type = LENS_3PG;
	*sensor_type = FP_ET713;
	return EGIS_OK;
	}
	else
	{
		*model_type = MODEL_UNKNOW;
		*lens_type = LENS_UNKNOW;
		*sensor_type = FP_NONE;
		return EGIS_MODEL_UNKNOWN;
	}
};

void dimension_from_sensor(enum fp_type sensor_type, int *width, int *height)
{
	switch (sensor_type)
	{
	case FP_ET711:
		*width = ET711_SENSOR_WIDTH;
		*height = ET711_SENSOR_HEIGHT;
		return;
	case FP_ET713:
		*width = ET713_SENSOR_WIDTH;
		*height = ET713_SENSOR_HEIGHT;
		return;
	case FP_ET715:
		*width = ET715_SENSOR_WIDTH;
		*height = ET715_SENSOR_HEIGHT;
		return;
	case FP_ET715S:
		*width = ET715S_SENSOR_WIDTH;
		*height = ET715S_SENSOR_HEIGHT;
		return;
	case FP_NONE:
	default:		
		*width = 0;
		*height = 0;
		return;
	}
}

void get_template_size(const char *MODEL_NAME, int *template_size)
{
	if (_stricmp(MODEL_NAME, PHONE_MODEL[MODEL_A51_5G]) == 0)
	{
		*template_size = 500 * 1024;
		return EGIS_OK;
	}
	else if (_stricmp(MODEL_NAME, PHONE_MODEL[MODEL_A42_5G]) == 0)
	{
		*template_size = 1000 * 1024;
		return EGIS_OK;
	}
	else
	{
		*template_size = 500 * 1024;
		return EGIS_MODEL_UNKNOWN;
	}
}

int init_model_setting(model_setting *session, const char *MODEL_NAME)
{
	int ret;
	int width, height;
	DEBUG(("Phone Model %s \n", MODEL_NAME));
	ret = get_phone_model(MODEL_NAME, &session->phone_model_type, &session->phone_lens_type, &session->phone_sensor_type);
	if (ret)
	{
		DEBUG(("Incorrect Phone Model \n"));
		fprintf(stderr, "Incorrect Phone Model\n");  //new
		return ret;
	}
	DEBUG(("Model init successful \n"));
	fprintf(stderr, "Model %s init successful \n", MODEL_NAME);  //new
	init_th_by_model_lens_sensor(session->phone_model_type, session->phone_lens_type, session->phone_sensor_type);
	dimension_from_sensor(session->phone_sensor_type, &width, &height);
	session->g_dry_finger_mode = MATCHER_API_DRY_FINGER_MODE_DISABLE;
	get_template_size(MODEL_NAME, &session->g_enroll_template_size); // TODO: Need to modified to fit each model ??
	session->g_enroll_redundant_level = threshold_manager.matcher_g5_redundant_level;
	session->g_enroll_quality_reject_level = threshold_manager.matcher_g5_reject_level;
	session->g_enroll_latent_reject_level = LATENT_REJECT_LEVEL_ACCEPT_ALL;
	session->g_normal_far_ratio = threshold_manager.normal_far_ratio;
	session->g_wash_hand_far_ratio = threshold_manager.wash_hand_far_ratio;
	session->g_easy_mode_2_far_ratio = threshold_manager.easy_mode_2_far_ratio;
	session->g_easy_mode_3_far_ratio = threshold_manager.easy_mode_3_far_ratio;
	session->g_latency_adjustment = 0;
	session->g_resolution = threshold_manager.matcher_g5_dpi;
	session->g_resolution_v2 = (int)threshold_manager.matcher_g5_dpi * 0.9;
	session->g_boost = 0;
	session->g_spd = FP_ENABLE_SPD;
	session->g_matcher_g5_spd_th = threshold_manager.matcher_g5_spd_th;
	session->g_mask_enable = 0;
	session->g_centroid_X = width/2;
	session->g_centroid_Y = height/2;
	session->g_radius = threshold_manager.matcher_g5_radius;
	session->g_dyn_mask_threshold = 0;
	session->g_latent_finger_check = 0;
	session->g_skip_failimage_learn = 0;
	session->g_update_learn_by_filename = 0;
	session->g_sensor_type = threshold_manager.matcher_g5_algo_type;
	DEBUG(("Variable assign successful \n"));
	return EGIS_OK;
}
const char *get_sensor_name(enum algo_api_sensor_type sensor)
{
	switch (sensor)
	{
	case FP_ALGOAPI_MODE_UNKNOWN:
		return "UNKNOWN";
	case FP_ALGOAPI_MODE_EGIS_ET528:
		return "ET528";
	case FP_ALGOAPI_MODE_EGIS_ET713_2Px:
		return "ET713_2Px";
	case FP_ALGOAPI_MODE_EGIS_ET713_2PA:
		return "ET713_2PA";
	case FP_ALGOAPI_MODE_EGIS_ET713S_2PB:
		return "ET713S_2PB";
	case FP_ALGOAPI_MODE_EGIS_ET713_2PA_S2PA4:
		return "ET713_2PA_S2PA4";
	case FP_ALGOAPI_MODE_EGIS_ET713_2PA_NEW:
		return "ET713_2PA_NEW";
	case FP_ALGOAPI_MODE_EGIS_ET713_3Px:
		return "ET713_3Px";
	case FP_ALGOAPI_MODE_EGIS_ET713_3PC:
		return "ET713_3PC";
	case FP_ALGOAPI_MODE_EGIS_ET713_3PD:
		return "ET713_3PD";
	case FP_ALGOAPI_MODE_EGIS_ET713_3PG_S3PG1:
		return "ET713_3PG_S3PG1";
	case FP_ALGOAPI_MODE_EGIS_ET713_3PG_S3PG2:
		return "ET713_3PG_S3PG2";
	case FP_ALGOAPI_MODE_EGIS_ET713_3PG_S3PG3:
		return "ET713_3PG_S3PG3";
	case FP_ALGOAPI_MODE_EGIS_ET713_3PG_S3PG3_Latency:
		return "ET713_3PG_S3PG3_Latency";
	case FP_ALGOAPI_MODE_EGIS_ET713_3PG_S3PG4:
		return "ET713_3PG_S3PG4";
	case FP_ALGOAPI_MODE_EGIS_ET713_3PG_S3PG5:
		return "ET713_3PG_S3PG5";
	case FP_ALGOAPI_MODE_EGIS_ET713_3PG_CH1E_SV:
		return "ET713_3PG_CH1E_SV";
	case FP_ALGOAPI_MODE_EGIS_ET713_3PG_CH1E_SB:
		return "ET713_3PG_CH1E_SB";
	case FP_ALGOAPI_MODE_EGIS_ET713_3PG_CH1J_SB:
		return "ET713_3PG_CH1J_SB";
	case FP_ALGOAPI_MODE_EGIS_ET713_3PG_CH1E_H:
		return "ET713_3PG_CH1E_H";
	case FP_ALGOAPI_MODE_EGIS_ET713_3PG_CH1B_H:
		return "ET713_3PG_CH1B_H";
	case FP_ALGOAPI_MODE_EGIS_ET713_3PCLA_CH1LA:
		return "ET713_3PCLA";
	case FP_ALGOAPI_MODE_EGIS_ET713_3PCLB_CH1SEA:
		return "ET713_3PCLB";
	case FP_ALGOAPI_MODE_EGIS_ET713_3PCLA_CH1LA_NEW:
		return "ET713_3PCLA_CH1LA_NEW";
	case FP_ALGOAPI_MODE_EGIS_ET713_3PC_CL1MH2:
		return "ET713_3PC_CL1MH2";
	case FP_ALGOAPI_MODE_EGIS_ET713_3PC_CL1MH2V:
		return "ET713_3PC_CL1MH2V";
	case FP_ALGOAPI_MODE_EGIS_ET713_3PCLC_CO1D151:
		return "ET713_3PCLC_CO1D151";
	case FP_ALGOAPI_MODE_EGIS_ET713_3PCLD_CO1A118:
		return "ET713_3PCLD_CO1A118";
	case FP_ALGOAPI_MODE_EGIS_ET713_3PG_CO1A118:
		return "ET713_3PG_CO1A118";
	case FP_ALGOAPI_MODE_EGIS_ET713_3PC_CS3ZE2:
		return "ET713_3PC_CS3ZE2";
	case FP_ALGOAPI_MODE_EGIS_ET713_3PC_CV1CPD1960:
		return "ET713_3PC_CV1CPD1960";
	case FP_ALGOAPI_MODE_EGIS_ET713_3PC_CL1MH2_CLT3:
		return "ET713_3PC_CL1MH2_CLT3";
	case FP_ALGOAPI_MODE_EGIS_ET715_3Px:
		return "ET715_3Px";
	case FP_ALGOAPI_MODE_EGIS_ET715_3PA:
		return "ET715_3PA";
	case FP_ALGOAPI_MODE_EGIS_ET715_3PE:
		return "ET715_3PE";
	case FP_ALGOAPI_MODE_EGIS_ET715_3PF:
		return "ET715_3PF";
	case FP_ALGOAPI_MODE_EGIS_ET715_3PF_S3PF5:
		return "ET715_3PF_S3PF5";
	case FP_ALGOAPI_MODE_EGIS_ET715_3PF_S3PF2:
		return "ET715_3PF_S3PF2";
	case FP_ALGOAPI_MODE_EGIS_ET715_3PA_S3PA2:
		return "ET715_3PA_S3PA2";
	case FP_ALGOAPI_MODE_EGIS_ET715_3PA_S3PA2_latency:
		return "ET715_3PA_S3PA2_latency";
	case FP_ALGOAPI_MODE_EGIS_ET715_3PF_CL1TIME:
		return "ET715_3PF_CL1TIME";
	case FP_ALGOAPI_MODE_EGIS_ET715_3PF_CL1CAY:
		return "ET715_3PF_CL1CAY";
	case FP_ALGOAPI_MODE_EGIS_ET701:
		return "ET701";
	case FP_ALGOAPI_MODE_EGIS_ET702:
		return "ET702";
	case FP_ALGOAPI_MODE_EGIS_ET702_SXC210:
		return "ET702_SXC210";
	case FP_ALGOAPI_MODE_EGIS_ET702_CH1M30:
		return "ET702_CH1M30";
	case FP_ALGOAPI_MODE_EGIS_ET702_CL1MH2:
		return "ET702_CL1MH2";
	case FP_ALGOAPI_MODE_EGIS_ET702_CH1M30_INV:
		return "ET702_CH1M30_INV";
	case FP_ALGOAPI_MODE_EGIS_ET702_CL1MH2_INV:
		return "ET702_CL1MH2_INV";
	case FP_ALGOAPI_MODE_EGIS_ET702_CL1MH2_C230:
		return "ET702_CL1MH2_C230";
	case FP_ALGOAPI_MODE_EGIS_ET702_INV:
		return "ET702_INV";
	case FP_ALGOAPI_MODE_EGIS_ET703_CH1M30:
		return "ET703_CH1M30";
	case FP_ALGOAPI_MODE_EGIS_ET715S_2PB_S2PB1:
		return "ET715S_2PB_S2PB1";
	case FP_ALGOAPI_MODE_EGIS_ET720_2PB_CH1M30:
		return "ET720_2PB_CH1M30";
	case FP_ALGOAPI_MODE_EGIS_ET901:
		return "ET901";
	default:
		return 0;
	}
}

// matcher_controller.c
BOOL get_mlq_t_table(enum model_type model, unsigned char **mlq_temp,
					 int *mlq_temp_size, int *mlq_temp_count,
					 enum lens_type lenstype)
{
	switch (model)
	{
	case MODEL_A50:
	case MODEL_A70:
	case MODEL_A80:
	case MODEL_A70S:
		mlq_temp[0] = g_a_mlq_b_000;
		mlq_temp[1] = g_a_mlq_b_001;
		mlq_temp[2] = g_a_mlq_b_002;
		mlq_temp[3] = g_a_mlq_b_003;
		mlq_temp[4] = g_a_mlq_b_004;
		mlq_temp_size[0] = G_A_MLQ_T_000_SIZE;
		mlq_temp_size[1] = G_A_MLQ_T_001_SIZE;
		mlq_temp_size[2] = G_A_MLQ_T_002_SIZE;
		mlq_temp_size[3] = G_A_MLQ_T_003_SIZE;
		mlq_temp_size[4] = G_A_MLQ_T_004_SIZE;
		*mlq_temp_count = 5;
		return TRUE;
	case MODEL_TAB_S6:
		mlq_temp[0] = g_b_mlq_b_000;
		mlq_temp[1] = g_b_mlq_b_001;
		mlq_temp[2] = g_b_mlq_b_002;
		mlq_temp_size[0] = G_B_MLQ_T_000_SIZE;
		mlq_temp_size[1] = G_B_MLQ_T_001_SIZE;
		mlq_temp_size[2] = G_B_MLQ_T_002_SIZE;
		*mlq_temp_count = 3;
		return TRUE;
	case MODEL_A50S:
		mlq_temp[0] = g_c_mlq_b_000;
		mlq_temp[1] = g_c_mlq_b_001;
		mlq_temp[2] = g_c_mlq_b_002;
		mlq_temp_size[0] = G_C_MLQ_T_000_SIZE;
		mlq_temp_size[1] = G_C_MLQ_T_001_SIZE;
		mlq_temp_size[2] = G_C_MLQ_T_002_SIZE;
		*mlq_temp_count = 3;
		return TRUE;
	case MODEL_A90:
		if (lenstype != LENS_3PF)
			return FALSE;
		mlq_temp[0] = g_d_mlq_b_000;
		mlq_temp_size[0] = G_D_MLQ_T_000_SIZE;
		*mlq_temp_count = 1;
		return TRUE;
	case MODEL_A91:
		mlq_temp[0] = g_e_mlq_b_000;
		mlq_temp[1] = g_e_mlq_b_001;
		mlq_temp[2] = g_e_mlq_b_002;
		mlq_temp_size[0] = G_E_MLQ_T_000_SIZE;
		mlq_temp_size[1] = G_E_MLQ_T_001_SIZE;
		mlq_temp_size[2] = G_E_MLQ_T_002_SIZE;
		*mlq_temp_count = 3;
		return TRUE;

	case MODEL_A30S_LENS_3PF:
	case MODEL_A30S_LENS_3PA:
		switch (lenstype)
		{
		case LENS_3PF:
			mlq_temp[0] = g_d_mlq_b_000;
			mlq_temp_size[0] = G_D_MLQ_T_000_SIZE;
			*mlq_temp_count = 1;
			return TRUE;
		case LENS_3PA:
			mlq_temp[0] = g_f_mlq_b_000;
			mlq_temp_size[0] = G_F_MLQ_T_000_SIZE;
			*mlq_temp_count = 1;
			return TRUE;
		default:
			break;
		}
		break;

	case MODEL_A51:
	case MODEL_A_NOTE:
	case MODEL_A31:
	case MODEL_A71:
	case MODEL_A51_5G:
	case MODEL_A71_5G:
	case MODEL_A71_G7_ET713_3PG:
	case MODEL_A42_5G:
		mlq_temp[0] = g_g_mlq_b_000;
		mlq_temp[1] = g_g_mlq_b_001;
		mlq_temp[2] = g_g_mlq_b_002;
		mlq_temp_size[0] = G_G_MLQ_T_000_SIZE;
		mlq_temp_size[1] = G_G_MLQ_T_001_SIZE;
		mlq_temp_size[2] = G_G_MLQ_T_002_SIZE;
		*mlq_temp_count = 3;
		return TRUE;
	default:
		break;
	}
	return FALSE;
}

// matcher_controller.c
BOOL get_mlq_f_table(enum model_type model, int mlq_index,
					 unsigned char **mlq_f, int *mlq_f_size,
					 enum lens_type lenstype)
{
	switch (model)
	{
	case MODEL_A50:
	case MODEL_A70:
	case MODEL_A80:
	case MODEL_A70S:
		switch (mlq_index)
		{
		case 0:
			if (model == MODEL_A70S)
			{
				mlq_f[0] = g_a_mlq_f4_000;
				mlq_f_size[0] = G_A_MLQ_F4_000_SIZE;
				mlq_f[1] = g_a_mlq_f4_001;
				mlq_f_size[1] = G_A_MLQ_F4_001_SIZE;
				mlq_f[2] = g_a_mlq_f4_002;
				mlq_f_size[2] = G_A_MLQ_F4_002_SIZE;
				mlq_f[3] = g_a_mlq_f4_003;
				mlq_f_size[3] = G_A_MLQ_F4_003_SIZE;
				mlq_f[4] = g_a_mlq_f4_004;
				mlq_f_size[4] = G_A_MLQ_F4_004_SIZE;
			}
			else
			{
				mlq_f[0] = g_a_mlq_f0_000;
				mlq_f_size[0] = G_A_MLQ_F0_000_SIZE;
				mlq_f[1] = g_a_mlq_f0_001;
				mlq_f_size[1] = G_A_MLQ_F0_001_SIZE;
				mlq_f[2] = g_a_mlq_f0_002;
				mlq_f_size[2] = G_A_MLQ_F0_002_SIZE;
				mlq_f[3] = g_a_mlq_f0_003;
				mlq_f_size[3] = G_A_MLQ_F0_003_SIZE;
				mlq_f[4] = g_a_mlq_f0_004;
				mlq_f_size[4] = G_A_MLQ_F0_004_SIZE;
			}
			return TRUE;
		case 1:
			mlq_f[0] = g_a_mlq_f1_000;
			mlq_f_size[0] = G_A_MLQ_F1_000_SIZE;
			mlq_f[1] = g_a_mlq_f1_001;
			mlq_f_size[1] = G_A_MLQ_F1_001_SIZE;
			mlq_f[2] = g_a_mlq_f1_002;
			mlq_f_size[2] = G_A_MLQ_F1_002_SIZE;
			mlq_f[3] = g_a_mlq_f1_003;
			mlq_f_size[3] = G_A_MLQ_F1_003_SIZE;
			mlq_f[4] = g_a_mlq_f1_004;
			mlq_f_size[4] = G_A_MLQ_F1_004_SIZE;
			return TRUE;
		case 2:
			mlq_f[0] = g_a_mlq_f2_000;
			mlq_f_size[0] = G_A_MLQ_F2_000_SIZE;
			mlq_f[1] = g_a_mlq_f2_001;
			mlq_f_size[1] = G_A_MLQ_F2_001_SIZE;
			mlq_f[2] = g_a_mlq_f2_002;
			mlq_f_size[2] = G_A_MLQ_F2_002_SIZE;
			mlq_f[3] = g_a_mlq_f2_003;
			mlq_f_size[3] = G_A_MLQ_F2_003_SIZE;
			mlq_f[4] = g_a_mlq_f2_004;
			mlq_f_size[4] = G_A_MLQ_F2_004_SIZE;
			return TRUE;
		case 4:
			mlq_f[0] = g_a_mlq_f4_000;
			mlq_f_size[0] = G_A_MLQ_F4_000_SIZE;
			mlq_f[1] = g_a_mlq_f4_001;
			mlq_f_size[1] = G_A_MLQ_F4_001_SIZE;
			mlq_f[2] = g_a_mlq_f4_002;
			mlq_f_size[2] = G_A_MLQ_F4_002_SIZE;
			mlq_f[3] = g_a_mlq_f4_003;
			mlq_f_size[3] = G_A_MLQ_F4_003_SIZE;
			mlq_f[4] = g_a_mlq_f4_004;
			mlq_f_size[4] = G_A_MLQ_F4_004_SIZE;
			return TRUE;
		}

		break;
	case MODEL_TAB_S6:
		switch (mlq_index)
		{
		case 0:
			mlq_f[0] = g_b_mlq_f0_000;
			mlq_f_size[0] = G_B_MLQ_F0_000_SIZE;
			mlq_f[1] = g_b_mlq_f0_001;
			mlq_f_size[1] = G_B_MLQ_F0_001_SIZE;
			mlq_f[2] = g_b_mlq_f0_002;
			mlq_f_size[2] = G_B_MLQ_F0_002_SIZE;
			mlq_f[3] = g_b_mlq_f0_003;
			mlq_f_size[3] = G_B_MLQ_F0_003_SIZE;
			mlq_f[4] = g_b_mlq_f0_004;
			mlq_f_size[4] = G_B_MLQ_F0_004_SIZE;
			return TRUE;
		case 1:
			mlq_f[0] = g_b_mlq_f1_000;
			mlq_f_size[0] = G_B_MLQ_F1_000_SIZE;
			mlq_f[1] = g_b_mlq_f1_001;
			mlq_f_size[1] = G_B_MLQ_F1_001_SIZE;
			mlq_f[2] = g_b_mlq_f1_002;
			mlq_f_size[2] = G_B_MLQ_F1_002_SIZE;
			mlq_f[3] = g_b_mlq_f1_003;
			mlq_f_size[3] = G_B_MLQ_F1_003_SIZE;
			mlq_f[4] = g_b_mlq_f1_004;
			mlq_f_size[4] = G_B_MLQ_F1_004_SIZE;
			return TRUE;
		case 2:
			mlq_f[0] = g_b_mlq_f2_000;
			mlq_f_size[0] = G_B_MLQ_F2_000_SIZE;
			mlq_f[1] = g_b_mlq_f2_001;
			mlq_f_size[1] = G_B_MLQ_F2_001_SIZE;
			mlq_f[2] = g_b_mlq_f2_002;
			mlq_f_size[2] = G_B_MLQ_F2_002_SIZE;
			mlq_f[3] = g_b_mlq_f2_003;
			mlq_f_size[3] = G_B_MLQ_F2_003_SIZE;
			mlq_f[4] = g_b_mlq_f2_004;
			mlq_f_size[4] = G_B_MLQ_F2_004_SIZE;
			return TRUE;
		}
		break;
	case MODEL_A50S:
		switch (mlq_index)
		{
		case 0:
			mlq_f[0] = g_c_mlq_f0_000;
			mlq_f_size[0] = G_C_MLQ_F0_000_SIZE;
			mlq_f[1] = g_c_mlq_f0_001;
			mlq_f_size[1] = G_C_MLQ_F0_001_SIZE;
			mlq_f[2] = g_c_mlq_f0_002;
			mlq_f_size[2] = G_C_MLQ_F0_002_SIZE;
			mlq_f[3] = g_c_mlq_f0_003;
			mlq_f_size[3] = G_C_MLQ_F0_003_SIZE;
			mlq_f[4] = g_c_mlq_f0_004;
			mlq_f_size[4] = G_C_MLQ_F0_004_SIZE;
			return TRUE;
		case 1:
			mlq_f[0] = g_c_mlq_f1_000;
			mlq_f_size[0] = G_C_MLQ_F1_000_SIZE;
			mlq_f[1] = g_c_mlq_f1_001;
			mlq_f_size[1] = G_C_MLQ_F1_001_SIZE;
			mlq_f[2] = g_c_mlq_f1_002;
			mlq_f_size[2] = G_C_MLQ_F1_002_SIZE;
			mlq_f[3] = g_c_mlq_f1_003;
			mlq_f_size[3] = G_C_MLQ_F1_003_SIZE;
			mlq_f[4] = g_c_mlq_f1_004;
			mlq_f_size[4] = G_C_MLQ_F1_004_SIZE;
			return TRUE;
		case 2:
			mlq_f[0] = g_c_mlq_f2_000;
			mlq_f_size[0] = G_C_MLQ_F2_000_SIZE;
			mlq_f[1] = g_c_mlq_f2_001;
			mlq_f_size[1] = G_C_MLQ_F2_001_SIZE;
			mlq_f[2] = g_c_mlq_f2_002;
			mlq_f_size[2] = G_C_MLQ_F2_002_SIZE;
			mlq_f[3] = g_c_mlq_f2_003;
			mlq_f_size[3] = G_C_MLQ_F2_003_SIZE;
			mlq_f[4] = g_c_mlq_f2_004;
			mlq_f_size[4] = G_C_MLQ_F2_004_SIZE;
			return TRUE;
		}
		break;
	case MODEL_A90:
		if (lenstype != LENS_3PF)
			return FALSE;
		switch (mlq_index)
		{
		case 0:
			mlq_f[0] = g_d_mlq_f0_000;
			mlq_f_size[0] = G_D_MLQ_F0_000_SIZE;
			mlq_f[1] = g_d_mlq_f0_001;
			mlq_f_size[1] = G_D_MLQ_F0_001_SIZE;
			mlq_f[2] = g_d_mlq_f0_002;
			mlq_f_size[2] = G_D_MLQ_F0_002_SIZE;
			mlq_f[3] = g_d_mlq_f0_003;
			mlq_f_size[3] = G_D_MLQ_F0_003_SIZE;
			mlq_f[4] = g_d_mlq_f0_004;
			mlq_f_size[4] = G_D_MLQ_F0_004_SIZE;
			return TRUE;
		}
		break;
	case MODEL_A91:
		switch (mlq_index)
		{
		case 0:
			mlq_f[0] = g_e_mlq_f0_000;
			mlq_f_size[0] = G_E_MLQ_F0_000_SIZE;
			mlq_f[1] = g_e_mlq_f0_001;
			mlq_f_size[1] = G_E_MLQ_F0_001_SIZE;
			mlq_f[2] = g_e_mlq_f0_002;
			mlq_f_size[2] = G_E_MLQ_F0_002_SIZE;
			mlq_f[3] = g_e_mlq_f0_003;
			mlq_f_size[3] = G_E_MLQ_F0_003_SIZE;
			mlq_f[4] = g_e_mlq_f0_004;
			mlq_f_size[4] = G_E_MLQ_F0_004_SIZE;
			return TRUE;
		case 1:
			mlq_f[0] = g_e_mlq_f1_000;
			mlq_f_size[0] = G_E_MLQ_F1_000_SIZE;
			mlq_f[1] = g_e_mlq_f1_001;
			mlq_f_size[1] = G_E_MLQ_F1_001_SIZE;
			mlq_f[2] = g_e_mlq_f1_002;
			mlq_f_size[2] = G_E_MLQ_F1_002_SIZE;
			mlq_f[3] = g_e_mlq_f1_003;
			mlq_f_size[3] = G_E_MLQ_F1_003_SIZE;
			mlq_f[4] = g_e_mlq_f1_004;
			mlq_f_size[4] = G_E_MLQ_F1_004_SIZE;
			return TRUE;
		case 2:
			mlq_f[0] = g_e_mlq_f2_000;
			mlq_f_size[0] = G_E_MLQ_F2_000_SIZE;
			mlq_f[1] = g_e_mlq_f2_001;
			mlq_f_size[1] = G_E_MLQ_F2_001_SIZE;
			mlq_f[2] = g_e_mlq_f2_002;
			mlq_f_size[2] = G_E_MLQ_F2_002_SIZE;
			mlq_f[3] = g_e_mlq_f2_003;
			mlq_f_size[3] = G_E_MLQ_F2_003_SIZE;
			mlq_f[4] = g_e_mlq_f2_004;
			mlq_f_size[4] = G_E_MLQ_F2_004_SIZE;
			return TRUE;
		}
		break;
	case MODEL_A30S_LENS_3PF:
	case MODEL_A30S_LENS_3PA:
		switch (lenstype)
		{
		case LENS_3PF:
			switch (mlq_index)
			{
			case 0:
				mlq_f[0] = g_d_mlq_f0_000;
				mlq_f_size[0] = G_D_MLQ_F0_000_SIZE;
				mlq_f[1] = g_d_mlq_f0_001;
				mlq_f_size[1] = G_D_MLQ_F0_001_SIZE;
				mlq_f[2] = g_d_mlq_f0_002;
				mlq_f_size[2] = G_D_MLQ_F0_002_SIZE;
				mlq_f[3] = g_d_mlq_f0_003;
				mlq_f_size[3] = G_D_MLQ_F0_003_SIZE;
				mlq_f[4] = g_d_mlq_f0_004;
				mlq_f_size[4] = G_D_MLQ_F0_004_SIZE;
				return TRUE;
			}
			break;
		case LENS_3PA:
			switch (mlq_index)
			{
			case 0:
				mlq_f[0] = g_f_mlq_f0_000;
				mlq_f_size[0] = G_F_MLQ_F0_000_SIZE;
				mlq_f[1] = g_f_mlq_f0_001;
				mlq_f_size[1] = G_F_MLQ_F0_001_SIZE;
				mlq_f[2] = g_f_mlq_f0_002;
				mlq_f_size[2] = G_F_MLQ_F0_002_SIZE;
				mlq_f[3] = g_f_mlq_f0_003;
				mlq_f_size[3] = G_F_MLQ_F0_003_SIZE;
				mlq_f[4] = g_f_mlq_f0_004;
				mlq_f_size[4] = G_F_MLQ_F0_004_SIZE;
				return TRUE;
			}
			break;
		default:
			break;
		}
		break;

	case MODEL_A51:
	case MODEL_A_NOTE:
	case MODEL_A31:
	case MODEL_A71:
	case MODEL_A51_5G:
	case MODEL_A71_5G:
	case MODEL_A71_G7_ET713_3PG:
	case MODEL_A42_5G:
		switch (mlq_index)
		{
		case 0:
			mlq_f[0] = g_g_mlq_f0_000;
			mlq_f_size[0] = G_G_MLQ_F0_000_SIZE;
			mlq_f[1] = g_g_mlq_f0_001;
			mlq_f_size[1] = G_G_MLQ_F0_001_SIZE;
			mlq_f[2] = g_g_mlq_f0_002;
			mlq_f_size[2] = G_G_MLQ_F0_002_SIZE;
			mlq_f[3] = g_g_mlq_f0_003;
			mlq_f_size[3] = G_G_MLQ_F0_003_SIZE;
			mlq_f[4] = g_g_mlq_f0_004;
			mlq_f_size[4] = G_G_MLQ_F0_004_SIZE;
			return TRUE;
		case 1:
			mlq_f[0] = g_g_mlq_f1_000;
			mlq_f_size[0] = G_G_MLQ_F1_000_SIZE;
			mlq_f[1] = g_g_mlq_f1_001;
			mlq_f_size[1] = G_G_MLQ_F1_001_SIZE;
			mlq_f[2] = g_g_mlq_f1_002;
			mlq_f_size[2] = G_G_MLQ_F1_002_SIZE;
			mlq_f[3] = g_g_mlq_f1_003;
			mlq_f_size[3] = G_G_MLQ_F1_003_SIZE;
			mlq_f[4] = g_g_mlq_f1_004;
			mlq_f_size[4] = G_G_MLQ_F1_004_SIZE;
			return TRUE;
		case 2:
			mlq_f[0] = g_g_mlq_f2_000;
			mlq_f_size[0] = G_G_MLQ_F2_000_SIZE;
			mlq_f[1] = g_g_mlq_f2_001;
			mlq_f_size[1] = G_G_MLQ_F2_001_SIZE;
			mlq_f[2] = g_g_mlq_f2_002;
			mlq_f_size[2] = G_G_MLQ_F2_002_SIZE;
			mlq_f[3] = g_g_mlq_f2_003;
			mlq_f_size[3] = G_G_MLQ_F2_003_SIZE;
			mlq_f[4] = g_g_mlq_f2_004;
			mlq_f_size[4] = G_G_MLQ_F2_004_SIZE;
			return TRUE;
		}
		break;

	default:
		break;
	}
	return FALSE;
}

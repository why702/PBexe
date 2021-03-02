/******************************************************************************\
|*                                                                            *|
|*  dl_api.h                                                                  *|
|*  Version: 1.0.0.0                                                          *|
|*  Date: 2020/01/13                                                          *|
|*  Revise Date: 2020/01/13                                                   *|
|*  Copyright (C) 2007-2020 Egis Technology Inc.                              *|
|*                                                                            *|
\******************************************************************************/
#ifndef dl_api_h
#define dl_api_h
#include "ml_type.h"

#define DL_API_VERSION "1.0.1.31"

typedef struct _LatentInput {
	int dx; 
	int dy;
	int rot;
	int qty_1;
	int qty_2;
	int score;
	int q3_1;
	int q3_2;
	int isp_1;
	int isp_2;
	int egp_1; 
	int egp_2;
} LatentInput;

typedef struct _LatentInput_SDK {
	int qty_1;
	int qty_2;
	int q3_1;
	int q3_2;
	int isp_1;
	int isp_2;
	int egp_1;
	int egp_2;
	int Mc_1;
	int Mc_2;
	int DLx_1;
	int DLy_1;
	int DL2_1;
	int DLs_1;
	int cDLx_1;
	int cDLy_1;
	int DLx_2;
	int DLy_2;
	int DL2_2;
	int DLs_2;
	int cDLx_2;
	int cDLy_2;
} LatentInput_SDK;

void get_ML_lib_version(unsigned char *version);

int MLgenerate_edge(unsigned char *img, unsigned char *dst_img, int  *width, int *height, int h_extend, int w_extend, int *wgt, int *mem_buf);
int MLgenerate_edge_7to3(unsigned char *img, unsigned char *dst_img, int *width, int *height, int h_extend, int w_extend, int *wgt, int *mem_buf);

void *MLdescriptor_init();
void MLdescriptor_uninit(void *data);
void MLdescriptor_32(unsigned char *img8, int block_size, int *wgt, unsigned char *desc, void *ctx);
void MLdescriptor_16(unsigned char *img8, int block_size, int *wgt, unsigned char *desc, void *ctx);

int MLmatch_predict_ET713_36p(unsigned short *img_a, unsigned short *img_b, int width, int height, int *wgt);
int MLmatch_predict_ET713_46p(unsigned short *img_a, unsigned short *img_b, int width, int height, int *wgt);
int MLmatch_predict_ET713_51p(unsigned short *img_a, unsigned short *img_b, int width, int height, int *wgt);
int MLmatch_predict_ET713_51p_dist128(unsigned short *img_a, unsigned short *img_b, int width, int height, int *wgt);
int MLmatch_predict_ET713_31p_QTY(unsigned short *img_a, unsigned short *img_b, short *img_c, short *img_d, int width, int height, int *wgt);

int MLmatch_predict_ET528_50p(unsigned short *img_a, unsigned short *img_b, int width, int height, int *wgt);
int MLmatch_predict_ET528_50x9(unsigned short *img_a, unsigned short *img_b, int width, int height, int *wgt);
int MLmatch_predict_ET528_50x9p(unsigned short *img_a, unsigned short *img_b, int width, int height, int *wgt);
int MLmatch_predict_ET522_18p(unsigned short *img_a, unsigned short *img_b, int width, int height, int *wgt);
int MLmatch_predict_ET522_14p(unsigned short *img_a, unsigned short *img_b, int width, int height, int *wgt);
int MLvdm_528_check(unsigned char *img8, int width, int height, int *wgt);
int MLfake_fingerprint_test_516_int8(unsigned char *img8, int width, int height, int *wgt);
int MLfake_fingerprint_test_516_int16(unsigned char *img8, int width, int height, int *wgt);

int MLvdm_check(unsigned char *img8, int width, int height, int *wgt);

int MLmatch_predict_G6_31p(unsigned short *img_a, unsigned short *img_b, int width, int height, int *wgt);

int MLmatch_predict_G6_31p_dist128_N(unsigned short *img_a, unsigned short *img_b, int width, int height, int *wgt);
int MLmatch_predict_G6_31p_dist128_Nm(unsigned short *img_a, unsigned short *img_b, int width, int height, int *wgt);

int MLfinger_detect_three_type(unsigned char *image, int width, int height, int col_scan, int row_scan, int *wgt, VggResult *vgg_ret);
int MLfinger_detect_four_type(unsigned char *image, int width, int height, int col_scan, int row_scan, int *wgt, VggResult *vgg_ret);
int MLfinger_detect_three_type_circle(unsigned char *image, int width, int height, int col_scan, int row_scan, int radius, int *wgt, VggResult *vgg_ret);
int MLfinger_detect_three_type_16(unsigned char *image, int width, int height, int col_scan, int row_scan, int *wgt, VggResult *vgg_ret);

int MLlatent_predict(int q1, int q2, int score, int dx, int dy, int dtheda, int *wgt);
int MLlatent_predict_G5_pair(LatentInput *input, int *wgt);

int MLqty_predict_ET713_32(int *img_padding, int width, int height, int *layer_1, int *wgt);

int MLbds_compatibility(unsigned short *raw, unsigned short *bkg, int w, int h, int hc, int *wgt);
//int *MLperfect_gen(unsigned short *raw, unsigned short *w_bkg, unsigned short *b_bkg, int w, int h, int raw_hc, int k_hc, int *wgt);
int *MLperfect_gen(unsigned short *raw, unsigned short *w_bkg, unsigned short *b_bkg, int w, int h, int hc, int *wgt);

#endif

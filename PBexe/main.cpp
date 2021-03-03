/******************************************************************************\
|*                                                                            *|
|*  main.c                                                                    *|
|*  Version: v0.9.9.9_2018_1129                                               *|
|*  Date: 2017/06/30                                                          *|
|*  Revise Date: 2018/11/30                                                   *|
|*  Copyright (C) 2007-2018 Egis Technology Inc.                              *|
|*                                                                            *|
\******************************************************************************/
#include <string>

#include "../g5matcher/g5_match.h"
#include "fileio.h"

using namespace std;

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

int main(int argc, char** argv) {
    if (argc == 3) {
        string sImg0 = *(argv + 1);
        string sImg1 = *(argv + 2);

        printf("image0 = %s\n", sImg0.c_str());
        printf("image1 = %s\n", sImg1.c_str());

        int w = 200, h = 200;

        unsigned char* pimg0 = read_8bit_bin_file(sImg0.c_str(), w, h);
        if (pimg0 == NULL) {
            printf("Load image0 file fail\r\n");
            return -1;
        }
        unsigned char* pimg1 = read_8bit_bin_file(sImg1.c_str(), w, h);
        if (pimg1 == NULL) {
            PLAT_FREE(pimg0);
            printf("Load image1 file fail\r\n");
            return -1;
        }

        int match_score = 0, rot = 0, dx = 0, dy = 0;
        images_compare_(&pimg0, &pimg1, w, h, &match_score, &rot, &dx, &dy);
		printf("match_score = %i, rot = %i, dx = %i, dy = %i, \n", match_score, rot, dx, dy);
        PLAT_FREE(pimg0);
        PLAT_FREE(pimg1);
    }
	else
	{
		printf("argc = %i\n", argc);
	}
    return 0;
}

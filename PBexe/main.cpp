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
#include "merge_opencv.h"

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
    if (argc == 3 || argc == 4) {
        string sImg0 = *(argv + 1);
        string sImg1 = *(argv + 2);
        string sShow = "";
        if (argc == 4) {
            sShow = *(argv + 3);
        }

        printf("image0 = %s\n", sImg0.c_str());
        printf("image1 = %s\n", sImg1.c_str());

        int w = 200, h = 200;
        unsigned char* pimg0 = NULL;
        unsigned char* pimg1 = NULL;
        MergeOpencv mergeOpencv;

        if (sImg0.find(".png") != std::string::npos) {
            pimg0 = (unsigned char*)plat_alloc(w * h * sizeof(unsigned char));
            if (pimg0 == NULL) {
                printf("pimg0 == NULL\n");
                return -1;
            }
            if (!mergeOpencv.ReadPng(sImg0, pimg0, w, h)) {
                printf("ReadPng image0 file fail\n");
                return -1;
            };
        } else {
           pimg0 = read_8bit_bin_file(sImg0.c_str(), w, h);
            if (pimg0 == NULL) {
                printf("Load image0 file fail\n");
                return -1;
            }
        }

		if (sImg1.find(".png") != std::string::npos) {
			pimg1 = (unsigned char*)plat_alloc(w * h * sizeof(unsigned char));
			if (pimg1 == NULL) {
				printf("pimg1 == NULL\n");
				return -1;
			}
			if (!mergeOpencv.ReadPng(sImg1, pimg1, w, h)) {
				printf("ReadPng image1 file fail\n");
				return -1;
			};
		}
		else {
			pimg1 = read_8bit_bin_file(sImg1.c_str(), w, h);
			if (pimg1 == NULL) {
				printf("Load image1 file fail\n");
				return -1;
			}
		}

        U82CSV("pimg0.csv", pimg0, w, h);

        int match_score = 0, rot = 0, dx = 0, dy = 0;
        images_compare_(&pimg0, &pimg1, w, h, &match_score, &rot, &dx, &dy);
        printf("w = %i, h = %i, match_score = %i, rot = %i, dx = %i, dy = %i\n", w, h, match_score,
               rot, dx, dy);

        U82CSV("pimg0_.csv", pimg0, w, h);

        if (sShow == "s" || sShow == "-s" || sShow == "S" || sShow == "-S") {
            mergeOpencv.Merge(pimg0, pimg1, w, h, match_score, rot, dx, dy);
        }

        PLAT_FREE(pimg0);
        PLAT_FREE(pimg1);
    } else {
        printf("argc = %i\n", argc);
    }
    return 0;
}

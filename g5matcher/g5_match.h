#ifndef G5_MATCH_H_
#define G5_MATCH_H_

#ifdef __cplusplus
extern "C" {
#endif

//#include <stdio.h>
//#include <stdlib.h>
	
struct algo_info {
    int sensor_type;
    int radius;
    int resolution;
};

void images_compare_(unsigned char** raw1, unsigned char** raw2, int w, int h, int* match_score,
                     int* rot, int* dx, int* dy);

void images_compare_by_algo(unsigned char** raw1, unsigned char** raw2, int w, int h,
                            int* match_score, int* rot, int* dx, int* dy,
                            struct algo_info* algo_info);

#ifdef __cplusplus
}
#endif

#endif

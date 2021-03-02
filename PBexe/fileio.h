#ifndef FILEIO_H_
#define FILEIO_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include <stdlib.h>

#define LITTLE_ENDIAN 0
#define BIG_ENDIAN 1

int *read_bin_file(const char *fn, int width, int height, int endian);
int *read_bin_file_head(const char *fn, int width, int height, int endian,
                        int head_size);

unsigned char *read_8bit_bin_file(const char *fn, int width, int height);
unsigned char *read_8bit_bin_file_head(const char *fn, int width, int height,
                                       int head_size);

void write_bin_file(const char *fn, int *img, int width, int height,
                    int endian);
void write_USbin_file(const char *fn, unsigned short *img, int width,
                      int height, int endian);

unsigned char *read_U8bin_file(const char *fn, int width, int height);

void write_U8bin_file(const char *fn, unsigned char *img, int width,
                      int height);

void float2BMP_color(const char *filename, float *RG_map, float *BG_map,
                     int rows, int cols);
void int2BMP_color(const char *filename, int *R_map, int *G_map, int *B_map,
                   int rows, int cols);

void ppmWrite(const char *filename, int rows, int cols, unsigned char *image,
              char *comment);

void pgmWrite(const char *filename, int rows, int cols, unsigned char *image,
              char *comment);

// void pngWrite(const char* filename, int rows, int cols, unsigned char*
// image);

void int2CSV(const char *filename, int *img, int width, int height);
void US2CSV(const char *filename, unsigned short *img, int width, int height);
void U82CSV(const char *filename, unsigned char *_pimg, int width, int height);

void normalize_int2UINT8(int *input, unsigned char *output, int SZ);
void normalize_float2UINT8(float *input, unsigned char *output, int SZ);

#ifdef __cplusplus
}
#endif

#endif

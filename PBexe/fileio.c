#include "fileio.h"
//#include "lodepng.h"
#include <errno.h>
#include <string.h>

int* read_bin_file(const char* fn, int width, int height, int endian) {
    FILE* f;
    unsigned char SWAP[2];

    int* img = (int*)malloc(width * height * sizeof(int));
    if (img == NULL) {
        return NULL;
    }
    unsigned short* tmp = (unsigned short*)malloc(width * height * sizeof(unsigned short));
    if (tmp == NULL) {
        free(img);
        return NULL;
    }

    // for long file name
    char fn_long[1024];
    sprintf_s(fn_long, 1024, "\\\\?\\%s", fn);

    if (fopen_s(&f, fn, "rb") == 0) {
        fread(tmp, sizeof(unsigned short), width * height, f);
        for (int i = 0; i < width * height; i++) {
            if (endian == BIG_ENDIAN) {
                SWAP[0] = (tmp[i] & 0xFF00) >> 8;
                SWAP[1] = tmp[i] & 0x00FF;
                img[i] = ((SWAP[1] << 8) + SWAP[0]);
            } else
                img[i] = tmp[i];
        }

        fclose(f);
    } else if (fopen_s(&f, fn_long, "rb") == 0) {
        fread(tmp, sizeof(unsigned short), width * height, f);
        for (int i = 0; i < width * height; i++) {
            if (endian == BIG_ENDIAN) {
                SWAP[0] = (tmp[i] & 0xFF00) >> 8;
                SWAP[1] = tmp[i] & 0x00FF;
                img[i] = ((SWAP[1] << 8) + SWAP[0]);
            } else
                img[i] = tmp[i];
        }

        fclose(f);
    } else {
        char errmsg[500];
#ifdef BILL_DEBUG
        printf("Error: %s", strerror_s(errmsg, 500, errno));
#endif
        free(img);
        free(tmp);
        return NULL;
    }

    free(tmp);
    return img;
}

int* read_bin_file_head(const char* fn, int width, int height, int endian, int head_size) {
    int i, j;
    FILE* f;
    unsigned char SWAP[2];

    int* img = (int*)malloc(width * height * sizeof(int));
    if (img == NULL) {
        return NULL;
    }
    unsigned short* tmp =
        (unsigned short*)malloc((head_size + width * height) * sizeof(unsigned short));

    if (tmp == NULL) {
        free(img);
        return NULL;
    }

    // for long file name
    char fn_long[1024];
    sprintf_s(fn_long, 1024, "\\\\?\\%s", fn);

    if (fopen_s(&f, fn, "rb") == 0) {
        fread(tmp, sizeof(unsigned short), head_size + width * height, f);
        for (i = head_size, j = 0; i < head_size + width * height; i++, j++) {
            if (endian == BIG_ENDIAN) {
                SWAP[0] = (tmp[i] & 0xFF00) >> 8;
                SWAP[1] = tmp[i] & 0x00FF;
                img[j] = ((SWAP[1] << 8) + SWAP[0]);
            } else
                img[j] = tmp[i];
        }

        fclose(f);
    } else if (fopen_s(&f, fn_long, "rb") == 0) {
        fread(tmp, sizeof(unsigned short), head_size + width * height, f);
        for (i = head_size, j = 0; i < head_size + width * height; i++, j++) {
            if (endian == BIG_ENDIAN) {
                SWAP[0] = (tmp[i] & 0xFF00) >> 8;
                SWAP[1] = tmp[i] & 0x00FF;
                img[j] = ((SWAP[1] << 8) + SWAP[0]);
            } else
                img[j] = tmp[i];
        }

        fclose(f);
    } else {
        char errmsg[500];
#ifdef BILL_DEBUG
        printf("Error: %s", strerror_s(errmsg, 500, errno));
#endif
        free(img);
        free(tmp);
        return NULL;
    }

    free(tmp);
    return img;
}

unsigned char* read_8bit_bin_file(const char* fn, int width, int height) {
    FILE* f;

    unsigned char* img = (unsigned char*)malloc(width * height * sizeof(unsigned char));
    if (img == NULL) {
        return NULL;
    }

    // for long file name
    char fn_long[1024];
    sprintf_s(fn_long, 1024, "\\\\?\\%s", fn);

    if (fopen_s(&f, fn, "rb") == 0) {
        fread(img, sizeof(unsigned char), width * height, f);
        fclose(f);
    } else if (fopen_s(&f, fn_long, "rb") == 0) {
        fread(img, sizeof(unsigned char), width * height, f);
        fclose(f);
    } else {
        char errmsg[500];
#ifdef BILL_DEBUG
        printf("Error: %s", strerror_s(errmsg, 500, errno));
#endif
        free(img);
        return NULL;
    }
    return img;
}

unsigned char* read_8bit_bin_file_head(const char* fn, int width, int height, int head_size) {
    int i, j;
    FILE* f;

    unsigned char* tmp =
        (unsigned char*)malloc((head_size + width * height) * sizeof(unsigned char));
    if (tmp == NULL) {
        return NULL;
    }
    unsigned char* img = (unsigned char*)malloc(width * height * sizeof(unsigned char));
    if (img == NULL) {
        free(tmp);
        return NULL;
    }

    // for long file name
    char fn_long[1024];
    sprintf_s(fn_long, 1024, "\\\\?\\%s", fn);

    if (fopen_s(&f, fn, "rb") == 0) {
        fread(tmp, sizeof(unsigned char), head_size + width * height, f);
        fclose(f);
    } else if (fopen_s(&f, fn_long, "rb") == 0) {
        fread(tmp, sizeof(unsigned char), head_size + width * height, f);
        fclose(f);
    } else {
        char errmsg[500];
#ifdef BILL_DEBUG
        printf("Error: %s", strerror_s(errmsg, 500, errno));
#endif
        free(img);
        free(tmp);
        return NULL;
    }

    for (i = head_size, j = 0; i < head_size + width * height; i++, j++) {
        img[j] = tmp[i];
    }

    free(tmp);
    return img;
}

void write_bin_file(const char* fn, int* img, int width, int height, int endian) {
    FILE* f;
    unsigned char SWAP[2];

    unsigned short* tmp = (unsigned short*)malloc(width * height * sizeof(unsigned short));
    if (tmp == NULL) {
        return;
    }

    for (int k = 0; k < width * height; k++) {
        if (endian == BIG_ENDIAN) {
            SWAP[0] = (img[k] & 0xFF00) >> 8;
            SWAP[1] = img[k] & 0x00FF;
            tmp[k] = ((SWAP[1] << 8) + SWAP[0]);
        } else
            tmp[k] = img[k];
    }

    // for long file name
    char fn_long[1024];
    sprintf_s(fn_long, 1024, "\\\\?\\%s", fn);

    if (fopen_s(&f, fn, "wb") == 0) {
        fwrite(tmp, sizeof(unsigned short), width * height, f);
    } else if (fopen_s(&f, fn_long, "wb") == 0) {
        fwrite(tmp, sizeof(unsigned short), width * height, f);
    } else {
        char errmsg[500];
#ifdef BILL_DEBUG
        printf("Error: %s", strerror_s(errmsg, 500, errno));
#endif

        free(tmp);
        return;
    }

    fclose(f);
    free(tmp);
    return;
}

void write_USbin_file(const char* fn, unsigned short* img, int width, int height, int endian) {
    FILE* f;
    unsigned char SWAP[2];

    unsigned short* tmp = (unsigned short*)malloc(width * height * sizeof(unsigned short));
    if (tmp == NULL) {
        return;
    }

    for (int k = 0; k < width * height; k++) {
        if (endian == BIG_ENDIAN) {
            SWAP[0] = (img[k] & 0xFF00) >> 8;
            SWAP[1] = img[k] & 0x00FF;
            tmp[k] = ((SWAP[1] << 8) + SWAP[0]);
        } else
            tmp[k] = img[k];
    }

    // for long file name
    char fn_long[1024];
    sprintf_s(fn_long, 1024, "\\\\?\\%s", fn);

    if (fopen_s(&f, fn, "wb") == 0) {
        fwrite(tmp, sizeof(unsigned short), width * height, f);
    } else if (fopen_s(&f, fn_long, "wb") == 0) {
        fwrite(tmp, sizeof(unsigned short), width * height, f);
    } else {
        char errmsg[500];
#ifdef BILL_DEBUG
        printf("Error: %s", strerror_s(errmsg, 500, errno));
#endif
        free(tmp);
        return;
    }

    fclose(f);
    free(tmp);
    return;
}

unsigned char* read_U8bin_file(const char* fn, int width, int height) {
    FILE* f;
    unsigned char* img = (unsigned char*)malloc(width * height * sizeof(unsigned char));
    if (img == NULL) {
        return NULL;
    }

    // for long file name
    char fn_long[1024];
    sprintf_s(fn_long, 1024, "\\\\?\\%s", fn);

    if (fopen_s(&f, fn, "rb") == 0) {
        fread(img, sizeof(unsigned char), width * height, f);
        fclose(f);
    } else if (fopen_s(&f, fn_long, "rb") == 0) {
        fread(img, sizeof(unsigned char), width * height, f);
        fclose(f);
    } else {
        char errmsg[500];
#ifdef BILL_DEBUG
        printf("Error: %s", strerror_s(errmsg, 500, errno));
#endif
        free(img);
        return NULL;
    }
    return img;
}

void write_U8bin_file(const char* fn, unsigned char* img, int width, int height) {
    FILE* f = NULL;

    // for long file name
    char fn_long[1024];
    sprintf_s(fn_long, 1024, "\\\\?\\%s", fn);

    if (fopen_s(&f, fn, "wb") == 0) {
        fwrite(img, sizeof(unsigned char), width * height, f);
    } else if (fopen_s(&f, fn_long, "wb") == 0) {
        fwrite(img, sizeof(unsigned char), width * height, f);
    } else {
        char errmsg[500];
#ifdef BILL_DEBUG
        printf("Error: %s", strerror_s(errmsg, 500, errno));
#endif
        return;
    }
    fclose(f);
    return;
}

void ppmWrite(const char* filename, int rows, int cols, unsigned char* image, char* comment) {
    FILE* file;

    // for long file name
    char fn_long[1024];
    sprintf_s(fn_long, 1024, "\\\\?\\%s", filename);

    if (fopen_s(&file, filename, "wb") == 0) {
        fprintf(file, "P6\r\n");

        if (comment != NULL) fprintf(file, "# %s \r\n", comment);

        fprintf(file, "%d %d\n", cols, rows);
        fprintf(file, "%d\n", (int)255);
        fwrite((unsigned char*)image, sizeof(unsigned char), cols * rows * 3, file);

        fclose(file);
    } else if (fopen_s(&file, fn_long, "wb") == 0) {
        fprintf(file, "P6\r\n");

        if (comment != NULL) fprintf(file, "# %s \r\n", comment);

        fprintf(file, "%d %d\n", cols, rows);
        fprintf(file, "%d\n", (int)255);
        fwrite((unsigned char*)image, sizeof(unsigned char), cols * rows * 3, file);

        fclose(file);
    } else
        printf("ppmWrite fail!\n");

    return;
}

void pgmWrite(const char* filename, int rows, int cols, unsigned char* image, char* comment) {
    FILE* file;

    // for long file name
    char fn_long[1024];
    sprintf_s(fn_long, 1024, "\\\\?\\%s", filename);

    if (fopen_s(&file, fn_long, "wb") == 0) {
        fprintf(file, "P5\r\n");

        if (comment != NULL) fprintf(file, "# %s \r\n", comment);

        fprintf(file, "%d %d\n", cols, rows);
        fprintf(file, "%d\n", (int)255);
        fwrite((unsigned char*)image, sizeof(unsigned char), cols * rows, file);

        fclose(file);
    } else
        printf("pgmWrite fail!\n");

    return;
}

void float2BMP_color(const char* filename, float* RG_map, float* BG_map, int rows, int cols) {
    FILE* file;

    unsigned char* img = NULL;
    int filesize = 54 + 3 * cols * rows;
    if (img) free(img);
    img = (unsigned char*)malloc(3 * cols * rows);
    if (img == NULL) {
        return;
    }

    memset(img, 0, sizeof(img));
    int x, y, r, g, b;

    for (int i = 0; i < cols; i++) {
        for (int j = 0; j < rows; j++) {
            x = i;
            y = (rows - 1) - j;
            r = RG_map[i + j * cols] * 255;
            g = 255;
            b = BG_map[i + j * cols] * 255;
            if (r > 255) r = 255;
            if (g > 255) g = 255;
            if (b > 255) b = 255;
            img[(x + y * cols) * 3 + 2] = (unsigned char)(r);
            img[(x + y * cols) * 3 + 1] = (unsigned char)(g);
            img[(x + y * cols) * 3 + 0] = (unsigned char)(b);
        }
    }

    unsigned char bmpfileheader[14] = {'B', 'M', 0, 0, 0, 0, 0, 0, 0, 0, 54, 0, 0, 0};
    unsigned char bmpinfoheader[40] = {40, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 24, 0};
    unsigned char bmppad[3] = {0, 0, 0};

    bmpfileheader[2] = (unsigned char)(filesize);
    bmpfileheader[3] = (unsigned char)(filesize >> 8);
    bmpfileheader[4] = (unsigned char)(filesize >> 16);
    bmpfileheader[5] = (unsigned char)(filesize >> 24);

    bmpinfoheader[4] = (unsigned char)(cols);
    bmpinfoheader[5] = (unsigned char)(cols >> 8);
    bmpinfoheader[6] = (unsigned char)(cols >> 16);
    bmpinfoheader[7] = (unsigned char)(cols >> 24);
    bmpinfoheader[8] = (unsigned char)(rows);
    bmpinfoheader[9] = (unsigned char)(rows >> 8);
    bmpinfoheader[10] = (unsigned char)(rows >> 16);
    bmpinfoheader[11] = (unsigned char)(rows >> 24);

    // for long file name
    char fn_long[1024];
    sprintf_s(fn_long, 1024, "\\\\?\\%s", filename);

    if (fopen_s(&file, filename, "wb") == 0) {
        fwrite(bmpfileheader, 1, 14, file);
        fwrite(bmpinfoheader, 1, 40, file);
        for (int i = 0; i < rows; i++) {
            fwrite(img + (cols * (rows - i - 1) * 3), 3, cols, file);
            fwrite(bmppad, 1, (4 - (cols * 3) % 4) % 4, file);
        }
        fclose(file);
    } else if (fopen_s(&file, fn_long, "wb") == 0) {
        fwrite(bmpfileheader, 1, 14, file);
        fwrite(bmpinfoheader, 1, 40, file);
        for (int i = 0; i < rows; i++) {
            fwrite(img + (cols * (rows - i - 1) * 3), 3, cols, file);
            fwrite(bmppad, 1, (4 - (cols * 3) % 4) % 4, file);
        }
        fclose(file);
    } else
        printf("bmpWrite fail!\n");

    free(img);
    return;
}

void int2BMP_color(const char* filename, int* R_map, int* G_map, int* B_map, int rows, int cols) {
    int i, j, x, y, r, g, b;
    FILE* file;

    // normalize
    {
        int max = -0xFFFF;
        int min = 0xFFFF;
        for (i = 0; i < rows * cols; i++) {
            max = max > R_map[i] ? max : R_map[i];
            max = max > G_map[i] ? max : G_map[i];
            max = max > B_map[i] ? max : B_map[i];
            min = min < R_map[i] ? min : R_map[i];
            min = min < G_map[i] ? min : G_map[i];
            min = min < B_map[i] ? min : B_map[i];
        }
        for (i = 0; i < rows * cols; i++) {
            R_map[i] = (int)((float)(R_map[i] - min) / (max - min) * 255);
            G_map[i] = (int)((float)(G_map[i] - min) / (max - min) * 255);
            B_map[i] = (int)((float)(B_map[i] - min) / (max - min) * 255);
        }
    }

    unsigned char* img = NULL;
    int filesize = 54 + 3 * cols * rows;
    if (img) free(img);
    img = (unsigned char*)malloc(3 * cols * rows);
    if (img == NULL) {
        return;
    }
    memset(img, 0, sizeof(img));

    for (i = 0; i < cols; i++) {
        for (j = 0; j < rows; j++) {
            x = i;
            y = (rows - 1) - j;
            r = R_map[i + j * cols];
            g = G_map[i + j * cols];
            b = B_map[i + j * cols];
            if (r > 255) r = 255;
            if (g > 255) g = 255;
            if (b > 255) b = 255;
            img[(x + y * cols) * 3 + 2] = (unsigned char)(r);
            img[(x + y * cols) * 3 + 1] = (unsigned char)(g);
            img[(x + y * cols) * 3 + 0] = (unsigned char)(b);
        }
    }

    unsigned char bmpfileheader[14] = {'B', 'M', 0, 0, 0, 0, 0, 0, 0, 0, 54, 0, 0, 0};
    unsigned char bmpinfoheader[40] = {40, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 24, 0};
    unsigned char bmppad[3] = {0, 0, 0};

    bmpfileheader[2] = (unsigned char)(filesize);
    bmpfileheader[3] = (unsigned char)(filesize >> 8);
    bmpfileheader[4] = (unsigned char)(filesize >> 16);
    bmpfileheader[5] = (unsigned char)(filesize >> 24);

    bmpinfoheader[4] = (unsigned char)(cols);
    bmpinfoheader[5] = (unsigned char)(cols >> 8);
    bmpinfoheader[6] = (unsigned char)(cols >> 16);
    bmpinfoheader[7] = (unsigned char)(cols >> 24);
    bmpinfoheader[8] = (unsigned char)(rows);
    bmpinfoheader[9] = (unsigned char)(rows >> 8);
    bmpinfoheader[10] = (unsigned char)(rows >> 16);
    bmpinfoheader[11] = (unsigned char)(rows >> 24);

    // for long file name
    char fn_long[1024];
    sprintf_s(fn_long, 1024, "\\\\?\\%s", filename);

    if (fopen_s(&file, filename, "wb") == 0) {
        fwrite(bmpfileheader, 1, 14, file);
        fwrite(bmpinfoheader, 1, 40, file);
        for (int i = 0; i < rows; i++) {
            fwrite(img + (cols * (rows - i - 1) * 3), 3, cols, file);
            fwrite(bmppad, 1, (4 - (cols * 3) % 4) % 4, file);
        }
        fclose(file);
    } else if (fopen_s(&file, fn_long, "wb") == 0) {
        fwrite(bmpfileheader, 1, 14, file);
        fwrite(bmpinfoheader, 1, 40, file);
        for (int i = 0; i < rows; i++) {
            fwrite(img + (cols * (rows - i - 1) * 3), 3, cols, file);
            fwrite(bmppad, 1, (4 - (cols * 3) % 4) % 4, file);
        }
        fclose(file);
    } else
        printf("bmpWrite fail!\n");

    free(img);
    return;
}
// void encodeOneStep(const char *filename, const unsigned char *image,
//                   unsigned width, unsigned height) {
//  /*Encode the image*/
//  unsigned error = lodepng_encode32_file(filename, image, width, height);
//
//  /*if there's an error, display it*/
//  if (error) {
//    printf("error %u: %s\n", error, lodepng_error_text(error));
//  }
//}
//
// void pngWrite(const char *filename, int rows, int cols, unsigned char *image)
// {
//  unsigned char *out = (unsigned char *)malloc(cols * rows * 4);
//  unsigned x, y;
//  for (y = 0; y < rows; y++)
//    for (x = 0; x < cols; x++) {
//      out[4 * cols * y + 4 * x + 0] = image[cols * y + x];
//      out[4 * cols * y + 4 * x + 1] = image[cols * y + x];
//      out[4 * cols * y + 4 * x + 2] = image[cols * y + x];
//      out[4 * cols * y + 4 * x + 3] = 255;
//    }
//  encodeOneStep(filename, out, cols, rows);
//  free(out);
//}

void int2CSV(const char* filename, int* img, int width, int height) {
    FILE* file;

    // for long file name
    char fn_long[1024];
    sprintf_s(fn_long, 1024, "\\\\?\\%s", filename);

    if (fopen_s(&file, filename, "w") == 0) {
        int need_comma = 0;
        for (int i = 0; i < width * height; ++i) {
            if (need_comma)
                fprintf(file, ", ");
            else
                need_comma = 1;
            if ((i % width) == 0 && i != 0) fprintf(file, "\n");
            if (img[i] == -9999) {
                fprintf(file, "NAN");
            } else {
                fprintf(file, "%i", img[i]);
            }
        }
        fprintf(file, "\n");

        fclose(file);
    } else if (fopen_s(&file, fn_long, "w") == 0) {
        int need_comma = 0;
        for (int i = 0; i < width * height; ++i) {
            if (need_comma)
                fprintf(file, ", ");
            else
                need_comma = 1;
            if ((i % width) == 0 && i != 0) fprintf(file, "\n");
            if (img[i] == -9999) {
                fprintf(file, "NAN");
            } else {
                fprintf(file, "%i", img[i]);
            }
        }
        fprintf(file, "\n");

        fclose(file);
    } else
        printf("Write fail!\n");
}

void US2CSV(const char* filename, unsigned short* img, int width, int height) {
    FILE* file;

    // for long file name
    char fn_long[1024];
    sprintf_s(fn_long, 1024, "\\\\?\\%s", filename);

    if (fopen_s(&file, filename, "w") == 0) {
        int need_comma = 0;
        for (int i = 0; i < width * height; ++i) {
            if (need_comma)
                fprintf(file, ", ");
            else
                need_comma = 1;
            if ((i % width) == 0 && i != 0) fprintf(file, "\n");
            if (img[i] == -9999) {
                fprintf(file, "NAN");
            } else {
                fprintf(file, "%i", img[i]);
            }
        }
        fprintf(file, "\n");

        fclose(file);
    } else if (fopen_s(&file, fn_long, "w") == 0) {
        int need_comma = 0;
        for (int i = 0; i < width * height; ++i) {
            if (need_comma)
                fprintf(file, ", ");
            else
                need_comma = 1;
            if ((i % width) == 0 && i != 0) fprintf(file, "\n");
            if (img[i] == -9999) {
                fprintf(file, "NAN");
            } else {
                fprintf(file, "%i", img[i]);
            }
        }
        fprintf(file, "\n");

        fclose(file);
    } else
        printf("Write fail!\n");
}

void U82CSV(const char* filename, unsigned char* _pimg, int width, int height) {
    FILE* file;

    // for long file name
    char fn_long[1024];
    sprintf_s(fn_long, 1024, "\\\\?\\%s", filename);

    if (fopen_s(&file, filename, "w") == 0) {
        int need_comma = 0;
        for (int i = 0; i < width * height; ++i) {
            if (need_comma)
                fprintf(file, ", ");
            else
                need_comma = 1;
            if ((i % width) == 0 && i != 0) fprintf(file, "\n");
            fprintf(file, "%i", _pimg[i]);
        }
        fprintf(file, "\n");

        fclose(file);
    } else if (fopen_s(&file, fn_long, "w") == 0) {
        int need_comma = 0;
        for (int i = 0; i < width * height; ++i) {
            if (need_comma)
                fprintf(file, ", ");
            else
                need_comma = 1;
            if ((i % width) == 0 && i != 0) fprintf(file, "\n");
            fprintf(file, "%i", _pimg[i]);
        }
        fprintf(file, "\n");

        fclose(file);
    } else
        printf("Write fail!\n");
}

void normalize_int2UINT8(int* input, unsigned char* output, int SZ) {
    int max = -0xFFFF;
    int min = 0xFFFF;

    for (int k = 0; k < SZ; k++) {
        min = min < input[k] ? min : input[k];
        max = max > input[k] ? max : input[k];
    }

    if (output) {
        if (min == 0 && max == 255) {
            for (int k = 0; k < SZ; k++) output[k] = input[k];
        } else if (min == max) {
            for (int k = 0; k < SZ; k++) output[k] = input[k];
        } else {
            for (int k = 0; k < SZ; k++) output[k] = (255 * (input[k] - min)) / (max - min);
        }
    }

    return;
}

void normalize_float2UINT8(float* input, unsigned char* output, int SZ) {
    float max = -0xFFFF;
    float min = 0xFFFF;

    for (int k = 0; k < SZ; k++) {
        min = min < input[k] ? min : input[k];
        max = max > input[k] ? max : input[k];
    }

    if (output) {
        if (min == 0 && max == 255) {
            for (int k = 0; k < SZ; k++) output[k] = input[k];
        } else if (min == max) {
            for (int k = 0; k < SZ; k++) output[k] = input[k];
        } else {
            for (int k = 0; k < SZ; k++) output[k] = (255 * (input[k] - min)) / (max - min);
        }
    }

    return;
}

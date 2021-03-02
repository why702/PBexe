#include <Windows.h>
#include <direct.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <time.h>

#include "plat_file.h"
#include "plat_log.h"

#define RESULT_FILE_SUCCESS 0
#define RESULT_FILE_INVALID_PARAM -1
#define RESULT_FILE_ALLOCATE_FAILED -2
#define RESULT_FILE_FAILED -3
#define RESULT_FILE_OPEN_FAILED -201
#define RESULT_FILE_WRITE_FAILED -202
#define RESULT_FILE_READ_FAILED -203
#define RESULT_FILE_BLURRY_DATA -204
#define RESULT_FILE_NOT_EXIST PLAT_FILE_NOT_EXIST
#define RESULT_FILE_OUT_OF_MEMORY PLAT_FILE_OUT_OF_MEMORY

#define MAX_READ_LEN 1024

int save_secure_file(const char* path_name, unsigned char* data, int data_len);
int get_secure_file(const char* path_name, unsigned char* data, int* data_len);
int remove_file(const char* path_name);

int plat_save_file(char* path, unsigned char* buf, unsigned int len) {
    return save_secure_file(path, buf, len);
}
int plat_load_file(char* path, unsigned char* buf, unsigned int len, unsigned int* real_size) {
    int ret = get_secure_file(path, buf, &len);
    if (ret == len) *real_size = len;
    return ret;
}
int plat_load_raw_image(char* path, unsigned char* pImage, unsigned int width,
                        unsigned int height) {
    int image_len = width * height;
    return get_secure_file(path, pImage, &image_len);
}
int plat_save_raw_image(char* path, unsigned char* pImage, unsigned int width,
                        unsigned int height) {
    return save_secure_file(path, pImage, width * height);
}
int plat_remove_file(char* path) {
    return remove_file(path);
}
int save_secure_file(const char* path_name, unsigned char* data, int data_len) {
    int ret = RESULT_FILE_SUCCESS;
    unsigned char* buffer = NULL;
    int write_len;
    FILE* file;
    if (data == NULL || data_len <= 0) return RESULT_FILE_INVALID_PARAM;

    buffer = (unsigned char*)malloc(data_len);

    if (buffer == NULL) return RESULT_FILE_ALLOCATE_FAILED;

    memcpy(buffer, data, data_len);

    fopen_s(&file,path_name, "wb");
    if (file == NULL) {
        ret = RESULT_FILE_OPEN_FAILED;
        goto exit;
    }

    write_len = fwrite(buffer, 1, data_len, file);
    if (write_len < data_len)
        ret = RESULT_FILE_WRITE_FAILED;
    else
        ret = write_len;

    fclose(file);

exit:
    if (buffer) free(buffer);

    return ret;
}

int get_secure_file(const char* path_name, unsigned char* buffer, int* buffer_len) {
    int ret = RESULT_FILE_SUCCESS;
    FILE* file;
    int read_len;
    int read_total_len = 0;
    unsigned char* pbuf;
    int in_bufer_len;
    unsigned char* read_buffer;

    if (buffer == NULL || buffer_len == NULL) return RESULT_FILE_INVALID_PARAM;

    if (buffer_len <= 0) return RESULT_FILE_INVALID_PARAM;

    in_bufer_len = *buffer_len;
    read_buffer = (unsigned char*)malloc(in_bufer_len);

    if (read_buffer == NULL) return RESULT_FILE_ALLOCATE_FAILED;

    fopen_s(&file,path_name, "rb+");
    if (file == NULL) {
        ret = RESULT_FILE_NOT_EXIST;
        goto exit;
    }

    while (1) {
        pbuf = buffer + read_total_len;
        read_len = fread(read_buffer, 1, MAX_READ_LEN, file);
        if (read_len > 0) {
            read_total_len += read_len;
            if (*buffer_len < read_total_len) {
                ret = RESULT_FILE_READ_FAILED;
                goto exit;
            }
            memcpy(pbuf, read_buffer, read_len);
        }
        if (read_len < MAX_READ_LEN) {
            ret = read_total_len;
            break;
        }
    }

    *buffer_len = read_total_len;
exit:
    if (file != NULL) {
        fclose(file);
    }
    if (read_buffer != NULL) {
        free(read_buffer);
        read_buffer = NULL;
    }

    return ret;
}

int remove_file(const char* path_name) {
    return remove(path_name);
}

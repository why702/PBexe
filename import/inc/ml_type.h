#ifndef ml_type_h
#define ml_type_h

#define VGG_BLOCK_SZ 32
#define PADDING_SZ 34
#define VGG_BLOCK_SZ_C 16
#define VGG_MAX_BLOCK 256
typedef struct _VggResult{
	unsigned char percentage;
	unsigned char block_count;
	unsigned char block_label[VGG_MAX_BLOCK];
	unsigned short block_address[VGG_MAX_BLOCK];
	int x_offset, y_offset;
} VggResult;


#endif

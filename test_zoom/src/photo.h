/*
 * xStudio/photo.h
 */

#ifndef XSTUDIO_PHOTO_H
#define XSTUDIO_PHOTO_H

#include <stdio.h>	/* should be included before jpeglib.h */
#include <jpeglib.h>
#include <setjmp.h>	/* jmp_buf */

#include "../xStudio/display.h"

/************************************************************************/
/*      bmp bitmap info                                                 */
/************************************************************************/
typedef struct tagRGBTRIPLE {
	uint8_t rgbtBlue;
	uint8_t rgbtGreen;
	uint8_t rgbtRed;
} RGBTRIPLE;

typedef struct tagRGBQUAD {
	uint8_t rgbBlue;
	uint8_t rgbGreen;
	uint8_t rgbRed;
	uint8_t rgbReserved;
} RGBQUAD;

#define MAX_COLORS		256

typedef struct tagBITMAPCOREHEADER {
	uint32_t bcSize;
	uint16_t bcWidth;
	uint16_t bcHeight;
	uint16_t bcPlanes;
	uint16_t bcBitCount;
} BITMAPCOREHEADER;

typedef struct tagBITMAPINFOHEADER {
	uint32_t biSize;
	int32_t  biWidth;
	int32_t  biHeight;
	uint16_t biPlanes;
	uint16_t biBitCount;
	uint32_t biCompression;
	uint32_t biSizeImage;
	int32_t  biXPelsPerMeter;
	int32_t  biYPelsPerMeter;
	uint32_t biClrUsed;
	uint32_t biClrImportant;
} BITMAPINFOHEADER;

/* constants for the biCompression field */
#define BI_RGB			0
#define BI_RLE8			1
#define BI_RLE4			2
#define BI_BITFIELDS		3

typedef struct tagBITMAPFILEHEADER {
	uint16_t bfType;
	uint32_t bfSize;
	uint16_t bfReserved1;
	uint16_t bfReserved2;
	uint32_t bfOffBits;
} BITMAPFILEHEADER;

#define CoreHeaderSize		12
#define InfoHeaderSize		40
#define FileHeaderSize		14

#define END_OF_LINE		0
#define END_OF_BITMAP	1
#define DELTA			2


/************************************************************************/
/*  gif bitmap info                                                     */
/************************************************************************/
#define ANIMATION_FLAG 0x80

#define LAST_FRAME_FLAG		0x40
/************************************************************************/
/* jpg bitmap info                                                      */
/************************************************************************/
struct jpg_error_mgr {
	struct jpeg_error_mgr pub;	/* "public" fields */
	jmp_buf setjmp_buffer;		/* for return to caller */
};

typedef struct jpg_error_mgr *jpg_error_ptr;
void close_gif_file(struct REGION *region);
void xStudio_draw_bmp_R8G8B8(const char *filename, const struct REGION *region, struct region_buffer *buf);

void xStudio_draw_gif_R8G8B8(const char *filename, struct REGION *region,struct region_buffer *buf);
	 
void xStudio_draw_jpg_R8G8B8(const char *filename, const struct REGION *region,struct region_buffer *buf);
	 

#endif	/* XSTUDIO_PHOTO_H */

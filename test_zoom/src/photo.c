/*
 *  xStudio/photo.c
 */

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "photo.h"



/* ../display.c */
extern size_t pixel_size;

/************************************************************************/
/*  the function of getting bmp photo pixel info                        */
/************************************************************************/
static inline int read_file_header(int fd, BITMAPFILEHEADER *bfh)
{
	uint8_t buf[FileHeaderSize];

	if (full_read(fd, buf, FileHeaderSize) != FileHeaderSize)
		return -1;
	bfh->bfType = GET_LE16(&buf[0]);
	bfh->bfSize = GET_LE32(&buf[2]);
	bfh->bfReserved1 = GET_LE16(&buf[6]);
	bfh->bfReserved2 = GET_LE16(&buf[8]);
	bfh->bfOffBits = GET_LE32(&buf[10]);
	if (bfh->bfType != 0x4d42)	/* 'BM' */
		return -1;
	return 0;
}

static inline int read_core_header(int fd, BITMAPINFOHEADER *bih)
{
	uint8_t buf[CoreHeaderSize - 4];

	if (full_read(fd, buf, CoreHeaderSize - 4) != CoreHeaderSize - 4)
		return -1;
	bih->biWidth = GET_LE16(&buf[0]);
	bih->biHeight = GET_LE16(&buf[2]);
	bih->biPlanes = GET_LE16(&buf[4]);
	bih->biBitCount = GET_LE16(&buf[6]);
	bih->biCompression = BI_RGB;
	return 0;
}

static inline int read_info_header(int fd, BITMAPINFOHEADER *bih)
{
	uint8_t buf[InfoHeaderSize - 4];

	if (full_read(fd, buf, InfoHeaderSize - 4) != InfoHeaderSize - 4)
		return -1;
	bih->biWidth = GET_LE32(&buf[0]);
	bih->biHeight = GET_LE32(&buf[4]);
	bih->biPlanes = GET_LE16(&buf[8]);
	bih->biBitCount = GET_LE16(&buf[10]);
	bih->biCompression = GET_LE32(&buf[12]);
	bih->biSizeImage = GET_LE32(&buf[16]);
	bih->biXPelsPerMeter = GET_LE32(&buf[20]);
	bih->biYPelsPerMeter = GET_LE32(&buf[24]);
	bih->biClrUsed = GET_LE32(&buf[28]);
	bih->biClrImportant = GET_LE32(&buf[32]);
	return 0;
}

static inline int read_palette(int fd, RGBTRIPLE *palette, size_t size,int ncolors)
{
	uint8_t buf[4 * MAX_COLORS];
	int i;

	if (full_read(fd, buf, size * ncolors) != size * ncolors)
		return -1;
	for (i = 0; i < ncolors; i++) {
		palette[i].rgbtBlue = buf[size * i + 0];
		palette[i].rgbtGreen = buf[size * i + 1];
		palette[i].rgbtRed = buf[size * i + 2];
	}
	return 0;
}

/* Count the number of set bits */
static inline int count_bits(uint32_t v)
{
	int i, n = 0;

	for (i = 0; i < 32; i++)
		if (v & (1 << i))
			n++;
	return n;
}

/* Find the rightmost set bit */
static inline int rightmost_bit(uint32_t v)
{
	int i;

	for (i = 0; i < 32; i++)
		if (v & (1 << i))
			return i;
	return i;
}

/* Find the leftmost set bit */
static inline int leftmost_bit(uint32_t v)
{
	int i;

	for (i = 31; i >= 0; i--)
		if (v & (1 << i))
			return i;
	return i;
}

/* Check the mask (contiguous bits) */
static inline int IsValidMask(uint32_t *mask, int *rshift, int *lshift)
{
	uint32_t v = *mask;
	int leftmost, i;

	if (count_bits(v) == 0) {
		*rshift = 0;
		*lshift = 0;
		return 1;
	}

	leftmost = leftmost_bit(v);
	for (i = rightmost_bit(v); i <= leftmost; i++)
		if (!(v & (1 << i)))
			return 0;

	/* While more than 8 bits set, turn off the rightmost bit */
	while (count_bits(v) > 8)
		v -= (1 << rightmost_bit(v));

	*rshift = rightmost_bit(v);
	*lshift = 8 - count_bits(v);
	*mask = v;
	return 1;
}


#define SET_PIXEL_R8G8B8(x, y, color) \
	if ((x) < w && (y) < region->height) { \
		*b++ = (color)->rgbtRed; \
		*b++ = (color)->rgbtGreen; \
		*b++ = (color)->rgbtBlue; \
	}

void xStudio_draw_bmp_R8G8B8(const char *filename, unsigned char *buf)
{
	int fd;
	BITMAPFILEHEADER bfh;
	BITMAPINFOHEADER bih;
	RGBTRIPLE palette[MAX_COLORS];
	unsigned char s[256];
	unsigned long header_size;
	int ncolors;
	unsigned long r_mask = 0, g_mask = 0, b_mask = 0;
	int r_rshift = 0, g_rshift = 0, b_rshift = 0;
	int r_lshift = 0, g_lshift = 0, b_lshift = 0;
	int bottom_up = 1;
	size_t line_bytes;
	unsigned char *linebuf = NULL;
	unsigned char *line_buf;
	int w, h, x, y, y1, i, m, n;
	size_t line_size;
	size_t offset;
	unsigned char *b;

	unsigned char *src_buf=NULL;
	size_t src_size;

	RGBTRIPLE *color;
	unsigned char red, green, blue;
	unsigned short v16;
	unsigned long v32;

	

	if ((fd = open(filename, O_RDONLY)) == -1)

	{
		fprintf(stderr, "%s(): open fialed \n", __FUNCTION__);
		return;
	}

	/* Read file header and verify the signature */
	if (read_file_header(fd, &bfh) < 0)
		goto out;

	memset(palette, 0, sizeof(RGBTRIPLE) * MAX_COLORS);

	if (full_read(fd, s, 4) != 4)
		goto out;
	header_size = GET_LE32(s);

	if (header_size == CoreHeaderSize) {	/* OS/2 */
		if (read_core_header(fd, &bih) < 0)
			goto out;
		/* Read color table */
		if (bih.biBitCount <= 8) {
			ncolors = (1 << bih.biBitCount);
			if (read_palette(fd, palette, 3, ncolors) < 0)
				goto out;
		}
	} else if (header_size == InfoHeaderSize) {
		if (read_info_header(fd, &bih) < 0)
			goto out;
		if (bih.biCompression != BI_RGB
		    && bih.biCompression != BI_RLE8
		    && bih.biCompression != BI_RLE4
		    && bih.biCompression != BI_BITFIELDS)
			goto out;
		if ((bih.biCompression == BI_RLE8 && bih.biBitCount != 8)
		    || (bih.biCompression == BI_RLE4 && bih.biBitCount != 4)
		    || (bih.biCompression == BI_BITFIELDS
			&& bih.biBitCount != 16 && bih.biBitCount != 32))
			goto out;
		/* Read color table */
		if (bih.biBitCount <= 8) {
			ncolors = (1 << bih.biBitCount);
			if (bih.biClrUsed > 0 && bih.biClrUsed < ncolors)
				ncolors = bih.biClrUsed;
			if (read_palette(fd, palette, 4, ncolors) < 0)
				goto out;
		} else if (bih.biCompression == BI_BITFIELDS) {
			if (full_read(fd, s, 12) != 12)
				goto out;
			r_mask = GET_LE32(&s[0]);
			g_mask = GET_LE32(&s[4]);
			b_mask = GET_LE32(&s[8]);
			if (!IsValidMask(&r_mask, &r_rshift, &r_lshift)
			    || !IsValidMask(&g_mask, &g_rshift, &g_lshift)
			    || !IsValidMask(&b_mask, &b_rshift, &b_lshift))
				goto out;
		}
	} else
		goto out;

	if (bih.biBitCount == 16 && bih.biCompression != BI_BITFIELDS) {
		/* Use default 5-5-5 format */
		r_mask = 0x7c00;
		g_mask = 0x03e0;
		b_mask = 0x001f;
		r_rshift = 10;
		g_rshift = 5;
		b_rshift = 0;
		r_lshift = 3;
		g_lshift = 3;
		b_lshift = 3;
	}

	if (bih.biHeight < 0) {
		bih.biHeight = -bih.biHeight;
		bottom_up = 0;
	}

	if (bih.biWidth < 1 || bih.biWidth > 8192)
		goto out;
	if (bih.biHeight < 1 || bih.biHeight > 8192)
		goto out;

	if (bfh.bfOffBits)
		if (lseek(fd, bfh.bfOffBits, SEEK_SET) != bfh.bfOffBits)
			goto out;

	/* Pad to a 32 bit boundary */
	line_bytes = (bih.biWidth * bih.biBitCount + 31) / 32 * 4;
	if ((linebuf = (unsigned char *)malloc(line_bytes)) == NULL)
		goto out;

	
	
	//w = MIN(bih.biWidth, region->width);
	w = bih.biWidth;
	h = bih.biHeight;

	src_size = line_bytes*h;
	if ((src_buf = (unsigned char*)malloc(src_size)) == NULL)
		goto out;

	fprintf(stderr, "%s():w=%d h=%d bih.biBitCount=%d  line_bytes=%d\n", __FUNCTION__, w, h, bih.biBitCount, line_bytes);
	switch (bih.biBitCount) {
	case 1:
		for (y = 0; y < h; y++) {
			if (full_read(fd, linebuf, line_bytes) != line_bytes)
				goto out;
			if (bottom_up) {
				y1 = h - y - 1;
				//if (y1 >= region->height)
				//	continue;
			} else {
				y1 = y;
				//if (y1 >= region->height)
				//	break;
			}
			offset = line_size * y1;
			//b = REGION_BUF_DATA(buf) + offset;
			b = src_buf+offset;
			for (x = 0; x < w; x++) {
				i = ((linebuf[x / 8] >> (7 - x % 8)) & 0x01);
				color = &palette[i];
				*b++ = color->rgbtRed;
				*b++ = color->rgbtGreen;
				*b++ = color->rgbtBlue;
			}
		}
		break;

	case 4:
		if (bih.biCompression == BI_RLE4) {
			color = &palette[0];
			x = 0;
			y = 0;
			while (y < h) {
				y1 = bottom_up ? (h - y - 1) : y;
				if (full_read(fd, s, 2) != 2)
					goto out;
				if (s[0]) {	/* encoded mode */
					n = s[0];	/* number of pixels */
					offset = line_size * y1
						 + pixel_size * x;
					//b = REGION_BUF_DATA(buf) + offset;
					b = src_buf + offset;
					for (m = 0; m < n; m++) {
						i = (m & 1)
						    ? (s[1] & 0x0f)
						    : ((s[1] >> 4) & 0x0f);
						color = &palette[i];
						SET_PIXEL_R8G8B8(x, y1, color);
						x++;
					}
				} else if (s[1] == END_OF_LINE) {
					x = 0;
					y++;
				} else if (s[1] == END_OF_BITMAP) {
					break;
				} else if (s[1] == DELTA) {
					if (full_read(fd, s, 2) != 2)
						goto out;
					x += s[0];
					y += s[1];
				} else {	/* absolute mode */
					n = s[1];	/* number of pixels */
					m = (n + 3) / 4 * 2;	/* padding */
					if (full_read(fd, s, m) != m)
						goto out;
					offset = line_size * y1
						 + pixel_size * x;
					//b = REGION_BUF_DATA(buf) + offset;
					b = src_buf + offset;
					for (m = 0; m < n; m++) {
						i = (m & 1)
						    ? (s[m / 2] & 0x0f)
						    : ((s[m / 2] >> 4) & 0x0f);
						color = &palette[i];
						SET_PIXEL_R8G8B8(x, y1, color);
						x++;
					}
				}
			}
			break;
		}
		for (y = 0; y < h; y++) {
			if (full_read(fd, linebuf, line_bytes) != line_bytes)
				goto out;
			if (bottom_up) {
				y1 = h - y - 1;
				//if (y1 >= region->height)
				//	continue;
			} else {
				y1 = y;
				//if (y1 >= region->height)
				//	break;
			}
			line_buf = linebuf;
			offset = line_size * y1;
			//b = REGION_BUF_DATA(buf) + offset;
			b = src_buf + offset;
			for (x = 0; x < w; x++) {
				i = ((*line_buf >> 4) & 0x0f);
				color = &palette[i];
				*b++ = color->rgbtRed;
				*b++ = color->rgbtGreen;
				*b++ = color->rgbtBlue;
				x++;
				if (x >= w)
					break;
				i = ((*line_buf++) & 0x0f);
				color = &palette[i];
				*b++ = color->rgbtRed;
				*b++ = color->rgbtGreen;
				*b++ = color->rgbtBlue;
			}
		}
		break;

	case 8:
		if (bih.biCompression == BI_RLE8) {
			color = &palette[0];
			
			x = 0;
			y = 0;
			while (y < h) {
				y1 = bottom_up ? (h - y - 1) : y;
				if (full_read(fd, s, 2) != 2)
					goto out;
				if (s[0]) {	/* encoded mode */
					n = s[0];	/* number of pixels */
					i = s[1];	/* color index */
					color = &palette[i];
					offset = line_size * y1
						 + pixel_size * x;
					//b = REGION_BUF_DATA(buf) + offset;
					b = src_buf + offset;
					for (m = 0; m < n; m++) {
						SET_PIXEL_R8G8B8(x, y1, color);
						x++;
					}
				} else if (s[1] == END_OF_LINE) {
					x = 0;
					y++;
				} else if (s[1] == END_OF_BITMAP) {
					break;
				} else if (s[1] == DELTA) {
					if (full_read(fd, s, 2) != 2)
						goto out;
					x += s[0];
					y += s[1];
				} else {	/* absolute mode */
					n = s[1];	/* number of pixels */
					m = (n + 1) / 2 * 2;	/* padding */
					if (full_read(fd, s, m) != m)
						goto out;
					offset = line_size * y1
						 + pixel_size * x;
					//b = REGION_BUF_DATA(buf) + offset;
					b = src_buf + offset;
					for (m = 0; m < n; m++) {
						i = s[m];
						color = &palette[i];
						SET_PIXEL_R8G8B8(x, y1, color);
						x++;
					}
				}
			}
			break;
		}
		for (y = 0; y < h; y++) {
			if (full_read(fd, linebuf, line_bytes) != line_bytes)
				goto out;
			if (bottom_up) {
				y1 = h - y - 1;
				//if (y1 >= region->height)
				//	continue;
			} else {
				y1 = y;
				//if (y1 >= region->height)
				//	break;
			}
			line_buf = linebuf;
			offset = line_size * y1;
			//b = REGION_BUF_DATA(buf) + offset;
			b = src_buf + offset;
			for (x = 0; x < w; x++) {
				i = *line_buf++;
				color = &palette[i];
				*b++ = color->rgbtRed;
				*b++ = color->rgbtGreen;
				*b++ = color->rgbtBlue;
			}
		}
		break;

	case 16:
		for (y = 0; y < h; y++) {
			if (full_read(fd, linebuf, line_bytes) != line_bytes)
				goto out;
			if (bottom_up) {
				y1 = h - y - 1;
				//if (y1 >= region->height)
				//	continue;
			} else {
				y1 = y;
				//if (y1 >= region->height)
				//	break;
			}
			line_buf = linebuf;
			offset = line_size * y1;
			//b = REGION_BUF_DATA(buf) + offset;
			b = src_buf + offset;
			for (x = 0; x < w; x++) {
				v16 = GET_LE16(line_buf);
				line_buf += 2;
				red   = ((v16 & r_mask) >> r_rshift);
				green = ((v16 & g_mask) >> g_rshift);
				blue  = ((v16 & b_mask) >> b_rshift);
				*b++ = (red   << r_lshift);
				*b++ = (green << g_lshift);
				*b++ = (blue  << b_lshift);
			}
		}
		break;

	case 24:
		for (y = 0; y < h; y++) {
			if (full_read(fd, linebuf, line_bytes) != line_bytes)
				goto out;
			if (bottom_up) {
				y1 = h - y - 1;
				//if (y1 >= region->height)
				//	continue;
			} else {
				y1 = y;
				//if (y1 >= region->height)
				//	break;
			}
			line_buf = linebuf;
			offset = line_size * y1;
			//b = REGION_BUF_DATA(buf) + offset;
			b = src_buf + offset;
			for (x = 0; x < w; x++) {
				blue  = *line_buf++;
				green = *line_buf++;
				*b++  = *line_buf++;
				*b++  = green;
				*b++  = blue;
			}
		}
		break;

	case 32:
		for (y = 0; y < h; y++) {
			if (full_read(fd, linebuf, line_bytes) != line_bytes)
				goto out;
			if (bottom_up) {
				y1 = h - y - 1;
				//if (y1 >= region->height)
				//	continue;
			} else {
				y1 = y;
				//if (y1 >= region->height)
				//	break;
			}
			line_buf = linebuf;
			offset = line_size * y1;
			//b = REGION_BUF_DATA(buf) + offset;
			b = src_buf + offset;
			for (x = 0; x < w; x++) {
				if (bih.biCompression == BI_BITFIELDS) {
					v32 = GET_LE32(line_buf);
					line_buf += 4;
					red   = ((v32 & r_mask) >> r_rshift);
					green = ((v32 & g_mask) >> g_rshift);
					blue  = ((v32 & b_mask) >> b_rshift);
					*b++ = (red   << r_lshift);
					*b++ = (green << g_lshift);
					*b++ = (blue  << b_lshift);
				} else {
					blue  = *line_buf++;
					green = *line_buf++;
					*b++  = *line_buf++;
					*b++  = green;
					*b++  = blue;
					line_buf++;
				}
			}
		}
		break;

	default:
		goto out;
	}
	//memcpy(REGION_BUF_DATA(buf), src_buf, region->width*region->height*pixel_size);
	
	fprintf(stderr, "%s():regio->w=%d ,region->h=%d\n", __FUNCTION__, region->width, region->height);
	if (bih.biWidth==region->width && bih.biHeight==region->height)
	{
		memcpy(REGION_BUF_DATA(buf), src_buf, src_size);
	}
	else if ((zoom_image(src_buf, line_bytes,bih.biWidth, bih.biHeight, 
		(unsigned char*)REGION_BUF_DATA(buf),(int32_t)region->width,(int32_t)region->height))!=0)
	{
		fprintf(stderr, "%s():zoom image failed \n", __FUNCTION__);
	}
	
	fprintf(stderr, "%s():regio->w=%d ,region->h=%d\n", __FUNCTION__, region->width, region->height);
out:
	if (linebuf) 
		free(linebuf);
	if (src_buf) 
		free(src_buf);
	close(fd);
}



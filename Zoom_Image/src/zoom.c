#pragma once
#pragma execution_character_set("utf-8")

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>


int pixel_size = 3;

static unsigned char* pixels_bound(unsigned char *src, const uint32_t src_line_bytes, int32_t x, int32_t y)
{
	return (src + src_line_bytes*y + pixel_size*x);
}

static  void bilinear_save_dest(unsigned char *dest_buf, unsigned char *src_buf1, unsigned char*src_buf2, uint32_t u, uint32_t v)
{
	uint32_t pw3 = u*v; uint32_t pw2 = (u << 8) - pw3;
	uint32_t pw1 = (v << 8) - pw3; uint32_t pw0 = (1 << 16) - pw1 - pw2 - pw3;
	dest_buf[0] = ((pw0*(src_buf1[0]) + pw1*src_buf2[0] + pw2*src_buf1[0] + pw3*src_buf2[0])>>16);
	dest_buf[1] = ((pw0*(src_buf1[1]) + pw1*src_buf2[1] + pw2*src_buf1[1] + pw3*src_buf2[1])>>16);
	dest_buf[2] = ((pw0*(src_buf1[2]) + pw1*src_buf2[2] + pw2*src_buf1[2] + pw3*src_buf2[2])>>16);
	//fprintf(stderr, "%s():r=%d g=%d b=%d sr=%d sg=%d sb=%d\n", __FUNCTION__, dest_buf[0], dest_buf[1], dest_buf[2], src_buf1[0], src_buf1[1], src_buf1[2]);
}

static  void bilinear_border(unsigned char* src_image, const uint32_t src_line_bytes, const int32_t x_16, const int32_t y_16,
	unsigned char*dest_buf, const uint32_t dest_line_bytes)
{
	long x = (x_16 >> 16); long y = (y_16 >> 16);
	uint32_t u_16 = ((unsigned short)(x_16));
	uint32_t v_16 = ((unsigned short)(y_16));
	unsigned char *buf1; unsigned char *buf2;
	unsigned char *buf3; unsigned char *buf4;

	//get the source image 4 pixels around the destination position
	buf1 = pixels_bound(src_image, src_line_bytes, x, y);
	buf2 = pixels_bound(src_image, src_line_bytes, x + 1, y);
	buf3 = pixels_bound(src_image, src_line_bytes, x, y + 1);
	buf4 = pixels_bound(src_image, src_line_bytes, x + 1, y + 1);

	//calculate the best pixel to the dest_buf 
	bilinear_save_dest(dest_buf, buf1, buf3, u_16 >> 8, v_16 >> 8);
}

static int bilinear_zoom(unsigned char* src_image, int32_t src_width, int32_t src_height,
	unsigned char * dest_image, int32_t dest_width, int32_t dest_height)
{
	fprintf(stderr, "%s():--start\n", __FUNCTION__);
	uint32_t src_line_bytes = (uint32_t)src_width*pixel_size;
	uint32_t dest_line_bytes = (uint32_t)dest_width*pixel_size;

	int32_t x_zoom_ratio = (src_width << 16) / dest_width + 1;
	int32_t y_zoom_ratio = (src_height << 16) / dest_height + 1;
	const int32_t x_error = -(1 << 15) + (x_zoom_ratio >> 1);
	const int32_t y_error = -(1 << 15) + (y_zoom_ratio >> 1);

	int32_t x, y; unsigned char *dest_buf = dest_image;
	//get the special border info 
	int32_t border_y0 = -y_error / y_zoom_ratio + 1;
	if (border_y0 >= dest_height)
		border_y0 = dest_height;
	int32_t border_x0 = -x_error / x_zoom_ratio + 1;
	if (border_x0 >= dest_width)
		border_x0 = dest_width;
	int32_t border_y1 = (((src_height - 2) << 16) - y_error) / y_zoom_ratio + 1;
	if (border_y1 < border_y0)
		border_y1 = border_y0;
	int32_t border_x1 = (((src_width - 2) << 16) - x_error) / x_zoom_ratio + 1;
	if (border_x1 < border_x0)border_x1 = border_x0;

	int32_t src_offset_y = y_error;
	for (y = 0; y < border_y0; y++)
	{
		int32_t src_offset_x = x_error;
		for (x = 0; x < border_x0; x++)
		{
			bilinear_border(src_image, src_line_bytes, src_offset_x, src_offset_y,
				dest_buf + x*pixel_size, dest_line_bytes);
			src_offset_x += x_zoom_ratio;
		}
		src_offset_y += y_zoom_ratio;
		dest_buf += dest_line_bytes;
	}
	for (y = border_y0; y < border_y1; y++)
	{
		int32_t src_offset_x = x_error;
		for (x = 0; x < border_x0; x++)
		{
			bilinear_border(src_image, src_line_bytes, src_offset_x, src_offset_y,
				dest_buf + x*pixel_size, dest_line_bytes);
			src_offset_x += x_zoom_ratio;
		}
		{
			uint32_t v_8 = (src_offset_y & 0xFFFF) >> 8;
			unsigned char *cur_src_line_buf = src_image + src_line_bytes*(src_offset_y >> 16);
			uint32_t i;
			for (i = border_x0; i < border_x1; i++)
			{
				unsigned char *buf1 = cur_src_line_buf + (src_offset_x >> 16);
				unsigned char *buf2 = buf1 + src_line_bytes;
				bilinear_save_dest(dest_buf + i*pixel_size, buf1, buf2, (src_offset_x & 0xFFFF) >> 8, v_8);
				src_offset_x += x_zoom_ratio;
			}

		}
		for (x = border_x1; x < dest_width; x++)
		{
			bilinear_border(src_image, src_line_bytes, src_offset_x, src_offset_y,
				dest_buf + x*pixel_size, dest_line_bytes);
			src_offset_x += x_zoom_ratio;
		}
		src_offset_y += y_zoom_ratio;
		dest_buf += dest_line_bytes;
	}
	for (y = border_y1; y < dest_height; y++)
	{
		int32_t src_offset_x = x_error;
		for (x = 0; x < dest_width; x++)
		{
			bilinear_border(src_image, src_line_bytes, src_offset_x, src_offset_y,
				dest_buf + x*pixel_size, dest_line_bytes);
			src_offset_x += x_zoom_ratio;
		}
		src_offset_y += y_zoom_ratio;
		dest_buf += dest_line_bytes;
	}
	fprintf(stderr, "%s():--down\n", __FUNCTION__);
}

static int nearest_zoom(unsigned char* src_image, int32_t src_width, int32_t src_height,
	unsigned char * dest_image, int32_t dest_width, int32_t dest_height, float x_zoom_ratio, float y_zoom_ratio)
{
	fprintf(stderr, "%s():--start\n", __FUNCTION__);
	int x, y;
	unsigned char *p0;
	unsigned char *p1;
	int32_t *attr_x = NULL;
	int32_t *attr_y = NULL;
	if ((attr_x = (int32_t *)malloc(sizeof(int32_t)*dest_width)) == NULL)
		return -1;
	if ((attr_y = (int32_t *)malloc(sizeof(int32_t)*dest_height)) == NULL)
		return -1;
	int32_t src_line_bytes = pixel_size*src_width;
	int32_t dest_line_bytes = pixel_size*dest_width;
	for (x = 0; x < dest_width; x++)
		*(attr_x+x) = (int32_t)(x*x_zoom_ratio);
	for (y = 0; y < dest_height; y++)
		*(attr_y+y) = (int32_t)(y*y_zoom_ratio);
	for (y = 0; y < dest_height; y++)
	{
		p0 = src_image + src_line_bytes*attr_y[y];
		p1 = dest_image + dest_line_bytes*y;
		for (x = 0; x < dest_width; x++)
			memcpy(p1 + pixel_size * x, p0 + attr_x[x] * pixel_size, pixel_size);
	}
	if (attr_x) free(attr_x);
	if (attr_y) free(attr_y);

	fprintf(stderr, "%s()--down \n", __FUNCTION__);
	return 0;
}

 int zoom_image(unsigned char* src_image, int32_t src_width, int32_t src_height,
	unsigned char * dest_image, int32_t dest_width, int32_t dest_height)
{
	int ret = -1;
	if (src_image == NULL || src_width <= 0 || src_height <= 0 || dest_image == NULL || dest_width <= 0 || dest_height <= 0)
		return -1;
	float x_zoom_ratio = (float)src_width / dest_width;
	float y_zoom_ratio = (float)src_height / dest_height;
	fprintf(stderr, "%s():x_zoom_ratio=%f y_zoom_ratio=%f \n", __FUNCTION__, x_zoom_ratio, y_zoom_ratio);
	ret = bilinear_zoom(src_image, src_width, src_height, dest_image, dest_width, dest_height);
	
	if (0)/* narrow */
	{
		if (x_zoom_ratio<4 && y_zoom_ratio < 4) /* nrarest_zoom */
		{
			ret=nearest_zoom(src_image, src_width, src_height, dest_image, dest_width, dest_height, x_zoom_ratio, y_zoom_ratio);
		}
		else /* bilinear_zoom */
		{
			ret = bilinear_zoom(src_image, src_width, src_height, dest_image, dest_width, dest_height);
		}

	}
	//else /* enlarge */
	if (0)
	{
		if (x_zoom_ratio > 0.25 && y_zoom_ratio>0.25)/* nrarest_zoom */
		{
			ret = nearest_zoom(src_image, src_width, src_height, dest_image, dest_width, dest_height, x_zoom_ratio, y_zoom_ratio);
		}
		else/* bilinear_zoom */
		{
			ret = bilinear_zoom(src_image, src_width, src_height, dest_image, dest_width, dest_height);
		}

	}
}

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
static unsigned char* pixels_bound1(unsigned char *src, const uint32_t src_line_bytes, int32_t src_width, int32_t src_height, int32_t x, int32_t y)
{
	if (x < 0)x = 0; else if (x >= src_width)x = src_width - 1;
	if (y < 0)y = 0; else if (y >= src_height)y = src_height - 1;
	return (src + src_line_bytes*y + pixel_size*x);
}
static inline void bilinear_save_dest(unsigned char *dest_buf, unsigned char*src_buf1, unsigned char*src_buf2, unsigned char*src_buf3, unsigned char *src_buf4, uint32_t u, uint32_t v)
{
	/*uint32_t pw3 = u*v; uint32_t pw2 = (u << 8) - pw3;
	uint32_t pw1 = (v << 8) - pw3; uint32_t pw0 = (1 << 16) - pw1 - pw2 - pw3;*/
	uint32_t pw3 = u*v; uint32_t pw2 = (u*(uint32_t)(256 - v));
	uint32_t pw1 = (v*(uint32_t)(256 - u)); uint32_t pw0 = (256 - u)*(256 - v);
	dest_buf[0] = ((pw0*(src_buf1[0]) + pw1*src_buf2[0] + pw2*src_buf3[0] + pw3*src_buf4[0]) >> 16);
	dest_buf[1] = ((pw0*(src_buf1[1]) + pw1*src_buf2[1] + pw2*src_buf3[1] + pw3*src_buf4[1]) >> 16);
	dest_buf[2] = ((pw0*(src_buf1[2]) + pw1*src_buf2[2] + pw2*src_buf3[2] + pw3*src_buf4[2]) >> 16);
}
static inline void bilinear_save_dest1(unsigned char *dest_buf, unsigned char*src_buf1, unsigned char*src_buf2, uint32_t u, uint32_t v)
{
	uint32_t pw3 = u*v; uint32_t pw2 = (u << 8) - pw3;
	uint32_t pw1 = (v << 8) - pw3; uint32_t pw0 = (1 << 16) - pw1 - pw2 - pw3;
	/*uint32_t pw3 = u*v; uint32_t pw2 = (u*(uint32_t)(256 - v));
	uint32_t pw1 = (v*(uint32_t)(256 - u)); uint32_t pw0 = (256 - u)*(256 - v);*/
	dest_buf[0] = ((pw0*(src_buf1[0]) + pw1*src_buf2[0] + pw2*src_buf1[0] + pw3*src_buf2[0]) >> 16);
	dest_buf[1] = ((pw0*(src_buf1[1]) + pw1*src_buf2[1] + pw2*src_buf1[1] + pw3*src_buf2[1]) >> 16);
	dest_buf[2] = ((pw0*(src_buf1[2]) + pw1*src_buf2[2] + pw2*src_buf1[2] + pw3*src_buf2[2]) >> 16);
}
static inline void bilinear_border(unsigned char* src_image, const uint32_t src_line_bytes, const int32_t x_16, const int32_t y_16,
	unsigned char*dest_buf, const uint32_t dest_line_bytes)
{
	int32_t x = (x_16 >> 16); int32_t y = (y_16 >> 16);
	uint32_t u = (x_16 & 0xFFFF) >> 8;
	uint32_t v = (y_16 & 0xFFFF) >> 8;
	unsigned char *buf1; unsigned char *buf2;
	unsigned char *buf3; unsigned char *buf4;
	//get the source image 4 pixels around the destination position
	buf1 = pixels_bound(src_image, src_line_bytes, x, y);
	buf2 = pixels_bound(src_image, src_line_bytes, x + 1, y);
	buf3 = pixels_bound(src_image, src_line_bytes, x, y + 1);
	buf4 = pixels_bound(src_image, src_line_bytes, x + 1, y + 1);

	/*
	uint32_t pm4 = u*v; uint32_t pm3 = u*(uint32_t)(256 - v);
	uint32_t pm2 = v*(uint32_t)(256 - u); uint32_t pm1 = (256 - u)*(256 - v);
	dest_buf[0] = ((pm1*buf1[0] + pm2*buf2[0] + pm3*buf3[0] + pm4*buf4[0])>>16);
	dest_buf[1] = ((pm1*buf1[1] + pm2*buf2[1] + pm3*buf3[1] + pm4*buf4[1])>>16);
	dest_buf[2] = ((pm1*buf1[2] + pm2*buf2[2] + pm3*buf3[2] + pm4*buf4[2])>>16);
	*/
	//calculate the best pixel to the dest_buf 
	bilinear_save_dest1(dest_buf, buf1, buf3, u, v);
	//bilinear_save_dest(dest_buf, buf1,buf2, buf3,buf4,u, v);
}

static int bilinear_zoom(unsigned char* src_image, size_t src_line_bytes, int32_t src_width, int32_t src_height,
	unsigned char * dest_image, int32_t dest_width, int32_t dest_height)
{
	//uint32_t src_line_bytes = (uint32_t)src_width*pixel_size;
	uint32_t dest_line_bytes = (uint32_t)dest_width*pixel_size;
	fprintf(stderr, "%s():src_line_bytes=%d\n", __FUNCTION__, src_line_bytes);
	int32_t x_zoom_ratio = ((src_width) << 16) / dest_width + 1;
	int32_t y_zoom_ratio = ((src_height) << 16) / dest_height + 1;
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
	/*
	int32_t src_offset_y = y_error;
	for (y = 0; y <dest_height; y++)
	{
	int32_t src_offset_x = x_error;
	for (x = 0; x < dest_width; x++)
	{
	bilinear_border(src_image, src_line_bytes, src_offset_x, src_offset_y,
	dest_buf + x*pixel_size, dest_line_bytes);
	src_offset_x += x_zoom_ratio;
	}
	fprintf(stderr, "%s():y=%d src_y=%d, dest_r=%d dest_g=%d dest_b=%d \n", __FUNCTION__, y,src_offset_y>>16, *dest_buf, *(dest_buf + 1), *(dest_buf + 2));
	src_offset_y += y_zoom_ratio;
	dest_buf += dest_line_bytes;
	}
	*/

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
		fprintf(stderr, "%s():y=%d dest_r=%d dest_g=%d dest_b=%d \n", __FUNCTION__, y, *dest_buf, *(dest_buf + 1), *(dest_buf + 2));
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
				unsigned char *buf1 = cur_src_line_buf + (src_offset_x >> 16) * 3;
				unsigned char *buf2 = buf1 + src_line_bytes;
				bilinear_save_dest1(dest_buf + i*pixel_size, buf1, buf2, (src_offset_x & 0xFFFF) >> 8, v_8);
				src_offset_x += x_zoom_ratio;
			}

		}
		for (x = border_x1; x < dest_width; x++)
		{
			bilinear_border(src_image, src_line_bytes, src_offset_x, src_offset_y,
				dest_buf + x*pixel_size, dest_line_bytes);
			src_offset_x += x_zoom_ratio;
		}
		fprintf(stderr, "%s():y=%d dest_r=%d dest_g=%d dest_b=%d \n", __FUNCTION__, y, *dest_buf, *(dest_buf + 1), *(dest_buf + 2));
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
		fprintf(stderr, "%s():y=%d dest_r=%d dest_g=%d dest_b=%d \n", __FUNCTION__, y, *dest_buf, *(dest_buf + 1), *(dest_buf + 2));
		src_offset_y += y_zoom_ratio;
		dest_buf += dest_line_bytes;
	}

	return 0;
}

static int bilinear1(unsigned char* src_buf, uint32_t src_line_bytes, float src_x, float src_y, unsigned char* dest_buf)
{
	int32_t x = (int32_t)src_x; if (x > src_x) --x;
	int32_t y = (int32_t)src_y; if (y > src_y) --y;
	unsigned char *buf1; unsigned char *buf2;
	unsigned char *buf3; unsigned char *buf4;

	//get the source image 4 pixels around the destination position
	buf1 = pixels_bound(src_buf, src_line_bytes, x, y);
	buf2 = pixels_bound(src_buf, src_line_bytes, x + 1, y);
	buf3 = pixels_bound(src_buf, src_line_bytes, x, y + 1);
	buf4 = pixels_bound(src_buf, src_line_bytes, x + 1, y + 1);
	float u = src_x - x; float v = src_y - y;
	float pm4 = u*v; float pm3 = u*(1 - v);
	float pm2 = v*(1 - u); float pm1 = (1 - u)*(1 - v);
	dest_buf[0] = (pm1*buf1[0] + pm2*buf2[0] + pm3*buf3[0] + pm4*buf4[0]);
	dest_buf[1] = (pm1*buf1[1] + pm2*buf2[1] + pm3*buf3[1] + pm4*buf4[1]);
	dest_buf[2] = (pm1*buf1[2] + pm2*buf2[2] + pm3*buf3[2] + pm4*buf4[2]);
	return 0;
}

static int bilinear_zoom1(unsigned char* src_image, int32_t src_width, int32_t src_height,
	unsigned char * dest_image, int32_t dest_width, int32_t dest_height)
{
	uint32_t x, y;
	uint32_t src_line_bytes = src_width*pixel_size;
	uint32_t dest_line_bytes = dest_width*pixel_size;
	float x_zoom_ratio = (float)src_width / dest_width;
	float y_zoom_ratio = (float)src_height / dest_height;
	unsigned char* src_buf = src_image; unsigned char* dest_buf = dest_image;

	for (y = 0; y < dest_height; y++)
	{
		float src_y = (y + 0.5)*y_zoom_ratio - 0.5;
		for (x = 0; x < dest_width; x++)
		{
			float src_x = (x + 0.5)*x_zoom_ratio - 0.5;
			bilinear1(src_buf, src_line_bytes, src_x, src_y, dest_buf + x * 3);
		}
		fprintf(stderr, "%s():y=%d src_y=%d, r=%d g=%d b=%d\n", __FUNCTION__, y, (uint32_t)src_y, dest_buf[0], dest_buf[1], dest_buf[2]);
		dest_buf += dest_line_bytes;
	}
	return 0;


}

static int bilinear2(unsigned char* src_buf, uint32_t src_line_bytes, int32_t src_width, int32_t src_height, int32_t src_x, int32_t src_y, unsigned char* dest_buf)
{
	//int32_t x = (int32_t)src_x; if (x > src_x) --x;
	//int32_t y = (int32_t)src_y; if (y > src_y) --y;
	int32_t x = src_x >> 16;
	int32_t y = src_y >> 16;
	unsigned char *buf1; unsigned char *buf2;
	unsigned char *buf3; unsigned char *buf4;
	//fprintf(stderr, "%s():x=%d y=%d\n", __FUNCTION__, x, y);

	//get the source image 4 pixels around the destination position
	buf1 = pixels_bound1(src_buf, src_line_bytes, src_width, src_height, x, y);
	buf2 = pixels_bound1(src_buf, src_line_bytes, src_width, src_height, x + 1, y);
	buf3 = pixels_bound1(src_buf, src_line_bytes, src_width, src_height, x, y + 1);
	buf4 = pixels_bound1(src_buf, src_line_bytes, src_width, src_height, x + 1, y + 1);

	uint32_t u = (src_x & 0xFFFF) >> 8; uint32_t v = (src_y & 0xFFFF) >> 8;
	uint32_t pm4 = u*v; uint32_t pm3 = u*(uint32_t)(256 - v);
	uint32_t pm2 = v*(uint32_t)(256 - u); uint32_t pm1 = (256 - u)*(256 - v);
	dest_buf[0] = ((pm1*buf1[0] + pm2*buf2[0] + pm3*buf3[0] + pm4*buf4[0]) >> 16);
	dest_buf[1] = ((pm1*buf1[1] + pm2*buf2[1] + pm3*buf3[1] + pm4*buf4[1]) >> 16);
	dest_buf[2] = ((pm1*buf1[2] + pm2*buf2[2] + pm3*buf3[2] + pm4*buf4[2]) >> 16);
	return 0;
}

static int bilinear_zoom2(unsigned char* src_image, size_t src_line_bytes, int32_t src_width, int32_t src_height,
	unsigned char * dest_image, int32_t dest_width, int32_t dest_height)
{
	uint32_t x, y;
	//uint32_t src_line_bytes = src_width*pixel_size;
	uint32_t dest_line_bytes = dest_width*pixel_size;
	int32_t x_zoom_ratio = (src_width << 16) / dest_width + 1;
	int32_t y_zoom_ratio = (src_height << 16) / dest_height + 1;
	const int32_t x_error = -(1 << 15) + (x_zoom_ratio >> 1);
	const int32_t y_error = -(1 << 15) + (y_zoom_ratio >> 1);
	unsigned char* src_buf = src_image; unsigned char* dest_buf = dest_image;

	int32_t src_y = y_error;
	for (y = 0; y < dest_height; y++)
	{
		int32_t src_x = x_error;
		for (x = 0; x < dest_width; x++)
		{
			bilinear2(src_buf, src_line_bytes, src_width, src_height, src_x, src_y, dest_buf + x * 3);
			src_x += x_zoom_ratio;
		}
		fprintf(stderr, "%s():y=%d src_y=%d r=%d g=%d b=%d\n", __FUNCTION__, y, src_y >> 16, *src_buf, *(src_buf + 1), *(src_buf + 2));
		src_y += y_zoom_ratio;
		dest_buf += dest_line_bytes;
	}



}

static int nearest_zoom(unsigned char *src_image, size_t src_line_bytes, int32_t src_width, int32_t src_height,
	unsigned char *dest_image, int32_t dest_width, int32_t dest_height, float x_zoom_ratio, float y_zoom_ratio)
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
	//int32_t src_line_bytes = pixel_size*src_width;
	int32_t dest_line_bytes = pixel_size*dest_width;
	for (x = 0; x < dest_width; x++)
		*(attr_x + x) = (int32_t)(x*x_zoom_ratio);
	for (y = 0; y < dest_height; y++)
		*(attr_y + y) = (int32_t)(y*y_zoom_ratio);
	for (y = 0; y < dest_height; y++)
	{
		p0 = src_image + src_line_bytes*attr_y[y];
		//p0=src_image;
		p1 = dest_image + dest_line_bytes*y;
		fprintf(stderr, "%s():y=%d attr_y[y]=%d ,  src_r=%d ,src_g=%d ,src_b=%d \n", __FUNCTION__, y, attr_y[y], *p0, *(p0 + 1), *(p0 + 2));
		for (x = 0; x < dest_width; x++)
		{
			memcpy(p1 + pixel_size * x, p0 + attr_x[x] * pixel_size, pixel_size);
		}
	}
	//fprintf(stderr, "%s():y=%d , x=%d , dest_height=%d ,dest_width=%d\n", __FUNCTION__,y,x,dest_height,dest_width);
	if (attr_x) free(attr_x);
	if (attr_y) free(attr_y);
	fprintf(stderr, "%s()--down \n", __FUNCTION__);
	return 0;
}

static double calculate_interpolation(double x)
{
	if (x < 0)
		x = -x;
	double x2 = x*x; double x3 = x2*x;
	if (x < 1)
		return (1 - 2 * x2 + x3);
	else if (x < 2)
		return (4 - 8 * x + 5 * x2 - x3);
	else
		return 0;
}
static int cubic_convolution(unsigned char *src_image, size_t src_line_bytes, int32_t src_width, int32_t src_height,
	unsigned char * dest_image, int32_t dest_width, int32_t dest_height)
{

}
static int zoom_image(unsigned char *src_image, size_t src_line_bytes, int32_t src_width, int32_t src_height,
	unsigned char *dest_image, int32_t dest_width, int32_t dest_height)
{
	int ret = -1;
	if (src_image == NULL || src_width <= 0 || src_height <= 0 || dest_image == NULL || dest_width <= 0 || dest_height <= 0)
		return -1;
	float x_zoom_ratio = (float)src_width / dest_width;
	float y_zoom_ratio = (float)src_height / dest_height;

	ret = bilinear_zoom(src_image, src_line_bytes, src_width, src_height, dest_image, dest_width, dest_height);
	//ret = bilinear_zoom1(src_image, src_width, src_height, dest_image, dest_width, dest_height);
	//ret = bilinear_zoom2(src_image, src_line_bytes,src_width, src_height, dest_image, dest_width, dest_height);
	//ret = nearest_zoom(src_image, src_line_bytes,src_width, src_height, dest_image, dest_width, dest_height, x_zoom_ratio, y_zoom_ratio);
	return ret;
	if (x_zoom_ratio>1)/* narrow */
	{
		if (x_zoom_ratio<4 && y_zoom_ratio< 4) /* nrarest_zoom */
		{
		}
		else /* bilinear_zoom */
		{

		}

	}
	else /* enlarge */
	{
		if (x_zoom_ratio>0.25 && y_zoom_ratio>0.25)/* nrarest_zoom */
		{

		}
		else/* bilinear_zoom */
		{

		}

	}
}
#pragma once
#pragma execution_character_set("utf-8")
#ifndef __ZOOM_H_
#define __ZOOM_H_
#include <stdint.h>

int zoom_image(unsigned char* src_image, int32_t src_width, int32_t src_height,
	unsigned char * dest_image, int32_t dest_width, int32_t dest_height);

#endif // !__ZOOM_H_

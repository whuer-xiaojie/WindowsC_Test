#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <iostream>

static void reverse_char_position(uint16_t *str, size_t size)
{
	
	uint16_t *start, *end;
	if (str == NULL || size <= 0)
		return;
	
	for (int k = 0; k < size; k++)
		printf("%#X  ", str[k]);
	start = str; end = str + size-1;
	for (; end > start; start++,end--){
		uint16_t ch = start[0];
		start[0] = end[0];
		end[0] = ch;
	}
	printf("%s()£º\n", __FUNCTION__);
	
	for (int k = 0; k < size; k++)
		printf("%#X  ", str[k]);
	printf("%s()£º\n", __FUNCTION__);
}
static void reverse_fun(uint8_t *src, size_t size)
{
	uint16_t *src_data = (uint16_t*)src;
	reverse_char_position(src_data, size / 2);
	//uint8_t buf[100];
	//memcpy(buf, (uint8_t*)src_data, size);
	//memcpy(src, buf, size);
	//src = (uint8_t *)src_data;
}
int main()
{
	uint8_t str[200]; int i;
	memset(str, '\0', 200);
	for (i = 0; i < 10; i++)
		str[i] = i;
	/*//uint16_t str1[200];
	//memset(str1, '\0', 200);
	uint16_t *str1 = (uint16_t *)str;
	//int len = strlen(str);
	for (i = 0; i < 20; i++)
		fprintf(stderr, "%#X ", str[i]);
	//len = strlen(str1);
	fprintf(stderr, "str----\n");
	for (i = 0; i < 20;i++)
		fprintf(stderr, "%#X ", str1[i]);
	fprintf(stderr, "str1-----\n");
	uint8_t *test = (uint8_t*)str1;
	for (int i = 0; i < 10; i++)
		fprintf(stderr, "%#X ", test[i]);
	printf("test-----\n");
	for (i = 0; i < 10; i+=2){
		uint8_t c = str[i];
		str[i] = str[i + 1];
		str[i + 1] = c;
	}
	uint16_t *str2 = (uint16_t *)str;
	//int len = strlen(str);
	for (i = 0; i < 20; i++)
		fprintf(stderr, "%#X ", str[i]);
	fprintf(stderr, "str----\n");
	//len = strlen(str1);
	for (i = 0; i < 20; i++)
		fprintf(stderr, "%#X ", str2[i]);
	fprintf(stderr, "str2----\n");*/
	reverse_fun(str, 10);
	for (i = 0; i < 10; i++)
		fprintf(stderr, "%#X ", str[i]);
	system("pause");
	return 0;
}
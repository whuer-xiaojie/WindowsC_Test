#include <iostream>
#include <stdio.h>
#include <stdlib.h>
void main()
{
	int i = 3;
	int *p;
	p = &i+5;

	printf("i 存放的内容的值: %d, i 自己所在的地址: %p\n", i, &i);

	printf("p 存放的地址的值: %p; p 自己所在的地址: %p; p 存放的地址所指所存放内容的值: %d\n", p, &p, *p);

	system("pause");
	return;
}
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
void main()
{
	int i = 3;
	int *p;
	p = &i+5;

	printf("i ��ŵ����ݵ�ֵ: %d, i �Լ����ڵĵ�ַ: %p\n", i, &i);

	printf("p ��ŵĵ�ַ��ֵ: %p; p �Լ����ڵĵ�ַ: %p; p ��ŵĵ�ַ��ָ��������ݵ�ֵ: %d\n", p, &p, *p);

	system("pause");
	return;
}
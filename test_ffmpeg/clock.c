/*
* clock.c
*/

#include <stdio.h>
#include <sys/select.h>

#include "clock.h"

/************************************************************************/
/*  
*������void __sleep(unsigned long ms)
*���ܣ�ϵͳ���ã�ʵ������
*����������:unsigned long ms ��Ҫ���ߵ�ʱ�䵥λ����
*/
/************************************************************************/
void __sleep(unsigned long ms)
{
	struct timeval tv;

	tv.tv_sec = ms / 1000;
	tv.tv_usec = ms % 1000 * 1000;
	select(0, NULL, NULL, NULL, &tv);
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
void __delay(unsigned long ms)
{
	unsigned long i, j;

	for (i = 0; i < ms; i++)
		for (j = 0; j < 60000; j++)
			__asm__("");
}
/************************************************************************/
/*    
*������unsigned long __clock(void)
*���ܣ���ȡϵͳ��ǰ��ʱ��
*������
*�����ϵͳ��ǰ��ʱ�䣨UTC����λΪ����
*/
/************************************************************************/
unsigned long __clock(void)
{
	struct timespec ts;
	unsigned long n_ticks;

	if (clock_gettime(CLOCK_MONOTONIC_RAW, &ts) != 0)
		return 0;
	n_ticks = ts.tv_sec;
	n_ticks *= 1000;
	n_ticks += ((unsigned long)ts.tv_nsec + 500000L) / 1000000L;
	return n_ticks;
}
/************************************************************************/
/*  
*������struct tm* getCurTime(void)
*���ܣ���ȡϵͳ��ǰ��ʱ�䣨����ʱ�䣩
*������
*�����ϵͳʱ��ת���󵱵�ʱ��ṹ�壨sturct tm*��
*/
/************************************************************************/
struct tm* getCurTime(void)
{
	time_t timer;//time_t����long int ����
	struct tm *t;
	timer = time(NULL);
	t = localtime(&timer);
	return t;
}

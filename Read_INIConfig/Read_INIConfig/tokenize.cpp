/*
*tokenize.c
*/

#include "tokenize.h"

char*Tokenize(const char*str, const char c, unsigned int * const uPos)
{
	unsigned int uLen = strlen(str);
	if (*uPos >= uLen)
	{
		return "";
	}
	char *strRet = (char*)malloc(uLen + 1 - *uPos);
	memset(strRet, 0, uLen + 1);
	unsigned int i = *uPos;
	for (i = *uPos; i < uLen + 1; i++)
	{
		if (str[i] != c)
		{
			strRet[i - *uPos] = str[i];
		}
		else
			break;
	}
	strRet[i - *uPos] = '\0';
	*uPos = i + 1;
	return strRet;
}
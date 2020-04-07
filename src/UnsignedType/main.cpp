/*
 * MIT License
 *
 * Copyright (c) 2019 Whuer_XiaoJie <1939346428@qq.com>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *
 */

#include <inttypes.h>
#include <stdarg.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include <iostream>
#include <list>
#include <map>
#include <string>
#include <vector>

using namespace std;

#pragma warning(disable : 4996)

#define _CRT_SECURE_NO_DEPRECATE

#define FPRINTF(format, ...)    \
    va_list ap;                 \
    char logs[10240];           \
    va_start(ap, format);       \
    vsprintf(logs, format, ap); \
    va_end(ap);                 \
    fprintf(stderr, "[file:%s  function:%s  line: %d]  %s", __FILE__, __FUNCTION__, __LINE__, logs);

#define debug(fmt, ...)                                          \
    fprintf(stderr, "%s:%d:%s()\t" fmt "\n", __FILE__, __LINE__, \
            __FUNCTION__, ##__VA_ARGS__)

typedef struct MyStruct {
    int a;
    string s;
} MyStruct;

int main() {
    //unsigned long size = 100;
    //void *addr;
    //addr = malloc(size_t(size));
    //if (addr == NULL){
    //	fprintf(stderr, "malloc failed \n");
    //	return 0;
    //}
    //fprintf(stderr, "addr=%lu\n", addr);
    //unsigned long ret = (unsigned long)addr;
    //fprintf(stderr, "ret=%lu \n", ret);
    //
    //fprintf(stderr, "size_t=%d\n", sizeof(size_t));
    //
    // 	char s = 5;
    // 	s = s&(~1);
    // 	fprintf(stderr, "%d\n", s);
    //
    // 	s = 3;
    // 	s = s&(~1);
    // 	fprintf(stderr, "%d\n", s);
    //
    // 	s = 0xF0;
    // 	s = s&(~1);
    // 	fprintf(stderr, "%#x\n", s);

    //list<int> data;
    //data.push_back(0);
    //data.push_back(1); data.push_back(2);
    //list<int>::iterator itra = data.end();
    //data.push_back(4);
    //data.push_back(5);
    //
    //for (list<int>::iterator it=data.begin(); it !=data.end(); it++){
    //	fprintf(stderr, "%d  ", *it);
    //}
    //fprintf(stderr, "\n");
    //data.insert(itra, 3);
    //for (list<int>::iterator it = data.begin(); it != data.end(); it++){
    //	fprintf(stderr, "%d  ", *it);
    //}

    //int curBrighness = 250;
    //fprintf(stderr, "%s():%d\n", __FUNCTION__, (uint8_t)(((curBrighness / 4 + 1) / 64.0) * 100));
    //char SchemeInfo[] = "55qwert5asdfg5zxcvb5qazws5dfghjk";
    //std::vector<string>allSchemeID;
    //char schemeID[1024];
    //size_t nScheme = 5;
    //int offset = 1;
    //for (int i = 0; i < nScheme; i++){
    //	int idLen = 5;
    //	memcpy(schemeID, SchemeInfo + offset + 1, idLen);
    //	offset += (idLen + 1);
    //	schemeID[idLen] = '\0';
    //	string str = schemeID;
    //	allSchemeID.push_back(str);
    //	memset(schemeID, 0, 1024);
    //}
    //std::vector<string>::iterator itor;
    //for (itor = allSchemeID.begin(); itor != allSchemeID.end();itor++)
    //{
    //	fprintf(stderr, "%s %d\n", (*itor).data(), (*itor).length());
    //}

    //unsigned char c1 = 0x00, c2 = 0x01;
    //unsigned char c3[2] = { 0x00, 0x01 };
    //int n1 = 0x55; int n2 = 0xa0;
    //char buf[20];
    ////sprintf_s(buf, "%d%d", n1, n2);
    //string s1;
    ////s1 += c1; s1 += c2;
    ////s1 =(char *) c3;
    //s1 += to_string(c1);
    //s1 += to_string(c2);
    //s1 = buf;
    //for (int i = 0; i < 20; i++){
    //	fprintf(stderr, "buf[%d]=%d\n",i,buf[i]);
    //}
    //fprintf(stderr, "%s  %d\n", s1.data(), s1.size());

    //map<int, string> myMap;
    //myMap[0] = "aaaa";
    //myMap[1] = "bbbb";
    //myMap[2] = "cccc";
    //myMap.erase(2);
    //myMap[3] = "dddd";
    //map<int, string>::iterator itor = myMap.begin();
    //
    //cout << "count=" << myMap.size() << endl;
    //for (; itor != myMap.end(); itor++){
    //	cout << "myMap[" << itor->first << "]=" << itor->second << endl;
    //	if (itor->first == 1){
    //		itor->second = "BBBB";
    //		cout << "after:myMap[" << itor->first << "]=" << itor->second << endl;
    //	}
    //}
    //
    //uint8_t c = ((~0x00) >> 1);
    //uint8_t c1 = 0xFF;
    //uint8_t c2 = c1 & 0x7F;
    //fprintf(stderr, "%s():%#X  %#x\n", __FUNCTION__, c1,c2);
    //FPRINTF("%s():%#X  %#x\n", __FUNCTION__, c1, c2);
    //debug("%s():%#X  %#x\n", __FUNCTION__, c1, c2);

    //vector<MyStruct*> test;
    //MyStruct *s1, *s2;
    //char *str = "struct1";
    ////s1 = (MyStruct *)malloc(sizeof(MyStruct));
    ////s2 = (MyStruct *)malloc(sizeof(MyStruct));
    //s1 = new MyStruct; s2 = new MyStruct;
    //s1->a = 1; s1->s = str;
    //s2->a = 2; s2->s = "struct2";
    //test.push_back(s1);
    //test.push_back(s2);
    //MyStruct s3;
    //s3.a = 3; s3.s = "struct3";
    //vector<MyStruct*>::iterator itor = test.begin();
    //for (; itor != test.end(); itor++){
    //	cout << "a=" << (*itor)->a << "    string=" << (*itor)->s << endl;
    //	if ((*itor)->a == 1){
    //		*itor = &s3;
    //	}
    //}
    //for (itor = test.begin(); itor != test.end(); itor++){
    //	cout << "a=" << (*itor)->a << "    string=" << (*itor)->s << endl;
    //	//if ((*itor)->a == 1){
    //	//	*itor = &s3;
    //	//}
    //}

    char *pTest = NULL;
    pTest = (char *)malloc(101);
    memset(pTest, 'a', 100);
    fprintf(stderr, "pTest=%d    %c\n", &(*pTest++), *pTest);
    fprintf(stderr, "pTest++=%d  %c\n", pTest++, *pTest);
    fprintf(stderr, "pTest2=%d   %c\n", pTest++, *pTest);

    system("pause");
    return 0;
}
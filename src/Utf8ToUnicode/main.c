
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
bool unicode_to_utf8(char *inbuf, size_t *inlen, char *outbuf, size_t *outlen) {
    /* 目的编码, TRANSLIT：遇到无法转换的字符就找相近字符替换
	*           IGNORE ：遇到无法转换字符跳过*/
    char *encTo = "UTF-8//IGNORE";
    /* 源编码 */
    char *encFrom = "UNICODE";

    /* 获得转换句柄
	*@param encTo 目标编码方式
	*@param encFrom 源编码方式
	*
	* */
    iconv_t cd = iconv_open(encTo, encFrom);
    if (cd == (iconv_t)-1) {
        perror("iconv_open");
    }

    /* 需要转换的字符串 */
    printf("inbuf=%s\n", inbuf);

    /* 打印需要转换的字符串的长度 */
    printf("inlen=%d\n", *inlen);

    /* 由于iconv()函数会修改指针，所以要保存源指针 */
    char *tmpin = inbuf;
    char *tmpout = outbuf;
    size_t insize = *inlen;
    size_t outsize = *outlen;

    /* 进行转换
	*@param cd iconv_open()产生的句柄
	*@param srcstart 需要转换的字符串
	*@param inlen 存放还有多少字符没有转换
	*@param tempoutbuf 存放转换后的字符串
	*@param outlen 存放转换后,tempoutbuf剩余的空间
	*
	* */
    size_t ret = iconv(cd, &tmpin, inlen, &tmpout, outlen);
    if (ret == -1) {
        perror("iconv");
    }

    /* 存放转换后的字符串 */
    printf("outbuf=%s\n", outbuf);

    //存放转换后outbuf剩余的空间
    printf("outlen=%d\n", *outlen);

    int i = 0;

    for (i = 0; i < (outsize - (*outlen)); i++) {
        //printf("%2c", outbuf[i]);
        printf("%x\n", outbuf[i]);
    }

    /* 关闭句柄 */
    iconv_close(cd);

    return 0;
}

bool utf8_to_unicode(char *inbuf, size_t *inlen, char *outbuf, size_t *outlen) {
    /* 目的编码, TRANSLIT：遇到无法转换的字符就找相近字符替换
	*           IGNORE ：遇到无法转换字符跳过*/
    char *encTo = "UNICODE//IGNORE";
    /* 源编码 */
    char *encFrom = "UTF-8";

    /* 获得转换句柄
	*@param encTo 目标编码方式
	*@param encFrom 源编码方式
	*
	* */
    iconv_t cd = iconv_open(encTo, encFrom);
    if (cd == (iconv_t)-1) {
        perror("iconv_open");
    }

    /* 需要转换的字符串 */
    printf("inbuf=%s\n", inbuf);

    /* 打印需要转换的字符串的长度 */
    printf("inlen=%d\n", *inlen);

    /* 由于iconv()函数会修改指针，所以要保存源指针 */
    char *tmpin = inbuf;
    char *tmpout = outbuf;
    size_t insize = *inlen;
    size_t outsize = *outlen;

    /* 进行转换
	*@param cd iconv_open()产生的句柄
	*@param srcstart 需要转换的字符串
	*@param inlen 存放还有多少字符没有转换
	*@param tempoutbuf 存放转换后的字符串
	*@param outlen 存放转换后,tempoutbuf剩余的空间
	*
	* */
    size_t ret = iconv(cd, &tmpin, inlen, &tmpout, outlen);
    if (ret == -1) {
        perror("iconv");
    }

    /* 存放转换后的字符串 */
    printf("outbuf=%s\n", outbuf);

    //存放转换后outbuf剩余的空间
    printf("outlen=%d\n", *outlen);

    int i = 0;

    for (i = 0; i < (outsize - (*outlen)); i++) {
        //printf("%2c", outbuf[i]);
        printf("%x\n", outbuf[i]);
    }

    /* 关闭句柄 */
    iconv_close(cd);

    return 0;
}
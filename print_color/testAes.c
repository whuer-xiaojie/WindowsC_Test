// AES_test.cpp : Defines the entry point for the console application.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "polarssl/aes.h"
//FIXME  �����ʱ�� ��Ҫ��װopenssl����������libcrypto
//	gcc -o test test_aes.c -lcrypto
/*
//���Լӽ����ַ��� 16�ֽ�һ��
int test_encrypt()
{
	printf("enter test_encrypt\n");

	//��Կ
	AES_KEY aes_key;

	//��Կ��ֵ
	unsigned char userKey[16] = { 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x00 };

	//���Ŀռ�
	//unsigned char plain[16] = "helloworld";
	unsigned char plain[1024] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 0, 1, 2, 3, 4 };
	//���ܺ����ĵĿռ�
	//unsigned char dec_plain[16]={0};
	unsigned char dec_plain[1024] = { 0 };

	//���Ŀռ�
	//unsigned char cipher[16]={0};
	unsigned char cipher[1024] = { 0 };

	int i = 0;
	//
	time_t nowTime;
	nowTime = time(NULL);

	struct tm *sysTime = localtime(&nowTime);

	printf("ϵͳʱ�䣺%d��%d��%d��%dʱ%d��\n", sysTime->tm_year + 1900, sysTime->tm_mon + 1, sysTime->tm_mday, sysTime->tm_hour, sysTime->tm_min);

	//system("pause");
	//

	//���ü����õ���Կ userKey:��Կ��ֵ aes_key:AES_KEY����ָ��
	if (AES_set_encrypt_key(userKey, 128, &aes_key) < 0)
	{
		printf("������Կʧ��!!\n");
		return 0;
	}

	printf("\n*********** ����ǰ��%s\n", plain);
	//���� in, out, key:AES��Կ
	//AES_ecb_encrypt(plain, cipher, &aes_key, AES_ENCRYPT);
	for (i = 0; i < 64; i++)
	{
		AES_ecb_encrypt(plain + (i * 16), cipher + (i * 16), &aes_key, AES_ENCRYPT);
	}



	printf("\n*********** ���ܺ�%s\n", cipher);

	//���ý�����Կ userKey:��Կ��ֵ aes_key:AES_KEY����ָ��
	if (AES_set_decrypt_key(userKey, 128, &aes_key) < 0)
	{
		printf("���ý�����Կʧ��!!\n");
		return 0;
	}

	//���� in, out, key:AES��Կ 
	//AES_ecb_encrypt(cipher,dec_plain,&aes_key,AES_DECRYPT);
	for (i = 0; i < 64; i++)
	{
		AES_ecb_encrypt(cipher + (i * 16), dec_plain + (i * 16), &aes_key, AES_DECRYPT);
	}

	printf("\n*********** ���ܺ�%s\n", dec_plain);

	printf("exit test_encrypt\n");

	return 0;
}

int main(int argc, char *argv[])
{
	test_encrypt();
	sleep(5);
	return 0;
}
*/
// mbedTLS_test_res.cpp : Defines the entry point for the console application.
//

int main(int argc, char *argv[])
{
	mbedtls_aes_context aes_ctx;
	//��Կ��ֵ
	unsigned char key[16] = { 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x00 };

	//���Ŀռ�
	unsigned char plain[16] = "helloworld";
	//���ܺ����ĵĿռ�
	unsigned char dec_plain[16] = { 0 };
	//���Ŀռ�
	unsigned char cipher[16] = { 0 };

	mbedtls_aes_init(&aes_ctx);


	//���ü�����Կ
	mbedtls_aes_setkey_enc(&aes_ctx, key, 128);

	printf("\n*********** ����ǰ��%s\n", plain);
	mbedtls_aes_crypt_ecb(&aes_ctx, MBEDTLS_AES_ENCRYPT, plain, cipher);
	printf("\n*********** ���ܺ�%s\n", cipher);
	//���ý�����Կ
	mbedtls_aes_setkey_dec(&aes_ctx, key, 128);

	mbedtls_aes_crypt_ecb(&aes_ctx, MBEDTLS_AES_DECRYPT, cipher, dec_plain);
	printf("\n*********** ���ܺ�%s\n", dec_plain);

	mbedtls_aes_free(&aes_ctx);

}


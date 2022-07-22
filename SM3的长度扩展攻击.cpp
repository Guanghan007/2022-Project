#include <stdio.h>
#include "string.h"
#include <time.h> 
#include "math.h"
#include "sm3.h"

int main()
{
	unsigned char Hash[32] = { 0 };
	unsigned char Hash2[32] = { 0 };
	unsigned char Hash3[32] = { 0 };
	unsigned int* hash_len = NULL;
	unsigned char str[] = { 'd','f','g',0 };
	sm3(str, strlen((char*)str), Hash, hash_len);
	for (int i = 0; i < 32; i++)
	{
		printf("0x%x  ", Hash[i]);
	}
	printf("\n");
	//Ԥ����hash(m1||m2),����str2=m1||m2
	unsigned char str2[100] = { 0 };
	str2[0] = 'a';
	str2[1] = 'b';
	str2[2] = 'c';
	str2[3] = 0x80;
	str2[63] = 0x18;
	str2[64] = 'a';
	str2[65] = 'b';
	str2[66] = 'c';
	//66c7f0f4 62eeedd9 d1f2d46b dc10e4e2 4167c487 5cf2f7a2 297da02b 8f4ba8e0
	sm3(str2, 67, Hash2, hash_len);
	for (int i = 0; i < 32; i++)
	{
		printf("0x%x  ", Hash2[i]);
	}
	printf("\n");

	//������չ��������֪��ԭʼ����m1����֪���������β��������m2�������ܳ���NSrcLen
	unsigned int tempHash[8] = { 0 };     //������һ��ѹ�������Ĺ�ϣֵ
	unsigned int temp = 0;
	for (int i = 0; i < 8; i++) 
	{
		for (int j = 4 * i + 3; j >= 4 * i; j--) 
		{
			temp = Hash[j];
			temp = temp * pow(256, (4 * i + 3 - j));
			tempHash[i] += temp;
		}
	}
	//��m2������Ϣ���
	int nSrcLen = strlen((char*)str);
	int NSrcLen = strlen((char*)str) + 64;
	unsigned int nGroupNum = (nSrcLen + 1 + 8 + 64) / 64;
	unsigned char* ucpMsgBuf = (unsigned char*)malloc(nGroupNum * 64);
	memset(ucpMsgBuf, 0, nGroupNum * 64);
	memcpy(ucpMsgBuf, str, nSrcLen);
	ucpMsgBuf[nSrcLen] = 0x80;
	int i = 0;
	for (i = 0; i < 8; i++)
	{
		ucpMsgBuf[nGroupNum * 64 - i - 1] = ((unsigned long long)(NSrcLen * 8) >> (i * 8)) & 0xFF;
	}
	//ִ��ѹ����������������ֱ�����һ��ѹ�������Ĺ�ϣֵ��Ҫ��ӵ�β������Ϣ�����ֵ
	//for (int i = 0; i < 64; i++) {
	//	printf("%x", ucpMsgBuf[i]);
	//}
	_CF(ucpMsgBuf, tempHash);
	free(ucpMsgBuf);
	for (i = 0; i < 8; i++)
	{
		Hash3[i * 4 + 0] = (unsigned char)((tempHash[i] >> 24) & 0xFF);
		Hash3[i * 4 + 1] = (unsigned char)((tempHash[i] >> 16) & 0xFF);
		Hash3[i * 4 + 2] = (unsigned char)((tempHash[i] >> 8) & 0xFF);
		Hash3[i * 4 + 3] = (unsigned char)((tempHash[i] >> 0) & 0xFF);
	}
	for (int i = 0; i < 32; i++)
	{
		printf("0x%x  ", Hash3[i]);
	}
	printf("\n");
	for (int i = 0; i < 32; i++) 
	{
		if (Hash2[i] == Hash3[i]) 
		{
			if (i != 31)
				continue;
			else
				printf("α��ɹ���");
		}
		else
			printf("α��ʧ�ܣ�");
	}
}

#ifndef __DELAY_H__
#define __DELAY_H__
#include <reg52.h>
//#include <string.h>
#define uchar unsigned char
#define uint  unsigned int
/************************************************
** �������� �� Delay(uint T)
** �������� �� ��ʱ����
** ��    �� �� T--��ʱ���� ��С--0~65535
** ��    �� �� ��
** ˵    �� ��
************************************************/
void Delay(uint i)
{
	uint j;
	for(;i>0;i--)
	for(j=120;j>0;j--);
}
#endif
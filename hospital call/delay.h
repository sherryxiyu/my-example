
#ifndef __DELAY_H__
#define __DELAY_H__
#include <reg52.h>
//#include <string.h>
#define uchar unsigned char
#define uint  unsigned int
/************************************************
** 函数名称 ： Delay(uint T)
** 函数功能 ： 延时函数
** 输    入 ： T--延时倍数 大小--0~65535
** 输    出 ： 无
** 说    明 ：
************************************************/
void Delay(uint i)
{
	uint j;
	for(;i>0;i--)
	for(j=120;j>0;j--);
}
#endif
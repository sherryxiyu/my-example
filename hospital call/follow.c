
#include "nrf24l01.h"//无线模块头文件

sbit LED  = P1^0;	 //LED指示灯
sbit key1 = P3^2;	 //1号病床输入
sbit key2 = P3^3;	 //2号病床输入
sbit key3 = P3^4;	 //3号病床输入
sbit key4 = P3^5;	 //4号病床输入

/************************************************
** 函数名称 ： void main(void)
** 函数功能 ： 主函数
** 输    入 ： 无
** 输    出 ： 无
** 说    明 ：
************************************************/
void main(void)
{	 
	uint count=0;//计数变量
	uchar TxBuf[12];//发射缓存数组
	//无线模块检测函数
	while(NRF24L01_Check())
	{
		LED=0;
		Delay(200);
		LED=1;
		Delay(200);
    }
	init_NRF24L01();//无线模块初始化
	TxBuf[0]=0xFF;
	TxBuf[1]=0x00;
	//TxBuf[2]=0x02;//按键值
	TxBuf[3]=0x55;
	TxBuf[4]=0x55;
	TxBuf[5]=0x55;
	TxBuf[6]=0x55;
	TxBuf[7]=0x55;
	TxBuf[8]=0x55;
	TxBuf[9]=0x55;
	TxBuf[10]=0x00;
	TxBuf[11]=0xFF;
	while(1)
	{
		if(!key1)//1号是否病床呼叫
		{
			Delay(5);//延时防抖
			if(!key1)
			{
		   		LED=1;
				TxBuf[2]=0x01;//1号
			}
		}
		if(!key2)//2号是否病床呼叫
		{
		   	Delay(5);
			if(!key2)//延时防抖
			{
		   		LED=1;
				TxBuf[2]=0x02;//2号
			}
		}
		if(!key3)//3号是否病床呼叫
		{
		   	Delay(5);//延时防抖
			if(!key3)
			{
		   		LED=1;
				TxBuf[2]=0x03;//3号
			}
		}
		if(!key4)//4号是否病床呼叫
		{
		   	Delay(5);//延时防抖
			if(!key4)
			{
		   		LED=1;
				TxBuf[2]=0x04;//3号
			}
		}
		if(key1 && key2 && key3 && key4)//没有病床呼叫，执行下面指示 
		{
			count++;if(count>330)count=34;
			if(count%33==0)
			{
				LED=~LED;//led取反
			}
			TxBuf[2]=0x00;//没有病床呼叫
		}
		nRF24L01_TxPacket(TxBuf);///无线发射函数
		Delay(5);
	}
}

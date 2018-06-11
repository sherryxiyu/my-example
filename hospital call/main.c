
#include"delay.h"    //延时函数头文件
#include"lcd1602.h"    //LCD1602头文件
#include "nrf24l01.h"  //NRF24L01头文件

sbit BUZZER  = P1^1;	 //蜂鸣器输出端口
sbit KEY   = P3^7;	 //呼应按键

/************************************************
** 函数名称 ： void main(void)
** 函数功能 ： 主函数
** 输    入 ： 无
** 输    出 ： 无
** 说    明 ：
************************************************/
void main(void)
{
	uchar flagNo1=0;
	uchar RxBuf[12];//接收缓存数组
	uchar disp[16]={"Number:         "};//显示数组					 
	LCD_Init();//LCD初始化		                 				 
	WriteChar(1,0,16,"Sickbed Calling ");//第一行显示“Sickbed Calling ”
	//无线模块检测
	while(NRF24L01_Check())
	{
		BUZZER=0;
		Delay(200);
		BUZZER=1;
		Delay(200);
    }
	init_NRF24L01();//无线模块初始化
	while(1)
	{
		nRF24L01_RxPacket(RxBuf);//接收从机数据
		SetRX_Mode();//配置为接收模块
		//判断
		if(RxBuf[0]==0XFF || RxBuf[1]==0X00 || RxBuf[10]==0X00 || RxBuf[11]==0XFF)
		{
			switch(RxBuf[2])
			{
				case 0x01: flagNo1=1; disp[8]='1'; BUZZER=0; break;	//1号
				case 0x02: flagNo1=2; disp[10]='2'; BUZZER=0; break;//2号
				case 0x03: flagNo1=3; disp[12]='3'; BUZZER=0; break;//3号
				case 0x04: flagNo1=4; disp[14]='4'; BUZZER=0; break;//4号
			}		
		} 
		//呼应处理
		if(!KEY)
		{
			BUZZER=1;//关闭蜂鸣器
			disp[8]=' ';disp[10]=' ';disp[12]=' ';disp[14]=' ';//清空显示
		}
		WriteChar(2,0,16,disp);//调用显示  显示液晶第二行
	}
}
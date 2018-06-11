
#include"delay.h"    //��ʱ����ͷ�ļ�
#include"lcd1602.h"    //LCD1602ͷ�ļ�
#include "nrf24l01.h"  //NRF24L01ͷ�ļ�

sbit BUZZER  = P1^1;	 //����������˿�
sbit KEY   = P3^7;	 //��Ӧ����

/************************************************
** �������� �� void main(void)
** �������� �� ������
** ��    �� �� ��
** ��    �� �� ��
** ˵    �� ��
************************************************/
void main(void)
{
	uchar flagNo1=0;
	uchar RxBuf[12];//���ջ�������
	uchar disp[16]={"Number:         "};//��ʾ����					 
	LCD_Init();//LCD��ʼ��		                 				 
	WriteChar(1,0,16,"Sickbed Calling ");//��һ����ʾ��Sickbed Calling ��
	//����ģ����
	while(NRF24L01_Check())
	{
		BUZZER=0;
		Delay(200);
		BUZZER=1;
		Delay(200);
    }
	init_NRF24L01();//����ģ���ʼ��
	while(1)
	{
		nRF24L01_RxPacket(RxBuf);//���մӻ�����
		SetRX_Mode();//����Ϊ����ģ��
		//�ж�
		if(RxBuf[0]==0XFF || RxBuf[1]==0X00 || RxBuf[10]==0X00 || RxBuf[11]==0XFF)
		{
			switch(RxBuf[2])
			{
				case 0x01: flagNo1=1; disp[8]='1'; BUZZER=0; break;	//1��
				case 0x02: flagNo1=2; disp[10]='2'; BUZZER=0; break;//2��
				case 0x03: flagNo1=3; disp[12]='3'; BUZZER=0; break;//3��
				case 0x04: flagNo1=4; disp[14]='4'; BUZZER=0; break;//4��
			}		
		} 
		//��Ӧ����
		if(!KEY)
		{
			BUZZER=1;//�رշ�����
			disp[8]=' ';disp[10]=' ';disp[12]=' ';disp[14]=' ';//�����ʾ
		}
		WriteChar(2,0,16,disp);//������ʾ  ��ʾҺ���ڶ���
	}
}

#include "nrf24l01.h"//����ģ��ͷ�ļ�

sbit LED  = P1^0;	 //LEDָʾ��
sbit key1 = P3^2;	 //1�Ų�������
sbit key2 = P3^3;	 //2�Ų�������
sbit key3 = P3^4;	 //3�Ų�������
sbit key4 = P3^5;	 //4�Ų�������

/************************************************
** �������� �� void main(void)
** �������� �� ������
** ��    �� �� ��
** ��    �� �� ��
** ˵    �� ��
************************************************/
void main(void)
{	 
	uint count=0;//��������
	uchar TxBuf[12];//���仺������
	//����ģ���⺯��
	while(NRF24L01_Check())
	{
		LED=0;
		Delay(200);
		LED=1;
		Delay(200);
    }
	init_NRF24L01();//����ģ���ʼ��
	TxBuf[0]=0xFF;
	TxBuf[1]=0x00;
	//TxBuf[2]=0x02;//����ֵ
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
		if(!key1)//1���Ƿ񲡴�����
		{
			Delay(5);//��ʱ����
			if(!key1)
			{
		   		LED=1;
				TxBuf[2]=0x01;//1��
			}
		}
		if(!key2)//2���Ƿ񲡴�����
		{
		   	Delay(5);
			if(!key2)//��ʱ����
			{
		   		LED=1;
				TxBuf[2]=0x02;//2��
			}
		}
		if(!key3)//3���Ƿ񲡴�����
		{
		   	Delay(5);//��ʱ����
			if(!key3)
			{
		   		LED=1;
				TxBuf[2]=0x03;//3��
			}
		}
		if(!key4)//4���Ƿ񲡴�����
		{
		   	Delay(5);//��ʱ����
			if(!key4)
			{
		   		LED=1;
				TxBuf[2]=0x04;//3��
			}
		}
		if(key1 && key2 && key3 && key4)//û�в������У�ִ������ָʾ 
		{
			count++;if(count>330)count=34;
			if(count%33==0)
			{
				LED=~LED;//ledȡ��
			}
			TxBuf[2]=0x00;//û�в�������
		}
		nRF24L01_TxPacket(TxBuf);///���߷��亯��
		Delay(5);
	}
}

#ifndef NRF24L01_H
#define NRF24L01_H 
#include "delay.h"

sbit 	MISO	=P2^0;
sbit	SCK	    =P2^1;
sbit	CE	    =P2^2;

sbit	IRQ		=P2^7;
sbit 	MOSI	=P2^6;
sbit	CSN		=P2^5;

/*********NRF24L01*****************/
#define TX_ADR_WIDTH    5   	// 5 uints TX address width
#define RX_ADR_WIDTH    5   	// 5 uints RX address width
#define TX_PLOAD_WIDTH  12  	// 12 uints TX payload
#define RX_PLOAD_WIDTH  12  	// 12 uints TX payload
uchar const TX_ADDRESS[TX_ADR_WIDTH]= {0x34,0x43,0x10,0x10,0x01};	//本地地址
uchar const RX_ADDRESS[RX_ADR_WIDTH]= {0x34,0x43,0x10,0x10,0x01};	//接收地址
/*********NRF24L01寄存器指令*****************/
#define READ_REG        0x00  	// 读寄存器指令
#define WRITE_REG       0x20 	// 写寄存器指令
#define RD_RX_PLOAD     0x61  	// 读取接收数据指令
#define WR_TX_PLOAD     0xA0  	// 写待发数据指令
#define FLUSH_TX        0xE1 	// 冲洗发送 FIFO指令
#define FLUSH_RX        0xE2  	// 冲洗接收 FIFO指令
#define REUSE_TX_PL     0xE3  	// 定义重复装载数据指令
#define NOP             0xFF  	// 保留
/*********nRF24L01寄存器地址*****************/
#define CONFIG          0x00  // 配置收发状态，CRC校验模式以及收发状态响应方式
#define EN_AA           0x01  // 自动应答功能设置
#define EN_RXADDR       0x02  // 可用信道设置
#define SETUP_AW        0x03  // 收发地址宽度设置
#define SETUP_RETR      0x04  // 自动重发功能设置
#define RF_CH           0x05  // 工作频率设置
#define RF_SETUP        0x06  // 发射速率、功耗功能设置
#define STATUS          0x07  // 状态寄存器
#define OBSERVE_TX      0x08  // 发送监测功能
#define CD              0x09  // 地址检测           
#define RX_ADDR_P0      0x0A  // 频道0接收数据地址
#define RX_ADDR_P1      0x0B  // 频道1接收数据地址
#define RX_ADDR_P2      0x0C  // 频道2接收数据地址
#define RX_ADDR_P3      0x0D  // 频道3接收数据地址
#define RX_ADDR_P4      0x0E  // 频道4接收数据地址
#define RX_ADDR_P5      0x0F  // 频道5接收数据地址
#define TX_ADDR         0x10  // 发送地址寄存器
#define RX_PW_P0        0x11  // 接收频道0接收数据长度
#define RX_PW_P1        0x12  // 接收频道0接收数据长度
#define RX_PW_P2        0x13  // 接收频道0接收数据长度
#define RX_PW_P3        0x14  // 接收频道0接收数据长度
#define RX_PW_P4        0x15  // 接收频道0接收数据长度
#define RX_PW_P5        0x16  // 接收频道0接收数据长度
#define FIFO_STATUS     0x17  // FIFO栈入栈出状态寄存器设置

/**********************************************
/*函数：uint SPI_RW(uint uchar)
/*功能：NRF24L01的SPI写时序
/**********************************************/
uchar SPI_RW(uchar datal)
{
	uchar i;
   	for(i=0;i<8;i++) 
   	{
		MOSI = (datal & 0x80);         
		datal = (datal << 1);           
		SCK = 1;                      
		datal |= MISO;       		 
		SCK = 0;            		  
   	}
    return(datal);           		  
}
/***********************************
//函数：uint SPI_Read_Buf(uchar reg, uchar *pBuf, uchar uchars)
//功能: 用于读数据，reg：为寄存器地址，pBuf：为待读出数据地址，uchars：读出数据的个数
***********************************/
uchar SPI_Read_Buf(uchar reg, uchar *pBuf, uchar uchars)
{
	uchar status,i;
	CSN=0;                   
	status = SPI_RW(reg);       	
	for(i=0;i<uchars;i++)
		pBuf[i] = SPI_RW(0);  
	CSN=1;                           
	return status;                
}
/***********************************
//函数：uint SPI_Write_Buf(uchar reg, uchar *pBuf, uchar uchars)
//功能: 用于写数据：为寄存器地址，pBuf：为待写入数据地址，uchars：写入数据的个数
***********************************/
uchar SPI_Write_Buf(uchar reg, uchar *pBuf, uchar uchars)
{
	uchar status,i;
	CSN=0;             
	status = SPI_RW(reg);   
	for(i=0; i<uchars; i++) 
	SPI_RW(*pBuf++);
	CSN=1;          
	return status;
}
uint SPI_RW_Reg(uchar reg, uchar value)
{
	uint status;
	
	CSN = 0;                   // CSN low, init SPI transaction
	status = SPI_RW(reg);      // select register
	SPI_RW(value);             // ..and write value to it..
	CSN = 1;                   // CSN high again
	
	return(status);            // return nRF24L01 status uchar
}
void init_NRF24L01(void)
{
 	CE=0;    // chip enable
 	CSN=1;   // Spi disable 
 	SCK=0;   // Spi clock line init high
	SPI_Write_Buf(WRITE_REG + TX_ADDR, TX_ADDRESS, TX_ADR_WIDTH);    // 写本地地址	
	SPI_Write_Buf(WRITE_REG + RX_ADDR_P0, RX_ADDRESS, RX_ADR_WIDTH); // 写接收端地址
	SPI_RW_Reg(WRITE_REG + EN_AA, 0x3f);      //  频道0自动	ACK应答允许	
	SPI_RW_Reg(WRITE_REG + EN_RXADDR, 0x3f);  //  允许接收地址只有频道0，如果需要多频道可以参考Page21  
	SPI_RW_Reg(WRITE_REG + RF_CH, 0);        //   设置信道工作为2.4GHZ，收发必须一致
	SPI_RW_Reg(WRITE_REG + RX_PW_P0, RX_PLOAD_WIDTH); //设置接收数据长度，本次设置为20字节
	SPI_RW_Reg(WRITE_REG + RF_SETUP, 0x07);   		//设置发射速率为1MHZ，发射功率为最大值0dB	
	SPI_RW_Reg(WRITE_REG + CONFIG, 0x0e);   		 // IRQ收发完成中断响应，16位CRC，主发送
}
void nRF24L01_TxPacket(uchar * tx_buf)
{
	CE=0;			//StandBy I模式	
	SPI_Write_Buf(WRITE_REG + RX_ADDR_P0, TX_ADDRESS, TX_ADR_WIDTH); // 装载接收端地址
	SPI_Write_Buf(WR_TX_PLOAD, tx_buf, TX_PLOAD_WIDTH); 			 // 装载数据	
	CE=1;		 //置高CE，激发数据发送
	Delay(10);
	SPI_RW_Reg(WRITE_REG+STATUS,0XFF);
}
/***********************************
//检测24L01是否存在
//返回值:0:成功    1:失败	
***********************************/
uchar NRF24L01_Check(void)
{	
	uchar i;
	uchar buf[5]={0XA5,0XA5,0XA5,0XA5,0XA5}; 
	SPI_Write_Buf(WRITE_REG+TX_ADDR,buf,5);//写入5个字节的地址.	
	SPI_Read_Buf(TX_ADDR,buf,5); //读出写入的地址  
	for(i=0;i<5;i++){if(buf[i]!=0XA5)break;}	 							   
	if(i!=5)return 1;//检测24L01错误	
	return 0;		 //检测到24L01
}
#endif
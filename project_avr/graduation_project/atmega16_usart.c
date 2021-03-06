#include "atmega16_usart.h"

/************************************************************************/
/*void USART_Init();													*/
/*																		*/
/*此函数用来初始化USART，有两个参数                                       */
/*一.baud：设置波特率													*/
/*二.crystal：晶振频率													*/
/*																		*/
/************************************************************************/

void USART_Init(uint32_t baud,uint32_t crystal)					//串口初始化
{
	uint8_t baud_h,baud_l;
	IO_PinMode(PORTD_Pin_0,Input,High);			//PD0设置为输入高电平
	IO_PinMode(PORTD_Pin_1,Output,High);			//PD1设置为输出高电平
	baud_l=(uint8_t)((uint16_t)((uint32_t)crystal/(16*baud)-1));				//波特率计算公式
	baud_h=(uint8_t)(((uint16_t)((uint32_t)crystal/(16*baud)-1))>>8);
	UCSRA=0x00;	
	UBRRH=baud_h;								//设置波特率
	UBRRL=baud_l;				
	UCSRB|=(1<<RXEN)|(1<<TXEN);					//使能RX,TX
	UCSRC|=(1<<URSEL)|(1<<UCSZ1)|(1<<UCSZ0);	//无校验位，1位停止位，8位数据位
}

/************************************************************************/
/*void USART_SendByte();												*/
/*																		*/
/*此函数用来发送一个字节的数据，有一个参数									*/
/*一.data：你要发送的字节数据												*/
/*																		*/
/************************************************************************/

void USART_SendByte(uint8_t data)				//发送一个字节数据
{
	while (!(UCSRA & (1<<UDRE)));			//等待数据缓冲寄存器为空
	UDR=data;
}

/************************************************************************/
/*void USART_ReceiveByte();												*/
/*																		*/
/*此函数用来接收一个字节的数据，无参数										*/
/*返回UDR寄存器的值，也就是上位机传过来的值								*/
/*																		*/
/************************************************************************/

uint8_t USART_ReceiveByte()						//接收一个字节数据
{
	while (!(UCSRA & (1<<RXC)));			//等待数据缓冲器从上位机传出数据
	return UDR;
}

/************************************************************************/
/*void USART_SendString();												*/
/*																		*/
/*此函数用来发送一个字符串的数据，有一个参数								*/
/**data：一个字节的首地址													*/
/*																		*/
/************************************************************************/

void USART_SendString(uint8_t *data)			//发送一个字符串
{
	while (*data)
	{
		USART_SendByte(*data);
		data++;
	}
}
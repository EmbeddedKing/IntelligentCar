/*
 * 毕业设计：智能小车的设计与实现
 * 2017.10.21：实现了蓝牙与手机的通讯
 * 2017.10.23：实现了手机控制小车的行进并调试了ISD1820模块
 * 2017.10.28：调试好超声波测距模块
 * 2017.10.29：调试好DS18B20测温模块，对程序进行了整理
 */ 

#include <avr/io.h>
#define F_CPU 8000000UL
#include <util/delay.h>
#include "atmega16_io.h"
#include "atmega16_usart.h"
#include "atmega16_interrupt.h"
#include "atmega16_hc_sr04.h"
#include "atmega16_ds18b20.h"
#include "atmega16_lcd1602.h"
#define LEFT_A PORTB_Pin_0				//左侧轮子正极
#define LEFT_K PORTB_Pin_1				//左侧轮子负极
#define RIGHT_A PORTB_Pin_2				//右侧轮子正极
#define RIGHT_K PORTB_Pin_3				//右侧轮子负极
#define PLAYE PORTB_Pin_4				//ISD1820播放,高电平播放
#define PLAYL PORTB_Pin_5				//ISD1820播放，高电平播放
#define TRIG PORTA_Pin_0				//超声波模块信号发送引脚
#define ECHO PORTA_Pin_1				//超声波模块信号接收引脚
uint8_t car_data,data[3],temp[7];							//控制命令全局变量
uint16_t car_distance,car_temp;
uint8_t table[16]="only you        ";
void car_init()
{
	IO_PinMode(LEFT_A,Output,Low);				//左侧轮子正极
	IO_PinMode(LEFT_K,Output,Low);				//左侧轮子负极
	IO_PinMode(RIGHT_A,Output,Low);				//右侧轮子正极
	IO_PinMode(RIGHT_K,Output,Low);				//右侧轮子负极
	//IO_PinMode(PLAYE,Output,Low);
	//IO_PinMode(PLAYL,Output,Low);
	IO_PinMode(TRIG,Output,Low);
	IO_PinMode(ECHO,Input,Low);
	USART_Init(9600,8000000);
	HC_SR04_Init();
	IT_Config(IT_USARTRXC,ENABLE);					//开启串口中断
}

void car_control()
{
	if (car_data=='a')									//前进,左侧轮子和右侧轮子都向前
	{
		IO_PBSet(LEFT_A,High);
		IO_PBSet(LEFT_K,Low);
		IO_PBSet(RIGHT_A,High);
		IO_PBSet(RIGHT_K,Low);
		IO_PBSet(PLAYE,High);
	}
	else if (car_data=='b')								//后退，左侧轮子和右侧轮子都向后
	{
		IO_PBSet(LEFT_A,Low);
		IO_PBSet(LEFT_K,High);
		IO_PBSet(RIGHT_A,Low);
		IO_PBSet(RIGHT_K,High);
		IO_PBSet(PLAYE,High);
	}
	else if (car_data=='c')								//左转，左侧轮子向后，右侧轮子向前
	{
		IO_PBSet(LEFT_A,Low);
		IO_PBSet(LEFT_K,High);
		IO_PBSet(RIGHT_A,High);
		IO_PBSet(RIGHT_K,Low);
		IO_PBSet(PLAYE,High);
	}
	else if (car_data=='d')								//右转，左侧轮子向前，右侧轮子向后
	{
		IO_PBSet(LEFT_A,High);
		IO_PBSet(LEFT_K,Low);
		IO_PBSet(RIGHT_A,Low);
		IO_PBSet(RIGHT_K,High);
		IO_PBSet(PLAYE,High);
	}
	else if (car_data=='s')								//停止，马达没有电势差，停止
	{
		IO_PBSet(PORTB_Pin_0,Low);
		IO_PBSet(PORTB_Pin_1,Low);
		IO_PBSet(RIGHT_A,Low);
		IO_PBSet(RIGHT_K,Low);
		IO_PBSet(PLAYE,Low);
	}
}

void car_senddistance()
{
	car_distance=HC_SR04_GetDistance();
	data[0]=(uint8_t)(car_distance/100)+'0';
	data[1]=(uint8_t)(car_distance/10%10)+'0';
	data[2]=(uint8_t)(car_distance%10)+'0';
	/*if (car_distance<=20)
	{*/
		//USART_SendByte('q');
		USART_SendString("Distance is:");
		if (data[0]!='0')
		{
			USART_SendByte(data[0]);
		}
		if (data[1]!='0')
		{
			USART_SendByte(data[1]);
		}
		USART_SendByte(data[2]);
		USART_SendString("cm\n");
	//}
	/*else
	{
		USART_SendByte('w');
	}
	*/
}

void car_sendtemp()
{
	DS18B20_Reset();
	_delay_us(10);
	DS18B20_Start();
	car_temp=DS18B20_ReadTempData();
	car_temp&=0x07ff;
	car_temp=car_temp*0.625;
	temp[0]=car_temp/100+'0';
	temp[1]=car_temp%100/10+'0';
	temp[2]='.';
	temp[3]=car_temp%10+'0';
	temp[4]='c';
	temp[5]='\n';
	temp[6]='\0';
	USART_SendString("Temperature is:");
	USART_SendString(temp);
	/*for (uint8_t i=0;i<6;i++)
	{
		USART_SendByte(temp[i]);
	}
	_delay_ms(500);*/
}

void car_display()
{
	LCD1602_DisplayString(1,0,table);
}

int main(void)
{
    /* Replace with your application code */
	car_init();
    while (1)
    {
		//car_senddistance();
		//car_sendtemp();
		_delay_ms(1000);
		
    }
}

ISR(USARTRXC_vect)						//串口接收中断函数
{
	car_data=USART_ReceiveByte();
	car_control();
}
/*
 * atmega16_hc_sr04.c
 *
 * Created: 2017/10/25 11:17:14
 *  Author: win10
 */ 
#include <avr/io.h>
#define F_CPU 8000000UL
#include <util/delay.h>
#include "atmega16_hc_sr04.h"
uint16_t distance,timer;

void HC_SR04_Init()
{
	TCCR1A=0X00;
	TCCR1B=0X02;					//8分频
}
uint16_t HC_SR04_GetDistance()
{
	PORTA&=~(1<<PORTA1); 			//10us的触发信号，顺便把接收口拉低
	PORTA|=(1<<PORTA0);
	_delay_us(15);
	PORTA&=~(1<<PORTA0);
	while (!(PINA&0x02));
	TCNT1H=0; 						//低电平一过，计时器清零，开始计时
	TCNT1L=0;
	while ((PINA&0x02));
	timer=TCNT1L+256*TCNT1H; 		//高电平过后，计时结束，计算距离
	distance=timer*1.72/150;		//本来应该是1.7，1.72是修正后的值(声速跟温度有关)
	return distance;
}
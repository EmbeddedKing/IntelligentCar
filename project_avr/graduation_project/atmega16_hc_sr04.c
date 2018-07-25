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
	TCCR1B=0X02;					//8��Ƶ
}
uint16_t HC_SR04_GetDistance()
{
	PORTA&=~(1<<PORTA1); 			//10us�Ĵ����źţ�˳��ѽ��տ�����
	PORTA|=(1<<PORTA0);
	_delay_us(15);
	PORTA&=~(1<<PORTA0);
	while (!(PINA&0x02));
	TCNT1H=0; 						//�͵�ƽһ������ʱ�����㣬��ʼ��ʱ
	TCNT1L=0;
	while ((PINA&0x02));
	timer=TCNT1L+256*TCNT1H; 		//�ߵ�ƽ���󣬼�ʱ�������������
	distance=timer*1.72/150;		//����Ӧ����1.7��1.72���������ֵ(���ٸ��¶��й�)
	return distance;
}
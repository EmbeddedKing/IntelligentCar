#ifndef __ATMEGA16_DS18B20_H
#define __ATMEGA16_DS18B20_H

#include <avr/io.h>
#define F_CPU 8000000UL
#include <util/delay.h>
#include "atmega16_io.h"

//�ܽŶ����޸���
/***********************************************************/
#define DQ_OUT   DDRA|=(1<<PORTA2)				//���ó����
#define DQ_IN    DDRA&=~(1<<PORTA2)				//���ó�����
#define DQ_HIGH  PORTA|=(1<<PORTA2)				//���óɸߵ�ƽ
#define DQ_LOW   PORTA&=~(1<<PORTA2)			//���óɵ͵�ƽ
#define DQ_READ  PINA&(1<<PINA2)				//��ȡ���ŵ�ƽֵ
/***********************************************************/

void DS18B20_Reset();
void DS18B20_Start();
uint8_t DS18B20_ReadByte();
uint16_t DS18B20_ReadTempData();


#endif
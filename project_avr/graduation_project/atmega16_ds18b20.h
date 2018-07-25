#ifndef __ATMEGA16_DS18B20_H
#define __ATMEGA16_DS18B20_H

#include <avr/io.h>
#define F_CPU 8000000UL
#include <util/delay.h>
#include "atmega16_io.h"

//管脚定义修改区
/***********************************************************/
#define DQ_OUT   DDRA|=(1<<PORTA2)				//设置成输出
#define DQ_IN    DDRA&=~(1<<PORTA2)				//设置成输入
#define DQ_HIGH  PORTA|=(1<<PORTA2)				//设置成高电平
#define DQ_LOW   PORTA&=~(1<<PORTA2)			//设置成低电平
#define DQ_READ  PINA&(1<<PINA2)				//读取引脚电平值
/***********************************************************/

void DS18B20_Reset();
void DS18B20_Start();
uint8_t DS18B20_ReadByte();
uint16_t DS18B20_ReadTempData();


#endif
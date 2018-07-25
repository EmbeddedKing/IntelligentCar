#include <reg52.h>
#define uint unsigned int
#define uchar unsigned char
sbit BUZZER=P2^4;
sbit UP=P2^3;
sbit DOWN=P2^2;
sbit LEFT=P2^1;
sbit RIGHT=P2^0;
uchar car_data=0,car_flag=0;

void car_init()
{
	TMOD=0x20;
	TH1=0xfd;
	TL1=0xfd;
	TR1=1;
	REN=1;
	SM0=0;
	SM1=1;
	EA=1;
	ES=0;
	BUZZER=0;
}

void car_delay(uint x)
{
	uint i,j;
	for(i=0;i<x;i++)
		for(j=0;j<114;j++);
}

void main()
{
	car_init();
	while(1)
	{
		if(UP==0)
		{
			car_delay(10);
			if(UP==0)
			{
				ES=0;
				SBUF='a';
				while(!TI);
				TI=0;
				while(!UP);
				SBUF='s';
				while(!TI);
				TI=0;
				ES=1;
			}
		}
		else if(DOWN==0)
		{
			car_delay(10);
			if(DOWN==0)
			{
				ES=0;
				SBUF='b';
				while(!TI);
				TI=0;
				while(!DOWN);
				SBUF='s';
				while(!TI);
				TI=0;
				ES=1;
			}
		}
		else if(LEFT==0)
		{
			car_delay(10);
			if(LEFT==0)
			{
				ES=0;
				SBUF='c';
				while(!TI);
				TI=0;
				while(!LEFT);
				SBUF='s';
				while(!TI);
				TI=0;
				ES=1;
			}
		}
		else if(RIGHT==0)
		{
			car_delay(10);
			if(RIGHT==0)
			{
				ES=0;
				SBUF='d';
				while(!TI);
				TI=0;
				while(!RIGHT);			//等键释放
				SBUF='s';						//发送停止指令
				while(!TI);
				TI=0;
				ES=1;
			}
		}
	}
}

void ser() interrupt 4
{
	RI=0;
}


#include <reg52.h>
unsigned char count;		//0.5ms次数标签
sbit PWM=P2^0;
sbit sub=P3^1;
sbit paiduan=P3^0;
sbit add=P3^2;
sbit del=P3^3;
sbit LSA=P2^2;
sbit LSB=P2^3;
sbit LSC=P2^4;
sbit lcden=P2^7;	//LCD使能
sbit lcdrs=P2^6;		//LCD数据/命令选择端
sbit lcdwr=P2^5;		//LCD读写选择端
int a,b,c;
unsigned char jd;	//角度标识；
unsigned char key;
unsigned char code table[]={0xc0,0xf9,0xa4,0xb0,0x99,0x92,0x82,0xf8,
					0x80,0x90,0x88,0x83,0xc6,0xa1,0x86,0x8e};
unsigned char code lcd[]="0123456789abcdef";
unsigned char code pass[]="PASS:";
unsigned char code ag[]="JD:";					
unsigned char code ag1[]="000";
unsigned char code ag2[]="045";		
unsigned char code ag3[]="090";
unsigned char code ag4[]="135";	
unsigned char code ag5[]="180";	
unsigned char code cur[]="current";
unsigned char code err[]="ERROR";
unsigned char code kb[]="----------------";					
unsigned char mima[8];	
unsigned char code password[8]="10190931";		
void delayms(unsigned char i)		//延时
{
	unsigned char j,k;
	for(j=i;j>0;j--)
		for(k=110;k>0;k--);
}

//*************************************************************//
//lcd显示
void lcd_write_com(unsigned char com)		//LCD写命令函数
{
	lcden=0;
	lcdrs=0;
	lcdwr=0;
	P0=com;
	delayms(5);
	lcden=1;
	delayms(5);
	lcden=0;
}

void lcd_write_data(unsigned char date)		//LCD写数据函数
{
	lcden=0;
	lcdwr=0;
	lcdrs=1;
	P0=date;
	delayms(5);
	lcden=1;
	delayms(5);
	lcden=0;
}

void lcd_init()		//LCD初始化函数
{
	lcden=0;
	lcd_write_com(0x38);		//显示模式设置为16X2显示，5X7点阵，8位数据接口
	lcd_write_com(0x0c);		//设置并显示，光标不闪烁
	lcd_write_com(0x06);		//写一个字符后地址加1.整屏不移动
	lcd_write_com(0x01);		//显示清0，数据指针清0
}

void Time0_Init()
{
	TMOD=0X01;		//设置定时器工作方式1
	TH0=(65536-459)/256;		//启用定时器，每隔0.5ms溢出
	TL0=(65536-459)%256;		//TH0=(65536-N)/256,N=tus/1.09us
	EA=1;		//打开全局中断
	EX0=1;	//外部中断0中断允许位
	IT0=1;	//设置外部中断触发方式
					//0	--	低电平触发
					//1	--	负跳变触发
	EX1=1;	//外部中断1中断允许位
	IT1=1;	//设置外部中断触发方式
					//0	--	低电平触发
					//1	--	负跳变触发
	ET0=1;		//定时器0中断允许位
	TR0=1;		//定时器0运行控制位，由软件清0关闭定时器0，置1启动定时器0
}

void keydown()
{
	int m=0;
	int y=0;
	unsigned char temp;
	if(sub==0)
	{
		lcd_write_com(0x80+0x40);
	}
	if(paiduan==0)
	{
		int p,q;
		lcd_write_com(0x80+0x40);
		for(p=0;p<8;p++)
		{
				if(mima[p]==password[p])
				{
					for(q=0;q<8;q++)
					{
						lcd_write_data(cur[q]);
						delayms(500);
					}
					lcd_write_com(0x80+0x40);
					for(q=0;q<16;q++)
					{
						lcd_write_data(kb[q]);
					}
					jd=5;
				}
				else
				{
					for(q=0;q<5;q++)
					{
						lcd_write_data(err[q]);
						delayms(500);
					}
					lcd_write_com(0x80+0x40);
					for(q=0;q<16;q++)
					{
						lcd_write_data(kb[q]);
					}
				}
			}
	}
	P1=0x7f;	//0111 1111,将第一行P1^7拉低
	temp=P1;
	temp=temp&0x0f;			//判断列是否有被按下
	if(temp!=0x0f)
	{
		delayms(10);
		temp=P1;
		temp=temp&0x0f;
		if(temp!=0x0f)
		{
			temp=P1;
			switch(temp)
			{
				case 0x77:	//0111 0111
					lcd_write_data(lcd[0]);
					mima[m]=lcd[0];
					m++;
					break;
				case 0x7b:			//0111 1011
					lcd_write_data(lcd[1]);
					mima[m]=lcd[1];
					m++;
					break;
				case 0x7d:			//0111 1101
					lcd_write_data(lcd[2]);
					mima[m]=lcd[2];
					m++;
					break;
				case 0x7e:					//0111 1110
					lcd_write_data(lcd[3]);
					mima[m]=lcd[3];
					m++;
					break;
			}
			while(temp!=0x0f)
			{
				temp=P1;
				temp=temp&0x0f;
			}
		}
	}
	P1=0xbf;			//1011 1111
	temp=P1;
	temp=temp&0x0f;
	if(temp!=0x0f)
	{
		delayms(10);
		temp=P1;
		temp=temp&0x0f;
		if(temp!=0x0f)
		{
			temp=P1;
			switch(temp)
			{
				case 0xb7:			//1011 0111 
					lcd_write_data(lcd[4]);
					mima[m]=lcd[4];
					m++;
					break;
				case 0xbb:					//1011 1011
					lcd_write_data(lcd[5]);
					mima[m]=lcd[5];
					m++;
					break;
				case 0xbd:					//1011 1101
					lcd_write_data(lcd[6]);
					mima[m]=lcd[6];
					m++;
					break;
				case 0xbe:					//1011 1110
					lcd_write_data(lcd[7]);
					mima[m]=lcd[7];
					m++;
					break;
			}
			while(temp!=0x0f)
			{
				temp=P1;
				temp=temp&0x0f;
			}
		}
	}
	P1=0xdf;		//1101 1111
	temp=P1;
	temp=temp&0x0f;
	if(temp!=0x0f)
	{
		delayms(10);
		temp=P1;
		temp=temp&0x0f;
		if(temp!=0x0f)
		{
			temp=P1;
			switch(temp)
			{
				case 0xd7:			//1101 0111  
					lcd_write_data(lcd[8]);
					mima[m]=lcd[8];
					m++;
					break;
				case 0xdb:					//1101 1011
					lcd_write_data(lcd[9]);
					mima[m]=lcd[9];
					m++;
					break;
				case 0xdd:					//1101 1101
					lcd_write_data(lcd[10]);
					mima[m]=lcd[10];
					m++;
					break;
				case 0xde:					//1101 1110
					lcd_write_data(lcd[11]);
					mima[m]=lcd[11];
					m++;
					break;
			}
			while(temp!=0x0f)
			{
				temp=P1;
				temp=temp&0x0f;
			}
		}
	}	
	P1=0xef;		//1110 1111
	temp=P1;
	temp=temp&0x0f;
	if(temp!=0x0f)
	{
		delayms(10);
		temp=P1;
		temp=temp&0x0f;
		if(temp!=0x0f)
		{
			temp=P1;
			switch(temp)
			{
				case 0xe7:			//1110 0111  
					lcd_write_data(lcd[12]);
					mima[m]=lcd[12];
					m++;
					break;
				case 0xeb:					//1110 1011
					lcd_write_data(lcd[13]);
					mima[m]=lcd[13];
					m++;
					break;
				case 0xed:					//1110 1101
					lcd_write_data(lcd[14]);
					mima[m]=lcd[14];
					m++;
					break;
				case 0xee:					//1110 1110
					lcd_write_data(lcd[15]);
					mima[m]=lcd[15];
					m++;
					break;
			}
			while(temp!=0x0f)
			{
				temp=P1;
				temp=temp&0x0f;
			}
		}
	}
}

void keyscan()
{
	if(add==0)
	{
		delayms(10);	//延时消抖
		if(add==0)
		{
			jd++;
			count=0;
			if(jd==6)
			{
				jd=5;
			}
			while(add==0);
		}
	}
	if(del==0)
	{
		delayms(10);
		if(del==0)
		{
			jd--;
			count=0;
			if(jd==0)
			{
				jd=1;
			}
			while(del==0);
		}
	}
}

void main()
{
	lcd_init();
	jd=1;	//初始化0.5ms,为0度
	count=0;
	Time0_Init();
	lcd_write_com(0x01);
	lcd_write_com(0x80);
	for(a=0;a<5;a++)
	{
		lcd_write_data(pass[a]);
	}
	lcd_write_com(0x80+0x0a);
	for(b=0;b<3;b++)
	{
		lcd_write_data(ag[b]);
	}
	lcd_write_com(0x80+0x40);
	while(1)
	{
		keydown();
		EX0=1;
		EX1=1;
	}
}

void keyscan_1() interrupt 0				//外部中断0
{
		keyscan();
		switch(jd)
		{
			case 1:
				lcd_write_com(0x80+0x0d);
				for(c=0;c<4;c++)
				{
					lcd_write_data(ag1[c]);
				}
				break;
		case 2:
				lcd_write_com(0x80+0x0d);
				for(c=0;c<4;c++)
				{
					lcd_write_data(ag2[c]);
				}
				break;
		case 3:
				lcd_write_com(0x80+0x0d);
				for(c=0;c<4;c++)
				{
					lcd_write_data(ag3[c]);
				}
				break;
		case 4:
				lcd_write_com(0x80+0x0d);
				for(c=0;c<4;c++)
				{
					lcd_write_data(ag4[c]);
				}
				break;
		case 5:
				lcd_write_com(0x80+0x0d);
				for(c=0;c<4;c++)
				{
					lcd_write_data(ag5[c]);
				}
				break;
		}
		EX0=0;	//外部中断0中断允许位
}

void Time0_Int() interrupt 1		//定时器0
{
	TH0=(65536-459)/256;
	TL0=(65536-459)%256;
	if(count<jd)		//jd:0.5ms定时器运行jd次
	{
		PWM=1;
	}
	else
	{
		PWM=0;
	}
	count+=1;
	count=count%40;	//20ms为1周期，0.5ms为1定时器溢出
}

void keyscan_2() interrupt 2			//外部中断1
{
		keyscan();
		switch(jd)
		{
			case 1:
				lcd_write_com(0x80+0x0d);
				for(c=0;c<4;c++)
				{
					lcd_write_data(ag1[c]);
				}
				break;
		case 2:
				lcd_write_com(0x80+0x0d);
				for(c=0;c<4;c++)
				{
					lcd_write_data(ag2[c]);
				}
				break;
		case 3:
				lcd_write_com(0x80+0x0d);
				for(c=0;c<4;c++)
				{
					lcd_write_data(ag3[c]);
				}
				break;
		case 4:
				lcd_write_com(0x80+0x0d);
				for(c=0;c<4;c++)
				{
					lcd_write_data(ag4[c]);
				}
				break;
		case 5:
				lcd_write_com(0x80+0x0d);
				for(c=0;c<4;c++)
				{
					lcd_write_data(ag5[c]);
				}
				break;
		}
		EX1=0;
}



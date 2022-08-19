#include <reg52.h>
unsigned char count;		//0.5ms������ǩ
sbit PWM=P2^0;
sbit sub=P3^1;
sbit paiduan=P3^0;
sbit add=P3^2;
sbit del=P3^3;
sbit LSA=P2^2;
sbit LSB=P2^3;
sbit LSC=P2^4;
sbit lcden=P2^7;	//LCDʹ��
sbit lcdrs=P2^6;		//LCD����/����ѡ���
sbit lcdwr=P2^5;		//LCD��дѡ���
int a,b,c;
unsigned char jd;	//�Ƕȱ�ʶ��
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
void delayms(unsigned char i)		//��ʱ
{
	unsigned char j,k;
	for(j=i;j>0;j--)
		for(k=110;k>0;k--);
}

//*************************************************************//
//lcd��ʾ
void lcd_write_com(unsigned char com)		//LCDд�����
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

void lcd_write_data(unsigned char date)		//LCDд���ݺ���
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

void lcd_init()		//LCD��ʼ������
{
	lcden=0;
	lcd_write_com(0x38);		//��ʾģʽ����Ϊ16X2��ʾ��5X7����8λ���ݽӿ�
	lcd_write_com(0x0c);		//���ò���ʾ����겻��˸
	lcd_write_com(0x06);		//дһ���ַ����ַ��1.�������ƶ�
	lcd_write_com(0x01);		//��ʾ��0������ָ����0
}

void Time0_Init()
{
	TMOD=0X01;		//���ö�ʱ��������ʽ1
	TH0=(65536-459)/256;		//���ö�ʱ����ÿ��0.5ms���
	TL0=(65536-459)%256;		//TH0=(65536-N)/256,N=tus/1.09us
	EA=1;		//��ȫ���ж�
	EX0=1;	//�ⲿ�ж�0�ж�����λ
	IT0=1;	//�����ⲿ�жϴ�����ʽ
					//0	--	�͵�ƽ����
					//1	--	�����䴥��
	EX1=1;	//�ⲿ�ж�1�ж�����λ
	IT1=1;	//�����ⲿ�жϴ�����ʽ
					//0	--	�͵�ƽ����
					//1	--	�����䴥��
	ET0=1;		//��ʱ��0�ж�����λ
	TR0=1;		//��ʱ��0���п���λ���������0�رն�ʱ��0����1������ʱ��0
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
	P1=0x7f;	//0111 1111,����һ��P1^7����
	temp=P1;
	temp=temp&0x0f;			//�ж����Ƿ��б�����
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
		delayms(10);	//��ʱ����
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
	jd=1;	//��ʼ��0.5ms,Ϊ0��
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

void keyscan_1() interrupt 0				//�ⲿ�ж�0
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
		EX0=0;	//�ⲿ�ж�0�ж�����λ
}

void Time0_Int() interrupt 1		//��ʱ��0
{
	TH0=(65536-459)/256;
	TL0=(65536-459)%256;
	if(count<jd)		//jd:0.5ms��ʱ������jd��
	{
		PWM=1;
	}
	else
	{
		PWM=0;
	}
	count+=1;
	count=count%40;	//20msΪ1���ڣ�0.5msΪ1��ʱ�����
}

void keyscan_2() interrupt 2			//�ⲿ�ж�1
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



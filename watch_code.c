#include <reg52.h>
#include "ds1302.h"
#include "key.h"
#include "nongli.h"

#define uchar unsigned char
#define uint unsigned int

bit WATCH_TIME=1;	//�������ʾʱ���־λ��Ĭ���������ʾʱ��
bit WATCH_DATE=0;	//�������ʾ���ڱ�־λ
bit WATCH_NONG=0;
bit WATCH_CLK=0;
bit SET_TIME=0;	    //�������ʾ��ʱʱ���ڵ�ʱ��
bit SET_DATE=0;		//�������ʾ�ڵ�����
bit SET_CLOK=0;		//�������ʾ�ڵ����ӵ���ֵ
uchar CLK_NUM=0;

sbit ZN_1=P2^0;
sbit ZN_2=P2^1;
sbit ZN_3=P2^2;
sbit ZN_4=P2^3;
sbit beef=P2^4;

bit CLK1=0;
bit CLK2=0;
bit CLK3=0;
bit CLK4=0;
bit CLK5=0;

uchar DIS_NUM=18;

//uchar tab_date[14]="2012-11-06 TUE";//��ʾ����������
//uchar tab_time[8]="00:00:00";//��ʾʱ����
uchar tab_t[6],tab_d[6],date[7],time[6];	 //������ʾ����ʱ�䡢���ӡ�����ʱ��ʾ��Ӧ����


uchar tab_n[5][6]={
{1,9,1,1,0,1},
{2,0,2,2,0,2},
{2,1,3,3,0,3},
{2,2,4,4,0,4},
{2,3,5,5,0,5}
};  

uchar clkh=24,clkm=0;
//���������Ϣ
uchar tiao,second;//��ʱ����������ʱ����Ӧ��ʱ��������
uchar code tabdu[]={0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x6f};/*��������*/
//-----------
uchar code tabdu_new[]={0xbf,0x86,0xdb,0xcf,0xe6,0xed,0xfd,0x87,0xff,0xef};/*���������ӵ�*/
uchar code tabwe[]={0xfB,0xf7,0xef,0xdf,0xbf,0x7f};
uchar keyval,tiao=0;
uchar hour,min,sec;
uchar year,month,day,week;
//uchar clkh,clkm;
uchar tab_z[5][2]={
{0,0},
{0,0},
{0.0},
{0.0},
{0.0}
};
uchar code monthday[]={31,28,31,30,31,30,31,31,30,31,30,31};

void refresh_weekday();
void calc_nong();
void delay(uint t)
{
	uint x,y;
	for(x=0;x<t;x++)
		for(y=0;y<40;y++);
}
//------------------------------------------
//              ����ת��
//------------------------------------------
void trans()
{
	 
	 date[0]=tab_1302[4]/10;     //��ʮλ
	 date[1]=tab_1302[4]%10;     //�¸�λ
	 date[2]=tab_1302[3]/10;     //��ʮλ
	 date[3]=tab_1302[3]%10;     //�ո�λ
	 date[4]=tab_1302[6]/10;     //��ʮλ
	 date[5]=tab_1302[6]%10;     //���λ
	
	 time[0]=tab_1302[2]/10;     //Сʱʮλ
	 time[1]=tab_1302[2]%10;     //Сʱ��λ
	 time[2]=tab_1302[1]/10;     //��ʮλ
	 time[3]=tab_1302[1]%10;     //�ָ�λ
	 time[4]=tab_1302[0]/10;     //��ʮλ
	 time[5]=tab_1302[0]%10;     //���λ
}
void Getval(uchar hour1,uchar min1,uchar sec1)
{
	tab_t[0]=hour1/10;
	tab_t[1]=hour1%10;
	tab_t[2]=min1/10;
	tab_t[3]=min1%10;
	tab_t[4]=sec1/10;
	tab_t[5]=sec1%10;
}
void Getdate(uchar yearl,uchar month1,uchar day1)
{
	tab_d[4]=yearl/10;
	tab_d[5]=yearl%10;
	tab_d[0]=month1/10;
	tab_d[1]=month1%10;
	tab_d[2]=day1/10;
	tab_d[3]=day1%10;
}

void Getclk()
{
	tab_n[CLK_NUM][0]=clkh/10;
	tab_n[CLK_NUM][1]=clkh%10;
	tab_n[CLK_NUM][2]=clkm/10;
	tab_n[CLK_NUM][3]=clkm%10;
}

void dsptime()	  //��ʾʵʱʱ��
{
	uchar i;
	for(i=0;i<6;i++)
	{
		P3=tabwe[i];
		if(i!=1)
		{
			if((i==0||i==2||i==4)&&time[i]==0)
				P0=0x00;
			else 
				P0=tabdu[time[i]];
		}
		if(i==1)
			if(second<25)
			{	
				P3=tabwe[i];
				P0=tabdu_new[time[i]];
									
			}				
			else
			{
				P3=tabwe[i];
				P0=tabdu[time[i]];
						
			}
		delay(DIS_NUM);    
		
	}
}

void dspdate()	//��ʾ����
{	
	uchar i;
	for(i=0;i<5;i++)	
	{		
		if(i<4)
		{	
			P3=tabwe[i];
			if(i==1)
				P0=tabdu_new[date[i]];
			if(i!=1)
				if((i==0||i==2)&&date[i]==0)
					P0=0x00;
				else 
					P0=tabdu[date[i]];
		}
		else
		{
			if(date[6]==0)       //����λ��ʾ������Ϣ1-6��8
			{	
				P3=tabwe[i+1];
				P0=tabdu[8];
			}
			else
			{
				P3=tabwe[i+1];
				P0=tabdu[date[6]];
			}
		}
		delay(DIS_NUM);
	}	
}



void duibi()
{
	uchar i;
	for(i=0;i<5;i++)
	{
		tab_z[i][0]=tab_n[i][0]*10+tab_n[i][1];
		tab_z[i][1]=tab_n[i][2]*10+tab_n[i][3];
	}
}

void dspnong()	//��ʾũ��
{	
	uchar i;
	for(i=0;i<4;i++)	
	{				
		P3=tabwe[i];
		P0=tabdu[datenong[i]];
	
		if(i==1)
			P0=tabdu_new[datenong[i]];
		if(i!=1)
			if((i==0||i==2)&&datenong[i]==0)
				P0=0x00;
			else 
				P0=tabdu[datenong[i]];
	
		delay(DIS_NUM);
	}	
}

void setdisp(uchar tab_set[])  //�������ʾ��ǰ������Ӧ����ֵ������
{
	uchar i;
	for(i=0;i<6;i++)
	{	
		switch(tiao)
		{
			case 0:if(i==0||i==1)
					{
						if(second<25)
						{	
							P3=tabwe[i];
							P0=tabdu[tab_set[i]];
									
						}
					}
					else
					{
						P3=tabwe[i];
						P0=tabdu[tab_set[i]];
						
					}break;
			case 1:if(i==2||i==3)
					{
						if(second<25)
						{
						 	P3=tabwe[i];
							P0=tabdu[tab_set[i]];					
						}
					}
					else
					{	
						P3=tabwe[i];
						P0=tabdu[tab_set[i]];						
					}break;
			case 2:if(i==4||i==5)
					{
						if(second<25)
						{	
							P3=tabwe[i];
							P0=tabdu[tab_set[i]];
									
						}
					}
					else
					{	
						P3=tabwe[i];
						P0=tabdu[tab_set[i]];
						
					}break;
		}
		delay(DIS_NUM);
	}
}
void setdisp_for_clk()  //�������ʾ��ǰ������Ӧ����ֵ������------���ӵ�
{
	uchar i;
	for(i=0;i<6;i++)
	{	
		switch(tiao)
		{
			case 0:if(i==0||i==1)
					{
						if(second<25)
						{	
							P3=tabwe[i];
							P0=tabdu[tab_n[CLK_NUM][i]];
									
						}
					}
					else
					{
						P3=tabwe[i];
						P0=tabdu[tab_n[CLK_NUM][i]];
						
					}break;
			case 1:if(i==2||i==3)
					{
						if(second<25)
						{
						 	P3=tabwe[i];
							P0=tabdu[tab_n[CLK_NUM][i]];					
						}
					}
					else
					{	
						P3=tabwe[i];
						P0=tabdu[tab_n[CLK_NUM][i]];						
					}break;
			case 2:if(i==4||i==5)
					{
						if(second<25)
						{
						 	P3=tabwe[i];
							if(i==4)
								if(tab_n[CLK_NUM][i]==1)
									P0=0x77;
								else
									P0=tabdu[tab_n[CLK_NUM][i]];
							else		
								P0=tabdu[tab_n[CLK_NUM][i]];					
						}
					}
					else
					{	
						P3=tabwe[i];
						if(i==4)
							if(tab_n[CLK_NUM][i]==1)
								P0=0x77;
							else
								P0=tabdu[tab_n[CLK_NUM][i]];
						else		
							P0=tabdu[tab_n[CLK_NUM][i]];							
					}break;
		}
		delay(DIS_NUM);
	}
}

void dspclk(uchar loc,uchar tab_clk[][6])	//��ʾ����
{	
	void setdisp_for_clk_come();
	if((tab_clk[loc][4]==1&&loc==0&&ZN_1==0||loc==1&&ZN_2==0||loc==2&&ZN_3==0||loc==3&&ZN_4==0))
		setdisp_for_clk_come();
	else 
	{
		uchar i;
		for(i=0;i<6;i++)	
		{		

			P3=tabwe[i];
			if(i==4)
				if(tab_clk[CLK_NUM][i]==1)
				{
				//	if(loc==0&&ZN_1==0||loc==1&&ZN_2==0||loc==2&&ZN_3==0||loc==3&&ZN_4==0)
					//	P0=0x79;
				
				//	else
						P0=0x77;
				}			
				else
					P0=tabdu[0];
			if(i==0||i==3)
				P0=tabdu_new[tab_clk[CLK_NUM][i]];

			if(i==1||i==2||i==5)
				P0=tabdu[tab_clk[CLK_NUM][i]];
			delay(DIS_NUM);
		}	
	}
}

void setdisp_for_clk_come()  //�������ʾ��ǰ������Ӧ����ֵ������------���ӵĵ�ʱ
{
	uchar i;
	for(i=0;i<6;i++)
	{	
		if(second<25)
		{
			if(i!=4)
			{	
				P3=tabwe[i];
				P0=tabdu[tab_n[CLK_NUM][i]];	
			}
			else
			{
				P3=tabwe[i];
				P0=0x79;								
			}
		}
		else
		{
			if(i!=4&&i!=5)
			{
				P3=tabwe[i];
				//P0=tabdu[tab_n[CLK_NUM][i]];
				P0=0x00;
			}
			if(i==4)
			{
				P3=tabwe[i];	
				P0=0x79;		
			}
			if(i==5)
			{
				P3=tabwe[i];
				P0=tabdu[tab_n[CLK_NUM][i]];
			}
		}
		delay(DIS_NUM);
	}
}
void settime()	 //����ʱ��
{
	SET_TIME=1;
	SET_DATE=0;
	WATCH_TIME=0;
	hour=time[0]*10+time[1];
	min=time[2]*10+time[3];
	sec=time[4]*10+time[5];
	//year=date[4]*10+date[5];
	//month=date[0]*10+date[1];
	//day=date[2]*10+date[3];
	while(1)
	{
		keyval=Getkey();
		switch(keyval)
		{
			case KEY_MU:tiao++;tiao=tiao%3;break;	//��ʱ
			case KEY_UP:if(tiao==0){hour++;if(hour==24) hour=0;}
						if(tiao==1){min++;if(min==60) min=0;}
						if(tiao==2){sec++;if(sec==60) sec=0;};
						break;
			case KEY_DN:if(tiao==0){hour--;if(hour==255) hour=23;}
						if(tiao==1){min--;if(min==255) min=59;}
						if(tiao==2){sec--;if(sec==255) sec=59;};
						break;
			case KEY_OK:tab_1302[2]=hour;tab_1302[1]=min;tab_1302[0]=sec;
						//tab_1302[6]=year;tab_1302[4]=month;tab_1302[3]=day;
						ds1302_init();
						SET_TIME=0;WATCH_TIME=1;tiao=0;return;break;
			case KEY_BK:SET_TIME=0;WATCH_TIME=1;tiao=0;return;
			
		}	
	}
}



uchar month_n_day(uchar m)  //���ĳ��ĳ���ж�����
{
	year=2000+date[4]*10+date[5];
	month=date[0]*10+date[1];
	if(month==2)
		return(((year%4 == 0) && (year%100 != 0) || (year%400 == 0))? 29: 28);
	else
		return(monthday[m-1]);
}

void refresh_weekday()    //����ĳ��ĳ��ĳ�������ڼ�(1900�꿪ʼ)
{
	long nday=0;
    uint i;
	year=date[4]*10+date[5];
	month=date[0]*10+date[1];
	day=date[2]*10+date[3];
	for(i=1900;i<2000+year;i++)
		nday =nday+(((i%4 == 0) && (i%100 != 0) || (i%400 == 0))? 366: 365);
    for(i=1;i<month;i++)
    	nday=nday+month_n_day(i);
    nday=nday+day;
    date[6] = nday%7;
}




void calc_nong()
{

	year=date[4]*10+date[5];
	month=date[0]*10+date[1];
	day=date[2]*10+date[3];
	nongli(year,month,day);
	
}
void setdate()	 //��������
{
	uchar keyval;
	SET_TIME=0;
	SET_DATE=1;
	WATCH_DATE=0;
	WATCH_TIME=0;
	year=date[4]*10+date[5];
	month=date[0]*10+date[1];
	day=date[2]*10+date[3];
	week=date[6];
	while(1)
	{
		keyval=Getkey();
		switch(keyval)
		{
			case KEY_MU:tiao++;tiao=tiao%3;break;
			case KEY_UP:if(tiao==2){year++;if(year==100) year=0;}
						if(tiao==0){month++;if(month==13) month=0;}
						if(tiao==1){day++;if(day==32) day=0;};
						break;
			case KEY_DN:if(tiao==2){year--;if(year==255) year=99;}
						if(tiao==0){month--;if(month==0) month=12;}
						if(tiao==1){day--;if(day==0) day=31;};
						break;
			case KEY_OK:tab_1302[6]=year;tab_1302[4]=month;tab_1302[3]=day;
						ds1302_init();SET_DATE=0;WATCH_DATE=1;SET_TIME=0;tiao=2;return;
						break;
			case KEY_BK:SET_DATE=0;WATCH_DATE=1;SET_TIME=0;tiao=0;return;
			
		}	
	}	
}
void setclok()	//��������
{	
	
	clkh=tab_n[CLK_NUM][0]*10+tab_n[CLK_NUM][1];
	clkm=tab_n[CLK_NUM][2]*10+tab_n[CLK_NUM][3];
	SET_CLOK=1;
	tiao=0;
	while(1)
	{
		keyval=Getkey();
		switch(keyval)
		{
			case KEY_MU:tiao++;tiao=tiao%2;break;
			case KEY_UP:if(tiao==0){clkh++;if(clkh>=24) clkh=0;}
						if(tiao==1){clkm++;if(clkm==60) clkm=0;};
						break;
			case KEY_DN:if(tiao==0){clkh--;if(clkh==255) clkh=23;}
						if(tiao==1){clkm--;if(clkm==255) clkm=59;};
						break;
			case KEY_OK:SET_CLOK=0;WATCH_CLK=1;tiao=0;tab_n[CLK_NUM][4]=1;return;break;
			case KEY_BK:clkh=24;clkm=0;SET_CLOK=0;tab_n[CLK_NUM][4]=0;WATCH_CLK=1;tiao=0;return;
			
		}
	}
}
void smg()	
{
	if(WATCH_TIME) 
		dsptime(); 	//��ʾʱ��
	else if(WATCH_DATE)	
		dspdate();	//��ʾ����
	else if(WATCH_NONG)
		dspnong();
	else if(WATCH_CLK)
		dspclk(CLK_NUM,tab_n);
	else if(SET_TIME)	
	{	
		Getval(hour,min,sec);
		setdisp(tab_t);	//��ʾ�ڵ�ʱ��
	}
	else if(SET_DATE)
	{
		Getdate(year,month,day);
		setdisp(tab_d);	//��ʾ�ڵ�����
	}
	else if(SET_CLOK)
	{	
		Getclk();
		setdisp_for_clk();	//��ʾ�ڵ����� 
	}  
}
void dealkey(uchar key)
{
	switch(key)
	{
		case KEY_MU:if(WATCH_DATE) 
						setdate();
					if(WATCH_TIME)
						settime();
					if(WATCH_CLK)
					{
						WATCH_TIME=0;WATCH_DATE=0;WATCH_NONG=0;WATCH_CLK=0;
						setclok();
					}
					break;
		case KEY_CK:
			if(WATCH_TIME||WATCH_CLK)
			{
				if(WATCH_TIME)
						CLK_NUM=0;
				WATCH_TIME=~WATCH_TIME;WATCH_CLK=~WATCH_CLK;
			}
			else if(WATCH_DATE||WATCH_CLK)
			{
				if(WATCH_DATE)
						CLK_NUM=0;
				WATCH_DATE=~WATCH_DATE;WATCH_CLK=~WATCH_CLK;
			}
			else if(WATCH_NONG||WATCH_CLK)
			{
				if(WATCH_NONG)
						CLK_NUM=0;
				WATCH_NONG=~WATCH_NONG;WATCH_CLK=~WATCH_CLK;
			}
			if(~ZN_1)
				CLK_NUM=0;
			if(~ZN_2)
				CLK_NUM=1;
			if(~ZN_3)
				CLK_NUM=2;
			if(~ZN_4)
				CLK_NUM=3;
			if(~beef)
			{
				beef=1;
				tab_n[4][4]=0;
				CLK_NUM=4;	
			}

			break;
		case KEY_TD:
			refresh_weekday();
			if(WATCH_TIME||WATCH_DATE)
			{	
				WATCH_TIME=~WATCH_TIME;WATCH_DATE=~WATCH_DATE;
			}
			else if(WATCH_NONG||WATCH_DATE)
			{
				WATCH_NONG=~WATCH_NONG;WATCH_DATE=~WATCH_DATE;
			}
			else if(WATCH_CLK||WATCH_DATE)
			{
				WATCH_CLK=~WATCH_CLK;WATCH_DATE=~WATCH_DATE;
			}
			break;//ʱ�������ڵ��л�
		case KEY_CC:	
			calc_nong();
			if(WATCH_TIME||WATCH_NONG)
			{	
				WATCH_TIME=~WATCH_TIME;WATCH_NONG=~WATCH_NONG;
			}
			else if(WATCH_DATE||WATCH_NONG)
			{
				WATCH_DATE=~WATCH_DATE;WATCH_NONG=~WATCH_NONG;
			}
			else if(WATCH_CLK||WATCH_NONG)
			{
				WATCH_CLK=~WATCH_CLK;WATCH_NONG=~WATCH_NONG;
			}		
			break;//ʱ����ũ�����л�

		case KEY_BK:
			if(WATCH_CLK)
			{
				tab_n[CLK_NUM][4]=0;
				if(~ZN_1&&CLK1)
				{
					ZN_1=1;
					CLK1=0;
				}
				if(~ZN_2&&CLK2)
				{
					ZN_2=1;
					CLK2=0;
				}
				if(~ZN_3&&CLK3)
				{
					ZN_3=1;
					CLK3=0;
				}
				if(~ZN_4&&CLK4)
				{
					ZN_4=1;
					CLK4=0;
				}
				if(~beef&&CLK5)
				{
					beef=1;
					CLK5=0;
				}
			}
			break;
		case KEY_OK:
			if(WATCH_CLK)
				if(tab_n[CLK_NUM][4]==0)
					tab_n[CLK_NUM][4]=1;
				else
					tab_n[CLK_NUM][4]=0;
			break;
			
		case KEY_UP:
			if(WATCH_CLK)
			{
				CLK_NUM++;
				if(CLK_NUM==5)
					CLK_NUM=0;
			}
			if(WATCH_TIME)
				DIS_NUM=18;
			break;
		case KEY_DN:
			if(WATCH_CLK)
			{
				if(CLK_NUM==0)
					CLK_NUM=5;
				CLK_NUM--;
			}
			if(WATCH_TIME)
				DIS_NUM=5;
			break;
	}
}

void main()
{
	uchar num;
	TMOD=0X01; //T0������ʽ1
	TH0=(65536-10000)/256;
	TL0=(65536-10000)%256;	 //10�����ж�һ��
	EA=1;
	ET0=1;
	TR0=1;	  
//	ds1302_init();
	P3=0xff;
	ZN_1=1;
	ZN_2=1;
	ZN_3=1;
	ZN_4=1;
	while(1)
	{
		duibi();
		num=Getkey();//�ж��Ƿ��м�����
		if(0!=num)
			dealkey(num);		
	}
}
void T0_SEVR() interrupt 1
{
	TH0=(65536-40000)/256;
	TL0=(65536-40000)%256;
	second++;
	if(second==50)
		second=0;
	ds1302();	//��ȡds1302�е�����
    trans();	//����ȡ����ת��
	smg();
	/*
	if(tab_1302[0]==0&&tab_1302[1]==0)
	{
		beef=0;
		delay(10);
		beef=1;
		delay(10);
	}
	if(tab_1302[0]!=0||tab_1302[1]!=0)
		beef=1;

	*/
	if(tab_n[0][4]&&tab_z[0][0]==tab_1302[2]&&tab_z[0][1]==tab_1302[1])   //����һ
	{
		ZN_1=0;
		CLK1=1;
	}
	if(tab_n[1][4]&&tab_z[1][0]==tab_1302[2]&&tab_z[1][1]==tab_1302[1])   //���Ӷ�
		{
		ZN_2=0;
		CLK2=1;
	}
	if(tab_n[2][4]&&tab_z[2][0]==tab_1302[2]&&tab_z[2][1]==tab_1302[1])   //������
	{
		ZN_3=0;
		CLK3=1;
	}
	if(tab_n[3][4]&&tab_z[3][0]==tab_1302[2]&&tab_z[3][1]==tab_1302[1])   //������
	{
		ZN_4=0;
		CLK4=1;
	}
	if(tab_n[4][4]&&tab_z[4][0]==tab_1302[2]&&tab_z[4][1]==tab_1302[1])   //������
	{
		beef=0;
		CLK5=1;
	}
}



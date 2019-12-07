#ifndef __KEY_H__
#define __KEY_H__

#define KEY_CK 0x01	  //设置闹钟
#define KEY_UP 0x02	  //加
#define KEY_DN 0x03	  //减
#define KEY_BK 0x04	  //取消键
#define KEY_OK 0x05	  //确认键
#define KEY_MU 0x06   //设置时间及选择时分秒
#define KEY_TD 0x07	  //切换时间和日期
#define KEY_CC 0x08   //选择闹钟

void keydelay(unsigned int t)
{
	unsigned int x,y;
	for(x=t;x>0;x--)
		for(y=110;y>0;y--);
}
unsigned char Getkey()
{
	unsigned char temp;
	P1=0xff;
	keydelay(3);
	temp=P1;
	if(temp!=0xff)
	{
		keydelay(6);
		temp=P1;
		if(temp!=0xff)
		{
			while(P1!=0xff);
			switch(temp)
			{
				case 0xfe:return KEY_CK;break; //P1^0
				case 0xfd:return KEY_UP;break; //1
				case 0xfb:return KEY_DN;break; //2
				case 0xf7:return KEY_BK;break; //3
				case 0xef:return KEY_OK;break; //4
				case 0xdf:return KEY_MU;break; //5
				case 0xbf:return KEY_TD;break; //6
				case 0x7f:return KEY_CC;break; //P1^7
				default:break;
			}
		}
	}
	return 0;
}
#endif

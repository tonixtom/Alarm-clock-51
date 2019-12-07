#ifndef __KEY_H__
#define __KEY_H__

#define KEY_CK 0x01	  //��������
#define KEY_UP 0x02	  //��
#define KEY_DN 0x03	  //��
#define KEY_BK 0x04	  //ȡ����
#define KEY_OK 0x05	  //ȷ�ϼ�
#define KEY_MU 0x06   //����ʱ�估ѡ��ʱ����
#define KEY_TD 0x07	  //�л�ʱ�������
#define KEY_CC 0x08   //ѡ������

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

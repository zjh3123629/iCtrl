#include "key.h"
#include "delay.h"
								    
//������ʼ������
void KEY_Init(void) //IO��ʼ��
{ 
 	GPIO_InitTypeDef GPIO_InitStructure;
	
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOC,ENABLE);//ʹ��PORTA,PORTC

	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_2;//PA3
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; //���ó���������
 	GPIO_Init(GPIOA, &GPIO_InitStructure);//��ʼ��GPIOA3

	
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; //���ó���������
	GPIO_Init(GPIOC, &GPIO_InitStructure);//��ʼ��GPIOC9

}
//����������
//���ذ���ֵ
//mode:0,��֧��������;1,֧��������;
//0��û���κΰ�������
//1��KEY0����
//2��KEY1����

u8 KEY_Scan(u8 mode)
{	 
	static u8 key_up=1;//�������ɿ���־
	if(mode)key_up=1;  //֧������		  
	if(key_up&&(KEYOK==0||KEYESC==0 || KEYCUT==0 || KEYADD==0))
	{
		delay_ms(10);//ȥ���� 
		key_up=0;
		if(KEYOK==0)return KEY_OK;
		else if(KEYESC==0)return KEY_ESC;
		else if(KEYCUT==0)return KEY_CUT;
		else if(KEYADD==0)return KEY_ADD;
		
	}else if(KEYOK==1&&KEYESC==1&&KEYCUT==1&&KEYADD==1)key_up=1; 	    
 	return 0;// �ް�������
}

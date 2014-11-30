#include "key.h"
#include "delay.h"
								    
//按键初始化函数
void KEY_Init(void) //IO初始化
{ 
 	GPIO_InitTypeDef GPIO_InitStructure;
	
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOC,ENABLE);//使能PORTA,PORTC

	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_2;//PA3
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; //设置成上拉输入
 	GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化GPIOA3

	
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; //设置成上拉输入
	GPIO_Init(GPIOC, &GPIO_InitStructure);//初始化GPIOC9

}
//按键处理函数
//返回按键值
//mode:0,不支持连续按;1,支持连续按;
//0，没有任何按键按下
//1，KEY0按下
//2，KEY1按下

u8 KEY_Scan(u8 mode)
{	 
	static u8 key_up=1;//按键按松开标志
	if(mode)key_up=1;  //支持连按		  
	if(key_up&&(KEYOK==0||KEYESC==0 || KEYCUT==0 || KEYADD==0))
	{
		delay_ms(10);//去抖动 
		key_up=0;
		if(KEYOK==0)return KEY_OK;
		else if(KEYESC==0)return KEY_ESC;
		else if(KEYCUT==0)return KEY_CUT;
		else if(KEYADD==0)return KEY_ADD;
		
	}else if(KEYOK==1&&KEYESC==1&&KEYCUT==1&&KEYADD==1)key_up=1; 	    
 	return 0;// 无按键按下
}

/******************** (C) COPYRIGHT 2012 WildFire Team ***************************
 * 文件名  ：led.c
 * 描述    ：led 应用函数库         
 * 实验平台：野火STM32开发板
 * 库版本  ：ST3.5.0
 * 作者    ：wildfire team 
 * 论坛    ：http://www.amobbs.com/forum-1008-1.html
 * 淘宝    ：http://firestm32.taobao.com
**********************************************************************************/
#include "led.h"

/*
 * 函数名：LED_GPIO_Config
 * 描述  ：配置LED用到的I/O口
 * 输入  ：无
 * 输出  ：无
 */
void LED_GPIO_Config(void)
{		
	/*定义一个GPIO_InitTypeDef类型的结构体*/
	GPIO_InitTypeDef GPIO_InitStructure;

	/*开启GPIOB的外设时钟*/
	RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOD, ENABLE); 
	RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOC, ENABLE); 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);

	/*选择要控制的GPIOC引脚*/															   
  	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;	

	/*设置引脚模式为通用推挽输出*/
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;   

	/*设置引脚速率为50MHz */   
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 

	/*调用库函数，初始化GPIOB*/
  	GPIO_Init(GPIOD, &GPIO_InitStructure);		  

	/* 关闭所有led灯	*/
	GPIO_SetBits(GPIOD, GPIO_Pin_2);	 
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
	/*调用库函数，初始化GPIOC*/
  GPIO_Init(GPIOC, &GPIO_InitStructure);		  

	/* 关闭蜂鸣器	*/
	GPIO_ResetBits(GPIOC, GPIO_Pin_6);		
		
}


/******************* (C) COPYRIGHT 2012 WildFire Team *****END OF FILE************/

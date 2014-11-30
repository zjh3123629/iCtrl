/******************** (C) COPYRIGHT 2012 WildFire Team ***************************
 * �ļ���  ��led.c
 * ����    ��led Ӧ�ú�����         
 * ʵ��ƽ̨��Ұ��STM32������
 * ��汾  ��ST3.5.0
 * ����    ��wildfire team 
 * ��̳    ��http://www.amobbs.com/forum-1008-1.html
 * �Ա�    ��http://firestm32.taobao.com
**********************************************************************************/
#include "led.h"

/*
 * ��������LED_GPIO_Config
 * ����  ������LED�õ���I/O��
 * ����  ����
 * ���  ����
 */
void LED_GPIO_Config(void)
{		
	/*����һ��GPIO_InitTypeDef���͵Ľṹ��*/
	GPIO_InitTypeDef GPIO_InitStructure;

	/*����GPIOB������ʱ��*/
	RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOD, ENABLE); 
	RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOC, ENABLE); 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);

	/*ѡ��Ҫ���Ƶ�GPIOC����*/															   
  	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;	

	/*��������ģʽΪͨ���������*/
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;   

	/*������������Ϊ50MHz */   
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 

	/*���ÿ⺯������ʼ��GPIOB*/
  	GPIO_Init(GPIOD, &GPIO_InitStructure);		  

	/* �ر�����led��	*/
	GPIO_SetBits(GPIOD, GPIO_Pin_2);	 
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
	/*���ÿ⺯������ʼ��GPIOC*/
  GPIO_Init(GPIOC, &GPIO_InitStructure);		  

	/* �رշ�����	*/
	GPIO_ResetBits(GPIOC, GPIO_Pin_6);		
		
}


/******************* (C) COPYRIGHT 2012 WildFire Team *****END OF FILE************/

#ifndef __KEY_H
#define __KEY_H	 

#include "stm32f10x.h"
 
#define KEYOK  GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_2)//��ȡ����0//ҡ���м�
#define KEYESC  GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_9)//��ȡ����1
#define KEYCUT  GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_8)//��ȡ����1
#define KEYADD  GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_7)//��ȡ����1

#define KEY_OK 		0x10
#define KEY_ESC 	0x20
#define KEY_CUT 	0x30
#define KEY_ADD 	0x40

void KEY_Init(void);//IO��ʼ��
u8 KEY_Scan(u8);  	//����ɨ�躯��					    
#endif

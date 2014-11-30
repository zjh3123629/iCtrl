#ifndef __KEY_H
#define __KEY_H	 

#include "stm32f10x.h"
 
#define KEYOK  GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_2)//读取按键0//摇杆中键
#define KEYESC  GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_9)//读取按键1
#define KEYCUT  GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_8)//读取按键1
#define KEYADD  GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_7)//读取按键1

#define KEY_OK 		0x10
#define KEY_ESC 	0x20
#define KEY_CUT 	0x30
#define KEY_ADD 	0x40

void KEY_Init(void);//IO初始化
u8 KEY_Scan(u8);  	//按键扫描函数					    
#endif

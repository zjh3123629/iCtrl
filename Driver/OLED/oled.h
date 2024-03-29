#ifndef __OLED_H
#define __OLED_H			  	 

    
#include "include.h"
//OLED模式设置
//0:4线串行模式
//1:并行8080模式
#define OLED_MODE 0

//CS-----GND
//RST----PB12
//RS-----PB13
//SCK----PB14
//SIN----PB15

//-----------------OLED端口定义----------------  					   



#define OLED_RST_Clr() GPIO_ResetBits(GPIOB,GPIO_Pin_13)
#define OLED_RST_Set() GPIO_SetBits(GPIOB,GPIO_Pin_13)//RST

#define OLED_RS_Clr() GPIO_ResetBits(GPIOB,GPIO_Pin_12)//DC
#define OLED_RS_Set() GPIO_SetBits(GPIOB,GPIO_Pin_12)


#define OLED_SCLK_Clr() GPIO_ResetBits(GPIOB,GPIO_Pin_11)//D0
#define OLED_SCLK_Set() GPIO_SetBits(GPIOB,GPIO_Pin_11)

#define OLED_SDIN_Clr() GPIO_ResetBits(GPIOB,GPIO_Pin_10)//D1
#define OLED_SDIN_Set() GPIO_SetBits(GPIOB,GPIO_Pin_10)

 		     
#define OLED_CMD  0	//写命令
#define OLED_DATA 1	//写数据
//OLED控制用函数
void OLED_WR_Byte(u8 dat,u8 cmd);	    
void OLED_Display_On(void);
void OLED_Display_Off(void);
void OLED_Refresh_Gram(void);		   
							   		    
void OLED_Init(void);
void OLED_Clear(void);
void OLED_DrawPoint(u8 x,u8 y,u8 t);
void OLED_Fill(u8 x1,u8 y1,u8 x2,u8 y2,u8 dot);
void OLED_ShowChar(u8 x,u8 y,u8 chr,u8 size,u8 mode);
void OLED_ShowNum(u8 x,u8 y,u32 num,u8 len,u8 size);
void OLED_ShowString(u8 x,u8 y,const u8 *p);	 
#endif  
	 




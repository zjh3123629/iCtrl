#include "include.h"
#include "app.h"
#include "ico_MENU.h"

uint16_t VirtAddVarTab[NumbOfVar] = {0xAA00, 0xAA01, 0xAA02};
extern int16_t yaw_fine;  
State Init_State;

void MCO_INIT(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8; 
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; 
  GPIO_Init(GPIOA, &GPIO_InitStructure);
	RCC_MCOConfig(RCC_MCO_HSE); 
}




void IAC_Init(void)
{
	 MCO_INIT();  //MCO初始化
	 delay_init(72); 
	 Nvic_Init();  //中断初始化
//   I2C_INIT();
	 KEY_Init();   //按键初始化
	 delay(0XFFF); 
	 ADC1_Init();      //AD初始化
	 GUI_Initialize(); //GUI初始化
	 NRF24L01_Init();  //2401接口初始化
	 Init_State.NRF2401_State= NRF24L01_Check(); //检测2401是否已连接
	 NRF24L01_Mode(2);  //2401 设置为发送模式
	 TIM3_Init(2500);   //定时器初始化 2.5MS
	 usart1_config();   //串口初始化
	 FLASH_Unlock();    //flash初始化
	 EE_Init();	 
	 EE_ReadVariable(VirtAddVarTab[0], &yaw_fine);
}




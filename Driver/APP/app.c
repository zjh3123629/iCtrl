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
	 MCO_INIT();  //MCO��ʼ��
	 delay_init(72); 
	 Nvic_Init();  //�жϳ�ʼ��
//   I2C_INIT();
	 KEY_Init();   //������ʼ��
	 delay(0XFFF); 
	 ADC1_Init();      //AD��ʼ��
	 GUI_Initialize(); //GUI��ʼ��
	 NRF24L01_Init();  //2401�ӿڳ�ʼ��
	 Init_State.NRF2401_State= NRF24L01_Check(); //���2401�Ƿ�������
	 NRF24L01_Mode(2);  //2401 ����Ϊ����ģʽ
	 TIM3_Init(2500);   //��ʱ����ʼ�� 2.5MS
	 usart1_config();   //���ڳ�ʼ��
	 FLASH_Unlock();    //flash��ʼ��
	 EE_Init();	 
	 EE_ReadVariable(VirtAddVarTab[0], &yaw_fine);
}




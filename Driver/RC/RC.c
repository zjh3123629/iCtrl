#include "RC.h"
#include "include.h"

#define BYTE0(dwTemp)       (*(char *)(&dwTemp))
#define BYTE1(dwTemp)       (*((char *)(&dwTemp) + 1))
#define BYTE2(dwTemp)       (*((char *)(&dwTemp) + 2))
#define BYTE3(dwTemp)       (*((char *)(&dwTemp) + 3))


RC_TRANSDATA RC_Data;
int airBatt_Buffer[100];
extern int16_t yaw_fine;  
extern u8 tumble_flag;

void RC_Sent(void)
{
	static u8 mode_change=0;
	static u8 p=0;
	int16_t  roll,pitch,yaw;
	
	if(ADC_ConvertedValue[4] >= 2348) 
		 roll = (ADC_ConvertedValue[4] - 2348) / 70 + (2048 - ADC_ConvertedValue[2]) / 400;
	else if(ADC_ConvertedValue[4] <= 1748)
		 roll = (ADC_ConvertedValue[4] - 1748) / 70 + (2048 - ADC_ConvertedValue[2]) / 400;
	else 	
		 roll = (2048 - ADC_ConvertedValue[2]) / 400;
	
	if(ADC_ConvertedValue[5] >= 2348) 
		 pitch = (ADC_ConvertedValue[5] - 2348) / 70 + (2048 - ADC_ConvertedValue[3]) / 400;
	else if(ADC_ConvertedValue[5] <= 1748)
		 pitch = (ADC_ConvertedValue[5] - 1748) / 70 + (2048 - ADC_ConvertedValue[3]) / 400;
	else 	
		 pitch = (2048 - ADC_ConvertedValue[3]) / 400;
		
	if(ADC_ConvertedValue[1] >= 2148)
     yaw = (ADC_ConvertedValue[1] -2148)/15 + yaw_fine;
	else if(ADC_ConvertedValue[1] <= 1948)
		 yaw = (ADC_ConvertedValue[1] - 1948) / 15 + yaw_fine;
	else 
		 yaw = yaw_fine;
	
  RC_Data.NRF24L01_TXDATA[0] = 0xAF;
	RC_Data.NRF24L01_TXDATA[1] = 0x0C;
	RC_Data.NRF24L01_TXDATA[2] = 0xfa;
	RC_Data.NRF24L01_TXDATA[3] = BYTE1(ADC_ConvertedValue[0]);
	RC_Data.NRF24L01_TXDATA[4] = BYTE0(ADC_ConvertedValue[0]);
	RC_Data.NRF24L01_TXDATA[5] = BYTE1(yaw);
	RC_Data.NRF24L01_TXDATA[6] = BYTE0(yaw);
	RC_Data.NRF24L01_TXDATA[7] = BYTE1(roll);
	RC_Data.NRF24L01_TXDATA[8] = BYTE0(roll);
	RC_Data.NRF24L01_TXDATA[9] = BYTE1(pitch);
	RC_Data.NRF24L01_TXDATA[10]= BYTE0(pitch);
	RC_Data.NRF24L01_TXDATA[11]= tumble_flag;
	
  mode_change++;
	
	switch(mode_change)
	{
		case 1:   
	  case 2:   NRF24L01_TxPacket(RC_Data.NRF24L01_TXDATA);  break;
		case 3:   
		{   
			RC_Data.NRF24L01_TXDATA[31]= 1;   //提前通知 飞机准备进入发送模式
			NRF24L01_TxPacket(RC_Data.NRF24L01_TXDATA); 
		} break;   
		case 4:   
		{ 
			RC_Data.NRF24L01_TXDATA[31]= 1;  //再次通知  飞机进入发送模式
			NRF24L01_TxPacket(RC_Data.NRF24L01_TXDATA); 
			NRF24L01_Mode(1);  //遥控进入接收模式
		}	break;
		case 5:
		case 6:  
		{		
			NRF24L01_RxPacket(RC_Data.NRF24L01_RXDATA);  //已切换到 接收模式 读取接收到的数据
		  if(RC_Data.NRF24L01_RXDATA[0] == 0x1A && RC_Data.NRF24L01_RXDATA[1] == 0x0C) //校验帧头
				
			airBatt_Buffer[p]= (RC_Data.NRF24L01_RXDATA[2]* 256 + RC_Data.NRF24L01_RXDATA[3]);
			p++;
			if(p >=100)  p=0;
		}	break; 
		
		case 7: 
		{
			mode_change = 0;    
			NRF24L01_RxPacket(RC_Data.NRF24L01_RXDATA);  //再次读取接收到的数据
		  NRF24L01_Mode(2);  //切换为发送模式
			RC_Data.NRF24L01_TXDATA[31]= 0;
			NRF24L01_TxPacket(RC_Data.NRF24L01_TXDATA);   //已切换到发送模式  接着发送数据
		} break;			
		default:  break;
	}
	
}

//发送矫正信号
void sentCorrect(u8 ent)
{
	if(ent)
	{
    RC_Data.NRF24L01_TXDATA[0] = 0xAF;
	  RC_Data.NRF24L01_TXDATA[1] = 0x0C;
	  RC_Data.NRF24L01_TXDATA[2] = 0xfb;
	  RC_Data.NRF24L01_TXDATA[3] = 0xfb;
	}
	else
	{
	  RC_Data.NRF24L01_TXDATA[0] = 0;
	  RC_Data.NRF24L01_TXDATA[1] = 0;
	  RC_Data.NRF24L01_TXDATA[2] = 0;
	  RC_Data.NRF24L01_TXDATA[3] = 0;
	}
	
	NRF24L01_TxPacket(RC_Data.NRF24L01_TXDATA);
}


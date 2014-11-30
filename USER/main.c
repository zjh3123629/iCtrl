
#include "include.h"



extern const uint8 gImage_logo[];
extern MY_MENU  MAIN_MENU[5];

int main(void)
{
	  SystemInit();   
		IAC_Init();   
		GUI_SetColor(1,0);	
		GUI_LoadPic(0,0,( uint8*)gImage_logo,128,64);  //OLED加载LOGO
		OLED_Refresh_Gram();  
	  EnTIM3();     //开定时中断
  	while(1)
	  {    		
				GUI_ICOMenuClass3(MAIN_MENU);
		}
}


 

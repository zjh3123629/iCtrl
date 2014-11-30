#include "ico_MENU.h"
#include "key.h"
#include "adc.h"
#include "oled.h"



static MENU_CLASS  menu_class[3];  // 
static uint8  menu_class_no;       // 

extern const uint8 gImage_bat[];
extern const uint8 ico_single[];
MENUICO  g_disp_buf; 

void  GUI_MenuIcoDrawA(MENUICO *ico, uint8 sel)
{	g_disp_buf.x = ico->x; 
	g_disp_buf.y = ico->y; 
	g_disp_buf.icodat = ico->icodat; 	
	g_disp_buf.title = ico->title; 
	g_disp_buf.state = sel; 
	GUI_MenuIcoDraw(&g_disp_buf);

	OLED_Refresh_Gram();
}


uint8_t GUI_ICOMenuSelect(MY_MENU sel_menu[], uint8 sel_no)
{
		uint8_t   key;
    uint8_t   i;
		unsigned int LR_ADval;
	
		//清除上级菜单，通过向所选矩形框填充指定颜色来清除菜单
		GUI_ClearSCR();
		
		//OLED_Refresh_Gram();
		for(i=0; i<3; i++)
		{
			if(sel_menu[i].mdata == NULL) break;
		
			if(i == sel_no)
			{	
					GUI_MenuIcoDrawA(sel_menu[i].mdata, 1);	//参数1代表选中此菜单项
			}
			else		
			{
					GUI_MenuIcoDrawA(sel_menu[i].mdata, 0);
			}	
	}

    while(1)
    {   
 				key = KEY_Scan(0);    
        LR_ADval=ADC_ConvertedValue[4];
				if(LR_ADval<650)//左移
				{
						delay_ms(10);
						if(sel_no != 0)
                {   
										GUI_MenuIcoDrawA(sel_menu[sel_no].mdata, 0);		   
                    sel_no--;
										if(sel_no==2)
										{
														GUI_ClearSCR();
														OLED_Refresh_Gram();
														
														GUI_MenuIcoDrawA(sel_menu[sel_no-1].mdata, 0); 
														GUI_MenuIcoDrawA(sel_menu[sel_no-2].mdata, 0); 
										}										
                    GUI_MenuIcoDrawA(sel_menu[sel_no].mdata, 1);  		
                }
								while(!(ADC_ConvertedValue[4]<2400));
				}
				else if(LR_ADval>3450)//右移	
				{
						delay_ms(10);
						if(sel_no < 5) 
             {   

										i = sel_no + 1;
                    if(sel_menu[i].mdata != NULL)  
                    {   
												GUI_MenuIcoDrawA(sel_menu[sel_no].mdata, 0);	                                 
                        sel_no++; 
												
												if(sel_no==3)
												{
														//GUI_RectangleFill(0,0,127,63,0x00);
														//GUI_FillSCR(0x00);   							// 清屏
														GUI_ClearSCR();
														OLED_Refresh_Gram();
														
														//GUI_MenuIcoDrawA(sel_menu[sel_no].mdata, 1); 
												}	
												GUI_MenuIcoDrawA(sel_menu[sel_no].mdata, 1); 												
                    }
              } 

								while(!(ADC_ConvertedValue[4]>1600));
				}
				
        switch(key) 
        {   
					  case KEY_OK:
                return(sel_menu[sel_no].ret_no);    //                            
            
            case KEY_ESC:
                return(RET_MAINMENU);   // 
        } // end of switch(key)        
    } // end of while(1)
}


void  GUI_ICOMenuClass3(MY_MENU  main_menu[])
{   
		uint8    menu_sel;  // 所选择的菜单返回值
    MY_MENU  *pmenu;    //菜单句柄指针
    

    /* 参数过滤 */
    if(main_menu==NULL) return;         	// 空菜单结构指针，直接返回
    if(main_menu[0].mdata==NULL) return;	// 第一个菜单无效，直接返回
    
    
    /* 初始化menu_class[0]和menu_class_no*/
    menu_class[0].menu_addr = main_menu;    //主菜单  
    menu_class[0].sel_no = 0;               // 默认选择菜单项
    menu_class_no = 0;                      //第一级菜单 
    
    
    while(1)
    {   /*菜单显示，选择菜单 */
        menu_sel = GUI_ICOMenuSelect( menu_class[menu_class_no].menu_addr,   // 菜单结构句柄,如MAIN_MENU
                                      menu_class[menu_class_no].sel_no );    // 默认选择菜单项(序号)
        
        /* 对菜单所选值进行处理 */
        switch(menu_sel)
        {  
						case RET_MAINMENU:  // 直接返回第一层菜单
                menu_class_no = 0; 
                break;
                
            case RET_UPMENU:    // 返回上一层菜单  //GUI_ICOMenuSelect没有返回RET_UPMENU
                if(menu_class_no==0)
                {  
									return;     // 若是在第一层菜单中返回,则GUI_ICOMenuClass3
                }
                else
                {   
									menu_class_no--;
                }                
                break;
                
            case 0:                                                     // 
            case 1:
            case 2:
            case 3:
            case 4:
            case 5:
                pmenu = menu_class[menu_class_no].menu_addr + menu_sel; // 
                if(pmenu->next_menu != NULL)                            // 
                {   if(menu_class_no<2)                                 // 
                    {   menu_class[menu_class_no].sel_no = menu_sel;    //
                    
                        menu_class_no++;                                //                              
                        menu_class[menu_class_no].menu_addr = pmenu->next_menu; //
                        menu_class[menu_class_no].sel_no = 0;           // 
												
                    }
                }
                else                       
                {   if(pmenu->mdata->Function != NULL) 
                    {   
												GUI_FillSCR(0x00);   							// 清屏 
                        (*pmenu->mdata->Function)();               		// 
                        menu_class[menu_class_no].sel_no = menu_sel;    //                    
                    }                   
                }                
                
            default:
                break;                    
        } // end of switch(menu_sel)        
    
    } // end of  while(1)
}

extern uint16_t VirtAddVarTab[NumbOfVar];

int airBatt;

int16_t yaw_fine;   //航向微调
u8 rcsentFlag = 0; //遥控发送标志位  用于 控制遥控是否发送数据
u8 tumble_flag=0;


void FUN_Fly(void)
{
		unsigned int Thro,roll,pitch,roll_fine,pitch_fine;    
		unsigned int bat_val,bat_val1;   //遥控盒回家电池电量
		unsigned char key=0,i=0;             //按键 
	  unsigned long sum=0;
	  static u8 chang_flag=0,tumbletim=0; 
	
		GUI_ClearSCR();
		GUI_LoadPic(0,0,( uint8*)gImage_bat,34,16);  //画遥控电池
	  GUI_LoadPic(93,0,( uint8*)gImage_bat,34,16); //画灰机电池
		GUI_LoadPic(58,0,( uint8*)ico_single,34,16); //画信号
		
		GUI_Rectangle(10, 16, 25, 62, 1);  //画油门框
	  GUI_Rectangle(30, 17, 75, 30, 1);  //画俯仰微调框
	  GUI_Rectangle(30, 33, 75, 46, 1);  //画横滚稳调框
	  GUI_Rectangle(30, 49, 75, 62, 1);  //画航向微调框
		GUI_Circle(104, 39, 23, 1);        //画圆
		OLED_Refresh_Gram();  
	  rcsentFlag = 1;
		while(1)
		{
				key=KEY_Scan(0);  //按键扫描
			  if(key==KEY_CUT && ADC_ConvertedValue[0] < 50)  
				{
				  yaw_fine--;
					chang_flag = 1;
				}
				else if(key==KEY_ADD && ADC_ConvertedValue[0] < 50)
				{
				  yaw_fine++;
					chang_flag=1;
				}
				else if(key==KEY_ESC)    //退出按键
				{
					  rcsentFlag = 0;     //遥控发送标志清零  遥控就不在发送数据
						return;
				}
				else if(key==KEY_OK)
				{
				    tumble_flag = 1;
				}
				if(tumble_flag)  tumbletim++;
				if(tumbletim > 5) 
        {
				   tumble_flag=0;
					 tumbletim=0;
				}
			  if(yaw_fine>20)  yaw_fine =20;            //航向微调限幅
				else if(yaw_fine < -20)  yaw_fine = -20;  
				
				if(chang_flag)  //航向进行了微调
				{
				    if(ADC_ConvertedValue[0]>100)   //推油门之后自动保存航向微调
				    {
						   EE_WriteVariable(VirtAddVarTab[0],yaw_fine);  //写入FLASH
							 chang_flag=0;
						}
				}
				
				
				GUI_RectangleFill(11, 17, 24, 62, 0);  //擦油门
				GUI_CircleFill(roll, pitch, 2, 0);     //擦摇杆位置
				GUI_RectangleFill(31, 18, 74, 29, 0);  //擦灰机电量显示
				GUI_RectangleFill(31, 34, 74, 45, 0);  //擦灰机电量显示
				GUI_RectangleFill(31, 50, 74, 61, 0);  //擦灰机电量显示
				GUI_RectangleFill(2, 4, 28, 12, 0);    //擦遥控电量显示
				GUI_RectangleFill(97, 4, 120, 12, 0);  //擦灰机电量显示
				
				
				Thro=61-ADC_ConvertedValue[0]/93;      //计算油门
				roll=120-(4096 - ADC_ConvertedValue[4])*0.008;
				pitch=23+(4096 - ADC_ConvertedValue[5])*0.008;
				roll_fine = 31+ (4096 - ADC_ConvertedValue[2])/102;
				pitch_fine = 31+ (4096 - ADC_ConvertedValue[3])/102;
				bat_val=(ADC_ConvertedValue[6]*4/30-249)/3;
				sum=0;
				for(i=0;i<100;i++)   sum += airBatt_Buffer[i];
				airBatt = sum/100;
				bat_val1 = ( airBatt*4/30-249)/3-4;
				if(bat_val1 > 34)  bat_val1=0;

				
				GUI_RectangleFill(11, Thro, 24, 62, 1);     //油门显示
				GUI_CircleFill(roll, pitch, 2, 1);          //右侧摇杆位置显示  
        GUI_RectangleFill(roll_fine, 18, roll_fine+3, 29, 1);   //画横滚微调滑块
				GUI_RectangleFill(pitch_fine, 34, pitch_fine+3, 45, 1); //画俯仰微调滑块
				GUI_RectangleFill(51+yaw_fine, 50, 54+yaw_fine, 61, 1); //画航向微调滑块
				GUI_RectangleFill(4, 4, bat_val, 12, 1);    //遥控电量显示
				GUI_RectangleFill(97, 4,97+bat_val1, 12, 1);//灰机电量显示

				OLED_Refresh_Gram();

		}
}

extern const unsigned char button_adjust[];
extern const unsigned char button_adjust1[];
void FUN_Debug(void)
{
		unsigned char key;
		GUI_ClearSCR();
		OLED_ShowString(4,0,"Please place   the plane level");

		GUI_LoadPic(80,40,( uint8*)button_adjust1,46,22);
		OLED_Refresh_Gram();
		while(1)
		{
				
				key=KEY_Scan(0);
				if(key==KEY_OK)//校正
				{
					OLED_ShowString(4,0,"The plane is   correctting,    please wait");
					OLED_Refresh_Gram();
					sentCorrect(1);//发送校正指令
					sentCorrect(1);//发送校正指令
					delay_ms(100);
					sentCorrect(0);
					delay_ms(3000);
          return;				
				}
				else if(key==KEY_ESC)
				{
						return;
				}
		}
}



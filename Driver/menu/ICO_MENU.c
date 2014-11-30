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
	
		//����ϼ��˵���ͨ������ѡ���ο����ָ����ɫ������˵�
		GUI_ClearSCR();
		
		//OLED_Refresh_Gram();
		for(i=0; i<3; i++)
		{
			if(sel_menu[i].mdata == NULL) break;
		
			if(i == sel_no)
			{	
					GUI_MenuIcoDrawA(sel_menu[i].mdata, 1);	//����1����ѡ�д˲˵���
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
				if(LR_ADval<650)//����
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
				else if(LR_ADval>3450)//����	
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
														//GUI_FillSCR(0x00);   							// ����
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
		uint8    menu_sel;  // ��ѡ��Ĳ˵�����ֵ
    MY_MENU  *pmenu;    //�˵����ָ��
    

    /* �������� */
    if(main_menu==NULL) return;         	// �ղ˵��ṹָ�룬ֱ�ӷ���
    if(main_menu[0].mdata==NULL) return;	// ��һ���˵���Ч��ֱ�ӷ���
    
    
    /* ��ʼ��menu_class[0]��menu_class_no*/
    menu_class[0].menu_addr = main_menu;    //���˵�  
    menu_class[0].sel_no = 0;               // Ĭ��ѡ��˵���
    menu_class_no = 0;                      //��һ���˵� 
    
    
    while(1)
    {   /*�˵���ʾ��ѡ��˵� */
        menu_sel = GUI_ICOMenuSelect( menu_class[menu_class_no].menu_addr,   // �˵��ṹ���,��MAIN_MENU
                                      menu_class[menu_class_no].sel_no );    // Ĭ��ѡ��˵���(���)
        
        /* �Բ˵���ѡֵ���д��� */
        switch(menu_sel)
        {  
						case RET_MAINMENU:  // ֱ�ӷ��ص�һ��˵�
                menu_class_no = 0; 
                break;
                
            case RET_UPMENU:    // ������һ��˵�  //GUI_ICOMenuSelectû�з���RET_UPMENU
                if(menu_class_no==0)
                {  
									return;     // �����ڵ�һ��˵��з���,��GUI_ICOMenuClass3
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
												GUI_FillSCR(0x00);   							// ���� 
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

int16_t yaw_fine;   //����΢��
u8 rcsentFlag = 0; //ң�ط��ͱ�־λ  ���� ����ң���Ƿ�������
u8 tumble_flag=0;


void FUN_Fly(void)
{
		unsigned int Thro,roll,pitch,roll_fine,pitch_fine;    
		unsigned int bat_val,bat_val1;   //ң�غлؼҵ�ص���
		unsigned char key=0,i=0;             //���� 
	  unsigned long sum=0;
	  static u8 chang_flag=0,tumbletim=0; 
	
		GUI_ClearSCR();
		GUI_LoadPic(0,0,( uint8*)gImage_bat,34,16);  //��ң�ص��
	  GUI_LoadPic(93,0,( uint8*)gImage_bat,34,16); //���һ����
		GUI_LoadPic(58,0,( uint8*)ico_single,34,16); //���ź�
		
		GUI_Rectangle(10, 16, 25, 62, 1);  //�����ſ�
	  GUI_Rectangle(30, 17, 75, 30, 1);  //������΢����
	  GUI_Rectangle(30, 33, 75, 46, 1);  //������ȵ���
	  GUI_Rectangle(30, 49, 75, 62, 1);  //������΢����
		GUI_Circle(104, 39, 23, 1);        //��Բ
		OLED_Refresh_Gram();  
	  rcsentFlag = 1;
		while(1)
		{
				key=KEY_Scan(0);  //����ɨ��
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
				else if(key==KEY_ESC)    //�˳�����
				{
					  rcsentFlag = 0;     //ң�ط��ͱ�־����  ң�ؾͲ��ڷ�������
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
			  if(yaw_fine>20)  yaw_fine =20;            //����΢���޷�
				else if(yaw_fine < -20)  yaw_fine = -20;  
				
				if(chang_flag)  //���������΢��
				{
				    if(ADC_ConvertedValue[0]>100)   //������֮���Զ����溽��΢��
				    {
						   EE_WriteVariable(VirtAddVarTab[0],yaw_fine);  //д��FLASH
							 chang_flag=0;
						}
				}
				
				
				GUI_RectangleFill(11, 17, 24, 62, 0);  //������
				GUI_CircleFill(roll, pitch, 2, 0);     //��ҡ��λ��
				GUI_RectangleFill(31, 18, 74, 29, 0);  //���һ�������ʾ
				GUI_RectangleFill(31, 34, 74, 45, 0);  //���һ�������ʾ
				GUI_RectangleFill(31, 50, 74, 61, 0);  //���һ�������ʾ
				GUI_RectangleFill(2, 4, 28, 12, 0);    //��ң�ص�����ʾ
				GUI_RectangleFill(97, 4, 120, 12, 0);  //���һ�������ʾ
				
				
				Thro=61-ADC_ConvertedValue[0]/93;      //��������
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

				
				GUI_RectangleFill(11, Thro, 24, 62, 1);     //������ʾ
				GUI_CircleFill(roll, pitch, 2, 1);          //�Ҳ�ҡ��λ����ʾ  
        GUI_RectangleFill(roll_fine, 18, roll_fine+3, 29, 1);   //�����΢������
				GUI_RectangleFill(pitch_fine, 34, pitch_fine+3, 45, 1); //������΢������
				GUI_RectangleFill(51+yaw_fine, 50, 54+yaw_fine, 61, 1); //������΢������
				GUI_RectangleFill(4, 4, bat_val, 12, 1);    //ң�ص�����ʾ
				GUI_RectangleFill(97, 4,97+bat_val1, 12, 1);//�һ�������ʾ

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
				if(key==KEY_OK)//У��
				{
					OLED_ShowString(4,0,"The plane is   correctting,    please wait");
					OLED_Refresh_Gram();
					sentCorrect(1);//����У��ָ��
					sentCorrect(1);//����У��ָ��
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



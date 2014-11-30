#ifndef __ICO_MENU_H
#define __ICO_MENU_H	

#include  "config.h"


/* ���⹦�ܵĲ˵�����ֵ*/
#define  RET_MAINMENU   0x80            /* ֱ�ӷ������˵������˵��� */
#define  RET_UPMENU     0x40            /*������һ��˵� */

/*����˵�������ݽṹ*/
struct ST_MY_MENU
{   MENUICO  *mdata;    	//һ��ͼ��˵����������
    uint8    ret_no;      	// �˵�����ֵ
    struct ST_MY_MENU  *next_menu;   	// ��һ���Ӳ˵����ָ�룬��û���Ӳ˵�������ΪNULL
};
typedef struct ST_MY_MENU  MY_MENU;

/* ���嵱ǰ����˵������ַ����ѡ���Ӳ˵���(���) */
typedef  struct
{   MY_MENU  *menu_addr;    // ��ǰ����˵������ַ
    uint8    sel_no;        // ��ѡ���Ӳ˵���(���)�������ϼ��˵�����ʾ��ǰ�˵���ʶʱ�õ�
} MENU_CLASS;


extern u8 rcsentFlag;
void  GUI_MenuIcoDrawA(MENUICO *ico, uint8 sel);
void  GUI_ICOMenuClass3(MY_MENU  main_menu[]);
#endif

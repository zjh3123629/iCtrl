#ifndef __ICO_MENU_H
#define __ICO_MENU_H	

#include  "config.h"


/* 特殊功能的菜单返回值*/
#define  RET_MAINMENU   0x80            /* 直接返回最顶层菜单（主菜单） */
#define  RET_UPMENU     0x40            /*返回上一层菜单 */

/*定义菜单句柄数据结构*/
struct ST_MY_MENU
{   MENUICO  *mdata;    	//一个图标菜单项的主数据
    uint8    ret_no;      	// 菜单返回值
    struct ST_MY_MENU  *next_menu;   	// 下一级子菜单句柄指针，若没有子菜单则设置为NULL
};
typedef struct ST_MY_MENU  MY_MENU;

/* 定义当前级别菜单句柄地址与所选的子菜单项(序号) */
typedef  struct
{   MY_MENU  *menu_addr;    // 当前级别菜单句柄地址
    uint8    sel_no;        // 所选的子菜单项(序号)。返回上级菜单和显示当前菜单标识时用到
} MENU_CLASS;


extern u8 rcsentFlag;
void  GUI_MenuIcoDrawA(MENUICO *ico, uint8 sel);
void  GUI_ICOMenuClass3(MY_MENU  main_menu[]);
#endif

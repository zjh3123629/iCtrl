#include  "config.h"
#include "ico_MENU.h"

#define  ICO1_X		5
#define  ICO1_Y		14

#define  ICO2_X		47
#define  ICO2_Y		14

#define  ICO3_X		90
#define  ICO3_Y		14

#define  ICO4_X		5
#define  ICO4_Y		14  




extern uint8  ico_plane[];//·É¿Ø²Ëµ¥
extern uint8  f_plane[];
extern void FUN_Fly(void);
const  MENUICO	mico1_1 = 
{	
	ICO1_X, 	// x
	ICO1_Y,		// y
	ico_plane,	// icodat
	f_plane,		// title
	0,			// state
	FUN_Fly		// Function
};

extern uint8  ico_debug[];
extern uint8  f_debug[];
extern void FUN_Debug(void);
const  MENUICO	mico1_2 = 
{	
	ICO2_X, 	// x
	ICO2_Y,		// y
	ico_debug,	// icodat
	f_debug,//	// title
	0,			// state
	FUN_Debug		// Function
	
};
extern uint8  ico_ctrl[];
extern uint8  f_ctrl[];//Ò£¿Ø²Ëµ¥->
const  MENUICO	mico1_3 = 
{	
	ICO3_X, 	// x
	ICO3_Y,		// y
	ico_ctrl,	// icodat
	f_ctrl,		// title
	0,			// state
	NULL		// Function
};

/***************************************************/
extern uint8  ico_flymode[];
extern uint8  gImage_sq[];

const  MENUICO	mico2_11 = 
{	
	ICO1_X, 	// x
	ICO1_Y,		// y
	ico_flymode,	// icodat
	gImage_sq,	// title
	0,			// state
	FUN_Fly		// Function
	
};

extern uint8  ico_debugmode[];
extern void FUN_Debug(void);
const  MENUICO	mico2_12 = 
{	
	ICO2_X, 	// x
	ICO2_Y,		// y
	ico_debugmode,	// icodat
	gImage_sq,	// title
	0,			// state
	FUN_Debug		// Function
	
};

extern uint8  ico_ret[];
extern uint8  f_ret[];//·µ»Ø²Ëµ¥
const  MENUICO	mico2_13 = 
{	
	ICO3_X, 	// x
	ICO3_Y,		// y
	ico_ret,	// icodat
	f_ret,		// title
	0,			// state
	NULL		// Function
};








extern uint8  ico_set[];
extern uint8  f_set[];//Ò£¿Ø²Ëµ¥->
const  MENUICO	mico2_31 = 
{	
	ICO1_X, 	// x
	ICO1_Y,		// y
	ico_set,	// icodat
	f_set,		// title
	0,			// state
	NULL		// Function
};


const  MENUICO	mico2_24 = 
{	
	ICO2_X, 	// x
	ICO2_Y,		// y
	ico_ret,	// icodat
	f_ret,		// title
	0,			// state
	NULL		// Function
};

const  MENUICO	mico2_33 = 
{	
	ICO2_X, 	// x
	ICO2_Y,		// y
	ico_ret,	// icodat
	f_ret,		// title
	0,			// state
	NULL		// Function
};




const MY_MENU  Mode_MENU[] = 
{   
		{(MENUICO *)&mico2_11, 0,  NULL },  	// "???"->"??"
    {(MENUICO *)&mico2_12, 1,  NULL },    	// "???"->"??"
    {(MENUICO *)&mico2_13, RET_UPMENU,  NULL },	// "???"->"??"
    {NULL, RET_UPMENU,  NULL }
};
		
const MY_MENU  Plane_MENU[5] = 
{   //{(MENUICO *)&mico2_21, 0,  NULL },  	// "??"->"???"
   // {(MENUICO *)&mico2_22, 1,  NULL },    	// "??"->"???"
   // {(MENUICO *)&mico2_23, 2,  NULL },    	// "??"->"???"
    {(MENUICO *)&mico2_24, RET_UPMENU,  NULL },	// "??"->"??"
    {NULL, RET_UPMENU,  NULL }
};


const MY_MENU  Ctrl_MENU[4] = 
{  	{(MENUICO *)&mico2_31, 0,  NULL },  	// "??"->"??"->SETCLK_MENU
    //{(MENUICO *)&mico2_32, 1,  NULL },    		// "??"->"??"
    {(MENUICO *)&mico2_33, RET_UPMENU,  NULL },	// "??"->"??"
    {NULL, RET_UPMENU,  NULL }
};

const MY_MENU  MAIN_MENU[] = 
{  
		{(MENUICO *)&mico1_1, 0, NULL},   // 
		{(MENUICO *)&mico1_2, 1, NULL},   // 
		//{(MENUICO *)&mico1_3, 2, NULL},   // 
    {NULL, RET_UPMENU,  NULL }		// ????
};




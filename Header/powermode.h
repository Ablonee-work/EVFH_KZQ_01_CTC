
/*----------------------------------------------------------------------*/
/* Confidential and Proprietary Information of MicroPerfect Corporation */
/* (C) 2017 ,MicroPerfect Corporation . All rights reserved. */
/* FileName : powermode.h */
/* New File Date:2017.11.02 */
/* Function :  */
/* Ver:001 */
/* Author:MP_Ahren*/
/* History:YYYY.MM.DD Version Author Description */
/* */
/*-------------------------------------------------------------*/

#ifndef _POWERMODE_H_
#define	_POWERMODE_H_

/***************头文件****************/
/**/
/*************************************/

#include"config.h"

/******************宏定义*************/
/**/
/*************************************/
/*****************电源管理引脚配置********************/

#define BU_DET_State   		    RB1			//BU状态检测Prot I/O
#define BU_DET_Direction		TRISB1		//BU状态Prot I/O 方向

#define ACC_DET_State   		RB2			//ACC状态检测Prot I/O
#define ACC_DET_Direction		TRISB2		//ACC状态Prot I/O 方向

/*************************************/

/***************标志位申明************/
/**/
/*************************************/

extern u8 Power_ACC_Timer_Flag_g;			
extern u8 Power_BU_Timer_Flag_g;           

/***************函数申明**************/
/**/
/*************************************/
void Power_Det_Port_Init(void);
u8 Power_ACC_State_Det();					//ACC状态检测函数
u8 Power_Work_Mode_Confirm();            //电源工作模式确认函数		
/*************************************/

#endif	


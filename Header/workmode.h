
/*----------------------------------------------------------------------*/
/* Confidential and Proprietary Information of MicroPerfect Corporation */
/* (C) 2017 ,MicroPerfect Corporation . All rights reserved. */
/* FileName : workmode.h */
/* New File Date:2017.11.09 */
/* Function :  */
/* Ver:001 */
/* Author:MP_Ahren*/
/* History:YYYY.MM.DD Version Author Description */
/* */
/*-------------------------------------------------------------*/

#ifndef _WORKMODE_H_
#define	_WORKMODE_H_

/***************头文件****************/
/**/
/*************************************/

#include "config.h"

/******************宏定义*************/
/**/
/*************************************/


/*************************************/

/***************标志位申明************/
/**/
/*************************************/

extern u8 Work_Mode_Confirm_Flag_g;				
extern u8 Implement_Only_1_Variable;
extern u8 Sys_Onesecond_Count_g;        
/***************函数申明**************/
/**/
/*************************************/

void Work_Mode_Choice(void);					//工作模式选择函数
u8 Sys_Time_Count_WorkMode(void);
/*************************************/

#endif

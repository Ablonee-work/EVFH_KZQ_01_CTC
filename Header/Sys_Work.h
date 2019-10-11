/*----------------------------------------------------------------------*/
/* Confidential and Proprietary Information of MicroPerfect Corporation */
/* (C) 2018 ,MicroPerfect Corporation . All rights reserved. */
/* FileName : sys_work.h */
/* Function : */
/* Ver: 0.01  */
/* MP_Ahren NewFile */
/* History:YYYY.MM.DD Version author Description 
          :2018.05.24 0.01 @Ahren New file  */
/*-------------------------------------------------------------*/

#ifndef SYS_WORK_H
#define	SYS_WORK_H

/***************头文件****************/
/**/
#include "config.h"
/*************************************/

/******************宏定义*************/
/**/
/*************************************/


/*************************************/

/***************标志位申明************/
/**/
u8 Smoke_State_Flag_g = 0x00;   //光电烟感状态
u8 Fire_State_Flag_g  = 0x00;   //点火头状态

/*************************************/


/***************函数申明**************/
/**/

void Sys_Function_Work(void);
/*************************************/

/*************************************/

#endif	/* SYS_WORK_H */


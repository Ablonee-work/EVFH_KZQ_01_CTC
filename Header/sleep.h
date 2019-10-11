
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

#ifndef _SLEEP_H_
#define _SLEEP_H_

/***************头文件****************/
/**/
/*************************************/

#include"config.h"

/******************宏定义*************/
/**/
/*************************************/


/*************************************/

/***************标志位申明************/
/**/
/*************************************/


/***************函数申明**************/
/**/
/*************************************/

void Sleep_Mode_Init(void);			
void PRI_RUN_Mode_Init(void);
void SEC_RUN_Mode_Init(void);
void RC_RUN_Mode_Init(void);
void PRI_IDLE_Mode_Init(void);
void SEC_IDLE_Mode_Init(void);
void RC_IDLE_Mode_Init(void);

/*************************************/

#endif


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

/***************ͷ�ļ�****************/
/**/
/*************************************/

#include "config.h"

/******************�궨��*************/
/**/
/*************************************/


/*************************************/

/***************��־λ����************/
/**/
/*************************************/

extern u8 Work_Mode_Confirm_Flag_g;				
extern u8 Implement_Only_1_Variable;
extern u8 Sys_Onesecond_Count_g;        
/***************��������**************/
/**/
/*************************************/

void Work_Mode_Choice(void);					//����ģʽѡ����
u8 Sys_Time_Count_WorkMode(void);
/*************************************/

#endif

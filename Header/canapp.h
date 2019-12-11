
/*----------------------------------------------------------------------*/
/* Confidential and Proprietary Information of MicroPerfect Corporation */
/* (C) 2017 ,MicroPerfect Corporation . All rights reserved. */
/* FileName : led.h */
/* New File Date:2019.06.04 */
/* Function :  */
/* Ver:001 */
/* Author:MP_Ahren*/
/* History:YYYY.MM.DD Version Author Description */
/* */
/*-------------------------------------------------------------*/


#ifndef CANAPP_H
#define	CANAPP_H

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
u8 CAN_Sent_Time_Cnt_g = 0;
/*************************************/


/***************函数申明**************/
/**/
void Get_TC04_CAN_Data(void);
void Sent_TC04_CAN_Data(void);

void CAN_Data_Choice_Analysis(void);
/*************************************/

/*************************************/

#endif	/* CANAPP_H */


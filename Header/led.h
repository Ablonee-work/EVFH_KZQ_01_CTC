
/*----------------------------------------------------------------------*/
/* Confidential and Proprietary Information of MicroPerfect Corporation */
/* (C) 2017 ,MicroPerfect Corporation . All rights reserved. */
/* FileName : led.h */
/* New File Date:2017.11.03 */
/* Function :  */
/* Ver:001 */
/* Author:MP_Ahren*/
/* History:YYYY.MM.DD Version Author Description */
/* */
/*-------------------------------------------------------------*/


#ifndef _LED_H_
#define	_LED_H_

/***************头文件****************/
/**/
/*************************************/

#include"config.h"

/******************宏定义*************/
/**/
/*************************************/

#define LED_Display		LATF1					//定义LED显示Port I/O
#define LED_Direction   TRISF1				//LED Prot I/O 方向

/*************************************/

/***************标志位申明************/
/**/
/*************************************/


/***************函数申明**************/
/**/
/*************************************/

void Led_Display_Init(void);				//LED显示初始化函数
void Led_Display_Fuction(void);				//点亮LED函数

/*************************************/

#endif	


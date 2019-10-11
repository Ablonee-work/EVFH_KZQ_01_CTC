
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

/***************ͷ�ļ�****************/
/**/
/*************************************/

#include"config.h"

/******************�궨��*************/
/**/
/*************************************/

#define LED_Display		LATF1					//����LED��ʾPort I/O
#define LED_Direction   TRISF1				//LED Prot I/O ����

/*************************************/

/***************��־λ����************/
/**/
/*************************************/


/***************��������**************/
/**/
/*************************************/

void Led_Display_Init(void);				//LED��ʾ��ʼ������
void Led_Display_Fuction(void);				//����LED����

/*************************************/

#endif	


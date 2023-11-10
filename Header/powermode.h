
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

/***************ͷ�ļ�****************/
/**/
/*************************************/

#include"config.h"

/******************�궨��*************/
/**/
/*************************************/
/*****************��Դ������������********************/

#define BU_DET_State   		    RB1			//BU״̬���Prot I/O
#define BU_DET_Direction		TRISB1		//BU״̬Prot I/O ����

#define ACC_DET_State   		RB2			//ACC״̬���Prot I/O
#define ACC_DET_Direction		TRISB2		//ACC״̬Prot I/O ����

/*************************************/

/***************��־λ����************/
/**/
/*************************************/

extern u8 Power_ACC_Timer_Flag_g;			
extern u8 Power_BU_Timer_Flag_g;           

/***************��������**************/
/**/
/*************************************/
void Power_Det_Port_Init(void);
u8 Power_ACC_State_Det();					//ACC״̬��⺯��
u8 Power_Work_Mode_Confirm();            //��Դ����ģʽȷ�Ϻ���		
/*************************************/

#endif	


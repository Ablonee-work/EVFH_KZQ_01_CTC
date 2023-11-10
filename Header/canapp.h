
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
#define TC_NUM_MAX     16
#define TC_DATA_BUFF   64

/*************************************/
enum//Can to Can 与主机通信类型
{
    CTC_ZHU_Wake = 0x01,
    CTC_ZHU_Sleep = 0x02,
    CTC_ZHU_CodeFault = 0x03,
    CTC_ZHU_CodeNormal = 0x04,
    CTC_ZHU_BackupDataFault = 0x05,
    CTC_ZHU_BackupDataClear = 0x06
};

/***************标志位申明************/
/**/
extern u8 TC04_Buff[TC_NUM_MAX][9];
extern u8 TC_Num;
extern u8 MCP2515_Sent_Time_Flag;
extern u8 TJA1042_Sent_Time_Flag;
extern u8 TC04_OffLine_Flag[TC_NUM_MAX];
extern uint16_t TC04_Off_Time_Cnt_g;
extern u8 Outgoing_Data_Cycle;


extern u8 TC_Info_SendOut[TC_NUM_MAX][4];
extern u8 Sys_Sleep_Flag;
extern u8 TC_Sleep_Flag[TC_NUM_MAX];
extern u8 TC_Code_Err;
extern u8 Sys_Sleep_Wake_TC;
extern u8 PowerOn_OfflineDet_Flag;

extern uint16_t TC_C1_R_10Min;//10分钟计时 
extern u8 Mamual_Start;

extern uint8_t RxMsgBuff_Flag[TC_DATA_BUFF];

void CanToCan_TC_F0(u8 state); //低功耗

/***************函数申明**************/
/**/
void TC04_CAN_Data_Process(void);
void Get_TC04_CAN_Data(void);
void Sent_TC04_CAN_Data(void);

/*************************************/

/*************************************/

#endif	/* CANAPP_H */


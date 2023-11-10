/*----------------------------------------------------------------------*/
/* Confidential and Proprietary Information of MicroPerfect Corporation */
/* (C) 2018 ,MicroPerfect Corporation . All rights reserved. */
/* FileName : MCP2515.h */
/* Function : */
/* Ver: 0.01  */
/* MP_Ahren NewFile */
/* History:YYYY.MM.DD Version author Description 
          :2018.07.03 0.01 @wuwenfang Follow the coding standard  */
/*-------------------------------------------------------------*/
#ifndef __MCP2515_H
#define __MCP2515_H

/***************头文件****************/
/**/
/*************************************/

#include"config.h"

/******************宏定义*************/
/**/
/*************************************/
#define MCP2515_SILENT_DIR      TRISC0
#define MCP2515_CS_DIR          TRISC1
#define MCP2515_RESET_DIR       TRISC2
#define MCP2515_SCK_DIR         TRISC3
#define MCP2515_MISO_DIR        TRISC4
#define MCP2515_MOSI_DIR        TRISC5

#define MCP2515_SILENT          LATC0
#define MCP2515_CS              LATC1
#define MCP2515_RESET           LATC2
#define MCP2515_SCK             LATC3
#define MCP2515_MISO            LATC4
#define MCP2515_MOSI            LATC5

#define MCP2515_SILENT_ON()       MCP2515_SILENT=0
#define MCP2515_SILENT_OFF()      MCP2515_SILENT=1
/***************标志位申明************/
/**/
/*************************************/

/***************变量定义**************/
/**/


/*************************************/

/***************函数申明**************/
/**/
/*************************************/
void MCP2515_Init(void);
void MCP2515_Data_Send(uint32_t ID, u8 *Dtat, u8 Len);
u8 MCP2515_Sleep_Mode(void);
/****************************************************************************/

#endif

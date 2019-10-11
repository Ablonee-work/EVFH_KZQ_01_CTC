/*----------------------------------------------------------------------*/
/* Confidential and Proprietary Information of MicroPerfect Corporation */
/* (C) 2017 ,MicroPerfect Corporation . All rights reserved. */
/* FileName : can.h */
/* Function : */
/* Ver: 0.01  */
/* MP_Ahren NewFile */
/* History:YYYY.MM.DD Version author Description 
          :2017.12.04 0.01 @huajifeng Follow the coding standard  */
/*-------------------------------------------------------------*/

#ifndef _CAN_H_
#define _CAN_H_

/***************头文件****************/
/**/
/*************************************/

#include"config.h"

/******************宏定义*************/
/**/
/*************************************/

#define CAN_STB_ON    RB5=0 						       //使能CAN 收发器 TJA1042
#define CAN_STB_OFF   RB5=1						           //关闭CAN 收发器 TJA1042

/***************标志位申明************/
/**/
/*************************************/

bit CAN_RXD_Interrupt_Flag_g = 0;					    	//RX0接收中断标志位 "=1"表示有CAN有接收到信号
bit CAN2_RXD_Interrupt_Flag_g = 0;					    	//RX0接收中断标志位 "=1"表示有CAN2有接收到信号

/***************变量定义**************/
/**/
/*************************************/
u8  CAN_TX0_Buff[9]        = {0x00};				    	//CAN发送缓冲区				
u8  CAN_RX0_Buff[9]	       = {0x00};				    	//CAN接收缓冲区

u8  CAN2_TX0_Buff[9]        = {0x00};				    	//CAN2发送缓冲区				
u8  CAN2_RX0_Buff[9]	    = {0x00};				    	//CAN2接收缓冲区

u8  CAN_Receive_Data[8]    = {0x00};

uint16 CAN_Life_Signal_Count = 0;                               //心跳包计数变量
u8 	MCU_CAN_Send_1s_Flag_g = 0;
u8 	MCP_CAN_Send_1s_Flag_g = 0;

u8 MCU_TxID[4]={0x18,0xFF,0x1D,0x00};

u8 MCP2515_RXB0SIDH_R = 0;
u8 MCP2515_RXB0SIDL_R = 0;
u8 MCP2515_RXB0EID8_R = 0;
u8 MCP2515_RXB0EID0_R = 0;
u8 CAN_ID_Flag = 0; 
/***************函数申明**************/
/**/
/*************************************/

void CAN_Init();                                                  //CAN初始化函数
void CAN_Send_8Byte_Data(void);                                  //CAN发送函数，用于各项数据				
void CAN_Send_Unfixed_Data(void);
void CAN_Interrupt_Occurred(void);                              //发生中断接收函数（测试用）
void CAN_Send_DCL_Data(unsigned char CAN_TX_Buf);                //发送DLC长度的报文
void CAN_Receive_Interrupt_Data(unsigned char *CAN_RX_Buf);     //CAN接收发送通信测试函数		
void Clear_CAN_TX0_Buf_New(u8 CAN_Buf_Data,u8 Send_Num);        //设置CAN_TX0_Buf值
void CAN_Life_Signal(void);                                      //心跳包函数
void CAN_RXD_TXD_Test(void);                                     //CAN接收发送通信测函数


void CAN_Send_Alarm_Warning_Data(void);
void CAN_Send_8Byte_Data_MCP_R(u8 *arr);                        //邮箱0发送DLC长度的报文
void TJA1042_CAN_Send_Data(u8 Box_Num, u8 *Data_Buff, u8 Data_Leng,u8 Life_Signal);
/****************************************************************************/

#endif



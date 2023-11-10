/*----------------------------------------------------------------------*/
/* Confidential and Proprietary Information of MicroPerfect Corporation */
/* (C) 2017 ,MicroPerfect Corporation . All rights reserved. */
/* FileName : uart.h */
/* Function : */
/* Ver: 0.01  */
/* MP_Ahren NewFile */
/* History:YYYY.MM.DD Version author Description 
          :2017.12.04 0.01 @huajifeng Follow the coding standard  */
/*-------------------------------------------------------------*/

#ifndef _UART_H_
#define _UART_H_

/***************头文件****************/
/**/
/*************************************/
#include"config.h"

/******************宏定义*************/
/**/
/*************************************/
#define Uart_TX1   RG3
#define Uart_RX1   RG0

#define Uart_TX2   RE7
#define Uart_RX2   RE6

#define Uart_Send_number   300                               //定义发送多少个字节     
#define Uart_Rece_Num   8                                  //定义接收多少字节



/***************标志位申明************/
/**/
/*************************************/
extern u8 Uart_Rece_Data[Uart_Rece_Num];                 
extern bit UART_Rece_Interrupt_Flag_g;
extern u8 receive_num;     

/******************函数声明*************/
/**/
/***************************************/
void Uart1_Init();                                           //串口1初始化函数
void Uart2_Init();                                          //串口1初始化函数
void Uart1_Send_Byte(u8 Uart1_number, u8 *Uart1_data);     //串口发送函数
/*20180716*/
void Uart1_Send_Byte_1(uint16 Send_Data);                 //串口发送16个Byte,MCU下位机发数据给上位机


void Uart1_Interrupt_Receive_Byte();                     //串口中断接收函数

#endif



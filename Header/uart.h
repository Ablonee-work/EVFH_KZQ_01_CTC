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

/***************ͷ�ļ�****************/
/**/
/*************************************/
#include"config.h"

/******************�궨��*************/
/**/
/*************************************/
#define Uart_TX1   RG3
#define Uart_RX1   RG0

#define Uart_TX2   RE7
#define Uart_RX2   RE6

#define Uart_Send_number   300                               //���巢�Ͷ��ٸ��ֽ�     
#define Uart_Rece_Num   8                                  //������ն����ֽ�

u8 Uart_Send_Data[Uart_Send_number] = {0x00};              // �����ֽڻ�����
u8 Uart_Rece_Data[Uart_Rece_Num]    = (0x00);                 //�����ֽڻ�����

/***************��־λ����************/
/**/
/*************************************/
bit UART_Rece_Interrupt_Flag_g = 0;
u8 receive_num = 0;     //�ܹ��յ����ֽ���

/******************��������*************/
/**/
/***************************************/
void Uart1_Init();                                           //����1��ʼ������
void Uart2_Init();                                          //����1��ʼ������
void Uart1_Send_Byte(u8 Uart1_number, u8 *Uart1_data);     //���ڷ��ͺ���
/*20180716*/
void Uart1_Send_Byte_1(uint16 Send_Data);                 //���ڷ���16��Byte,MCU��λ�������ݸ���λ��


void Uart1_Interrupt_Receive_Byte();                     //�����жϽ��պ���

#endif



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

/***************ͷ�ļ�****************/
/**/
/*************************************/

#include"config.h"

/******************�궨��*************/
/**/
/*************************************/

#define CAN_STB_ON    RB5=0 						       //ʹ��CAN �շ��� TJA1042
#define CAN_STB_OFF   RB5=1						           //�ر�CAN �շ��� TJA1042

/***************��־λ����************/
/**/
/*************************************/

bit CAN_RXD_Interrupt_Flag_g = 0;					    	//RX0�����жϱ�־λ "=1"��ʾ��CAN�н��յ��ź�
bit CAN2_RXD_Interrupt_Flag_g = 0;					    	//RX0�����жϱ�־λ "=1"��ʾ��CAN2�н��յ��ź�

/***************��������**************/
/**/
/*************************************/
u8  CAN_TX0_Buff[9]        = {0x00};				    	//CAN���ͻ�����				
u8  CAN_RX0_Buff[9]	       = {0x00};				    	//CAN���ջ�����

u8  CAN2_TX0_Buff[9]        = {0x00};				    	//CAN2���ͻ�����				
u8  CAN2_RX0_Buff[9]	    = {0x00};				    	//CAN2���ջ�����

u8  CAN_Receive_Data[8]    = {0x00};

uint16 CAN_Life_Signal_Count = 0;                               //��������������
u8 	MCU_CAN_Send_1s_Flag_g = 0;
u8 	MCP_CAN_Send_1s_Flag_g = 0;

u8 MCU_TxID[4]={0x18,0xFF,0x1D,0x00};

u8 MCP2515_RXB0SIDH_R = 0;
u8 MCP2515_RXB0SIDL_R = 0;
u8 MCP2515_RXB0EID8_R = 0;
u8 MCP2515_RXB0EID0_R = 0;
u8 CAN_ID_Flag = 0; 
/***************��������**************/
/**/
/*************************************/

void CAN_Init();                                                  //CAN��ʼ������
void CAN_Send_8Byte_Data(void);                                  //CAN���ͺ��������ڸ�������				
void CAN_Send_Unfixed_Data(void);
void CAN_Interrupt_Occurred(void);                              //�����жϽ��պ����������ã�
void CAN_Send_DCL_Data(unsigned char CAN_TX_Buf);                //����DLC���ȵı���
void CAN_Receive_Interrupt_Data(unsigned char *CAN_RX_Buf);     //CAN���շ���ͨ�Ų��Ժ���		
void Clear_CAN_TX0_Buf_New(u8 CAN_Buf_Data,u8 Send_Num);        //����CAN_TX0_Bufֵ
void CAN_Life_Signal(void);                                      //����������
void CAN_RXD_TXD_Test(void);                                     //CAN���շ���ͨ�Ų⺯��


void CAN_Send_Alarm_Warning_Data(void);
void CAN_Send_8Byte_Data_MCP_R(u8 *arr);                        //����0����DLC���ȵı���
void TJA1042_CAN_Send_Data(u8 Box_Num, u8 *Data_Buff, u8 Data_Leng,u8 Life_Signal);
/****************************************************************************/

#endif



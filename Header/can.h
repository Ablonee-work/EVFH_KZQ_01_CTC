#ifndef _CAN_H_
#define _CAN_H_

#include"config.h"

//CAN �շ��� TJA1042����
#define CAN_STB_Pin      RB5 						                       //TJA1042��������
#define CAN_STB_Out()   (TRISB5 = 0)		

#define CAN_STB_ON()    (CAN_STB_Pin=0) 						       //ʹ��CAN �շ��� TJA1042
#define CAN_STB_OFF()   (CAN_STB_Pin=1)						           //�ر�CAN �շ��� TJA1042
				    	
extern bit CAN2_RXD_Interrupt_Flag_g;					    	
			
void CAN_Init(void);
void CAN_DeInit(void);

void CAN_Send_Data(uint32_t id, uint8_t len, uint8_t *buf);
uint8_t CAN_Rece_Data(uint32_t *id, uint8_t *len, uint8_t *buf);

#endif


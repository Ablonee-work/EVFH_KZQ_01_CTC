/*----------------------------------------------------------------------*/
/* Confidential and Proprietary Information of MicroPerfect Corporation */
/* (C) 2019 ,MicroPerfect Corporation . All rights reserved. */
/* FileName : XXX.h */
/* New File Date:2020.01.08 */
/* Function :  */
/* Ver:1.0 */
/* Author:MP_22*/
/* History:YYYY.MM.DD Version Author Description */
/* */
/*-------------------------------------------------------------*/
#ifndef __W25QXX_H
#define __W25QXX_H

/***************头文件****************/
/**/
#include<pic18.h>
#include <stdint.h>
#include"delay.h"
#include"canapp.h"
/*************************************/

/******************宏定义*************/
/**/

#define W25Q80 	0XEF13 	
#define W25Q16 	0XEF14
#define W25Q32 	0XEF15
#define W25Q64 	0XEF16
#define W25Q128	0XEF17
#define W25Q256 0XEF18
   
//指令表
#define W25X_WriteEnable		0x06 
#define W25X_WriteDisable		0x04 
#define W25X_ReadStatusReg1		0x05 
#define W25X_ReadStatusReg2		0x35 
#define W25X_ReadStatusReg3		0x15 
#define W25X_WriteStatusReg1    0x01 
#define W25X_WriteStatusReg2    0x31 
#define W25X_WriteStatusReg3    0x11 
#define W25X_ReadData			0x03 
#define W25X_FastReadData		0x0B 
#define W25X_FastReadDual		0x3B 
#define W25X_PageProgram		0x02 
#define W25X_BlockErase			0xD8 
#define W25X_SectorErase		0x20 
#define W25X_ChipErase			0xC7 
#define W25X_PowerDown			0xB9 
#define W25X_ReleasePowerDown	0xAB 
#define W25X_DeviceID			0xAB 
#define W25X_ManufactDeviceID	0x90 
#define W25X_JedecDeviceID		0x9F 
#define W25X_Enable4ByteAddr    0xB7
#define W25X_Exit4ByteAddr      0xE9

/*************************************/

/***************标志位申明************/
/**/
#define TC_BACKUP_COUNT 20
//#define TC_DATA_BASE           0    //见TC_X_Base
#define HOST_C2_DATA_ID           18

#define TC_BEFORE_DATA_OFFSET  0
#define TC_B3_DATA_OFFSET      40
#define TC_LAST_DATA_OFFSET    41
//以上都指扇区地址：0-2047

//扇区大小：4K字节
#define REC_SIZE_MAX             32             //每条记录最大长度
#define REC_SECTION_COUNT        100            //一个扇区内记录条数（最大：SECTION_SIZE/REC_SIZE_MAX=128）
#define REC_MAX_COUNT            600            //启动后记录总数
#define REC_MAX_COUNT_BEFORE     3000           //启动前记录总数

extern uint8_t BackupData_PowerON_Clear;
extern uint8_t BackupData_State[TC_BACKUP_COUNT];//备份数据状态  0：前十分钟   1：后十分钟
extern uint16_t BackupData_Cnt[TC_BACKUP_COUNT]; 
extern uint8_t BackupData_Flag[TC_BACKUP_COUNT];
extern const uint16_t TC_X_Base[TC_BACKUP_COUNT];
/*************************************/

/***************函数申明**************/
/**/
void W25QXX_Init(void);//初始化SPI FLASH的IO口
void W25QXX_delay_us(uint16_t nus);
void W25QXX_Read(uint8_t* pBuffer,uint16_t SecAddr, uint16_t Addr,uint16_t NumByteToRead); //读取SPI FLASH  

void W25QXX_Erase_Sector(uint16_t SecAddr, uint16_t Addr); //擦除一个扇区
void W25QXX_Erase_Chip(void); //擦除整个芯片
void W25QXX_PowerDown(void);//进入掉电模式
void W25QXX_WAKEUP(void); //唤醒

void CTC_BackupData_Init(void);
void CTC_BackupData_Before_Write(uint8_t TC_x,uint8_t* pBuffer, uint8_t len);
void CTC_BackupData_B3_Write(uint8_t TC_x,uint8_t* pBuffer, uint8_t len);
void CTC_BackupData_Last_Write(uint8_t TC_x,uint8_t* pBuffer, uint8_t len);
void CTC_BackupData_Buffer_Read(uint8_t TC_x);

/*************************************/

#endif


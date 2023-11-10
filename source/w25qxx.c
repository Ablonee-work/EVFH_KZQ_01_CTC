/*----------------------------------------------------------------------*/
/* Confidential and Proprietary Information of MicroPerfect Corporation */
/* (C) 2019 ,MicroPerfect Corporation . All rights reserved. */
/* FileName : xxx.c */
/* New File Date:2020.01.08 */
/* Function :  */
/* Ver:1.0 */
/* Author:MP_22*/
/* History:YYYY.MM.DD Version Author Description */
/* 
         eg:2020.01.08 002 @MP_22 Add : XXX
 */
/*-------------------------------------------------------------*/

/***************头文件****************/
/**/
#include "w25qxx.h"

/*************************************/

/******************宏定义*************/
#define W25Q64_SCK_Pin      LATF3
#define W25Q64_SCK_Port     TRISF3

#define W25Q64_MISO_Pin      RE1
#define W25Q64_MISO_Port     TRISE1

#define W25Q64_MOSI_Pin      LATA5
#define W25Q64_MOSI_Port     TRISA5

#define W25Q64_NSS_Pin      LATE2
#define W25Q64_NSS_Port     TRISE2

#define W25Q64_SCK_H()     (W25Q64_SCK_Pin=1)  
#define W25Q64_SCK_L()     (W25Q64_SCK_Pin=0)  	
	
#define W25Q64_MISO_IN()    W25Q64_MISO_Pin   

#define W25Q64_MOSI_H()    (W25Q64_MOSI_Pin=1)    
#define W25Q64_MOSI_L()    (W25Q64_MOSI_Pin=0) 

#define W25Q64_NSS_H()     (W25Q64_NSS_Pin=1)   
#define W25Q64_NSS_L()     (W25Q64_NSS_Pin=0)	
////////////////////////////////////////////////////////////////////////////////// 
/**/
/*************************************/

/***************标志位申明************/
/**/
static uint16_t W25QXX_TYPE = 0;	
/*************************************/

/***************函数申明**************/
/**/
static void SPI_Init(void);
static uint8_t W25QXX_SPI_RWByte(uint8_t TxData);
static uint8_t W25QXX_ReadSR(uint8_t regno);
//static void W25QXX_Write_SR(uint8_t regno,uint8_t sr);
static void W25QXX_Write_Enable(void);
static void W25QXX_Write_Disable(void);
static void W25QXX_Wait_Busy(void);
static uint16_t W25QXX_ReadID(void);
static void W25QXX_Write_Page(uint8_t* pBuffer,uint16_t SecAddr, uint16_t Addr,uint16_t NumByteToWrite);
/*************************************/
/*************************************/

//W25Q64容量(单位：字节(byte))
//一个page（256）, 16个page为一个Sector
//4K为一个Sector
//16个Sector为1个Block（64K）
//容量为8M字节,共有128个Block,8192个Sector 
//地址范围：0x0 - 0x7FFFFF

static void SPI_Init(void)
{
    W25Q64_SCK_Port  = 0;   
    W25Q64_MISO_Port = 1; 
    W25Q64_MOSI_Port = 0;      
    W25Q64_NSS_Port  = 0;          
}

//SPI读写数据
static uint8_t W25QXX_SPI_RWByte(uint8_t TxData)
{
	uint8_t bit_ctr;
	for(bit_ctr=0;bit_ctr<8;bit_ctr++)  // 输出8位
	{		
		if(TxData&0x80)
        {
            W25Q64_MOSI_H();
        }
		else 
        {
            W25Q64_MOSI_L();
        }		
		TxData=(TxData<<1);			
		W25QXX_delay_us(1);
		
		W25Q64_SCK_L();
		W25QXX_delay_us(1);
		
		W25Q64_SCK_H();
		W25QXX_delay_us(1);	
		TxData |= W25Q64_MISO_IN();
	}
	return TxData;
}


void W25QXX_delay_us(uint16_t nus)
{
    //delay_10us(nus);
}	

//读取W25QXX的状态寄存器，W25QXX一共有3个状态寄存器
//状态寄存器1：
//BIT7  6   5   4   3   2   1   0
//SPR   RV  TB BP2 BP1 BP0 WEL BUSY
//SPR:默认0,状态寄存器保护位,配合WP使用
//TB,BP2,BP1,BP0:FLASH区域写保护设置
//WEL:写使能锁定
//BUSY:忙标记位(1,忙;0,空闲)
//默认:0x00
//状态寄存器2：
//BIT7  6   5   4   3   2   1   0
//SUS   CMP LB3 LB2 LB1 (R) QE  SRP1
//状态寄存器3：
//BIT7      6    5    4   3   2   1   0
//HOLD/RST  DRV1 DRV0 (R) (R) WPS ADP ADS
//regno:状态寄存器号，范:1~3
//返回值:状态寄存器值
static uint8_t W25QXX_ReadSR(uint8_t regno)   
{  
	uint8_t byte=0,command=0; 
    switch(regno)
    {
        case 1:
            command=W25X_ReadStatusReg1;    //读状态寄存器1指令
            break;
        case 2:
            command=W25X_ReadStatusReg2;    //读状态寄存器2指令
            break;
        case 3:
            command=W25X_ReadStatusReg3;    //读状态寄存器3指令
            break;
        default:
            command=W25X_ReadStatusReg1;    
            break;
    }    
	W25Q64_NSS_L();                            //使能器件   
	W25QXX_delay_us(3);	
	W25QXX_SPI_RWByte(command);            //发送读取状态寄存器命令    
	byte=W25QXX_SPI_RWByte(0Xff);          //读取一个字节  
	W25Q64_NSS_H();                            //取消片选     
	return byte;   
} 

////写W25QXX状态寄存器
//static void W25QXX_Write_SR(uint8_t regno,uint8_t sr)   
//{   
//    uint8_t command=0;
//    switch(regno)
//    {
//        case 1:
//            command=W25X_WriteStatusReg1;    //写状态寄存器1指令
//            break;
//        case 2:
//            command=W25X_WriteStatusReg2;    //写状态寄存器2指令
//            break;
//        case 3:
//            command=W25X_WriteStatusReg3;    //写状态寄存器3指令
//            break;
//        default:
//            command=W25X_WriteStatusReg1;    
//            break;
//    }   
//	W25Q64_NSS_L();                            //使能器件   
//	W25QXX_SPI_RWByte(command);            //发送写取状态寄存器命令    
//	W25QXX_SPI_RWByte(sr);                 //写入一个字节  
//	W25Q64_NSS_H();                            //取消片选     	      
//}   

//W25QXX写使能	
//将WEL置位   
static void W25QXX_Write_Enable(void)   
{
	W25Q64_NSS_L();                        //使能器件
	W25QXX_delay_us(1);		
    W25QXX_SPI_RWByte(W25X_WriteEnable);   //发送写使能  
	W25Q64_NSS_H();                        //取消片选     	      
} 

//W25QXX写禁止	
//将WEL清零  
static void W25QXX_Write_Disable(void)   
{  
	W25Q64_NSS_L();                            //使能器件   
	W25QXX_delay_us(1);	
  W25QXX_SPI_RWByte(W25X_WriteDisable);  //发送写禁止指令    
	W25Q64_NSS_H();                            //取消片选     	      
} 

//等待空闲
static void W25QXX_Wait_Busy(void)   
{   
	while((W25QXX_ReadSR(1)&0x01)==0x01);   // 等待BUSY位清空
}  

//读取芯片ID
//返回值如下:				   
//0XEF13,表示芯片型号为W25Q80  
//0XEF14,表示芯片型号为W25Q16    
//0XEF15,表示芯片型号为W25Q32  
//0XEF16,表示芯片型号为W25Q64 
//0XEF17,表示芯片型号为W25Q128 	  
//0XEF18,表示芯片型号为W25Q256
static uint16_t W25QXX_ReadID(void)
{
	uint16_t Temp = 0;	  
	W25Q64_NSS_L();				    
	W25QXX_delay_us(3);	
	W25QXX_SPI_RWByte(0x90);//发送读取ID命令	    
	W25QXX_SPI_RWByte(0x00); 	    
	W25QXX_SPI_RWByte(0x00); 	    
	W25QXX_SPI_RWByte(0x00); 	 			   
	Temp|=W25QXX_SPI_RWByte(0xFF)<<8;  
	Temp|=W25QXX_SPI_RWByte(0xFF);	 
	W25Q64_NSS_H();				    
	return Temp;
}   		    

//SPI在一页(0~65535)内写入少于256个字节的数据
//在指定地址开始写入最大256字节的数据
//pBuffer:数据存储区
//WriteAddr:开始写入的地址(24bit)
//NumByteToWrite:要写入的字节数(最大256),该数不应该超过该页的剩余字节数!!!	 
static void W25QXX_Write_Page(uint8_t* pBuffer, uint16_t SecAddr, uint16_t Addr, uint16_t NumByteToWrite)
{
 	uint16_t i;  
    
    i = SecAddr&0x0F;
    Addr |= i << 12;
    SecAddr = SecAddr>>4;
    
	W25QXX_Write_Enable();                  //SET WEL 
	W25Q64_NSS_L();                            //使能器件   
	W25QXX_delay_us(3);	
	W25QXX_SPI_RWByte(W25X_PageProgram);   //发送写页命令   
	if(W25QXX_TYPE==W25Q256)                //如果是W25Q256的话地址为4字节的，要发送最高8位
	{
			W25QXX_SPI_RWByte((uint8_t)((SecAddr)>>8)); 
	}
	W25QXX_SPI_RWByte((uint8_t)SecAddr); //发送24bit地址    
	W25QXX_SPI_RWByte((uint8_t)((Addr)>>8));   
	W25QXX_SPI_RWByte((uint8_t)Addr);   
	for(i=0;i<NumByteToWrite;i++)
    {
       W25QXX_SPI_RWByte(pBuffer[i]);//循环写数    
    }        
	W25Q64_NSS_H();                            //取消片选 
	W25QXX_Wait_Busy();					   //等待写入结束
	W25QXX_Write_Disable();
} 

//应用
//初始化SPI FLASH的IO口
void W25QXX_Init(void)
{ 
    uint8_t temp;
    SPI_Init();		   			        //初始化SPI
    W25Q64_NSS_H();			            //SPI FLASH不选中
    
    W25QXX_TYPE = W25QXX_ReadID();	      //读取FLASH ID.
    if(W25QXX_TYPE == W25Q256)            //SPI FLASH为W25Q256
    {
        temp=W25QXX_ReadSR(3);              //读取状态寄存器3，判断地址模式
        if((temp&0X01)==0)			        //如果不是4字节地址模式,则进入4字节地址模式
        {
            W25Q64_NSS_L(); 			        //选中
            W25QXX_delay_us(3);	
            W25QXX_SPI_RWByte(W25X_Enable4ByteAddr);//发送进入4字节地址模式指令   
            W25Q64_NSS_H();       		        //取消片选   
        }
    }
} 

//读取SPI FLASH  
//在指定地址开始读取指定长度的数据
//pBuffer:数据存储区
//ReadAddr:开始读取的地址(24bit)
//NumByteToRead:要读取的字节数(最大65535)
void W25QXX_Read(uint8_t* pBuffer, uint16_t SecAddr, uint16_t Addr, uint16_t NumByteToRead)   
{ 
 	uint16_t i;  
    
    i = SecAddr&0x0F;
    Addr |= i << 12;
    SecAddr = SecAddr>>4;    
    
	W25Q64_NSS_L();                            //使能器件   
	W25QXX_delay_us(3);	
	W25QXX_SPI_RWByte(W25X_ReadData);      //发送读取命令  
	if(W25QXX_TYPE==W25Q256)                //如果是W25Q256的话地址为4字节的，要发送最高8位
	{
		W25QXX_SPI_RWByte((uint8_t)((SecAddr)>>8));    
	}
	W25QXX_SPI_RWByte((uint8_t)SecAddr);   //发送24bit地址    
	W25QXX_SPI_RWByte((uint8_t)((Addr)>>8));   
	W25QXX_SPI_RWByte((uint8_t)Addr);   
	for(i=0;i<NumByteToRead;i++)
	{ 
		pBuffer[i]=W25QXX_SPI_RWByte(0XFF);    //循环读数  
	}
	W25Q64_NSS_H();  
}  

//擦除一个扇区
//Dst_Addr:扇区地址 根据实际容量设置   W25Q64 : 0 - 2047   0x0-0x7FF000  低12位为0
//擦除一个扇区的最少时间:150ms
void W25QXX_Erase_Sector(uint16_t SecAddr, uint16_t Addr)   
{  
 	uint16_t i;  
    
    i = SecAddr&0x0F;
    Addr |= i << 12;
    SecAddr = SecAddr>>4;
    
	//监视falsh擦除情况,测试用   
 	//printf("fe:%x\r\n",Dst_Addr);	  
    W25QXX_Write_Enable();                  //SET WEL 	 
    W25QXX_Wait_Busy();   
  	W25Q64_NSS_L();                            //使能器件 
    W25QXX_delay_us(3);	  
    W25QXX_SPI_RWByte(W25X_SectorErase);   //发送扇区擦除指令 
    if(W25QXX_TYPE==W25Q256)                //如果是W25Q256的话地址为4字节的，要发送最高8位
    {
        W25QXX_SPI_RWByte((uint8_t)((SecAddr)>>8)); 
    }
    W25QXX_SPI_RWByte((uint8_t)SecAddr);  //发送24bit地址    
    W25QXX_SPI_RWByte((uint8_t)((Addr)>>8));   
    W25QXX_SPI_RWByte((uint8_t)Addr);  
	W25Q64_NSS_H();                            //取消片选     	      
    W25QXX_Wait_Busy();   				    //等待擦除完成
    W25QXX_Write_Disable();
} 

//擦除整个芯片		  
//等待时间超长...
void W25QXX_Erase_Chip(void)   
{                                   
    W25QXX_Write_Enable();                    //SET WEL 
    W25QXX_Wait_Busy();   
  	W25Q64_NSS_L();                           //使能器件   
	W25QXX_delay_us(3);	
    W25QXX_SPI_RWByte(W25X_ChipErase);        //发送片擦除命令  
    W25Q64_NSS_H();                           //取消片选     	      
    W25QXX_Wait_Busy();   				      //等待芯片擦除结束
    W25QXX_Write_Disable();
} 

//进入掉电模式
void W25QXX_PowerDown(void)   
{ 
  	W25Q64_NSS_L();                        //使能器件  
    W25QXX_delay_us(3);		
    W25QXX_SPI_RWByte(W25X_PowerDown);     //发送掉电命令  
	W25Q64_NSS_H();                        //取消片选     	      
    W25QXX_delay_us(3);                    //等待TPD  
}   

//唤醒
void W25QXX_WAKEUP(void)   
{  
  	W25Q64_NSS_L();                            //使能器件  
    W25QXX_delay_us(3);		
    W25QXX_SPI_RWByte(W25X_ReleasePowerDown);  //  send W25X_PowerDown command 0xAB    
	W25Q64_NSS_H();                            //取消片选     	      
    W25QXX_delay_us(3);                        //等待TRES1
}   

//CAN to CAN 应用
uint8_t BackupData_PowerON_Clear = 0;    //上电检测是否需要清除存储数据   0：正常   1：需要清除数据
uint8_t BackupData_Flag[TC_BACKUP_COUNT] = {0}; //存储标志，0：未存满，1：已存满
uint8_t BackupData_State[TC_BACKUP_COUNT] = {0};//备份数据状态  0：前十分钟   1：后十分钟 2:存储结束
uint16_t BackupData_Cnt[TC_BACKUP_COUNT] = {0}; //存储计数，即存储位置
const uint16_t TC_X_Base[TC_BACKUP_COUNT] = {0,50,100,150,200,250,300,350,400,450,500,550,600,650,700,750  ,800, 850,900,950};

//探测器总数为16，第19个为C2数据存储
void CTC_BackupData_Init(void)
{
    uint8_t pBuffer[4] = {0};
    uint8_t i;
    uint8_t datah=0,datal=0;
    uint16_t datapos = 0;
    W25QXX_Init();
    
    //W25QXX_Erase_Chip();
    
    BackupData_PowerON_Clear = 0;
    for(i=0;i<TC_BACKUP_COUNT;i++)
    {
        W25QXX_Read(pBuffer, TC_X_Base[i]+TC_B3_DATA_OFFSET, 0, 2);  
        if((pBuffer[0] == 0xA5)&&(pBuffer[1] == 0x5A))//需要擦除，不可存储
        {
            BackupData_PowerON_Clear = 1; //需要擦除
            BackupData_State[i] = 2;      //存储结束，需要擦除
        }
        else 
        {
            datah = eeprom_read(0x20+ 2*i);
            datal = eeprom_read(0x20+ 2*i +1);
            datapos = (datah<<8)|datal;          
            if((datapos == 0xFFFF)||(datapos == 0x0))
            {
                W25QXX_Erase_Sector(TC_X_Base[i]+TC_BEFORE_DATA_OFFSET, 0); //擦除前10分钟数据记录起始扇区，上电清零数据    
                BackupData_Cnt[i] = 0;
                BackupData_Flag[i] = 0;
                BackupData_State[i] = 0;                               
            }
            else
            {
                BackupData_Cnt[i] = datapos;
                BackupData_Flag[i] = 0;
                BackupData_State[i] = 0;                   
            }
        }       
    }
}

void CTC_BackupData_Write(uint16_t base_addr, uint16_t *bud_cnt, uint8_t* pBuffer, uint8_t len)
{
    uint8_t sec_index, rec_posi;
    uint8_t sec_index_new;
    
    sec_index = (*bud_cnt) / REC_SECTION_COUNT; //所在扇区
    rec_posi  = (*bud_cnt) % REC_SECTION_COUNT;  //所在扇区内的位置
    
    W25QXX_Write_Page(pBuffer, base_addr + sec_index, REC_SIZE_MAX*rec_posi, len);
    
    (*bud_cnt)++;

    sec_index_new = (*bud_cnt) / REC_SECTION_COUNT; //所在扇区
    
    if(sec_index != sec_index_new)//扇区地址改变
    {
        W25QXX_Erase_Sector(base_addr + sec_index_new, 0);//擦除新的扇区
    }
}

//前10分钟数据存储
void CTC_BackupData_Before_Write(uint8_t TC_x, uint8_t* pBuffer, uint8_t len)
{ 
    uint8_t dat_h = 0, dat_l =0;
    
    CTC_BackupData_Write(TC_X_Base[TC_x] + TC_BEFORE_DATA_OFFSET, &BackupData_Cnt[TC_x],  pBuffer, len);
    
    dat_h = (BackupData_Cnt[TC_x]>>8)&0xFF;
    dat_l = BackupData_Cnt[TC_x]&0xFF;
      
    eeprom_write(0x20+2*TC_x, dat_h);
    eeprom_write(0x20+2*TC_x+1, dat_l);
      
    if(BackupData_Cnt[TC_x] >= REC_MAX_COUNT_BEFORE)
    {
        BackupData_Cnt[TC_x]  = 0;
        BackupData_Flag[TC_x] = 1;
        W25QXX_Erase_Sector(TC_X_Base[TC_x]+TC_BEFORE_DATA_OFFSET, 0); //擦除前10分钟数据记录起始扇区，上电清零数据
    }
}

//后10分钟数据存储
void CTC_BackupData_Last_Write(uint8_t TC_x, uint8_t* pBuffer, uint8_t len)
{
    CTC_BackupData_Write(TC_X_Base[TC_x] + TC_LAST_DATA_OFFSET, &BackupData_Cnt[TC_x],  pBuffer, len);
    if(BackupData_Cnt[TC_x] >= REC_MAX_COUNT)
    {
        BackupData_State[TC_x] = 2;//结束存储动作，不再存储
        return;
    }
}

void CTC_BackupData_B3_Write(uint8_t TC_x, uint8_t* pBuffer, uint8_t len)
{
    uint8_t buff[32] = {0};  
      
    W25QXX_Erase_Sector(TC_X_Base[TC_x]+TC_B3_DATA_OFFSET, 0);     //擦除B3数据记录扇区    
    
    buff[0] = 0xA5;//已写入数据标志
    buff[1] = 0x5A;
    buff[2] = BackupData_Flag[TC_x];//存储标志，0：未存满，1：已存满  
    buff[3] = (BackupData_Cnt[TC_x]>>8)&0xFF;//存储位置
    buff[4] = BackupData_Cnt[TC_x]&0xFF;
    
    buff[5] = pBuffer[0];
    buff[6] = pBuffer[1];
    buff[7] = pBuffer[2];
    buff[8] = pBuffer[3];
    buff[9] = pBuffer[4];
    buff[10] = pBuffer[5];
    buff[11] = pBuffer[6];
    buff[12] = pBuffer[7];
    
    buff[13] = pBuffer[8];
    buff[14] = pBuffer[9];
    buff[15] = pBuffer[10];
    buff[16] = pBuffer[11];
    buff[17] = pBuffer[12];
    buff[18] = pBuffer[13];
    buff[19] = pBuffer[14];
    buff[20] = pBuffer[15];
    
    W25QXX_Write_Page(buff, TC_X_Base[TC_x]+TC_B3_DATA_OFFSET, 0, 5+len);   
    
    BackupData_State[TC_x] = 1;//进入后10分钟存储数据
    BackupData_Cnt[TC_x] = 0;  //开始10分钟数据存储    

    W25QXX_Erase_Sector(TC_X_Base[TC_x]+TC_LAST_DATA_OFFSET, 0);   //擦除后10分钟数据记录起始扇区    
}

//读取数据
uint32_t BackUpData_Send_ID = 0x18FFFF00; //存储数据发送报文ID

void CTC_BackupData_Read(uint16_t base_addr, uint16_t start, uint16_t end)
{
    uint16_t i;
    uint8_t pBuffer[16] = {0}; 
    uint8_t section_index, recode_index;

    for(i=start; i<end; i++)
    {
        section_index = i / REC_SECTION_COUNT; //所在扇区
        recode_index  = i % REC_SECTION_COUNT;  //所在扇区内的位置           
        W25QXX_Read(pBuffer, base_addr + section_index, REC_SIZE_MAX*recode_index, 16);       
        CAN_Send_Data(BackUpData_Send_ID, 8, pBuffer);
        delay_10ms(1);
        CAN_Send_Data(BackUpData_Send_ID, 8, &pBuffer[8]);
        delay_10ms(1);
    }
}
void CTC_BackupData_B3_Read(uint16_t base_addr, uint16_t start, uint16_t end)
{
    uint16_t i;
    uint8_t pBuffer[32] = {0}; 
    uint8_t section_index, recode_index;

    for(i=start; i<end; i++)
    {
        section_index = i / REC_SECTION_COUNT; //所在扇区
        recode_index  = i % REC_SECTION_COUNT;  //所在扇区内的位置           
        W25QXX_Read(pBuffer, base_addr + section_index, REC_SIZE_MAX*recode_index, 32);
        if((pBuffer[0] == 0xFF)&&(pBuffer[1] == 0xFF)&&(pBuffer[2] == 0xFF))
        {
            break;
        }       
        CAN_Send_Data(BackUpData_Send_ID, 8, &pBuffer[5]);
        delay_10ms(1);
        CAN_Send_Data(BackUpData_Send_ID, 8, &pBuffer[13]);
        delay_10ms(1);
    }
}

void CTC_BackupData_Buffer_Read(uint8_t TC_x)
{
    uint8_t buffer[16] = {0};
    
    BackUpData_Send_ID = 0x18FFFF00;
    
    if(TC_x==HOST_C2_DATA_ID)
    {
        BackUpData_Send_ID |= 0xC2;
    }
    else 
    {
        BackUpData_Send_ID |= (TC_x+1);
    }

    W25QXX_Read(buffer, TC_X_Base[TC_x]+TC_B3_DATA_OFFSET, 0, 16); 
    
    if((buffer[0] == 0xA5)&&(buffer[1] == 0x5A))
    {   
        CTC_BackupData_Read(TC_X_Base[TC_x]+TC_BEFORE_DATA_OFFSET, 0, REC_MAX_COUNT_BEFORE); 
        //获取B3数据
        CTC_BackupData_B3_Read(TC_X_Base[TC_x]+TC_B3_DATA_OFFSET, 0, 1); 
        
        CTC_BackupData_Read(TC_X_Base[TC_x]+TC_LAST_DATA_OFFSET, 0, REC_MAX_COUNT); 
    }
    else 
    {
        CTC_BackupData_Read(TC_X_Base[TC_x]+TC_BEFORE_DATA_OFFSET, 0, REC_MAX_COUNT_BEFORE); 
    }
}


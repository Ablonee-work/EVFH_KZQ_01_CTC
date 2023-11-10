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

/***************ͷ�ļ�****************/
/**/
#include "w25qxx.h"

/*************************************/

/******************�궨��*************/
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

/***************��־λ����************/
/**/
static uint16_t W25QXX_TYPE = 0;	
/*************************************/

/***************��������**************/
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

//W25Q64����(��λ���ֽ�(byte))
//һ��page��256��, 16��pageΪһ��Sector
//4KΪһ��Sector
//16��SectorΪ1��Block��64K��
//����Ϊ8M�ֽ�,����128��Block,8192��Sector 
//��ַ��Χ��0x0 - 0x7FFFFF

static void SPI_Init(void)
{
    W25Q64_SCK_Port  = 0;   
    W25Q64_MISO_Port = 1; 
    W25Q64_MOSI_Port = 0;      
    W25Q64_NSS_Port  = 0;          
}

//SPI��д����
static uint8_t W25QXX_SPI_RWByte(uint8_t TxData)
{
	uint8_t bit_ctr;
	for(bit_ctr=0;bit_ctr<8;bit_ctr++)  // ���8λ
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

//��ȡW25QXX��״̬�Ĵ�����W25QXXһ����3��״̬�Ĵ���
//״̬�Ĵ���1��
//BIT7  6   5   4   3   2   1   0
//SPR   RV  TB BP2 BP1 BP0 WEL BUSY
//SPR:Ĭ��0,״̬�Ĵ�������λ,���WPʹ��
//TB,BP2,BP1,BP0:FLASH����д��������
//WEL:дʹ������
//BUSY:æ���λ(1,æ;0,����)
//Ĭ��:0x00
//״̬�Ĵ���2��
//BIT7  6   5   4   3   2   1   0
//SUS   CMP LB3 LB2 LB1 (R) QE  SRP1
//״̬�Ĵ���3��
//BIT7      6    5    4   3   2   1   0
//HOLD/RST  DRV1 DRV0 (R) (R) WPS ADP ADS
//regno:״̬�Ĵ����ţ���:1~3
//����ֵ:״̬�Ĵ���ֵ
static uint8_t W25QXX_ReadSR(uint8_t regno)   
{  
	uint8_t byte=0,command=0; 
    switch(regno)
    {
        case 1:
            command=W25X_ReadStatusReg1;    //��״̬�Ĵ���1ָ��
            break;
        case 2:
            command=W25X_ReadStatusReg2;    //��״̬�Ĵ���2ָ��
            break;
        case 3:
            command=W25X_ReadStatusReg3;    //��״̬�Ĵ���3ָ��
            break;
        default:
            command=W25X_ReadStatusReg1;    
            break;
    }    
	W25Q64_NSS_L();                            //ʹ������   
	W25QXX_delay_us(3);	
	W25QXX_SPI_RWByte(command);            //���Ͷ�ȡ״̬�Ĵ�������    
	byte=W25QXX_SPI_RWByte(0Xff);          //��ȡһ���ֽ�  
	W25Q64_NSS_H();                            //ȡ��Ƭѡ     
	return byte;   
} 

////дW25QXX״̬�Ĵ���
//static void W25QXX_Write_SR(uint8_t regno,uint8_t sr)   
//{   
//    uint8_t command=0;
//    switch(regno)
//    {
//        case 1:
//            command=W25X_WriteStatusReg1;    //д״̬�Ĵ���1ָ��
//            break;
//        case 2:
//            command=W25X_WriteStatusReg2;    //д״̬�Ĵ���2ָ��
//            break;
//        case 3:
//            command=W25X_WriteStatusReg3;    //д״̬�Ĵ���3ָ��
//            break;
//        default:
//            command=W25X_WriteStatusReg1;    
//            break;
//    }   
//	W25Q64_NSS_L();                            //ʹ������   
//	W25QXX_SPI_RWByte(command);            //����дȡ״̬�Ĵ�������    
//	W25QXX_SPI_RWByte(sr);                 //д��һ���ֽ�  
//	W25Q64_NSS_H();                            //ȡ��Ƭѡ     	      
//}   

//W25QXXдʹ��	
//��WEL��λ   
static void W25QXX_Write_Enable(void)   
{
	W25Q64_NSS_L();                        //ʹ������
	W25QXX_delay_us(1);		
    W25QXX_SPI_RWByte(W25X_WriteEnable);   //����дʹ��  
	W25Q64_NSS_H();                        //ȡ��Ƭѡ     	      
} 

//W25QXXд��ֹ	
//��WEL����  
static void W25QXX_Write_Disable(void)   
{  
	W25Q64_NSS_L();                            //ʹ������   
	W25QXX_delay_us(1);	
  W25QXX_SPI_RWByte(W25X_WriteDisable);  //����д��ָֹ��    
	W25Q64_NSS_H();                            //ȡ��Ƭѡ     	      
} 

//�ȴ�����
static void W25QXX_Wait_Busy(void)   
{   
	while((W25QXX_ReadSR(1)&0x01)==0x01);   // �ȴ�BUSYλ���
}  

//��ȡоƬID
//����ֵ����:				   
//0XEF13,��ʾоƬ�ͺ�ΪW25Q80  
//0XEF14,��ʾоƬ�ͺ�ΪW25Q16    
//0XEF15,��ʾоƬ�ͺ�ΪW25Q32  
//0XEF16,��ʾоƬ�ͺ�ΪW25Q64 
//0XEF17,��ʾоƬ�ͺ�ΪW25Q128 	  
//0XEF18,��ʾоƬ�ͺ�ΪW25Q256
static uint16_t W25QXX_ReadID(void)
{
	uint16_t Temp = 0;	  
	W25Q64_NSS_L();				    
	W25QXX_delay_us(3);	
	W25QXX_SPI_RWByte(0x90);//���Ͷ�ȡID����	    
	W25QXX_SPI_RWByte(0x00); 	    
	W25QXX_SPI_RWByte(0x00); 	    
	W25QXX_SPI_RWByte(0x00); 	 			   
	Temp|=W25QXX_SPI_RWByte(0xFF)<<8;  
	Temp|=W25QXX_SPI_RWByte(0xFF);	 
	W25Q64_NSS_H();				    
	return Temp;
}   		    

//SPI��һҳ(0~65535)��д������256���ֽڵ�����
//��ָ����ַ��ʼд�����256�ֽڵ�����
//pBuffer:���ݴ洢��
//WriteAddr:��ʼд��ĵ�ַ(24bit)
//NumByteToWrite:Ҫд����ֽ���(���256),������Ӧ�ó�����ҳ��ʣ���ֽ���!!!	 
static void W25QXX_Write_Page(uint8_t* pBuffer, uint16_t SecAddr, uint16_t Addr, uint16_t NumByteToWrite)
{
 	uint16_t i;  
    
    i = SecAddr&0x0F;
    Addr |= i << 12;
    SecAddr = SecAddr>>4;
    
	W25QXX_Write_Enable();                  //SET WEL 
	W25Q64_NSS_L();                            //ʹ������   
	W25QXX_delay_us(3);	
	W25QXX_SPI_RWByte(W25X_PageProgram);   //����дҳ����   
	if(W25QXX_TYPE==W25Q256)                //�����W25Q256�Ļ���ַΪ4�ֽڵģ�Ҫ�������8λ
	{
			W25QXX_SPI_RWByte((uint8_t)((SecAddr)>>8)); 
	}
	W25QXX_SPI_RWByte((uint8_t)SecAddr); //����24bit��ַ    
	W25QXX_SPI_RWByte((uint8_t)((Addr)>>8));   
	W25QXX_SPI_RWByte((uint8_t)Addr);   
	for(i=0;i<NumByteToWrite;i++)
    {
       W25QXX_SPI_RWByte(pBuffer[i]);//ѭ��д��    
    }        
	W25Q64_NSS_H();                            //ȡ��Ƭѡ 
	W25QXX_Wait_Busy();					   //�ȴ�д�����
	W25QXX_Write_Disable();
} 

//Ӧ��
//��ʼ��SPI FLASH��IO��
void W25QXX_Init(void)
{ 
    uint8_t temp;
    SPI_Init();		   			        //��ʼ��SPI
    W25Q64_NSS_H();			            //SPI FLASH��ѡ��
    
    W25QXX_TYPE = W25QXX_ReadID();	      //��ȡFLASH ID.
    if(W25QXX_TYPE == W25Q256)            //SPI FLASHΪW25Q256
    {
        temp=W25QXX_ReadSR(3);              //��ȡ״̬�Ĵ���3���жϵ�ַģʽ
        if((temp&0X01)==0)			        //�������4�ֽڵ�ַģʽ,�����4�ֽڵ�ַģʽ
        {
            W25Q64_NSS_L(); 			        //ѡ��
            W25QXX_delay_us(3);	
            W25QXX_SPI_RWByte(W25X_Enable4ByteAddr);//���ͽ���4�ֽڵ�ַģʽָ��   
            W25Q64_NSS_H();       		        //ȡ��Ƭѡ   
        }
    }
} 

//��ȡSPI FLASH  
//��ָ����ַ��ʼ��ȡָ�����ȵ�����
//pBuffer:���ݴ洢��
//ReadAddr:��ʼ��ȡ�ĵ�ַ(24bit)
//NumByteToRead:Ҫ��ȡ���ֽ���(���65535)
void W25QXX_Read(uint8_t* pBuffer, uint16_t SecAddr, uint16_t Addr, uint16_t NumByteToRead)   
{ 
 	uint16_t i;  
    
    i = SecAddr&0x0F;
    Addr |= i << 12;
    SecAddr = SecAddr>>4;    
    
	W25Q64_NSS_L();                            //ʹ������   
	W25QXX_delay_us(3);	
	W25QXX_SPI_RWByte(W25X_ReadData);      //���Ͷ�ȡ����  
	if(W25QXX_TYPE==W25Q256)                //�����W25Q256�Ļ���ַΪ4�ֽڵģ�Ҫ�������8λ
	{
		W25QXX_SPI_RWByte((uint8_t)((SecAddr)>>8));    
	}
	W25QXX_SPI_RWByte((uint8_t)SecAddr);   //����24bit��ַ    
	W25QXX_SPI_RWByte((uint8_t)((Addr)>>8));   
	W25QXX_SPI_RWByte((uint8_t)Addr);   
	for(i=0;i<NumByteToRead;i++)
	{ 
		pBuffer[i]=W25QXX_SPI_RWByte(0XFF);    //ѭ������  
	}
	W25Q64_NSS_H();  
}  

//����һ������
//Dst_Addr:������ַ ����ʵ����������   W25Q64 : 0 - 2047   0x0-0x7FF000  ��12λΪ0
//����һ������������ʱ��:150ms
void W25QXX_Erase_Sector(uint16_t SecAddr, uint16_t Addr)   
{  
 	uint16_t i;  
    
    i = SecAddr&0x0F;
    Addr |= i << 12;
    SecAddr = SecAddr>>4;
    
	//����falsh�������,������   
 	//printf("fe:%x\r\n",Dst_Addr);	  
    W25QXX_Write_Enable();                  //SET WEL 	 
    W25QXX_Wait_Busy();   
  	W25Q64_NSS_L();                            //ʹ������ 
    W25QXX_delay_us(3);	  
    W25QXX_SPI_RWByte(W25X_SectorErase);   //������������ָ�� 
    if(W25QXX_TYPE==W25Q256)                //�����W25Q256�Ļ���ַΪ4�ֽڵģ�Ҫ�������8λ
    {
        W25QXX_SPI_RWByte((uint8_t)((SecAddr)>>8)); 
    }
    W25QXX_SPI_RWByte((uint8_t)SecAddr);  //����24bit��ַ    
    W25QXX_SPI_RWByte((uint8_t)((Addr)>>8));   
    W25QXX_SPI_RWByte((uint8_t)Addr);  
	W25Q64_NSS_H();                            //ȡ��Ƭѡ     	      
    W25QXX_Wait_Busy();   				    //�ȴ��������
    W25QXX_Write_Disable();
} 

//��������оƬ		  
//�ȴ�ʱ�䳬��...
void W25QXX_Erase_Chip(void)   
{                                   
    W25QXX_Write_Enable();                    //SET WEL 
    W25QXX_Wait_Busy();   
  	W25Q64_NSS_L();                           //ʹ������   
	W25QXX_delay_us(3);	
    W25QXX_SPI_RWByte(W25X_ChipErase);        //����Ƭ��������  
    W25Q64_NSS_H();                           //ȡ��Ƭѡ     	      
    W25QXX_Wait_Busy();   				      //�ȴ�оƬ��������
    W25QXX_Write_Disable();
} 

//�������ģʽ
void W25QXX_PowerDown(void)   
{ 
  	W25Q64_NSS_L();                        //ʹ������  
    W25QXX_delay_us(3);		
    W25QXX_SPI_RWByte(W25X_PowerDown);     //���͵�������  
	W25Q64_NSS_H();                        //ȡ��Ƭѡ     	      
    W25QXX_delay_us(3);                    //�ȴ�TPD  
}   

//����
void W25QXX_WAKEUP(void)   
{  
  	W25Q64_NSS_L();                            //ʹ������  
    W25QXX_delay_us(3);		
    W25QXX_SPI_RWByte(W25X_ReleasePowerDown);  //  send W25X_PowerDown command 0xAB    
	W25Q64_NSS_H();                            //ȡ��Ƭѡ     	      
    W25QXX_delay_us(3);                        //�ȴ�TRES1
}   

//CAN to CAN Ӧ��
uint8_t BackupData_PowerON_Clear = 0;    //�ϵ����Ƿ���Ҫ����洢����   0������   1����Ҫ�������
uint8_t BackupData_Flag[TC_BACKUP_COUNT] = {0}; //�洢��־��0��δ������1���Ѵ���
uint8_t BackupData_State[TC_BACKUP_COUNT] = {0};//��������״̬  0��ǰʮ����   1����ʮ���� 2:�洢����
uint16_t BackupData_Cnt[TC_BACKUP_COUNT] = {0}; //�洢���������洢λ��
const uint16_t TC_X_Base[TC_BACKUP_COUNT] = {0,50,100,150,200,250,300,350,400,450,500,550,600,650,700,750  ,800, 850,900,950};

//̽��������Ϊ16����19��ΪC2���ݴ洢
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
        if((pBuffer[0] == 0xA5)&&(pBuffer[1] == 0x5A))//��Ҫ���������ɴ洢
        {
            BackupData_PowerON_Clear = 1; //��Ҫ����
            BackupData_State[i] = 2;      //�洢��������Ҫ����
        }
        else 
        {
            datah = eeprom_read(0x20+ 2*i);
            datal = eeprom_read(0x20+ 2*i +1);
            datapos = (datah<<8)|datal;          
            if((datapos == 0xFFFF)||(datapos == 0x0))
            {
                W25QXX_Erase_Sector(TC_X_Base[i]+TC_BEFORE_DATA_OFFSET, 0); //����ǰ10�������ݼ�¼��ʼ�������ϵ���������    
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
    
    sec_index = (*bud_cnt) / REC_SECTION_COUNT; //��������
    rec_posi  = (*bud_cnt) % REC_SECTION_COUNT;  //���������ڵ�λ��
    
    W25QXX_Write_Page(pBuffer, base_addr + sec_index, REC_SIZE_MAX*rec_posi, len);
    
    (*bud_cnt)++;

    sec_index_new = (*bud_cnt) / REC_SECTION_COUNT; //��������
    
    if(sec_index != sec_index_new)//������ַ�ı�
    {
        W25QXX_Erase_Sector(base_addr + sec_index_new, 0);//�����µ�����
    }
}

//ǰ10�������ݴ洢
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
        W25QXX_Erase_Sector(TC_X_Base[TC_x]+TC_BEFORE_DATA_OFFSET, 0); //����ǰ10�������ݼ�¼��ʼ�������ϵ���������
    }
}

//��10�������ݴ洢
void CTC_BackupData_Last_Write(uint8_t TC_x, uint8_t* pBuffer, uint8_t len)
{
    CTC_BackupData_Write(TC_X_Base[TC_x] + TC_LAST_DATA_OFFSET, &BackupData_Cnt[TC_x],  pBuffer, len);
    if(BackupData_Cnt[TC_x] >= REC_MAX_COUNT)
    {
        BackupData_State[TC_x] = 2;//�����洢���������ٴ洢
        return;
    }
}

void CTC_BackupData_B3_Write(uint8_t TC_x, uint8_t* pBuffer, uint8_t len)
{
    uint8_t buff[32] = {0};  
      
    W25QXX_Erase_Sector(TC_X_Base[TC_x]+TC_B3_DATA_OFFSET, 0);     //����B3���ݼ�¼����    
    
    buff[0] = 0xA5;//��д�����ݱ�־
    buff[1] = 0x5A;
    buff[2] = BackupData_Flag[TC_x];//�洢��־��0��δ������1���Ѵ���  
    buff[3] = (BackupData_Cnt[TC_x]>>8)&0xFF;//�洢λ��
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
    
    BackupData_State[TC_x] = 1;//�����10���Ӵ洢����
    BackupData_Cnt[TC_x] = 0;  //��ʼ10�������ݴ洢    

    W25QXX_Erase_Sector(TC_X_Base[TC_x]+TC_LAST_DATA_OFFSET, 0);   //������10�������ݼ�¼��ʼ����    
}

//��ȡ����
uint32_t BackUpData_Send_ID = 0x18FFFF00; //�洢���ݷ��ͱ���ID

void CTC_BackupData_Read(uint16_t base_addr, uint16_t start, uint16_t end)
{
    uint16_t i;
    uint8_t pBuffer[16] = {0}; 
    uint8_t section_index, recode_index;

    for(i=start; i<end; i++)
    {
        section_index = i / REC_SECTION_COUNT; //��������
        recode_index  = i % REC_SECTION_COUNT;  //���������ڵ�λ��           
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
        section_index = i / REC_SECTION_COUNT; //��������
        recode_index  = i % REC_SECTION_COUNT;  //���������ڵ�λ��           
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
        //��ȡB3����
        CTC_BackupData_B3_Read(TC_X_Base[TC_x]+TC_B3_DATA_OFFSET, 0, 1); 
        
        CTC_BackupData_Read(TC_X_Base[TC_x]+TC_LAST_DATA_OFFSET, 0, REC_MAX_COUNT); 
    }
    else 
    {
        CTC_BackupData_Read(TC_X_Base[TC_x]+TC_BEFORE_DATA_OFFSET, 0, REC_MAX_COUNT_BEFORE); 
    }
}


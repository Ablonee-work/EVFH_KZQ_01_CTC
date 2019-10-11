/*----------------------------------------------------------------------*/
/* Confidential and Proprietary Information of MicroPerfect Corporation */
/* (C) 2017 ,MicroPerfect Corporation . All rights reserved. */
/* FileName : can.c */
/* Function : */
/* Ver: 0.01  */
/* MP_Ahren NewFile */
/* History:YYYY.MM.DD Version author Description 
          :2017.12.04 0.01 @huajifeng Follow the coding standard  */
/*-------------------------------------------------------------*/

/***************ͷ�ļ�****************/
/**/
/*************************************/
#include "can.h"

//******************�궨��*************/
/**/
/*************************************/


/***************��������************/
/**/
/*************************************/

static u8 Send_DLC = 0;

/***************��������**************/
/**/
/*************************************/

static void Clear_CAN_TX0_Buf(unsigned char CAN_TX0_Data);    //����CAN_TX0_Bufֵ
static void Clear_CAN_RX0_Buf(unsigned char CAN_RX0_Data);    //����CAN_TX0_Bufֵ
static void CAN_Port_Set(void);                               //CAN I/O ���ݷ�������
static void CAN_Baud_Rate_Set(void);                          //���������ú���
static void CAN_Interrupt_Mode_Set(void);     //   ��ʼ��CAN�жϡ�CANģ�����ģʽѡ��
static void CAN_TXD_Extended_Identifier(void);//  ������չ��ʶ�����ã�������ID��
static void CAN_RXD_Extended_Identifier(void);//  ������չ��ʶ�����á�������չ��ʶ����������
static void CAN_Config_Mode(void);
/*************************************/

/*************************************************************************/
//������:Clear_CAN_TX0_Buf
//��  ��:����CAN_TX0_Bufֵ
//��  ��:CAN_TX0_Data
//����ֵ����
//ʱ�䣺20171123
//�����ߣ�ahren
/************************************************************************/
static void Clear_CAN_TX0_Buf(unsigned char CAN_TX0_Data)
{	
	unsigned char Count = 0;
	
	unsigned char Data  = 0;
	
	Data = CAN_TX0_Data;
	
	for(Count=0; Count<9; Count++)
	{
	  	CAN_TX0_Buff[Count]= Data; 						//��CAN_TX0_Buf��ֵ��Ӧ��ֵ
	}
}

/*************************************************************************/
//������:Clear_CAN_TX0_Buf_New
//��  ��:����CAN_TX0_Bufֵ
//��  ��:CAN_TX0_Data��Send_Num
//����ֵ����
//ʱ�䣺20171123
//�����ߣ�ahren
/************************************************************************/
void Clear_CAN_TX0_Buf_New(unsigned char CAN_TX0_Data,unsigned char Send_Num)
 {	
	unsigned char Count = 0;
 	
 	unsigned char Data  = 0;
 	
 	unsigned char Num   = 0;
 	
 	Data = CAN_TX0_Data;								//CANҪ���͵�����
 	
 	Send_DLC = Send_Num;								//CAN�������ݵĳ���
 	
 	for(Count=0; Count<Send_Num; Count++)
	{
      	CAN_TX0_Buff[Count] = Data; 					//��CAN_TX0_Buf��ֵ��Ӧ��ֵ
	}
}

 /*************************************************************************/
//������:Clear_CAN_RX0_Buf
//��  ��:����CAN_RX0_Bufֵ
//��  ��:CAN_RX0_Data
//����ֵ����
//ʱ�䣺20171123
//�����ߣ�ahren
/************************************************************************/
static void Clear_CAN_RX0_Buf(unsigned char CAN_RX0_Data) 
 {
	unsigned char Count = 0;
 	
 	unsigned char Data  = 0;
 	
 	Data = CAN_RX0_Data;	
 	
 	for(Count=0; Count<9; Count++)
	{
      	CAN_RX0_Buff[Count] = Data; 					//��CAN_RX0_Buf��ֵ��Ӧ��ֵ
	}
}
 
 /*************************************************************************/
//������:CAN_Send_Receive_TXD_ID_Set
//��  ��:���ý��չ�����IDֵ
//��  ��:Channel_Flag = 1ʱ������ѡ�����÷��͹�����0
//��  ��:Channel_Flag = 2ʱ������ѡ�����÷��͹�����1
//��  ��:Channel_Flag = 3ʱ������ѡ�����ý��չ�����1
//��  ��:RX_TX_SID_H�Ĵ���SID�߰�λ
//��  ��:RX_TX_SID_L�Ĵ���SID�Ͱ�λ
//��  ��:RX_TX_EID_H�Ĵ���EID�߰�λ
//��  ��:RX_TX_EID_L�Ĵ���EID�Ͱ�λ
//����ֵ����
//ʱ�䣺20171123
//�����ߣ�ahren
/************************************************************************/
static void CAN_Send_Receive_TXD_ID_Set(u8 Channel_Flag,u8 RX_TX_SID_H,u8 RX_TX_SID_L,u8 RX_TX_EID_H,u8 RX_TX_EID_L)
{
	u8 ID_H_Data = 0x00;
	u8 ID_L_Data = 0x00;
	
	u8 ID_L = 0x00;
	u8 ID_H = 0x00;
	
	ID_H_Data = RX_TX_SID_H;
	ID_L_Data = RX_TX_SID_L;
	
	ID_L = (ID_L_Data & 0xF0) >> 5;
	ID_H = (ID_H_Data << 3) + ID_L;				//���ù�����SIDH
	
	ID_L = (ID_L_Data & 0x1C) << 3;
	ID_L = (ID_L_Data & 0x03) + ID_L + 8;		//���ù�����SIDL(��չ֡)	
	
	if(1 == Channel_Flag)						//���ý��չ�����0ID
	{
		RXF0SIDH = ID_H;		
		RXF0SIDL = ID_L;
		RXF0EIDH = RX_TX_EID_H;
	//	RXF0EIDL = RX_TX_EID_L;		
	}
	
	if(2 == Channel_Flag)						//���ý��չ�����1ID
	{
		RXF1SIDH = ID_H;		
		RXF1SIDL = ID_L;
		RXF1EIDH = RX_TX_EID_H;
	//	RXF1EIDL = RX_TX_EID_L;	
	}
	
	if(3 == Channel_Flag)						//���÷��͹�����ID
	{
		TXB0SIDH = ID_H;
		TXB0SIDL = ID_L;
		TXB0EIDH = RX_TX_EID_H;
		TXB0EIDL = RX_TX_EID_L;		
	}				
}

/*************************************************************************/
//������:CAN_Port_Set
//��  ��:CAN I/O ���ݷ�������
//��  ��:��
//����ֵ����
//ʱ�䣺20171123
//�����ߣ�ahren
/************************************************************************/
static void CAN_Port_Set(void)
{
    TRISB5 = 0;			             //RG4�˿����			
	LATB5 = 1;						//����CANʹ��PIN
    
	TRISE5 = 0;						//CANTX/RE5Ϊ���
		
	TRISE4 = 1;						//CANRX/RE4Ϊ����
}

/*************************************************************************/
//������:CAN_Baud_Rate_Set
//��  ��:CAN����������(���üĴ���)
//��  ��:��
//����ֵ����
//ʱ�䣺20171123
//�����ߣ�ahren
/************************************************************************/
static void CAN_Baud_Rate_Set(void)
{
 #if 0  
    /****************************************************************/	
	//4M
    /****************************************************************/
 // BRGCON1=0X01;					//Sync_Seg(bit7-6)=1TQ//����SJW��BRP��SJW=1TQ��BRP=0x01,������Ϊ 125Kbps
    BRGCON1=0X00;					//Sync_Seg(bit7-6)=1TQ//����SJW��BRP��SJW=1TQ��BRP=0x00,������Ϊ 250Kbps   
	BRGCON2=0X90;					//����Phase_Seg1=3TQ��Prog_Seg=1TQ	
	BRGCON3=0X42;					//����Phase_Seg2=3TQ
									//TQ=((2*(BRP+1))/Fosc=4/4M=1us 
									//TQ=((2*(BRP+1))/Fosc=2/4M=0.5us 	
									//λʱ�䣽TQ*(Sync_Seg+ Prop_Seg+ Phase_seg1+ Phase_seg2)=(1+1+3+3)TQ��8TQ, 
									//λ��=1/(8*1us)=125Kbps 
									//λ��=1/(8*0.5us)=250Kbps 
									//���÷������� 0 �ı�ʶ���ͷ��͵����ݣ��Լ����͵����ݳ���	
									
	TXB0CON=0X03;					//�������ȼ�Ϊ������ȼ���TXPRI=11
#endif
    /****************************************************************/	
	//8M
    /****************************************************************/
     BRGCON1=0X01;					//Sync_Seg(bit7-6)=1TQ//����SJW��BRP��SJW=1TQ��BRP=0x01,������Ϊ 250Kbps
//    BRGCON1=0X00;					//Sync_Seg(bit7-6)=1TQ//����SJW��BRP��SJW=1TQ��BRP=0x00,������Ϊ 500Kbps   
	BRGCON2=0X90;					//����Phase_Seg1=3TQ��Prog_Seg=1TQ	
	BRGCON3=0X42;					//����Phase_Seg2=3TQ
									//TQ=((2*(BRP+1))/Fosc=4/8M=0.5us 
									//TQ=((2*(BRP+1))/Fosc=2/8M=0.25us 	
									//λʱ�䣽TQ*(Sync_Seg+ Prop_Seg+ Phase_seg1+ Phase_seg2)=(1+1+3+3)TQ��8TQ, 
									//λ��=1/(8*0.5us)=250Kbps 
									//λ��=1/(8*0.25us)=500Kbps 
									//���÷������� 0 �ı�ʶ���ͷ��͵����ݣ��Լ����͵����ݳ���	
									
	TXB0CON=0X03;					//�������ȼ�Ϊ������ȼ���TXPRI=11
}

/*************************************************************************/
//������:CAN_TXD_Extended_Identifier
//��  ��:������չ��ʶ�����ã�������ID��
//��  ��:��
//����ֵ����
//ʱ�䣺20171123
//�����ߣ�ahren
/************************************************************************/
static void CAN_TXD_Extended_Identifier(void)
{
    /****************************************/	
    //���CAN ID���ó�ʼ������                        
    //ahren
    //20190719
    
    u8 CAN_ID_SIDH = 0;
    u8 CAN_ID_SIDL = 0;
    u8 CAN_ID_EIDH = 0;
    u8 CAN_ID_EIDL = 0;    

    CAN_ID_Flag = eeprom_read(0x09);//CANĬ��ID���޸ı�־λ
    NOP();NOP();NOP();NOP();      
    //if(CAN_ID_Flag == 0x5A)
    if(0x5A == CAN_ID_Flag)
    {
        CAN_ID_SIDH = eeprom_read(0x04);                        
        NOP();NOP();NOP();NOP();
        CAN_ID_SIDL = eeprom_read(0x05);                     
        NOP();NOP();NOP();NOP();  
        CAN_ID_EIDH = eeprom_read(0x06);                        
        NOP();NOP();NOP();NOP();
        CAN_ID_EIDL = eeprom_read(0x07);                        
        NOP();NOP();NOP();NOP();    
        
		#if 0
        CAN_Send_Receive_TXD_ID_Set(0x03,CAN_ID_SIDH,CAN_ID_SIDL,CAN_ID_EIDH,CAN_ID_EIDL);  
		#endif
        /****************************************/	
        //��ʼ��MCP2515 ����ID                      
        //ahren
        //20190722
        MCP2515_TxID[0] = CAN_ID_SIDH;
        MCP2515_TxID[1] = CAN_ID_SIDL;
        MCP2515_TxID[2] = CAN_ID_EIDH;
        MCP2515_TxID[3] = CAN_ID_EIDL; 
        /****************************************/	        
    }
    else
    {
        CAN_Send_Receive_TXD_ID_Set(0x03,0x18,0xFF,0x74,0x1D);    
    }
    /*************************************/	  
	TXB0D0=0X00;												
	TXB0D1=0X00;
	TXB0D2=0X00;
	TXB0D3=0X00;
	TXB0D4=0X00;
	TXB0D5=0X00;
	TXB0D6=0X00;
	TXB0D7=0X00;
}

/*************************************************************************/
//������:CAN_RXD_Extended_Identifier
//��  ��:������չ��ʶ�����á�������չ��ʶ����������
//��  ��:
//����ֵ����
//ʱ�䣺20171123
//�����ߣ�ahren
/************************************************************************/
static void CAN_RXD_Extended_Identifier(void)
{
	/****************************************************/
	//������չ��ʶ����������
 	/****************************************************/	

	RXB0CON=0x40;  												// bit6-5=10//��������չ��ʶ������Ч��Ϣ��FILHIT0=0��ʾRXB0 ������FILHIT0      
#if 0
	RXM0SIDH = 0xff;		
	RXM0SIDL = 0xff;											//��3λΪ��ʶ����3λ����5λ������
	RXM0EIDH = 0x00;
	RXM0EIDL = 0x00;	
#endif 
    RXM0SIDH = 0xFF;		
	RXM0SIDL = 0xFF;											//��3λΪ��ʶ����3λ����5λ�����ã� bit3 EXIDEN�������� RXFnSIDL �е� EXIDEN λѡ��ı���
	RXM0EIDH = 0x00;
	RXM0EIDL = 0x00;	
	/****************************************************/
	//������չ��ʶ�����ã�������ID��
 	/****************************************************/ 
 	
 	CAN_Send_Receive_TXD_ID_Set(0x01,0x18,0xFF,0x00,0x00);			//���ý��չ�����0ID
	CAN_Send_Receive_TXD_ID_Set(0x02,0X04,0x02,0x00,0x00);			//���ý��չ�����1ID				
	
	//RXB0DLC = 0X08;											//���ý������ݵĳ���Ϊ1���ֽ�
	RXB0D0=0X00;												//��ʼ�����ܻ�����0������������
	RXB0D1=0X00;
	RXB0D2=0X00;
	RXB0D3=0X00;
	RXB0D4=0X00;
	RXB0D5=0X00;
	RXB0D6=0X00;
	RXB0D7=0X00;	
}

/*************************************************************************/
//������:CAN_Interrupt_Mode_Set
//��  ��:��ʼ��CAN�жϡ�CANģ�����ģʽѡ��
//��  ��: �Ĵ���
//����ֵ����
//ʱ�䣺20171123
//�����ߣ�ahren
/*************************************************************************/
static void CAN_Interrupt_Mode_Set(void)
{
	CIOCON=0X21;												//��ʼ��CANģ���I/O���ƼĴ���	 	
 	
 	CANCON=0X00;												//������������ģʽ
	while((CANSTAT&0x40)!=0); 									//����������	

	PIR5=0X00;													//�������жϱ�־
	PIE5=0X01;													//ʹ�ܽ��ջ�����0��1 �Ľ����жϣ����򿪽����ж�ʹ�ܣ�
}

/*************************************************************************/
//������:CAN_Send_8Byte_Data
//��  ��:CAN���ͺ���
//��  ��:
//����ֵ����
//ʱ�䣺20171123
//�����ߣ�ahren
/************************************************************************/
void CAN_Send_8Byte_Data(void)  							//����0����DLC���ȵı���
{
	TXB0CON = (TXB0CON&0xf7);				//TXREQ��0���ȣ���ֹ����	
	TXB0IF = 0;								//��շ�����ɱ�־		
	TXB0DLC = 0x08;							//���÷������ݵĳ���Ϊ8���ֽ�
	//TXB0DLC = Send_DLC;					//���ͳ��ȸ������ݳ��ȶ���
	
	TXB0D0 = CAN_TX0_Buff[0];				//����ȴ���������
	TXB0D1 = CAN_TX0_Buff[1];
	TXB0D2 = CAN_TX0_Buff[2];
	TXB0D3 = CAN_TX0_Buff[3];
	TXB0D4 = CAN_TX0_Buff[4];
	TXB0D5 = CAN_TX0_Buff[5];
	TXB0D6 = CAN_TX0_Buff[6];
	TXB0D7 = CAN_TX0_Buff[7];
	
	TXB0CON=(TXB0CON|0x08);					//׼����ϣ�������,TXREQ=1
}

/*************************************************************************/
//������:TJA1042_CAN_Send_Data
//��  ��:CAN���ͺ���
//��  ��:Box_Num:�������
//��  ��:Data_Buff:���͵����ݱ���
//��  ��:Data_Leng:���͵����ݳ���
//����ֵ����
//ʱ�䣺20190606
//�����ߣ�ahren
/************************************************************************/
void TJA1042_CAN_Send_Data(u8 Box_Num, u8 *Data_Buff, u8 Data_Leng,u8 Life_Signal)
{
    u8 i = 0;

	TXB0CON = (TXB0CON&0xf7);				//TXREQ��0���ȣ���ֹ����	
	TXB0IF = 0;								//��շ�����ɱ�־		

	TXB0DLC = Data_Leng;							//���÷������ݵĳ���Ϊ8���ֽ�
    
    TXB0D0 = Box_Num;
    //Data_Buff[6] = Life_Signal;
	for(i=1; i<Data_Leng; i++)
	{	
		*(&TXB0D0 + i) = Data_Buff[i];				
	}  
    TXB0D6 = Life_Signal;    
	TXB0CON=(TXB0CON|0x08);					//׼����ϣ�������,TXREQ=1    
}

/*************************************************************************/
//������:CAN_Send_Unfixed_Data
//��  ��:���ݽ��յ����ݳ��ȷ�����Ӧ���ȵ����ݺ���
//��  ��:
//����ֵ����
//ʱ�䣺20171123
//�����ߣ�ahren
/************************************************************************/
void CAN_Send_Unfixed_Data(void)
{
	TXB0CON = (TXB0CON&0xf7);				//TXREQ��0���ȣ���ֹ����	
	TXB0IF = 0;								//��շ�����ɱ�־		
						
	TXB0DLC = RXB0DLC;						//���÷������ݵĳ�������յ������ݳ���һ��
	
	TXB0D0 = CAN_TX0_Buff[0];				//����ȴ���������
	TXB0D1 = CAN_TX0_Buff[1];
	TXB0D2 = CAN_TX0_Buff[2];
	TXB0D3 = CAN_TX0_Buff[3];
	TXB0D4 = CAN_TX0_Buff[4];
	TXB0D5 = CAN_TX0_Buff[5];
	TXB0D6 = CAN_TX0_Buff[6];
	TXB0D7 = CAN_TX0_Buff[7];
	
	TXB0CON =(TXB0CON|0x08);				//׼����ϣ�������,TXREQ=1
}

/*************************************************************************/
//������:CAN_Send_DCL_Data
//��  ��:����DLC���ȵı���
//��  ��:
//����ֵ����
//ʱ�䣺20171123
//�����ߣ�ahren
/************************************************************************/
void CAN_Send_DCL_Data(unsigned char CAN_TX_Buf)  	
{
	unsigned char i;
	//TXB0REQ=0;						 	//���ȣ���ֹ����
	TXB0CON=(TXB0CON&0xf7);		 			//TXREQ��0���ȣ���ֹ����
	
	TXB0IF=0;								//��շ�����ɱ�־	
		
	TXB0DLC=0x01;
	
	TXB0D0=CAN_TX_Buf;
	
	//TXB0REQ=1;							//׼����ϣ�������,TXB0REQ=1
	TXB0CON=(TXB0CON|0x08);					//׼����ϣ�������,TXREQ=1
}

/*************************************************************************/
//������:CAN_Receive_Data
//��  ��:CAN���ղ�ͬ���ȵ�����
//��  ��:
//����ֵ����
//ʱ�䣺20171123
//�����ߣ�ahren
/************************************************************************/
void CAN_Receive_Interrupt_Data(unsigned char *CAN_RX_Buf)
{
	unsigned char Leng_Num = 0;
	
	unsigned char DCL_Num = RXB0DLC;
	
	for(Leng_Num=0; Leng_Num<DCL_Num; Leng_Num++)
	{	
		CAN_RX_Buf[Leng_Num] = *(&RXB0D0 + Leng_Num);				
	}

	RXFUL=0;					    		//�򿪽��ջ���������������Ϣ	
}

/*************************************************************************/
//������:CAN_Life_Signal
//��  ��:CAN����������0x00������0xFF,
//��  ��:
//����ֵ����
//ʱ�䣺20171123
//�����ߣ�ahren
/************************************************************************/
void CAN_Life_Signal(void)
{
	CAN_TX0_Buff[7] = CAN_Life_Signal_Count;	
 
    CAN_Life_Signal_Count++;
	
	if(CAN_Life_Signal_Count > 0xFF)
	{
		CAN_Life_Signal_Count = 0x00;
	}		
}

/*************************************************************************/
//������:CAN_RXD_TXD_Test
//��  ��:CAN���շ���ͨ�Ų���
//��  ��:
//����ֵ����
//ʱ�䣺20171123
//�����ߣ�ahren
/************************************************************************/
void CAN_RXD_TXD_Test(void)
{
	unsigned char Count = 0;
	
	for(Count=0; Count<8; Count++)
	{
		CAN_TX0_Buff[Count] = CAN_Receive_Data[Count];
	}	
}

/*************************************************************************/
//������:CAN_RXD_TXD_Test
//��  ��:CAN ����ģʽ
//��  ��:
//����ֵ����
//ʱ�䣺20171127
//�����ߣ�huajifeng 
/************************************************************************/
static void CAN_Config_Mode(void)
{
    CANCON=0X80;					//�������CAN����ģʽREQOP=100	(���ƼĴ���)
	while((CANSTAT&0X80)==0);		//�ȴ�����CAN����ģʽOPMODE=100(״̬�Ĵ���)	
}

/*************************************************************************/
//������:CAN_Init
//��  ��:CAN��ʼ������
//��  ��:
//����ֵ����
//ʱ�䣺20171123
//�����ߣ�ahren
/************************************************************************/
void CAN_Init()
{	
	Clear_CAN_TX0_Buf(0x00);	      //���CAN_TX0_BUF��	
	Clear_CAN_RX0_Buf(0x00);
    
	/****************************************************/
	//CAN I/O ��ʼ��
 	/****************************************************/
 		
	CAN_Port_Set();					//����CAN���շ���I/O��������	

	/****************************************************/
	//��������ģʽ
 	/****************************************************/
	
	CAN_Config_Mode();                           // �ȴ���������ģʽ
	
	/****************************************************/
	//CAN����������
 	/****************************************************/

	CAN_Baud_Rate_Set();			   //����CAN���������ú���	

	/****************************************************/
	//������չ��ʶ������
 	/****************************************************/		

	CAN_TXD_Extended_Identifier();	
	
	/****************************************************/
	//������չ��ʶ�����á�������չ��ʶ����������
 	/****************************************************/
 	
	CAN_RXD_Extended_Identifier();
 		
	/****************************************************/
	//��ʼ��CAN�жϡ�CANģ�����ģʽѡ��
 	/****************************************************/
 	
	CAN_Interrupt_Mode_Set();

 	/****************************************************/
	
	CAN_STB_OFF;												//�ر�CANʹ��PIN
	
 	/****************************************************/	
}

/*************************************************************************/
//������:CAN_Send_8Byte_Data
//��  ��:CAN���ͺ���
//��  ��:
//����ֵ����
//ʱ�䣺20171123
//�����ߣ�ahren
/************************************************************************/
void CAN_Send_8Byte_Data_MCP_R(u8 *arr)  							//����0����DLC���ȵı���
{
	TXB0CON = (TXB0CON&0xf7);				//TXREQ��0���ȣ���ֹ����	
	TXB0IF = 0;								//��շ�����ɱ�־		
	TXB0DLC = 0x08;							//���÷������ݵĳ���Ϊ8���ֽ�
	//TXB0DLC = Send_DLC;					//���ͳ��ȸ������ݳ��ȶ���
	
	TXB0D0 = arr[0];				//����ȴ���������
	TXB0D1 = arr[1];
	TXB0D2 = arr[2];
	TXB0D3 = arr[3];
	TXB0D4 = arr[4];
	TXB0D5 = arr[5];
	TXB0D6 = arr[6];
	TXB0D7 = arr[7];
	
	TXB0CON=(TXB0CON|0x08);					//׼����ϣ�������,TXREQ=1
}

/*************************************************************************/
//������:CAN_Data_Integration
//��  ��:
//��  ��:
//����ֵ���ޣ�
//ʱ�䣺20171123
//�����ߣ�ahren
/************************************************************************/
void CAN_Data_Integration(void)
{
    CAN_TX0_Buff[0] = 0xFF;
    CAN_TX0_Buff[1] = 0xFF;
    CAN_TX0_Buff[2] = 0xFF ;            
    CAN_TX0_Buff[3] = 0xFF;            
    CAN_TX0_Buff[4] = 0xFF;       
    CAN_TX0_Buff[5] = 0xFF;         
    CAN_TX0_Buff[6] = 0xFF; 
}

/*************************************************************************/
//������:CAN_Send_Alarm_Warning_Data
//��  ��:CAN����ģ�����ݺ���
//��  ��:
//����ֵ����
//ʱ�䣺20180822
//�����ߣ�Chenlf
/************************************************************************/
void CAN_Send_Alarm_Warning_Data(void)
{
	if(1 ==MCU_CAN_Send_1s_Flag_g)		//CAN 1s��һ��
	{		
		MCU_CAN_Send_1s_Flag_g = 0;
		
		CAN_Data_Integration();     //CAN���ձ��ĵ�����Byte[0]��[1]��[2]��[3]��[4]��[5]��[7]           
        
        CAN_Life_Signal();          //��������CAN����Byte[7]ѭ������
        
        CAN_Send_8Byte_Data();		//�����ã�MCU CANѭ��������	
	}  
}

/*************************************************************************/
//������:CAN_Interrupt_Occurred
//��  ��:���������жϣ�������յ�����
//��  ��:
//����ֵ����
//ʱ�䣺20171123
//�����ߣ�ahren
/************************************************************************/
void CAN_Interrupt_Occurred(void)
{
    if(1 == CAN_RXD_Interrupt_Flag_g)           //250ms�ж�һ��
	{			
		CAN_RXD_Interrupt_Flag_g = 0;	
		
		CAN_Receive_Interrupt_Data(CAN_Receive_Data);
		
		CAN_RXD_TXD_Test();    
	}
}
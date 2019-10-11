/*----------------------------------------------------------------------*/
/* Confidential and Proprietary Information of MicroPerfect Corporation */
/* (C) 2018 ,MicroPerfect Corporation . All rights reserved. */
/* FileName : MCP2515.c */
/* Function : */
/* Ver: 0.01  */
/* MP_Ahren NewFile */
/* History:YYYY.MM.DD Version author Description 
          :2018.07.03 0.01 @wuwenfang Follow the coding standard  */
/*-------------------------------------------------------------*/

/***************ͷ�ļ�****************/
/**/
/*************************************/
#include "MCP2515.H"
//******************�궨��*************/
/**/
/*************************************/

//#define	SPI_ReadByte()	SPI_RWByte(0xff)
//#define	SPI_WriteByte(dat)	SPI_RWByte(dat)
/***************��������************/
/**/
u8 MCP2515_Buff[9] = {0};
/*************************************/

/***************��������**************/
/**/
/*************************************/

/*************************************/

void SPI_Init(void)
{
    //��������
    TRISC5 = 0;   //SDO
    TRISC3 = 0;   //SCK
    TRISC4 = 1;   //SDI
    
   // TRISC1 = 0;   //CS
   // TRISA5 = 1;

    SSPMD = 0;   //SSPMD ��MSSP ģ���ֹλ   1 = ��ֹ MSSP ģ�顣���� SSP �Ĵ����������ڸ�λ״̬�Ҳ���д��  0 = ʹ�� MSSP ģ��
    SSPOD = 0;   //SSPOD ��SPI ©����·���ʹ��λ    1 = ʹ��©����·����     0 = ��ֹ©����·����
    //SSPSTAT:
    SMP = 1;         //1 = ���������ʱ���ĩβ������������   0 = ���������ʱ����м������������
    CKE = 1;         //SPI ʱ��ѡ��λ   1 = ʱ��״̬����Чת��������ʱ����     0 = ʱ��״̬�ӿ���ת������Чʱ����
    //BF           //��������״̬λ �����޽���ģʽ�� 1 = ������ɣ� SSPBUF ����   0 = ����δ��ɣ� SSPBUF Ϊ��
    //SSPCON1:
    //WCOL = 0;    ///��д��ͻ���λ  1 = ���ڷ���ǰһ����ʱ����������д�� SSPBUF �Ĵ��� ��������������㣩0 = δ������ͻ
    //SSPOV =0;//���������ָʾλ   SPI ��ģʽ��1 = SSPBUF �Ĵ������Ա���ǰһ����ʱ���ֽ��յ�һ���µ��ֽڡ������������� SSPSR �е����ݻᶪʧ�����ֻ���ڴ�ģʽ�·�������ʹֻ�Ƿ������ݣ��û�Ҳ����� SSPBUF���Ա��⽫�����־λ�� 1 ����λ������������㣩��0 = �����
    SSPEN = 1;       //1 = ʹ�ܴ��ڲ��� SCK�� SDO�� SDI �� SS ����Ϊ��������   0 = ��ֹ���ڲ���������������Ϊ I/O �˿�����
    CKP = 0;         //1 = ����״̬ʱ��ʱ��Ϊ�ߵ�ƽ      0 = ����״̬ʱ��ʱ��Ϊ�͵�ƽ
    SSPM3 = 0;
    SSPM2 = 0;
    SSPM1 = 0;
    SSPM0 = 0;          //1010 = SPI ��ģʽ��ʱ�� = F OSC /8
                        //0101 = SPI ��ģʽ��ʱ�� = SCK ���ţ���ֹ SS ���ſ��ƣ��ɽ� SS ���� I/O ����
                        //0100 = SPI ��ģʽ��ʱ�� = SCK ���ţ�ʹ�� SS ���ſ���
                        //0011 = SPI ��ģʽ��ʱ�� = TMR2 ��� /2
                        //0010 = SPI ��ģʽ��ʱ�� = F OSC /64
                        //0001 = SPI ��ģʽ��ʱ�� = F OSC /16
                        //0000 = SPI ��ģʽ��ʱ�� = F OSC /4
      
}

/*************************************************************************/
//������:SPI_RWByte
//��  ��:
//��  ��:
//����ֵ��
//ʱ�䣺20180703
//�����ߣ�ahren
/************************************************************************/
u8 SPI_RWByte(u8 dat)
{    
    while(WCOL)WCOL=0;
    SSPBUF = dat;
    while(!BF);
    return SSPBUF;
}

/*************************************************************************/
//������:SPI_ReadByte
//��  ��:ģ��SPIͨ�Ŷ�ʱ��
//��  ��:��
//����ֵ����ȡ������
//ʱ�䣺20180703
//�����ߣ�ahren
/************************************************************************/
u8 SPI_ReadByte(void)
{
	u8 i,RDate=0;	
//	MCP2515_SCK = 0;
//	for(i=0;i<8;i++)
//	{
//		MCP2515_SCK = 1;
//		RDate <<= 1;
//		RDate |= MCP2515_MISO;
//		MCP2515_SCK = 0;	
//	}
//	return RDate;
    
    i = SPI_RWByte(0xff);
    return i;
}
//
///*************************************************************************/
////������:SPI_WriteByte
////��  ��:ģ��SPIͨ��дʱ��
////��  ��:д������
////����ֵ����
////ʱ�䣺20180703
////�����ߣ�ahren
///************************************************************************/
void SPI_WriteByte(u8 dt)
{
//	u8 i;		
//	for(i=0;i<8;i++)
//	{	
//		MCP2515_SCK = 0;
//		if((dt<<i) & 0x80)
//			MCP2515_MOSI = 1;
//		else
//			MCP2515_MOSI = 0;					
//		MCP2515_SCK = 1;
//	}
//	MCP2515_SCK = 0;
    
    SPI_RWByte(dt);
}

/*************************************************************************/
//������:MCP2515_WriteByte
//��  ��:MCP2515д����
//��  ��:addr����ַ, dat������
//����ֵ����
//ʱ�䣺20180703
//�����ߣ�ahren
/************************************************************************/
void MCP2515_WriteByte(u8 addr,u8 dat)
{
	MCP2515_CS = 0;
	SPI_WriteByte(MCP2515_CAN_WRITE);	//д����
	SPI_WriteByte(addr);		        //д�Ĵ�����ַ
	SPI_WriteByte(dat);			        //д����

    MCP2515_CS = 1;				
}

/*************************************************************************/
//������:MCP2515_ReadByte
//��  ��:MCP2515������
//��  ��:addr����ַ
//����ֵ������
//ʱ�䣺20180703
//�����ߣ�ahren
/************************************************************************/
u8 MCP2515_ReadByte(u8 addr)
{
	u8 RData;	
	MCP2515_CS = 0;
	SPI_WriteByte(MCP2515_CAN_READ);		//������
	SPI_WriteByte(addr);		           	//д�Ĵ�����ַ
	RData = SPI_ReadByte();	                //������
	MCP2515_CS = 1;	
	return RData;				
}

/*************************************************************************/
//������:MCP2515_Reset
//��  ��:MCP2515�����λ
//��  ��:��
//����ֵ����
//ʱ�䣺20180703
//�����ߣ�ahren
/************************************************************************/
void MCP2515_Reset(void)
{
	MCP2515_CS = 0;				
	SPI_WriteByte(MCP2515_CAN_RESET);	
	MCP2515_CS = 1;				
}

/*************************************************************************/
//������:MCP2515_CAN_BaudRate
//��  ��:MCP2515CAN����������
//��  ��:��
//����ֵ����
//ʱ�䣺20180703
//�����ߣ�ahren
/************************************************************************/
void MCP2515_CAN_BaudRate(void)
{
	MCP2515_WriteByte(MCP2515_CNF1,CAN_250Kbps);
	MCP2515_WriteByte(MCP2515_CNF2,0x80|MCP2515_PHSEG1_1TQ|MCP2515_PRSEG_1TQ);    	
	MCP2515_WriteByte(MCP2515_CNF3,MCP2515_PHSEG2_1TQ);			
}

/*************************************************************************/
//������:MCP2515_CAN_TxID
//��  ��:MCP2515����ID����
//��  ��:ID�����׵�ַ���������鳤��
//����ֵ����
//ʱ�䣺20180703
//�����ߣ�ahren
/************************************************************************/
void MCP2515_CAN_TxID(u8 *ID, u8 len)
{
    u8 SIDH,SIDL;
    SIDH = (ID[0] << 3)|(ID[1] >> 5);
    SIDL = ((ID[1]&0X03)|0X08)|((ID[1]<<3)&0XE0);
    
	MCP2515_WriteByte(MCP2515_TXB0SIDH,SIDH); //���ͻ���Ĵ����ı�ʶ�����ã���׼+��չ��
	MCP2515_WriteByte(MCP2515_TXB0SIDL,SIDL);
	MCP2515_WriteByte(MCP2515_TXB0EID8,ID[2]);
	MCP2515_WriteByte(MCP2515_TXB0EID0,ID[3]);
    MCP2515_WriteByte(MCP2515_TXB0DLC,len);			
}

/*************************************************************************/
//������:MCP2515_CAN_RxID
//��  ��:MCP2515����ID����
//��  ��:ID�����׵�ַ���������鳤��
//����ֵ����
//ʱ�䣺20180703
//�����ߣ�ahren
/************************************************************************/
void MCP2515_CAN_RxID(u8 *ID, u8 len)
{
    u8 SIDH,SIDL;
    SIDH = (ID[0] << 3)|(ID[1] >> 5);
    SIDL = ((ID[1]&0X03)|0X08)|((ID[1]<<3)&0XE0);
    
	MCP2515_WriteByte(MCP2515_RXB0SIDH,0x00);      //��������˲��Ĵ�����ʶ�����ã���׼+��չ��
	MCP2515_WriteByte(MCP2515_RXB0SIDL,0x00);
	MCP2515_WriteByte(MCP2515_RXB0EID8,0x00);
	MCP2515_WriteByte(MCP2515_RXB0EID0,0x00);
	MCP2515_WriteByte(MCP2515_RXB0CTRL,0x40);
	MCP2515_WriteByte(MCP2515_RXB0DLC,len);
	
	MCP2515_WriteByte(MCP2515_RXF0SIDH,SIDH);    //���������˲��Ĵ�����ʶ�����ã���׼+��չ��
	MCP2515_WriteByte(MCP2515_RXF0SIDL,SIDL);
	MCP2515_WriteByte(MCP2515_RXF0EID8,ID[2]);
	MCP2515_WriteByte(MCP2515_RXF0EID0,ID[3]);
#if 0
	MCP2515_WriteByte(MCP2515_RXM0SIDH,0xFF);    //�����������μĴ�����ʶ�����ã���׼+��չ��
	MCP2515_WriteByte(MCP2515_RXM0SIDL,0xFF);
	MCP2515_WriteByte(MCP2515_RXM0EID8,0xFF);
	MCP2515_WriteByte(MCP2515_RXM0EID0,0x00);	
#endif
    MCP2515_WriteByte(MCP2515_RXM0SIDH,0xFF);    //�����������μĴ�����ʶ�����ã���׼+��չ��
	MCP2515_WriteByte(MCP2515_RXM0SIDL,0xFF);
	MCP2515_WriteByte(MCP2515_RXM0EID8,0x00);
	MCP2515_WriteByte(MCP2515_RXM0EID0,0x00);
}

/*************************************************************************/
//������:MCP2515_Init
//��  ��:MCP2515��ʼ��
//��  ��:��
//����ֵ����
//ʱ�䣺20180703
//�����ߣ�ahren
/************************************************************************/
void MCP2515_Init(void)
{   
//    u8 MCP2515_TxID[4]={0x18,0xF0,0xF4,0x00};
//    u8 MCP2515_RxID[4]={0x0C,0xF0,0xF3,0x00};       //��֤PC��ģ�ⷢ�͵�֡   
    
//    TRISC5 = 0;              //�˿�����
//    TRISC4 = 1;
//    TRISC3 = 0;
      
    SPI_Init();   
    delay_10ms(1);
   
    TRISC1 = 0;
    TRISC2 = 0;
    TRISC0 = 0;      
    
    MCP2515_RESET = 1;               //�ر�Ӳ����λ
    //MCP2515_SILENT = 0;              //  MCP2515ʹ��
    MCP2515_SILENT_OFF;
    delay_10ms(1);
	MCP2515_Reset();                 //�����λ
    delay_10ms(1);
    MCP2515_CAN_BaudRate();          //����������
	delay_10ms(1);
    MCP2515_CAN_TxID(MCP2515_TxID,8);        //����ID���� 
    delay_10ms(1);
#if 1                                //
	MCP2515_CAN_RxID(MCP2515_RxID,8);        //����ID����
    delay_10ms(1);
#endif    
	MCP2515_WriteByte(MCP2515_CANINTE,0x01);                                        //MCP2515�ж�����
	delay_10ms(1);
	MCP2515_WriteByte(MCP2515_CANCTRL,MCP2515_REQOP_NORMAL|MCP2515_CLKOUT_ENABLED);	//���ó�����ģʽ
    delay_10ms(1);
	if(MCP2515_OPMODE_NORMAL != (MCP2515_ReadByte(MCP2515_CANSTAT) & 0xE0))         //ȷ���Ƿ����óɹ�
	{
		MCP2515_WriteByte(MCP2515_CANCTRL,MCP2515_REQOP_NORMAL|MCP2515_CLKOUT_ENABLED);
	}
    delay_10ms(1);
    MCP2515_WriteByte(MCP2515_CANINTF,0x00); 
}

/*************************************************************************/
//������:MCP2515_CAN_Tx_Buffer
//��  ��:MCP2515_CAN���ͺ���
//��  ��:���������׵�ַ�����鳤��
//����ֵ����
//ʱ�䣺20180703
//�����ߣ�ahren
/************************************************************************/
void MCP2515_CAN_Tx_Buffer(u8 BOX_Num, u8 *CAN_TX_Buf, u8 len,u8 Life_Signal)
{
	u8 j,dly;
    dly=0;

    //CAN_TX_Buf[0] = BOX_Num;
    //CAN_TX_Buf[6] = Life_Signal;
    memcpy(MCP2515_Buff, CAN_TX_Buf, 8);          //�� TC04_01_11_Buff ���Ƹ�TC04_One_Buff   

    MCP2515_Data_Analysis(BOX_Num, MCP2515_Buff, Life_Signal);

    while((MCP2515_ReadByte(MCP2515_TXB0CTRL)&0x08) && (dly<50))        //��ȡ״ֵ̬���Ƿ���Է���
    {			
        dly++;
    }

    for(j=0;j<len;j++)
    {
        MCP2515_WriteByte(MCP2515_TXB0D0 + j,MCP2515_Buff[j]);            //д�뷢�����ݵ����ͻ�����    
    }
    /********************************/
    //�����г�CAN����Э���޸�
    //20190731
    //ahren  

    //MCP2515_WriteByte(MCP2515_TXB0D1,BOX_Num);//����life        
    //MCP2515_WriteByte(MCP2515_TXB0D7,Life_Signal);//����life      
    /********************************/    

    MCP2515_WriteByte(MCP2515_TXB0DLC, len);                            //д�뷢�͵����ݳ���
    MCP2515_CS = 0;
    MCP2515_WriteByte(MCP2515_TXB0CTRL,0x08);                           //���ͱ���
    MCP2515_CS = 1;  
}

/*************************************************************************/
//������:MCP2515_CAN_Rx_Buffer
//��  ��:MCP2515_CAN���պ���
//��  ��:���������׵�ַ
//����ֵ�������ֽڳ���
//ʱ�䣺20180703
//�����ߣ�ahren
/************************************************************************/
u8 MCP2515_CAN_Rx_Buffer(u8 *CAN_RX_Buf)
{
	u8 i=0,len=0,temp=0;
    
    u8 *reg=CAN_RX_Buf ;
    
	temp = MCP2515_ReadByte(MCP2515_CANINTF);      //��ȡ�жϱ�־λ���Ƿ�������
	if(temp & 0x01)
	{

		for(i=0;i<8;i++)
		{	

            *(reg + i) = MCP2515_ReadByte(MCP2515_RXB0D0+i);
            
		}             
	}    
            
    MCP2515_WriteByte(MCP2515_CANINTF,0); 

	return len;
}

/*************************************************************************/
//������:MCP2515_Enter_Sleep_Mode
//��  ��:MCP2515����˯��ģʽ
//��  ��:
//����ֵ��
//ʱ�䣺20180703
//�����ߣ�ahren
/************************************************************************/
u8 MCP2515_Enter_Sleep_Mode(void)
{
	u8 time_out = 0;
	MCP2515_WriteByte(MCP2515_CANCTRL, MCP2515_REQOP_SLEEP);                //���ó�˯��ģʽ
    //delay_10ms(1);
    NOP();NOP();NOP();NOP();
	
	while(MCP2515_OPMODE_SLEEP != (MCP2515_ReadByte(MCP2515_CANSTAT) & 0xE0))//ȷ���Ƿ����óɹ�
	{
//		delay_10ms(1);
        NOP();NOP();NOP();NOP();        
		time_out++;
		if(time_out>100)//��ʱ����
		{
			return 0;
		}		
	}
	return 1;	
}


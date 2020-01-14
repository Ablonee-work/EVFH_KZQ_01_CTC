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

/***************��������************/
/**/

/*************************************/

/***************��������**************/
/**/
/*************************************/

/*************************************/

void SPI_Init(void)
{
    //��������
    MCP2515_SILENT_DIR   = 0;
    MCP2515_CS_DIR       = 0;
    MCP2515_RESET_DIR    = 0;
    MCP2515_SCK_DIR      = 0;
    MCP2515_MISO_DIR     = 1;
    MCP2515_MOSI_DIR     = 0;    

    SSPMD = 0;   //SSPMD = 1:��ֹ MSSP ģ��,SSPMD = 0:ʹ�� MSSP ģ��
    SSPOD = 0;   //SSPOD = 1:SPI ©����·���ʹ��λ,SSPOD = 0:��ֹ©����·����

    SMP = 1;  //1 = ���������ʱ���ĩβ������������   0 = ���������ʱ����м������������
    CKE = 1;  //SPI ʱ��ѡ��λ   1:ʱ��״̬����Чת��������ʱ����  0:ʱ��״̬�ӿ���ת������Чʱ����

    SSPEN = 1; //1 = ʹ�ܴ��ڲ��� SCK�� SDO�� SDI �� SS ����Ϊ��������,0 = ��ֹ���ڲ���������������Ϊ I/O �˿�����
    CKP = 0;   //1 = ����״̬ʱ��ʱ��Ϊ�ߵ�ƽ ,0 = ����״̬ʱ��ʱ��Ϊ�͵�ƽ

    //1010 = SPI ��ģʽ��ʱ�� = F OSC /8
    //0101 = SPI ��ģʽ��ʱ�� = SCK ���ţ���ֹ SS ���ſ��ƣ��ɽ� SS ���� I/O ����
    //0100 = SPI ��ģʽ��ʱ�� = SCK ���ţ�ʹ�� SS ���ſ���
    //0011 = SPI ��ģʽ��ʱ�� = TMR2 ��� /2
    //0010 = SPI ��ģʽ��ʱ�� = FOSC /64
    //0001 = SPI ��ģʽ��ʱ�� = FOSC /16
    //0000 = SPI ��ģʽ��ʱ�� = FOSC /4
    SSPM3 = 0;
    SSPM2 = 0;
    SSPM1 = 0;
    SSPM0 = 0;          
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
    while(!BF);//��������״̬λ 1������0����
    return SSPBUF;
}

/*************************************************************************/
//������:SPI_ReadByte
//��  ��:ģ��SPIͨ�Ŷ�ʱ��
//��  ��:��
//����ֵ����ȡ������
//ʱ�䣺20180703
//�����ߣ�ahren
/***************************/
u8 SPI_ReadByte(void)
{
    u8 RDate = 0;	

    RDate = SPI_RWByte(0xFF);

    return RDate;
}
//
/***************************/
//������:SPI_WriteByte
//��  ��:ģ��SPIͨ��дʱ��
//��  ��:д������
//����ֵ����
//ʱ�䣺20180703
//�����ߣ�ahren
/***************************/
void SPI_WriteByte(u8 Data)
{    
    SPI_RWByte(Data);
}

/***************************/
//������:MCP2515_WriteByte
//��  ��:MCP2515д����
//��  ��:addr����ַ, dat������
//����ֵ����
//ʱ�䣺20180703
//�����ߣ�ahren
/***************************/
void MCP2515_WriteByte(u8 Addr,u8 Data)
{
	MCP2515_CS = 0;
	SPI_WriteByte(MCP2515_CAN_WRITE);	//д����
	SPI_WriteByte(Addr);		        //д�Ĵ�����ַ
	SPI_WriteByte(Data);			    //д����
    MCP2515_CS = 1;				
}

/***************************/
//������:MCP2515_ReadByte
//��  ��:MCP2515������
//��  ��:addr����ַ
//����ֵ������
//ʱ�䣺20180703
//�����ߣ�ahren
/***************************/
u8 MCP2515_ReadByte(u8 Addr)
{
	u8 RData;	

	MCP2515_CS = 0;
	SPI_WriteByte(MCP2515_CAN_READ);		//������
	SPI_WriteByte(Addr);		           	//д�Ĵ�����ַ
	RData = SPI_ReadByte();	                //������
	MCP2515_CS = 1;	

	return RData;				
}

/***************************/
//������:MCP2515_Reset
//��  ��:MCP2515�����λ
//��  ��:��
//����ֵ����
//ʱ�䣺20180703
//�����ߣ�ahren
/***************************/
void MCP2515_Reset(void)
{
	MCP2515_CS = 0;				
	SPI_WriteByte(MCP2515_CAN_RESET);	//��λָ��
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
void MCP2515_CAN_BaudRate(u8 Fosc, uint16 BRP)
{
    static u8 Fosc_Val    = 0;
    static uint16 BRP_Val = 0;

    Fosc_Val = Fosc;
    BRP_Val = BRP;
    /***************************************************/
    //20191204
    //ahren
    //�����ʼ��㹫ʽ
    //CNF1 (SJW = 1TQ, BRP = X) 
    //CNF2 (PHSEG1 = 3TQ, PRSEG = 1TQ)
    //CNF3 (PHSEG2 = 3TQ)    
    //TQ = 2*(BRP+1)/Fosc = Y
    //λʱ�� = TQ*(1+3+1+3)=Y*8=8Y
    //λ�� = 1/8Y
    /***************************************************/    
    if (Fosc_Val == 0x08)
    {
        if (BRP_Val == 0xFA)
        {
            //250Kbps
            //BRP=X=0; Fosc=8; Y=0.5; 8Y=4; 1/8Y=1/4=0.25;
            MCP2515_WriteByte(MCP2515_CNF1,MCP2515_SJW_1TQ|MCP2515_BRP_2_F);
            MCP2515_WriteByte(MCP2515_CNF2,MCP2515_BTLMODE|MCP2515_1_SAM|MCP2515_PHSEG1_8TQ|MCP2515_PRSEG_1TQ);    	
            MCP2515_WriteByte(MCP2515_CNF3,MCP2515_SOF_DISABLED|MCP2515_WAKFIL_DISABLED|MCP2515_PHSEG2_6TQ); 
	            
        }
        else if (BRP_Val == 0x01F4)
        {
            //500Kbps
            //BRP=X=0; Fosc=8; Y=0.25; 8Y=2; 1/8Y=1/2=0.5; 
           MCP2515_WriteByte(MCP2515_CNF1,MCP2515_SJW_1TQ|MCP2515_BRP_2_F);
           MCP2515_WriteByte(MCP2515_CNF2,MCP2515_BTLMODE|MCP2515_PHSEG1_4TQ|MCP2515_PRSEG_1TQ);    	
           MCP2515_WriteByte(MCP2515_CNF3,MCP2515_SOF_DISABLED|MCP2515_WAKFIL_DISABLED|MCP2515_PHSEG2_2TQ); 
        }
    }   			
}

/*************************************************************************/
//������:MCP2515_CAN_TxID
//��  ��:MCP2515����ID����
//��  ��:ID�����׵�ַ���������鳤��
//����ֵ����
//ʱ�䣺20180703
//�����ߣ�ahren
/************************************************************************/
void MCP2515_CAN_TxID(u8 *ID, u8 Len)
{
    u8 SIDH = 0;
    u8 SIDL = 0;

    SIDL = (ID[1] & 0xF0) >> 5;
    SIDH = (ID[0] << 3) + SIDL;

    SIDL = (ID[1] & 0x1C) << 3;
    SIDL = (ID[1] & 0x03) + SIDL + 0x08;

    //���ͻ���Ĵ����ı�ʶ�����ã���չ��
	MCP2515_WriteByte(MCP2515_TXB0SIDH,SIDH); 
    delay_10us(100); 
	MCP2515_WriteByte(MCP2515_TXB0SIDL,SIDL);
    delay_10us(100);     
	MCP2515_WriteByte(MCP2515_TXB0EID8,ID[2]);
    delay_10us(100);     
	MCP2515_WriteByte(MCP2515_TXB0EID0,ID[3]);
    delay_10us(100);   
    MCP2515_WriteByte(MCP2515_TXB0DLC,Len);	
    delay_10us(100);  		
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
    u8 SIDH = 0;
    u8 SIDL = 0;

    SIDL = (ID[1] & 0xF0) >> 5;
    SIDH = (ID[0] << 3) + SIDL;

    SIDL = (ID[1] & 0x1C) << 3;
    SIDL = (ID[1] & 0x03) + SIDL + 0x08;

	MCP2515_WriteByte(MCP2515_RXB0DLC,len);

	//���������˲��Ĵ�����ʶ�����ã���չ��
	MCP2515_WriteByte(MCP2515_RXF0SIDH,SIDH);    
	MCP2515_WriteByte(MCP2515_RXF0SIDL,SIDL);
	MCP2515_WriteByte(MCP2515_RXF0EID8,ID[2]);
	MCP2515_WriteByte(MCP2515_RXF0EID0,ID[3]);

    //�����������μĴ�����ʶ�����ã���չ��
    MCP2515_WriteByte(MCP2515_RXM0SIDH,0xFF);    
	MCP2515_WriteByte(MCP2515_RXM0SIDL,0xFF);
	MCP2515_WriteByte(MCP2515_RXM0EID8,0xFF);
	MCP2515_WriteByte(MCP2515_RXM0EID0,0xFF);  

	MCP2515_WriteByte(MCP2515_RXB1DLC,len);
	//���������˲��Ĵ�����ʶ�����ã���չ��
	MCP2515_WriteByte(MCP2515_RXF1SIDH,SIDH);    
	MCP2515_WriteByte(MCP2515_RXF1SIDL,SIDL);
	MCP2515_WriteByte(MCP2515_RXF1EID8,ID[2]);
	MCP2515_WriteByte(MCP2515_RXF1EID0,ID[3]);

    //�����������μĴ�����ʶ�����ã���չ��
    MCP2515_WriteByte(MCP2515_RXM1SIDH,0xFF);    
	MCP2515_WriteByte(MCP2515_RXM1SIDL,0xFF);
	MCP2515_WriteByte(MCP2515_RXM1EID8,0xFF);
	MCP2515_WriteByte(MCP2515_RXM1EID0,0xFF);         
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
    SPI_Init();   
    delay_10us(10);   

    MCP2515_SILENT_DIR = 0;
    MCP2515_CS_DIR     = 0;
    MCP2515_RESET_DIR  = 0;   
    
    MCP2515_SILENT_OFF;
    delay_10us(5);  

    MCP2515_RESET = 1;//�ر�Ӳ����λ
	MCP2515_Reset();//�����λ,��������ģʽ
    delay_10ms(1);       
    /**************************************************************************************/
    //�������λû�н�������ģʽ����ͨ�����üĴ�����������ģʽ
    //20200114
    //ahren
	if(MCP2515_REQOP_CONFIG != (MCP2515_ReadByte(MCP2515_CANSTAT) & 0x80))         
	{
		MCP2515_WriteByte(MCP2515_CANCTRL,MCP2515_REQOP_CONFIG|MCP2515_ABORT_DISABLED|
                          MCP2515_OSM_DISABLED|MCP2515_CLKOUT_DISABLED|MCP2515_CLKOUT_PRE_1);
        delay_10ms(1);         
	}
    /**************************************************************************************/
	MCP2515_WriteByte(MCP2515_TXRTSCTRL,0x00);//��������״̬����                                        
    delay_10us(10);  

	MCP2515_WriteByte(MCP2515_BFPCTRL,0x00);//��������״̬����                                        
    delay_10us(10);     

	MCP2515_WriteByte(MCP2515_CANINTE,0x00);//MCP2515�ж�����                                        
    delay_10us(10);  
    
    MCP2515_WriteByte(MCP2515_CANINTF,0x00);//�жϱ�־λȫ��д0
    delay_10us(10);   

	MCP2515_WriteByte(MCP2515_TXB0CTRL,0x03); //���÷��ͻ�����0                                       
    delay_10us(10);  

	MCP2515_WriteByte(MCP2515_TXB1CTRL,0x00); //���÷��ͻ�����1                                       
    delay_10us(10);  

	MCP2515_WriteByte(MCP2515_TXB2CTRL,0x00); //���÷��ͻ�����2                                        
    delay_10us(10);               

    MCP2515_CAN_TxID(MCP2515_TxID,8); //����ID���� 
    delay_10us(10); 

    //ֻ������չ֡��������
	MCP2515_WriteByte(MCP2515_RXB0CTRL,0x44);  
	MCP2515_WriteByte(MCP2515_RXB1CTRL,0x40);       
#if 0                                
	MCP2515_CAN_RxID(MCP2515_RxID,8);//����ID����
    delay_10us(10); 
#endif    

    MCP2515_CAN_BaudRate(SYS_FOSC_8MHZ,CAN_BRP_500);//����������
    delay_10ms(1);     
    //����ģʽ���ã����ó�����ģʽ
    MCP2515_WriteByte(MCP2515_CANCTRL,MCP2515_REQOP_NORMAL|MCP2515_ABORT_DISABLED|
                        MCP2515_OSM_DISABLED|MCP2515_CLKOUT_DISABLED|MCP2515_CLKOUT_PRE_1);    
//	MCP2515_WriteByte(MCP2515_CANCTRL,MCP2515_REQOP_NORMAL|MCP2515_CLKOUT_ENABLED);	
    delay_10ms(1);  
    //ȷ�Ϲ���ģʽ�Ƿ����óɹ�
	if(MCP2515_OPMODE_NORMAL != (MCP2515_ReadByte(MCP2515_CANSTAT) & 0xE0))         
	{
		//MCP2515_WriteByte(MCP2515_CANCTRL,MCP2515_REQOP_NORMAL|MCP2515_CLKOUT_ENABLED);
    MCP2515_WriteByte(MCP2515_CANCTRL,MCP2515_REQOP_NORMAL|MCP2515_ABORT_DISABLED|
                      MCP2515_OSM_DISABLED|MCP2515_CLKOUT_DISABLED|MCP2515_CLKOUT_PRE_1);           
	}
    delay_10ms(1);  
}


/*************************************************************************/
//������:MCP2515_Tx_Buffer
//��  ��:MCP2515_CAN���ͺ���
//��  ��:�������飬���鳤��
//����ֵ����
//ʱ�䣺20180703
//�����ߣ�ahren
/************************************************************************/
void MCP2515_Tx_Buffer(u8 *CAN_TX_Buf, u8 len)
{
	u8 i = 0;
    u8 Cnt = 0;

    //��ȡ״ֵ̬���Ƿ���Է��� 
    while((MCP2515_ReadByte(MCP2515_TXB0CTRL)&0x0B) && (Cnt<50))   
    {			
        Cnt++;
    }

    //д�뷢�����ݵ����ͻ����� 
    for(i=0; i<len; i++)
    {
        MCP2515_WriteByte(MCP2515_TXB0D0 + i, CAN_TX_Buf[i]);
        delay_10us(5);                
    }

    MCP2515_CS = 0;    
    MCP2515_WriteByte(MCP2515_TXB0DLC, len);//д�뷢�͵����ݳ���
    delay_10us(5);                                    
    MCP2515_WriteByte(MCP2515_TXB0CTRL,0x0B);//���ͱ��� 
    delay_10us(5);                               
    MCP2515_CS = 1;  
}
/*************************************************************************/
//������:MCP2515_Rx_Buffer
//��  ��:MCP2515_CAN���պ���
//��  ��:���������׵�ַ
//����ֵ�������ֽڳ���
//ʱ�䣺20180703
//�����ߣ�ahren
/************************************************************************/
u8 MCP2515_Rx_Buffer(u8 *CAN_RX_Buf)
{
	u8 i = 0;
    u8 len = 0;
    u8 temp = 0;   
    u8 *reg = CAN_RX_Buf ;

    //��ȡ�жϱ�־λ���Ƿ�������    
	temp = MCP2515_ReadByte(MCP2515_CANINTF);
    delay_10us(5);             
    //���յ������ݳ���
    len = (MCP2515_ReadByte(MCP2515_RXB0DLC) & 0x0F);
    delay_10us(5);    
	if(temp & 0x01)
	{
		for(i=0; i<len; i++)
		{	
            *(reg + i) = MCP2515_ReadByte(MCP2515_RXB0D0+i); 
            delay_10us(5);          
		}             
	}               
    MCP2515_WriteByte(MCP2515_CANINTF,0); 
    delay_10us(5); 
	return len;
}

/*************************************************************************/
//������:MCP2515_Sleep_Mode
//��  ��:MCP2515����˯��ģʽ
//��  ��:
//����ֵ��
//ʱ�䣺20180703
//�����ߣ�ahren
/************************************************************************/
u8 MCP2515_Sleep_Mode(void)
{
	u8 time_out = 0;

	MCP2515_WriteByte(MCP2515_CANCTRL, MCP2515_REQOP_SLEEP);                //���ó�˯��ģʽ
    NOP();NOP();NOP();NOP();
    //delay_10ms(1);   
    delay_10us(5);      	
	while(MCP2515_OPMODE_SLEEP != (MCP2515_ReadByte(MCP2515_CANSTAT) & 0xE0))//ȷ���Ƿ����óɹ�
	{
        //delay_10ms(1);  
        delay_10us(5);           
        NOP();NOP();NOP();NOP();        
		time_out++;
		if(time_out>100)//��ʱ����
		{
			return 0;
		}		
	}
	return 1;	
}


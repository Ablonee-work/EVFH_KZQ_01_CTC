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
#define MCP2515_CANSTAT         0x0E
#define MCP2515_CANCTRL         0x0F
#define MCP2515_BFPCTRL         0x0C
#define MCP2515_TEC             0x1C
#define MCP2515_REC             0x1D
#define MCP2515_CNF3            0x28
#define MCP2515_CNF2            0x29
#define MCP2515_CNF1            0x2A
#define MCP2515_CANINTE         0x2B
#define MCP2515_CANINTF         0x2C
#define MCP2515_EFLG            0x2D
#define MCP2515_TXRTSCTRL       0x0D

#define MCP2515_RXF0SIDH        0x00
#define MCP2515_RXF0SIDL        0x01
#define MCP2515_RXF0EID8        0x02
#define MCP2515_RXF0EID0        0x03
#define MCP2515_RXF1SIDH        0x04
#define MCP2515_RXF1SIDL        0x05
#define MCP2515_RXF1EID8        0x06
#define MCP2515_RXF1EID0        0x07
#define MCP2515_RXF2SIDH        0x08
#define MCP2515_RXF2SIDL        0x09
#define MCP2515_RXF2EID8        0x0A
#define MCP2515_RXF2EID0        0x0B
#define MCP2515_RXF3SIDH        0x10
#define MCP2515_RXF3SIDL        0x11
#define MCP2515_RXF3EID8        0x12
#define MCP2515_RXF3EID0        0x13
#define MCP2515_RXF4SIDH        0x14
#define MCP2515_RXF4SIDL        0x15
#define MCP2515_RXF4EID8        0x16
#define MCP2515_RXF4EID0        0x17
#define MCP2515_RXF5SIDH        0x18
#define MCP2515_RXF5SIDL        0x19
#define MCP2515_RXF5EID8        0x1A
#define MCP2515_RXF5EID0        0x1B

#define MCP2515_RXM0SIDH        0x20
#define MCP2515_RXM0SIDL        0x21
#define MCP2515_RXM0EID8        0x22
#define MCP2515_RXM0EID0        0x23
#define MCP2515_RXM1SIDH        0x24
#define MCP2515_RXM1SIDL        0x25
#define MCP2515_RXM1EID8        0x26
#define MCP2515_RXM1EID0        0x27

#define MCP2515_TXB0CTRL        0x30
#define MCP2515_TXB0SIDH        0x31
#define MCP2515_TXB0SIDL        0x32
#define MCP2515_TXB0EID8        0x33
#define MCP2515_TXB0EID0        0x34
#define MCP2515_TXB0DLC         0x35
#define MCP2515_TXB0D0          0x36
#define MCP2515_TXB0D1          0x37
#define MCP2515_TXB0D2          0x38
#define MCP2515_TXB0D3          0x39
#define MCP2515_TXB0D4          0x3A
#define MCP2515_TXB0D5          0x3B
#define MCP2515_TXB0D6          0x3C
#define MCP2515_TXB0D7          0x3D

#define MCP2515_TXB1CTRL        0x40
#define MCP2515_TXB1SIDH        0x41
#define MCP2515_TXB1SIDL        0x42
#define MCP2515_TXB1EID8        0x43
#define MCP2515_TXB1EID0        0x44
#define MCP2515_TXB1DLC         0x45
#define MCP2515_TXB1D0          0x46
#define MCP2515_TXB1D1          0x47
#define MCP2515_TXB1D2          0x48
#define MCP2515_TXB1D3          0x49
#define MCP2515_TXB1D4          0x4A
#define MCP2515_TXB1D5          0x4B
#define MCP2515_TXB1D6          0x4C
#define MCP2515_TXB1D7          0x4D

#define MCP2515_TXB2CTRL        0x50
#define MCP2515_TXB2SIDH        0x51
#define MCP2515_TXB2SIDL        0x52
#define MCP2515_TXB2EID8        0x53
#define MCP2515_TXB2EID0        0x54
#define MCP2515_TXB2DLC         0x55
#define MCP2515_TXB2D0          0x56
#define MCP2515_TXB2D1          0x57
#define MCP2515_TXB2D2          0x58
#define MCP2515_TXB2D3          0x59
#define MCP2515_TXB2D4          0x5A
#define MCP2515_TXB2D5          0x5B
#define MCP2515_TXB2D6          0x5C
#define MCP2515_TXB2D7          0x5D

#define MCP2515_RXB0CTRL        0x60
#define MCP2515_RXB0SIDH        0x61
#define MCP2515_RXB0SIDL        0x62
#define MCP2515_RXB0EID8        0x63
#define MCP2515_RXB0EID0        0x64
#define MCP2515_RXB0DLC         0x65
#define MCP2515_RXB0D0          0x66
#define MCP2515_RXB0D1          0x67
#define MCP2515_RXB0D2          0x68
#define MCP2515_RXB0D3          0x69
#define MCP2515_RXB0D4          0x6A
#define MCP2515_RXB0D5          0x6B
#define MCP2515_RXB0D6          0x6C
#define MCP2515_RXB0D7          0x6D

#define MCP2515_RXB1CTRL        0x70
#define MCP2515_RXB1SIDH        0x71
#define MCP2515_RXB1SIDL        0x72
#define MCP2515_RXB1EID8        0x73
#define MCP2515_RXB1EID0        0x74
#define MCP2515_RXB1DLC         0x75
#define MCP2515_RXB1D0          0x76
#define MCP2515_RXB1D1          0x77
#define MCP2515_RXB1D2          0x78
#define MCP2515_RXB1D3          0x79
#define MCP2515_RXB1D4          0x7A
#define MCP2515_RXB1D5          0x7B
#define MCP2515_RXB1D6          0x7C
#define MCP2515_RXB1D7          0x7D

#define MCP2515_TXREQ           0x08
#define MCP2515_TXP             0x03

#define MCP2515_RXM             0x60
#define MCP2515_BUKT            0x04

#define MCP2515_REQOP           0xE0
#define MCP2515_ABAT            0x10
#define	OSM                     0x08
#define MCP2515_CLKEN           0x04
#define MCP2515_CLKPRE          0x03

#define MCP2515_REQOP           0xE0
#define MCP2515_ICOD            0x0E

#define MCP2515_RX0IE           0x01
#define MCP2515_RX1IE           0x02
#define MCP2515_TX0IE           0x04
#define MCP2515_TX1IE           0x80
#define MCP2515_TX2IE           0x10
#define MCP2515_ERRIE           0x20
#define MCP2515_WAKIE           0x40
#define MCP2515_MERRE           0x80

#define MCP2515_RX0IF           0x01
#define MCP2515_RX1IF           0x02
#define MCP2515_TX0IF           0x04
#define MCP2515_TX1IF           0x80
#define MCP2515_TX2IF           0x10
#define MCP2515_ERRIF           0x20
#define MCP2515_WAKIF           0x40
#define MCP2515_MERRF           0x80

#define MCP2515_B1BFS           0x20
#define MCP2515_B0BFS           0x10
#define MCP2515_B1BFE           0x08
#define MCP2515_B0BFE           0x04
#define MCP2515_B1BFM           0x02
#define MCP2515_B0BFM           0x01

// #define MCP2515_SJW             0xC0
// #define MCP2515_BRP             0x3F

#define MCP2515_BTLMODE         0x80
#define MCP2515_3_SAM           0x40
#define MCP2515_1_SAM           0x00
#define MCP2515_PHSEG1          0x38
#define MCP2515_PRSEG           0x07

#define MCP2515_WAKFIL          0x40
#define MCP2515_PHSEG2          0x07

#define MCP2515_TXB2RTS         0x04
#define MCP2515_TXB1RTS         0x02
#define MCP2515_TXB0RTS         0x01

#define MCP2515_SJW_1TQ         0x00
#define MCP2515_SJW_2TQ         0x40
#define MCP2515_SJW_3TQ         0x80
#define MCP2515_SJW_4TQ         0xC0

#define MCP2515_BRP_2_F         0x00
#define MCP2515_BRP_4_F         0x01

#define MCP2515_BTLMODE_CNF3    0x80
#define MCP2515_BTLMODE_PH1_IPT 0x00

#define MCP2515_SMPL_3X         0x40
#define MCP2515_SMPL_1X         0x00

#define MCP2515_PHSEG1_8TQ      0x38
#define MCP2515_PHSEG1_7TQ      0x30
#define MCP2515_PHSEG1_6TQ      0x28
#define MCP2515_PHSEG1_5TQ      0x20
#define MCP2515_PHSEG1_4TQ      0x18
#define MCP2515_PHSEG1_3TQ      0x10
#define MCP2515_PHSEG1_2TQ      0x08
#define MCP2515_PHSEG1_1TQ      0x00

#define MCP2515_PRSEG_8TQ       0x07
#define MCP2515_PRSEG_7TQ       0x06
#define MCP2515_PRSEG_6TQ       0x05
#define MCP2515_PRSEG_5TQ       0x04
#define MCP2515_PRSEG_4TQ       0x03
#define MCP2515_PRSEG_3TQ       0x02
#define MCP2515_PRSEG_2TQ       0x01
#define MCP2515_PRSEG_1TQ       0x00

#define MCP2515_PHSEG2_8TQ      0x07
#define MCP2515_PHSEG2_7TQ      0x06
#define MCP2515_PHSEG2_6TQ      0x05
#define MCP2515_PHSEG2_5TQ      0x04
#define MCP2515_PHSEG2_4TQ      0x03
#define MCP2515_PHSEG2_3TQ      0x02
#define MCP2515_PHSEG2_2TQ      0x01
#define MCP2515_PHSEG2_1TQ      0x00

#define MCP2515_SOF_ENABLED     0x80
#define MCP2515_SOF_DISABLED    0x00
#define MCP2515_WAKFIL_ENABLED  0x40
#define MCP2515_WAKFIL_DISABLED 0x00


#define MCP2515_RX0IE_ENABLED   0x01
#define MCP2515_RX0IE_DISABLED  0x00
#define MCP2515_RX1IE_ENABLED   0x02
#define MCP2515_RX1IE_DISABLED  0x00
#define MCP2515_G_RXIE_ENABLED  0x03
#define MCP2515_G_RXIE_DISABLED 0x00

#define MCP2515_TX0IE_ENABLED   0x04
#define MCP2515_TX0IE_DISABLED  0x00
#define MCP2515_TX1IE_ENABLED   0x08
#define MCP2515_TX2IE_DISABLED  0x00
#define MCP2515_TX2IE_ENABLED   0x10
#define MCP2515_TX2IE_DISABLED  0x00
#define MCP2515_G_TXIE_ENABLED  0x1C
#define MCP2515_G_TXIE_DISABLED 0x00

#define MCP2515_ERRIE_ENABLED   0x20
#define MCP2515_ERRIE_DISABLED  0x00
#define MCP2515_WAKIE_ENABLED   0x40
#define MCP2515_WAKIE_DISABLED  0x00
#define MCP2515_IVRE_ENABLED    0x80
#define MCP2515_IVRE_DISABLED   0x00

#define MCP2515_RX0IF_SET       0x01
#define MCP2515_RX0IF_RESET     0x00
#define MCP2515_RX1IF_SET       0x02
#define MCP2515_RX1IF_RESET     0x00
#define MCP2515_TX0IF_SET       0x04
#define MCP2515_TX0IF_RESET     0x00
#define MCP2515_TX1IF_SET       0x08
#define MCP2515_TX2IF_RESET     0x00
#define MCP2515_TX2IF_SET       0x10
#define MCP2515_TX2IF_RESET     0x00
#define MCP2515_ERRIF_SET       0x20
#define MCP2515_ERRIF_RESET     0x00
#define MCP2515_WAKIF_SET       0x40
#define MCP2515_WAKIF_RESET     0x00
#define MCP2515_IVRF_SET        0x80
#define MCP2515_IVRF_RESET      0x00

#define MCP2515_REQOP_CONFIG    0x80
#define MCP2515_REQOP_LISTEN    0x60
#define MCP2515_REQOP_LOOPBACK  0x40
#define MCP2515_REQOP_SLEEP     0x20
#define MCP2515_REQOP_NORMAL    0x00

#define MCP2515_ABORT_ENABLED   0x10
#define MCP2515_ABORT_DISABLED  0x00

#define MCP2515_OSM_ENABLED     0x08
#define MCP2515_OSM_DISABLED    0x00

#define MCP2515_CLKOUT_ENABLED  0x04
#define MCP2515_CLKOUT_DISABLED 0x00
#define MCP2515_CLKOUT_PRE_8    0x03
#define MCP2515_CLKOUT_PRE_4    0x02
#define MCP2515_CLKOUT_PRE_2    0x01
#define MCP2515_CLKOUT_PRE_1    0x00

#define MCP2515_OPMODE_CONFIG   0x80
#define MCP2515_OPMODE_LISTEN   0x60
#define MCP2515_OPMODE_LOOPBACK 0x40
#define MCP2515_OPMODE_SLEEP    0x20
#define MCP2515_OPMODE_NORMAL   0x00


#define MCP2515_RXM_RCV_ALL     0x60
#define MCP2515_RXM_VALID_EXT   0x40
#define MCP2515_RXM_VALID_STD   0x20
#define MCP2515_RXM_VALID_ALL   0x00

#define MCP2515_RXRTR_REMOTE    0x08
#define MCP2515_RXRTR_NO_REMOTE 0x00

#define MCP2515_BUKT_ROLLOVER    0x04
#define MCP2515_BUKT_NO_ROLLOVER 0x00

#define MCP2515_FILHIT0_FLTR_1  0x01
#define MCP2515_FILHIT0_FLTR_0  0x00

#define MCP2515_FILHIT1_FLTR_5  0x05
#define MCP2515_FILHIT1_FLTR_4  0x04
#define MCP2515_FILHIT1_FLTR_3  0x03
#define MCP2515_FILHIT1_FLTR_2  0x02
#define MCP2515_FILHIT1_FLTR_1  0x01
#define MCP2515_FILHIT1_FLTR_0  0x00

#define MCP2515_TXREQ_CLEAR     0x00
#define MCP2515_TXREQ_SET       0x08

#define MCP2515_TXP_LOWEST      0x00
#define MCP2515_TXP_INTER_LOW   0x01    
#define MCP2515_TXP_INTER_HIGH  0x02
#define MCP2515_TXP_HIGHEST     0x03

 #define MCP2515_DLC_0          0x00
 #define MCP2515_DLC_1          0x01
 #define MCP2515_DLC_2          0x02
 #define MCP2515_DLC_3          0x03
 #define MCP2515_DLC_4          0x04
 #define MCP2515_DLC_5          0x05
 #define MCP2515_DLC_6          0x06
 #define MCP2515_DLC_7          0x07    
 #define MCP2515_DLC_8          0x08
 
#define MCP2515_CAN_WRITE       0x02
#define MCP2515_CAN_READ        0x03
#define MCP2515_CAN_BIT_MODIFY  0x05 
#define MCP2515_CAN_LOAD_TX     0X40 
#define MCP2515_CAN_RTS         0x80
#define MCP2515_CAN_RTS_TXB0    0x81
#define MCP2515_CAN_RTS_TXB1    0x82
#define MCP2515_CAN_RTS_TXB2    0x84
#define MCP2515_CAN_RD_RX_BUFF  0x90
#define MCP2515_CAN_RD_STATUS   0xA0 
#define MCP2515_CAN_RX_STATUS   0xB0
#define MCP2515_CAN_RESET       0xC0 

#define MCP2515_DUMMY_BYTE      0x00
#define MCP2515_TXB0            0x31
#define MCP2515_TXB1            0x41
#define MCP2515_TXB2            0x51
#define MCP2515_RXB0            0x61
#define MCP2515_RXB1            0x71
#define MCP2515_EXIDE_SET       0x08
#define MCP2515_EXIDE_RESET     0x00

#define SYS_FOSC_4MHZ           0x04
#define SYS_FOSC_8MHZ           0x08
#define CAN_BRP_125             0x7D
#define CAN_BRP_250             0xFA
#define CAN_BRP_500             0x01F4

/***************��������************/
/**/
    static uint32_t Send_Data_ID = 0;
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
    if (Fosc_Val == SYS_FOSC_8MHZ)
    {
        if (BRP_Val == CAN_BRP_250)
        {
            //250Kbps
            //BRP=X=0; Fosc=8; Y=0.5; 8Y=4; 1/8Y=1/4=0.25;
            MCP2515_WriteByte(MCP2515_CNF1,MCP2515_SJW_1TQ|MCP2515_BRP_2_F);
            MCP2515_WriteByte(MCP2515_CNF2,MCP2515_BTLMODE|MCP2515_1_SAM|MCP2515_PHSEG1_7TQ|MCP2515_PRSEG_6TQ);    	
            MCP2515_WriteByte(MCP2515_CNF3,MCP2515_SOF_DISABLED|MCP2515_WAKFIL_DISABLED|MCP2515_PHSEG2_2TQ); 
	            
        }
        else if (BRP_Val == CAN_BRP_500)
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
    u8 MCP2515_RxID[4]={0x18,0xFF,0x1D,0x00}; 
    u8 id_buf[4] = {0}; 
    SPI_Init();   
    delay_10us(10);   

    MCP2515_SILENT_DIR = 0;
    MCP2515_CS_DIR     = 0;
    MCP2515_RESET_DIR  = 0;   
    
    MCP2515_SILENT_OFF();
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

    id_buf[0] = (Send_Data_ID>>24)&0xFF;
    id_buf[1] = (Send_Data_ID>>16)&0xFF;
    id_buf[2] = (Send_Data_ID>>8)&0xFF;
    id_buf[3] = (Send_Data_ID>>0)&0xFF;
    MCP2515_CAN_TxID(id_buf,8); //����ID���� 
    delay_10us(10); 

    //ֻ������չ֡��������
	MCP2515_WriteByte(MCP2515_RXB0CTRL,0x44);  
	MCP2515_WriteByte(MCP2515_RXB1CTRL,0x40);       
#if 0                                
	MCP2515_CAN_RxID(MCP2515_RxID,8);//����ID����
    delay_10us(10); 
#endif    

    MCP2515_CAN_BaudRate(SYS_FOSC_8MHZ, CAN_BRP_250);//����������
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
void MCP2515_Data_Send(uint32_t ID, u8 *Dtat, u8 Len)
{
	u8 i = 0;
    u8 Cnt = 0;
    u8 SIDH = 0;
    u8 SIDL = 0;   
    u8 id_buf[4] = {0};     


    if(ID != Send_Data_ID)
    {
        delay_10us(200); 
        
        Send_Data_ID = ID;    
        id_buf[0] = (Send_Data_ID>>24)&0xFF;
        id_buf[1] = (Send_Data_ID>>16)&0xFF;
        id_buf[2] = (Send_Data_ID>>8)&0xFF;
        id_buf[3] = (Send_Data_ID>>0)&0xFF;
                     
        SIDL = (id_buf[1] & 0xF0) >> 5;
        SIDH = (id_buf[0] << 3) + SIDL;

        SIDL = (id_buf[1] & 0x1C) << 3;
        SIDL = (id_buf[1] & 0x03) + SIDL + 0x08;

        //���ͻ���Ĵ����ı�ʶ�����ã���չ��
        
        MCP2515_WriteByte(MCP2515_TXB0SIDH,SIDH); 
        delay_10us(100); 
        MCP2515_WriteByte(MCP2515_TXB0SIDL,SIDL);
        delay_10us(100);     
        MCP2515_WriteByte(MCP2515_TXB0EID8,id_buf[2]);
        delay_10us(100);     
        MCP2515_WriteByte(MCP2515_TXB0EID0,id_buf[3]);
        delay_10us(100);                 
    }
    
    //��ȡ״ֵ̬���Ƿ���Է��� 
    while((MCP2515_ReadByte(MCP2515_TXB0CTRL)&0x0B) && (Cnt<250))   
    {			
        Cnt++;
    }

    //д�뷢�����ݵ����ͻ����� 
    for(i=0; i<Len; i++)
    {
        MCP2515_WriteByte(MCP2515_TXB0D0 + i, Dtat[i]);
        delay_10us(5);                
    }

    MCP2515_CS = 0;    
    MCP2515_WriteByte(MCP2515_TXB0DLC, Len);//д�뷢�͵����ݳ���
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


#include "can.h"

//CAN����
#define CAN_TX_Out()    (TRISE5 = 0) 						       //CANTX/RE5Ϊ���
#define CAN_RX_In()     (TRISE4 = 1)						       //CANRX/RE4Ϊ����

bit CAN2_RXD_Interrupt_Flag_g = 0;					    	//RX0�����жϱ�־λ "=1"��ʾ��CAN2�н��յ��ź�

static void CAN_Baud_Rate_Set(void)
{
    //CANʱ��F OSC 
    BRGCON1bits.SJW = 0b00;//ͬ����ת���λ
                        //11 = ͬ����ת���ʱ�� = 4 x T Q
                        //10 = ͬ����ת���ʱ�� = 3 x T Q
                        //01 = ͬ����ת���ʱ�� = 2 x T Q
                        //00 = ͬ����ת���ʱ�� = 1 x T Q
    
    BRGCON1bits.BRP = 0;//������Ԥ��Ƶ��λ
                        //111111 = T Q = (2 x 64)/F OSC
                        //111110 = T Q = (2 x 63)/F OSC
                        //:
                        //:
                        //000001 = T Q = (2 x 2)/F OSC
                        //000000 = T Q = (2 x 1)/F OSC    
    
    BRGCON2bits.SEG2PHTS = 1;//����λ����� 2 ʱ��ѡ��λ    1  = �����ɱ��      0 = PHEG1 �����ֵ����Ϣ����ʱ�䣨Information Processing Time�� IPT���еĽϴ�ֵ
    BRGCON2bits.SAM = 0;//CAN ������·����λ  1 = �ڲ�����֮ǰ��������·��������   0 = �ڲ������������·����һ��
    BRGCON2bits.SEG1PH = 0b110; // ����λ����� 1 λ
                            //111 = ��λ����� 1 ʱ�� = 8 x T Q
                            //110 = ��λ����� 1 ʱ�� = 7 x T Q
                            //101 = ��λ����� 1 ʱ�� = 6 x T Q
                            //100 = ��λ����� 1 ʱ�� = 5 x T Q
                            //011 = ��λ����� 1 ʱ�� = 4 x T Q
                            //010 = ��λ����� 1 ʱ�� = 3 x T Q
                            //001 = ��λ����� 1 ʱ�� = 2 x T Q
                            //000 = ��λ����� 1 ʱ�� = 1 x T Q    
    BRGCON2bits.PRSEG = 0b101;  // ������ʱ��ѡ��λ
                            //111 = ����ʱ�� = 8 x T Q
                            //110 = ����ʱ�� = 7 x T Q
                            //101 = ����ʱ�� = 6 x T Q
                            //100 = ����ʱ�� = 5 x T Q
                            //011 = ����ʱ�� = 4 x T Q
                            //010 = ����ʱ�� = 3 x T Q
                            //001 = ����ʱ�� = 2 x T Q
                            //000 = ����ʱ�� = 1 x T Q    
    
    BRGCON3bits.WAKDIS = 0;// ���ѽ�ֹλ    1 = ��ֹ CAN ���߻���ѹ���   0 = ʹ�� CAN ���߻���ѹ���
    BRGCON3bits.WAKFIL = 1;// ѡ���Ƿ�ʹ�� CAN ������·�˲������ѵ�λ   1 = ʹ�� CAN ������·�˲���������  0 = ��ʹ�� CAN ������·�˲���������
    BRGCON3bits.SEG2PH = 0b001;// ��λ����� 2 ʱ��ѡ��λ (1)
                        //111 = ��λ����� 2 ʱ�� = 8 x T Q
                        //110 = ��λ����� 2 ʱ�� = 7 x T Q
                        //101 = ��λ����� 2 ʱ�� = 6 x T Q
                        //100 = ��λ����� 2 ʱ�� = 5 x T Q
                        //011 = ��λ����� 2 ʱ�� = 4 x T Q
                        //010 = ��λ����� 2 ʱ�� = 3 x T Q
                        //001 = ��λ����� 2 ʱ�� = 2 x T Q
                        //000 = ��λ����� 2 ʱ�� = 1 x T Q    
    
    //TQ=((2*(BRP+1))/Fosc= 2/4M = 0.5us 
    //λʱ�䣽TQ*(Sync_Seg+ Prop_Seg+ Phase_seg1+ Phase_seg2)=(1+1+3+3)TQ��8TQ
    //λ��=1/(8*0.5us)=250Kbps
									
	TXB0CON=0x03;					//�������ȼ�Ϊ������ȼ���TXPRI=11     
}

static void CAN_TXD_ID_Config(uint32_t tx_id)
{
    uint8_t RX_TX_SID_H, RX_TX_SID_L, RX_TX_EID_H, RX_TX_EID_L;
	uint8_t ID_L = 0X00;
	uint8_t ID_H = 0X00;    
    
    RX_TX_SID_H = (tx_id>>24)&0xFF;
    RX_TX_SID_L = (tx_id>>16)&0xFF;
    RX_TX_EID_H = (tx_id>>8)&0xFF;
    RX_TX_EID_L = (tx_id)&0xFF;
	
	ID_L = (RX_TX_SID_L & 0XF0) >> 5;
	ID_H = (RX_TX_SID_H << 3) | ID_L;				//���ù�����SIDH
	ID_L = (RX_TX_SID_L & 0X1C) << 3;
	ID_L = (RX_TX_SID_L & 0X03) | ID_L | 0X08;		//���ù�����SIDL(��չ֡)	    
       
    TXB0SIDH = ID_H;
    TXB0SIDL = ID_L;
    TXB0EIDH = RX_TX_EID_H;
    TXB0EIDL = RX_TX_EID_L;		    

    //��շ��ͻ�����
	TXB0D0=0X00;												
	TXB0D1=0X00;
	TXB0D2=0X00;
	TXB0D3=0X00;
	TXB0D4=0X00;
	TXB0D5=0X00;
	TXB0D6=0X00;
	TXB0D7=0X00;       
}

static void CAN_RXD_ID_Config(uint8_t chan, uint32_t rx_id, uint32_t rxid_mask)
{
    uint8_t Rx_Id[4] = {0};
    uint8_t RX_TX_SID_H, RX_TX_SID_L, RX_TX_EID_H, RX_TX_EID_L;
	uint8_t ID_L = 0X00;
	uint8_t ID_H = 0X00;        
    
    if(chan == 0)//ͨ��0
    {
        RXB0CON=0x40;  												// bit6-5=10//��������չ��ʶ������Ч��Ϣ��FILHIT0=0��ʾRXB0 ������FILHIT0   

        RX_TX_SID_H = (rx_id>>24)&0xFF;
        RX_TX_SID_L = (rx_id>>16)&0xFF;
        RX_TX_EID_H = (rx_id>>8)&0xFF;
        RX_TX_EID_L = (rx_id)&0xFF;  

        ID_L = (RX_TX_SID_L & 0XF0) >> 5;
        ID_H = (RX_TX_SID_H << 3) | ID_L;				//���ù�����SIDH
        ID_L = (RX_TX_SID_L & 0X1C) << 3;
        ID_L = (RX_TX_SID_L & 0X03) | ID_L | 0X08;		//���ù�����SIDL(��չ֡)	        

        RXF0SIDH = ID_H;		
        RXF0SIDL = ID_L;
        RXF0EIDH = RX_TX_EID_H;
        RXF0EIDL = RX_TX_EID_L;		  

        Rx_Id[0] = (rxid_mask>>24)&0xFF;
        Rx_Id[1] = (rxid_mask>>16)&0xFF;
        Rx_Id[2] = (rxid_mask>>8)&0xFF;
        Rx_Id[3] = (rxid_mask)&0xFF;  

        RXM0SIDH = (Rx_Id[0] << 3)|(Rx_Id[1] >> 5);		
        RXM0SIDL = ((Rx_Id[1]&0X03)|0X08)|((Rx_Id[1]<<3)&0XE0);										//��3λΪ��ʶ����3λ����5λ������
        RXM0EIDH = Rx_Id[2];
        RXM0EIDL = Rx_Id[3];    

        //��ʼ�����ܻ�����0������������
        RXB0D0=0X00;												
        RXB0D1=0X00;
        RXB0D2=0X00;
        RXB0D3=0X00;
        RXB0D4=0X00;
        RXB0D5=0X00;
        RXB0D6=0X00;
        RXB0D7=0X00;            
    }
    else if(chan == 1)//ͨ��1
    {
        RXB1CON=0x40;  												// bit6-5=10//��������չ��ʶ������Ч��Ϣ��FILHIT0=0��ʾRXB0 ������FILHIT0   

        RX_TX_SID_H = (rx_id>>24)&0xFF;
        RX_TX_SID_L = (rx_id>>16)&0xFF;
        RX_TX_EID_H = (rx_id>>8)&0xFF;
        RX_TX_EID_L = (rx_id)&0xFF;  

        ID_L = (RX_TX_SID_L & 0XF0) >> 5;
        ID_H = (RX_TX_SID_H << 3) | ID_L;				//���ù�����SIDH
        ID_L = (RX_TX_SID_L & 0X1C) << 3;
        ID_L = (RX_TX_SID_L & 0X03) | ID_L | 0X08;		//���ù�����SIDL(��չ֡)	        

        RXF1SIDH = ID_H;		
        RXF1SIDL = ID_L;
        RXF1EIDH = RX_TX_EID_H;
        RXF1EIDL = RX_TX_EID_L;		  

        Rx_Id[0] = (rxid_mask>>24)&0xFF;
        Rx_Id[1] = (rxid_mask>>16)&0xFF;
        Rx_Id[2] = (rxid_mask>>8)&0xFF;
        Rx_Id[3] = (rxid_mask)&0xFF;  

        RXM1SIDH = (Rx_Id[0] << 3)|(Rx_Id[1] >> 5);		
        RXM1SIDL = ((Rx_Id[1]&0X03)|0X08)|((Rx_Id[1]<<3)&0XE0);										//��3λΪ��ʶ����3λ����5λ������
        RXM1EIDH = Rx_Id[2];
        RXM1EIDL = Rx_Id[3];    

        //��ʼ�����ܻ�����0������������
        RXB1D0=0X00;												
        RXB1D1=0X00;
        RXB1D2=0X00;
        RXB1D3=0X00;
        RXB1D4=0X00;
        RXB1D5=0X00;
        RXB1D6=0X00;
        RXB1D7=0X00;             
    }
}

void CAN_Init(void)
{
    CAN_STB_Out();		
	CAN_STB_OFF();						//����CANʹ��PIN
    
    CAN_TX_Out();
    CAN_RX_In();      
    
    CANCON=0X80;					//�������CAN����ģʽREQOP=100	(���ƼĴ���)
	while((CANSTAT&0X80)==0);		//�ȴ�����CAN����ģʽOPMODE=100(״̬�Ĵ���)	

    CAN_Baud_Rate_Set();

    //��������
    CAN_TXD_ID_Config(0x18FF741D);

    //��������
    CAN_RXD_ID_Config(0, 0x18FF0000, 0x0);//ͨ��0��0��ȫ��  
    CAN_RXD_ID_Config(1, 0x04020000, 0x0);//ͨ��1��
      
	CIOCON=0X21;												//��ʼ��CANģ���I/O���ƼĴ���	 	 	
 	CANCON=0X00;												//������������ģʽ
	while((CANSTAT&0x40)!=0); 									//����������

	PIR5=0X00;													//�������жϱ�־
	PIE5=0X01;													//ʹ�ܽ��ջ�����0��1 �Ľ����жϣ����򿪽����ж�ʹ�ܣ�    
    
    CAN_STB_ON();												//����CAN�շ���
}

void CAN_DeInit(void)
{
    CAN_STB_OFF();						//����CANʹ��PIN
}

void CAN_Send_Data(uint32_t id, uint8_t len, uint8_t *buf) //����0����DLC���ȵı���
{   
    if(CAN_STB_Pin)return;
    
    uint8_t i = 0;
    while(TXB0CON & 0x08)
    {
        i++;
        if(i>250)break;
    }    
    
    CAN_TXD_ID_Config(id);
    
	TXB0CON = (TXB0CON&0xf7);				//TXREQ��0���ȣ���ֹ����	
	TXB0IF = 0;								//��շ�����ɱ�־		
	TXB0DLC = len;							//���÷������ݵĳ���Ϊ8���ֽ�
	
    for(i=0; i<len; i++)
	{	
		*(&TXB0D0 + i) = buf[i];				
	} 
	TXB0CON=(TXB0CON|0x08);					//׼����ϣ�������,TXREQ=1
}

uint8_t CAN_Rece_Data(uint32_t *id, uint8_t *len, uint8_t *buf)
{
	uint8_t i = 0;
    uint32_t sid_tem = 0; 
    uint16_t sid_tem1 = 0; 
    uint16_t sid_tem2 = 0;
    if(CAN_STB_Pin)
    {
        RXFUL = 0;//�򿪽��ջ���������������Ϣ
        return 0;
    }
    
    sid_tem1 = (RXB0SIDH<<8)| RXB0SIDL;     
    sid_tem = ((sid_tem1>>3)&(~0x03))|(sid_tem1 & 0x03);   
    sid_tem2 = (RXB0EIDH<<8)|RXB0EIDL;
    *id = (sid_tem<<16)|sid_tem2;
	*len = RXB0DLC;
	for(i=0; i<(*len); i++)
	{	
		buf[i] = *(&RXB0D0 + i);				
	}  
    RXFUL = 0;//�򿪽��ջ���������������Ϣ		
    return 1;
}


/*----------------------------------------------------------------------*/
/* Confidential and Proprietary Information of MicroPerfect Corporation */
/* (C) 2019 ,MicroPerfect Corporation . All rights reserved. */
/* FileName : delay.c */
/* Function : */
/* Ver: 0.01  */
/* MP_Ahren NewFile */
/* History:YYYY.MM.DD Version author Description 
          :2019.12.04 0.01 @ahren Follow the coding standard  */
/*-------------------------------------------------------------*/

/***************ͷ�ļ�****************/
/**/
/*************************************/
#include"MCP2515_App.h"

//******************�궨��*************/
/**/
u8 MCP2515_Buff[9] = {0};

extern u8 TC04_One_Buff[9];//1�ŵ����CAN����
extern u8 TC04_Two_Buff[9];//2�ŵ����CAN����
extern u8 TC04_Three_Buff[9];//3�ŵ����CAN����
extern u8 TC04_Four_Buff[9];//4�ŵ����CAN����
extern u8 TC04_Five_Buff[9];//5�ŵ����CAN����
extern u8 TC04_Six_Buff[9];//6�ŵ����CAN����
extern u8 TC04_Seven_Buff[9];//7�ŵ����CAN����
extern u8 TC04_Eight_Buff[9];//8�ŵ����CAN����
extern u8 TC04_Nine_Buff[9];//9�ŵ����CAN����
extern u8 TC04_Ten_Buff[9];//10�ŵ����CAN����
extern u8 TC04_Eleven_Buff[9];//11�ŵ����CAN����
extern u8 TC04_Twelve_Buff[9];//12�ŵ����CAN����
extern u8 TC04_Thirteen_Buff[9];//13�ŵ����CAN����
extern u8 TC04_Fourteen_Buff[9];//14�ŵ����CAN����
extern u8 TC04_Fifteen_Buff[9];//15�ŵ����CAN����
extern u8 TC04_Sixteen_Buff[9];//16�ŵ����CAN����
/*************************************/


/*************************************/

/***************��־λ����************/
/**/
extern u8 Cnt_Num;

/*************************************/


/***************��������**************/
/**/
void MCP2515_Data_Analysis(u8 Num_Cnt, u8 *Data_Buff, u8 Life_Cnt);
/*************************************/

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
	u8 i = 0;
    u8 Cnt = 0;

    memcpy(MCP2515_Buff, CAN_TX_Buf, 8);

    MCP2515_Data_Analysis(BOX_Num, MCP2515_Buff, Life_Signal);
    //��ȡ״ֵ̬���Ƿ���Է��� 
    while((MCP2515_ReadByte(MCP2515_TXB0CTRL)&0x0B) && (Cnt<50))   
    {			
        Cnt++;
    }

    //д�뷢�����ݵ����ͻ����� 
    for(i=0; i<len; i++)
    {
        MCP2515_WriteByte(MCP2515_TXB0D0 + i, MCP2515_Buff[i]);               
    } 

    MCP2515_CS = 0;          
    MCP2515_WriteByte(MCP2515_TXB0DLC, len);//д�뷢�͵����ݳ���                                 
    MCP2515_WriteByte(MCP2515_TXB0CTRL,0x0B);//���ͱ���                           
    MCP2515_CS = 1;  
}

/*************************************************************************/
//������:TBOX_MCP2515_Sent
//��  ��:���͵�TBOX
//��  ��:���������׵�ַ�����鳤��
//����ֵ����
//ʱ�䣺20190803
//�����ߣ�ahren
/************************************************************************/
void Version_MCP2515_Sent(u8 *CAN_TX_Buf, u8 len)
{
	u8 i = 0;
    u8 Cnt = 0;

    while((MCP2515_ReadByte(MCP2515_TXB0CTRL)&0x0B) && (Cnt<50))        //��ȡ״ֵ̬���Ƿ���Է���
    {			
        Cnt++;
    }

    for(i=0; i<len; i++)
    {
        MCP2515_WriteByte(MCP2515_TXB0D0+i, Version_Data_Normal[i]);            //д�뷢�����ݵ����ͻ�����    
    }

    MCP2515_CS = 0;
    MCP2515_WriteByte(MCP2515_TXB0DLC, len);                            //д�뷢�͵����ݳ���
    MCP2515_WriteByte(MCP2515_TXB0CTRL,0x0B);                           //���ͱ���
    MCP2515_CS = 1;  
}

void Version_ID_Change(void)
{
    MCP2515_TxID[0] = 0x18;              
    MCP2515_TxID[1] = 0xFE;   
    MCP2515_TxID[2] = 0xDA;  
    MCP2515_TxID[3] = 0x1D;      
    MCP2515_CAN_TxID(MCP2515_TxID,8);//����ID���� 
    delay_10us(100); 
    NOP();NOP();NOP();NOP();NOP();NOP(); 
}

void Meter_ID_Change(void)
{
    MCP2515_TxID[0] = 0x18;                 
    MCP2515_TxID[1] = 0xFF;   
    MCP2515_TxID[2] = 0x30;
    MCP2515_TxID[3] = 0x1D;                              
    MCP2515_CAN_TxID(MCP2515_TxID,8);//����ID���� 
    delay_10us(100);     
    NOP();NOP();NOP();NOP();NOP();NOP(); 
}

void Device_Data_Conversion(u8 Dev_Num, uint16 Life_Signal)
{
    u8 Dev_Val = 0;//�豸���
    uint16 Life_Val = 0;//�ź�ֵ

    static u8 Offset_Val = 1;//Dev_Num ƫ����
    static u8 Length = 0x08;//���ݷ��ͳ���

    Dev_Val = Dev_Num;
    Life_Val = Life_Signal;

    switch (Dev_Val)
    {
        case 0x01:
            MCP2515_CAN_Tx_Buffer(Dev_Val-Offset_Val, TC04_One_Buff, Length, Life_Val);                          
            break;
        case 0x02:
            MCP2515_CAN_Tx_Buffer(Dev_Val-Offset_Val, TC04_Two_Buff, Length, Life_Val); 
            break;
        case 0x03:
            MCP2515_CAN_Tx_Buffer(Dev_Val-Offset_Val, TC04_Three_Buff, Length, Life_Val);
            break;
        case 0x04:
            MCP2515_CAN_Tx_Buffer(Dev_Val-Offset_Val, TC04_Four_Buff, Length, Life_Val); 
            break;
        case 0x05:
            MCP2515_CAN_Tx_Buffer(Dev_Val-Offset_Val, TC04_Five_Buff, Length, Life_Val); 
            break;   
        case 0x06:
            MCP2515_CAN_Tx_Buffer(Dev_Val-Offset_Val, TC04_Six_Buff, Length, Life_Val); 
            break;                 
        case 0x07:
            MCP2515_CAN_Tx_Buffer(Dev_Val-Offset_Val, TC04_Seven_Buff, Length, Life_Val);  
            break;
        case 0x08:
            MCP2515_CAN_Tx_Buffer(Dev_Val-Offset_Val, TC04_Eight_Buff, Length, Life_Val);  
            break;
        case 0x09:
            MCP2515_CAN_Tx_Buffer(Dev_Val-Offset_Val, TC04_Nine_Buff, Length, Life_Val);  
            break;   
        case 0x0A:
            MCP2515_CAN_Tx_Buffer(Dev_Val-Offset_Val, TC04_Ten_Buff, Length, Life_Val);  
            break;
        default:
            break;
    }
}

/*************************************************************************/
//������:MCP2515_User_Agreement
//��  ��:�����û�Э������
//��  ��:��
//����ֵ����
//ʱ�䣺20191206
//�����ߣ�ahren
/************************************************************************/
void MCP2515_User_Agreement(u8 Time_Flag)
{
    u8 MCP2515_Sent_Time_Flag = 0;//ʱ���־λ

    static u8 MCP2515_ID_Change_Flag = 0;//ID�л���־λ
    static u8 MCP2515_Sent_Cnt = 0;
    static u8 Ten_Seconds_Cnt = 0;//10S����
    static uint16 MCP2515_Life_Cnt = 0; 

    MCP2515_Sent_Time_Flag = Time_Flag;

#if 1    
    //MCP2515 �������ݽ����봦��
    if ((0x01 == MCP2515_Sent_Time_Flag))
    {
        MCP2515_Sent_Time_Flag = 0;         
        MCP2515_Life_Cnt++;//��Ϣ����
        MCP2515_Sent_Cnt++;//������ż��� 

        if (MCP2515_Life_Cnt > 0xFF)//��Ϣ�������255
        {
            MCP2515_Life_Cnt = 0;
        }
        
        if (0x01 == MCP2515_ID_Change_Flag)//�Ǳ�ID�л� 
        {
            MCP2515_ID_Change_Flag = 0;     
            Meter_ID_Change();             
        }             
        //ѡ���Ӧ�ĵ�����ţ������ݷ��͵������Ǳ�
        if (0x00 == MCP2515_ID_Change_Flag)
        {
            Device_Data_Conversion(MCP2515_Sent_Cnt, MCP2515_Life_Cnt);
            //������������ж�
            if(MCP2515_Sent_Cnt == Cnt_Num)
            {
                MCP2515_Sent_Cnt = 0;  
                MCP2515_ID_Change_Flag = 0; 
            } 
        }
        /**********************************************/
        //���ݿͻ�Э�飬�޸ķ��Ͱ汾��������ʱ��(10s)
        //20191108
        //ahren
        /**********************************************/ 
        Ten_Seconds_Cnt++;
        if (0x0A == Ten_Seconds_Cnt)
        {
            Ten_Seconds_Cnt = 0;
            MCP2515_ID_Change_Flag = 1; 

            Version_ID_Change();//�汾����ID�л� 

            Version_MCP2515_Sent(Version_Data_Normal, 8);                              
        }    
        /**********************************************/           
    } 
#endif  
}

//*******************************************/
//������:MCP2515_Data_Analysis
//��  ��:�������ݽ���
//��  ��:��
//����ֵ:��
//ʱ  ��:20190819
//������:ahren
/*******************************************/
void MCP2515_Data_Analysis(u8 Num_Cnt, u8 *Data_Buff, u8 Life_Cnt)
{
    if (0x00 == Data_Buff[1])//̽����ϵͳ����
    {
        Data_Buff[0] = Num_Cnt;//�������  
        Data_Buff[1] = 0x00;//�����ϵͳ����
        Data_Buff[2] = 0x00;//�޹�����
        Data_Buff[3] = 0xFF;//Ԥ��
        Data_Buff[4] = 0xFF;//Ԥ��
        Data_Buff[5] = 0xFF;//Ԥ��
        Data_Buff[6] = 0xFF;//Ԥ��
        Data_Buff[7] = Life_Cnt;//������   
    }  
    else if (0x02 == Data_Buff[1])//̽����ϵͳ����Ԥ��
    {
        Data_Buff[0] = Num_Cnt;//�������
        Data_Buff[1] = 0x01;//Ԥ������  
        Data_Buff[2] = 0x00;//�޹�����
        Data_Buff[3] = 0xFF;//Ԥ��
        Data_Buff[4] = 0xFF;//Ԥ��
        Data_Buff[5] = 0xFF;//Ԥ��
        Data_Buff[6] = 0xFF;//Ԥ��
        Data_Buff[7] = Life_Cnt;//������         
    } 
    else if (0x03 == Data_Buff[1])//̽����ϵͳ����Ԥ��
    {
        Data_Buff[0] = Num_Cnt;//�������
        Data_Buff[1] = 0x02;//Ԥ������ 
        Data_Buff[2] = 0x00;//�޹�����
        Data_Buff[3] = 0xFF;//Ԥ��
        Data_Buff[4] = 0xFF;//Ԥ��
        Data_Buff[5] = 0xFF;//Ԥ��
        Data_Buff[6] = 0xFF;//Ԥ��
        Data_Buff[7] = Life_Cnt;//������         
    }  
    else if (0x04 == Data_Buff[1])//̽����ϵͳ�ļ�Ԥ��
    {
        Data_Buff[0] = Num_Cnt;//�������
        Data_Buff[1] = 0x03;//Ԥ������  
        Data_Buff[2] = 0x02;//̽����������
        Data_Buff[3] = 0xFF;//Ԥ��
        Data_Buff[4] = 0xFF;//Ԥ��
        Data_Buff[5] = 0xFF;//Ԥ��
        Data_Buff[6] = 0xFF;//Ԥ��
        Data_Buff[7] = Life_Cnt;//������
    }   
    else if (0xFF == Data_Buff[1])//̽����ϵͳ����
    {
        if(0x90 == Data_Buff[7])
        {
            Data_Buff[1] = 0x10;//ͨѶ����         
        }
        else if(0x88 == Data_Buff[7])
        {
            Data_Buff[1] = 0x18;//����������   
        }
        Data_Buff[0] = Num_Cnt;//������� 
        Data_Buff[2] = 0x00;//
        Data_Buff[3] = 0xFF;//Ԥ��
        Data_Buff[4] = 0xFF;//Ԥ��
        Data_Buff[5] = 0xFF;//Ԥ��
        Data_Buff[6] = 0xFF;//Ԥ��
        Data_Buff[7] = Life_Cnt;//������
    }
    else
    {

    }
}


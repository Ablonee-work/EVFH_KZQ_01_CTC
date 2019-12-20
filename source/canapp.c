
/*----------------------------------------------------------------------*/
/* Confidential and Proprietary Information of MicroPerfect Corporation */
/* (C) 2017 ,MicroPerfect Corporation . All rights reserved. */
/* FileName : led.c */
/* New File Date:2017.11.03 */
/* Function : power det (BU/ACC/Sleep)*/
/* Ver:001 */
/* Author:MP_Ahren*/
/* History:YYYY.MM.DD Version Author Description */
/* */
/*-------------------------------------------------------------*/


/***************ͷ�ļ�****************/
/**/
/*************************************/
#include"canapp.h"
//******************�궨��*************/
/**/
/*************************************/


/*************************************/

/***************��־λ����************/
/**/
u8 TC04_One_Buff[9]         = {0x00};//1�ŵ����CAN����
u8 TC04_Two_Buff[9]         = {0x00};//2�ŵ����CAN����
u8 TC04_Three_Buff[9]       = {0x00};//3�ŵ����CAN����
u8 TC04_Four_Buff[9]        = {0x00};//4�ŵ����CAN����
u8 TC04_Five_Buff[9]        = {0x00};//5�ŵ����CAN����
u8 TC04_Six_Buff[9]         = {0x00};//6�ŵ����CAN����
u8 TC04_Seven_Buff[9]       = {0x00};//7�ŵ����CAN����
u8 TC04_Eight_Buff[9]       = {0x00};//8�ŵ����CAN����
u8 TC04_Nine_Buff[9]        = {0x00};//9�ŵ����CAN����
u8 TC04_Ten_Buff[9]         = {0x00};//10�ŵ����CAN����
u8 TC04_Eleven_Buff[9]      = {0x00};//11�ŵ����CAN����
u8 TC04_Twelve_Buff[9]      = {0x00};//12�ŵ����CAN����
u8 TC04_Thirteen_Buff[9]    = {0x00};//13�ŵ����CAN����
u8 TC04_Fourteen_Buff[9]    = {0x00};//14�ŵ����CAN����
u8 TC04_Fifteen_Buff[9]     = {0x00};//15�ŵ����CAN����
u8 TC04_Sixteen_Buff[9]     = {0x00};//16�ŵ����CAN����

u8 TC04_CAN_Data_Buff[9]    = {0x00};//����TC04 CAN����
u8 TC04_ID_Num[20]          = {0x00};//ID���ݴ洢
u8 TC04_ID_Cnt[20]          = {0x00};//IDͳ��

u8 TBOX_Error_Buff_Flag[10] = {0x00};
//u8 TBOX_Error_Buff[10]      = {0x00};

u8 CAN_Data_Normal[9] = {00,00,0xFF,0xFF,0xFF,0xFF,0xFF,00,00};//TJA1042�������ݱ���
//u8 NULL_Buff[9] = {0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF};

u8 TC04_Off_Line_Val_Buff[20] = 0;//���߱�־λ
u8 TC04_Off_Line_Cnt_Buff[20] = 0;//���߱�־λ
u8 TC04_Off_Time_Cnt_g = 0;

u8 Sent_Flag   = 0;
u8 Sent_Cnt    = 0;
u8 Error_Flag  = 0;
u8 Error_Cnt   = 0;
u8 Cnt_Num     = 0;
u8 Time_5ms_Flag_g = 0;

/*************************************/

/***************��������**************/
/**/
void TC04_Num_Set(void);
void Equipment_Off_Line_Det(void);
u8 CAN_Sent_Time_Choice(void);
/*************************************/

//*******************************************/
//������:TC04_CAN_Data_Analysis
//��  ��:CAN���ݽ���
//��  ��:��
//����ֵ:��
//ʱ  ��:20190605
//������:ahren
/*******************************************/
void TC04_CAN_Data_Analysis(u8 ID,u8 *Data_Buff_New)
{
    u8 i = 0;
    u8 Box_Num_ID = 0;

    Box_Num_ID = ID;

    //����ȡ����CAN �������ݸ�ֵ������̽������������
    for(i=0; i<8; i++)
    {
        Data_Buff_New[i] = TC04_CAN_Data_Buff[i];
    }
    Data_Buff_New[0] = Box_Num_ID;//������Ÿ�ֵ����һ������
    //Data_Buff_New[6] = 0xFF;   
}

//*******************************************/
//������:Get_TC04_CAN_Data
//��  ��:��ȡ̽����CAN����
//��  ��:��
//����ֵ:��
//ʱ  ��:20190604
//������:ahren
/*******************************************/
void Get_TC04_CAN_Data(void)
{
    u8 BOX_ID_Num = 0x00;//�������

    //ֻ����CAN IDΪ 0x18FF01XX  ��̽�������� 
    if ((0xC7 == RXB0SIDH) && (0xFB == RXB0SIDL) && (0x01 == RXB0EIDH))
    {
        CAN_Receive_Interrupt_Data(CAN_Receive_Data);//��ȡCAN��������
        
        BOX_ID_Num = RXB0EIDL;//��ȡ�������
#if 0
        /*************************************************************/
        //����Ԥ����⡢���������ϼ��
        //20190802
        //ahren
        if((0x02 == CAN_Receive_Data[1]) || (0x03 == CAN_Receive_Data[1]) || (0x04 == CAN_Receive_Data[1]) || (0x88 == CAN_Receive_Data[7]))
        {
            CAN_Receive_Data[0] = 0x00;
            CAN_Receive_Data[1] = 0x00;
            CAN_Receive_Data[7] = 0x00;            
        }
#endif
        
#if 1
        /*************************************************************/
        //���δ��������ϼ��
        //20191101
        //ahren
        if((0x88 == CAN_Receive_Data[7]))
        {
            CAN_Receive_Data[0] = 0x00;
            CAN_Receive_Data[1] = 0x00;
            CAN_Receive_Data[7] = 0x00;            
        }
#endif        
        /*************************************************************/
        //�� CAN_Receive_Data ���Ƹ� TC04_CAN_Data_Buff        
        memcpy(TC04_CAN_Data_Buff, CAN_Receive_Data, 8); 
        
        //�ж�ѡ��洢����������
        switch (BOX_ID_Num)
        {
            case 0x01:
                TC04_CAN_Data_Analysis(BOX_ID_Num,TC04_One_Buff);   //��ȡ1�ŵ���䱨������
                break;
            case 0x02: 
                TC04_CAN_Data_Analysis(BOX_ID_Num,TC04_Two_Buff);   //��ȡ2�ŵ���䱨������     
                break;
            case 0x03: 
                TC04_CAN_Data_Analysis(BOX_ID_Num,TC04_Three_Buff); //��ȡ3�ŵ���䱨������      
                break;
            case 0x04:
                TC04_CAN_Data_Analysis(BOX_ID_Num,TC04_Four_Buff);  //��ȡ4�ŵ���䱨������        
                break; 
            case 0x05:
                TC04_CAN_Data_Analysis(BOX_ID_Num,TC04_Five_Buff);  //��ȡ5�ŵ���䱨������          
                break;
            case 0x06:
                TC04_CAN_Data_Analysis(BOX_ID_Num,TC04_Six_Buff);   //��ȡ6�ŵ���䱨������       
            break;      
            case 0x07: 
                TC04_CAN_Data_Analysis(BOX_ID_Num,TC04_Seven_Buff);//��ȡ7�ŵ���䱨������      
                break;
            case 0x08:
                TC04_CAN_Data_Analysis(BOX_ID_Num,TC04_Eight_Buff); //��ȡ8�ŵ���䱨������        
                break; 
            case 0x09:
                TC04_CAN_Data_Analysis(BOX_ID_Num,TC04_Nine_Buff);//��ȡ9�ŵ���䱨������          
                break;
            case 0x0A:
                TC04_CAN_Data_Analysis(BOX_ID_Num,TC04_Ten_Buff);//��ȡ10�ŵ���䱨������       
            break; 
            case 0x0B:
                TC04_CAN_Data_Analysis(BOX_ID_Num,TC04_Eleven_Buff);//��ȡ10�ŵ���䱨������       
            break;     
            case 0x0C:
                TC04_CAN_Data_Analysis(BOX_ID_Num,TC04_Twelve_Buff);//��ȡ10�ŵ���䱨������       
            break;  
            case 0x0D:
                TC04_CAN_Data_Analysis(BOX_ID_Num,TC04_Thirteen_Buff);//��ȡ10�ŵ���䱨������       
            break;  
            case 0x0E:
                TC04_CAN_Data_Analysis(BOX_ID_Num,TC04_Fourteen_Buff);//��ȡ10�ŵ���䱨������       
            break; 
            case 0x0F:
                TC04_CAN_Data_Analysis(BOX_ID_Num,TC04_Fifteen_Buff);//��ȡ10�ŵ���䱨������       
            break;   
            case 0x10:
                TC04_CAN_Data_Analysis(BOX_ID_Num,TC04_Sixteen_Buff);//��ȡ10�ŵ���䱨������       
            break;                                                                                          
            default:
                break;
        }	
    }
    else if((0xF6 == RXB0EIDL) && (0xF5 == RXB0EIDH)) //���õ��������ID��0x0402F5F6 
    {
        TC04_Num_Set(); //��ȡ���������           
    }
    else if ((0xC7 == RXB0SIDH) && (0xFB == RXB0SIDL) && (0x5E == RXB0EIDH))
    {
        //����������ͱ��Ĵ������
        //20190902
        //ahren
        CAN_Receive_Interrupt_Data(CAN_Receive_Data);               //��ȡCAN��������
        //memset(CAN_Receive_Data, 0, 8); 
    }
    else
    {
        //�յ���ָ��ID �ı���ʱ�������ر�־λ
        //20190902
        //ahren
        RXFUL=0;//�򿪽��ջ���������������Ϣ    
    }
      
}

//*******************************************/
//������:TC04_Off_Line
//��  �ܣ������豸�Ƿ�����
//��  ��:BOX_Num,Data_Buff,Life_Signal_Num
//����ֵ:Off_Line_Val
//ʱ  ��:20190605
//������:ahren
/*******************************************/
u8 TC04_Off_Line_Det(u8 BOX_Num, u8 *Data_Buff, u8 Life_Signal_Num)
{
    u8 Off_Line_Val = 0;

    u8 i = 0;
    //�ж������������Ƿ���ǰһ��һ��
    if (TC04_Off_Line_Val_Buff[BOX_Num] == Data_Buff[Life_Signal_Num])
    {

        //��������ͬ�ļ���������ֵ��Off_Line_Val
        Off_Line_Val = TC04_Off_Line_Cnt_Buff[BOX_Num];

        Off_Line_Val++;//�����ԼӼ���

        //���ԼӺ��ֵ��ֵ����Ӧ������Buff��         
        TC04_Off_Line_Cnt_Buff[BOX_Num] = Off_Line_Val;

    }
    else
    {
        //�����µ����������ݸ�ֵ����Ӧ���������ݵ�Buff��
        TC04_Off_Line_Val_Buff[BOX_Num] = Data_Buff[Life_Signal_Num];
        
        TC04_Off_Line_Cnt_Buff[BOX_Num] = 0;                            //��������������
    }
    //���߹��ϸ�ֵ
    if (Off_Line_Val >= 0x02)
    {
        Data_Buff[1] = 0xFF;
        Data_Buff[7] = 0x90;//�������������CAN���ĸ�ʽ������        
        //�ϵ�δ��̽���������
        if((0x00 != Data_Buff[1]) || (0x00 != Data_Buff[7]))
        {
            if(Data_Buff[0] == 0)//δ�յ��κ�̽��������
            {
                TC04_ID_Num[BOX_Num] = BOX_Num;//��������Ÿ�ֵ��ID����
                
                for(i=1; i<6; i++)
                {
                    Data_Buff[i] = 0xFF;//��������1-5�����ݣ���Ϊ0xFF                   
                }          
            }
        }      
    }
    return Off_Line_Val;
}

//*******************************************/
//������:Sent_TC04_CAN_Data
//��  ��:��ȡCAN��������
//��  ��:��
//����ֵ:��
//ʱ  ��:20190605
//������:ahren
/*******************************************/
void Sent_TC04_CAN_Data(void)
{
    u8 i = 0;
    u8 j = 0;
    u8 ID_Cnt = 0;
    u8 Sent_Time_Flag = 0;

    u8 TJA1042_Sent_Time_Flag = 0;
    u8 MCP2515_Sent_Time_Flag = 0;

    static u8 Cnt = 0;
    static u8 Length_Val = 0x08;//�������ݳ���
    static uint16 Life_Signal_Cnt = 0;
    static u8 TJA1042_Sent_Time_Cnt = 0;    

     u8 MCP2515_One_Sec_Time_Flag = 0;
    
    /*************************************************/
    //ÿ��5ms x N = 5Nms ����ÿ��̽��������������
    //NΪ̽��������
    //ahren
    //20190716
    CAN_Data_Choice_Analysis();
    /*************************************************/
    
    /*************************************************/
    //��ȡCAN��������ʱ�䷵��ֵ
    //ahren
    //20190719   
    Sent_Time_Flag = CAN_Sent_Time_Choice(); 

    /*************************************************/
    //�жϷ����豸�Ƿ�����
    Equipment_Off_Line_Det();  
    /*************************************************/
    //ȷ��TJA1042���͵�����һֱΪ1S
    //ahren
    //20190731 
    if (Sent_Time_Flag == 0x03)//1S����ֵ
    {
        Sent_Time_Flag = 0;
        TJA1042_Sent_Time_Flag = 1;
        MCP2515_Sent_Time_Flag = 1;
    }
    else if (Sent_Time_Flag == 0x02)//500ms����ֵ
    {
        Sent_Time_Flag = 0;
        TJA1042_Sent_Time_Cnt++;
        if (0x02 == TJA1042_Sent_Time_Cnt)
        {
            TJA1042_Sent_Time_Cnt = 0;
            TJA1042_Sent_Time_Flag = 1;
            MCP2515_Sent_Time_Flag = 1;            
        }      
    }
    else if (Sent_Time_Flag == 0x01)//250ms����ֵ
    {
        Sent_Time_Flag = 0;
        TJA1042_Sent_Time_Cnt++;   
        if (0x04 == TJA1042_Sent_Time_Cnt)
        {
            TJA1042_Sent_Time_Cnt = 0;
            TJA1042_Sent_Time_Flag = 1;
            MCP2515_Sent_Time_Flag = 1;
            MCP2515_One_Sec_Time_Flag = 1;            
        }      
    }    
    /*************************************************/    
  
    /*************************************************/     
    if(0x01 == TJA1042_Sent_Time_Flag)//��׼ʱ��Time1  1s     
    {
        //MCP_CAN_Send_1s_Flag_g = 0;
        TJA1042_Sent_Time_Flag = 0;
        //CAN����������
        Life_Signal_Cnt++;  
        if(Life_Signal_Cnt > 0xFF)
        {
            Life_Signal_Cnt = 0;    
        }        
#if 1
        /*************************************************/
        //ÿ��1s x N = Ns ���һ��̽�����Ƿ�����
        //NΪ̽��������
        //ahren
        //20190717      
        //Equipment_Off_Line_Det();
        /*************************************************/        
        if(Error_Flag == 0)
        {
            if ((Cnt_Num != 0) && (Cnt_Num != 0xFF))
            {
                for(i=1; i<=Cnt_Num; i++)
                {
                    if(TC04_ID_Num[i] != 0)
                    {
                        //���쳣������Ÿ��Ƹ�TC04_ID_Cnt����
                        TC04_ID_Cnt[ID_Cnt] = TC04_ID_Num[i];
                        Error_Flag = 1;//�쳣���ݱ�־λ��1
                        Sent_Cnt = 0;//�쳣���ݷ��ʹ�������
                        ID_Cnt++;//�ж���һ���쳣����
                    }
                }            
            }
            Error_Cnt = strlen(TC04_ID_Cnt);//������鲻Ϊ0������
        }
        if(Error_Cnt != 0)//�����쳣����
        {
            if(Sent_Cnt <= Error_Cnt)//�ж���ѯ���͵Ĵ����Ƿ�ﵽ
            {
                Sent_Flag = TC04_ID_Cnt[Sent_Cnt];//��������Ÿ�ֵ��Sent_Flag
                Sent_Cnt++;               
            }
            if(Sent_Cnt == Error_Cnt)//�쳣������ѯ����
            {
                memset(TC04_ID_Cnt, 0, 20); // �� TC04_ID_Cnt ��������
                Error_Cnt = 0;//�쳣����ͳ������
                Error_Flag = 0; //�쳣���ݱ�־λ����
            }           
            switch (Sent_Flag)//�ж����ĸ��������
            {
                case 0x01:              
                    TJA1042_CAN_Send_Data(Sent_Flag, TC04_One_Buff, Length_Val, Life_Signal_Cnt);//ͨ��TJA1042���ͱ��ĸ�����                             
                    break;
                case 0x02:
                    TJA1042_CAN_Send_Data(Sent_Flag, TC04_Two_Buff, Length_Val, Life_Signal_Cnt);//ͨ��TJA1042���ͱ��ĸ�����  
                    break;
                case 0x03:
                    TJA1042_CAN_Send_Data(Sent_Flag, TC04_Three_Buff, Length_Val, Life_Signal_Cnt); //ͨ��TJA1042���ͱ��ĸ����� 
                    break;
                case 0x04:
                    TJA1042_CAN_Send_Data(Sent_Flag, TC04_Four_Buff, Length_Val, Life_Signal_Cnt);//ͨ��TJA1042���ͱ��ĸ�����  
                    break;
                case 0x05:
                    TJA1042_CAN_Send_Data(Sent_Flag, TC04_Five_Buff, Length_Val, Life_Signal_Cnt);//ͨ��TJA1042���ͱ��ĸ�����  
                    break;   
                case 0x06:
                    TJA1042_CAN_Send_Data(Sent_Flag, TC04_Six_Buff, Length_Val, Life_Signal_Cnt);//ͨ��TJA1042���ͱ��ĸ�����  
                    break;                 
                case 0x07:
                    TJA1042_CAN_Send_Data(Sent_Flag, TC04_Seven_Buff, Length_Val, Life_Signal_Cnt); //ͨ��TJA1042���ͱ��ĸ����� 
                    break;
                case 0x08:
                    TJA1042_CAN_Send_Data(Sent_Flag, TC04_Eight_Buff, Length_Val, Life_Signal_Cnt);//ͨ��TJA1042���ͱ��ĸ�����  
                    break;
                case 0x09:
                    TJA1042_CAN_Send_Data(Sent_Flag, TC04_Nine_Buff, Length_Val, Life_Signal_Cnt);//ͨ��TJA1042���ͱ��ĸ�����  
                    break;   
                case 0x0A:
                    TJA1042_CAN_Send_Data(Sent_Flag, TC04_Ten_Buff, Length_Val, Life_Signal_Cnt);//ͨ��TJA1042���ͱ��ĸ�����  
                    break;
                case 0x0B:
                    TJA1042_CAN_Send_Data(Sent_Flag, TC04_Eleven_Buff, Length_Val, Life_Signal_Cnt);//ͨ��TJA1042���ͱ��ĸ�����  
                    break; 
                case 0x0C:
                    TJA1042_CAN_Send_Data(Sent_Flag, TC04_Twelve_Buff, Length_Val, Life_Signal_Cnt);//ͨ��TJA1042���ͱ��ĸ�����  
                    break;
                case 0x0D:
                    TJA1042_CAN_Send_Data(Sent_Flag, TC04_Thirteen_Buff, Length_Val, Life_Signal_Cnt);//ͨ��TJA1042���ͱ��ĸ�����  
                    break;
                case 0x0E:
                    TJA1042_CAN_Send_Data(Sent_Flag, TC04_Fourteen_Buff, Length_Val, Life_Signal_Cnt);//ͨ��TJA1042���ͱ��ĸ�����  
                    break;
                case 0x0F:
                    TJA1042_CAN_Send_Data(Sent_Flag, TC04_Fifteen_Buff, Length_Val, Life_Signal_Cnt);//ͨ��TJA1042���ͱ��ĸ�����  
                    break;
                case 0x10:
                    TJA1042_CAN_Send_Data(Sent_Flag, TC04_Sixteen_Buff, Length_Val, Life_Signal_Cnt);//ͨ��TJA1042���ͱ��ĸ�����  
                    break;                                                               
                default:
                    break;
            }        
        }
        else
        {
            /***********************************************************/
            //������ͨ����CAN���ĸ�ʽ��̽������������ͳһ��ʽ�ı���
            //20190711
            //ahren  
            /***********************************************************/  
            if ((Cnt_Num != 0) && (Cnt_Num != 0xFF))
            {
                TJA1042_CAN_Send_Data(0, CAN_Data_Normal, 8, Life_Signal_Cnt);//ͨ��TJA1042���ͱ��ĸ����� 
            }
        }
#endif
    }
    /****************************************/	
    //�����ͻ�Ӧ��                      
    //ahren
    //20191206
    /*************************************/	    
    MCP2515_User_Agreement(MCP2515_Sent_Time_Flag); 

    /*************************************/	    
}

//*******************************************/
//������:Get_CAN_Data
//��  ��:��ȡCAN��������
//��  ��:��
//����ֵ:��
//ʱ  ��:20180605
//������:ahren
/*******************************************/
void Get_CAN_Data(void)
{    
	if(1 == CAN_RXD_Interrupt_Flag_g)//CAN���������ж�
	{			
		CAN_RXD_Interrupt_Flag_g = 0;	

        /****************************************/	
        //�жϽ���̽���� CAN��������                         
        //ahren
        //20190604
        /*************************************/	        
        Get_TC04_CAN_Data();
	}     
}

//������:TC04_Num_Set
//��  ��:���õ������
//��  ��:��
//����ֵ:��
//ʱ  ��:20180611
//������:ahren
/*******************************************/
void TC04_Num_Set(void)
{
    u8 TC04_Num = 0;
    u8 TC04_Time_Val = 0;
 
    //ͨ��CAN�����豸����
    CAN_Receive_Interrupt_Data(CAN_Receive_Data);                   //��ȡCAN��������
    //�� CAN_Receive_Data ���Ƹ� CAN_TX0_Buff        
    memcpy(CAN_TX0_Buff, CAN_Receive_Data, 8);     
    
    if((CAN_Receive_Data[0] == 0x23) && (CAN_Receive_Data[7] == 0X16))  // 23 xx 00 00 00 00 00 16
    {
        if((CAN_Receive_Data[1] == 0x01))
        {
            /*********************************************/
            //���ͨ��CAN��ȡ�������������
            //20190727
            //ahren            
            if ((0x00 == CAN_Receive_Data[2]) && (0x01 == CAN_Receive_Data[3]))
            {
                CAN_TX0_Buff[2] = eeprom_read(0x03);//��EEPROM��ȡ���������
                NOP();NOP();NOP();NOP(); 
            }
            /*********************************************/            
            else
            {
                TC04_Num =  CAN_Receive_Data[2]; //��ȡ���õ�������� 

                eeprom_write(0x03,TC04_Num); //�����������д������EEPROM����
                NOP();NOP();NOP();NOP();
                
                eeprom_write(0x0A,0xA5);//�����Ĭ���������޸�
                NOP();NOP();NOP();NOP();
                
                CAN_TX0_Buff[3] = eeprom_read(0x03);//��EEPROM��ȡ���������
                NOP();NOP();NOP();NOP(); 
                
                CAN_TX0_Buff[2] = 0x00;                 
            } 
            CAN_TX0_Buff[0] = 0x23;
            CAN_TX0_Buff[7] = 0x16;

            CAN_Send_8Byte_Data();//ͨ��TJA1042 ����ȷ��
            delay_10ms(1);  
            CAN_STB_OFF;//���û��ȡ���������ϵ�        
        }
        else if((CAN_Receive_Data[1]==0x02))
        {
            TC04_Time_Val =  CAN_Receive_Data[2];                        

            eeprom_write(0x08,TC04_Time_Val);                           
            NOP();NOP();NOP();NOP();

            CAN_TX0_Buff[3] = eeprom_read(0x08);                      
            NOP();NOP();NOP();NOP();  
            
            CAN_TX0_Buff[0] = 0x23;
            CAN_TX0_Buff[2] = 0x00;
            CAN_TX0_Buff[7] = 0x16;

            CAN_Send_8Byte_Data();//ͨ��TJA1042 ����ȷ��
            delay_10ms(1);  
            CAN_STB_OFF;//���û��ȡ���������ϵ�              
        }       
        else if((CAN_Receive_Data[1]==0x03))
        {
            eeprom_write(0x04,CAN_Receive_Data[2]);                              
            NOP();NOP();NOP();NOP();
            eeprom_write(0x05,CAN_Receive_Data[3]);                         
            NOP();NOP();NOP();NOP();
            eeprom_write(0x06,CAN_Receive_Data[4]);                               
            NOP();NOP();NOP();NOP();
            eeprom_write(0x07,CAN_Receive_Data[5]);
            NOP();NOP();NOP();NOP();    
            eeprom_write(0x09,0x5A);//CANĬ��ID���޸ı�־λ
            NOP();NOP();NOP();NOP(); 
            
            CAN_TX0_Buff[2] = eeprom_read(0x04);                        
            NOP();NOP();NOP();NOP();
            CAN_TX0_Buff[3] = eeprom_read(0x05);                     
            NOP();NOP();NOP();NOP();  
            CAN_TX0_Buff[4] = eeprom_read(0x06);                        
            NOP();NOP();NOP();NOP();
            CAN_TX0_Buff[5] = eeprom_read(0x07);                        
            NOP();NOP();NOP();NOP();
            CAN_TX0_Buff[6] = eeprom_read(0x09);                        
            NOP();NOP();NOP();NOP(); 

            CAN_TX0_Buff[0] = 0x23;
            CAN_TX0_Buff[7] = 0x16;

            CAN_Send_8Byte_Data();//ͨ��TJA1042 ����ȷ��
            delay_10ms(1);  
            CAN_STB_OFF;//���û��ȡ���������ϵ�                            
        }      
    }
}
//*******************************************/
//������:CAN_Data_Analysis_Det
//��  ��:̽�����쳣���ݽ���
//��  ��:��
//����ֵ:��
//ʱ  ��:20190715
//������:ahren
/*******************************************/
void CAN_Data_Analysis_Det(u8 Num,u8 *Data_Buff)
{
    //̽����CAN���ĵ�1���ֽڻ��7���ֽڲ�Ϊ0
    if((0x00 != Data_Buff[1]) || (0x00 != Data_Buff[7]))
    {       
        if(Data_Buff[0] != 0)
        {
            TC04_ID_Num[Num] = Data_Buff[0];        
        }
    }
    else
    {
        TC04_ID_Num[Num] = 0;    
    }
}

//*******************************************/
//������:CAN_Data_Choice_Analysis
//��  ��:̽����can ���ݷ���
//��  ��:��
//����ֵ:��
//ʱ  ��:20190715
//������:ahren
/*******************************************/
void CAN_Data_Choice_Analysis(void)
{
    static u8 Cnt = 0;
    u8 Cnt_Num_Flag = 0;
    
    //��ȡ���������
    if ((Cnt_Num == 0) || (Cnt_Num == 0xFF))
    {
        Cnt_Num_Flag = eeprom_read(0x0A);//�ж�Ĭ�ϵ���������Ƿ��޸�
        NOP();NOP();NOP();NOP();    
        /**********************************/        
        //���Ĭ�ϵ������Ϊ6̨
        //20190724
        //ahren
        /**********************************/            
        if((Cnt_Num_Flag == 0) || (Cnt_Num_Flag == 0xFF))
        {
            eeprom_write(0x03,0x04);//�����������д������EEPROM����
            NOP();NOP();NOP();NOP();
            Cnt_Num = eeprom_read(0x03);//�������������EEPROM����
            NOP();NOP();NOP();NOP();        
        }
        else if(Cnt_Num_Flag == 0xA5)
        {
            Cnt_Num = eeprom_read(0x03);//�������������EEPROM����
            NOP();NOP();NOP();NOP();         
        }
    }  
    else if(Time_5ms_Flag_g == 1)//5ms��ʱ
    {
        Time_5ms_Flag_g = 0;
        Cnt++;//���ݸ��¼����Լ�
        //if(Cnt > 0x06)//���ݸ���һ�ֽ���
        if(Cnt > Cnt_Num)
        {
            Cnt = 1;//���ݴӵ�һ����ʼ
        }        
    }
    
    switch(Cnt)
    {
        case 0x01:
                CAN_Data_Analysis_Det(Cnt,TC04_One_Buff);//�жϵ�һ��̽���������Ƿ��쳣
            break;                  
        case 0x02:
                CAN_Data_Analysis_Det(Cnt,TC04_Two_Buff);//�жϵڶ���̽���������Ƿ��쳣
            break; 
        case 0x03:
                CAN_Data_Analysis_Det(Cnt,TC04_Three_Buff);//�жϵ�����̽���������Ƿ��쳣
            break;                  
        case 0x04:
                CAN_Data_Analysis_Det(Cnt,TC04_Four_Buff);//�жϵ��ĸ�̽���������Ƿ��쳣
            break;
        case 0x05:
                CAN_Data_Analysis_Det(Cnt,TC04_Five_Buff);//�жϵ����̽���������Ƿ��쳣
            break;                  
        case 0x06:
                CAN_Data_Analysis_Det(Cnt,TC04_Six_Buff);//�жϵ�����̽���������Ƿ��쳣
            break;
        case 0x07:
                CAN_Data_Analysis_Det(Cnt,TC04_Seven_Buff);//�жϵ��߸�̽���������Ƿ��쳣
            break;                  
        case 0x08:
                CAN_Data_Analysis_Det(Cnt,TC04_Eight_Buff);//�жϵڰ˸�̽���������Ƿ��쳣
            break;
        case 0x09:
                CAN_Data_Analysis_Det(Cnt,TC04_Nine_Buff);//�жϵھŸ�̽���������Ƿ��쳣
            break;                  
        case 0x0A:
                CAN_Data_Analysis_Det(Cnt,TC04_Ten_Buff);//�жϵ�ʮ��̽���������Ƿ��쳣
            break;
        case 0x0B:
                CAN_Data_Analysis_Det(Cnt,TC04_Eleven_Buff);//��ȡ10�ŵ���䱨������       
            break;     
        case 0x0C:
                CAN_Data_Analysis_Det(Cnt,TC04_Twelve_Buff);//��ȡ10�ŵ���䱨������       
            break;  
        case 0x0D:
                CAN_Data_Analysis_Det(Cnt,TC04_Thirteen_Buff);//��ȡ10�ŵ���䱨������       
            break;  
        case 0x0E:
                CAN_Data_Analysis_Det(Cnt,TC04_Fourteen_Buff);//��ȡ10�ŵ���䱨������       
            break; 
        case 0x0F:
                CAN_Data_Analysis_Det(Cnt,TC04_Fifteen_Buff);//��ȡ10�ŵ���䱨������       
            break;   
        case 0x10:
                CAN_Data_Analysis_Det(Cnt,TC04_Sixteen_Buff);//��ȡ10�ŵ���䱨������       
            break;             
        default:
            break;    
    }
}

//*******************************************/
//������:Equipment_Off_Line_Det
//��  ��:̽�������߼��
//��  ��:��
//����ֵ:��
//ʱ  ��:20190716
//������:ahren
/*******************************************/
void Equipment_Off_Line_Det(void)
{
    static u8 Cnt = 0;
    if(TC04_Off_Time_Cnt_g == 0x32)
    {
        TC04_Off_Time_Cnt_g = 0;
        if ((Cnt_Num != 0) && (Cnt_Num != 0xFF))
        {
            Cnt++;

            if(Cnt > Cnt_Num)
            {
                Cnt = 1;
            }     
        }      

        switch(Cnt)
        {
            case 0x01:
                    TC04_Off_Line_Det(Cnt, TC04_One_Buff, 6);//����Ƿ�����
                break;                  
            case 0x02:
                    TC04_Off_Line_Det(Cnt, TC04_Two_Buff, 6);//����Ƿ�����
                break; 
            case 0x03:
                    TC04_Off_Line_Det(Cnt, TC04_Three_Buff, 6);//����Ƿ�����
                break;                  
            case 0x04:
                    TC04_Off_Line_Det(Cnt, TC04_Four_Buff, 6);//����Ƿ�����
                break;
            case 0x05:
                    TC04_Off_Line_Det(Cnt, TC04_Five_Buff, 6);//����Ƿ�����
                break;                  
            case 0x06:
                    TC04_Off_Line_Det(Cnt, TC04_Six_Buff, 6);//����Ƿ�����
                break;
            case 0x07: 
                    TC04_Off_Line_Det(Cnt,TC04_Seven_Buff, 6);//��ȡ7�ŵ���䱨������      
                break;
            case 0x08:
                    TC04_Off_Line_Det(Cnt,TC04_Eight_Buff, 6); //��ȡ8�ŵ���䱨������        
                break; 
            case 0x09:
                    TC04_Off_Line_Det(Cnt,TC04_Nine_Buff, 6);//��ȡ9�ŵ���䱨������          
                break;
            case 0x0A:
                    TC04_Off_Line_Det(Cnt,TC04_Ten_Buff, 6);//��ȡ10�ŵ���䱨������          
                break;     
            case 0x0B:
                    TC04_Off_Line_Det(Cnt,TC04_Eleven_Buff, 6);//��ȡ11�ŵ���䱨������          
                break;
            case 0x0C:
                    TC04_Off_Line_Det(Cnt,TC04_Twelve_Buff, 6);//��ȡ12�ŵ���䱨������          
                break;    
            case 0x0D:
                    TC04_Off_Line_Det(Cnt,TC04_Thirteen_Buff, 6);//��ȡ13�ŵ���䱨������          
                break;
            case 0x0E:
                    TC04_Off_Line_Det(Cnt,TC04_Fourteen_Buff, 6);//��ȡ14�ŵ���䱨������          
                break;    
            case 0x0F:
                    TC04_Off_Line_Det(Cnt,TC04_Fifteen_Buff, 6);//��ȡ15�ŵ���䱨������          
                break;
            case 0x10:
                    TC04_Off_Line_Det(Cnt,TC04_Sixteen_Buff, 6);//��ȡ16�ŵ���䱨������          
                break;                                               
            default:
                break;    
        }
    }
}

//*******************************************/
//������:CAN_Sent_Time_Choice
//��  ��:����CAN ��������
//��  ��:��
//����ֵ:Ret_Val
//ʱ  ��:20190719
//������:ahren
/*******************************************/
u8 CAN_Sent_Time_Choice(void)
{
    static u8 Time_Val = 0;
    u8 Ret_Val  = 0;
    
    if(0x00 == Time_Val)
    {
        eeprom_write(0x08,0x03);                           
        NOP();NOP();NOP();NOP();

        Time_Val = eeprom_read(0x08);//��CAN�������ڴ�EEPROM�ж�ȡѡ��
        NOP();NOP();NOP();NOP();        
    }
    else if(0x01 == Time_Val)
    {
        if(CAN_Sent_Time_Cnt_g == 0x32)//250ms
        {
            CAN_Sent_Time_Cnt_g = 0;
            Ret_Val = 1;
        }
    }
    else if(0x02 == Time_Val)
    {
        if(CAN_Sent_Time_Cnt_g == 0x64)//500ms
        {
            CAN_Sent_Time_Cnt_g = 0;
            Ret_Val = 2;
        }    
    }
    else
    {
        if(CAN_Sent_Time_Cnt_g == 0xC8)//1000ms
        {
            CAN_Sent_Time_Cnt_g = 0;
            Ret_Val = 3;
        }     
    }    
    return Ret_Val;
}

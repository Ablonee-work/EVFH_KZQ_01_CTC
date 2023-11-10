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

/*************************************/

/*************************************/

/***************��־λ����************/
/**/

uint32_t MCP2515_TxID = 0x18FF301D;
uint16_t Time_Send_Cnt = 0;
u8 Get_Abnormal_Cnt = 0;
/*************************************/


/***************��������**************/
/**/

/*************************************/

/*************************************************************************/
//������:MCP2515_User_Agreement
//��  ��:�����û�Э������
//��  ��:��
//����ֵ����
//ʱ�䣺20191206
//�����ߣ�ahren
/************************************************************************/
u8 Version_Data_Normal[9] = {0x23,0x08,0x28,0x14,0x35,0xFF,0x00,0x0C};//��Ӳ���汾��Ϣ 1.1

void MCP2515_User_Agreement()
{    
    static u8 TC_Idx = 0;
    static u8 Life_Signal = 0;//10S����
    uint16_t Temp_SumData = 0x00;
    u8 MCP2515_Buff_F1[9] = {0};
    u8 MCP2515_Buff_E2[9] = {0};
    
    //MCP2515 �������ݽ����봦��
    if ((0x01 == MCP2515_Sent_Time_Flag))
    {
        MCP2515_Sent_Time_Flag = 0;   
        Time_Send_Cnt++;
        Life_Signal++;
        if(Life_Signal>255)     Life_Signal = 0;
        
        //ѡ���Ӧ�ĵ�����ţ������ݷ��͵������Ǳ�       
        MCP2515_Buff_F1[0] = 0x00;//Ԥ��
        MCP2515_Buff_F1[1] = 0x00;//���ϴ���+���ϵȼ�
        MCP2515_Buff_F1[2] = 0x00;//Ԥ��
        MCP2515_Buff_F1[3] = 0x00;//Ԥ��
        MCP2515_Buff_F1[4] = TC_Idx+1;//�������
        MCP2515_Buff_F1[5] = 0x00;//Ԥ������
        MCP2515_Buff_F1[6] = 0x00;//Ԥ��
        MCP2515_Buff_F1[7] = Life_Signal;//��Ϣ������
        
        
        //ѡ���Ӧ�ĵ�����ţ������ݷ��͵������Ǳ�       
        MCP2515_Buff_E2[0] = TC_Idx+1;//�������
        MCP2515_Buff_E2[1] = 0x00;//̽���������������
        MCP2515_Buff_E2[2] = 0x00;//���鼶��
        MCP2515_Buff_E2[3] = 0x00;//������
        MCP2515_Buff_E2[4] = 0x00;//�ӷ��򿪻�ر�����
        MCP2515_Buff_E2[5] = 0x00;//���ϵͳ������
        MCP2515_Buff_E2[6] = 0x00;//Ԥ��
        MCP2515_Buff_E2[7] = Life_Signal & 0x0F;//����֡   
        
        
        if(TC04_OffLine_Flag[TC_Idx]<3)//����
        {
            if(TC04_Buff[TC_Idx][1]==0x00)
            {
                MCP2515_Buff_F1[5] = 0x00;//Ԥ������
//                MCP2515_Buff_F1[4] = 0x00;
                MCP2515_Buff_F1[3] = 0x00;//���ϴ���+���ϵȼ�     
                
                MCP2515_Buff_E2[2] = 0x00;//���鼶��
                MCP2515_Buff_E2[3] = 0x00;//������
            }
            else if(TC04_Buff[TC_Idx][1]==0x02)
            {
                MCP2515_Buff_F1[5] = 0x04 << 5;
                MCP2515_Buff_F1[3] = 0x01;  
                
                MCP2515_Buff_E2[2] = 0x02 << 5;//���鼶��
                MCP2515_Buff_E2[3] = 0x00;//������     
            }
            else if(TC04_Buff[TC_Idx][1]==0x03)
            {
                MCP2515_Buff_F1[5] = 0x04 << 5;
                MCP2515_Buff_F1[3] = 0x02;      
                
                MCP2515_Buff_E2[2] = 0x03 << 5;//���鼶��
                MCP2515_Buff_E2[3] = 0x00;//������ 
            }
            else if(TC04_Buff[TC_Idx][1]==0x04)
            {
                MCP2515_Buff_F1[5] = 0x04 << 5;
                MCP2515_Buff_F1[3] = 0x03;        
                
                MCP2515_Buff_E2[2] = 0x04 << 5;//���鼶��
                MCP2515_Buff_E2[3] = 0x01 << 5;//������ //�������ʧЧ
                MCP2515_Buff_E2[1] = 0x03;//�����������
                MCP2515_Buff_E2[4] = 0x01 << 6;//�ӷ��򿪹ر�����
            }
            else if(TC04_Buff[TC_Idx][1]==0xFF)
            {
                if(TC04_Buff[TC_Idx][7]==0x88)
                {
                    MCP2515_Buff_F1[0] = 0x02;//Ԥ������
                    MCP2515_Buff_F1[5] = 0x01 << 5;//���ϴ���+���ϵȼ�
                    MCP2515_Buff_E2[3] = 0x02 << 5;//������ 
                }              
            }       
        }
        else //����
        {
            MCP2515_Buff_F1[0] = 0x01;//Ԥ������
            MCP2515_Buff_F1[5] = 0x01 << 5;//���ϴ���+���ϵȼ�
            MCP2515_Buff_E2[3] = 0x03 << 5;//������ 
            MCP2515_Buff_E2[5] = 0x03;
        }        
        
        Temp_SumData = MCP2515_Buff_E2[0] + MCP2515_Buff_E2[1] + MCP2515_Buff_E2[2] + MCP2515_Buff_E2[3] 
                     + MCP2515_Buff_E2[4] + MCP2515_Buff_E2[5] + MCP2515_Buff_E2[6] + MCP2515_Buff_E2[7];
        Temp_SumData = (Temp_SumData & 0x00FF) + 0x18 + 0xFE + 0x30 + 0x1D;
        
        MCP2515_Buff_E2[8] = ( (Temp_SumData>>4) + Temp_SumData) & 0x000F;        // �����
        MCP2515_Buff_E2[7] = (MCP2515_Buff_E2[7] & 0x0F) + (MCP2515_Buff_E2[8]<<4);
        
            
        
        MCP2515_Data_Send(0x18FE301D, MCP2515_Buff_E2, 8);  
        MCP2515_Data_Send(0x18FF311D, MCP2515_Buff_E2, 8);  
        delay_10ms(5);
        MCP2515_Data_Send(0x18FF401D, Version_Data_Normal, 8);
        delay_10ms(5);
#if 0          //ȡ��18FF301D
        if (Get_Abnormal_Cnt < 2)          
        {
            Time_Send_Cnt = 0;
            MCP2515_Data_Send(0x18FF301D, MCP2515_Buff_F1, 8);
        }
        else
        {
            if (Time_Send_Cnt == 2)
            {
                Time_Send_Cnt = 0;
                MCP2515_Data_Send(0x18FF301D, MCP2515_Buff_F1, 8);
            }
                
        }
#endif
        //������������ж�
        TC_Idx++;//������ż���   
        if(TC_Idx >= TC_Num)
        {
            TC_Idx = 0;  
        }       
    } 
}

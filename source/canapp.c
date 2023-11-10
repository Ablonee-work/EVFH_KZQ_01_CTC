
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

u8 TC04_Buff[TC_NUM_MAX][9] = {0x00};//�����CAN���� [x][y] x:��ţ�0-15��ʾ̽����1-16��
uint16_t TC04_Off_Time_Cnt_g = 0;//̽�������߼������ʱ��
u8 MCP2515_Sent_Time_Flag = 0;//���ⷢ�ͱ�������ʱ�����
u8 TJA1042_Sent_Time_Flag = 0;//���ڷ��ͱ�������ʱ�����

u8 TC_Num = 0;//̽��������
u8 TC04_OffLine_Flag[TC_NUM_MAX] = {0};//���߱�־λ  TC04_OffLine_Flag[x]:0-15��ʾ̽����1-16���   ��ֵ>3��ʾ����
u8 TC04_OffLine_LifeCnt_Temp[TC_NUM_MAX] = {0};//�������ݴ棬���ڼ������
u8 TC04_Abnormal_Table[TC_NUM_MAX]          = {0x00};//̽�����쳣��
u8 Send_TCNum_Cnt = 0;  //���ⷢ�ͱ��ĵ�ǰ�������
u8 Outgoing_Data_Cycle = 0; //���ⷢ�ͱ�������

/*************************************/
//20210222
u8 PowerOn_OfflineDet_Flag = 0;
u8 TC_Start_Flag[TC_NUM_MAX] = {0};  //00:����  01����C1����  02�յ�C1�ظ�    03����C3(����)   04�յ�C3�ظ������Ѿ�����   05�ַ��ر����  06������ȴ��ر�
uint16_t TC_C1_R_10Min = 0;//10���Ӽ�ʱ 
u8 TC_Start_Wait_Cnt[TC_NUM_MAX] = {0};
u8 Mamual_Start = 0;

u8 time_3s_cnt = 0;  
u8 TC_Waring_4_Flag[TC_NUM_MAX] = {0};
u8 TC_Sleep_Flag[TC_NUM_MAX] = {0};
u8 Sys_Sleep_Flag = 0;//0����״̬    1��˯��״̬   2������˯�ߺ󱻱��Ļ���  3���Ļ���״̬   4���½���˯��
u8 Sys_Sleep_Wake_TC = 0;//����������̽������־δ�����ͻ�������3��

//���ⱨ�Ĵ���      

                                      //x:0-19  :���1-20
u8 Sys_Sleep_Host_cnt = 0;
u8 TC_Code_Err = 0;//0������   1��δ�������  2���ָ�����
u8 TC_Code_Err_Cnt = 0;//δ������ϱ��ķ��ʹ���
u8 TC_Code_Recover_Flag = 0;
u8 TC_Code_Recover_Cnt = 0;
u8 TC_Quantity_Auto[TC_NUM_MAX] = {0};
u8 backupdata_read = 0;
u8 TC_RT_Data[TC_NUM_MAX][8] = {0};
u8 TC_RT_Data_Old[TC_NUM_MAX] = 0;//����Ԥ����Ϣ
u8 Host_C2_Data[8] = {0};
u8 Host_C2_Data_Old[8] = {0};

u8 B3_Data_Buffer[9] = {0};//��0�����ݱ�ʾ��ţ��Ҳ�Ϊ0����������

//20210902-̽�����������ݻ�����
uint32_t RxMsgBuff_ID[TC_DATA_BUFF]={0};
uint8_t RxMsgBuff_Len[TC_DATA_BUFF]={0};
uint8_t RxMsgBuff_Data[TC_DATA_BUFF][8]={0};
uint8_t RxMsgBuff_Flag[TC_DATA_BUFF]={0};

/***************��������**************/
/**/
void TC04_Num_Set(u8 *CAN_Receive_Data);
void Equipment_Off_Line_Det(void);

/*************************************/

void CanToCan_TC_B3(u8 num)
{
    u8 data[8] = {0};   
    uint32_t cmd_id = 0x18B300F6;    
    cmd_id = cmd_id|(num<<8);      
    data[0] = 0x0;
    data[1] = 0x0;
    data[2] = 0x0;
    data[3] = 0x0;
    data[4] = 0x0;
    data[5] = 0x0;
    data[6] = 0x0;
    data[7] = 0x0;
    CAN_Send_Data(cmd_id, 8, data);
}
void CanToCan_TC_F0(u8 state) //�͹���
{
    u8 data[8] = {0};   
    uint32_t cmd_id = 0x18F0FFF6;
    
    data[0] = state; //0x00 ����������(Normal)      0x01���͹���(Sleep)
    data[1] = 0x0;//����
    data[2] = 0x0;//����
    data[3] = 0x0;//����
    data[4] = 0x0;//����
    data[5] = 0x0;//����
    data[6] = 0x0;//����
    data[7] = 0x0;//����
    CAN_Send_Data(cmd_id, 8, data);
}

void CanToCan_Host_Communication(u8 state)
{
    u8 data[8] = {0};    
    uint32_t cmd_id = 0x18FFA5A5;
    data[0] = 0x23; 
    data[1] = state;
    data[2] = ~data[1];
    data[3] = 0;
    data[4] = 0;
    data[5] = 0;
    data[6] = 0;
    data[7] = 0x16;      
    CAN_Send_Data(cmd_id, 8, data);
}

void CanToCan_TC_C3(u8 num,u8 state)
{
    u8 data[8] = {0};   
    uint32_t cmd_id = 0x18C300F6;    
    cmd_id = cmd_id|(num<<8);      

    data[0] = state;
    data[1] = 0xFF-state;
    data[2] = 0x5A;
    data[3] = 0x5A;
    data[4] = 0x5A;
    data[5] = 0x5A;
    data[6] = 0x5A;
    data[7] = 0x5A;       
    CAN_Send_Data(cmd_id, 8, data);
}

void CanToCan_TC_C1(void)//�ֶ������������3�룬�յ�̽�����ظ���ֹͣ��������10����
{
    u8 data[8] = {0};   
    uint32_t cmd_id = 0x18C1FFF6;
    CAN_Send_Data(cmd_id, 8, data);
}

void CanToCan_BackupData_Replay(void)
{
    u8 data[8] = {0};   
    uint32_t cmd_id = 0x0402F5F6;
    data[0] = 0x23;
    data[1] = 0x04;
    data[2] = 0x02;
    data[3] = 0xFF;
    data[4] = 0x0;
    data[5] = 0x0;
    data[6] = 0x0;
    data[7] = 0x16;
    CAN_Send_Data(cmd_id, 8, data);
}

//20210902-����������������⣬�ֿ�̽�������ݽ��պʹ�������
void Get_TC04_CAN_Data(void)//����̽��������
{
    u8 i,j;
    j = 0;
    for(i=0;i<TC_DATA_BUFF;i++)
    {
        if(RxMsgBuff_Flag[i] == 0)
        {
            CAN_Rece_Data(&RxMsgBuff_ID[i], &RxMsgBuff_Len[i], &RxMsgBuff_Data[i][0]);
            RxMsgBuff_Flag[i] = 1;
            j = 1;
            break;
        }      
    }
    if(j == 0)
    {
        RXFUL = 0;//�򿪽��ջ���������������Ϣ
    }
}

void TC04_CAN_Data_Process(void)//̽�������ݴ���
{
    uint32_t rx_id=0;
    uint8_t rx_len=0;
    uint8_t rx_buf[8]={0};
    uint8_t buftemp[8]={0};
    u8 BOX_ID_Num = 0x00;//�������
    u8 i,temp,k;
    u8 waring = 0,fault = 0;

    //��ȡ���յ�̽��������
    for(k=0;k<TC_DATA_BUFF;k++)
    {
        if(RxMsgBuff_Flag[k] == 1)
        {
            rx_id = RxMsgBuff_ID[k];
            rx_len = RxMsgBuff_Len[k];           
            memcpy(rx_buf, &RxMsgBuff_Data[k][0], 8);           
            RxMsgBuff_Flag[k] = 0;
                       
            if((rx_id&0xFFFFFFF00) == 0x18B0F600)
            {
                BOX_ID_Num = rx_id&0xFF;//��ȡ�������

                if((BOX_ID_Num == 0xFF)||(BOX_ID_Num == 0x00)||(BOX_ID_Num > 16))
                {
                    if(TC_Code_Err != 1)
                    {
                        TC_Code_Err = 1;//δ����
                        TC_Code_Err_Cnt = 0;//���ʹ�������
                    } 
                    TC_Code_Recover_Flag = 1;

                    for(i=0; i<TC_Num; i++)
                    {
                        if(TC_Quantity_Auto[i] == 0)break;
                    }
                    if(i >= TC_Num)
                    {
                        if((TC_Num<=15)&&(TC_Num!=0))
                        {
                            TC_Num = TC_Num+1;
                            eeprom_write(0x03, TC_Num);//ȫ�����ߺ󣬼�⵽0xFF����������1  
                        }         
                    }         
                }
                else 
                {            
                    memcpy(&TC_RT_Data[BOX_ID_Num-1][0], rx_buf, 8);//�������ݻ�ȡ           

                    TC_Quantity_Auto[BOX_ID_Num-1] = 1;                  

                    //����ת������Ϊ̽��������ת��Ϊ��Ӧ����������������
                    waring = (rx_buf[7]>>4)&0x0F;              
                    fault = rx_buf[7] & 0x0F;
                  
                    if(Sys_Sleep_Flag == 1)//˯��״̬,Ԥ���Ż���
                    {
                        if(waring != 0)Sys_Sleep_Flag = 2;  
                    }

                    memcpy(buftemp, &TC04_Buff[BOX_ID_Num-1][0], 8); 
                    //�Ѿ�����
                    if(TC_Waring_4_Flag[BOX_ID_Num-1] == 0xAA)//������ƿ
                    {
                        buftemp[1] = 0x04;//������ƿ
                        buftemp[7] = 0x41;             
                    }
                    else if((fault == 0x02)||(fault == 0x03))
                    {
                        buftemp[1] = 0xFF;
                        buftemp[7] = 0x88;                
                    }
                    else 
                    {
                        switch(waring)
                        {
                            case 0x01:
                            case 0x02:
                                buftemp[1] = 0x02;
                                buftemp[7] = 0x44;                   
                                if(TC_Start_Flag[BOX_ID_Num-1] == 0x02)//�յ�C1�ظ�
                                {
                                    TC_Start_Flag[BOX_ID_Num-1] = 0x03;
                                }                    
                                break;
                            case 0x03:
                                buftemp[1] = 0x03;
                                buftemp[7] = 0x42;
                                if(TC_Start_Flag[BOX_ID_Num-1] == 0x02)//�յ�C1�ظ�
                                {
                                    TC_Start_Flag[BOX_ID_Num-1] = 0x03;
                                }                     
                                break;
                            case 0x04:
                                //ִ���Զ���������
                                TC_Start_Flag[BOX_ID_Num-1] = 0x03;//����C3����Զ��������̣�����̽�����ӷ�           
                                buftemp[1] = 0x03;
                                buftemp[7] = 0x42;                
                                break;
                            default:
                                if(fault == 0x01)//���ͷ��������̽�����Ѿ����������������
                                {
                                    buftemp[1] = 0xFF;
                                    buftemp[7] = 0x88;
                                }
                                else 
                                {
                                    buftemp[1] = 0x00;
                                    buftemp[7] = 0x00;  
                                }
                                break;
                        }
                    }

                    buftemp[0] = 0xFF;       
                    buftemp[2] = 0xFF;
                    buftemp[3] = 0xFF;
                    buftemp[4] = 0xFF;
                    buftemp[5] = 0xFF;           
                    buftemp[6]++;//������
                    memcpy(&TC04_Buff[BOX_ID_Num-1][0], buftemp, 8);            

                    //�Զ�ʶ������-��������ʱ
                    if(BOX_ID_Num > TC_Num)
                    {
                        TC_Num = BOX_ID_Num;     
                        eeprom_write(0x03, TC_Num);
                    }        
                    
                }
            }    
            else if((rx_id&0xFFFFFFF00) == 0x18B1F600)   
            {  
                BOX_ID_Num = rx_id&0xFF;//��ȡ�������
                if((BOX_ID_Num == 0xFF)||(BOX_ID_Num == 0x00)||(BOX_ID_Num > 16))
                {
                    if(TC_Code_Err!=1)
                    {
                        TC_Code_Err = 1;//δ����
                        TC_Code_Err_Cnt = 0;//���ʹ�������
                    }            
                    TC_Code_Recover_Flag = 1;

                    for(i=0; i<TC_Num; i++)
                    {
                        if(TC_Quantity_Auto[i] == 0)
                        {
                            break;
                        }
                    }
                    if(i >= TC_Num)
                    {
                        if(TC_Num<=15)
                        {
                            TC_Num = TC_Num+1;
                            eeprom_write(0x03,TC_Num);//ȫ�����ߺ󣬼�⵽0xFF����������1  
                        }                          
                    }
                }
                else 
                {
                    TC_Quantity_Auto[BOX_ID_Num-1] = 1;
                    TC04_Buff[BOX_ID_Num-1][6]++;//������
                }
            } 
            else if((rx_id&0xFFFFFFF00) == 0x18FF5E00)
            {       
                if((rx_buf[0]==0x24)&&(rx_buf[7]==0x42))
                {
                    if((rx_buf[2]==0xFE)&&(rx_buf[3]==0xEF))//����
                    {
                        //���������ֶ�������������                
                        if(Mamual_Start == 0)
                        {
                            for(i=0; i<TC_Num; i++)
                            {
                                if(TC_Start_Flag[i] == 0)
                                {
                                    TC_Start_Flag[i] = 0x01;//����C1����   
                                    Mamual_Start = 1;
                                    TC_C1_R_10Min = 0;
                                    time_3s_cnt = 3;//���̷���C1
                                }
                            }                    
                        }  
                    }
                }        
            }
            //����̽����C1�ظ�
            else if((rx_id&0xFFFFFFF00) == 0x18C1F600)
            {
                BOX_ID_Num = rx_id&0xFF;//��ȡ�������
                TC04_Buff[BOX_ID_Num-1][6]++;//������

                if(TC_Start_Flag[BOX_ID_Num-1] == 0x01)
                {
                    TC_Start_Flag[BOX_ID_Num-1] = 0x02;//�յ�C1�ظ�  
                } 
            }

            //����̽����C3�ظ�-�ӷ��򿪻ظ�
            else if((rx_id&0xFFFFFFF00) == 0x18C3F600)
            {
                BOX_ID_Num = rx_id&0xFF;//��ȡ�������
                TC04_Buff[BOX_ID_Num-1][6]++;//������

                if(TC_Start_Flag[BOX_ID_Num-1] == 0x03)
                {           
                    if((rx_buf[0] == 0x01)) //����״̬���߷���״̬����Ϊ�����ܷ�����Ч�ź�   ||(CAN_Receive_Data[1] == 0x01)
                    {           
                        TC_Start_Flag[BOX_ID_Num-1] = 0x06;//������ȴ��ر�
                        TC_Start_Wait_Cnt[BOX_ID_Num-1] = 0;

                        TC_Waring_4_Flag[BOX_ID_Num-1] = 0xAA;//������ƿ 
                    }  
                    else if(rx_buf[1] == 0x01)
                    {
                        TC_Waring_4_Flag[BOX_ID_Num-1] = 0xAA;//������ƿ    
                    }
                }  
                else if(TC_Start_Flag[BOX_ID_Num-1] == 0x04)
                {
                    if(rx_buf[0] == 0x00)
                    {
                        TC_Start_Flag[BOX_ID_Num-1] = 0x05;//�ر����
                    }          
                }
            }  
            //���յ�B3����ָ��󣬻ظ�����ָ��
            else if((rx_id&0xFFFFFFF00) == 0x18B3F600)
            {
                BOX_ID_Num = rx_id&0xFF;//��ȡ�������   
                if((BOX_ID_Num <= 16) && (BOX_ID_Num >= 1))
                {           
                    TC04_Buff[BOX_ID_Num-1][6]++;//������

                    CanToCan_TC_B3(BOX_ID_Num);      

                    if(B3_Data_Buffer[0] == 0)
                    {
                        memcpy(&B3_Data_Buffer[1], rx_buf, 8); 
                        B3_Data_Buffer[0] = BOX_ID_Num;
                    }
                }        
            } 
            //̽�����͹���F0�ظ�
            else if((rx_id&0xFFFFFFF00) == 0x18F0F600)
            { 
                BOX_ID_Num = rx_id&0xFF;//��ȡ������� 
                TC_Sleep_Flag[BOX_ID_Num-1] = 0;
            }     
            //Уʱ���ݱ��Ľ���-�������͵�C0����
            else if((rx_id&0xFFFFFFF00) == 0x18C0FFF6)
            {      
                RX8010_Time_Set(rx_buf);        
            }
            //����C2���ģ��洢����״̬
            else if((rx_id&0xFFFFFFF00) == 0x18C2FFF6)
            {
                memcpy(Host_C2_Data, rx_buf, 8);//��ȡCAN��������        
            }

        #if 0 //����ZQK̽�������ձ��Ĵ���
            //ֻ����CAN IDΪ 0x18FF01XX  ��̽�������� 
            if((rx_id&0xFFFFFFF00) == 0x18FF0100)
            {
                BOX_ID_Num = rx_id&0xFF;//��ȡ�������

                if((BOX_ID_Num==0)||(BOX_ID_Num>TC_NUM_MAX))return;//̽������ų�����Χ

                //�Զ�ʶ������-��������ʱ
                if(BOX_ID_Num > TC_Num)
                {
                    TC_Num = BOX_ID_Num;     
                    eeprom_write(0x03, TC_Num);
                }    
        #if 1
                /*************************************************************/
                //���δ��������ϼ��
                //20191101
                //ahren
                if((0x88 == rx_buf[7]))
                {
                    rx_buf[0] = 0x00;
                    rx_buf[1] = 0x00;
                    rx_buf[7] = 0x00;            
                }
        #endif            
                memcpy(&TC04_Buff[BOX_ID_Num-1][0], rx_buf, 8); 
            }
        #endif    
            else if(rx_id == 0x0402F5F6) //���õ��������ID��0x0402F5F6 
            {
                TC04_Num_Set(rx_buf); //��ȡ���������           
            }  
        }
    }
}

//*******************************************/
//������:Sent_TC04_CAN_Data
//��  ��:��ȡCAN��������
//��  ��:��
//����ֵ:��
//ʱ  ��:20190605
//������:ahren
/*******************************************/
u8 BackupData_Temp[16] = {0};
uint8_t cnt_30s = 0;
void Sent_TC04_CAN_Data(void)
{
    u8 i = 0;
    u8 j = 0;
    u8 abnormal_state = 0;
    u8 table_idx = 0;
    u8 send_data_buf[8] = {0};
    u8 temp;

    static u8 Life_Signal_Cnt = 0;   //������

     if((TC_Num >= 1)&&(TC_Num <= TC_NUM_MAX))//̽��������׼����
     {
        if(PowerOn_OfflineDet_Flag >= 20)
        {
            Equipment_Off_Line_Det();//̽�������߼��
        }
        
        //20210223-��ȡ��������
        if(backupdata_read != 0)
        {
            if((backupdata_read>0)&&(backupdata_read<=16))
            {
                CTC_BackupData_Buffer_Read(backupdata_read-1);
            }
            else 
            {
                CTC_BackupData_Buffer_Read(HOST_C2_DATA_ID);//C2����
            }  
            backupdata_read = 0;
        }       

        if(Sys_Sleep_Flag == 4)
        {       
            CanToCan_Host_Communication(CTC_ZHU_Sleep);
            Sys_Sleep_Host_cnt++;
            if(Sys_Sleep_Host_cnt>= 3)
            {
                Sys_Sleep_Flag = 0;
            }           
        }    
    
        //���ݴ洢-B3���ݴ洢
        if(B3_Data_Buffer[0] != 0)
        {
            temp = B3_Data_Buffer[0]-1;
            RX8010_Time_Get(BackupData_Temp);//��ȡʵʱʱ��
            BackupData_Temp[6] = 0x03;
            BackupData_Temp[7] = 0xB3;           
            memcpy(&BackupData_Temp[8], &B3_Data_Buffer[1], 8);          

            if(BackupData_State[temp] == 0)//ǰ10����
            {                        
                CTC_BackupData_B3_Write(temp, BackupData_Temp, 16);     
            }
            else if(BackupData_State[temp] == 1)//��10�����յ�B3
            {                
                CTC_BackupData_Last_Write(temp, BackupData_Temp, 16);
            }

            if(BackupData_State[HOST_C2_DATA_ID] == 0)//C2����
            {
                BackupData_Temp[6] = 0x03;
                BackupData_Temp[7] = 0xC2;

                BackupData_Temp[8] = 0x5A;
                BackupData_Temp[9] = 0x5A;
                BackupData_Temp[10] = 0x5A;
                BackupData_Temp[11] = 0x5A;
                BackupData_Temp[12] = 0x5A;
                BackupData_Temp[13] = 0x5A;
                BackupData_Temp[14] = 0x5A;
                BackupData_Temp[15] = 0x5A;                  
                CTC_BackupData_B3_Write(HOST_C2_DATA_ID, BackupData_Temp, 16);     
            }         
            B3_Data_Buffer[0] = 0;
        }

        //���������ͱ���
        if(0x01 == TJA1042_Sent_Time_Flag)//��׼ʱ��Time1  1s   
        {
            TJA1042_Sent_Time_Flag = 0;
            
            //20210223
            //�ӷ�����
             for(i=0; i<TC_Num; i++)//�ļ�Ԥ���Զ����������ȼ����
             {
                 if(TC_Start_Flag[i] == 0x03)
                 {
                     CanToCan_TC_C3(i+1, 0x01);//���ӷ���1������           
                 }
                 else if(TC_Start_Flag[i] == 0x04)//�յ�������ɣ����Թر�
                 {
                     CanToCan_TC_C3(i+1, 0x02);//�ر��ӷ���1������
                 }
             }  

            //����TC����
            if(Sys_Sleep_Wake_TC != 0)
            {
                CanToCan_TC_F0(0x00); //�͹���
                CanToCan_Host_Communication(CTC_ZHU_Wake);
                Sys_Sleep_Wake_TC++;
                if(Sys_Sleep_Wake_TC>=4)
                {
                    Sys_Sleep_Wake_TC = 0;
                }  
            }
            
            //��������ʱ����
           for(i=0; i<TC_Num; i++)
           {
               if(TC_Start_Flag[i] == 0x06)
               {
                   TC_Start_Wait_Cnt[i]++;
                   if(TC_Start_Wait_Cnt[i] >= 2)
                   {
                       TC_Start_Flag[i] = 0x04;//ȷ������,ִ�йر�
                   }
               }
           }

            //̽����δ����״̬
            if(TC_Code_Err == 1)//δ����
            {                     
                TC_Code_Recover_Cnt++;            
                if(TC_Code_Recover_Cnt > 5)
                {
                    TC_Code_Recover_Cnt = 0;
                    if(TC_Code_Recover_Flag == 0)
                    {
                        TC_Code_Err = 2;//�ָ�����
                        TC_Code_Err_Cnt = 0;
                    }
                    else
                    {
                        TC_Code_Recover_Flag = 0;
                    }
                }          
            } 
        
            if(TC_Code_Err == 1)
            {           
                if(TC_Code_Err_Cnt<3)
                {
                    CanToCan_Host_Communication(CTC_ZHU_CodeFault);//����
                }            
                TC_Code_Err_Cnt++;
            }
            else if(TC_Code_Err == 2)
            {
                CanToCan_Host_Communication(CTC_ZHU_CodeNormal);//�ָ�����
                TC_Code_Err_Cnt++;
                if(TC_Code_Err_Cnt>3)
                {
                    TC_Code_Err = 0;
                    TC_Code_Err_Cnt = 0;
                }
            }
        
            //20210324-��������     
            cnt_30s++;
            if(cnt_30s >= 30)cnt_30s = 0;

            RX8010_Time_Get(BackupData_Temp);//��ȡʵʱʱ��        
            for(i=0; i<TC_Num; i++)
            {
                if(BackupData_State[i] == 0)//ǰ10����
                {
                    if(TC_RT_Data[i][7] != TC_RT_Data_Old[i])//״̬�ı�
                    {
                        TC_RT_Data_Old[i] = TC_RT_Data[i][7];                   
                        BackupData_Temp[6] = 0x01;
                        BackupData_Temp[7] = 0xB0;

                        BackupData_Temp[8] = TC_RT_Data[i][0];
                        BackupData_Temp[9] = TC_RT_Data[i][1];
                        BackupData_Temp[10] = TC_RT_Data[i][2];
                        BackupData_Temp[11] = TC_RT_Data[i][3];
                        BackupData_Temp[12] = TC_RT_Data[i][4];
                        BackupData_Temp[13] = TC_RT_Data[i][5];
                        BackupData_Temp[14] = TC_RT_Data[i][6];
                        BackupData_Temp[15] = TC_RT_Data[i][7];
                        CTC_BackupData_Before_Write(i, BackupData_Temp, 16);                    
                    }
                    else//����30s��¼һ��
                    {
                        if(cnt_30s == 1)
                        {
                            BackupData_Temp[6] = 0x01;
                            BackupData_Temp[7] = 0xB0;                         

                            BackupData_Temp[8] = TC_RT_Data[i][0];
                            BackupData_Temp[9] = TC_RT_Data[i][1];
                            BackupData_Temp[10] = TC_RT_Data[i][2];
                            BackupData_Temp[11] = TC_RT_Data[i][3];
                            BackupData_Temp[12] = TC_RT_Data[i][4];
                            BackupData_Temp[13] = TC_RT_Data[i][5];
                            BackupData_Temp[14] = TC_RT_Data[i][6];
                            BackupData_Temp[15] = TC_RT_Data[i][7];
                            CTC_BackupData_Before_Write(i, BackupData_Temp, 16);                                                                    
                        }
                    }              
                }
                else if(BackupData_State[i] == 1)//��10����
                {
                    BackupData_Temp[6] = 0x02; 
                    BackupData_Temp[7] = 0xB0;

                    BackupData_Temp[8] = TC_RT_Data[i][0];
                    BackupData_Temp[9] = TC_RT_Data[i][1];
                    BackupData_Temp[10] = TC_RT_Data[i][2];
                    BackupData_Temp[11] = TC_RT_Data[i][3];
                    BackupData_Temp[12] = TC_RT_Data[i][4];
                    BackupData_Temp[13] = TC_RT_Data[i][5];
                    BackupData_Temp[14] = TC_RT_Data[i][6];
                    BackupData_Temp[15] = TC_RT_Data[i][7];                

                    CTC_BackupData_Last_Write(i, BackupData_Temp, 16);                    
                }               
            } 
            //�洢C2����-Host_C2_Data
            if(BackupData_State[HOST_C2_DATA_ID] == 0)//ǰ
            {
                if(memcmp(Host_C2_Data, Host_C2_Data_Old, 8)!=0)
                {
                    memcpy(Host_C2_Data_Old, Host_C2_Data,8);               
                    BackupData_Temp[6] = 0x01;
                    BackupData_Temp[7] = 0xC2;
                    memcpy(&BackupData_Temp[8], &Host_C2_Data[0], 8);
                    CTC_BackupData_Before_Write(HOST_C2_DATA_ID, BackupData_Temp, 16);
                }
                else
                {
                    if(cnt_30s == 1)
                    { 
                        BackupData_Temp[6] = 0x01;
                        BackupData_Temp[7] = 0xC2;
                        memcpy(&BackupData_Temp[8], &Host_C2_Data[0], 8);
                        CTC_BackupData_Before_Write(HOST_C2_DATA_ID, BackupData_Temp, 16);                    
                    }
                }
            }     
            else if(BackupData_State[HOST_C2_DATA_ID] == 1)//��
            {
                BackupData_Temp[6] = 0x02; 
                BackupData_Temp[7] = 0xC2;                                  
                memcpy(&BackupData_Temp[8], &Host_C2_Data[0], 8);          
                CTC_BackupData_Last_Write(HOST_C2_DATA_ID, BackupData_Temp, 16); 
            }

            if(BackupData_PowerON_Clear == 1)//����������ݵĹ���
            {  
                CanToCan_Host_Communication(CTC_ZHU_BackupDataFault);         
            }
            else if(BackupData_PowerON_Clear >= 2)//�������������
            {
                BackupData_PowerON_Clear++;
                CanToCan_Host_Communication(CTC_ZHU_BackupDataClear);  
                if(BackupData_PowerON_Clear>=7)//�Ѿ�������ķ���5��
                {
                    BackupData_PowerON_Clear = 0;
                }
            }       
            //20210223  
        
            //20210223
            if(Mamual_Start == 1)
            {
                time_3s_cnt++;
                if(time_3s_cnt > 3)//3�붨ʱ
                {
                    time_3s_cnt = 0;
                    for(i=0; i<TC_Num; i++)
                    {
                        if(TC_Start_Flag[i] == 0x01)
                        {
                            CanToCan_TC_C1();//ǿ������ ����3��
                            break;
                        }
                    }
                    
                    if(TC_C1_R_10Min > 600)//10���Ӻ�������з���C1
                    {
                        TC_C1_R_10Min = 0;
                        Mamual_Start = 0;//10���Ӻ󣬽���ֶ�����
                        for(j=0; j<TC_Num; j++)
                        {
                            if((TC_Start_Flag[j] == 0x01)||(TC_Start_Flag[j] == 0x02))//10���ӵ���������з�C1���յ�C1�ظ�
                            {
                                TC_Start_Flag[j] = 0x00;                           
                            }                            
                        }
                    }                
                }             
            }
            
            //20210308-BUG:�õȵ�����̽����ȫ�����������ܽ���˯��                   
            if(Sys_Sleep_Flag == 3)//˯�ߺ󣬱��Ļ���״̬
            {
                temp = 0;
                for(i=0; i<TC_Num; i++)
                {
                    if(TC04_OffLine_Flag[i]<=3)//����
                    {
                        if((TC04_Buff[i][1] != 0)&&(TC04_Buff[i][1] != 0xFF))//����Ԥ��
                        {
                            temp = 1;
                            break;
                        }                                
                    }           
                }
                if(temp == 0)
                {
                    Sys_Sleep_Flag = 4;//���½���˯��
                    Sys_Sleep_Host_cnt = 0;                
                } 
            }            
                        
            //���ķ���
            memset(TC04_Abnormal_Table,0 ,TC_NUM_MAX);
            abnormal_state = 0;
            for(i=0; i<TC_Num; i++)
            {
                if((0x00 != TC04_Buff[i][1]) || (0x00 != TC04_Buff[i][7])||(TC04_OffLine_Flag[i]>3))//�쳣
                {
                    TC04_Abnormal_Table[abnormal_state++] = i;  
                }
            }    
            Get_Abnormal_Cnt = abnormal_state;
            if(abnormal_state != 0)
            {
                if(Send_TCNum_Cnt >= abnormal_state)
                {
                    Send_TCNum_Cnt = 0;
                }
                
                table_idx = TC04_Abnormal_Table[Send_TCNum_Cnt];
                Send_TCNum_Cnt++;
                
                if(TC04_OffLine_Flag[table_idx] > 3)//����
                {
                    send_data_buf[0] = table_idx+1;
                    send_data_buf[1] = 0xFF;
                    send_data_buf[2] = 0xFF;
                    send_data_buf[3] = 0xFF;
                    send_data_buf[4] = 0xFF;
                    send_data_buf[5] = 0xFF;
                    send_data_buf[6] = Life_Signal_Cnt++;
                    send_data_buf[7] = 0x90;                                          
                }
                else//�쳣
                {
                    send_data_buf[0] = table_idx+1;
                    send_data_buf[1] = TC04_Buff[table_idx][1];
                    send_data_buf[2] = TC04_Buff[table_idx][2];
                    send_data_buf[3] = TC04_Buff[table_idx][3];
                    send_data_buf[4] = TC04_Buff[table_idx][4];
                    send_data_buf[5] = TC04_Buff[table_idx][5];
                    send_data_buf[6] = Life_Signal_Cnt++;
                    send_data_buf[7] = TC04_Buff[table_idx][7];                       
                }
            }
            else //ȫ������
            {
                send_data_buf[0] = 0;
                send_data_buf[1] = 0;
                send_data_buf[2] = 0xFF;
                send_data_buf[3] = 0xFF;
                send_data_buf[4] = 0xFF;
                send_data_buf[5] = 0xFF;
                send_data_buf[6] = Life_Signal_Cnt++;
                send_data_buf[7] = 0;   
            } 
            
            CAN_Send_Data(0x18FF741D, 8, send_data_buf);//ͨ��TJA1042���ͱ��ĸ�����                
        }      
     }
     else //̽������������
     {
        if(0x01 == TJA1042_Sent_Time_Flag)         
        {
            TJA1042_Sent_Time_Flag = 0;  
            
            send_data_buf[0] = 0xFF;
            send_data_buf[1] = 0xFF;
            send_data_buf[2] = 0xFF;
            send_data_buf[3] = 0xFF;
            send_data_buf[4] = 0xFF;
            send_data_buf[5] = 0xFF;
            send_data_buf[6] = Life_Signal_Cnt++;
            send_data_buf[7] = 0x91;               
            CAN_Send_Data(0x18FF741D, 8, send_data_buf);//ͨ��TJA1042���ͱ��ĸ����� 
            
            if(TC_Code_Err == 1)//δ����
            {                     
                TC_Code_Recover_Cnt++;            
                if(TC_Code_Recover_Cnt > 5)
                {
                    TC_Code_Recover_Cnt = 0;
                    if(TC_Code_Recover_Flag == 0)
                    {
                        TC_Code_Err = 2;//�ָ�����
                        TC_Code_Err_Cnt = 0;
                    }
                    else
                    {
                        TC_Code_Recover_Flag = 0;
                    }
                }          
            }      
            if(TC_Code_Err == 1)
            {           
                if(TC_Code_Err_Cnt<3)
                {
                    CanToCan_Host_Communication(CTC_ZHU_CodeFault);//����
                }            
                TC_Code_Err_Cnt++;
            }
            else if(TC_Code_Err == 2)
            {
                CanToCan_Host_Communication(CTC_ZHU_CodeNormal);//�ָ�����
                TC_Code_Err_Cnt++;
                if(TC_Code_Err_Cnt>3)
                {
                    TC_Code_Err = 0;
                    TC_Code_Err_Cnt = 0;
                }
            }
        }   
     }
}


//������:TC04_Num_Set
//��  ��:���õ������
//��  ��:��
//����ֵ:��
//ʱ  ��:20180611
//������:ahren
/*******************************************/
void TC04_Num_Set(u8 *CAN_Receive_Data)
{
    u8 TC04_Num = 0;
    u8 CAN_TX0_Buff[8] = {0};
    u8 i;
    
    //ͨ��CAN�����豸����
    //�� CAN_Receive_Data ���Ƹ� CAN_TX0_Buff        
    memcpy(CAN_TX0_Buff, CAN_Receive_Data, 8);     
    
    if((CAN_Receive_Data[0] == 0x23) && (CAN_Receive_Data[7] == 0x16))  // 23 xx 00 00 00 00 00 16
    {
        if((CAN_Receive_Data[1] == 0x01))//���������Ͷ�ȡ����
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
                
                TC_Num = CAN_TX0_Buff[3];
            } 
            CAN_TX0_Buff[0] = 0x23;
            CAN_TX0_Buff[7] = 0x16;

            CAN_Send_Data(0x0402F5F6, 8, CAN_TX0_Buff);           
            delay_10ms(1);                      
            CAN_STB_OFF();//���û��ȡ���������ϵ� 
        }
        else if((CAN_Receive_Data[1] == 0x02))//���ö��ⷢ������
        {                     
            eeprom_write(0x08, CAN_Receive_Data[2]);                           
            NOP();NOP();NOP();NOP();

            CAN_TX0_Buff[3] = eeprom_read(0x08);                      
            NOP();NOP();NOP();NOP();  
            
            CAN_TX0_Buff[0] = 0x23;
            CAN_TX0_Buff[2] = 0x00;
            CAN_TX0_Buff[7] = 0x16;

            CAN_Send_Data(0x0402F5F6, 8, CAN_TX0_Buff);//ͨ��TJA1042 ����ȷ��
            delay_10ms(1);  
            CAN_STB_OFF();//���û��ȡ���������ϵ�              
        }       
        else if((CAN_Receive_Data[1]==0x03))//���ö����Ǳ�ID
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

            CAN_Send_Data(0x0402F5F6, 8, CAN_TX0_Buff);//ͨ��TJA1042 ����ȷ��
            delay_10ms(1);  
            CAN_STB_OFF();//���û��ȡ���������ϵ�                            
        }   
        //20210405-�������ݶ�ȡ�����ݼ�¼���
        else if(CAN_Receive_Data[1] == 0x04)
        {
            if(CAN_Receive_Data[2] == 0x01)//��ȡ��������
            {
                if(backupdata_read == 0)
                {
                    backupdata_read = CAN_Receive_Data[3];
                }                        
            }
            else if(CAN_Receive_Data[2] == 0x02)//�����������
            {
                for(i=0; i<TC_BACKUP_COUNT; i++)
                {
                   W25QXX_Erase_Sector(TC_X_Base[i]+TC_B3_DATA_OFFSET, 0);//����
                   W25QXX_Erase_Sector(TC_X_Base[i]+TC_BEFORE_DATA_OFFSET, 0);//����
                   BackupData_PowerON_Clear = 2;//���Դ洢
                   BackupData_State[i] = 0;//����ǰ10���Ӵ洢����
                   BackupData_Cnt[i] = 0;    //��ʼ10�������ݴ洢    
                   BackupData_Flag[i] = 0;
                   eeprom_write(0x20+2*i, 0);
                   eeprom_write(0x20+2*i+1, 0);                           
                }
                CanToCan_BackupData_Replay();
            }
        }
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
     u8 TC_Idx = 0;
      
    if(TC04_Off_Time_Cnt_g >= 350)//200*5ms = 1000ms ���һ������
    {
        TC04_Off_Time_Cnt_g = 0;
                 
        for(TC_Idx=0; TC_Idx<TC_Num; TC_Idx++)
        {
            if(TC04_Buff[TC_Idx][6] != TC04_OffLine_LifeCnt_Temp[TC_Idx])
            {
                TC04_OffLine_LifeCnt_Temp[TC_Idx] = TC04_Buff[TC_Idx][6];
                TC04_OffLine_Flag[TC_Idx] = 0;               
            }
            else 
            {
                if(TC04_OffLine_Flag[TC_Idx] <= 3)//����3��ʾ����
                {
                    TC04_OffLine_Flag[TC_Idx]++;                    
                    if(TC04_OffLine_Flag[TC_Idx]>3)
                    {
                        memset(&TC_RT_Data[TC_Idx][0], 0xCC, 8);
                    }                    
                }
            }
        }
    }
}


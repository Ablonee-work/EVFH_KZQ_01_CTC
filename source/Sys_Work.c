/*----------------------------------------------------------------------*/
/* Confidential and Proprietary Information of MicroPerfect Corporation */
/* (C) 2018 ,MicroPerfect Corporation . All rights reserved. */
/* FileName : sys_work.c */
/* Function : */
/* Ver: 0.01  */
/* MP_Ahren NewFile */
/* History:YYYY.MM.DD Version author Description 
          :2018.05.24 0.01 @Ahren New file  */
/*-------------------------------------------------------------*/

/***************ͷ�ļ�****************/
/**/
/*************************************/
#include "config.h"
//******************�궨��*************/
/**/
/*************************************/

/*************************************/

/***************��־λ����************/
/**/
u8 Sys_Time_Count_g      = 0x00;                //ϵͳʱ�Ӽ�����־λ
u8 Sys_Onesecond_Count_g = 0x00;               //ϵͳ��ʱ1���Ӽ�����־λ

u8 Return_Time_Data = 0x00;                   //ʱ���������ֵ 

/*************************************/

/***************��������**************/
/**/
void Sys_Work_State_Display(void);
void Sys_Mode_State(void);
/*************************************/


//*******************************************/
//������:Sys_Time_Count
//��  ��:ϵͳʱ�Ӽ�������
//��  ��:��
//����ֵ:Value
//ʱ  ��:20180408
//������:ahren
/*******************************************/
u8 Sys_Time_Count()
{   
    if(0x01 == Sys_Timer_Flag_g)           
    {
        Sys_Timer_Flag_g = 0x00;
        
        Sys_Time_Count_g++;           //ϵͳʱ�Ӽ�����־λ�Լ�1
        
        Sys_Onesecond_Count_g++;     //ϵͳ��ʱ1�������־λ�Լ�1
        
#if 0
        if(0x1E == Sys_Onesecond_Count_g)
        {
            //NB_Mode_Reset_Flag_g = 1;
            
            //RG4 = 0;
        }
#endif       
        Fire_Start_Test_Flag_g = 1;             //���ͷ������ʱʱ���־λ
        
        Fire_Open_Test_Time_Flag_g = 1;         //���ͷ��·���ʱ���־λ
        
        //Fire_Start_Test_Conut_g++;            //ʱ�������־λ
        
        Smoke_Time_Flag_g = 1;               //��������ʱ��־λ��1
        Battery_Voltage_Time_Flag_g = 1;    //��ؼ����ʱ��־λ��1
        
        if(RX8010SJ_Wake_UP_Flag_g)
        {        
            RX8010SJ_Wake_UP_Count_g++;   //�ⲿRTC���Ѻ���ʱ1S��������ѹ���
        }
    } 
    /*******************************************/
    if(0xC8 == Sys_Onesecond_Count_g)
    {        
        Sys_Onesecond_Count_g = 0x00;
        Sys_Onesecond_Flag_g  = 0x01;   //1���־λ��1
        
        MCU_Send_Data_Time_Flag_g = 1;
        
        MCU_Send_Data_Time_Count_g++;
        
        if(0x03 == MCU_Send_Data_Time_Count_g)
        {
            NB_Mode_Reset_Flag_g = 1;       //NB ģ�黽����ȴ�3��
        }                     
        //ϵͳ�ϵ�10S��ʼ��ʼ��NBģ�飬10S��NBģ����ź�ֵ�űȽ��ȶ�
        if((!MCU_Send_Data_Stop_Count_g) && (MCU_Send_Data_Time_Count_g == 0x0A))
        {
            //MCU_Send_Data_Time_Count_g = 0;
            NB_Init_End_Flag_g = 1;
            MCU_Send_Data_Stop_Count_g = 1;
        }
        
        if(0x0A == MCU_Send_Data_Time_Count_g)
        {
            NB_Dealy_10S_Time_Flag_g = 1;//10s��һ�����ݱ�־λ
            MCU_Send_Data_Time_Count_g = 0;//MCU_Send_Data_Time_Count_g�Լӵ�10������
        } 
        /****************************************/	
        //LED_Sys���ΪLED_Green                          
        //ahren
        //20180518
        /*************************************/	        
        if(!Sys_Fault_Flag_g)
        {
            //LED_Sys =! LED_Sys; 
            
            LED_Green =! LED_Green;  
        }
        else
        {
            LED_Green = 1;
        }
 
#if 0
        Read_RTC_Data(0x16,1);   
        TXREG1 = Read_Data_Buff[0];                                
        Read_RTC_Data(0x15,1);   
        TXREG1 = Read_Data_Buff[0];   
        Read_RTC_Data(0x14,1);   
        TXREG1 = Read_Data_Buff[0];   
        Read_RTC_Data(0x13,1);   
        TXREG1 = Read_Data_Buff[0];                                
        Read_RTC_Data(0x12,1);   
        TXREG1 = Read_Data_Buff[0];   
        Read_RTC_Data(0x11,1);   
        TXREG1 = Read_Data_Buff[0];   
        Read_RTC_Data(0x10,1);   
        TXREG1 = Read_Data_Buff[0];         
#endif
        //return Sys_Onesecond_Flag_g;
    }
    
    if(0xC8 == RX8010SJ_Wake_UP_Count_g)
    {
        RX8010SJ_Wake_UP_Count_g = 0;
            
        RX8010SJ_Wake_UP_Flag_g  = 0;

        RTC_Tran_Write_Data(0x1E,0x20); 
    }
    /*******************************************/   
    
    if(0x01 == Sys_Time_Count_g)
    {
        Return_Time_Data = 0x01;
        
        return Return_Time_Data;
    }
    else if(0x02 == Sys_Time_Count_g)
    {       
        Return_Time_Data = 0x02;
        
        return Return_Time_Data;
    }
    else if(0x03 == Sys_Time_Count_g)
    {      
        Return_Time_Data = 0x03;
        
        return Return_Time_Data;
    }
    else if(0x04 == Sys_Time_Count_g)
    {      
        Return_Time_Data = 0x04;
        
        return Return_Time_Data;
    }
    else if(0x05 == Sys_Time_Count_g)
    {        
        Return_Time_Data = 0x05;
        
        return Return_Time_Data;
    }
    else if(0x06 == Sys_Time_Count_g)
    {        
        Return_Time_Data = 0x06;
        
        return Return_Time_Data;   
    }
    else 
    {
        Return_Time_Data = 0x00;
        
        return Return_Time_Data; 
    }
}
//*******************************************/
//������:Sys_Function_Work
//��  ��:ϵͳ�����������
//��  ��:��
//����ֵ:Value
//ʱ  ��:20180408
//������:ahren
/*******************************************/
void Sys_Function_Work(void)
{
    u8 Sys_Function_Work_Flag = 0x00;
    
    Sys_Function_Work_Flag = Sys_Time_Count();
    
    Return_Time_Data = 0x00;
    
    switch(Sys_Function_Work_Flag)
    {
        case 1:
                Fire_Open_Test_Result_g = Fire_Open_Det();    //���ͷ��·���
                
                break;
        case 2:
                Smoke_Det_Result_g    = Smoke_Det_Function(); //����Ԥ�����
                
                break;
        case 3:          
                Battery_Voltage_Det_Result_g = Battery_Voltage_Det();  //��ص�ѹ���           
                                
                break;
        case 4:
                Fire_Start_Signal();    //���ͷ�����źż��
                Fire_Start_Key_Result_g = Fire_Start_Key();   //�ֱ��������
                Fire_Back_Pressure_Result_g = Fire_Back_Pressure();//�����ź���©ѹ���
                
                break;
        case 5:
                NTC_MF58_Det_Result_g = NTC_MF58_Vol_Det();   //�¶�Ԥ�����
                
                break;
        case 6:
                Sys_Time_Count_g = 0x00; 
                Sys_Mode_State();   //ϵͳ����ģ��״̬���
                Sys_Work_State_Display();   //��ϵͳ��ģ��״̬����ͨ��NB����   
                
                break;                
        default:
            
               break;
    }
}

//*******************************************/
//������:Sys_Function_Work
//��  ��:��ȡϵͳ��������״̬
//��  ��:��
//����ֵ:Value
//ʱ  ��:20180410
//������:ahren
/*******************************************/
void Sys_Work_State_Display(void)
{
    if((!Fire_Start_Signal_Flag_g) && (!Fire_Start_Key_Result_g))//���ͷδ����
    {
        Sys_Wake_Up_Flag_g = 1; //���ѱ�־λ��1
        
        //�ж�ϵͳ����ģ���Ƿ���Ԥ�������
        if((Fire_Open_Test_Result_g) || (Smoke_Det_Result_g) || 
           (Battery_Voltage_Det_Result_g) || (NTC_MF58_Det_Result_g) ||
           (0x02 == Fire_Back_Pressure_Result_g))                       
        {       
            Sys_Fault_Flag_g = 1;
            
            /****************************************/	
            //��LED_Green_Function()�����޸�ΪLED_Red_Function()����                           
            //ahren
           //20180518
           /*************************************/	
            //LED_Green_Function(); 
            LED_Red_Function();
           /*************************************/	
            
            //if((Sys_Wake_Up_Flag_g) && (Sleep_Start_Flag_g) && (NB_Mode_Reset_Flag_g))
            if((Sys_Wake_Up_Flag_g) && (Sleep_Start_Flag_g) && (NB_Dealy_10S_Time_Flag_g))//10s��һ������
            {                             
                NB_Dealy_10S_Time_Flag_g = 0;
                
                NB_Protocol_Data_Format_Upload(Smoke_Det_Result_g,NTC_MF58_Det_Result_g,
                                               Fire_Start_Key_Result_g,Floor_Alarm_Staet_g,
                                               Fire_State_Flag_g);  
            }  
        }
        else
        {
            /****************************************/	
            //��LED_Green_Function()�����޸�ΪLED_Red_Function()����                           
            //ahren
           //20180518
           /*************************************/	
            //LED_Green = 1; 
            LED_Red = 1; 
            Sys_Fault_Flag_g = 0;
            NB_Dealy_10S_Time_Flag_g = 0;
           /*************************************/	
            if((Sys_Wake_Up_Flag_g) && (Sleep_Start_Flag_g) && (NB_Mode_Reset_Flag_g))//����3S����һ������
            {                
                NB_Protocol_Data_Format_Upload(Smoke_State_Flag_g,NTC_MF58_Det_Result_g,
                                               Fire_Start_Key_Result_g,Floor_Alarm_Staet_g,
                                               Fire_State_Flag_g);                                  
            }  
        }  
        //����Sleepģʽǰ���ж������Ƿ���Ԥ��
        if((!Smoke_Det_Result_g) && (Sys_Wake_Up_Flag_g) && (Sleep_Start_Flag_g) && (NB_Mode_Reset_Flag_g))
        {
            NB_Mode_Reset_Flag_g = 0;
            //Sys_Fault_Flag_g = 0;
            Sleep_Function();        
        }
    }
    else
    {
        if(NB_Dealy_10S_Time_Flag_g)//�������飬10S����һ������
        {                             
            NB_Dealy_10S_Time_Flag_g = 0;
                  
            NB_Protocol_Data_Format_Upload(Smoke_State_Flag_g,NTC_MF58_Det_Result_g,
                                           Fire_Start_Key_Result_g,Floor_Alarm_Staet_g,
                                           Fire_State_Flag_g);              
            
        }     
    }
}

//*******************************************/
//������:Sys_Mode_State
//��  ��:ϵͳÿ��ģ���״̬
//��  ��:��
//����ֵ:��
//ʱ  ��:20180525
//������:ahren
/*******************************************/
void Sys_Mode_State(void)
{
    if(Smoke_Det_Result_g)
    {
        Smoke_State_Flag_g = 0x01;  //����Ԥ��
    }
    else if(Battery_Voltage_Det_Result_g)
    {
        Smoke_State_Flag_g = 0x02;  //�����ѹ����    
    }
    else
    {
        Smoke_State_Flag_g = 0x00;  //��������
    }
    
    if((Fire_Start_Signal_Flag_g) || (Fire_Start_Key_Result_g) || (Fire_Back_Pressure_Result_g == 0x01))
    {
        Fire_State_Flag_g = 0x01;   //���ͷ����    
    }
    else if((Fire_Open_Test_Result_g) || (Fire_Back_Pressure_Result_g == 0x02))
    {
         Fire_State_Flag_g = 0x02;   //���ͷ����  
    }
    else
    {
        Fire_State_Flag_g = 0x00;   //���ͷ����
    }
}
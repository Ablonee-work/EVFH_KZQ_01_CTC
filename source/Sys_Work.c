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

/***************头文件****************/
/**/
/*************************************/
#include "config.h"
//******************宏定义*************/
/**/
/*************************************/

/*************************************/

/***************标志位申明************/
/**/
u8 Sys_Time_Count_g      = 0x00;                //系统时钟计数标志位
u8 Sys_Onesecond_Count_g = 0x00;               //系统定时1秒钟计数标志位

u8 Return_Time_Data = 0x00;                   //时间计数返回值 

/*************************************/

/***************函数申明**************/
/**/
void Sys_Work_State_Display(void);
void Sys_Mode_State(void);
/*************************************/


//*******************************************/
//函数名:Sys_Time_Count
//功  能:系统时钟计数函数
//参  数:无
//返回值:Value
//时  间:20180408
//创建者:ahren
/*******************************************/
u8 Sys_Time_Count()
{   
    if(0x01 == Sys_Timer_Flag_g)           
    {
        Sys_Timer_Flag_g = 0x00;
        
        Sys_Time_Count_g++;           //系统时钟计数标志位自加1
        
        Sys_Onesecond_Count_g++;     //系统定时1秒计数标志位自加1
        
#if 0
        if(0x1E == Sys_Onesecond_Count_g)
        {
            //NB_Mode_Reset_Flag_g = 1;
            
            //RG4 = 0;
        }
#endif       
        Fire_Start_Test_Flag_g = 1;             //点火头启动延时时间标志位
        
        Fire_Open_Test_Time_Flag_g = 1;         //点火头开路检测时间标志位
        
        //Fire_Start_Test_Conut_g++;            //时间计数标志位
        
        Smoke_Time_Flag_g = 1;               //烟雾检测延时标志位置1
        Battery_Voltage_Time_Flag_g = 1;    //电池检测延时标志位置1
        
        if(RX8010SJ_Wake_UP_Flag_g)
        {        
            RX8010SJ_Wake_UP_Count_g++;   //外部RTC唤醒后，延时1S钟清楚唤醒功能
        }
    } 
    /*******************************************/
    if(0xC8 == Sys_Onesecond_Count_g)
    {        
        Sys_Onesecond_Count_g = 0x00;
        Sys_Onesecond_Flag_g  = 0x01;   //1秒标志位置1
        
        MCU_Send_Data_Time_Flag_g = 1;
        
        MCU_Send_Data_Time_Count_g++;
        
        if(0x03 == MCU_Send_Data_Time_Count_g)
        {
            NB_Mode_Reset_Flag_g = 1;       //NB 模块唤醒需等待3秒
        }                     
        //系统上电10S后开始初始化NB模块，10S后NB模块的信号值才比较稳定
        if((!MCU_Send_Data_Stop_Count_g) && (MCU_Send_Data_Time_Count_g == 0x0A))
        {
            //MCU_Send_Data_Time_Count_g = 0;
            NB_Init_End_Flag_g = 1;
            MCU_Send_Data_Stop_Count_g = 1;
        }
        
        if(0x0A == MCU_Send_Data_Time_Count_g)
        {
            NB_Dealy_10S_Time_Flag_g = 1;//10s发一次数据标志位
            MCU_Send_Data_Time_Count_g = 0;//MCU_Send_Data_Time_Count_g自加到10后清零
        } 
        /****************************************/	
        //LED_Sys变更为LED_Green                          
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
//函数名:Sys_Function_Work
//功  能:系统工作任务分配
//参  数:无
//返回值:Value
//时  间:20180408
//创建者:ahren
/*******************************************/
void Sys_Function_Work(void)
{
    u8 Sys_Function_Work_Flag = 0x00;
    
    Sys_Function_Work_Flag = Sys_Time_Count();
    
    Return_Time_Data = 0x00;
    
    switch(Sys_Function_Work_Flag)
    {
        case 1:
                Fire_Open_Test_Result_g = Fire_Open_Det();    //点火头开路检测
                
                break;
        case 2:
                Smoke_Det_Result_g    = Smoke_Det_Function(); //烟雾预警检测
                
                break;
        case 3:          
                Battery_Voltage_Det_Result_g = Battery_Voltage_Det();  //电池低压检测           
                                
                break;
        case 4:
                Fire_Start_Signal();    //点火头启动信号检测
                Fire_Start_Key_Result_g = Fire_Start_Key();   //手报按键检测
                Fire_Back_Pressure_Result_g = Fire_Back_Pressure();//反馈信号与漏压检测
                
                break;
        case 5:
                NTC_MF58_Det_Result_g = NTC_MF58_Vol_Det();   //温度预警检测
                
                break;
        case 6:
                Sys_Time_Count_g = 0x00; 
                Sys_Mode_State();   //系统各个模块状态检测
                Sys_Work_State_Display();   //将系统各模块状态数据通过NB发送   
                
                break;                
        default:
            
               break;
    }
}

//*******************************************/
//函数名:Sys_Function_Work
//功  能:读取系统各个工作状态
//参  数:无
//返回值:Value
//时  间:20180410
//创建者:ahren
/*******************************************/
void Sys_Work_State_Display(void)
{
    if((!Fire_Start_Signal_Flag_g) && (!Fire_Start_Key_Result_g))//点火头未启动
    {
        Sys_Wake_Up_Flag_g = 1; //唤醒标志位置1
        
        //判断系统各个模块是否发送预警或故障
        if((Fire_Open_Test_Result_g) || (Smoke_Det_Result_g) || 
           (Battery_Voltage_Det_Result_g) || (NTC_MF58_Det_Result_g) ||
           (0x02 == Fire_Back_Pressure_Result_g))                       
        {       
            Sys_Fault_Flag_g = 1;
            
            /****************************************/	
            //将LED_Green_Function()函数修改为LED_Red_Function()函数                           
            //ahren
           //20180518
           /*************************************/	
            //LED_Green_Function(); 
            LED_Red_Function();
           /*************************************/	
            
            //if((Sys_Wake_Up_Flag_g) && (Sleep_Start_Flag_g) && (NB_Mode_Reset_Flag_g))
            if((Sys_Wake_Up_Flag_g) && (Sleep_Start_Flag_g) && (NB_Dealy_10S_Time_Flag_g))//10s发一次数据
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
            //将LED_Green_Function()函数修改为LED_Red_Function()函数                           
            //ahren
           //20180518
           /*************************************/	
            //LED_Green = 1; 
            LED_Red = 1; 
            Sys_Fault_Flag_g = 0;
            NB_Dealy_10S_Time_Flag_g = 0;
           /*************************************/	
            if((Sys_Wake_Up_Flag_g) && (Sleep_Start_Flag_g) && (NB_Mode_Reset_Flag_g))//唤醒3S后发送一次数据
            {                
                NB_Protocol_Data_Format_Upload(Smoke_State_Flag_g,NTC_MF58_Det_Result_g,
                                               Fire_Start_Key_Result_g,Floor_Alarm_Staet_g,
                                               Fire_State_Flag_g);                                  
            }  
        }  
        //进入Sleep模式前，判断烟雾是否有预警
        if((!Smoke_Det_Result_g) && (Sys_Wake_Up_Flag_g) && (Sleep_Start_Flag_g) && (NB_Mode_Reset_Flag_g))
        {
            NB_Mode_Reset_Flag_g = 0;
            //Sys_Fault_Flag_g = 0;
            Sleep_Function();        
        }
    }
    else
    {
        if(NB_Dealy_10S_Time_Flag_g)//发生火情，10S发送一次数据
        {                             
            NB_Dealy_10S_Time_Flag_g = 0;
                  
            NB_Protocol_Data_Format_Upload(Smoke_State_Flag_g,NTC_MF58_Det_Result_g,
                                           Fire_Start_Key_Result_g,Floor_Alarm_Staet_g,
                                           Fire_State_Flag_g);              
            
        }     
    }
}

//*******************************************/
//函数名:Sys_Mode_State
//功  能:系统每个模块的状态
//参  数:无
//返回值:无
//时  间:20180525
//创建者:ahren
/*******************************************/
void Sys_Mode_State(void)
{
    if(Smoke_Det_Result_g)
    {
        Smoke_State_Flag_g = 0x01;  //烟雾预警
    }
    else if(Battery_Voltage_Det_Result_g)
    {
        Smoke_State_Flag_g = 0x02;  //烟雾低压故障    
    }
    else
    {
        Smoke_State_Flag_g = 0x00;  //烟雾正常
    }
    
    if((Fire_Start_Signal_Flag_g) || (Fire_Start_Key_Result_g) || (Fire_Back_Pressure_Result_g == 0x01))
    {
        Fire_State_Flag_g = 0x01;   //点火头启动    
    }
    else if((Fire_Open_Test_Result_g) || (Fire_Back_Pressure_Result_g == 0x02))
    {
         Fire_State_Flag_g = 0x02;   //点火头故障  
    }
    else
    {
        Fire_State_Flag_g = 0x00;   //点火头正常
    }
}
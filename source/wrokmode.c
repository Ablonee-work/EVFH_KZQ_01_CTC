/*----------------------------------------------------------------------*/
/* Confidential and Proprietary Information of MicroPerfect Corporation */
/* (C) 2017 ,MicroPerfect Corporation . All rights reserved. */
/* FileName : workmode.c */
/* New File Date:2017.11.09 */
/* Function : work mode choice*/
/* Ver:001 */
/* Author:MP_Ahren*/
/* History:YYYY.MM.DD Version Author Description */
/*        : */
/*-------------------------------------------------------------*/


/***************头文件****************/
/**/
/*************************************/
#include"workmode.h"
//******************宏定义*************/
/**/
/*************************************/


/*************************************/

/***************标志位申明************/
/**/
//static u8 Return_Time_Data = 0x00;                  //时间计数返回值 
static u8 Sleep_Work_Mode_Count_g = 0x00;           //睡眠延时计数标志位
extern u8 Time_5ms_Flag_g;
extern u8 Cnt_Num;
extern u8 Error_Flag;

extern u8 TC04_ID_Num[20];//ID数据存储
extern u8 TC04_ID_Cnt[20];//ID统计
extern u8 TC04_Off_Line_Val_Buff[20];//离线标志位
extern u8 TC04_Off_Line_Cnt_Buff[20];//离线标志位
extern u8 TC04_Off_Time_Cnt_g;

/***************函数申明**************/
/**/
/*************************************/

static void Sys_Sleep_Work_Mode(void);            //睡眠工作模式函数		

/*************************************/

//*******************************************/
//函数名:Sys_Time_Count_WorkMode
//功  能:工作模式的系统时间计数,一些需要计数的工作可以在此函数操作
//参  数:无
//返回值:无 
//时  间:20171106
//创建者:ahren
/*******************************************/
u8 Sys_Time_Count_WorkMode(void)
{	
    static u8 Sys_Time_Count_g = 0x00;				//系统时间计数标志位
    //static u8 Sys_Onesecond_Count_g = 0x00;         //1秒时间计数标志位
    u8 Return_Time_Data = 0x00;             //系统任务时间分配返回值     
    
	if(0x01 == Sys_Time_Falg_g)
	{
		Sys_Time_Falg_g   = 0x00;                   //将系统定时5ms标志位清零
        
		Sys_Time_Count_g++;                         //系统任务时间分配计数加1
        Sys_Onesecond_Count_g++;	
        CAN_Sent_Time_Cnt_g++;//CAN 发送时间计数
        
		Power_ACC_Timer_Flag_g = 0x01;              //Power_ACC_Timer_Flag_g置1,用于ACC检测消抖计数
        Power_BU_Timer_Flag_g  = 0x01;              //Power_BU_Timer_Flag_g置1，用于BU检测消抖计数
        
        Time_5ms_Flag_g = 1;//CAN 数据分析时间标志位
        
		if((0x05 == Sys_Onesecond_Count_g) && (Work_Mode_Confirm_Flag_g == 0))
		{	
			Work_Mode_Confirm_Flag_g = 0x01;        //定时25ms，用于确认系统工作模式           
		}
	}
    /*******************************************/   
    
    if(0xC8 == Sys_Onesecond_Count_g)               //计数定时1秒钟
    {
        Sys_Onesecond_Count_g = 0x00;        
        Sys_Onesecond_Flag_g = 0x01;                //1秒标志位置1 
        
        MCU_CAN_Send_1s_Flag_g =1;
        
        MCP_CAN_Send_1s_Flag_g = 1;
        
        Led_Display_Fuction();
               
    } 
    
    /*******************************************/  
    
    if(0x01 == Sys_Time_Count_g)
    {
        Return_Time_Data = 0x01;
    }
    else if(0x02 == Sys_Time_Count_g)
    {       
        Return_Time_Data = 0x02;
    }
    else if(0x03 == Sys_Time_Count_g)
    {      
        Return_Time_Data = 0x03;
        Sys_Time_Count_g = 0;
    }
    else if(0x04 == Sys_Time_Count_g)
    {      
        Return_Time_Data = 0x04;
    }
    else if(0x05 == Sys_Time_Count_g)
    {        
        Return_Time_Data = 0x05;
    }
    else if(0x06 == Sys_Time_Count_g)
    {        
        Return_Time_Data = 0x06;
    }
    else 
    {
        Return_Time_Data = 0x00;
    }
    
    return Return_Time_Data;     
}

//*******************************************/
//函数名:Sys_Function_Work
//功  能:系统工作任务分配
//参  数:无
//返回值:Value
//时  间:20180625
//创建者:ahren
/*******************************************/
void Sys_Function_Work(void)
{
    u8 Sys_Function_Work_Flag = 0x00;

    if(Implement_Only_1_Variable == 0)
    {         
        Implement_Only_1_Variable = 1;  //该标志位置1，不进入睡眠则只执行一遍
        
        ACC_ON_Sys_Init();              //ACC ON 相关初始化
        
        CAN_STB_ON;                     //开启MCU CAN使能PIN

        MCP2515_SILENT_ON;              //开启mcp2515 CAN使能PIN
    }
#if 0        
    //*******************************************/
    //系统任务少，屏蔽任务分配功能
    //时  间:20180610
    //修改者:ahren 

    Sys_Function_Work_Flag = Sys_Time_Count_WorkMode();

    /*******************************************/       
    
    //Return_Time_Data = 0x00;
    
    switch(Sys_Function_Work_Flag)
    {
        case 1:
#if 0
                CAN_Interrupt_Occurred();           //can接收   //MCU CAN收，MCP2515 CAN发
#endif
            break;
            
        case 2:
#if 0
                MCP2515_Interrupt_Occurred();           //can2接收 //MCP2515 CAN收，MCU CAN发
#endif
            break;
            
        case 3:  
#if 0
                CAN_Send_Alarm_Warning_Data();       //MCU CAN1 发送心跳包
#endif 
#if 0
                CAN2_Send_Alarm_Warning_Data();
#endif
            break;

        default:
            
            break;
    }
#endif
}

//*******************************************/
//函数名:Work_Mode_Choice
//功  能:工作模式选择
//参  数:无
//返回值:无
//时  间:20171109
//创建者:ahren
//*******************************************/
void Work_Mode_Choice(void)
{
	u8 Work_Mode_Value = 0x00;		
	
	Work_Mode_Value = Power_Work_Mode_Confirm();				//读取电源状态返回值
							                                   
    if(Work_Mode_Confirm_Flag_g == 1)                           //判断25ms计时是否到了
	{			
		switch(Work_Mode_Value)
		{
			case 0x01:
						Work_Mode_Value = 0x00;					//将Work_Mode_Value清零
						Sleep_Work_Mode_Count_g = 0x00;			//BU +ACC ON 工作模式，将睡眠模式计数清零						 
                        Sys_Function_Work();
				break;
				
			case 0x02:			
						Work_Mode_Value = 0x00;					//将Work_Mode_Value清零
						//Work_Mode_Confirm_Flag_g = 0x00;		//将工作模式确认标志位清零
						Sys_Sleep_Work_Mode();					//调用睡眠工作模式函数   
				break;
                
			case 0x03:	
                		Work_Mode_Value = 0x00;					//将Work_Mode_Value清零                  
				break;                
				
			default:
						Work_Mode_Value = 0x00;
				break;
		}
	}

}

//*******************************************/
//函数名:Sys_Sleep_Work_Mode
//功  能:睡眠工作模式
//参  数:无
//返回值:无
//时  间:20171109
//创建者:ahren
//*******************************************/
static void Sys_Sleep_Work_Mode(void)
{
    Sleep_Work_Mode_Count_g++;                  //进入睡眠模式计数，即进行两次睡眠条件监测
	
	if(0x02 <= Sleep_Work_Mode_Count_g)
	{
		Sleep_Work_Mode_Count_g = 0x00;
		
		/****************************************************************/
		//进入Sleep模式前需要将相关的功能关闭，降低功耗
		//20171109
		//arhen
		/****************************************************************/	
        
        MCP2515_Enter_Sleep_Mode();      

        ACC_OFF_Sys_Init();		
		/****************************************************************/
		//清除电池箱数量
		//20191012
		//arhen
        Cnt_Num = 0;       
        memset(TC04_ID_Cnt, 0, 20); // 将 TC04_ID_Cnt 数组清零
        memset(TC04_ID_Num, 0, 20); // 将 TC04_ID_Num 数组清零
        memset(TC04_Off_Line_Val_Buff, 0, 20); // 将 TC04_Off_Line_Val_Buff 数组清零
        memset(TC04_Off_Line_Cnt_Buff, 0, 20); // 将 TC04_Off_Line_Cnt_Buff 数组清零        
		/****************************************************************/       
		//清除电池箱数量
		//20191012
		//arhen        
        Error_Flag = 0;   

		/****************************************************************/	
		/****************************************************************/
		//确定系统进入了睡眠模式，执行睡眠指令SLEEP();
		//20171109
		//arhen
		/****************************************************************/	
        if(ACC_DET_State)
        {
            SLEEP();	//睡眠指令

            /****************************************************************/

            /***************************************************************/
            //系统被唤醒后，先执行两条空指令，使时钟稳定再进行系统初始化
            //20171109
            //arhen
            /***************************************************************/	
            NOP();		//空指令
            NOP();		//空指令
        }         
        Sleep_Work_Mode_Count_g = 0x00;			//唤醒后，将睡眠模式计数清零
        
        BU_ON_Sys_Init();		
		/****************************************************************/		
	}	
}

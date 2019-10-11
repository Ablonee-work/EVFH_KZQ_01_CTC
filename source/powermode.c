/*----------------------------------------------------------------------*/
/* Confidential and Proprietary Information of MicroPerfect Corporation */
/* (C) 2017 ,MicroPerfect Corporation . All rights reserved. */
/* FileName : powermode.c */
/* New File Date:2017.11.02 */
/* Function : power det (BU/ACC/Sleep)*/
/* Ver:001 */
/* Author:MP_Ahren*/
/* History:YYYY.MM.DD Version Author Description */
/* 
          :2018.06.25 002 @Ahren Add BU Det Function
 */
/*-------------------------------------------------------------*/


/***************头文件****************/
/**/
/*************************************/

#include"powermode.h"


//******************宏定义*************/
/**/


/*************************************/

/***************标志位申明************/
/**/
/*************************************/

static u8 ACC_DET_State_Count_g = 0x00;			//ACC状态检测计数 
static u8 ACC_DET_State_Flag_g  = 0x00;			//ACC状态检测计数 

static u8 BU_DET_State_Count_g = 0x00;			//ACC状态检测计数 
static u8 BU_DET_State_Flag_g  = 0x00;			//ACC状态检测计数 

/*20180712*/
static u8 Sleep_Work_Mode_Count_g = 0x00;		//睡眠延时计数标志位

/***************函数申明**************/
/**/
/*************************************/


/*************************************/


/********************************************/
//函数名:BU_State_Det_Init
//功  能:电源BU检测相关Prot Pin初始化
//参  数:BU_State_Init
//返回值:无 
//时  间:20171102
//创建者:ahren
/*******************************************/
void BU_State_Det_Init(void)
{	 
	BU_DET_Direction = 1;		//设置I/O口方向为输入
		
	BU_DET_State 	 = 1;		//设置I/O口初始化为高电平		
}

/********************************************/
//函数名:ACC_State_Det_Init
//功  能:电源ACC检测相关Prot Pin初始化
//参  数:ACC_State_Init
//返回值:无 
//时  间:20171102
//创建者:ahren
/*******************************************/
void ACC_State_Det_Init(void)
{	
	ACC_DET_Direction = 1;		//设置I/O口方向为输入
	
	ACC_DET_State     = 1;  	//设置I/O口初始化为高电平		
}

/********************************************/
//函数名:Power_BU_State_Det
//功  能:电源 BU 状态检测
//参  数:无
//返回值:
//时  间:20171102
//创建者:ahren
/*******************************************/
u8 Power_BU_State_Det()
{      
    //if(BU_DET_State == 0x00)                    //0→1高电平检出
    if(BU_DET_State)  //1高电平检出    
    {
        if(Power_BU_Timer_Flag_g)
        {
            Power_BU_Timer_Flag_g = 0x00;
            
            BU_DET_State_Count_g++;
            
            if(0x04 <= BU_DET_State_Count_g)
            {
                BU_DET_State_Count_g = 0;
                /*20180712*/
                if(BU_DET_State)//1高电平检出   
                {   
                    BU_DET_State_Flag_g = 0x01;
                }
            }
        }
    }
    else
    {
        BU_DET_State_Count_g = 0;
                
        BU_DET_State_Flag_g  = 0;    
    }
    
    return (BU_DET_State_Flag_g);
}

/********************************************/
//函数名:Power_ACC_State_Det
//功  能:电源 ACC 状态检测
//参  数:无
//返回值:
//时  间:20171102
//创建者:ahren
/*******************************************/
u8 Power_ACC_State_Det()
{    
    if(!ACC_DET_State)									//检测ACC Pin脚是否为低电平
    {
 		if(Power_ACC_Timer_Flag_g)						//检测延时消抖标志位是否为1		
 		{
	 		Power_ACC_Timer_Flag_g = 0x00;				//将延时消抖标志位清零	
	 		
	 		ACC_DET_State_Count_g++;					//ACC状态检测计数	
	 		
			if(0x04 <= ACC_DET_State_Count_g)			//计数≥4 消抖延时20ms
			{
				ACC_DET_State_Count_g = 0x00;			//将计数清零
				/*20180712*/
                if(ACC_DET_State  == 0x00)
                {   
                    ACC_DET_State_Flag_g = 0x01;			//将ACC状态返回值置1
                }
			}	 			
	 	}	   
    }
    else
    {
    	ACC_DET_State_Count_g = 0x00;					//将ACC状态计数清零
    	
    	ACC_DET_State_Flag_g  = 0x00;  					//将ACC状态返回值清零
    }
       
    return (ACC_DET_State_Flag_g);						//返回ACC状态值
}

/********************************************/
//函数名:Power_Det_Port_Init
//功  能:电源管脚初始化
//参  数:无
//返回值:
//时  间:20180625
//创建者:ahren
/*******************************************/
void Power_Det_Port_Init(void)
{
    BU_State_Det_Init();
    ACC_State_Det_Init();
}

//*******************************************/
//函数名:Powr_Work_Mode_Confirm
//功  能:电源工作模式确认
//参  数:无
//返回值:ACC_Work_Mode_Value/BU_Work_Mode_Value
//		:ACC_Work_Mode_Value=0x01代表ACC ON  工作模式
//		:ACC_Work_Mode_Value=0x02代表ACC OFF 工作模式
//		:BU_Work_Mode_Value=0x01代表BU ON  工作模式
//		:BU_Work_Mode_Value=0x02代表BU OFF 工作模式
//时  间:20171109
//创建者:ahren
//*******************************************/
u8 Power_Work_Mode_Confirm()
{	
	u8 ACC_Work_Mode_Value = 0x00;	
    u8 BU_Work_Mode_Value  = 0x01;
           
	ACC_Work_Mode_Value = Power_ACC_State_Det();	//读取ACC状态返回值,01有检到
#if 1
	//BU_Work_Mode_Value  = Power_BU_State_Det();     //读取BU状态返回值，01有检到
  
    if((BU_Work_Mode_Value == 0x01) && (ACC_Work_Mode_Value == 0x01))
    {   
        return (0x01);								//BU +ACC ON,工作模式返回值 
    }
    else if((BU_Work_Mode_Value == 0x01) && (ACC_Work_Mode_Value == 0x00))
	{		
		return (0x02);								//ACC OFF工作模式返回值,Sleep
	}
    else
    {
        return (0x02);								//ACC OFF工作模式返回值
    }
#endif

 #if 0 
	if(ACC_Work_Mode_Value)
	{		
		return (0x01);								//ACC ON工作模式返回值
	}
	else
	{		
		return (0x02);								//ACC OFF工作模式返回值
	}
#endif 	
}

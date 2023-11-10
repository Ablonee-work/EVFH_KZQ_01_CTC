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

/***************函数申明**************/
/**/
/*************************************/

u8 Work_Mode_Confirm_Flag_g = 0x00;				//工作模式确认标志位
u8 Implement_Only_1_Variable = 0x00;      //当检测到ACC时才初始化那些用户函数，上电只执行一次  
u8 Sys_Onesecond_Count_g = 0x00;          //系统时间定时1s
static void Sys_Sleep_Work_Mode(void); //睡眠工作模式函数		
u8 CAN_Sent_Time_Cnt_g = 0;  //CAN报文发送周期时间

u8 PowerOn_Cnt = 0;  //CAN报文发送周期时间
/*************************************/
//*******************************************/
//函数名:CAN_Sent_Time_Choice
//功  能:设置CAN 发送周期
//参  数:无
//返回值:Ret_Val
//时  间:20190719
//创建者:ahren
/*******************************************/
u8 CAN_Sent_Time_Choice(u8 select)
{
    CAN_Sent_Time_Cnt_g++;   //CAN报文发送周期时间
    if(0x01 == select)
    {
        if(CAN_Sent_Time_Cnt_g >= 50)//250ms
        {
            CAN_Sent_Time_Cnt_g = 0;
            return 1;
        }
    }
    else if(0x02 == select)
    {
        if(CAN_Sent_Time_Cnt_g >= 100)//500ms
        {
            CAN_Sent_Time_Cnt_g = 0;
            return 1;
        }    
    }
    else
    {
        if(CAN_Sent_Time_Cnt_g >= 200)//1000ms
        {
            CAN_Sent_Time_Cnt_g = 0;
            return 1;
        }     
    }    
    return 0;
}
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
	if(0x01 == Sys_Time_Falg_g)
	{
		Sys_Time_Falg_g = 0x00;                   //将系统定时5ms标志位清零
        
        Sys_Onesecond_Count_g++;
          
        //报文发送周期计数
        if(CAN_Sent_Time_Choice(Outgoing_Data_Cycle))//对外发送报文周期，定时时间到
        {
            TJA1042_Sent_Time_Flag = 1;
            if((TC_Num >= 1)&&(TC_Num <= TC_NUM_MAX))
            {
                MCP2515_Sent_Time_Flag = 1; //探测器数量正确且数量确认完成，则开始对外发送报文
            }              
        }        

        TC04_Off_Time_Cnt_g++;   //探测器离线检测周期时间
        
		Power_ACC_Timer_Flag_g = 0x01;              //Power_ACC_Timer_Flag_g置1,用于ACC检测消抖计数
        Power_BU_Timer_Flag_g  = 0x01;              //Power_BU_Timer_Flag_g置1，用于BU检测消抖计数
                
		if((5 == Sys_Onesecond_Count_g) && (Work_Mode_Confirm_Flag_g == 0))
		{	
			Work_Mode_Confirm_Flag_g = 0x01;        //定时25ms，用于确认系统工作模式           
		}
	}
    /*******************************************/   
    
    if(200 == Sys_Onesecond_Count_g)               //计数定时1秒钟
    {
        Sys_Onesecond_Count_g = 0x00;        
        Led_Display_Fuction();  
        
        if(PowerOn_OfflineDet_Flag < 20)
        {
            PowerOn_OfflineDet_Flag++;
        } 
    } 
    return 0;     
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
    uint32_t CAN_ID_SIDH = 0;
    uint32_t CAN_ID_SIDL = 0;
    uint32_t CAN_ID_EIDH = 0;
    uint32_t CAN_ID_EIDL = 0;      
    uint32_t CAN_ID_Flag = 0;
    
    if(Implement_Only_1_Variable == 0)
    {         
        Implement_Only_1_Variable = 1;  //该标志位置1，不进入睡眠则只执行一遍
        
        ACC_ON_Sys_Init();              //ACC ON 相关初始化
        
        MCP2515_SILENT_ON();              //开启mcp2515 CAN使能PIN
        
        //读取EEPROM中的探测器数量
        TC_Num = eeprom_read(0x03);//将电池箱数量从EEPROM读出
        NOP();NOP();NOP();NOP();  
        
        if((TC_Num == 0)||(TC_Num > TC_NUM_MAX))
        {
            TC_Num = 0;//写入默认数量
            eeprom_write(0x03, TC_Num);
            NOP();NOP();NOP();NOP();       
        }
        
        //读取对整车报文ID
        CAN_ID_Flag = eeprom_read(0x09);//CAN默认ID被修改标志位
        NOP();NOP();NOP();NOP();      
        if(CAN_ID_Flag == 0x5A)
        {
            CAN_ID_SIDH = eeprom_read(0x04);                        
            NOP();NOP();NOP();NOP();
            CAN_ID_SIDL = eeprom_read(0x05);                     
            NOP();NOP();NOP();NOP();  
            CAN_ID_EIDH = eeprom_read(0x06);                        
            NOP();NOP();NOP();NOP();
            CAN_ID_EIDL = eeprom_read(0x07);                        
            NOP();NOP();NOP();NOP();    
	        MCP2515_TxID = (CAN_ID_SIDH<<24)|(CAN_ID_SIDL<<16)|(CAN_ID_EIDH<<8)|(CAN_ID_EIDL);
        }
        
        //读取对外发送报文周期时间
        Outgoing_Data_Cycle = eeprom_read(0x08);
        NOP();NOP();NOP();NOP();      
        if(0x01 == Outgoing_Data_Cycle)
        {
        }
        else if(0x02 == Outgoing_Data_Cycle)
        {
        }
        else if(0x03 == Outgoing_Data_Cycle)
        {
        }
        else
        {
            Outgoing_Data_Cycle = 3;//默认3：1s周期
            eeprom_write(0x08,Outgoing_Data_Cycle);                           
            NOP();NOP();NOP();NOP();
        }            
    }
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
						Sleep_Work_Mode_Count_g = 0x00;			//BU +ACC ON 工作模式，将睡眠模式计数清零						 
                        Sys_Function_Work();
				break;
				
			case 0x02:			
						Sys_Sleep_Work_Mode();					//调用睡眠工作模式函数   
				break;           				
			default:break;
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
    u8 i,j;
    static u8 sec_cnt = 0;
    static u8 send_cnt = 0;
    
    Sleep_Work_Mode_Count_g++;                  //进入睡眠模式计数，即进行两次睡眠条件监测
	
	if(0x02 <= Sleep_Work_Mode_Count_g)
	{
		Sleep_Work_Mode_Count_g = 0x00;
		
        MCP2515_Sleep_Mode();      

        ACC_OFF_Sys_Init();		

        if(ACC_DET_State)
        {
            //SLEEP();	//睡眠指令
            //20210225
            //探测器睡眠
            
            for(i=0; i<TC_Num; i++)TC_Sleep_Flag[i] = 1;
            
            send_cnt = 0;
            CanToCan_TC_F0(0x01); //低功耗
            send_cnt++;
            sec_cnt = 0;                        
            while(1)
            {
                TC04_CAN_Data_Process();            //探测器数据处理
                
                if(0x01 == Sys_Time_Falg_g)
                {
                    Sys_Time_Falg_g = 0x00;   
                    sec_cnt++;   
                    
                    if(sec_cnt>200)
                    {
                        sec_cnt = 0;
                        
                       if(send_cnt>=3)
                       {
                           break;
                       }
                       else 
                       {
                            CanToCan_TC_F0(0x01); //低功耗
                            send_cnt++;
                       }
                    } 
                    
                    //检测接收到F0报文回复
                    j=0;
                    for(i=0; i<TC_Num; i++)j += TC_Sleep_Flag[i];
                    if(j==0)break;
                }                 
            }
            
            Sys_Sleep_Flag = 1;  
            TC_Code_Err = 0;
            
            memset(RxMsgBuff_Flag, 0, TC_DATA_BUFF); 
            memset(TC04_Buff, 0, TC_NUM_MAX*9);//清空所有探测器数据
            memset(TC04_OffLine_Flag, 0, TC_NUM_MAX); //清除离线标志
            
            while(1)
            {
                TC04_CAN_Data_Process();            //探测器数据处理
                
                if(!ACC_DET_State)//ACC唤醒
                {
                    Sys_Sleep_Flag = 0;
                    break;
                }                
                if(Sys_Sleep_Flag == 2)
                {      
                    Sys_Sleep_Flag = 3;
                    break;
                }
            }
            Sys_Sleep_Wake_TC = 1;
            PowerOn_OfflineDet_Flag = 0;

            //20210225
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

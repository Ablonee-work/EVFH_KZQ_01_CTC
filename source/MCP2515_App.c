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

/***************头文件****************/
/**/
/*************************************/
#include"MCP2515_App.h"

//******************宏定义*************/
/**/

/*************************************/

/*************************************/

/***************标志位申明************/
/**/

uint32_t MCP2515_TxID = 0x18FF301D;
uint16_t Time_Send_Cnt = 0;
u8 Get_Abnormal_Cnt = 0;
/*************************************/


/***************函数申明**************/
/**/

/*************************************/

/*************************************************************************/
//函数名:MCP2515_User_Agreement
//功  能:车厂用户协议数据
//参  数:无
//返回值：无
//时间：20191206
//创建者：ahren
/************************************************************************/
u8 Version_Data_Normal[9] = {0x23,0x08,0x28,0x14,0x35,0xFF,0x00,0x0C};//软硬件版本信息 1.1

void MCP2515_User_Agreement()
{    
    static u8 TC_Idx = 0;
    static u8 Life_Signal = 0;//10S计数
    uint16_t Temp_SumData = 0x00;
    u8 MCP2515_Buff_F1[9] = {0};
    u8 MCP2515_Buff_E2[9] = {0};
    
    //MCP2515 发送数据解析与处理
    if ((0x01 == MCP2515_Sent_Time_Flag))
    {
        MCP2515_Sent_Time_Flag = 0;   
        Time_Send_Cnt++;
        Life_Signal++;
        if(Life_Signal>255)     Life_Signal = 0;
        
        //选择对应的电池箱编号，将数据发送到车身仪表       
        MCP2515_Buff_F1[0] = 0x00;//预留
        MCP2515_Buff_F1[1] = 0x00;//故障代码+故障等级
        MCP2515_Buff_F1[2] = 0x00;//预留
        MCP2515_Buff_F1[3] = 0x00;//预留
        MCP2515_Buff_F1[4] = TC_Idx+1;//电池箱编号
        MCP2515_Buff_F1[5] = 0x00;//预警级别
        MCP2515_Buff_F1[6] = 0x00;//预留
        MCP2515_Buff_F1[7] = Life_Signal;//消息计数器
        
        
        //选择对应的电池箱编号，将数据发送到车身仪表       
        MCP2515_Buff_E2[0] = TC_Idx+1;//电池箱编号
        MCP2515_Buff_E2[1] = 0x00;//探测器启动灭火弹类型
        MCP2515_Buff_E2[2] = 0x00;//火情级别
        MCP2515_Buff_E2[3] = 0x00;//故障码
        MCP2515_Buff_E2[4] = 0x00;//子阀打开或关闭命令
        MCP2515_Buff_E2[5] = 0x00;//灭火系统故障码
        MCP2515_Buff_E2[6] = 0x00;//预留
        MCP2515_Buff_E2[7] = Life_Signal & 0x0F;//生命帧   
        
        
        if(TC04_OffLine_Flag[TC_Idx]<3)//在线
        {
            if(TC04_Buff[TC_Idx][1]==0x00)
            {
                MCP2515_Buff_F1[5] = 0x00;//预警级别
//                MCP2515_Buff_F1[4] = 0x00;
                MCP2515_Buff_F1[3] = 0x00;//故障代码+故障等级     
                
                MCP2515_Buff_E2[2] = 0x00;//火情级别
                MCP2515_Buff_E2[3] = 0x00;//故障码
            }
            else if(TC04_Buff[TC_Idx][1]==0x02)
            {
                MCP2515_Buff_F1[5] = 0x04 << 5;
                MCP2515_Buff_F1[3] = 0x01;  
                
                MCP2515_Buff_E2[2] = 0x02 << 5;//火情级别
                MCP2515_Buff_E2[3] = 0x00;//故障码     
            }
            else if(TC04_Buff[TC_Idx][1]==0x03)
            {
                MCP2515_Buff_F1[5] = 0x04 << 5;
                MCP2515_Buff_F1[3] = 0x02;      
                
                MCP2515_Buff_E2[2] = 0x03 << 5;//火情级别
                MCP2515_Buff_E2[3] = 0x00;//故障码 
            }
            else if(TC04_Buff[TC_Idx][1]==0x04)
            {
                MCP2515_Buff_F1[5] = 0x04 << 5;
                MCP2515_Buff_F1[3] = 0x03;        
                
                MCP2515_Buff_E2[2] = 0x04 << 5;//火情级别
                MCP2515_Buff_E2[3] = 0x01 << 5;//故障码 //灭火器已失效
                MCP2515_Buff_E2[1] = 0x03;//启动灭火类型
                MCP2515_Buff_E2[4] = 0x01 << 6;//子阀打开关闭命令
            }
            else if(TC04_Buff[TC_Idx][1]==0xFF)
            {
                if(TC04_Buff[TC_Idx][7]==0x88)
                {
                    MCP2515_Buff_F1[0] = 0x02;//预警级别
                    MCP2515_Buff_F1[5] = 0x01 << 5;//故障代码+故障等级
                    MCP2515_Buff_E2[3] = 0x02 << 5;//故障码 
                }              
            }       
        }
        else //离线
        {
            MCP2515_Buff_F1[0] = 0x01;//预警级别
            MCP2515_Buff_F1[5] = 0x01 << 5;//故障代码+故障等级
            MCP2515_Buff_E2[3] = 0x03 << 5;//故障码 
            MCP2515_Buff_E2[5] = 0x03;
        }        
        
        Temp_SumData = MCP2515_Buff_E2[0] + MCP2515_Buff_E2[1] + MCP2515_Buff_E2[2] + MCP2515_Buff_E2[3] 
                     + MCP2515_Buff_E2[4] + MCP2515_Buff_E2[5] + MCP2515_Buff_E2[6] + MCP2515_Buff_E2[7];
        Temp_SumData = (Temp_SumData & 0x00FF) + 0x18 + 0xFE + 0x30 + 0x1D;
        
        MCP2515_Buff_E2[8] = ( (Temp_SumData>>4) + Temp_SumData) & 0x000F;        // 检验和
        MCP2515_Buff_E2[7] = (MCP2515_Buff_E2[7] & 0x0F) + (MCP2515_Buff_E2[8]<<4);
        
            
        
        MCP2515_Data_Send(0x18FE301D, MCP2515_Buff_E2, 8);  
        MCP2515_Data_Send(0x18FF311D, MCP2515_Buff_E2, 8);  
        delay_10ms(5);
        MCP2515_Data_Send(0x18FF401D, Version_Data_Normal, 8);
        delay_10ms(5);
#if 0          //取消18FF301D
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
        //电池箱编号数据判断
        TC_Idx++;//电池箱编号计数   
        if(TC_Idx >= TC_Num)
        {
            TC_Idx = 0;  
        }       
    } 
}

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
u8 MCP2515_Buff[9] = {0};

extern u8 TC04_One_Buff[9];//1号电池箱CAN数据
extern u8 TC04_Two_Buff[9];//2号电池箱CAN数据
extern u8 TC04_Three_Buff[9];//3号电池箱CAN数据
extern u8 TC04_Four_Buff[9];//4号电池箱CAN数据
extern u8 TC04_Five_Buff[9];//5号电池箱CAN数据
extern u8 TC04_Six_Buff[9];//6号电池箱CAN数据
extern u8 TC04_Seven_Buff[9];//7号电池箱CAN数据
extern u8 TC04_Eight_Buff[9];//8号电池箱CAN数据
extern u8 TC04_Nine_Buff[9];//9号电池箱CAN数据
extern u8 TC04_Ten_Buff[9];//10号电池箱CAN数据
extern u8 TC04_Eleven_Buff[9];//11号电池箱CAN数据
extern u8 TC04_Twelve_Buff[9];//12号电池箱CAN数据
extern u8 TC04_Thirteen_Buff[9];//13号电池箱CAN数据
extern u8 TC04_Fourteen_Buff[9];//14号电池箱CAN数据
extern u8 TC04_Fifteen_Buff[9];//15号电池箱CAN数据
extern u8 TC04_Sixteen_Buff[9];//16号电池箱CAN数据
/*************************************/


/*************************************/

/***************标志位申明************/
/**/
extern u8 Cnt_Num;

/*************************************/


/***************函数申明**************/
/**/
void MCP2515_Data_Analysis(u8 Num_Cnt, u8 *Data_Buff, u8 Life_Cnt);
/*************************************/

/*************************************************************************/
//函数名:MCP2515_CAN_Tx_Buffer
//功  能:MCP2515_CAN发送函数
//参  数:发送数组首地址，数组长度
//返回值：无
//时间：20180703
//创建者：ahren
/************************************************************************/
void MCP2515_CAN_Tx_Buffer(u8 BOX_Num, u8 *CAN_TX_Buf, u8 len,u8 Life_Signal)
{
	u8 i = 0;
    u8 Cnt = 0;

    memcpy(MCP2515_Buff, CAN_TX_Buf, 8);

    MCP2515_Data_Analysis(BOX_Num, MCP2515_Buff, Life_Signal);
    //读取状态值，是否可以发送 
    while((MCP2515_ReadByte(MCP2515_TXB0CTRL)&0x0B) && (Cnt<50))   
    {			
        Cnt++;
    }

    //写入发送数据到发送缓冲区 
    for(i=0; i<len; i++)
    {
        MCP2515_WriteByte(MCP2515_TXB0D0 + i, MCP2515_Buff[i]);               
    } 

    MCP2515_CS = 0;          
    MCP2515_WriteByte(MCP2515_TXB0DLC, len);//写入发送的数据长度                                 
    MCP2515_WriteByte(MCP2515_TXB0CTRL,0x0B);//发送报文                           
    MCP2515_CS = 1;  
}

/*************************************************************************/
//函数名:TBOX_MCP2515_Sent
//功  能:发送到TBOX
//参  数:发送数组首地址，数组长度
//返回值：无
//时间：20190803
//创建者：ahren
/************************************************************************/
void Version_MCP2515_Sent(u8 *CAN_TX_Buf, u8 len)
{
	u8 i = 0;
    u8 Cnt = 0;

    while((MCP2515_ReadByte(MCP2515_TXB0CTRL)&0x0B) && (Cnt<50))        //读取状态值，是否可以发送
    {			
        Cnt++;
    }

    for(i=0; i<len; i++)
    {
        MCP2515_WriteByte(MCP2515_TXB0D0+i, Version_Data_Normal[i]);            //写入发送数据到发送缓冲区    
    }

    MCP2515_CS = 0;
    MCP2515_WriteByte(MCP2515_TXB0DLC, len);                            //写入发送的数据长度
    MCP2515_WriteByte(MCP2515_TXB0CTRL,0x0B);                           //发送报文
    MCP2515_CS = 1;  
}

void Version_ID_Change(void)
{
    MCP2515_TxID[0] = 0x18;              
    MCP2515_TxID[1] = 0xFE;   
    MCP2515_TxID[2] = 0xDA;  
    MCP2515_TxID[3] = 0x1D;      
    MCP2515_CAN_TxID(MCP2515_TxID,8);//发送ID设置 
    delay_10us(100); 
    NOP();NOP();NOP();NOP();NOP();NOP(); 
}

void Meter_ID_Change(void)
{
    MCP2515_TxID[0] = 0x18;                 
    MCP2515_TxID[1] = 0xFF;   
    MCP2515_TxID[2] = 0x30;
    MCP2515_TxID[3] = 0x1D;                              
    MCP2515_CAN_TxID(MCP2515_TxID,8);//发送ID设置 
    delay_10us(100);     
    NOP();NOP();NOP();NOP();NOP();NOP(); 
}

void Device_Data_Conversion(u8 Dev_Num, uint16 Life_Signal)
{
    u8 Dev_Val = 0;//设备编号
    uint16 Life_Val = 0;//信号值

    static u8 Offset_Val = 1;//Dev_Num 偏移量
    static u8 Length = 0x08;//数据发送长度

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
//函数名:MCP2515_User_Agreement
//功  能:车厂用户协议数据
//参  数:无
//返回值：无
//时间：20191206
//创建者：ahren
/************************************************************************/
void MCP2515_User_Agreement(u8 Time_Flag)
{
    u8 MCP2515_Sent_Time_Flag = 0;//时间标志位

    static u8 MCP2515_ID_Change_Flag = 0;//ID切换标志位
    static u8 MCP2515_Sent_Cnt = 0;
    static u8 Ten_Seconds_Cnt = 0;//10S计数
    static uint16 MCP2515_Life_Cnt = 0; 

    MCP2515_Sent_Time_Flag = Time_Flag;

#if 1    
    //MCP2515 发送数据解析与处理
    if ((0x01 == MCP2515_Sent_Time_Flag))
    {
        MCP2515_Sent_Time_Flag = 0;         
        MCP2515_Life_Cnt++;//消息计数
        MCP2515_Sent_Cnt++;//电池箱编号计数 

        if (MCP2515_Life_Cnt > 0xFF)//消息计算最大255
        {
            MCP2515_Life_Cnt = 0;
        }
        
        if (0x01 == MCP2515_ID_Change_Flag)//仪表ID切换 
        {
            MCP2515_ID_Change_Flag = 0;     
            Meter_ID_Change();             
        }             
        //选择对应的电池箱编号，将数据发送到车身仪表
        if (0x00 == MCP2515_ID_Change_Flag)
        {
            Device_Data_Conversion(MCP2515_Sent_Cnt, MCP2515_Life_Cnt);
            //电池箱编号数据判断
            if(MCP2515_Sent_Cnt == Cnt_Num)
            {
                MCP2515_Sent_Cnt = 0;  
                MCP2515_ID_Change_Flag = 0; 
            } 
        }
        /**********************************************/
        //依据客户协议，修改发送版本报文周期时间(10s)
        //20191108
        //ahren
        /**********************************************/ 
        Ten_Seconds_Cnt++;
        if (0x0A == Ten_Seconds_Cnt)
        {
            Ten_Seconds_Cnt = 0;
            MCP2515_ID_Change_Flag = 1; 

            Version_ID_Change();//版本发送ID切换 

            Version_MCP2515_Sent(Version_Data_Normal, 8);                              
        }    
        /**********************************************/           
    } 
#endif  
}

//*******************************************/
//函数名:MCP2515_Data_Analysis
//功  能:发送数据解析
//参  数:无
//返回值:无
//时  间:20190819
//创建者:ahren
/*******************************************/
void MCP2515_Data_Analysis(u8 Num_Cnt, u8 *Data_Buff, u8 Life_Cnt)
{
    if (0x00 == Data_Buff[1])//探测器系统正常
    {
        Data_Buff[0] = Num_Cnt;//电池箱编号  
        Data_Buff[1] = 0x00;//灭火器系统正常
        Data_Buff[2] = 0x00;//无故障码
        Data_Buff[3] = 0xFF;//预留
        Data_Buff[4] = 0xFF;//预留
        Data_Buff[5] = 0xFF;//预留
        Data_Buff[6] = 0xFF;//预留
        Data_Buff[7] = Life_Cnt;//心跳包   
    }  
    else if (0x02 == Data_Buff[1])//探测器系统二级预警
    {
        Data_Buff[0] = Num_Cnt;//电池箱编号
        Data_Buff[1] = 0x01;//预警级别  
        Data_Buff[2] = 0x00;//无故障码
        Data_Buff[3] = 0xFF;//预留
        Data_Buff[4] = 0xFF;//预留
        Data_Buff[5] = 0xFF;//预留
        Data_Buff[6] = 0xFF;//预留
        Data_Buff[7] = Life_Cnt;//心跳包         
    } 
    else if (0x03 == Data_Buff[1])//探测器系统三级预警
    {
        Data_Buff[0] = Num_Cnt;//电池箱编号
        Data_Buff[1] = 0x02;//预警级别 
        Data_Buff[2] = 0x00;//无故障码
        Data_Buff[3] = 0xFF;//预留
        Data_Buff[4] = 0xFF;//预留
        Data_Buff[5] = 0xFF;//预留
        Data_Buff[6] = 0xFF;//预留
        Data_Buff[7] = Life_Cnt;//心跳包         
    }  
    else if (0x04 == Data_Buff[1])//探测器系统四级预警
    {
        Data_Buff[0] = Num_Cnt;//电池箱编号
        Data_Buff[1] = 0x03;//预警级别  
        Data_Buff[2] = 0x02;//探测器已启动
        Data_Buff[3] = 0xFF;//预留
        Data_Buff[4] = 0xFF;//预留
        Data_Buff[5] = 0xFF;//预留
        Data_Buff[6] = 0xFF;//预留
        Data_Buff[7] = Life_Cnt;//心跳包
    }   
    else if (0xFF == Data_Buff[1])//探测器系统故障
    {
        if(0x90 == Data_Buff[7])
        {
            Data_Buff[1] = 0x10;//通讯故障         
        }
        else if(0x88 == Data_Buff[7])
        {
            Data_Buff[1] = 0x18;//传感器故障   
        }
        Data_Buff[0] = Num_Cnt;//电池箱编号 
        Data_Buff[2] = 0x00;//
        Data_Buff[3] = 0xFF;//预留
        Data_Buff[4] = 0xFF;//预留
        Data_Buff[5] = 0xFF;//预留
        Data_Buff[6] = 0xFF;//预留
        Data_Buff[7] = Life_Cnt;//心跳包
    }
    else
    {

    }
}


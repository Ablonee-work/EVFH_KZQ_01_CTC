
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


/***************头文件****************/
/**/
/*************************************/
#include"canapp.h"
//******************宏定义*************/
/**/
/*************************************/


/*************************************/

/***************标志位申明************/
/**/
u8 TC04_CAN_Data_Buff[9] = {0};//缓存TC04 CAN数据

u8 TC04_One_Buff[9]     = {0};//1号电池箱CAN数据
u8 TC04_Two_Buff[9]     = {0};//2号电池箱CAN数据
u8 TC04_Three_Buff[9]   = {0};//3号电池箱CAN数据
u8 TC04_Four_Buff[9]    = {0};//4号电池箱CAN数据
u8 TC04_Five_Buff[9]    = {0};//5号电池箱CAN数据
u8 TC04_Six_Buff[9]     = {0};//6号电池箱CAN数据
u8 TC04_Seven_Buff[9]   = {0};//7号电池箱CAN数据
u8 TC04_Eight_Buff[9]   = {0};//8号电池箱CAN数据
u8 TC04_Nine_Buff[9]    = {0};//9号电池箱CAN数据
u8 TC04_Ten_Buff[9]     = {0};//10号电池箱CAN数据

u8 TC04_ID_Num[16]      = {0};//ID数据存储
u8 TC04_ID_Cnt[16]      = {0};//ID统计

u8 CAN_Data_Normal[9]     = {00,00,0xFF,0xFF,0xFF,0xFF,0xFF,00,00};//TJA1042正常数据报文
u8 TBOX_CAN_Data_Normal[9]     = {0x01,0x00,0x02,0x00,0x13,0x08,0x13,0xFF,00};//软硬件版本信息
u8 TC04_01_11_Buff[9]   = {0};//设备未设置ID时使用，只能使用一台
u8 NULL_Buff[9] = {0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF};
u8 TC04_Off_Line_Val_Buff[16] = 0;//离线标志位
u8 TC04_Off_Line_Cnt_Buff[16] = 0;//离线标志位

u8 TBOX_Error_Buff_Flag[10] = {0x00};
u8 TBOX_Error_Buff[10] = {0x00};

u8 Time_5ms_Flag_g = 0;
u8 Sent_Flag = 0;
u8 Sent_Cnt  = 0;
u8 Error_Flag = 0;
u8 Error_Cnt = 0;
u8 Cnt_Num = 0;

u8 TBOX_3Time_Flag = 0;
u8 TBOX_3Time_Cnt_Flag  = 0;
u8 TBOX_3Time_Sent_Cnt = 0;
u8 TBOX_3Time_Cnt = 0;
/*************************************/

/***************函数申明**************/
/**/
void TC04_Num_Set(void);
void Equipment_Off_Line_Det(void);
u8 CAN_Sent_Time_Choice(void);
void TBOX_MCP2515_Sent(u8 BOX_Num, u8 *CAN_TX_Buf, u8 len);
/*************************************/

/*************************************/

//*******************************************/
//函数名:TC04_CAN_Data_Analysis
//功  能:CAN数据解析
//参  数:无
//返回值:无
//时  间:20190605
//创建者:ahren
/*******************************************/
void TC04_CAN_Data_Analysis(u8 ID,u8 *Data_Buff_New)
{
    u8 i = 0;
    u8 Box_Num_ID = 0;

    Box_Num_ID = ID;

    //将获取到的CAN 报文数据赋值给各个探测器缓存数组
    for(i=0; i<8; i++)
    {
        Data_Buff_New[i] = TC04_CAN_Data_Buff[i];
    
        //Data_Buff_New[0] = Box_Num_ID;//电池箱编号赋值给第一个数组
    }
    Data_Buff_New[0] = Box_Num_ID;//电池箱编号赋值给第一个数组
    //Data_Buff_New[6] = 0xFF;   
}

//*******************************************/
//函数名:Get_TC04_CAN_Data
//功  能:获取探测器CAN数据
//参  数:无
//返回值:无
//时  间:20190604
//创建者:ahren
/*******************************************/
void Get_TC04_CAN_Data(void)
{
    u8 BOX_ID_Num = 0x00;//电池箱编号

    //只接收CAN ID为 0x18FF01XX  的探测器数据 
    if ((0xC7 == RXB0SIDH) && (0xFB == RXB0SIDL) && (0x01 == RXB0EIDH))
    {
        CAN_Receive_Interrupt_Data(CAN_Receive_Data);//读取CAN报文数据
        
        BOX_ID_Num = RXB0EIDL;//获取电池箱编号
#if 0
        /*************************************************************/
        //屏蔽预警监测、传感器故障检测
        //20190802
        //ahren
        if((0x02 == CAN_Receive_Data[1]) || (0x03 == CAN_Receive_Data[1]) || (0x04 == CAN_Receive_Data[1]) || (0x88 == CAN_Receive_Data[7]))
        {
            CAN_Receive_Data[0] = 0x00;
            CAN_Receive_Data[1] = 0x00;
            CAN_Receive_Data[7] = 0x00;            
        }
#endif
        //将 CAN_Receive_Data 复制给 TC04_CAN_Data_Buff        
        memcpy(TC04_CAN_Data_Buff, CAN_Receive_Data, 8); 
        
        //判断选择存储电池箱的数据
        switch (BOX_ID_Num)
        {
            case 0x01:
                TC04_CAN_Data_Analysis(BOX_ID_Num,TC04_One_Buff);   //获取1号电池箱报文数据
                break;
            case 0x02: 
                TC04_CAN_Data_Analysis(BOX_ID_Num,TC04_Two_Buff);   //获取2号电池箱报文数据     
                break;
            case 0x03: 
                TC04_CAN_Data_Analysis(BOX_ID_Num,TC04_Three_Buff); //获取3号电池箱报文数据      
                break;
            case 0x04:
                TC04_CAN_Data_Analysis(BOX_ID_Num,TC04_Four_Buff);  //获取4号电池箱报文数据        
                break; 
            case 0x05:
                TC04_CAN_Data_Analysis(BOX_ID_Num,TC04_Five_Buff);  //获取5号电池箱报文数据          
                break;
            case 0x06:
                TC04_CAN_Data_Analysis(BOX_ID_Num,TC04_Six_Buff);   //获取6号电池箱报文数据       
            break;      
            case 0x07: 
                TC04_CAN_Data_Analysis(BOX_ID_Num,TC04_Seven_Buff);//获取7号电池箱报文数据      
                break;
            case 0x08:
                TC04_CAN_Data_Analysis(BOX_ID_Num,TC04_Eight_Buff); //获取8号电池箱报文数据        
                break; 
            case 0x09:
                TC04_CAN_Data_Analysis(BOX_ID_Num,TC04_Nine_Buff);//获取9号电池箱报文数据          
                break;
            case 0x0A:
                TC04_CAN_Data_Analysis(BOX_ID_Num,TC04_Ten_Buff);//获取10号电池箱报文数据       
            break;            
            case 0x1D:
                TC04_CAN_Data_Analysis(BOX_ID_Num,TC04_01_11_Buff); //获取6号电池箱报文数据 
                memcpy(TC04_One_Buff, TC04_01_11_Buff, 8);          //将 TC04_01_11_Buff 复制给TC04_One_Buff                   
            break;

            default:
                break;
        }	
    }
    else if((0xF6 == RXB0EIDL) && (0xF5 == RXB0EIDH)) //设置电池箱数量ID：0x0402F5F6 
    {
        TC04_Num_Set(); //获取电池箱数量           
    }
    else if ((0xC7 == RXB0SIDH) && (0xFB == RXB0SIDL) && (0x5E == RXB0EIDH))
    {
        //添加主机发送报文处理机制
        //20190902
        //ahren
        CAN_Receive_Interrupt_Data(CAN_Receive_Data);               //读取CAN报文数据
        //memset(CAN_Receive_Data, 0, 8); 
    }
    else
    {
        //收到非指定ID 的报文时，清楚相关标志位
        //20190902
        //ahren
        RXFUL=0;//打开接收缓冲器来接收新信息    
    }
}

//*******************************************/
//函数名:TC04_Off_Line
//功  能：分析设备是否离线
//参  数:BOX_Num,Data_Buff,Life_Signal_Num
//返回值:Off_Line_Val
//时  间:20190605
//创建者:ahren
/*******************************************/
u8 TC04_Off_Line_Det(u8 BOX_Num, u8 *Data_Buff, u8 Life_Signal_Num)
{
    u8 Off_Line_Val = 0;
    u8 i = 0;
    //判断心跳包数据是否与前一次一致
    if (TC04_Off_Line_Val_Buff[BOX_Num] == Data_Buff[Life_Signal_Num])
    {
        //将心跳相同的计数次数赋值给Off_Line_Val
        Off_Line_Val = TC04_Off_Line_Cnt_Buff[BOX_Num];

        Off_Line_Val++;//离线自加计数

        //将自加后的值赋值给对应计数的Buff中         
        TC04_Off_Line_Cnt_Buff[BOX_Num] = Off_Line_Val;
    }
    else
    {
        //将最新的心跳包数据赋值给对应的心跳数据的Buff中
        TC04_Off_Line_Val_Buff[BOX_Num] = Data_Buff[Life_Signal_Num];
        
        TC04_Off_Line_Cnt_Buff[BOX_Num] = 0; //将心跳计数清零                           
    }
    //离线故障赋值
    if (Off_Line_Val >= 0x02)
    {
        Data_Buff[1] = 0xFF;
        Data_Buff[7] = 0x90;//依据输入输出表CAN报文格式而定义        
        //上电未接探测器检测检测
        if((0x00 != Data_Buff[1]) || (0x00 != Data_Buff[7]))
        {
            if(Data_Buff[0] == 0)//未收到任何探测器数据
            {
                TC04_ID_Num[BOX_Num] = BOX_Num;//将电池箱编号赋值给ID数组
                
                for(i=1; i<6; i++)
                {
                    Data_Buff[i] = 0xFF;//补充数组1-5的数据，均为0xFF                   
                }          
            }
        }      
    }
    return Off_Line_Val;
}

//*******************************************/
//函数名:Sent_TC04_CAN_Data
//功  能:获取CAN接收数据
//参  数:无
//返回值:无
//时  间:20190605
//创建者:ahren
/*******************************************/
void Sent_TC04_CAN_Data(void)
{
    static u8 Cnt = 0;
    static uint16 Life_Signal_Cnt = 0;
    u8 ID_Cnt = 0;
    u8 Sent_Time_Flag = 0;
    u8 i = 0;
    u8 j = 0;
    
    u8 TJA1042_Sent_Time_Flag = 0;
    static u8 TJA1042_Sent_Time_Cnt = 0;

    u8 MCP2515_Sent_Time_Flag = 0;
    u8 MCP2515_One_Sec_Time_Flag = 0;
    static u8 MCP2515_One_Min_Flag = 0;
    static u8 MCP2515_Sent_Cnt = 0;
    static u8 MCP2515_One_Min_Cnt = 0;
    static uint16 MCP2515_Life_Cnt = 0;
    
    /*************************************************/
    //每隔5ms x N = 5Nms 更新每个探测器的数组数据
    //N为探测器数量
    //ahren
    //20190716
    CAN_Data_Choice_Analysis();
    /*************************************************/
    
    /*************************************************/
    //获取CAN发送周期时间返回值
    //ahren
    //20190719   
    Sent_Time_Flag = CAN_Sent_Time_Choice();
    /*************************************************/
    
    /*************************************************/
    //确保TJA1042发送的周期一直为1S
    //ahren
    //20190731 
    if (Sent_Time_Flag == 0x03)//1S返回值
    {
        Sent_Time_Flag = 0;
        TJA1042_Sent_Time_Flag = 1;
        MCP2515_Sent_Time_Flag = 1;//MCP2515发送标志位置1
    }
    else if (Sent_Time_Flag == 0x02)//500ms返回值
    {
        Sent_Time_Flag = 0;
        MCP2515_Sent_Time_Flag = 1;//MCP2515发送标志位置1
        TJA1042_Sent_Time_Cnt++;
        if (0x02 == TJA1042_Sent_Time_Cnt)
        {
            TJA1042_Sent_Time_Cnt = 0;
            TJA1042_Sent_Time_Flag = 1;            
        }      
    }
    else if (Sent_Time_Flag == 0x01)//250ms返回值
    {
        Sent_Time_Flag = 0;
        TJA1042_Sent_Time_Cnt++;
        MCP2515_Sent_Time_Flag = 1;//MCP2515发送标志位置1        
        if (0x04 == TJA1042_Sent_Time_Cnt)
        {
            TJA1042_Sent_Time_Cnt = 0;
            TJA1042_Sent_Time_Flag = 1;
            MCP2515_One_Sec_Time_Flag = 1;            
        }      
    }    
    /*************************************************/    
  
    /*************************************************/     
    if(0x01 == TJA1042_Sent_Time_Flag)//基准时间Time1  1s     
    {
        //MCP_CAN_Send_1s_Flag_g = 0;
        TJA1042_Sent_Time_Flag = 0;
        //CAN心跳包数据
        Life_Signal_Cnt++;  
        MCP2515_Life_Cnt++;  
        if(Life_Signal_Cnt > 0xFF)
        {
            Life_Signal_Cnt = 0;  
            MCP2515_Life_Cnt = 0;         
        }
#if 0        
        MCP2515_CAN_Tx_Buffer(0, CAN_Data_Normal, 8, Life_Signal_Cnt); //CAN2 2515发送缓存数据
        TJA1042_CAN_Send_Data(0, CAN_Data_Normal, 8, Life_Signal_Cnt);//通过TJA1042发送报文给主机             
#endif
        
#if 1
        /*************************************************/
        //每隔1s x N = Ns 检测一次探测器是否离线
        //N为探测器数量
        //ahren
        //20190717      
        Equipment_Off_Line_Det();
        /*************************************************/        
        if(Error_Flag == 0)
        {
            if ((Cnt_Num != 0) && (Cnt_Num != 0xFF))
            {
                for(i=1; i<=Cnt_Num; i++)
                {
                    if(TC04_ID_Num[i] != 0)
                    {
                        //将异常电池箱编号复制给TC04_ID_Cnt数组
                        TC04_ID_Cnt[ID_Cnt] = TC04_ID_Num[i];
                        Error_Flag = 1;//异常数据标志位置1
                        Sent_Cnt = 0;//异常数据发送次数清零
                        ID_Cnt++;//判断下一个异常数据
                    }
                }            
            }
            Error_Cnt = strlen(TC04_ID_Cnt);//检测数组不为0的数量
        }
        if(Error_Cnt != 0)//出现异常数据
        {
            if(Sent_Cnt <= Error_Cnt)//判断轮询发送的次数是否达到
            {
                Sent_Flag = TC04_ID_Cnt[Sent_Cnt];//将电池箱编号赋值给Sent_Flag
                Sent_Cnt++;               
            }
            if(Sent_Cnt == Error_Cnt)//异常数据轮询结束
            {
                memset(TC04_ID_Cnt, 0, 16); // 将 TC04_ID_Cnt 数组清零
                //memset(TBOX_Error_Buff, 0, 10); // 将 TC04_ID_Cnt 数组清零
                Error_Cnt = 0;//异常数据统计清零
                Error_Flag = 0; //异常数据标志位清零
            } 
#if 0            
            NOP();NOP();NOP();NOP();NOP();NOP(); 
            MCP2515_TxID[3] = 0xA5;   
            MCP2515_CAN_TxID(MCP2515_TxID,8);//发送ID设置 
#endif            
            switch (Sent_Flag)//判断是哪个电池箱编号
            {
                case 0x01:
                    TJA1042_CAN_Send_Data(Sent_Flag, TC04_One_Buff, 8, Life_Signal_Cnt);//通过TJA1042发送报文给主机 
                    //MCP2515_CAN_Tx_Buffer(Sent_Flag, TC04_One_Buff, 8, MCP2515_Life_Cnt);                     
                    break;
                case 0x02:
                    TJA1042_CAN_Send_Data(Sent_Flag, TC04_Two_Buff, 8, Life_Signal_Cnt);//通过TJA1042发送报文给主机
                    //MCP2515_CAN_Tx_Buffer(Sent_Flag, TC04_Two_Buff, 8, MCP2515_Life_Cnt);  
                    break;
                case 0x03:
                    TJA1042_CAN_Send_Data(Sent_Flag, TC04_Three_Buff, 8, Life_Signal_Cnt); //通过TJA1042发送报文给主机
                    //MCP2515_CAN_Tx_Buffer(Sent_Flag, TC04_Three_Buff, 8, MCP2515_Life_Cnt);  
                    break;
                case 0x04:
                    TJA1042_CAN_Send_Data(Sent_Flag, TC04_Four_Buff, 8, Life_Signal_Cnt);//通过TJA1042发送报文给主机
                    //MCP2515_CAN_Tx_Buffer(Sent_Flag, TC04_Four_Buff, 8, MCP2515_Life_Cnt);
                    break;
                case 0x05:
                    TJA1042_CAN_Send_Data(Sent_Flag, TC04_Five_Buff, 8, Life_Signal_Cnt);//通过TJA1042发送报文给主机 
                    //MCP2515_CAN_Tx_Buffer(Sent_Flag, TC04_Five_Buff, 8, MCP2515_Life_Cnt);                      
                    break;   
                case 0x06:
                    TJA1042_CAN_Send_Data(Sent_Flag, TC04_Six_Buff, 8, Life_Signal_Cnt);//通过TJA1042发送报文给主机
                    //MCP2515_CAN_Tx_Buffer(Sent_Flag, TC04_Six_Buff, 8, MCP2515_Life_Cnt);                      
                    break;                 
                case 0x07:
                    TJA1042_CAN_Send_Data(Sent_Flag, TC04_Seven_Buff, 8, Life_Signal_Cnt); //通过TJA1042发送报文给主机 
                    //MCP2515_CAN_Tx_Buffer(Sent_Flag, TC04_Seven_Buff, 8, MCP2515_Life_Cnt);                      
                    break;
                case 0x08:
                    TJA1042_CAN_Send_Data(Sent_Flag, TC04_Eight_Buff, 8, Life_Signal_Cnt);//通过TJA1042发送报文给主机  
                    //MCP2515_CAN_Tx_Buffer(Sent_Flag, TC04_Eight_Buff, 8, MCP2515_Life_Cnt);                      
                    break;
                case 0x09:
                    TJA1042_CAN_Send_Data(Sent_Flag, TC04_Nine_Buff, 8, Life_Signal_Cnt);//通过TJA1042发送报文给主机
                    //MCP2515_CAN_Tx_Buffer(Sent_Flag, TC04_Nine_Buff, 8, MCP2515_Life_Cnt);                      
                    break;   
                case 0x0A:
                    TJA1042_CAN_Send_Data(Sent_Flag, TC04_Ten_Buff, 8, Life_Signal_Cnt);//通过TJA1042发送报文给主机
                    //MCP2515_CAN_Tx_Buffer(Sent_Flag, TC04_Ten_Buff, 8, MCP2515_Life_Cnt);                      
                    break;
                default:
                    break;
            }        
        }
        else
        {
            /***********************************************************/
            //依据宇通最新CAN报文格式，探测器正常发送统一格式的报文
            //20190711
            //ahren  
            /***********************************************************/  
            if ((Cnt_Num != 0) && (Cnt_Num != 0xFF))
            {
                //MCP2515_CAN_Tx_Buffer(0, CAN_Data_Normal, 8, Life_Signal_Cnt); //CAN2 2515发送缓存数据
                TJA1042_CAN_Send_Data(0, CAN_Data_Normal, 8, Life_Signal_Cnt);//通过TJA1042发送报文给主机 
            }
        }
#endif
    }
    //MCP2515 发送数据解析与处理
    if ((0x01 == MCP2515_Sent_Time_Flag))
    {
        MCP2515_Sent_Time_Flag = 0;
        MCP2515_One_Min_Cnt++;
        if (0x0A == MCP2515_One_Min_Cnt)
        {
            MCP2515_One_Min_Cnt = 0;
            MCP2515_One_Min_Flag = 1;
            MCP2515_TxID[1] = 0xFE;   
            MCP2515_TxID[2] = 0xDA;     
            MCP2515_CAN_TxID(MCP2515_TxID,8);//发送ID设置 
            //NOP();NOP();NOP();NOP();NOP();NOP(); 
            TBOX_MCP2515_Sent(0, TBOX_CAN_Data_Normal, 8);            
        }
        else
        {
            MCP2515_Sent_Cnt++;//电池箱编号计数 
            if (0x01 == MCP2515_One_Min_Flag)
            {
                MCP2515_One_Min_Flag = 0;         
                MCP2515_TxID[1] = 0xFF;   
                MCP2515_TxID[2] = 0x30;                  
                MCP2515_CAN_TxID(MCP2515_TxID,8);//发送ID设置 
                //NOP();NOP();NOP();NOP();NOP();NOP();                
            } 
        } 
#if 1
        //选择对应的电池箱编号，将数据发送到车身仪表
        if (0x00 == MCP2515_One_Min_Flag)
        {
            switch (MCP2515_Sent_Cnt)
            {
                case 0x01:
                    MCP2515_CAN_Tx_Buffer(MCP2515_Sent_Cnt, TC04_One_Buff, 8, MCP2515_Life_Cnt);                          
                    break;
                case 0x02:
                    MCP2515_CAN_Tx_Buffer(MCP2515_Sent_Cnt, TC04_Two_Buff, 8, MCP2515_Life_Cnt); 
                    break;
                case 0x03:
                    MCP2515_CAN_Tx_Buffer(MCP2515_Sent_Cnt, TC04_Three_Buff, 8, MCP2515_Life_Cnt);
                    break;
                case 0x04:
                    MCP2515_CAN_Tx_Buffer(MCP2515_Sent_Cnt, TC04_Four_Buff, 8, MCP2515_Life_Cnt); 
                    break;
                case 0x05:
                    MCP2515_CAN_Tx_Buffer(MCP2515_Sent_Cnt, TC04_Five_Buff, 8, MCP2515_Life_Cnt); 
                    break;   
                case 0x06:
                    MCP2515_CAN_Tx_Buffer(MCP2515_Sent_Cnt, TC04_Six_Buff, 8, MCP2515_Life_Cnt); 
                    break;                 
                case 0x07:
                    MCP2515_CAN_Tx_Buffer(MCP2515_Sent_Cnt, TC04_Seven_Buff, 8, MCP2515_Life_Cnt);  
                    break;
                case 0x08:
                    MCP2515_CAN_Tx_Buffer(MCP2515_Sent_Cnt, TC04_Eight_Buff, 8, MCP2515_Life_Cnt);  
                    break;
                case 0x09:
                    MCP2515_CAN_Tx_Buffer(MCP2515_Sent_Cnt, TC04_Nine_Buff, 8, MCP2515_Life_Cnt);  
                    break;   
                case 0x0A:
                    MCP2515_CAN_Tx_Buffer(MCP2515_Sent_Cnt, TC04_Ten_Buff, 8, MCP2515_Life_Cnt);  
                    break;
                default:
                    break;
            }
            //电池箱编号数据判断
            if(MCP2515_Sent_Cnt == Cnt_Num)
            {
                MCP2515_Sent_Cnt = 0;  
                MCP2515_One_Min_Flag = 0; 
            } 
        }    
#endif
    } 
}

//*******************************************/
//函数名:Get_CAN_Data
//功  能:获取CAN接收数据
//参  数:无
//返回值:无
//时  间:20180605
//创建者:ahren
/*******************************************/
void Get_CAN_Data(void)
{    
	if(1 == CAN_RXD_Interrupt_Flag_g)//CAN产生接收中断
	{			
		CAN_RXD_Interrupt_Flag_g = 0;	

        /****************************************/	
        //判断接收探测器 CAN报文数据                         
        //ahren
        //20190604
        /*************************************/	        
        Get_TC04_CAN_Data();
	}     
}

//函数名:TC04_Num_Set
//功  能:设置电池箱编号
//参  数:无
//返回值:无
//时  间:20180611
//创建者:ahren
/*******************************************/
void TC04_Num_Set(void)
{
    u8 TC04_Num = 0;
    u8 TC04_Time_Val = 0;
   
    //通过CAN设置设备数量
    CAN_Receive_Interrupt_Data(CAN_Receive_Data);                   //读取CAN报文数据
    //将 CAN_Receive_Data 复制给 CAN_TX0_Buff        
    memcpy(CAN_TX0_Buff, CAN_Receive_Data, 8);     
    
    if((CAN_Receive_Data[0] == 0x23) && (CAN_Receive_Data[7] == 0X16))  // 23 xx 00 00 00 00 00 16
    {
        if((CAN_Receive_Data[1] == 0x01))
        {
            /*********************************************/
            //添加通过CAN获取电池箱数量功能
            //20190727
            //ahren            
            if ((0x00 == CAN_Receive_Data[2]) && (0x01 == CAN_Receive_Data[3]))
            {
                CAN_TX0_Buff[2] = eeprom_read(0x03);//从EEPROM获取电池箱数量
                NOP();NOP();NOP();NOP(); 
            }
            /*********************************************/            
            else
            {
                TC04_Num =  CAN_Receive_Data[2]; //获取设置电池箱数量 

                eeprom_write(0x03,TC04_Num); //将电池箱数量写入内容EEPROM里面
                NOP();NOP();NOP();NOP();
                
                eeprom_write(0x0A,0xA5);//电池箱默认数量被修改
                NOP();NOP();NOP();NOP();
                
                CAN_TX0_Buff[3] = eeprom_read(0x03);//从EEPROM获取电池箱数量
                NOP();NOP();NOP();NOP(); 
                
                CAN_TX0_Buff[2] = 0x00;                 
            } 
            CAN_TX0_Buff[0] = 0x23;
            CAN_TX0_Buff[7] = 0x16;

            CAN_Send_8Byte_Data();//通过TJA1042 发送确认
            delay_10ms(1);  
            CAN_STB_OFF;//设置或读取后需重新上电        
        }
        else if((CAN_Receive_Data[1]==0x02))
        {
            TC04_Time_Val =  CAN_Receive_Data[2];                        

            eeprom_write(0x08,TC04_Time_Val);                           
            NOP();NOP();NOP();NOP();

            CAN_TX0_Buff[3] = eeprom_read(0x08);                      
            NOP();NOP();NOP();NOP();  
            
            CAN_TX0_Buff[2] = 0x00;


            CAN_TX0_Buff[0] = 0x23;
            CAN_TX0_Buff[7] = 0x16;

            CAN_Send_8Byte_Data();//通过TJA1042 发送确认
            delay_10ms(1);  
            CAN_STB_OFF;//设置或读取后需重新上电              
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
            eeprom_write(0x09,0x5A);//CAN默认ID被修改标志位
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

            CAN_Send_8Byte_Data();//通过TJA1042 发送确认
            delay_10ms(1);  
            CAN_STB_OFF;//设置或读取后需重新上电                            
        }      
    }
}
//*******************************************/
//函数名:CAN_Data_Analysis_Det
//功  能:探测器异常数据解析
//参  数:无
//返回值:无
//时  间:20190715
//创建者:ahren
/*******************************************/
void CAN_Data_Analysis_Det(u8 Num,u8 *Data_Buff)
{
    //探测器CAN报文第1个字节或第7个字节不为0
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
//函数名:CAN_Data_Choice_Analysis
//功  能:探测器can 数据分析
//参  数:无
//返回值:无
//时  间:20190715
//创建者:ahren
/*******************************************/
void CAN_Data_Choice_Analysis(void)
{
    static u8 Cnt = 0;
    u8 Cnt_Num_Flag = 0;
    
    //获取电池箱数量
    if ((Cnt_Num == 0) || (Cnt_Num == 0xFF))
    {
        Cnt_Num_Flag = eeprom_read(0x0A);//判断默认电池箱数量是否被修改
        NOP();NOP();NOP();NOP();    
        /**********************************/        
        //添加默认电池数量为6台
        //20190724
        //ahren
        /**********************************/            
        if((Cnt_Num_Flag == 0) || (Cnt_Num_Flag == 0xFF))
        {
            eeprom_write(0x03,0x06);//将电池箱数量写入内容EEPROM里面
            NOP();NOP();NOP();NOP();
            Cnt_Num = eeprom_read(0x03);//将电池箱数量从EEPROM读出
            NOP();NOP();NOP();NOP();        
        }
        else if(Cnt_Num_Flag == 0xA5)
        {
            Cnt_Num = eeprom_read(0x03);//将电池箱数量从EEPROM读出
            NOP();NOP();NOP();NOP();         
        }
    }  
    else if(Time_5ms_Flag_g == 1)//5ms定时
    {
        Time_5ms_Flag_g = 0;
        Cnt++;//数据更新计数自加
        //if(Cnt > 0x06)//数据更新一轮结束
        if(Cnt > Cnt_Num)
        {
            Cnt = 1;//数据从第一个开始
        }        
    }    
    switch(Cnt)
    {
        case 0x01:
                CAN_Data_Analysis_Det(Cnt,TC04_One_Buff);//判断第一个探测器数据是否异常
            break;                  
        case 0x02:
                CAN_Data_Analysis_Det(Cnt,TC04_Two_Buff);//判断第二个探测器数据是否异常
            break; 
        case 0x03:
                CAN_Data_Analysis_Det(Cnt,TC04_Three_Buff);//判断第三个探测器数据是否异常
            break;                  
        case 0x04:
                CAN_Data_Analysis_Det(Cnt,TC04_Four_Buff);//判断第四个探测器数据是否异常
            break;
        case 0x05:
                CAN_Data_Analysis_Det(Cnt,TC04_Five_Buff);//判断第五个探测器数据是否异常
            break;                  
        case 0x06:
                CAN_Data_Analysis_Det(Cnt,TC04_Six_Buff);//判断第六个探测器数据是否异常
            break;
        case 0x07:
                CAN_Data_Analysis_Det(Cnt,TC04_Seven_Buff);//判断第七个探测器数据是否异常
            break;                  
        case 0x08:
                CAN_Data_Analysis_Det(Cnt,TC04_Eight_Buff);//判断第八个探测器数据是否异常
            break;
        case 0x09:
                CAN_Data_Analysis_Det(Cnt,TC04_Nine_Buff);//判断第九个探测器数据是否异常
            break;                  
        case 0x0A:
                CAN_Data_Analysis_Det(Cnt,TC04_Ten_Buff);//判断第十个探测器数据是否异常
            break;
        default:
            break;    
    }
}

//*******************************************/
//函数名:Equipment_Off_Line_Det
//功  能:探测器离线检测
//参  数:无
//返回值:无
//时  间:20190716
//创建者:ahren
/*******************************************/
void Equipment_Off_Line_Det(void)
{
    static u8 Cnt = 0;

    if ((Cnt_Num != 0) && (Cnt_Num != 0xFF))
    //if(1)
    {
        Cnt++;

        if(Cnt > Cnt_Num)
        //if(Cnt > 0x06)//判断轮询次数
        {
            Cnt = 1;
        }     
    }      
    
    switch(Cnt)
    {
        case 0x01:
                TC04_Off_Line_Det(Cnt, TC04_One_Buff, 6);//检测是否离线
            break;                  
        case 0x02:
                TC04_Off_Line_Det(Cnt, TC04_Two_Buff, 6);//检测是否离线
            break; 
        case 0x03:
                TC04_Off_Line_Det(Cnt, TC04_Three_Buff, 6);//检测是否离线
            break;                  
        case 0x04:
                TC04_Off_Line_Det(Cnt, TC04_Four_Buff, 6);//检测是否离线
            break;
        case 0x05:
                TC04_Off_Line_Det(Cnt, TC04_Five_Buff, 6);//检测是否离线
            break;                  
        case 0x06:
                TC04_Off_Line_Det(Cnt, TC04_Six_Buff, 6);//检测是否离线
            break;
        case 0x07: 
                TC04_Off_Line_Det(Cnt,TC04_Seven_Buff, 6);//获取7号电池箱报文数据      
            break;
        case 0x08:
                TC04_Off_Line_Det(Cnt,TC04_Eight_Buff, 6); //获取8号电池箱报文数据        
            break; 
        case 0x09:
                TC04_Off_Line_Det(Cnt,TC04_Nine_Buff, 6);//获取9号电池箱报文数据          
            break;
        case 0x0A:
                TC04_Off_Line_Det(Cnt,TC04_Ten_Buff, 6);//获取10号电池箱报文数据          
            break;            
        default:
            break;    
    }
}

//*******************************************/
//函数名:CAN_Sent_Time_Choice
//功  能:设置CAN 发送周期
//参  数:无
//返回值:Ret_Val
//时  间:20190719
//创建者:ahren
/*******************************************/
u8 CAN_Sent_Time_Choice(void)
{
    static u8 Time_Val = 0;
    u8 Ret_Val  = 0;
    
    if(0x00 == Time_Val)
    {
        eeprom_write(0x08,0x03);                           
        NOP();NOP();NOP();NOP();

        Time_Val = eeprom_read(0x08);//将CAN发送周期从EEPROM中读取选择
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
#if 1
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
        TBOX_CAN_Data_Normal[0] = 0x00;//TBOX 无故障代码      
        TBOX_Error_Buff[0] = 0;
        TBOX_Error_Buff[1] = 0;
        TBOX_Error_Buff[2] = 0;
    }

#endif 
}

/*************************************************************************/
//函数名:TBOX_MCP2515_Sent
//功  能:发送到TBOX
//参  数:发送数组首地址，数组长度
//返回值：无
//时间：20190803
//创建者：ahren
/************************************************************************/
void TBOX_MCP2515_Sent(u8 BOX_Num, u8 *CAN_TX_Buf, u8 len)
{
	u8 j,dly;
    dly=0;

    while((MCP2515_ReadByte(MCP2515_TXB0CTRL)&0x08) && (dly<50))        //读取状态值，是否可以发送
    {			
        dly++;
    }

    for(j=0;j<len;j++)
    {
        MCP2515_WriteByte(MCP2515_TXB0D0 + j,TBOX_CAN_Data_Normal[j]);            //写入发送数据到发送缓冲区    
    }

    MCP2515_WriteByte(MCP2515_TXB0DLC, len);                            //写入发送的数据长度
    MCP2515_CS = 0;
    MCP2515_WriteByte(MCP2515_TXB0CTRL,0x08);                           //发送报文
    MCP2515_CS = 1;  
}



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

u8 TC04_Buff[TC_NUM_MAX][9] = {0x00};//电池箱CAN数据 [x][y] x:编号，0-15表示探测器1-16号
uint16_t TC04_Off_Time_Cnt_g = 0;//探测器离线检测周期时间
u8 MCP2515_Sent_Time_Flag = 0;//对外发送报文周期时间计数
u8 TJA1042_Sent_Time_Flag = 0;//对内发送报文周期时间计数

u8 TC_Num = 0;//探测器数量
u8 TC04_OffLine_Flag[TC_NUM_MAX] = {0};//离线标志位  TC04_OffLine_Flag[x]:0-15表示探测器1-16编号   数值>3表示离线
u8 TC04_OffLine_LifeCnt_Temp[TC_NUM_MAX] = {0};//心跳包暂存，用于检测离线
u8 TC04_Abnormal_Table[TC_NUM_MAX]          = {0x00};//探测器异常表
u8 Send_TCNum_Cnt = 0;  //对外发送报文当前发送箱号
u8 Outgoing_Data_Cycle = 0; //对外发送报文周期

/*************************************/
//20210222
u8 PowerOn_OfflineDet_Flag = 0;
u8 TC_Start_Flag[TC_NUM_MAX] = {0};  //00:正常  01发送C1命令  02收到C1回复    03发送C3(启动)   04收到C3回复，且已经启动   05分阀关闭完成  06开启后等待关闭
uint16_t TC_C1_R_10Min = 0;//10分钟计时 
u8 TC_Start_Wait_Cnt[TC_NUM_MAX] = {0};
u8 Mamual_Start = 0;

u8 time_3s_cnt = 0;  
u8 TC_Waring_4_Flag[TC_NUM_MAX] = {0};
u8 TC_Sleep_Flag[TC_NUM_MAX] = {0};
u8 Sys_Sleep_Flag = 0;//0正常状态    1：睡眠状态   2：进入睡眠后被报文唤醒  3报文唤醒状态   4重新进入睡眠
u8 Sys_Sleep_Wake_TC = 0;//唤醒主机和探测器标志未，发送唤醒命令3次

//对外报文处理      

                                      //x:0-19  :箱号1-20
u8 Sys_Sleep_Host_cnt = 0;
u8 TC_Code_Err = 0;//0：正常   1：未编码故障  2：恢复正常
u8 TC_Code_Err_Cnt = 0;//未编码故障报文发送次数
u8 TC_Code_Recover_Flag = 0;
u8 TC_Code_Recover_Cnt = 0;
u8 TC_Quantity_Auto[TC_NUM_MAX] = {0};
u8 backupdata_read = 0;
u8 TC_RT_Data[TC_NUM_MAX][8] = {0};
u8 TC_RT_Data_Old[TC_NUM_MAX] = 0;//故障预警信息
u8 Host_C2_Data[8] = {0};
u8 Host_C2_Data_Old[8] = {0};

u8 B3_Data_Buffer[9] = {0};//第0个数据表示箱号，且不为0不接收数据

//20210902-探测器接收数据缓冲区
uint32_t RxMsgBuff_ID[TC_DATA_BUFF]={0};
uint8_t RxMsgBuff_Len[TC_DATA_BUFF]={0};
uint8_t RxMsgBuff_Data[TC_DATA_BUFF][8]={0};
uint8_t RxMsgBuff_Flag[TC_DATA_BUFF]={0};

/***************函数申明**************/
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
void CanToCan_TC_F0(u8 state) //低功耗
{
    u8 data[8] = {0};   
    uint32_t cmd_id = 0x18F0FFF6;
    
    data[0] = state; //0x00 ：正常功耗(Normal)      0x01：低功耗(Sleep)
    data[1] = 0x0;//保留
    data[2] = 0x0;//保留
    data[3] = 0x0;//保留
    data[4] = 0x0;//保留
    data[5] = 0x0;//保留
    data[6] = 0x0;//保留
    data[7] = 0x0;//保留
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

void CanToCan_TC_C1(void)//手动启动命令，周期3秒，收到探测器回复则停止，否则发送10分钟
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

//20210902-解决苏龙报离线问题，分开探测器数据接收和处理流程
void Get_TC04_CAN_Data(void)//接收探测器数据
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
        RXFUL = 0;//打开接收缓冲器来接收新信息
    }
}

void TC04_CAN_Data_Process(void)//探测器数据处理
{
    uint32_t rx_id=0;
    uint8_t rx_len=0;
    uint8_t rx_buf[8]={0};
    uint8_t buftemp[8]={0};
    u8 BOX_ID_Num = 0x00;//电池箱编号
    u8 i,temp,k;
    u8 waring = 0,fault = 0;

    //读取接收的探测器数据
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
                BOX_ID_Num = rx_id&0xFF;//获取电池箱编号

                if((BOX_ID_Num == 0xFF)||(BOX_ID_Num == 0x00)||(BOX_ID_Num > 16))
                {
                    if(TC_Code_Err != 1)
                    {
                        TC_Code_Err = 1;//未编码
                        TC_Code_Err_Cnt = 0;//发送次数清零
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
                            eeprom_write(0x03, TC_Num);//全部在线后，检测到0xFF，则总数加1  
                        }         
                    }         
                }
                else 
                {            
                    memcpy(&TC_RT_Data[BOX_ID_Num-1][0], rx_buf, 8);//备份数据获取           

                    TC_Quantity_Auto[BOX_ID_Num-1] = 1;                  

                    //数据转换，创为探测器数据转换为对应中汽客主机的数据
                    waring = (rx_buf[7]>>4)&0x0F;              
                    fault = rx_buf[7] & 0x0F;
                  
                    if(Sys_Sleep_Flag == 1)//睡眠状态,预警才唤醒
                    {
                        if(waring != 0)Sys_Sleep_Flag = 2;  
                    }

                    memcpy(buftemp, &TC04_Buff[BOX_ID_Num-1][0], 8); 
                    //已经启动
                    if(TC_Waring_4_Flag[BOX_ID_Num-1] == 0xAA)//启动主瓶
                    {
                        buftemp[1] = 0x04;//启动主瓶
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
                                if(TC_Start_Flag[BOX_ID_Num-1] == 0x02)//收到C1回复
                                {
                                    TC_Start_Flag[BOX_ID_Num-1] = 0x03;
                                }                    
                                break;
                            case 0x03:
                                buftemp[1] = 0x03;
                                buftemp[7] = 0x42;
                                if(TC_Start_Flag[BOX_ID_Num-1] == 0x02)//收到C1回复
                                {
                                    TC_Start_Flag[BOX_ID_Num-1] = 0x03;
                                }                     
                                break;
                            case 0x04:
                                //执行自动启动流程
                                TC_Start_Flag[BOX_ID_Num-1] = 0x03;//发送C3命令，自动启动流程，启动探测器子阀           
                                buftemp[1] = 0x03;
                                buftemp[7] = 0x42;                
                                break;
                            default:
                                if(fault == 0x01)//点火头启动，如探测器已经启动过，则算故障
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
                    buftemp[6]++;//心跳包
                    memcpy(&TC04_Buff[BOX_ID_Num-1][0], buftemp, 8);            

                    //自动识别数量-总数增加时
                    if(BOX_ID_Num > TC_Num)
                    {
                        TC_Num = BOX_ID_Num;     
                        eeprom_write(0x03, TC_Num);
                    }        
                    
                }
            }    
            else if((rx_id&0xFFFFFFF00) == 0x18B1F600)   
            {  
                BOX_ID_Num = rx_id&0xFF;//获取电池箱编号
                if((BOX_ID_Num == 0xFF)||(BOX_ID_Num == 0x00)||(BOX_ID_Num > 16))
                {
                    if(TC_Code_Err!=1)
                    {
                        TC_Code_Err = 1;//未编码
                        TC_Code_Err_Cnt = 0;//发送次数清零
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
                            eeprom_write(0x03,TC_Num);//全部在线后，检测到0xFF，则总数加1  
                        }                          
                    }
                }
                else 
                {
                    TC_Quantity_Auto[BOX_ID_Num-1] = 1;
                    TC04_Buff[BOX_ID_Num-1][6]++;//心跳包
                }
            } 
            else if((rx_id&0xFFFFFFF00) == 0x18FF5E00)
            {       
                if((rx_buf[0]==0x24)&&(rx_buf[7]==0x42))
                {
                    if((rx_buf[2]==0xFE)&&(rx_buf[3]==0xEF))//启动
                    {
                        //开启发送手动启动命令流程                
                        if(Mamual_Start == 0)
                        {
                            for(i=0; i<TC_Num; i++)
                            {
                                if(TC_Start_Flag[i] == 0)
                                {
                                    TC_Start_Flag[i] = 0x01;//发送C1命令   
                                    Mamual_Start = 1;
                                    TC_C1_R_10Min = 0;
                                    time_3s_cnt = 3;//立刻发送C1
                                }
                            }                    
                        }  
                    }
                }        
            }
            //接收探测器C1回复
            else if((rx_id&0xFFFFFFF00) == 0x18C1F600)
            {
                BOX_ID_Num = rx_id&0xFF;//获取电池箱编号
                TC04_Buff[BOX_ID_Num-1][6]++;//心跳包

                if(TC_Start_Flag[BOX_ID_Num-1] == 0x01)
                {
                    TC_Start_Flag[BOX_ID_Num-1] = 0x02;//收到C1回复  
                } 
            }

            //接收探测器C3回复-子阀打开回复
            else if((rx_id&0xFFFFFFF00) == 0x18C3F600)
            {
                BOX_ID_Num = rx_id&0xFF;//获取电池箱编号
                TC04_Buff[BOX_ID_Num-1][6]++;//心跳包

                if(TC_Start_Flag[BOX_ID_Num-1] == 0x03)
                {           
                    if((rx_buf[0] == 0x01)) //启动状态或者反馈状态来作为启动总阀的有效信号   ||(CAN_Receive_Data[1] == 0x01)
                    {           
                        TC_Start_Flag[BOX_ID_Num-1] = 0x06;//开启后等待关闭
                        TC_Start_Wait_Cnt[BOX_ID_Num-1] = 0;

                        TC_Waring_4_Flag[BOX_ID_Num-1] = 0xAA;//启动主瓶 
                    }  
                    else if(rx_buf[1] == 0x01)
                    {
                        TC_Waring_4_Flag[BOX_ID_Num-1] = 0xAA;//启动主瓶    
                    }
                }  
                else if(TC_Start_Flag[BOX_ID_Num-1] == 0x04)
                {
                    if(rx_buf[0] == 0x00)
                    {
                        TC_Start_Flag[BOX_ID_Num-1] = 0x05;//关闭完成
                    }          
                }
            }  
            //接收到B3备份指令后，回复备份指令
            else if((rx_id&0xFFFFFFF00) == 0x18B3F600)
            {
                BOX_ID_Num = rx_id&0xFF;//获取电池箱编号   
                if((BOX_ID_Num <= 16) && (BOX_ID_Num >= 1))
                {           
                    TC04_Buff[BOX_ID_Num-1][6]++;//心跳包

                    CanToCan_TC_B3(BOX_ID_Num);      

                    if(B3_Data_Buffer[0] == 0)
                    {
                        memcpy(&B3_Data_Buffer[1], rx_buf, 8); 
                        B3_Data_Buffer[0] = BOX_ID_Num;
                    }
                }        
            } 
            //探测器低功耗F0回复
            else if((rx_id&0xFFFFFFF00) == 0x18F0F600)
            { 
                BOX_ID_Num = rx_id&0xFF;//获取电池箱编号 
                TC_Sleep_Flag[BOX_ID_Num-1] = 0;
            }     
            //校时数据报文接收-主机发送的C0报文
            else if((rx_id&0xFFFFFFF00) == 0x18C0FFF6)
            {      
                RX8010_Time_Set(rx_buf);        
            }
            //接收C2报文，存储主机状态
            else if((rx_id&0xFFFFFFF00) == 0x18C2FFF6)
            {
                memcpy(Host_C2_Data, rx_buf, 8);//读取CAN报文数据        
            }

        #if 0 //屏蔽ZQK探测器接收报文处理
            //只接收CAN ID为 0x18FF01XX  的探测器数据 
            if((rx_id&0xFFFFFFF00) == 0x18FF0100)
            {
                BOX_ID_Num = rx_id&0xFF;//获取电池箱编号

                if((BOX_ID_Num==0)||(BOX_ID_Num>TC_NUM_MAX))return;//探测器编号超出范围

                //自动识别数量-总数增加时
                if(BOX_ID_Num > TC_Num)
                {
                    TC_Num = BOX_ID_Num;     
                    eeprom_write(0x03, TC_Num);
                }    
        #if 1
                /*************************************************************/
                //屏蔽传感器故障检测
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
            else if(rx_id == 0x0402F5F6) //设置电池箱数量ID：0x0402F5F6 
            {
                TC04_Num_Set(rx_buf); //获取电池箱数量           
            }  
        }
    }
}

//*******************************************/
//函数名:Sent_TC04_CAN_Data
//功  能:获取CAN接收数据
//参  数:无
//返回值:无
//时  间:20190605
//创建者:ahren
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

    static u8 Life_Signal_Cnt = 0;   //心跳包

     if((TC_Num >= 1)&&(TC_Num <= TC_NUM_MAX))//探测器数量准备好
     {
        if(PowerOn_OfflineDet_Flag >= 20)
        {
            Equipment_Off_Line_Det();//探测器离线检测
        }
        
        //20210223-读取备份数据
        if(backupdata_read != 0)
        {
            if((backupdata_read>0)&&(backupdata_read<=16))
            {
                CTC_BackupData_Buffer_Read(backupdata_read-1);
            }
            else 
            {
                CTC_BackupData_Buffer_Read(HOST_C2_DATA_ID);//C2数据
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
    
        //数据存储-B3数据存储
        if(B3_Data_Buffer[0] != 0)
        {
            temp = B3_Data_Buffer[0]-1;
            RX8010_Time_Get(BackupData_Temp);//获取实时时间
            BackupData_Temp[6] = 0x03;
            BackupData_Temp[7] = 0xB3;           
            memcpy(&BackupData_Temp[8], &B3_Data_Buffer[1], 8);          

            if(BackupData_State[temp] == 0)//前10分钟
            {                        
                CTC_BackupData_B3_Write(temp, BackupData_Temp, 16);     
            }
            else if(BackupData_State[temp] == 1)//后10分钟收到B3
            {                
                CTC_BackupData_Last_Write(temp, BackupData_Temp, 16);
            }

            if(BackupData_State[HOST_C2_DATA_ID] == 0)//C2数据
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

        //对主机发送报文
        if(0x01 == TJA1042_Sent_Time_Flag)//基准时间Time1  1s   
        {
            TJA1042_Sent_Time_Flag = 0;
            
            //20210223
            //子阀开启
             for(i=0; i<TC_Num; i++)//四级预警自动启动，优先级最高
             {
                 if(TC_Start_Flag[i] == 0x03)
                 {
                     CanToCan_TC_C3(i+1, 0x01);//打开子阀，1秒周期           
                 }
                 else if(TC_Start_Flag[i] == 0x04)//收到启动完成，可以关闭
                 {
                     CanToCan_TC_C3(i+1, 0x02);//关闭子阀，1秒周期
                 }
             }  

            //唤醒TC报文
            if(Sys_Sleep_Wake_TC != 0)
            {
                CanToCan_TC_F0(0x00); //低功耗
                CanToCan_Host_Communication(CTC_ZHU_Wake);
                Sys_Sleep_Wake_TC++;
                if(Sys_Sleep_Wake_TC>=4)
                {
                    Sys_Sleep_Wake_TC = 0;
                }  
            }
            
            //启动后延时两秒
           for(i=0; i<TC_Num; i++)
           {
               if(TC_Start_Flag[i] == 0x06)
               {
                   TC_Start_Wait_Cnt[i]++;
                   if(TC_Start_Wait_Cnt[i] >= 2)
                   {
                       TC_Start_Flag[i] = 0x04;//确认启动,执行关闭
                   }
               }
           }

            //探测器未编码状态
            if(TC_Code_Err == 1)//未编码
            {                     
                TC_Code_Recover_Cnt++;            
                if(TC_Code_Recover_Cnt > 5)
                {
                    TC_Code_Recover_Cnt = 0;
                    if(TC_Code_Recover_Flag == 0)
                    {
                        TC_Code_Err = 2;//恢复正常
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
                    CanToCan_Host_Communication(CTC_ZHU_CodeFault);//故障
                }            
                TC_Code_Err_Cnt++;
            }
            else if(TC_Code_Err == 2)
            {
                CanToCan_Host_Communication(CTC_ZHU_CodeNormal);//恢复正常
                TC_Code_Err_Cnt++;
                if(TC_Code_Err_Cnt>3)
                {
                    TC_Code_Err = 0;
                    TC_Code_Err_Cnt = 0;
                }
            }
        
            //20210324-备份数据     
            cnt_30s++;
            if(cnt_30s >= 30)cnt_30s = 0;

            RX8010_Time_Get(BackupData_Temp);//获取实时时间        
            for(i=0; i<TC_Num; i++)
            {
                if(BackupData_State[i] == 0)//前10分钟
                {
                    if(TC_RT_Data[i][7] != TC_RT_Data_Old[i])//状态改变
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
                    else//否则30s记录一次
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
                else if(BackupData_State[i] == 1)//后10分钟
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
            //存储C2数据-Host_C2_Data
            if(BackupData_State[HOST_C2_DATA_ID] == 0)//前
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
            else if(BackupData_State[HOST_C2_DATA_ID] == 1)//后
            {
                BackupData_Temp[6] = 0x02; 
                BackupData_Temp[7] = 0xC2;                                  
                memcpy(&BackupData_Temp[8], &Host_C2_Data[0], 8);          
                CTC_BackupData_Last_Write(HOST_C2_DATA_ID, BackupData_Temp, 16); 
            }

            if(BackupData_PowerON_Clear == 1)//报需清除备份的故障
            {  
                CanToCan_Host_Communication(CTC_ZHU_BackupDataFault);         
            }
            else if(BackupData_PowerON_Clear >= 2)//已清除备份数据
            {
                BackupData_PowerON_Clear++;
                CanToCan_Host_Communication(CTC_ZHU_BackupDataClear);  
                if(BackupData_PowerON_Clear>=7)//已经清除报文发送5次
                {
                    BackupData_PowerON_Clear = 0;
                }
            }       
            //20210223  
        
            //20210223
            if(Mamual_Start == 1)
            {
                time_3s_cnt++;
                if(time_3s_cnt > 3)//3秒定时
                {
                    time_3s_cnt = 0;
                    for(i=0; i<TC_Num; i++)
                    {
                        if(TC_Start_Flag[i] == 0x01)
                        {
                            CanToCan_TC_C1();//强制启动 周期3秒
                            break;
                        }
                    }
                    
                    if(TC_C1_R_10Min > 600)//10分钟后清空所有发送C1
                    {
                        TC_C1_R_10Min = 0;
                        Mamual_Start = 0;//10分钟后，解除手动启动
                        for(j=0; j<TC_Num; j++)
                        {
                            if((TC_Start_Flag[j] == 0x01)||(TC_Start_Flag[j] == 0x02))//10分钟到，清空所有发C1和收到C1回复
                            {
                                TC_Start_Flag[j] = 0x00;                           
                            }                            
                        }
                    }                
                }             
            }
            
            //20210308-BUG:得等到所有探测器全部正常，才能进入睡眠                   
            if(Sys_Sleep_Flag == 3)//睡眠后，报文唤醒状态
            {
                temp = 0;
                for(i=0; i<TC_Num; i++)
                {
                    if(TC04_OffLine_Flag[i]<=3)//在线
                    {
                        if((TC04_Buff[i][1] != 0)&&(TC04_Buff[i][1] != 0xFF))//存在预警
                        {
                            temp = 1;
                            break;
                        }                                
                    }           
                }
                if(temp == 0)
                {
                    Sys_Sleep_Flag = 4;//重新进入睡眠
                    Sys_Sleep_Host_cnt = 0;                
                } 
            }            
                        
            //报文发送
            memset(TC04_Abnormal_Table,0 ,TC_NUM_MAX);
            abnormal_state = 0;
            for(i=0; i<TC_Num; i++)
            {
                if((0x00 != TC04_Buff[i][1]) || (0x00 != TC04_Buff[i][7])||(TC04_OffLine_Flag[i]>3))//异常
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
                
                if(TC04_OffLine_Flag[table_idx] > 3)//离线
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
                else//异常
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
            else //全部正常
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
            
            CAN_Send_Data(0x18FF741D, 8, send_data_buf);//通过TJA1042发送报文给主机                
        }      
     }
     else //探测器数量错误
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
            CAN_Send_Data(0x18FF741D, 8, send_data_buf);//通过TJA1042发送报文给主机 
            
            if(TC_Code_Err == 1)//未编码
            {                     
                TC_Code_Recover_Cnt++;            
                if(TC_Code_Recover_Cnt > 5)
                {
                    TC_Code_Recover_Cnt = 0;
                    if(TC_Code_Recover_Flag == 0)
                    {
                        TC_Code_Err = 2;//恢复正常
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
                    CanToCan_Host_Communication(CTC_ZHU_CodeFault);//故障
                }            
                TC_Code_Err_Cnt++;
            }
            else if(TC_Code_Err == 2)
            {
                CanToCan_Host_Communication(CTC_ZHU_CodeNormal);//恢复正常
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


//函数名:TC04_Num_Set
//功  能:设置电池箱编号
//参  数:无
//返回值:无
//时  间:20180611
//创建者:ahren
/*******************************************/
void TC04_Num_Set(u8 *CAN_Receive_Data)
{
    u8 TC04_Num = 0;
    u8 CAN_TX0_Buff[8] = {0};
    u8 i;
    
    //通过CAN设置设备数量
    //将 CAN_Receive_Data 复制给 CAN_TX0_Buff        
    memcpy(CAN_TX0_Buff, CAN_Receive_Data, 8);     
    
    if((CAN_Receive_Data[0] == 0x23) && (CAN_Receive_Data[7] == 0x16))  // 23 xx 00 00 00 00 00 16
    {
        if((CAN_Receive_Data[1] == 0x01))//设置数量和读取数量
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
                
                TC_Num = CAN_TX0_Buff[3];
            } 
            CAN_TX0_Buff[0] = 0x23;
            CAN_TX0_Buff[7] = 0x16;

            CAN_Send_Data(0x0402F5F6, 8, CAN_TX0_Buff);           
            delay_10ms(1);                      
            CAN_STB_OFF();//设置或读取后需重新上电 
        }
        else if((CAN_Receive_Data[1] == 0x02))//设置对外发送周期
        {                     
            eeprom_write(0x08, CAN_Receive_Data[2]);                           
            NOP();NOP();NOP();NOP();

            CAN_TX0_Buff[3] = eeprom_read(0x08);                      
            NOP();NOP();NOP();NOP();  
            
            CAN_TX0_Buff[0] = 0x23;
            CAN_TX0_Buff[2] = 0x00;
            CAN_TX0_Buff[7] = 0x16;

            CAN_Send_Data(0x0402F5F6, 8, CAN_TX0_Buff);//通过TJA1042 发送确认
            delay_10ms(1);  
            CAN_STB_OFF();//设置或读取后需重新上电              
        }       
        else if((CAN_Receive_Data[1]==0x03))//设置对外仪表ID
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

            CAN_Send_Data(0x0402F5F6, 8, CAN_TX0_Buff);//通过TJA1042 发送确认
            delay_10ms(1);  
            CAN_STB_OFF();//设置或读取后需重新上电                            
        }   
        //20210405-备份数据读取，备份记录清除
        else if(CAN_Receive_Data[1] == 0x04)
        {
            if(CAN_Receive_Data[2] == 0x01)//读取备份数据
            {
                if(backupdata_read == 0)
                {
                    backupdata_read = CAN_Receive_Data[3];
                }                        
            }
            else if(CAN_Receive_Data[2] == 0x02)//清除备份数据
            {
                for(i=0; i<TC_BACKUP_COUNT; i++)
                {
                   W25QXX_Erase_Sector(TC_X_Base[i]+TC_B3_DATA_OFFSET, 0);//擦除
                   W25QXX_Erase_Sector(TC_X_Base[i]+TC_BEFORE_DATA_OFFSET, 0);//擦除
                   BackupData_PowerON_Clear = 2;//可以存储
                   BackupData_State[i] = 0;//进入前10分钟存储数据
                   BackupData_Cnt[i] = 0;    //开始10分钟数据存储    
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
//函数名:Equipment_Off_Line_Det
//功  能:探测器离线检测
//参  数:无
//返回值:无
//时  间:20190716
//创建者:ahren
/*******************************************/
void Equipment_Off_Line_Det(void)
{
     u8 TC_Idx = 0;
      
    if(TC04_Off_Time_Cnt_g >= 350)//200*5ms = 1000ms 检测一次离线
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
                if(TC04_OffLine_Flag[TC_Idx] <= 3)//大于3表示离线
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


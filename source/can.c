#include "can.h"

//CAN引脚
#define CAN_TX_Out()    (TRISE5 = 0) 						       //CANTX/RE5为输出
#define CAN_RX_In()     (TRISE4 = 1)						       //CANRX/RE4为输入

bit CAN2_RXD_Interrupt_Flag_g = 0;					    	//RX0接收中断标志位 "=1"表示有CAN2有接收到信号

static void CAN_Baud_Rate_Set(void)
{
    //CAN时钟F OSC 
    BRGCON1bits.SJW = 0b00;//同步跳转宽度位
                        //11 = 同步跳转宽度时间 = 4 x T Q
                        //10 = 同步跳转宽度时间 = 3 x T Q
                        //01 = 同步跳转宽度时间 = 2 x T Q
                        //00 = 同步跳转宽度时间 = 1 x T Q
    
    BRGCON1bits.BRP = 0;//波特率预分频比位
                        //111111 = T Q = (2 x 64)/F OSC
                        //111110 = T Q = (2 x 63)/F OSC
                        //:
                        //:
                        //000001 = T Q = (2 x 2)/F OSC
                        //000000 = T Q = (2 x 1)/F OSC    
    
    BRGCON2bits.SEG2PHTS = 1;//：相位缓冲段 2 时间选择位    1  = 可自由编程      0 = PHEG1 的最大值与信息处理时间（Information Processing Time， IPT）中的较大值
    BRGCON2bits.SAM = 0;//CAN 总线线路采样位  1 = 在采样点之前对总线线路采样三次   0 = 在采样点对总线线路采样一次
    BRGCON2bits.SEG1PH = 0b110; // ：相位缓冲段 1 位
                            //111 = 相位缓冲段 1 时间 = 8 x T Q
                            //110 = 相位缓冲段 1 时间 = 7 x T Q
                            //101 = 相位缓冲段 1 时间 = 6 x T Q
                            //100 = 相位缓冲段 1 时间 = 5 x T Q
                            //011 = 相位缓冲段 1 时间 = 4 x T Q
                            //010 = 相位缓冲段 1 时间 = 3 x T Q
                            //001 = 相位缓冲段 1 时间 = 2 x T Q
                            //000 = 相位缓冲段 1 时间 = 1 x T Q    
    BRGCON2bits.PRSEG = 0b101;  // ：传播时间选择位
                            //111 = 传播时间 = 8 x T Q
                            //110 = 传播时间 = 7 x T Q
                            //101 = 传播时间 = 6 x T Q
                            //100 = 传播时间 = 5 x T Q
                            //011 = 传播时间 = 4 x T Q
                            //010 = 传播时间 = 3 x T Q
                            //001 = 传播时间 = 2 x T Q
                            //000 = 传播时间 = 1 x T Q    
    
    BRGCON3bits.WAKDIS = 0;// 唤醒禁止位    1 = 禁止 CAN 总线活动唤醒功能   0 = 使能 CAN 总线活动唤醒功能
    BRGCON3bits.WAKFIL = 1;// 选择是否使用 CAN 总线线路滤波器唤醒的位   1 = 使用 CAN 总线线路滤波器来唤醒  0 = 不使用 CAN 总线线路滤波器来唤醒
    BRGCON3bits.SEG2PH = 0b001;// 相位缓冲段 2 时间选择位 (1)
                        //111 = 相位缓冲段 2 时间 = 8 x T Q
                        //110 = 相位缓冲段 2 时间 = 7 x T Q
                        //101 = 相位缓冲段 2 时间 = 6 x T Q
                        //100 = 相位缓冲段 2 时间 = 5 x T Q
                        //011 = 相位缓冲段 2 时间 = 4 x T Q
                        //010 = 相位缓冲段 2 时间 = 3 x T Q
                        //001 = 相位缓冲段 2 时间 = 2 x T Q
                        //000 = 相位缓冲段 2 时间 = 1 x T Q    
    
    //TQ=((2*(BRP+1))/Fosc= 2/4M = 0.5us 
    //位时间＝TQ*(Sync_Seg+ Prop_Seg+ Phase_seg1+ Phase_seg2)=(1+1+3+3)TQ＝8TQ
    //位率=1/(8*0.5us)=250Kbps
									
	TXB0CON=0x03;					//发送优先级为最高优先级，TXPRI=11     
}

static void CAN_TXD_ID_Config(uint32_t tx_id)
{
    uint8_t RX_TX_SID_H, RX_TX_SID_L, RX_TX_EID_H, RX_TX_EID_L;
	uint8_t ID_L = 0X00;
	uint8_t ID_H = 0X00;    
    
    RX_TX_SID_H = (tx_id>>24)&0xFF;
    RX_TX_SID_L = (tx_id>>16)&0xFF;
    RX_TX_EID_H = (tx_id>>8)&0xFF;
    RX_TX_EID_L = (tx_id)&0xFF;
	
	ID_L = (RX_TX_SID_L & 0XF0) >> 5;
	ID_H = (RX_TX_SID_H << 3) | ID_L;				//设置过滤器SIDH
	ID_L = (RX_TX_SID_L & 0X1C) << 3;
	ID_L = (RX_TX_SID_L & 0X03) | ID_L | 0X08;		//设置过滤器SIDL(扩展帧)	    
       
    TXB0SIDH = ID_H;
    TXB0SIDL = ID_L;
    TXB0EIDH = RX_TX_EID_H;
    TXB0EIDL = RX_TX_EID_L;		    

    //清空发送缓冲区
	TXB0D0=0X00;												
	TXB0D1=0X00;
	TXB0D2=0X00;
	TXB0D3=0X00;
	TXB0D4=0X00;
	TXB0D5=0X00;
	TXB0D6=0X00;
	TXB0D7=0X00;       
}

static void CAN_RXD_ID_Config(uint8_t chan, uint32_t rx_id, uint32_t rxid_mask)
{
    uint8_t Rx_Id[4] = {0};
    uint8_t RX_TX_SID_H, RX_TX_SID_L, RX_TX_EID_H, RX_TX_EID_L;
	uint8_t ID_L = 0X00;
	uint8_t ID_H = 0X00;        
    
    if(chan == 0)//通道0
    {
        RXB0CON=0x40;  												// bit6-5=10//仅接收扩展标识符的有效信息，FILHIT0=0表示RXB0 ，采用FILHIT0   

        RX_TX_SID_H = (rx_id>>24)&0xFF;
        RX_TX_SID_L = (rx_id>>16)&0xFF;
        RX_TX_EID_H = (rx_id>>8)&0xFF;
        RX_TX_EID_L = (rx_id)&0xFF;  

        ID_L = (RX_TX_SID_L & 0XF0) >> 5;
        ID_H = (RX_TX_SID_H << 3) | ID_L;				//设置过滤器SIDH
        ID_L = (RX_TX_SID_L & 0X1C) << 3;
        ID_L = (RX_TX_SID_L & 0X03) | ID_L | 0X08;		//设置过滤器SIDL(扩展帧)	        

        RXF0SIDH = ID_H;		
        RXF0SIDL = ID_L;
        RXF0EIDH = RX_TX_EID_H;
        RXF0EIDL = RX_TX_EID_L;		  

        Rx_Id[0] = (rxid_mask>>24)&0xFF;
        Rx_Id[1] = (rxid_mask>>16)&0xFF;
        Rx_Id[2] = (rxid_mask>>8)&0xFF;
        Rx_Id[3] = (rxid_mask)&0xFF;  

        RXM0SIDH = (Rx_Id[0] << 3)|(Rx_Id[1] >> 5);		
        RXM0SIDL = ((Rx_Id[1]&0X03)|0X08)|((Rx_Id[1]<<3)&0XE0);										//高3位为标识符低3位，低5位是配置
        RXM0EIDH = Rx_Id[2];
        RXM0EIDL = Rx_Id[3];    

        //初始化接受缓冲器0的数据区数据
        RXB0D0=0X00;												
        RXB0D1=0X00;
        RXB0D2=0X00;
        RXB0D3=0X00;
        RXB0D4=0X00;
        RXB0D5=0X00;
        RXB0D6=0X00;
        RXB0D7=0X00;            
    }
    else if(chan == 1)//通道1
    {
        RXB1CON=0x40;  												// bit6-5=10//仅接收扩展标识符的有效信息，FILHIT0=0表示RXB0 ，采用FILHIT0   

        RX_TX_SID_H = (rx_id>>24)&0xFF;
        RX_TX_SID_L = (rx_id>>16)&0xFF;
        RX_TX_EID_H = (rx_id>>8)&0xFF;
        RX_TX_EID_L = (rx_id)&0xFF;  

        ID_L = (RX_TX_SID_L & 0XF0) >> 5;
        ID_H = (RX_TX_SID_H << 3) | ID_L;				//设置过滤器SIDH
        ID_L = (RX_TX_SID_L & 0X1C) << 3;
        ID_L = (RX_TX_SID_L & 0X03) | ID_L | 0X08;		//设置过滤器SIDL(扩展帧)	        

        RXF1SIDH = ID_H;		
        RXF1SIDL = ID_L;
        RXF1EIDH = RX_TX_EID_H;
        RXF1EIDL = RX_TX_EID_L;		  

        Rx_Id[0] = (rxid_mask>>24)&0xFF;
        Rx_Id[1] = (rxid_mask>>16)&0xFF;
        Rx_Id[2] = (rxid_mask>>8)&0xFF;
        Rx_Id[3] = (rxid_mask)&0xFF;  

        RXM1SIDH = (Rx_Id[0] << 3)|(Rx_Id[1] >> 5);		
        RXM1SIDL = ((Rx_Id[1]&0X03)|0X08)|((Rx_Id[1]<<3)&0XE0);										//高3位为标识符低3位，低5位是配置
        RXM1EIDH = Rx_Id[2];
        RXM1EIDL = Rx_Id[3];    

        //初始化接受缓冲器0的数据区数据
        RXB1D0=0X00;												
        RXB1D1=0X00;
        RXB1D2=0X00;
        RXB1D3=0X00;
        RXB1D4=0X00;
        RXB1D5=0X00;
        RXB1D6=0X00;
        RXB1D7=0X00;             
    }
}

void CAN_Init(void)
{
    CAN_STB_Out();		
	CAN_STB_OFF();						//禁用CAN使能PIN
    
    CAN_TX_Out();
    CAN_RX_In();      
    
    CANCON=0X80;					//请求进入CAN配置模式REQOP=100	(控制寄存器)
	while((CANSTAT&0X80)==0);		//等待进入CAN配置模式OPMODE=100(状态寄存器)	

    CAN_Baud_Rate_Set();

    //发送配置
    CAN_TXD_ID_Config(0x18FF741D);

    //接收配置
    CAN_RXD_ID_Config(0, 0x18FF0000, 0x0);//通道0：0：全收  
    CAN_RXD_ID_Config(1, 0x04020000, 0x0);//通道1：
      
	CIOCON=0X21;												//初始化CAN模块的I/O控制寄存器	 	 	
 	CANCON=0X00;												//进入正常操作模式
	while((CANSTAT&0x40)!=0); 									//检测配置完成

	PIR5=0X00;													//清所有中断标志
	PIE5=0X01;													//使能接收缓冲器0、1 的接收中断（仅打开接收中断使能）    
    
    CAN_STB_ON();												//开启CAN收发器
}

void CAN_DeInit(void)
{
    CAN_STB_OFF();						//禁用CAN使能PIN
}

void CAN_Send_Data(uint32_t id, uint8_t len, uint8_t *buf) //邮箱0发送DLC长度的报文
{   
    if(CAN_STB_Pin)return;
    
    uint8_t i = 0;
    while(TXB0CON & 0x08)
    {
        i++;
        if(i>250)break;
    }    
    
    CAN_TXD_ID_Config(id);
    
	TXB0CON = (TXB0CON&0xf7);				//TXREQ清0首先，禁止发送	
	TXB0IF = 0;								//清空发送完成标志		
	TXB0DLC = len;							//设置发送数据的长度为8个字节
	
    for(i=0; i<len; i++)
	{	
		*(&TXB0D0 + i) = buf[i];				
	} 
	TXB0CON=(TXB0CON|0x08);					//准备完毕，请求发送,TXREQ=1
}

uint8_t CAN_Rece_Data(uint32_t *id, uint8_t *len, uint8_t *buf)
{
	uint8_t i = 0;
    uint32_t sid_tem = 0; 
    uint16_t sid_tem1 = 0; 
    uint16_t sid_tem2 = 0;
    if(CAN_STB_Pin)
    {
        RXFUL = 0;//打开接收缓冲器来接收新信息
        return 0;
    }
    
    sid_tem1 = (RXB0SIDH<<8)| RXB0SIDL;     
    sid_tem = ((sid_tem1>>3)&(~0x03))|(sid_tem1 & 0x03);   
    sid_tem2 = (RXB0EIDH<<8)|RXB0EIDL;
    *id = (sid_tem<<16)|sid_tem2;
	*len = RXB0DLC;
	for(i=0; i<(*len); i++)
	{	
		buf[i] = *(&RXB0D0 + i);				
	}  
    RXFUL = 0;//打开接收缓冲器来接收新信息		
    return 1;
}


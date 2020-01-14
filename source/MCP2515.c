/*----------------------------------------------------------------------*/
/* Confidential and Proprietary Information of MicroPerfect Corporation */
/* (C) 2018 ,MicroPerfect Corporation . All rights reserved. */
/* FileName : MCP2515.c */
/* Function : */
/* Ver: 0.01  */
/* MP_Ahren NewFile */
/* History:YYYY.MM.DD Version author Description 
          :2018.07.03 0.01 @wuwenfang Follow the coding standard  */
/*-------------------------------------------------------------*/

/***************头文件****************/
/**/
/*************************************/
#include "MCP2515.H"
//******************宏定义*************/
/**/
/*************************************/

/***************变量定义************/
/**/

/*************************************/

/***************函数申明**************/
/**/
/*************************************/

/*************************************/

void SPI_Init(void)
{
    //引脚配置
    MCP2515_SILENT_DIR   = 0;
    MCP2515_CS_DIR       = 0;
    MCP2515_RESET_DIR    = 0;
    MCP2515_SCK_DIR      = 0;
    MCP2515_MISO_DIR     = 1;
    MCP2515_MOSI_DIR     = 0;    

    SSPMD = 0;   //SSPMD = 1:禁止 MSSP 模块,SSPMD = 0:使能 MSSP 模块
    SSPOD = 0;   //SSPOD = 1:SPI 漏极开路输出使能位,SSPOD = 0:禁止漏极开路功能

    SMP = 1;  //1 = 在数据输出时间的末尾采样输入数据   0 = 在数据输出时间的中间采样输入数据
    CKE = 1;  //SPI 时钟选择位   1:时钟状态从有效转换到空闲时发送  0:时钟状态从空闲转换到有效时发送

    SSPEN = 1; //1 = 使能串口并将 SCK、 SDO、 SDI 和 SS 配置为串口引脚,0 = 禁止串口并将上述引脚配置为 I/O 端口引脚
    CKP = 0;   //1 = 空闲状态时，时钟为高电平 ,0 = 空闲状态时，时钟为低电平

    //1010 = SPI 主模式：时钟 = F OSC /8
    //0101 = SPI 从模式：时钟 = SCK 引脚；禁止 SS 引脚控制；可将 SS 用作 I/O 引脚
    //0100 = SPI 从模式：时钟 = SCK 引脚；使能 SS 引脚控制
    //0011 = SPI 主模式：时钟 = TMR2 输出 /2
    //0010 = SPI 主模式：时钟 = FOSC /64
    //0001 = SPI 主模式：时钟 = FOSC /16
    //0000 = SPI 主模式：时钟 = FOSC /4
    SSPM3 = 0;
    SSPM2 = 0;
    SSPM1 = 0;
    SSPM0 = 0;          
}

/*************************************************************************/
//函数名:SPI_RWByte
//功  能:
//参  数:
//返回值：
//时间：20180703
//创建者：ahren
/************************************************************************/
u8 SPI_RWByte(u8 dat)
{    
    while(WCOL)WCOL=0;
    SSPBUF = dat;
    while(!BF);//缓冲区满状态位 1：满，0：空
    return SSPBUF;
}

/*************************************************************************/
//函数名:SPI_ReadByte
//功  能:模拟SPI通信读时序
//参  数:无
//返回值：读取的数据
//时间：20180703
//创建者：ahren
/***************************/
u8 SPI_ReadByte(void)
{
    u8 RDate = 0;	

    RDate = SPI_RWByte(0xFF);

    return RDate;
}
//
/***************************/
//函数名:SPI_WriteByte
//功  能:模拟SPI通信写时序
//参  数:写的数据
//返回值：无
//时间：20180703
//创建者：ahren
/***************************/
void SPI_WriteByte(u8 Data)
{    
    SPI_RWByte(Data);
}

/***************************/
//函数名:MCP2515_WriteByte
//功  能:MCP2515写数据
//参  数:addr：地址, dat：数据
//返回值：无
//时间：20180703
//创建者：ahren
/***************************/
void MCP2515_WriteByte(u8 Addr,u8 Data)
{
	MCP2515_CS = 0;
	SPI_WriteByte(MCP2515_CAN_WRITE);	//写命令
	SPI_WriteByte(Addr);		        //写寄存器地址
	SPI_WriteByte(Data);			    //写数据
    MCP2515_CS = 1;				
}

/***************************/
//函数名:MCP2515_ReadByte
//功  能:MCP2515读数据
//参  数:addr：地址
//返回值：数据
//时间：20180703
//创建者：ahren
/***************************/
u8 MCP2515_ReadByte(u8 Addr)
{
	u8 RData;	

	MCP2515_CS = 0;
	SPI_WriteByte(MCP2515_CAN_READ);		//读命令
	SPI_WriteByte(Addr);		           	//写寄存器地址
	RData = SPI_ReadByte();	                //读数据
	MCP2515_CS = 1;	

	return RData;				
}

/***************************/
//函数名:MCP2515_Reset
//功  能:MCP2515软件复位
//参  数:无
//返回值：无
//时间：20180703
//创建者：ahren
/***************************/
void MCP2515_Reset(void)
{
	MCP2515_CS = 0;				
	SPI_WriteByte(MCP2515_CAN_RESET);	//复位指令
	MCP2515_CS = 1;				
}

/*************************************************************************/
//函数名:MCP2515_CAN_BaudRate
//功  能:MCP2515CAN波特率设置
//参  数:无
//返回值：无
//时间：20180703
//创建者：ahren
/************************************************************************/
void MCP2515_CAN_BaudRate(u8 Fosc, uint16 BRP)
{
    static u8 Fosc_Val    = 0;
    static uint16 BRP_Val = 0;

    Fosc_Val = Fosc;
    BRP_Val = BRP;
    /***************************************************/
    //20191204
    //ahren
    //波特率计算公式
    //CNF1 (SJW = 1TQ, BRP = X) 
    //CNF2 (PHSEG1 = 3TQ, PRSEG = 1TQ)
    //CNF3 (PHSEG2 = 3TQ)    
    //TQ = 2*(BRP+1)/Fosc = Y
    //位时间 = TQ*(1+3+1+3)=Y*8=8Y
    //位率 = 1/8Y
    /***************************************************/    
    if (Fosc_Val == 0x08)
    {
        if (BRP_Val == 0xFA)
        {
            //250Kbps
            //BRP=X=0; Fosc=8; Y=0.5; 8Y=4; 1/8Y=1/4=0.25;
            MCP2515_WriteByte(MCP2515_CNF1,MCP2515_SJW_1TQ|MCP2515_BRP_2_F);
            MCP2515_WriteByte(MCP2515_CNF2,MCP2515_BTLMODE|MCP2515_1_SAM|MCP2515_PHSEG1_8TQ|MCP2515_PRSEG_1TQ);    	
            MCP2515_WriteByte(MCP2515_CNF3,MCP2515_SOF_DISABLED|MCP2515_WAKFIL_DISABLED|MCP2515_PHSEG2_6TQ); 
	            
        }
        else if (BRP_Val == 0x01F4)
        {
            //500Kbps
            //BRP=X=0; Fosc=8; Y=0.25; 8Y=2; 1/8Y=1/2=0.5; 
           MCP2515_WriteByte(MCP2515_CNF1,MCP2515_SJW_1TQ|MCP2515_BRP_2_F);
           MCP2515_WriteByte(MCP2515_CNF2,MCP2515_BTLMODE|MCP2515_PHSEG1_4TQ|MCP2515_PRSEG_1TQ);    	
           MCP2515_WriteByte(MCP2515_CNF3,MCP2515_SOF_DISABLED|MCP2515_WAKFIL_DISABLED|MCP2515_PHSEG2_2TQ); 
        }
    }   			
}

/*************************************************************************/
//函数名:MCP2515_CAN_TxID
//功  能:MCP2515发送ID设置
//参  数:ID数组首地址，接收数组长度
//返回值：无
//时间：20180703
//创建者：ahren
/************************************************************************/
void MCP2515_CAN_TxID(u8 *ID, u8 Len)
{
    u8 SIDH = 0;
    u8 SIDL = 0;

    SIDL = (ID[1] & 0xF0) >> 5;
    SIDH = (ID[0] << 3) + SIDL;

    SIDL = (ID[1] & 0x1C) << 3;
    SIDL = (ID[1] & 0x03) + SIDL + 0x08;

    //发送缓冲寄存器的标识符设置（扩展）
	MCP2515_WriteByte(MCP2515_TXB0SIDH,SIDH); 
    delay_10us(100); 
	MCP2515_WriteByte(MCP2515_TXB0SIDL,SIDL);
    delay_10us(100);     
	MCP2515_WriteByte(MCP2515_TXB0EID8,ID[2]);
    delay_10us(100);     
	MCP2515_WriteByte(MCP2515_TXB0EID0,ID[3]);
    delay_10us(100);   
    MCP2515_WriteByte(MCP2515_TXB0DLC,Len);	
    delay_10us(100);  		
}

/*************************************************************************/
//函数名:MCP2515_CAN_RxID
//功  能:MCP2515接收ID设置
//参  数:ID数组首地址，接收数组长度
//返回值：无
//时间：20180703
//创建者：ahren
/************************************************************************/
void MCP2515_CAN_RxID(u8 *ID, u8 len)
{
    u8 SIDH = 0;
    u8 SIDL = 0;

    SIDL = (ID[1] & 0xF0) >> 5;
    SIDH = (ID[0] << 3) + SIDL;

    SIDL = (ID[1] & 0x1C) << 3;
    SIDL = (ID[1] & 0x03) + SIDL + 0x08;

	MCP2515_WriteByte(MCP2515_RXB0DLC,len);

	//配置验收滤波寄存器标识符设置（扩展）
	MCP2515_WriteByte(MCP2515_RXF0SIDH,SIDH);    
	MCP2515_WriteByte(MCP2515_RXF0SIDL,SIDL);
	MCP2515_WriteByte(MCP2515_RXF0EID8,ID[2]);
	MCP2515_WriteByte(MCP2515_RXF0EID0,ID[3]);

    //配置验收屏蔽寄存器标识符设置（扩展）
    MCP2515_WriteByte(MCP2515_RXM0SIDH,0xFF);    
	MCP2515_WriteByte(MCP2515_RXM0SIDL,0xFF);
	MCP2515_WriteByte(MCP2515_RXM0EID8,0xFF);
	MCP2515_WriteByte(MCP2515_RXM0EID0,0xFF);  

	MCP2515_WriteByte(MCP2515_RXB1DLC,len);
	//配置验收滤波寄存器标识符设置（扩展）
	MCP2515_WriteByte(MCP2515_RXF1SIDH,SIDH);    
	MCP2515_WriteByte(MCP2515_RXF1SIDL,SIDL);
	MCP2515_WriteByte(MCP2515_RXF1EID8,ID[2]);
	MCP2515_WriteByte(MCP2515_RXF1EID0,ID[3]);

    //配置验收屏蔽寄存器标识符设置（扩展）
    MCP2515_WriteByte(MCP2515_RXM1SIDH,0xFF);    
	MCP2515_WriteByte(MCP2515_RXM1SIDL,0xFF);
	MCP2515_WriteByte(MCP2515_RXM1EID8,0xFF);
	MCP2515_WriteByte(MCP2515_RXM1EID0,0xFF);         
}

/*************************************************************************/
//函数名:MCP2515_Init
//功  能:MCP2515初始化
//参  数:无
//返回值：无
//时间：20180703
//创建者：ahren
/************************************************************************/
void MCP2515_Init(void)
{         
    SPI_Init();   
    delay_10us(10);   

    MCP2515_SILENT_DIR = 0;
    MCP2515_CS_DIR     = 0;
    MCP2515_RESET_DIR  = 0;   
    
    MCP2515_SILENT_OFF;
    delay_10us(5);  

    MCP2515_RESET = 1;//关闭硬件复位
	MCP2515_Reset();//软件复位,进入配置模式
    delay_10ms(1);       
    /**************************************************************************************/
    //若软件复位没有进入配置模式，将通过设置寄存器进入配置模式
    //20200114
    //ahren
	if(MCP2515_REQOP_CONFIG != (MCP2515_ReadByte(MCP2515_CANSTAT) & 0x80))         
	{
		MCP2515_WriteByte(MCP2515_CANCTRL,MCP2515_REQOP_CONFIG|MCP2515_ABORT_DISABLED|
                          MCP2515_OSM_DISABLED|MCP2515_CLKOUT_DISABLED|MCP2515_CLKOUT_PRE_1);
        delay_10ms(1);         
	}
    /**************************************************************************************/
	MCP2515_WriteByte(MCP2515_TXRTSCTRL,0x00);//发送引脚状态设置                                        
    delay_10us(10);  

	MCP2515_WriteByte(MCP2515_BFPCTRL,0x00);//接收引脚状态设置                                        
    delay_10us(10);     

	MCP2515_WriteByte(MCP2515_CANINTE,0x00);//MCP2515中断设置                                        
    delay_10us(10);  
    
    MCP2515_WriteByte(MCP2515_CANINTF,0x00);//中断标志位全部写0
    delay_10us(10);   

	MCP2515_WriteByte(MCP2515_TXB0CTRL,0x03); //配置发送缓冲器0                                       
    delay_10us(10);  

	MCP2515_WriteByte(MCP2515_TXB1CTRL,0x00); //配置发送缓冲器1                                       
    delay_10us(10);  

	MCP2515_WriteByte(MCP2515_TXB2CTRL,0x00); //配置发送缓冲器2                                        
    delay_10us(10);               

    MCP2515_CAN_TxID(MCP2515_TxID,8); //发送ID设置 
    delay_10us(10); 

    //只接收扩展帧报文数据
	MCP2515_WriteByte(MCP2515_RXB0CTRL,0x44);  
	MCP2515_WriteByte(MCP2515_RXB1CTRL,0x40);       
#if 0                                
	MCP2515_CAN_RxID(MCP2515_RxID,8);//接收ID设置
    delay_10us(10); 
#endif    

    MCP2515_CAN_BaudRate(SYS_FOSC_8MHZ,CAN_BRP_500);//波特率设置
    delay_10ms(1);     
    //工作模式设置，设置成正常模式
    MCP2515_WriteByte(MCP2515_CANCTRL,MCP2515_REQOP_NORMAL|MCP2515_ABORT_DISABLED|
                        MCP2515_OSM_DISABLED|MCP2515_CLKOUT_DISABLED|MCP2515_CLKOUT_PRE_1);    
//	MCP2515_WriteByte(MCP2515_CANCTRL,MCP2515_REQOP_NORMAL|MCP2515_CLKOUT_ENABLED);	
    delay_10ms(1);  
    //确认工作模式是否设置成功
	if(MCP2515_OPMODE_NORMAL != (MCP2515_ReadByte(MCP2515_CANSTAT) & 0xE0))         
	{
		//MCP2515_WriteByte(MCP2515_CANCTRL,MCP2515_REQOP_NORMAL|MCP2515_CLKOUT_ENABLED);
    MCP2515_WriteByte(MCP2515_CANCTRL,MCP2515_REQOP_NORMAL|MCP2515_ABORT_DISABLED|
                      MCP2515_OSM_DISABLED|MCP2515_CLKOUT_DISABLED|MCP2515_CLKOUT_PRE_1);           
	}
    delay_10ms(1);  
}


/*************************************************************************/
//函数名:MCP2515_Tx_Buffer
//功  能:MCP2515_CAN发送函数
//参  数:发送数组，数组长度
//返回值：无
//时间：20180703
//创建者：ahren
/************************************************************************/
void MCP2515_Tx_Buffer(u8 *CAN_TX_Buf, u8 len)
{
	u8 i = 0;
    u8 Cnt = 0;

    //读取状态值，是否可以发送 
    while((MCP2515_ReadByte(MCP2515_TXB0CTRL)&0x0B) && (Cnt<50))   
    {			
        Cnt++;
    }

    //写入发送数据到发送缓冲区 
    for(i=0; i<len; i++)
    {
        MCP2515_WriteByte(MCP2515_TXB0D0 + i, CAN_TX_Buf[i]);
        delay_10us(5);                
    }

    MCP2515_CS = 0;    
    MCP2515_WriteByte(MCP2515_TXB0DLC, len);//写入发送的数据长度
    delay_10us(5);                                    
    MCP2515_WriteByte(MCP2515_TXB0CTRL,0x0B);//发送报文 
    delay_10us(5);                               
    MCP2515_CS = 1;  
}
/*************************************************************************/
//函数名:MCP2515_Rx_Buffer
//功  能:MCP2515_CAN接收函数
//参  数:接收数组首地址
//返回值：接收字节长度
//时间：20180703
//创建者：ahren
/************************************************************************/
u8 MCP2515_Rx_Buffer(u8 *CAN_RX_Buf)
{
	u8 i = 0;
    u8 len = 0;
    u8 temp = 0;   
    u8 *reg = CAN_RX_Buf ;

    //读取中断标志位，是否接收完成    
	temp = MCP2515_ReadByte(MCP2515_CANINTF);
    delay_10us(5);             
    //接收到的数据长度
    len = (MCP2515_ReadByte(MCP2515_RXB0DLC) & 0x0F);
    delay_10us(5);    
	if(temp & 0x01)
	{
		for(i=0; i<len; i++)
		{	
            *(reg + i) = MCP2515_ReadByte(MCP2515_RXB0D0+i); 
            delay_10us(5);          
		}             
	}               
    MCP2515_WriteByte(MCP2515_CANINTF,0); 
    delay_10us(5); 
	return len;
}

/*************************************************************************/
//函数名:MCP2515_Sleep_Mode
//功  能:MCP2515进入睡眠模式
//参  数:
//返回值：
//时间：20180703
//创建者：ahren
/************************************************************************/
u8 MCP2515_Sleep_Mode(void)
{
	u8 time_out = 0;

	MCP2515_WriteByte(MCP2515_CANCTRL, MCP2515_REQOP_SLEEP);                //设置成睡眠模式
    NOP();NOP();NOP();NOP();
    //delay_10ms(1);   
    delay_10us(5);      	
	while(MCP2515_OPMODE_SLEEP != (MCP2515_ReadByte(MCP2515_CANSTAT) & 0xE0))//确认是否设置成功
	{
        //delay_10ms(1);  
        delay_10us(5);           
        NOP();NOP();NOP();NOP();        
		time_out++;
		if(time_out>100)//超时处理
		{
			return 0;
		}		
	}
	return 1;	
}


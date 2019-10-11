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

//#define	SPI_ReadByte()	SPI_RWByte(0xff)
//#define	SPI_WriteByte(dat)	SPI_RWByte(dat)
/***************变量定义************/
/**/
u8 MCP2515_Buff[9] = {0};
/*************************************/

/***************函数申明**************/
/**/
/*************************************/

/*************************************/

void SPI_Init(void)
{
    //引脚配置
    TRISC5 = 0;   //SDO
    TRISC3 = 0;   //SCK
    TRISC4 = 1;   //SDI
    
   // TRISC1 = 0;   //CS
   // TRISA5 = 1;

    SSPMD = 0;   //SSPMD ：MSSP 模块禁止位   1 = 禁止 MSSP 模块。所有 SSP 寄存器都保持在复位状态且不可写。  0 = 使能 MSSP 模块
    SSPOD = 0;   //SSPOD ：SPI 漏极开路输出使能位    1 = 使能漏极开路功能     0 = 禁止漏极开路功能
    //SSPSTAT:
    SMP = 1;         //1 = 在数据输出时间的末尾采样输入数据   0 = 在数据输出时间的中间采样输入数据
    CKE = 1;         //SPI 时钟选择位   1 = 时钟状态从有效转换到空闲时发送     0 = 时钟状态从空闲转换到有效时发送
    //BF           //缓冲区满状态位 （仅限接收模式） 1 = 接收完成， SSPBUF 已满   0 = 接收未完成， SSPBUF 为空
    //SSPCON1:
    //WCOL = 0;    ///：写冲突检测位  1 = 正在发送前一个字时，又有数据写入 SSPBUF 寄存器 （必须用软件清零）0 = 未发生冲突
    //SSPOV =0;//：接收溢出指示位   SPI 从模式：1 = SSPBUF 寄存器中仍保存前一数据时，又接收到一个新的字节。如果发生溢出， SSPSR 中的数据会丢失。溢出只会在从模式下发生。即使只是发送数据，用户也必须读 SSPBUF，以避免将溢出标志位置 1 （该位必须用软件清零）。0 = 无溢出
    SSPEN = 1;       //1 = 使能串口并将 SCK、 SDO、 SDI 和 SS 配置为串口引脚   0 = 禁止串口并将上述引脚配置为 I/O 端口引脚
    CKP = 0;         //1 = 空闲状态时，时钟为高电平      0 = 空闲状态时，时钟为低电平
    SSPM3 = 0;
    SSPM2 = 0;
    SSPM1 = 0;
    SSPM0 = 0;          //1010 = SPI 主模式：时钟 = F OSC /8
                        //0101 = SPI 从模式：时钟 = SCK 引脚；禁止 SS 引脚控制；可将 SS 用作 I/O 引脚
                        //0100 = SPI 从模式：时钟 = SCK 引脚；使能 SS 引脚控制
                        //0011 = SPI 主模式：时钟 = TMR2 输出 /2
                        //0010 = SPI 主模式：时钟 = F OSC /64
                        //0001 = SPI 主模式：时钟 = F OSC /16
                        //0000 = SPI 主模式：时钟 = F OSC /4
      
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
    while(!BF);
    return SSPBUF;
}

/*************************************************************************/
//函数名:SPI_ReadByte
//功  能:模拟SPI通信读时序
//参  数:无
//返回值：读取的数据
//时间：20180703
//创建者：ahren
/************************************************************************/
u8 SPI_ReadByte(void)
{
	u8 i,RDate=0;	
//	MCP2515_SCK = 0;
//	for(i=0;i<8;i++)
//	{
//		MCP2515_SCK = 1;
//		RDate <<= 1;
//		RDate |= MCP2515_MISO;
//		MCP2515_SCK = 0;	
//	}
//	return RDate;
    
    i = SPI_RWByte(0xff);
    return i;
}
//
///*************************************************************************/
////函数名:SPI_WriteByte
////功  能:模拟SPI通信写时序
////参  数:写的数据
////返回值：无
////时间：20180703
////创建者：ahren
///************************************************************************/
void SPI_WriteByte(u8 dt)
{
//	u8 i;		
//	for(i=0;i<8;i++)
//	{	
//		MCP2515_SCK = 0;
//		if((dt<<i) & 0x80)
//			MCP2515_MOSI = 1;
//		else
//			MCP2515_MOSI = 0;					
//		MCP2515_SCK = 1;
//	}
//	MCP2515_SCK = 0;
    
    SPI_RWByte(dt);
}

/*************************************************************************/
//函数名:MCP2515_WriteByte
//功  能:MCP2515写数据
//参  数:addr：地址, dat：数据
//返回值：无
//时间：20180703
//创建者：ahren
/************************************************************************/
void MCP2515_WriteByte(u8 addr,u8 dat)
{
	MCP2515_CS = 0;
	SPI_WriteByte(MCP2515_CAN_WRITE);	//写命令
	SPI_WriteByte(addr);		        //写寄存器地址
	SPI_WriteByte(dat);			        //写数据

    MCP2515_CS = 1;				
}

/*************************************************************************/
//函数名:MCP2515_ReadByte
//功  能:MCP2515读数据
//参  数:addr：地址
//返回值：数据
//时间：20180703
//创建者：ahren
/************************************************************************/
u8 MCP2515_ReadByte(u8 addr)
{
	u8 RData;	
	MCP2515_CS = 0;
	SPI_WriteByte(MCP2515_CAN_READ);		//读命令
	SPI_WriteByte(addr);		           	//写寄存器地址
	RData = SPI_ReadByte();	                //读数据
	MCP2515_CS = 1;	
	return RData;				
}

/*************************************************************************/
//函数名:MCP2515_Reset
//功  能:MCP2515软件复位
//参  数:无
//返回值：无
//时间：20180703
//创建者：ahren
/************************************************************************/
void MCP2515_Reset(void)
{
	MCP2515_CS = 0;				
	SPI_WriteByte(MCP2515_CAN_RESET);	
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
void MCP2515_CAN_BaudRate(void)
{
	MCP2515_WriteByte(MCP2515_CNF1,CAN_250Kbps);
	MCP2515_WriteByte(MCP2515_CNF2,0x80|MCP2515_PHSEG1_1TQ|MCP2515_PRSEG_1TQ);    	
	MCP2515_WriteByte(MCP2515_CNF3,MCP2515_PHSEG2_1TQ);			
}

/*************************************************************************/
//函数名:MCP2515_CAN_TxID
//功  能:MCP2515发送ID设置
//参  数:ID数组首地址，接收数组长度
//返回值：无
//时间：20180703
//创建者：ahren
/************************************************************************/
void MCP2515_CAN_TxID(u8 *ID, u8 len)
{
    u8 SIDH,SIDL;
    SIDH = (ID[0] << 3)|(ID[1] >> 5);
    SIDL = ((ID[1]&0X03)|0X08)|((ID[1]<<3)&0XE0);
    
	MCP2515_WriteByte(MCP2515_TXB0SIDH,SIDH); //发送缓冲寄存器的标识符设置（标准+扩展）
	MCP2515_WriteByte(MCP2515_TXB0SIDL,SIDL);
	MCP2515_WriteByte(MCP2515_TXB0EID8,ID[2]);
	MCP2515_WriteByte(MCP2515_TXB0EID0,ID[3]);
    MCP2515_WriteByte(MCP2515_TXB0DLC,len);			
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
    u8 SIDH,SIDL;
    SIDH = (ID[0] << 3)|(ID[1] >> 5);
    SIDL = ((ID[1]&0X03)|0X08)|((ID[1]<<3)&0XE0);
    
	MCP2515_WriteByte(MCP2515_RXB0SIDH,0x00);      //清空验收滤波寄存器标识符设置（标准+扩展）
	MCP2515_WriteByte(MCP2515_RXB0SIDL,0x00);
	MCP2515_WriteByte(MCP2515_RXB0EID8,0x00);
	MCP2515_WriteByte(MCP2515_RXB0EID0,0x00);
	MCP2515_WriteByte(MCP2515_RXB0CTRL,0x40);
	MCP2515_WriteByte(MCP2515_RXB0DLC,len);
	
	MCP2515_WriteByte(MCP2515_RXF0SIDH,SIDH);    //配置验收滤波寄存器标识符设置（标准+扩展）
	MCP2515_WriteByte(MCP2515_RXF0SIDL,SIDL);
	MCP2515_WriteByte(MCP2515_RXF0EID8,ID[2]);
	MCP2515_WriteByte(MCP2515_RXF0EID0,ID[3]);
#if 0
	MCP2515_WriteByte(MCP2515_RXM0SIDH,0xFF);    //配置验收屏蔽寄存器标识符设置（标准+扩展）
	MCP2515_WriteByte(MCP2515_RXM0SIDL,0xFF);
	MCP2515_WriteByte(MCP2515_RXM0EID8,0xFF);
	MCP2515_WriteByte(MCP2515_RXM0EID0,0x00);	
#endif
    MCP2515_WriteByte(MCP2515_RXM0SIDH,0xFF);    //配置验收屏蔽寄存器标识符设置（标准+扩展）
	MCP2515_WriteByte(MCP2515_RXM0SIDL,0xFF);
	MCP2515_WriteByte(MCP2515_RXM0EID8,0x00);
	MCP2515_WriteByte(MCP2515_RXM0EID0,0x00);
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
//    u8 MCP2515_TxID[4]={0x18,0xF0,0xF4,0x00};
//    u8 MCP2515_RxID[4]={0x0C,0xF0,0xF3,0x00};       //验证PC端模拟发送单帧   
    
//    TRISC5 = 0;              //端口设置
//    TRISC4 = 1;
//    TRISC3 = 0;
      
    SPI_Init();   
    delay_10ms(1);
   
    TRISC1 = 0;
    TRISC2 = 0;
    TRISC0 = 0;      
    
    MCP2515_RESET = 1;               //关闭硬件复位
    //MCP2515_SILENT = 0;              //  MCP2515使能
    MCP2515_SILENT_OFF;
    delay_10ms(1);
	MCP2515_Reset();                 //软件复位
    delay_10ms(1);
    MCP2515_CAN_BaudRate();          //波特率设置
	delay_10ms(1);
    MCP2515_CAN_TxID(MCP2515_TxID,8);        //发送ID设置 
    delay_10ms(1);
#if 1                                //
	MCP2515_CAN_RxID(MCP2515_RxID,8);        //接收ID设置
    delay_10ms(1);
#endif    
	MCP2515_WriteByte(MCP2515_CANINTE,0x01);                                        //MCP2515中断设置
	delay_10ms(1);
	MCP2515_WriteByte(MCP2515_CANCTRL,MCP2515_REQOP_NORMAL|MCP2515_CLKOUT_ENABLED);	//设置成正常模式
    delay_10ms(1);
	if(MCP2515_OPMODE_NORMAL != (MCP2515_ReadByte(MCP2515_CANSTAT) & 0xE0))         //确认是否设置成功
	{
		MCP2515_WriteByte(MCP2515_CANCTRL,MCP2515_REQOP_NORMAL|MCP2515_CLKOUT_ENABLED);
	}
    delay_10ms(1);
    MCP2515_WriteByte(MCP2515_CANINTF,0x00); 
}

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
	u8 j,dly;
    dly=0;

    //CAN_TX_Buf[0] = BOX_Num;
    //CAN_TX_Buf[6] = Life_Signal;
    memcpy(MCP2515_Buff, CAN_TX_Buf, 8);          //将 TC04_01_11_Buff 复制给TC04_One_Buff   

    MCP2515_Data_Analysis(BOX_Num, MCP2515_Buff, Life_Signal);

    while((MCP2515_ReadByte(MCP2515_TXB0CTRL)&0x08) && (dly<50))        //读取状态值，是否可以发送
    {			
        dly++;
    }

    for(j=0;j<len;j++)
    {
        MCP2515_WriteByte(MCP2515_TXB0D0 + j,MCP2515_Buff[j]);            //写入发送数据到发送缓冲区    
    }
    /********************************/
    //依据中车CAN报文协议修改
    //20190731
    //ahren  

    //MCP2515_WriteByte(MCP2515_TXB0D1,BOX_Num);//发送life        
    //MCP2515_WriteByte(MCP2515_TXB0D7,Life_Signal);//发送life      
    /********************************/    

    MCP2515_WriteByte(MCP2515_TXB0DLC, len);                            //写入发送的数据长度
    MCP2515_CS = 0;
    MCP2515_WriteByte(MCP2515_TXB0CTRL,0x08);                           //发送报文
    MCP2515_CS = 1;  
}

/*************************************************************************/
//函数名:MCP2515_CAN_Rx_Buffer
//功  能:MCP2515_CAN接收函数
//参  数:接收数组首地址
//返回值：接收字节长度
//时间：20180703
//创建者：ahren
/************************************************************************/
u8 MCP2515_CAN_Rx_Buffer(u8 *CAN_RX_Buf)
{
	u8 i=0,len=0,temp=0;
    
    u8 *reg=CAN_RX_Buf ;
    
	temp = MCP2515_ReadByte(MCP2515_CANINTF);      //读取中断标志位，是否接收完成
	if(temp & 0x01)
	{

		for(i=0;i<8;i++)
		{	

            *(reg + i) = MCP2515_ReadByte(MCP2515_RXB0D0+i);
            
		}             
	}    
            
    MCP2515_WriteByte(MCP2515_CANINTF,0); 

	return len;
}

/*************************************************************************/
//函数名:MCP2515_Enter_Sleep_Mode
//功  能:MCP2515进入睡眠模式
//参  数:
//返回值：
//时间：20180703
//创建者：ahren
/************************************************************************/
u8 MCP2515_Enter_Sleep_Mode(void)
{
	u8 time_out = 0;
	MCP2515_WriteByte(MCP2515_CANCTRL, MCP2515_REQOP_SLEEP);                //设置成睡眠模式
    //delay_10ms(1);
    NOP();NOP();NOP();NOP();
	
	while(MCP2515_OPMODE_SLEEP != (MCP2515_ReadByte(MCP2515_CANSTAT) & 0xE0))//确认是否设置成功
	{
//		delay_10ms(1);
        NOP();NOP();NOP();NOP();        
		time_out++;
		if(time_out>100)//超时处理
		{
			return 0;
		}		
	}
	return 1;	
}


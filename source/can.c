/*----------------------------------------------------------------------*/
/* Confidential and Proprietary Information of MicroPerfect Corporation */
/* (C) 2017 ,MicroPerfect Corporation . All rights reserved. */
/* FileName : can.c */
/* Function : */
/* Ver: 0.01  */
/* MP_Ahren NewFile */
/* History:YYYY.MM.DD Version author Description 
          :2017.12.04 0.01 @huajifeng Follow the coding standard  */
/*-------------------------------------------------------------*/

/***************头文件****************/
/**/
/*************************************/
#include "can.h"

//******************宏定义*************/
/**/
/*************************************/


/***************变量定义************/
/**/
/*************************************/

static u8 Send_DLC = 0;

/***************函数申明**************/
/**/
/*************************************/

static void Clear_CAN_TX0_Buf(unsigned char CAN_TX0_Data);    //设置CAN_TX0_Buf值
static void Clear_CAN_RX0_Buf(unsigned char CAN_RX0_Data);    //设置CAN_TX0_Buf值
static void CAN_Port_Set(void);                               //CAN I/O 数据方向设置
static void CAN_Baud_Rate_Set(void);                          //波特率设置函数
static void CAN_Interrupt_Mode_Set(void);     //   初始化CAN中断、CAN模块操作模式选择
static void CAN_TXD_Extended_Identifier(void);//  发送扩展标识符设置（即发送ID）
static void CAN_RXD_Extended_Identifier(void);//  接收扩展标识符设置、接收扩展标识符屏蔽设置
static void CAN_Config_Mode(void);
/*************************************/

/*************************************************************************/
//函数名:Clear_CAN_TX0_Buf
//功  能:设置CAN_TX0_Buf值
//参  数:CAN_TX0_Data
//返回值：无
//时间：20171123
//创建者：ahren
/************************************************************************/
static void Clear_CAN_TX0_Buf(unsigned char CAN_TX0_Data)
{	
	unsigned char Count = 0;
	
	unsigned char Data  = 0;
	
	Data = CAN_TX0_Data;
	
	for(Count=0; Count<9; Count++)
	{
	  	CAN_TX0_Buff[Count]= Data; 						//将CAN_TX0_Buf赋值相应的值
	}
}

/*************************************************************************/
//函数名:Clear_CAN_TX0_Buf_New
//功  能:设置CAN_TX0_Buf值
//参  数:CAN_TX0_Data，Send_Num
//返回值：无
//时间：20171123
//创建者：ahren
/************************************************************************/
void Clear_CAN_TX0_Buf_New(unsigned char CAN_TX0_Data,unsigned char Send_Num)
 {	
	unsigned char Count = 0;
 	
 	unsigned char Data  = 0;
 	
 	unsigned char Num   = 0;
 	
 	Data = CAN_TX0_Data;								//CAN要发送的数据
 	
 	Send_DLC = Send_Num;								//CAN发送数据的长度
 	
 	for(Count=0; Count<Send_Num; Count++)
	{
      	CAN_TX0_Buff[Count] = Data; 					//将CAN_TX0_Buf赋值相应的值
	}
}

 /*************************************************************************/
//函数名:Clear_CAN_RX0_Buf
//功  能:设置CAN_RX0_Buf值
//参  数:CAN_RX0_Data
//返回值：无
//时间：20171123
//创建者：ahren
/************************************************************************/
static void Clear_CAN_RX0_Buf(unsigned char CAN_RX0_Data) 
 {
	unsigned char Count = 0;
 	
 	unsigned char Data  = 0;
 	
 	Data = CAN_RX0_Data;	
 	
 	for(Count=0; Count<9; Count++)
	{
      	CAN_RX0_Buff[Count] = Data; 					//将CAN_RX0_Buf赋值相应的值
	}
}
 
 /*************************************************************************/
//函数名:CAN_Send_Receive_TXD_ID_Set
//功  能:设置接收过滤器ID值
//参  数:Channel_Flag = 1时，用于选择设置发送过滤器0
//参  数:Channel_Flag = 2时，用于选择设置发送过滤器1
//参  数:Channel_Flag = 3时，用于选择设置接收过滤器1
//参  数:RX_TX_SID_H寄存器SID高八位
//参  数:RX_TX_SID_L寄存器SID低八位
//参  数:RX_TX_EID_H寄存器EID高八位
//参  数:RX_TX_EID_L寄存器EID低八位
//返回值：无
//时间：20171123
//创建者：ahren
/************************************************************************/
static void CAN_Send_Receive_TXD_ID_Set(u8 Channel_Flag,u8 RX_TX_SID_H,u8 RX_TX_SID_L,u8 RX_TX_EID_H,u8 RX_TX_EID_L)
{
	u8 ID_H_Data = 0x00;
	u8 ID_L_Data = 0x00;
	
	u8 ID_L = 0x00;
	u8 ID_H = 0x00;
	
	ID_H_Data = RX_TX_SID_H;
	ID_L_Data = RX_TX_SID_L;
	
	ID_L = (ID_L_Data & 0xF0) >> 5;
	ID_H = (ID_H_Data << 3) + ID_L;				//设置过滤器SIDH
	
	ID_L = (ID_L_Data & 0x1C) << 3;
	ID_L = (ID_L_Data & 0x03) + ID_L + 8;		//设置过滤器SIDL(扩展帧)	
	
	if(1 == Channel_Flag)						//设置接收过滤器0ID
	{
		RXF0SIDH = ID_H;		
		RXF0SIDL = ID_L;
		RXF0EIDH = RX_TX_EID_H;
	//	RXF0EIDL = RX_TX_EID_L;		
	}
	
	if(2 == Channel_Flag)						//设置接收过滤器1ID
	{
		RXF1SIDH = ID_H;		
		RXF1SIDL = ID_L;
		RXF1EIDH = RX_TX_EID_H;
	//	RXF1EIDL = RX_TX_EID_L;	
	}
	
	if(3 == Channel_Flag)						//设置发送过滤器ID
	{
		TXB0SIDH = ID_H;
		TXB0SIDL = ID_L;
		TXB0EIDH = RX_TX_EID_H;
		TXB0EIDL = RX_TX_EID_L;		
	}				
}

/*************************************************************************/
//函数名:CAN_Port_Set
//功  能:CAN I/O 数据方向设置
//参  数:无
//返回值：无
//时间：20171123
//创建者：ahren
/************************************************************************/
static void CAN_Port_Set(void)
{
    TRISB5 = 0;			             //RG4端口输出			
	LATB5 = 1;						//禁用CAN使能PIN
    
	TRISE5 = 0;						//CANTX/RE5为输出
		
	TRISE4 = 1;						//CANRX/RE4为输入
}

/*************************************************************************/
//函数名:CAN_Baud_Rate_Set
//功  能:CAN波特率设置(设置寄存器)
//参  数:无
//返回值：无
//时间：20171123
//创建者：ahren
/************************************************************************/
static void CAN_Baud_Rate_Set(void)
{
 #if 0  
    /****************************************************************/	
	//4M
    /****************************************************************/
 // BRGCON1=0X01;					//Sync_Seg(bit7-6)=1TQ//设置SJW和BRP，SJW=1TQ，BRP=0x01,波特率为 125Kbps
    BRGCON1=0X00;					//Sync_Seg(bit7-6)=1TQ//设置SJW和BRP，SJW=1TQ，BRP=0x00,波特率为 250Kbps   
	BRGCON2=0X90;					//设置Phase_Seg1=3TQ和Prog_Seg=1TQ	
	BRGCON3=0X42;					//设置Phase_Seg2=3TQ
									//TQ=((2*(BRP+1))/Fosc=4/4M=1us 
									//TQ=((2*(BRP+1))/Fosc=2/4M=0.5us 	
									//位时间＝TQ*(Sync_Seg+ Prop_Seg+ Phase_seg1+ Phase_seg2)=(1+1+3+3)TQ＝8TQ, 
									//位率=1/(8*1us)=125Kbps 
									//位率=1/(8*0.5us)=250Kbps 
									//设置发送邮箱 0 的标识符和发送的数据，以及发送的数据长度	
									
	TXB0CON=0X03;					//发送优先级为最高优先级，TXPRI=11
#endif
    /****************************************************************/	
	//8M
    /****************************************************************/
     BRGCON1=0X01;					//Sync_Seg(bit7-6)=1TQ//设置SJW和BRP，SJW=1TQ，BRP=0x01,波特率为 250Kbps
//    BRGCON1=0X00;					//Sync_Seg(bit7-6)=1TQ//设置SJW和BRP，SJW=1TQ，BRP=0x00,波特率为 500Kbps   
	BRGCON2=0X90;					//设置Phase_Seg1=3TQ和Prog_Seg=1TQ	
	BRGCON3=0X42;					//设置Phase_Seg2=3TQ
									//TQ=((2*(BRP+1))/Fosc=4/8M=0.5us 
									//TQ=((2*(BRP+1))/Fosc=2/8M=0.25us 	
									//位时间＝TQ*(Sync_Seg+ Prop_Seg+ Phase_seg1+ Phase_seg2)=(1+1+3+3)TQ＝8TQ, 
									//位率=1/(8*0.5us)=250Kbps 
									//位率=1/(8*0.25us)=500Kbps 
									//设置发送邮箱 0 的标识符和发送的数据，以及发送的数据长度	
									
	TXB0CON=0X03;					//发送优先级为最高优先级，TXPRI=11
}

/*************************************************************************/
//函数名:CAN_TXD_Extended_Identifier
//功  能:发送扩展标识符设置（即发送ID）
//参  数:无
//返回值：无
//时间：20171123
//创建者：ahren
/************************************************************************/
static void CAN_TXD_Extended_Identifier(void)
{
    /****************************************/	
    //添加CAN ID设置初始化功能                        
    //ahren
    //20190719
    
    u8 CAN_ID_SIDH = 0;
    u8 CAN_ID_SIDL = 0;
    u8 CAN_ID_EIDH = 0;
    u8 CAN_ID_EIDL = 0;    

    CAN_ID_Flag = eeprom_read(0x09);//CAN默认ID被修改标志位
    NOP();NOP();NOP();NOP();      
    //if(CAN_ID_Flag == 0x5A)
    if(0x5A == CAN_ID_Flag)
    {
        CAN_ID_SIDH = eeprom_read(0x04);                        
        NOP();NOP();NOP();NOP();
        CAN_ID_SIDL = eeprom_read(0x05);                     
        NOP();NOP();NOP();NOP();  
        CAN_ID_EIDH = eeprom_read(0x06);                        
        NOP();NOP();NOP();NOP();
        CAN_ID_EIDL = eeprom_read(0x07);                        
        NOP();NOP();NOP();NOP();    
        
		#if 0
        CAN_Send_Receive_TXD_ID_Set(0x03,CAN_ID_SIDH,CAN_ID_SIDL,CAN_ID_EIDH,CAN_ID_EIDL);  
		#endif
        /****************************************/	
        //初始化MCP2515 发送ID                      
        //ahren
        //20190722
        MCP2515_TxID[0] = CAN_ID_SIDH;
        MCP2515_TxID[1] = CAN_ID_SIDL;
        MCP2515_TxID[2] = CAN_ID_EIDH;
        MCP2515_TxID[3] = CAN_ID_EIDL; 
        /****************************************/	        
    }
    else
    {
        CAN_Send_Receive_TXD_ID_Set(0x03,0x18,0xFF,0x74,0x1D);    
    }
    /*************************************/	  
	TXB0D0=0X00;												
	TXB0D1=0X00;
	TXB0D2=0X00;
	TXB0D3=0X00;
	TXB0D4=0X00;
	TXB0D5=0X00;
	TXB0D6=0X00;
	TXB0D7=0X00;
}

/*************************************************************************/
//函数名:CAN_RXD_Extended_Identifier
//功  能:接收扩展标识符设置、接收扩展标识符屏蔽设置
//参  数:
//返回值：无
//时间：20171123
//创建者：ahren
/************************************************************************/
static void CAN_RXD_Extended_Identifier(void)
{
	/****************************************************/
	//接收扩展标识符屏蔽设置
 	/****************************************************/	

	RXB0CON=0x40;  												// bit6-5=10//仅接收扩展标识符的有效信息，FILHIT0=0表示RXB0 ，采用FILHIT0      
#if 0
	RXM0SIDH = 0xff;		
	RXM0SIDL = 0xff;											//高3位为标识符低3位，低5位是配置
	RXM0EIDH = 0x00;
	RXM0EIDL = 0x00;	
#endif 
    RXM0SIDH = 0xFF;		
	RXM0SIDL = 0xFF;											//高3位为标识符低3位，低5位是配置， bit3 EXIDEN：接收由 RXFnSIDL 中的 EXIDEN 位选择的报文
	RXM0EIDH = 0x00;
	RXM0EIDL = 0x00;	
	/****************************************************/
	//接收扩展标识符设置（即接收ID）
 	/****************************************************/ 
 	
 	CAN_Send_Receive_TXD_ID_Set(0x01,0x18,0xFF,0x00,0x00);			//设置接收过滤器0ID
	CAN_Send_Receive_TXD_ID_Set(0x02,0X04,0x02,0x00,0x00);			//设置接收过滤器1ID				
	
	//RXB0DLC = 0X08;											//设置接收数据的长度为1个字节
	RXB0D0=0X00;												//初始化接受缓冲器0的数据区数据
	RXB0D1=0X00;
	RXB0D2=0X00;
	RXB0D3=0X00;
	RXB0D4=0X00;
	RXB0D5=0X00;
	RXB0D6=0X00;
	RXB0D7=0X00;	
}

/*************************************************************************/
//函数名:CAN_Interrupt_Mode_Set
//功  能:初始化CAN中断、CAN模块操作模式选择
//参  数: 寄存器
//返回值：无
//时间：20171123
//创建者：ahren
/*************************************************************************/
static void CAN_Interrupt_Mode_Set(void)
{
	CIOCON=0X21;												//初始化CAN模块的I/O控制寄存器	 	
 	
 	CANCON=0X00;												//进入正常操作模式
	while((CANSTAT&0x40)!=0); 									//检测配置完成	

	PIR5=0X00;													//清所有中断标志
	PIE5=0X01;													//使能接收缓冲器0、1 的接收中断（仅打开接收中断使能）
}

/*************************************************************************/
//函数名:CAN_Send_8Byte_Data
//功  能:CAN发送函数
//参  数:
//返回值：无
//时间：20171123
//创建者：ahren
/************************************************************************/
void CAN_Send_8Byte_Data(void)  							//邮箱0发送DLC长度的报文
{
	TXB0CON = (TXB0CON&0xf7);				//TXREQ清0首先，禁止发送	
	TXB0IF = 0;								//清空发送完成标志		
	TXB0DLC = 0x08;							//设置发送数据的长度为8个字节
	//TXB0DLC = Send_DLC;					//发送长度根据数据长度而定
	
	TXB0D0 = CAN_TX0_Buff[0];				//载入等待发送数据
	TXB0D1 = CAN_TX0_Buff[1];
	TXB0D2 = CAN_TX0_Buff[2];
	TXB0D3 = CAN_TX0_Buff[3];
	TXB0D4 = CAN_TX0_Buff[4];
	TXB0D5 = CAN_TX0_Buff[5];
	TXB0D6 = CAN_TX0_Buff[6];
	TXB0D7 = CAN_TX0_Buff[7];
	
	TXB0CON=(TXB0CON|0x08);					//准备完毕，请求发送,TXREQ=1
}

/*************************************************************************/
//函数名:TJA1042_CAN_Send_Data
//功  能:CAN发送函数
//参  数:Box_Num:电池箱编号
//参  数:Data_Buff:发送的数据报文
//参  数:Data_Leng:发送的数据长度
//返回值：无
//时间：20190606
//创建者：ahren
/************************************************************************/
void TJA1042_CAN_Send_Data(u8 Box_Num, u8 *Data_Buff, u8 Data_Leng,u8 Life_Signal)
{
    u8 i = 0;

	TXB0CON = (TXB0CON&0xf7);				//TXREQ清0首先，禁止发送	
	TXB0IF = 0;								//清空发送完成标志		

	TXB0DLC = Data_Leng;							//设置发送数据的长度为8个字节
    
    TXB0D0 = Box_Num;
    //Data_Buff[6] = Life_Signal;
	for(i=1; i<Data_Leng; i++)
	{	
		*(&TXB0D0 + i) = Data_Buff[i];				
	}  
    TXB0D6 = Life_Signal;    
	TXB0CON=(TXB0CON|0x08);					//准备完毕，请求发送,TXREQ=1    
}

/*************************************************************************/
//函数名:CAN_Send_Unfixed_Data
//功  能:根据接收的数据长度发送相应长度的数据函数
//参  数:
//返回值：无
//时间：20171123
//创建者：ahren
/************************************************************************/
void CAN_Send_Unfixed_Data(void)
{
	TXB0CON = (TXB0CON&0xf7);				//TXREQ清0首先，禁止发送	
	TXB0IF = 0;								//清空发送完成标志		
						
	TXB0DLC = RXB0DLC;						//设置发送数据的长度与接收到的数据长度一样
	
	TXB0D0 = CAN_TX0_Buff[0];				//载入等待发送数据
	TXB0D1 = CAN_TX0_Buff[1];
	TXB0D2 = CAN_TX0_Buff[2];
	TXB0D3 = CAN_TX0_Buff[3];
	TXB0D4 = CAN_TX0_Buff[4];
	TXB0D5 = CAN_TX0_Buff[5];
	TXB0D6 = CAN_TX0_Buff[6];
	TXB0D7 = CAN_TX0_Buff[7];
	
	TXB0CON =(TXB0CON|0x08);				//准备完毕，请求发送,TXREQ=1
}

/*************************************************************************/
//函数名:CAN_Send_DCL_Data
//功  能:发送DLC长度的报文
//参  数:
//返回值：无
//时间：20171123
//创建者：ahren
/************************************************************************/
void CAN_Send_DCL_Data(unsigned char CAN_TX_Buf)  	
{
	unsigned char i;
	//TXB0REQ=0;						 	//首先，禁止发送
	TXB0CON=(TXB0CON&0xf7);		 			//TXREQ清0首先，禁止发送
	
	TXB0IF=0;								//清空发送完成标志	
		
	TXB0DLC=0x01;
	
	TXB0D0=CAN_TX_Buf;
	
	//TXB0REQ=1;							//准备完毕，请求发送,TXB0REQ=1
	TXB0CON=(TXB0CON|0x08);					//准备完毕，请求发送,TXREQ=1
}

/*************************************************************************/
//函数名:CAN_Receive_Data
//功  能:CAN接收不同长度的数据
//参  数:
//返回值：无
//时间：20171123
//创建者：ahren
/************************************************************************/
void CAN_Receive_Interrupt_Data(unsigned char *CAN_RX_Buf)
{
	unsigned char Leng_Num = 0;
	
	unsigned char DCL_Num = RXB0DLC;
	
	for(Leng_Num=0; Leng_Num<DCL_Num; Leng_Num++)
	{	
		CAN_RX_Buf[Leng_Num] = *(&RXB0D0 + Leng_Num);				
	}

	RXFUL=0;					    		//打开接收缓冲器来接收新信息	
}

/*************************************************************************/
//函数名:CAN_Life_Signal
//功  能:CAN心跳包，从0x00递增到0xFF,
//参  数:
//返回值：无
//时间：20171123
//创建者：ahren
/************************************************************************/
void CAN_Life_Signal(void)
{
	CAN_TX0_Buff[7] = CAN_Life_Signal_Count;	
 
    CAN_Life_Signal_Count++;
	
	if(CAN_Life_Signal_Count > 0xFF)
	{
		CAN_Life_Signal_Count = 0x00;
	}		
}

/*************************************************************************/
//函数名:CAN_RXD_TXD_Test
//功  能:CAN接收发送通信测试
//参  数:
//返回值：无
//时间：20171123
//创建者：ahren
/************************************************************************/
void CAN_RXD_TXD_Test(void)
{
	unsigned char Count = 0;
	
	for(Count=0; Count<8; Count++)
	{
		CAN_TX0_Buff[Count] = CAN_Receive_Data[Count];
	}	
}

/*************************************************************************/
//函数名:CAN_RXD_TXD_Test
//功  能:CAN 配置模式
//参  数:
//返回值：无
//时间：20171127
//创建者：huajifeng 
/************************************************************************/
static void CAN_Config_Mode(void)
{
    CANCON=0X80;					//请求进入CAN配置模式REQOP=100	(控制寄存器)
	while((CANSTAT&0X80)==0);		//等待进入CAN配置模式OPMODE=100(状态寄存器)	
}

/*************************************************************************/
//函数名:CAN_Init
//功  能:CAN初始化函数
//参  数:
//返回值：无
//时间：20171123
//创建者：ahren
/************************************************************************/
void CAN_Init()
{	
	Clear_CAN_TX0_Buf(0x00);	      //清空CAN_TX0_BUF的	
	Clear_CAN_RX0_Buf(0x00);
    
	/****************************************************/
	//CAN I/O 初始化
 	/****************************************************/
 		
	CAN_Port_Set();					//调用CAN接收发送I/O方向设置	

	/****************************************************/
	//进入配置模式
 	/****************************************************/
	
	CAN_Config_Mode();                           // 等待进入配置模式
	
	/****************************************************/
	//CAN波特率设置
 	/****************************************************/

	CAN_Baud_Rate_Set();			   //调用CAN波特率设置函数	

	/****************************************************/
	//发送扩展标识符设置
 	/****************************************************/		

	CAN_TXD_Extended_Identifier();	
	
	/****************************************************/
	//接收扩展标识符设置、接收扩展标识符屏蔽设置
 	/****************************************************/
 	
	CAN_RXD_Extended_Identifier();
 		
	/****************************************************/
	//初始化CAN中断、CAN模块操作模式选择
 	/****************************************************/
 	
	CAN_Interrupt_Mode_Set();

 	/****************************************************/
	
	CAN_STB_OFF;												//关闭CAN使能PIN
	
 	/****************************************************/	
}

/*************************************************************************/
//函数名:CAN_Send_8Byte_Data
//功  能:CAN发送函数
//参  数:
//返回值：无
//时间：20171123
//创建者：ahren
/************************************************************************/
void CAN_Send_8Byte_Data_MCP_R(u8 *arr)  							//邮箱0发送DLC长度的报文
{
	TXB0CON = (TXB0CON&0xf7);				//TXREQ清0首先，禁止发送	
	TXB0IF = 0;								//清空发送完成标志		
	TXB0DLC = 0x08;							//设置发送数据的长度为8个字节
	//TXB0DLC = Send_DLC;					//发送长度根据数据长度而定
	
	TXB0D0 = arr[0];				//载入等待发送数据
	TXB0D1 = arr[1];
	TXB0D2 = arr[2];
	TXB0D3 = arr[3];
	TXB0D4 = arr[4];
	TXB0D5 = arr[5];
	TXB0D6 = arr[6];
	TXB0D7 = arr[7];
	
	TXB0CON=(TXB0CON|0x08);					//准备完毕，请求发送,TXREQ=1
}

/*************************************************************************/
//函数名:CAN_Data_Integration
//功  能:
//参  数:
//返回值：无＋
//时间：20171123
//创建者：ahren
/************************************************************************/
void CAN_Data_Integration(void)
{
    CAN_TX0_Buff[0] = 0xFF;
    CAN_TX0_Buff[1] = 0xFF;
    CAN_TX0_Buff[2] = 0xFF ;            
    CAN_TX0_Buff[3] = 0xFF;            
    CAN_TX0_Buff[4] = 0xFF;       
    CAN_TX0_Buff[5] = 0xFF;         
    CAN_TX0_Buff[6] = 0xFF; 
}

/*************************************************************************/
//函数名:CAN_Send_Alarm_Warning_Data
//功  能:CAN发送模拟数据函数
//参  数:
//返回值：无
//时间：20180822
//创建者：Chenlf
/************************************************************************/
void CAN_Send_Alarm_Warning_Data(void)
{
	if(1 ==MCU_CAN_Send_1s_Flag_g)		//CAN 1s发一次
	{		
		MCU_CAN_Send_1s_Flag_g = 0;
		
		CAN_Data_Integration();     //CAN接收报文的数据Byte[0]、[1]、[2]、[3]、[4]、[5]、[7]           
        
        CAN_Life_Signal();          //心跳包，CAN接收Byte[7]循环计数
        
        CAN_Send_8Byte_Data();		//测试用，MCU CAN循环发数据	
	}  
}

/*************************************************************************/
//函数名:CAN_Interrupt_Occurred
//功  能:发生接收中断，处理接收的数据
//参  数:
//返回值：无
//时间：20171123
//创建者：ahren
/************************************************************************/
void CAN_Interrupt_Occurred(void)
{
    if(1 == CAN_RXD_Interrupt_Flag_g)           //250ms中断一次
	{			
		CAN_RXD_Interrupt_Flag_g = 0;	
		
		CAN_Receive_Interrupt_Data(CAN_Receive_Data);
		
		CAN_RXD_TXD_Test();    
	}
}
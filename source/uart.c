/*----------------------------------------------------------------------*/
/* Confidential and Proprietary Information of MicroPerfect Corporation */
/* (C) 2017 ,MicroPerfect Corporation . All rights reserved. */
/* FileName : uart.c */
/* Function : */
/* Ver: 0.01  */
/* MP_Ahren NewFile */
/* History:YYYY.MM.DD Version author Description 
          :2017.12.04 0.01 @huajifeng Follow the coding standard  */
/*-------------------------------------------------------------*/

/***************头文件****************/
/**/
/*************************************/
#include"uart.h"
//******************宏定义*************/
/**/
/*************************************/


/*************************************/

/***************标志位申明************/
/**/
/*************************************/
u8 Uart_Rece_Data[Uart_Rece_Num]    = (0x00);                 //接收字节缓冲区
bit UART_Rece_Interrupt_Flag_g = 0;
u8 receive_num = 0;     //总共收到的字节数

/***************函数申明**************/
/**/
/*************************************/


/*****************************************************/
//?????
//TXSTAx:
//CSRC   TX9   TXEN   SYNC   SENDB   BRGH   TRMT  TX9D
//	|	  |      |     |       |      |       |    |
//	x	  0      1     0       0      1       0    0
/*****************************************************/
//RCSTAx:
//SPEN   RX9   SREN   CREN   ADDEN   FERR   OERR  RX9D
//	|	  |      |     |       |      |       |    |
//	1	  0      ?     1       ?      ?       ?    ?
/*****************************************************/
//BAUDCONx:
//ABDOVF   RCIDL   RXDTP   TXCKP   BRG16   ---   WUE  ABDEN
//	|	     |       |       |       |      |     |    |
//	0	     0       0       0       1      0     0    0
/*****************************************************/

/****************************************************************/
//名称；Uart初始化函数
//功能:    初始化管教，寄存器操作
//参数：Port Pin、串口使能、波特率
//返回值:无
//时间:  20171127    
//创建者:MP_Ahren
/***************************************************************/
void Uart1_Init()
{
	TRISG0 = 1;                     // RG0   设置为输入
	TRISG3 = 0;                     // RG3   设置为输出
	
	LATG0 = 0;                      //低电平使能
	LATG3 = 0;

	Uart_TX1 = 0;					//设置TX1 Pin脚为输出口
	Uart_RX1 = 1;					//设置RX1 Pin脚为输入口

    //寄存器控制操作	
	TXSTA1 = 0x24;				    //8bit数据格式发送、使能发送、异步通信、高速波特率.TXEN = 1\SYNC = 0\BRGH = 1;
	RCSTA1 = 0x90;			        //使能串口工作、8bit数据格式接收、使能接收.SPEN = 1\RX9 = 0\CREN = 1;
	BAUDCON1 = 0x08;		  	    //BRG16=1 16位定时器

    //波特率设置
	SPBRGH1 = 0x00;
	SPBRG1 = 0x19;					//FOSC/[4 (n+1)]				
									//SYNC =  0 , BRGH =  1 BRG16=1 FOSC=4MHz  
									//4000000/[4(X+1)]=38400  --->X=25	
}

/****************************************************************/
//名称；Uart初始化函数
//功能:    初始化管教，寄存器操作
//参数：Port Pin、串口使能、波特率
//返回值:无
//时间:  20171127    
//创建者:MP_Ahren
/***************************************************************/

void Uart2_Init()
{
    TRISE6 = 1;                     // RE6     设置为输入
	TRISE7 = 0;                     // RE7     设置为输出
	
	LATE6 = 0;                      //低电平使能
	LATE7 = 0;

	Uart_TX2 = 0;					//设置TX1 Pin脚为输出口
	Uart_RX2 = 1;					//设置RX1 Pin脚为输入口

    //寄存器控制操作	
	TXSTA2 = 0x24;				    //8bit数据格式发送、使能发送、异步通信、高速波特率.TXEN = 1\SYNC = 0\BRGH = 1;
	RCSTA2 = 0x90;			        //使能串口工作、8bit数据格式接收、使能接收.SPEN = 1\RX9 = 0\CREN = 1;
	BAUDCON2 = 0x08;		  	    //BRG16=1 16位定时器

    //波特率设置
	SPBRGH2 = 0x00;
	SPBRG2 = 0x67;					//FOSC/[4(n+1)]				
									//SYNC =  0 , BRGH =  1 BRG16=1 FOSC=4MHz  
									//4000000/[4(X+1)]=38400  --->X=25	
}


/*****************************************************************/
//名称；Uart1_Send_Byte
//功能: 串口发送具体几个字节
//参数：字节数，具体字节
//返回值:无
//时间:  20171103    
//创建者:huajifeng
/******************************************************************/
/*20180716*/
/*
void Uart1_Send_Byte(u8 Uart1_number, u8 *Uart1_data)
{	
	int8  num = 0;		          
	
	for(num = 0;num < Uart1_number;num++)
	{
		TXREG1=Uart1_data[num];
		while(TRMT1==0); 
		//TXEN1 = 0;
	}	  	
}
*/
/*20180712*/
/**************************************/
//名称；Uart1发送数据
//参数：发送测试数据,MCU（下位机）发数据给串口（上位机）
/**************************************/
void Uart1_Send_Byte_1(uint16 Send_Data)
{
	u8 Send_Data_H;
	u8 Send_Data_L;
	Send_Data_H=((Send_Data&0xff00)>>8); 	//发送数据的高8位：取决于发送数据取高8位然后右移8位,&是bit与，&&是表达式与
	Send_Data_L=(Send_Data&0x00ff);		//发送数据的低8位：取决于发送数据取低8位
	
	TXREG1=Send_Data_H;  	//EUSART1 发送寄存器缓冲器
	while(TRMT1==0);		//EUSART1 发送移位寄存器状态位,TSR满了，则跳出while
	
	TXREG1=Send_Data_L;  	//EUSART1 发送寄存器缓冲器
	while(TRMT1==0);		//EUSART1 发送移位寄存器状态位,TSR满了，则跳出while		
}

/********************************************************************/
//名称；Uart1_Interrupt_Receive_Byte
//功能: 串口中断接收字节，可根据需要设置几个字节，默认8个
//参数：接收到的字节
//返回值:无
//时间:  20171103    
//创建者:huajifeng
/********************************************************************/
void Uart1_Interrupt_Receive_Byte()
{   
	if(1 == UART_Rece_Interrupt_Flag_g)
	{
		UART_Rece_Interrupt_Flag_g = 0;
		
		Uart1_Send_Byte(Uart_Rece_Num,Uart_Rece_Data);        //串口发送接收到的字节
	}         
}



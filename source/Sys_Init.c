/*----------------------------------------------------------------------*/
/* Confidential and Proprietary Information of MicroPerfect Corporation */
/* (C) 2017 ,MicroPerfect Corporation . All rights reserved. */
/* FileName : sys_init.c */
/* Function : */
/* Ver: 0.01  */
/* MP_Ahren NewFile */
/* History:YYYY.MM.DD Version author Description 
          :2017.12.04 0.01 @huajifeng Follow the coding standard  */
/*-------------------------------------------------------------*/

/*************************************/
/*头文件*/
/*************************************/
#include"Sys_Init.h"
/*************************************/

//******************宏定义*************/
/**/
/*************************************/


/*************************************/

/***************标志位申明************/
/**/
/*************************************/


/***************函数申明**************/
/**/
void Peripheral_Mode_Init(void);
/*************************************/

/****************************************************************/
//函数名:Sys_Port_Init
//功  能:系统 I/O、AD等功能初始化
//参  数：无
//返回值:无
//时间:  20180705    
//创建者:MP_Bin
/***************************************************************/
void Sys_Port_Init()
{
    /*********Port_A_Init**********************/
    /*RA0,RA1,RA2,RA3,RA6,RA7 as NC Output And Out L*/ 
    /*RA5 as Input*/
    /*****************************************/  	  	
    TRISA = 0x00;    
    LATA  = 0x00;						
 
    /*********Port_B_Init**********************/
    /*RB0 as Output And Out H */
	/*RB1，RB2，RB4 as Interrupt InputPort*/
   	/*RB5、RB6、RB7 as NC Output And Out L*/   
    /*****************************************/    	
//    TRISB = 0x16;
    TRISB = 0x0F;
    LATB  = 0x2D;		
    
    /**********Set_Port_C_Init********************/
	/*RC0 as NC Output And Out L*/ 
    /*RC4、RC7 as Input*/
    /*RC1、RC2 as Output And Out H*/    
   	/*RC3、RC5、RC6 as Output And Out L */	 
   	/********************************************/	
   	TRISC = 0x10;       //设置输入输出的方向
 	LATC  = 0x07;        //
    
     /**********Set_Port_D_Init********************/
    /*RD4 as Input*/ 
   	/*RD2、RD3、RD5、RD6、RD7 as Output And Out L */
    /*RD0、RD1 as NC Output And Out L*/ 
   	/********************************************/    	
   	TRISD = 0x00;
   	LATD  = 0x00;   
   	
    /**********Set_Port_E_Init********************/
	/*RE0---RE7 as Output*/     
   	/********************************************/    
   	TRISE = 0x12;
   	LATE  = 0x08; 
    
    /**********Set_Port_F_Init********************/
	/*RF0---RF7 as Output*/   
   	/********************************************/    
   	TRISF = 0x00;
   	LATF  = 0x06;
  	
     /**********Set_Port_G_Init********************/
	/*RG0---RG7 as Output*/    
   	/********************************************/ 
   	TRISG = 0x00;
   	LATG  = 0x00;  	 	  	  			
}

/*************************************************************************/
//函数名:BU_ON_Sys_Init
//功  能:BU上电系统初始化函数
//参  数:无
//返回值:无
//时间:  20180818 
//创建者:ahren 
/************************************************************************/
void BU_ON_Sys_Init(void)
{
    Sys_Port_Init();//I/O口初始化函数
    delay_10us(20);

    Sys_AD_Function_Init();  //AD初始化函数
    delay_10us(20); 
    
	All_INT_Init();//调用外部中断初始化函数
	delay_10us(20);
		
	All_Time_Init(); //调用定时器初始化函数
	delay_10us(20);  
    
    Total_Interruput_Switch();//调用中断总开关初始化函数(最后开启总中断)
 	delay_10us(20);  

    Power_Det_Port_Init();//电源检测初始化函数
	delay_10us(20);     
    
//	Peripheral_Mode_Init();	
//	delay_10us(20);  
    
    MCP2515_Init();
    delay_10us(20);     
}

/*************************************************************************/
//函数名:Sys_Init
//功  能:系统初始化函数
//参  数:无
//返回值:无
//时间:  20180408  
//创建者:ahren 
/************************************************************************/
//修改记录：
//1:修改函数名称，由Sys_Init变更为，ACC_ON_Sys_Init
//2:删除I/O口初始化函数、AD初始化函数
//时间：20180818 @ahren
/************************************************************************/
void ACC_ON_Sys_Init(void)
{   
    CAN_Init();
 	delay_10us(20);     

	Uart1_Init();                               //调用串口初始化函数
	delay_10us(20);	

    Uart2_Init();                               //调用串口初始化函数
	delay_10us(20);	

//    CCP4_Comparer_Inint();                     //使能CCP4，输出方波
// 	delay_10us(20);  
 
    Sleep_Mode_Init();
    delay_10us(20);     

/*********************用户函数*************************/    
    
    Led_Display_Init();                        //调用LED初始化函数
 	delay_10us(20);    
   
//    MCP2515_Init();
//    delay_10us(20);  

/***************************************************/     
}

/*************************************************************************/
//函数名:ACC_OFF_Sys_Init
//功  能:ACC掉电系统初始化函数（具体实现根据用户使用而定）
//参  数:无
//返回值:无
//时间:20180818 
//创建者:ahren 
/************************************************************************/
void ACC_OFF_Sys_Init(void)
{
	Work_Mode_Confirm_Flag_g = 0x00;		// 将Work_Mode_Confirm_Flag_g清零	
	Implement_Only_1_Variable = 0;
    
    Sys_Onesecond_Count_g = 0;
    
	LED_Display = 1;						//LED
    LATC1 = 1;
	MCP2515_SILENT_OFF;
	CAN_STB_OFF;
}

/************************************************************************/
//函数名:Peripheral_Mode_Init
//功  能:ACC掉电系统初始化函数（具体实现根据用户使用而定）
//参  数:无
//返回值:无
//时间:20180818 
//创建者:ahren 
/************************************************************************/
void Peripheral_Mode_Init(void)
{
	//PDM2 Init
	MODMD = 1;   //禁止调制器输出模块
	// ECANMD = 1;  //禁止增强型CAN 模块
	//CMP2MD = 1;  //禁止比较器2 模块
	CMP1MD = 1;  //禁止比较器1 模块

	//PDM1 Init
	PSPMD = 1;   //禁止PSP 模
	CTMUMD = 1;  //禁止CTMU 模块
	// ADCMD = 1;  //禁止ADC 模
	TMR4MD = 1;  //禁止Timer4 模
	// TMR3MD = 1;  //禁止Timer3 模块
	//TMR2MD = 1;  //禁止Timer2 模块
	// TMR1MD = 1;  //禁止Timer1 模块
	// TMR0MD = 1;  //禁止Timer0 模块

	//PDM0 Init
	CCP5MD = 1;  //禁止CCP5 模块
	CCP4MD = 1;  //禁止CCP4 模块
	CCP3MD = 1;  //禁止CCP3 模块
	CCP2MD = 1;  //禁止CCP2 模块
	CCP1MD = 1;  //禁止CCP1 模块

	UART2MD  = 1; //禁止USART2 模块
	UART1MD  = 1; //禁止USART1 模块

	SSPMD = 1;  //禁止MSSP 模块  
}


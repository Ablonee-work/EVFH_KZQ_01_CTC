
/*----------------------------------------------------------------------*/
/* Confidential and Proprietary Information of MicroPerfect Corporation */
/* (C) 2017 ,MicroPerfect Corporation . All rights reserved. */
/* FileName : mian.c */
/* New File Date:2017.11.02 */
/* Function :  */
/* Ver:001 */
/* Author:MP_Ahren*/
/* History:YYYY.MM.DD Version Author Description */
/* */
/*----------------------------------------------------------------------*/

/***************头文件****************/
/**/
/*************************************/
#include"config.h"
/*************************************/

/********************系统配置***********************************************/

__CONFIG(1,RETEN_ON&INTOSCSEL_LOW&XINST_OFF&SOSCSEL_DIG
&IESO_OFF&FCMEN_OFF&PLLCFG_OFF&FOSC_HS1);
//HS oscillator (Medium power, 4 MHz - 16 MHz)
//振荡器切换OFF&故障保护时钟监视器OFF&高速模式&外部晶振&直接使用振荡器
//10 = 数字 （ SCLKI）模式；使能 RC0 和 RC1 的 I/O 端口功能

__CONFIG(2,PWRTEN_ON&BOREN_ON&BORV_1&BORPWR_MEDIUM&WDTEN_OFF);
//上电延时定时器使能&2.7V欠压使能&设置为中等功耗级别&看门狗关闭

__CONFIG(3,CANMX_PORTE&MSSPMSK_MSK5&MCLRE_ON);
//CANTX 和 CANRX 引脚分别位于 RE5 和 RE4 上& 使能 MCLR 引脚；禁止 RE3 输入引脚

__CONFIG(4,STVREN_ON&BBSIZ_BB1K);//堆栈满 / 下溢导致复位&1 kW 引导区大小

__CONFIG(5,CP0_ON&CP1_ON&CP2_ON&CP3_ON&CPB_ON&CPD_ON);//所有代码保护

//软件版本号：19110814
__IDLOC(19110814); 

/********************************************************************************/

/******************宏定义*************/
/**/
/*************************************/


/*************************************/

/***************标志位申明************/
/**/
/*************************************/

/***************函数申明**************/
/**/
/*************************************/

/*************************************/

//*******************************************/
//函数名:main
//功  能:main函数
//参  数:无
//返回值:无 
//时  间:20171102
//创建者:ahren
/*******************************************/
void main(void)
{
    BU_ON_Sys_Init();

    while(1)
    {          
        Sys_Time_Count_WorkMode();			//调用系统时间计数函数

        Work_Mode_Choice();					//调用系统工作模式选择函数
        
        if(Work_Mode_Confirm_Flag_g == 0x01)
        {
            Sent_TC04_CAN_Data();        
        }            
    }
}



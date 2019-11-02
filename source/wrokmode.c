/*----------------------------------------------------------------------*/
/* Confidential and Proprietary Information of MicroPerfect Corporation */
/* (C) 2017 ,MicroPerfect Corporation . All rights reserved. */
/* FileName : workmode.c */
/* New File Date:2017.11.09 */
/* Function : work mode choice*/
/* Ver:001 */
/* Author:MP_Ahren*/
/* History:YYYY.MM.DD Version Author Description */
/*        : */
/*-------------------------------------------------------------*/


/***************ͷ�ļ�****************/
/**/
/*************************************/
#include"workmode.h"
//******************�궨��*************/
/**/
/*************************************/


/*************************************/

/***************��־λ����************/
/**/
//static u8 Return_Time_Data = 0x00;                  //ʱ���������ֵ 
static u8 Sleep_Work_Mode_Count_g = 0x00;           //˯����ʱ������־λ
extern u8 Time_5ms_Flag_g;
extern u8 Cnt_Num;
extern u8 Error_Flag;

extern u8 TC04_ID_Num[20];//ID���ݴ洢
extern u8 TC04_ID_Cnt[20];//IDͳ��
extern u8 TC04_Off_Line_Val_Buff[20];//���߱�־λ
extern u8 TC04_Off_Line_Cnt_Buff[20];//���߱�־λ
extern u8 TC04_Off_Time_Cnt_g;

/***************��������**************/
/**/
/*************************************/

static void Sys_Sleep_Work_Mode(void);            //˯�߹���ģʽ����		

/*************************************/

//*******************************************/
//������:Sys_Time_Count_WorkMode
//��  ��:����ģʽ��ϵͳʱ�����,һЩ��Ҫ�����Ĺ��������ڴ˺�������
//��  ��:��
//����ֵ:�� 
//ʱ  ��:20171106
//������:ahren
/*******************************************/
u8 Sys_Time_Count_WorkMode(void)
{	
    static u8 Sys_Time_Count_g = 0x00;				//ϵͳʱ�������־λ
    //static u8 Sys_Onesecond_Count_g = 0x00;         //1��ʱ�������־λ
    u8 Return_Time_Data = 0x00;             //ϵͳ����ʱ����䷵��ֵ     
    
	if(0x01 == Sys_Time_Falg_g)
	{
		Sys_Time_Falg_g   = 0x00;                   //��ϵͳ��ʱ5ms��־λ����
        
		Sys_Time_Count_g++;                         //ϵͳ����ʱ����������1
        Sys_Onesecond_Count_g++;	
        CAN_Sent_Time_Cnt_g++;//CAN ����ʱ�����
        
		Power_ACC_Timer_Flag_g = 0x01;              //Power_ACC_Timer_Flag_g��1,����ACC�����������
        Power_BU_Timer_Flag_g  = 0x01;              //Power_BU_Timer_Flag_g��1������BU�����������
        
        Time_5ms_Flag_g = 1;//CAN ���ݷ���ʱ���־λ
        
		if((0x05 == Sys_Onesecond_Count_g) && (Work_Mode_Confirm_Flag_g == 0))
		{	
			Work_Mode_Confirm_Flag_g = 0x01;        //��ʱ25ms������ȷ��ϵͳ����ģʽ           
		}
	}
    /*******************************************/   
    
    if(0xC8 == Sys_Onesecond_Count_g)               //������ʱ1����
    {
        Sys_Onesecond_Count_g = 0x00;        
        Sys_Onesecond_Flag_g = 0x01;                //1���־λ��1 
        
        MCU_CAN_Send_1s_Flag_g =1;
        
        MCP_CAN_Send_1s_Flag_g = 1;
        
        Led_Display_Fuction();
               
    } 
    
    /*******************************************/  
    
    if(0x01 == Sys_Time_Count_g)
    {
        Return_Time_Data = 0x01;
    }
    else if(0x02 == Sys_Time_Count_g)
    {       
        Return_Time_Data = 0x02;
    }
    else if(0x03 == Sys_Time_Count_g)
    {      
        Return_Time_Data = 0x03;
        Sys_Time_Count_g = 0;
    }
    else if(0x04 == Sys_Time_Count_g)
    {      
        Return_Time_Data = 0x04;
    }
    else if(0x05 == Sys_Time_Count_g)
    {        
        Return_Time_Data = 0x05;
    }
    else if(0x06 == Sys_Time_Count_g)
    {        
        Return_Time_Data = 0x06;
    }
    else 
    {
        Return_Time_Data = 0x00;
    }
    
    return Return_Time_Data;     
}

//*******************************************/
//������:Sys_Function_Work
//��  ��:ϵͳ�����������
//��  ��:��
//����ֵ:Value
//ʱ  ��:20180625
//������:ahren
/*******************************************/
void Sys_Function_Work(void)
{
    u8 Sys_Function_Work_Flag = 0x00;

    if(Implement_Only_1_Variable == 0)
    {         
        Implement_Only_1_Variable = 1;  //�ñ�־λ��1��������˯����ִֻ��һ��
        
        ACC_ON_Sys_Init();              //ACC ON ��س�ʼ��
        
        CAN_STB_ON;                     //����MCU CANʹ��PIN

        MCP2515_SILENT_ON;              //����mcp2515 CANʹ��PIN
    }
#if 0        
    //*******************************************/
    //ϵͳ�����٣�����������书��
    //ʱ  ��:20180610
    //�޸���:ahren 

    Sys_Function_Work_Flag = Sys_Time_Count_WorkMode();

    /*******************************************/       
    
    //Return_Time_Data = 0x00;
    
    switch(Sys_Function_Work_Flag)
    {
        case 1:
#if 0
                CAN_Interrupt_Occurred();           //can����   //MCU CAN�գ�MCP2515 CAN��
#endif
            break;
            
        case 2:
#if 0
                MCP2515_Interrupt_Occurred();           //can2���� //MCP2515 CAN�գ�MCU CAN��
#endif
            break;
            
        case 3:  
#if 0
                CAN_Send_Alarm_Warning_Data();       //MCU CAN1 ����������
#endif 
#if 0
                CAN2_Send_Alarm_Warning_Data();
#endif
            break;

        default:
            
            break;
    }
#endif
}

//*******************************************/
//������:Work_Mode_Choice
//��  ��:����ģʽѡ��
//��  ��:��
//����ֵ:��
//ʱ  ��:20171109
//������:ahren
//*******************************************/
void Work_Mode_Choice(void)
{
	u8 Work_Mode_Value = 0x00;		
	
	Work_Mode_Value = Power_Work_Mode_Confirm();				//��ȡ��Դ״̬����ֵ
							                                   
    if(Work_Mode_Confirm_Flag_g == 1)                           //�ж�25ms��ʱ�Ƿ���
	{			
		switch(Work_Mode_Value)
		{
			case 0x01:
						Work_Mode_Value = 0x00;					//��Work_Mode_Value����
						Sleep_Work_Mode_Count_g = 0x00;			//BU +ACC ON ����ģʽ����˯��ģʽ��������						 
                        Sys_Function_Work();
				break;
				
			case 0x02:			
						Work_Mode_Value = 0x00;					//��Work_Mode_Value����
						//Work_Mode_Confirm_Flag_g = 0x00;		//������ģʽȷ�ϱ�־λ����
						Sys_Sleep_Work_Mode();					//����˯�߹���ģʽ����   
				break;
                
			case 0x03:	
                		Work_Mode_Value = 0x00;					//��Work_Mode_Value����                  
				break;                
				
			default:
						Work_Mode_Value = 0x00;
				break;
		}
	}

}

//*******************************************/
//������:Sys_Sleep_Work_Mode
//��  ��:˯�߹���ģʽ
//��  ��:��
//����ֵ:��
//ʱ  ��:20171109
//������:ahren
//*******************************************/
static void Sys_Sleep_Work_Mode(void)
{
    Sleep_Work_Mode_Count_g++;                  //����˯��ģʽ����������������˯���������
	
	if(0x02 <= Sleep_Work_Mode_Count_g)
	{
		Sleep_Work_Mode_Count_g = 0x00;
		
		/****************************************************************/
		//����Sleepģʽǰ��Ҫ����صĹ��ܹرգ����͹���
		//20171109
		//arhen
		/****************************************************************/	
        
        MCP2515_Enter_Sleep_Mode();      

        ACC_OFF_Sys_Init();		
		/****************************************************************/
		//������������
		//20191012
		//arhen
        Cnt_Num = 0;       
        memset(TC04_ID_Cnt, 0, 20); // �� TC04_ID_Cnt ��������
        memset(TC04_ID_Num, 0, 20); // �� TC04_ID_Num ��������
        memset(TC04_Off_Line_Val_Buff, 0, 20); // �� TC04_Off_Line_Val_Buff ��������
        memset(TC04_Off_Line_Cnt_Buff, 0, 20); // �� TC04_Off_Line_Cnt_Buff ��������        
		/****************************************************************/       
		//������������
		//20191012
		//arhen        
        Error_Flag = 0;   

		/****************************************************************/	
		/****************************************************************/
		//ȷ��ϵͳ������˯��ģʽ��ִ��˯��ָ��SLEEP();
		//20171109
		//arhen
		/****************************************************************/	
        if(ACC_DET_State)
        {
            SLEEP();	//˯��ָ��

            /****************************************************************/

            /***************************************************************/
            //ϵͳ�����Ѻ���ִ��������ָ�ʹʱ���ȶ��ٽ���ϵͳ��ʼ��
            //20171109
            //arhen
            /***************************************************************/	
            NOP();		//��ָ��
            NOP();		//��ָ��
        }         
        Sleep_Work_Mode_Count_g = 0x00;			//���Ѻ󣬽�˯��ģʽ��������
        
        BU_ON_Sys_Init();		
		/****************************************************************/		
	}	
}

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

/***************��������**************/
/**/
/*************************************/

u8 Work_Mode_Confirm_Flag_g = 0x00;				//����ģʽȷ�ϱ�־λ
u8 Implement_Only_1_Variable = 0x00;      //����⵽ACCʱ�ų�ʼ����Щ�û��������ϵ�ִֻ��һ��  
u8 Sys_Onesecond_Count_g = 0x00;          //ϵͳʱ�䶨ʱ1s
static void Sys_Sleep_Work_Mode(void); //˯�߹���ģʽ����		
u8 CAN_Sent_Time_Cnt_g = 0;  //CAN���ķ�������ʱ��

u8 PowerOn_Cnt = 0;  //CAN���ķ�������ʱ��
/*************************************/
//*******************************************/
//������:CAN_Sent_Time_Choice
//��  ��:����CAN ��������
//��  ��:��
//����ֵ:Ret_Val
//ʱ  ��:20190719
//������:ahren
/*******************************************/
u8 CAN_Sent_Time_Choice(u8 select)
{
    CAN_Sent_Time_Cnt_g++;   //CAN���ķ�������ʱ��
    if(0x01 == select)
    {
        if(CAN_Sent_Time_Cnt_g >= 50)//250ms
        {
            CAN_Sent_Time_Cnt_g = 0;
            return 1;
        }
    }
    else if(0x02 == select)
    {
        if(CAN_Sent_Time_Cnt_g >= 100)//500ms
        {
            CAN_Sent_Time_Cnt_g = 0;
            return 1;
        }    
    }
    else
    {
        if(CAN_Sent_Time_Cnt_g >= 200)//1000ms
        {
            CAN_Sent_Time_Cnt_g = 0;
            return 1;
        }     
    }    
    return 0;
}
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
	if(0x01 == Sys_Time_Falg_g)
	{
		Sys_Time_Falg_g = 0x00;                   //��ϵͳ��ʱ5ms��־λ����
        
        Sys_Onesecond_Count_g++;
          
        //���ķ������ڼ���
        if(CAN_Sent_Time_Choice(Outgoing_Data_Cycle))//���ⷢ�ͱ������ڣ���ʱʱ�䵽
        {
            TJA1042_Sent_Time_Flag = 1;
            if((TC_Num >= 1)&&(TC_Num <= TC_NUM_MAX))
            {
                MCP2515_Sent_Time_Flag = 1; //̽����������ȷ������ȷ����ɣ���ʼ���ⷢ�ͱ���
            }              
        }        

        TC04_Off_Time_Cnt_g++;   //̽�������߼������ʱ��
        
		Power_ACC_Timer_Flag_g = 0x01;              //Power_ACC_Timer_Flag_g��1,����ACC�����������
        Power_BU_Timer_Flag_g  = 0x01;              //Power_BU_Timer_Flag_g��1������BU�����������
                
		if((5 == Sys_Onesecond_Count_g) && (Work_Mode_Confirm_Flag_g == 0))
		{	
			Work_Mode_Confirm_Flag_g = 0x01;        //��ʱ25ms������ȷ��ϵͳ����ģʽ           
		}
	}
    /*******************************************/   
    
    if(200 == Sys_Onesecond_Count_g)               //������ʱ1����
    {
        Sys_Onesecond_Count_g = 0x00;        
        Led_Display_Fuction();  
        
        if(PowerOn_OfflineDet_Flag < 20)
        {
            PowerOn_OfflineDet_Flag++;
        } 
    } 
    return 0;     
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
    uint32_t CAN_ID_SIDH = 0;
    uint32_t CAN_ID_SIDL = 0;
    uint32_t CAN_ID_EIDH = 0;
    uint32_t CAN_ID_EIDL = 0;      
    uint32_t CAN_ID_Flag = 0;
    
    if(Implement_Only_1_Variable == 0)
    {         
        Implement_Only_1_Variable = 1;  //�ñ�־λ��1��������˯����ִֻ��һ��
        
        ACC_ON_Sys_Init();              //ACC ON ��س�ʼ��
        
        MCP2515_SILENT_ON();              //����mcp2515 CANʹ��PIN
        
        //��ȡEEPROM�е�̽��������
        TC_Num = eeprom_read(0x03);//�������������EEPROM����
        NOP();NOP();NOP();NOP();  
        
        if((TC_Num == 0)||(TC_Num > TC_NUM_MAX))
        {
            TC_Num = 0;//д��Ĭ������
            eeprom_write(0x03, TC_Num);
            NOP();NOP();NOP();NOP();       
        }
        
        //��ȡ����������ID
        CAN_ID_Flag = eeprom_read(0x09);//CANĬ��ID���޸ı�־λ
        NOP();NOP();NOP();NOP();      
        if(CAN_ID_Flag == 0x5A)
        {
            CAN_ID_SIDH = eeprom_read(0x04);                        
            NOP();NOP();NOP();NOP();
            CAN_ID_SIDL = eeprom_read(0x05);                     
            NOP();NOP();NOP();NOP();  
            CAN_ID_EIDH = eeprom_read(0x06);                        
            NOP();NOP();NOP();NOP();
            CAN_ID_EIDL = eeprom_read(0x07);                        
            NOP();NOP();NOP();NOP();    
	        MCP2515_TxID = (CAN_ID_SIDH<<24)|(CAN_ID_SIDL<<16)|(CAN_ID_EIDH<<8)|(CAN_ID_EIDL);
        }
        
        //��ȡ���ⷢ�ͱ�������ʱ��
        Outgoing_Data_Cycle = eeprom_read(0x08);
        NOP();NOP();NOP();NOP();      
        if(0x01 == Outgoing_Data_Cycle)
        {
        }
        else if(0x02 == Outgoing_Data_Cycle)
        {
        }
        else if(0x03 == Outgoing_Data_Cycle)
        {
        }
        else
        {
            Outgoing_Data_Cycle = 3;//Ĭ��3��1s����
            eeprom_write(0x08,Outgoing_Data_Cycle);                           
            NOP();NOP();NOP();NOP();
        }            
    }
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
						Sleep_Work_Mode_Count_g = 0x00;			//BU +ACC ON ����ģʽ����˯��ģʽ��������						 
                        Sys_Function_Work();
				break;
				
			case 0x02:			
						Sys_Sleep_Work_Mode();					//����˯�߹���ģʽ����   
				break;           				
			default:break;
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
    u8 i,j;
    static u8 sec_cnt = 0;
    static u8 send_cnt = 0;
    
    Sleep_Work_Mode_Count_g++;                  //����˯��ģʽ����������������˯���������
	
	if(0x02 <= Sleep_Work_Mode_Count_g)
	{
		Sleep_Work_Mode_Count_g = 0x00;
		
        MCP2515_Sleep_Mode();      

        ACC_OFF_Sys_Init();		

        if(ACC_DET_State)
        {
            //SLEEP();	//˯��ָ��
            //20210225
            //̽����˯��
            
            for(i=0; i<TC_Num; i++)TC_Sleep_Flag[i] = 1;
            
            send_cnt = 0;
            CanToCan_TC_F0(0x01); //�͹���
            send_cnt++;
            sec_cnt = 0;                        
            while(1)
            {
                TC04_CAN_Data_Process();            //̽�������ݴ���
                
                if(0x01 == Sys_Time_Falg_g)
                {
                    Sys_Time_Falg_g = 0x00;   
                    sec_cnt++;   
                    
                    if(sec_cnt>200)
                    {
                        sec_cnt = 0;
                        
                       if(send_cnt>=3)
                       {
                           break;
                       }
                       else 
                       {
                            CanToCan_TC_F0(0x01); //�͹���
                            send_cnt++;
                       }
                    } 
                    
                    //�����յ�F0���Ļظ�
                    j=0;
                    for(i=0; i<TC_Num; i++)j += TC_Sleep_Flag[i];
                    if(j==0)break;
                }                 
            }
            
            Sys_Sleep_Flag = 1;  
            TC_Code_Err = 0;
            
            memset(RxMsgBuff_Flag, 0, TC_DATA_BUFF); 
            memset(TC04_Buff, 0, TC_NUM_MAX*9);//�������̽��������
            memset(TC04_OffLine_Flag, 0, TC_NUM_MAX); //������߱�־
            
            while(1)
            {
                TC04_CAN_Data_Process();            //̽�������ݴ���
                
                if(!ACC_DET_State)//ACC����
                {
                    Sys_Sleep_Flag = 0;
                    break;
                }                
                if(Sys_Sleep_Flag == 2)
                {      
                    Sys_Sleep_Flag = 3;
                    break;
                }
            }
            Sys_Sleep_Wake_TC = 1;
            PowerOn_OfflineDet_Flag = 0;

            //20210225
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

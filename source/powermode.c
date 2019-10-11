/*----------------------------------------------------------------------*/
/* Confidential and Proprietary Information of MicroPerfect Corporation */
/* (C) 2017 ,MicroPerfect Corporation . All rights reserved. */
/* FileName : powermode.c */
/* New File Date:2017.11.02 */
/* Function : power det (BU/ACC/Sleep)*/
/* Ver:001 */
/* Author:MP_Ahren*/
/* History:YYYY.MM.DD Version Author Description */
/* 
          :2018.06.25 002 @Ahren Add BU Det Function
 */
/*-------------------------------------------------------------*/


/***************ͷ�ļ�****************/
/**/
/*************************************/

#include"powermode.h"


//******************�궨��*************/
/**/


/*************************************/

/***************��־λ����************/
/**/
/*************************************/

static u8 ACC_DET_State_Count_g = 0x00;			//ACC״̬������ 
static u8 ACC_DET_State_Flag_g  = 0x00;			//ACC״̬������ 

static u8 BU_DET_State_Count_g = 0x00;			//ACC״̬������ 
static u8 BU_DET_State_Flag_g  = 0x00;			//ACC״̬������ 

/*20180712*/
static u8 Sleep_Work_Mode_Count_g = 0x00;		//˯����ʱ������־λ

/***************��������**************/
/**/
/*************************************/


/*************************************/


/********************************************/
//������:BU_State_Det_Init
//��  ��:��ԴBU������Prot Pin��ʼ��
//��  ��:BU_State_Init
//����ֵ:�� 
//ʱ  ��:20171102
//������:ahren
/*******************************************/
void BU_State_Det_Init(void)
{	 
	BU_DET_Direction = 1;		//����I/O�ڷ���Ϊ����
		
	BU_DET_State 	 = 1;		//����I/O�ڳ�ʼ��Ϊ�ߵ�ƽ		
}

/********************************************/
//������:ACC_State_Det_Init
//��  ��:��ԴACC������Prot Pin��ʼ��
//��  ��:ACC_State_Init
//����ֵ:�� 
//ʱ  ��:20171102
//������:ahren
/*******************************************/
void ACC_State_Det_Init(void)
{	
	ACC_DET_Direction = 1;		//����I/O�ڷ���Ϊ����
	
	ACC_DET_State     = 1;  	//����I/O�ڳ�ʼ��Ϊ�ߵ�ƽ		
}

/********************************************/
//������:Power_BU_State_Det
//��  ��:��Դ BU ״̬���
//��  ��:��
//����ֵ:
//ʱ  ��:20171102
//������:ahren
/*******************************************/
u8 Power_BU_State_Det()
{      
    //if(BU_DET_State == 0x00)                    //0��1�ߵ�ƽ���
    if(BU_DET_State)  //1�ߵ�ƽ���    
    {
        if(Power_BU_Timer_Flag_g)
        {
            Power_BU_Timer_Flag_g = 0x00;
            
            BU_DET_State_Count_g++;
            
            if(0x04 <= BU_DET_State_Count_g)
            {
                BU_DET_State_Count_g = 0;
                /*20180712*/
                if(BU_DET_State)//1�ߵ�ƽ���   
                {   
                    BU_DET_State_Flag_g = 0x01;
                }
            }
        }
    }
    else
    {
        BU_DET_State_Count_g = 0;
                
        BU_DET_State_Flag_g  = 0;    
    }
    
    return (BU_DET_State_Flag_g);
}

/********************************************/
//������:Power_ACC_State_Det
//��  ��:��Դ ACC ״̬���
//��  ��:��
//����ֵ:
//ʱ  ��:20171102
//������:ahren
/*******************************************/
u8 Power_ACC_State_Det()
{    
    if(!ACC_DET_State)									//���ACC Pin���Ƿ�Ϊ�͵�ƽ
    {
 		if(Power_ACC_Timer_Flag_g)						//�����ʱ������־λ�Ƿ�Ϊ1		
 		{
	 		Power_ACC_Timer_Flag_g = 0x00;				//����ʱ������־λ����	
	 		
	 		ACC_DET_State_Count_g++;					//ACC״̬������	
	 		
			if(0x04 <= ACC_DET_State_Count_g)			//������4 ������ʱ20ms
			{
				ACC_DET_State_Count_g = 0x00;			//����������
				/*20180712*/
                if(ACC_DET_State  == 0x00)
                {   
                    ACC_DET_State_Flag_g = 0x01;			//��ACC״̬����ֵ��1
                }
			}	 			
	 	}	   
    }
    else
    {
    	ACC_DET_State_Count_g = 0x00;					//��ACC״̬��������
    	
    	ACC_DET_State_Flag_g  = 0x00;  					//��ACC״̬����ֵ����
    }
       
    return (ACC_DET_State_Flag_g);						//����ACC״ֵ̬
}

/********************************************/
//������:Power_Det_Port_Init
//��  ��:��Դ�ܽų�ʼ��
//��  ��:��
//����ֵ:
//ʱ  ��:20180625
//������:ahren
/*******************************************/
void Power_Det_Port_Init(void)
{
    BU_State_Det_Init();
    ACC_State_Det_Init();
}

//*******************************************/
//������:Powr_Work_Mode_Confirm
//��  ��:��Դ����ģʽȷ��
//��  ��:��
//����ֵ:ACC_Work_Mode_Value/BU_Work_Mode_Value
//		:ACC_Work_Mode_Value=0x01����ACC ON  ����ģʽ
//		:ACC_Work_Mode_Value=0x02����ACC OFF ����ģʽ
//		:BU_Work_Mode_Value=0x01����BU ON  ����ģʽ
//		:BU_Work_Mode_Value=0x02����BU OFF ����ģʽ
//ʱ  ��:20171109
//������:ahren
//*******************************************/
u8 Power_Work_Mode_Confirm()
{	
	u8 ACC_Work_Mode_Value = 0x00;	
    u8 BU_Work_Mode_Value  = 0x01;
           
	ACC_Work_Mode_Value = Power_ACC_State_Det();	//��ȡACC״̬����ֵ,01�м쵽
#if 1
	//BU_Work_Mode_Value  = Power_BU_State_Det();     //��ȡBU״̬����ֵ��01�м쵽
  
    if((BU_Work_Mode_Value == 0x01) && (ACC_Work_Mode_Value == 0x01))
    {   
        return (0x01);								//BU +ACC ON,����ģʽ����ֵ 
    }
    else if((BU_Work_Mode_Value == 0x01) && (ACC_Work_Mode_Value == 0x00))
	{		
		return (0x02);								//ACC OFF����ģʽ����ֵ,Sleep
	}
    else
    {
        return (0x02);								//ACC OFF����ģʽ����ֵ
    }
#endif

 #if 0 
	if(ACC_Work_Mode_Value)
	{		
		return (0x01);								//ACC ON����ģʽ����ֵ
	}
	else
	{		
		return (0x02);								//ACC OFF����ģʽ����ֵ
	}
#endif 	
}

/* ------------------------- Includes ------------------------------------- */
#include "RX8010.h"
#include"config.h"

/* ------------------------- Defines  -------------------------------------- */
#define IIC_SCL_Pin   RG4
#define IIC_SCL_Port  TRISG4

#define IIC_SDA_Pin   RB4
#define IIC_SDA_Port  TRISB4

//SCL
#define IIC_SCL_H()     (IIC_SCL_Pin = 1)	
#define IIC_SCL_L()     (IIC_SCL_Pin = 0)	
//SDA
#define IIC_SDA_H()   	(IIC_SDA_Pin = 1)
#define IIC_SDA_L()  	(IIC_SDA_Pin = 0)
#define IIC_SDA_R()     IIC_SDA_Pin
//SDA Port Set
#define IIC_SDA_IN()    (IIC_SDA_Port = 1)
#define IIC_SDA_OUT()   (IIC_SDA_Port = 0)

#define  IIC_READ   ((uint8_t)0x01)	//读
#define  IIC_WRITE  ((uint8_t)0x00)	//写

#ifndef IIC_OK
    #define IIC_OK		1
#endif
#ifndef IIC_NG
    #define IIC_NG		0
#endif

typedef enum 
{
    IIC_NACK = 0,
    IIC_ACK  
}IIC_ACK_Def;

/* ------------------------- Variable Prototypes ------------------- */
/* ------------------------- Private Function Prototypes ------------------- */
static void IIC_Delay_Us(uint16_t nus);
static void IIC_Start(void);
static void IIC_Stop(void);
static IIC_ACK_Def IIC_Wait_Ack(void);
static void IIC_Send_Ack(IIC_ACK_Def ack);
static void IIC_Send_Byte(uint8_t txd);
static uint8_t IIC_Read_Byte(void);

/**
 * @brief 
 * @param            
 * @return 
 */
static void IIC_Delay_Us(uint16_t nus)
{
	uint16_t  i,j ;
	for(i=0;i<nus;i++)
		for(j=0;j<8;j++);
}

/**
 * @brief 
 * @param            
 * @return 
 */
static void IIC_Start(void)
{
	IIC_SDA_OUT();
	IIC_SDA_H();
	IIC_SCL_H();
	IIC_Delay_Us(1);    
 	IIC_SDA_L();
	IIC_Delay_Us(1); 
    IIC_SCL_L();  
    IIC_Delay_Us(1);
}	 

/**
 * @brief 
 * @param            
 * @return 
 */
static void IIC_Stop(void)
{
	IIC_SDA_OUT();
	IIC_SCL_H();
	IIC_SDA_L();
 	IIC_Delay_Us(1);
    IIC_SDA_H();
	IIC_Delay_Us(1);	
}

/**
 * @brief 
 * @param            
 * @return 
 */
static IIC_ACK_Def IIC_Wait_Ack(void)
{
	uint8_t ucErrTime=0;
	IIC_SDA_IN();
	IIC_SCL_H();
	IIC_Delay_Us(1);

	while(IIC_SDA_R())
	{
		ucErrTime++;
		if(ucErrTime > 250)
		{
			IIC_Stop();
			return IIC_NACK;
		}
	}
    
    IIC_SCL_L();
    IIC_Delay_Us(1);
	return IIC_ACK;  
} 

/**
 * @brief 
 * @param            
 * @return 
 */
static void IIC_Send_Ack(IIC_ACK_Def ack)
{
	IIC_SDA_OUT();
	if(ack == IIC_ACK)
	{
		IIC_SDA_L();
	}
	else
	{
		IIC_SDA_H();
	}
	IIC_SCL_H();
	IIC_Delay_Us(1);
    IIC_SCL_L();
    IIC_Delay_Us(1);    
}

/**
 * @brief 
 * @param            
 * @return 
 */
static void IIC_Send_Byte(uint8_t txd)
{                        
    uint8_t t;   
	IIC_SDA_OUT();
    for(t=0; t<8; t++)
    {              
        if(txd&0x80)
        {
            IIC_SDA_H();
        }
        else
        {
            IIC_SDA_L();
        }
        txd <<= 1;
        IIC_SCL_H();
        IIC_Delay_Us(1);
        IIC_SCL_L();
        IIC_Delay_Us(1);
    }	 
} 	

/**
 * @brief 
 * @param            
 * @return 
 */
static uint8_t IIC_Read_Byte(void)
{
	uint8_t i,receive = 0;
	IIC_SDA_IN();
	for(i=0; i<8; i++)
	{
		IIC_SCL_H(); 
        IIC_Delay_Us(1);
		receive <<= 1;
		if(IIC_SDA_R())
		{
			receive++; 
		}             		
		IIC_SCL_L(); 
		IIC_Delay_Us(1);              
	}					 
	return receive;
}

/**
 * @brief 
 * @param            
 * @return 
 */
uint8_t IIC_Write_One_Byte(uint8_t addr, uint8_t reg, uint8_t data)
{
    IIC_Start();

    IIC_Send_Byte((addr<<1) | IIC_WRITE);
    if (IIC_Wait_Ack()==IIC_NACK)
    {
        return IIC_NG;
    }

    IIC_Send_Byte(reg);
    if (IIC_Wait_Ack()==IIC_NACK)
    {
        return IIC_NG;
    }

    IIC_Send_Byte(data);
    if (IIC_Wait_Ack()==IIC_NACK)
    {
        return IIC_NG;
    }

    IIC_Stop();
    return IIC_OK;
}

/**
 * @brief 
 * @param            
 * @return 
 */
uint8_t IIC_Read_One_Byte(uint8_t addr, uint8_t reg)
{
    uint8_t data = 0;
    IIC_Start();

    IIC_Send_Byte((addr<<1) | IIC_WRITE);
    if (IIC_Wait_Ack() == IIC_NACK)
    {
        return IIC_NG;
    }

    IIC_Send_Byte(reg);
    if (IIC_Wait_Ack() == IIC_NACK)
    {
        return IIC_NG;
    }

    IIC_Start();

    IIC_Send_Byte((addr<<1) | IIC_READ);
    if (IIC_Wait_Ack() == IIC_NACK)
    {
        return IIC_NG;
    }

    data = IIC_Read_Byte();

    IIC_Stop();
    return data;
}

/**
 * @brief 
 * @param            
 * @return 
 */
uint8_t IIC_Write_Buffer(uint8_t addr, uint8_t reg, uint16_t len, uint8_t *data)
{
    uint16_t i;
    IIC_Start();

    IIC_Send_Byte((addr<<1) | IIC_WRITE);
    if (IIC_Wait_Ack() == IIC_NACK)
    {
        return IIC_NG;
    }

    IIC_Send_Byte(reg);
    if (IIC_Wait_Ack() == IIC_NACK)
    {
        return IIC_NG;
    }

    for (i=0; i<len; i++) 
    {
        IIC_Send_Byte(*data);
        if (IIC_Wait_Ack() == IIC_NACK)
        {
            return IIC_NG;
        }
        data++;
    }

    IIC_Stop();
    return IIC_OK;
}

/**
 * @brief 
 * @param            
 * @return 
 */
uint8_t IIC_Read_Buffer(uint8_t addr, uint8_t reg, uint16_t len, uint8_t *buf)
{
    uint16_t i;
    IIC_Start();

    IIC_Send_Byte((addr<<1) | IIC_WRITE);
    if (IIC_Wait_Ack() == IIC_NACK)
    {
        return IIC_NG;
    }

    IIC_Send_Byte(reg);
    if (IIC_Wait_Ack() == IIC_NACK)
    {
        return IIC_NG;
    }

    IIC_Start();

    IIC_Send_Byte((addr<<1) | IIC_READ);
    if (IIC_Wait_Ack() == IIC_NACK)
    {
        return IIC_NG;
    }

    for (i=0; i<len; i++) 
    {
        *buf = IIC_Read_Byte();
        if(i == (len-1))
        {
            IIC_Send_Ack(IIC_NACK);
        }
        else 
        {
            IIC_Send_Ack(IIC_ACK);
        }			
        buf++;   
    }                 
   IIC_Stop();
   return IIC_OK;
}

/**
 * @brief 
 * @param            
 * @return 
 */
void IIC_Init(void)
{  
    IIC_SCL_Port = 0;
    IIC_SDA_Port = 0;   
    
	IIC_SDA_OUT();    
	IIC_SCL_H();
	IIC_SDA_H();
    IIC_Delay_Us(1000); 
}

/**
 * @brief 
 * @param            
 * @return 
 */
void IIC_Deinit(void)
{
	IIC_SDA_OUT();
	IIC_SCL_H();
	IIC_SDA_H();
	IIC_Delay_Us(100); 
}

//RX8010
//地址：011 0010  =  0x32
#define  SLAVE_ADDR   0x32

#define  REG_SEC   0x10
#define  REG_MIN   0x11
#define  REG_HOUR  0x12
#define  REG_SWEEK 0x13
#define  REG_DAY   0x14
#define  REG_MONTH 0x15
#define  REG_YEAR  0x16

#define  MIN_Alarm      0x18
#define  HOUR_Alarm      0x19
#define  WEEK_DAY_Alarm      0x1A

#define  Timer_Counter0      0x1B
#define  Timer_Counter1      0x1C
#define  Extension_Register  0x1D
#define  Flag_Register       0x1E
#define  Control_Register    0x1F

#define  IRQ_Control         0x32

void RX8010_Init(void)
{
    uint8_t data;
    uint8_t time_ready[6] = {21,3,31,16,12,15};
    IIC_Init();
    
    IIC_Delay_Us(1000); 
      
    data = IIC_Read_One_Byte(SLAVE_ADDR, Flag_Register);
    
    if(data&0x02)
    {
        IIC_Write_One_Byte(SLAVE_ADDR, Flag_Register, data&(~0x02));
        
        IIC_Delay_Us(1000); 
                
        data = IIC_Read_One_Byte(SLAVE_ADDR, Flag_Register);
        
         if(data&0x02)
         {
             IIC_Write_One_Byte(SLAVE_ADDR, Flag_Register, data&(~0x02));
         }          
    }
    
    //寄存器初始化
    IIC_Write_One_Byte(SLAVE_ADDR, 0x17, 0xD8);
    IIC_Write_One_Byte(SLAVE_ADDR, 0x30, 0x00);
    IIC_Write_One_Byte(SLAVE_ADDR, 0x31, 0x08);
    
    IIC_Write_One_Byte(SLAVE_ADDR, IRQ_Control, 0);
    IIC_Write_One_Byte(SLAVE_ADDR, Extension_Register, 0);
    IIC_Write_One_Byte(SLAVE_ADDR, Flag_Register, 0);
    IIC_Write_One_Byte(SLAVE_ADDR, Control_Register, 0x40);
    
    //时间设置
    //RX8010_Time_Set(time_ready);
    
    IIC_Write_One_Byte(SLAVE_ADDR, Control_Register, 0x00);
}


void RX8010_Time_Set(uint8_t *time)
{
    uint8_t time_data[7];
    
    time_data[6] = ((time[0]/10)<<4) | (time[0]%10);
    time_data[5] = ((time[1]/10)<<4) | (time[1]%10);
    time_data[4] = ((time[2]/10)<<4) | (time[2]%10);
    time_data[3] = 0x01;
    time_data[2] = ((time[3]/10)<<4) | (time[3]%10);
    time_data[1] = ((time[4]/10)<<4) | (time[4]%10);
    time_data[0] = ((time[5]/10)<<4) | (time[5]%10);
    
    IIC_Write_Buffer(SLAVE_ADDR, REG_SEC, 7, time_data);
}

void RX8010_Time_Get(uint8_t *time)
{
    uint8_t time_data[7];
    
    IIC_Read_Buffer(SLAVE_ADDR, REG_SEC, 7, time_data);
    
    //16进制
//    time[0] = (time_data[6]>>4)*10 + (time_data[6]&0x0F);    //年
//    time[1] = (time_data[5]>>4)*10 + (time_data[5]&0x0F);    //月
//    time[2] = (time_data[4]>>4)*10 + (time_data[4]&0x0F);    //日
//    time[3] = (time_data[2]>>4)*10 + (time_data[2]&0x0F);    //时
//    time[4] = (time_data[1]>>4)*10 + (time_data[1]&0x0F);    //分
//    time[5] = (time_data[0]>>4)*10 + (time_data[0]&0x0F);    //秒
    
    //BCD码
    time[0] = time_data[6];    //年
    time[1] = time_data[5];    //月
    time[2] = time_data[4];    //日
    
    time[3] = time_data[2];    //时
    time[4] = time_data[1];    //分
    time[5] = time_data[0];    //秒       
}



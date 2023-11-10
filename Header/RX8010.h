#ifndef _RX8010_H
#define _RX8010_H
/* ------------------------- Includes ------------------------------------- */
#include <stdint.h>
/* ------------------------- Defines  ------------------------------------- */
/* ------------------------- Public Variable Prototypes ------------------- */
/* ------------------------- Public Function Prototypes ------------------- */
void IIC_Init(void);
void IIC_Deinit(void);
uint8_t IIC_Write_One_Byte(uint8_t addr, uint8_t reg, uint8_t data);
uint8_t IIC_Read_One_Byte(uint8_t addr, uint8_t reg);
uint8_t IIC_Write_Buffer(uint8_t addr, uint8_t reg, uint16_t len, uint8_t *data);
uint8_t IIC_Read_Buffer(uint8_t addr, uint8_t reg, uint16_t len, uint8_t *buf);


void RX8010_Init(void);
void RX8010_Time_Set(uint8_t *time);
void RX8010_Time_Get(uint8_t *time);
#endif


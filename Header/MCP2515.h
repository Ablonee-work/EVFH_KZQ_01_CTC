/*----------------------------------------------------------------------*/
/* Confidential and Proprietary Information of MicroPerfect Corporation */
/* (C) 2018 ,MicroPerfect Corporation . All rights reserved. */
/* FileName : MCP2515.h */
/* Function : */
/* Ver: 0.01  */
/* MP_Ahren NewFile */
/* History:YYYY.MM.DD Version author Description 
          :2018.07.03 0.01 @wuwenfang Follow the coding standard  */
/*-------------------------------------------------------------*/
#ifndef __MCP2515_H
#define __MCP2515_H

/***************头文件****************/
/**/
/*************************************/

#include"config.h"

/******************宏定义*************/
/**/
/*************************************/

#define MCP2515_CANSTAT         0x0E
#define MCP2515_CANCTRL         0x0F
#define MCP2515_BFPCTRL         0x0C
#define MCP2515_TEC             0x1C
#define MCP2515_REC             0x1D
#define MCP2515_CNF3            0x28
#define MCP2515_CNF2            0x29
#define MCP2515_CNF1            0x2A
#define MCP2515_CANINTE         0x2B
#define MCP2515_CANINTF         0x2C
#define MCP2515_EFLG            0x2D
#define MCP2515_TXRTSCTRL       0x0D

#define MCP2515_RXF0SIDH        0x00
#define MCP2515_RXF0SIDL        0x01
#define MCP2515_RXF0EID8        0x02
#define MCP2515_RXF0EID0        0x03
#define MCP2515_RXF1SIDH        0x04
#define MCP2515_RXF1SIDL        0x05
#define MCP2515_RXF1EID8        0x06
#define MCP2515_RXF1EID0        0x07
#define MCP2515_RXF2SIDH        0x08
#define MCP2515_RXF2SIDL        0x09
#define MCP2515_RXF2EID8        0x0A
#define MCP2515_RXF2EID0        0x0B
#define MCP2515_RXF3SIDH        0x10
#define MCP2515_RXF3SIDL        0x11
#define MCP2515_RXF3EID8        0x12
#define MCP2515_RXF3EID0        0x13
#define MCP2515_RXF4SIDH        0x14
#define MCP2515_RXF4SIDL        0x15
#define MCP2515_RXF4EID8        0x16
#define MCP2515_RXF4EID0        0x17
#define MCP2515_RXF5SIDH        0x18
#define MCP2515_RXF5SIDL        0x19
#define MCP2515_RXF5EID8        0x1A
#define MCP2515_RXF5EID0        0x1B

#define MCP2515_RXM0SIDH        0x20
#define MCP2515_RXM0SIDL        0x21
#define MCP2515_RXM0EID8        0x22
#define MCP2515_RXM0EID0        0x23
#define MCP2515_RXM1SIDH        0x24
#define MCP2515_RXM1SIDL        0x25
#define MCP2515_RXM1EID8        0x26
#define MCP2515_RXM1EID0        0x27

#define MCP2515_TXB0CTRL        0x30
#define MCP2515_TXB0SIDH        0x31
#define MCP2515_TXB0SIDL        0x32
#define MCP2515_TXB0EID8        0x33
#define MCP2515_TXB0EID0        0x34
#define MCP2515_TXB0DLC         0x35
#define MCP2515_TXB0D0          0x36
#define MCP2515_TXB0D1          0x37
#define MCP2515_TXB0D2          0x38
#define MCP2515_TXB0D3          0x39
#define MCP2515_TXB0D4          0x3A
#define MCP2515_TXB0D5          0x3B
#define MCP2515_TXB0D6          0x3C
#define MCP2515_TXB0D7          0x3D

#define MCP2515_TXB1CTRL        0x40
#define MCP2515_TXB1SIDH        0x41
#define MCP2515_TXB1SIDL        0x42
#define MCP2515_TXB1EID8        0x43
#define MCP2515_TXB1EID0        0x44
#define MCP2515_TXB1DLC         0x45
#define MCP2515_TXB1D0          0x46
#define MCP2515_TXB1D1          0x47
#define MCP2515_TXB1D2          0x48
#define MCP2515_TXB1D3          0x49
#define MCP2515_TXB1D4          0x4A
#define MCP2515_TXB1D5          0x4B
#define MCP2515_TXB1D6          0x4C
#define MCP2515_TXB1D7          0x4D

#define MCP2515_TXB2CTRL        0x50
#define MCP2515_TXB2SIDH        0x51
#define MCP2515_TXB2SIDL        0x52
#define MCP2515_TXB2EID8        0x53
#define MCP2515_TXB2EID0        0x54
#define MCP2515_TXB2DLC         0x55
#define MCP2515_TXB2D0          0x56
#define MCP2515_TXB2D1          0x57
#define MCP2515_TXB2D2          0x58
#define MCP2515_TXB2D3          0x59
#define MCP2515_TXB2D4          0x5A
#define MCP2515_TXB2D5          0x5B
#define MCP2515_TXB2D6          0x5C
#define MCP2515_TXB2D7          0x5D

#define MCP2515_RXB0CTRL        0x60
#define MCP2515_RXB0SIDH        0x61
#define MCP2515_RXB0SIDL        0x62
#define MCP2515_RXB0EID8        0x63
#define MCP2515_RXB0EID0        0x64
#define MCP2515_RXB0DLC         0x65
#define MCP2515_RXB0D0          0x66
#define MCP2515_RXB0D1          0x67
#define MCP2515_RXB0D2          0x68
#define MCP2515_RXB0D3          0x69
#define MCP2515_RXB0D4          0x6A
#define MCP2515_RXB0D5          0x6B
#define MCP2515_RXB0D6          0x6C
#define MCP2515_RXB0D7          0x6D

#define MCP2515_RXB1CTRL        0x70
#define MCP2515_RXB1SIDH        0x71
#define MCP2515_RXB1SIDL        0x72
#define MCP2515_RXB1EID8        0x73
#define MCP2515_RXB1EID0        0x74
#define MCP2515_RXB1DLC         0x75
#define MCP2515_RXB1D0          0x76
#define MCP2515_RXB1D1          0x77
#define MCP2515_RXB1D2          0x78
#define MCP2515_RXB1D3          0x79
#define MCP2515_RXB1D4          0x7A
#define MCP2515_RXB1D5          0x7B
#define MCP2515_RXB1D6          0x7C
#define MCP2515_RXB1D7          0x7D

#define MCP2515_TXREQ           0x08
#define MCP2515_TXP             0x03

#define MCP2515_RXM             0x60
#define MCP2515_BUKT            0x04

#define MCP2515_REQOP           0xE0
#define MCP2515_ABAT            0x10
#define	OSM                     0x08
#define MCP2515_CLKEN           0x04
#define MCP2515_CLKPRE          0x03

#define MCP2515_REQOP           0xE0
#define MCP2515_ICOD            0x0E

#define MCP2515_RX0IE           0x01
#define MCP2515_RX1IE           0x02
#define MCP2515_TX0IE           0x04
#define MCP2515_TX1IE           0x80
#define MCP2515_TX2IE           0x10
#define MCP2515_ERRIE           0x20
#define MCP2515_WAKIE           0x40
#define MCP2515_MERRE           0x80

#define MCP2515_RX0IF           0x01
#define MCP2515_RX1IF           0x02
#define MCP2515_TX0IF           0x04
#define MCP2515_TX1IF           0x80
#define MCP2515_TX2IF           0x10
#define MCP2515_ERRIF           0x20
#define MCP2515_WAKIF           0x40
#define MCP2515_MERRF           0x80

#define MCP2515_B1BFS           0x20
#define MCP2515_B0BFS           0x10
#define MCP2515_B1BFE           0x08
#define MCP2515_B0BFE           0x04
#define MCP2515_B1BFM           0x02
#define MCP2515_B0BFM           0x01

// #define MCP2515_SJW             0xC0
// #define MCP2515_BRP             0x3F

#define MCP2515_BTLMODE         0x80
#define MCP2515_3_SAM           0x40
#define MCP2515_1_SAM           0x00
#define MCP2515_PHSEG1          0x38
#define MCP2515_PRSEG           0x07

#define MCP2515_WAKFIL          0x40
#define MCP2515_PHSEG2          0x07

#define MCP2515_TXB2RTS         0x04
#define MCP2515_TXB1RTS         0x02
#define MCP2515_TXB0RTS         0x01

#define MCP2515_SJW_1TQ         0x00
#define MCP2515_SJW_2TQ         0x40
#define MCP2515_SJW_3TQ         0x80
#define MCP2515_SJW_4TQ         0xC0

#define MCP2515_BRP_2_F         0x00
#define MCP2515_BRP_4_F         0x01

#define MCP2515_BTLMODE_CNF3    0x80
#define MCP2515_BTLMODE_PH1_IPT 0x00

#define MCP2515_SMPL_3X         0x40
#define MCP2515_SMPL_1X         0x00

#define MCP2515_PHSEG1_8TQ      0x38
#define MCP2515_PHSEG1_7TQ      0x30
#define MCP2515_PHSEG1_6TQ      0x28
#define MCP2515_PHSEG1_5TQ      0x20
#define MCP2515_PHSEG1_4TQ      0x18
#define MCP2515_PHSEG1_3TQ      0x10
#define MCP2515_PHSEG1_2TQ      0x08
#define MCP2515_PHSEG1_1TQ      0x00

#define MCP2515_PRSEG_8TQ       0x07
#define MCP2515_PRSEG_7TQ       0x06
#define MCP2515_PRSEG_6TQ       0x05
#define MCP2515_PRSEG_5TQ       0x04
#define MCP2515_PRSEG_4TQ       0x03
#define MCP2515_PRSEG_3TQ       0x02
#define MCP2515_PRSEG_2TQ       0x01
#define MCP2515_PRSEG_1TQ       0x00

#define MCP2515_PHSEG2_8TQ      0x07
#define MCP2515_PHSEG2_7TQ      0x06
#define MCP2515_PHSEG2_6TQ      0x05
#define MCP2515_PHSEG2_5TQ      0x04
#define MCP2515_PHSEG2_4TQ      0x03
#define MCP2515_PHSEG2_3TQ      0x02
#define MCP2515_PHSEG2_2TQ      0x01
#define MCP2515_PHSEG2_1TQ      0x00

#define MCP2515_SOF_ENABLED     0x80
#define MCP2515_WAKFIL_ENABLED  0x40
#define MCP2515_WAKFIL_DISABLED 0x00


#define MCP2515_RX0IE_ENABLED   0x01
#define MCP2515_RX0IE_DISABLED  0x00
#define MCP2515_RX1IE_ENABLED   0x02
#define MCP2515_RX1IE_DISABLED  0x00
#define MCP2515_G_RXIE_ENABLED  0x03
#define MCP2515_G_RXIE_DISABLED 0x00

#define MCP2515_TX0IE_ENABLED   0x04
#define MCP2515_TX0IE_DISABLED  0x00
#define MCP2515_TX1IE_ENABLED   0x08
#define MCP2515_TX2IE_DISABLED  0x00
#define MCP2515_TX2IE_ENABLED   0x10
#define MCP2515_TX2IE_DISABLED  0x00
#define MCP2515_G_TXIE_ENABLED  0x1C
#define MCP2515_G_TXIE_DISABLED 0x00

#define MCP2515_ERRIE_ENABLED   0x20
#define MCP2515_ERRIE_DISABLED  0x00
#define MCP2515_WAKIE_ENABLED   0x40
#define MCP2515_WAKIE_DISABLED  0x00
#define MCP2515_IVRE_ENABLED    0x80
#define MCP2515_IVRE_DISABLED   0x00

#define MCP2515_RX0IF_SET       0x01
#define MCP2515_RX0IF_RESET     0x00
#define MCP2515_RX1IF_SET       0x02
#define MCP2515_RX1IF_RESET     0x00
#define MCP2515_TX0IF_SET       0x04
#define MCP2515_TX0IF_RESET     0x00
#define MCP2515_TX1IF_SET       0x08
#define MCP2515_TX2IF_RESET     0x00
#define MCP2515_TX2IF_SET       0x10
#define MCP2515_TX2IF_RESET     0x00
#define MCP2515_ERRIF_SET       0x20
#define MCP2515_ERRIF_RESET     0x00
#define MCP2515_WAKIF_SET       0x40
#define MCP2515_WAKIF_RESET     0x00
#define MCP2515_IVRF_SET        0x80
#define MCP2515_IVRF_RESET      0x00

#define MCP2515_REQOP_CONFIG    0x80
#define MCP2515_REQOP_LISTEN    0x60
#define MCP2515_REQOP_LOOPBACK  0x40
#define MCP2515_REQOP_SLEEP     0x20
#define MCP2515_REQOP_NORMAL    0x00

#define MCP2515_ABORT           0x10

#define MCP2515_OSM_ENABLED     0x08

#define MCP2515_CLKOUT_ENABLED  0x04
#define MCP2515_CLKOUT_DISABLED 0x00
#define MCP2515_CLKOUT_PRE_8    0x03
#define MCP2515_CLKOUT_PRE_4    0x02
#define MCP2515_CLKOUT_PRE_2    0x01
#define MCP2515_CLKOUT_PRE_1    0x00

#define MCP2515_OPMODE_CONFIG   0x80
#define MCP2515_OPMODE_LISTEN   0x60
#define MCP2515_OPMODE_LOOPBACK 0x40
#define MCP2515_OPMODE_SLEEP    0x20
#define MCP2515_OPMODE_NORMAL   0x00


#define MCP2515_RXM_RCV_ALL     0x60
#define MCP2515_RXM_VALID_EXT   0x40
#define MCP2515_RXM_VALID_STD   0x20
#define MCP2515_RXM_VALID_ALL   0x00

#define MCP2515_RXRTR_REMOTE    0x08
#define MCP2515_RXRTR_NO_REMOTE 0x00

#define MCP2515_BUKT_ROLLOVER    0x04
#define MCP2515_BUKT_NO_ROLLOVER 0x00

#define MCP2515_FILHIT0_FLTR_1  0x01
#define MCP2515_FILHIT0_FLTR_0  0x00

#define MCP2515_FILHIT1_FLTR_5  0x05
#define MCP2515_FILHIT1_FLTR_4  0x04
#define MCP2515_FILHIT1_FLTR_3  0x03
#define MCP2515_FILHIT1_FLTR_2  0x02
#define MCP2515_FILHIT1_FLTR_1  0x01
#define MCP2515_FILHIT1_FLTR_0  0x00

#define MCP2515_TXREQ_CLEAR     0x00
#define MCP2515_TXREQ_SET       0x08

#define MCP2515_TXP_LOWEST      0x00
#define MCP2515_TXP_INTER_LOW   0x01    
#define MCP2515_TXP_INTER_HIGH  0x02
#define MCP2515_TXP_HIGHEST     0x03

 #define MCP2515_DLC_0          0x00
 #define MCP2515_DLC_1          0x01
 #define MCP2515_DLC_2          0x02
 #define MCP2515_DLC_3          0x03
 #define MCP2515_DLC_4          0x04
 #define MCP2515_DLC_5          0x05
 #define MCP2515_DLC_6          0x06
 #define MCP2515_DLC_7          0x07    
 #define MCP2515_DLC_8          0x08
 
#define MCP2515_CAN_WRITE       0x02
#define MCP2515_CAN_READ        0x03
#define MCP2515_CAN_BIT_MODIFY  0x05 
#define MCP2515_CAN_LOAD_TX     0X40 
#define MCP2515_CAN_RTS         0x80
#define MCP2515_CAN_RTS_TXB0    0x81
#define MCP2515_CAN_RTS_TXB1    0x82
#define MCP2515_CAN_RTS_TXB2    0x84
#define MCP2515_CAN_RD_RX_BUFF  0x90
#define MCP2515_CAN_RD_STATUS   0xA0 
#define MCP2515_CAN_RX_STATUS   0xB0
#define MCP2515_CAN_RESET       0xC0 

#define MCP2515_DUMMY_BYTE      0x00
#define MCP2515_TXB0            0x31
#define MCP2515_TXB1            0x41
#define MCP2515_TXB2            0x51
#define MCP2515_RXB0            0x61
#define MCP2515_RXB1            0x71
#define MCP2515_EXIDE_SET       0x08
#define MCP2515_EXIDE_RESET     0x00


#define MCP2515_SILENT_DIR      TRISC0
#define MCP2515_CS_DIR          TRISC1
#define MCP2515_RESET_DIR       TRISC2
#define MCP2515_SCK_DIR         TRISC3
#define MCP2515_MISO_DIR        TRISC4
#define MCP2515_MOSI_DIR        TRISC5

#define MCP2515_SILENT          LATC0
#define MCP2515_CS              LATC1
#define MCP2515_RESET           LATC2
#define MCP2515_SCK             LATC3
#define MCP2515_MISO            LATC4
#define MCP2515_MOSI            LATC5

#define MCP2515_SILENT_ON       MCP2515_SILENT=0
#define MCP2515_SILENT_OFF      MCP2515_SILENT=1

#define SYS_FOSC_4MHZ           0x04
#define SYS_FOSC_8MHZ           0x08
#define CAN_BRP_125             0x7D
#define CAN_BRP_250             0xFA
#define CAN_BRP_500             0x01F4


/***************标志位申明************/
/**/
/*************************************/

/***************变量定义**************/
/**/
u8 MCP2515_TxID[4]={0x18,0xFF,0x30,0x1D};
u8 MCP2515_RxID[4]={0x18,0xFF,0x1D,0x00}; 
/*************************************/

/***************函数申明**************/
/**/
/*************************************/

void MCP2515_Init(void);
void MCP2515_WriteByte(u8 Addr,u8 Data);
void MCP2515_CAN_TxID(u8 *ID, u8 Len);
void MCP2515_CAN_RxID(u8 *ID, u8 Len);
u8 MCP2515_ReadByte(u8 Addr);
u8 MCP2515_Rx_Buffer(u8 *CAN_RX_Buf);    
u8 MCP2515_Sleep_Mode(void);
/****************************************************************************/

#endif

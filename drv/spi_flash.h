/*********************************************************************************************************//**
 * @file    spi_flash.h
 * @version V1.00
 * @date    10/01/2013
 * @brief   The header file of spi_flash.c module.
 *************************************************************************************************************
 *
 * <h2><center>Copyright (C) 2013 Holtek Semiconductor Inc. All rights reserved</center></h2>
 *
 ************************************************************************************************************/
// <<< Use Configuration Wizard in Context Menu >>>

/* Define to prevent recursive inclusion -------------------------------------------------------------------*/
#ifndef __SPI_FLASH_H
#define __SPI_FLASH_H

#ifdef __cplusplus
 extern "C" {
#endif

/*----------------------------------------------------------------------------------------------------------*/
/* SPI Flash module Configuration Setting                                                                   */
/*----------------------------------------------------------------------------------------------------------*/
//  <h> SPI Flash Module Configuration
//    <o0> SPI Channel Selection      <0=> SPI0  <1=> SPI1
//    <o1> SPI SEL Channel Selection  <0=> SEL0  <1=> SEL11
//    <o2> SPI Flash Page Size (Max. 4096)    <1-4096>
//    <o3> SPI Flash Sector Size (Max. 65536)  <1-65536>
//    <o4> SPI Flash JEDEC_ID Byte 3 <0x00-0xFF>
//    <o5> SPI Flash JEDEC_ID Byte 2 <0x00-0xFF>
//    <o6> SPI Flash JEDEC_ID Byte 1 <0x00-0xFF>
//#define SPI_FLASH_CH                    0
//#define SPI_FLASH_SEL_CH                0
#define SPI_FLASH_PAGESIZE              (256)
#define SPI_FLASH_SECTOR_SIZE           (65536)
#define SPI_FLASH_JEDECID_B3            (0xC2)
#define SPI_FLASH_JEDECID_B2            (0x20)
#define SPI_FLASH_JEDECID_B1            (0x18)
//  </h>

/* Includes ------------------------------------------------------------------------------------------------*/
//#include "ht32.h"
#include "ht32_board.h"

/* Exported types ------------------------------------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------------------------------------*/
#define SPI_FLASH_DMA_ADR_FIX_MODE          0
#define SPI_FLASH_DMA_ADR_INC_MODE          1

#define PROGRAM_IF_COM          (0)
#define PROGRAM_IF_VCOM         (1)
//  <o0> Program Interface 
//    <0=> COM Port <1=> Virtual COM Port
#define PROGRAM_IF               1

//#if (PROGRAM_IF == PROGRAM_IF_VCOM)
//  #define XMODEM_ACK_CNT                30
//  #define SPI_FLASH_BUFFER_SIZE         ( (133*31)+1 ) 
//#else
  #define XMODEM_ACK_CNT                4
  #define SPI_FLASH_BUFFER_SIZE         640 
//#endif

//#if (SPI_FLASH_CH == 0) 
//  #define SPI_FLASH_SCK_GPIO_PORT_ID    SPI_CH0_SCK_GPIO_PORT_ID
//  #define SPI_FLASH_SCK_GPIO_PIN_ID     SPI_CH0_SCK_GPIO_PIN_ID
//  #define SPI_FLASH_MOSI_GPIO_PORT_ID   SPI_CH0_MOSI_GPIO_PORT_ID
//  #define SPI_FLASH_MOSI_GPIO_PIN_ID    SPI_CH0_MOSI_GPIO_PIN_ID
//  #define SPI_FLASH_MISO_GPIO_PORT_ID   SPI_CH0_MISO_GPIO_PORT_ID
//  #define SPI_FLASH_MISO_GPIO_PIN_ID    SPI_CH0_MISO_GPIO_PIN_ID
//  #if (SPI_FLASH_SEL_CH == 0)
//  #define SPI_FLASH_SEL_GPIO_PORT_ID    SPI_CH0_SEL0_GPIO_PORT_ID
//  #define SPI_FLASH_SEL_GPIO_PIN_ID     SPI_CH0_SEL0_GPIO_PIN_ID
//  #else
//  #define SPI_FLASH_SEL_GPIO_PORT_ID    SPI_CH0_SEL1_GPIO_PORT_ID
//  #define SPI_FLASH_SEL_GPIO_PIN_ID     SPI_CH0_SEL1_GPIO_PIN_ID
//  #endif
//#else
//  #define SPI_FLASH_SCK_GPIO_PORT_ID    SPI_CH1_SCK_GPIO_PORT_ID
//  #define SPI_FLASH_SCK_GPIO_PIN_ID     SPI_CH1_SCK_GPIO_PIN_ID
//  #define SPI_FLASH_MOSI_GPIO_PORT_ID   SPI_CH1_MOSI_GPIO_PORT_ID
//  #define SPI_FLASH_MOSI_GPIO_PIN_ID    SPI_CH1_MOSI_GPIO_PIN_ID
//  #define SPI_FLASH_MISO_GPIO_PORT_ID   SPI_CH1_MISO_GPIO_PORT_ID
//  #define SPI_FLASH_MISO_GPIO_PIN_ID    SPI_CH1_MISO_GPIO_PIN_ID
//  #if (SPI_FLASH_SEL_CH == 0)
//  #define SPI_FLASH_SEL_GPIO_PORT_ID    SPI_CH1_SEL0_GPIO_PORT_ID
//  #define SPI_FLASH_SEL_GPIO_PIN_ID     SPI_CH1_SEL0_GPIO_PIN_ID
//  #else
//  #define SPI_FLASH_SEL_GPIO_PORT_ID    SPI_CH1_SEL1_GPIO_PORT_ID
//  #define SPI_FLASH_SEL_GPIO_PIN_ID     SPI_CH1_SEL1_GPIO_PIN_ID
//  #endif
//#endif

extern bool SPI_FLASH_IsDMA_Busy;

/* Exported macro ------------------------------------------------------------------------------------------*/
/* Exported functions --------------------------------------------------------------------------------------*/

void SPI_FLASH_Init(void);
void SPI_FLASH_SectorErase(u32 SectorAddr);
void SPI_FLASH_BlockErase(u32 BlockAddr);
void SPI_FLASH_ChipErase(void);
void SPI_FLASH_PageWrite(u8* WriteBuffer, u32 WriteAddr, u16 NumOfWriteByte);
void SPI_FLASH_BufferWrite(u8* WriteBuffer, u32 WriteAddr, u16 NumOfWriteByte);
void SPI_FLASH_BufferRead(u8* ReadBuffer, u32 ReadAddr, u16 NumOfReadByte);
void SPI_FLASH_DualOutputBufferRead(u16* ReadBuffer, u32 ReadAddr, u16 NumOfReadByte);
u32 SPI_FLASH_ReadJEDECID(void);
void SPI_FLASH_WriteStatus(u8 Value);
u16 SPI_FLASH_SendByte(u8 byte);
void SPI_FLASH_WriteEnable(void);
void SPI_FLASH_WriteDisable(void);
void SPI_FLASH_WaitForWriteEnd(void);


void SPI_FLASH_ReadDMA(u32 ReadAddr,u32 DstAddr, u32 DataSize, u8 DstAddrMode);
void SPI_FLASH_WaitDMA_Finished(void);

void SPI_FLASH_PowerDown(void);
void SPI_FLASH_Wakeup(void);

#ifdef __cplusplus
}
#endif

#endif /* __SPI_FLASH_H ------------------------------------------------------------------------------------*/

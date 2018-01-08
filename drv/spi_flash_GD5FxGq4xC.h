/*********************************************************************************************************//**
 * @file    spi_flash_GD5FxGQ4xC.h
 * @version V1.00
 * @date    12/10/2013
 * @brief   The header file of spi_flash.c module.
 *************************************************************************************************************
 *
 * <h2><center>Copyright (C) 2013 Holtek Semiconductor Inc. All rights reserved</center></h2>
 *
 ************************************************************************************************************/

/* Define to prevent recursive inclusion -------------------------------------------------------------------*/
#ifndef __SPI_FLASH_GD5FxGQ4xC_H
#define __SPI_FLASH_GD5FxGQ4xC_H

#ifdef __cplusplus
 extern "C" {
#endif
	 
#include "ht32.h"
   
   
#define STRCAT2_(a, b)       a##b
#define STRCAT2(a, b)        STRCAT2_(a, b)
#define STRCAT3_(a, b, c)    a##b##c
#define STRCAT3(a, b, c)     STRCAT3_(a, b, c)
   
	 
	 
#define FLASH_SPI_CLK       			  SPI1
#define FLASH_SPI               	 (HT_SPI1)
#define FLASH_SPI_IRQn          	 (SPI1_IRQn)
	 

#define FLASH_SPI_CS_PORT           A
#define FLASH_SPI_CS_PIN            6 
#define FLASH_SPI_CS_GPIO_CLK      STRCAT2(P, FLASH_SPI_CS_PORT)
#define FLASH_SPI_CS_GPIO_ID       STRCAT2(GPIO_P, FLASH_SPI_CS_PORT)
#define FLASH_SPI_CS_GPIO_PORT     STRCAT2(HT_GPIO, FLASH_SPI_CS_PORT)
#define FLASH_SPI_CS_GPIO_PIN      STRCAT2(AFIO_PIN_, FLASH_SPI_CS_PIN)
#define FLASH_SPI_CS_AFIO_MODE     (AFIO_MODE_DEFAULT)
//#define FLASH_SPI_CS_GPIO_ID       (GPIO_PD)
//#define FLASH_SPI_CS_GPIO_PIN      (AFIO_PIN_0)
//#define FLASH_SPI_CS_AFIO_MODE     (AFIO_MODE_DEFAULT)
	 
#define FLASH_SPI_SCLK_PORT         E
#define FLASH_SPI_SCLK_PIN          9
#define FLASH_SPI_SCLK_GPIO_CLK    STRCAT2(P, FLASH_SPI_SCLK_PORT)
#define FLASH_SPI_SCLK_GPIO_ID     STRCAT2(GPIO_P, FLASH_SPI_SCLK_PORT)
#define FLASH_SPI_SCLK_GPIO_PORT   STRCAT2(HT_GPIO, FLASH_SPI_SCLK_PORT)
#define FLASH_SPI_SCLK_GPIO_PIN    STRCAT2(AFIO_PIN_, FLASH_SPI_SCLK_PIN)
#define FLASH_SPI_SCLK_AFIO_MODE   (AFIO_FUN_SPI)
//#define FLASH_SPI_SCLK_GPIO_ID     (GPIO_PC)
//#define FLASH_SPI_SCLK_GPIO_PIN    (AFIO_PIN_5)
//#define FLASH_SPI_SCLK_AFIO_MODE   (AFIO_FUN_SPI)
	 
#define FLASH_SPI_MOSI_PORT         E
#define FLASH_SPI_MOSI_PIN          10
#define FLASH_SPI_MOSI_GPIO_CLK    STRCAT2(P, FLASH_SPI_MOSI_PORT)
#define FLASH_SPI_MOSI_GPIO_ID     STRCAT2(GPIO_P, FLASH_SPI_MOSI_PORT)
#define FLASH_SPI_MOSI_GPIO_PORT   STRCAT2(HT_GPIO, FLASH_SPI_MOSI_PORT)
#define FLASH_SPI_MOSI_GPIO_PIN    STRCAT2(AFIO_PIN_, FLASH_SPI_MOSI_PIN)
#define FLASH_SPI_MOSI_AFIO_MODE   (AFIO_FUN_SPI)
//#define FLASH_SPI_MOSI_GPIO_ID     (GPIO_PC)
//#define FLASH_SPI_MOSI_GPIO_PIN    (AFIO_PIN_8)
//#define FLASH_SPI_MOSI_AFIO_MODE   (AFIO_FUN_SPI)
	 
#define FLASH_SPI_MISO_PORT         E
#define FLASH_SPI_MISO_PIN          11
#define FLASH_SPI_MISO_GPIO_CLK    STRCAT2(P, FLASH_SPI_MISO_PORT)
#define FLASH_SPI_MISO_GPIO_ID     STRCAT2(GPIO_P, FLASH_SPI_MISO_PORT)
#define FLASH_SPI_MISO_GPIO_PORT   STRCAT2(HT_GPIO, FLASH_SPI_MISO_PORT)
#define FLASH_SPI_MISO_GPIO_PIN    STRCAT2(AFIO_PIN_, FLASH_SPI_MISO_PIN)
#define FLASH_SPI_MISO_AFIO_MODE   (AFIO_FUN_SPI)
//#define FLASH_SPI_MISO_GPIO_ID     (GPIO_PC)
//#define FLASH_SPI_MISO_GPIO_PIN    (AFIO_PIN_9)
//#define FLASH_SPI_MISO_AFIO_MODE   (AFIO_FUN_SPI)
	 

/* Includes ------------------------------------------------------------------------------------------------*/
/* Exported types ------------------------------------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------------------------------------*/
extern bool SPI_FLASH_IsDMA_Busy; 
#define SPI_FLASH_SEL_ACTIVE()    GPIO_ClearOutBits(FLASH_SPI_CS_GPIO_PORT,FLASH_SPI_CS_GPIO_PIN)
#define SPI_FLASH_SEL_INACTIVE()  GPIO_SetOutBits(FLASH_SPI_CS_GPIO_PORT,FLASH_SPI_CS_GPIO_PIN)


/*----------------------------------------------------------------------------------------------------------*/
/* SPI Flash module Configuration Setting                                                                   */
/*----------------------------------------------------------------------------------------------------------*/
#define MAX_BLOCK_NUM 			1024
#define BLOCK_SIZE     			(2048*64)
#define NAND_FLASH_PAGESIZE (2048)


/** @defgroup SPI_FLASH_Exported_Constants SPI FLASH exported constants
  * @{
  */
#define SPI_FLASH_DMA_ADR_FIX_MODE          0
#define SPI_FLASH_DMA_ADR_INC_MODE          1

#define SPI_FLASH_CMD_WREN              0x06  /* Write enable instruction */
#define SPI_FLASH_CMD_WRDI              0x04  /* Write disable instruction */
#define SPI_FLASH_CMD_GETFEATURE        0x0F
#define SPI_FLASH_CMD_SETFEATURE        0x1F
#define SPI_FLASH_FEATURE_A0            0xA0
#define SPI_FLASH_FEATURE_B0            0xB0
#define SPI_FLASH_FEATURE_C0            0xC0
#define SPI_FLASH_FEATURE_D0            0xD0

#define SPI_FLASH_CMD_PAGEREAD2CACHE   	0x13
#define SPI_FLASH_CMD_READCACHE        	0x03
#define SPI_FLASH_CMD_FASTREADCACHE     0x0B
#define SPI_FLASH_CMD_READCACHE2        0x3B
#define SPI_FLASH_CMD_READCACHE4        0x6B
#define SPI_FLASH_CMD_READCACHED        0xBB
#define SPI_FLASH_CMD_READCACHEQ        0xEB

#define SPI_FLASH_CMD_RDID              0x9F  /* Read JEDEC ID instruction  */

#define SPI_FLASH_CMD_PLOAD        			0x02
#define SPI_FLASH_CMD_PLOAD4        		0x32
#define SPI_FLASH_CMD_PEXECUTE        	0x10
#define SPI_FLASH_CMD_PLOADRD        		0x84	// only available in Internal Data Move
#define SPI_FLASH_CMD_PLOADRD4        	0x34	//0xC4  only available in Internal Data Move

#define SPI_FLASH_CMD_BE                0xD8  //128k
#define SPI_FLASH_RESET              		0xFF

#define SPI_FLASH_BUSY_FLAG             0x01  /* Write operation in progress */

#define SPI_FLASH_DUMMY_BYTE            0x00

#define SPI_FLASH_PORT_TX_DMA_CH        PDMA_SPI1_TX
#define SPI_FLASH_PORT_RX_DMA_CH        PDMA_SPI1_RX

/* Exported macro ------------------------------------------------------------------------------------------*/
/* Exported functions --------------------------------------------------------------------------------------*/
void SPI_FLASH_Init(void);
u16 SPI_FLASH_SendByte(u8 byte);
void SPI_FLASH_WriteEnable(void);
void SPI_FLASH_WriteDisable(void);
u8 SPI_FLASH_GetFeature(u8 feature_addr);
void SPI_FLASH_SetFeature(u8 feature_addr, u8 feature_dat);
void SPI_FLASH_WaitForWriteEnd(void);

void SPI_FLASH_Read2Cache(u32 RowAddr);
void SPI_FLASH_PageRead(u8* ReadBuffer, u32 ReadAddr, u16 NumOfReadByte);
void SPI_FLASH_Read_Spare(u8* ReadBuffer, u32 ReadAddr, u16 NumOfReadByte);

u32 SPI_FLASH_ReadJEDECID(void);

void SPI_FLASH_PageWrite(u8* WriteBuffer, u32 WriteAddr, u16 NumOfWriteByte);
void SPI_NAND_FLASH_BlockErase(u32 BlockAddr);

void SPI_FLASH_reset(void);
void SPI_FLASH_ReadDMA(u8* ReadBuffer,u32 DstAddr, u32 DataSize, u8 DstAddrMode);
void SPI_FLASH_WaitDMA_Finished(void);
bool SPI_FLASH_CheckDMA_Finished(void);



#ifdef __cplusplus
}
#endif

#endif /* __SPI_FLASH_GD5FxGQ4xC_H ------------------------------------------------------------------------*/

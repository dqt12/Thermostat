/*********************************************************************************************************//**
 * @file    spi_flash_mx66l51235f.h
 * @version V1.00
 * @date    12/10/2013
 * @brief   The header file of spi_flash.c module.
 *************************************************************************************************************
 *
 * <h2><center>Copyright (C) 2013 Holtek Semiconductor Inc. All rights reserved</center></h2>
 *
 ************************************************************************************************************/

/* Define to prevent recursive inclusion -------------------------------------------------------------------*/
#ifndef __SPI_FLASH_MX66L51235F_H
#define __SPI_FLASH_MX66L51235F_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------------------------------------*/
/* Exported types ------------------------------------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------------------------------------*/

/** @defgroup SPI_FLASH_Exported_Constants SPI FLASH exported constants
  * @{
  */
#define SPI_FLASH_CMD_WRITE             0x12  /* Program the selected page instruction (4 byte Addr. Cmd.) */
#define SPI_FLASH_CMD_WRSR              0x01  /* Write Status Register instruction */
#define SPI_FLASH_CMD_WREN              0x06  /* Write enable instruction */
#define SPI_FLASH_CMD_WRDI              0x04  /* Write disable instruction */

#define SPI_FLASH_CMD_READ              0x13  /* Read Data instruction (4 byte Addr. Cmd.) */
#define SPI_FLASH_CMD_FREAD             0x0C  /* Read Data at high speed instruction (4 byte Addr. Cmd.) */
#define SPI_FLASH_CMD_DREAD             0x3C  /* Dual Output Mode Read instruction (4 byte Addr. Cmd.) */
#define SPI_FLASH_CMD_RDSR              0x05  /* Read Status Register instruction  */
#define SPI_FLASH_CMD_RDID              0x9F  /* Read JEDEC ID instruction  */
#define SPI_FLASH_CMD_REMS              0x90  /* Read electronic manufacturer & device ID instruction  */
#define SPI_FLASH_CMD_SE                0xDC  /* (64KB BE replace)Sector Erase instruction (4 byte Addr. Cmd.) */
#define SPI_FLASH_CMD_BE                0xDC  /* (64KB BE replace)Block Erase instruction (4 byte Addr. Cmd.) */
#define SPI_FLASH_CMD_CE                0x60  /* Chip Erase instruction */

#define SPI_FLASH_CMD_POWER_DOWN          0xB9
#define SPI_FLASH_CMD_RELEASE_POWER_DOWN  0xAB

#define SPI_FLASH_BUSY_FLAG             0x01  /* Write operation in progress */

#define SPI_FLASH_DUMMY_BYTE            0x00

#if (SPI_FLASH_CH == 0)
#define SPI_FLASH_PORT_TX_DMA_CH        PDMA_SPI0_TX
#define SPI_FLASH_PORT_RX_DMA_CH        PDMA_SPI0_RX
#else
#define SPI_FLASH_PORT_TX_DMA_CH        PDMA_SPI1_TX
#define SPI_FLASH_PORT_RX_DMA_CH        PDMA_SPI1_RX
#endif

/* Exported macro ------------------------------------------------------------------------------------------*/
/* Exported functions --------------------------------------------------------------------------------------*/

#ifdef __cplusplus
}
#endif

#endif /* __SPI_FLASH_MX66L51235F_H ------------------------------------------------------------------------*/

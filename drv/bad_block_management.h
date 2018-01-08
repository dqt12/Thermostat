/*********************************************************************************************************//**
 * @file    bad_block_management.h
 * @version V1.00
 * @date    20/7/2016
 * @brief   spi nand flash bad block management
 *************************************************************************************************************
 *
 * <h2><center>Copyright (C) 2016 Holtek Semiconductor Inc. All rights reserved</center></h2>
 *
 ************************************************************************************************************/
/* Define to prevent recursive inclusion -------------------------------------------------------------------*/
#ifndef __BAD_BLOCK_MANAGEMENT_H
#define __BAD_BLOCK_MANAGEMENT_H

#include "ht32.h"

/* Includes ------------------------------------------------------------------------------------------------*/
#define MAX_BAD_BLOCK 24

typedef struct
{
  u16 bad;
  u16 remap;
} BAD_BLOCK_Typedef;
 
typedef struct
{
  u32 total_num;
  BAD_BLOCK_Typedef info[MAX_BAD_BLOCK];
} BAD_BLOCK_INFO_Typedef;


extern void SPI_FLASH_Init(void);
extern u32 SPI_FLASH_ReadJEDECID(void);

void init_spi_nand_flash(void);
void search_bad_block(void);
void SPI_FLASH_BufferRead(u8* ReadBuffer,u32 DstAddr, u32 DataSize);
void SPI_FLASH_BlockErase(u32 BlockAddr);
void SPI_FLASH_BufferWrite(u8* WriteBuffer, u32 WriteAddr, u16 NumOfWriteByte);
void SPI_FLASH_WriteStatus(u8 Value);
void SPI_FLASH_Config_ECC(u8 enable);


void SPI_FLASH_BufferRead2LCD(u32 DstAddr, u32 DataSize);

#endif /* __BAD_BLOCK_MANAGEMENT_H -----------------------------------------------------------------------------------*/

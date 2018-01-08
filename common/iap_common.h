/*********************************************************************************************************//**
 * @file    USBD/GK_LQFP100_STD/Src_IAP/iap_common.h
 * @version V1.00
 * @date    11/22/2012
 * @brief   The header file of IAP Handler.
 *************************************************************************************************************
 *
 * <h2><center>Copyright (C) 2012 Holtek Semiconductor Inc. All rights reserved</center></h2>
 *
 ************************************************************************************************************/
// <<< Use Configuration Wizard in Context Menu >>>
/* Define to prevent recursive inclusion -------------------------------------------------------------------*/
#ifndef __IAP_HANDLER_H
#define __IAP_HANDLER_H

#include "iap_config.h"

/* Includes ------------------------------------------------------------------------------------------------*/
/* Settings ------------------------------------------------------------------------------------------------*/
//<o0.0> Enable IAP Debug mode
// <i> Enable IAP Debug mode which output debug message to retarget COM port or SWV (ITM).
#define IAP_DEBUG_MODE                (0)

/* Exported types ------------------------------------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------------------------------------*/
#define IAP_TIMEOUT_OFF               (-1)

#define IAP_APFLASH_START             (IAP_CODE_SIZE + (IAP_VERSION_PAGE * 1024))
#define IAP_APFLASH_END               (IAP_APFLASH_SIZE - 1)

#define IAP_APSRAM_START              (0x20000000)
#define IAP_APSRAM_END                (IAP_APSRAM_START + IAP_APSRAM_SIZE)

#define BOOT_MODE_APP                 (0x55AAFAF0)                  /* Magic number for SBVT1               */
#define BOOT_MODE_IAP                 (0x55AAFAF5)

#define TYPE_MAIN_FLASH					(0x00000001)
#define TYPE_SPI_FLASH					(0x00000002)

#define	BOOT_MODE_ADDR		(IAP_APSRAM_END-4)

/* Exported macro ------------------------------------------------------------------------------------------*/
/* Exported functions --------------------------------------------------------------------------------------*/
					void	IAP_Init(void);
unsigned	int		IAP_Reset(unsigned int uMode);
unsigned	int		IAP_isAPValid(void);
					void	IAP_GoCMD(unsigned int address);
unsigned 	int 	IAP_isVersionValid(void);


unsigned	int		IAP_Erase(unsigned int type, unsigned int saddr, unsigned int eaddr);
unsigned	int		IAP_Read(unsigned int type, unsigned int saddr, unsigned int len,unsigned char * buf);
unsigned	int		IAP_Write(unsigned int type, unsigned int saddr, unsigned int len,unsigned int * buf);
#endif /* __IAP_HANDLER_H ----------------------------------------------------------------------------------*/

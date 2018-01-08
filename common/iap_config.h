/*********************************************************************************************************//**
 * @file    USBD/GK_LQFP100_STD/Src_IAP/iap_config.h
 * @version V1.00
 * @date    11/22/2012
 * @brief   The configuration file.
 *************************************************************************************************************
 *
 * <h2><center>Copyright (C) 2012 Holtek Semiconductor Inc. All rights reserved</center></h2>
 *
 ************************************************************************************************************/
// <<< Use Configuration Wizard in Context Menu >>>

/* Define to prevent recursive inclusion -------------------------------------------------------------------*/
#ifndef __IAP_CONFIG_H
#define __IAP_CONFIG_H

/* Includes ------------------------------------------------------------------------------------------------*/
/* Settings ------------------------------------------------------------------------------------------------*/

/* IAP                                                                                                      */
#define IAP_VERSION_PAGE              (1)
#define IAP_CODE_PAGE                 (15)
#define IAP_CODE_SIZE                 (1024 * IAP_CODE_PAGE)        /* IAP code size in Bytes               */


#define IAP_APFLASH_SIZE              (1024 * 255)                  /* 127 KB                               */
#define IAP_APSRAM_SIZE               (1024 * 128)                   /* 64  KB                               */

#define LOADER_VERSION                (0x100)

//<o0.0> Enable wait IAP command timeout function
// <i> If IAP Loader did not received any valid IAP command after specific timeout time, it starts AP automatically.
//<o1> Wait IAP command timeout value (ms) <1-5000:1>
#define IAP_TIMEOUT_EN                (0)
#define IAP_TIMEOUT                   (2000)                        /* IAP Command timeout based on ms      */

/* Exported typedef ----------------------------------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------------------------------------*/
/* Exported variables --------------------------------------------------------------------------------------*/
/* Exported functions --------------------------------------------------------------------------------------*/

#endif /* __IAP_CONFIG_H -----------------------------------------------------------------------------------*/

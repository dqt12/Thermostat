/*********************************************************************************************************//**
 * @file    EBI_LCD/SPI_Flash_VCP/iap_xmodem.h
 * @version $Rev:: 425          $
 * @date    $Date:: 2017-07-04 #$
 * @brief   The header file of Xmodem.
 *************************************************************************************************************
 * @attention
 *
 * Firmware Disclaimer Information
 *
 * 1. The customer hereby acknowledges and agrees that the program technical documentation, including the
 *    code, which is supplied by Holtek Semiconductor Inc., (hereinafter referred to as "HOLTEK") is the
 *    proprietary and confidential intellectual property of HOLTEK, and is protected by copyright law and
 *    other intellectual property laws.
 *
 * 2. The customer hereby acknowledges and agrees that the program technical documentation, including the
 *    code, is confidential information belonging to HOLTEK, and must not be disclosed to any third parties
 *    other than HOLTEK and the customer.
 *
 * 3. The program technical documentation, including the code, is provided "as is" and for customer reference
 *    only. After delivery by HOLTEK, the customer shall use the program technical documentation, including
 *    the code, at their own risk. HOLTEK disclaims any expressed, implied or statutory warranties, including
 *    the warranties of merchantability, satisfactory quality and fitness for a particular purpose.
 *
 * <h2><center>Copyright (C) Holtek Semiconductor Inc. All rights reserved</center></h2>
 ************************************************************************************************************/

/* Define to prevent recursive inclusion -------------------------------------------------------------------*/
#ifndef __IAP_XMODEM_H
#define __IAP_XMODEM_H

/* Includes ------------------------------------------------------------------------------------------------*/
#include "ht32.h"

#ifdef __cplusplus
 extern "C" {
#endif
/** @addtogroup HT32_Series_Peripheral_Examples HT32 Peripheral Examples
  * @{
  */

/** @addtogroup LCD
  * @{
  */

/** @addtogroup SPI_Flash_VCP
  * @{
  */


/* Settings ------------------------------------------------------------------------------------------------*/
#define USING_CRC                       (1)

/* Exported constants --------------------------------------------------------------------------------------*/
#define XMODEM_PROGRAM                  (0x1)
#define XMODEM_VERIFY                   (0x0)

#define X_SSUCCESS                      (0)
#define X_SUSERCANCEL                   (1)
#define X_SPACKET_NUM_ERROR             (2)
#define X_CRC_ERROR                     (3)
#define X_VERIFY_ERROR                  (4)
#define X_OVER_RANGE                    (5)

/* Exported functions --------------------------------------------------------------------------------------*/
u32 xmodem(u8 *bptr, u32 address, u32 mode);


/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */
#ifdef __cplusplus
}
#endif

#endif /* __IAP_XMODEM_H -----------------------------------------------------------------------------------*/

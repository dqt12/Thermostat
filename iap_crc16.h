/*********************************************************************************************************//**
 * @file    EBI_LCD/SPI_Flash_VCP/iap_crc16.h
 * @version $Rev:: 425          $
 * @date    $Date:: 2017-07-04 #$
 * @brief   The header file of CRC 16.
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
#ifndef __IAP_CRC16_H
#define __IAP_CRC16_H

#ifdef __cplusplus
 extern "C" {
#endif
	 
/* Includes ------------------------------------------------------------------------------------------------*/
#include "ht32.h"
/** @addtogroup HT32_Series_Peripheral_Examples HT32 Peripheral Examples
  * @{
  */

/** @addtogroup LCD
  * @{
  */

/** @addtogroup SPI_Flash_VCP
  * @{
  */


/* Exported functions --------------------------------------------------------------------------------------*/
void CRC_InitTable(void);
u16 CRC16(u16 crc, u8 *buffer, u32 length);


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

#endif /* __IAP_CRC16_H ------------------------------------------------------------------------------------*/

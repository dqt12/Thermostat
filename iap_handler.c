/*********************************************************************************************************//**
 * @file    EBI_LCD/SPI_Flash_VCP/iap_handler.c
 * @version $Rev:: 425          $
 * @date    $Date:: 2017-07-04 #$
 * @brief   This file contains IAP handler.
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

/* Includes ------------------------------------------------------------------------------------------------*/
#include "ht32_board.h"
#include "ht32_usbd_class.h"
#include "iap_handler.h"
#include "iap_xmodem.h"
#include "iap_crc16.h"
#include "iap_buffer.h"
#include "spi_flash.h"

/** @addtogroup HT32_Series_Peripheral_Examples HT32 Peripheral Examples
  * @{
  */

/** @addtogroup LCD
  * @{
  */

/** @addtogroup SPI_Flash_VCP
  * @{
  */


/* Private function prototypes -----------------------------------------------------------------------------*/
static void IAP_Download(void);

/* Global functions ----------------------------------------------------------------------------------------*/
/*********************************************************************************************************//**
  * @brief  IAP mode handler.
  * @retval None
  ***********************************************************************************************************/
void IAP_Handler(void)
{
	
	#if (USING_CRC == 1)
	  CRC_InitTable();
	#endif

  SPI_FLASH_Init();             /* Initializes SPI Flash for IAP function                                   */

  SPI_FLASH_WriteStatus(0x00);  /* Clear the Block Protection bit                                           */

  IAP_Download();
}

/* Private functions ---------------------------------------------------------------------------------------*/

/*********************************************************************************************************//**
  * @brief  Download image to Flash.
  * @retval None
  ***********************************************************************************************************/
static void IAP_Download(void)
{
  u32 result;
  u8 IAP_Buffer[136];
  u8 select;

  while (1)
  {
    sprintf((char*)IAP_Buffer, "\r\n\r\n[1] Download\r\n[2] Verify\r\nSelect?");
    PutStrToUSB(IAP_Buffer);
    BufferIndex.Write = BufferIndex.Read = 0;
    select = ReadBuffer();

    if (select == '1')
    {
      sprintf((char*)IAP_Buffer, "\r\nStart Download....");
      PutStrToUSB(IAP_Buffer);
      result = xmodem(IAP_Buffer, 0, XMODEM_PROGRAM);
      if (result == X_SSUCCESS)
      {
        sprintf((char*)IAP_Buffer, "\r\n\r\nDownload successful!\r\n");
        PutStrToUSB(IAP_Buffer);
      }
      else
      {
        sprintf((char*)IAP_Buffer, "\r\n\r\nDownload failed!\r\n");
        PutStrToUSB(IAP_Buffer);
      }
    }
    else if (select == '2')
    {
      sprintf((char*)IAP_Buffer, "\r\nStart Verify....");
      PutStrToUSB(IAP_Buffer);
      result = xmodem(IAP_Buffer, 0, XMODEM_VERIFY);
      if (result == X_SSUCCESS)
      {
        sprintf((char*)IAP_Buffer, "\r\n\r\nVerify successful!\r\n");
        PutStrToUSB(IAP_Buffer);
      }
      else
      {
        sprintf((char*)IAP_Buffer, "\r\n\r\nVerify failed!\r\n");
        PutStrToUSB(IAP_Buffer);
      }
    }
  }
}


/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */

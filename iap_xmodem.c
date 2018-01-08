/*********************************************************************************************************//**
 * @file    EBI_LCD/SPI_Flash_VCP/iap_xmodem.c
 * @version $Rev:: 425          $
 * @date    $Date:: 2017-07-04 #$
 * @brief   This file contains xmodem transmission protocol.
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
#include "ht32.h"
#include "ht32_board.h"
#include "ht32_usbd_class.h"
#include "iap_xmodem.h"
#include "iap_buffer.h"
#include "iap_crc16.h"
#include <string.h>
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


/* Settings ------------------------------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------------------------------------*/
/* Increase XMODEM_ACK_CNT to speed up, but may cause transfer error on USB to UART cable.                  */
/*----------------------------------------------------------------------------------------------------------*/
//#define XMODEM_ACK_CNT                  (100)

/* Private constants ---------------------------------------------------------------------------------------*/
#define SOH                             (0x1)
#define EOT                             (0x4)
#define ACK                             (0x6)
#define NAK                             (0x15)
#define CANCEL                          (0x18)
#define XMODEM_C_DELAY                  (0xFFFFF)

/* Private macro -------------------------------------------------------------------------------------------*/
#define WriteByte                       WriteByteToUSB

/* Private function prototypes -----------------------------------------------------------------------------*/
static void xmodem_stop(void);

/* Global functions ----------------------------------------------------------------------------------------*/
/*********************************************************************************************************//**
  * @brief  The Xmodem transmission function.
  * @param  bptr: Buffer address for save data
  * @param  address: Program/compare start address
  * @param  mode: Program/compare mode
  *         @arg PROGRAM_MODE:  Program the received data into the main Flash
  *         @arg COMPARE_MODE:  Compare the received data with the main Flash
  * @retval X_SSUCCESS: The transmission is successful
  * @retval X_SUSERCANCEL: Received CANCEL (0x18) command from Host
  * @retval X_SPACKET_NUM_ERROR: Packet number is not correct
  * @retval X_CRC_ERROR: CRC error
  * @retval X_VERIFY_ERROR: Compare error
  ***********************************************************************************************************/

u32 xmodem(u8 *bptr, u32 address, u32 mode)
{
  u32 i = 0;
  #if (USING_CRC == 1)
  u16 crcWord;
  #endif
  u8 packetNo = 1;
  u32 startPacket = 1;

  u8 buffer[128];  /* SPI flash data read buffer                                                            */
  u32 *wptr = NULL;
// 	u8 *wptr;

  /*--------------------------------------------------------------------------------------------------------*/
  /* Erase first sector when the start address is unaligned                                                 */
  /*--------------------------------------------------------------------------------------------------------*/
//  if ((address & (SPI_FLASH_SECTOR_SIZE - 1)) != 0x0 && mode == XMODEM_PROGRAM)
	if ((address & (SPI_FLASH_SECTOR_SIZE - 1)) != 0x0 && mode == XMODEM_PROGRAM)
  {
    SPI_FLASH_SectorErase(address);
  }

  while (1)
  {
    if (!IS_BUFFER_EMPTY())
    {
     wptr = (u32 *)(bptr + 4);
			 //wptr = (bptr + 4);
      /*
        Message Block: (Unit: Byte)

        +-----+------+----------+------+----------+
        | SOH | blk# | 255-blk# | data | checksum |
        +-----+------+----------+------+----------+

        SOH      : 1B. 0x01.
        blk#     : 1B. Starts at 0x1 increments by 1, and wraps 0xFF to 0x00.
        255-blk# : 1B. Ones complement.
        data     : 128B.
        checksum : 2B. The sum of the data bytes only. Toss any carry.
        ----------------
        Total    : 133B

      */

      bptr[1] = ReadBuffer();  /* bptr[0], reserved for Data word aligned                                   */

      if (bptr[1] == SOH)
      {
        /*--------------------------------------------------------------------------------------------------*/
        /*  bptr[0]: Reserved (Let Data word aligned)                                                       */
        /*  bptr[1]: Start of frame (SOH)                                                                   */
        /*  bptr[2]: Packet number                                                                          */
        /*  bptr[3]: Complement of packet number                                                            */
        /*  bptr[4~131]: Data                                                                               */
        /*  bptr[132~133]: CRC16                                                                            */
        /*--------------------------------------------------------------------------------------------------*/
        for (i = 2; i < 134; i++)
        {
          bptr[i] = ReadBuffer();
        }

        /*--------------------------------------------------------------------------------------------------*/
        /* Discard repeat package silently                                                                  */
        /*--------------------------------------------------------------------------------------------------*/
        if ((bptr[2] == packetNo - 1) && (bptr[2] + bptr[3] == 0xFF))
        {
          continue;
        }

        /*--------------------------------------------------------------------------------------------------*/
        /* Check packet number                                                                              */
        /*--------------------------------------------------------------------------------------------------*/
        if ((bptr[2] != packetNo) || (bptr[2] + bptr[3] != 0xFF))
        {
          xmodem_stop();
          return X_SPACKET_NUM_ERROR;
        }

        /*--------------------------------------------------------------------------------------------------*/
        /* Check CRC value of Xmodem packet                                                                 */
        /*--------------------------------------------------------------------------------------------------*/
        #if (USING_CRC == 1)
        crcWord = (bptr[132] << 8) | bptr[133];
        if (crcWord != CRC16(0, (u8 *)(&(bptr[4])), 128))
        {
          xmodem_stop();
          return X_CRC_ERROR;
        }
        #endif

        if (mode == XMODEM_PROGRAM)
        {
       //   if ((address & (SPI_FLASH_SECTOR_SIZE - 1)) == 0x0)  /* Sector align                              */
				  if ((address & (SPI_FLASH_SECTOR_SIZE - 1)) == 0x0)  /* Sector align                              */
          {
            /*----------------------------------------------------------------------------------------------*/
            /* Erase necessary sector                                                                       */
            /*----------------------------------------------------------------------------------------------*/
            SPI_FLASH_SectorErase(address);
          }
        }

        /*--------------------------------------------------------------------------------------------------*/
        /* Send numerous ACK after first packet to speed up Xmodem                                          */
        /*--------------------------------------------------------------------------------------------------*/
        if (startPacket)
        {
          for (i = 0; i < XMODEM_ACK_CNT; i++)
          {
            WriteByte(ACK);
          }
          startPacket = 0;            /* Stop 'C' sending                                                   */
        }

        /*--------------------------------------------------------------------------------------------------*/
        /* Send back ACK to start next packet                                                               */
        /*--------------------------------------------------------------------------------------------------*/
        WriteByte(ACK);
        packetNo++;

        if (mode == XMODEM_PROGRAM)
        {
          /*------------------------------------------------------------------------------------------------*/
          /* Start Flash programming                                                                        */
          /*------------------------------------------------------------------------------------------------*/      
					SPI_FLASH_BufferWrite((u8 *)wptr, address, 128);//
          address += 128;
        }
        else
        {
          /*------------------------------------------------------------------------------------------------*/
          /* Start Flash verification                                                                       */
          /*------------------------------------------------------------------------------------------------*/
          SPI_FLASH_BufferRead(buffer, address, 128);
          address += 128;
          for (i = 0; i < 128; i += 4)
          {
           if (*wptr != *(u32 *)(buffer + i))
            {
              xmodem_stop();
              return X_VERIFY_ERROR;
            }
            wptr++;
          }
        }
      }
      else if (bptr[1] == EOT)            /* Transmission is finished                                       */
      {
        WriteByte(ACK);
        break;
      }
      else if (bptr[1] == CANCEL)         /* User cancel the transmission                                   */
      {
        return X_SUSERCANCEL;
      }
    }
    else
    {
      if (startPacket)
      {
        if ((i & XMODEM_C_DELAY) == 0)
        {
          WriteByte('C');
        }
        i++;
      }
    }
  }

  return X_SSUCCESS;
}

/* Private functions ---------------------------------------------------------------------------------------*/
/*********************************************************************************************************//**
  * @brief  This function is used to stop Xmodem transmission.
  * @retval None
  ***********************************************************************************************************/
static void xmodem_stop(void)
{
  u32 i;
  for (i = 3; i != 0; i--)
  {
    WriteByte(CANCEL);
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

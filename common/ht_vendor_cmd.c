#include "ht_vendor_cmd.h"
#include "ht_vendor_cmd_config.h"
#include "ht32.h"
#include "ht32_usbd_core.h"

#include "string.h"
#include "iap_common.h"
//#include "spi_flash_mx66l51235f.h"
//#include "bad_block_management.h"

typedef enum
{
  SBC_SUCCESS        = 0,
  SBC_CMD_NOTSUPPORT = 2,
  SBC_ERROR          = 3,
} SBC_CBWState_Enum;

typedef struct
{
  u32 uAddress;
  u32 uLength;
  SBC_CBWState_Enum CommandStatus;
} SBCInfo_Typedef;

extern SBCInfo_Typedef gSBCInfo;
extern void _SBC_GetAddress(u8 *pCB, u32 uBlockSize);
extern void clear_dCSWDataResidue(void);

//#define CMD_TRACE

#ifdef	CMD_TRACE
#define TRACE		printf
#else
#define TRACE(...)
#endif

#define REBOOT_ENABLE

/*********************************************************************************************************//**
  * @brief  SCSI Block Command (SPC-4) - Vendor Get ID 10 (0xF1).
  * @param  
  * @param  
  * @retval 
  ***********************************************************************************************************/
void _SBC_CMDVendorTest(BOT_Media_TypeDef *pMedia, u8 *pCB)
{
	u32 buf[2] = {0};

	TRACE("CMDVendorTest\r\n");
	
#ifndef IS_IAP_MODE	
	buf[0] |= 0x80000000;
#endif
	
#ifdef	REBOOT_ENABLE
	buf[0] |= 0x02; 
#endif	
	
  BOT_WriteINData((u8*)buf, 8, BOT_STATE_DATAIN_LAST);
	
	clear_dCSWDataResidue();
}

//u8 uBufferx[4096];
u32 uAddressX;
u32 uLengthX;
//u32 offset, count4K;

/*********************************************************************************************************//**
  * @brief  SCSI Block Command (SPC-4) - Vendor Write 10 (0xF3).
  * @param  
  * @param  
  * @retval 
  ***********************************************************************************************************/
void _SBC_CMDVendorWrite(BOT_Media_TypeDef *pMedia, u8 *pCB)
{
  u32 uBuffer[64/4];
  u32 uLen;
	static u8 iap_return_flag;

//	TRACE("CMDVendor Write\r\n");
  /*--------------------------------------------------------------------------------------------------------*/
  /* When receive SBC Write 10 command, get address from CB and set state as OUT, then the BOT state machine*/
  /* will call this function when next OUT data is received.                                                */
  /*--------------------------------------------------------------------------------------------------------*/
  if (uLengthX == 0)
  {
		memcpy(&uAddressX, &pCB[3], 4);
		memcpy(&uLengthX, &pCB[7], 4);		

    BOT_ReadOUTData((u8 *)NULL, USB_DISCARD_OUT_DATA);
		iap_return_flag = 1;
		return;
  }

  uLen = USBDCore_EPTGetTransferCount(BOT_OUT_EPT, USBD_CNTB0);
  BOT_ReadOUTData((u8 *)uBuffer, uLen);	

	if(pCB[1])
		iap_return_flag &= IAP_Write( TYPE_MAIN_FLASH, uAddressX, uLen, (unsigned int *)uBuffer);
	else
		iap_return_flag &= IAP_Write( TYPE_SPI_FLASH, uAddressX, uLen, (unsigned int *)uBuffer);
	
	uAddressX += uLen;
	uLengthX -= uLen;
  /*--------------------------------------------------------------------------------------------------------*/
  /* Send CSW when all data is received.                                                                    */
  /*--------------------------------------------------------------------------------------------------------*/
  if (uLengthX == 0)
  {
		if(iap_return_flag)
			BOT_SendCSW(BOT_CSW_STATUS_PASSED);
		else
			BOT_SendCSW(BOT_CSW_STATUS_FAILED);
  }
	else
		BOT_ReadOUTData((u8 *)NULL, USB_DISCARD_OUT_DATA);
}


/*********************************************************************************************************//**
  * @brief  SCSI Block Command (SPC-4) - Vendor Read 10 (0xF4).
  * @param  
  * @param  
  * @retval 
  ***********************************************************************************************************/
void _SBC_CMDVendorRead(BOT_Media_TypeDef *pMedia, u8 *pCB)
{
	u32 uLen;
  u32 uAddr;
  u8 ucBuffer[512];
  BOT_State_Enum status = BOT_STATE_DATAIN;

  /*--------------------------------------------------------------------------------------------------------*/
  /* When receive SBC Read 10 command, get address from CB and read first block of data. Send the data to   */
  /* USB Buffer for IN transmission. The BOT state machine will call this function again when IN data       */
  /* transmission is finished.                                                                              */
  /*--------------------------------------------------------------------------------------------------------*/
  if (gSBCInfo.uLength == 0)
  {
    _SBC_GetAddress(pCB, pMedia->Size.uBlockSize);
    if (BOT_CheckCBWParameter(BOT_DIR_IN, gSBCInfo.uLength) == FALSE)
    {

      return;
    }
  }

  uAddr = gSBCInfo.uAddress;
  uLen = (gSBCInfo.uLength > BOT_MAX_PACKET_SIZE) ? BOT_MAX_PACKET_SIZE : gSBCInfo.uLength;

  /*--------------------------------------------------------------------------------------------------------*/
  /* Set state as BOT_STATE_DATAIN_LAST which cause the BOT state machine send CSW to Host when IN data     */
  /* transmission is finished.                                                                              */
  /*--------------------------------------------------------------------------------------------------------*/
  if ((gSBCInfo.uLength - uLen) == 0)
  {
    status = BOT_STATE_DATAIN_LAST;
  }


	
	TRACE("uLen = %.4X , uAddr = %.4X\r\n", uLen, uAddr);
	
	if(pCB[1]) 
	{
		IAP_Read(TYPE_MAIN_FLASH,uAddr,uLen,ucBuffer);		
	}
	else
	{
		IAP_Read(TYPE_SPI_FLASH,uAddr,uLen,ucBuffer);				
	}
	
	gSBCInfo.uAddress += uLen;
	gSBCInfo.uLength  -= uLen;

	BOT_WriteINData(ucBuffer, uLen, status);
	  
	TRACE("gSBCInfo.uLength = %.4X\r\n", gSBCInfo.uLength);
	
	clear_dCSWDataResidue();
  return;
}


/*********************************************************************************************************//**
  * @brief  SCSI Block Command (SPC-4) - Vendor Erase 10 (0xF5).
  * @param  
  * @param  
  * @retval 
  ***********************************************************************************************************/
void _SBC_CMDVendorErase(BOT_Media_TypeDef *pMedia, u8 *pCB)
{
  u32 uAddr;
	u32 len;
	
	if(pCB[1]) 
	{
			memcpy(&uAddr,&pCB[3],4);
			memcpy(&len,&pCB[7],4);

			IAP_Erase(TYPE_MAIN_FLASH,uAddr,uAddr + len - 1);
	}
	else
	{
		_SBC_GetAddress(pCB, pMedia->Size.uBlockSize);
		memcpy(&gSBCInfo.uAddress, &pCB[3], sizeof(u8)*4);
		uAddr = gSBCInfo.uAddress;
		
		IAP_Erase(TYPE_SPI_FLASH,uAddr,uAddr+pMedia->Size.uBlockSize);
	}
			
	BOT_ReadOUTData((u8 *)NULL, USB_DISCARD_OUT_DATA);
		
	BOT_SendCSW(BOT_CSW_STATUS_PASSED);
	
	return;
}

/*********************************************************************************************************//**
  * @brief  SCSI Block Command (SPC-4) - Vendor Alternate Mode 10 (0xF6).
  * @param  
  * @param  
  * @retval 
  ***********************************************************************************************************/
void _SBC_CMDVendorAlternateMode(BOT_Media_TypeDef *pMedia, u8 *pCB)
{
	TRACE("CMD Vendor Alternate Mode\r\n");

	_SBC_GetAddress(pCB, pMedia->Size.uBlockSize);

  /*----------------------------------------------------------------------------------------------------*/
	/* Read OUT data with zero length to inform USB Device peripheral that process of CBW OUT             */
	/* data is finished. The USB Device will send ACK to Host when received next OUT data                 */
	/*----------------------------------------------------------------------------------------------------*/
      
	memcpy(&gSBCInfo.uAddress, &pCB[3], sizeof(u8)*4);
			
	BOT_ReadOUTData((u8 *)NULL, USB_DISCARD_OUT_DATA);
		
	BOT_SendCSW(BOT_CSW_STATUS_PASSED);
	
#ifdef	REBOOT_ENABLE	
	IAP_Reset((unsigned int)pCB[3]);
#endif	
	
	return;
}

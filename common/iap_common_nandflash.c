/*********************************************************************************************************//**
 * @file    USBD/STD_GK/Src_IAP/iap_handler.c
 * @version V1.00
 * @date    12/05/2012
 * @brief   This file contains IAP handler.
 *************************************************************************************************************
 *
 * <h2><center>Copyright (C) 2012 Holtek Semiconductor Inc. All rights reserved</center></h2>
 *
 ************************************************************************************************************/

/* Includes ------------------------------------------------------------------------------------------------*/
#include "ht32.h"
#include "iap_common.h"
//#include "bad_block_management.h"
#include "sdio_sd.h"
#include "string.h"

//#define COMMON_TRACE

#ifdef	COMMON_TRACE
#define TRACE		printf
#else
#define TRACE(...)
#endif

u8 sdcard_data_cache[512];
u32 cache_ct;
/*********************************************************************************************************//**
  * @brief  Reset Command.
  * @param  uMode: Mode after reset
  * @retval FALSE or TRUE
  ***********************************************************************************************************/
unsigned int IAP_Reset(unsigned int uMode)
{
  s32 i;
	
	TRACE("IAP test...A1\r\n");
  
  if (uMode == 0)
  {
    if ( (IAP_isAPValid() == FALSE) || (IAP_isVersionValid() == FALSE) )
    {
      /* AP image is invalid. Jump to AP is not allow                                                       */
      TRACE("AP incorrect\r\n");
      return FALSE;
    }
    *(u32 *)BOOT_MODE_ADDR = BOOT_MODE_APP;
  }
  else
  {
    /*------------------------------------------------------------------------------------------------------*/
    /* Disconnect USB and wait for PC aware it                                                              */
    /*------------------------------------------------------------------------------------------------------*/
    *(u32 *)BOOT_MODE_ADDR  = BOOT_MODE_IAP;

  }
	
  NVIC_DisableIRQ(USB_IRQn);
  USBD_DPpullupCmd(DISABLE);
  for (i = 400000; i != 0; i--);
	
  NVIC_SystemReset();

  return TRUE;
}

/*********************************************************************************************************//**
  * @brief  Check AP is valid or not.
  * @retval FALSE or TRUE
  ***********************************************************************************************************/
unsigned int IAP_isAPValid(void)
{
  u32 SP, PC;

  /* Check Stack Point in range                                                                             */
  SP = rw(IAP_APFLASH_START);
  if (SP < IAP_APSRAM_START || SP > IAP_APSRAM_END)
  {
    TRACE("AP SP Err[%08X]\r\n", SP);
    return FALSE;
  }

  /* Check PC in range                                                                                      */
  PC = rw(IAP_APFLASH_START + 0x4);
  if (PC < IAP_APFLASH_START || PC > IAP_APFLASH_END)
  {
    TRACE("AP PC Err[%08X]\r\n", PC);
    return FALSE;
  }

  return TRUE;
}

/*********************************************************************************************************//**
  * @brief  Check version is valid or not.
  * @retval FALSE or TRUE
  ***********************************************************************************************************/
unsigned int IAP_isVersionValid(void)
{
  u32 i, ver, len;
  u32 veraddr = IAP_CODE_SIZE;
  len = rw(veraddr);
	if(len == 0xFFFFFFFF)
	{
		return FALSE;
	}
	veraddr += 0x4;
  for (i = 0; i < len; i += 4)
  {
    ver = rw(veraddr + i);
    if (ver == 0xFFFFFFFF)
    {
      TRACE("Version Err[%08X]\r\n", ver);
      return FALSE;
    }
  } 
  
  return TRUE;
}

#if defined (__CC_ARM)
/*********************************************************************************************************//**
  * @brief  Jump to user application by change PC.
  * @param  address: Start address of user application
  * @retval None
  ***********************************************************************************************************/
__asm void IAP_GoCMD(unsigned int address)
{
  LDR R1, [R0]
  MOV SP, R1
  LDR R1, [R0, #4]
  BX R1
}
#elif defined (__ICCARM__)
void IAP_GoCMD(u32 address)
{
  __asm("LDR R1, [R0]");
  __asm("MOV SP, R1");
  __asm("LDR R1, [R0, #4]");
  __asm("BX R1");
}
#endif

/* Private functions ---------------------------------------------------------------------------------------*/
/*********************************************************************************************************//**
  * @brief  Mass/Page Erase.
  * @param  type: Erase type
  *         @arg IAP_MASS_ERASE: Mass Erase (Not support in IAP mode)
  *         @arg IAP_PAGE_ERASE: Page Erase
  * @param  saddr: Start address
  * @param  eaddr: End address
  * @retval 1 or 0
  ***********************************************************************************************************/
unsigned int IAP_Erase(unsigned int type, unsigned int saddr, unsigned int eaddr)
{
  u32 i,j;
	
  if ((type & TYPE_MAIN_FLASH) == TYPE_MAIN_FLASH && saddr >= IAP_CODE_SIZE)
  {
		for (i = saddr, j=0; i <= eaddr; i += FLASH_PAGE_SIZE, j++)
		{
			FLASH_ErasePage(i);
		}
    return 1;
  }

	if ((type & TYPE_SPI_FLASH) == TYPE_SPI_FLASH)
  {
//		SPI_FLASH_BlockErase(saddr); //固定擦64K
    return 1;
  }
  
	return 0;
}

unsigned int IAP_Read(unsigned int type, unsigned int saddr, unsigned int len,unsigned char * buf)
{
  if ((type & TYPE_MAIN_FLASH) == TYPE_MAIN_FLASH)
  {
		if(saddr >= IAP_CODE_PAGE * 1024 && len <= IAP_VERSION_PAGE*1024)
		{
			memcpy(buf,(u8 *)saddr,len);	
			return 1;	
		}
		return 0;
  }

	if ((type & TYPE_SPI_FLASH) == TYPE_SPI_FLASH)
  {
    if((saddr & (sizeof(sdcard_data_cache) -1)) == 0x00)
    {
//      printf("R %x\r\n", (0 + saddr)&(~(sizeof(sdcard_data_cache)-1)));
      SD_ReadBlock(sdcard_data_cache, saddr, sizeof(sdcard_data_cache));
      cache_ct = 0;
    }
        
    memcpy(buf, &sdcard_data_cache[cache_ct], len);
    cache_ct += len;
    return 1;
  }
	
	return 0;
}


unsigned int IAP_Write(unsigned int type, unsigned int saddr, unsigned int len,unsigned int * buf)
{
  if ((type & TYPE_MAIN_FLASH) == TYPE_MAIN_FLASH)
  {
		FLASH_OptionByte Options;
		FLASH_GetOptionByteStatus(&Options);
		
		if (Options.MainSecurity == 0 && saddr >= IAP_CODE_PAGE * 1024)
		{
			int i = 0,end;
			len &= ~0x03; //不足4byte的部分直接丢弃
			
			end = saddr + len;			
			
			for(i = saddr; i < end; i += 4)
			{
				FLASH_ProgramWordData(i, *buf);
				buf++;
			}					
			return 1;
		}
		
		return 0;
  }

	if ((type & TYPE_SPI_FLASH) == TYPE_SPI_FLASH)
  {
    u32 sizeof_array = sizeof(sdcard_data_cache);
    if(!saddr)
      cache_ct = 0; //reset cache_ct
    
    memcpy(&sdcard_data_cache[cache_ct], buf, len);
    cache_ct += len;
    if(cache_ct >= sizeof_array)
    {
      cache_ct -= sizeof_array;
//      printf("W %x\r\n", (0 + saddr)&(~(sizeof_array-1)));
      SD_WriteBlock(sdcard_data_cache, (0 + saddr)&(~(sizeof_array-1)), sizeof_array);
    }
    return 1;
  }
	
	return 0;	
}

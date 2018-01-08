/*********************************************************************************************************//**
 * @file    spi_flash_GD5FxGq4xC.c
 * @version V1.00
 * @date    10/01/2013
 * @brief   This file provides a set of functions needed to manage the
 *          communication between SPI peripheral and SPI MX25L1606E FLASH.
 *************************************************************************************************************
 *
 * <h2><center>Copyright (C) 2013 Holtek Semiconductor Inc. All rights reserved</center></h2>
 *
 ************************************************************************************************************/

/* Includes ------------------------------------------------------------------------------------------------*/
#include "spi_flash_GD5FxGq4xC.h"
//#include "TRACE.h"

/* Private typedef -----------------------------------------------------------------------------------------*/
/* Private define ------------------------------------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------------------------------------*/
bool SPI_FLASH_IsDMA_Busy;

/* Private variables ---------------------------------------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------------------------------------*/
/* Private functions ---------------------------------------------------------------------------------------*/

/*********************************************************************************************************//**
  * @brief  Initializes peripherals used by the SPI flash driver.
  * @retval None
  ***********************************************************************************************************/
void SPI_FLASH_Init(void)
{
    SPI_InitTypeDef  SPI_InitStructure;

    /*  Enable AFIO & SPI SEL pin port & SPI clock  */
    CKCU_PeripClockConfig_TypeDef CKCUClock = {{0}};
    CKCUClock.Bit.FLASH_SPI_CLK           = 1;
    CKCUClock.Bit.FLASH_SPI_CS_GPIO_CLK   = 1;
    CKCUClock.Bit.FLASH_SPI_SCLK_GPIO_CLK = 1;
    CKCUClock.Bit.FLASH_SPI_MISO_GPIO_CLK = 1;
    CKCUClock.Bit.FLASH_SPI_MOSI_GPIO_CLK = 1;
    CKCUClock.Bit.AFIO                    = 1;
    CKCUClock.Bit.PDMA                    = 1;
    CKCU_PeripClockConfig(CKCUClock, ENABLE);
    
    /*  Configure SPI SEL pin                                                 */
    GPIO_SetOutBits(FLASH_SPI_CS_GPIO_PORT, FLASH_SPI_CS_GPIO_PIN);
    GPIO_DirectionConfig(FLASH_SPI_CS_GPIO_PORT, FLASH_SPI_CS_GPIO_PIN, GPIO_DIR_OUT);

    /*  Configure SPI SCK pin, SPI MISO pin, SPI MOSI pin        */
    AFIO_GPxConfig(FLASH_SPI_SCLK_GPIO_ID, FLASH_SPI_SCLK_GPIO_PIN, FLASH_SPI_SCLK_AFIO_MODE);
    AFIO_GPxConfig(FLASH_SPI_MOSI_GPIO_ID, FLASH_SPI_MOSI_GPIO_PIN, FLASH_SPI_MOSI_AFIO_MODE);
    AFIO_GPxConfig(FLASH_SPI_MISO_GPIO_ID, FLASH_SPI_MISO_GPIO_PIN, FLASH_SPI_MISO_AFIO_MODE);
    
    /*  SPI Configuration */
    SPI_InitStructure.SPI_Mode = SPI_MASTER;
    SPI_InitStructure.SPI_FIFO = SPI_FIFO_ENABLE;
    SPI_InitStructure.SPI_DataLength = SPI_DATALENGTH_8;
    SPI_InitStructure.SPI_SELMode = SPI_SEL_SOFTWARE;
    SPI_InitStructure.SPI_SELPolarity = SPI_SELPOLARITY_LOW;
    SPI_InitStructure.SPI_FirstBit = SPI_FIRSTBIT_MSB;
    SPI_InitStructure.SPI_CPOL = SPI_CPOL_HIGH;
    SPI_InitStructure.SPI_CPHA = SPI_CPHA_SECOND;
    SPI_InitStructure.SPI_RxFIFOTriggerLevel = 1;
    SPI_InitStructure.SPI_TxFIFOTriggerLevel = 1;
    SPI_InitStructure.SPI_ClockPrescaler = 2; /* 24MHz */
//    SPI_InitStructure.SPI_ClockPrescaler = 8;
    SPI_Init(FLASH_SPI, &SPI_InitStructure);

    SPI_SELOutputCmd(FLASH_SPI, ENABLE);
    SPI_Cmd(FLASH_SPI, ENABLE);
      
    SPI_FLASH_IsDMA_Busy = FALSE;
}



/*********************************************************************************************************//**
  * @brief  Sends a byte through the SPI interface and return the byte received from the SPI bus.
  * @param  byte : byte to send.
  * @retval The value of the received byte.
  ***********************************************************************************************************/
u16 SPI_FLASH_SendByte(u8 byte)
{
  /* Wait until TX FIFO empty                                                                               */
  while (SPI_GetFIFOStatus(FLASH_SPI, SPI_FIFO_TX) != 0);
 
  /* Clear all RX data                                                                                      */
  while (SPI_GetFIFOStatus(FLASH_SPI, SPI_FIFO_RX) != 0)
  {
    SPI_ReceiveData(FLASH_SPI);
  }
  
  /* Send	byte through the SPI_FLASH_PORT	peripheral */
  SPI_SendData(FLASH_SPI,	byte);
  
  /* Loop	while	Rx is	not	empty	*/
  while (SPI_GetFIFOStatus(FLASH_SPI, SPI_FIFO_RX) == 0);
  
  /* Return	the	byte read	from the SPI */
  return SPI_ReceiveData(FLASH_SPI);
}




/*********************************************************************************************************//**
  * @brief  Enables the write access to the FLASH.
  * @retval None
  ***********************************************************************************************************/
void SPI_FLASH_WriteEnable(void)
{
  /* Select the SPI FLASH */
  SPI_FLASH_SEL_ACTIVE();

  /* Send "Write Enable" instruction */
  SPI_FLASH_SendByte(SPI_FLASH_CMD_WREN);

  /* Deselect the SPI FLASH */
  SPI_FLASH_SEL_INACTIVE();
}

/*********************************************************************************************************//**
  * @brief  Disables the write access to the FLASH.
  * @retval None
  ***********************************************************************************************************/
void SPI_FLASH_WriteDisable(void)
{
  /* Select the SPI FLASH */
  SPI_FLASH_SEL_ACTIVE();

  /* Send "Write Disable" instruction */
  SPI_FLASH_SendByte(SPI_FLASH_CMD_WRDI);

  /* Deselect the SPI FLASH */
  SPI_FLASH_SEL_INACTIVE();
}


/*********************************************************************************************************//**
  * @brief  get feature.
  * @retval None
  ***********************************************************************************************************/
u8 SPI_FLASH_GetFeature(u8 feature_addr)
{
  u8 ret;
  /* Send write enable instruction */
  SPI_FLASH_WriteEnable();

  /* Select the SPI FLASH */
  SPI_FLASH_SEL_ACTIVE();
  /* Send "Block Erase" instruction */
  SPI_FLASH_SendByte(SPI_FLASH_CMD_GETFEATURE);

  SPI_FLASH_SendByte(feature_addr);

  ret = SPI_FLASH_SendByte(SPI_FLASH_DUMMY_BYTE);
  /* Deselect the SPI FLASH */
  SPI_FLASH_SEL_INACTIVE();
  
  return ret;
}

/*********************************************************************************************************//**
  * @brief  set feature.
  * @retval None
  ***********************************************************************************************************/
void SPI_FLASH_SetFeature(u8 feature_addr, u8 feature_dat)
{
  /* Send write enable instruction */
  SPI_FLASH_WriteEnable();

  /* Select the SPI FLASH */
  SPI_FLASH_SEL_ACTIVE();
  /* Send "Block Erase" instruction */
  SPI_FLASH_SendByte(SPI_FLASH_CMD_SETFEATURE);

  SPI_FLASH_SendByte(feature_addr);
  SPI_FLASH_SendByte(feature_dat);

  /* Deselect the SPI FLASH */
  SPI_FLASH_SEL_INACTIVE();
}




/*********************************************************************************************************//**
  * @brief  Polls the status of the BUSY flag in the FLASH's status register and waits for the end of Flash
            writing.
  * @retval None
  ***********************************************************************************************************/
void SPI_FLASH_WaitForWriteEnd(void)
{
  u8 FLASH_Status = 0;

//  SPI_FLASH_GetFeature(SPI_FLASH_FEATURE_B0);
  /* Select the SPI FLASH */
  SPI_FLASH_SEL_ACTIVE();

  /* Send "Read Status Register" instruction */
  SPI_FLASH_SendByte(SPI_FLASH_CMD_GETFEATURE);
  SPI_FLASH_SendByte(SPI_FLASH_FEATURE_C0);

  /* Loop as long as the busy flag is set */
  do
  {
    /* Send a dummy byte to generate the clock to read the value of the status register */
    FLASH_Status = SPI_FLASH_SendByte(SPI_FLASH_DUMMY_BYTE);

  } while((FLASH_Status & SPI_FLASH_BUSY_FLAG) == SET);

  /* Deselect the SPI FLASH */
  SPI_FLASH_SEL_INACTIVE();
}




/*********************************************************************************************************//**
  * @brief  Reads a block of data to cache from the FLASH.
  * @param  ReadBuffer : pointer to the buffer that receives the data read from the FLASH.
  * @param  ReadAddr : FLASH's internal address to read from.
  * @param  NumOfReadByte : number of bytes to read from the FLASH.
  * @retval None
  ***********************************************************************************************************/
void SPI_FLASH_Read2Cache(u32 RowAddr)
{
  /* Select the SPI FLASH */
  SPI_FLASH_SEL_ACTIVE();
  /* Send "Read from Memory " instruction */
  SPI_FLASH_SendByte(SPI_FLASH_CMD_PAGEREAD2CACHE);
  /* Send high byte address of ReadAddr */
  SPI_FLASH_SendByte((RowAddr & 0xFF0000) >> 16);
  /* Send medium byte address of ReadAddr */
  SPI_FLASH_SendByte((RowAddr& 0xFF00) >> 8);
  /* Send low byte address of ReadAddr */
  SPI_FLASH_SendByte(RowAddr & 0xFF);
  /* Select the SPI FLASH */
  SPI_FLASH_SEL_INACTIVE();
  
  SPI_FLASH_WaitForWriteEnd();
}


/*********************************************************************************************************//**
  * @brief  Reads a block of data from the FLASH.
  * @param  ReadBuffer : pointer to the buffer that receives the data read from the FLASH.
  * @param  ReadAddr : FLASH's internal address to read from.
  * @param  NumOfReadByte : number of bytes to read from the FLASH.
  * @retval None
  ***********************************************************************************************************/
void SPI_FLASH_PageRead(u8* ReadBuffer, u32 ReadAddr, u16 NumOfReadByte)
{
  u32 RowAddr = ReadAddr>>11;
  
  SPI_FLASH_Read2Cache(RowAddr);
  
  /* Select the SPI FLASH */
  SPI_FLASH_SEL_ACTIVE();

  /* Send "Read from Memory " instruction */
  SPI_FLASH_SendByte(SPI_FLASH_CMD_READCACHE);
  SPI_FLASH_SendByte(SPI_FLASH_DUMMY_BYTE);

  SPI_FLASH_SendByte((ReadAddr>>8)&0x07);
  SPI_FLASH_SendByte(ReadAddr&0xFF);

  while(NumOfReadByte--)
  {
    /* Read a byte from the FLASH */
    *ReadBuffer = SPI_FLASH_SendByte(SPI_FLASH_DUMMY_BYTE);
    /* Point to the next location where the byte read will be saved */
    ReadBuffer++;
  }

  /* Deselect the SPI FLASH */
  SPI_FLASH_SEL_INACTIVE();
}

/*********************************************************************************************************//**
  * @brief  Reads a block of data from the FLASH.
  * @param  ReadBuffer : pointer to the buffer that receives the data read from the FLASH.
  * @param  ReadAddr : FLASH's internal address to read from.
  * @param  NumOfReadByte : number of bytes to read from the FLASH.
  * @retval None
  ***********************************************************************************************************/
void SPI_FLASH_Read_Spare(u8* ReadBuffer, u32 ReadAddr, u16 NumOfReadByte)
{
  u32 RowAddr = ReadAddr>>11;
  
  SPI_FLASH_Read2Cache(RowAddr);
  
  /* Select the SPI FLASH */
  SPI_FLASH_SEL_ACTIVE();

  /* Send "Read from Memory " instruction */
  SPI_FLASH_SendByte(SPI_FLASH_CMD_READCACHE);
  SPI_FLASH_SendByte(SPI_FLASH_DUMMY_BYTE);

  SPI_FLASH_SendByte(0x08);
  SPI_FLASH_SendByte(0x00);

  while(NumOfReadByte--)
  {
    /* Read a byte from the FLASH */
    *ReadBuffer = SPI_FLASH_SendByte(SPI_FLASH_DUMMY_BYTE);
    /* Point to the next location where the byte read will be saved */
    ReadBuffer++;
  }

  /* Deselect the SPI FLASH */
  SPI_FLASH_SEL_INACTIVE();
}

#if 0
/*********************************************************************************************************//**
  * @brief  Reads a block of data from the FLASH using Dual Output Mode.
  * @param  ReadBuffer : pointer to the buffer that receives the data read from the FLASH.
  * @param  ReadAddr : FLASH's internal address to read from.
  * @param  NumOfReadByte : number of half words to read from the FLASH.
  * @retval None
  ***********************************************************************************************************/
void SPI_FLASH_DualOutputBufferRead(u16* ReadBuffer, u32 ReadAddr, u16 NumOfReadByte)
{
  u32 RowAddr = ReadAddr>>11;
  
  SPI_FLASH_Read2Cache(RowAddr);

  /* Select the SPI FLASH */
  SPI_FLASH_SEL_ACTIVE();

  /* Send "Read from Memory " instruction */
  SPI_FLASH_SendByte(SPI_FLASH_CMD_READCACHE2);
  /* Send dummy byte */
  SPI_FLASH_SendByte(SPI_FLASH_DUMMY_BYTE);
  
  /* Send address */
  SPI_FLASH_SendByte((ReadAddr>>8)&0x07);
  SPI_FLASH_SendByte(ReadAddr&0xFF);

  /* Send dummy byte */
  SPI_FLASH_SendByte(SPI_FLASH_DUMMY_BYTE);
  /* Enable Dual Port */
  SPI_DUALCmd(FLASH_SPI, ENABLE);

  NumOfReadByte >>= 1;
  while(NumOfReadByte--)
  {
    /* Read a byte from the FLASH */
    *ReadBuffer = SPI_FLASH_SendByte(SPI_FLASH_DUMMY_BYTE);
    /* Point to the next location where the byte read will be saved */
    ReadBuffer++;
  }

  /* Deselect the SPI FLASH */
  SPI_FLASH_SEL_INACTIVE();

  /* Disable Dual Port */
  SPI_DUALCmd(FLASH_SPI, DISABLE);
}
#endif

/*********************************************************************************************************//**
  * @brief  Reads JEDEC ID.
  * @retval JEDEC ID value.
  ***********************************************************************************************************/
u32 SPI_FLASH_ReadJEDECID(void)
{
  u32 Temp = 0, Temp0 = 0, Temp1 = 0, Temp2 = 0;

  /* Select the SPI FLASH */
  SPI_FLASH_SEL_ACTIVE();

  /* Send "JEDEC Read-ID" instruction */
  SPI_FLASH_SendByte(SPI_FLASH_CMD_RDID);

  /* Read a byte from the FLASH */
  Temp0 = SPI_FLASH_SendByte(SPI_FLASH_DUMMY_BYTE);

  /* Read a byte from the FLASH */
  Temp1 = SPI_FLASH_SendByte(SPI_FLASH_DUMMY_BYTE);

  /* Read a byte from the FLASH */
  Temp2 = SPI_FLASH_SendByte(SPI_FLASH_DUMMY_BYTE);

  /* Deselect the SPI FLASH */
  SPI_FLASH_SEL_INACTIVE();

  Temp = (Temp0 << 16) | (Temp1 << 8) | Temp2;

  return Temp;

}



/*********************************************************************************************************//**
  * @brief  Writes block of data to the FLASH with a single WRITE cycle.
  * @param  WriteBuffer : pointer to the buffer  containing the data to be written to the FLASH.
  * @param  WriteAddr : FLASH's internal address to write to.
  * @param  NumOfWriteByte : number of bytes to write to the FLASH.
  * @retval None
  ***********************************************************************************************************/
void SPI_FLASH_PageWrite(u8* WriteBuffer, u32 WriteAddr, u16 NumOfWriteByte)
{
  /* Select the SPI FLASH */
  SPI_FLASH_SEL_ACTIVE();
  /* Send "Page Program" instruction */
  SPI_FLASH_SendByte(SPI_FLASH_CMD_PLOAD);

  SPI_FLASH_SendByte((WriteAddr>>8)&0x07);
  SPI_FLASH_SendByte(WriteAddr&0xFF);


  while(NumOfWriteByte--)
  {
    /* Send the current byte */
    SPI_FLASH_SendByte(*WriteBuffer);
    /* Point on the next byte to be written */
    WriteBuffer++;
  }

  /* Deselect the SPI FLASH */
  SPI_FLASH_SEL_INACTIVE();

  /* Enable the write access to the FLASH */
  SPI_FLASH_WriteEnable();

  
  WriteAddr >>= 11;
  /* Select the SPI FLASH */
  SPI_FLASH_SEL_ACTIVE();
  /* Send "Page Program" instruction */
  SPI_FLASH_SendByte(SPI_FLASH_CMD_PEXECUTE);

  SPI_FLASH_SendByte((WriteAddr>>16)&0xff);
  SPI_FLASH_SendByte((WriteAddr>>8 )&0xff);
  SPI_FLASH_SendByte((WriteAddr>>0 )&0xff);

  /* Deselect the SPI FLASH */
  SPI_FLASH_SEL_INACTIVE();


  SPI_FLASH_WaitForWriteEnd();
}



/*********************************************************************************************************//**
  * @brief  Erases the specified FLASH block.
  * @param  SectorAddr: address of the block to erase.
  * @retval None
  ***********************************************************************************************************/
void SPI_NAND_FLASH_BlockErase(u32 BlockAddr)
{
  u32 RowAddr = BlockAddr>>11;
  /* Send write enable instruction */
  SPI_FLASH_WriteEnable();

  /* Select the SPI FLASH */
  SPI_FLASH_SEL_ACTIVE();
  /* Send "Block Erase" instruction */
  SPI_FLASH_SendByte(SPI_FLASH_CMD_BE);
  /* Send high byte address of BlockAddr */
  SPI_FLASH_SendByte((RowAddr & 0xFF0000) >> 16);
  SPI_FLASH_SendByte((RowAddr & 0xFF00) >> 8);
  SPI_FLASH_SendByte(RowAddr & 0xFF);
  /* Deselect the SPI FLASH */
  SPI_FLASH_SEL_INACTIVE();

  /* Wait the end of Flash writing */
  SPI_FLASH_WaitForWriteEnd();
}



/*********************************************************************************************************//**
  * @brief  Writes the new value to the Status Register.
  * @param  Value : the new value to be written to the Status Register.
  * @retval None
  ***********************************************************************************************************/
void SPI_FLASH_reset(void)
{
  SPI_FLASH_SEL_ACTIVE();

  /* Send "Write Status Register" instruction */
  SPI_FLASH_SendByte(SPI_FLASH_RESET);
  /* Writes to the Status Register */

  /* Deselect the SPI FLASH */
  SPI_FLASH_SEL_INACTIVE();
}


/*********************************************************************************************************//**
	*	@brief	SPI_FLASH_ReadDMA
  * @param  ReadAddr :  
  * @param  DstAddr :  
  * @param  DataSize : 
  * @param  DstAddrMode : 
  *   @arg : SPI_FLASH_DMA_ADR_FIX_MODE
  *   @arg : SPI_FLASH_DMA_ADR_INC_MODE
	*	@retval	None
	***********************************************************************************************************/
void SPI_FLASH_ReadDMA(u8* ReadBuffer,u32 DstAddr, u32 DataSize, u8 DstAddrMode)
{
  u32 RowAddr = DstAddr>>11;
  PDMACH_InitTypeDef PDMACH_InitStructure;
  u32 blkcnt = 0;
  
  SPI_FLASH_Read2Cache(RowAddr);

  /* Wait until TX FIFO empty                                                                               */
  while (SPI_GetFIFOStatus(FLASH_SPI, SPI_FIFO_TX) > 0);

  /* Select the SPI FLASH */
  SPI_FLASH_SEL_ACTIVE();

  /* Send "Read from Memory " instruction */
  SPI_FLASH_SendByte(SPI_FLASH_CMD_READCACHE2);
  /* Send dummy byte */
  SPI_FLASH_SendByte(SPI_FLASH_DUMMY_BYTE);
  
  /* Send address */
  SPI_FLASH_SendByte((DstAddr>>8)&0x07);
  SPI_FLASH_SendByte(DstAddr&0xFF);

  /* Send dummy byte */
  SPI_FLASH_SendByte(SPI_FLASH_DUMMY_BYTE);
  /* Enable Dual Port */
  SPI_DUALCmd(FLASH_SPI, ENABLE);
  blkcnt = (DataSize+1) / 2;  

/////////////////////////////////////////
 
  /* SPIx Tx  PDMA_SPI0_TX */  
  PDMACH_InitStructure.PDMACH_SrcAddr = 0; 
  PDMACH_InitStructure.PDMACH_DstAddr = (u32)&FLASH_SPI->DR;
  PDMACH_InitStructure.PDMACH_BlkCnt = blkcnt;
  PDMACH_InitStructure.PDMACH_BlkLen = 1;
  PDMACH_InitStructure.PDMACH_DataSize = WIDTH_16BIT;
  PDMACH_InitStructure.PDMACH_Priority = H_PRIO;
  PDMACH_InitStructure.PDMACH_AdrMod  = SRC_ADR_FIX | DST_ADR_FIX;
  PDMA_Config(SPI_FLASH_PORT_TX_DMA_CH, &PDMACH_InitStructure); 
  
  /* SPIx Rx */
  PDMACH_InitStructure.PDMACH_SrcAddr = (u32)&FLASH_SPI->DR;
  PDMACH_InitStructure.PDMACH_DstAddr = (u32)ReadBuffer;
  PDMACH_InitStructure.PDMACH_Priority = VH_PRIO;
  if(DstAddrMode == SPI_FLASH_DMA_ADR_INC_MODE)
  {
    PDMACH_InitStructure.PDMACH_AdrMod  = SRC_ADR_FIX | DST_ADR_LIN_INC;
  }
  PDMA_Config(SPI_FLASH_PORT_RX_DMA_CH, &PDMACH_InitStructure);
 
  PDMA_IntConig(SPI_FLASH_PORT_TX_DMA_CH, PDMA_INT_TC | PDMA_INT_GE, ENABLE);
  PDMA_IntConig(SPI_FLASH_PORT_RX_DMA_CH, PDMA_INT_TC | PDMA_INT_GE, ENABLE);
  PDMA_EnaCmd(SPI_FLASH_PORT_RX_DMA_CH, ENABLE);
  PDMA_EnaCmd(SPI_FLASH_PORT_TX_DMA_CH, ENABLE);
  
  SPI_PDMACmd(FLASH_SPI, SPI_PDMAREQ_TX | SPI_PDMAREQ_RX, ENABLE);
  
  SPI_FLASH_IsDMA_Busy = TRUE;
  
  SPI_FLASH_WaitDMA_Finished();
  
  /* Deselect the SPI FLASH */
  SPI_FLASH_SEL_INACTIVE();

  /* Disable Dual Port */
  SPI_DUALCmd(FLASH_SPI, DISABLE);
  
  
  
//  //debug only
//  {
//    u8 ecc_status = (SPI_FLASH_GetFeature(0XC0)>>4)&0x07;
//    if(ecc_status)
//    {  
//      TRACE("ecc:%X\tAddr:%X\r\n",ecc_status, DstAddr);
//    }
//  }    
}


/*********************************************************************************************************//**
 * @brief  : SPI_FLASH_CheckDMA_Finished
 * @param  :  
 * @param  : 
 * @retval None 
 ************************************************************************************************************/
bool SPI_FLASH_CheckDMA_Finished(void)
{
  if(SPI_FLASH_IsDMA_Busy)
  {
//  if(PDMA_GetFlagStatus(SPI_FLASH_PORT_RX_DMA_CH, PDMA_FLAG_TC))
    if(SPI_GetFlagStatus(FLASH_SPI, SPI_INT_TXE))
    {
      PDMA_ClearFlag(SPI_FLASH_PORT_RX_DMA_CH, PDMA_FLAG_TC);
      SPI_FLASH_SEL_INACTIVE();  
      SPI_PDMACmd(FLASH_SPI, SPI_PDMAREQ_TX | SPI_PDMAREQ_RX, DISABLE);
      SPI_DUALCmd(FLASH_SPI, DISABLE);
      SPI_FLASH_IsDMA_Busy = FALSE;
    }
  }
  return (bool)(!SPI_FLASH_IsDMA_Busy);
}

/*********************************************************************************************************//**
 * @brief  : WaitsSPI_FLASH_DMA_Finished
 * @param  :  
 * @param  : 
 * @retval None 
 ************************************************************************************************************/
void SPI_FLASH_WaitDMA_Finished(void)
{
  while(SPI_FLASH_CheckDMA_Finished() == FALSE);
}


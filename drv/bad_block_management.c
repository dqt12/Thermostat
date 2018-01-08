/*********************************************************************************************************//**
 * @file    bad_block_management.c
 * @version V1.00
 * @date    20/7/2016
 * @brief   spi nand flash bad block management
 *************************************************************************************************************
 *
 * <h2><center>Copyright (C) 2016 Holtek Semiconductor Inc. All rights reserved</center></h2>
 *
 ************************************************************************************************************/

/* Includes ------------------------------------------------------------------------------------------------*/
#include "bad_block_management.h"
#include "spi_flash_GD5FxGq4xC.h"
#include "string.h"

//#include "TRACE.h"
#define TRACE(...)


#define LCD_Bank_ADDR         0x60000000

#define BAD_BLOCK_MAP_ADDR    (127*1024) //for HT32F52352
BAD_BLOCK_INFO_Typedef bad_block_info;

/*********************************************************************************************************//**
 * @brief  : 
 * @retval : 
 ************************************************************************************************************/
static u16 crc16(u8 *ptr,u8 len)
{
	u16 i; 
	u16 crc=0xffff;
	while(len--) { 
		for(i=1; i <= 0x80; i<<=1) { 
			if((crc&0x8000)!=0) {
				crc <<= 1; 
				crc ^= 0x8005;
			} else 
				crc <<= 1;                      
			if((*ptr&i) != 0) 
				crc ^= 0x8005;                         
		} 
		ptr++; 
	}
	crc ^= 0xffff;	//取反
	crc &= 0xffff;	//只要低16bit
	return(crc); 
} 


/*********************************************************************************************************//**
 * @brief  : 
 * @retval : 
 ************************************************************************************************************/
void init_spi_nand_flash(void)
{
  u32 *pTable = (u32 *)BAD_BLOCK_MAP_ADDR;
  u16 i, crc;
  u8 error_table = 0;
  SPI_FLASH_Init();
	
  TRACE("FLASH ID:%X\r\n", SPI_FLASH_ReadJEDECID());  
//  TRACE("A0:%X\r\n", SPI_FLASH_GetFeature(0xA0));  
//  TRACE("B0:%X\r\n", SPI_FLASH_GetFeature(0xB0));  
//  TRACE("C0:%X\r\n", SPI_FLASH_GetFeature(0xC0));  
  
  if(pTable[0] <= MAX_BAD_BLOCK)
  {
    crc = crc16((u8 *)pTable, (pTable[0] + 1) * 4);
    if(crc != pTable[pTable[0] + 1])  //crc match
      error_table = 1;
    else
    {
      memcpy((u8 *)&bad_block_info.total_num, (u8 *)pTable, (pTable[0] + 1) * 4);
    }
  }
  else
    error_table = 1;

  if(error_table)
  {
    u32 addr = BAD_BLOCK_MAP_ADDR;
    u32 *dat = (u32 *)&bad_block_info.total_num;

    search_bad_block();
    crc = crc16((u8 *)&bad_block_info.total_num, (bad_block_info.total_num + 1) * 4);
    
    FLASH_ErasePage(BAD_BLOCK_MAP_ADDR);
    for(i=0; i<bad_block_info.total_num + 1; i++)
    {
      FLASH_ProgramWordData(addr, *dat++);
      addr += 4;
    }
    FLASH_ProgramWordData(addr, crc);
  }
	
	SPI_FLASH_WriteStatus(0x00);
}


/*********************************************************************************************************//**
 * @brief  : 
 * @retval : 
 ************************************************************************************************************/
void search_bad_block(void)
{
	s16 total_blk,bad_blk,i;
	u32 address = 0;
	u32 temp_flash_buf[4];
	
	for(total_blk=0;total_blk<MAX_BLOCK_NUM;total_blk++)
	{
//    SPI_FLASH_ReadDMA((u16 *)nand_flash_buf, address, NAND_FLASH_PAGESIZE,SPI_FLASH_DMA_ADR_INC_MODE);
    SPI_FLASH_Read_Spare((u8 *)temp_flash_buf, address, 16);
    
    if( (temp_flash_buf[0] != 0xffffffff)
      ||(temp_flash_buf[1] != 0xffffffff)
      ||(temp_flash_buf[2] != 0xffffffff)
      ||(temp_flash_buf[3] != 0xffffffff) )	//1st page spare data
    {
      bad_block_info.info[bad_block_info.total_num++].bad = address/BLOCK_SIZE;

//      TRACE("%.8X:bad block[%d]\r\n",address,total_blk);
    }
    address += BLOCK_SIZE;
	}
  
  total_blk = MAX_BLOCK_NUM-1;
  bad_blk = bad_block_info.total_num;
  if(bad_blk)
  {
    while(total_blk)
    {
      u8 flag = 0;
      for(i=0; i<bad_block_info.total_num; i++) //检查此块是否是坏块列表内？
      {
        if(bad_block_info.info[i].bad == total_blk)
        {
          flag = 1; //是坏块，退出
          break;
        }
      }
      
      if(!flag) //不是坏块
      {
        bad_block_info.info[bad_blk-1].remap = total_blk;
        bad_blk--;
        if(bad_blk <= 0)
          break;
      }
      total_blk--;
    }
  }
  
  for(i=0; i<bad_block_info.total_num; i++)
  {
    TRACE("bad block %d remap to-->%d\r\n", bad_block_info.info[i].bad, bad_block_info.info[i].remap);
  }
}

u16 block_remap(u16 block)
{
  u8 i;
  for(i=0; i<bad_block_info.total_num; i++)
  {
    if(block == bad_block_info.info[i].bad)
    {
      block = bad_block_info.info[i].remap;
      break;
    }
  }
  return block;
}


/*********************************************************************************************************//**
 * @brief  : 请从偶地址读取，读取数目也需要是偶数；不支持奇数!!
 * @retval : 
 ************************************************************************************************************/
void SPI_FLASH_BufferRead(u8* ReadBuffer,u32 DstAddr, u32 DataSize)
{
  u32 remap_address = DstAddr + DataSize;
  u32 length;
  u32 target_blk = block_remap(DstAddr>>17); //DstAddr/BLOCK_SIZE;

//  TRACE("R:%X\t%X\r\n", DstAddr, DataSize);
  
  if( (DstAddr & (~(NAND_FLASH_PAGESIZE-1))) != (remap_address & (~(NAND_FLASH_PAGESIZE-1))) ) //是否跨页?
  {
    length = NAND_FLASH_PAGESIZE - (DstAddr & (NAND_FLASH_PAGESIZE-1));
    remap_address = target_blk*BLOCK_SIZE + (DstAddr&(BLOCK_SIZE-1));
    SPI_FLASH_ReadDMA(ReadBuffer, remap_address, length, SPI_FLASH_DMA_ADR_INC_MODE);
    
    DataSize -= length;
    DstAddr += length;
    ReadBuffer += length;
    target_blk = block_remap(DstAddr>>17);
  }
  
  while(DataSize > NAND_FLASH_PAGESIZE)
  {
    remap_address = target_blk*BLOCK_SIZE + (DstAddr&(BLOCK_SIZE-1));
    SPI_FLASH_ReadDMA(ReadBuffer, remap_address, NAND_FLASH_PAGESIZE, SPI_FLASH_DMA_ADR_INC_MODE);
    DataSize -= NAND_FLASH_PAGESIZE;
    DstAddr += NAND_FLASH_PAGESIZE;
    ReadBuffer += NAND_FLASH_PAGESIZE;
    target_blk = block_remap(DstAddr>>17);
  }
  if(DataSize)
  {
    remap_address = target_blk*BLOCK_SIZE + (DstAddr&(BLOCK_SIZE-1));
    SPI_FLASH_ReadDMA(ReadBuffer, remap_address, DataSize, SPI_FLASH_DMA_ADR_INC_MODE);
  }
}


/*********************************************************************************************************//**
 * @brief  : 请从偶地址读取，读取数目也需要是偶数；不支持奇数!!
 * @retval : 
 ************************************************************************************************************/
void SPI_FLASH_BufferRead2LCD(u32 DstAddr, u32 DataSize)
{
  u32 remap_address = DstAddr + DataSize;
  u32 length;
  u32 target_blk = block_remap(DstAddr>>17); //DstAddr/BLOCK_SIZE;
  u8* ReadBuffer = (u8*)(LCD_Bank_ADDR+2);

//  TRACE("R:%X\t%X\r\n", DstAddr, DataSize);
  
  if( (DstAddr & (~(NAND_FLASH_PAGESIZE-1))) != (remap_address & (~(NAND_FLASH_PAGESIZE-1))) ) //是否跨页?
  {
    length = NAND_FLASH_PAGESIZE - (DstAddr & (NAND_FLASH_PAGESIZE-1));
    remap_address = target_blk*BLOCK_SIZE + (DstAddr&(BLOCK_SIZE-1));
    SPI_FLASH_ReadDMA(ReadBuffer, remap_address, length, SPI_FLASH_DMA_ADR_FIX_MODE);
    
    DataSize -= length;
    DstAddr += length;
//    ReadBuffer += length/2;
    target_blk = block_remap(DstAddr>>17);
  }
  
  while(DataSize > NAND_FLASH_PAGESIZE)
  {
    remap_address = target_blk*BLOCK_SIZE + (DstAddr&(BLOCK_SIZE-1));
    SPI_FLASH_ReadDMA(ReadBuffer, remap_address, NAND_FLASH_PAGESIZE, SPI_FLASH_DMA_ADR_FIX_MODE);
    DataSize -= NAND_FLASH_PAGESIZE;
    DstAddr += NAND_FLASH_PAGESIZE;
//    ReadBuffer += NAND_FLASH_PAGESIZE/2;
    target_blk = block_remap(DstAddr>>17);
  }
  if(DataSize)
  {
    remap_address = target_blk*BLOCK_SIZE + (DstAddr&(BLOCK_SIZE-1));
    SPI_FLASH_ReadDMA(ReadBuffer, remap_address, DataSize, SPI_FLASH_DMA_ADR_FIX_MODE);
  }
}



/*********************************************************************************************************//**
 * @brief  : 
 * @retval : 
 ************************************************************************************************************/
void SPI_FLASH_BlockErase(u32 BlockAddr)
{
  u32 remap_address;
  u32 target_blk = block_remap(BlockAddr>>17); //BlockAddr/BLOCK_SIZE;
//  TRACE("E:%.8X\t%X\r\n", BlockAddr, target_blk);

  remap_address = target_blk*BLOCK_SIZE + (BlockAddr&(BLOCK_SIZE-1));
  SPI_NAND_FLASH_BlockErase(remap_address);
}

/*********************************************************************************************************//**
 * @brief  : 
 * @retval : 
 ************************************************************************************************************/
void SPI_FLASH_BufferWrite(u8* WriteBuffer, u32 WriteAddr, u16 NumOfWriteByte)
{
  u32 remap_address;
  u32 target_blk = block_remap(WriteAddr>>17); //WriteAddr/BLOCK_SIZE;

//  TRACE("W:%X\t%X\r\n", WriteAddr, NumOfWriteByte);
  
  while(NumOfWriteByte > NAND_FLASH_PAGESIZE)
  {
    remap_address = target_blk*BLOCK_SIZE + (WriteAddr & (BLOCK_SIZE-1));
    SPI_FLASH_PageWrite(WriteBuffer, remap_address, NAND_FLASH_PAGESIZE);
    NumOfWriteByte -= NAND_FLASH_PAGESIZE;
    WriteAddr += NAND_FLASH_PAGESIZE;
    WriteBuffer += NAND_FLASH_PAGESIZE/2;
    target_blk = block_remap(WriteAddr>>17);
  }
  remap_address = target_blk*BLOCK_SIZE + (WriteAddr & (BLOCK_SIZE-1));
  SPI_FLASH_PageWrite(WriteBuffer, remap_address, NumOfWriteByte);

}


/*********************************************************************************************************//**
 * @brief  : 
 * @retval : 
 ************************************************************************************************************/
void SPI_FLASH_WriteStatus(u8 Value)
{
  SPI_FLASH_SetFeature(0xA0, Value);
}

/*********************************************************************************************************//**
 * @brief  : 
 * @retval : 
 ************************************************************************************************************/
void SPI_FLASH_Config_ECC(u8 enable)
{
  SPI_FLASH_SetFeature(0xB0, enable ? 0x10 : 0x00);
}



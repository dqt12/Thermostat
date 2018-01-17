/*********************************************************************************************************//**
 * @file    EBI/LCD/main.c
 * @version V1.00
 * @date    10/01/2013
 * @brief   Main program.
 *************************************************************************************************************
 *
 * <h2><center>Copyright (C) 2013 Holtek Semiconductor Inc. All rights reserved</center></h2>
 *
 ************************************************************************************************************/

/* Includes ------------------------------------------------------------------------------------------------*/
#include <stdlib.h>

#include "lcd_display.h"
#include "lcm.h"
#include "spi_flash.h"

/* Setting */
#define LCD_DISPLAY_MAX_COLUMN_PIXEL  (480)
#define LCD_DISPLAY_PAGE_BUFFER_LINE  (4)
#define LCD_DISPLAY_PIC_BUFFER_SIZE   (LCD_DISPLAY_MAX_COLUMN_PIXEL * LCD_DISPLAY_PAGE_BUFFER_LINE)

#define LCD_DISPLAY_TIMEBASE_FREQ     10000 //TIMEBASE_FREQUENCY

/* Private typedef -----------------------------------------------------------------------------------------*/

/* Private LCD Display data typedef */
typedef __PACKED_H struct
{
  /* For Transparent Display Mode */
  LCD_DISPLAY_PositionTypeDef Position;
  u16 CurrentImageWidth;
  u16 TransparentColor;
  bool IsResetPosition;

  /* Frame Information */
  LCD_DISPLAY_FrameInfoTypeDef* pFrameInfo;

  /* Play Information */
  u16 ImageStartIndex;
  u16 ImageEndIndex;
  u16* pImageRemapTable;
  u16 ImageCurrentIndex;
  u16 ImageCurrentIndexMap;
  u16 ImagePositionIndex;
  /* For GetNextDisplayInfo function use */
  u32 ImageEndAddress;
  bool IsNeedSetDisplayArea;
  
  /* Frame Rate Information */
  u8 FrameRate;
  vu16 FrameRateCounter;
  u16 FrameRateCounterReload;

  /* SPI Flash information */
  u32 NextDataAddress; 
  u16 NextDataLength;
  u8 AltBufferIndex;
} __PACKED_F LCD_DISPLAY_Typedef;

/* Private constants ---------------------------------------------------------------------------------------*/
/* Global variables ----------------------------------------------------------------------------------------*/
LCD_DISPLAY_InfoTypedef gLCD_DISPLAY;

/* Private define ------------------------------------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------------------------------------*/

/* Image Information */
LCD_DISPLAY_ImageInfoTypeDef gLCD_Display_ImageInfo;
/* since the buffer use PDMA, it must align 4  */
__ALIGN4 u8 LCD_Display_PinponBuffer[2][LCD_DISPLAY_PIC_BUFFER_SIZE];
LCD_DISPLAY_Typedef gLCD_Display;
static void (*LCD_Display_fnDisplayProcess)(LCD_DISPLAY_Typedef* pDisplay);

/* Private function prototypes -----------------------------------------------------------------------------*/

void LCD_DISPLAY_Process_Normal(LCD_DISPLAY_Typedef* pDisplay);
void LCD_DISPLAY_Process_Transparent(LCD_DISPLAY_Typedef* pDisplay);
void LCD_DISPLAY_Process_TransparentReverse(LCD_DISPLAY_Typedef* pDisplay);
void LCD_DISPLAY_GetNextDisplayInfo(LCD_DISPLAY_Typedef* pDisplay);
void LCD_DISPLAY_GetNextDisplayInfo_Transparent(LCD_DISPLAY_Typedef* pDisplay);
LCD_DISPLAY_FlagTypedef LCD_DISPLAY_GetImageInfo(void);

/* Private functions ---------------------------------------------------------------------------------------*/

/*********************************************************************************************************//**
 * @brief  : LCD_DISPLAY_TimebaseHandler 
 * @param  : none
 * @retval : none
 ************************************************************************************************************/
void LCD_DISPLAY_TimebaseHandler(void)
{
  if(gLCD_Display.FrameRateCounter)
  {
    gLCD_Display.FrameRateCounter--;
  }
}

/*********************************************************************************************************//**
  * @brief  LCD_DISPLAY_Process
  * @param  : None
  * @retval : None
  ***********************************************************************************************************/
void LCD_DISPLAY_Process(void)
{
  if(LCD_Display_fnDisplayProcess)
  {
    LCD_Display_fnDisplayProcess(&gLCD_Display);
  }
}

/*********************************************************************************************************//**
 * @brief  : LCD_DISPLAY_Init 
 * @param  : pInit->pFrameInfo
 * @param  : pInit->ImageStartIndex
 * @param  : pInit->ImageLength
 * @param  : pInit->pImageRemapTable
 * @param  : pInit->FrameRate
 * @param  : pInit->Mode
 * @param  : pInit->TransparentColor
 * @retval : None
 ************************************************************************************************************/
void LCD_DISPLAY_Init(LCD_DISPLAY_InitTypedef* pInit)
{ 
  LCD_DISPLAY_Typedef *pDisplay = &gLCD_Display;
  LCD_DISPLAY_ImageTypeDef *pImgInfo;
  LCD_DISPLAY_PositionTypeDef *pImgPosition;
  u32 tmp;

  gLCD_DISPLAY.ImageCounter     = 0;

  pDisplay->pFrameInfo          = pInit->pFrameInfo;
  pDisplay->pImageRemapTable    = pInit->pImageRemapTable;
  pDisplay->ImageStartIndex     = pInit->ImageStartIndex;
  pDisplay->ImageEndIndex       = pInit->ImageStartIndex + pInit->ImageLength;
  pDisplay->ImageCurrentIndex   = pDisplay->ImageStartIndex;
  pDisplay->ImagePositionIndex  = 0;
  pDisplay->AltBufferIndex      = 0;
  pDisplay->FrameRate           = pInit->FrameRate;

  /* Get frame rate counter reload value */
  if(pInit->FrameRate)
  {
    pDisplay->FrameRateCounterReload = LCD_DISPLAY_TIMEBASE_FREQ / pInit->FrameRate;
  }
  else
  {
    pDisplay->FrameRateCounterReload = 0;
  }
  pDisplay->FrameRateCounter = pDisplay->FrameRateCounterReload;

  /* Get current image index remap value */
  if(pDisplay->pImageRemapTable != NULL)
  {
    pDisplay->ImageCurrentIndexMap = pDisplay->pImageRemapTable[pDisplay->ImageCurrentIndex];
  }
  else
  {
    pDisplay->ImageCurrentIndexMap = pDisplay->ImageCurrentIndex;
  }

  pImgPosition = &pDisplay->pFrameInfo->ImagePositionTable[0];
  pImgInfo = &gLCD_Display_ImageInfo.Table[pDisplay->ImageCurrentIndexMap];
  pDisplay->NextDataAddress = pImgInfo->ImageStartAddr;
  pDisplay->ImageEndAddress = pDisplay->NextDataAddress + \
                              (((u32)pImgInfo->ImageWidth * pImgInfo->ImageHigh) * 3);
  
  if(pInit->Mode == LCD_DISPLAY_MODE_NORMAL)
  {
    LCD_SetDisplayArea(pImgPosition->Column, pImgPosition->Page,
                       pImgInfo->ImageHigh, pImgInfo->ImageWidth);
    LCD_WriteRAMPrior();
    
    pDisplay->IsNeedSetDisplayArea = FALSE;   

    /* Select Display Function  */
    LCD_Display_fnDisplayProcess = LCD_DISPLAY_Process_Normal;
  }
  else if(pInit->Mode == LCD_DISPLAY_MODE_TRANSPARENT_1)
  {
    LCD_DISPLAY_PositionTypeDef *pTransparentPosition;

    pDisplay->TransparentColor = pInit->TransparentColor;
    
    pTransparentPosition = &pDisplay->Position;
    pTransparentPosition->Column = 0;
    pTransparentPosition->Page = 0;

    /* Select Display Function */
    LCD_Display_fnDisplayProcess = LCD_DISPLAY_Process_Transparent;
  }
  else if(pInit->Mode == LCD_DISPLAY_MODE_TRANSPARENT_2)
  {
    LCD_DISPLAY_PositionTypeDef *pTransparentPosition;

    pDisplay->TransparentColor = pInit->TransparentColor;
    
    pTransparentPosition = &pDisplay->Position;
    pTransparentPosition->Column = 0;
    pTransparentPosition->Page = 0;

    /* Select Display Function  */
    LCD_Display_fnDisplayProcess = LCD_DISPLAY_Process_TransparentReverse;
  }
  else
  {
    /* Display Mode Not Define */
    while(1);
  }

  /* Compute the next data length */
  tmp = pDisplay->ImageEndAddress - pDisplay->NextDataAddress;
  if(tmp > LCD_DISPLAY_PIC_BUFFER_SIZE)
  {
    pDisplay->NextDataLength = LCD_DISPLAY_PIC_BUFFER_SIZE;
  }
  else
  {
    pDisplay->NextDataLength = tmp;
  }

  /* Wait previous SPI DMA finished */
  if(SPI_FLASH_IsDMA_Busy)
  {
    SPI_FLASH_WaitDMA_Finished();
  }

  /* Start SPI Flash DMA transfer */
  SPI_FLASH_ReadDMA(  pDisplay->NextDataAddress, 
                      (u32)&LCD_Display_PinponBuffer[pDisplay->AltBufferIndex][0],
                      //(u32)&EBI_LCD->EBI_LCD_RAM,
                      pDisplay->NextDataLength,
                      SPI_FLASH_DMA_ADR_INC_MODE);
                      //SPI_FLASH_DMA_ADR_FIX_MODE);
}

/*********************************************************************************************************//**
 * @brief  : Get the next image information (For Normal Display Mode) 
 * @param  : pDisplay
 * @retval : None
 ************************************************************************************************************/
void LCD_DISPLAY_GetNextDisplayInfo(LCD_DISPLAY_Typedef* pDisplay)
{
  LCD_DISPLAY_ImageTypeDef *pImgInfo;
  LCD_DISPLAY_PositionTypeDef *pImgPosition;
  u32 tmp;

  if(pDisplay->IsNeedSetDisplayArea)
  {
    pImgInfo = &gLCD_Display_ImageInfo.Table[pDisplay->ImageCurrentIndexMap];
    pImgPosition = &pDisplay->pFrameInfo->ImagePositionTable[pDisplay->ImagePositionIndex];

    pDisplay->IsNeedSetDisplayArea = FALSE;    
    LCD_SetDisplayArea(pImgPosition->Column, pImgPosition->Page,
                       pImgInfo->ImageHigh, pImgInfo->ImageWidth);
    LCD_WriteRAMPrior();
    
    while(pDisplay->FrameRateCounter);
   
    pDisplay->FrameRateCounter = pDisplay->FrameRateCounterReload;
  }

  pDisplay->NextDataAddress += pDisplay->NextDataLength;
  
  /* Process if SPI Flash address is the end of this image */
  if(pDisplay->NextDataAddress == pDisplay->ImageEndAddress)
  {
    tmp = pDisplay->ImageCurrentIndex + 1;
    if(tmp == pDisplay->ImageEndIndex)
    {
      pDisplay->ImageCurrentIndex = pDisplay->ImageStartIndex;
    }
    else
    {
      pDisplay->ImageCurrentIndex = tmp;
    }

    tmp = pDisplay->ImagePositionIndex + 1;
    if(tmp == pDisplay->pFrameInfo->ImageCountPerFrame)
    {
      pDisplay->ImagePositionIndex = 0;
    }
    else
    {
      pDisplay->ImagePositionIndex = tmp;
    }

    if(pDisplay->pImageRemapTable != NULL)
    {
      pDisplay->ImageCurrentIndexMap = pDisplay->pImageRemapTable[pDisplay->ImageCurrentIndex];
    }
    else
    {
      pDisplay->ImageCurrentIndexMap = pDisplay->ImageCurrentIndex;
    }

    pImgInfo = &gLCD_Display_ImageInfo.Table[pDisplay->ImageCurrentIndexMap];

    pDisplay->NextDataAddress = pImgInfo->ImageStartAddr;
    pDisplay->ImageEndAddress = pDisplay->NextDataAddress + \
                                (((u32)pImgInfo->ImageWidth * pImgInfo->ImageHigh) * 3);
    pDisplay->IsNeedSetDisplayArea = TRUE;
    gLCD_DISPLAY.ImageCounter++;
  }
  
  /* Toggle PinPon Buffer */
  pDisplay->AltBufferIndex = 1 - pDisplay->AltBufferIndex;    

  /* Compute the next data length */
  tmp = pDisplay->ImageEndAddress - pDisplay->NextDataAddress;
  if(tmp > LCD_DISPLAY_PIC_BUFFER_SIZE)
  {
    pDisplay->NextDataLength = LCD_DISPLAY_PIC_BUFFER_SIZE;
  }
  else
  {
    pDisplay->NextDataLength = tmp;
  }
} 

/*********************************************************************************************************//**
  * @brief  LCD_DISPLAY_Process_Normal (For Normal Display Mode)
  * @param  : None
  * @retval : None
  ***********************************************************************************************************/
void LCD_DISPLAY_Process_Normal(LCD_DISPLAY_Typedef* pDisplay)
{
  u32 picsize;
  u8* pBuf = (u8*)&LCD_Display_PinponBuffer[pDisplay->AltBufferIndex][0];
	static u16 color; 
  /* Getting the next SPI Flash Address and Length and the next AltBufferIndex */
  LCD_DISPLAY_GetNextDisplayInfo(pDisplay);

  /* Wait previous SPI DMA finished */
  if(SPI_FLASH_IsDMA_Busy)
  {
    SPI_FLASH_WaitDMA_Finished();
  }
  
  /* Configures SPI and PDMA to reading the data from SPI Flash via PDMA */
  SPI_FLASH_ReadDMA(  pDisplay->NextDataAddress, 
                      (u32)&LCD_Display_PinponBuffer[pDisplay->AltBufferIndex][0],
                      //(u32)&EBI_LCD->EBI_LCD_RAM,
                      pDisplay->NextDataLength,
                      SPI_FLASH_DMA_ADR_INC_MODE);
                      //SPI_FLASH_DMA_ADR_FIX_MODE);
    
  /* Write part of picture to LCD GRAM.
     The action of read SPI Flash via DMA is process at the same time*/
#if  (LCD_CONTROL_MODE == RGB888_EBI8)
		picsize = pDisplay->NextDataLength /3;//a Pixel use a 8bit * 3 =24bit 
		while(picsize--)
		{
			LCD_WriteRAM(*pBuf);//write a 8bit RGB data for EBI8
			pBuf++;
			LCD_WriteRAM(*pBuf);
			pBuf++;
			LCD_WriteRAM(*pBuf);
			pBuf++;		
		}
	
#elif (LCD_CONTROL_MODE == RGB565_EBI16)
		picsize = pDisplay->NextDataLength /2;//a Pixel use a 8bit * 2 =14bit 
		while(picsize--)
		{
			color = *pBuf;
			pBuf++;
			color += (*pBuf)<<8;
			pBuf++;
			LCD_WriteRAM(color);
		}
		
#endif
  
}

/*********************************************************************************************************//**
 * @brief  Get the next image information (For Transparent Display Mode) 
 * @param  pDisplay
 * @retval None
 ************************************************************************************************************/
void LCD_DISPLAY_GetNextDisplayInfo_Transparent(LCD_DISPLAY_Typedef* pDisplay)
{
  LCD_DISPLAY_ImageTypeDef *pImgInfo;
  u32 tmp;

  pDisplay->NextDataAddress += pDisplay->NextDataLength;

  /* Process if SPI Flash address is the end of this image */
  if(pDisplay->NextDataAddress == pDisplay->ImageEndAddress)
  {
    tmp = pDisplay->ImageCurrentIndex + 1;
    if(tmp == pDisplay->ImageEndIndex)
    {
      pDisplay->ImageCurrentIndex = pDisplay->ImageStartIndex;
    }
    else
    {
      pDisplay->ImageCurrentIndex = tmp;
    }

    tmp = pDisplay->ImagePositionIndex + 1;
    if(tmp == pDisplay->pFrameInfo->ImageCountPerFrame)
    {
      pDisplay->ImagePositionIndex = 0;
    }
    else
    {
      pDisplay->ImagePositionIndex = tmp;
    }

    if(pDisplay->pImageRemapTable != NULL)
    {
      pDisplay->ImageCurrentIndexMap = pDisplay->pImageRemapTable[pDisplay->ImageCurrentIndex];
    }
    else
    {
      pDisplay->ImageCurrentIndexMap = pDisplay->ImageCurrentIndex;
    }

    pImgInfo = &gLCD_Display_ImageInfo.Table[pDisplay->ImageCurrentIndexMap];

    pDisplay->NextDataAddress = pImgInfo->ImageStartAddr;
    pDisplay->ImageEndAddress = pDisplay->NextDataAddress + \
                                (((u32)pImgInfo->ImageWidth * pImgInfo->ImageHigh) * 3);

    pDisplay->IsResetPosition = TRUE;
    
    gLCD_DISPLAY.ImageCounter++;
  }

  pDisplay->AltBufferIndex = 1 - pDisplay->AltBufferIndex;    

  /* Compute the next data length */
  tmp = pDisplay->ImageEndAddress - pDisplay->NextDataAddress;
  if(tmp > LCD_DISPLAY_PIC_BUFFER_SIZE)
  {
    pDisplay->NextDataLength = LCD_DISPLAY_PIC_BUFFER_SIZE;
  }
  else
  {
    pDisplay->NextDataLength = tmp;
  }
} 

/*********************************************************************************************************//**
 * @brief  LCD_DISPLAY_Process_Transparent
 * @param  pDisplay
 * @retval None
 ***********************************************************************************************************/
void LCD_DISPLAY_Process_Transparent(LCD_DISPLAY_Typedef* pDisplay)
{
  LCD_DISPLAY_PositionTypeDef *pImgPosition;
  LCD_DISPLAY_ImageTypeDef *pImgInfo;
  LCD_DISPLAY_PositionTypeDef *pTransparentPosition;
  u16 *pBuf;
  u32 picsize;
  u16 ColumnTemp, PageTemp;

  pImgPosition          = &pDisplay->pFrameInfo->ImagePositionTable[pDisplay->ImagePositionIndex];
  pImgInfo              = &gLCD_Display_ImageInfo.Table[pDisplay->ImageCurrentIndexMap];
  pTransparentPosition  = &pDisplay->Position;
  pBuf                  = (u16*)&LCD_Display_PinponBuffer[pDisplay->AltBufferIndex][0];
  picsize               = pDisplay->NextDataLength / 2;
  ColumnTemp = pTransparentPosition->Column;
  PageTemp = pTransparentPosition->Page;
  
  
  /* Getting the next SPI Flash Address and Length and the next AltBufferIndex */
  LCD_DISPLAY_GetNextDisplayInfo_Transparent(pDisplay);

  /* Wait previous SPI DMA finished */
  if(SPI_FLASH_IsDMA_Busy)
  {
    SPI_FLASH_WaitDMA_Finished();
  }
   
  /* Configures SPI and PDMA to reading the data from SPI Flash via PDMA */
  SPI_FLASH_ReadDMA(  pDisplay->NextDataAddress, 
                      (u32)&LCD_Display_PinponBuffer[pDisplay->AltBufferIndex][0],
                      pDisplay->NextDataLength,
                      SPI_FLASH_DMA_ADR_INC_MODE);
    
  /* Write part of picture to LCD GRAM.
     The action of read SPI Flash via DMA is process at the same time*/   
  while(picsize--)
  {
    u16 ColorTemp = *pBuf;
    pBuf++;

    if(ColorTemp != pDisplay->TransparentColor)
    {
      LCD_DrawPoint(pImgPosition->Column + ColumnTemp, 
                    pImgPosition->Page + PageTemp,
                    ColorTemp);
    }
    ColumnTemp++;
    if(ColumnTemp == pImgInfo->ImageWidth)
    {
      ColumnTemp = 0;
      PageTemp++;
    }
  }
  if(pDisplay->IsResetPosition)
  {
    pTransparentPosition->Column = 0;
    pTransparentPosition->Page = 0;
    pDisplay->IsResetPosition = FALSE;
  }
  else
  {
    pTransparentPosition->Column = ColumnTemp;
    pTransparentPosition->Page = PageTemp;
  }
}

/*********************************************************************************************************//**
 * @brief  LCD_DISPLAY_Process_TransparentReverse.
 * @param  pDisplay
 * @retval None
 ***********************************************************************************************************/
void LCD_DISPLAY_Process_TransparentReverse(LCD_DISPLAY_Typedef* pDisplay)
{
  LCD_DISPLAY_PositionTypeDef *pImgPosition;
  LCD_DISPLAY_ImageTypeDef *pImgInfo;
  LCD_DISPLAY_PositionTypeDef *pTransparentPosition;
  u16 *pBuf;
  u32 picsize;
  u16 ColumnTemp, PageTemp;

  pImgPosition          = &pDisplay->pFrameInfo->ImagePositionTable[pDisplay->ImagePositionIndex];
  pImgInfo              = &gLCD_Display_ImageInfo.Table[pDisplay->ImageCurrentIndexMap];
  pTransparentPosition  = &pDisplay->Position;
  pBuf                  = (u16*)&LCD_Display_PinponBuffer[pDisplay->AltBufferIndex][0];
  picsize               = pDisplay->NextDataLength / 2;
  ColumnTemp = pTransparentPosition->Column;
  PageTemp = pTransparentPosition->Page;
  
  /* Getting the next SPI Flash Address and Length and the next AltBufferIndex */
  LCD_DISPLAY_GetNextDisplayInfo_Transparent(pDisplay);

  /* Wait previous SPI DMA finished */
  if(SPI_FLASH_IsDMA_Busy)
  {
    SPI_FLASH_WaitDMA_Finished();
  }
   
  /* Configures SPI and PDMA to reading the data from SPI Flash via PDMA */
  SPI_FLASH_ReadDMA(  pDisplay->NextDataAddress, 
                      (u32)&LCD_Display_PinponBuffer[pDisplay->AltBufferIndex][0],
                      pDisplay->NextDataLength,
                      SPI_FLASH_DMA_ADR_INC_MODE);
    
  /* Write part of picture to LCD GRAM.
     The action of read SPI Flash via DMA is process at the same time*/   
  while(picsize--)
  {
    u16 ColorTemp = *pBuf;
    pBuf++;

    if(ColorTemp == pDisplay->TransparentColor)
    {
      LCD_DrawPoint(pImgPosition->Column + ColumnTemp, 
                    pImgPosition->Page + PageTemp,
                    ColorTemp);
    }
    ColumnTemp++;
    if(ColumnTemp == pImgInfo->ImageWidth)
    {
      ColumnTemp = 0;
      PageTemp++;
    }
  }
  if(pDisplay->IsResetPosition)
  {
    pTransparentPosition->Column = 0;
    pTransparentPosition->Page = 0;
    pDisplay->IsResetPosition = FALSE;
  }
  else
  {
    pTransparentPosition->Column = ColumnTemp;
    pTransparentPosition->Page = PageTemp;
  }
}

/*********************************************************************************************************//**
  * @brief  LCD_DISPLAY_GetImageInfo
  * @param  None 
  * @retval None
  ***********************************************************************************************************/
LCD_DISPLAY_FlagTypedef LCD_DISPLAY_GetImageInfo(void)
{
  LCD_DISPLAY_ImageInfoTypeDef *pImageInfo = &gLCD_Display_ImageInfo;
  u32 TableSize;
  /* Release memory 1st */
  free(pImageInfo->Table);
  
  /* 
      Get image table from serial Flash:
      
      |---------------------------------------------|
      | PIC | An | Hn | Wn | Fn | An+1 | ... | Fn+1 |
      |---------------------------------------------|
    
      > PIC : 2 byte, number of pictures.
      > A   : 4 byte, start address of the specific picture.
      > H   : 2 byte, the height of the specific picture. 
      > W   : 2 byte, the width of the specific picture.  
      > F   : 2 byte, the specific picture format.
  */
  SPI_FLASH_BufferRead((u8*)&pImageInfo->Count, 0x0, 2);

  if ( (pImageInfo->Count != 0) && (pImageInfo->Count != 0xFFFF) )
  {
    TableSize = sizeof(LCD_DISPLAY_ImageTypeDef) * pImageInfo->Count;
    pImageInfo->Table = (LCD_DISPLAY_ImageTypeDef*) malloc(TableSize);
    if (pImageInfo->Table == NULL)
    {
      #if 0  
      /* Without image table condition */
      SPI_FLASH_SectorErase(0x0); 
      BOARD_DBG_PRINTF("First sector erase done!\n\r");
      #endif
		
			
      //BOARD_DBG_PRINTF("\n\rPicture memory alloc error! [%d]\n\r", pImageInfo->Count);
			
      //while (1);
			
			return NODATA;
    }
  }
  else
  {
	
    //BOARD_DBG_PRINTF("\n\rThere is no picture!\n\r");
		
    // while (1);
		
		return NOPIC;
  }
                        
  SPI_FLASH_BufferRead((u8*)pImageInfo->Table, 0x2, TableSize);

	
#if 0  // ... debug info
  {
    int i;
    for (i = 0; i < pImageInfo->Count; i++)
    {
      BOARD_DBG_PRINTF("%2d [0x%06X][%03d][%03d][%02d]\n\r", i, pImageInfo->Table[i].ImageStartAddr, 
                                                      pImageInfo->Table[i].ImageWidth,
                                                      pImageInfo->Table[i].ImageHigh,
                                                      pImageInfo->Table[i].ImageFormat);
    }
  }
#endif
	
		return PICOK;
	
}

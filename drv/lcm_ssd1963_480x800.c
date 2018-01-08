 /*********************************************************************************************************//**
 * @file    lcm_ssd1963_480x800.c
 * @version V1.0
 * @date    12/03/2014
 * @brief   This file provides a set of functions needed to manage the
 *          communication between EBI peripheral and LCD HX8347-D.
 *************************************************************************************************************
 *
 * <h2><center>Copyright (C) 2014 Holtek Semiconductor Inc. All rights reserved</center></h2>
 *
 ************************************************************************************************************/

/* Includes ------------------------------------------------------------------*/
//#include "lcm_ssd1963_480x800.h"

#include "ht32.h"
#include "font.h"
#include <stdlib.h>
#include "lcm.h"
#include "ZH_GPTM.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
//#define LCM_EXCHANGE_X_Y


#if (LCM_DIRECTION == LCM_NORMAL)
  // ---> Column 480 (MY=0 MX=0 MV=0)
  // | 
  // V
  // Page 800
  #define EC (480)
  #define EP (800)
  #define MADCTR (0x00)
#elif (LCM_DIRECTION == LCM_LEFT_90)
  // Column 800
  // ^
  // |  
  // |--> Page 480   (MY=1 MX=0 MV=1)
  #define EC (800)
  #define EP (480)
  #define MADCTR (0xA0)
#elif (LCM_DIRECTION == LCM_RIGHT_90)
  //   Page 480  <--| (MY=0 MX=1 MV=1)
  //                |
  //                V
  //           Column 800
  #define EC (800)
  #define EP (480)
  #define MADCTR (0x60)
#elif (LCM_DIRECTION == LCM_ROTATE_180)
  //              Page 800
  //                ^ 
  //                |
  //  Column 480 <--- (MY=1 MX=1 MV=0)
  #define EC (480)
  #define EP (800)
  #define MADCTR (0xC0)
#elif (LCM_DIRECTION == LCM_ROTATE_180_INV)
  // Page 800
  //   ^ 
  //   |
  //   ---> Column 480  (MY=1 MX=0 MV=0)
  #define EC (480)
  #define EP (800)
  #define MADCTR (0x80)
#endif

#define EBI_LCD_Width		(u16)480	// width, 0-indexed
#define EBI_LCD_Height	(u16)272	// height, 0-indexed

#define	HDP					(u16)479			// Horizontal Display Period
#define	HT					(u16)531			// Horizontal Total
#define	HPS					(u16)43				// LLINE Pulse Start Position
#define	LPS					(u16)8				// Horizontal Display Period Start Position
#define	HPW					(u16)1				// LLINE Pulse Width

#define	VDP					(u16)271			// Vertical Display Period
#define	VT					(u16)288			// Vertical Total
#define	VPS					(u16)12				// LFRAME Pulse Start Position
#define	FPS					(u16)4				// Vertical Display Period Start Positio
#define	VPW					(u16)10				// LFRAME Pulse Width


/* Private macro -------------------------------------------------------------*/
/** @addtogroup HT32F1656_DVB_EBI_LCD
  * @{
  */
#define LCD_EBI                     (HT_EBI)

#define LCD_EBI_CS_GPIO_ID          (GPIO_PB)
#define LCD_EBI_CS_AFIO_PIN         (AFIO_PIN_7)
#define LCD_EBI_CS_AFIO_MODE        (AFIO_FUN_EBI)

#define LCD_EBI_OE_GPIO_ID          (GPIO_PB)
#define LCD_EBI_OE_AFIO_PIN         (AFIO_PIN_6)
#define LCD_EBI_OE_AFIO_MODE        (AFIO_FUN_EBI)

#define LCD_EBI_WE_GPIO_ID          (GPIO_PB)
#define LCD_EBI_WE_AFIO_PIN         (AFIO_PIN_8)
#define LCD_EBI_WE_AFIO_MODE        (AFIO_FUN_EBI)

#define LCD_EBI_RS_GPIO_ID          (GPIO_PA)
#define LCD_EBI_RS_AFIO_PIN         (AFIO_PIN_9)
#define LCD_EBI_RS_AFIO_MODE        (AFIO_FUN_EBI)

#define LCD_EBI_AD0_GPIO_ID         (GPIO_PA)
#define LCD_EBI_AD0_AFIO_PIN        (AFIO_PIN_14)
#define LCD_EBI_AD0_AFIO_MODE       (AFIO_FUN_EBI)

#define LCD_EBI_AD1_GPIO_ID         (GPIO_PA)
#define LCD_EBI_AD1_AFIO_PIN        (AFIO_PIN_15)
#define LCD_EBI_AD1_AFIO_MODE       (AFIO_FUN_EBI)

#define LCD_EBI_AD2_GPIO_ID         (GPIO_PB)
#define LCD_EBI_AD2_AFIO_PIN        (AFIO_PIN_0)
#define LCD_EBI_AD2_AFIO_MODE       (AFIO_FUN_EBI)

#define LCD_EBI_AD3_GPIO_ID         (GPIO_PB)
#define LCD_EBI_AD3_AFIO_PIN        (AFIO_PIN_1)
#define LCD_EBI_AD3_AFIO_MODE       (AFIO_FUN_EBI)

#define LCD_EBI_AD4_GPIO_ID         (GPIO_PB)
#define LCD_EBI_AD4_AFIO_PIN        (AFIO_PIN_2)
#define LCD_EBI_AD4_AFIO_MODE       (AFIO_FUN_EBI)

#define LCD_EBI_AD5_GPIO_ID         (GPIO_PB)
#define LCD_EBI_AD5_AFIO_PIN        (AFIO_PIN_3)
#define LCD_EBI_AD5_AFIO_MODE       (AFIO_FUN_EBI)

#define LCD_EBI_AD6_GPIO_ID         (GPIO_PB)
#define LCD_EBI_AD6_AFIO_PIN        (AFIO_PIN_4)
#define LCD_EBI_AD6_AFIO_MODE       (AFIO_FUN_EBI)

#define LCD_EBI_AD7_GPIO_ID         (GPIO_PB)
#define LCD_EBI_AD7_AFIO_PIN        (AFIO_PIN_5)
#define LCD_EBI_AD7_AFIO_MODE       (AFIO_FUN_EBI)

#define LCD_EBI_AD8_GPIO_ID         (GPIO_PC)
#define LCD_EBI_AD8_AFIO_PIN        (AFIO_PIN_7)
#define LCD_EBI_AD8_AFIO_MODE       (AFIO_FUN_EBI)

#define LCD_EBI_AD9_GPIO_ID         (GPIO_PC)
#define LCD_EBI_AD9_AFIO_PIN        (AFIO_PIN_8)
#define LCD_EBI_AD9_AFIO_MODE       (AFIO_FUN_EBI)

#define LCD_EBI_AD10_GPIO_ID        (GPIO_PC)
#define LCD_EBI_AD10_AFIO_PIN       (AFIO_PIN_4)
#define LCD_EBI_AD10_AFIO_MODE      (AFIO_FUN_EBI)

#define LCD_EBI_AD11_GPIO_ID        (GPIO_PC)
#define LCD_EBI_AD11_AFIO_PIN       (AFIO_PIN_5)
#define LCD_EBI_AD11_AFIO_MODE      (AFIO_FUN_EBI)

#define LCD_EBI_AD12_GPIO_ID        (GPIO_PC)
#define LCD_EBI_AD12_AFIO_PIN       (AFIO_PIN_6)
#define LCD_EBI_AD12_AFIO_MODE      (AFIO_FUN_EBI)

#define LCD_EBI_AD13_GPIO_ID        (GPIO_PC)
#define LCD_EBI_AD13_AFIO_PIN       (AFIO_PIN_0)
#define LCD_EBI_AD13_AFIO_MODE      (AFIO_FUN_EBI)

#define LCD_EBI_AD14_GPIO_ID        (GPIO_PC)
#define LCD_EBI_AD14_AFIO_PIN       (AFIO_PIN_1)
#define LCD_EBI_AD14_AFIO_MODE      (AFIO_FUN_EBI)

#define LCD_EBI_AD15_GPIO_ID        (GPIO_PC)
#define LCD_EBI_AD15_AFIO_PIN       (AFIO_PIN_2)
#define LCD_EBI_AD15_AFIO_MODE      (AFIO_FUN_EBI)

#define LCD_EBI_RST_GPIO_ID         (GPIO_PA)
#define LCD_EBI_RST_GPIO_PIN        (GPIO_PIN_11)
#define LCD_EBI_RST_AFIO_MODE       (AFIO_MODE_DEFAULT)
#define LCD_EBI_RST_CLK(CK)         (CK.Bit.PA)
/**
  * @}
  */


/* Private variables ---------------------------------------------------------*/
EBI_InitTypeDef  EBI_InitStructure;

static vu16 Color_Text = 0x0000;  // default: Black
static vu16 Color_Back = 0xFFFF;  // default: White


/* Private function prototypes -----------------------------------------------*/
static void delay_ms(u32 uDelay);
void LCD_Clear_test(u32 Color);

/* Private functions ---------------------------------------------------------*/

/***************************************************************************//**
  * @brief  LCM_CMD
  * @retval None
  *****************************************************************************/
void LCM_CMD(u16 cmd)
{
  EBI_LCD->EBI_LCD_REG = cmd << 8;
}

/***************************************************************************//**
  * @brief  LCM_WRITE_DATA
  * @retval None
  *****************************************************************************/
void LCM_WRITE_DATA(u16 data)
{
  EBI_LCD->EBI_LCD_RAM = data;
}

/***************************************************************************//**
  * @brief  LCM_READ_DATA
  * @retval None
  *****************************************************************************/
u16 LCM_READ_DATA(void)
{
  return EBI_LCD->EBI_LCD_RAM;
}

void config_rs_mode(u8 mode)
{
  if(mode)
    AFIO_GPxConfig(LCD_EBI_RS_GPIO_ID, LCD_EBI_RS_AFIO_PIN, LCD_EBI_RS_AFIO_MODE);
  else
    AFIO_GPxConfig(LCD_EBI_RS_GPIO_ID, LCD_EBI_RS_AFIO_PIN, AFIO_FUN_DEFAULT);
}

/***************************************************************************//**
  * @brief  Configures the EBI and GPIOs to interface with the SRAM memory.
  * @retval None
  *****************************************************************************/
void LCD_Init(void)
{
 // EBI_InitTypeDef  EBI_InitStructure;
	CKCU_PeripClockConfig_TypeDef CKCUClock = {{0}};
	CKCUClock.Bit.PA 						= 1;
	CKCUClock.Bit.PB 						= 1;
	CKCUClock.Bit.PC 						= 1;
	CKCUClock.Bit.EBI          = 1;
	CKCUClock.Bit.AFIO         = 1;
	CKCU_PeripClockConfig(CKCUClock, ENABLE);
	
	GPIO_DirectionConfig(HT_GPIOA, GPIO_PIN_11, GPIO_DIR_OUT);		
	GPIO_SetOutBits(HT_GPIOA, GPIO_PIN_11);	
	
	AFIO_GPxConfig(LCD_EBI_CS_GPIO_ID, LCD_EBI_CS_AFIO_PIN, LCD_EBI_CS_AFIO_MODE);
	AFIO_GPxConfig(LCD_EBI_OE_GPIO_ID, LCD_EBI_OE_AFIO_PIN, LCD_EBI_OE_AFIO_MODE);
	AFIO_GPxConfig(LCD_EBI_WE_GPIO_ID, LCD_EBI_WE_AFIO_PIN, LCD_EBI_WE_AFIO_MODE);
	AFIO_GPxConfig(LCD_EBI_RS_GPIO_ID, LCD_EBI_RS_AFIO_PIN, LCD_EBI_RS_AFIO_MODE);
	AFIO_GPxConfig(LCD_EBI_AD0_GPIO_ID, LCD_EBI_AD0_AFIO_PIN, LCD_EBI_AD0_AFIO_MODE);
	AFIO_GPxConfig(LCD_EBI_AD1_GPIO_ID, LCD_EBI_AD1_AFIO_PIN, LCD_EBI_AD1_AFIO_MODE);
	AFIO_GPxConfig(LCD_EBI_AD2_GPIO_ID, LCD_EBI_AD2_AFIO_PIN, LCD_EBI_AD2_AFIO_MODE);
	AFIO_GPxConfig(LCD_EBI_AD3_GPIO_ID, LCD_EBI_AD3_AFIO_PIN, LCD_EBI_AD3_AFIO_MODE);
	AFIO_GPxConfig(LCD_EBI_AD4_GPIO_ID, LCD_EBI_AD4_AFIO_PIN, LCD_EBI_AD4_AFIO_MODE);
	AFIO_GPxConfig(LCD_EBI_AD5_GPIO_ID, LCD_EBI_AD5_AFIO_PIN, LCD_EBI_AD5_AFIO_MODE);
	AFIO_GPxConfig(LCD_EBI_AD6_GPIO_ID, LCD_EBI_AD6_AFIO_PIN, LCD_EBI_AD6_AFIO_MODE);
	AFIO_GPxConfig(LCD_EBI_AD7_GPIO_ID, LCD_EBI_AD7_AFIO_PIN, LCD_EBI_AD7_AFIO_MODE);
	AFIO_GPxConfig(LCD_EBI_AD8_GPIO_ID, LCD_EBI_AD8_AFIO_PIN, LCD_EBI_AD8_AFIO_MODE);
	AFIO_GPxConfig(LCD_EBI_AD9_GPIO_ID, LCD_EBI_AD9_AFIO_PIN, LCD_EBI_AD9_AFIO_MODE);
	AFIO_GPxConfig(LCD_EBI_AD10_GPIO_ID, LCD_EBI_AD10_AFIO_PIN, LCD_EBI_AD10_AFIO_MODE);
	AFIO_GPxConfig(LCD_EBI_AD11_GPIO_ID, LCD_EBI_AD11_AFIO_PIN, LCD_EBI_AD11_AFIO_MODE);
	AFIO_GPxConfig(LCD_EBI_AD12_GPIO_ID, LCD_EBI_AD12_AFIO_PIN, LCD_EBI_AD12_AFIO_MODE);
	AFIO_GPxConfig(LCD_EBI_AD13_GPIO_ID, LCD_EBI_AD13_AFIO_PIN, LCD_EBI_AD13_AFIO_MODE);
	AFIO_GPxConfig(LCD_EBI_AD14_GPIO_ID, LCD_EBI_AD14_AFIO_PIN, LCD_EBI_AD14_AFIO_MODE);
	AFIO_GPxConfig(LCD_EBI_AD15_GPIO_ID, LCD_EBI_AD15_AFIO_PIN, LCD_EBI_AD15_AFIO_MODE);

	/*  EBI Configuration                                                                                     */
	EBI_InitStructure.EBI_Bank = EBI_BANK_0;
	EBI_InitStructure.EBI_Mode = EBI_MODE_D16;
	EBI_InitStructure.EBI_IdleCycle = EBI_IDLECYCLE_DISABLE;
	EBI_InitStructure.EBI_ChipSelectPolarity = EBI_CHIPSELECTPOLARITY_LOW;
	EBI_InitStructure.EBI_AddressLatchPolarity = EBI_ADDRESSLATCHPOLARITY_LOW;
	EBI_InitStructure.EBI_WriteEnablePolarity = EBI_WRITEENABLEPOLARITY_LOW;
	EBI_InitStructure.EBI_ReadEnablePolarity = EBI_READENABLEPOLARITY_LOW;
	EBI_InitStructure.EBI_AddressSetupTime = 2;
	EBI_InitStructure.EBI_AddressHoldTime = 5;    /* For low speed access */
	EBI_InitStructure.EBI_WriteSetupTime = 8;     /* For low speed access */
	EBI_InitStructure.EBI_WriteStrobeTime = 10;    /* For low speed access */
	EBI_InitStructure.EBI_WriteHoldTime = 5;      /* For low speed access */
	EBI_InitStructure.EBI_ReadSetupTime = 5;
	EBI_InitStructure.EBI_ReadStrobeTime = 10;
	EBI_InitStructure.EBI_ReadHoldTime = 2;
	EBI_InitStructure.EBI_PageMode = EBI_PAGEMODE_ENABLE;//EBI_PAGEMODE_DISABLE;
	EBI_InitStructure.EBI_PageLength = EBI_PAGELENGTH_32;//EBI_PAGELENGTH_4;
	EBI_InitStructure.EBI_PageHitMode = EBI_PAGEHITMODE_INTRPAGE;//EBI_PAGEHITMODE_ADDINC;
	EBI_InitStructure.EBI_PageAccessTime = 0xF;//0xF;
	EBI_InitStructure.EBI_PageOpenTime = 0xFF;
	EBI_Init(&EBI_InitStructure);
	EBI_Cmd(EBI_BANK_0, ENABLE);
  
  ZH_gGPTM.PinLv = 500;
	ZH_gGPTM.JingQueDu = 100;
	ZH_gGPTM.XiTongPinLv = 96000000;
  ZH_GPTM_Init();
  
  ZH_GPTM_CCR_Num(90);
  ZH_GPTM_Switch(ENABLE);
  ZH_GPTMCH_Switch(ENABLE);
  delay_ms(100);
}


/***************************************************************************//**
  * @brief  Configures the EBI and GPIOs to interface with the SRAM memory.
  * @retval None
  *****************************************************************************/
void LCD_Config(void)
{
//	u8 i;
	u8 pll_lock=0;
	u16 temp;
  #if 1
	//reset
	GPIO_ClearOutBits(HT_GPIOA, GPIO_PIN_11);
	delay_ms(100);
	GPIO_SetOutBits(HT_GPIOA, GPIO_PIN_11);
  delay_ms(100);
	
  
  EBI_LCD->EBI_LCD_REG = 0x01;  // software reset
  //delay(100);
	delay_ms(20);
  
	EBI_LCD->EBI_LCD_REG = 0xE2;
//	EBI_LCD->EBI_LCD_RAM = 0x42;
//  EBI_LCD->EBI_LCD_RAM = 0x0A;
	EBI_LCD->EBI_LCD_RAM = 0x0027;
  EBI_LCD->EBI_LCD_RAM = 0x0004;
  EBI_LCD->EBI_LCD_RAM = 0x00054;			//PLL = 40MHZ
  
  EBI_LCD->EBI_LCD_REG = 0xE0;  // PLL enable
  EBI_LCD->EBI_LCD_RAM = 0x01;
  delay_ms(20);

	while(pll_lock == 0)
	{
		EBI_LCD->EBI_LCD_REG = 0xE4;
		temp = EBI_LCD->EBI_LCD_RAM;
		if((temp&0x04) == 0x04)	pll_lock = 1;
	}

	// set pll
  EBI_LCD->EBI_LCD_REG = 0xE0;	// Lock PLL
  EBI_LCD->EBI_LCD_RAM = 0x03;
  delay_ms(20);
	
	EBI_LCD->EBI_LCD_REG = 0x01;
	delay_ms(20);
	
  /* 4 clock write 1 pixel (16bits) */
	EBI_InitStructure.EBI_AddressHoldTime = 0x2;
  EBI_InitStructure.EBI_WriteSetupTime = 2;
  EBI_InitStructure.EBI_WriteStrobeTime = 4;
  EBI_InitStructure.EBI_WriteHoldTime = 1;
  EBI_Init(&EBI_InitStructure);


  EBI_LCD->EBI_LCD_REG = 0x00E6;
  EBI_LCD->EBI_LCD_RAM = 0x0001;
  EBI_LCD->EBI_LCD_RAM = 0x00F9;
  EBI_LCD->EBI_LCD_RAM = 0x0099;			//CLK=40M-->8MHz
	
	EBI_LCD->EBI_LCD_REG = 0x00B0;
  EBI_LCD->EBI_LCD_RAM = 0x0008;//0x20;	//24bit TFT H&V active high
  EBI_LCD->EBI_LCD_RAM = 0x0000;	//TFT mode
  EBI_LCD->EBI_LCD_RAM = (HDP>>8)&0x00FF;
  EBI_LCD->EBI_LCD_RAM = HDP&0x00FF;
  EBI_LCD->EBI_LCD_RAM = (VDP>>8)&0x00FF;
  EBI_LCD->EBI_LCD_RAM = VDP&0x00FF;
  EBI_LCD->EBI_LCD_RAM = 0x00;	//RGB
	
  EBI_LCD->EBI_LCD_REG = 0x00B4;
  EBI_LCD->EBI_LCD_RAM = (HT>>8)&0x00FF;
  EBI_LCD->EBI_LCD_RAM = HT&0x00FF;
  EBI_LCD->EBI_LCD_RAM = (HPS>>8)&0x00FF;
  EBI_LCD->EBI_LCD_RAM = HPS&0x00FF;
  EBI_LCD->EBI_LCD_RAM = HPW;
  EBI_LCD->EBI_LCD_RAM = (LPS>>8)&0x00FF;
  EBI_LCD->EBI_LCD_RAM = LPS&0x00FF;
  EBI_LCD->EBI_LCD_RAM = 0x00;

  EBI_LCD->EBI_LCD_REG = 0x00B6;	//VSYNC
  EBI_LCD->EBI_LCD_RAM = (VT>>8)&0x00FF;   // ... Set VT
  EBI_LCD->EBI_LCD_RAM = VT&0x00FF;
  EBI_LCD->EBI_LCD_RAM = (VPS>>8)&0x00FF;  // ... Set VPS
  EBI_LCD->EBI_LCD_RAM = VPS&0x00FF;
  EBI_LCD->EBI_LCD_RAM = VPW;			   // ...Set VPW
  EBI_LCD->EBI_LCD_RAM = (FPS>>8)&0x00FF;  // ... Set FPS
  EBI_LCD->EBI_LCD_RAM = FPS&0x00FF;
	
//STB on --then-> GRB on

  EBI_LCD->EBI_LCD_REG = 0x00B8;
  EBI_LCD->EBI_LCD_RAM = 0x000F;    //GPIO[3:0]=output
  EBI_LCD->EBI_LCD_RAM = 0x0001;    //GPIO0 normal
  
  EBI_LCD->EBI_LCD_REG = 0x00BA;
  EBI_LCD->EBI_LCD_RAM = 0x0000;    //GPIO[3:0] out 1
  //delay(10);
	delay_ms(10);

  EBI_LCD->EBI_LCD_REG = 0x00BA;
  EBI_LCD->EBI_LCD_RAM = 0x0001;    //GPIO[3:0] out 1
  //delay(10);
	delay_ms(10);

  EBI_LCD->EBI_LCD_REG = 0x00D0;//set DBC config 
  EBI_LCD->EBI_LCD_RAM = 0x000D; 
  	
  EBI_LCD->EBI_LCD_REG = 0x00F0; //pixel data interface
//	EBI_LCD->EBI_LCD_RAM = 0x0000;  // 8-bit packed
EBI_LCD->EBI_LCD_RAM = 0x0003;  // 16-bit (565 format)  
 // 	EBI_LCD->EBI_LCD_RAM = 0x0002;  // 16-bit packed
	
	
	
//  EBI_LCD->EBI_LCD_REG = 0x00F1; //pixel data interface	
//  EBI_LCD->EBI_LCD_RAM = 0x0003;  // 16-bit (565 format)  
//	//EBI_LCD->EBI_LCD_RAM = 0x0002;  // 16-bit packed

	delay_ms(5);
  
  
  EBI_LCD->EBI_LCD_REG = 0x0026; //display on
  EBI_LCD->EBI_LCD_RAM = 0x0001;
  	
  EBI_LCD->EBI_LCD_REG = 0x0029; //display on   			
  //delay(20);
	delay_ms(20);

  /* Show Direction */
  //LCD_WriteReg(0x16,0x20);
	
	EBI_LCD->EBI_LCD_REG = 0x0036;	 //display mirror & turn 180 degree
	EBI_LCD->EBI_LCD_RAM = 0x0000;
//	EBI_LCD->EBI_LCD_RAM = 0x0008;//BGR
	
//	EBI_LCD->EBI_LCD_RAM = 0x0060;
//  EBI_LCD->EBI_LCD_RAM = 0x0083;
//	LCD_Clear(Black);

	

//  while(1)
//  {
//    temp = LCD_GetScanLine();
//    temp = LCD_GetScanLine();
//    temp = LCD_GetScanLine();
//    temp = LCD_GetScanLine();
//  }
	//BL
//  LCD_BL_DutySet(200);
  
//  ZH_GPTM_CCR_Num(90);
//  ZH_GPTM_Switch(ENABLE);
//  ZH_GPTMCH_Switch(ENABLE);
//  delay_ms(100);

#else
	delay_ms(20);

  /* BL */
	//GPIO_SetOutBits(HT_GPIOA, GPIO_PIN_0);;
	
	// reset LCD
  LCM_RST_ACTIVE();
  delay_ms(20);
  LCM_RST_INACTIVE();
  delay_ms(30);
	
  
  EBI_LCD->EBI_LCD_REG = 0x01;  // Soft reset
  delay_ms(20);

  EBI_LCD->EBI_LCD_REG = 0xE0;  // Enable PLL
  EBI_LCD->EBI_LCD_RAM = 0x01;
  delay_ms(10);

	// set pll
  EBI_LCD->EBI_LCD_REG = 0xE0;	// Lock PLL
  EBI_LCD->EBI_LCD_RAM = 0x03;
  delay_ms(20);

  // 4 clock write 1 pixel (16bits)
//  EBI_InitStructure.EBI_WriteSetupTime = 1;
//  EBI_InitStructure.EBI_WriteStrobeTime = 2;
//  EBI_InitStructure.EBI_WriteHoldTime = 1;
//  EBI_Init(&EBI_InitStructure);

	// set pll mn
	EBI_LCD->EBI_LCD_REG = 0x00E2;
	EBI_LCD->EBI_LCD_RAM = 0x0018;
  EBI_LCD->EBI_LCD_RAM = 0x0001;
  EBI_LCD->EBI_LCD_RAM = 0x0004;								//PLL = 120MHZ

	// set lshift freq
  EBI_LCD->EBI_LCD_REG = 0x00E6;
  EBI_LCD->EBI_LCD_RAM = 0x0004;
  EBI_LCD->EBI_LCD_RAM = 0x0070;
  EBI_LCD->EBI_LCD_RAM = 0x00A3;								//CLK=33.3MHz
	
	// set lcd mode
	EBI_LCD->EBI_LCD_REG = 0xB0;							
  EBI_LCD->EBI_LCD_RAM = 0x20;									// 24bit TFT H&V active high
  EBI_LCD->EBI_LCD_RAM = 0x00;									// TFT mode
  EBI_LCD->EBI_LCD_RAM = (799 >> 8) & 0x00FF;
  EBI_LCD->EBI_LCD_RAM = 799 & 0x00FF;
  EBI_LCD->EBI_LCD_RAM = (479 >> 8) & 0x00FF;
  EBI_LCD->EBI_LCD_RAM = 479 & 0x00FF;
  EBI_LCD->EBI_LCD_RAM = 0x00;									//RGB
	
	// set hori period
  EBI_LCD->EBI_LCD_REG = 0x00B4;
  EBI_LCD->EBI_LCD_RAM = (1055 >> 8) & 0x00FF;
  EBI_LCD->EBI_LCD_RAM = 1055 & 0x00FF;
  EBI_LCD->EBI_LCD_RAM = (45 >> 8) & 0x00FF;
  EBI_LCD->EBI_LCD_RAM = 45&0x00FF;
  EBI_LCD->EBI_LCD_RAM = 10;
  EBI_LCD->EBI_LCD_RAM = (210 >> 8) & 0x00FF;
  EBI_LCD->EBI_LCD_RAM = 210 & 0x00FF;
  EBI_LCD->EBI_LCD_RAM = 0x00;

	// set vert period
  EBI_LCD->EBI_LCD_REG = 0x00B6;								// VSYNC
  EBI_LCD->EBI_LCD_RAM = (524 >> 8) & 0x00FF;   	// ... Set VT
  EBI_LCD->EBI_LCD_RAM = 524 & 0x00FF;
  EBI_LCD->EBI_LCD_RAM = (23 >> 8) & 0x00FF;  		// ... Set VPS
  EBI_LCD->EBI_LCD_RAM = 23 & 0x00FF;
  EBI_LCD->EBI_LCD_RAM = 10;			   						// ...Set VPW
  EBI_LCD->EBI_LCD_RAM = (22 >> 8) & 0x00FF;  			// ... Set FPS
  EBI_LCD->EBI_LCD_RAM = 22&0x00FF;
	
	//STB on --then-> GRB on
	
	// set gpio conf
  EBI_LCD->EBI_LCD_REG = 0x00B8;
  EBI_LCD->EBI_LCD_RAM = 0x000F;    						// GPIO[3:0]=output
  EBI_LCD->EBI_LCD_RAM = 0x0001;    						// GPIO0 normal
  
	// set gpio value
  EBI_LCD->EBI_LCD_REG = 0x00BA;
  EBI_LCD->EBI_LCD_RAM = 0x0002;    // GPIO[3:0] out 1
  delay_ms(20);

  EBI_LCD->EBI_LCD_REG = 0x00BA;
  EBI_LCD->EBI_LCD_RAM = 0x0000;    // GPIO[3:0] out 1
  delay_ms(20);

  EBI_LCD->EBI_LCD_REG = 0x00BA;
  EBI_LCD->EBI_LCD_RAM = 0x0002;    // GPIO[3:0] out 1
	 
	EBI_LCD->EBI_LCD_REG = 0x00BA;
  EBI_LCD->EBI_LCD_RAM = 0x0003;    // GPIO[3:0] out 1

	// set pwm conf
  EBI_LCD->EBI_LCD_REG = 0x00BE; 		// set PWM for B/L
  EBI_LCD->EBI_LCD_RAM = 0x000E;
  EBI_LCD->EBI_LCD_RAM = 0x00FF;   	
  EBI_LCD->EBI_LCD_RAM = 0x0009;
  EBI_LCD->EBI_LCD_RAM = 0x00FF;
  EBI_LCD->EBI_LCD_RAM = 0x0000;
  EBI_LCD->EBI_LCD_RAM = 0x0000;
  
	// set dbc conf
  EBI_LCD->EBI_LCD_REG = 0x00D0;		// set DBC config 
  EBI_LCD->EBI_LCD_RAM = 0x000D; 
  	
	// set pixel data interface
  EBI_LCD->EBI_LCD_REG = 0x00F0; 		// pixel data interface
  EBI_LCD->EBI_LCD_RAM = 0x0003;  	// 16-bit (565 format)  
  delay_ms(10);
  
  // set gamma curve
  EBI_LCD->EBI_LCD_REG = 0x0026; 		// display on
  EBI_LCD->EBI_LCD_RAM = 0x0001;
  	
	// set display on
  EBI_LCD->EBI_LCD_REG = 0x0029; 		// display on   			
  delay_ms(40);

  // Show Direction
  //LCD_WriteReg(0x16,0x20);
	
	// set address mode
	EBI_LCD->EBI_LCD_REG = 0x36;	 //display mirror & turn 180 degree
  EBI_LCD->EBI_LCD_RAM = 0x80;

  //EBI_LCD_Clear(EBI_LCD_Black);
	LCD_Clear(Black);
	

#endif

//#define LCM_TEST_MODE

 #ifdef LCM_TEST_MODE 

	//test cloor
		delay_ms(2000);
		LCD_Clear(Red);
		delay_ms(2000);
		LCD_Clear(Green);
		delay_ms(2000);
		LCD_Clear(Blue);  
		
			
	LCD_Clear(White);
//		//test BL
//   for(i=0;i<90;i++) 
//   {
//    ZH_GPTM_CCR_Num(i);
//	 }
	LCD_Clear(Black);
#endif

	LCD_Clear(Red);
}



/***************************************************************************//**
  * @brief  
  * @retval None
  *****************************************************************************/
void LCD_DeInit(void)
{
	/*!< LCD Display Off */
	
	
}

/*********************************************************************************************************//**
  * @brief  Write value to the selected LCD register.
  * @param  LCD_Reg_Index: address of the selected register.
  * @param  LCD_Reg_Value: value of to write the selected register.
  * @retval None
  ***********************************************************************************************************/
void LCD_WriteReg(u8 LCD_Reg_Index, u16 LCD_Reg_Value)
{
  EBI_LCD->EBI_LCD_REG = LCD_Reg_Index;
  EBI_LCD->EBI_LCD_RAM = LCD_Reg_Value;
}

/*********************************************************************************************************//**
  * @brief  Prepare to write to the LCD RAM.
  * @param  None
  * @retval None
  ***********************************************************************************************************/
void LCD_WriteRAMPrior(void)
{
  EBI_LCD->EBI_LCD_REG = 0x2C;
}

void LCD_WriteRAMContinue(void)
{
  EBI_LCD->EBI_LCD_REG = 0x3C;
}
void LCD_ReadRAMPrior(void)
{
  EBI_LCD->EBI_LCD_REG = 0x2E;
  EBI_LCD->EBI_LCD_RAM;
}
void LCD_ReadRAMContinue(void)
{
  EBI_LCD->EBI_LCD_REG = 0x3E;
  EBI_LCD->EBI_LCD_RAM;
}


/*********************************************************************************************************//**
  * @brief  Writes to the LCD RAM.
  * @param  RGB_Code: the pixel color in RGB mode (5-6-5).
  * @retval None
  ***********************************************************************************************************/
void LCD_WriteRAM(u16 RGB_Set)
{
#if 1
  EBI_LCD->EBI_LCD_RAM = RGB_Set;
#else
  EBI_LCD->EBI_LCD_RAM = RGB_Set >> 8;
  EBI_LCD->EBI_LCD_RAM = RGB_Set;
#endif
}

/*********************************************************************************************************//**
  * @brief  Set the cursor position.
  * @param  X_Location: specify the X position.
  * @param  Y_Location: specify the Y position.
  * @retval None
  ***********************************************************************************************************/
void LCD_StarterSet(u16 X_Location, u16 Y_Location)
{
#if 1
  EBI_LCD->EBI_LCD_REG = 0x2A;
  EBI_LCD->EBI_LCD_RAM = X_Location >> 8;
  EBI_LCD->EBI_LCD_RAM = X_Location & 0xff;
  EBI_LCD->EBI_LCD_RAM = HDP >> 8;
  EBI_LCD->EBI_LCD_RAM = HDP & 0xff;

  EBI_LCD->EBI_LCD_REG = 0x2B;
  EBI_LCD->EBI_LCD_RAM = Y_Location >> 8;
  EBI_LCD->EBI_LCD_RAM = Y_Location & 0xff;
  EBI_LCD->EBI_LCD_RAM = VDP >> 8;
  EBI_LCD->EBI_LCD_RAM = VDP & 0xff;

#else
  //LCD_WriteReg(REG6,((Xpos>>8) & 0xff));
  EBI_LCD->EBI_LCD_REG = REG6;
  EBI_LCD->EBI_LCD_RAM  = (X_Location>>8) & 0xff;
  //LCD_WriteReg(REG7,(Xpos & 0xff));       //Column Start
  EBI_LCD->EBI_LCD_REG = REG7;
  EBI_LCD->EBI_LCD_RAM  = X_Location & 0xff;

  //LCD_WriteReg(REG2,((Ypos>>8) & 0xff));
  //LCD_WriteReg(REG3,(Ypos & 0xff));       //Row Start
  EBI_LCD->EBI_LCD_REG = REG2;
  EBI_LCD->EBI_LCD_RAM  = (Y_Location>>8) & 0xff;
  EBI_LCD->EBI_LCD_REG = REG3;
  EBI_LCD->EBI_LCD_RAM  = Y_Location & 0xff;
#endif
}


void LCD_XY_LocationSet(u16 X_Sta, u16 X_End,u16 Y_Sta,u16 Y_End)
{
  EBI_LCD->EBI_LCD_REG = 0x2A;
  EBI_LCD->EBI_LCD_RAM = X_Sta >> 8;
  EBI_LCD->EBI_LCD_RAM = X_Sta & 0xff;
  EBI_LCD->EBI_LCD_RAM = X_End >> 8;
  EBI_LCD->EBI_LCD_RAM = X_End & 0xff;

  EBI_LCD->EBI_LCD_REG = 0x2B;
  EBI_LCD->EBI_LCD_RAM = Y_Sta >> 8;
  EBI_LCD->EBI_LCD_RAM = Y_Sta & 0xff;
  EBI_LCD->EBI_LCD_RAM = Y_End >> 8;
  EBI_LCD->EBI_LCD_RAM = Y_End & 0xff;


}
/*********************************************************************************************************//**
  * @brief  LCD_SetDisplayArea.
  * @param  
  * @retval None
  ***********************************************************************************************************/
void LCD_SetDisplayArea(u16 Column, u16 Page, u16 Height, u16 Width)
{
  EBI_LCD->EBI_LCD_REG = 0x2A;
  EBI_LCD->EBI_LCD_RAM = Column >> 8;
  EBI_LCD->EBI_LCD_RAM = Column & 0xff;
  EBI_LCD->EBI_LCD_RAM = (Column+Width-1) >> 8;
  EBI_LCD->EBI_LCD_RAM = (Column+Width-1) & 0xff;

  EBI_LCD->EBI_LCD_REG = 0x2B;
  EBI_LCD->EBI_LCD_RAM = Page >> 8;
  EBI_LCD->EBI_LCD_RAM = Page & 0xff;
  EBI_LCD->EBI_LCD_RAM = (Page+Height-1) >> 8;
  EBI_LCD->EBI_LCD_RAM = (Page+Height-1) & 0xff;
}

/*********************************************************************************************************//**
  * @brief  LCD_DrawCheckerPattern.
  * @param  
  * @retval None
  ***********************************************************************************************************/
void LCD_DrawCheckerPattern(short blue, short green, short red)
{
	unsigned int lineWidth = HDP + 1,height = VDP + 1, w;
	unsigned short rgb565_1  =  0;
//	unsigned short rgb565_2  =  0;
//	int check_x = 10;
//	int check_y = 8;

	// Convert the R,G,B values into RGB666
	rgb565_1  =  red<<11;
	rgb565_1 |=  green<<5;
	rgb565_1 |=  blue;

//	rgb565_2 = !rgb565_1;

	LCD_SetDisplayArea(0, 0, VDP, HDP);

	LCD_WriteRAMPrior();
	
	while(lineWidth--)
	{
	    for(w  =  0;w < height;w++)
		{    
        LCD_WriteRAM(rgb565_1);
		}
	}
}

/*********************************************************************************************************//**
  * @brief  Clear the whole LCD.
  * @param  Color: the color to be the background.
  * @retval None
  ***********************************************************************************************************/
void LCD_Clear(u16 Color)
{
	unsigned int Height,Width;
	
	Height=VDP+1;
  
  LCD_StarterSet(0, 0);
  
  /* Prepare to write GRAM */
  LCD_WriteRAMPrior();
	
	while(Height--)		  /* VDP行 */
	{
		for(Width = 0; Width <= HDP+1; Width++)  /* 每行HDP个像素点 */
		{ 	
			//f = 50;
			 EBI_LCD->EBI_LCD_RAM = Color ;
			
		}
	}
}


void LCD_WriteRamRGB(u8 R,u8 G,u8 B)//24bit RGB888
{

	EBI_LCD->EBI_LCD_RAM = R<<8|G ;
	EBI_LCD->EBI_LCD_RAM = B<<8|R ;
	EBI_LCD->EBI_LCD_RAM = G<<8|B ;

}

void LCD_WriteRamColor(u32 Color)//24bit RGB888
{

	EBI_LCD->EBI_LCD_RAM = Color>>8 ;
	
	EBI_LCD->EBI_LCD_RAM = ((Color&0xFF)<<8)|(Color>>16);
	
	EBI_LCD->EBI_LCD_RAM = Color&0xFFFF ;

}

/*********************************************************************************************************//**
  * @brief  Clear the whole LCD.
  * @param  Color: the color to be the background.
  * @retval None
  ***********************************************************************************************************/
void LCD_Clear_test(u32 Color)//24bit RGB888
{
	unsigned int Height,Width;
	u8 R, G, B;
	
	Height=VDP+1;
  
  LCD_StarterSet(0, 0);
  R = (u8)(Color>>16);
	G = (u8)((Color>>8)&0xFF);
	B = (u8)(Color&0xFF);
  /* Prepare to write GRAM */
  LCD_WriteRAMPrior();
	
	while(Height--)		  /* VDP行 */
	{
		for(Width = 0; Width <= HDP+1; Width++)  /* 每行HDP个像素点 */
		{ 	
			
					EBI_LCD->EBI_LCD_RAM = R<<8|G ;
					EBI_LCD->EBI_LCD_RAM = B<<8|R ;
					EBI_LCD->EBI_LCD_RAM = G<<8|B ;
		}
	}

}

void TFT_Write_Color(u16 Height,uc8 *Pptr)//24bit RGB888
{
	u16 Width;
	u32 ImgAdds = 0;
	//Height=VDP+1;
  LCD_StarterSet(0, Height);

  /* Prepare to write GRAM */
  LCD_WriteRAMPrior();
	
	while(Height--)		  /* VDP行 */
	{
		for(Width = 0; Width <= HDP+1; Width++)  /* 每行HDP个像素点 */
		{ 	
			
			
			LCD_WriteRamRGB(Pptr[ImgAdds],Pptr[ImgAdds+1],Pptr[ImgAdds+2]);
			ImgAdds +=3;		
		//	LCD_WriteRamColor(Color);
		
		}
	}

}

//在指定区域内填充指定颜色
//区域大小:
//  (xend-xsta)*(yend-ysta)
void TFT_Fill(unsigned int xsta,unsigned int ysta,unsigned int xend,unsigned int yend,unsigned int color)
{                    
    unsigned long n;

	n=(unsigned long)(yend-ysta+1)*(xend-xsta+1);

	LCD_XY_LocationSet(xsta,xend,ysta,yend);
	
	LCD_WriteRAMPrior();
  

	while(n--)
	{
		EBI_LCD->EBI_LCD_RAM = color ;
	}

} 
//画点
//x:0~239
//y:0~319
//POINT_COLOR:此点的颜色
void TFT_DrawPoint(unsigned int x,unsigned int y,unsigned int fontcolor)
{
	LCD_StarterSet(x,y);
	LCD_WriteRAMPrior();
	EBI_LCD->EBI_LCD_RAM = fontcolor ;

} 

//在指定位置显示一个字符
//x:0~234
//y:0~308
//num:要显示的字符:" "--->"~"
//size:字体大小 12/16
//mode:叠加方式(1)还是非叠加方式(0)

#define MAX_CHAR_POSX 472
#define MAX_CHAR_POSY 264 
void TFT_ShowChar(u16 x,u16 y,u8 num,u8 font_size,u8 mode)
{       
    unsigned int temp;
    unsigned int pos,t;    	
	
    if(x>MAX_CHAR_POSX||y>MAX_CHAR_POSY)
			return;
   //设定一个字符所占的大小 
    //开辟空间
		LCD_SetDisplayArea(x,y,font_size,font_size/2);
	
		LCD_WriteRAMPrior();

		num=num-32;//得到偏移后的值

		for(pos=0;pos<font_size;pos++)
		{

			switch(font_size)
			{
				case 12 : temp=ASCII_1206[num][pos];break;
				case 16	: temp=ASCII_1608[num][pos];break;
				//case 24 : temp=ASCII_2416[num][pos];break;
				default	: temp=ASCII_1206[num][pos];break;
			}
			
			for(t=0;t<font_size/2;t++)
			{                 
					if(temp&0x01)	 //叠加方式
						EBI_LCD->EBI_LCD_RAM = Color_Text ;
					else 
					{
						if(!mode) //非叠加方式
							EBI_LCD->EBI_LCD_RAM = Color_Back ;
					}
					temp>>=1; 
			}
			
		}
			
} 

u16 TFT_ShowString(u16 x,u16 y,char* Str,u8 font_size,u8 mode)
{
	u16 Line = 1;
//	u8 flag = 0;
	u16 x_buf;
	x_buf = x;
//	while((*Str!='\0')&&(*Str!='\r'))
	while(*Str!='\0')
	{       
			if(x > MAX_CHAR_POSX)
			{
				x = x_buf;
				y += font_size;
				Line++;
			}
			
			if(y > MAX_CHAR_POSY)
			{
				x = x_buf,
			  y = 0;
		  	//LCD_Clear(White);
				TFT_Fill(x_buf,0,HDP,VDP,White);
				Line = 0;
			}
			
			
			if(*Str == '\r')
			{
				Str++;
				if(*Str =='\n')
				{
					Str++;
					x=x_buf;
					y+=font_size;
					Line++;
				}
			}
			
			
				if(*Str =='\0')
				{
					Line--;
					break;
				}

				TFT_ShowChar(x,y,*Str,font_size,mode);
				x+=font_size/2;
				Str++;
		
	}  

	return Line*font_size;
}

void TFT_ShowNum(u16 x,u16 y,u8 num,u8 font_size,u8 mode)
{         							   
    TFT_ShowChar(x,y,(num/10)+'0',font_size,mode); 
    TFT_ShowChar(x+font_size/2,y,num%10+'0',font_size,mode); 
} 


//显示4个数字
//x,y:起点坐标
//size:字体大小
//mode:模式	0,填充模式;1,叠加模式
//num:数值(0~9999);
void TFT_Show4Num(u16 x,u16 y,u16 num,u8 font_size,u8 mode)
{   
	TFT_ShowChar(x,y,(num/1000)+'0',font_size,mode);
	TFT_ShowChar(x+font_size/2,y,(num/100)%10+'0',font_size,mode);      							   
  TFT_ShowChar(x+font_size,y,(num/10)%10+'0',font_size,mode); 
  TFT_ShowChar(x+font_size+font_size/2,y,num%10+'0',font_size,mode); 
}

//30*60大小的字体 0~9 :
void TFT_ShowBigChar(u16 x,u16 y,u8 num,u8 mode)
{
   unsigned int n,t;
	unsigned int temp;
	unsigned int t1,deadline;
	
	if(x>MAX_CHAR_POSX||y>MAX_CHAR_POSY)
	return;
	
	
	LCD_SetDisplayArea(x,y,60,30);
	
	
	LCD_WriteRAMPrior();

	if(num==':')t1=150;
	else if(num=='.')t1=165;
	else if(num=='C')t1=180;
	else t1=15*num;
	
	deadline=t1+15;
		   
	for(;t1<deadline;t1++)
	{	 
		for(n=0;n<16;n++)
		{
			temp=ASCII_3060[t1][n];
			for(t=0;t<8;t++)
			{
				if(temp&0x80) 
				EBI_LCD->EBI_LCD_RAM = Color_Text ;
				else 
				{
					if(!mode)
						EBI_LCD->EBI_LCD_RAM = Color_Back ;
				}
				temp<<=1;
				if(((n%4)==3)&&t==5)break;
			}
		}
	} 
	

}



void TFT_ShowTemp(u16 x,u16 y,u16 num,u8 mode)
{
		u8 size = 30;
		TFT_ShowBigChar(x+size,y,(num/100),mode);    
		TFT_ShowBigChar(x+2*size,y,(num/10)%10,mode);      							   
    TFT_ShowBigChar(x+3*size,y,'.',mode); 
    TFT_ShowBigChar(x+4*size,y,num%10,mode); 
		TFT_ShowBigChar(x+5*size,y,'C',mode); 
}

void TFT_DrawPicture(u16 X_Location,u16 Y_Location,u16 Height ,u16 Width ,uc8 *Pptr)
{
  u16 ImgAdds = 0;
  u16 i = 0, j = 0, color = 0;
	
//	if(X_Location>MAX_CHAR_POSX||Y_Location>MAX_CHAR_POSY)
//	return;
	
	LCD_SetDisplayArea(X_Location,Y_Location,Height,Width);
	
	LCD_WriteRAMPrior(); // Get ready to write GRAM

  for (i = 0; i < Height; i++)
  {
    for (j = 0; j < Width; j++)
    {
      ImgAdds = ((Height-1-i) * Width * 2) + (j * 2);

      color = Pptr[ImgAdds] << 8 | (Pptr[ImgAdds + 1]);
			//to pic back color 
			
			if(color == Black) color = Red;
				
      EBI_LCD->EBI_LCD_RAM = color;
    }
	}
	
}


//void TFT_DrawPicture(u16 X,u16 Y,u16 Height ,u16 Width ,uc8 *Pptr)
//{
//  u32 ImgAdds = 0;
//  u16 i = 0, j = 0;

////	if(X_Location>MAX_CHAR_POSX||Y_Location>MAX_CHAR_POSY)
////	return;
//	
//	LCD_SetDisplayArea(X,Y,Height,Width);
//	LCD_WriteRAMPrior(); // Get ready to write GRAM
//	

//  for (i = 0; i < Height; i++)
//  {
//    for (j = 0; j < Width; j++)
//    {
//			ImgAdds = (i * Width * 6) + (j * 6);

//			EBI_LCD->EBI_LCD_RAM = Pptr[ImgAdds+2]<<8|Pptr[ImgAdds+1];
//			EBI_LCD->EBI_LCD_RAM = Pptr[ImgAdds]<<8|Pptr[ImgAdds+5] ;
//			EBI_LCD->EBI_LCD_RAM = Pptr[ImgAdds+4]<<8|Pptr[ImgAdds+3] ;
//    }
//	}
//	
//}

void TFT_DrawPicDMA(u16 X,u16 Y,u16 Height,u16 Width ,u8 ptr_num)
{
	u32 picsize;
	u32 pic_addr;
//	if(X_Location>MAX_CHAR_POSX||Y_Location>MAX_CHAR_POSY)
//	return;
	pic_addr = 0;
	LCD_SetDisplayArea(X,Y,Height,Width);
	
	
	EBI_LCD->EBI_LCD_REG = 0x00F0; //pixel data interface
	EBI_LCD->EBI_LCD_RAM = 0x0000;  // 8-bit packed
	
	LCD_WriteRAMPrior(); // Get ready to write GRAM
  picsize = (Height) * (Width) * 3;//byte

	SPI_FLASH_ReadDMA(pic_addr,(u32)&EBI_LCD->EBI_LCD_RAM ,picsize,SPI_FLASH_DMA_ADR_FIX_MODE);
//

	
	EBI_LCD->EBI_LCD_REG = 0x00F0; //pixel data interface
	//EBI_LCD->EBI_LCD_RAM = 0x0002;  // 16-bit packed
	EBI_LCD->EBI_LCD_RAM = 0x0003;  // 16-bit (565 format)  
}


/*********************************************************************************************************//**
  * @brief  Draw a picture.
  * @param  X_Location: X position.
  * @param  Y_Location: Y position.
  * @param  Height: rectangle height.
  * @param  Width: rectangle width.
  * @param  Pptr: point to pixel data of picture.
  * @retval None
  ***********************************************************************************************************/
void LCD_PicDraw(u8 X_Location, u16 Y_Location, u8 Height, u16 Width, uc8 *Pptr)
{
  u32 xid = 0;
  u32 ImgAdds = 0;
  u32 yid = 0;
  u32 i = 0, j = 0, color = 0;

  xid = Height + X_Location;
  yid = Y_Location;
  LCD_StarterSet(xid, yid);

  for (i = 0; i < Height; i++)
  {
    LCD_WriteRAMPrior();  // Get ready to write GRAM

    for (j = 0; j < Width; j++)
    {
      ImgAdds = (i * Width * 2) + (j * 2);

      color = Pptr[ImgAdds] << 8 | (Pptr[ImgAdds + 1]);

      EBI_LCD->EBI_LCD_RAM = color;
    }
    xid--;
    LCD_StarterSet(xid, yid);
  }
}




/*********************************************************************************************************//**
  * @brief  Set the background color.
  * @param  Color: background color of RGB 5-6-5.
  * @retval None
  ***********************************************************************************************************/
void LCD_BackColorSet(u16 Color)
{
  Color_Back = Color;
}

/*********************************************************************************************************//**
  * @brief  Set the text color.
  * @param  Color: text color of RGB 5-6-5.
  * @retval None
  ***********************************************************************************************************/
void LCD_TextColorSet(u16 Color)
{
  Color_Text = Color;
}

/*********************************************************************************************************//**
  * @brief  Draw a point.
  * @param  u16 Xpos,u16 Ypos,u16 Color.
  * @retval None
  ***********************************************************************************************************/
void LCD_DrawPoint(u16 x, u16 y, u16 color)
{
	EBI_LCD->EBI_LCD_REG = 0x2A;
  EBI_LCD->EBI_LCD_RAM = x >> 8;
  EBI_LCD->EBI_LCD_RAM = x & 0xff;
  EBI_LCD->EBI_LCD_RAM = x >> 8;
  EBI_LCD->EBI_LCD_RAM = x & 0xff;
  
  EBI_LCD->EBI_LCD_REG = 0x2B;
  EBI_LCD->EBI_LCD_RAM = y >> 8;
  EBI_LCD->EBI_LCD_RAM = y & 0xff;
  EBI_LCD->EBI_LCD_RAM = y >> 8;
  EBI_LCD->EBI_LCD_RAM = y & 0xff;
  
  EBI_LCD->EBI_LCD_REG = 0x2C;
  EBI_LCD->EBI_LCD_RAM = color;
}  

/*********************************************************************************************************//**
  * @brief  Display the characters on line.
  * @param  u16 Xpos, u16 Ypos,  u16 Height, u16 Width, u8 Mode:0(非叠加方式).
  * @param  uc8 *Sptr: point to the string data.
  * @retval None
  ***********************************************************************************************************/
//void LCD_ShowString(u16 Xpos, u16 Ypos, u16 Height, u16 Width, u8 Mode, uc8 *Sptr)
//{         
//	while(*Sptr != '\0')
//	{       
//		if(Xpos > EBI_LCD_Width - 1)
//		{
//			Xpos = 0;
//			Ypos += Height;
//		}
//		if(Ypos > EBI_LCD_Height - 1)
//		{
//			Ypos = Xpos = 0;
//		}
//    
//		LCD_CharDraw(Xpos, Ypos, Height, Width, Mode, *Sptr);
//		Xpos += Width;
//		Sptr++;
//	}   
//}

/*********************************************************************************************************//**
  * @brief  Display a character.
  * @param  u16 Xpos, u16 Ypos, u16 Height, u16 Width, u8 Mode, u8 Ascii
  * @retval None
  ***********************************************************************************************************/
#define Font_Column             (16)
#define Font_Raw                (24)

//#define Font_Column             (24)
//#define Font_Raw                (16)

void LCD_CharDisplay(u32 Line_Num, u32 Column, u32 Ascii)
{
  Ascii -= 32;
  LCD_CharDraw(Line_Num, Column, (u16 *)&ASCII_2416[Ascii * 24]);//ASCII_1206//ASCII_1206
}

void LCD_CharDraw(u32 X_Location, u32 Y_Location, u16 *Cptr)
{
  u32 xid = X_Location;
  u32 i = 0, j = 0;

  LCD_StarterSet(X_Location, Y_Location);

  for (i = 0; i < Font_Raw; i++)
  {
    LCD_WriteRAMPrior();  // Get ready to write GRAM
    for (j = 0; j < Font_Column; j++)
    {
      if ((Cptr[i] & (1 << j)) == 0x00)
      {
        LCD_WriteRAM(Color_Back);
      }
      else
      {
        LCD_WriteRAM(Color_Text);
      }
    }
    xid++;
    LCD_StarterSet(xid, Y_Location);
  }
}


//void LCD_StringLineDisplay(u8 Line_Num, char *Sptr)
//{
//  u32 i = 0;
//  u16 reference_column = 50;
//  /* Send character by character on LCD */
//  while((*Sptr != 0) & (i < 20))
//  {
//    LCD_CharDisplay(Line_Num, reference_column, *Sptr);

//    /* Increment the column position by 16, because character is size of 16x24 */
//    reference_column += 16;
//    /* Point to the next character */
//    Sptr ++;

//    /* Increase the character counter */
//    i ++;
//  }
//}
void LCD_StringLineDisplay(u8 Line_Num, char *Sptr)
{
  u32 i = 0;
  u16 reference_column = 0;
  /* Send character by character on LCD */
  while((*Sptr != 0) & (i < 20))
  {
    LCD_CharDisplay(Line_Num, reference_column, *Sptr);

    /* Increment the column position by 16, because character is size of 16x24 */
    reference_column += 16;
    /* Point to the next character */
    Sptr ++;

    /* Increase the character counter */
    i ++;
  }
}




//void LCD_CharDraw(u16 Xpos, u16 Ypos, u16 Height, u16 Width, u8 Mode, u8 Ascii)
//{
//	u32 temp = 0;
//	u16 x, y;

//	if(Xpos > EBI_LCD_Width - 1 || Ypos > EBI_LCD_Height - 1)
//		return;

//	LCD_SetDisplayArea(Xpos, Ypos, Height, Width);

//	Ascii = Ascii - 32;	//-' ' ???????
//	for(y = 0; y < Height; y++)
//	{
//		if(Height == 12)
//			temp = ASCII_1206[Ascii][y];				//??1206??
//		else if (Height == 16) 
//			temp = ASCII_1608[Ascii][y]; 				//??1608??
//		else if(Height == 24)
//			temp = ASCII_2416[Ascii * 24 + y];	//??2416??
//		
//		for(x = 0; x < Width; x++)
//		{                 
//			if(temp & 0x01)
//				LCD_DrawPoint(Xpos+x, Ypos+y, Color_Text);
//			else
//			{
//				if(!Mode)//?????				
//					LCD_DrawPoint(Xpos+x, Ypos+y, Color_Back);   
//			}					
//			temp >>= 1; 
//		}
//	}
//	
//	/*
//	u32 temp=0;
//	u16 x, y;

//	if(Xpos > HDP || Ypos > VDP)
//		return;

//  LCD_SetDisplayArea(Xpos, Ypos, Height, Width);

//	Ascii = Ascii - 32;											//-' ' 得到偏移后的值
//	for(y = 0; y < Height; y++)
//	{
//		if(Height == 12)
//			temp = ASCII_1206[Ascii][y];				//调用1206字体
//		else if (Height == 16) 
//			temp = ASCII_1608[Ascii][y]; 				//调用1608字体
//		else if(Height == 24)
//			temp = ASCII_2416[Ascii * 24 + y];	//调用2416字体
//		
//		for(x = 0; x < Width; x++)
//		{                 
//			if(temp & 0x01)
//        LCD_DrawPoint(Xpos+x, Ypos+y, Color_Text);
//			else
//			{
//				if(!Mode)//非叠加方式				
//          LCD_DrawPoint(Xpos+x, Ypos+y, Color_Back);   
//			}					
//			temp >>= 1; 
//		}
//	}	
////	*/
//} 

/*********************************************************************************************************//**
  * @brief  Display one character (16 dots width, 24 dots height).
  * @param  Line_Num: Where to display the character, should be LINE0 ~ LINE9.
  * @param  Column: start column address.
  * @param  Ascii: character ascii code, must be between 0x20 and 0x7E.
  * @retval None
  ***********************************************************************************************************/
/*void LCD_CharDisplay(u8 Line_Num, u16 Column, u8 Ascii)
{
  Ascii -= 32;
  LCD_CharDraw(Line_Num, Column, &ASCII_Font_Table[Ascii * 24]);
}*/


/*********************************************************************************************************//**
  * @brief  LCD_GetScanLine.
  * @retval None
  ***********************************************************************************************************/
u16 LCD_GetScanLine(void)
{
  u16 line;
  
  EBI_LCD->EBI_LCD_REG = 0x45; 
	__NOP();__NOP();__NOP();__NOP();__NOP();
  line = (EBI_LCD->EBI_LCD_RAM & 0xFF) << 8;
	__NOP();__NOP();__NOP();__NOP();__NOP();
  line |= EBI_LCD->EBI_LCD_RAM & 0xFF; 
	__NOP();__NOP();__NOP();__NOP();__NOP();
  return line;
}


void LCD_BL_DutySet(u8 num)
{
  EBI_LCD->EBI_LCD_REG = 0x00BE;			//set PWM for B/L
  EBI_LCD->EBI_LCD_RAM = 0x000E;			//PWM signal frequency = 120M/(256*(14+1))/256 = 122 hz
  EBI_LCD->EBI_LCD_RAM = 0x0080;			//由于0XD0置A0为1,故此参数被忽略
  EBI_LCD->EBI_LCD_RAM = 0x0009;			//PWM controlled by DBC , PWM enable
  EBI_LCD->EBI_LCD_RAM = num;	  			//PWM duty cycle = DBC output * num / 255 , brightest
  EBI_LCD->EBI_LCD_RAM = 0x0000;			//Set the minimum brightness level.  Dimmest
  EBI_LCD->EBI_LCD_RAM = 0x0000;			//Divcode off
}

/*********************************************************************************************************//**
  * @brief  Display the maximum 20 characters on one line.
  * @param  Line_Num: Where to display the character, should be LINE0 ~ LINE9.
  * @param  Sptr: point to the string data.
  * @retval None
  ***********************************************************************************************************/
#if 0  // ... 
void LCD_StringLineDisplay(u8 Line_Num, char *Sptr)
{
  u32 i = 0;
  u16 reference_column = 0;

  /* Send character by character on LCD */
  while((*Sptr != 0) & (i < 20))
  {
    LCD_CharDisplay(Line_Num, reference_column, *Sptr);
    /* Increment the column position by 16, because character is size of 16x24 */
   // reference_column += 16;
		
		reference_column += 24;
		
    /* Point to the next character */
    Sptr ++;

    /* Increase the character counter */
    i ++;
  }
}
#endif


/*********************************************************************************************************//**
  * @brief  Inserts a delay_ms time.
  * @param  nCount: specifies the delay_ms time length.
  * @retval None
  ***********************************************************************************************************/
static void delay_ms(u32 uDelay)
{
  u16 i=0;  
  while(uDelay--)
  {
		i=12000;  //????
		while(i--) ;    
  }
}

/*********************************************************************************************************//**
  * @brief  Display a line.
  * @param  X_Location: specify the X position.
  * @param  Y_Location: specify the Y position.
  * @param  Length: line length.
  * @param  Direction: line direction.
  *   This parameter can be one of the following values:
  *     @arg Vertical
  *     @arg Horizontal
  * @retval None
  ***********************************************************************************************************/
void LCD_LineDraw(u8 X_Location, u16 Y_Location, u16 Length, u8 Direction)
{
  u32 i = 0;

  LCD_StarterSet(X_Location, Y_Location);

  if (Direction == Horizontal)
  {
    LCD_WriteRAMPrior();  // Get ready to write GRAM
    for (i = 0; i < Length; i++)
    {
      LCD_WriteRAM(0xFFFF);
    }
  }
  else
  {
    for (i = 0; i < Length; i++)
    {
			LCD_WriteRAMPrior();
      LCD_WriteRAM(0xFFFF);
      X_Location++;
      LCD_StarterSet(X_Location, Y_Location);
    }
  }
}

#define ABS(X)  ((X) > 0 ? (X) : -(X))    

void LCD_DrawUniLine(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2)
{
  int16_t deltax = 0, deltay = 0, x = 0, y = 0, xinc1 = 0, xinc2 = 0, 
  yinc1 = 0, yinc2 = 0, den = 0, num = 0, numadd = 0, numpixels = 0, 
  curpixel = 0;
  
  deltax = ABS(x2 - x1);        /* The difference between the x's */
  deltay = ABS(y2 - y1);        /* The difference between the y's */
  x = x1;                       /* Start x off at the first pixel */
  y = y1;                       /* Start y off at the first pixel */
  
  if (x2 >= x1)                 /* The x-values are increasing */
  {
    xinc1 = 1;
    xinc2 = 1;
  }
  else                          /* The x-values are decreasing */
  {
    xinc1 = -1;
    xinc2 = -1;
  }
  
  if (y2 >= y1)                 /* The y-values are increasing */
  {
    yinc1 = 1;
    yinc2 = 1;
  }
  else                          /* The y-values are decreasing */
  {
    yinc1 = -1;
    yinc2 = -1;
  }
  
  if (deltax >= deltay)         /* There is at least one x-value for every y-value */
  {
    xinc1 = 0;                  /* Don't change the x when numerator >= denominator */
    yinc2 = 0;                  /* Don't change the y for every iteration */
    den = deltax;
    num = deltax / 2;
    numadd = deltay;
    numpixels = deltax;         /* There are more x-values than y-values */
  }
  else                          /* There is at least one y-value for every x-value */
  {
    xinc2 = 0;                  /* Don't change the x for every iteration */
    yinc1 = 0;                  /* Don't change the y when numerator >= denominator */
    den = deltay;
    num = deltay / 2;
    numadd = deltax;
    numpixels = deltay;         /* There are more y-values than x-values */
  }
  
  for (curpixel = 0; curpixel <= numpixels; curpixel++)
  {
    //PutPixel(x, y);             /* Draw the current pixel */
    LCD_DrawPoint(x,y, 0xFFFF);
		num += numadd;              /* Increase the numerator by the top of the fraction */
    if (num >= den)             /* Check if numerator >= denominator */
    {
      num -= den;               /* Calculate the new numerator value */
      x += xinc1;               /* Change the x as appropriate */
      y += yinc1;               /* Change the y as appropriate */
    }
    x += xinc2;                 /* Change the x as appropriate */
    y += yinc2;                 /* Change the y as appropriate */
  }
}


/*
void DrawArc(double x, double y,double r,double s, double e)
{
	// declare circle points array variables
	double xcoords[360];
	double ycoords[360];

	// declare circle centre co-ordinates variables
	double d_centre_x;
	double d_centre_y;

	// declare radius
	double radius;

	// declare start of arc
	double start_arc;

	// declare end of arc
	double end_arc;

	// declare degree
	double degree;

	// declare degrees to radians
	double deg_to_rads;
	
	// declare radians to degrees
	double rads_to_deg;
	
	// declare int version of circle point co-ordinates
	int px;
	int py;

	// declare int version of circle centre co-ordinates
	int cx;
	int cy;

	//declare int  version of arc start point and end point
	int start;
	int end;
	
	// declare ctr counter variables
	int ctr;
	int ctr1;

	// initialise deg_to_rad
	deg_to_rads=0.0174532925;

	//initialise rads_to_deg
	rads_to_deg=57.2957795;

	// import parameters into variables
	d_centre_x=x;
	d_centre_y=y;
	radius=r;
	start_arc=s;
	end_arc=e;
	
        // declare int version of circle centre x & y co-ordinates
	cx=(int)d_centre_x;
	cy=(int)d_centre_y;

	// declare int version of arc start and end
        start=(int)start_arc;
	end=(int)end_arc;

	// set up loop to calculate circle points
	
	for(ctr=1;ctr<360;ctr++)
	{  
     degree=ctr*deg_to_rads;
	   xcoords[ctr]=d_centre_x+(radius*cos(degree));
	   ycoords[ctr]=d_centre_y+(radius*sin(degree));
	   px=(int)xcoords[ctr];
	   py=(int)ycoords[ctr];
	}
	   
	// set up loop to draw arc
	for(ctr1 = 1;ctr1<360;ctr1++)
	   {
	   if(ctr1>=start && ctr1<=end)
	   	{
		 cx=(int)d_centre_x;
	         cy=(int)d_centre_y;

		 px=(int)xcoords[ctr1];
		 py=(int)ycoords[ctr1];
		 
		 //MoveToEx(hdc,px,py,NULL);
		 //LineTo(hdc,cx,cy);
				
			LCD_DrawUniLine(px, py, cx, cy);
		}
	 }
}
*/

/*********************************************************************************************************//**
  * @brief  Display a circle.
  * @param  X_Location: specify the X position.
  * @param  Y_Location: specify the Y position.
  * @param  Radius: radius of the circle.
  * @retval None
  ***********************************************************************************************************/
void LCD_CircleDraw(u8 X_Location, u16 Y_Location, u16 Radius)
{
  s32  DV;  // Decision Variable
  u32  X_Value;  // Current X Value
  u32  Y_Value;  // Current Y Value

  DV = 3 - (Radius << 1);
  X_Value = 0;
  Y_Value = Radius;

  while (X_Value <= Y_Value)
  {
    LCD_StarterSet(X_Location + X_Value, Y_Location + Y_Value);
    LCD_WriteRAM(Color_Text);

    LCD_StarterSet(X_Location + X_Value, Y_Location - Y_Value);
    LCD_WriteRAM(Color_Text);

    LCD_StarterSet(X_Location - X_Value, Y_Location + Y_Value);
    LCD_WriteRAM(Color_Text);

    LCD_StarterSet(X_Location - X_Value, Y_Location - Y_Value);
    LCD_WriteRAM(Color_Text);

    LCD_StarterSet(X_Location + Y_Value, Y_Location + X_Value);
    LCD_WriteRAM(Color_Text);

    LCD_StarterSet(X_Location + Y_Value, Y_Location - X_Value);
    LCD_WriteRAM(Color_Text);

    LCD_StarterSet(X_Location - Y_Value, Y_Location + X_Value);
    LCD_WriteRAM(Color_Text);

    LCD_StarterSet(X_Location - Y_Value, Y_Location - X_Value);
    LCD_WriteRAM(Color_Text);

    if (DV < 0)
    {
      DV += (X_Value << 2) + 6;
    }
    else
    {
      DV += ((X_Value - Y_Value) << 2) + 10;
      Y_Value--;
    }
    X_Value++;
  }
}


/*********************************************************************************************************//**
  * @brief
  * @param  
  * @param  
  * @param  
  * @retval
  ***********************************************************************************************************/
void LCD_FillRect(u16 x, u16 y, u16 width, u16 height, u16 color){
	int i, j;

	for (i = 0; i < height; i++) {

		EBI_LCD->EBI_LCD_REG = 0x2B;
		EBI_LCD->EBI_LCD_RAM = (y+i) >> 8;
		EBI_LCD->EBI_LCD_RAM = (y+i) & 0xff;
		EBI_LCD->EBI_LCD_RAM = (y+i) >> 8;
		EBI_LCD->EBI_LCD_RAM = (y+i) & 0xff;
			
		for (j = 0; j < width; j++) {
			EBI_LCD->EBI_LCD_REG = 0x2A;
			EBI_LCD->EBI_LCD_RAM = (x+j) >> 8;
			EBI_LCD->EBI_LCD_RAM = (x+j) & 0xff;
			EBI_LCD->EBI_LCD_RAM = (x+j) >> 8;
			EBI_LCD->EBI_LCD_RAM = (x+j) & 0xff;

			EBI_LCD->EBI_LCD_REG = 0x2C;
			EBI_LCD->EBI_LCD_RAM = color;
		}
   }
}

void LCD_FillRect2(uint16_t Xpos, uint16_t Ypos, uint16_t Width, uint16_t Height, u16 color)
{
	//LCD_SetTextColor(TextColor);
	
	LCD_TextColorSet(color);
	
	LCD_LineDraw(Xpos, Ypos, Width, Horizontal);
  LCD_LineDraw((Xpos + Height), Ypos, Width, Horizontal);
  
  LCD_LineDraw(Xpos, Ypos, Height, Vertical);
  LCD_LineDraw(Xpos, (Ypos - Width + 1), Height, Vertical);

  Width -= 2;
  Height--;
  Ypos--;

  //LCD_SetTextColor(color);

  while(Height--)
  {
    LCD_LineDraw(++Xpos, Ypos, Width, Horizontal);    
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

/**
  * @}
  */

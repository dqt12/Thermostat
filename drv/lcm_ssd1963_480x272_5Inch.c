 /*********************************************************************************************************//**
 * @file    ebi_lcd.c
 * @version V1.0
 * @date    03/12/2012
 * @brief   This file provides a set of functions needed to manage the
 *          communication between EBI peripheral and LCD HX8347-D.
 *************************************************************************************************************
 *
 * <h2><center>Copyright (C) 2012 Holtek Semiconductor Inc. All rights reserved</center></h2>
 *
 ************************************************************************************************************/

/* Includes ------------------------------------------------------------------------------------------------*/
#include "lcm.h"
#include "lcm_ssd1963_480x272_5Inch.h"
#include "font.h"
#include "ZH_GPTM.h"
/* Private typedef -----------------------------------------------------------------------------------------*/
/* Private define ------------------------------------------------------------------------------------------*/



//#define LCM_EXCHANGE_X_Y  1

//LCD_DrawSize LCD_IData;


#if (LCM_DIRECTION == LCM_NORMAL)
  // ---> Column 480 (MY=0 MX=0 MV=0)
  // | 
  // V
  // Page 800
  #define EC (272)
  #define EP (480)
  #define MADCTR (0x00)
#elif (LCM_DIRECTION == LCM_LEFT_90)
  // Column 800
  // ^
  // |  
  // |--> Page 480   (MY=1 MX=0 MV=1)
  #define EC (480)
  #define EP (272)
  #define MADCTR (0xA0)
#elif (LCM_DIRECTION == LCM_RIGHT_90)
  //   Page 480  <--| (MY=0 MX=1 MV=1)
  //                |
  //                V
  //           Column 800
  #define EC (480)
  #define EP (272)
  #define MADCTR (0x60)
#elif (LCM_DIRECTION == LCM_ROTATE_180)
  //              Page 800
  //                ^ 
  //                |
  //  Column 480 <--- (MY=1 MX=1 MV=0)
  #define EC (272)
  #define EP (480)
  #define MADCTR (0xC0)
#elif (LCM_DIRECTION == LCM_ROTATE_180_INV)
  // Page 800
  //   ^ 
  //   |
  //   ---> Column 480  (MY=1 MX=0 MV=0)
  #define EC (272)
  #define EP (480)
  #define MADCTR (0x80)
#endif

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

///* Private macro -------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------------------------------------*/

ColorType Color_Text = 0x0000;  // default: Black
ColorType Color_Back = 0xFFFF;  // default: White

EBI_InitTypeDef  EBI_InitStructure;

/* Private function prototypes -----------------------------------------------------------------------------*/

static void delay_ms(u32 ms);
static void delay_us(u32 nTime);
/* Private functions ---------------------------------------------------------------------------------------*/

/*********************************************************************************************************//**
  * @brief  LCM_CMD
  * @retval None
  ***********************************************************************************************************/
void LCM_CMD(u16 cmd)
{  
  EBI_LCD->EBI_LCD_REG = cmd;
}

/*********************************************************************************************************//**
  * @brief  LCM_WRITE_DATA
  * @retval None
  ***********************************************************************************************************/
void LCM_WRITE_DATA(u16 data)
{
  EBI_LCD->EBI_LCD_RAM = data;
}

/*********************************************************************************************************//**
  * @brief  LCM_READ_DATA
  * @retval None
  ***********************************************************************************************************/
u16 LCM_READ_DATA(void)
{
  return EBI_LCD->EBI_LCD_RAM;
}

///***************************************************************************//**
//  * @brief  Configures the EBI and GPIOs to interface with the SRAM memory.
//  * @retval None
//  *****************************************************************************/
void LCD_Init(void)
{
	CKCU_PeripClockConfig_TypeDef CKCUClock = {{0}};
	CKCUClock.Bit.PA 					= 1;
	CKCUClock.Bit.PB 					= 1;
	CKCUClock.Bit.PC 					= 1;
	CKCUClock.Bit.EBI         = 1;
	CKCUClock.Bit.AFIO        = 1;
	CKCU_PeripClockConfig(CKCUClock, ENABLE);

	/* LCM RST configuration */
	LCM_RST_INIT();
	LCM_RST_INACTIVE();  
	
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
	
#if  (LCD_CONTROL_MODE == RGB565_EBI16)
	AFIO_GPxConfig(LCD_EBI_AD8_GPIO_ID, LCD_EBI_AD8_AFIO_PIN, LCD_EBI_AD8_AFIO_MODE);
	AFIO_GPxConfig(LCD_EBI_AD9_GPIO_ID, LCD_EBI_AD9_AFIO_PIN, LCD_EBI_AD9_AFIO_MODE);
	AFIO_GPxConfig(LCD_EBI_AD10_GPIO_ID, LCD_EBI_AD10_AFIO_PIN, LCD_EBI_AD10_AFIO_MODE);
	AFIO_GPxConfig(LCD_EBI_AD11_GPIO_ID, LCD_EBI_AD11_AFIO_PIN, LCD_EBI_AD11_AFIO_MODE);
	AFIO_GPxConfig(LCD_EBI_AD12_GPIO_ID, LCD_EBI_AD12_AFIO_PIN, LCD_EBI_AD12_AFIO_MODE);
	AFIO_GPxConfig(LCD_EBI_AD13_GPIO_ID, LCD_EBI_AD13_AFIO_PIN, LCD_EBI_AD13_AFIO_MODE);
	AFIO_GPxConfig(LCD_EBI_AD14_GPIO_ID, LCD_EBI_AD14_AFIO_PIN, LCD_EBI_AD14_AFIO_MODE);
	AFIO_GPxConfig(LCD_EBI_AD15_GPIO_ID, LCD_EBI_AD15_AFIO_PIN, LCD_EBI_AD15_AFIO_MODE);
#endif

	/*  EBI Configuration                                                                                     */
	EBI_InitStructure.EBI_Bank = EBI_BANK_0;
	EBI_InitStructure.EBI_Mode = EBI_MODE_D16;
	EBI_InitStructure.EBI_IdleCycle = EBI_IDLECYCLE_DISABLE;
	EBI_InitStructure.EBI_ChipSelectPolarity = EBI_CHIPSELECTPOLARITY_LOW;
	EBI_InitStructure.EBI_AddressLatchPolarity = EBI_ADDRESSLATCHPOLARITY_LOW;
	EBI_InitStructure.EBI_WriteEnablePolarity = EBI_WRITEENABLEPOLARITY_LOW;
	EBI_InitStructure.EBI_ReadEnablePolarity = EBI_READENABLEPOLARITY_LOW;
//	EBI_InitStructure.EBI_AddressSetupTime = 2;
//	EBI_InitStructure.EBI_AddressHoldTime = 5;    /* For low speed access */
//	EBI_InitStructure.EBI_WriteSetupTime = 5;     /* For low speed access */
//	EBI_InitStructure.EBI_WriteStrobeTime = 6;    /* For low speed access */
//	EBI_InitStructure.EBI_WriteHoldTime = 5;      /* For low speed access */
//	EBI_InitStructure.EBI_ReadSetupTime = 2;
//	EBI_InitStructure.EBI_ReadStrobeTime = 5;
//	EBI_InitStructure.EBI_ReadHoldTime = 1;

	EBI_InitStructure.EBI_AddressSetupTime = 2;
	EBI_InitStructure.EBI_AddressHoldTime = 5;    /* For low speed access */
	EBI_InitStructure.EBI_WriteSetupTime = 8;     /* For low speed access */
	EBI_InitStructure.EBI_WriteStrobeTime = 9;    /* For low speed access */
	EBI_InitStructure.EBI_WriteHoldTime = 5;      /* For low speed access */
	EBI_InitStructure.EBI_ReadSetupTime = 5;
	EBI_InitStructure.EBI_ReadStrobeTime = 9;
	EBI_InitStructure.EBI_ReadHoldTime = 2;

	EBI_InitStructure.EBI_PageMode = EBI_PAGEMODE_ENABLE;//EBI_PAGEMODE_DISABLE;
	EBI_InitStructure.EBI_PageLength = EBI_PAGELENGTH_32;//EBI_PAGELENGTH_4;
	EBI_InitStructure.EBI_PageHitMode = EBI_PAGEHITMODE_INTRPAGE;//EBI_PAGEHITMODE_ADDINC;
	EBI_InitStructure.EBI_PageAccessTime = 0xF;
	EBI_InitStructure.EBI_PageOpenTime = 0xFF;
	EBI_Init(&EBI_InitStructure);
	
	EBI_Cmd(EBI_BANK_0, ENABLE);
  

	/* LCD BL USE GPTM configuration */
  ZH_gGPTM.PinLv = 500;
	ZH_gGPTM.JingQueDu = 100;
	ZH_gGPTM.XiTongPinLv = 96000000;
  ZH_GPTM_Init();
  
  ZH_GPTM_CCR_Num(90);
  ZH_GPTM_Switch(ENABLE);
  ZH_GPTMCH_Switch(ENABLE);
  delay_ms(100);
}


/*********************************************************************************************************//**
  * @brief  Configures the EBI and GPIOs to interface with the SRAM memory.
  * @retval None
  ***********************************************************************************************************/
 /**/
void LCD_Config(void)
{
	u16 pll_lock;
  //RESET LCD 
  LCM_RST_ACTIVE();
  delay_ms(20);
  LCM_RST_INACTIVE();
  delay_ms(20);

 
  EBI_LCD->EBI_LCD_REG = 0x01;  // software reset
  delay_ms(10);

  EBI_LCD->EBI_LCD_REG = 0xE0;  // PLL enable
  EBI_LCD->EBI_LCD_RAM = 0x01;/*使用参考时钟作为系统时钟*/
  delay_us(500);
	
//tako
	EBI_LCD->EBI_LCD_REG = 0x00E2;
	EBI_LCD->EBI_LCD_RAM = 0x0023;			//(35+1)*10=360MHZ			250<360<800
  EBI_LCD->EBI_LCD_RAM = 0x0003;			//PLL = 360MHZ/3 = 120MHZ
  EBI_LCD->EBI_LCD_RAM = 0x0054;			//dummy
		
	do
	{
	  EBI_LCD->EBI_LCD_REG = 0xE4;
		pll_lock = EBI_LCD->EBI_LCD_RAM;
		if((pll_lock&0x04) == 0x04)	
			break;
	}while(1);

  EBI_LCD->EBI_LCD_REG = 0xE0;
  EBI_LCD->EBI_LCD_RAM = 0x03;//使用锁相环时钟作为系统时钟	
	
  EBI_LCD->EBI_LCD_REG = 0x01;  // software reset
  delay_ms(10);
	
  //4 clock write 1 pixel (16bits) 
	EBI_InitStructure.EBI_AddressHoldTime = 2;
  EBI_InitStructure.EBI_WriteSetupTime = 2;
  EBI_InitStructure.EBI_WriteStrobeTime = 4;
  EBI_InitStructure.EBI_WriteHoldTime = 2;
  EBI_Init(&EBI_InitStructure);
	
	//Frame
//	EBI_LCD->EBI_LCD_REG = 0x00E6;
//  EBI_LCD->EBI_LCD_RAM = 0x0002;	// 60HZ 800*480
//  EBI_LCD->EBI_LCD_RAM = 0x00AA;	//LCDC_FPR=0x2AAA9=174761	
//  EBI_LCD->EBI_LCD_RAM = 0x00A9;	//CLK=PLL x ((LCDC_FPR + 1) / 2^20) = 20MHz			
//																					
  EBI_LCD->EBI_LCD_REG = 0x00E6;
  EBI_LCD->EBI_LCD_RAM = 0x0001;			// 60HZ 272*480
  EBI_LCD->EBI_LCD_RAM = 0x000A;			//LCDC_FPR=0x10A3E=68158	
  EBI_LCD->EBI_LCD_RAM = 0x003E;			//CLK=PLL x ((LCDC_FPR + 1) / 2^20) = 7.8MHz	

	delay_ms(5);																		
	// set hori period	
	EBI_LCD->EBI_LCD_REG = 0x00B0;
  EBI_LCD->EBI_LCD_RAM = 0x0008;						//TFT panel data width : 24-bit
  EBI_LCD->EBI_LCD_RAM = 0x0000;						//TFT mode
  EBI_LCD->EBI_LCD_RAM = (HDP>>8)&0x00FF;	//Horizontal panel size(HDP) : 480
  EBI_LCD->EBI_LCD_RAM = HDP&0x00FF;
  EBI_LCD->EBI_LCD_RAM = (VDP>>8)&0x00FF;	//Vertical panel size(VDP) : 272
  EBI_LCD->EBI_LCD_RAM = VDP&0x00FF;
  EBI_LCD->EBI_LCD_RAM = 0x00;						//RGB
	
  EBI_LCD->EBI_LCD_REG = 0x00B4;
  EBI_LCD->EBI_LCD_RAM = (HT>>8) & 0x00FF;
  EBI_LCD->EBI_LCD_RAM = HT & 0x00FF;
  EBI_LCD->EBI_LCD_RAM = (HPS>>8) & 0x00FF;
  EBI_LCD->EBI_LCD_RAM = HPS&0x00FF;
  EBI_LCD->EBI_LCD_RAM = HPW;
  EBI_LCD->EBI_LCD_RAM = (LPS>>8) & 0x00FF;
  EBI_LCD->EBI_LCD_RAM = LPS&0x00FF;
  EBI_LCD->EBI_LCD_RAM = 0x00;

	// set vert period
  EBI_LCD->EBI_LCD_REG = 0x00B6;								// VSYNC
  EBI_LCD->EBI_LCD_RAM = (VT>>8) & 0x00FF;   	// ... Set VT
  EBI_LCD->EBI_LCD_RAM = VT & 0x00FF;
  EBI_LCD->EBI_LCD_RAM = (VPS>>8) & 0x00FF;  		// ... Set VPS
  EBI_LCD->EBI_LCD_RAM = VPS & 0x00FF;
  EBI_LCD->EBI_LCD_RAM = VPW;			   						// ...Set VPW
  EBI_LCD->EBI_LCD_RAM = (FPS>>8) & 0x00FF;  			// ... Set FPS
  EBI_LCD->EBI_LCD_RAM = FPS&0x00FF;

//STB on --then-> GRB on
  EBI_LCD->EBI_LCD_REG = 0x00B8;
  EBI_LCD->EBI_LCD_RAM = 0x000F;			//GPIO[3:0]=output
  EBI_LCD->EBI_LCD_RAM = 0x0001;			//GPIO0 normal
  
  EBI_LCD->EBI_LCD_REG = 0x00BA;
  EBI_LCD->EBI_LCD_RAM = 0x0000;			//GPIO[3:0] out 1		//GPIO[3:0] outputs 0
  delay_ms(1);
	
	EBI_LCD->EBI_LCD_REG = 0x00BA;
  EBI_LCD->EBI_LCD_RAM = 0x0001;			//GPIO[3:0] out 1		//GPIO1 out 1
  delay_ms(5);

//  EBI_LCD->EBI_LCD_REG = 0x00BA;
//  EBI_LCD->EBI_LCD_RAM = 0x00002;			//GPIO[3:0] out 1

//  EBI_LCD->EBI_LCD_REG = 0x00BA;
//  EBI_LCD->EBI_LCD_RAM = 0x00003;  		//GPIO[3:0] out 1
//	delay_ms(10);
//////////////////////////////////////////////////////////////////	

  EBI_LCD->EBI_LCD_REG = 0x00D0;			//set DBC config 
  EBI_LCD->EBI_LCD_RAM = 0x000D;			//DBC enable , Aggressive mode


  //EBI_LCD->EBI_LCD_RAM = 0x00FC;  
  //EBI_LCD->EBI_LCD_RAM = 0x002B;
  //EBI_LCD->EBI_LCD_RAM = 0x0023;		// ... important, LCM Board v1.0    A[3] = 1, RGB->BGR
  
//  EBI_LCD->EBI_LCD_REG = 0x00BE;			//set PWM for B/L
//  EBI_LCD->EBI_LCD_RAM = 0x000E;			//PWM signal frequency = 120M/(256*(14+1))/256 = 122 hz
//  EBI_LCD->EBI_LCD_RAM = 0x00FF;			//由于0XD0置A0为1，故此参数被忽略
//  EBI_LCD->EBI_LCD_RAM = 0x0009;			//PWM controlled by DBC , PWM enable
//  EBI_LCD->EBI_LCD_RAM = 0x00FF;			//PWM duty cycle = DBC output * 0x00FF / 255 , brightest
//  EBI_LCD->EBI_LCD_RAM = 0x0000;			//Set the minimum brightness level.  Dimmest
//  EBI_LCD->EBI_LCD_RAM = 0x0000;			//Divcode off
	

#if  (LCD_CONTROL_MODE == RGB888_EBI8)
	EBI_LCD->EBI_LCD_REG = 0x00F0;			//pixel data interface
	EBI_LCD->EBI_LCD_RAM = 0x0000;			// 8-bit (RGB format)  
	
#elif  (LCD_CONTROL_MODE == RGB565_EBI16)	
  EBI_LCD->EBI_LCD_REG = 0x00F0;			//pixel data interface
	EBI_LCD->EBI_LCD_RAM = 0x0003;			// 16-bit (565 format) 	
	
#endif
	
  delay_ms(5);
	
  EBI_LCD->EBI_LCD_REG = 0x0036;			//display mirror & turn 180 degree
	EBI_LCD->EBI_LCD_RAM = 0x0000;
 // EBI_LCD->EBI_LCD_RAM = 0x0008;			// ... important, LCM Board v1.0    A[3] = 1, RGB->BGR
	delay_ms(5);
	
  EBI_LCD->EBI_LCD_REG = 0x0026;			//display on
  EBI_LCD->EBI_LCD_RAM = 0x0001;			//Gamma curve 0
  	
  EBI_LCD->EBI_LCD_REG = 0x0029;			//display on   			
  delay_ms(5);
	
	LCD_Clear(Black);
//	LCD_Clear(Green);

//  while(1)
//  {
//    delay_ms(1000);
//    LCD_Clear(Red);
//    delay_ms(1000);
//    LCD_Clear(Green);
//    delay_ms(1000);	
//	  LCD_Clear(Blue);
//	  delay_ms(1000);	
//	  LCD_Clear(Black);
//		delay_ms(1000);	
//	  LCD_Clear(White);
//  }
}



/*********************************************************************************************************//**
  * @brief  Write value to the selected LCD register.
  * @param  LCD_Reg_Index: address of the selected register.
  * @param  LCD_Reg_Value: value of to write the selected register.
  * @retval None
  ***********************************************************************************************************/
void LCD_WriteReg(u16 LCD_Reg_Index, u16 LCD_Reg_Value)
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

/*********************************************************************************************************//**
  * @brief  Writes to the LCD RAM.
  * @param  RGB_Code: the data for RAM
  * @retval None
  ***********************************************************************************************************/
void LCD_WriteRAM(u16 Data)
{
		EBI_LCD->EBI_LCD_RAM = Data;
}

/*********************************************************************************************************//**
  * @brief  Writes to the LCD RAM for a Pixel.
  * @param  RGB_Code: the pixel color in RGB mode (5-6-5) and (8-8-8)
  * @retval None
  ***********************************************************************************************************/

void LCD_WritePixel(ColorType Color)
{
#if  (LCD_CONTROL_MODE == RGB888_EBI8)
		EBI_LCD->EBI_LCD_RAM = (Color>>16)&0xFF;
		EBI_LCD->EBI_LCD_RAM = (Color>>8)&0xFF;
		EBI_LCD->EBI_LCD_RAM = Color&0xFF;
#elif (LCD_CONTROL_MODE == RGB565_EBI16)
		EBI_LCD->EBI_LCD_RAM = Color;
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
#ifndef LCM_EXCHANGE_X_Y 

  EBI_LCD->EBI_LCD_REG = 0x2A;
  EBI_LCD->EBI_LCD_RAM = X_Location >> 8;
  EBI_LCD->EBI_LCD_RAM = X_Location & 0xff;
  EBI_LCD->EBI_LCD_RAM = HDP >> 8;//
  EBI_LCD->EBI_LCD_RAM = HDP & 0xff;//

  EBI_LCD->EBI_LCD_REG = 0x2B;
  EBI_LCD->EBI_LCD_RAM = Y_Location >> 8;
  EBI_LCD->EBI_LCD_RAM = Y_Location & 0xff;
  EBI_LCD->EBI_LCD_RAM = VDP >> 8;//
  EBI_LCD->EBI_LCD_RAM = VDP & 0xff;//
	
#else
	
	EBI_LCD->EBI_LCD_REG = 0x2B;
  EBI_LCD->EBI_LCD_RAM = X_Location >> 8;
  EBI_LCD->EBI_LCD_RAM = X_Location & 0xff;
  EBI_LCD->EBI_LCD_RAM = HDP >> 8;//
  EBI_LCD->EBI_LCD_RAM = HDP & 0xff;//

  EBI_LCD->EBI_LCD_REG = 0x2A;
  EBI_LCD->EBI_LCD_RAM = Y_Location >> 8;
  EBI_LCD->EBI_LCD_RAM = Y_Location & 0xff;
  EBI_LCD->EBI_LCD_RAM = VDP >> 8;//
  EBI_LCD->EBI_LCD_RAM = VDP & 0xff;//

#endif

}

void LCD_SetDisplayArea(u16 Column, u16 Page, u16 Height, u16 Width)
{
#ifndef LCM_EXCHANGE_X_Y 
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
	
#else
	
  EBI_LCD->EBI_LCD_REG = 0x2B;
  EBI_LCD->EBI_LCD_RAM = Column >> 8;
  EBI_LCD->EBI_LCD_RAM = Column & 0xff;
  EBI_LCD->EBI_LCD_RAM = (Column+Width-1) >> 8;
  EBI_LCD->EBI_LCD_RAM = (Column+Width-1) & 0xff;

  EBI_LCD->EBI_LCD_REG = 0x2A;
  EBI_LCD->EBI_LCD_RAM = Page >> 8;
  EBI_LCD->EBI_LCD_RAM = Page & 0xff;
  EBI_LCD->EBI_LCD_RAM = (Page+Height-1) >> 8;
  EBI_LCD->EBI_LCD_RAM = (Page+Height-1) & 0xff;
#endif  
}

void LCD_SetPartialDisplayArea(u16 Start, u16 End)
{
}

void LCD_NormalDisplay(void)
{
}

void LCD_PartialDisplayOn(void)
{
}

/*********************************************************************************************************//**
  * @brief  Clear the whole LCD.
  * @param  Color: the color to be the background.
  * @retval None
  ***********************************************************************************************************/
void LCD_Clear(ColorType Color)
{
  u32 i = ((VDP+1) * (HDP+1));
  
  LCD_StarterSet(0, 0);
  
  /* Prepare to write GRAM */
  LCD_WriteRAMPrior();
   while(i--)
  {	
		LCD_WritePixel(Color);
  } 
	
}

/*********************************************************************************************************//**
  * @brief  Set the background color.
  * @param  Color: background color of RGB 5-6-5.
  * @retval None
  ***********************************************************************************************************/
void LCD_BackColorSet(ColorType Color)
{
  Color_Back = Color;
}

/*********************************************************************************************************//**
  * @brief  Set the text color.
  * @param  Color: text color of RGB 5-6-5.
  * @retval None
  ***********************************************************************************************************/
void LCD_TextColorSet(ColorType Color)
{
  Color_Text = Color;
}

/*********************************************************************************************************//**
  * @brief  Display the characters on line.
  * @param  u16 Xpos, u16 Ypos,  u16 Height, u16 Width, u8 Mode:0(非叠加方式).
  * @param  uc8 *Sptr: point to the string data.
  * @retval None
  ***********************************************************************************************************/
void LCD_DrawString(u16 Xpos, u16 Ypos, u16 Height, u16 Width, u8 Mode, char *Sptr)
{         
	while(*Sptr != '\0')
	{       
		if(Xpos > HDP)
		{
			Xpos = 0;
			Ypos += Height;
		}
		if(Ypos > VDP)
		{
			Ypos = Xpos = 0;
		}
    
    LCD_DrawChar(Xpos, Ypos, Height, Width, Mode, *Sptr);
		Xpos += Width;
		Sptr++;
	}  
}

/*********************************************************************************************************//**
  * @brief  Display a character.
  * @param  u16 Xpos, u16 Ypos, u16 Height, u16 Width, u8 Mode, u8 Ascii
  * @retval None
  ***********************************************************************************************************/
void LCD_DrawChar(u16 Xpos, u16 Ypos, u16 Height, u16 Width, u8 Mode, u8 Ascii)
{
	u32 temp=0;
	u16 x, y;

	if(Xpos > HDP || Ypos > VDP)
		return;
	
  LCD_SetDisplayArea(Xpos, Ypos, Height, Width);

	Ascii = Ascii - 32;	//-' ' 得到偏移后的值
	
	
	LCD_WriteRAMPrior();
	for(y = 0; y < Height; y++)
	{
		 
		switch(Height)
		{
			case 12 : temp = ASCII_1206[Ascii][y];break;//调用1206字体
			case 16	: temp = ASCII_1608[Ascii][y];break;//调用1608字体
			case 24 : temp = ASCII_2416[Ascii*24+y];break;//调用2416字体
			default	: temp = ASCII_1206[Ascii][y];break;
		}

		for(x = 0; x < Width; x++)
		{		
			if(!Mode)//非叠加方式				 
			{				
					if(temp & 0x01)
						LCD_WritePixel(Color_Text);
					else
						LCD_WritePixel(Color_Back);					
			}
			else 
			{
					if(temp & 0x01)
					{
						LCD_StarterSet(Xpos+x, Ypos+y); 
						LCD_WriteRAMWord(Color_Text);
					}
			}
			
			temp >>= 1; 
		}
	}	
} 


void LCD_ShowTemp(u16 Xpos, u16 Ypos, u8 Mode, u16 Num)
{
		u8 size = 16;
		LCD_DrawChar(Xpos,Ypos,24,16,Mode,(Num/100)+'0');    
		LCD_DrawChar(Xpos+size,Ypos,24,16,Mode,(Num/10)%10+'0');      							   
    LCD_DrawChar(Xpos+2*size,Ypos,24,16,Mode,'.'); 
    LCD_DrawChar(Xpos+3*size,Ypos,24,16,Mode,Num%10+'0'); 
		LCD_DrawChar(Xpos+4*size,Ypos,24,16,Mode,'C'); 
}

void LCD_ShowNum(u16 Xpos, u16 Ypos,u8 Font_Size,u8 Mode, u16 Num)
{
		LCD_DrawChar(Xpos,Ypos,Font_Size,Font_Size/2,Mode,(Num/1000)+'0');    
		LCD_DrawChar(Xpos+Font_Size/2,Ypos,Font_Size,Font_Size/2,Mode,(Num/100)%10+'0');      							   
    LCD_DrawChar(Xpos+Font_Size,Ypos,Font_Size,Font_Size/2,Mode,(Num/10)%10+'0'); 
    LCD_DrawChar(Xpos+Font_Size*3/2,Ypos,Font_Size,Font_Size/2,Mode,Num%10+'0'); 
}

/*********************************************************************************************************//**
  * @brief  LCD_StringDisplay.
  * @param  char *Sptr
  * @retval None
  ***********************************************************************************************************/
void LCD_StringDisplay(char *Sptr)
{
	u16 x=LCD_IData.Xpos,y=LCD_IData.Ypos;
	while(*Sptr != 0)
	{
		LCD_DrawChar(x, y, LCD_IData.Height, LCD_IData.Width, LCD_IData.Mode, *Sptr);
		Sptr ++;
		x += LCD_IData.Width;
		if(x > LCD_IData.XposEnd)
		{
			x = LCD_IData.Xpos;
			y += LCD_IData.Height;
		}
		if(y > LCD_IData.YposEnd)
		{
			break;
		}
	}
}

void LCD_IData_ALL(u16 Xpos, u16 Ypos, u16 XposEnd, u16 YposEnd, u16 Height, u16 Width, u8 Mode)
{
	LCD_IData.Xpos = Xpos;
	LCD_IData.Ypos = Ypos;
	LCD_IData.XposEnd = XposEnd;
	LCD_IData.YposEnd = YposEnd;
	LCD_IData.Height = Height;
	LCD_IData.Width = Width;
	LCD_IData.Mode = Mode;
}

/*********************************************************************************************************//**
  * @brief  Write 1 word to the LCD RAM.
  * @param  RGB_Set: the pixel color in RGB mode 
  * @retval None
  ***********************************************************************************************************/
void LCD_WriteRAMWord(ColorType RGB_Set)
{
  LCD_WriteRAMPrior();
  LCD_WritePixel(RGB_Set);
}


/*********************************************************************************************************//**
  * @brief  Draw a point.
  * @param  u16 Xpos,u16 Ypos,u16 Color.
  * @retval None
  ***********************************************************************************************************/
void LCD_DrawPoint(u16 Xpos, u16 Ypos, ColorType Color)
{
  LCD_StarterSet(Xpos, Ypos); 
	LCD_WriteRAMPrior();
  LCD_WritePixel(Color);
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
  * @param  Color: line color.
  * @retval None
  ***********************************************************************************************************/
void LCD_DrawLine(u16 X_Location, u16 Y_Location, u16 Length, u8 Direction, ColorType Color)
{
  u32 i = 0;

  LCD_StarterSet(X_Location, Y_Location);

  if (Direction == Horizontal)
  {
    LCD_WriteRAMPrior();  // Get ready to write GRAM
    for (i = 0; i < Length; i ++)
    {
      LCD_WritePixel(Color);
    }
  }
  else
  {
    for (i = 0; i < Length; i ++)
    {
      LCD_WriteRAMWord(Color);
      Y_Location++;
      LCD_StarterSet(X_Location, Y_Location);
    }
  }
}

/*********************************************************************************************************//**
  * @brief  Draw a rectangle.
  * @param  X_Location: X position.
  * @param  Y_Location: Y position.
  * @param  Height: rectangle height.
  * @param  Width: rectangle width.
  * @param  Color: line color.
  * @retval None
  ***********************************************************************************************************/
void LCD_DrawRect(u16 X_Location, u16 Y_Location, u16 Height, u16 Width,ColorType Color)
{
  LCD_DrawLine(X_Location, Y_Location, Width, Horizontal,Color);
  LCD_DrawLine(X_Location, (Y_Location + Height), Width, Horizontal,Color);

  LCD_DrawLine(X_Location, Y_Location, Height, Vertical,Color);
  LCD_DrawLine(X_Location + Width - 1, Y_Location, Height, Vertical,Color);
}

/*********************************************************************************************************//**
  * @brief  Draw a Fill rectangle.
  * @param  X_Location: X position.
  * @param  Y_Location: Y position.
  * @param  Height: rectangle height.
  * @param  Width: rectangle width.
  * @param  Color: fill color.
  * @retval None
  ***********************************************************************************************************/
void LCD_DrawFillRect(u16 X_Location, u16 Y_Location, u16 Height, u16 Width,ColorType Color)
{
	
	u32 i = (Height) * (Width);
	LCD_SetDisplayArea(X_Location, Y_Location, Height, Width);
	
	LCD_WriteRAMPrior();
	
  while(i--)
  {	
		LCD_WritePixel(Color);
  } 

}
/*********************************************************************************************************//**
  * @brief  Draw a circle.
  * @param  X_Location: X position.
  * @param  Y_Location: Y position.
  * @param  Height: rectangle height.
  * @param  Width: rectangle width.
  * @param  Color: line color.
  * @retval None
  ***********************************************************************************************************/
void LCD_DrawCircle(u16 X_Location, u16 Y_Location, u16 Radius,ColorType Color)
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
    LCD_WriteRAMWord(Color);

    LCD_StarterSet(X_Location + X_Value, Y_Location - Y_Value);
    LCD_WriteRAMWord(Color);

    LCD_StarterSet(X_Location - X_Value, Y_Location + Y_Value);
    LCD_WriteRAMWord(Color);

    LCD_StarterSet(X_Location - X_Value, Y_Location - Y_Value);
    LCD_WriteRAMWord(Color);

    LCD_StarterSet(X_Location + Y_Value, Y_Location + X_Value);
    LCD_WriteRAMWord(Color);

    LCD_StarterSet(X_Location + Y_Value, Y_Location - X_Value);
    LCD_WriteRAMWord(Color);

    LCD_StarterSet(X_Location - Y_Value, Y_Location + X_Value);
    LCD_WriteRAMWord(Color);

    LCD_StarterSet(X_Location - Y_Value, Y_Location - X_Value);
    LCD_WriteRAMWord(Color);

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
  * @brief  Draw a Fill circle.
  * @param  X_Location: X position.
  * @param  Y_Location: Y position.
  * @param  Height: rectangle height.
  * @param  Width: rectangle width.
  * @param  Color: fill color.
  * @retval None
  ***********************************************************************************************************/
void LCD_DrawFillCircle(u16 X_Location, u16 Y_Location, u16 Radius,ColorType Color)
{
 u32 i;
 u32 iMax = ((u32)Radius*707)/1000+1;
 u32 SqMax = (u32)Radius*(u32)Radius+(u32)Radius/2;
 u32 X_Value = Radius;
 
	LCD_DrawLine(X_Location - Radius , Y_Location, 2*Radius, Horizontal,Color);
 
 for (i=1;i<=iMax;i++)
 {
  if ((i*i+X_Value*X_Value)>SqMax)// draw lines from outside 
  {
    if (X_Value>iMax)
   {

		LCD_DrawLine(X_Location - i + 1, Y_Location + X_Value, 2*(i-1), Horizontal,Color);
		LCD_DrawLine(X_Location - i + 1, Y_Location - X_Value, 2*(i-1), Horizontal,Color);
		 
   }
   X_Value--;
  }

  	LCD_DrawLine(X_Location - X_Value, Y_Location + i, 2*X_Value, Horizontal,Color);
		LCD_DrawLine(X_Location - X_Value, Y_Location - i, 2*X_Value, Horizontal,Color);

 }

}

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
    reference_column += 16;
    /* Point to the next character */
    Sptr ++;

    /* Increase the character counter */
    i ++;
  }
}
#endif

/*********************************************************************************************************//**
  * @brief  Draw a picture.
  * @param  Pptr: point to pixel data of picture.
  * @retval None
  ***********************************************************************************************************/
void LCD_PicDraw(u8 X_Location, u16 Y_Location, u16 Height, u16 Width, uc8 *Pptr)
{
}

/*********************************************************************************************************//**
  * @brief  Inserts a delay_us time.
  * @param  nCount: specifies the delay_ms time length.
  * @retval None
  ***********************************************************************************************************/
static void delay_us(u32 nTime)
{
  nTime *= 18;
  while(nTime--);
}

/*********************************************************************************************************//**
  * @brief  Inserts a delay_ms time.
  * @param  nCount: specifies the delay_ms time length.
  * @retval None
  ***********************************************************************************************************/
static void delay_ms(u32 nTime)
{
  nTime *= 18000;
  while(nTime--);
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

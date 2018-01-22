/*********************************************************************************************************//**
 * @file    PDMA/SoftwareTrigger/ht32f12365_66_it.c
 * @version $Rev:: 114          $
 * @date    $Date:: 2016-03-07 #$
 * @brief   This file provides all interrupt service routine.
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
#include "ht32_usbd_core.h"
#include <string.h>

#include "ESP8266.h"
#include "main.h"
#include "queue.h" 

/* Global functions ----------------------------------------------------------------------------------------*/
/*********************************************************************************************************//**
 * @brief   This function handles NMI exception.
 * @retval  None
 ************************************************************************************************************/
void NMI_Handler(void)
{
}

/*********************************************************************************************************//**
 * @brief   This function handles Memory Manage exception.
 * @retval  None
 ************************************************************************************************************/
void MemManage_Handler(void)
{
  while (1);
}

/*********************************************************************************************************//**
 * @brief   This function handles Bus Fault exception.
 * @retval  None
 ************************************************************************************************************/
void BusFault_Handler(void)
{
  while (1);
}

/*********************************************************************************************************//**
 * @brief   This function handles Usage Fault exception.
 * @retval  None
 ************************************************************************************************************/
void UsageFault_Handler(void)
{
  while (1);
}

/*********************************************************************************************************//**
 * @brief   This function handles SVCall exception.
 * @retval  None
 ************************************************************************************************************/
void SVC_Handler(void)
{
}

/*********************************************************************************************************//**
 * @brief   This function handles Debug Monitor exception.
 * @retval  None
 ************************************************************************************************************/
void DebugMon_Handler(void)
{
}

/*********************************************************************************************************//**
 * @brief   This function handles PendSVC exception.
 * @retval  None
 ************************************************************************************************************/
void PendSV_Handler(void)
{
}

/*********************************************************************************************************//**
 * @brief   This function handles SysTick Handler.
 * @retval  None
 ************************************************************************************************************/
void SysTick_Handler(void)
{
  extern void TimingDelay(void);
  TimingDelay();
}

/*********************************************************************************************************//**
 * @brief   This function handles USB interrupt.
 * @retval  None
 ************************************************************************************************************/
void USB_IRQHandler(void)
{
  __ALIGN4 extern USBDCore_TypeDef gUSBCore;
  USBDCore_IRQHandler(&gUSBCore);
}


/*********************************************************************************************************//**
 * @brief   This function handles Hard Fault exception.
 * @retval  None
 ************************************************************************************************************/

//__ASM wait()
//{

//	BX lr //BX

//}



void HardFault_Handler(void)
{
 // printf(" HF");
	
//	
//	wait();
//	
	
  while (1)
	{
	;
	
	}
	
}

/*********************************************************************************************************//**
 * @brief   This function handles PDMA_CH6 interrupt.
 * @retval  None
 ************************************************************************************************************/
void PDMA_CH6_IRQHandler(void)
{
//  if (HT_PDMA->ISR1 & (PDMA_FLAG_TE << ((PDMA_CH6-6)*5)))
//  {
//    printf(" TE6");
//    while (1);
//  }

//  HT_PDMA->ISCR1 = PDMA_FLAG_TC << ((PDMA_CH6-6)*5);
}

/*********************************************************************************************************//**
 * @brief   This function handles PDMA_CH7 interrupt.
 * @retval  None
 ************************************************************************************************************/
void PDMA_CH7_IRQHandler(void)
{
//  if (HT_PDMA->ISR1 & (PDMA_FLAG_TE << ((PDMA_CH7-6)*5)))
//  {
//    printf(" TE7");
//    while (1);
//  }

//  HT_PDMA->ISCR1 = PDMA_FLAG_TC << ((PDMA_CH7-6)*5);
}

/*********************************************************************************************************//**
 * @brief   This function handles EXTI interrupt.
 * @retval  None
 ************************************************************************************************************/

extern u8 KEY_STATE;

//key1
void EXTI0_IRQHandler(void)
{
		EXTI_ClearEdgeFlag(EXTI_CHANNEL_0);
	
		KEY_STATE |= (1<<0);
}
//key2
void EXTI14_IRQHandler(void)
{
		EXTI_ClearEdgeFlag(EXTI_CHANNEL_14);
	
		KEY_STATE |= (1<<1);
}
//key3
void EXTI15_IRQHandler(void)
{
		EXTI_ClearEdgeFlag(EXTI_CHANNEL_15);
	
		KEY_STATE |= (1<<2);
}
//key4
void EXTI8_IRQHandler(void)
{
		EXTI_ClearEdgeFlag(EXTI_CHANNEL_8);
	
		KEY_STATE |= (1<<3);
}





/*********************************************************************************************************//**
 * @brief   This function handles BFTM0 interrupt.
 * @retval  None
 ************************************************************************************************************/

void TIME_SLICE_CHECK(Time_Slice_TypeDef *p,u16 cnt)
{
	if(p->cnt >= cnt) 
	{
		p->cnt  = 0;	
		p->flag = TRUE;
	}
	else p->cnt ++;
	
}



void BFTM0_IRQHandler(void)
{

  BFTM_ClearFlag(HT_BFTM0);

	TIME_SLICE_CHECK(&TimeSlice._10ms,10);
	TIME_SLICE_CHECK(&TimeSlice._20ms,20);
	TIME_SLICE_CHECK(&TimeSlice._100ms,200);
	TIME_SLICE_CHECK(&TimeSlice._500ms,500);
	

}
/*********************************************************************************************************//**
 * @brief   This function handles RTC interrupt.
 * @retval  None
 ************************************************************************************************************/
//extern vu8 FLAG_1S;
void RTC_IRQHandler(void)
{
	
  RTC_GetFlagStatus();
//	FLAG_1S = 1;	
	
	
	if(WIFI_REC.WAIT_EN == SET)
			WIFI_REC.WAIT_1SCNT++;
	else
			WIFI_REC.WAIT_1SCNT = 0;
}


/*********************************************************************************************************//**
 * @brief   This function handles USART0 interrupt.
 * @retval  None
 ************************************************************************************************************/

extern SqQueue WIFI_FRAM;
extern WIFI_REC_TypeDef WIFI_REC;
extern vu8 	URRxFin;

extern char *URTxBuf;
extern vu8 URTxCont;

void USART0_IRQHandler(void)
{
	 //static u8 URTxContBuf = 0;
	char buf; 
	  /* Rx, move data from UART to buffer                                                                */
  if(USART_GetFlagStatus(HT_USART0,USART_FLAG_RXDR))
  {
    USART_ClearFlag(HT_USART0,USART_FLAG_RXDR);
	
		buf = USART_ReceiveData(HT_USART0);
		
		if(buf == '\n')
		{
				WIFI_REC.JUMP = NULL;
				URRxFin++;
		}
		
		if(WIFI_REC.JUMP  ==  NULL)
			EnQueue(&WIFI_FRAM,buf);
  }
	
  /* Tx, move data from buffer to UART                                                                */
  if(USART_GetFlagStatus(HT_USART0,USART_FLAG_TXDE)) 
  {
    if(URTxCont)
    {
      USART_SendData(HT_USART0, *(URTxBuf++));
			URTxCont--;
			//URTxContBuf++;
    }
		else
		{
//			URTxContBuf = 0;
			USART_IntConfig(HT_USART0,USART_INT_TXDE,DISABLE);
		}
		
  }
}

/*********************************************************************************************************//**
 * @brief   This function handles ADC0 interrupt.
 * @retval  None
 ************************************************************************************************************/
extern vu16 ADC_DATA[3];


extern Touch_Screen_Enum Tocuh_Sreen_ADC_CH;

extern vu16 ADC_T;
extern vu8 FLAG_ADC_END;
void ADC_IRQHandler(void)
{
	ADC_ClearIntPendingBit(HT_ADC,ADC_INT_CYCLE_EOC);

	ADC_DATA[0] = ADC_GetConversionData(HT_ADC,ADC_REGULAR_DATA0);
	ADC_DATA[1] = ADC_GetConversionData(HT_ADC,ADC_REGULAR_DATA1);
	ADC_DATA[2] = ADC_GetConversionData(HT_ADC,ADC_REGULAR_DATA2);
	
	
	if(Tocuh_Sreen_ADC_CH == READ_X)
	{
		 ADC_T = ADC_GetConversionData(HT_ADC,ADC_REGULAR_DATA4);	
	}		
	else if(Tocuh_Sreen_ADC_CH == READ_Y)
	{

		 ADC_T = ADC_GetConversionData(HT_ADC,ADC_REGULAR_DATA3);	
	}
	
	FLAG_ADC_END = SET;


}


//void PDMA_CH0_IRQHandler(void)
//{
//  extern vu32 gIsPDMAFinish;
//  PDMA_ClearFlag(PDMA_CH0, PDMA_FLAG_TC);
//  gIsPDMAFinish = TRUE;

//}
//	
//	








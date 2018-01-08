/* Includes ------------------------------------------------------------------------------------------------*/
#include "ZH_GPTM.h"


ZH_GPTM_TypeDef ZH_gGPTM;



/*******************************************************************************
������   ��void ZH_GPTM_Init
����     ��GPTM��ʼ������
������� ����
����ֵ   ����
���ʱ�� ��2016.12.20
����     ��Kaven
�汾     ����һ��
�޸�ʱ�� ��
********************************************************************************/
void ZH_GPTM_Init(void)
{
  CKCU_PeripClockConfig_TypeDef CKCUClock = {{ 0 }};
	TM_TimeBaseInitTypeDef TimeBaseInit;
  TM_OutputInitTypeDef Output_Init;
  
#if (ZH_GPTM_ZNSZ)
  {
    u32 a;
    u16 PinLv,JingQueDu;
    PinLv = ZH_gGPTM.PinLv;
    JingQueDu = ZH_gGPTM.JingQueDu;
    ZH_gGPTM.PZJG.gChar = 0;
    if(PinLv > ZH_gGPTM.XiTongPinLv)
    {
      return;
    }
    a = ZH_gGPTM.XiTongPinLv / PinLv;
    if(a < JingQueDu)
    {
      ZH_gGPTM.JingQueDu = a;
    }
    a = ZH_gGPTM.JingQueDu * ZH_gGPTM.PinLv;
    while((ZH_gGPTM.XiTongPinLv/a)>0xFFFF)
    {
      a += a;
    }
    ZH_gGPTM.pscr = (ZH_gGPTM.XiTongPinLv + a/2) / a - 1;
    a = (ZH_gGPTM.pscr+1) * ZH_gGPTM.PinLv;
    ZH_gGPTM.crr = (ZH_gGPTM.XiTongPinLv + a/2) / a - 1;
    ZH_gGPTM.JingQueDu = ZH_gGPTM.crr + 1;
    if(ZH_gGPTM.ccr == 0)
      ZH_gGPTM.ccr = (ZH_gGPTM.crr+1) / 2;
    ZH_gGPTM.PinLv = ZH_gGPTM.XiTongPinLv / (ZH_gGPTM.pscr+1) / (ZH_gGPTM.crr+1);
    
    if(PinLv == ZH_gGPTM.PinLv)
    {
      ZH_gGPTM.PZJG.Bit.ZH_GPTM_PL = 1;
    }
    if(JingQueDu == ZH_gGPTM.JingQueDu)
    {
      ZH_gGPTM.PZJG.Bit.ZH_GPTM_JQD = 1;
    }
    ZH_gGPTM.PZJG.Bit.ZH_GPTM_OK = 1;
  }
#else
  {
    ZH_gGPTM.pscr = ZH_GPTM_PSCR_PSCV;
    ZH_gGPTM.crr = ZH_GPTM_CRR_CRV;
    ZH_gGPTM.ccr = ZH_GPTM_CCR_CCV;
  }
#endif
  
  CKCUClock.Bit.AFIO          = 1;
  ZH_GPTM_CLK(CKCUClock)      = 1;
  CKCU_PeripClockConfig(CKCUClock, ENABLE);
  
  AFIO_GPxConfig(ZH_GPTM_GPIO_ID, ZH_GPTM_AFIO_PIN, ZH_GPTM_AFIO_MODE);
  
  
  TimeBaseInit.CounterMode 			  = ZH_GPTM_CNTCFR_CMSEL;
  TimeBaseInit.CounterReload 		  = ZH_gGPTM.crr;
  TimeBaseInit.Prescaler 				  = ZH_gGPTM.pscr;
  TimeBaseInit.PSCReloadTime 		  = TM_PSC_RLD_IMMEDIATE;
  TM_TimeBaseInit(ZH_GPTM_N, &TimeBaseInit);
  
  Output_Init.Channel 						= ZH_GPTM_CH;
  Output_Init.Compare 						= 0;
  Output_Init.Control 						= ZH_GPTM_CHCTR;
  Output_Init.OutputMode 					= ZH_GPTM_CHNOCFR_OM;
  Output_Init.Polarity 						= ZH_GPTM_CHPOLR;
  TM_OutputInit(ZH_GPTM_N, &Output_Init);
  
  TM_Cmd(ZH_GPTM_N, DISABLE);
  
}

/*******************************************************************************
������   ��void ZH_GPTM_CCR_Num
����     ���޸�GPTM ��CCR�Ĵ�����ֵ
������� ��Num��CCR��ֵ������ռ�ձ� ��Χ��0~ZH_GPTM_CRR_CRV
����ֵ   ����
���ʱ�� ��2016.12.07
����     ��Kaven
�汾     ����һ��
�޸�ʱ�� ��
********************************************************************************/
void ZH_GPTM_CCR_Num(u16 Num)
{
  if(Num<ZH_gGPTM.crr)
    ZH_GPTM_CCR = Num;
  else
    ZH_GPTM_CCR = ZH_gGPTM.crr;
  
  ZH_gGPTM.ccr = ZH_GPTM_CCR;
}

/*******************************************************************************
������   ��void ZH_GPTM_CRR_Num
����     ���޸�GPTM ��CRR�Ĵ�����ֵ
������� ��Num��CRR��ֵ
����ֵ   ����
���ʱ�� ��2016.12.07
����     ��Kaven
�汾     ����һ��
�޸�ʱ�� ��
********************************************************************************/
void ZH_GPTM_CRR_Num(u16 Num)
{
  ZH_GPTM_N->CRR = Num;
  ZH_gGPTM.crr = Num;
}

/*******************************************************************************
������   ��void ZH_GPTM_Switch
����     ��GPTM ��ʱ��ʹ�ܿ���
������� ��NewState�� ENABLE or DISABLE.
����ֵ   ����
���ʱ�� ��2016.12.07
����     ��Kaven
�汾     ����һ��
�޸�ʱ�� ��
********************************************************************************/
void ZH_GPTM_Switch(ControlStatus NewState)
{
  TM_Cmd(ZH_GPTM_N, NewState);
}

/*******************************************************************************
������   ��void ZH_GPTMCH_Switch
����     ��GPTM ��ʱ��ͨ��ʹ�ܿ���
������� ��NewState�� ENABLE or DISABLE.
����ֵ   ����
���ʱ�� ��2016.12.07
����     ��Kaven
�汾     ����һ��
�޸�ʱ�� ��
********************************************************************************/
void ZH_GPTMCH_Switch(ControlStatus NewState)
{
  ZH_GPTM_N->CHCTR &= ~(u32)(0x1 << (ZH_GPTM_CH << 1));
  ZH_GPTM_N->CHCTR |= NewState<<(ZH_GPTM_CH<<1);
}












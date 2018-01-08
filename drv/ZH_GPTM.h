#include "ht32.h"

#ifndef __ZH_GPTM_H
#define __ZH_GPTM_H


#define ZH_GPTM_CLK(CK)                 (CK.Bit.GPTM1)
#define ZH_GPTM_N                       HT_GPTM1
#define ZH_GPTM_CH                      TM_CH_3
#define ZH_GPTM_CCR                     ZH_GPTM_N->CH3CCR
#define ZH_GPTM_GPIO_ID                 GPIO_PC
#define ZH_GPTM_AFIO_PIN                AFIO_PIN_3
#define ZH_GPTM_AFIO_MODE               AFIO_MODE_4

#define ZH_GPTM_CNTCFR_CMSEL            TM_CNT_MODE_UP
#define ZH_GPTM_CHPOLR                  TM_CHP_NONINVERTED 
#define ZH_GPTM_CHNOCFR_OM              TM_OM_PWM1
#define ZH_GPTM_CHCTR                   TM_CHCTL_ENABLE


#define ZH_GPTM_ZNSZ                    1                       // PWM���������Ƿ�����

#if (ZH_GPTM_ZNSZ)

typedef union
{
  struct
  {
    unsigned char ZH_GPTM_OK          :1;                       // �Ƿ����óɹ���1���ɹ�����PWM   0��δ�ɹ�����PWM
    unsigned char ZH_GPTM_PL          :1;                       // Ƶ���Ƿ�ﵽҪ��1����   0����
    unsigned char ZH_GPTM_JQD         :1;                       // ռ�ձȾ�ȷ���Ƿ�ﵽҪ��1����   0����
  } Bit;
  u8 gChar;
} ZH_GPTM_JG_TypeDef;
typedef struct 
{
  u16 PinLv;                            // ��������PWMƵ��
  u16 JingQueDu;                        // ��������PWMռ�ձ���С��ȷ��   ֵ��100��ռ�ձȿɾ�ȷ��1%
  u32 XiTongPinLv;                      // ��ǰϵͳƵ��
  u16 pscr;
  u16 crr;
  u16 ccr;
  ZH_GPTM_JG_TypeDef PZJG;              // PWM ���ý��
} ZH_GPTM_TypeDef;
extern ZH_GPTM_TypeDef ZH_gGPTM;

#else

#define ZH_GPTM_32KCRR                  8                       // �����м���
#define ZH_GPTM_8MCRR                   2000
#define ZH_GPTM_48MCRR                  12000

#define ZH_GPTM_PSCR_PSCV               0
#define ZH_GPTM_CRR_CRV                 12000
#define ZH_GPTM_CCR_CCV                 6000 
typedef struct 
{
  u16 pscr;
  u16 crr;
  u16 ccr;
} ZH_GPTM_TypeDef;
extern ZH_GPTM_TypeDef ZH_gGPTM;

#endif


void ZH_GPTM_Init(void);
void ZH_GPTM_CCR_Num(u16 Num);
void ZH_GPTM_CRR_Num(u16 Num);
void ZH_GPTM_Switch(ControlStatus NewState);
void ZH_GPTMCH_Switch(ControlStatus NewState);


#endif


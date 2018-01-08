#ifndef __ESP8266_H_
#define __ESP8266_H_

#include "ht32.h"
#include "stdlib.h"
#include "queue.h" 

#ifdef __cplusplus
 extern "C" {
#endif


//USE PC12
#define WIFI_RST_PORT   HT_GPIOC
#define WIFI_RST_PIN    GPIO_PIN_12


#define USE_WIFI_AUTOLINK
//#define USE_WIFI_DISPLAY

typedef enum 
{
    STA = 1,
    AP,
    STA_AP,
	
}WIFI_MODE_TypeDef;

//����ģ�K�Y���w
typedef struct 
{
	char *CMD; //�l������
	char *ACK; //�ȴ�����
	
	u8 WAIT_EN;  //�Ƿ񆢄ӵȴ�
	u8 WAIT_MAXTIME; //�ȴ����L�r�gnS
	u8 WAIT_1SCNT;	//�ȴ�Ӌ��
	
	//����ģ�K��ǰ��B
	enum 
	{
		CENABLE,
		CDISABLE,
		CSUCCESS,
		CTIME_OUT,
	}STA;	
	
	//����ģ�K��ʽ
	enum 
	{
		FREE = 0X00,
		SEND = 0x01,
		REC = 0x02,
		SENDREC = SEND|REC,
	}MODE;

	vu8 JUMP;	//���^�y�a
	
}WIFI_REC_TypeDef ;

//�l��ģ�K���̱�
typedef enum 
{
	L_READY,
	L_CHECK_LINK_1,
	L_CHECK_LINK_2,
	L_REST,
	L_5,
	L_6,
	L_7,
	L_8,
	L_9,
	L_10,
	L_LINKED,
	L_12,
	L_13,
	L_14,
	L_15,
	L_Dev,
	L_REC,
	L_SEND_1,
	L_SEND_2,
	L_SEND_3,
	L_NULL,
	CAP_STATUE = 100,
  CAP_DATA = 101,
}WIFI_CMD_CONT_TypeDef;


//URATʹ��
extern vu8  URRxFin ;
extern char *URTxBuf;
extern vu8 URTxCont ;

#define REC_BUF_SIZE		64  //�����������L�ȣ�30~64��
extern SqQueue WIFI_FRAM;   //���Ք������

extern WIFI_REC_TypeDef WIFI_REC;//����ģģ�K׃����ӳ���B
extern WIFI_CMD_CONT_TypeDef CMD_Cont,CMD_Cont_Trg;	 //�l��ģ�K�����Ŀ


//extern u8 FLAG_CMDSET ;//ATָ��ؑ�
extern u8 FALG_WIFI_AUTOLINK;//WIFI�Ƿ񆢄��Ԅ��B��
extern u8 FALG_WIFI_LINK ;//WIFI�Ƿ��ѽ��B����·��
extern u8 FALG_WIFI_DEVLINK;//WIFI�Ƿ����O���B��
extern u8 FLAG_WIFI_UPDATA;//WIFI�Ƿ��н��յ��O����Ϣ

void WIFI_INIT(void);//WIFI��ʼ��
void WIFI_HW_STA(FlagStatus sta);//WIFI Ӳ����λ
void WIFI_SendCMD(char *Str);//�l��ָ��


void WIFI_CMDSET(char *cmd,char *ack ,u16 waittime);//�O�ðl��ָ��؏���Ϣ���ȴ��؏͕r�g
void WIFI_LIST_POINT(WIFI_CMD_CONT_TypeDef type);//�O���������D������ģ�K�����
void WIFI_READY(void);
void WIFI_AUTOLINK(void);

extern char WIFI_IP[14];//WIFI IP��ַ
extern char WIFI_MAC[18];//WIFI MAC��ַ
u8 WIFI_GotIP(char *mac,char *ip);//�@ȡWIFI MAC IP ��ַ

void WIFI_Control(void);//WIFI�l��ģ�K��300ms~1s��

void WIFI_CAP_STATUE(void);//���Ն�һ�؏�
void WIFI_CAP_DATA(void);//���Ք����؏�
void WIFI_CAP_CONNECT(void);//�z�y�O���Ƿ���_

void WIFI_CAP(void);//WIFI����ģ�K��<20ms��

#ifdef __cplusplus
}
#endif


#endif


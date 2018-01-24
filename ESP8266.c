#include "ESP8266.h"
#include <string.h>
#include "main.h"

void Clear_Buf(char *str,u8 size)
{
	size++;
	do
	{
		*str = 0;
		size--;
		str++;
	}while(size);
	
}



char URRxBuf[REC_BUF_SIZE];
SqQueue WIFI_FRAM;
WIFI_UART_TypeDef WIFI_UART;


void USART_Configuration(void)
{
  USART_InitTypeDef USART_InitStructure;
	  /*
        USARTx configured as follow:
            - BaudRate = 115200 baud
            - Word Length = 8 Bits
            - One Stop Bit
            - None parity bit
  */
	USART_DeInit(HT_USART0);
  /* Configure PA2, PA3 pin: TX, RX pin */
  AFIO_GPxConfig(GPIO_PA, AFIO_PIN_3, AFIO_MODE_6);
  AFIO_GPxConfig(GPIO_PA, AFIO_PIN_2, AFIO_MODE_6);

  USART_InitStructure.USART_BaudRate = 115200;
  USART_InitStructure.USART_WordLength = USART_WORDLENGTH_8B;
  USART_InitStructure.USART_StopBits = USART_STOPBITS_1;
  USART_InitStructure.USART_Parity = USART_PARITY_NO;
  USART_InitStructure.USART_Mode = USART_MODE_NORMAL;
  USART_Init(HT_USART0, &USART_InitStructure);                                     
  USART_IntConfig(HT_USART0, USART_INT_RXDR, ENABLE);

//	USART_FIFOReset(HT_USART0,USART_FIFO_RX);

//  USART_IntConfig(HT_USART0, USART_INT_TOUT, ENABLE);
  /* Seting Rx FIFO Level                                                                                   */ 
//  USART_RXTLConfig(HT_USART0, USART_RXTL_04);

  USART_IntConfig(HT_USART0, USART_INT_RXDR , ENABLE);	
	USART_RxCmd(HT_USART0, ENABLE);
	USART_TxCmd(HT_USART0, ENABLE);
	
	NVIC_EnableIRQ(USART0_IRQn);

}

void WIFI_GPIO_Configuration(void) //RESET PIN
{
  /* Configure the GPIO pin                                                                                 */
  GPIO_PullResistorConfig(WIFI_RST_PORT, WIFI_RST_PIN, GPIO_PR_DISABLE);
  GPIO_DriveConfig(WIFI_RST_PORT, WIFI_RST_PIN, GPIO_DV_8MA);
  GPIO_DirectionConfig(WIFI_RST_PORT, WIFI_RST_PIN, GPIO_DIR_OUT);
	GPIO_WriteOutBits(WIFI_RST_PORT, WIFI_RST_PIN, RESET); 

}

void WIFI_HW_STA(FlagStatus sta) //0:reset 1:work
{
	GPIO_WriteOutBits(WIFI_RST_PORT, WIFI_RST_PIN, sta); 
}


void WIFI_SendCMD(char *Str)
{
		WIFI_UART.TxCont = strlen((const char*)Str) ;  
		WIFI_UART.TxBuf = Str;
		USART_IntConfig(HT_USART0, USART_INT_TXDE, ENABLE);
}

void WIFI_INIT(void)
{
	USART_Configuration();
	WIFI_GPIO_Configuration();
	WIFI_HW_STA(RESET);	
	InitQueue(&WIFI_FRAM,REC_BUF_SIZE,URRxBuf);
	
	#ifdef USE_WIFI_AUTOLINK
		FLAG_WIFI.AUTOLINK = TRUE;
	#else 
		FLAG_WIFI.AUTOLINK = FALSE;
	#endif
	
	
}

///////////////////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
WIFI_REC_TypeDef WIFI_REC; //����ģ�K
WIFI_CMD_CONT_TypeDef CMD_Cont;
WIFI_CMD_CONT_TypeDef CMD_Cont_Trg;	 //����E
WIFI_FLAG_TypeDef FLAG_WIFI;
////////////////////////////////////////////////////////////////////////////////////////////

void WIFI_WAIT_FUNC(void)
{
		if(WIFI_REC.WAIT_EN == TRUE)
			WIFI_REC.WAIT_1SCNT++;
		else
			WIFI_REC.WAIT_1SCNT = 0;
}
/*******************************************************************************
������   ��void WIFI_CMDSET(char *cmd,char *ack ,u16 waittime)
����     ��WIFI ATָ��l�ͣ���Ͻ���ģ�Kʹ�ã�
������� ��ATָ�ATָ��؏ͣ��ȴ��r�gx1s����ȴ��r�g���㣬�t��Ӌ�r��
����ֵ   ����
���]     ����
********************************************************************************/
void WIFI_CMDSET(char *cmd,char *ack ,u16 waittime)
{
		WIFI_REC.ACK = ack;
		WIFI_REC.CMD = cmd;
		WIFI_REC.MODE = FREE;
	
		if(waittime != 0 ) 
		{
			 WIFI_REC.WAIT_MAXTIME = waittime;
			 WIFI_REC.WAIT_1SCNT = 0;
			 WIFI_REC.WAIT_EN = TRUE; //�ȴ��؏�ģʽ
		}	
		else 
		{
			 WIFI_REC.WAIT_MAXTIME = 0xFF;
			 WIFI_REC.WAIT_1SCNT = 0; 
			 WIFI_REC.WAIT_EN = FALSE;
		}
		
		
		if(strstr((const char*)ack,"ready"))
		{
				WIFI_REC.JUMP = TRUE;
		}
	
		if(WIFI_REC.CMD != NULL) 
		{
//				WIFI_DISPLAY(WIFI_REC.CMD);
				WIFI_REC.STA = CENABLE; 			
				WIFI_REC.MODE |= SEND;
				WIFI_UART.TxCont = strlen((const char*)cmd);  
				WIFI_UART.TxBuf = cmd;	
				USART_IntConfig(HT_USART0, USART_INT_TXDE, ENABLE);
		}
		
		if(WIFI_REC.ACK != NULL) 
		{
				WIFI_REC.STA = CENABLE; 
				WIFI_REC.MODE |= REC;
		}

}

/*******************************************************************************
������   ��void WIFI_LIST_POINT(WIFI_CMD_CONT type)
����     ��ATָ��l��ģ�K���x�����D������ģ�K����ͣ���Ͻ���ģ�Kʹ�ã�
������� ������ģ�K����ͣ� CAP_STATUE��CAP_DATA
����ֵ   ����
���]     ����
********************************************************************************/
void WIFI_LIST_POINT(WIFI_CMD_CONT_TypeDef type)
{
	if(WIFI_REC.MODE == SEND )
	{
			CMD_Cont++;
	}	
	if(WIFI_REC.MODE & REC )
	{
			CMD_Cont_Trg = CMD_Cont; 
			CMD_Cont = type; 
	}
}




/*******************************************************************************
������   ��void WIFI_CAP_STATUE(void)
����     ������ģ�K��ͣ�����ATָ��μ��Ļ؏ͣ���Ͻ���ģ�Kʹ�ã�
������� ������ģ�K����ͣ� CAP_STATUE��CAP_DATA
����ֵ   ����
���]     ����
********************************************************************************/
void WIFI_CAP_STATUE(void)//WIFI���ջؑ���B
{
	char *p;
	
	p = StrQueue(&WIFI_FRAM,WIFI_REC.ACK);
		
	if(p != NULL) // �ҵ��˻ؑ�
	{
			CMD_Cont_Trg++;
			CMD_Cont = CMD_Cont_Trg;
			WIFI_REC.STA = CSUCCESS;
	}
}

/*******************************************************************************
������   ��void WIFI_CAP_CONNECT(void)
����     ���z���O���Ƿ���_����Ͻ���ģ�Kʹ�ã�
������� ���o
����ֵ   ����
���]     ����o�z�y���O����_�t�{���z���O���B�Ӳ��E
********************************************************************************/
void WIFI_CAP_CONNECT(void)
{
		if(strstr(&WIFI_FRAM.pbase[WIFI_FRAM.front],"0,CLOSE"))
		{
			
			FLAG_WIFI.DEVLINK = FALSE;
			CMD_Cont_Trg = L_Dev ;//���D���z�y�O���Ƿ����B��
			CMD_Cont = CMD_Cont_Trg;
			WIFI_REC.STA = CDISABLE;
			
			#ifdef USE_WIFI_LIST_DISPLAY
				WIFI_DISPLAY("Dev_Close\r\n\0");
			#endif

		}
}

/*******************************************************************************
������   ��void WIFI_CAP_DATA(void)
����     ������ģ�K��ͣ����Ք�������Ͻ���ģ�Kʹ�ã�
������� ���o
����ֵ   ����
���]     ��������ݔ��WIFI�f�h����
********************************************************************************/
void WIFI_CAP_DATA(void)//����͸����Ϣ
{	
	char *p = NULL;
	u8 len;
	
	p = StrQueue(&WIFI_FRAM,WIFI_REC.ACK);
	
		if(p != NULL)
		{

			if(*(p++) == '0')//+IPD,0,12:XXXXXX
			{
				DeQueue(&WIFI_FRAM,&p);//0
				DeQueue(&WIFI_FRAM,&p);//,
				DeQueue(&WIFI_FRAM,&p);//1
				len = *p -'0';
				DeQueue(&WIFI_FRAM,&p);//2
				
				if(*p != ':')//:
				{
					len = len * 10;
					len += *p -'0';
					DeQueue(&WIFI_FRAM,&p);//:
				}
				//cpy �f�h����
				CpyQueue(&WIFI_FRAM,(char*)Thermostat_DATA,len);		
				FLAG_WIFI.UPDATA = TRUE;				
				CMD_Cont_Trg++;
				CMD_Cont = CMD_Cont_Trg;
				WIFI_REC.STA = CDISABLE;
				
			}
		}
}



/*******************************************************************************
������   ��void WIFI_READY(void)
����     ���z��WIFIģ�K��λ�Ƿ�ɹ�����ϰl��ģ�Kʹ�ã�
������� ���o
����ֵ   ����
���]     ���o
********************************************************************************/
void WIFI_READY(void)
{
		WIFI_HW_STA(RESET);		
		WIFI_CMDSET(NULL,"ready" ,10);		
	  WIFI_HW_STA(SET);		
}

/*******************************************************************************
������   ��void WIFI_AUTOLINK(void)
����     ���ȴ�WIFI�Ԅ��B��·�ɹ��ܣ���ϰl��ģ�Kʹ�ã�
������� ���o
����ֵ   ����
���]     ��FLAG_WIFI.AUTOLINK �����Ƿ�ʹ�� WIFI�Ԅ��B��·�ɹ��ܣ�
						���ʧ�ܻ�ȴ����r���t���D���֏ͳ��S�O��ָ�
********************************************************************************/
void WIFI_AUTOLINK(void)
{
	if(FLAG_WIFI.AUTOLINK == TRUE)
	{
		
		if(WIFI_REC.STA != CTIME_OUT)
		{
			WIFI_CMDSET(NULL,"WIFI GOT IP" ,20);			
			CMD_Cont_Trg = CMD_Cont; 
			CMD_Cont = CAP_STATUE; 
		}		 	
		else
		{
			CMD_Cont = L_REST;
			FLAG_WIFI.AUTOLINK = FALSE;
		}
		
	}
	else 
	{	
		//jump
	  	CMD_Cont = L_REST;
	}

}

	
/*******************************************************************************
������   ��u8 WIFI_GotIP(char *mac,char *ip)
����     ���xȡ��ǰWIFIģ�K��IP&MAC����ϰl��ģ�Kʹ�ã�
������� ��MAC�惦���Mָᘣ�IP�惦���Mָ�
����ֵ   ����ԃ�ɹ�:TRUE;��ԃʧ����FALSE
���]     :

AT+CIFSR
+CIFSR:STAIP,"192.168.31.14"
+CIFSR:STAMAC,"2c:3a:e8:00:dd:a9"
OK

cost : 42ms
********************************************************************************/
char WIFI_IP[14];
char WIFI_MAC[18];
bool WIFI_GotIP(char *mac,char *ip)
{	
	char *p = NULL;
	WIFI_UART.RxFin = 0;

	WIFI_SendCMD("AT+CIFSR\r\n");	
	
	while(!WIFI_UART.RxFin);
	WIFI_UART.RxFin = 0;
	
	p = StrQueue(&WIFI_FRAM,"+CIFSR:STAIP,");
	if(p != NULL)
	{
		DeQueue(&WIFI_FRAM,&p);
		CpyQueue(&WIFI_FRAM,ip,13);
		
		*(ip+13)='\0';
		
	#ifdef USE_WIFI_LIST_DISPLAY
		WIFI_DISPLAY("WIFI_INF_��\0");	
		WIFI_DISPLAY(ip);	
	#endif
		
	}
	else return FALSE;
	
	while(!WIFI_UART.RxFin);
	p = StrQueue(&WIFI_FRAM,"+CIFSR:STAMAC,");
	if(p != NULL)
	{
		DeQueue(&WIFI_FRAM,&p);
		CpyQueue(&WIFI_FRAM,mac,17);
		*(mac+18)='\0';
		
	#ifdef USE_WIFI_LIST_DISPLAY
		WIFI_DISPLAY(mac);	
	#endif
	}
	else return FALSE;

	return TRUE;
}



/*******************************************************************************
������   ��void WIFI_Control(void)
����     ��WIFI �l��ģ�K��WIFI�������̲��E����Ҫ����WIFI����ģ�Kʹ��
������� ���o
����ֵ   ���o
���]     : ��WIFI_CMD_CONT_TypeDef�Ќ������p����
********************************************************************************/
void WIFI_Control(void)
{
	
	switch(CMD_Cont)
	{

		///////////////////////////////
		case L_READY 	:					WIFI_READY();										WIFI_LIST_POINT(CAP_STATUE);;break;			  	
		case L_CHECK_LINK_1	:		WIFI_AUTOLINK();			;break;
		case L_CHECK_LINK_2	:		CMD_Cont = L_LINKED; 	;break;			
		
		///////////////////////////////		
		case L_REST	:	WIFI_CMDSET("AT+RESTORE\r\n","ready",5);	WIFI_LIST_POINT(CAP_STATUE);;break;
		case L_5	:		WIFI_CMDSET("ATE0\r\n","OK" ,5);					WIFI_LIST_POINT(CAP_STATUE);;break;
		case L_6	:		WIFI_CMDSET("AT+CWMODE=1\r\n","OK",5);	  WIFI_LIST_POINT(CAP_STATUE);;break;
		case L_7	:		WIFI_CMDSET("AT+RST\r\n","ready" ,5);			WIFI_LIST_POINT(CAP_STATUE);;break;
		case L_8	:		WIFI_CMDSET("ATE0\r\n","OK" ,5);					WIFI_LIST_POINT(CAP_STATUE);;break;
		
		case L_SMART :	FLAG_WIFI.SMARTLINK = TRUE;
										WIFI_CMDSET("AT+CWSTARTSMART=3\r\n","smartconfig connected wifi",0);
										WIFI_LIST_POINT(CAP_STATUE);
										;break;
		
		case L_10	:		FLAG_WIFI.SMARTLINK = FALSE;	
									WIFI_CMDSET("AT+CWSTOPSMART\r\n","OK" ,5);
									WIFI_LIST_POINT(CAP_STATUE);
									;break;
		
		///////////////////////////////		
		case L_LINKED	:		FLAG_WIFI.APLINK = TRUE; 
										  WIFI_CMDSET("ATE0\r\n","OK",2);	
									   	WIFI_LIST_POINT(CAP_STATUE);
											;break;
											
		case L_12	:		if(WIFI_GotIP(WIFI_MAC,WIFI_IP) == TRUE) CMD_Cont++;break;
		case L_13	:		CMD_Cont++;break;
		case L_14	:		WIFI_CMDSET("AT+CIPMUX=1\r\n","OK",5);		     WIFI_LIST_POINT(CAP_STATUE);;break;
		case L_15	:		WIFI_CMDSET("AT+CIPSERVER=1,1001\r\n","OK",5); WIFI_LIST_POINT(CAP_STATUE);;break;
		case L_Dev	:	WIFI_CMDSET(NULL,"0,CONNECT" ,0);					     WIFI_LIST_POINT(CAP_STATUE);;break;
		
		case L_REC	:	FLAG_WIFI.DEVLINK = TRUE;	
									Clear_Buf(WIFI_FRAM.pbase,REC_BUF_SIZE);
									WIFI_CMDSET(NULL,"+IPD,",0);			
									WIFI_LIST_POINT(CAP_DATA);
									;break;
		
		case L_SEND_1	:	WIFI_CMDSET("AT+CIPSEND=0,12\r\n",">",5);    WIFI_LIST_POINT(CAP_STATUE);break;
		case L_SEND_2	:	WIFI_CMDSET((char*)Thermostat_DATA,"OK",5);  WIFI_LIST_POINT(CAP_STATUE);break;
		case L_SEND_3	: CMD_Cont = L_REC; break;
		case L_NULL	:  ;break;
		default : ;break;
	}		
}

/*******************************************************************************
������   ��void WIFI_CAP(void)
����     ��WIFI ����ģ�K��WIFI�l��ģ�K�l��ָ��󣬵ȴ�WIFI�Ļؑ���
������� ���o
����ֵ   ���o
���]     : WIFI_REC_TypeDef WIFI_REC;//����ģģ�K׃����ӳ���B
********************************************************************************/
void WIFI_CAP(void)
{
	if(WIFI_REC.STA == CENABLE )
	{
//////////////////////////////////////////////////////////////////////////////			
		if(FLAG_WIFI.DEVLINK == TRUE ) 
		{
				WIFI_CAP_CONNECT();
		}
			 		
		switch(CMD_Cont)
		{
				case CAP_STATUE : WIFI_CAP_STATUE();break;		//���ՠ�B
				case CAP_DATA 	:	WIFI_CAP_DATA(); ;break;//����͸����Ϣ
				default : ;break;
		}

//////////////////////////////////////////////////////////////////////////////				
		if(WIFI_REC.STA == CSUCCESS )
		{
				WIFI_REC.WAIT_EN = FALSE;
			
			#ifdef USE_WIFI_LIST_DISPLAY
			if(WIFI_REC.CMD!=NULL)
				WIFI_DISPLAY(WIFI_REC.CMD);	
			
			WIFI_DISPLAY(WIFI_REC.ACK);	
			#endif
		}		
		
		
/////////////////////////////////////////////////////////////////////////////////////////			
		if(WIFI_REC.WAIT_EN == TRUE) //���ӵȴ�
		{
			if(WIFI_REC.WAIT_1SCNT >= WIFI_REC.WAIT_MAXTIME )//���r�e�`
			{
				WIFI_REC.WAIT_EN = FALSE;
				WIFI_REC.STA = CTIME_OUT;
				CMD_Cont = CMD_Cont_Trg;
				WIFI_UART.RxFin = 0;
			}
		}		
		
		
/////////////////////////////////////////////////////////////////////////////////////////			
	 }	

}




#ifndef __NETWORK_H
#define __NETWORK_H

#include "sys.h"
#include "usart.h"
#include "delay.h"



/*���ӷ������궨��*/
#define TCP "TCP"
#define UDP "UDP"
#define IP  "103.46.128.43"//������ IP	caisiyu.wicp.vip:56695
#define PORT 56695
//#define IP  "103.46.128.43"//������ IP	2u7919q843.zicp.vip:11967
//#define PORT 11967
//#define IP  "10.101.77.111"//���� ����������
//#define PORT 8888


/*���ͽ��ջ��������Ⱥ궨��*/
#define TXBUFFER_LEN 50
#define RXBUFFER_LEN 30

u8 initGPRS(void);
u8 Set_Text_Mode(void);
u8 Send_Text_Sms(char *str);

u8 findStr(u8* dest,u8* src,u16 retry_cn);

#endif

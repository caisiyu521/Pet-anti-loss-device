#ifndef __Config_H
#define __Config_H

#include "nvic.h"
#include "usart.h"
#include "timer.h"
#include "led.h"
#include "delay.h"
#include "key.h"
#include "dht11.h"
#include "usart1.h"	   
#include "gps.h"
#include "gprs.h"
#include "mpu6050.h"
#include "inv_mpu.h"
#include "inv_mpu_dmp_motion_driver.h" 
#include "dmp_int_read.h"

uchar ret = 0;

///////////////////////////////////////////////////////////
uchar Dog_Status = 0;

/*					������� gps�� ����								*/				
nmea_msg gpsx; 											//GPS��Ϣ
__align(4) u8 dtbuf[50];   								//��ӡ������
const u8*fixmode_tbl[4]={"Fail","Fail"," 2D "," 3D "};	//fix mode�ַ��� 
float tp;												//���� �洢gps�������ݵ� ����
u8 TEMP_BUF[USART1_MAX_RECV_LEN];						//���� GPS ���ڷ��ص�����

/*					������� DHT11 ����								*/	
uint8_t temp1 , shi;

/*					������� GPS ����								*/	
//*****************************************
	float temp;
	float pitch,roll,yaw; 		//ŷ����
	uchar res;
	unsigned long step_count;
	short aacx,aacy,aacz;		//���ٶȴ�����ԭʼ����
	short gyrox,gyroy,gyroz;	//������ԭʼ����
/*****************************************
*/

#endif

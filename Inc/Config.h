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

/*					定义关于 gps的 变量								*/				
nmea_msg gpsx; 											//GPS信息
__align(4) u8 dtbuf[50];   								//打印缓存器
const u8*fixmode_tbl[4]={"Fail","Fail"," 2D "," 3D "};	//fix mode字符串 
float tp;												//用来 存储gps返回数据的 变量
u8 TEMP_BUF[USART1_MAX_RECV_LEN];						//接受 GPS 串口返回的数据

/*					定义关于 DHT11 变量								*/	
uint8_t temp1 , shi;

/*					定义关于 GPS 变量								*/	
//*****************************************
	float temp;
	float pitch,roll,yaw; 		//欧拉角
	uchar res;
	unsigned long step_count;
	short aacx,aacy,aacz;		//加速度传感器原始数据
	short gyrox,gyroy,gyroz;	//陀螺仪原始数据
/*****************************************
*/

#endif

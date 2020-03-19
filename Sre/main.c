#include "sys.h"
#include "Config.h"

uint8_t string[20];			//用来格式化字符串	存储数组
extern uchar  GPRS_Delay_1S ;

void Gps_Msg_Show(void);	//显示GPS定位信息 
void GPS_INIT(void);		//GPS 模块初始化
void GPS_SHUJU(void);		//GPS 模块数据解析
  			
int main(void){
	
	delay_init();			//delay 初始化
	uart2_init(115200);		//串口2 初始化
	uart3_init(9600);		//串口3 初始化		GPRS模块 的波特率是9600
	Timer2_Init(1000,72);	//初始化 Timer2 1ms 中断一次
//	LED_Init();
				
	DHT11Configuration();	//DHT11 温湿度模块

	NVIC_Config();			//配置 中断的 优先级 ， 放初始化 后面
	
	GPS_INIT();				//GPS初始化

//*****************************************	
//	if(MPU_Init() == 0)
//		printf("MPU ok!\r\n");
//	while(mpu_dmp_init() )
//		printf("dmp error!\r\n");

//*****************************************

	printf("CPU Start! \r\n");
	
	ret = initGPRS();								//查询等待模块注册成功
	if(ret)	ret =  Set_Text_Mode();					//接入服务器
//	if(ret)	ret =  Send_Text_Sms("caisiyu");					//发送一条短消息
	if(ret)	printf("GPRS 模块初始化成功! \r\n");
	else 	printf("GPRS 模块初始化失败! \r\n");

	
	GPRS_Delay_1S = 1;while(GPRS_Delay_1S);				//精确延时1S
	GPRS_Delay_1S = 1;while(GPRS_Delay_1S);	
	
	while(1){
		DHT11_Read_Data(&temp1,&shi);				//获取 DHT11 返回的 温度 湿度
		printf("temp = %d ,shi = %d\r\n",temp1,shi);
////		
		GPS_SHUJU();
		delay_ms(100);
//****************************************************************************
		if(mpu_dmp_get_data(&pitch,&roll,&yaw)==0){ 
			temp=MPU_Get_Temperature();	//得到温度值
//			MPU_Get_Accelerometer(&aacx,&aacy,&aacz);	//得到加速度传感器数据
//			MPU_Get_Gyroscope(&gyrox,&gyroy,&gyroz);	//得到陀螺仪数据
//			printf("aacx  = %d ,aacy  = %d ,aacz  = %d\r\n",aacx,aacy,aacz);//原始三轴值
//			printf("gyrox = %d ,gyroy = %d ,gyroz = %d\r\n",gyrox,gyroy,gyroz);//原始加速度值
			printf("temp = %f , pitch = %f ,roll  = %f ,yaw   = %f\r\n",temp,pitch,roll,yaw);//DMP处理后的 pitch ， roll，yaw值
		}
		res=dmp_get_pedometer_step_count(&step_count); 		
		if(!res)
			printf ("count = %ld\r\n",step_count);
	}
}



void GPS_SHUJU(void){
	u16 rxlen , lenx ,i;
	if(USART1_RX_STA&0x8000){					   
		rxlen=USART1_RX_STA&0X7FFF;	//得到数据长度
		for(i=0;i<rxlen;i++)TEMP_BUF[i]=USART1_RX_BUF[i];	   
		USART1_RX_STA=0;		   	//启动下一次接收
		TEMP_BUF[i]=0;				//自动添加结束符
		GPS_Analysis(&gpsx,(u8*)TEMP_BUF);//分析字符串
		Gps_Msg_Show();
	}
		lenx++;	
}

void GPS_INIT(void){
	u8 key = 1;
	if(Ublox_Cfg_Rate(1000,1)!=0)	//设置定位信息更新速度为1000ms,顺便判断GPS模块是否在位. 
	{
		printf("NEO-6M Setting...\r\n");
		while((Ublox_Cfg_Rate(1000,1)!=0)&&key)	//持续判断,直到可以检查到NEO-6M,且数据保存成功
		{
			USART1_Init(9600);				//初始化串口1波特率为9600(EEPROM没有保存数据的时候,波特率为9600.)
	  		Ublox_Cfg_Prt(38400);			//重新设置模块的波特率为38400
			USART1_Init(38400);				//初始化串口1波特率为38400 
			Ublox_Cfg_Tp(1000000,100000,1);	//设置PPS为1秒钟输出1次,脉冲宽度为100ms	    
			key=Ublox_Cfg_Cfg_Save();		//保存配置  
		}	  					 
		printf("NEO-6M Set Done!\r\n");
		delay_ms(500);
	}
}

//显示GPS定位信息 
void Gps_Msg_Show(void)
{
 	float tp;		
	printf("\r\n\r\n\n\n\n\n");    
	tp=gpsx.longitude;	   
	sprintf((char *)dtbuf,"Longitude:%.5f %1c   \r\n",tp/=100000,gpsx.ewhemi);	//得到经度字符串
	printf("%s	\r\n",dtbuf);
	tp=gpsx.latitude;	   
	sprintf((char *)dtbuf,"Latitude:%.5f %1c   ",tp/=100000,gpsx.nshemi);	//得到纬度字符串
	printf("%s	\r\n",dtbuf);
	tp=gpsx.altitude;	   
 	sprintf((char *)dtbuf,"Altitude:%.1fm     ",tp/=10);	    			//得到高度字符串
	printf("%s	\r\n",dtbuf);
	tp=gpsx.speed;	   
 	sprintf((char *)dtbuf,"Speed:%.3fkm/h     ",tp/=1000);		    		//得到速度字符串	 
	printf("%s	\r\n",dtbuf);
	if(gpsx.fixmode<=3)														//定位状态
	{  
		sprintf((char *)dtbuf,"Fix Mode:%s",fixmode_tbl[gpsx.fixmode]);	
		printf("%s	\r\n",dtbuf);
	}	 	   
	sprintf((char *)dtbuf,"Valid satellite:%02d",gpsx.posslnum);	 		//用于定位的卫星数
	printf("%s	\r\n",dtbuf);
	sprintf((char *)dtbuf,"Visible satellite:%02d",gpsx.svnum%100);	 		//可见卫星数
	printf("%s	\r\n",dtbuf);
	printf("%s	\r\n",dtbuf);	
	sprintf((char *)dtbuf,"UTC Date:%04d/%02d/%02d   ",gpsx.utc.year,gpsx.utc.month,gpsx.utc.date);	//显示UTC日期
	printf("year2:%d\r\n",gpsx.utc.year);
	printf("%s	\r\n",dtbuf);	
	sprintf((char *)dtbuf,"UTC Time:%02d:%02d:%02d   ",gpsx.utc.hour,gpsx.utc.min,gpsx.utc.sec);	//显示UTC时间
	printf("%s	\r\n",dtbuf);
}








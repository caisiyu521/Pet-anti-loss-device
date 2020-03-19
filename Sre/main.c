#include "sys.h"
#include "Config.h"

uint8_t string[20];			//������ʽ���ַ���	�洢����
extern uchar  GPRS_Delay_1S ;

void Gps_Msg_Show(void);	//��ʾGPS��λ��Ϣ 
void GPS_INIT(void);		//GPS ģ���ʼ��
void GPS_SHUJU(void);		//GPS ģ�����ݽ���
  			
int main(void){
	
	delay_init();			//delay ��ʼ��
	uart2_init(115200);		//����2 ��ʼ��
	uart3_init(9600);		//����3 ��ʼ��		GPRSģ�� �Ĳ�������9600
	Timer2_Init(1000,72);	//��ʼ�� Timer2 1ms �ж�һ��
//	LED_Init();
				
	DHT11Configuration();	//DHT11 ��ʪ��ģ��

	NVIC_Config();			//���� �жϵ� ���ȼ� �� �ų�ʼ�� ����
	
	GPS_INIT();				//GPS��ʼ��

//*****************************************	
//	if(MPU_Init() == 0)
//		printf("MPU ok!\r\n");
//	while(mpu_dmp_init() )
//		printf("dmp error!\r\n");

//*****************************************

	printf("CPU Start! \r\n");
	
	ret = initGPRS();								//��ѯ�ȴ�ģ��ע��ɹ�
	if(ret)	ret =  Set_Text_Mode();					//���������
//	if(ret)	ret =  Send_Text_Sms("caisiyu");					//����һ������Ϣ
	if(ret)	printf("GPRS ģ���ʼ���ɹ�! \r\n");
	else 	printf("GPRS ģ���ʼ��ʧ��! \r\n");

	
	GPRS_Delay_1S = 1;while(GPRS_Delay_1S);				//��ȷ��ʱ1S
	GPRS_Delay_1S = 1;while(GPRS_Delay_1S);	
	
	while(1){
		DHT11_Read_Data(&temp1,&shi);				//��ȡ DHT11 ���ص� �¶� ʪ��
		printf("temp = %d ,shi = %d\r\n",temp1,shi);
////		
		GPS_SHUJU();
		delay_ms(100);
//****************************************************************************
		if(mpu_dmp_get_data(&pitch,&roll,&yaw)==0){ 
			temp=MPU_Get_Temperature();	//�õ��¶�ֵ
//			MPU_Get_Accelerometer(&aacx,&aacy,&aacz);	//�õ����ٶȴ���������
//			MPU_Get_Gyroscope(&gyrox,&gyroy,&gyroz);	//�õ�����������
//			printf("aacx  = %d ,aacy  = %d ,aacz  = %d\r\n",aacx,aacy,aacz);//ԭʼ����ֵ
//			printf("gyrox = %d ,gyroy = %d ,gyroz = %d\r\n",gyrox,gyroy,gyroz);//ԭʼ���ٶ�ֵ
			printf("temp = %f , pitch = %f ,roll  = %f ,yaw   = %f\r\n",temp,pitch,roll,yaw);//DMP������ pitch �� roll��yawֵ
		}
		res=dmp_get_pedometer_step_count(&step_count); 		
		if(!res)
			printf ("count = %ld\r\n",step_count);
	}
}



void GPS_SHUJU(void){
	u16 rxlen , lenx ,i;
	if(USART1_RX_STA&0x8000){					   
		rxlen=USART1_RX_STA&0X7FFF;	//�õ����ݳ���
		for(i=0;i<rxlen;i++)TEMP_BUF[i]=USART1_RX_BUF[i];	   
		USART1_RX_STA=0;		   	//������һ�ν���
		TEMP_BUF[i]=0;				//�Զ���ӽ�����
		GPS_Analysis(&gpsx,(u8*)TEMP_BUF);//�����ַ���
		Gps_Msg_Show();
	}
		lenx++;	
}

void GPS_INIT(void){
	u8 key = 1;
	if(Ublox_Cfg_Rate(1000,1)!=0)	//���ö�λ��Ϣ�����ٶ�Ϊ1000ms,˳���ж�GPSģ���Ƿ���λ. 
	{
		printf("NEO-6M Setting...\r\n");
		while((Ublox_Cfg_Rate(1000,1)!=0)&&key)	//�����ж�,ֱ�����Լ�鵽NEO-6M,�����ݱ���ɹ�
		{
			USART1_Init(9600);				//��ʼ������1������Ϊ9600(EEPROMû�б������ݵ�ʱ��,������Ϊ9600.)
	  		Ublox_Cfg_Prt(38400);			//��������ģ��Ĳ�����Ϊ38400
			USART1_Init(38400);				//��ʼ������1������Ϊ38400 
			Ublox_Cfg_Tp(1000000,100000,1);	//����PPSΪ1�������1��,������Ϊ100ms	    
			key=Ublox_Cfg_Cfg_Save();		//��������  
		}	  					 
		printf("NEO-6M Set Done!\r\n");
		delay_ms(500);
	}
}

//��ʾGPS��λ��Ϣ 
void Gps_Msg_Show(void)
{
 	float tp;		
	printf("\r\n\r\n\n\n\n\n");    
	tp=gpsx.longitude;	   
	sprintf((char *)dtbuf,"Longitude:%.5f %1c   \r\n",tp/=100000,gpsx.ewhemi);	//�õ������ַ���
	printf("%s	\r\n",dtbuf);
	tp=gpsx.latitude;	   
	sprintf((char *)dtbuf,"Latitude:%.5f %1c   ",tp/=100000,gpsx.nshemi);	//�õ�γ���ַ���
	printf("%s	\r\n",dtbuf);
	tp=gpsx.altitude;	   
 	sprintf((char *)dtbuf,"Altitude:%.1fm     ",tp/=10);	    			//�õ��߶��ַ���
	printf("%s	\r\n",dtbuf);
	tp=gpsx.speed;	   
 	sprintf((char *)dtbuf,"Speed:%.3fkm/h     ",tp/=1000);		    		//�õ��ٶ��ַ���	 
	printf("%s	\r\n",dtbuf);
	if(gpsx.fixmode<=3)														//��λ״̬
	{  
		sprintf((char *)dtbuf,"Fix Mode:%s",fixmode_tbl[gpsx.fixmode]);	
		printf("%s	\r\n",dtbuf);
	}	 	   
	sprintf((char *)dtbuf,"Valid satellite:%02d",gpsx.posslnum);	 		//���ڶ�λ��������
	printf("%s	\r\n",dtbuf);
	sprintf((char *)dtbuf,"Visible satellite:%02d",gpsx.svnum%100);	 		//�ɼ�������
	printf("%s	\r\n",dtbuf);
	printf("%s	\r\n",dtbuf);	
	sprintf((char *)dtbuf,"UTC Date:%04d/%02d/%02d   ",gpsx.utc.year,gpsx.utc.month,gpsx.utc.date);	//��ʾUTC����
	printf("year2:%d\r\n",gpsx.utc.year);
	printf("%s	\r\n",dtbuf);	
	sprintf((char *)dtbuf,"UTC Time:%02d:%02d:%02d   ",gpsx.utc.hour,gpsx.utc.min,gpsx.utc.sec);	//��ʾUTCʱ��
	printf("%s	\r\n",dtbuf);
}








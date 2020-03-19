#include "gprs.h"
#include "led.h"
#include "usart.h"
#include "delay.h"
#include "string.h"

uchar  GPRS_Delay_1S = 0;

//unsigned char *SMS_Num="\"+8613800377500\"";//�������ĺ� �������ĺŵĽ��ܿ��Կ����ڵ��Աʼǵĵڶ�����
//unsigned char *phone="\"15517175421\"";     //���Ž��շ��ֻ���
//unsigned char *content="Thanks";//���Ͷ�������

const char *string123 = "AT+CIPSTART=\"TCP\",\"103.46.128.43\",56695\r\n";	//IP��¼������

u8 TXBuffer[TXBUFFER_LEN] = {0};  //����ͨ�����ݷ��ͻ���
u8 RXBuffer[RXBUFFER_LEN] = {0};  //����ͨ�����ݽ��ջ���
uchar OVER = 0;	

u8 initGPRS(void){
		//��ֹһֱ��ѭ��
	Usart_SendString(USART3,"AT\r\n");    delay_ms(100);  //����GPRSģ���뵥Ƭ���Ƿ�����
	if(findStr(RXBuffer,(u8 *)"OK",200)==0)   { 
		Usart_SendString(USART3,"AT\r\n"); 
		GPRS_Delay_1S = 1;while(GPRS_Delay_1S);				//��ȷ��ʱ1S
		if(findStr(RXBuffer,(u8 *)"OK",200)==0)   {
			printf("AT����ָ�����\r\n");
			return 0;  
		}     
    }
	memset(RXBuffer,0,sizeof(RXBuffer));
	
	Usart_SendString(USART3,"AT+CREG?\r\n");   		//��ѯģ���Ƿ�ע��ɹ�		�Ƚ���Ҫ
	GPRS_Delay_1S = 1;while(GPRS_Delay_1S);			//��ȷ��ʱ1S
	OVER  = 0;
	while(findStr(RXBuffer,(u8 *)"+CREG: 0,1",200)==0){ //���û���յ����ͳɹ��Ļظ����������
		Usart_SendString(USART3,"AT+CREG?\r\n");	//��ȷ��ʱ1S
		printf("Send error\r\n");
		GPRS_Delay_1S = 1;while(GPRS_Delay_1S);		//��ȷ��ʱ1S
//		GPRS_Delay_1S = 1;while(GPRS_Delay_1S);		//��ȷ��ʱ1S
		if(++OVER == 20)	return 0;				//����10�뻹û�����ӳɹ� ���˳�
    }
	memset(RXBuffer,0,sizeof(RXBuffer));
	printf("zhuce success ok\r\n");
	
    return 1;                         //���óɹ�
}

u8 Set_Text_Mode(void)
{
	Usart_SendString(USART3,"ATE0\r\n");				//�رջ���
	if(findStr(RXBuffer,(unsigned char*)"OK",200)==0) {
        return 0;      
    }
	
	memset(RXBuffer,0,sizeof(RXBuffer));
	Usart_SendString(USART3,"AT+CIPSHUT\r\n");			//�ر��ƶ�����		
	if(findStr(RXBuffer,(unsigned char*)"OK",200)==0) {
        return 0;      
    }
	
	memset(RXBuffer,0,sizeof(RXBuffer));
	Usart_SendString(USART3,"AT+CGCLASS=\"B\"\r\n");	//����GPRS�ƶ�̨���ΪB,֧�ְ����������ݽ��� 
	if(findStr(RXBuffer,(unsigned char*)"OK",200)==0) {
        return 0;      
    }
	
	memset(RXBuffer,0,sizeof(RXBuffer));
	Usart_SendString(USART3,"AT+CGDCONT=1,\"IP\",\"CMNET\"\r\n");	//����PDP������,��������Э��,��������Ϣ
	if(findStr(RXBuffer,(unsigned char*)"OK",200)==0) {
        return 0;      
    }
	
	memset(RXBuffer,0,sizeof(RXBuffer));
	Usart_SendString(USART3,"AT+CGATT=1\r\n");	//����GPRSҵ��
	if(findStr(RXBuffer,(unsigned char*)"OK",200)==0) {
        return 0;      
    }
	
	memset(RXBuffer,0,sizeof(RXBuffer));
	Usart_SendString(USART3,"AT+CIPCSGP=1,\"CMNET\"\r\n");	//����ΪGPRS����ģʽ
	if(findStr(RXBuffer,(unsigned char*)"OK",200)==0) {
        return 0;      
    }
	
	memset(RXBuffer,0,sizeof(RXBuffer));
	Usart_SendString(USART3,"AT+CIPHEAD=1\r\n");	//���ý���������ʾIPͷ(�����ж�������Դ,���ڵ�·������Ч)
	if(findStr(RXBuffer,(unsigned char*)"OK",200)==0) {
        return 0;      
    }
	
	memset(RXBuffer,0,sizeof(RXBuffer));
	Usart_SendString(USART3,(char*)string123);	//����ΪGPRS����ģʽ
	if(findStr(RXBuffer,(unsigned char*)"OK",200)==0) {
        return 0;      
    }
	
	memset(RXBuffer,0,sizeof(RXBuffer));
	
	return 1;   

}

//u8 Set_Text_Mode(void)
//{
//	char temp[50]="AT+CSCA=";							
//	Usart_SendString(USART3,"ATE0\r\n");				//�رջ���
//	if(findStr(RXBuffer,(unsigned char*)"OK",200)==0) {
//        return 0;      
//    }
//	memset(RXBuffer,0,sizeof(RXBuffer));
//	Usart_SendString(USART3,"AT+CMGF=1\r\n");			
//	if(findStr(RXBuffer,(unsigned char*)"OK",200)==0) {
//        return 0;      
//    }
//	memset(RXBuffer,0,sizeof(RXBuffer));
//	Usart_SendString(USART3,"AT+CPMS=\"SM\",\"SM\",\"SM\"\r\n");
//	if(findStr(RXBuffer,(unsigned char*)"OK",200)==0) {
//        return 0;      
//    }
//	memset(RXBuffer,0,sizeof(RXBuffer));

//	strcat(temp,(char*)SMS_Num);				//�������ĺ�
//	strcat(temp,"\r\n");
//	Usart_SendString(USART3,temp);
//	if(findStr(RXBuffer,(unsigned char*)"OK",200)==0) {
//        return 0;      
//    }
//	memset(RXBuffer,0,sizeof(RXBuffer));
//	
//	return 1;   

//}

//u8 Send_Text_Sms(char *str)
//{
//	char temp[50]="AT+CMGS=";
//	
//	memset(RXBuffer,0,sizeof(RXBuffer));
//	
//	strcat(temp,(char*)phone); //�ַ���ƴ�Ӻ������⺯����
//	strcat(temp,"\r\n");
//	Usart_SendString(USART3,temp);
//	GPRS_Delay_1S = 1;while(GPRS_Delay_1S);
//	
//	if(findStr(RXBuffer,(unsigned char*)">",200)==0) {
//		printf("no > \r\n");
//        return 0;      
//		
//    }
//	Usart_SendString(USART3,(char*)str);
//	GPRS_Delay_1S = 1;while(GPRS_Delay_1S);
//	
//	memset(RXBuffer,0,sizeof(RXBuffer));
//	
//	USART_SendData(USART3 ,0X1A);  //���ͽ�����
////	Usart_SendString(USART3,"\r\n");
//	
//	OVER  = 0;
////	
//	while(findStr(RXBuffer,(u8 *)"+CMGS",200)==0){ //���û���յ����ͳɹ��Ļظ����������
//		printf("Send error\r\n");
//		GPRS_Delay_1S = 1;while(GPRS_Delay_1S);		//��ȷ��ʱ1S
//		GPRS_Delay_1S = 1;while(GPRS_Delay_1S);		//��ȷ��ʱ1S
//		if(++OVER == 10)	
//			return 0;				//����10�뻹û�����ӳɹ� ���˳�
//    }
//	memset(RXBuffer,0,sizeof(RXBuffer)); 
//	return 1;
//}


u8 findStr(u8* dest,u8* src,u16 retry_cn)
{
	u16 retry = retry_cn;                   //��ʱʱ��
	u8 result_flag = 0;                     //���ҽ��
	
	while(strstr((const char *)dest,(const char *)src)==0 && --retry!=0){//�ȴ����ڽ�����ϻ�ʱ�˳�
		Delay_ms(10);
	}
	if(retry==0){                     //�����ʱ��������,��ʱ����0
	   return 0;
	}
	result_flag = 1;                        //ִ�е�����˵��һ������, ��ʾ���ҳɹ�

	if(result_flag){
		return 1;
	}else {
		return 0;
	}
} 

#include "usart.h"
#include "WiFiToCloud.h"

//#define MY_DEVICE_ID  "WLW176Cosmoon"
//#define MA_SECRET_KEY "eb1c3acda5d84c8c9d1e10b2fd5a98d3"


void LinkServer(void){
	 /************需填写代码******************************/
	
	for(int i = 0;i<5;i++){
		printf("AT+CIPSTART=\"TCP\",\"ndp.nlecloud.com\",8600\r\n");
		HAL_Delay(1500);
	}
		for(int i = 0;i<5;i++){
			printf("AT+CIPSEND=86\r\n");//这里的数字为发送数据的长度 要发送的数据时下一条printf中的数据长度
			HAL_Delay(1500);
			printf("{\"t\":1,\"device\":\"sn54645645\",\"key\":\"346863b85cec4bb292465c91bb84359d\",\"ver\":\"v0.0.0.0\"}");
			//device 后的数据为设备标识  key为设备的传输密钥 这里条发送结束之后设备应当显示在线
			HAL_Delay(1000);
		}
}

void SendData(int i){
	 /************需填写代码******************************/
	printf("AT+CIPSEND=75\r\n");
		HAL_Delay(1000);
	printf("{\"t\":3,\"datatype\":2,\"datas\":{\"fire\":{\"2022-06-9 14:10:26\":%d}},\"msgid\":001}",i);
	HAL_Delay(1000);
	
}
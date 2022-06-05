/**
  ******************************************************************************
  * File Name          : main.c
  * Description        : Main program body
  ******************************************************************************
  */
#include <string.h>
#include <stdio.h>
#include "board.h"
#include "radio.h"
#include "NS_Radio.h"
#include "hal_temHum.h"

uint8_t Rcv_Temp=0,Rcv_Hum=0;
uint16_t Rcv_Light=0;
		
// *** 用户代码添加：LoRa节点定义开始

//#define LORA_GATEWAY
//#define LORA_NODE_TEHU
#define LORA_NODE_LIGHT

// *** 用户代码添加：LoRa节点定义结束

// *** 通信协议相关宏定义
#define START_HEAD	0x55
#define CMD_READ		0x01
#define ACK_OK			0x00
#define ACK_NONE		0x01
#define ACK_ERR			0x02
#define MY_NET_ID		0xc0c2

#ifdef LORA_NODE_TEHU
	#define MY_ADDR			0x01
#endif

#ifdef LORA_NODE_LIGHT
	#define MY_ADDR			0x02
#endif

#ifdef LORA_GATEWAY
	#define ADDR_MIN 1
	#define ADDR_MAX 2
#endif



// *** 传感器全局变量声明

#ifdef LORA_NODE_TEHU
	int8_t temperature=25;
	int8_t humidity=70;
#endif

#ifdef LORA_NODE_LIGHT
	uint16_t LightLux=200;
#endif

uint8_t CheckSum(uint8_t *buf,uint8_t len);
uint8_t *ExtractCmdframe(uint8_t *buf,uint8_t len,uint8_t CmdStart);
uint16_t GetHexStr(uint8_t *input ,uint16_t len,uint8_t *output);
void LoRa_DataParse(uint8_t *LoRaRxBuf,uint16_t len);
void LoRa_GetSensorDataProcess(void);
void LoRa_SendRead(uint16_t NetId,uint8_t addr);
void LoRa_AutoCTL(uint8_t *LoRaRxBuf,uint16_t len);


void LoRa_GateWay_AutoCTL(uint8_t *DataBuf,uint16_t len)
{
	uint8_t *DestData =NULL;
	#define HEAD_DATA   	*DestData
	#define CMD_DATA			*(DestData+1)
	#define NETH_DATA 		*(DestData+2)
	#define NETL_DATA			*(DestData+3)
	#define ADDR_DATA    	*(DestData+4)
	#define ACK_DATA     	*(DestData+5)
	#define LEN_DATA    	*(DestData+6)
	#define DATASTAR_DATA *(DestData+7)
	
	DestData =ExtractCmdframe((uint8_t *)DataBuf ,len,START_HEAD );
	if(DestData !=NULL )
	{
		if((DestData - DataBuf )>(len-6)) return;
		if(CMD_DATA !=CMD_READ ) return;
		if(CheckSum((uint8_t *)DestData ,6+DestData[6]) !=(*(DestData+6+(*(DestData+6))))) return;
		if(((((uint16_t)NETH_DATA )<<8)+NETL_DATA ) !=MY_NET_ID ) return;
		char OledBuf[32];
		memset(OledBuf  ,' ',30 );
		memcpy(OledBuf,&DATASTAR_DATA,(LEN_DATA-1)>30?30:(LEN_DATA-1));
		OLED_ShowString(0,4,(uint8_t *)OledBuf);
		
		USART1_SendStr((uint8_t *)DestData,LEN_DATA+7);
	}
	
	// *** 用户添加代码：LoRa网关数据解析与逻辑控制开始
	if(ADDR_DATA==0x01)	// 温湿度
	{
		Rcv_Temp=(*(DestData+11)-'0')*10+(*(DestData+12)-'0');
		Rcv_Hum=(*(DestData+20)-'0')*10+(*(DestData+21)-'0');
	}
	if(ADDR_DATA==0x02)	// 光照度
	{
		if(*(DestData+6)==12)
			Rcv_Light=(*(DestData+13)-'0')*10+(*(DestData+14)-'0');
		else
			Rcv_Light=(*(DestData+13)-'0')*100+(*(DestData+14)-'0')*10+(*(DestData+15)-'0');
	}
	if(Rcv_Hum>70||Rcv_Temp>25||Rcv_Light<200)
		GpioWrite( &Relay, 1 );
	else
		GpioWrite( &Relay, 0);			
	// *** 用户代码添加：LoRa网关数据解析与逻辑控制结束	
}


void LoRa_SendRead(uint16_t NetId,uint8_t addr)
{
		uint8_t TxBuffer[BUFFER_SIZE];
		TxBuffer[0]=START_HEAD;
		TxBuffer[1]=CMD_READ;
		TxBuffer[2]=(uint8_t)(NetId>>8);
		TxBuffer[3]=(uint8_t)NetId;
		TxBuffer[4]=addr;
		TxBuffer[5]=CheckSum((uint8_t*)TxBuffer,5);
		Radio.Send(TxBuffer,6);
}

// 网关：读取传感器数据处理
void LoRa_ReadSensorProcess(uint8_t AddrMin,uint8_t AddrMax)
{
	static uint16_t time=1000;
	static uint8_t addr=1;
	if(User0Timer_MS >time )
	{
		User0Timer_MS =0;
		time=randr(1000,4000);
#ifndef TRANSPARENCY	
		printf("读取网络ID为0x%04x,地址为%d的传感节点\r\n",MY_NET_ID,addr);
#endif

		char StrBuf[32]={0};
		memset(StrBuf ,'\0',32);
		sprintf(StrBuf ,"ID:%04X,Addr:%d",MY_NET_ID,addr );
		OLED_ShowString(0,2,(uint8_t *)StrBuf);		
		memset(StrBuf ,' ',32);
		OLED_ShowString(0,4,(uint8_t *)StrBuf);		
		LoRa_SendRead(MY_NET_ID,addr++);
		if(addr>AddrMax)
			addr=AddrMin;
		GpioToggle(&Led1 );
	}
	
}

// 通信协议：校验和
uint8_t CheckSum(uint8_t *buf,uint8_t len)
{
	uint8_t temp =0;
	while(len--)
	{
		temp +=*buf;
		buf++;		
	}
	return (uint8_t )temp;
}

// 通信协议：通信帧头解析
uint8_t *ExtractCmdframe(uint8_t *buf,uint8_t len,uint8_t CmdStart)
{
	uint8_t *point = NULL;
	uint8_t i;
	for(i=0;i<len;i++)
	{
		if(CmdStart ==*buf )
		{
			point = buf;
			return point;
		}
	buf++;
	}
return NULL;
}

// 通信协议：命令帧解析
void LoRa_DataParse(uint8_t *LoRaRxBuf,uint16_t len)
{
	#if defined(LORA_GATEWAY)
		LoRa_GateWay_AutoCTL(LoRaRxBuf,len);
	#endif
	
	#ifdef LORA_NODE_TEHU
		uint8_t *DestData =NULL;
		#define HEAD_DATA   *DestData
		#define CMD_DATA		*(DestData+1)
		#define NETH_DATA 	*(DestData+2)
		#define NETL_DATA		*(DestData+3)
		#define ADDR_DATA    *(DestData+4)
		#define ACK_DATA     *(DestData+5)
		#define LEN_DATA    *(DestData+6)
		#define DATASTAR_DATA   *(DestData+7)
		DestData =ExtractCmdframe((uint8_t *)LoRaRxBuf ,len,START_HEAD );
		if(DestData !=NULL )
		{
			if((DestData - LoRaRxBuf )>(len-6)) return;
			if(CMD_DATA !=CMD_READ ) return;
			if(CheckSum((uint8_t *)DestData ,5) !=(*(DestData+5)) ) return;
			if(((((uint16_t)NETH_DATA )<<8)+NETL_DATA ) !=MY_NET_ID ) return;
			if(ADDR_DATA  !=MY_ADDR ) return;
			uint8_t RspBuf[BUFFER_SIZE]={0};
			memset(RspBuf ,'\0',BUFFER_SIZE );
			RspBuf[0]=START_HEAD ;
			RspBuf[1]=CMD_READ ;
			RspBuf[2]=(uint8_t)(MY_NET_ID>>8) ;
			RspBuf[3]=(uint8_t)MY_NET_ID ;
			RspBuf[4]=MY_ADDR ;
			RspBuf[5]=ACK_OK ;
			sprintf((char *)(RspBuf +7),"tem:%dCe,hum:%d%%",temperature,humidity);
			RspBuf[6]=strlen((const char *)(RspBuf +7))+1;
			RspBuf[6+RspBuf[6]]=CheckSum((uint8_t *)RspBuf,6 +RspBuf[6]);
			Radio.Send(RspBuf ,7 +RspBuf[6]);
			GpioToggle(&Led1 );
		}
	#endif
	
	#ifdef LORA_NODE_LIGHT
		uint8_t *DestData =NULL;
		#define HEAD_DATA   *DestData
		#define CMD_DATA		*(DestData+1)
		#define NETH_DATA 	*(DestData+2)
		#define NETL_DATA		*(DestData+3)
		#define ADDR_DATA    *(DestData+4)
		#define ACK_DATA     *(DestData+5)
		#define LEN_DATA    *(DestData+6)
		#define DATASTAR_DATA   *(DestData+7)
		DestData =ExtractCmdframe((uint8_t *)LoRaRxBuf ,len,START_HEAD );
		if(DestData !=NULL )
		{
			if((DestData - LoRaRxBuf )>(len-6)) return;
			if(CMD_DATA !=CMD_READ ) return;
			if(CheckSum((uint8_t *)DestData ,5) !=(*(DestData+5)) ) return;
			if(((((uint16_t)NETH_DATA )<<8)+NETL_DATA ) !=MY_NET_ID ) return;
			if(ADDR_DATA  !=MY_ADDR ) return;
			uint8_t RspBuf[BUFFER_SIZE]={0};
			memset(RspBuf ,'\0',BUFFER_SIZE );
			RspBuf[0]=START_HEAD ;
			RspBuf[1]=CMD_READ ;
			RspBuf[2]=(uint8_t)(MY_NET_ID>>8) ;
			RspBuf[3]=(uint8_t)MY_NET_ID ;
			RspBuf[4]=MY_ADDR ;
			RspBuf[5]=ACK_OK ;
			sprintf((char *)(RspBuf +7),"Light:%dLux",LightLux );
			RspBuf[6]=strlen((const char *)(RspBuf +7))+1;
			RspBuf[6+RspBuf[6]]=CheckSum((uint8_t *)RspBuf,6 +RspBuf[6]);
			Radio.Send(RspBuf ,7 +RspBuf[6]);
			GpioToggle(&Led1 );
		}	
	#endif
}

// 格式转换
uint16_t GetHexStr(uint8_t *input ,uint16_t len,uint8_t *output)
{
	for(uint16_t i=0;i<len;i++)
	{
		sprintf((char *)(output+i*3),"%02X ",*input );
		input++;
	}
	return strlen((const char *)output);
}


// 读取传感器参数
void LoRa_GetSensorDataProcess(void)
{
	#ifdef LORA_NODE_TEHU
		const uint16_t time=1000;
		if(User0Timer_MS >time )
		{
			User0Timer_MS =0;
			uint16_t Temp,Rh;
			call_sht11((uint16_t *)(&Temp),(uint16_t *)(&Rh));
			temperature =(int8_t)Temp;
			if(temperature<10)
				temperature=10;
			if(temperature>=100)
				temperature=99;
			humidity  =(int8_t)Rh;
			if(humidity<10)
				humidity=10;
			char StrBuf[64]={0};
			memset(StrBuf ,'\0',64);
			sprintf(StrBuf ,"tem:%dCe,hum:%d%%",temperature,humidity);
			OLED_ShowString(0,6,(uint8_t *)StrBuf);		
		}
	#endif
	
	#ifdef LORA_NODE_LIGHT
		const uint16_t time=1000;
		if(User0Timer_MS >time )
		{
			User0Timer_MS =0;
			uint16_t AdcNum,voltage;
			AdcNum =AdcReadChannel(&Adc,ADC_CHANNEL_0);
			voltage=AdcNum*3300/(4096-1.0);
			LightLux=5/2.0*(voltage/10.0);
			if(LightLux>999)
				LightLux=999;
			if(LightLux<10)
				LightLux=10;			
			char StrBuf[64]={0};	
			memset(StrBuf ,'\0',64);
			sprintf(StrBuf ,"Light:%3dLux",LightLux );
			OLED_ShowString(0,6,(uint8_t *)StrBuf);	
		}
	#endif
}


// *** 用户添加部分结束

void LoRa_Send( uint8_t *TxBuffer, uint8_t len );
void MyRadioRxDoneProcess( void );
void OLED_InitView(void);
void PlatformInit(void);

/**********************************************************************************************
*函数：void LoRa_Send( uint8_t *TxBuffer, uint8_t len )
*功能：发送LoRa无线数据
*输入：
*       uint8_t *TxBuffer, 待发送数据
*       uint8_t len, 待发送数据长度
*输出：无
*返回：无
*特殊说明：无
**********************************************************************************************/
void LoRa_Send( uint8_t *TxBuffer, uint8_t len )
{
    Radio.Send( TxBuffer, len);
}

/**********************************************************************************************
*函数：void MyRadioRxDoneProcess( void )
*功能：无线模块数据接收完成处理进程函数
*输入：无
*输出：无
*返回：无
*特殊说明：接收到的无线数据保存在RxBuffer中，BufferSize为接收到的无线数据长度
**********************************************************************************************/
void MyRadioRxDoneProcess( void )
{
    uint16_t BufferSize = 0;
    uint8_t RxBuffer[BUFFER_SIZE];

    BufferSize = ReadRadioRxBuffer( (uint8_t *)RxBuffer );
    if(BufferSize>0)
    {
        //用户在此处添加接收数据处理功能的代码
        ;
			GpioToggle(&Led2 );
			LoRa_DataParse((uint8_t *)RxBuffer ,BufferSize );        
        
    }
}

/**********************************************************************************************
*函数：void OLED_InitView()
*功能：Lora液晶初始化
*输入：无
*输出：无
*特殊说明：无
**********************************************************************************************/
void OLED_InitView(void)
{
	char StrBuf[32]={0};
	memset(StrBuf ,'\0',32);
	
	OLED_Clear();
	#if defined(LORA_GATEWAY)
		OLED_ShowString(0,0, (uint8_t *)"LoRa GateWay");
	#else
		OLED_ShowString(0,0,(uint8_t*)"Freq:");
		OLED_ShowNum(5*8,0,(uint32_t)RF_PING_PONG_FREQUENCY,9,16);	
		sprintf(StrBuf ,"ID:%04X,Addr:%d",MY_NET_ID,MY_ADDR );
		OLED_ShowString(0,2,(uint8_t *)StrBuf);	
	#endif
	
	#if defined(LORA_NODE_TEHU)
		OLED_ShowString(0,4, (uint8_t *)"Lora Temp/Hum");
	#endif
	
	#if defined(LORA_NODE_LIGHT)
		OLED_ShowString(0,4, (uint8_t *)"Lora Light");
	#endif
	
		
}

/**********************************************************************************************
*函数：void PlatformInit( void )
*功能：平台初始化
*输入：无
*输出：无
*特殊说明：无
**********************************************************************************************/
void PlatformInit(void)
{
    // 开发板平台初始化
    BoardInitMcu();
    BoardInitPeriph();

    // 开发板设备初始化
    OLED_Init();//液晶初始化
    USART1_Init(115200);//串口1初始化
    OLED_Clear();
    OLED_InitView();//OLED屏幕显示初始信息
    
    printf("新大陆教育 LoRa \r\n");

    //Lora模块初始化
    NS_RadioInit( (uint32_t) RF_PING_PONG_FREQUENCY, (int8_t) TX_OUTPUT_POWER, (uint32_t) TX_TIMEOUT_VALUE, (uint32_t) RX_TIMEOUT_VALUE );
    
    //请在下方添加用户初始化代码
	#if defined(LORA_GATEWAY)
		printf("Gateway\r\n");	
	#endif
	
	#if defined(LORA_NODE_TEHU)
		printf("LoRa TempRh\r\n");
		hal_temHumInit();	
	#endif

	#if defined(LORA_NODE_LIGHT)
		printf("LoRa Light\r\n");
		AdcInit(&Adc,ADC_0);
	#endif
	
	  connectionreset();
    Tim3McuInit(1);
    //IWDG_PrmInit(2048);//独立看门狗初始化，超时设置为2048ms
}


/**
 * Main application entry point.
 */
int main( void )
{
    PlatformInit();
    while( 1 )
    {
        //IWDG_PrmRefresh( );//喂独立看门狗
			
        //请在下方添加用户功能代码
				MyRadioRxDoneProcess();
			#if defined(LORA_NODE_LIGHT)||defined(LORA_NODE_TEHU)
				LoRa_GetSensorDataProcess(); 
			#endif
			
			#ifdef LORA_GATEWAY
				LoRa_ReadSensorProcess(ADDR_MIN,ADDR_MAX);
			#endif
    }
}

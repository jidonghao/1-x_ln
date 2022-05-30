#include "user_can.h"
#include "user.h"

uint32_t TxMailbox;

//#define  Exd_ID_1   0x08ac0006
//#define  Exd_ID_2   0x182C0004
//#define  Exd_ID_3   0x08AC0003 

CAN_TxHeaderTypeDef     TxMeg;

#define CAN_ID_STDaa  	  0xAA
#define CAN_ID_STDbb  	  0xBB
#define CAN_ID_STDcc      0xCC
#define CAN_ID_STDdd  	  0xDD
extern CAN_HandleTypeDef  hcan1;


/******************************************************************************
 * FunctionName : void CAN_User_Config(CAN_HandleTypeDef* hcan )
 * Description  : CAN总线配置
 * Parameters   : hcan
 * Returns      : none
*******************************************************************************/
void CAN_User_Config(CAN_HandleTypeDef* hcan )  
{
    CAN_FilterTypeDef sFilterConfig;
    HAL_StatusTypeDef HAL_Status;
    
    sFilterConfig.FilterBank = 0;
    sFilterConfig.FilterMode = CAN_FILTERMODE_IDMASK;
    sFilterConfig.FilterScale = CAN_FILTERSCALE_32BIT;
    sFilterConfig.FilterIdHigh = 0x0000;
    sFilterConfig.FilterIdLow = 0x0000;
    sFilterConfig.FilterMaskIdHigh = 0x0000;
    sFilterConfig.FilterMaskIdLow = 0x0000;
    sFilterConfig.FilterFIFOAssignment = CAN_RX_FIFO0;
    sFilterConfig.FilterActivation = ENABLE;
    sFilterConfig.SlaveStartFilterBank = 0;
    
    HAL_Status=HAL_CAN_ConfigFilter(hcan,&sFilterConfig);
    if(HAL_Status != HAL_OK)
    {
        printf("开启CAN失败\r\n");
    }

    HAL_Status=HAL_CAN_ActivateNotification(hcan,CAN_IT_RX_FIFO0_MSG_PENDING);
    if(HAL_Status != HAL_OK)
    {
        printf("开启挂起中断允许失败\r\n");
    }
}





void CAN_User_Init(CAN_HandleTypeDef* hcan)  
{
    CAN_FilterTypeDef sFilterConfig;
    HAL_StatusTypeDef HAL_Status;
    
    sFilterConfig.FilterBank = 0;
    sFilterConfig.FilterMode = CAN_FILTERMODE_IDMASK;
    sFilterConfig.FilterScale = CAN_FILTERSCALE_32BIT;
    sFilterConfig.FilterIdHigh = 0x0000;
    sFilterConfig.FilterIdLow = 0x0000;
    sFilterConfig.FilterMaskIdHigh = 0x0000;
    sFilterConfig.FilterMaskIdLow = 0x0000;
    sFilterConfig.FilterFIFOAssignment = CAN_RX_FIFO0;
    sFilterConfig.FilterActivation = ENABLE;
    sFilterConfig.SlaveStartFilterBank = 0;
    
    HAL_Status=HAL_CAN_ConfigFilter(hcan,&sFilterConfig);
    if(HAL_Status != HAL_OK)
    {
        printf("开启CAN失败\r\n");
    }

    HAL_Status=HAL_CAN_ActivateNotification(hcan,CAN_IT_RX_FIFO0_MSG_PENDING);
    if(HAL_Status != HAL_OK)
    {
        printf("开启挂起中断允许失败\r\n");
    }
}







/******************************************************************************
 * FunctionName : void can_start(void)
 * Description  : 启动CAN总线
 * Parameters   : none
 * Returns      : none
*******************************************************************************/
void can_start(void)
{
    HAL_CAN_Start(&hcan);   
}

/******************************************************************************
 * FunctionName : void can_stop(void)
 * Description  : 停止CAN总线
 * Parameters   : none
 * Returns      : none
*******************************************************************************/
void can_stop(void)
{
    HAL_CAN_Stop(&hcan);  
}

/******************************************************************************
 * FunctionName : uint8_t Can_Send_Msg(uint8_t* msg,uint8_t len)
 * Description  : can发送一组数据(固定格式:ID为0X12,标准帧,数据帧)	
 * Parameters   : len:数据长度(最大为8)
 *                msg:数据指针,最大为8个字节.
 * Returns      : 0,成功;
 *            		其他,失败;
*******************************************************************************/
uint8_t Can_Send_Msg(uint8_t* msg,uint8_t len)
{
    uint16_t i=0;  
    uint8_t data[8];  
      
    CAN_TxHeaderTypeDef  TxMeg;   
  
    TxMeg.StdId=0x12;           // 标准标识符   
    TxMeg.ExtId=0x12;           // 设置扩展标示符   
    TxMeg.IDE=CAN_ID_STD;   // 标准帧  
    TxMeg.RTR=CAN_RTR_DATA;       // 数据帧  
    TxMeg.DLC=len;              // 要发送的数据长度  
    for(i=0;i<len;i++)  
    {  
        data[i]=msg[i];  
    }  
      
    if (HAL_CAN_AddTxMessage(&hcan, &TxMeg, data, &TxMailbox) != HAL_OK)  
    {  
        printf("Can send data error\r\n");  
    }  
    else  
    {  
        printf("Can send data success\r\n");  
    }  
    return 0;
}

/******************************************************************************
 * FunctionName : uint8_t Can_Send_Msg(uint8_t* msg,uint8_t len)
 * Description  : can发送一组数据(固定格式:ID为0X12,标准帧,数据帧)	
 * Parameters   : len:数据长度(最大为8)
 *                msg:数据指针,最大为8个字节.
 * Returns      : 0,成功;
 *            		其他,失败;
*******************************************************************************/
uint8_t Can_Send_Msg_ID(uint8_t* msg,uint8_t stdid,uint8_t len)
{
    uint16_t i=0;  
    uint8_t data[8];  
      
    CAN_TxHeaderTypeDef  TxMeg;   
  
    TxMeg.StdId=stdid;       // 标准标识符   
    TxMeg.ExtId=0;           // 设置扩展标示符   
    TxMeg.IDE=CAN_ID_STD;    // 标准帧  
    TxMeg.RTR=CAN_RTR_DATA;  // 数据帧  
    TxMeg.DLC=len;           // 要发送的数据长度  
    for(i=0;i<len;i++)  
    {  
        data[i]=msg[i];  
    }  
      
    if (HAL_CAN_AddTxMessage(&hcan, &TxMeg, data, &TxMailbox) != HAL_OK)  
    {  
        printf("Can send data error\r\n");  
    }  
    else  
    {  
        printf("Can send data success\r\n");  
    }  
    return 0;
}

/******************************************************************************
 * FunctionName : uint8_t Can_Send_Msg_StdId(uint16_t My_StdId,uint8_t len,uint8_t Type_Sensor)
 * Description  : can发送一组数据(固定格式:ID为My_StdId,标准帧,数据帧)	
 * Parameters   : My_StdId:标准标识符 
 *                len:数据长度(最大为8)
 *                msg:数据指针,最大为8个字节.
 * Returns      : 0,成功;
 *            		其他,失败;
*******************************************************************************/
uint8_t Can_Send_Msg_StdId(uint16_t My_StdId,uint8_t len,uint8_t Type_Sensor)
{
    CAN_TxHeaderTypeDef TxMsg;
    ValueType ValueType_t;
    
    uint8_t vol_H,vol_L;
    uint16_t i=0;
    uint8_t data[8];
    
    TxMsg.StdId = My_StdId;
    TxMsg.ExtId = 0x00;
    TxMsg.IDE=CAN_ID_STD;
    TxMsg.RTR=CAN_RTR_DATA;
    TxMsg.DLC=len;
    for(i=0;i<len;i++)
        data[i]=0;
    
    data[0]=Sensor_Type_t;
    data[4]=(uint8_t)My_StdId;
    printf("can send msg stdid >> my stdid 标准 ID=%x \r\n",My_StdId);
    printf("can send msg stdid >> sensor type %d\r\n",data[0]);
    ValueType_t=ValueTypes(Type_Sensor);
    printf("can send msg stdid >> value type %d\r\n",ValueType_t);
    
    
    switch(ValueType_t)
    {
        case Value_ADC:
            vol_H = (vol&0xff00)>>8;
            vol_L = (vol&0x00ff);
            data[1]=vol_H;
            data[2]=vol_L;
            printf("can send msg stdid >> value adc txmessage data[1]=%d\r\n",data[1]);
            printf("can send msg stdid >> value adc txmessage data[2]=%d\r\n",data[2]);
            break;
        
        case Value_Switch:
            data[1]=switching;
            data[2]=0;
            break;
        
        
        case Value_I2C:
            data[1]=sensor_tem;
            data[2]=sensor_hum;
            printf("can send msg stdid >> value i2c txmessage data[1]=%d\r\n",data[1]);
            printf("can send msg stdid >> value i2c txmessage data[2]=%d\r\n",data[2]);
            break;
        default:
            break;
    }
    
    if(HAL_CAN_AddTxMessage(&hcan,&TxMsg,data,&TxMailbox) != HAL_OK)
    {
        printf("can send data error\r\n");
    }
    else
    {
        printf("can send data success\r\n");
    }
    
    return 0;
}



//CAN接收回调函数
void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan)      
{
    CAN_RxHeaderTypeDef RxMsg;
    
    uint8_t data[8]={0};
    HAL_StatusTypeDef HAL_RetVal;
    
    int  i;
    
    RxMsg.StdId=0x00;
    RxMsg.ExtId=0x00;
    RxMsg.IDE=0;
    RxMsg.DLC=0;
    
    HAL_RetVal=HAL_CAN_GetRxMessage(hcan,CAN_RX_FIFO0,&RxMsg,data);
    
    if(HAL_OK == HAL_RetVal)
    {
        for(i=0;i<RxMsg.DLC;i++)
        {
            Can_data[i]=data[i];
            printf("%02X ",data[i]);
        }
        printf("\r\n");
        flag_send_data=1;
    }
}
   
/*******************************************************************************
* 函数名 		: FilterID
* 函数描述    	: 过滤屏蔽接收ID
* 输入参数     	: gaodiwei为0时选择低16位，1时选择高16位，FID为要屏蔽的ID号
* 输出结果     	: 无
* 返回值       	: Filter_IDL/Filter_IDH ,ID值
*******************************************************************************/
unsigned short FilterID(uint8_t gaodiwei,uint32_t FID)  
{  	
   uint32_t Filter_ID;
   uint16_t Filter_IDH;
   uint16_t Filter_IDL;
   if(gaodiwei==0)				 	//取出CAN-ID低位字节
   {
	    Filter_IDL=FID&0x0000FFFF;		 
		Filter_IDL=Filter_IDL<<3;
		Filter_IDL=Filter_IDL|0x0004;
		return	Filter_IDL; 
   }
   else if(gaodiwei==1)				//取出CAN-ID高位字节
   {
	 	Filter_ID=FID&0xffff0000;
		Filter_IDH=Filter_ID>>16;
	    Filter_IDH=Filter_IDH<<3;
	   	return	Filter_IDH;	 	 
   }
   else
   {
   		return 0x0000;
   } 		 	
}


#include "user_can.h"
#include "user.h"

uint32_t TxMailbox;

 
/******************************************************************************
 * FunctionName : void CAN_User_Config(CAN_HandleTypeDef* hcan )
 * Description  : CAN��������
 * Parameters   : hcan
 * Returns      : none
*******************************************************************************/
void CAN_User_Config(CAN_HandleTypeDef* hcan )  
{
	  CAN_FilterTypeDef  sFilterConfig;  
    HAL_StatusTypeDef  HAL_Status;  
    sFilterConfig.FilterBank = 0; //������0   
    sFilterConfig.FilterMode =  CAN_FILTERMODE_IDMASK;//����λģʽ  
    sFilterConfig.FilterScale = CAN_FILTERSCALE_32BIT; //32λ��   
    sFilterConfig.FilterIdHigh = 0x0000;  //32λID  
    sFilterConfig.FilterIdLow  = 0x0000;  
    sFilterConfig.FilterMaskIdHigh = 0x0000; //32λMASK  
    sFilterConfig.FilterMaskIdLow  = 0x0000;  
    sFilterConfig.FilterFIFOAssignment = CAN_RX_FIFO0; //���յ��ı��ķ��뵽FIFO0��  
    sFilterConfig.FilterActivation = ENABLE; //���������    
    sFilterConfig.SlaveStartFilterBank  = 0;  
    HAL_Status=HAL_CAN_ConfigFilter(hcan, &sFilterConfig);  
    HAL_Status=HAL_CAN_Start(hcan);  //����CAN     
    if(HAL_Status!=HAL_OK)  
    {  
        printf("����CANʧ��\r\n");  
    }  
    HAL_Status=HAL_CAN_ActivateNotification(hcan, CAN_IT_RX_FIFO0_MSG_PENDING);     
    if(HAL_Status!=HAL_OK)  
    {  
        printf("���������ж�����ʧ��\r\n");         
    } 
}


/******************************************************************************
 * FunctionName : void can_start(void)
 * Description  : ����CAN����
 * Parameters   : none
 * Returns      : none
*******************************************************************************/
void can_start(void)
{
	HAL_CAN_Start(&hcan);   
}

/******************************************************************************
 * FunctionName : void can_stop(void)
 * Description  : ֹͣCAN����
 * Parameters   : none
 * Returns      : none
*******************************************************************************/
void can_stop(void)
{
	HAL_CAN_Stop(&hcan);  
}

/******************************************************************************
 * FunctionName : uint8_t Can_Send_Msg(uint8_t* msg,uint8_t len)
 * Description  : can����һ������(�̶���ʽ:IDΪ0X12,��׼֡,����֡)	
 * Parameters   : len:���ݳ���(���Ϊ8)
 *                msg:����ָ��,���Ϊ8���ֽ�.
 * Returns      : 0,�ɹ�;
 *            		����,ʧ��;
*******************************************************************************/
uint8_t Can_Send_Msg(uint8_t* msg,uint8_t len)
{	
		uint16_t i=0;  
    uint8_t data[8];  
      
    CAN_TxHeaderTypeDef  TxMeg;   
  
    TxMeg.StdId=0x12;           // ��׼��ʶ��   
    TxMeg.ExtId=0x12;           // ������չ��ʾ��   
    TxMeg.IDE=CAN_ID_STD;   // ��׼֡  
    TxMeg.RTR=CAN_RTR_DATA;       // ����֡  
    TxMeg.DLC=len;              // Ҫ���͵����ݳ���  
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
 * Description  : can����һ������(�̶���ʽ:IDΪMy_StdId,��׼֡,����֡)	
 * Parameters   : My_StdId:��׼��ʶ�� 
 *                len:���ݳ���(���Ϊ8)
 *                msg:����ָ��,���Ϊ8���ֽ�.
 * Returns      : 0,�ɹ�;
 *            		����,ʧ��;
*******************************************************************************/
uint8_t Can_Send_Msg_StdId(uint16_t My_StdId,uint8_t len,uint8_t Type_Sensor)
{	
		CAN_TxHeaderTypeDef  TxMeg;   
    ValueType ValueType_t;  
    uint8_t vol_H,vol_L;  
    uint16_t i=0;  
    uint8_t data[8];  
  
    TxMeg.StdId=My_StdId;             // ��׼��ʶ��   
    TxMeg.ExtId=0x00;                       // ������չ��ʾ��   
    TxMeg.IDE=CAN_ID_STD;   // ��׼֡  
    TxMeg.RTR=CAN_RTR_DATA;           // ����֡  
    TxMeg.DLC=len;                          // Ҫ���͵����ݳ���  
    for(i=0;i<len;i++)  
    {  
        data[i]=0;  
    }  
      
    data[0] =   Sensor_Type_t;  
		data[3] =   (uint8_t)My_StdId&0x00ff;        //ȡ����λ��ID  
    data[4] =  My_StdId>>8;   //��λ�ں�
    printf("Can_Send_Msg_StdId >>My_StdId ��׼֡ID= %x   \r\n",My_StdId);  
    printf("Can_Send_Msg_StdId >>Sensor_Type_t %d \r\n",data[0]);  
    ValueType_t=ValueTypes(Type_Sensor);  
    printf("Can_Send_Msg_StdId >>ValueType_t %d \r\n",ValueType_t);  
      
      
    switch(ValueType_t)  
    {  
        case Value_ADC:  
              
                    vol_H = (vol&0xff00)>>8;  
                    vol_L = vol&0x00ff;  
                    data[1]=vol_H;  
                    data[2]=vol_L;        
                    printf("Can_Send_Msg_StdId >> Value_ADC TxMessage.Data[1]=vol_L %d \r\n",data[1]);  
                    printf("Can_Send_Msg_StdId >> Value_ADC TxMessage.Data[2]=vol_L %d \r\n",data[2]);  
            break;  
        case Value_Switch:  
                    data[1]=switching;  
                    data[2]=0;  
            break;  
        case Value_I2C:  
                    data[1]=sensor_tem;  
                    data[2]=sensor_hum;  
                    printf("Can_Send_Msg_StdId >> Value_I2C TxMessage.Data[1]=vol_L %d \r\n",data[1]);  
                    printf("Can_Send_Msg_StdId >> Value_I2C TxMessage.Data[2]=vol_L %d \r\n",data[2]);   
            break;  
        default:  
            break;  
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

//CAN�жϻص�����
void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan)      
{
	CAN_RxHeaderTypeDef RxMeg;  
    uint8_t  Data[8] = {0};  
    HAL_StatusTypeDef   HAL_RetVal;  
    int i;  
      
    RxMeg.StdId=0x00;  
    RxMeg.ExtId=0x00;  
    RxMeg.IDE=0;  
    RxMeg.DLC=0;  
  
    HAL_RetVal=HAL_CAN_GetRxMessage(hcan,  CAN_RX_FIFO0, &RxMeg,  Data);  
    if ( HAL_OK==HAL_RetVal)  
    {                                         
        for(i=0;i<RxMeg.DLC;i++)  
        {    
            Can_data[i]= Data[i];  
            printf("%02X ",Data[i]);  
              
        }         
        printf("\r\n");  
        flag_send_data=1;  
    } 
}
   



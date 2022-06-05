/************************************************************************************************************************************************
*
*
*
*
*
*
*
*************************************************************************************************************************************************/

#include "stm32l1xx_hal.h"
#include "stm32l1xx_hal_iwdg.h"
#include "iwdg-board.h"

/* IWDG handlers declaration */
IWDG_HandleTypeDef IwdgHandle;

/*******************************************************************
*������void IWDG_PrmInit(void)
*���ܣ��������Ź�������ʼ��
*���룺uint16_t IwdgMs,���Ź���ʱʱ�䣬��λ:ms����
*�������
*���أ���
*����˵��������LSI����32768Hz��32��Ƶ�����ؼĴ������ֵ0xFFF������ɵ�
IwdgMs�����ֵ���ó���4000ms
*******************************************************************/
void IWDG_PrmInit(uint16_t IwdgMs)
{
    /*##-1- Check if the system has resumed from IWDG reset ####################*/
    if (__HAL_RCC_GET_FLAG(RCC_FLAG_IWDGRST) != RESET)
    {
        /* IWDGRST flag set*/
        printf("�Ӷ������Ź�����\r\n");
        /* Clear reset flags */
        __HAL_RCC_CLEAR_RESET_FLAGS();
    }
    else
    {
        /* IWDGRST flag is not set*/
        ;
    }

    /*##-2- Get the LSI frequency: TIM10 is used to measure the LSI frequency ###*/
#define IWDG_LSI_FREQUENCY ((uint32_t)(0x8000))//�����õ���32.768KHzʯӢ����

    /*##-3- Configure the IWDG peripheral ######################################*/
    /*
       Set counter reload value to obtain 250ms IWDG TimeOut.
       IWDG counter clock Frequency = LsiFreq / 32
       Counter Reload Value = 250ms / IWDG counter clock period
                            = 0.25s / (32/LsiFreq)
                            = LsiFreq / (32 * 4)
                            = LsiFreq / 128
    */
    IwdgHandle.Instance = IWDG;

    IwdgHandle.Init.Prescaler = IWDG_PRESCALER_32;
    //IwdgHandle.Init.Reload    = IWDG_LSI_FREQUENCY / 128;
    IwdgHandle.Init.Reload    = (uint16_t)((IWDG_LSI_FREQUENCY / 32)*(IwdgMs/1000.0));

    if (HAL_IWDG_Init(&IwdgHandle) != HAL_OK)
    {
        /* Initialization Error */
        printf("�������Ź���ʼ��ʧ��\r\n");
    }

    /*##-4- Start the IWDG #####################################################*/
    /*if (HAL_IWDG_Start(&IwdgHandle) != HAL_OK)//�ɰ����Ҫ����������°�Ĳ���Ҫ
    {
        ;
    }*/

    /* Refresh IWDG: reload counter */
    if (HAL_IWDG_Refresh(&IwdgHandle) != HAL_OK)
    {
        /* Refresh Error */
        printf("�������Ź�ˢ��ʧ��\r\n");
    }
}

/*******************************************************************
*������void IWDG_PrmRefresh(void)
*���ܣ��������Ź�����ˢ�£�ι����
*���룺��
*�������
*���أ���
*����˵������
*******************************************************************/
void IWDG_PrmRefresh(void)
{
    if (HAL_IWDG_Refresh(&IwdgHandle) != HAL_OK)
    {
        /* Refresh Error */
        printf("�������Ź�ˢ��ʧ��\r\n");
    }
}

/************************ (C) COPYRIGHT NEWLAND_EDU *****END OF FILE****/

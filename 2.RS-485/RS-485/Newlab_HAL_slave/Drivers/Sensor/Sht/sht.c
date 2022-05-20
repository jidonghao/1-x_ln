
#include <stdio.h>
#include "sht1x.h"
#include "sht3x.h"

#define TYPE_SHT1X 0x00
#define TYPE_SHT3X 0x01

static uint8_t SHT_TypeFlag = 0x00;//Mean��0-SHT1X, 1-SHT3X

/*******************************************************************
*������void SHT_Init(void)
*���ܣ���ʪ�ȴ�������ʼ��
*���룺��
*�������
*���أ���
*����˵������
*******************************************************************/
void SHT_Init(void)
{
    etError   error;       // error code
    error = SHT3X_Init(DEFAULT_SHT3X_ADDR);
    if(error != NO_ERROR)
    {
        SHT1X_Init();
        SHT_TypeFlag = TYPE_SHT1X;
    }
    else
    {
        SHT_TypeFlag = TYPE_SHT3X;
    }
    if(SHT_TypeFlag != TYPE_SHT1X)
    {
        printf("��ʪ�ȴ������ͺţ�SHT3X\r\n");
    }
    else
    {
        printf("��ʪ�ȴ������ͺţ�SHT1X\r\n");
    }
}

/*******************************************************************
*������uint8_t ChkSum(uint8_t *buf, uint8_t len)
*���ܣ��ɼ����������ݣ��¶ȡ�ʪ��
*���룺��
*�����
*       int8_t *tem, �¶�
*       uint8_t *hum, ʪ��
*���أ���
*����˵������
*******************************************************************/
void SHT_SmpSnValue(int8_t *tem, uint8_t *hum)
{
    
    if(SHT_TypeFlag != TYPE_SHT1X)
    {
        etError error;       // error code
        ft      temperature; // temperature [��]
        ft      humidity;    // relative humidity [%RH]
      
        error = NO_ERROR;
        // demonstrate a single shot measurement with polling and 50ms timeout
        error = SHT3X_GetTempAndHumi(&temperature, &humidity, REPEATAB_HIGH, MODE_POLLING, 50);
        if(error != NO_ERROR) // do error handling here
        {
            error = SHT3X_SoftReset();
            // ... if the soft reset fails, do a hard reset
            if(error != NO_ERROR)
            {
                SHT3X_HardReset();
            }
        }
#if (ENGINEER_DEBUG != false)
        printf("SHT3X �¶�=%f��, ʪ��=%f%%\r\n", temperature, humidity);
#endif
        *tem=(int8_t)(((temperature*10)+5)/10);
        *hum=(uint8_t)(((humidity*10)+5)/10);
    }
    else
    {
        int tem_val, hum_val;
        call_sht11(&tem_val, &hum_val);
        *tem=(int8_t)(tem_val);
        *hum=(uint8_t)(hum_val);
#if (ENGINEER_DEBUG != false)
        printf("SHT1X �¶�:%d�� ʪ��:%d%%\r\n", tem_val, hum_val);
#endif
    }
#if (ENGINEER_DEBUG != false)
    float dew_point;
    dew_point=calc_dewpoint((float)(*tem), (float)(*hum)); //calculate dew point
    printf("SHT �¶�:%d�� ʪ��:%d%%\r\n", *tem, *hum);
    printf("¶��Dew Point:%5.1f��\n", dew_point);
#endif
}

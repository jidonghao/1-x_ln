
#include <stdio.h>
#include "sh10.h"
#include "sht1x.h"
#include "sht3x.h"


#define TYPE_SHT1X 0x00
#define TYPE_SHT3X 0x01

static uint8 SHT_TypeFlag = 0x00;//Mean��0-SHT1X, 1-SHT3X

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
*������void SHT_SmpSnValue(int8 *tem, uint8 *hum)
*���ܣ��ɼ����������ݣ��¶ȡ�ʪ��
*���룺��
*�����
*       int8 *tem, �¶�
*       uint8 *hum, ʪ��
*���أ���
*����˵������
*******************************************************************/
void SHT_SmpSnValue(int8 *tem, uint8 *hum)
{
    
    if(SHT_TypeFlag != TYPE_SHT1X)
    {
        etError error;       // error code
        float      temperature; // temperature [��]
        float      humidity;    // relative humidity [%RH]
      
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
        *tem=(int8)(((temperature*10)+5)/10);
        *hum=(uint8)(((humidity*10)+5)/10);
    }
    else
    {
        int tem_val, hum_val;
        call_sht1x(&tem_val, &hum_val);
        *tem=(int8)(tem_val);
        *hum=(uint8)(hum_val);
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

/*******************************************************************
*������void call_sht11(unsigned int *tem, unsigned int *hum)
*���ܣ��ɼ����������ݣ��¶ȡ�ʪ��
*���룺��
*�����
*       unsigned int *tem, �¶�
*       unsigned int *hum, ʪ��
*���أ���
*����˵������
*******************************************************************/
void call_sht11(unsigned int *tem, unsigned int *hum)
{
  int8 sensor_tem;
  uint8 sensor_val;
  SHT_SmpSnValue((int8 *)(&sensor_tem), (uint8 *)(&sensor_val));
  *tem = sensor_tem;
  *hum = sensor_val;
}


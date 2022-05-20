
#include <stdio.h>
#include "sht1x.h"
#include "sht3x.h"

#define TYPE_SHT1X 0x00
#define TYPE_SHT3X 0x01

static uint8_t SHT_TypeFlag = 0x00;//Mean：0-SHT1X, 1-SHT3X

/*******************************************************************
*函数：void SHT_Init(void)
*功能：温湿度传感器初始化
*输入：无
*输出：无
*返回：无
*特殊说明：无
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
        printf("温湿度传感器型号：SHT3X\r\n");
    }
    else
    {
        printf("温湿度传感器型号：SHT1X\r\n");
    }
}

/*******************************************************************
*函数：uint8_t ChkSum(uint8_t *buf, uint8_t len)
*功能：采集传感器数据：温度、湿度
*输入：无
*输出：
*       int8_t *tem, 温度
*       uint8_t *hum, 湿度
*返回：无
*特殊说明：无
*******************************************************************/
void SHT_SmpSnValue(int8_t *tem, uint8_t *hum)
{
    
    if(SHT_TypeFlag != TYPE_SHT1X)
    {
        etError error;       // error code
        ft      temperature; // temperature [℃]
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
        printf("SHT3X 温度=%f℃, 湿度=%f%%\r\n", temperature, humidity);
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
        printf("SHT1X 温度:%d℃ 湿度:%d%%\r\n", tem_val, hum_val);
#endif
    }
#if (ENGINEER_DEBUG != false)
    float dew_point;
    dew_point=calc_dewpoint((float)(*tem), (float)(*hum)); //calculate dew point
    printf("SHT 温度:%d℃ 湿度:%d%%\r\n", *tem, *hum);
    printf("露点Dew Point:%5.1f℃\n", dew_point);
#endif
}

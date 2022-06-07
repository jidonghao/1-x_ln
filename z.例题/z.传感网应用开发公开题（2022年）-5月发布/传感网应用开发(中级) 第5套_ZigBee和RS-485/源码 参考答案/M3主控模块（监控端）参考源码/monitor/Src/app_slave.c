/******************************************************************************
******************************************************************************
* @file    app/app_slave.c
* @author  zhao
* @version V1.0.0
* @date    2019.07.10
* @update  2019.09.14
* @brief   主程序
******************************************************************************
******************************************************************************/

/**************include*************************/

#include  "app_slave.h"
#include "tim.h"
#include  "flash.h"
void modbus_slave_init(void);


u8  SLAVE_ADDRESS =0;



void app_modbus_init(void)
{

    MX_TIM3_Init();

    RS4851_Init(115200);  //初始化RS485
    RS4852_Init(115200);  //初始化RS4852
    RS4853_Init(115200);  //初始化RS4853

    modbus_slave_init();
}

void modbus_slave_init(void)
{
    u8	read_state=0;
    u8  write_value=0;

    bsp_flash_read(FLASH_SAVE_FIRST, &read_state,1);
    if(read_state!=0xAA) {
        //DBG_B_INFO("第一次上电配置 默认传感器模式为1 默认从机地址0x01");
        write_value=0XAA;
        bsp_flash_erase(FLASH_SAVE_FIRST, 1);
        bsp_flash_write(FLASH_SAVE_FIRST,&write_value,1);
        write_value=0X01;
        bsp_flash_erase(FLASH_SAVE_ADDRESS, 1);
        bsp_flash_write(FLASH_SAVE_ADDRESS,&write_value,1);
        write_value=0X01;
        bsp_flash_erase(FLASH_SAVE_SENSOR_STATE, 1);
        bsp_flash_write(FLASH_SAVE_SENSOR_STATE,&write_value,1);
    }

    //开机获取上次传感器保存状态
    bsp_flash_read(FLASH_SAVE_ADDRESS, &read_state,1);
    SLAVE_ADDRESS =read_state;
    //DBG_B_INFO("当前从机地址为状态为 %d ",SLAVE_ADDRESS);



}





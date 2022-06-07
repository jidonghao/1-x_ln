/******************************************************************************
* File Name          : flash.c
* Description        :
******************************************************************************/
#include "flash.h"


#define FLASH_START_ADDRESS         (0x8000000)
#define FLASH_BASE_ADDRESS          (0*1024)    //loader size 0K
#define FLASH_LIMIT_MAX_SIZE        (256*2048)  //flash limit max
#define FLASH_BLOCK_SIZE            (2048)      //2K
#define FLASH_BLOCK_NUMBER          (256)       //256个块 数据

#define MIN_FLASH_ADDRESS = FLASH_START_ADDRESS + FLASH_BASE_ADDRESS;
#define MAX_FLASH_ADDRESS = FLASH_START_ADDRESS + FLASH_LIMIT_MAX_SIZE;

//#define FLASH_PAGE_SIZE  FLASH_BLOCK_SIZE
#define FLASH_MIN_ADDR   MIN_FLASH_ADDRESS
#define FLASH_MAX_ADDR   MAX_FLASH_ADDRESS


/*
* @brief  flash_read
* @param  none
* @note
* @Date:2018.7.27
* @author:zhao
* @return:len
*/
int32_t bsp_flash_read(uint32_t addr,uint8_t* p_dest,int32_t len)
{

    memcpy((void*)p_dest, (void *)addr, len);
    return len;
}

/*
  * @brief  flash_erase
  * @param  none
  * @note
  * @Date:2018.7.27
  * @author:zhao
  * @return:int32_t
*/
int32_t bsp_flash_erase		 (uint32_t address,uint32_t len)
{

    uint32_t NbrOfPage = 0;
    uint32_t ret =0;
    FLASH_EraseInitTypeDef  p_earse;
    HAL_StatusTypeDef flashstatus;
    flashstatus=HAL_FLASH_Unlock();

    if(flashstatus==HAL_OK) {

        NbrOfPage = (len) / FLASH_PAGE_SIZE;
        if(len&(FLASH_PAGE_SIZE-1)) {
            NbrOfPage++;   /*如果字节数大于当前块数*/
        }

        p_earse.PageAddress=address;
        p_earse.NbPages=NbrOfPage;
        p_earse.TypeErase=FLASH_TYPEERASE_PAGES;

        HAL_FLASHEx_Erase(&p_earse,&ret);
    }
    HAL_FLASH_Lock();
    return 0;
}


/*
  * @brief  flash write
  * @param  none
  * @note
  * @Date:2018.7.27
  * @author:zhao
  * @return:len
*/
int32_t bsp_flash_write    (uint32_t addr,uint8_t* pbuf,int32_t len)
{
    uint32_t DATA_32 = 0;
    uint32_t i = 0;
    HAL_StatusTypeDef flashstatus;
    flashstatus=HAL_FLASH_Unlock();

    if(flashstatus==HAL_OK) {
        while (i < len) {
            if(len>(i+4)) {
                memcpy((void*)&DATA_32,(void*)&pbuf[i],sizeof(uint32_t));
            } else          {
                memcpy((void*)&DATA_32,(void*)&pbuf[i],(len-i) );
            }

            if (HAL_FLASH_Program(FLASH_TYPEPROGRAM_HALFWORD,addr+i, DATA_32) ==HAL_OK ) {
                i+=4;
            } else {
                /* Error occurred while writing data in Flash memory. User can add here some code to deal with this error */
                //   DBG_E("bsp_flash_write error:0x%x ",addr+i);
                while (1);
            }
        }
    }
    HAL_FLASH_Lock();
    /* Lock the Flash to disable the flash control register access (recommended
     to protect the FLASH memory against possible unwanted operation) *********/

    return len;
}



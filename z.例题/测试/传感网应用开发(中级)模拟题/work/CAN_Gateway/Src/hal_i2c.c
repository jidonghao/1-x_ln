//=============================================================================
//    S E N S I R I O N
//=============================================================================
// Project   :  SHT3x Sample Code (V1.1)
// File      :  i2c_hal.c (V1.1)
// Brief     :  I2C驱动程序
//=============================================================================

//-- Includes -----------------------------------------------------------------
#include "hal_i2c.h"

//-- Defines ------------------------------------------------------------------
// I2C IO-Pins                        /* -- adapt the defines for your uC -- */
#define I2C_SCL_Pin GPIO_PIN_6
#define I2C_SCL_Port GPIOB
#define I2C_SDA_Pin GPIO_PIN_7
#define I2C_SDA_Port GPIOB

// SDA
#define SDA_LOW()  (HAL_GPIO_WritePin(I2C_SDA_Port, I2C_SDA_Pin, GPIO_PIN_RESET)) // set SDA to low 置低电平
#define SDA_OPEN() (HAL_GPIO_WritePin(I2C_SDA_Port, I2C_SDA_Pin, GPIO_PIN_SET))   // set SDA to open-drain 置高电平
#define SDA_READ   ((u8t)HAL_GPIO_ReadPin(I2C_SDA_Port, I2C_SDA_Pin))     // read SDA

// SCL /* -- adapt the defines for your uC -- */
#define SCL_LOW()  (HAL_GPIO_WritePin(I2C_SCL_Port, I2C_SCL_Pin, GPIO_PIN_RESET)) // set SCL to low 置低电平
#define SCL_OPEN() (HAL_GPIO_WritePin(I2C_SCL_Port, I2C_SCL_Pin, GPIO_PIN_SET))   // set SCL to open-drain 置高电平
#define SCL_READ   ((u8t)HAL_GPIO_ReadPin(I2C_SCL_Port, I2C_SCL_Pin))     // read SCL

#define I2C_INPUT  1//置成输入
#define I2C_OUTPUT 0//置成输出

//-- Static function prototypes -----------------------------------------------
static etError I2c_WaitWhileClockStreching(u8t timeout);

//-----------------------------------------------------------------------------
void I2c_sdaInOutInit(u8t InOut)                      /* -- 根据硬件平台进行修改，初始化SDA引脚，InOut-1输入，InOut-0输出 -- */
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  
  /*Configure GPIO pins*/
  GPIO_InitStruct.Pin = I2C_SDA_Pin;
  if(InOut != 1)
  {
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD;
  }
  else
  {
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  }
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(I2C_SDA_Port, &GPIO_InitStruct);
}

void I2c_SclInOutInit(u8t InOut)                      /* -- 根据硬件平台进行修改，初始化SCL引脚，InOut-1输入，InOut-0输出 -- */
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  
  /*Configure GPIO pins*/
  GPIO_InitStruct.Pin = I2C_SCL_Pin;
  if(InOut != 1)
  {
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD;
  }
  else
  {
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  }
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(I2C_SCL_Port, &GPIO_InitStruct);
}

void I2c_Init(void)                      /* -- adapt the init for your uC -- */
{
#if 0
    //初始化I2C总线IO口SDA、SCL
    ;
    ;
    SDA_OPEN();                  // I2C-bus idle mode SDA released
    SCL_OPEN();                  // I2C-bus idle mode SCL released

    // SDA
    // SCL
    // set open-drain output for SDA and SCL 
    //SDA、SCL设置为漏极开路
    ;
    ;
#else
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  
  __HAL_RCC_GPIOB_CLK_ENABLE();
  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, I2C_SCL_Pin|I2C_SDA_Pin, GPIO_PIN_SET);

  /*Configure GPIO pins : PBPin PBPin */
  GPIO_InitStruct.Pin = I2C_SCL_Pin|I2C_SDA_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
#endif
}

//-----------------------------------------------------------------------------
void I2c_StartCondition(void)
{
    SDA_OPEN();
    DelayMicroSeconds(1);
    SCL_OPEN();
    DelayMicroSeconds(1);
    SDA_LOW();
    DelayMicroSeconds(10);  // hold time start condition (t_HD;STA)
    SCL_LOW();
    DelayMicroSeconds(10);
}

//-----------------------------------------------------------------------------
void I2c_StopCondition(void)
{
    SCL_LOW();
    DelayMicroSeconds(1);
    SDA_LOW();
    DelayMicroSeconds(1);
    SCL_OPEN();
    DelayMicroSeconds(10);  // set-up time stop condition (t_SU;STO)
    SDA_OPEN();
    DelayMicroSeconds(10);
}

//-----------------------------------------------------------------------------
etError I2c_WriteByte(u8t txByte)
{
    etError error = NO_ERROR;
    u8t     mask;
    for(mask = 0x80; mask > 0; mask >>= 1)// shift bit for masking (8 times)
    {
        if((mask & txByte) == 0) SDA_LOW(); // masking txByte, write bit to SDA-Line
        else                     SDA_OPEN();
        DelayMicroSeconds(1);               // data set-up time (t_SU;DAT)
        SCL_OPEN();                         // generate clock pulse on SCL
        DelayMicroSeconds(5);               // SCL high time (t_HIGH)
        SCL_LOW();
        DelayMicroSeconds(1);               // data hold time(t_HD;DAT)
    }
    SDA_OPEN();                           // release SDA-line
    SCL_OPEN();                           // clk #9 for ack
    DelayMicroSeconds(1);                 // data set-up time (t_SU;DAT)
    I2c_sdaInOutInit(I2C_INPUT);          //SDA置为输入
    if(SDA_READ) error = ACK_ERROR;       // check ack from i2c slave
    SCL_LOW();
    DelayMicroSeconds(20);                // wait to see byte package on scope
    I2c_sdaInOutInit(I2C_OUTPUT);         //SDA置为输出
    return error;                         // return error code
}

//-----------------------------------------------------------------------------
etError I2c_ReadByte(u8t *rxByte, etI2cAck ack, u8t timeout)
{
    etError error = NO_ERROR;
    u8t mask;
    *rxByte = 0x00;
    SDA_OPEN();                            // release SDA-line
    I2c_sdaInOutInit(I2C_INPUT);           //SDA置为输入
    for(mask = 0x80; mask > 0; mask >>= 1) // shift bit for masking (8 times)
    {
        SCL_OPEN();                          // start clock on SCL-line
        DelayMicroSeconds(1);                // clock set-up time (t_SU;CLK)
        error = I2c_WaitWhileClockStreching(timeout);// wait while clock streching
        DelayMicroSeconds(3);                // SCL high time (t_HIGH)
        if(SDA_READ) *rxByte |= mask;        // read bit
        SCL_LOW();
        DelayMicroSeconds(1);                // data hold time(t_HD;DAT)
    }
    I2c_sdaInOutInit(I2C_OUTPUT);          //SDA置为输出
    if(ack == ACK) SDA_LOW();              // send acknowledge if necessary
    else           SDA_OPEN();
    DelayMicroSeconds(1);                  // data set-up time (t_SU;DAT)
    SCL_OPEN();                            // clk #9 for ack
    DelayMicroSeconds(5);                  // SCL high time (t_HIGH)
    SCL_LOW();
    SDA_OPEN();                            // release SDA-line
    DelayMicroSeconds(20);                 // wait to see byte package on scope

    return error;                          // return with no error
}

//-----------------------------------------------------------------------------
etError I2c_GeneralCallReset(void)
{
    etError error;

    I2c_StartCondition();
    error = I2c_WriteByte(0x00);
    if(error == NO_ERROR) error = I2c_WriteByte(0x06);

    return error;
}

//-----------------------------------------------------------------------------
void DelayMicroSeconds(u32t nbrOfUs)   /* -- adapt this delay for your uC -- */
{
    u32t i;
    for(i = 0; i < nbrOfUs; i++)
    {
        __nop();  // nop's may be added or removed for timing adjustment
        __nop();
        __nop();
        __nop();
    }
}

//-----------------------------------------------------------------------------
static etError I2c_WaitWhileClockStreching(u8t timeout)
{
    etError error = NO_ERROR;
    I2c_SclInOutInit(I2C_INPUT);          //SCL置为输入
    while(SCL_READ == 0)
    {
        if(timeout-- == 0) return TIMEOUT_ERROR;
        DelayMicroSeconds(1000);
    }
    I2c_SclInOutInit(I2C_OUTPUT);         //SCL置为输出
    return error;
}

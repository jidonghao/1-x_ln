/***********************************************************************************
* Project:          SHT1x/7x demo program (V2.4)
* Filename:         SHT1x.c
***********************************************************************************/

#include <math.h>          //library 
#include <stdio.h>          //library  
#include <ioCC2530.h>
#include "hal_defs.h"
#include "hal_cc8051.h"
#include "sht1x.h"         // controller register definitions 

#define noACK 0
#define ACK   1
//adr  command  r/w
#define STATUS_REG_W 0x06   //000   0011    0
#define STATUS_REG_R 0x07   //000   0011    1
#define MEASURE_TEMP 0x03   //000   0001    1
#define MEASURE_HUMI 0x05   //000   0010    1
#define RESET        0x1e   //000   1111    0


//-- Defines ------------------------------------------------------------------
// Serial IO-Pins                        /* -- adapt the defines for your uC -- */
/*
I2C????
p1.5-scl
p1.6-sda
*/

#define SHT_IO_DIR_OUTPUT(port, pin)      st( P##port##SEL &= ~BM(pin); P##port##DIR |= BM(pin); )
#define SHT_IO_WRITE(port, pin, val)      st( P##port##_##pin## = val; )
#define SHT_IO_DIR_INPUT(port, pin, func) st( \
                                               P##port##SEL &= ~BM(pin); \
                                               P##port##DIR &= ~BM(pin); \
                                               switch (func) \
                                               { \
                                                 case MCU_IO_PULLUP: P##port##INP &= ~BM(pin); P2INP &= ~BM(port + 5); break; \
                                                 case MCU_IO_PULLDOWN: P##port##INP &= ~BM(pin); P2INP |= BM(port + 5); break; \
                                                 default: P##port##INP |= BM(pin); break; \
                                               } )
#define SHT_IO_READ(port, pin)            (P##port## & BM(pin))

#define SCK_DIR_OUT()                     SHT_IO_DIR_OUTPUT(1, 6)
#define SCK_HIGH()                        SHT_IO_WRITE(1, 6, 1)// set SCL to open-drain ?øߵ?ƽ
#define SCK_LOW()                         SHT_IO_WRITE(1, 6, 0)// set SCL to low ?õ͵?ƽ
#define SCK_DIR_IN()                      SHT_IO_DIR_INPUT(1, 6, 2)
#define SCK_READ()                        SHT_IO_READ(1, 6)     // read SCL
#define SCK_WRITE(n)                      SHT_IO_WRITE(1, 6, n)// SCK = n

#define DATA_DIR_OUT()                    SHT_IO_DIR_OUTPUT(1, 5)
#define DATA_HIGH()                       SHT_IO_WRITE(1, 5, 1)// set SDA to open-drain ?øߵ?ƽ
#define DATA_LOW()                        SHT_IO_WRITE(1, 5, 0)// set SDA to low ?õ͵?ƽ
#define DATA_DIR_IN()                     SHT_IO_DIR_INPUT(1, 5,2)
#define DATA_READ()                       SHT_IO_READ(1, 5) // read SDA
#define DATA_WRITE(n)                     SHT_IO_WRITE(1, 5, n)// DATA = n

#ifndef SERIAL_INPUT
#define SERIAL_INPUT  1//?ó?????
#endif
#ifndef SERIAL_OUTPUT
#define SERIAL_OUTPUT 0//?ó?????
#endif

//-- Static function prototypes -----------------------------------------------
static void Serial_DelayUS(unsigned long nbrOfUs)   /* -- adapt this delay for your uC -- */
{
  unsigned long i,j;
  for(i = 0; i < nbrOfUs; i++)
  {
    for(j = 0; j < 8; j++)
    {
      asm("NOP");  // nop's may be added or removed for timing adjustment
      asm("NOP");  
      asm("NOP");  
      asm("NOP");
    }
  }
}

//-----------------------------------------------------------------------------
void SHT1X_Init(void)                      /* -- ??ʼ??GPIO??SHT1X??ʼ״̬ -- */
{
    Serial_Init();
    s_connectionreset();
    s_softreset();
    Serial_DelayUS((11+5)*1000);//????Ҫ??ʱ?ȴ?11ms??SHT1X?Ż?????
}

//-----------------------------------------------------------------------------
void Serial_DataInOutInit(unsigned char InOut)                      /* -- ????Ӳ??ƽ̨?????޸ģ???ʼ??DATA???ţ?InOut-1???룬InOut-0???? -- */
{
  /*Configure GPIO pins*/
  if(InOut != SERIAL_INPUT)
  {
    DATA_DIR_OUT();
  }
  else
  {
    DATA_DIR_IN();
  }
}

void Serial_SckInOutInit(unsigned char InOut)                      /* -- ????Ӳ??ƽ̨?????޸ģ???ʼ??SCK???ţ?InOut-1???룬InOut-0???? -- */
{
  /*Configure GPIO pins*/
  if(InOut != SERIAL_INPUT)
  {
    SCK_DIR_OUT();
  }
  else
  {
    SCK_DIR_IN();
  }
}

void Serial_Init(void)                      /* -- adapt the init for your uC -- */
{
    //??ʼ??Serial????IO??DATA??SCK
    SCK_DIR_OUT();
    DATA_DIR_OUT();
    
    //SERIAL_SCK|SERIAL_DATA??1
    SCK_HIGH();
    DATA_HIGH();
}

//----------------------------------------------------------------------------------
char s_write_byte(unsigned char value)
//----------------------------------------------------------------------------------
// writes a byte on the Sensibus and checks the acknowledge
{
    unsigned char i,error=0;
    for(i=0x80; i>0; i/=2)           //shift bit for masking
    {
        if (i & value)
            DATA_HIGH();          //masking value with i , write to SENSI-BUS
        else
            DATA_LOW();
        Serial_DelayUS(1);                        //observe setup time
        SCK_HIGH();                          //clk for SENSI-BUS
        Serial_DelayUS(3);        //pulswith approx. 5 us
        SCK_LOW();
        Serial_DelayUS(1);                         //observe hold time
    }
    DATA_HIGH();                           //release DATA-line
    Serial_DelayUS(1);                          //observe setup time
    SCK_HIGH();                            //clk #9 for ack
    Serial_DataInOutInit(SERIAL_INPUT);          //DATA??Ϊ????
    for(i=0; i<0xFF; i++)
    {
      error=DATA_READ();                       //check ack (DATA will be pulled down by SHT11)
      if(error != 1) break;
      Serial_DelayUS(1);
    }
    Serial_DataInOutInit(SERIAL_OUTPUT);         //DATA??Ϊ????
    SCK_LOW();
    return error;                     //error=1 in case of no acknowledge
}

//----------------------------------------------------------------------------------
char s_read_byte(unsigned char ack)
//----------------------------------------------------------------------------------
// reads a byte form the Sensibus and gives an acknowledge in case of "ack=1"
{
    unsigned char i,val=0;
    DATA_HIGH();                           //release DATA-line
    Serial_DataInOutInit(SERIAL_INPUT);          //DATA??Ϊ????
    for (i=0x80; i>0; i/=2)           //shift bit for masking
    {
        SCK_HIGH();                          //clk for SENSI-BUS
        Serial_DelayUS(3);
        if (DATA_READ())                    //read bit
        {val = (val|i);}        
        SCK_LOW();
        Serial_DelayUS(2);
    }
    Serial_DataInOutInit(SERIAL_OUTPUT);         //DATA??Ϊ????
    DATA_WRITE(!ack);                        //in case of "ack==1" pull down DATA-Line
    Serial_DelayUS(1);                          //observe setup time
    SCK_HIGH();                            //clk #9 for ack
    Serial_DelayUS(3);          //pulswith approx. 5 us
    SCK_LOW();
    Serial_DelayUS(1);                          //observe hold time
    DATA_HIGH();                           //release DATA-line
    return val;
}

//----------------------------------------------------------------------------------
void s_transstart(void)
//----------------------------------------------------------------------------------
// generates a transmission start
//       _____         ________
// DATA:      |_______|
//           ___     ___
// SCK : ___|   |___|   |______
{
    DATA_HIGH();
    SCK_LOW();                   //Initial state
    Serial_DelayUS(1);
    SCK_HIGH();
    Serial_DelayUS(1);
    DATA_LOW();
    Serial_DelayUS(1);
    SCK_LOW();
    Serial_DelayUS(3);
    SCK_HIGH();
    Serial_DelayUS(1);
    DATA_HIGH();
    Serial_DelayUS(1);
    SCK_LOW();
}

//----------------------------------------------------------------------------------
void s_connectionreset(void)
//----------------------------------------------------------------------------------
// communication reset: DATA-line=1 and at least 9 SCK cycles followed by transstart
//       _____________________________________________________         ________
// DATA:                                                      |_______|
//          _    _    _    _    _    _    _    _    _        ___     ___
// SCK : __| |__| |__| |__| |__| |__| |__| |__| |__| |______|   |___|   |______
{
    unsigned char i;
    DATA_HIGH();
    SCK_LOW();                    //Initial state
    for(i=0; i<9; i++)                //9 SCK cycles
    {
        Serial_DelayUS(1);
        SCK_HIGH();
        Serial_DelayUS(1);
        SCK_LOW();
    }
    s_transstart();                   //transmission start
}

//----------------------------------------------------------------------------------
char s_softreset(void)
//----------------------------------------------------------------------------------
// resets the sensor by a softreset
{
    unsigned char error=0;
    s_connectionreset();              //reset communication
    error+=s_write_byte(RESET);       //send RESET-command to sensor
    return error;                     //error=1 in case of no response form the sensor
}

//----------------------------------------------------------------------------------
char s_read_statusreg(unsigned char *p_value, unsigned char *p_checksum)
//----------------------------------------------------------------------------------
// reads the status register with checksum (8-bit)
{
    unsigned char error=0;
    s_transstart();                   //transmission start
    error=s_write_byte(STATUS_REG_R); //send command to sensor
    *p_value=s_read_byte(ACK);        //read status register (8-bit)
    *p_checksum=s_read_byte(noACK);   //read checksum (8-bit)
    return error;                     //error=1 in case of no response form the sensor
}

//----------------------------------------------------------------------------------
char s_write_statusreg(unsigned char *p_value)
//----------------------------------------------------------------------------------
// writes the status register with checksum (8-bit)
{
    unsigned char error=0;
    s_transstart();                   //transmission start
    error+=s_write_byte(STATUS_REG_W);//send command to sensor
    error+=s_write_byte(*p_value);    //send value of status register
    return error;                     //error>=1 in case of no response form the sensor
}

//----------------------------------------------------------------------------------
char s_measure(unsigned char *p_value, unsigned char *p_checksum, unsigned char mode)
//----------------------------------------------------------------------------------
// makes a measurement (humidity/temperature) with checksum
{
    unsigned char error=0;
    unsigned int i;

    s_transstart();                   //transmission start
    switch(mode)
    {   //send command to sensor
    case TEMP	:
        error+=s_write_byte(MEASURE_TEMP);
        break;
    case HUMI	:
        error+=s_write_byte(MEASURE_HUMI);
        break;
    default     :
        break;
    }
    Serial_DataInOutInit(SERIAL_INPUT);          //DATA??Ϊ????
    for (i=0; i<65535; i++)
    {
        if(DATA_READ()==0) break; //wait until sensor has finished the measurement
        Serial_DelayUS(3);
    }
    if(DATA_READ()) error+=1;                // or timeout (~2 sec.) is reached
    Serial_DataInOutInit(SERIAL_OUTPUT);         //DATA??Ϊ????
#if(LittleEndian!=true)
    //p_value???ô??˸?ʽ?洢
    *(p_value)  =s_read_byte(ACK);    //read the first byte (MSB)
    *(p_value+1)=s_read_byte(ACK);    //read the second byte (LSB)
#else
    //p_value????С?˸?ʽ?洢
    *(p_value+1)=s_read_byte(ACK);    //read the first byte (MSB)
    *(p_value)  =s_read_byte(ACK);    //read the second byte (LSB)
#endif
    *p_checksum =s_read_byte(noACK);  //read checksum
    return error;
}

//----------------------------------------------------------------------------------------
void calc_sth1x(float *p_humidity ,float *p_temperature)
//----------------------------------------------------------------------------------------
// calculates temperature [?C] and humidity [%RH]
// input :  humi [Ticks] (12 bit)
//          temp [Ticks] (14 bit)
// output:  humi [%RH]
//          temp [?C]
{
    const float C1=-2.0468;           // for 12 Bit RH
    const float C2=+0.0367;           // for 12 Bit RH
    const float C3=-0.0000015955;     // for 12 Bit RH
    const float T1=+0.01;             // for 12 Bit RH
    const float T2=+0.00008;          // for 12 Bit RH

    float rh=*p_humidity;             // rh:      Humidity [Ticks] 12 Bit
    float t=*p_temperature;           // t:       Temperature [Ticks] 14 Bit
    float rh_lin;                     // rh_lin:  Humidity linear
    float rh_true;                    // rh_true: Temperature compensated humidity
    float t_C;                        // t_C   :  Temperature [?C]

    t_C=t*0.01 - 40.1;                //calc. temperature [?C] from 14 bit temp. ticks @ 5V
    rh_lin=C3*rh*rh + C2*rh + C1;     //calc. humidity from ticks to [%RH]
    rh_true=(t_C-25)*(T1+T2*rh)+rh_lin;   //calc. temperature compensated humidity [%RH]
    if(rh_true>100)rh_true=100;       //cut if the value is outside of
    if(rh_true<0.1)rh_true=0.1;       //the physical possible range

    *p_temperature=t_C;               //return temperature [?C]
    *p_humidity=rh_true;              //return humidity[%RH]
}

//--------------------------------------------------------------------
float calc_dewpoint(float h,float t)
//--------------------------------------------------------------------
// calculates dew point
// input:   humidity [%RH], temperature [?C]
// output:  dew point [?C]
{
    float k,dew_point ;

    k = (log10(h)-2)/0.4343 + (17.62*t)/(243.12+t);
    dew_point = 243.12*k/(17.62-k);
    return dew_point;
}

//----------------------------------------------------------------------------------
void call_sht1x(int *tem_val, int *hum_val)
//----------------------------------------------------------------------------------
// ??ȡSH1x??ʪ??ֵ
//      *tem_val:?¶ȴ??ŵ?ַ
//      *hum_val:ʪ?ȴ??ŵ?ַ
// ????????????
// 1. connection reset
// 2. measure humidity [ticks](12 bit) and temperature [ticks](14 bit)
// 3. calculate humidity [%RH] and temperature [?C]
// 4. calculate dew point [?C]
// 5. end
{   
#define xDewPoint
  
    value humi_val,temp_val;
#ifdef DewPoint
    float dew_point;
#endif
    unsigned char error,checksum;

    s_connectionreset();
    error=0;
    error+=s_measure((unsigned char*) &humi_val.i,&checksum,HUMI);  //measure humidity
    error+=s_measure((unsigned char*) &temp_val.i,&checksum,TEMP);  //measure temperature
    if(error!=0) 
    {
        s_connectionreset();                 //in case of an error: connection reset
    }
    else
    {   humi_val.f=(float)humi_val.i;                   //converts integer to float
        temp_val.f=(float)temp_val.i;                   //converts integer to float
        calc_sth1x(&humi_val.f,&temp_val.f);            //calculate humidity, temperature
#ifdef DewPoint
        dew_point=calc_dewpoint(humi_val.f,temp_val.f); //calculate dew point
        printf("Temp:%5.1f?? Humi:%5.1f%% Dew Point:%5.1f??\n",temp_val.f,humi_val.f,dew_point);
#endif
    }
    //????????
    *tem_val=(int)(((temp_val.f*10)+5)/10);
    *hum_val=(int)(((humi_val.f*10)+5)/10);
}

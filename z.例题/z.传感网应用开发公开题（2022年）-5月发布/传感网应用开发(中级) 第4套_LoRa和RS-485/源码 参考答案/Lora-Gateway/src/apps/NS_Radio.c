/**********************************************************************************************************
*
*
*
*
**********************************************************************************************************/
#include <string.h>
#include "board.h"
#include "radio.h"
#include "NS_Radio.h"

static uint16_t LoRaBufferSize = BUFFER_SIZE;
static uint8_t LoRaBuffer[BUFFER_SIZE];
static int8_t RssiValue = 0;
static int8_t SnrValue = 0;

/*!
 * Radio events function pointer
 */
static RadioEvents_t RadioEvents;

/**********************************************************************************************
*������void NS_RadioEventsInit( void )
*���ܣ�����Ƶģ���¼��ص��������г�ʼ��
*���룺��
*�������
*����˵������
**********************************************************************************************/
void NS_RadioEventsInit( void )
{
    // Radio initialization
    RadioEvents.TxDone = OnTxDone;
    RadioEvents.RxDone = OnRxDone;
    RadioEvents.TxTimeout = OnTxTimeout;
    RadioEvents.RxTimeout = OnRxTimeout;
    RadioEvents.RxError = OnRxError;

    Radio.Init( &RadioEvents );
}


/**********************************************************************************************
*������void NS_RadioInit( uint32_t freq, int8_t power, uint32_t txTimeout )
*���ܣ�����Ƶģ�鷢�͡����ղ������г�ʼ��
*���룺
*			freq�ز�Ƶ�ʣ�����433MHz����Ƶ�����
*			power���书�ʣ����20
*			txTimeout���ͳ�ʱʱ��
*�������
*����˵������
**********************************************************************************************/
void NS_RadioSetTxRxConfig( uint32_t freq, int8_t power, uint32_t txTimeout )
{
    // Radio initialization
    Radio.SetChannel( freq );
    /*!
     * \brief Sets the transmission parameters
     *
     * \param [IN] modem        Radio modem to be used [0: FSK, 1: LoRa]
     * \param [IN] power        Sets the output power [dBm]
     * \param [IN] fdev         Sets the frequency deviation (FSK only)
     *                          FSK : [Hz]
     *                          LoRa: 0
     * \param [IN] bandwidth    Sets the bandwidth (LoRa only)
     *                          FSK : 0
     *                          LoRa: [0: 125 kHz, 1: 250 kHz,
     *                                 2: 500 kHz, 3: Reserved]
     * \param [IN] datarate     Sets the Datarate
     *                          FSK : 600..300000 bits/s
     *                          LoRa: [6: 64, 7: 128, 8: 256, 9: 512,
     *                                10: 1024, 11: 2048, 12: 4096  chips]
     * \param [IN] coderate     Sets the coding rate (LoRa only)
     *                          FSK : N/A ( set to 0 )
     *                          LoRa: [1: 4/5, 2: 4/6, 3: 4/7, 4: 4/8]
     * \param [IN] preambleLen  Sets the preamble length
     *                          FSK : Number of bytes
     *                          LoRa: Length in symbols (the hardware adds 4 more symbols)
     * \param [IN] fixLen       Fixed length packets [0: variable, 1: fixed]
     * \param [IN] crcOn        Enables disables the CRC [0: OFF, 1: ON]
     * \param [IN] FreqHopOn    Enables disables the intra-packet frequency hopping
     *                          FSK : N/A ( set to 0 )
     *                          LoRa: [0: OFF, 1: ON]
     * \param [IN] HopPeriod    Number of symbols between each hop
     *                          FSK : N/A ( set to 0 )
     *                          LoRa: Number of symbols
     * \param [IN] iqInverted   Inverts IQ signals (LoRa only)
     *                          FSK : N/A ( set to 0 )
     *                          LoRa: [0: not inverted, 1: inverted]
     * \param [IN] timeout      Transmission timeout [ms]
     */
    Radio.SetTxConfig( MODEM_LORA, power, 0, LORA_BANDWIDTH,
                       LORA_SPREADING_FACTOR, LORA_CODINGRATE,
                       LORA_PREAMBLE_LENGTH, LORA_FIX_LENGTH_PAYLOAD_ON,
                       true, 0, 0, LORA_IQ_INVERSION_ON, txTimeout/*TX_TIMEOUT_VALUE*/ );
    /*!
     * \brief Sets the reception parameters
     *
     * \param [IN] modem        Radio modem to be used [0: FSK, 1: LoRa]
     * \param [IN] bandwidth    Sets the bandwidth
     *                          FSK : >= 2600 and <= 250000 Hz
     *                          LoRa: [0: 125 kHz, 1: 250 kHz,
     *                                 2: 500 kHz, 3: Reserved]
     * \param [IN] datarate     Sets the Datarate
     *                          FSK : 600..300000 bits/s
     *                          LoRa: [6: 64, 7: 128, 8: 256, 9: 512,
     *                                10: 1024, 11: 2048, 12: 4096  chips]
     * \param [IN] coderate     Sets the coding rate (LoRa only)
     *                          FSK : N/A ( set to 0 )
     *                          LoRa: [1: 4/5, 2: 4/6, 3: 4/7, 4: 4/8]
     * \param [IN] bandwidthAfc Sets the AFC Bandwidth (FSK only)
     *                          FSK : >= 2600 and <= 250000 Hz
     *                          LoRa: N/A ( set to 0 )
     * \param [IN] preambleLen  Sets the Preamble length
     *                          FSK : Number of bytes
     *                          LoRa: Length in symbols (the hardware adds 4 more symbols)
     * \param [IN] symbTimeout  Sets the RxSingle timeout value
     *                          FSK : timeout in number of bytes
     *                          LoRa: timeout in symbols
     * \param [IN] fixLen       Fixed length packets [0: variable, 1: fixed]
     * \param [IN] payloadLen   Sets payload length when fixed length is used
     * \param [IN] crcOn        Enables/Disables the CRC [0: OFF, 1: ON]
     * \param [IN] FreqHopOn    Enables disables the intra-packet frequency hopping
     *                          FSK : N/A ( set to 0 )
     *                          LoRa: [0: OFF, 1: ON]
     * \param [IN] HopPeriod    Number of symbols between each hop
     *                          FSK : N/A ( set to 0 )
     *                          LoRa: Number of symbols
     * \param [IN] iqInverted   Inverts IQ signals (LoRa only)
     *                          FSK : N/A ( set to 0 )
     *                          LoRa: [0: not inverted, 1: inverted]
     * \param [IN] rxContinuous Sets the reception in continuous mode
     *                          [false: single mode, true: continuous mode]
     */
    Radio.SetRxConfig( MODEM_LORA, LORA_BANDWIDTH, LORA_SPREADING_FACTOR,
                       LORA_CODINGRATE, 0, LORA_PREAMBLE_LENGTH,
                       LORA_SYMBOL_TIMEOUT, LORA_FIX_LENGTH_PAYLOAD_ON,
                       0, true, 0, 0, LORA_IQ_INVERSION_ON, true );
}

/**********************************************************************************************
*������void NS_RadioInit( uint32_t freq, int8_t power, uint32_t txTimeout, uint32_t rxTimeout)
*���ܣ�����Ƶģ�鷢�͡����ղ������г�ʼ��
*���룺
*			freq�ز�Ƶ�ʣ�����433MHz����Ƶ�����
*			power���书�ʣ����20
*			txTimeout���ͳ�ʱʱ��
*			uint32_t rxTimeout ���ճ�ʱʱ��
*�������
*����˵������
**********************************************************************************************/
void NS_RadioInit( uint32_t freq, int8_t power, uint32_t txTimeout, uint32_t rxTimeout)
{
    NS_RadioEventsInit();//������Ƶģ��ע���¼��ص�����
    NS_RadioSetTxRxConfig( freq, power, txTimeout );
    Radio.Rx( rxTimeout );//����LoRaģ��Ϊ����ģʽ��ͬʱ���ó�ʱʱ�䣬������
}

/**********************************************************************************************
*������uint16_t ReadRadioRxBuffer( uint8_t *payload)
*���ܣ���ȡ��Ƶ���ջ�����Ϣ��������ȡ���յ������ݳ��Ⱥͻ������ݡ�
*���룺��
*�����uint8_t *payload, ָ�룬ָ��Ҫ���ڴ�Ž��ջ���ĵ�ַ
*���أ����ؽ��յ������ݳ���
*����˵����ÿ����һ��ReadRadioRxBuffer( )������LoRaBufferSize�ᱻ��0.
**********************************************************************************************/
uint16_t ReadRadioRxBuffer( uint8_t *payload)
{
    uint16_t LengthTemp;
    LengthTemp = LoRaBufferSize;
    if(LengthTemp <= 0)
    {
        return 0;
    }
    LoRaBufferSize = 0;
//    payload = LoRaBuffer;
    memcpy( payload, LoRaBuffer, LengthTemp );
    return LengthTemp;
}

/**********************************************************************************************
*������void ReadRadioRssiSnr( int16_t *rssi, int8_t *snr )
*���ܣ���ȡ��Ƶ�ź�ǿ�Ⱥ������
*���룺��
*�����int16_t *rssi,�ź�ǿ�� int8_t *snr�����
*���أ���
*����˵������
**********************************************************************************************/
void ReadRadioRssiSnr( int16_t *rssi, int8_t *snr )
{
    *rssi = RssiValue;
    *snr = SnrValue;
}

/*!
 * \brief Function to be executed on Radio Tx Done event
 */
void OnTxDone( void )
{
    Radio.Sleep( );
    Radio.Rx( RX_TIMEOUT_VALUE );
}

/*!
 * \brief Function to be executed on Radio Rx Done event
 */
void OnRxDone( uint8_t *payload, uint16_t size, int16_t rssi, int8_t snr )
{
    Radio.Sleep( );
    LoRaBufferSize = size;
    memcpy( LoRaBuffer, payload, LoRaBufferSize );
    RssiValue = rssi;
    SnrValue = snr;
//    printf( "Rx=%s\r\nRssiValue=%d\r\nSnrValue=%d\r\n",LoRaBuffer,RssiValue,SnrValue );
    Radio.Rx( RX_TIMEOUT_VALUE );
}

/*!
 * \brief Function executed on Radio Tx Timeout event
 */
void OnTxTimeout( void )
{
    Radio.Sleep( );
    Radio.Rx( RX_TIMEOUT_VALUE );
}

/*!
 * \brief Function executed on Radio Rx Timeout event
 */
void OnRxTimeout( void )
{
    Radio.Sleep( );
    Radio.Rx( RX_TIMEOUT_VALUE );
}

/*!
 * \brief Function executed on Radio Rx Error event
 */
void OnRxError( void )
{
    Radio.Sleep( );
    Radio.Rx( RX_TIMEOUT_VALUE );
}

/**********************************************************************************************************
*
*
*
*
**********************************************************************************************************/
#ifndef _NS_RADIO_H_
#define _NS_RADIO_H_

#include "board.h"
#include "radio.h"

#define RF_PING_PONG_FREQUENCY                      433300000 // Hz
#define TX_OUTPUT_POWER                             14        // dBm
#define LORA_BANDWIDTH                              0         // [0: 125 kHz,
                                                              //  1: 250 kHz,
                                                              //  2: 500 kHz,
                                                              //  3: Reserved]
#define LORA_SPREADING_FACTOR                       7         // [SF7..SF12]
#define LORA_CODINGRATE                             3         // [1: 4/5,
                                                              //  2: 4/6,
                                                              //  3: 4/7,
                                                              //  4: 4/8]
#define LORA_PREAMBLE_LENGTH                        8         // Same for Tx and Rx
#define LORA_SYMBOL_TIMEOUT                         5         // Symbols
#define LORA_FIX_LENGTH_PAYLOAD_ON                  false
#define LORA_IQ_INVERSION_ON                        false
#define TX_TIMEOUT_VALUE                            (1000)	//��Ԫ����906bps��113.25byte/s��128�ֽڴ�����Ҫ1S����
#define RX_TIMEOUT_VALUE                            (1000)	//��Ԫ����906bps��113.25byte/s��128�ֽڴ�����Ҫ1S����
#define BUFFER_SIZE                                 255			// Define the payload size here

/*!
 * Radio events function pointer
 */
static RadioEvents_t RadioEvents;

/*!
 * \brief Function to be executed on Radio Tx Done event
 */
void OnTxDone( void );

/*!
 * \brief Function to be executed on Radio Rx Done event
 */
void OnRxDone( uint8_t *payload, uint16_t size, int16_t rssi, int8_t snr );

/*!
 * \brief Function executed on Radio Tx Timeout event
 */
void OnTxTimeout( void );

/*!
 * \brief Function executed on Radio Rx Timeout event
 */
void OnRxTimeout( void );

/*!
 * \brief Function executed on Radio Rx Error event
 */
void OnRxError( void );

/**********************************************************************************************
*������void NS_RadioEventsInit( void )
*���ܣ�����Ƶģ���¼��ص��������г�ʼ��
*���룺��
*�������
*����˵������
**********************************************************************************************/
void NS_RadioEventsInit( void );

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
void NS_RadioSetTxRxConfig( uint32_t freq, int8_t power, uint32_t txTimeout );

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
void NS_RadioInit( uint32_t freq, int8_t power, uint32_t txTimeout, uint32_t rxTimeout);

/**********************************************************************************************
*������uint16_t ReadRadioRxBuffer( uint8_t *payload)
*���ܣ���ȡ��Ƶ���ջ�����Ϣ��������ȡ���յ������ݳ��Ⱥͻ������ݡ�
*���룺��
*�����uint8_t *payload, ָ�룬ָ��Ҫ���ڴ�Ž��ջ���ĵ�ַ
*���أ����ؽ��յ������ݳ���
*����˵����ÿ����һ��ReadRadioRxBuffer( )������LoRaBufferSize�ᱻ��0.
**********************************************************************************************/
uint16_t ReadRadioRxBuffer( uint8_t *payload);

/**********************************************************************************************
*������void ReadRadioRssiSnr( int16_t *rssi, int8_t *snr )
*���ܣ���ȡ��Ƶ�ź�ǿ�Ⱥ������
*���룺��
*�����int16_t *rssi,�ź�ǿ�� int8_t *snr�����
*���أ���
*����˵������
**********************************************************************************************/
void ReadRadioRssiSnr( int16_t *rssi, int8_t *snr );

#endif /*_NS_RADIO_H_*/

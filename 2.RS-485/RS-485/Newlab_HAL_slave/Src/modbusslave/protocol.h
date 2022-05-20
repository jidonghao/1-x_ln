#ifndef __PROTOCOL_H
#define __PROTOCOL_H


#include "sys.h"
#include "..\..\drivers\485\max485.h"
#include "mcheck.h"

#include "inputregister.h"

#include "holdregister.h"
#include "funcdisc.h"

//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////

//#define  SLAVE_ADDRESS  0x02

//#define  SLAVE_ADDRESS  0x01


#define M_MAX_FRAME_LENGTH	261		//���֡����Ϊ261�ֽ� ��ַ+����+֡��+���ݳ���+����+CRC(CRC16��2�ֽ�)
#define M_MIN_FRAME_LENGTH	5		//��С֡����Ϊ5�ֽ� ��ַ+����+֡��+���ݳ���+CRC8/XOR/SUM



//У�鷽ʽ�궨��
#define M_FRAME_CHECK_SUM	0		//У���
#define M_FRAME_CHECK_XOR	1		//���У��
#define M_FRAME_CHECK_CRC8	2		//CRC8У��
#define M_FRAME_CHECK_CRC16	3		//CRC16У��


//���ؽ�����������Ͷ���
typedef enum {
    MR_OK=0,						//����
    MR_FRAME_FORMAT_ERR = 1,		//֡��ʽ����
    MR_FRAME_CHECK_ERR = 2,			//У��ֵ��λ
    MR_FRAME_SLAVE_ADDRESS=3,       //֡��ַ����
    MR_FRANE_ILLEGAL_FUNCTION =4,   //���������
    MR_FRAME_ILLEGAL_DATA=5,        //֡���ݴ���
    MR_MEMORY_ERR = 6,				//�ڴ����
} m_result;

//��modbus֡����
__packed typedef struct {

    u8 address;						//�豸��ַ��0���㲥��ַ��1~255���豸��ַ��
    u8 function;					//֡���ܣ�0~255
    u8 count;						//֡���
    u8 datalen;						//��Ч���ݳ���
    u8 *data;						//���ݴ洢��
    u16 chkval;						//У��ֵ
} m_frame_typedef;


//��modbusЭ�������
typedef  struct {
    u8* rxbuf;						//���ջ�����
    u16 rxlen;						//�������ݵĳ���
    u8 frameok;						//һ֡���ݽ�����ɱ�ǣ�0����û��ɣ�1�������һ֡���ݵĽ���
    u8 checkmode;					//У��ģʽ��0,У���;1,���;2,CRC8;3,CRC16
} m_protocol_dev_typedef;

extern m_protocol_dev_typedef m_ctrl_dev;			//����modbus������

m_result mb_unpack_frame(m_frame_typedef *fx);		//����һ֡����
void mb_packsend_frame(m_frame_typedef *fx);		//���һ֡���ݣ�������
m_result mb_init(u8 checkmode);						//��ʼ��MODBUS,�����ڴ�
void mb_destroy(void);								//����MODBUS,�ͷ��ڴ�




#endif














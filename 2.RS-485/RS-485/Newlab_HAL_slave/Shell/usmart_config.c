#include "usmart.h"
#include "usmart_str.h"
////////////////////////////�û�������///////////////////////////////////////////////
//������Ҫ�������õ��ĺ�����������ͷ�ļ�(�û��Լ����)
//#include "delay.h"
#include "sys.h"

#include "..\Src\TEST\app_test.h"

//�������б��ʼ��(�û��Լ����)
//�û�ֱ������������Ҫִ�еĺ�����������Ҵ�
struct _m_usmart_nametab usmart_nametab[]= {
#if USMART_USE_WRFUNS==1 	//���ʹ���˶�д����
    (void*)read_addr,"u32 read_addr(u32 addr)",
    (void*)write_addr,"void write_addr(u32 addr,u32 val)",
#endif
    (void*)RS_485test,"void RS_485test(u8 cmd)",
    (void*)modbus_slave_test,"void modbus_slave_test(void)",
    (void*)shell_485_connect,"void shell_485_connect(void)",
    (void*)shell_485_set_address,"void shell_485_set_address(u16 address)",
    (void*)shell_485_set_mode,"void shell_485_set_mode(u16 mode)",
    (void*)shell_485_set,"void  shell_485_set(u16 mode,u16 address)",
    (void*)shell_485_check,"void shell_485_check(void)",

};
///////////////////////////////////END///////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
//�������ƹ�������ʼ��
//�õ������ܿغ���������
//�õ�����������
struct _m_usmart_dev usmart_dev= {
    usmart_nametab,
    usmart_init,
    usmart_cmd_rec,
    usmart_exe,
    usmart_scan,
    sizeof(usmart_nametab)/sizeof(struct _m_usmart_nametab),//��������
    0,	  	//��������
    0,	 	//����ID
    1,		//������ʾ����,0,10����;1,16����
    0,		//��������.bitx:,0,����;1,�ַ���
    0,	  	//ÿ�������ĳ����ݴ��,��ҪMAX_PARM��0��ʼ��
    0,		//�����Ĳ���,��ҪPARM_LEN��0��ʼ��
};




















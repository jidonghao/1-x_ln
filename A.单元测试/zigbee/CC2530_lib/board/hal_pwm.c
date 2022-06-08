
#include "hal_defs.h"
#include "hal_cc8051.h"
#include "hal_mcu.h"

/**********************************************************************************************
*������void TIM1_PwmInit(uint16 period, uint8 ration)
*���ܣ������PWM��������period���룬ռ�ձ�Ϊ�ٷ�֮ration
*���룺uint16 period-���ڣ���λ������, uint8 ration-ռ�ձȣ���λ��%
*�������
*���أ���
*����˵������
**********************************************************************************************/
void TIM1_PwmInit(uint16 period, uint8 ration)
{
    uint16 TimPeriod = 0;
    uint16 TimComp = 0;
//    CLKCONCMD &= ~0x40; //����ϵͳʱ��ԴΪ32MHZ�ľ���
//    while(CLKCONSTA & 0x40); //�ȴ������ȶ�Ϊ32MHZ
//    CLKCONCMD &= ~0x07;     //����ϵͳ��ʱ��Ƶ��Ϊ32MHZ
    CLKCONCMD |= 0x38;      //��ʱ��������Ϊ250KHZ

    //��ʱ��ͨ������
    P1SEL |= 0x01;         //��ʱ��1ͨ��2ӳ����P1_0������ѡ��
    PERCFG |= 0x40;        //����λ��2��˵����Ϣ
    P2SEL &= ~0x10;        //�����Timer4����ʱ��1����
    P2DIR |= 0xC0;         //��ʱ��ͨ��2-3���е�һ���ȼ�
    P1DIR |= 0x01;

    //��ʱ��ģʽ����
    T1CTL = 0x02;          //250KHZ����Ƶ��ģģʽ
    //�˴�P1_0�ڱ���װ��ʱ��1ͨ��2���бȽ�
    T1CCTL2 = 0x24;       //�����ϱȽ�����������0���ã�����Ƚ�ֵʱ������
    //װ��ʱ��ͨ��0��ֵ
    TimPeriod = period*250;//����TimPeriod���룬��λ��ms
    T1CC0H = (uint8)(TimPeriod>>8);
    T1CC0L = (uint8)TimPeriod;        //PWM�ź�����Ϊ1ms��Ƶ��Ϊ1KHZ
    //װ��ʱ��ͨ��2�Ƚ�ֵ
    TimComp = ration*TimPeriod/100;//��ռ�ձ����ɱȽ�ֵ
    T1CC2H = (uint8)(TimComp>>8);
    T1CC2L = (uint8)TimComp;
}

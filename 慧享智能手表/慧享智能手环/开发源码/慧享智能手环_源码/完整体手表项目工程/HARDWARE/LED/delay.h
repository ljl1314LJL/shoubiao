#ifndef DELAY_H
#define DELAY_H

#include "stm32f4xx.h" 
/**
  * @brief  �δ�ʱ��΢����ʱ�������������������
  * @param  nTime: ����ʱ��΢��ʱ��
  * @retval None
  * @note   
  *		1.������������798915us���������������ֵ����������ʧ�ܣ���Ϊ�Ĵ������������ĺ���������ֵд�ص����ؼĴ���
  *		2.�ڲ�ʱ��:FCLK����������ʱ�ӣ�Ƶ��168MHz��
  *		3.�ⲿʱ��:STCLK����������ʱ�ӣ���AHB���ߵ�ʱ��Ƶ�ʽ���8��Ƶ������168MHZ /8 = 21MHZ
  *		4.�δ�ʱ��ֻ֧�ֵݼ�����
  */
extern void delay_us(__IO uint32_t nTime);


/**
  * @brief  �δ�ʱ��������ʱ�������������������
  * @param  nTime: ����ʱ��΢��ʱ��
  * @retval None
  * @note   ������������798ms���������������ֵ����������ʧ�ܣ���Ϊ�Ĵ������������ĺ���������ֵд�ص����ؼĴ���
  */
extern void delay_ms(__IO uint32_t nTime);


/**
  * @brief  ����ʱ�������������������
  * @param  nTime: ����ʱ��΢��ʱ��
  * @retval None
  * @note   Ҫ��ǰ�������ʱ��������ʹ��
  */
extern void delay_s(__IO uint32_t nTime);



#endif

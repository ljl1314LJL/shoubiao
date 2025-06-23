#ifndef UARTAHT11_H
#define UARTAHT11_H

#include "stm32f4xx.h" //�������
#include <stdbool.h>
#include "UART.h" 
#include "delay.h"


/**
  * @brief  DHT11��ȡ��ʪ�ȣ�ͨ��У�鷵��true��ʧ�ܷ���false
	* @param  dhtbuf:�����ʪ�ȵ�5���ֽ�
  * @retval None
  * @note   None
  */
extern bool DHT11_ReadData(uint8_t *dhtbuf);

/**
  * @brief  DHT11��ȡ��ʪ�ȵ�һ������
	* @param  None
  * @retval None
  * @note   1.NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);�ǵ����ȼ�����
  */
extern void dht11_example(void);

extern void DHT11_Config(void);

#endif

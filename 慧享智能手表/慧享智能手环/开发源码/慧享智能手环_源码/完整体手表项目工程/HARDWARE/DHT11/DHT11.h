#ifndef UARTAHT11_H
#define UARTAHT11_H

#include "stm32f4xx.h" //必须包含
#include <stdbool.h>
#include "UART.h" 
#include "delay.h"


/**
  * @brief  DHT11读取温湿度，通过校验返回true，失败返回false
	* @param  dhtbuf:存放温湿度的5个字节
  * @retval None
  * @note   None
  */
extern bool DHT11_ReadData(uint8_t *dhtbuf);

/**
  * @brief  DHT11读取温湿度的一个案例
	* @param  None
  * @retval None
  * @note   1.NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);记得优先级分组
  */
extern void dht11_example(void);

extern void DHT11_Config(void);

#endif

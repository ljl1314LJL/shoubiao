#ifndef DELAY_H
#define DELAY_H

#include "stm32f4xx.h" 
/**
  * @brief  滴答定时器微秒延时函数，适用于裸机开发
  * @param  nTime: 待延时的微秒时间
  * @retval None
  * @note   
  *		1.函数参数不能798915us，如果参数超过阈值，则函数调用失败，因为寄存器溢出，溢出的后果是溢出的值写回到重载寄存器
  *		2.内部时钟:FCLK（自由运行时钟）频率168MHz。
  *		3.外部时钟:STCLK（自由运行时钟）对AHB总线的时钟频率进行8分频，就是168MHZ /8 = 21MHZ
  *		4.滴答定时器只支持递减计数
  */
extern void delay_us(__IO uint32_t nTime);


/**
  * @brief  滴答定时器毫秒延时函数，适用于裸机开发
  * @param  nTime: 待延时的微秒时间
  * @retval None
  * @note   函数参数不能798ms，如果参数超过阈值，则函数调用失败，因为寄存器溢出，溢出的后果是溢出的值写回到重载寄存器
  */
extern void delay_ms(__IO uint32_t nTime);


/**
  * @brief  秒延时函数，适用于裸机开发
  * @param  nTime: 待延时的微秒时间
  * @retval None
  * @note   要与前面毫秒延时函数配套使用
  */
extern void delay_s(__IO uint32_t nTime);



#endif

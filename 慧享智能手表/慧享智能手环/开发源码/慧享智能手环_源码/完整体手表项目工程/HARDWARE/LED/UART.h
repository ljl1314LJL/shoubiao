#ifndef UART_H
#define UART_H

#include "stm32f4xx.h" //必须包含
#include <stdio.h>

#define BUFFERSIZE 500			 //接收缓冲区容量


extern uint8_t Rx3Buffer [500];
extern __IO uint8_t WifiInitOK_Flag ;	      //WIFI初始化成功标志位
extern __IO uint8_t MsgFromServer_Flag ;	 //消息来自服务器标志位
extern __IO uint16_t Rx3Index ;	 //UART3接收计数器

/**
  * @brief  串口1和电脑端口使用串口通信，常用波特率9600，115200
  * @param  baud: 串口1和电脑的波特率
  * @retval None
  * @note   None
  */
extern void UART1_Config(u32 baud);

/**
  * @brief  串口2和外设(蓝牙)使用串口通信
  * @param  baud: 串口2和外设(蓝牙)的波特率
  * @retval None
  * @note   蓝牙默认波特率9600bps
  */
extern void UART2_Config(u32 baud);

/**
  * @brief  串口3初始化配置
  * @param  baud: 串口3和外设(WIFI模块)的波特率
  * @retval None
  * @note   None
  */
extern void UART3_Config(u32 baud);

/**
* @brief  UART1发送字节
* @param  None
* @retval None
*/
extern void UART1_SendByte(char * str,int n);

/**
* @brief  UART2发送字节
* @param  None
* @retval None
*/
extern void UART2_SendByte(char * str,int n);

/**
* @brief  UART3发送字节
* @param  None
* @retval None
*/
extern void UART3_SendByte(char * str,int n);

/**
* @brief  UART1发送字符串
* @param  None
* @retval None
*/
extern void UART1_SendString(char * str);

/**
* @brief  UART2发送字符串
* @param  None
* @retval None
*/
extern void UART2_SendString(char * str);

/**
* @brief  UART3发送字符串
* @param  None
* @retval None
*/
extern void UART3_SendString(char * str);

#endif

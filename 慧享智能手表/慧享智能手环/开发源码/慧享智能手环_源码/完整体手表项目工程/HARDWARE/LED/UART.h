#ifndef UART_H
#define UART_H

#include "stm32f4xx.h" //�������
#include <stdio.h>

#define BUFFERSIZE 500			 //���ջ���������


extern uint8_t Rx3Buffer [500];
extern __IO uint8_t WifiInitOK_Flag ;	      //WIFI��ʼ���ɹ���־λ
extern __IO uint8_t MsgFromServer_Flag ;	 //��Ϣ���Է�������־λ
extern __IO uint16_t Rx3Index ;	 //UART3���ռ�����

/**
  * @brief  ����1�͵��Զ˿�ʹ�ô���ͨ�ţ����ò�����9600��115200
  * @param  baud: ����1�͵��ԵĲ�����
  * @retval None
  * @note   None
  */
extern void UART1_Config(u32 baud);

/**
  * @brief  ����2������(����)ʹ�ô���ͨ��
  * @param  baud: ����2������(����)�Ĳ�����
  * @retval None
  * @note   ����Ĭ�ϲ�����9600bps
  */
extern void UART2_Config(u32 baud);

/**
  * @brief  ����3��ʼ������
  * @param  baud: ����3������(WIFIģ��)�Ĳ�����
  * @retval None
  * @note   None
  */
extern void UART3_Config(u32 baud);

/**
* @brief  UART1�����ֽ�
* @param  None
* @retval None
*/
extern void UART1_SendByte(char * str,int n);

/**
* @brief  UART2�����ֽ�
* @param  None
* @retval None
*/
extern void UART2_SendByte(char * str,int n);

/**
* @brief  UART3�����ֽ�
* @param  None
* @retval None
*/
extern void UART3_SendByte(char * str,int n);

/**
* @brief  UART1�����ַ���
* @param  None
* @retval None
*/
extern void UART1_SendString(char * str);

/**
* @brief  UART2�����ַ���
* @param  None
* @retval None
*/
extern void UART2_SendString(char * str);

/**
* @brief  UART3�����ַ���
* @param  None
* @retval None
*/
extern void UART3_SendString(char * str);

#endif

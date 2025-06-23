#include "UART.h" 
#include "lvgl.h"
#include "ui.h"

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

uint8_t aRxBuffer [BUFFERSIZE];  //接收缓冲区
__IO uint8_t ubRxIndex = 0x00;	 //接收计数器



uint8_t Rx3Buffer [500];  //UART3接收缓冲区
uint8_t Rx2Buffer [500];  //UART2接收缓冲区
__IO uint16_t Rx3Index = 0x00;	 //UART3接收计数器



__IO uint8_t WifiInitOK_Flag = 0;	      //WIFI初始化成功标志位

__IO uint8_t MsgFromServer_Flag = 0;	 //消息来自服务器标志位

#pragma import(__use_no_semihosting_swi)
struct __FILE { int handle; /* Add whatever you need here */ };
FILE __stdout;
FILE __stdin;

//对该函数进行重定向
int fputc(int ch, FILE *f) 
{
	USART_SendData(USART1,ch); //每次发1字节
	while( USART_GetFlagStatus(USART1,USART_FLAG_TXE) == RESET ); //等待字节发送完
	
	return ch;
}
void _sys_exit(int return_code) 
{
	label: goto label; /* endless loop */
} 
 

/**
  * @brief  串口1和电脑端口使用串口通信，常用波特率9600，115200
  * @param  baud: 串口1和电脑的波特率
  * @retval None
  * @note   None
  */
void UART1_Config(u32 baud)
{
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;

	//1.打开GPIOA端口的时钟
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
  
    //2.打开UART1的时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
  
	//3.选择IO口的复用功能
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource9 ,GPIO_AF_USART1);
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource10,GPIO_AF_USART1);
  
	//4.配置GPIOA引脚参数 + 初始化GPIO端口
	GPIO_InitStructure.GPIO_Mode 	= GPIO_Mode_AF;		//复用模式
	GPIO_InitStructure.GPIO_Speed 	= GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_OType 	= GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd 	= GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Pin 	= GPIO_Pin_9 | GPIO_Pin_10;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
 
    //5.配置UART串口参数 + 初始化UART串口
	USART_InitStructure.USART_BaudRate = baud;						//波特率
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;		//数据位  8bit
	USART_InitStructure.USART_StopBits = USART_StopBits_1;			//停止位  1bit
	USART_InitStructure.USART_Parity = USART_Parity_No;				//无校验
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None; //无流控
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx; //全双工
	USART_Init(USART1, &USART_InitStructure);
  
  
	//6.配置NVIC中断参数
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	//7.选择UART1的中断源  接收到数据则触发中断
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
	
	//8.使能UART1
	USART_Cmd(USART1, ENABLE);
}

/**
  * @brief  串口2和外设(蓝牙)使用串口通信
  * @param  baud: 串口2和外设(蓝牙)的波特率
  * @retval None
  * @note   蓝牙默认波特率9600bps
  */
void UART2_Config(u32 baud)
{
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;

	//1.打开GPIOA端口的时钟
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
  
    //2.打开UART2的时钟
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
  
	//3.选择IO口的复用功能
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource2 ,GPIO_AF_USART2);
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource3 ,GPIO_AF_USART2);
  
	//4.配置GPIOA引脚参数 + 初始化GPIO端口
	GPIO_InitStructure.GPIO_Mode 	= GPIO_Mode_AF;		//复用模式
	GPIO_InitStructure.GPIO_Speed 	= GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_OType 	= GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd 	= GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Pin 	= GPIO_Pin_2 | GPIO_Pin_3;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
 
    //5.配置UART串口参数 + 初始化UART串口
	USART_InitStructure.USART_BaudRate = baud;						//波特率
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;		//数据位  8bit
	USART_InitStructure.USART_StopBits = USART_StopBits_1;			//停止位  1bit
	USART_InitStructure.USART_Parity = USART_Parity_No;				//无校验
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None; //无流控
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx; //全双工
	USART_Init(USART2, &USART_InitStructure);
  
  
	//6.配置NVIC中断参数
	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	//7.选择UART2的中断源  接收到数据则触发中断
	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);
	
	
	//8.使能UART2
	USART_Cmd(USART2, ENABLE);
}


/**
  * @brief  串口3初始化配置
  * @param  baud: 串口3和外设(WIFI模块)的波特率
  * @retval None
  * @note   None
  */
void UART3_Config(u32 baud)
{
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;

	//1.打开GPIOB端口的时钟
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
  
    //2.打开UART3的时钟
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);
  
	//3.选择IO口的复用功能
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource10 ,GPIO_AF_USART3);
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource11 ,GPIO_AF_USART3);
  
	//4.配置GPIOB引脚参数 + 初始化GPIO端口
	GPIO_InitStructure.GPIO_Mode 	= GPIO_Mode_AF;		//复用模式
	GPIO_InitStructure.GPIO_Speed 	= GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_OType 	= GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd 	= GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Pin 	= GPIO_Pin_10 | GPIO_Pin_11;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
 
    //5.配置UART串口参数 + 初始化UART串口
	USART_InitStructure.USART_BaudRate = baud;										//波特率
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;		//数据位  8bit
	USART_InitStructure.USART_StopBits = USART_StopBits_1;				//停止位  1bit
	USART_InitStructure.USART_Parity = USART_Parity_No;						//无校验
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None; //无流控
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx; 								//全双工
	USART_Init(USART3, &USART_InitStructure);
  
  
	//6.配置NVIC中断参数
	NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	//7.选择UART3的中断源  接收到数据则触发中断
	USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);
	
	
	//8.使能UART3
	USART_Cmd(USART3, ENABLE);
}

 
/**
* @brief  UART1发送字节
* @param  None
* @retval None
*/
void UART1_SendByte(char * str,int n)
{
	while(n--)
	{
		USART_SendData(USART1,*str++); //每次发1字节
		while( USART_GetFlagStatus(USART1,USART_FLAG_TXE) == RESET ); //等待字节发送完
	}
}

/**
* @brief  UART2发送字节
* @param  None
* @retval None
*/
void UART2_SendByte(char * str,int n)
{
	while(n--)
	{
		USART_SendData(USART2,*str++); //每次发1字节
		while( USART_GetFlagStatus(USART2,USART_FLAG_TXE) == RESET ); //等待字节发送完
	}
}

/**
* @brief  UART3发送字节
* @param  None
* @retval None
*/
void UART3_SendByte(char * str,int n)
{
	while(n--)
	{
		USART_SendData(USART3,*str++); //每次发1字节
		while( USART_GetFlagStatus(USART3,USART_FLAG_TXE) == RESET ); //等待字节发送完
	}
}


/**
* @brief  UART1发送字符串
* @param  None
* @retval None
*/
void UART1_SendString(char * str)
{
	while(*str != '\0')
	{
		USART_SendData(USART1,*str++); //每次发1字节
		while( USART_GetFlagStatus(USART1,USART_FLAG_TXE) == RESET ); //等待字节发送完
	}
}

/**
* @brief  UART2发送字符串
* @param  None
* @retval None
*/
void UART2_SendString(char * str)
{
	while(*str != '\0')
	{
		USART_SendData(USART2,*str++); //每次发1字节
		while( USART_GetFlagStatus(USART2,USART_FLAG_TXE) == RESET ); //等待字节发送完
	}
}

/**
* @brief  UART3发送字符串
* @param  None
* @retval None
*/
void UART3_SendString(char * str)
{
	while(*str != '\0')
	{
		USART_SendData(USART3,*str++); //每次发1字节
		while( USART_GetFlagStatus(USART3,USART_FLAG_TXE) == RESET ); //等待字节发送完
	}
}

/**
* @brief  串口1收到数据中断函数
* @param  None
* @retval None
*/
void USART1_IRQHandler(void)
{
	//判断中断是否发生
	if (USART_GetITStatus(USART1, USART_IT_RXNE) == SET)
	{
		
		//情空标志位
		//USART_ClearITPendingBit(USART1, USART_IT_RXNE);
	}
}


/**
* @brief  串口2收到数据中断函数
* @param  None
* @retval None
*/
void USART2_IRQHandler(void)
{
	//判断中断是否发生
	if (USART_GetITStatus(USART2, USART_IT_RXNE) == SET)
	{
		//判断当前接收缓冲区是否已满
		if (ubRxIndex < BUFFERSIZE)
		{
			//把接收到的字节存储到缓冲区
			aRxBuffer[ubRxIndex++] = USART_ReceiveData(USART2);
			//printf("%c\r\n",aRxBuffer[ubRxIndex]);
			//为了测试是否收到数据，则可以把接收的数据发送出去
			
					
		//如果收到数据为':'表示收到结束符
		if(aRxBuffer[ubRxIndex-1] == ';')//英文分号
		{
			 char *dotPtr;
			// 使用 strstr 查找小数点的位置
			dotPtr = strstr(aRxBuffer, ".");

			if (dotPtr != NULL) 
			{
				// 提取整数部分
				char integerPart[2];
				strncpy(integerPart, aRxBuffer, dotPtr - aRxBuffer);
				integerPart[dotPtr - aRxBuffer] = '\0'; // 添加字符串结束符
				lv_label_set_text(ui_Label1,integerPart);
				// 提取小数部分
				char decimalPart[2];
				strncpy(decimalPart, dotPtr + 1, strlen(dotPtr + 1) - 1); // 去掉最后的分号
				decimalPart[strlen(dotPtr + 1) - 1] = '\0'; // 添加字符串结束符
				lv_label_set_text(ui_Label4,decimalPart);
				
				memset(aRxBuffer,0,strlen(aRxBuffer));
				ubRxIndex = 0;
			} else 
			{
				printf("未找到小数点\n");
			}

				
		}
		
			USART_SendData(USART1, aRxBuffer[ubRxIndex-1]);
		
		}
		
	}
}

/**
* @brief  串口3收到数据中断函数
* @param  None
* @retval None
*/
void USART3_IRQHandler(void)
{
	
	uint8_t rxdata = 0;
	
	//判断中断是否发生
	if (USART_GetITStatus(USART3, USART_IT_RXNE) == SET)
	{
		
		//判断当前接收缓冲区是否已满
		if (Rx3Index < BUFFERSIZE)
		{
			//把接收到的字节存储到缓冲区
			rxdata = USART_ReceiveData(USART3);
			Rx3Buffer[Rx3Index++] = rxdata;
		}
		
		//判断收到的消息是否来自云服务器
		if(WifiInitOK_Flag == 1 && Rx3Buffer[Rx3Index-1] == '\n' && Rx3Buffer[Rx3Index-2] == '\r')
		{
			MsgFromServer_Flag = 1;
		}
	}
}

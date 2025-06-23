#include "UART.h" 
#include "lvgl.h"
#include "ui.h"

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

uint8_t aRxBuffer [BUFFERSIZE];  //���ջ�����
__IO uint8_t ubRxIndex = 0x00;	 //���ռ�����



uint8_t Rx3Buffer [500];  //UART3���ջ�����
uint8_t Rx2Buffer [500];  //UART2���ջ�����
__IO uint16_t Rx3Index = 0x00;	 //UART3���ռ�����



__IO uint8_t WifiInitOK_Flag = 0;	      //WIFI��ʼ���ɹ���־λ

__IO uint8_t MsgFromServer_Flag = 0;	 //��Ϣ���Է�������־λ

#pragma import(__use_no_semihosting_swi)
struct __FILE { int handle; /* Add whatever you need here */ };
FILE __stdout;
FILE __stdin;

//�Ըú��������ض���
int fputc(int ch, FILE *f) 
{
	USART_SendData(USART1,ch); //ÿ�η�1�ֽ�
	while( USART_GetFlagStatus(USART1,USART_FLAG_TXE) == RESET ); //�ȴ��ֽڷ�����
	
	return ch;
}
void _sys_exit(int return_code) 
{
	label: goto label; /* endless loop */
} 
 

/**
  * @brief  ����1�͵��Զ˿�ʹ�ô���ͨ�ţ����ò�����9600��115200
  * @param  baud: ����1�͵��ԵĲ�����
  * @retval None
  * @note   None
  */
void UART1_Config(u32 baud)
{
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;

	//1.��GPIOA�˿ڵ�ʱ��
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
  
    //2.��UART1��ʱ��
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
  
	//3.ѡ��IO�ڵĸ��ù���
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource9 ,GPIO_AF_USART1);
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource10,GPIO_AF_USART1);
  
	//4.����GPIOA���Ų��� + ��ʼ��GPIO�˿�
	GPIO_InitStructure.GPIO_Mode 	= GPIO_Mode_AF;		//����ģʽ
	GPIO_InitStructure.GPIO_Speed 	= GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_OType 	= GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd 	= GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Pin 	= GPIO_Pin_9 | GPIO_Pin_10;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
 
    //5.����UART���ڲ��� + ��ʼ��UART����
	USART_InitStructure.USART_BaudRate = baud;						//������
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;		//����λ  8bit
	USART_InitStructure.USART_StopBits = USART_StopBits_1;			//ֹͣλ  1bit
	USART_InitStructure.USART_Parity = USART_Parity_No;				//��У��
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None; //������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx; //ȫ˫��
	USART_Init(USART1, &USART_InitStructure);
  
  
	//6.����NVIC�жϲ���
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	//7.ѡ��UART1���ж�Դ  ���յ������򴥷��ж�
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
	
	//8.ʹ��UART1
	USART_Cmd(USART1, ENABLE);
}

/**
  * @brief  ����2������(����)ʹ�ô���ͨ��
  * @param  baud: ����2������(����)�Ĳ�����
  * @retval None
  * @note   ����Ĭ�ϲ�����9600bps
  */
void UART2_Config(u32 baud)
{
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;

	//1.��GPIOA�˿ڵ�ʱ��
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
  
    //2.��UART2��ʱ��
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
  
	//3.ѡ��IO�ڵĸ��ù���
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource2 ,GPIO_AF_USART2);
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource3 ,GPIO_AF_USART2);
  
	//4.����GPIOA���Ų��� + ��ʼ��GPIO�˿�
	GPIO_InitStructure.GPIO_Mode 	= GPIO_Mode_AF;		//����ģʽ
	GPIO_InitStructure.GPIO_Speed 	= GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_OType 	= GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd 	= GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Pin 	= GPIO_Pin_2 | GPIO_Pin_3;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
 
    //5.����UART���ڲ��� + ��ʼ��UART����
	USART_InitStructure.USART_BaudRate = baud;						//������
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;		//����λ  8bit
	USART_InitStructure.USART_StopBits = USART_StopBits_1;			//ֹͣλ  1bit
	USART_InitStructure.USART_Parity = USART_Parity_No;				//��У��
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None; //������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx; //ȫ˫��
	USART_Init(USART2, &USART_InitStructure);
  
  
	//6.����NVIC�жϲ���
	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	//7.ѡ��UART2���ж�Դ  ���յ������򴥷��ж�
	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);
	
	
	//8.ʹ��UART2
	USART_Cmd(USART2, ENABLE);
}


/**
  * @brief  ����3��ʼ������
  * @param  baud: ����3������(WIFIģ��)�Ĳ�����
  * @retval None
  * @note   None
  */
void UART3_Config(u32 baud)
{
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;

	//1.��GPIOB�˿ڵ�ʱ��
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
  
    //2.��UART3��ʱ��
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);
  
	//3.ѡ��IO�ڵĸ��ù���
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource10 ,GPIO_AF_USART3);
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource11 ,GPIO_AF_USART3);
  
	//4.����GPIOB���Ų��� + ��ʼ��GPIO�˿�
	GPIO_InitStructure.GPIO_Mode 	= GPIO_Mode_AF;		//����ģʽ
	GPIO_InitStructure.GPIO_Speed 	= GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_OType 	= GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd 	= GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Pin 	= GPIO_Pin_10 | GPIO_Pin_11;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
 
    //5.����UART���ڲ��� + ��ʼ��UART����
	USART_InitStructure.USART_BaudRate = baud;										//������
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;		//����λ  8bit
	USART_InitStructure.USART_StopBits = USART_StopBits_1;				//ֹͣλ  1bit
	USART_InitStructure.USART_Parity = USART_Parity_No;						//��У��
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None; //������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx; 								//ȫ˫��
	USART_Init(USART3, &USART_InitStructure);
  
  
	//6.����NVIC�жϲ���
	NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	//7.ѡ��UART3���ж�Դ  ���յ������򴥷��ж�
	USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);
	
	
	//8.ʹ��UART3
	USART_Cmd(USART3, ENABLE);
}

 
/**
* @brief  UART1�����ֽ�
* @param  None
* @retval None
*/
void UART1_SendByte(char * str,int n)
{
	while(n--)
	{
		USART_SendData(USART1,*str++); //ÿ�η�1�ֽ�
		while( USART_GetFlagStatus(USART1,USART_FLAG_TXE) == RESET ); //�ȴ��ֽڷ�����
	}
}

/**
* @brief  UART2�����ֽ�
* @param  None
* @retval None
*/
void UART2_SendByte(char * str,int n)
{
	while(n--)
	{
		USART_SendData(USART2,*str++); //ÿ�η�1�ֽ�
		while( USART_GetFlagStatus(USART2,USART_FLAG_TXE) == RESET ); //�ȴ��ֽڷ�����
	}
}

/**
* @brief  UART3�����ֽ�
* @param  None
* @retval None
*/
void UART3_SendByte(char * str,int n)
{
	while(n--)
	{
		USART_SendData(USART3,*str++); //ÿ�η�1�ֽ�
		while( USART_GetFlagStatus(USART3,USART_FLAG_TXE) == RESET ); //�ȴ��ֽڷ�����
	}
}


/**
* @brief  UART1�����ַ���
* @param  None
* @retval None
*/
void UART1_SendString(char * str)
{
	while(*str != '\0')
	{
		USART_SendData(USART1,*str++); //ÿ�η�1�ֽ�
		while( USART_GetFlagStatus(USART1,USART_FLAG_TXE) == RESET ); //�ȴ��ֽڷ�����
	}
}

/**
* @brief  UART2�����ַ���
* @param  None
* @retval None
*/
void UART2_SendString(char * str)
{
	while(*str != '\0')
	{
		USART_SendData(USART2,*str++); //ÿ�η�1�ֽ�
		while( USART_GetFlagStatus(USART2,USART_FLAG_TXE) == RESET ); //�ȴ��ֽڷ�����
	}
}

/**
* @brief  UART3�����ַ���
* @param  None
* @retval None
*/
void UART3_SendString(char * str)
{
	while(*str != '\0')
	{
		USART_SendData(USART3,*str++); //ÿ�η�1�ֽ�
		while( USART_GetFlagStatus(USART3,USART_FLAG_TXE) == RESET ); //�ȴ��ֽڷ�����
	}
}

/**
* @brief  ����1�յ������жϺ���
* @param  None
* @retval None
*/
void USART1_IRQHandler(void)
{
	//�ж��ж��Ƿ���
	if (USART_GetITStatus(USART1, USART_IT_RXNE) == SET)
	{
		
		//��ձ�־λ
		//USART_ClearITPendingBit(USART1, USART_IT_RXNE);
	}
}


/**
* @brief  ����2�յ������жϺ���
* @param  None
* @retval None
*/
void USART2_IRQHandler(void)
{
	//�ж��ж��Ƿ���
	if (USART_GetITStatus(USART2, USART_IT_RXNE) == SET)
	{
		//�жϵ�ǰ���ջ������Ƿ�����
		if (ubRxIndex < BUFFERSIZE)
		{
			//�ѽ��յ����ֽڴ洢��������
			aRxBuffer[ubRxIndex++] = USART_ReceiveData(USART2);
			//printf("%c\r\n",aRxBuffer[ubRxIndex]);
			//Ϊ�˲����Ƿ��յ����ݣ�����԰ѽ��յ����ݷ��ͳ�ȥ
			
					
		//����յ�����Ϊ':'��ʾ�յ�������
		if(aRxBuffer[ubRxIndex-1] == ';')//Ӣ�ķֺ�
		{
			 char *dotPtr;
			// ʹ�� strstr ����С�����λ��
			dotPtr = strstr(aRxBuffer, ".");

			if (dotPtr != NULL) 
			{
				// ��ȡ��������
				char integerPart[2];
				strncpy(integerPart, aRxBuffer, dotPtr - aRxBuffer);
				integerPart[dotPtr - aRxBuffer] = '\0'; // ����ַ���������
				lv_label_set_text(ui_Label1,integerPart);
				// ��ȡС������
				char decimalPart[2];
				strncpy(decimalPart, dotPtr + 1, strlen(dotPtr + 1) - 1); // ȥ�����ķֺ�
				decimalPart[strlen(dotPtr + 1) - 1] = '\0'; // ����ַ���������
				lv_label_set_text(ui_Label4,decimalPart);
				
				memset(aRxBuffer,0,strlen(aRxBuffer));
				ubRxIndex = 0;
			} else 
			{
				printf("δ�ҵ�С����\n");
			}

				
		}
		
			USART_SendData(USART1, aRxBuffer[ubRxIndex-1]);
		
		}
		
	}
}

/**
* @brief  ����3�յ������жϺ���
* @param  None
* @retval None
*/
void USART3_IRQHandler(void)
{
	
	uint8_t rxdata = 0;
	
	//�ж��ж��Ƿ���
	if (USART_GetITStatus(USART3, USART_IT_RXNE) == SET)
	{
		
		//�жϵ�ǰ���ջ������Ƿ�����
		if (Rx3Index < BUFFERSIZE)
		{
			//�ѽ��յ����ֽڴ洢��������
			rxdata = USART_ReceiveData(USART3);
			Rx3Buffer[Rx3Index++] = rxdata;
		}
		
		//�ж��յ�����Ϣ�Ƿ������Ʒ�����
		if(WifiInitOK_Flag == 1 && Rx3Buffer[Rx3Index-1] == '\n' && Rx3Buffer[Rx3Index-2] == '\r')
		{
			MsgFromServer_Flag = 1;
		}
	}
}

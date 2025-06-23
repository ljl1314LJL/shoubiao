/**
  ******************************************************************************
  * @file    main.c 
  * @author  cecelmx@126.com
  * @version 
  * @date    2025/04/02
  * @brief   ��������DHT11��������ȡ���ڵ���ʪ�����ݣ���ͨ��PC�˽������
  ******************************************************************************
  * @note
  *			1. UART1���ڵ�IO����PA9��TXD����PA10��RXD��������IO�ڵ�ģʽΪ����ģʽ
  *			2. ���ں�PC�˵���λ��ͨ�ţ���ֻ��Ҫȷ��MCU��UART1�Ĳ�����PC�����õ�һ�£�
  *     3. ��Ҫ��fputc���������ض���
  *     4. DHT11���õ�����Э�飬ʹ�õ�MCU��IO����PG9����
  *     5. MCU��Ҫ��DHT11���Ϳ�ʼ�źţ����ȴ�DHT11��Ӧ���źţ�������40bit������
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/

#include "DHT11.h"
/* Private typedef -----------------------------------------------------------*/


/* Private define ------------------------------------------------------------*/

#define  DHT11_SET(n)  (n)? GPIO_SetBits(GPIOG,GPIO_Pin_9):GPIO_ResetBits(GPIOG,GPIO_Pin_9)
#define  DHT11_READ()   GPIO_ReadInputDataBit(GPIOG,GPIO_Pin_9)

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

//����PG9���ŵ�ģʽ
void DHT11_PinModeSet(GPIOMode_TypeDef Mode)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	//1.��GPIOG�˿ڵ�ʱ��
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOG, ENABLE);
	
	//2.����PG9���Ų��� + ��ʼ��GPIO�˿�
	GPIO_InitStructure.GPIO_Mode 	= Mode;					//����ģʽ
	GPIO_InitStructure.GPIO_Speed 	= GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_OType 	= GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd 	= GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Pin 	= GPIO_Pin_9 ;
	GPIO_Init(GPIOG, &GPIO_InitStructure); 
}

//DHT11�ĳ�ʼ��
void DHT11_Config(void)
{
	//����PG9����Ϊ���ģʽ
	DHT11_PinModeSet(GPIO_Mode_OUT);
}


//DHT11���Ϳ�ʼ�ź�
void DHT11_SendStart(void)
{
	//1.����Ϊ���ģʽ
	DHT11_PinModeSet(GPIO_Mode_OUT);
	
	//2.����PG9���ŵĵ�ƽ
	DHT11_SET(0);
	
	//3.��ʱ>18ms && <30ms,������ʱ20ms  
	delay_ms(20);
	
	//4.����Ϊ����ģʽ
	DHT11_PinModeSet(GPIO_Mode_IN);

}


//�ȴ�DHT11����Ӧ�ź�
bool DHT11_IsACK(void)
{
	int cnt = 0; //������
	
	//1.MCU�ȴ�PG9�յ��͵�ƽ����Ϊ����ӳ�ʱ���� ���賬ʱʱ������Ϊ100us
	while( DHT11_READ() == SET && cnt < 100)
	{
		delay_us(1);
		cnt++;
	}
	
	if(cnt >= 100)
	{
		return false; //δ��Ӧ��ԭ���ǳ�ʱ
	}
	
	//2.MCU�жϵ͵�ƽ�Ƿ����83us���ң���Ϊ����ӳ�ʱ���� ���賬ʱʱ������Ϊ100us
	cnt = 0;
	while( DHT11_READ() == RESET && cnt < 100)
	{
		delay_us(1);
		cnt++;
	}
	
	if(cnt >= 100)
		return false; //δ��Ӧ��ԭ���ǳ�ʱ
	else
		return true;  //˵����Ӧ
}

//DHT11��ȡ1bit  0x00 (0000000 0)˵���յ���bit=0  0x01(0000000 1) ˵���յ���bit=1
uint8_t DHT11_ReadBit(void)
{
	int cnt = 0;
	
	//1.MCU�ȴ��͵�ƽ
	while( DHT11_READ() == SET && cnt < 100)
	{
		delay_us(1);
		cnt++;
	}
	
	//2.MCU�ȴ��ߵ�ƽ
	cnt=0;
	while( DHT11_READ() == RESET && cnt < 100)
	{
		delay_us(1);
		cnt++;
	}
	
	//3.��ʱ > 27us && < 74us
	delay_us(40);
	
	//4.�ж���ʱ֮��ĵ�ƽ״̬
	if( DHT11_READ() == SET )
		return 0x01;
	else
		return 0x00;
}

uint8_t  DHT11_ReadByte(void)
{
	int cnt = 0;
	
	uint8_t data = 0; //���ڴ洢8bit���� 0000 0000
	
	//ѭ����ȡ8bit ��ѭMSB��λ�ȳ�
	for(cnt=0;cnt<8;cnt++)
	{
		data <<= 1;
		data |= DHT11_ReadBit(); 
	}
	
	return data;
}


bool DHT11_ReadData(uint8_t *dhtbuf)
{
	int i = 0;
	
	//1.MCU���Ϳ�ʼ�ź�
	DHT11_SendStart();
	
	//2.MCU�ж��Ƿ���Ӧ
	if( DHT11_IsACK() )
	{
		//ѭ����ȡ5���ֽ�
		for(i=0;i<5;i++)
		{
			dhtbuf[i] = DHT11_ReadByte();
		}
		
		//�����ݽ���У��
		if( dhtbuf[0] + dhtbuf[1] + dhtbuf[2] + dhtbuf[3] == dhtbuf[4] )
		{
			
			return true;
		}
		else
			return false;//˵�����ݶ�ȡʧ�ܣ�ԭ����У��ʧ��
	}
	else
		return false;   //˵�����ݶ�ȡʧ�ܣ�ԭ����DHT11δ��Ӧ
}

/**
  * @brief  DHT11��ȡ��ʪ�ȵ�һ������
	* @param  None
  * @retval None
  * @note   1.NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);�ǵ����ȼ�����
  */
void dht11_example(void)
{
	uint8_t dhtbuf[5] ={0};
	
	//1.Ӳ����ʼ��
	//NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4); //�ж����ȼ����� 4bit���ṩ����ռ���ȼ�

	//DHT11_Config();			//DHT11�ĳ�ʼ��
	
	DHT11_ReadData(dhtbuf);
	printf("temp = %d ��, humi = %d %%RH\r\n",dhtbuf[2],dhtbuf[0]);
}



/************************ (C) COPYRIGHT Your Name *****END OF FILE*************/

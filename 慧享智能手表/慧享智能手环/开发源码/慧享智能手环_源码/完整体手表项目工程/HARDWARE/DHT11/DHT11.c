/**
  ******************************************************************************
  * @file    main.c 
  * @author  cecelmx@126.com
  * @version 
  * @date    2025/04/02
  * @brief   这是利用DHT11传感器获取室内的温湿度数据，并通过PC端进行输出
  ******************************************************************************
  * @note
  *			1. UART1串口的IO口是PA9（TXD）和PA10（RXD），所以IO口的模式为复用模式
  *			2. 由于和PC端的上位机通信，则只需要确保MCU的UART1的参数和PC端设置的一致！
  *     3. 需要对fputc函数进行重定向
  *     4. DHT11采用单总线协议，使用的MCU的IO口是PG9引脚
  *     5. MCU需要向DHT11发送开始信号，并等待DHT11的应答信号，并接收40bit的数据
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

//设置PG9引脚的模式
void DHT11_PinModeSet(GPIOMode_TypeDef Mode)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	//1.打开GPIOG端口的时钟
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOG, ENABLE);
	
	//2.配置PG9引脚参数 + 初始化GPIO端口
	GPIO_InitStructure.GPIO_Mode 	= Mode;					//引脚模式
	GPIO_InitStructure.GPIO_Speed 	= GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_OType 	= GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd 	= GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Pin 	= GPIO_Pin_9 ;
	GPIO_Init(GPIOG, &GPIO_InitStructure); 
}

//DHT11的初始化
void DHT11_Config(void)
{
	//配置PG9引脚为输出模式
	DHT11_PinModeSet(GPIO_Mode_OUT);
}


//DHT11发送开始信号
void DHT11_SendStart(void)
{
	//1.设置为输出模式
	DHT11_PinModeSet(GPIO_Mode_OUT);
	
	//2.拉低PG9引脚的电平
	DHT11_SET(0);
	
	//3.延时>18ms && <30ms,建议延时20ms  
	delay_ms(20);
	
	//4.设置为输入模式
	DHT11_PinModeSet(GPIO_Mode_IN);

}


//等待DHT11的响应信号
bool DHT11_IsACK(void)
{
	int cnt = 0; //计数器
	
	//1.MCU等待PG9收到低电平，人为的添加超时机制 假设超时时间设置为100us
	while( DHT11_READ() == SET && cnt < 100)
	{
		delay_us(1);
		cnt++;
	}
	
	if(cnt >= 100)
	{
		return false; //未响应，原因是超时
	}
	
	//2.MCU判断低电平是否持续83us左右，人为的添加超时机制 假设超时时间设置为100us
	cnt = 0;
	while( DHT11_READ() == RESET && cnt < 100)
	{
		delay_us(1);
		cnt++;
	}
	
	if(cnt >= 100)
		return false; //未响应，原因是超时
	else
		return true;  //说明响应
}

//DHT11读取1bit  0x00 (0000000 0)说明收到的bit=0  0x01(0000000 1) 说明收到的bit=1
uint8_t DHT11_ReadBit(void)
{
	int cnt = 0;
	
	//1.MCU等待低电平
	while( DHT11_READ() == SET && cnt < 100)
	{
		delay_us(1);
		cnt++;
	}
	
	//2.MCU等待高电平
	cnt=0;
	while( DHT11_READ() == RESET && cnt < 100)
	{
		delay_us(1);
		cnt++;
	}
	
	//3.延时 > 27us && < 74us
	delay_us(40);
	
	//4.判断延时之后的电平状态
	if( DHT11_READ() == SET )
		return 0x01;
	else
		return 0x00;
}

uint8_t  DHT11_ReadByte(void)
{
	int cnt = 0;
	
	uint8_t data = 0; //用于存储8bit数据 0000 0000
	
	//循环读取8bit 遵循MSB高位先出
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
	
	//1.MCU发送开始信号
	DHT11_SendStart();
	
	//2.MCU判断是否响应
	if( DHT11_IsACK() )
	{
		//循环读取5个字节
		for(i=0;i<5;i++)
		{
			dhtbuf[i] = DHT11_ReadByte();
		}
		
		//对数据进行校验
		if( dhtbuf[0] + dhtbuf[1] + dhtbuf[2] + dhtbuf[3] == dhtbuf[4] )
		{
			
			return true;
		}
		else
			return false;//说明数据读取失败，原因是校验失败
	}
	else
		return false;   //说明数据读取失败，原因是DHT11未响应
}

/**
  * @brief  DHT11读取温湿度的一个案例
	* @param  None
  * @retval None
  * @note   1.NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);记得优先级分组
  */
void dht11_example(void)
{
	uint8_t dhtbuf[5] ={0};
	
	//1.硬件初始化
	//NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4); //中断优先级分组 4bit都提供给抢占优先级

	//DHT11_Config();			//DHT11的初始化
	
	DHT11_ReadData(dhtbuf);
	printf("temp = %d ℃, humi = %d %%RH\r\n",dhtbuf[2],dhtbuf[0]);
}



/************************ (C) COPYRIGHT Your Name *****END OF FILE*************/

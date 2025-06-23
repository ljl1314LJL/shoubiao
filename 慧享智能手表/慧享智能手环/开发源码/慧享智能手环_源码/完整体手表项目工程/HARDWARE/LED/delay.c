/**
  ******************************************************************************
  * @file    DELAY.c 
  * @author  baiye
  * @version 
  * @date    2025/04/8
  * @brief   这是已经封装好的延时函数，可以直接调用
  ******************************************************************************
  * @note
  *		
  *		
  *
  ******************************************************************************
  */

#include "DELAY.h" //必须包含
#include "FreeRTOSConfig.h"
#include "includes.h"

static __IO uint32_t TimingDelay;

#if RTOS_ISUSE
//#if 0

/**
  * @brief  微秒延时函数
  * @param  nTime: 待延时的微秒时间
  * @retval None
  * @note   只能在FreeRTOS中实现非阻塞延时，不可以在裸机环境下使用
  */
void delay_us(__IO uint32_t nTime)
{
	uint32_t t1,t2; //用于记录读取的SysTick的值
	
	uint32_t cnt = 0; //用于记录计数累加值
	
	//读取第1次Val值
	t1 = SysTick->VAL;
	
	vTaskSuspendAll();
	//进入循环
	for(;;)
	{
		//读取第2次Val值
		t2 = SysTick->VAL;
		
		//判断t1和t2是否相等
		if(t1 != t2)
		{
			//t1 > t2
			if(t1 > t2)
				cnt += t1 - t2;
			//t1 < t2
			else
				cnt += SysTick->LOAD - t2 + t1;
			
			//判断是否已经达到目标值
			if( cnt >= nTime*168 )
				break;
			
			//如果计数值未达到目标值，再循环1次
			t1 = t2;
		}	
	}
	xTaskResumeAll();
}

/**
  * @brief  毫秒延时函数
  * @param  nTime: 待延时的毫秒时间
  * @retval None
  * @note   只能在FreeRTOS中实现非阻塞延时，不可以在裸机环境下使用
  */
void delay_ms(__IO uint32_t nTime)
{
	while(nTime--)
	{
		delay_us(1000);
	}
}

#else

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
void delay_us(__IO uint32_t nTime)
{ 
	SysTick->CTRL = 0; 					// 关闭定时器
	SysTick->LOAD = 21 * nTime - 1; 	// 设置重载值
	SysTick->VAL = 0; 					// 清空计数器
	SysTick->CTRL = 1; 					//打开定时器，并且使用STCLK(21MHZ)时钟源
	while ((SysTick->CTRL & 0x00010000)==0);// 等待延时时间到达
	SysTick->CTRL = 0; 					// 关闭定时器
}

/**
  * @brief  滴答定时器毫秒延时函数，适用于裸机开发
  * @param  nTime: 待延时的微秒时间
  * @retval None
  * @note   None
  */
void delay_ms(__IO uint32_t nTime)
{ 
	while(nTime--)
	{
		delay_us(1000);
	}
}

#endif
/**
  * @brief  秒延时函数，适用于裸机开发
  * @param  nTime: 待延时的微秒时间
  * @retval None
  * @note   要与前面毫秒延时函数配套使用
  */
void delay_s(__IO uint32_t nTime)
{ 
	if(nTime <= 0) return;
	else {
		while(nTime--){
			delay_ms(500);	//延时500毫秒
			delay_ms(500);
		}
	}
}

///////////////////////////////////////////////////////////////
///**
//  * @brief  滴答定时器初始化函数，以毫秒为单位,适用于RTOS实时操作系统，
//  * @param  要延时多少毫秒
//  * @retval None
//  * @note   1.寄存器有32bit，但是只用了24bit
//  *		    2.必须使用168MHz的FCLK（自由运行时钟）
//  *		    3.如果参数超过阈值，则函数调用失败，因为寄存器溢出，溢出的后果是溢出的值写回到重载寄存器
//  *				4.滴答定时器只支持递减计数
//	*				5.使用前要加这句SysTick_Config(168 * 1000); 设置systick的2次中断的嘀嗒值  SystemCoreClock = 168000000，必须为这个值。
//  */
//void DelayMs(__IO uint32_t nTime)
//{ 
//  TimingDelay = nTime;

//  while(TimingDelay != 0); //进行延时
//}

///**
//  * @brief  滴答定时器ISR函数
//  * @param  None
//  * @retval None
//  * @note   1.触发频率是1KHz
//  * @note   2.不可与前面的delay一起使用，因为直接操作SysTick寄存器（LOAD、CTRL），覆盖了原有配置，禁用了中断，导致延时期间nTime停止更新。
//  */
//void SysTick_Handler(void)
//{
//	if (TimingDelay != 0)
//	{ 
//		TimingDelay--;
//	}
//}

/**
  * @brief  后台程序，要处理的主程序
  * @param  None
  * @retval None
  * @note   要与前面毫秒延时函数ISR函数配套使用配套使用
  */
//static void delay_example (void)
//{
//	//1.设置systick的2次中断的嘀嗒值  SystemCoreClock = 168000000
//	SysTick_Config(168 * 1000); 
//	
//	//2.硬件初始化
//	//BEEP_Init();
//	
//	while(1)
//	{
//		//GPIO_SetBits(GPIOF,GPIO_Pin_8);  		代码
//		DelayMs(1000);							//延时1S
//		//GPIO_ResetBits(GPIOF,GPIO_Pin_8);		代码
//		DelayMs(1000);
//	}
//}

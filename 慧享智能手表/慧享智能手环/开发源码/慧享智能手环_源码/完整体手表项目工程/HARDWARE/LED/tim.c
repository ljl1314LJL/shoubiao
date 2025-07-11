#include "tim.h"

/*************************************
定时器说明:
TIM3 -- APB1
TIM3频率：84MHZ
TIM3:16位定时器
*************************************/
void Tim3_Init(void)
{
	TIM_TimeBaseInitTypeDef		TIM_TimeBaseInitStruct;
	NVIC_InitTypeDef   NVIC_InitStructure;
	
	
	//使能定时器时钟。
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
	 
	TIM_TimeBaseInitStruct.TIM_Prescaler	= (84-1); 	//设置预分频器 84分频 84MHZ/84 = 1MHZ
	TIM_TimeBaseInitStruct.TIM_Period		= (1000-1); //计数周期 重装载值 确定溢出时间：1000/1MHZ = 1ms
	TIM_TimeBaseInitStruct.TIM_CounterMode	= TIM_CounterMode_Up; //向上计数
	TIM_TimeBaseInitStruct.TIM_ClockDivision= TIM_CKD_DIV1; //时钟分频与ETR TI有关，配置定时器时忽略即可
	
	
	//初始化定时器，配置ARR,PSC。
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseInitStruct);
	
	
	//启定时器中断，配置NVIC。
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;   				//通道编号，可在stm32f4xx.h中查看
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x01; 	//抢占优先级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x01;			//响应优先级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;					//通道使能
	NVIC_Init(&NVIC_InitStructure);
	
	
	//定时器是有多种中断源，你选择其中的一种
	//TIM3_DIER  允许更新中断
	TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);
	//使能定时器。
	TIM_Cmd(TIM3, ENABLE);

}

//定时器1ms中断服务函数
void TIM3_IRQHandler(void)
{
    /* 检测时间更新中断的标志位是否置位 */
    if (TIM_GetITStatus(TIM3, TIM_IT_Update) == SET)
    {
        /* 系统时间向前推进一个单位的时间间隔 */
        //lv_tick_inc(1);  //lv_tick_inc参数范围：1~10

        /* 清空标志位 */
        TIM_ClearITPendingBit(TIM3, TIM_IT_Update);
    }

}




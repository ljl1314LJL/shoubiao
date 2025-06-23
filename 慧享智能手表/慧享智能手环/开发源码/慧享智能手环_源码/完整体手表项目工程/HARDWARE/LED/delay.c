/**
  ******************************************************************************
  * @file    DELAY.c 
  * @author  baiye
  * @version 
  * @date    2025/04/8
  * @brief   �����Ѿ���װ�õ���ʱ����������ֱ�ӵ���
  ******************************************************************************
  * @note
  *		
  *		
  *
  ******************************************************************************
  */

#include "DELAY.h" //�������
#include "FreeRTOSConfig.h"
#include "includes.h"

static __IO uint32_t TimingDelay;

#if RTOS_ISUSE
//#if 0

/**
  * @brief  ΢����ʱ����
  * @param  nTime: ����ʱ��΢��ʱ��
  * @retval None
  * @note   ֻ����FreeRTOS��ʵ�ַ�������ʱ�������������������ʹ��
  */
void delay_us(__IO uint32_t nTime)
{
	uint32_t t1,t2; //���ڼ�¼��ȡ��SysTick��ֵ
	
	uint32_t cnt = 0; //���ڼ�¼�����ۼ�ֵ
	
	//��ȡ��1��Valֵ
	t1 = SysTick->VAL;
	
	vTaskSuspendAll();
	//����ѭ��
	for(;;)
	{
		//��ȡ��2��Valֵ
		t2 = SysTick->VAL;
		
		//�ж�t1��t2�Ƿ����
		if(t1 != t2)
		{
			//t1 > t2
			if(t1 > t2)
				cnt += t1 - t2;
			//t1 < t2
			else
				cnt += SysTick->LOAD - t2 + t1;
			
			//�ж��Ƿ��Ѿ��ﵽĿ��ֵ
			if( cnt >= nTime*168 )
				break;
			
			//�������ֵδ�ﵽĿ��ֵ����ѭ��1��
			t1 = t2;
		}	
	}
	xTaskResumeAll();
}

/**
  * @brief  ������ʱ����
  * @param  nTime: ����ʱ�ĺ���ʱ��
  * @retval None
  * @note   ֻ����FreeRTOS��ʵ�ַ�������ʱ�������������������ʹ��
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
  * @brief  �δ�ʱ��΢����ʱ�������������������
  * @param  nTime: ����ʱ��΢��ʱ��
  * @retval None
  * @note   
  *		1.������������798915us���������������ֵ����������ʧ�ܣ���Ϊ�Ĵ������������ĺ���������ֵд�ص����ؼĴ���
  *		2.�ڲ�ʱ��:FCLK����������ʱ�ӣ�Ƶ��168MHz��
  *		3.�ⲿʱ��:STCLK����������ʱ�ӣ���AHB���ߵ�ʱ��Ƶ�ʽ���8��Ƶ������168MHZ /8 = 21MHZ
  *		4.�δ�ʱ��ֻ֧�ֵݼ�����
  */
void delay_us(__IO uint32_t nTime)
{ 
	SysTick->CTRL = 0; 					// �رն�ʱ��
	SysTick->LOAD = 21 * nTime - 1; 	// ��������ֵ
	SysTick->VAL = 0; 					// ��ռ�����
	SysTick->CTRL = 1; 					//�򿪶�ʱ��������ʹ��STCLK(21MHZ)ʱ��Դ
	while ((SysTick->CTRL & 0x00010000)==0);// �ȴ���ʱʱ�䵽��
	SysTick->CTRL = 0; 					// �رն�ʱ��
}

/**
  * @brief  �δ�ʱ��������ʱ�������������������
  * @param  nTime: ����ʱ��΢��ʱ��
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
  * @brief  ����ʱ�������������������
  * @param  nTime: ����ʱ��΢��ʱ��
  * @retval None
  * @note   Ҫ��ǰ�������ʱ��������ʹ��
  */
void delay_s(__IO uint32_t nTime)
{ 
	if(nTime <= 0) return;
	else {
		while(nTime--){
			delay_ms(500);	//��ʱ500����
			delay_ms(500);
		}
	}
}

///////////////////////////////////////////////////////////////
///**
//  * @brief  �δ�ʱ����ʼ���������Ժ���Ϊ��λ,������RTOSʵʱ����ϵͳ��
//  * @param  Ҫ��ʱ���ٺ���
//  * @retval None
//  * @note   1.�Ĵ�����32bit������ֻ����24bit
//  *		    2.����ʹ��168MHz��FCLK����������ʱ�ӣ�
//  *		    3.�������������ֵ����������ʧ�ܣ���Ϊ�Ĵ������������ĺ���������ֵд�ص����ؼĴ���
//  *				4.�δ�ʱ��ֻ֧�ֵݼ�����
//	*				5.ʹ��ǰҪ�����SysTick_Config(168 * 1000); ����systick��2���жϵ����ֵ  SystemCoreClock = 168000000������Ϊ���ֵ��
//  */
//void DelayMs(__IO uint32_t nTime)
//{ 
//  TimingDelay = nTime;

//  while(TimingDelay != 0); //������ʱ
//}

///**
//  * @brief  �δ�ʱ��ISR����
//  * @param  None
//  * @retval None
//  * @note   1.����Ƶ����1KHz
//  * @note   2.������ǰ���delayһ��ʹ�ã���Ϊֱ�Ӳ���SysTick�Ĵ�����LOAD��CTRL����������ԭ�����ã��������жϣ�������ʱ�ڼ�nTimeֹͣ���¡�
//  */
//void SysTick_Handler(void)
//{
//	if (TimingDelay != 0)
//	{ 
//		TimingDelay--;
//	}
//}

/**
  * @brief  ��̨����Ҫ�����������
  * @param  None
  * @retval None
  * @note   Ҫ��ǰ�������ʱ����ISR��������ʹ������ʹ��
  */
//static void delay_example (void)
//{
//	//1.����systick��2���жϵ����ֵ  SystemCoreClock = 168000000
//	SysTick_Config(168 * 1000); 
//	
//	//2.Ӳ����ʼ��
//	//BEEP_Init();
//	
//	while(1)
//	{
//		//GPIO_SetBits(GPIOF,GPIO_Pin_8);  		����
//		DelayMs(1000);							//��ʱ1S
//		//GPIO_ResetBits(GPIOF,GPIO_Pin_8);		����
//		DelayMs(1000);
//	}
//}

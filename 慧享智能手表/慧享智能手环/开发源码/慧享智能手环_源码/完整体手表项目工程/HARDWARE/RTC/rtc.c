#include "rtc.h"
#include "ui.h"
#define  BKP  0x2502

RTC_TimeTypeDef  RTC_TimeStructure;
RTC_DateTypeDef  RTC_DateStructure;
void Rtc_Init()
{
    NVIC_InitTypeDef  NVIC_InitStructure;
    EXTI_InitTypeDef  EXTI_InitStructure;
    RTC_InitTypeDef  RTC_InitStructure;

    // 1. ��PWR�����ʱ��
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);

    // 2. ��PWR_CR�Ĵ�����DBPλ��λ�����RTCд�����Ĵ���
    PWR_BackupAccessCmd(ENABLE);

    // 3. ʹ��LSEʱ��
    RCC_LSEConfig(RCC_LSE_ON);

    // 4. �ȴ�LSE����
    while(RCC_GetFlagStatus(RCC_FLAG_LSERDY) == RESET);

    // 5. ѡ��RTC��ʱ��Դ
    RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);

    // 6. ʹ��RTC��ʱ��
    RCC_RTCCLKCmd(ENABLE);

    // 7. �ȴ�RTC�Ĵ���ͬ����� ��RTC�Ĵ������д����
    RTC_WaitForSynchro();

    // 8. ����RTC��Сʱ��ʽ��Ԥ��Ƶֵ
    RTC_InitStructure.RTC_AsynchPrediv = 128 - 1; // �첽��Ƶ
    RTC_InitStructure.RTC_SynchPrediv  = 256 - 1; // ͬ����Ƶ
    RTC_InitStructure.RTC_HourFormat   = RTC_HourFormat_24; // 24Сʱ��
    RTC_Init(&RTC_InitStructure);

//    // 9. ����RTC������ 2025/04/09 ������
//    RTC_DateStructure.RTC_Year  = 0x25; // ��� BCD��ʽ
//    RTC_DateStructure.RTC_Month = 0x04; // �·� BCD��ʽ
//    RTC_DateStructure.RTC_Date  = 0x09; // ���� BCD��ʽ
//    RTC_SetDate(RTC_Format_BCD, &RTC_DateStructure);

//    // 10. ����RTC��ʱ��
//    RTC_TimeStructure.RTC_Hours   = 0x16; // Сʱ BCD��ʽ
//    RTC_TimeStructure.RTC_Minutes = 0x30; // ���� BCD��ʽ
//    RTC_TimeStructure.RTC_Seconds = 0x00; // ���� BCD��ʽ
//    RTC_SetTime(RTC_Format_BCD, &RTC_TimeStructure);

    // 11. ���û����ж�ͨ���Ĳ��� NVIC
    NVIC_InitStructure.NVIC_IRQChannel = RTC_WKUP_IRQn; // �����ж�ͨ��
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    // 12. ����RTC�����жϵ�EXTI22
    EXTI_ClearITPendingBit(EXTI_Line22); // ����ж��߱�־
    EXTI_InitStructure.EXTI_Line = EXTI_Line22;
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStructure);

    // 13. �ر�RTC���Ѷ�ʱ��
    RTC_WakeUpCmd(DISABLE);

    // 14. ѡ��RTC�Ļ��Ѷ�ʱ����ʱ��Դ �ڲ�ʱ�� 1HZ
    RTC_WakeUpClockConfig(RTC_WakeUpClock_CK_SPRE_16bits);

    // 15. ����RTC�����Զ����ؼ�����
    RTC_SetWakeUpCounter(0);

    // 16. ѡ��RTC���ж�Դ �����ж�
    RTC_ITConfig(RTC_IT_WUT, ENABLE);

    // 17. ���RTC�����жϵı�־
    RTC_ClearITPendingBit(RTC_IT_WUT);

    // 18. ʹ��RTC���Ѽ�����
    RTC_WakeUpCmd(ENABLE);
}

void ParseAndSetRTC(char *time_str)
{
    int year, month, day, hour, minute, second;
    if (sscanf(time_str, "%d-%d-%d %d:%d:%d", &year, &month, &day, &hour, &minute, &second) == 6)
    {
		Rtc_Init();
        // ���� RTC ����
        RTC_DateStructure.RTC_Year  = (uint8_t)(year % 100); // ֻȡ��ݵĺ���λ
        RTC_DateStructure.RTC_Month = (uint8_t)month;
        RTC_DateStructure.RTC_Date  = (uint8_t)day;
        RTC_SetDate(RTC_Format_BIN, &RTC_DateStructure);

        // ���� RTC ʱ��
        RTC_TimeStructure.RTC_Hours   = (uint8_t)hour;
        RTC_TimeStructure.RTC_Minutes = (uint8_t)minute;
        RTC_TimeStructure.RTC_Seconds = (uint8_t)second;
        RTC_SetTime(RTC_Format_BIN, &RTC_TimeStructure);
    }
}


//��ȡ����
void Rtc_read(void)
{
	RTC_TimeTypeDef		RTC_Timedata;
	RTC_DateTypeDef	 	RTC_Datedata;
	
	RTC_GetTime(RTC_Format_BIN, &RTC_Timedata);
	RTC_GetDate(RTC_Format_BIN, &RTC_Datedata);
	
	printf("20%d��%d��%d������%d %d:%d:%d\r\n",RTC_Datedata.RTC_Year,RTC_Datedata.RTC_Month,RTC_Datedata.RTC_Date, \
	RTC_Datedata.RTC_WeekDay,RTC_Timedata.RTC_Hours,RTC_Timedata.RTC_Minutes,RTC_Timedata.RTC_Seconds);
	//printf("%d年\r\n",RTC_Datedata.RTC_Year);
	
	char str[4];
	snprintf(str, sizeof(str), "%d", RTC_Datedata.RTC_Date);
	lv_label_set_text(ui_Label5,str);
	snprintf(str, sizeof(str), "%d", RTC_Timedata.RTC_Hours);
	lv_label_set_text(ui_Label1,str);
	snprintf(str, sizeof(str), "%d", RTC_Timedata.RTC_Minutes);
	lv_label_set_text(ui_Label4,str);

}



//����A��ʼ��
void RTC_Alarm_AInit(uint8_t hour,uint8_t minute)
{
	
	RTC_AlarmTypeDef	RTC_AlarmStruct;
	RTC_TimeTypeDef 	RTC_AlarmTime;
	EXTI_InitTypeDef   EXTI_InitStructure;
	NVIC_InitTypeDef   NVIC_InitStructure;	
	
	//�ر����ӣ�
	RTC_AlarmCmd(RTC_Alarm_A,DISABLE); 
	
	//����ʱ������
	RTC_AlarmTime.RTC_H12		= RTC_H12_AM; //���磬��24Сʱ��ʱ���������������
	RTC_AlarmTime.RTC_Hours		= hour; //ʱ
	RTC_AlarmTime.RTC_Minutes	= minute; //��
	RTC_AlarmTime.RTC_Seconds	= 00; //��

	
	RTC_AlarmStruct.RTC_AlarmTime			= RTC_AlarmTime; //ʱ������
	RTC_AlarmStruct.RTC_AlarmMask			= RTC_AlarmMask_None;//������λ����ʵ������ʱ������Ӧ
	RTC_AlarmStruct.RTC_AlarmDateWeekDaySel	= RTC_AlarmDateWeekDaySel_Date; //��������Ӧ�¿γ� 
	RTC_AlarmStruct.RTC_AlarmDateWeekDay	= 9;
	
	//�������Ӳ�����
	RTC_SetAlarm(RTC_Format_BIN,RTC_Alarm_A, &RTC_AlarmStruct);
	//�������������жϣ�
	
	
	//�����ⲿ�жϿ�����
	EXTI_InitStructure.EXTI_Line 	= EXTI_Line17;  			//�ⲿ�ж���14
	EXTI_InitStructure.EXTI_Mode 	= EXTI_Mode_Interrupt; 	//�ж�ģʽ
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising; //�����ش��� 
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;				//�ж���ʹ��
	EXTI_Init(&EXTI_InitStructure);
	
	//NVIC����
	NVIC_InitStructure.NVIC_IRQChannel = RTC_Alarm_IRQn;   				//ͨ����ţ�����stm32f4xx.h�в鿴
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x01; 	//��ռ���ȼ�
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x00;			//��Ӧ���ȼ�
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;					//ͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);
	
	//��������A�ж�
	RTC_ITConfig(RTC_IT_ALRA, ENABLE);
	//�������ӣ�
	RTC_AlarmCmd(RTC_Alarm_A,ENABLE);
	
	
	printf("clock have set \r\n");


}



//����(��������A������B)�жϷ������
void RTC_Alarm_IRQHandler(void)
{
	if(RTC_GetITStatus(RTC_IT_ALRA) != RESET)
	{
		BEEP();
		printf("ʱ�䵽��\r\n");
		RTC_ClearITPendingBit(RTC_IT_ALRA);
		EXTI_ClearITPendingBit(EXTI_Line17);
	} 
}


void BEEP(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	
	//1.��GPIOF�˿ڵ�ʱ��
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF, ENABLE);
	
	//2.����PF8�����ĸ�����
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOF, &GPIO_InitStructure);
	
 
	//3.����ߵ�ƽ
	GPIO_SetBits(GPIOF,GPIO_Pin_8);

	delay_ms(100);

	//3.����͵�ƽ
	GPIO_ResetBits(GPIOF,GPIO_Pin_8);

}

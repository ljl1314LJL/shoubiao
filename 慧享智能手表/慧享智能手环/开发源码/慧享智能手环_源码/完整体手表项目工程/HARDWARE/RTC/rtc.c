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

    // 1. 打开PWR外设的时钟
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);

    // 2. 对PWR_CR寄存器的DBP位置位，解除RTC写保护寄存器
    PWR_BackupAccessCmd(ENABLE);

    // 3. 使能LSE时钟
    RCC_LSEConfig(RCC_LSE_ON);

    // 4. 等待LSE就绪
    while(RCC_GetFlagStatus(RCC_FLAG_LSERDY) == RESET);

    // 5. 选择RTC的时钟源
    RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);

    // 6. 使能RTC的时钟
    RCC_RTCCLKCmd(ENABLE);

    // 7. 等待RTC寄存器同步完成 对RTC寄存器解除写保护
    RTC_WaitForSynchro();

    // 8. 设置RTC的小时格式、预分频值
    RTC_InitStructure.RTC_AsynchPrediv = 128 - 1; // 异步分频
    RTC_InitStructure.RTC_SynchPrediv  = 256 - 1; // 同步分频
    RTC_InitStructure.RTC_HourFormat   = RTC_HourFormat_24; // 24小时制
    RTC_Init(&RTC_InitStructure);

//    // 9. 设置RTC的日期 2025/04/09 星期三
//    RTC_DateStructure.RTC_Year  = 0x25; // 年份 BCD格式
//    RTC_DateStructure.RTC_Month = 0x04; // 月份 BCD格式
//    RTC_DateStructure.RTC_Date  = 0x09; // 日期 BCD格式
//    RTC_SetDate(RTC_Format_BCD, &RTC_DateStructure);

//    // 10. 设置RTC的时间
//    RTC_TimeStructure.RTC_Hours   = 0x16; // 小时 BCD格式
//    RTC_TimeStructure.RTC_Minutes = 0x30; // 分钟 BCD格式
//    RTC_TimeStructure.RTC_Seconds = 0x00; // 秒钟 BCD格式
//    RTC_SetTime(RTC_Format_BCD, &RTC_TimeStructure);

    // 11. 设置唤醒中断通道的参数 NVIC
    NVIC_InitStructure.NVIC_IRQChannel = RTC_WKUP_IRQn; // 唤醒中断通道
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    // 12. 设置RTC唤醒中断的EXTI22
    EXTI_ClearITPendingBit(EXTI_Line22); // 清除中断线标志
    EXTI_InitStructure.EXTI_Line = EXTI_Line22;
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStructure);

    // 13. 关闭RTC唤醒定时器
    RTC_WakeUpCmd(DISABLE);

    // 14. 选择RTC的唤醒定时器的时钟源 内部时钟 1HZ
    RTC_WakeUpClockConfig(RTC_WakeUpClock_CK_SPRE_16bits);

    // 15. 设置RTC唤醒自动重载计数器
    RTC_SetWakeUpCounter(0);

    // 16. 选择RTC的中断源 唤醒中断
    RTC_ITConfig(RTC_IT_WUT, ENABLE);

    // 17. 清除RTC唤醒中断的标志
    RTC_ClearITPendingBit(RTC_IT_WUT);

    // 18. 使能RTC唤醒计数器
    RTC_WakeUpCmd(ENABLE);
}

void ParseAndSetRTC(char *time_str)
{
    int year, month, day, hour, minute, second;
    if (sscanf(time_str, "%d-%d-%d %d:%d:%d", &year, &month, &day, &hour, &minute, &second) == 6)
    {
		Rtc_Init();
        // 设置 RTC 日期
        RTC_DateStructure.RTC_Year  = (uint8_t)(year % 100); // 只取年份的后两位
        RTC_DateStructure.RTC_Month = (uint8_t)month;
        RTC_DateStructure.RTC_Date  = (uint8_t)day;
        RTC_SetDate(RTC_Format_BIN, &RTC_DateStructure);

        // 设置 RTC 时间
        RTC_TimeStructure.RTC_Hours   = (uint8_t)hour;
        RTC_TimeStructure.RTC_Minutes = (uint8_t)minute;
        RTC_TimeStructure.RTC_Seconds = (uint8_t)second;
        RTC_SetTime(RTC_Format_BIN, &RTC_TimeStructure);
    }
}


//读取数据
void Rtc_read(void)
{
	RTC_TimeTypeDef		RTC_Timedata;
	RTC_DateTypeDef	 	RTC_Datedata;
	
	RTC_GetTime(RTC_Format_BIN, &RTC_Timedata);
	RTC_GetDate(RTC_Format_BIN, &RTC_Datedata);
	
	printf("20%d年%d月%d日星期%d %d:%d:%d\r\n",RTC_Datedata.RTC_Year,RTC_Datedata.RTC_Month,RTC_Datedata.RTC_Date, \
	RTC_Datedata.RTC_WeekDay,RTC_Timedata.RTC_Hours,RTC_Timedata.RTC_Minutes,RTC_Timedata.RTC_Seconds);
	//printf("%d骞碶r\n",RTC_Datedata.RTC_Year);
	
	char str[4];
	snprintf(str, sizeof(str), "%d", RTC_Datedata.RTC_Date);
	lv_label_set_text(ui_Label5,str);
	snprintf(str, sizeof(str), "%d", RTC_Timedata.RTC_Hours);
	lv_label_set_text(ui_Label1,str);
	snprintf(str, sizeof(str), "%d", RTC_Timedata.RTC_Minutes);
	lv_label_set_text(ui_Label4,str);

}



//闹钟A初始化
void RTC_Alarm_AInit(uint8_t hour,uint8_t minute)
{
	
	RTC_AlarmTypeDef	RTC_AlarmStruct;
	RTC_TimeTypeDef 	RTC_AlarmTime;
	EXTI_InitTypeDef   EXTI_InitStructure;
	NVIC_InitTypeDef   NVIC_InitStructure;	
	
	//关闭闹钟：
	RTC_AlarmCmd(RTC_Alarm_A,DISABLE); 
	
	//闹钟时间设置
	RTC_AlarmTime.RTC_H12		= RTC_H12_AM; //上午，在24小时制时，这个参数是无用
	RTC_AlarmTime.RTC_Hours		= hour; //时
	RTC_AlarmTime.RTC_Minutes	= minute; //分
	RTC_AlarmTime.RTC_Seconds	= 00; //秒

	
	RTC_AlarmStruct.RTC_AlarmTime			= RTC_AlarmTime; //时间设置
	RTC_AlarmStruct.RTC_AlarmMask			= RTC_AlarmMask_None;//无掩码位，按实际设置时间来响应
	RTC_AlarmStruct.RTC_AlarmDateWeekDaySel	= RTC_AlarmDateWeekDaySel_Date; //按日期响应新课程 
	RTC_AlarmStruct.RTC_AlarmDateWeekDay	= 9;
	
	//配置闹钟参数：
	RTC_SetAlarm(RTC_Format_BIN,RTC_Alarm_A, &RTC_AlarmStruct);
	//开启配置闹钟中断：
	
	
	//配置外部中断控制器
	EXTI_InitStructure.EXTI_Line 	= EXTI_Line17;  			//外部中断线14
	EXTI_InitStructure.EXTI_Mode 	= EXTI_Mode_Interrupt; 	//中断模式
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising; //上升沿触发 
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;				//中断线使能
	EXTI_Init(&EXTI_InitStructure);
	
	//NVIC配置
	NVIC_InitStructure.NVIC_IRQChannel = RTC_Alarm_IRQn;   				//通道编号，可在stm32f4xx.h中查看
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x01; 	//抢占优先级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x00;			//响应优先级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;					//通道使能
	NVIC_Init(&NVIC_InitStructure);
	
	//开启闹钟A中断
	RTC_ITConfig(RTC_IT_ALRA, ENABLE);
	//开启闹钟：
	RTC_AlarmCmd(RTC_Alarm_A,ENABLE);
	
	
	printf("clock have set \r\n");


}



//闹钟(包括闹钟A与闹钟B)中断服务代码
void RTC_Alarm_IRQHandler(void)
{
	if(RTC_GetITStatus(RTC_IT_ALRA) != RESET)
	{
		BEEP();
		printf("时间到了\r\n");
		RTC_ClearITPendingBit(RTC_IT_ALRA);
		EXTI_ClearITPendingBit(EXTI_Line17);
	} 
}


void BEEP(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	
	//1.打开GPIOF端口的时钟
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF, ENABLE);
	
	//2.设置PF8引脚四个参数
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOF, &GPIO_InitStructure);
	
 
	//3.输出高电平
	GPIO_SetBits(GPIOF,GPIO_Pin_8);

	delay_ms(100);

	//3.输出低电平
	GPIO_ResetBits(GPIOF,GPIO_Pin_8);

}

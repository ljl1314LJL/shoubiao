#ifndef __RTC_H
#define __RTC_H


#include "stm32f4xx.h"
#include "delay.h"
#include "stdio.h"


extern RTC_TimeTypeDef  RTC_TimeStructure;
extern RTC_DateTypeDef  RTC_DateStructure;
void Rtc_Init();
void Rtc_read(void);


//闹钟A初始化
void RTC_Alarm_AInit(uint8_t hour,uint8_t minute);

//闹钟(包括闹钟A与闹钟B)中断服务代码
void RTC_Alarm_IRQHandler(void);

void ParseAndSetRTC(char *time_str);
void BEEP(void);

void create_sport();
#endif


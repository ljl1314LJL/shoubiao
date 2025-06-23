#include <stdio.h>
#include "delay.h"
#include "uart.h"
#include "string.h"
#include "includes.h"



#include "max30102.h"
#include "algorithm.h"
#include "rtc.h"
#include "mpu6050.h"
#include "inv_mpu.h"
#include "inv_mpu_dmp_motion_driver.h"

//#include "i2c.h"

#define MAX_BRIGHTNESS 255

TaskHandle_t xStart_Handle = NULL;	     //用于更新任务句柄
TaskHandle_t xLVGL_Handle = NULL;		 //用于更新任务句柄
 
TaskHandle_t xWIFI_Handle = NULL;			 //用于更新任务句柄
TaskHandle_t xMPU_Handle = NULL;			 //用于更新任务句柄
TaskHandle_t xWalk_Handle = NULL;			 //用于更新任务句柄
TaskHandle_t xCount_Handle = NULL;			 //用于更新任务句柄
TaskHandle_t xlcd_light = NULL;			 //用于更新任务句柄
TaskHandle_t xlcd_dark = NULL;			 //用于更新任务句柄
//TaskHandle_t xHeart_Handle = NULL;			 //用于更新任务句柄
//TaskHandle_t xClock_Handle = NULL;			 //用于更新任务句柄
//TaskHandle_t xBT_Handle = NULL;			 //用于更新任务句柄

__IO uint32_t uwTimeDisplay = 0; // 时间显示标志
SemaphoreHandle_t xSemaphore;		//用于记录互斥锁句柄
uint32_t cnt = 4000;
int32_t n_heart_rate;   //heart rate value


uint32_t g_system_no_opreation_cnt = 0;				//息屏计数
uint32_t g_system_no_opreation_max = 10;			//息屏的时长
uint32_t g_system_no_opreation_flag = 1;			//是否开启息屏
uint8_t TaiWan_flag = 0; 							//抬腕亮屏是否开启标志

extern lv_obj_t * ui_Image18;
extern lv_obj_t * ui_Image10;
extern lv_obj_t * ui_Image23;
extern lv_obj_t * ui_Image24;

//lcd显示打开/关闭
void lcd_display_on(uint8_t on)
{
	if(on)
		LCD_BLK_1;	 	// 命令
	else
		LCD_BLK_0;	 	// 命令
 
}

//用于亮屏的任务
void vlcd_light_Handle(void * pvParameters){
	while(1){
		//printf("1111111111111111111\r\n");
		if(tp_finger_num_get() > 0){
		//	printf("tp_finger_num_get11111111111111111111111111111111111111\r\n");
			//printf("222222222222222222222222222\r\n");
			lcd_display_on(1);
			g_system_no_opreation_cnt = 0;
			//printf("11111111111111111111111\r\n");
		}
		vTaskDelay(100);
	}
} 

//用于没有操作时息屏任务
void vlcd_dark_Handle(void * pvParameters )
{
	
	//TickType_t xLastWakeTime;
		
	while(1){
		//xLastWakeTime = xTaskGetTickCount();
		//互斥锁上锁
		//xSemaphoreTake(xSemaphore,portMAX_DELAY );
		//if(g_system_no_opreation_flag != 0){
			g_system_no_opreation_cnt++;
			//printf("########################%d#######################\r\n",g_system_no_opreation_cnt);
			if(g_system_no_opreation_cnt >= g_system_no_opreation_max){
				g_system_no_opreation_cnt = 0;
				lcd_display_on(0);			//息屏
			}
		//}

		//互斥锁解锁 
	//	xSemaphoreGive(xSemaphore);
		//绝对延时函数
		//vTaskDelayUntil(&xLastWakeTime, 1000);
		vTaskDelay(1000);
	}

}


void extract_weather_info(const char *json, char *code, char *weather, char *temperature) {
    const char *start, *end;

    // 提取代码
    start = strstr(json, "\"code\":\"");
    if (start) {
        start += 8; // 跳过前缀
        end = strchr(start, '"');
        if (end) {
            strncpy(code, start, end - start);
            code[end - start] = '\0'; // 确保字符串以null结尾
        }
    }

    // 提取天气
    start = strstr(json, "\"text\":\"");
    if (start) {
        start += 8; // 跳过前缀
        end = strchr(start, '"');
        if (end) {
            strncpy(weather, start, end - start);
            weather[end - start] = '\0'; // 确保字符串以null结尾
        }
    }

    // 提取温度
    start = strstr(json, "\"temperature\":\"");
    if (start) {
        start += 15; // 跳过前缀
        end = strchr(start, '"');
        if (end) {
            strncpy(temperature, start, end - start);
            temperature[end - start] = '\0'; // 确保字符串以null结尾
        }
    }
}

void RTC_WKUP_IRQHandler(void)
{
    if (RTC_GetITStatus(RTC_IT_WUT) != RESET)
    {
        uwTimeDisplay = 1;

        // 清除中断标志
        RTC_ClearITPendingBit(RTC_IT_WUT);
        EXTI_ClearITPendingBit(EXTI_Line22);
    }
}

void Select_Number(char * str)
{
 
    char date[9], time[9];
    int year, month, day, hour, minute, second;

    // 提取日期部分
    sscanf(str, "%d-%d-%d", &year, &month, &day);
    sprintf(date, "%04d%02d%02d", year, month, day);

    // 提取时间部分
    sscanf(str + 11, "%d:%d:%d", &hour, &minute, &second);
    sprintf(time, "%02d%02d%02d", hour, minute, second);

//    printf("提取的日期数字为: %s\n", date);
//    printf("提取的时间数字为: %s\n", time);
	
	Rtc_Init(hour,minute,second,month,day,3);


}


//运动计时
void vCount_Handle(void * pvParameters )
{
	
	//进来就先挂起
	vTaskSuspend(NULL);
	
	
	uint8_t seconds = 0;  // 秒计数器
	uint8_t minutes = 0;  // 分钟计数器
	uint8_t hours = 0;    // 小时计数器
	//double heart;	      //心率
	double hot = 0;

	char sec_text[3];	
	char min_text[3];
	char hour_text[3];	
	char h_r[4];
	
	while (1) 
	{
		//heart = (n_heart_rate/100);
	
	// 每秒更新一次
	vTaskDelay(1000);  // 延时1000ms

	// 更新秒数
	seconds++;

	snprintf(sec_text, sizeof(sec_text), "%d", seconds);
	lv_label_set_text(ui_Label44, sec_text);
	//printf("0000000000000000    ====     %s\r\n",sec_text);
	// 每60秒更新一次分钟数
		if (seconds > 59) 
		{
			minutes++;

			snprintf(min_text, sizeof(min_text), "%d", minutes);
			lv_label_set_text(ui_Label42, min_text);

			// 重置秒数
			seconds = 0;

			// 每60分钟更新一次小时数
			if (minutes > 59) 
			{
				hours++;

				snprintf(hour_text, sizeof(hour_text), "%d", hours);
				lv_label_set_text(ui_Label40, hour_text);

				// 重置分钟数
				minutes = 0;
			}
		}
		hot += 0.015;
		snprintf(h_r, sizeof(h_r), "%f", hot);
		lv_label_set_text(ui_Label46, h_r);

		
		 

	}
}

//将创建运动任务作为一个函数，每当运动一次就调用一次
void create_sport()
{
	/* 创建运动计时的任务 */
	xTaskCreate((TaskFunction_t)vCount_Handle,          	/* 任务入口函数 */
				(const char *)"app_task_count",          /* 任务名字 */
				(uint16_t)512,                          /* 任务栈大小 */
				(void *)NULL,                           /* 任务入口函数参数 */
				(UBaseType_t)6,                         /* 任务的优先级 */
				(TaskHandle_t *)&xCount_Handle); 		/* 任务控制块指针 */	
}

//抬腕显示
void vMPU_Handle(void * pvParameters )
{
	uint8_t res;
	
	float pitch,roll,yaw; 		//欧拉角
	short aacx,aacy,aacz;		//加速度传感器原始数据
	short gyrox,gyroy,gyroz;	//陀螺仪原始数据
//	short temp;					//温度		
	while(1)
	{
//		res=mpu_dmp_get_data(&pitch,&roll,&yaw);
//		if(res==0)
//		{ 
		///printf("-----------------------------------000000000000000000\r\n");
//			temp=MPU_Get_Temperature();	//得到温度值
			MPU_Get_Accelerometer(&aacx,&aacy,&aacz);	//得到加速度传感器数据
			MPU_Get_Gyroscope(&gyrox,&gyroy,&gyroz);	//得到陀螺仪数据
	//	printf("加速度：x=%d y=%d z=%d",aacx,aacy,aacz);
		//	printf("陀螺仪：x=%d y=%d z=%d\r\n",gyrox,gyroy,gyroz);
			if( aacy > -12000 && aacy < -8000)
			{
				printf("show lcd on up\r\n");
				lcd_display_on(1);
				g_system_no_opreation_cnt = 0;
			
			}vTaskDelay(100);
//		}	

	}
}

//获取步数
void vWalk_Handle(void * pvParameters )
{
	printf("create vWalk_Handle\r\n");
	unsigned long  step_count_last=0;
	unsigned long  step_count=0;
	//uint32_t sedentary_event=0;
//	uint32_t t=0;

	//挂起调度器
	vTaskSuspendAll( );
	//初始化MPU6050	
	MPU_Init();

	while(mpu_dmp_init())
	{
	//	printf("[ERROR] MPU6050 ERROR \r\n");
		vTaskDelay(500);
		MPU_Init();
	}


	/* 设置步数初值为0*/
	while(dmp_set_pedometer_step_count(0))
	{
		vTaskDelay(500);
	}
	
	//恢复调度器
	xTaskResumeAll( );	
	char str[3];
	while(1)
	{
		/* 获取步数 */
		dmp_get_pedometer_step_count(&step_count);
		if(step_count_last != step_count){
				//step_all += step_count;
				//set_main_walk(step_count);
			step_count_last = step_count;
		}

		
	//	printf("[INFO] 当前步数:%ld\r\n",step_count);
		snprintf(str,sizeof(str),"%ld",step_count);
		lv_label_set_text(ui_Label8,str);
		step_count = 0;
		/* 延时一会 */
		vTaskDelay(1000);
	}
}

//获取天气
void vGetWeather_Handle(void)
{
	
	printf("create vGetWeather_Handle OK\r\n");
	
	//退出透传
	UART3_SendString("+++"); 
	 
	vTaskDelay(50);
	UART3_SendString("+++"); 
	vTaskDelay(50);
	
	//测试指令  响应"OK\r\n"
	UART3_SendString("AT\r\n"); 
	vTaskDelay(100);
	
	UART1_SendString((char *)Rx3Buffer);  //进行测试
	
	memset(Rx3Buffer,0,BUFFERSIZE); //清空接收缓冲区
	Rx3Index = 0;                   //重置接收计数器
	
	//设置模式  3：STA+AP 响应"OK\r\n"
	UART3_SendString("AT+CWMODE=3\r\n"); 
	vTaskDelay(1000);
	
	UART1_SendString((char *)Rx3Buffer);  //进行测试
	memset(Rx3Buffer,0,BUFFERSIZE); //清空接收缓冲区
	Rx3Index = 0;                   //重置接收计数器
	
	//连接AP  响应"OK\r\n"
	UART3_SendString("AT+CWJAP=\"878\",\"123456789\"\r\n"); 
	while(cnt--)
	{
		vTaskDelay(2);
		
		if(strstr((char *)Rx3Buffer,"ok"))
		{
			break;
		}
	}
//	vTaskDelay(4000);
	
	UART1_SendString((char *)Rx3Buffer);  //进行测试
	memset(Rx3Buffer,0,BUFFERSIZE); //清空接收缓冲区
	Rx3Index = 0;                   //重置接收计数器
	
	
	//使能透传模式
	UART3_SendString("AT+CIPMODE=1\r\n"); 
	vTaskDelay(4000);
	
	UART1_SendString((char *)Rx3Buffer);  //进行测试
	memset(Rx3Buffer,0,BUFFERSIZE); //清空接收缓冲区
	Rx3Index = 0;                   //重置接收计数器
	
//获取天气

	//连接服务器 
	UART3_SendString("AT+CIPSTART=\"TCP\",\"116.62.81.138\",80\r\n"); 
	cnt = 4000;
	while(cnt--)
	{
		vTaskDelay(2);
		
		if(strstr((char *)Rx3Buffer,"ok"))
		{
			break;
		}
	}
//	vTaskDelay(4000);
		
	UART1_SendString((char *)Rx3Buffer);  //进行测试
	memset(Rx3Buffer,0,BUFFERSIZE); //清空接收缓冲区
	Rx3Index = 0;                   //重置接收计数器
	


	//进入透传模式
	UART3_SendString("AT+CIPSEND\r\n"); 

//	vTaskDelay(10000);
	cnt = 4000;
	while(cnt--)
	{
		vTaskDelay(2);

		if(strstr((char *)Rx3Buffer,">"))
		{
			break;
		}
	}	

	
	UART1_SendString((char *)Rx3Buffer);  //进行测试
	memset(Rx3Buffer,0,BUFFERSIZE); //清空接收缓冲区
	Rx3Index = 0;                   //重置接收计数器
	
	//	WifiInitOK_Flag = 1; //WIFI模块初始化成功


	printf("*********************\r\n");
	//订阅主题
	UART3_SendString("GET https://api.seniverse.com/v3/weather/now.json?key=S5jYTIuliBSEQmhkX&location=guangzhou&unit=c\r\n"); 
	cnt = 1000;
	while(cnt--)
	{
		vTaskDelay(2);
		//UART1_SendString((char *)Rx3Buffer);  //进行测试
		if(strstr((char *)Rx3Buffer,"\r\n"))
		{
			break;
		}
	}
	
	char code[3];
	char weather[8];
	char tmp[5];
	
	extract_weather_info((char *)Rx3Buffer,code,weather,tmp);
	
//	printf("code ========================= %s\r\n",code);
//	printf("weather ========================= %s\r\n",weather);
	printf("tmp ========================= %s\r\n",tmp);
	if(atoi(code) <= 3)
	{
		//晴朗
		lv_label_set_text(ui_Label18,"晴");
		lv_label_set_text(ui_Label22,tmp);		
		lv_obj_add_flag(ui_Image18,LV_OBJ_FLAG_HIDDEN);
		
		lv_obj_clear_flag(ui_Image24,LV_OBJ_FLAG_HIDDEN);
		
	}
	else if(atoi(code) <= 8)
	{
		lv_label_set_text(ui_Label22,tmp);
	}
	else if(atoi(code) == 9)
	{
		//阴
		lv_label_set_text(ui_Label22,tmp);
		lv_label_set_text(ui_Label18,"阴");
	}
	
	else if(atoi(code) == 13)
	{
		//小雨
		lv_label_set_text(ui_Label18,"小雨");
		lv_label_set_text(ui_Label22,tmp);
		lv_obj_add_flag(ui_Image18,LV_OBJ_FLAG_HIDDEN);
		
		lv_obj_clear_flag(ui_Image10,LV_OBJ_FLAG_HIDDEN);
		
	}
	
	else if(atoi(code) == 15)
	{
		//大雨
		lv_label_set_text(ui_Label18,"大雨");
		lv_label_set_text(ui_Label22,tmp);
		lv_obj_add_flag(ui_Image18,LV_OBJ_FLAG_HIDDEN);
		
		lv_obj_clear_flag(ui_Image23,LV_OBJ_FLAG_HIDDEN);
		
	}
	


}







//获取心率血氧
void vHeart_Handle( void )
{
	printf("create heart\r\n");
	//variables to calculate the on-board LED brightness that reflects the heartbeats
	uint32_t un_min, un_max, un_prev_data;  
	int32_t i;
	int32_t n_brightness;
	float f_temp;

	uint8_t temp[6];

	//NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);
	
	GPIO_InitTypeDef		GPIO_InitStruct;
	
	uint32_t aun_ir_buffer[500]; //IR LED sensor data
	int32_t n_ir_buffer_length;    //data length
	uint32_t aun_red_buffer[500];    //Red LED sensor data
	int32_t n_sp02; //SPO2 value
	int8_t ch_spo2_valid;   //indicator to show if the SP02 calculation is valid

	int8_t  ch_hr_valid;    //indicator to show if the heart rate calculation is valid
//	uint8_t uch_dummy;

	
	//使能端口F的硬件时钟，端口F才能工作，说白了就是对端口F上电
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF, ENABLE);	

	//配置硬件，配置GPIO，端口F，第9个引脚
	GPIO_InitStruct.GPIO_Pin=GPIO_Pin_9;			//第9 个引脚
	GPIO_InitStruct.GPIO_Mode=GPIO_Mode_OUT;		//输出模式
	GPIO_InitStruct.GPIO_Speed=GPIO_High_Speed;		//引脚高速工作，收到指令立即工作；缺点：功耗高
	GPIO_InitStruct.GPIO_OType=GPIO_OType_PP;		//增加输出电流的能力
	GPIO_InitStruct.GPIO_PuPd=GPIO_PuPd_NOPULL;		//不需要上下拉电阻
	GPIO_Init(GPIOF,&GPIO_InitStruct);
	
	PFout(9)=1;
	

	max30102_init();

	printf("This is max30102 test by teacher.wen\r\n");

	un_min=0x3FFFF;
	un_max=0;
	
	n_ir_buffer_length=500; //buffer length of 100 stores 5 seconds of samples running at 100sps
	//read the first 500 samples, and determine the signal range
    for(i=0;i<n_ir_buffer_length;i++)
    {
        while(MAX30102_INT==1);   //wait until the interrupt pin asserts
        
		max30102_FIFO_ReadBytes(REG_FIFO_DATA,temp);
		aun_red_buffer[i] =  (long)((long)((long)temp[0]&0x03)<<16) | (long)temp[1]<<8 | (long)temp[2];    // Combine values to get the actual number
		aun_ir_buffer[i] = (long)((long)((long)temp[3] & 0x03)<<16) |(long)temp[4]<<8 | (long)temp[5];   // Combine values to get the actual number
            
        if(un_min>aun_red_buffer[i])
            un_min=aun_red_buffer[i];    //update signal min
        if(un_max<aun_red_buffer[i])
            un_max=aun_red_buffer[i];    //update signal max
    }
	un_prev_data=aun_red_buffer[i];
	//calculate heart rate and SpO2 after first 500 samples (first 5 seconds of samples)
    maxim_heart_rate_and_oxygen_saturation(aun_ir_buffer, n_ir_buffer_length, aun_red_buffer, &n_sp02, &ch_spo2_valid, &n_heart_rate, &ch_hr_valid); 
	uint8_t num = 10;
	while(num--)
	{
		i=0;
        un_min=0x3FFFF;
        un_max=0;
		n_ir_buffer_length=500;
		
		/* dumping the first 100 sets of samples in the memory and shift the last 400 sets of samples to the top
		
		   将前100组样本转储到存储器中，并将最后400组样本移到顶部
		*/
		
        for(i=100;i<500;i++)
        {
            aun_red_buffer[i-100]=aun_red_buffer[i];
            aun_ir_buffer[i-100]=aun_ir_buffer[i];
            
            /* update the signal min and max 
			   更新信号最小值和最大值
			*/
			
            if(un_min>aun_red_buffer[i])
				un_min=aun_red_buffer[i];
			
            if(un_max<aun_red_buffer[i])
				un_max=aun_red_buffer[i];
        }
		
		/* take 100 sets of samples before calculating the heart rate 
		
		   在计算心率之前采集100组样本
		*/
		
        for(i=400;i<500;i++)
        {
            un_prev_data=aun_red_buffer[i-1];
			
            while(MAX30102_INT==1);
			
            max30102_FIFO_ReadBytes(REG_FIFO_DATA,temp);
			
			/* 组合值以获得实际数字 */
			aun_red_buffer[i] =  ((temp[0]&0x03)<<16) |(temp[1]<<8) | temp[2];   
			aun_ir_buffer[i] =   ((temp[3]&0x03)<<16) |(temp[4]<<8) | temp[5];   
        
            if(aun_red_buffer[i]>un_prev_data)
            {
                f_temp=aun_red_buffer[i]-un_prev_data;
				
                f_temp/=(un_max-un_min);
				
                f_temp*=MAX_BRIGHTNESS;
				
                n_brightness-=(int32_t)f_temp;
				
                if(n_brightness<0)
                    n_brightness=0;
            }
            else
            {
                f_temp=un_prev_data-aun_red_buffer[i];
				
                f_temp/=(un_max-un_min);
				
                f_temp*=MAX_BRIGHTNESS;
				
                n_brightness+=(int32_t)f_temp;
				
                if(n_brightness>MAX_BRIGHTNESS)
                    n_brightness=MAX_BRIGHTNESS;
            }
		}

		/* 计算心率和血氧饱和度 */
        maxim_heart_rate_and_oxygen_saturation(aun_ir_buffer, n_ir_buffer_length, aun_red_buffer, &n_sp02, &ch_spo2_valid, &n_heart_rate, &ch_hr_valid);
			
		
		char str[4];

		
		/* 通过UART将样本和计算结果发送到终端程序 */
		if((ch_hr_valid == 1)&& (n_heart_rate>=60) && (n_heart_rate<=120))
		{
			//设置心率
			snprintf(str, sizeof(str), "%d", n_heart_rate);
			lv_label_set_text(ui_Label7,str);
			lv_label_set_text(ui_Label38,str);
			lv_label_set_text(ui_Label19,str);
	 
			
			printf("心率=%d\r\n", n_heart_rate); 
		}

		
		if((ch_spo2_valid ==1)&& (n_sp02>=95) && (n_sp02<100))
		{
			//设置血氧
			snprintf(str, sizeof(str), "%d", n_sp02);
			lv_label_set_text(ui_Label26,str);

			
			printf("血氧浓度=%d\r\n", n_sp02); 
		}	
	
		//delay_ms(100);
		//vTaskDelay(100);
		
	}

	printf("test heart and algorithm over\r\n");
			//	vTaskDelete( NULL );
		
	
}









//获取时间
void vWIFI_Handle( void * pvParameters )
{	
	vGetWeather_Handle();

	
	

	//vGetWeather_Handle();
	printf("create WIFI\r\n");
//	uint32_t cnt = 8000;
	
	//退出透传
	UART3_SendString("+++"); 
	 
	vTaskDelay(50);
	UART3_SendString("+++"); 
	vTaskDelay(50);
	
	//测试指令  响应"OK\r\n"
	UART3_SendString("AT\r\n"); 
	vTaskDelay(100);
	
	UART1_SendString((char *)Rx3Buffer);  //进行测试
	
	memset(Rx3Buffer,0,BUFFERSIZE); //清空接收缓冲区
	Rx3Index = 0;                   //重置接收计数器
	
	//设置模式  3：STA+AP 响应"OK\r\n"
	UART3_SendString("AT+CWMODE=3\r\n"); 
	vTaskDelay(1000);
	
	UART1_SendString((char *)Rx3Buffer);  //进行测试
	memset(Rx3Buffer,0,BUFFERSIZE); //清空接收缓冲区
	Rx3Index = 0;                   //重置接收计数器
	
	//连接AP  响应"OK\r\n"
//	UART3_SendString("AT+CWJAP=\"878\",\"123456789\"\r\n"); 
//	while(cnt--)
//	{
//		vTaskDelay(2);
//		
//		if(strstr((char *)Rx3Buffer,"ok"))
//		{
//			break;
//		}
//	}
//	vTaskDelay(4000);
	
	UART1_SendString((char *)Rx3Buffer);  //进行测试
	memset(Rx3Buffer,0,BUFFERSIZE); //清空接收缓冲区
	Rx3Index = 0;                   //重置接收计数器
	
	
	//使能透传模式
	UART3_SendString("AT+CIPMODE=1\r\n"); 
	vTaskDelay(4000);
	
	UART1_SendString((char *)Rx3Buffer);  //进行测试
	memset(Rx3Buffer,0,BUFFERSIZE); //清空接收缓冲区
	Rx3Index = 0;                   //重置接收计数器
	
	//断开连接
	UART3_SendString("AT+CIPCLOSE\r\n"); 
	vTaskDelay(4000);
	
	UART1_SendString((char *)Rx3Buffer);  //进行测试
	memset(Rx3Buffer,0,BUFFERSIZE); //清空接收缓冲区
	Rx3Index = 0;                   //重置接收计数器
//获取时间	
{
	//连接服务器 
	UART3_SendString("AT+CIPSTART=\"TCP\",\"bemfa.com\",8344\r\n"); 
	cnt = 4000;
	while(cnt--)
	{
		vTaskDelay(2);
		
		if(strstr((char *)Rx3Buffer,"ok"))
		{
			break;
		}
	}
//	vTaskDelay(4000);
		
	UART1_SendString((char *)Rx3Buffer);  //进行测试
	memset(Rx3Buffer,0,BUFFERSIZE); //清空接收缓冲区
	Rx3Index = 0;                   //重置接收计数器
	


	//进入透传模式
	UART3_SendString("AT+CIPSEND\r\n"); 

//	vTaskDelay(10000);
	cnt = 4000;
	while(cnt--)
	{
		vTaskDelay(2);

		if(strstr((char *)Rx3Buffer,">"))
		{
			break;
		}
	}	

	
	UART1_SendString((char *)Rx3Buffer);  //进行测试
	memset(Rx3Buffer,0,BUFFERSIZE); //清空接收缓冲区
	Rx3Index = 0;                   //重置接收计数器
	
	
	WifiInitOK_Flag = 1; //WIFI模块初始化成功
	
	Rtc_Init();
 
//	//订阅主题
//	UART3_SendString("cmd=7&uid=b53e48fd98cb4e1a890021a95e170a51&type=1\r\n"); 
//	uint16_t cnt = 8000;
//	while(cnt--)
//	{
//		vTaskDelay(2);
//		//UART1_SendString((char *)Rx3Buffer);  //进行测试
//		if(strstr((char *)Rx3Buffer,"\r\n"))
//		{
// 
//			break;
//		}
//	}
//	
////	vTaskDelay(4000);
//	Select_Number((char *)Rx3Buffer);
//	
	
}
	lv_label_set_text(ui_Label6,"Friday");	
	printf("=============\r\n");


	char str[4];
	while (1)
    {
        // 发送命令获取时间
        UART3_SendString("cmd=7&uid=b53e48fd98cb4e1a890021a95e170a51&type=1\r\n");

        // 等待服务器响应
        delay_ms(1000);

        // 解析服务器返回的时间信息并设置 RTC
        ParseAndSetRTC((char *)Rx3Buffer);

        // 清空接收缓冲区
        memset(Rx3Buffer, 0, BUFFERSIZE);
        Rx3Index = 0;

        // 判断是否唤醒
        if (uwTimeDisplay)
        {
            uwTimeDisplay = 0; // 重置该标志

            // 获取 RTC 日期和时间并显示到 PC 端
            RTC_GetDate(RTC_Format_BIN, &RTC_DateStructure);
            RTC_GetTime(RTC_Format_BIN, &RTC_TimeStructure);

            printf("%0.2d-%0.2d-%0.2d %0.2d:%0.2d:%0.2d\r\n",
                   RTC_DateStructure.RTC_Year + 2000, // 年份
                   RTC_DateStructure.RTC_Month,      // 月份
                   RTC_DateStructure.RTC_Date,       // 日期
                   RTC_TimeStructure.RTC_Hours,      // 小时
                   RTC_TimeStructure.RTC_Minutes,    // 分钟
                   RTC_TimeStructure.RTC_Seconds);   // 秒钟
			
				snprintf(str, sizeof(str), "%d", RTC_DateStructure.RTC_Date);
				lv_label_set_text(ui_Label5,str);
				snprintf(str, sizeof(str), "%d", RTC_TimeStructure.RTC_Hours);
				lv_label_set_text(ui_Label1,str);
				snprintf(str, sizeof(str), "%d", RTC_TimeStructure.RTC_Minutes);
				lv_label_set_text(ui_Label4,str);
			
				//心率血氧检测
				vHeart_Handle();
        }
    }
	
	
//	while(1)
//	{
//		//打印每一秒的时间
//		Rtc_read();
//		
//		//心率血氧检测
//		vHeart_Handle();
//		
//		//获取天气
//		//vGetWeather_Handle();
//		
//		vTaskDelay(100);
//	}
}




//LVGL初始化
void vLVGL_Handle( void * pvParameters )
{
	printf("create LVGL\r\n");
	//lvgl init
    lv_init();
   
	//lvgl_disp init
    lv_port_disp_init();

    //lvgl_inut_dev init
    lv_port_indev_init();

    //lvgl_ui init
    ui_init();	
	
//	vTaskDelay(100);
	for (;;)
	{
		//互斥锁上锁
//		xSemaphoreTake( xSemaphore,portMAX_DELAY );
		
        lv_task_handler();	
        vTaskDelay(1);
		
		//互斥锁解锁 
//		xSemaphoreGive(xSemaphore);

    }
}



//开始任务调度
void vStart_Handle (void *pvParameters)
{ 
	/* 设置系统中断优先级分组4 */
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);
	
	UART2_Config(9600);
	UART1_Config(115200);	  //初始化UART1  和PC端通信
	UART3_Config(115200);     //初始化UART3  和WIFI通信		
	
	
	//蓝牙初始化
	{

		UART2_SendString("AT\r\n"); //测试指令  响应"OK\r\n"

	
		UART2_SendString("AT+NAME=LuoSanPao\r\n"); //修改名称

		
		UART2_SendString("AT+LADDR\r\n"); //MAC地址

	}

//		vGetWeather_Handle();
	
	 /* 创建LVGL_UI初始化的任务 */
    xTaskCreate((TaskFunction_t)vLVGL_Handle,           /* 任务入口函数 */
                (const char *)"app_task_lvgl",          /* 任务名字 */
                (uint16_t)512,                          /* 任务栈大小 */
                (void *)NULL,                           /* 任务入口函数参数 */
                (UBaseType_t)6,                         /* 任务的优先级 */
                (TaskHandle_t *)&xLVGL_Handle); 		/* 任务控制块指针 */
				
	
	/* 创建WIFI初始化的任务 */
	xTaskCreate((TaskFunction_t)vWIFI_Handle,          	/* 任务入口函数 */
				(const char *)"app_task_wifi",          /* 任务名字 */
				(uint16_t)4096,                          /* 任务栈大小 */
				(void *)NULL,                           /* 任务入口函数参数 */
				(UBaseType_t)6,                         /* 任务的优先级 */
				(TaskHandle_t *)&xWIFI_Handle); 		/* 任务控制块指针 */	
				
				
	/* 创建获取角度的任务 */
	xTaskCreate((TaskFunction_t)vMPU_Handle,          	/* 任务入口函数 */
				(const char *)"app_task_angle",          /* 任务名字 */
				(uint16_t)512,                          /* 任务栈大小 */
				(void *)NULL,                           /* 任务入口函数参数 */
				(UBaseType_t)6,                         /* 任务的优先级 */
				(TaskHandle_t *)&xMPU_Handle); 		/* 任务控制块指针 */	
				
	/* 创建获取步数的任务 */
	xTaskCreate((TaskFunction_t)vWalk_Handle,          	/* 任务入口函数 */
				(const char *)"app_task_walk",          /* 任务名字 */
				(uint16_t)512,                          /* 任务栈大小 */
				(void *)NULL,                           /* 任务入口函数参数 */
				(UBaseType_t)6,                         /* 任务的优先级 */
				(TaskHandle_t *)&xWalk_Handle); 		/* 任务控制块指针 */	
				
	//创建运动计时的任务
	create_sport();	

	/* 创建无操作息屏的任务 */
	xTaskCreate((TaskFunction_t)vlcd_dark_Handle,          	/* 任务入口函数 */
				(const char *)"app_task_lcd_dark",          /* 任务名字 */
				(uint16_t)512,                          /* 任务栈大小 */
				(void *)NULL,                           /* 任务入口函数参数 */
				(UBaseType_t)6,                         /* 任务的优先级 */
				(TaskHandle_t *)&xlcd_dark); 		/* 任务控制块指针 */	

	/* 创建点击亮屏的任务 */
	xTaskCreate((TaskFunction_t)vlcd_light_Handle,          	/* 任务入口函数 */
				(const char *)"app_task_lcd_light",          /* 任务名字 */
				(uint16_t)512,                          /* 任务栈大小 */
				(void *)NULL,                           /* 任务入口函数参数 */
				(UBaseType_t)6,                         /* 任务的优先级 */
				(TaskHandle_t *)&xlcd_light); 		/* 任务控制块指针 */	

				
	printf("create ok \r\n");
				
	vTaskDelete( NULL );
}




int main(void)
{
	 
	BaseType_t xReturned;				//用于记录返回值
	

    //1.创建任务
    xReturned = xTaskCreate(
								vStart_Handle ,       	//函数指针，指向入口函数
								"task for start" ,  //任务名称，是字符串常量
								1024,      			//任务堆栈，是以字为单位
								NULL,    			//任务参数，不传可以NULL
								9,					//优先级别，不要超过范围
								&xStart_Handle 		//任务句柄，可以控制任务
						   );    
	
	//2.错误处理
    if( xReturned == pdPASS )
    {
       
    }
	//3.启动调度器，调度器启动之后才可以对任务进行调度，写在该函数后面的代码就不会被执行
	vTaskStartScheduler();

	while(1)
	{
		
	}
}


void vApplicationTickHook(void)
{
    lv_tick_inc(1);
}

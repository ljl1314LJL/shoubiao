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

TaskHandle_t xStart_Handle = NULL;	     //���ڸ���������
TaskHandle_t xLVGL_Handle = NULL;		 //���ڸ���������
 
TaskHandle_t xWIFI_Handle = NULL;			 //���ڸ���������
TaskHandle_t xMPU_Handle = NULL;			 //���ڸ���������
TaskHandle_t xWalk_Handle = NULL;			 //���ڸ���������
TaskHandle_t xCount_Handle = NULL;			 //���ڸ���������
TaskHandle_t xlcd_light = NULL;			 //���ڸ���������
TaskHandle_t xlcd_dark = NULL;			 //���ڸ���������
//TaskHandle_t xHeart_Handle = NULL;			 //���ڸ���������
//TaskHandle_t xClock_Handle = NULL;			 //���ڸ���������
//TaskHandle_t xBT_Handle = NULL;			 //���ڸ���������

__IO uint32_t uwTimeDisplay = 0; // ʱ����ʾ��־
SemaphoreHandle_t xSemaphore;		//���ڼ�¼���������
uint32_t cnt = 4000;
int32_t n_heart_rate;   //heart rate value


uint32_t g_system_no_opreation_cnt = 0;				//Ϣ������
uint32_t g_system_no_opreation_max = 10;			//Ϣ����ʱ��
uint32_t g_system_no_opreation_flag = 1;			//�Ƿ���Ϣ��
uint8_t TaiWan_flag = 0; 							//̧�������Ƿ�����־

extern lv_obj_t * ui_Image18;
extern lv_obj_t * ui_Image10;
extern lv_obj_t * ui_Image23;
extern lv_obj_t * ui_Image24;

//lcd��ʾ��/�ر�
void lcd_display_on(uint8_t on)
{
	if(on)
		LCD_BLK_1;	 	// ����
	else
		LCD_BLK_0;	 	// ����
 
}

//��������������
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

//����û�в���ʱϢ������
void vlcd_dark_Handle(void * pvParameters )
{
	
	//TickType_t xLastWakeTime;
		
	while(1){
		//xLastWakeTime = xTaskGetTickCount();
		//����������
		//xSemaphoreTake(xSemaphore,portMAX_DELAY );
		//if(g_system_no_opreation_flag != 0){
			g_system_no_opreation_cnt++;
			//printf("########################%d#######################\r\n",g_system_no_opreation_cnt);
			if(g_system_no_opreation_cnt >= g_system_no_opreation_max){
				g_system_no_opreation_cnt = 0;
				lcd_display_on(0);			//Ϣ��
			}
		//}

		//���������� 
	//	xSemaphoreGive(xSemaphore);
		//������ʱ����
		//vTaskDelayUntil(&xLastWakeTime, 1000);
		vTaskDelay(1000);
	}

}


void extract_weather_info(const char *json, char *code, char *weather, char *temperature) {
    const char *start, *end;

    // ��ȡ����
    start = strstr(json, "\"code\":\"");
    if (start) {
        start += 8; // ����ǰ׺
        end = strchr(start, '"');
        if (end) {
            strncpy(code, start, end - start);
            code[end - start] = '\0'; // ȷ���ַ�����null��β
        }
    }

    // ��ȡ����
    start = strstr(json, "\"text\":\"");
    if (start) {
        start += 8; // ����ǰ׺
        end = strchr(start, '"');
        if (end) {
            strncpy(weather, start, end - start);
            weather[end - start] = '\0'; // ȷ���ַ�����null��β
        }
    }

    // ��ȡ�¶�
    start = strstr(json, "\"temperature\":\"");
    if (start) {
        start += 15; // ����ǰ׺
        end = strchr(start, '"');
        if (end) {
            strncpy(temperature, start, end - start);
            temperature[end - start] = '\0'; // ȷ���ַ�����null��β
        }
    }
}

void RTC_WKUP_IRQHandler(void)
{
    if (RTC_GetITStatus(RTC_IT_WUT) != RESET)
    {
        uwTimeDisplay = 1;

        // ����жϱ�־
        RTC_ClearITPendingBit(RTC_IT_WUT);
        EXTI_ClearITPendingBit(EXTI_Line22);
    }
}

void Select_Number(char * str)
{
 
    char date[9], time[9];
    int year, month, day, hour, minute, second;

    // ��ȡ���ڲ���
    sscanf(str, "%d-%d-%d", &year, &month, &day);
    sprintf(date, "%04d%02d%02d", year, month, day);

    // ��ȡʱ�䲿��
    sscanf(str + 11, "%d:%d:%d", &hour, &minute, &second);
    sprintf(time, "%02d%02d%02d", hour, minute, second);

//    printf("��ȡ����������Ϊ: %s\n", date);
//    printf("��ȡ��ʱ������Ϊ: %s\n", time);
	
	Rtc_Init(hour,minute,second,month,day,3);


}


//�˶���ʱ
void vCount_Handle(void * pvParameters )
{
	
	//�������ȹ���
	vTaskSuspend(NULL);
	
	
	uint8_t seconds = 0;  // �������
	uint8_t minutes = 0;  // ���Ӽ�����
	uint8_t hours = 0;    // Сʱ������
	//double heart;	      //����
	double hot = 0;

	char sec_text[3];	
	char min_text[3];
	char hour_text[3];	
	char h_r[4];
	
	while (1) 
	{
		//heart = (n_heart_rate/100);
	
	// ÿ�����һ��
	vTaskDelay(1000);  // ��ʱ1000ms

	// ��������
	seconds++;

	snprintf(sec_text, sizeof(sec_text), "%d", seconds);
	lv_label_set_text(ui_Label44, sec_text);
	//printf("0000000000000000    ====     %s\r\n",sec_text);
	// ÿ60�����һ�η�����
		if (seconds > 59) 
		{
			minutes++;

			snprintf(min_text, sizeof(min_text), "%d", minutes);
			lv_label_set_text(ui_Label42, min_text);

			// ��������
			seconds = 0;

			// ÿ60���Ӹ���һ��Сʱ��
			if (minutes > 59) 
			{
				hours++;

				snprintf(hour_text, sizeof(hour_text), "%d", hours);
				lv_label_set_text(ui_Label40, hour_text);

				// ���÷�����
				minutes = 0;
			}
		}
		hot += 0.015;
		snprintf(h_r, sizeof(h_r), "%f", hot);
		lv_label_set_text(ui_Label46, h_r);

		
		 

	}
}

//�������˶�������Ϊһ��������ÿ���˶�һ�ξ͵���һ��
void create_sport()
{
	/* �����˶���ʱ������ */
	xTaskCreate((TaskFunction_t)vCount_Handle,          	/* ������ں��� */
				(const char *)"app_task_count",          /* �������� */
				(uint16_t)512,                          /* ����ջ��С */
				(void *)NULL,                           /* ������ں������� */
				(UBaseType_t)6,                         /* ��������ȼ� */
				(TaskHandle_t *)&xCount_Handle); 		/* ������ƿ�ָ�� */	
}

//̧����ʾ
void vMPU_Handle(void * pvParameters )
{
	uint8_t res;
	
	float pitch,roll,yaw; 		//ŷ����
	short aacx,aacy,aacz;		//���ٶȴ�����ԭʼ����
	short gyrox,gyroy,gyroz;	//������ԭʼ����
//	short temp;					//�¶�		
	while(1)
	{
//		res=mpu_dmp_get_data(&pitch,&roll,&yaw);
//		if(res==0)
//		{ 
		///printf("-----------------------------------000000000000000000\r\n");
//			temp=MPU_Get_Temperature();	//�õ��¶�ֵ
			MPU_Get_Accelerometer(&aacx,&aacy,&aacz);	//�õ����ٶȴ���������
			MPU_Get_Gyroscope(&gyrox,&gyroy,&gyroz);	//�õ�����������
	//	printf("���ٶȣ�x=%d y=%d z=%d",aacx,aacy,aacz);
		//	printf("�����ǣ�x=%d y=%d z=%d\r\n",gyrox,gyroy,gyroz);
			if( aacy > -12000 && aacy < -8000)
			{
				printf("show lcd on up\r\n");
				lcd_display_on(1);
				g_system_no_opreation_cnt = 0;
			
			}vTaskDelay(100);
//		}	

	}
}

//��ȡ����
void vWalk_Handle(void * pvParameters )
{
	printf("create vWalk_Handle\r\n");
	unsigned long  step_count_last=0;
	unsigned long  step_count=0;
	//uint32_t sedentary_event=0;
//	uint32_t t=0;

	//���������
	vTaskSuspendAll( );
	//��ʼ��MPU6050	
	MPU_Init();

	while(mpu_dmp_init())
	{
	//	printf("[ERROR] MPU6050 ERROR \r\n");
		vTaskDelay(500);
		MPU_Init();
	}


	/* ���ò�����ֵΪ0*/
	while(dmp_set_pedometer_step_count(0))
	{
		vTaskDelay(500);
	}
	
	//�ָ�������
	xTaskResumeAll( );	
	char str[3];
	while(1)
	{
		/* ��ȡ���� */
		dmp_get_pedometer_step_count(&step_count);
		if(step_count_last != step_count){
				//step_all += step_count;
				//set_main_walk(step_count);
			step_count_last = step_count;
		}

		
	//	printf("[INFO] ��ǰ����:%ld\r\n",step_count);
		snprintf(str,sizeof(str),"%ld",step_count);
		lv_label_set_text(ui_Label8,str);
		step_count = 0;
		/* ��ʱһ�� */
		vTaskDelay(1000);
	}
}

//��ȡ����
void vGetWeather_Handle(void)
{
	
	printf("create vGetWeather_Handle OK\r\n");
	
	//�˳�͸��
	UART3_SendString("+++"); 
	 
	vTaskDelay(50);
	UART3_SendString("+++"); 
	vTaskDelay(50);
	
	//����ָ��  ��Ӧ"OK\r\n"
	UART3_SendString("AT\r\n"); 
	vTaskDelay(100);
	
	UART1_SendString((char *)Rx3Buffer);  //���в���
	
	memset(Rx3Buffer,0,BUFFERSIZE); //��ս��ջ�����
	Rx3Index = 0;                   //���ý��ռ�����
	
	//����ģʽ  3��STA+AP ��Ӧ"OK\r\n"
	UART3_SendString("AT+CWMODE=3\r\n"); 
	vTaskDelay(1000);
	
	UART1_SendString((char *)Rx3Buffer);  //���в���
	memset(Rx3Buffer,0,BUFFERSIZE); //��ս��ջ�����
	Rx3Index = 0;                   //���ý��ռ�����
	
	//����AP  ��Ӧ"OK\r\n"
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
	
	UART1_SendString((char *)Rx3Buffer);  //���в���
	memset(Rx3Buffer,0,BUFFERSIZE); //��ս��ջ�����
	Rx3Index = 0;                   //���ý��ռ�����
	
	
	//ʹ��͸��ģʽ
	UART3_SendString("AT+CIPMODE=1\r\n"); 
	vTaskDelay(4000);
	
	UART1_SendString((char *)Rx3Buffer);  //���в���
	memset(Rx3Buffer,0,BUFFERSIZE); //��ս��ջ�����
	Rx3Index = 0;                   //���ý��ռ�����
	
//��ȡ����

	//���ӷ����� 
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
		
	UART1_SendString((char *)Rx3Buffer);  //���в���
	memset(Rx3Buffer,0,BUFFERSIZE); //��ս��ջ�����
	Rx3Index = 0;                   //���ý��ռ�����
	


	//����͸��ģʽ
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

	
	UART1_SendString((char *)Rx3Buffer);  //���в���
	memset(Rx3Buffer,0,BUFFERSIZE); //��ս��ջ�����
	Rx3Index = 0;                   //���ý��ռ�����
	
	//	WifiInitOK_Flag = 1; //WIFIģ���ʼ���ɹ�


	printf("*********************\r\n");
	//��������
	UART3_SendString("GET https://api.seniverse.com/v3/weather/now.json?key=S5jYTIuliBSEQmhkX&location=guangzhou&unit=c\r\n"); 
	cnt = 1000;
	while(cnt--)
	{
		vTaskDelay(2);
		//UART1_SendString((char *)Rx3Buffer);  //���в���
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
		//����
		lv_label_set_text(ui_Label18,"��");
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
		//��
		lv_label_set_text(ui_Label22,tmp);
		lv_label_set_text(ui_Label18,"��");
	}
	
	else if(atoi(code) == 13)
	{
		//С��
		lv_label_set_text(ui_Label18,"С��");
		lv_label_set_text(ui_Label22,tmp);
		lv_obj_add_flag(ui_Image18,LV_OBJ_FLAG_HIDDEN);
		
		lv_obj_clear_flag(ui_Image10,LV_OBJ_FLAG_HIDDEN);
		
	}
	
	else if(atoi(code) == 15)
	{
		//����
		lv_label_set_text(ui_Label18,"����");
		lv_label_set_text(ui_Label22,tmp);
		lv_obj_add_flag(ui_Image18,LV_OBJ_FLAG_HIDDEN);
		
		lv_obj_clear_flag(ui_Image23,LV_OBJ_FLAG_HIDDEN);
		
	}
	


}







//��ȡ����Ѫ��
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

	
	//ʹ�ܶ˿�F��Ӳ��ʱ�ӣ��˿�F���ܹ�����˵���˾��ǶԶ˿�F�ϵ�
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF, ENABLE);	

	//����Ӳ��������GPIO���˿�F����9������
	GPIO_InitStruct.GPIO_Pin=GPIO_Pin_9;			//��9 ������
	GPIO_InitStruct.GPIO_Mode=GPIO_Mode_OUT;		//���ģʽ
	GPIO_InitStruct.GPIO_Speed=GPIO_High_Speed;		//���Ÿ��ٹ������յ�ָ������������ȱ�㣺���ĸ�
	GPIO_InitStruct.GPIO_OType=GPIO_OType_PP;		//�����������������
	GPIO_InitStruct.GPIO_PuPd=GPIO_PuPd_NOPULL;		//����Ҫ����������
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
		
		   ��ǰ100������ת�����洢���У��������400�������Ƶ�����
		*/
		
        for(i=100;i<500;i++)
        {
            aun_red_buffer[i-100]=aun_red_buffer[i];
            aun_ir_buffer[i-100]=aun_ir_buffer[i];
            
            /* update the signal min and max 
			   �����ź���Сֵ�����ֵ
			*/
			
            if(un_min>aun_red_buffer[i])
				un_min=aun_red_buffer[i];
			
            if(un_max<aun_red_buffer[i])
				un_max=aun_red_buffer[i];
        }
		
		/* take 100 sets of samples before calculating the heart rate 
		
		   �ڼ�������֮ǰ�ɼ�100������
		*/
		
        for(i=400;i<500;i++)
        {
            un_prev_data=aun_red_buffer[i-1];
			
            while(MAX30102_INT==1);
			
            max30102_FIFO_ReadBytes(REG_FIFO_DATA,temp);
			
			/* ���ֵ�Ի��ʵ������ */
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

		/* �������ʺ�Ѫ�����Ͷ� */
        maxim_heart_rate_and_oxygen_saturation(aun_ir_buffer, n_ir_buffer_length, aun_red_buffer, &n_sp02, &ch_spo2_valid, &n_heart_rate, &ch_hr_valid);
			
		
		char str[4];

		
		/* ͨ��UART�������ͼ��������͵��ն˳��� */
		if((ch_hr_valid == 1)&& (n_heart_rate>=60) && (n_heart_rate<=120))
		{
			//��������
			snprintf(str, sizeof(str), "%d", n_heart_rate);
			lv_label_set_text(ui_Label7,str);
			lv_label_set_text(ui_Label38,str);
			lv_label_set_text(ui_Label19,str);
	 
			
			printf("����=%d\r\n", n_heart_rate); 
		}

		
		if((ch_spo2_valid ==1)&& (n_sp02>=95) && (n_sp02<100))
		{
			//����Ѫ��
			snprintf(str, sizeof(str), "%d", n_sp02);
			lv_label_set_text(ui_Label26,str);

			
			printf("Ѫ��Ũ��=%d\r\n", n_sp02); 
		}	
	
		//delay_ms(100);
		//vTaskDelay(100);
		
	}

	printf("test heart and algorithm over\r\n");
			//	vTaskDelete( NULL );
		
	
}









//��ȡʱ��
void vWIFI_Handle( void * pvParameters )
{	
	vGetWeather_Handle();

	
	

	//vGetWeather_Handle();
	printf("create WIFI\r\n");
//	uint32_t cnt = 8000;
	
	//�˳�͸��
	UART3_SendString("+++"); 
	 
	vTaskDelay(50);
	UART3_SendString("+++"); 
	vTaskDelay(50);
	
	//����ָ��  ��Ӧ"OK\r\n"
	UART3_SendString("AT\r\n"); 
	vTaskDelay(100);
	
	UART1_SendString((char *)Rx3Buffer);  //���в���
	
	memset(Rx3Buffer,0,BUFFERSIZE); //��ս��ջ�����
	Rx3Index = 0;                   //���ý��ռ�����
	
	//����ģʽ  3��STA+AP ��Ӧ"OK\r\n"
	UART3_SendString("AT+CWMODE=3\r\n"); 
	vTaskDelay(1000);
	
	UART1_SendString((char *)Rx3Buffer);  //���в���
	memset(Rx3Buffer,0,BUFFERSIZE); //��ս��ջ�����
	Rx3Index = 0;                   //���ý��ռ�����
	
	//����AP  ��Ӧ"OK\r\n"
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
	
	UART1_SendString((char *)Rx3Buffer);  //���в���
	memset(Rx3Buffer,0,BUFFERSIZE); //��ս��ջ�����
	Rx3Index = 0;                   //���ý��ռ�����
	
	
	//ʹ��͸��ģʽ
	UART3_SendString("AT+CIPMODE=1\r\n"); 
	vTaskDelay(4000);
	
	UART1_SendString((char *)Rx3Buffer);  //���в���
	memset(Rx3Buffer,0,BUFFERSIZE); //��ս��ջ�����
	Rx3Index = 0;                   //���ý��ռ�����
	
	//�Ͽ�����
	UART3_SendString("AT+CIPCLOSE\r\n"); 
	vTaskDelay(4000);
	
	UART1_SendString((char *)Rx3Buffer);  //���в���
	memset(Rx3Buffer,0,BUFFERSIZE); //��ս��ջ�����
	Rx3Index = 0;                   //���ý��ռ�����
//��ȡʱ��	
{
	//���ӷ����� 
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
		
	UART1_SendString((char *)Rx3Buffer);  //���в���
	memset(Rx3Buffer,0,BUFFERSIZE); //��ս��ջ�����
	Rx3Index = 0;                   //���ý��ռ�����
	


	//����͸��ģʽ
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

	
	UART1_SendString((char *)Rx3Buffer);  //���в���
	memset(Rx3Buffer,0,BUFFERSIZE); //��ս��ջ�����
	Rx3Index = 0;                   //���ý��ռ�����
	
	
	WifiInitOK_Flag = 1; //WIFIģ���ʼ���ɹ�
	
	Rtc_Init();
 
//	//��������
//	UART3_SendString("cmd=7&uid=b53e48fd98cb4e1a890021a95e170a51&type=1\r\n"); 
//	uint16_t cnt = 8000;
//	while(cnt--)
//	{
//		vTaskDelay(2);
//		//UART1_SendString((char *)Rx3Buffer);  //���в���
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
        // ���������ȡʱ��
        UART3_SendString("cmd=7&uid=b53e48fd98cb4e1a890021a95e170a51&type=1\r\n");

        // �ȴ���������Ӧ
        delay_ms(1000);

        // �������������ص�ʱ����Ϣ������ RTC
        ParseAndSetRTC((char *)Rx3Buffer);

        // ��ս��ջ�����
        memset(Rx3Buffer, 0, BUFFERSIZE);
        Rx3Index = 0;

        // �ж��Ƿ���
        if (uwTimeDisplay)
        {
            uwTimeDisplay = 0; // ���øñ�־

            // ��ȡ RTC ���ں�ʱ�䲢��ʾ�� PC ��
            RTC_GetDate(RTC_Format_BIN, &RTC_DateStructure);
            RTC_GetTime(RTC_Format_BIN, &RTC_TimeStructure);

            printf("%0.2d-%0.2d-%0.2d %0.2d:%0.2d:%0.2d\r\n",
                   RTC_DateStructure.RTC_Year + 2000, // ���
                   RTC_DateStructure.RTC_Month,      // �·�
                   RTC_DateStructure.RTC_Date,       // ����
                   RTC_TimeStructure.RTC_Hours,      // Сʱ
                   RTC_TimeStructure.RTC_Minutes,    // ����
                   RTC_TimeStructure.RTC_Seconds);   // ����
			
				snprintf(str, sizeof(str), "%d", RTC_DateStructure.RTC_Date);
				lv_label_set_text(ui_Label5,str);
				snprintf(str, sizeof(str), "%d", RTC_TimeStructure.RTC_Hours);
				lv_label_set_text(ui_Label1,str);
				snprintf(str, sizeof(str), "%d", RTC_TimeStructure.RTC_Minutes);
				lv_label_set_text(ui_Label4,str);
			
				//����Ѫ�����
				vHeart_Handle();
        }
    }
	
	
//	while(1)
//	{
//		//��ӡÿһ���ʱ��
//		Rtc_read();
//		
//		//����Ѫ�����
//		vHeart_Handle();
//		
//		//��ȡ����
//		//vGetWeather_Handle();
//		
//		vTaskDelay(100);
//	}
}




//LVGL��ʼ��
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
		//����������
//		xSemaphoreTake( xSemaphore,portMAX_DELAY );
		
        lv_task_handler();	
        vTaskDelay(1);
		
		//���������� 
//		xSemaphoreGive(xSemaphore);

    }
}



//��ʼ�������
void vStart_Handle (void *pvParameters)
{ 
	/* ����ϵͳ�ж����ȼ�����4 */
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);
	
	UART2_Config(9600);
	UART1_Config(115200);	  //��ʼ��UART1  ��PC��ͨ��
	UART3_Config(115200);     //��ʼ��UART3  ��WIFIͨ��		
	
	
	//������ʼ��
	{

		UART2_SendString("AT\r\n"); //����ָ��  ��Ӧ"OK\r\n"

	
		UART2_SendString("AT+NAME=LuoSanPao\r\n"); //�޸�����

		
		UART2_SendString("AT+LADDR\r\n"); //MAC��ַ

	}

//		vGetWeather_Handle();
	
	 /* ����LVGL_UI��ʼ�������� */
    xTaskCreate((TaskFunction_t)vLVGL_Handle,           /* ������ں��� */
                (const char *)"app_task_lvgl",          /* �������� */
                (uint16_t)512,                          /* ����ջ��С */
                (void *)NULL,                           /* ������ں������� */
                (UBaseType_t)6,                         /* ��������ȼ� */
                (TaskHandle_t *)&xLVGL_Handle); 		/* ������ƿ�ָ�� */
				
	
	/* ����WIFI��ʼ�������� */
	xTaskCreate((TaskFunction_t)vWIFI_Handle,          	/* ������ں��� */
				(const char *)"app_task_wifi",          /* �������� */
				(uint16_t)4096,                          /* ����ջ��С */
				(void *)NULL,                           /* ������ں������� */
				(UBaseType_t)6,                         /* ��������ȼ� */
				(TaskHandle_t *)&xWIFI_Handle); 		/* ������ƿ�ָ�� */	
				
				
	/* ������ȡ�Ƕȵ����� */
	xTaskCreate((TaskFunction_t)vMPU_Handle,          	/* ������ں��� */
				(const char *)"app_task_angle",          /* �������� */
				(uint16_t)512,                          /* ����ջ��С */
				(void *)NULL,                           /* ������ں������� */
				(UBaseType_t)6,                         /* ��������ȼ� */
				(TaskHandle_t *)&xMPU_Handle); 		/* ������ƿ�ָ�� */	
				
	/* ������ȡ���������� */
	xTaskCreate((TaskFunction_t)vWalk_Handle,          	/* ������ں��� */
				(const char *)"app_task_walk",          /* �������� */
				(uint16_t)512,                          /* ����ջ��С */
				(void *)NULL,                           /* ������ں������� */
				(UBaseType_t)6,                         /* ��������ȼ� */
				(TaskHandle_t *)&xWalk_Handle); 		/* ������ƿ�ָ�� */	
				
	//�����˶���ʱ������
	create_sport();	

	/* �����޲���Ϣ�������� */
	xTaskCreate((TaskFunction_t)vlcd_dark_Handle,          	/* ������ں��� */
				(const char *)"app_task_lcd_dark",          /* �������� */
				(uint16_t)512,                          /* ����ջ��С */
				(void *)NULL,                           /* ������ں������� */
				(UBaseType_t)6,                         /* ��������ȼ� */
				(TaskHandle_t *)&xlcd_dark); 		/* ������ƿ�ָ�� */	

	/* ����������������� */
	xTaskCreate((TaskFunction_t)vlcd_light_Handle,          	/* ������ں��� */
				(const char *)"app_task_lcd_light",          /* �������� */
				(uint16_t)512,                          /* ����ջ��С */
				(void *)NULL,                           /* ������ں������� */
				(UBaseType_t)6,                         /* ��������ȼ� */
				(TaskHandle_t *)&xlcd_light); 		/* ������ƿ�ָ�� */	

				
	printf("create ok \r\n");
				
	vTaskDelete( NULL );
}




int main(void)
{
	 
	BaseType_t xReturned;				//���ڼ�¼����ֵ
	

    //1.��������
    xReturned = xTaskCreate(
								vStart_Handle ,       	//����ָ�룬ָ����ں���
								"task for start" ,  //�������ƣ����ַ�������
								1024,      			//�����ջ��������Ϊ��λ
								NULL,    			//�����������������NULL
								9,					//���ȼ��𣬲�Ҫ������Χ
								&xStart_Handle 		//�����������Կ�������
						   );    
	
	//2.������
    if( xReturned == pdPASS )
    {
       
    }
	//3.����������������������֮��ſ��Զ�������е��ȣ�д�ڸú�������Ĵ���Ͳ��ᱻִ��
	vTaskStartScheduler();

	while(1)
	{
		
	}
}


void vApplicationTickHook(void)
{
    lv_tick_inc(1);
}

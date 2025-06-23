#ifndef __INCLUDES_H__
#define __INCLUDES_H__

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include "stm32f4xx.h"
#include "stm32f4xx_it.h"


#include "sys.h"
#include "uart.h"
#include "delay.h"
#include "tft.h"
#include "bmp.h"
#include "touch.h" 



#include "lvgl.h"
#include "lv_port_disp.h"
#include "lv_port_indev.h"
#include "tim.h"
#include "bmp.h"
#include "lcd_font.h"
#include "FreeRTOS.h" 
#include "task.h" 
#include "queue.h"
#include "ui.h"
#include "semphr.h"
#include "event_groups.h"
#include "DHT11.h"
 

extern GPIO_InitTypeDef      GPIO_InitStructure;
extern NVIC_InitTypeDef     NVIC_InitStructure;        
extern SPI_InitTypeDef      SPI_InitStructure;



#endif

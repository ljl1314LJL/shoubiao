#include "stm32f4xx.h"
#include "max30102.h"
#include "delay.h"
#include "sys.h"

static GPIO_InitTypeDef GPIO_InitStructure;

#define SCL_W	PGout(9)
#define SDA_W	PAout(6)
#define SDA_R	PAin(6)

void i2c_init(void)
{

	//使能端口B的硬件时钟
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOG, ENABLE);


	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_6;				//第 6个引脚
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_OUT;			//输出模式
	GPIO_InitStructure.GPIO_Speed=GPIO_High_Speed;		//引脚高速工作，收到指令立即工作；缺点：功耗高
	GPIO_InitStructure.GPIO_OType=GPIO_OType_OD;		//开漏
	GPIO_InitStructure.GPIO_PuPd=GPIO_PuPd_NOPULL;		//不需要上下拉电阻
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_9;				//第 6个引脚
	GPIO_Init(GPIOG,&GPIO_InitStructure);
	
	//只要是输出模式，肯定会有初始电平状态，看时序图，空闲状态为高电平
	SCL_W=1;
	SDA_W=1;

}

void sda_pin_mode(GPIOMode_TypeDef pin_mode)
{
	//配置硬件，配置GPIO，端口F，第9个引脚
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_6;			//第9 个引脚
	GPIO_InitStructure.GPIO_Mode=pin_mode;				//输出模式
	GPIO_InitStructure.GPIO_Speed=GPIO_High_Speed;		//引脚高速工作，收到指令立即工作；缺点：功耗高
	GPIO_InitStructure.GPIO_OType=GPIO_OType_OD;		//开漏
	GPIO_InitStructure.GPIO_PuPd=GPIO_PuPd_NOPULL;		//不需要上下拉电阻
	GPIO_Init(GPIOA,&GPIO_InitStructure);


}


void i2c_start(void)
{	
	//保证SDA引脚为输出模式
	sda_pin_mode(GPIO_Mode_OUT);
	
	SDA_W=1;
	SCL_W=1;
	delay_us(1);
	
	SDA_W=0;
	delay_us(1);
	
	SCL_W=0;//总线进入忙状态
	delay_us(1);
}


void i2c_stop(void)
{
	//保证SDA引脚为输出模式
	sda_pin_mode(GPIO_Mode_OUT);
	
	SDA_W=0;
	SCL_W=1;
	delay_us(1);
	
	SDA_W=1;
	delay_us(1);

}


void i2c_send_byte(uint8_t byte)
{
	int32_t i=0;
	
	//保证SDA引脚为输出模式
	sda_pin_mode(GPIO_Mode_OUT);

	SDA_W=0;
	SCL_W=0;
	delay_us(1);
	
	//最高有效位优先传输，通过时序图观察到
	for(i=7; i>=0; i--)
	{
		//检测对应的bit位是1还是0
		if(byte & (1<<i))
			SDA_W=1;
		else
			SDA_W=0;
	
		delay_us(1);
	
		//时钟线拉高，数据有效
		SCL_W=1;
		delay_us(1);

	
		//时钟线拉低，数据变更
		SCL_W=0;
		delay_us(1);		
	
	}
}


uint8_t i2c_recv_byte(void)
{
	uint8_t d=0;
	int32_t i;
	
	//保证SDA引脚为输入模式
	sda_pin_mode(GPIO_Mode_IN);

	for(i=7; i>=0; i--)
	{
		//时钟线拉高，数据有效
		SCL_W=1;
		delay_us(1);
		
		//读取SDA引脚电平
		if(SDA_R)
			d|=1<<i;


		//时钟线拉低，保持占用总线，总线是忙状态
		SCL_W=0;
		delay_us(1);	
	
	}

	return d;
}


void i2c_ack(uint8_t ack)
{

	//保证SDA引脚为输出模式
	sda_pin_mode(GPIO_Mode_OUT);

	SDA_W=0;
	SCL_W=0;
	delay_us(1);
	

	if(ack)
		SDA_W=1;
	else
		SDA_W=0;

	delay_us(1);

	//时钟线拉高，数据有效
	SCL_W=1;
	delay_us(1);


	//时钟线拉低，数据变更
	SCL_W=0;
	delay_us(1);		
}

uint8_t i2c_wait_ack(void)
{
	uint8_t ack=0;
	//保证SDA引脚为输入模式
	sda_pin_mode(GPIO_Mode_IN);

	//时钟线拉高，数据有效
	SCL_W=1;
	delay_us(1);
	
	//读取SDA引脚电平
	if(SDA_R)
		ack=1;//无应答
	else
		ack=0;//有应答

	//时钟线拉低，保持占用总线，总线是忙状态
	SCL_W=0;
	delay_us(1);


	return ack;
}

void IIC_WriteBytes(u8 WriteAddr,u8* data,u8 dataLength)
{		
	u8 i;	
    i2c_start();  

	i2c_send_byte(WriteAddr);	    //发送写命令
	i2c_wait_ack();
	
	for(i=0;i<dataLength;i++)
	{
		i2c_send_byte(data[i]);
		i2c_wait_ack();
	}				    	   
    i2c_stop();//产生一个停止条件 
	delay_ms(10);	 
}

void IIC_ReadBytes(u8 deviceAddr, u8 writeAddr,u8* data,u8 dataLength)
{		
	u8 i;	
    i2c_start();  

	i2c_send_byte(deviceAddr);	    //发送写命令
	i2c_wait_ack();
	i2c_send_byte(writeAddr);
	i2c_wait_ack();
	i2c_send_byte(deviceAddr|0X01);//进入接收模式			   
	i2c_wait_ack();
	
	for(i=0;i<dataLength-1;i++)
	{
		data[i] = i2c_recv_byte();
		i2c_ack(0);
	}		
	data[dataLength-1] = i2c_recv_byte();	
	i2c_ack(1);
    i2c_stop();//产生一个停止条件 
	delay_ms(10);	 
}

void IIC_Read_One_Byte(u8 daddr,u8 addr,u8* data)
{				  	  	    																 
    i2c_start();  
	
	i2c_send_byte(daddr);	   //发送写命令
	i2c_wait_ack();
	i2c_send_byte(addr);//发送地址
	i2c_wait_ack();		 
	i2c_start();  	 	   
	i2c_send_byte(daddr|0X01);//进入接收模式			   
	i2c_wait_ack();	 
    *data = i2c_recv_byte();
	i2c_ack(1);
    i2c_stop();//产生一个停止条件	    
}

void IIC_Write_One_Byte(u8 daddr,u8 addr,u8 data)
{				   	  	    																 
    i2c_start();  
	
	i2c_send_byte(daddr);	    //发送写命令
	i2c_wait_ack();
	i2c_send_byte(addr);//发送地址
	i2c_wait_ack();	   	 										  		   
	i2c_send_byte(data);     //发送字节							   
	i2c_wait_ack();  		    	   
    i2c_stop();//产生一个停止条件 
	delay_ms(10);	 
}


u8 max30102_Bus_Write(u8 Register_Address, u8 Word_Data)
{

	/* 采用串行EEPROM随即读取指令序列，连续读取若干字节 */

	/* 第1步：发起I2C总线启动信号 */
	i2c_start();

	/* 第2步：发起控制字节，高7bit是地址，bit0是读写控制位，0表示写，1表示读 */
	i2c_send_byte(max30102_WR_address | I2C_WR);	/* 此处是写指令 */

	/* 第3步：发送ACK */
	if (i2c_wait_ack() != 0)
	{
		goto cmd_fail;	/* EEPROM器件无应答 */
	}

	/* 第4步：发送字节地址 */
	i2c_send_byte(Register_Address);
	if (i2c_wait_ack() != 0)
	{
		goto cmd_fail;	/* EEPROM器件无应答 */
	}
	
	/* 第5步：开始写入数据 */
	i2c_send_byte(Word_Data);

	/* 第6步：发送ACK */
	if (i2c_wait_ack() != 0)
	{
		goto cmd_fail;	/* EEPROM器件无应答 */
	}

	/* 发送I2C总线停止信号 */
	i2c_stop();
	return 1;	/* 执行成功 */

cmd_fail: /* 命令执行失败后，切记发送停止信号，避免影响I2C总线上其他设备 */
	/* 发送I2C总线停止信号 */
	i2c_stop();
	return 0;
}



u8 max30102_Bus_Read(u8 Register_Address)
{
	u8  data;


	/* 第1步：发起I2C总线启动信号 */
	i2c_start();

	/* 第2步：发起控制字节，高7bit是地址，bit0是读写控制位，0表示写，1表示读 */
	i2c_send_byte(max30102_WR_address | I2C_WR);	/* 此处是写指令 */

	/* 第3步：发送ACK */
	if (i2c_wait_ack() != 0)
	{
		goto cmd_fail;	/* EEPROM器件无应答 */
	}

	/* 第4步：发送字节地址， */
	i2c_send_byte((uint8_t)Register_Address);
	if (i2c_wait_ack() != 0)
	{
		goto cmd_fail;	/* EEPROM器件无应答 */
	}
	

	/* 第6步：重新启动I2C总线。下面开始读取数据 */
	i2c_start();

	/* 第7步：发起控制字节，高7bit是地址，bit0是读写控制位，0表示写，1表示读 */
	i2c_send_byte(max30102_WR_address | I2C_RD);	/* 此处是读指令 */

	/* 第8步：发送ACK */
	if (i2c_wait_ack() != 0)
	{
		goto cmd_fail;	/* EEPROM器件无应答 */
	}

	/* 第9步：读取数据 */
	{
		data = i2c_recv_byte();	/* 读1个字节 */

		i2c_ack(1);	/* 最后1个字节读完后，CPU产生NACK信号(驱动SDA = 1) */
	}
	/* 发送I2C总线停止信号 */
	i2c_stop();
	return data;	/* 执行成功 返回data值 */

cmd_fail: /* 命令执行失败后，切记发送停止信号，避免影响I2C总线上其他设备 */
	/* 发送I2C总线停止信号 */
	i2c_stop();
	return 0;
}


void max30102_FIFO_ReadWords(u8 Register_Address,u16 Word_Data[][2],u8 count)
{
	u8 i=0;
	u8 no = count;
	u8 data1, data2;
	/* 第1步：发起I2C总线启动信号 */
	i2c_start();

	/* 第2步：发起控制字节，高7bit是地址，bit0是读写控制位，0表示写，1表示读 */
	i2c_send_byte(max30102_WR_address | I2C_WR);	/* 此处是写指令 */

	/* 第3步：发送ACK */
	if (i2c_wait_ack() != 0)
	{
		goto cmd_fail;	/* EEPROM器件无应答 */
	}

	/* 第4步：发送字节地址， */
	i2c_send_byte((uint8_t)Register_Address);
	if (i2c_wait_ack() != 0)
	{
		goto cmd_fail;	/* EEPROM器件无应答 */
	}
	

	/* 第6步：重新启动I2C总线。下面开始读取数据 */
	i2c_start();

	/* 第7步：发起控制字节，高7bit是地址，bit0是读写控制位，0表示写，1表示读 */
	i2c_send_byte(max30102_WR_address | I2C_RD);	/* 此处是读指令 */

	/* 第8步：发送ACK */
	if (i2c_wait_ack() != 0)
	{
		goto cmd_fail;	/* EEPROM器件无应答 */
	}

	/* 第9步：读取数据 */
	while (no)
	{
		data1 = i2c_recv_byte();	
		i2c_ack(0);
		data2 = i2c_recv_byte();
		i2c_ack(0);
		Word_Data[i][0] = (((u16)data1 << 8) | data2);  //

		
		data1 = i2c_recv_byte();	
		i2c_ack(0);
		data2 = i2c_recv_byte();
		if(1==no)
			i2c_ack(1);	/* 最后1个字节读完后，CPU产生NACK信号(驱动SDA = 1) */
		else
			i2c_ack(0);
		Word_Data[i][1] = (((u16)data1 << 8) | data2); 

		no--;	
		i++;
	}
	/* 发送I2C总线停止信号 */
	i2c_stop();

cmd_fail: /* 命令执行失败后，切记发送停止信号，避免影响I2C总线上其他设备 */
	/* 发送I2C总线停止信号 */
	i2c_stop();
}

void max30102_FIFO_ReadBytes(u8 Register_Address,u8* Data)
{	
	max30102_Bus_Read(REG_INTR_STATUS_1);
	max30102_Bus_Read(REG_INTR_STATUS_2);
	
	/* 第1步：发起I2C总线启动信号 */
	i2c_start();

	/* 第2步：发起控制字节，高7bit是地址，bit0是读写控制位，0表示写，1表示读 */
	i2c_send_byte(max30102_WR_address | I2C_WR);	/* 此处是写指令 */

	/* 第3步：发送ACK */
	if (i2c_wait_ack() != 0)
	{
		goto cmd_fail;	/* EEPROM器件无应答 */
	}

	/* 第4步：发送字节地址， */
	i2c_send_byte((uint8_t)Register_Address);
	if (i2c_wait_ack() != 0)
	{
		goto cmd_fail;	/* EEPROM器件无应答 */
	}
	

	/* 第6步：重新启动I2C总线。下面开始读取数据 */
	i2c_start();

	/* 第7步：发起控制字节，高7bit是地址，bit0是读写控制位，0表示写，1表示读 */
	i2c_send_byte(max30102_WR_address | I2C_RD);	/* 此处是读指令 */

	/* 第8步：发送ACK */
	if (i2c_wait_ack() != 0)
	{
		goto cmd_fail;	/* EEPROM器件无应答 */
	}

	/* 第9步：读取数据 */
	Data[0] = i2c_recv_byte();i2c_ack(0);		
	Data[1] = i2c_recv_byte();i2c_ack(0);	
	Data[2] = i2c_recv_byte();i2c_ack(0);	
	Data[3] = i2c_recv_byte();i2c_ack(0);	
	Data[4] = i2c_recv_byte();i2c_ack(0);	
	Data[5] = i2c_recv_byte();i2c_ack(1);	
	/* 最后1个字节读完后，CPU产生NACK信号(驱动SDA = 1) */
	/* 发送I2C总线停止信号 */
	i2c_stop();

cmd_fail: /* 命令执行失败后，切记发送停止信号，避免影响I2C总线上其他设备 */
	/* 发送I2C总线停止信号 */
	i2c_stop();

//	u8 i;
//	u8 fifo_wr_ptr;
//	u8 firo_rd_ptr;
//	u8 number_tp_read;
//	//Get the FIFO_WR_PTR
//	fifo_wr_ptr = max30102_Bus_Read(REG_FIFO_WR_PTR);
//	//Get the FIFO_RD_PTR
//	firo_rd_ptr = max30102_Bus_Read(REG_FIFO_RD_PTR);
//	
//	number_tp_read = fifo_wr_ptr - firo_rd_ptr;
//	
//	//for(i=0;i<number_tp_read;i++){
//	if(number_tp_read>0){
//		IIC_ReadBytes(max30102_WR_address,REG_FIFO_DATA,Data,6);
//	}
	
	//max30102_Bus_Write(REG_FIFO_RD_PTR,fifo_wr_ptr);
}

void max30102_init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	//端口E硬件时钟使能
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);	

	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_6;//第6引脚
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IN;			//复用功能模式
	GPIO_InitStructure.GPIO_Speed=GPIO_High_Speed;		//引脚高速工作，收到指令立即工作；缺点：功耗高
	GPIO_InitStructure.GPIO_OType=GPIO_OType_PP;		//增加输出电流的能力
	GPIO_InitStructure.GPIO_PuPd=GPIO_PuPd_UP;		//不需要上下拉电阻
	GPIO_Init(GPIOE,&GPIO_InitStructure);
	
	i2c_init();
	
	max30102_reset();
	
//	max30102_Bus_Write(REG_MODE_CONFIG, 0x0b);  //mode configuration : temp_en[3]      MODE[2:0]=010 HR only enabled    011 SP02 enabled
//	max30102_Bus_Write(REG_INTR_STATUS_2, 0xF0); //open all of interrupt
//	max30102_Bus_Write(REG_INTR_STATUS_1, 0x00); //all interrupt clear
//	max30102_Bus_Write(REG_INTR_ENABLE_2, 0x02); //DIE_TEMP_RDY_EN
//	max30102_Bus_Write(REG_TEMP_CONFIG, 0x01); //SET   TEMP_EN

//	max30102_Bus_Write(REG_SPO2_CONFIG, 0x47); //SPO2_SR[4:2]=001  100 per second    LED_PW[1:0]=11  16BITS

//	max30102_Bus_Write(REG_LED1_PA, 0x47); 
//	max30102_Bus_Write(REG_LED2_PA, 0x47); 
	
	
	
	max30102_Bus_Write(REG_INTR_ENABLE_1,0xc0);	// INTR setting
	max30102_Bus_Write(REG_INTR_ENABLE_2,0x00);
	max30102_Bus_Write(REG_FIFO_WR_PTR,0x00);  	//FIFO_WR_PTR[4:0]
	max30102_Bus_Write(REG_OVF_COUNTER,0x00);  	//OVF_COUNTER[4:0]
	max30102_Bus_Write(REG_FIFO_RD_PTR,0x00);  	//FIFO_RD_PTR[4:0]
	max30102_Bus_Write(REG_FIFO_CONFIG,0x0f);  	//sample avg = 1, fifo rollover=false, fifo almost full = 17
	max30102_Bus_Write(REG_MODE_CONFIG,0x03);  	//0x02 for Red only, 0x03 for SpO2 mode 0x07 multimode LED
	max30102_Bus_Write(REG_SPO2_CONFIG,0x27);  	// SPO2_ADC range = 4096nA, SPO2 sample rate (100 Hz), LED pulseWidth (400uS)  
	max30102_Bus_Write(REG_LED1_PA,0x24);   	//Choose value for ~ 7mA for LED1
	max30102_Bus_Write(REG_LED2_PA,0x24);   	// Choose value for ~ 7mA for LED2
	max30102_Bus_Write(REG_PILOT_PA,0x7f);   	// Choose value for ~ 25mA for Pilot LED


	
//	// Interrupt Enable 1 Register. Set PPG_RDY_EN (data available in FIFO)
//	max30102_Bus_Write(0x2, 1<<6);

//	// FIFO configuration register
//	// SMP_AVE: 16 samples averaged per FIFO sample
//	// FIFO_ROLLOVER_EN=1
//	//max30102_Bus_Write(0x8,  1<<4);
//	max30102_Bus_Write(0x8, (0<<5) | 1<<4);

//	// Mode Configuration Register
//	// SPO2 mode
//	max30102_Bus_Write(0x9, 3);

//	// SPO2 Configuration Register
//	max30102_Bus_Write(0xa,
//			(3<<5)  // SPO2_ADC_RGE 2 = full scale 8192 nA (LSB size 31.25pA); 3 = 16384nA
//			| (1<<2) // sample rate: 0 = 50sps; 1 = 100sps; 2 = 200sps
//			| (3<<0) // LED_PW 3 = 411μs, ADC resolution 18 bits
//	);

//	// LED1 (red) power (0 = 0mA; 255 = 50mA)
//	max30102_Bus_Write(0xc, 0xb0);

//	// LED (IR) power
//	max30102_Bus_Write(0xd, 0xa0);
											
}

void max30102_reset(void)
{
	max30102_Bus_Write(REG_MODE_CONFIG,0x40);
	max30102_Bus_Write(REG_MODE_CONFIG,0x40);
}






void maxim_max30102_write_reg(uint8_t uch_addr, uint8_t uch_data)
{
//  char ach_i2c_data[2];
//  ach_i2c_data[0]=uch_addr;
//  ach_i2c_data[1]=uch_data;
//	
//  IIC_WriteBytes(I2C_WRITE_ADDR, ach_i2c_data, 2);
	IIC_Write_One_Byte(I2C_WRITE_ADDR,uch_addr,uch_data);
}

void maxim_max30102_read_reg(uint8_t uch_addr, uint8_t *puch_data)
{
//  char ch_i2c_data;
//  ch_i2c_data=uch_addr;
//  IIC_WriteBytes(I2C_WRITE_ADDR, &ch_i2c_data, 1);
//	
//  i2c.read(I2C_READ_ADDR, &ch_i2c_data, 1);
//  
//   *puch_data=(uint8_t) ch_i2c_data;
	IIC_Read_One_Byte(I2C_WRITE_ADDR,uch_addr,puch_data);
}

void maxim_max30102_read_fifo(uint32_t *pun_red_led, uint32_t *pun_ir_led)
{
	uint32_t un_temp;
	unsigned char uch_temp;
	char ach_i2c_data[6];
	*pun_red_led=0;
	*pun_ir_led=0;

  
  //read and clear status register
  maxim_max30102_read_reg(REG_INTR_STATUS_1, &uch_temp);
  maxim_max30102_read_reg(REG_INTR_STATUS_2, &uch_temp);
  
  IIC_ReadBytes(I2C_WRITE_ADDR,REG_FIFO_DATA,(u8 *)ach_i2c_data,6);
  
  un_temp=(unsigned char) ach_i2c_data[0];
  un_temp<<=16;
  *pun_red_led+=un_temp;
  un_temp=(unsigned char) ach_i2c_data[1];
  un_temp<<=8;
  *pun_red_led+=un_temp;
  un_temp=(unsigned char) ach_i2c_data[2];
  *pun_red_led+=un_temp;
  
  un_temp=(unsigned char) ach_i2c_data[3];
  un_temp<<=16;
  *pun_ir_led+=un_temp;
  un_temp=(unsigned char) ach_i2c_data[4];
  un_temp<<=8;
  *pun_ir_led+=un_temp;
  un_temp=(unsigned char) ach_i2c_data[5];
  *pun_ir_led+=un_temp;
  *pun_red_led&=0x03FFFF;  //Mask MSB [23:18]
  *pun_ir_led&=0x03FFFF;  //Mask MSB [23:18]
}

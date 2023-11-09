
#include "hk32f10x.h"
#include "hq_i2c.h"

void I2C_Configuration(void)
{
	
	I2C_InitTypeDef   I2C_InitStructure;				
	GPIO_InitTypeDef   GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB ,  ENABLE);	//使能GPIOB时钟
	RCC_APB1PeriphClockCmd( RCC_APB1Periph_I2C1, ENABLE );	//使能I2C1时钟
	
	//PB6 --SCL ;PB7 --SDA
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF_OD;			//复用开漏
	GPIO_InitStructure.GPIO_Pin   =  GPIO_Pin_6 | GPIO_Pin_7;	//I2C1   SCL:PB6   SDA: PB7
	GPIO_InitStructure.GPIO_Speed =  GPIO_Speed_50MHz;
 	GPIO_Init(GPIOB, &GPIO_InitStructure);						//初始化GPIOB结构体

  	I2C_DeInit(I2C1);											//初始化I2C1
  	I2C_InitStructure.I2C_Ack  		 	=  I2C_Ack_Enable;		//使能应答
	I2C_InitStructure.I2C_AcknowledgedAddress =  I2C_AcknowledgedAddress_7bit;		//7位地址
	I2C_InitStructure.I2C_ClockSpeed	= 400000 ; 				//时钟速度 400K
	I2C_InitStructure.I2C_DutyCycle  	= I2C_DutyCycle_2 ;		//2分频  16:9
	I2C_InitStructure.I2C_Mode 			=  I2C_Mode_I2C;		//I2C模式
	I2C_InitStructure.I2C_OwnAddress1 	= 0X30 ;				//主机地址 随便写 不影响
	I2C_Init(I2C1,&I2C_InitStructure );							//初始化结构体
	I2C_Cmd(I2C1,ENABLE);											//使能I2C1
}

//I2C写一个字节
void I2C_WriteByte(uint8_t addr,uint8_t data)
{
	
	while (I2C_GetFlagStatus(I2C1,  I2C_FLAG_BUSY));  					//检查I2C总线是否繁忙
	
	I2C_GenerateSTART(I2C1,  ENABLE);                							//开启I2C1
	while( !I2C_CheckEvent(I2C1,  I2C_EVENT_MASTER_MODE_SELECT)); //EV5,主模式
	
	I2C_Send7bitAddress(I2C1,LED_ADDRESS, I2C_Direction_Transmitter); //发送器件地址
	while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));
	
	I2C_SendData(I2C1,  addr);   //寄存器地址
	while(!I2C_CheckEvent(I2C1,  I2C_EVENT_MASTER_BYTE_TRANSMITTING));
	
	I2C_SendData(I2C1,  data);   //发送数据
	while(!I2C_CheckEvent(I2C1,  I2C_EVENT_MASTER_BYTE_TRANSMITTING));
	
	I2C_GenerateSTOP( I2C1,  ENABLE); //关闭I2C总线
}

void I2C_WriteNByte(uint8_t addr,uint8_t* p_buffer,uint8_t count)
{
	uint8_t data_num = 0;
	while (I2C_GetFlagStatus(I2C1,  I2C_FLAG_BUSY));  					//检查I2C总线是否繁忙

	I2C_GenerateSTART(I2C1,  ENABLE);
	while( !I2C_CheckEvent(I2C1,  I2C_EVENT_MASTER_MODE_SELECT)); //EV5,主模式

	I2C_Send7bitAddress(I2C1,LED_ADDRESS, I2C_Direction_Transmitter); //发送器件地址
	while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTING));

	//I2C_SendData(I2C1,  addr);   //寄存器地址
	//while(!I2C_CheckEvent(I2C1,  I2C_EVENT_MASTER_BYTE_TRANSMITTING));

	while (data_num != count)
	{
		while(!I2C_CheckEvent(I2C1,  I2C_EVENT_MASTER_BYTE_TRANSMITTING));
		I2C_SendData(I2C1,(uint8_t)p_buffer[data_num]);

		data_num++;
	}
	while(!I2C_CheckEvent(I2C1,  I2C_EVENT_MASTER_BYTE_TRANSMITTING));
	I2C_GenerateSTOP(I2C1,ENABLE);
}

void fx_i2c(uint8_t addr,uint8_t offset,uint8_t value)
{
	uint8_t w_buffer[2];
   // uint32_t ret;

	w_buffer[0] = offset;
	w_buffer[1] = value;
	I2C_WriteNByte(LED_ADDRESS,w_buffer,2);
}

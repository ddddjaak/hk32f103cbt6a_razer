#include "main.h"
#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "hq_i2c.h"
//#include "i2c.h"



uint8_t leds[][3] = {
  //RGB LED format:
  //{R,  G,  B} 
  {20,  10,  30},  // D40
  {50,  40,  60},  // D41
  {80,  70,  90},  // D42
  {50, 60, 70}, // D43
  {14, 13, 15}, // D44
  {17, 16, 18}, // D45
  {20, 19, 21}, // D46
  {23, 22, 24}, // D47
  {26, 25, 27}, // D48
  {29, 28, 30}, // D49
  {32, 31, 33}, // D50
  {35, 34, 36}  // D51
};

void led_driver_init(void)
{
  GPIO_InitTypeDef   GPIO_InitStructure;
  GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin   =  GPIO_Pin_8;	//SDB  GPIOB 8
	GPIO_InitStructure.GPIO_Speed =  GPIO_Speed_50MHz;
 	GPIO_Init(GPIOB, &GPIO_InitStructure);

  GPIO_WriteBit(GPIOB,GPIO_Pin_8,Bit_SET);
}

int main(void)
{
  uint8_t offset = 0;
  I2C_Configuration();
	delay_init();
  led_driver_init();
  for(offset = LED_CONTROL_REG_START;offset <= LED_CONTROL_REG_END;offset++ )
  {
    fx_i2c(LED_ADDRESS,offset,0xFF);
  }

  for(offset = PWM_OUTPUT_REG_START;offset <= PWM_OUTPUT_REG_END;offset++)
  {
    fx_i2c(LED_ADDRESS,offset,0xFF);
  }

  fx_i2c(LED_ADDRESS,PWM_UPDATE_REG,0x00);
  fx_i2c(LED_ADDRESS,OUTPUT_FREQ_REG,0x01);
  fx_i2c(LED_ADDRESS,SHUTDOWN_REG,0x01);

	while(1)
	{

    for(offset = PWM_OUTPUT_REG_START;offset <= PWM_OUTPUT_REG_END;offset++)
    {
      if(offset % 3 == 0)
      {
        fx_i2c(LED_ADDRESS,offset,leds[offset/3][0]);
        fx_i2c(LED_ADDRESS,PWM_UPDATE_REG,0x00);
        delay_ms(150);
      }
      if(offset % 3 == 1)
      {
        fx_i2c(LED_ADDRESS,offset,leds[offset/3][1]);
      }
      if(offset % 3 ==2)
      {
        fx_i2c(LED_ADDRESS,offset,leds[offset/3][2]);
      }
    }
    for(offset = PWM_OUTPUT_REG_START;offset <= PWM_OUTPUT_REG_END;offset++)
    {
      fx_i2c(LED_ADDRESS,offset,0x00);
    }
    fx_i2c(LED_ADDRESS,PWM_UPDATE_REG,0x00);

    for(offset = PWM_OUTPUT_REG_START;offset <= PWM_OUTPUT_REG_END;offset++)
    {
      if(offset % 3 == 0)
      {
        fx_i2c(LED_ADDRESS,PWM_OUTPUT_REG_END-offset+1,offset*7);
        fx_i2c(LED_ADDRESS,PWM_UPDATE_REG,0x00);
        delay_ms(150);
      }
      if(offset % 3 == 1)
      {
        fx_i2c(LED_ADDRESS,PWM_OUTPUT_REG_END-offset+1,255-offset*7);
      }
      if(offset % 3 ==2)
      {
        fx_i2c(LED_ADDRESS,PWM_OUTPUT_REG_END-offset+1,128-offset*2);
      }
    }
    for(offset = PWM_OUTPUT_REG_START;offset <= PWM_OUTPUT_REG_END;offset++)
    {
      fx_i2c(LED_ADDRESS,offset,0x00);
    }
    fx_i2c(LED_ADDRESS,PWM_UPDATE_REG,0x00);
	};
}

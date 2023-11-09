#include "delay.h"

static int8_t  fac_us=0;//us
static int16_t fac_ms=0;//ms

void delay_init(void)
{
	SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8);//  HCLK/8
	fac_us=SystemCoreClock/8000000;// 8个时钟周期是1us
	fac_ms=(int16_t)fac_us*1000;//8000个时钟周期是1ms
	// SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK);//  HCLK/8
	// fac_us=SystemCoreClock/1000000;// 64个时钟周期是1us
	// fac_ms=(int16_t)fac_us*1000;//64000个时钟周期是1ms
}
//��ʱNus
void delay_us(int32_t nus)
{		
	int32_t temp;	    	 
	SysTick->LOAD=nus*fac_us; //ʱ�����	  		 
	SysTick->VAL=0x00;        //��ռ�����
	SysTick->CTRL|=SysTick_CTRL_ENABLE_Msk ;          //��ʼ����	 
	do
	{
		temp=SysTick->CTRL;
	}
	while(temp&0x01&&!(temp&(1<<16)));//�ȴ�ʱ�䵽��   
	SysTick->CTRL&=~SysTick_CTRL_ENABLE_Msk;       //�رռ�����
	SysTick->VAL =0X00;       //��ռ�����	 
}
//��ʱNms

void delay_ms(int16_t nms)
{	 		  	  
	int32_t temp;		   
	SysTick->LOAD=(int32_t)nms*fac_ms;//ʱ�����(SysTick->LOADΪ24bit)
	SysTick->VAL =0x00;           //��ռ�����
	SysTick->CTRL|=SysTick_CTRL_ENABLE_Msk ;          //��ʼ����  
	do
	{
		temp=SysTick->CTRL;
	}
	while(temp&0x01&&!(temp&(1<<16)));//�ȴ�ʱ�䵽��   
	SysTick->CTRL&=~SysTick_CTRL_ENABLE_Msk;       //�رռ�����
	SysTick->VAL =0X00;       //��ռ�����	  	    
} 

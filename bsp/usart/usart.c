#include "usart.h"


#pragma import(__use_no_semihosting)               
//标准库需要的支持函数                   
struct __FILE   
{   
    int handle;   
    /* Whatever you require here. If the only file you are using is */   
    /* standard output using printf() for debugging, no file handling */   
    /* is required. */   
};   
/* FILE is typedef’ d in stdio.h. */   
FILE __stdout;         
//定义_sys_exit()以避免使用半主机模式      
void _sys_exit(int x)   
{   
    x = x;   
}  
/* USER CODE BEGIN 0 */
int fputc(int ch,FILE *f)
{
	while((USART1->SR&0x40)==0);
    USART1->DR = (u8) ch;      
	return ch;
}


void USART3_Init(void){
    GPIO_InitTypeDef GPIO_InitStructure;
    USART_InitTypeDef USART_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_10;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    USART_InitStructure.USART_BaudRate = 115200;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;
    USART_Init(USART3, &USART_InitStructure);

    USART_ITConfig(USART3, USART_IT_RXNE, ENABLE); //使能接收中断
    USART_Cmd(USART3, ENABLE); //使能USART3_SendByte

    NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_Init(&NVIC_InitStructure);
}
/*
void USART3_IRQHandler(void){
    unsigned char value = 0;
    if(USART_GetFlagStatus(USART3, USART_IT_RXNE) != RESET){
        value = USART_ReceiveData(USART3);
        extern void uart_receive_input(unsigned char value);
        uart_receive_input(value);
    }
}
*/
void USART3_SendByte(unsigned char data){
    while((USART3->SR & USART_FLAG_TXE) != USART_FLAG_TXE);
	USART3->DR = (uint16_t)data & (uint16_t)0x01FF;
}


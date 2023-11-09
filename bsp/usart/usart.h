#ifndef __usart_H
#define __usart_H
#ifdef __cplusplus
 extern "C" {
#endif

#include "hk32f10x.h"
#include "stdio.h"

void USART3_Init(void);
void USART3_SendByte(unsigned char data);


#endif

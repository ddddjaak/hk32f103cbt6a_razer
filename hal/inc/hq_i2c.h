#ifndef HQ_I2C_H
#define HQ_I2C_H

#include "hk32f10x.h"


#define SHUTDOWN_REG          0x00
#define PWM_OUTPUT_REG_START  0x01
#define PWM_OUTPUT_REG_END    0x24
#define PWM_UPDATE_REG        0x25
#define LED_CONTROL_REG_START 0x26
#define LED_CONTROL_REG_END   0x49
#define GLOBAL_CONTORL_REG    0x4A
#define OUTPUT_FREQ_REG       0x4B
#define RESET_REG             0x4F

/*
    A7:A3 = 01111,A0 = 0
    if  AD connected to GND,AD = 00;
        AD connected to VCC,AD = 11;
        AD connected to SCL,AD = 01;
        AD connected to SDA,AD = 10;
        if AD = 0,addr = 0x01111000 = 0x78
*/
#define LED_ADDRESS  0x78

void I2C_Configuration(void);

extern void I2C_WriteByte(uint8_t addr,uint8_t data);
extern void I2C_WriteNByte(uint8_t addr,uint8_t* p_buffer,uint8_t count);
extern void fx_i2c(uint8_t addr,uint8_t offset,uint8_t value);

#endif

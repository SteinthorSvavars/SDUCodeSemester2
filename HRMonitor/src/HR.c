#ifndef F_CPU
#define F_CPU 16E6 // CPU clock 16 MHz
#endif

#include "i2cmaster.h"
#include "HR.h"
#include "util/delay.h"
#include <stdio.h>

#define HR_ADR (0x57<<1) // 0b1010111
#define HR_INIT  ((0x00 | En) & ~Rs) & (~Rw)
#define ModeConfig 0x09
#define SPO2 0x0A
#define LEDPulse1 0x0C
#define LEDPulse2 0x0D
#define MultiLED1 0x11
#define MultiLED2 0x12
#define DieTempInt 0x1F
#define DieTempFrac 0x20
#define DieTempConfig 0x21
#define HRWrite 0xAE
#define HRRead 0xAF
#define Rw 0x02  // Read/Write bit (Read = 1, Write = 0)
#define Rs 0x01  // Register select bit (Data = 1, Control = 0)
#define En 0x04

void HR_init(void){
    // Set sensor configuration
	i2c_start_wait(HR_ADR + I2C_WRITE);
    i2c_write(ModeConfig);
    i2c_write(0b00000111);
    i2c_stop();
    i2c_start_wait(HR_ADR + I2C_WRITE);
    i2c_write(0x11);
    i2c_write(0b00000001);
    i2c_stop();
    i2c_start_wait(HR_ADR + I2C_WRITE);
    i2c_write(0x0C);
    i2c_write(0x01);
    i2c_stop();
    _delay_us(100);
    
}
unsigned char HR_Read(void)
{
    //return I2C_Read_Byte_Single_Reg(LCD_PCF8574A_ADDR);
	i2c_start_wait(HR_ADR + I2C_WRITE);
    i2c_write(0x1F);
    i2c_rep_start(HR_ADR + I2C_READ);
	unsigned char result = i2c_readNak();
	i2c_stop();
	return result;
}




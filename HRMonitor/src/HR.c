#ifndef F_CPU
#define F_CPU 16E6 // CPU clock 16 MHz
#endif

#include "i2cmaster.h"
#include "HR.h"
#include "util/delay.h"
#include <stdio.h>

#define ModeConfig 0x09;
#define SPO2 0x0A;
#define LEDPulse1 0x0C;
#define LEDPulse2 0x0D;
#define MultiLED1 0x11;
#define MultiLED2 0x12;
#define DieTempInt 0x1F;
#define DieTempFrac 0x20;
#define DieTempConfig 0x21;
#define HRWrite 0xAE;
#define HRRead 0xAF;

void HR_inint(){
    // Set sensor configuration
	i2c_start_wait(HR_ADR + I2C_WRITE);
    
}




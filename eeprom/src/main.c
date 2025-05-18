/*
 * HelloWorld.c
 *
 * Created: 11/9/2023 10:43:27 AM
 * Author : Alin
 */ 

#include <stdio.h>
#include <stdlib.h>

#include <avr/io.h>
#include <avr/eeprom.h>
#include <util/delay.h>

/* I2C coms */
#include "i2cmaster.h"
/* LCD */ 
#include "lcd.h"
/* Temp sens */
#include "lm75.h"

//#include "twimaster.c"
#include "usart.h"

unsigned int addr1 = 0;
unsigned int addr2 = 4;

int main() {  

  i2c_init();
  LCD_init();
  lm75_init();

  float current_temp;
  float min_temp;
  float max_temp;

  /*Buttor Config*/
  DDRC = 0xF0;
  PORTC = 0x3F;

  min_temp = eeprom_read_float((uint8_t*)addr1);
  max_temp = eeprom_read_float((uint8_t*)addr2);

  while (1){
    current_temp = (float)get_temperature();
    if(current_temp < min_temp){
      min_temp = current_temp;
      eeprom_write_float((uint8_t*)addr1, (float)min_temp);
    }
    if(current_temp > max_temp){
      max_temp = current_temp;
      eeprom_write_float((uint8_t*)addr2, (float)max_temp);
    }

    LCD_set_cursor(0,0);
    printf("CurTempIs %.2f", current_temp);
    LCD_set_cursor(0,1);
    printf("MinTempIs %.2f", min_temp);
    LCD_set_cursor(0,2);
    printf("MaxTempIs %.2f", max_temp);

    _delay_ms(1000);

    if(PINC == 0x37){
      min_temp = current_temp;
      max_temp = current_temp;
      eeprom_write_float((uint8_t*)addr1, (float)min_temp);
      eeprom_write_float((uint8_t)addr2, (float)max_temp);
    }
  }
}

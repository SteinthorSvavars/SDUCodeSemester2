#include <stdio.h>
#include <avr/io.h>
#include <util/delay.h>

#include"i2cmaster.h"
#include"lcd.h"
#include"lm75.h"
#include"usart.h"
#include <avr/eeprom.h>

unsigned int adress1 = 0;
unsigned int adress2 = 4;

int main()
{
  i2c_init();
  LCD_init();
  lm75_init();

  float current_temp;
  float min_temp;
  float max_temp;

  DDRC = 0xF0;
  PORTC = 0x3F;

  min_temp = eeprom_read_float((uint8_t*)adress1);
  max_temp = eeprom_read_float((uint8_t*)adress2);

  while(1)
  {
    current_temp = (float)get_temperature();

    if (current_temp < min_temp)
    {
      min_temp = current_temp;
      eeprom_write_float((uint8_t*)adress1, (float)min_temp);
    }
    if (current_temp > max_temp)
    {
      max_temp = current_temp;
      eeprom_write_float((uint8_t*)adress2, (float)max_temp);
    }

    LCD_set_cursor(0, 0);
    printf("Current temp:%.2f", current_temp);
    LCD_set_cursor(0, 1);
    printf("Min temp:%.2f", min_temp);
    LCD_set_cursor(0, 2);
    printf("Max temp:%.2f", max_temp);

    _delay_ms(1000);

    if (PINC == 0b00110111)
    {
      min_temp = current_temp;
      max_temp = current_temp;
      eeprom_write_float((uint8_t*)adress1, (float)min_temp);
      eeprom_write_float((uint8_t*)adress2, (float)max_temp);
    }
    
  }
}



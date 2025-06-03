// Practice assignment 4

#include <stdio.h>
#include <avr/io.h>
#include <util/delay.h>

#include <i2cmaster.h>
#include <lcd.h>

int main(void) {  

  i2c_init();
  LCD_init();

  DDRC = 0xF0; // set data direction for port C pins, 0-3 as input (i.e. the buttons)
  PORTC = 0x3F; // set pull-up resistor for port C
  DDRD = 0b11101111; 
  PORTD= 0b00010000; 

  TCCR0B |= (1 << CS02) | (1 << CS01) | (1 << CS00);
  while(1) {
    LCD_set_cursor(0,0);
    printf("%hhd",TCNT0);
  }

  return 0;
}

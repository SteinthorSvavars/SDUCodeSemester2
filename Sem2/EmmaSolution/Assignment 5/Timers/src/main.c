/*
 * HelloWorld.c
 *
 * Created: 11/9/2023 10:43:27 AM
 * Author : Alin
 */ 



#include <stdio.h>
#include <avr/io.h>
#include <util/delay.h>

#include <avr/eeprom.h>

#include "usart.h"
#include "i2cmaster.h"
#include "lcd.h"


void delay_ms(uint16_t);
void delay_hs(uint16_t);

int main(void) {  

  //i2c_init();
  uart_init(); // open the communication to the microcontroller
  io_redirect(); // redirect input and output to the communication
  

  DDRC = 0xF0;
  DDRD = 0xFF;
  DDRD &= ~(1<<DDD4);
  PORTD |= (1<<PORTD4);
  PORTC = 0x3F;
  TCCR0B |= (1<<CS00)|(1<<CS01)|(1<<CS02);


  //TCCR0B |= (1<<CS00)|(1<<CS01)|(1<<CS02);

  while(1) 
  {

    delay_hs(10);
    printf("%d\n", TCNT0);
    /*
    PORTD = 0x20;
    delay_ms(500);
    PORTD = 0x00;
    delay_ms(500);
    */

    /*
    PORTD = 0x20;
    delay_hs(10);
    PORTD = 0x00;
    delay_hs(10);
    */
    /*
    PORTD = 0x20;
    delay_hs(20);
    PORTD = 0x00;
    delay_hs(20);
    */


  }
  return 0;
}

void delay_ms(uint16_t delay_millisec)
{
  TCCR0A = 00000010;
  OCR0A = 249;
  TCCR0B = 00000011;

  for (uint16_t i = 0; i < delay_millisec; i++)
  {

    while ((TIFR0 & (1<<OCF0A)) == 0)
    {
    }
    TIFR0 = (1<<OCF0A);
  }
}

void delay_hs(uint16_t dellay_hunderetsec)
{
  TCCR1B = 00001011;
  OCR1A = 24999;

  for (uint16_t i = 0; i < dellay_hunderetsec; i++)
  {
    while ((TIFR1 & (1<<OCF1A)) == 0)
    {
    }
    TIFR1 = (1<<OCF1A);
  }
  
}






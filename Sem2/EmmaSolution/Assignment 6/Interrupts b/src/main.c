/*
 * HelloWorld.c
 *
 * Created: 11/9/2023 10:43:27 AM
 * Author : Alin
 */ 



#include <stdio.h>
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <avr/eeprom.h>

#include "usart.h"
#include "i2cmaster.h"
#include "lcd.h"

volatile unsigned int counter = 0;

void timers(unsigned int time)
{
  TCCR0A |= (1 << WGM01);
  OCR0A = 0xF9;
  TIMSK0 |= (1 << OCIE0A);
  sei();
  TCCR0B |= (1 << CS01) | (1 << CS00);
  counter=time;
  PORTD |= (1 << PIND4) | (1 << PIND5) | (1 << PIND6) | (1 << PIND7);
}

int main(void)
{
  DDRD = 0xFF;
  PORTD = 0x00;
  DDRC = 0xF0;
  PORTC =0x3F;

  
  

  while (1)
  {
    if (PINC == 0b00110111)
  {
    timers(2000);
  }
  }
  return 0;
}


ISR (TIMER0_COMPA_vect)
{
  
  if(counter)
    counter--;
  else {
    PORTD &= ~((1 << PIND4) | (1 << PIND5) | (1 << PIND6) | (1 << PIND7));
    cli();
  }
  
}




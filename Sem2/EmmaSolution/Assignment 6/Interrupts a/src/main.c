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

volatile int counter = 0;

void timers(void)
{
  TCCR0A |= (1 << WGM01);
  OCR0A = 249;
  TIMSK0 |= (1<<OCIE0A);
  sei();
  TCCR0B |= (1 << CS01) | (1 << CS00);
}

int main(void)
{
  DDRD = 0xFF;
  PORTD = 0x00;
  DDRC = 0xF0;
  PORTC =0x3F;

  timers();

  while (1)
  {
  }
  return 0;
}

ISR (TIMER0_COMPA_vect)
{
  counter ++;
  if (counter == 500)
  {
    PORTD ^= (1 << PIND4) ^ (1 << PIND5) ^ (1 << PIND6) ^ (1 << PIND7);
    counter = 0;
  }
  
}




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

void interrupt_init()
{
  EICRA |= (1 << ISC01) | (1 << ISC00);
  EIMSK |= (1 << INT0);
  sei();
}

void timers(unsigned int time)
{
  TCCR0A |= (1 << WGM01);
  OCR0A = 0xF9;
  TIMSK0 |= (1 << OCIE0A);
  TCCR0B |= (1 << CS01) | (1 << CS00);
  counter=time;
  PORTD |= (1 << PIND4) | (1 << PIND5) | (1 << PIND6) | (1 << PIND7);
}

int main(void)
{
  DDRC = 0xF0; // set data direction for port C pins, 0-3 as input (i.e. the buttons)
  PORTC = 0x3F; // set pull-up resistor for port C
  DDRD = 0b11110000; // set data direction for port D, all output 
  PORTD= 0b00001111;

  
  interrupt_init();

  while (1)
  {
  }
  return 0;
}

ISR (INT0_vect)
{
  timers(2000);
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





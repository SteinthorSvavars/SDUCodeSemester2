// Practice assignment 6b

#include <stdio.h>
#include <avr/io.h>
#include <avr/interrupt.h>

volatile unsigned int time_tick_counter=0;

void set_light(unsigned int time)
{
  TCCR0A |= (1 << WGM01);
  OCR0A = 0xF9;
  TIMSK0 |= (1 << OCIE0A);
  sei();
  TCCR0B |= (1 << CS01) | (1 << CS00);
  time_tick_counter=time;
  PORTD |= (1 << PIND4);
}

int main(void) {  

  DDRC = 0xF0; // set data direction for port C pins, 0-3 as input (i.e. the buttons)
  PORTC = 0x3F; // set pull-up resistor for port C
  DDRD = 0xFF; // set data direction for port D, all output 
  PORTD= 0x00; // set output for port D (none)

  while(1) {
    if(!(PINC & 1))
      set_light(2000);
  }

  return 0;
}

ISR (TIMER0_COMPA_vect)
{
  if(time_tick_counter)
    time_tick_counter--;
  else {
    PORTD &= ~(1 << PIND4);
    cli();
  }
}
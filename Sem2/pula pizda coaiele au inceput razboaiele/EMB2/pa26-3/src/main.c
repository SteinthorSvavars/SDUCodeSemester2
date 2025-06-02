// Practice assignment 6c

#include <stdio.h>
#include <avr/io.h>
#include <avr/interrupt.h>

volatile unsigned int time_tick_counter=0;

void init_ext_interrupts() {
  EICRA |= (1 << ISC01) | (1 << ISC00); // set INT0 to trigger on a rising edge
  EIMSK |= (1 << INT0); // Turns on interrupt for INT0
  sei();
}

void set_light(unsigned int time) //timer for the LED
{
  TCCR0A |= (1 << WGM01);
  OCR0A = 0xF9;
  TIMSK0 |= (1 << OCIE0A);
  TCCR0B |= (1 << CS01) | (1 << CS00);
  time_tick_counter=time;
  PORTD |= (1 << PIND4);
}

int main(void) {  

  DDRC = 0xF0; // set data direction for port C pins, 0-3 as input (i.e. the buttons)
  PORTC = 0x3F; // set pull-up resistor for port C
  DDRD = 0b11111011; // set data direction for port D, all output 
  PORTD= 0b00000100;

  init_ext_interrupts();

  while(1) {
  }

  return 0;
}

ISR (INT0_vect)
{
  set_light(1000);
} //set LED when PD4 is INT0 is triggered

ISR (TIMER0_COMPA_vect) // set up timer
{
  if(time_tick_counter)
    time_tick_counter--;
  else {
    PORTD &= ~(1 << PIND4);
    cli();
  }
}
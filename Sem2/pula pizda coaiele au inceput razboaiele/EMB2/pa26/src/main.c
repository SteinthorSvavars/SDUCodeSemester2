// Practice assignment 6, part a

#include <stdio.h>
#include <avr/io.h>
#include <avr/interrupt.h>

volatile int bazinga=0; // make variable for timer

void init_interrupts() // set up timer
{
  TCCR0A |= (1 << WGM01);
  OCR0A = 0xF9;
  TIMSK0 |= (1 << OCIE0A);
  sei();
  TCCR0B |= (1 << CS01) | (1 << CS00);
}

int main(void) {  

  DDRC = 0xF0; // set data direction for port C pins, 0-3 as input (i.e. the buttons)
  PORTC = 0x3F; // set pull-up resistor for port C
  DDRD = 0xFF; // set data direction for port D, all output 
  PORTD= 0x00; // set output for port D (none)

  init_interrupts();
  while(1) {}

  return 0;
}

ISR (TIMER0_COMPA_vect) // ISR for the timer ticking
{
  bazinga++;
  if(bazinga==500) {
    PORTD ^= (1 << PIND4); // just increment and when the value reaches what it's supposed to trigger the LED :p
    bazinga=0;
  }
}
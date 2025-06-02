// Practice assignment 4

#include <stdio.h>
#include <avr/io.h>
#include <util/delay.h>

void delay_ms(unsigned int milliseconds)
{
    for(unsigned int i=0;i<milliseconds;i++) {
      TCCR0A |= (1 << WGM01);
      OCR0A = 249;
      TCCR0B |= (1 << CS01) | (1 << CS00);
      while ( (TIFR0 & (1 << OCF0A) ) == 0) {}
      TIFR0 = (1 << OCF0A);
    }
}

void delay_hs(unsigned int hundred_milliseconds)
{
    for(unsigned int i=0;i<10*hundred_milliseconds;i++) {
      TCCR0A |= (1 << WGM01);
      OCR0A = 155;
      TCCR0B |= (1 << CS02) | (1 << CS00);
      while ( (TIFR0 & (1 << OCF0A) ) == 0) {}
      TIFR0 = (1 << OCF0A);
    }
}

int main(void) {  

  DDRC = 0xF0; // set data direction for port C pins, 0-3 as input (i.e. the buttons)
  PORTC = 0x3F; // set pull-up resistor for port C
  DDRD = 0xFF; // set data direction for port D, all output 
  PORTD= 0x00; // set output for port D (none)

  while(1) {
    delay_hs(20);
    PORTD ^= (1 << PIND4);
  }

  return 0;
}

// Practice assignment 4

#include <stdio.h>
#include <avr/io.h>
#include <util/delay.h>

void delay_ms(unsigned int milliseconds)
{
    // Set the Timer Mode to CTC
    TCCR1A = 0;
    
    TCCR1B = (1 << WGM12) | (1 << CS12) |(1 << CS10); // WGM12 sets CTC on timer 1, and it's on register B instead of A because fuck you
    OCR1A = 15624; // (16000000/1024)-1
    // start the timer
    //TCCR1B |= (1 << CS01) | (1 << CS00);// set prescaler to 64 and start the timer
    while ( (TIFR1 & (1 << OCF1A) ) == 0)  // wait for the overflow event
    {
      
    }
    // reset the overflow flag
    TIFR1 = (1 << OCF1A);
    PORTD ^= (1 << PIND4);
}

int main(void) {  

  DDRC = 0xF0; // set data direction for port C pins, 0-3 as input (i.e. the buttons)
  PORTC = 0x3F; // set pull-up resistor for port C
  DDRD = 0xFF; // set data direction for port D, all output 
  PORTD= 0x00; // set output for port D (none)

  while(1) {
    delay_ms(1);
  }

  return 0;
}

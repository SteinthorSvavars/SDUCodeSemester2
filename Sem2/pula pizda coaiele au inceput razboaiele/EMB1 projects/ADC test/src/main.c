// Practice assignment 9, exercise 1

#include <stdio.h>
#include <avr/io.h>
#include <util/delay.h>

//library init

int main(void) {  


  DDRC = 0xF0; // set data direction for port C pins, 0-3 as input (i.e. the buttons)
  PORTC = 0x3F; // set pull-up resistor for port C
  DDRD = 0xFF; // set data direction for port D, all output 
  PORTD= 0x00; // set output for port D (none)
    
  while(1) { // start program loop
    if(!(PINC & 1)) //read pin A0/PC0 by AND'ing it to mask 0x01. (as the Arduino is set to use pull-up resistors on the buttons, the default state of the buttons is high, and therefore we need to flip its result in the if statement)
    {
      PORTD ^= (1 << PIND4); //set pin with XOR and a mask to turn LED on
      _delay_ms(250); // dealy
      PORTD ^= (1 << PIND4); //set LED off
      PORTD ^= (1 << PIND5); // do that again
      _delay_ms(250);
      PORTD ^= (1 << PIND5);
      PORTD ^= (1 << PIND6);
      _delay_ms(250);
      PORTD ^= (1 << PIND6);
      PORTD ^= (1 << PIND7);
      _delay_ms(250);
      PORTD ^= (1 << PIND7);
    }
    else if(!((PINC >> 1) & 1)) //same thing, but for a different button and with a different delay
    {
      PORTD ^= (1 << PIND4);
      _delay_ms(500);
      PORTD ^= (1 << PIND4);
      PORTD ^= (1 << PIND5);
      _delay_ms(500);
      PORTD ^= (1 << PIND5);
      PORTD ^= (1 << PIND6);
      _delay_ms(500);
      PORTD ^= (1 << PIND6);
      PORTD ^= (1 << PIND7);
      _delay_ms(500);
      PORTD ^= (1 << PIND7);
    }
    else if(!((PINC >> 2) & 1)) //same thing, again
    {
      PORTD ^= (1 << PIND4);
      _delay_ms(1000);
      PORTD ^= (1 << PIND4);
      PORTD ^= (1 << PIND5);
      _delay_ms(1000);
      PORTD ^= (1 << PIND5);
      PORTD ^= (1 << PIND6);
      _delay_ms(1000);
      PORTD ^= (1 << PIND6);
      PORTD ^= (1 << PIND7);
      _delay_ms(1000);
      PORTD ^= (1 << PIND7);
    }
    else if(!((PINC >> 3) & 1)) //same thing, different delay
    {
      PORTD ^= (1 << PIND4);
      _delay_ms(1500);
      PORTD ^= (1 << PIND4);
      PORTD ^= (1 << PIND5);
      _delay_ms(1500);
      PORTD ^= (1 << PIND5);
      PORTD ^= (1 << PIND6);
      _delay_ms(1500);
      PORTD ^= (1 << PIND6);
      PORTD ^= (1 << PIND7);
      _delay_ms(1500);
      PORTD ^= (1 << PIND7);
    }
    else // default state (no buttons)
    {
      PORTD ^= (1 << PIND4);
      _delay_ms(500);
      PORTD ^= (1 << PIND4);
      PORTD ^= (1 << PIND5);
      _delay_ms(500);
      PORTD ^= (1 << PIND5);
      PORTD ^= (1 << PIND6);
      _delay_ms(500);
      PORTD ^= (1 << PIND6);
      PORTD ^= (1 << PIND7);
      _delay_ms(500);
      PORTD ^= (1 << PIND7);
    }
  }
  return 0;
}

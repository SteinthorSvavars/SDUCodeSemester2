#include <stdio.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#include "usart.h"

volatile uint8_t prev_state;
int LED1 = 0;
int LED2 = 0;

void setup() {
    // Set PC0 (Analog A0) as input
    DDRC &= ~(1 << PC0);
    PORTC |= (1 << PC0); // Enable pull-up resistor

    // Set PD5 as output (LED)
    DDRD |= (1 << PD5);
    //DDRD |= _BV(DDD4);

    // Save initial state
    prev_state = PINC & (1 << PC0);

    // Enable PCINT for PC0 (which is PCINT8)
    PCICR |= (1 << PCIE1);       // Enable pin change interrupt for PCINT[14:8]
    PCMSK1 |= (1 << PCINT8);     // Enable interrupt for PC0 (PCINT8)

    sei(); // Enable global interrupts
}

ISR(PCINT1_vect) {
    uint8_t current = PINC & (1 << PC0);
    if (current != prev_state) {
        // Pin changed
        PORTD ^= (1 << PD5);  // Toggle LED
        prev_state = current;
    
     _delay_ms(150);
      /*if(LED1 == 0){
        PORTD |= _BV(PORTD4);
      }
      else if(LED1 == 1){
        PORTD &= ~_BV(PORTD4);
      }
      if(LED1 == 0){
        LED1 = 1;
      }
      else if(LED1 == 1){
        LED1 = 0;
      }*/
    }
}

int main() {
    setup();
    while (1) {
      //PORTD |= _BV(PORTD4)|_BV(PORTD5)|_BV(PORTD6)|_BV(PORTD7);
    }
}

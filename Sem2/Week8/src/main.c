#include <stdio.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#include "usart.h"

volatile uint8_t timer_enabled = 0;

void setup() {
    // Set PC0 (Analog A0) as input
    DDRC &= ~(1 << PC0);
    PORTC |= (1 << PC0); // Enable pull-up resistor

    // Set PD5 as output (LED)
    DDRD |= (1 << PD5);
    DDRD |= _BV(DDD4);
    DDRD |= _BV(DDD6);
    DDRD |= _BV(DDD7);

    // Save initial state
    //prev_state = PINC & (1 << PC0);

    // Enable PCINT for PC0 (which is PCINT8)
    PCICR |= (1 << PCIE1);       // Enable pin change interrupt for PCINT[14:8]
    PCMSK1 |= (1 << PCINT8);     // Enable interrupt for PC0 (PCINT8)

    TCCR0A |= (1<<WGM01);
    OCR0A |= 0xF9;
    TIMSK0 |= (1<<OCIE0A);
    sei(); // Enable global interrupts
    TCCR0B |= (1<<CS02);
}
//PCINT1_vect
//TIMER1_OVF_vect
ISR(TIMER0_COMPA_vect) {
  if (timer_enabled)
    {
      PORTD ^= (1 << PD7);  // Toggle LED
      PORTD ^= (1 << PD6);  // Toggle LED
      PORTD ^= (1 << PD5);  // Toggle LED
      PORTD ^= (1 << PD4);  // Toggle LED
    }     
}

int main() {
    setup();

    uint8_t prev_button_state = 1;
    while (1) {
      if
      uint8_t current_button_state = PINC & (1 << PC0);
      // Detect button press (falling edge)
      if (current_button_state == 0 && prev_button_state != 0)
      {
        timer_enabled = !timer_enabled; // Toggle enable flag
      }
      prev_button_state = current_button_state;
    }
}
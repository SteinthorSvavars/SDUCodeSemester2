/*
 * HelloWorld.c
 *
 * Created: 11/9/2023 10:43:27 AM
 * Author : Alin
 */ 
#include <avr/io.h>
#include <avr/interrupt.h>

void timer1_init() {
    TCCR1A |= (1<<WGM12);              // Normal mode
    TCCR1B |= (1<<CS12);    // Prescaler = 256
    TCNT1 = 0;               // Start from 0
    TIMSK1 = (1 << TOIE1);   // Enable overflow interrupt
    sei();                   // Enable global interrupts
}

ISR(TIMER1_OVF_vect) {
        PORTB ^= (1 << PB5);     // Toggle LED on pin 13 (PB5)
}

int main() {
    DDRB |= (1 << PB5);      // Set pin 13 as output

    timer1_init();
    while (1) {
        // Main loop does nothing
    }
}


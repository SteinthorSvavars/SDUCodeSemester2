/*
 * HelloWorld.c
 *
 * Created: 11/9/2023 10:43:27 AM
 * Author : Alin
 */ 



#include <stdio.h>  
#include <util/delay.h>
#include <stdlib.h>   // For atoi() conversion
#include <avr/io.h>
#include <avr/interrupt.h>

#define BAUD 9600           // Desired baud rate
#define MYUBRR (F_CPU/16/BAUD-1)

/* USART */
void USART_Init(unsigned int ubrr) {
    // Set baud rate registers
    UBRR0H = (unsigned char)(ubrr >> 8);
    UBRR0L = (unsigned char)ubrr;

    // Enable transmitter and receiver
    UCSR0B = (1 << TXEN0) | (1 << RXEN0);
    
    // Set frame format: 8 data bits, 1 stop bit, no parity
    UCSR0C = (1 << UCSZ01) | (1 << UCSZ00);
}
// Transmit a single character over USART.
void USART_Transmit(unsigned char data) {
    // Wait for the transmit buffer to be ready
    while (!(UCSR0A & (1 << UDRE0)));
    // Send the data by placing it in the USART data register
    UDR0 = data;
}
// Helper function to send a null-terminated string over USART.
void USART_SendString(const char *str) {
    while (*str) {
        USART_Transmit(*str++);
    }
}
// Wait for and return a single received character.
unsigned char USART_Receive(void) {
    // Wait until a character is received
    while (!(UCSR0A & (1 << RXC0)));
    return UDR0;
}
// Receive a string from USART until a newline ('\n') or carriage return ('\r')
// is detected or the buffer size limit is reached.
void USART_ReceiveString(char *buffer, uint8_t buffer_size) {
    uint8_t i = 0;
    char ch;
    
    // Read characters one by one
    while (1) {
        ch = USART_Receive();
        
        // If a newline or carriage return is received, terminate the string.
        if (ch == '\n' || ch == '\r') {
            buffer[i] = '\0';
            break;
        }
        
        // Only add character if there is space (reserve space for the null-terminator)
        if (i < buffer_size - 1) {
            buffer[i++] = ch;
        }
    }
}


void timer1_init() {
    TCCR1A = 0;              // Normal mode
    TCCR1B = (1 << CS12);    // Prescaler = 256
    TCNT1 = 0;               // Start from 0
    TIMSK1 = (1 << TOIE1);   // Enable overflow interrupt
    sei();                   // Enable global interrupts
}

ISR(TIMER1_OVF_vect) {
    PORTB ^= (1 << PB5);     // Toggle LED on pin 13 (PB5)
}

int main(){
  // Initialize USART communication.
  USART_Init(MYUBRR);
  _delay_ms(100);  // Short delay to allow the serial monitor to connect

  /*char inputBuffer[20]; // Buffer to store the received characters
  int number1 = 0;
  int number2 = 0;
  char message[50];*/

  while(1){
    DDRB |= (1 << PB5);      // Set pin 13 as output
    timer1_init();
  }

}
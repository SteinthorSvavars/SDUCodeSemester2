/*
 * HelloWorld.c
 *
 * Created: 11/9/2023 10:43:27 AM
 * Author : Alin
 */ 



#include <stdio.h>
#include <avr/io.h>
#include <util/delay.h>
#include <stdlib.h>   // For atoi() conversion


#include "usart.h"

//#define F_CPU 16000000UL    // Define CPU clock as 16MHz
#define BAUD 9600           // Desired baud rate
#define MYUBRR (F_CPU/16/BAUD-1)

// Initialize USART with both transmitter (TX) and receiver (RX) enabled.
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

int main(void) {
    // Initialize USART communication.
    USART_Init(MYUBRR);
    _delay_ms(100);  // Short delay to allow the serial monitor to connect
    
    char inputBuffer[20]; // Buffer to store the received characters
    int number1 = 0;
    int number2 = 0;
    char message[50];
    
    while (1) {
        USART_SendString("Enter first number: ");
        do{
        // Receive first number as a string.
        USART_ReceiveString(inputBuffer, sizeof(inputBuffer));
        // Convert received string to integer.
        number1 = atoi(inputBuffer);
        }while(number1 == 0);
      
        // Mirror the newline for clarity.
        USART_SendString("\r\n");
      
        USART_SendString("Enter second number: ");
        do{ 
        // Receive second number as a string.
        USART_ReceiveString(inputBuffer, sizeof(inputBuffer));
        // Convert received string to integer.
        number2 = atoi(inputBuffer);
        }while(number2 == 0);
        USART_SendString("\r\n");
        
        // Create a message to echo the received numbers.
        sprintf(message, "Received numbers: %d and %d\r\n\r\n", number1, number2);
        USART_SendString(message);
    }
    
    return 0;
}






/*
#define BAUD 9600
#define UBRR_VAL ((F_CPU / 16 / BAUD) - 1)

// === USART SETUP ===
void usart_init() {
    UBRR0H = (uint8_t)(UBRR_VAL >> 8);
    UBRR0L = (uint8_t)UBRR_VAL;
    UCSR0B = (1 << RXEN0) | (1 << TXEN0); // Enable RX and TX
    UCSR0C = (1 << UCSZ01) | (1 << UCSZ00); // 8-bit data
}

void usart_send_char(char c) {
    while (!(UCSR0A & (1 << UDRE0))); // Wait for empty buffer
    UDR0 = c;
}

void usart_send_string(const char *s) {
    while (*s) {
        usart_send_char(*s++);
    }
}

char usart_receive_char() {
    while (!(UCSR0A & (1 << RXC0))); // Wait for data
    return UDR0;
}

void usart_receive_string(char *buffer, uint8_t max_len) {
    uint8_t i = 0;
    char c;
    while (i < (max_len - 1)) {
        c = usart_receive_char();
        if (c == '\n' || c == '\r') break;
        buffer[i++] = c;
        usart_send_char(c); // Echo back
    }
    buffer[i] = '\0';
}

// === MAIN ===
int main(void) {
    char buffer[16];
    int num1, num2;
    char out[32];

    usart_init();
    _delay_ms(100);
    usart_send_string("Enter first number:\r\n");

    usart_receive_string(buffer, sizeof(buffer));
    num1 = atoi(buffer);

    usart_send_string("\r\nEnter second number:\r\n");
    usart_receive_string(buffer, sizeof(buffer));
    num2 = atoi(buffer);

    usart_send_string("\r\nYou entered:\r\n");
    itoa(num1, buffer, 10);
    usart_send_string("Number 1: ");
    usart_send_string(buffer);
    usart_send_string("\r\n");

    itoa(num2, buffer, 10);
    usart_send_string("Number 2: ");
    usart_send_string(buffer);
    usart_send_string("\r\n");

    while (1) {
        // Do nothing
    }
}
*/

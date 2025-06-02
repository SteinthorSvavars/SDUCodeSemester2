/*
 * HelloWorld.c
 *
 * Created: 11/9/2023 10:43:27 AM
 * Author : Alin
 */ 
#include <stdio.h>
#include <stdlib.h>

#include <avr/io.h>
#include <util/delay.h>

/* I2C coms */
#include "i2cmaster.h"
/* LCD */ 
#include "lcd.h"
/* Temp sens */
/*
#include "lm75.c"
#include "lm75.h"
*/

//#include "twimaster.c"
#include "usart.h"

//#define F_CPU 16000000UL    // Define CPU clock as 16MHz
#define BAUD 19200           // Desired baud rate
#define MYUBRR (F_CPU/16/BAUD-1)

int number;

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
    
    i2c_init(); 
    LCD_init();
    // Initialize USART communication.
    USART_Init(MYUBRR);
    _delay_ms(100);  // Short delay to allow the serial monitor to connect
    
    DDRD |= _BV(DDD4)|_BV(DDD5)|_BV(DDD6)|_BV(DDD7);

    char inputBuffer[20]; // Buffer to store the received characters
    int number1 = 0;
    int number2 = 0;
    char message[50];
    unsigned char receivedChar;
    
    while (1) {
        USART_SendString("Enter first number: ");
        do{
        // Receive first number as a string.
        USART_ReceiveString(inputBuffer, sizeof(inputBuffer));
        // Convert received string to integer.
        number1 = atoi(inputBuffer);
        }while(number1 == 0);
        receivedChar = USART_Receive();
        
        LCD_write_char(receivedChar); // Display received char
        USART_SendString("\r\n");
      
        USART_SendString("Enter second number: ");
        do{ 
        // Receive second number as a string.
        USART_ReceiveString(inputBuffer, sizeof(inputBuffer));
        // Convert received string to integer.
        number2 = atoi(inputBuffer);
        }while(number2 == 0);

        receivedChar = USART_Receive();
        LCD_write_char(receivedChar); // Display received char
        USART_SendString("\r\n");
        // Create a message to echo the received numbers.
        sprintf(message, "Received numbers: %d and %d\r\n\r\n", number1, number2);
        USART_SendString(message);
    }
    return 0;
}

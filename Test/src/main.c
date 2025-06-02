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
//#include "usart.h"

void USART_Init(unsigned int ubrr) {
    UBRR0H = (unsigned char)(ubrr >> 8);
    UBRR0L = (unsigned char)ubrr;
    UCSR0B = (1 << RXEN0) | (1 << TXEN0); // Enable RX and TX
    UCSR0C = (1 << UCSZ01) | (1 << UCSZ00); // 8-bit data
}

unsigned char USART_Receive(void) {
    while (!(UCSR0A & (1 << RXC0))); // Wait for data
    return UDR0; // Get and return received data
}

void LCD_Command(unsigned char cmd);
void LCD_Char(unsigned char data);
void LCD_Init();

void LCD_Command(unsigned char cmd) {
    PORTC = cmd;
    PORTD &= ~(1 << PD0); // RS = 0 for command
    PORTD |= (1 << PD2);  // EN = 1
    _delay_ms(1);
    PORTD &= ~(1 << PD2); // EN = 0
    _delay_ms(2);
}

void LCD_Char(unsigned char data) {
    PORTC = data;
    PORTD |= (1 << PD0);  // RS = 1 for data
    PORTD |= (1 << PD2);  // EN = 1
    _delay_ms(1);
    PORTD &= ~(1 << PD2); // EN = 0
    _delay_ms(2);
}

void LCD_Init() {
    DDRC = 0xFF; // LCD data lines
    DDRD |= (1 << PD0) | (1 << PD2); // RS and EN as output
    _delay_ms(20);
    LCD_Command(0x38); // 8-bit, 2-line, 5x7 font
    LCD_Command(0x0C); // Display on, cursor off
    LCD_Command(0x01); // Clear display
    _delay_ms(2);
}

int main(void) {
    USART_Init(103); // Baud rate 9600 for 16 MHz clock
    LCD_Init();

    while (1) {
        unsigned char receivedChar = USART_Receive();
        LCD_Char(receivedChar); // Display received char
    }
}

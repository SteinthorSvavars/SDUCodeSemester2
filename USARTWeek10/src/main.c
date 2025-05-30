/*
 * HelloWorld.c
 *
 * Created: 11/9/2023 10:43:27 AM
 * Author : Alin
 */ 



#include <stdio.h>
#include <avr/io.h>
#include <util/delay.h>

#include "usart.h"

//#define F_CPU 16000000UL
#define BAUD 9600
#define MYUBRR (F_CPU/16/BAUD - 1)

void usart_init(unsigned int ubrr) {
  UBRR0H = (unsigned char)(ubrr >> 8);
  UBRR0L = (unsigned char)ubrr;
  UCSR0B = (1 << TXEN0);             // Enable transmitter
  UCSR0C = (1 << UCSZ01) | (1 << UCSZ00); // 8-bit data
}

void usart_send(unsigned char data) {
  while (!(UCSR0A & (1 << UDRE0)));  // Wait until buffer is empty
  UDR0 = data;
}

int main(void) {
  usart_init(MYUBRR);
  while (1) {
    usart_send('A');
    _delay_ms(1000);
  }
}

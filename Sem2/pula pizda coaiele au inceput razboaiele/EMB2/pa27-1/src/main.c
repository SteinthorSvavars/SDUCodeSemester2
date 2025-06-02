// Practice assignment 7, exercise 1

#include <stdio.h>
#include <avr/io.h>
#include <util/delay.h>

#define BAUDRATE 57600

#define BAUD_PRESCALER (((F_CPU / (BAUDRATE * 16UL))) - 1)

void usart_init(void);

void usart_send(unsigned char data);

int main(void) {  

  usart_init();

  DDRC = 0xF0; // set data direction for port C pins, 0-3 as input (i.e. the buttons)
  PORTC = 0x3F; // set pull-up resistor for port C
  DDRD = 0xFF; // set data direction for port D, all output 
  PORTD= 0x00; // set output for port D (none)
    
  while(1) { // start program loop
    if(!((PINC >> 3) & 1)) // send , same way to read pin as before
    {
      usart_send(4);
    }
    else if(!((PINC >> 2) & 1))
    {
      usart_send(3);
    }
    else if(!((PINC >> 1) & 1))
    {
      usart_send(2);
    }
    else if(!(PINC & 1))
    {
      usart_send(1);
    }
    else
    {
      usart_send(0);
    }
    _delay_ms(100);
  }
  return 0;
}

void usart_init(void){
  UBRR0H = (uint8_t)(BAUD_PRESCALER>>8);
  UBRR0L = (uint8_t)(BAUD_PRESCALER);
  UCSR0B = (1<<RXEN0)|(1<<TXEN0);
  UCSR0C = ((1<<UCSZ00)|(1<<UCSZ01));
}

void usart_send( unsigned char data){
    while(!(UCSR0A & (1<<UDRE0))); //wait for transmit buffer
    UDR0 = data; //data to be sent
}
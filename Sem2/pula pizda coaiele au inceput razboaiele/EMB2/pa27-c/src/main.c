// Practice assignment 7, exercise 1

#include <stdio.h>
#include <avr/io.h>
#include <util/delay.h>

#include <avr/interrupt.h>

#define BAUDRATE 57600

#define BAUD_PRESCALER (((F_CPU / (BAUDRATE * 16UL))) - 1)

void usart_init(void);

void usart_send(unsigned char data);

//unsigned char usart_receive(void);

int main(void) {
  
  DDRD = 0b11110000; // set data direction for port D, all output 

  DDRC = 0x00;
  PORTC = 0b00001111;
  
  usart_init();

  UCSR0B |= (1<<RXCIE0);

  sei();
    
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
    _delay_ms(1000);
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

/*
unsigned char usart_receive(void){
  while(!(UCSR0A & (1<<RXC0))); //wait for new data
  return UDR0; //received data
  }
*/
ISR(USART_RX_vect){
  volatile unsigned char received_data = UDR0;
  if (received_data == 1) PORTD = 0b00010000;
  if (received_data == 2) PORTD = 0b00100000;
  if (received_data == 3) PORTD = 0b01000000;
  if (received_data == 4) PORTD = 0b10000000;
  if (received_data == 0) PORTD &= ~((1 << PORTD4) | (1 << PORTD5) | (1 << PORTD6) | (1 <<
    PORTD7)); 
}
    
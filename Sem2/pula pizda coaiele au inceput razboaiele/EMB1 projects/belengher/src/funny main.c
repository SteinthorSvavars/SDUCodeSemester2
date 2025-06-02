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

int main(void) {  
  uart_init(); // open the communication to the microcontroller
  io_redirect(); // redirect input and output to the communication
  DDRB =0b00100000;
  PORTB=0b00100000;
  DDRD=0b00000000;
  PORTD=0b00000000;

    
  while(1) {
    
  }
  
  return 0;
}

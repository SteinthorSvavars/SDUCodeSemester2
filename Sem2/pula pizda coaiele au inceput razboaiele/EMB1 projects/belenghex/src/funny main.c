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
  
    
  
  
  return 0;
}

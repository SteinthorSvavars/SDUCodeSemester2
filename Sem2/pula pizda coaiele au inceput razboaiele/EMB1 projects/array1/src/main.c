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

  int yarr[10];
    
    for(int i=0;i<10;i++)
        yarr[i]=i+1;
    
    for(int i=0;i<10;i++)
        printf("%d ", yarr[i]);
  
  return 0;
}

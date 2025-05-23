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
#include "HR.h"


int main(void) {  
  
  //int input;  

  uart_init(); // open the communication to the microcontroller
  HR_init();
  io_redirect(); // redirect input and output to the communication

   char res = HR_Read();
  while(1) {
		
	  printf("%s", &res);
  }
  
  return 0;
}


// task 2, hexadecimal to decimal converter

#include <stdio.h>
#include <avr/io.h>
#include <util/delay.h>

#include "usart.h"

int main(void) {  
  int input;  // create variable for storing input number

  uart_init();
  io_redirect();
  //set up serial communication

  while(1) { //start loop
	  printf("Input a hexadecimal number\n"); // print message telling user to input data
    scanf("%x", &input); // scan for input data, as decimal, store in variable "input"

    printf("The number you typed is %x is %d in decimal \n", input, input); //print out the "input" variable, as a hexadecimal number by way of a printf operator
    
	  _delay_ms(1000); // delay before running loop again
  }
  return 0;
}

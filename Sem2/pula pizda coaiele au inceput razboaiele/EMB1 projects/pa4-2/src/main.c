
// task 2, EMB1 assignment 4

#include <stdio.h>
#include <avr/io.h>
#include <util/delay.h>

#include "usart.h"

int main(void) {  

  uart_init(); // open the communication to the microcontroller
  io_redirect(); // redirect input and output to the communication

  unsigned long int factorial=1; //initialize factorial int as a 32-bit unsigned integer

  int n; //init variable to calculate factorial of
  while(1) { //start program loop
  printf("Type in number to calculate factorial of \n"); //print message for user
  scanf("%d",&n); //scan for variable

  for(int i=1;i<=n;i++) //start factorial loop
  {
    factorial*=i; //multiply factorial variable with i
  }

  printf("Factorial of %d is %lu \n",n,factorial); //print final value
  factorial=1; //reset number
  }
  return 0;
}

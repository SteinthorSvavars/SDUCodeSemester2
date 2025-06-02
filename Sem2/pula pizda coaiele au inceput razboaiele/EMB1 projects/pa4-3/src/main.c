
// task 3, EMB1 assignment 4

#include <stdio.h>
#include <avr/io.h>
#include <util/delay.h>

#include "usart.h"

int main(void) {  

  uart_init(); // open the communication to the microcontroller
  io_redirect(); // redirect input and output to the communication

  unsigned int n,i; //set up variables; n is the number being divided, i is for the for loop

  while(1) { //start program loop
    printf("Type in a number \n"); //print message for user
    scanf("%d",&n); //get number

    for(i=2;i<=n/2;i++) //start divisor loop. starts at 2 because 1 divides all numbers, and goes to n/2, because that's the largest divisor of any number
    {
      if(n%i==0) //when i divides n cleanly (the rest of the division is 0), the program prints the divisor it got
        printf("number is divisible with %d \n",i);
    }
  }
  return 0;
}

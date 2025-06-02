// Practice Assignment 8, exercise 1


#include <stdio.h>
#include <avr/io.h>
#include <util/delay.h> //init libraries

#include "usart.h"

void swap(int *uno, int *dos) // define function, with two pointers as input
{
  int tmp = *uno; //make variables for swap, copy value at first pointer's address
  *uno = *dos; //set the value in the first pointer's address to the second's value
  *dos = tmp; // finish swap
}



int main(void) {  
  uart_init(); // open the communication to the microcontroller
  io_redirect(); // redirect input and output to the communication

  int n1,n2;


  while(1) { // start program loop
    printf("First number:"); // print for user
		scanf("%d",&n1); //get first number
    printf("Second number:"); //again...
    scanf("%d",&n2);

    printf("%d and %d\n",n1,n2); //print numbers before change

    swap(&n1,&n2); // call function to swap numbers

    printf("%d and %d\n",n1,n2); //print numbers after change
  }
  
  return 0;
}

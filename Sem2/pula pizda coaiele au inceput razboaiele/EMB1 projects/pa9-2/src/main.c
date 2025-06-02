
// Practice assignment 9, exercise 2

#include <stdio.h>
#include <avr/io.h>
#include <util/delay.h>

#include "usart.h" // lib init

char get_bit(char bit, char n) // function init, takes 2 arguments and sends back another char
{
  if(n & (1 << bit)) // compare a specific bit in the number's byte to a mask, calculated by shifting a bit right
    return 1; // if the bit compared is 1, return 1
  else return 0; // or not
}

int main(void) { 

  char c,a[8],i; // initiate variables

  uart_init(); // open the communication to the microcontroller
  io_redirect(); // redirect input and output to the communication

  while(1) //start program loop
  {
    printf("Type in a number\n"); // message for user
    scanf("%hhd", &c); //memorize variable

    for(i=0;i<8;i++) //populate array with all 8 bits of the byte, *backwards* because bits are counted right to left
      a[7-i]=get_bit(i,c);

    printf("The array is: "); //msg for user

    for(i=0;i<8;i++)
      printf("%hhd ",a[i]); // print all 8 bits from the previously-calculated array

    printf("\n"); //end-line symbol, to clean up console
  }
  return 0;
}


// task 3

#include <stdio.h>
#include <avr/io.h> //init libraries
#include <util/delay.h>

#include "usart.h"

unsigned long power(int n) // same function as task 2
{
  unsigned long p=1;
  for(int i=1;i<=n;i++)
  {
    p*=2;
  }
  return p;
}

unsigned int b2d(unsigned long n) // binary to decimal func prototype
{
  unsigned int d=0; // make somewhat big variable for the result
  for(int i=0;n!=0;i++) // make for loop, set condition to be when n is zero (when all binary digits have been used)
  {
    d=d+(n%10)*power(i);// go by the algorythm for B2D, result is the sum of 2^i * the last digit in the binary number
    n=n/10; // slice last digit off binary number
  }
  return d; // return variable
}

int main(void) {  

  unsigned long input;  

  uart_init(); // open the communication to the microcontroller
  io_redirect(); // redirect input and output to the communication
    
  while(1) {
		
	  printf("The binary number is...\n"); 
    scanf("%lu", &input);
    printf("The result: %u\n", b2d(input)); //call function while printing number
    
  }
  
  return 0;
}

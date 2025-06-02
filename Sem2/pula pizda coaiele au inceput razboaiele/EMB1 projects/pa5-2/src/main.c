
// task 2


#include <stdio.h>
#include <avr/io.h> //init libraries
#include <util/delay.h>

#include "usart.h"

unsigned long power(char n) //func prototype
{
  unsigned long p=1; //make very big variable for the power, init it as 1
  for(int i=1;i<=n;i++) // loop until the number is multiplied to the right point
  {
    p*=2; //multiply by 2
  }
  return p; //return value
}

int main(void) {  

  int input; //init input variable

  uart_init(); // open the communication to the microcontroller
  io_redirect(); // redirect input and output to the communication
    
  while(1) { // start program loop
		
	  printf("The exponent is...\n"); //message for user
    scanf("%d", &input); // scan for variable
    printf("The result: %lu\n", power(input)); // print result, call function while printing
    
  }
  
  return 0;
}

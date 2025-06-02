
// task 1

#include <stdio.h>
#include <avr/io.h>
#include <util/delay.h>

#include "usart.h"

void prime(int n) // func prototype
{
  char flag=0; //make variable for flag
  for(int i=2;i<=n/2;i++) // initiate for loop, set condition to go to n/2 (I think it should reduce attempts, since there aren't gonna be any divisors past n/2)
  {
    if(n%i==0) // check divisibility
    {
      flag=1; //set flag if divisible by something
    }
  }
  if(flag) // print messages based on flag
    printf("number is not prime\n");
  else 
    printf("number is prime\n");
}

int main(void) {  

  uart_init(); // open the communication to the microcontroller
  io_redirect(); // redirect input and output to the communication
    
  while(1) { //start program loop
		int p; //make variable to check
    printf("Give me a number \n"); // message for user
    scanf("%d",&p); //scan for number
    prime(p); //call function
  }
  
  return 0;
}

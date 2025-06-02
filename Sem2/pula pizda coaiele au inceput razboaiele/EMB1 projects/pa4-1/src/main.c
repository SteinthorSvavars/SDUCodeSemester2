
// task 1, EMB1 assignment 4

#include <stdio.h>
#include <avr/io.h>
#include <util/delay.h>

#include "usart.h"

int main(void) {  

  int n, guess, c=0; // initiate variables

  uart_init(); // open the communication to the microcontroller
  io_redirect(); // redirect input and output to the communication

  printf("Type in a number to guess\n"); // message for user
  scanf("%d", &n);
    
  while(n!=guess) { // start program loop
	  printf("Guess the number\n"); // message for user
    scanf("%d", &guess); // scan for user 1's number to be guessed
    if(n>=1000) {
      printf("Number is too big! Reset to restart program")
      return 0;
    }
    if(n<guess) // check if number too big
    {
      printf("The number is too large \n"); //tell user 2 the number is too big
    }
    if(n>guess) //check if number too big
    {
      printf("The number is too small \n"); //tell user 2 the number is too small
    }
    c++; //increment guess counter
  }
  printf("You got the correct number! Number of guesses: %d",c);
  //tell user that they guessed the number correctly, and tell them how many guesses it took
  return 0; //end program
}

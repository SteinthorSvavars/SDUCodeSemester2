
// Practice assignment 9, exercise 3

#include <stdio.h>
#include <avr/io.h>
#include <util/delay.h>

#include "usart.h" //init libs

char get_bit(char bit, char n) //same as previous exercise
{
  if(n & (1 << bit))
    return 1;
  else return 0;
}

char clear_bit(char bit_number, char input_number) // function init. Returns numbers with bit cleared, done with an AND a reverse mask, made by NOT-ing 1 shifted by a number
{
  return (input_number &(~(1<<bit_number)));
}

char set_bit(char bit_number, char input_number) // Returns numbers with bit set with a mask and a OR
{
  return (input_number |(1<<bit_number));
}


int main(void) {  

  char c,BitToSet,BitToClear,i; // initiate variables

  uart_init(); // open the communication to the microcontroller
  io_redirect(); // redirect input and output to the communication

  while(1)
  {
    printf("Type in a number\n"); // message for user
    scanf("%hhd", &c); //get number to mess with

    for(i=7;i>=0;i--)
      printf("%hhd ",get_bit(i,c)); // print byte before any changes, *backwards* because MSB is at the left of the number

    printf("\nType in a bit to clear\n"); // message for user
    scanf("%hhd", &BitToClear);//get bit to clear

    c=clear_bit(BitToClear-1,c); //clear the bit by calling the function

    for(i=7;i>=0;i--)
      printf("%hhd ",get_bit(i,c)); //print byte afterwards

    printf("\nType in a bit to set\n"); // message for user (has an endline at the beginning because the previous line does not)
    scanf("%hhd", &BitToSet); //get bit to set

    c=set_bit(BitToSet-1,c); //print byte afterwards

    for(i=7;i>=0;i--)
      printf("%hhd ",get_bit(i,c)); //print byte afterwards

    printf("\n"); // one final end of line for formatting reasons
  }
}

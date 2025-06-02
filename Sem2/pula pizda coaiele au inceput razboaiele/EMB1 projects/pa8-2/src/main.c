// Practice Assignment 8, exercise 2


#include <stdio.h>
#include <avr/io.h>
#include <util/delay.h>
#include <stdbool.h>

#include "usart.h" // init libraries

void swap(int *uno, int *dos) // swap function from previous exercise
{
  int tmp = *uno;
  *uno = *dos;
  *dos = tmp;
}



int main(void) { //start program loop 
  uart_init(); // open the communication to the microcontroller
  io_redirect(); // redirect input and output to the communication

  int i,j; //variables for for loops

  int a[10]; //make arrays

  bool swapped; // make "boolean" to memorize if number is swapped


  while(1) { // start program loop

    for(i=0;i<=9;i++) { // read numbers for arrays
      printf("number %d:",i+1); //print msg for user
		  scanf("%d",&a[i]); // scan for input
    }

    for(i=0;i<9;i++) //start sort, looking from 0 to n-1 numbers (n=10)
    {
      swapped=false; // set boolean to false. Before checking all remaining numbers, set a variable that memorizes if the row has been scanned and doesn't need to be sorted
      for(j=0;j<9-i;j++) //check remaining pairs of numbers to see if they need to be swapped
        if(a[j]>a[j+1]) {
          swap(&a[j],&a[j+1]); //if yes, swap them
          swapped=true; //set flag
        }

        if(!swapped) // if the array doesn't need sorting anymore, break loop
          break;
    }

    for(i=0;i<=9;i++)
      printf("%d ",a[i]); // print remaining numbers

    printf(" \n"); //end line to prepare console for next run
  }
  
  return 0;
}

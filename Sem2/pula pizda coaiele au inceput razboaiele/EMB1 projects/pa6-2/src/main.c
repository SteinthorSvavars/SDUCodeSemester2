// Practice Assignment 6, exercise 2


#include <stdio.h>
#include <avr/io.h>
#include <util/delay.h>

#include "usart.h"

int convert(int c) // Celsius to Fahrenheit function
{
  double f; // Create variable for Fahrenheit, double-precision floating point bc of accuracy
  f=((double)c*9/5)+32; // calculate Fahrenheit temp, casting the integer celsius to a double
  return f; // return Fahrenheit value, which gets converted back to an integer
}

int main(void) {  

  int i,minC,maxC,sumC,avgC; // create variables

  int temp_celsius[7]; // create celsius array
  int temp_fahrenheit[7]; // create fahrenheit array
  maxC=-100;minC=100;sumC=0; // assign numbers to variables. -100 and 100 are enough because 32767 would be hotter than the surface of the sun

  uart_init(); // open the communication to the microcontroller
  io_redirect(); // redirect input and output to the communication

    
  while(1) { // start program loop
		
	  for(i=0;i<=6;i++) //first for loop, for getting numbers and setting minimum/maximum/sum
    {
      printf("Type in temperature for day %d\n",i+1); //Print message for user

      scanf("%d", &temp_celsius[i]); //scan for input

      if(temp_celsius[i]>maxC)
        maxC=temp_celsius[i]; // set maximum temperature

      if(temp_celsius[i]<minC)
        minC=temp_celsius[i]; // set minimum temperature

      sumC+=temp_celsius[i]; // calculate sum of temperatures, for average
    }
    avgC=sumC/7; // calculate average

    for(i=0;i<=6;i++) // second loop, for calculating temps in Fahrenehit
    {
      temp_fahrenheit[i]=convert(temp_celsius[i]); // call function to set temps in Fahrenheit array
    }
    
    for(i=0;i<=6;i++) // third loop, for printing
    {
      printf("The temperature for day %d was %d C\n",i+1,temp_celsius[i]); // display temperature for every day
      printf("The temperature for day %d was %d F\n",i+1,temp_fahrenheit[i]); // display Fahrenheit temperature for every day
    }

    printf("The maximum temperature was %d C\n",maxC); // display maximum temp

    printf("The minimum temperature was %d C\n",minC); // display minimum temp

    printf("The average temperature was %d C\n",avgC); // display average temp
    
  }
  
  return 0;
}

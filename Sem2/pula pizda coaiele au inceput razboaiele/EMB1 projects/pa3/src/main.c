/*
 * HelloWorld.c
 *
 * Created: 11/9/2023 10:43:27 AM
 * Author : Alin
 */ 

#include <stdio.h>
#include <avr/io.h>
#include <util/delay.h>

#include "usart.h"

int main(void) {  

  int curT, minT, maxT, sumT, counter, avgT; // init variables

  sumT=0; // init the sum to something neutral

  minT=INT16_MAX; //init the minimum to the maximum int16 value, as such any number will be lesser than it on first run
  maxT=INT16_MIN; //same comment as above but backwards

  counter=1; //init counter as 1, to avoid a divide by zero on first run

  uart_init(); // open the communication to the microcontroller
  io_redirect(); // redirect input and output to the communication
    
  while(1) { // start program loop
	  printf("Input a temperature\n"); // message for user
    scanf("%d", &curT); // scan for new temperature

    sumT+=curT; // compute the sum of all recorded temperatures
    avgT=sumT/counter; // compute the average of all recorded temperatures, as the sum divided by counter

    if(curT>maxT) // if the current temp is greater than the current max, register it as the new max
      maxT=curT;
    
    if(curT<minT) // same as above but backwards
      minT=curT;
    
    printf("Current temperature is %d, max temperature is %d, min temperature is %d, avg temperature is %d \n", curT, maxT, minT, avgT);
    //print out all important values
    counter+=1; // increment counter
	  _delay_ms(1000);
  }
  
  return 0;
}

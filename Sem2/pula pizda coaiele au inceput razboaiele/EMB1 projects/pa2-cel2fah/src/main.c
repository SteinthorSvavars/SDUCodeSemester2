
// task 3, celsius to fahrenheit converter

#include <stdio.h>
#include <avr/io.h> // set up libraries 
#include <util/delay.h>

#include "usart.h"

int main(void) {  
  int degCelsius,degUSA; // set up variables

  uart_init();
  io_redirect();
  //set up serial comms
    
  while(1) { //start program loop
	  printf("Input a temperature in Celsius\n"); //print message for user
    scanf("%d", &degCelsius); // scan for input temperature

    degUSA = (degCelsius * 9 / 5) + 32; //calculate temp in fahrenheit

    printf("%d in Celsius is %d in Fahrenheit \n", degCelsius, degUSA); //print resulting temperature

	  _delay_ms(1000); //delay before re-running loop
  }
  return 0;
}

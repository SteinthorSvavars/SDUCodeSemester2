
#include <stdio.h>

#include <util/delay.h>
#include <avr/io.h>
#include <math.h>
#include "usart.h"
#include "adcpwm.h"


int main(void)
{ 
  
  uart_init(); 
  io_redirect();
  adc_init(); // initialize the ADC module

  DDRC = 0b11110000; // configure pins PC0 to PC3 as inputs
  PORTC = 0b00110000; // configure pins PC0 to PC3 to not use pullups for the ADC

  unsigned int adc_value;

  while(1)
  {
    
    adc_value = adc_read(0); // Value 0-1023 representing analog voltage on pin PC0
    printf("Result of the ADC conversion : %u\n", adc_value); 

    
  }
}


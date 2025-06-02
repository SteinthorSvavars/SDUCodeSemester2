// Practice assignment 8a

#include <stdio.h>
#include <avr/io.h>
#include <util/delay.h>

#include "i2cmaster.h"
#include "lcd.h"

float adc_read(uint8_t adc_channel); 

int main(void) {  

  i2c_init();
  LCD_init();

  uint16_t adc_result,i;

  ADMUX = (1<<REFS0); // Select Vref = AVcc

  ADCSRA = (1<<ADPS2)|(1<<ADPS1)|(1<<ADPS0)|(1<<ADEN); //set prescaler to 128 and turn on the ADC module
  
  DDRC = 0xF0; // set data direction for port C pins, 0-3 as input (i.e. the buttons)
  PORTC = 0x30; // set pull-up resistor for port C
  DDRD = 0xFF; // set data direction for port D, all output 
  PORTD= 0x00; // set output for port D (none)
    
  while(1) { // start program loop
    for(i=0;i<4;i++)
    {
      adc_result = adc_read(i);
      LCD_set_cursor(0,i);
      printf("ADC %hhd: %4.0f mV", i,  adc_result*4.8828125);
    }
  }
  return 0;
}

float adc_read(uint8_t adc_channel){
  ADMUX &= 0xf0; // clear any previously used channel, but keep internal reference

  ADMUX |= adc_channel; // set the desired channel 

  ADCSRA |= (1<<ADSC); //start a conversion

  while ( (ADCSRA & (1<<ADSC)) ); // now wait for the conversion to complete
    // now we have the result, so we return it to the calling function as a 16 bit unsigned int

  return ADC;
}

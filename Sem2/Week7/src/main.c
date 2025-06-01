/*
 * Author : Steinthor
 */ 



#include <stdio.h>
#include <stdlib.h>

#include <avr/io.h>
#include <avr/eeprom.h>
#include <util/delay.h>

/* I2C coms */
#include "i2cmaster.h"
/* LCD */ 
#include "lcd.h"
/* Temp sens */
#include "lm75.h"

//#include "twimaster.c"
#include "usart.h"

void delay_ms(uint16_t Milliseconds);
void delay_hs(uint16_t HundredMilliseconds);

int main(void) {  

  uart_init();
  io_redirect();

  //Setting Data direction register for port D as well as pullups
  DDRD = 0xFF;
  DDRD &= ~(1<<DDD4);
  PORTD |= (1<<PORTD4);

  //Turn on the counter0 to count on external rise on port D4
  TCCR0B |= (1<<CS02)|(1<<CS01)|(1<<CS00);

  while(1){
    delay_hs(10);
    printf("%d\n", TCNT0);

    //500ms Delay + PB5 LED Flashing 
    PORTB = 0x20;
    delay_ms(500);
    PORTB = 0x00;
    delay_ms(500);
    /*
    //1s Delay + PB5 LED Flashing
    PORTB = 0x20;
    delay_hs(10);
    PORTB = 0x00;
    delay_hs(10);*/
    /*
    //2s Delay + PB5 LED Flashing
    PORTB = 0x20;
    delay_hs(20);
    PORTB = 0x00;
    delay_hs(20);*/
  }
  return 0;
}

//8-bit Clock for millisecond delay using time register 0
void delay_ms(uint16_t Milliseconds){
  //Set Mode to CTC
  TCCR0A |= (1<<WGM01);

  //Top- set to 1 ms with this: 
  //Since 16MHz / 64 =  250 KHz : 1/250KHz = 4μs : 4μs *250 ticks = 1ms : max size of 8bit is 255
  OCR0A = 249;

  //Setting Prescaler to 64
  TCCR0B |= (1<<CS01)|(1<<CS00);

  for(uint16_t i = 0; i < Milliseconds; i++){
    //Delay Start + Waiting for Overflow flag
    while((TIFR0 & (1<<OCF0A)) == 0){
      }
      //Flag Reset at Overflow
      TIFR0 = (1<<OCF0A);
    
  }
}
//16 bit Counter for Hundred millisecond delay using timer register
void delay_hs(uint16_t HundredMilliseconds){
  //Set Mode to CTC
  TCCR1B |= (1<<WGM12);
  
  //To Determine to Delay to 100 ms 
  // - it should be 100 since the timer resoltionis 16MHz/64 = 250KHz  1/250KHz=4μs and 4μs * 25000 ticks = 100 ms : Max size of 16bit is65535
  OCR1A = 24999;

  //Setting Prescaler to 64
  TCCR1B |= (1<<CS11)|(1<<CS10);
  for(uint16_t i = 0; i < HundredMilliseconds; i++){
    //Delay Start Waiting for Overflow flag
    while((TIFR1 & (1<<OCF1A)) == 0){
    }
      //Flag Reset at Overflow
      TIFR1 = (1<<OCF1A);
    
  }
}
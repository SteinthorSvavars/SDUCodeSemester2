/*
 * HelloWorld.c
 *
 * Created: 11/9/2023 10:43:27 AM
 * Author : Alin
 */ 



#include <stdio.h>
#include <avr/io.h>
//#include <util/delay.h>
#include <avr/interrupt.h>

//#include "usart.h"

void init_timer1(void);

/* Variables */
volatile int MsCounter = 0; 

int main(void) {  

  /* Config for inputs and outputs */
  DDRB |= (1<<PB5); 
  init_timer1();
    
  while(1) {

  }
  
  return 0;
}

void init_timer1(void){
  /* Set timer mode to CTC */
  TCCR1B |= (1<<WGM12);
  /* Set the value that you want to count to */
  OCR1A = 0xF9; /* 249, with a prescaler of 64 this gives 1ms */
  TIMSK1 |= (1<<OCIE1A); /* Set the ISR COMPA vect */
  sei(); /* Enable interrupt */ 
  TCCR1B |= (1<<CS11)|(1<<CS10); /* Set the prescaler to 64 and start the timer */
}

ISC(TIMER1_COMPA_vect){
  if(MsCounter == 1000){
    PORTB ^= (1<<PB5); /* XOR */
    MsCounter = 0;
  }
  else{
    MsCounter++;
  }
}

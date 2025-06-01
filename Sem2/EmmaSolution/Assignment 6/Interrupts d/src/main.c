/*
 * HelloWorld.c
 *
 * Created: 11/9/2023 10:43:27 AM
 * Author : Alin
 */ 



#include <stdio.h>
#include <avr/io.h>
#include <avr/interrupt.h>

#include "usart.h"

volatile unsigned int ovf_counter=0; // set up overflow counting

void count_trans()
{
  float period; //make variable for period calc
  TCNT0=0;
  ovf_counter=0; //clear previous run
  TCCR0B |= (1 << CS02) | (1 << CS01) | (1 << CS00); //counter mode timer 0
  TIMSK0 |= (1 << TOIE0); //trigger by PD4
  sei(); // start interrupt-ing
  for(unsigned int i=0;i<1000;i++) {
    TCCR2A |= (1 << WGM21);
    OCR2A = 249;
    TCCR2B |= (1 << CS21) | (1 << CS20);
    while ( (TIFR2 & (1 << OCF2A) ) == 0) {}
    TIFR2 = (1 << OCF2A);
  } // 1 sec timer
  printf("Hz: %hhd\n",ovf_counter*256+2*TCNT0); // calculate hertz by just multiplying counts
  period=1/(float)TCNT0;
  printf("%f\n",period); // T=1/freq
}

int main(void) {  

  DDRC = 0xF0;
  PORTC = 0x3F;
  DDRD = 0b11101011;
  PORTD= 0b00000100;

  //register setup

  uart_init();
  io_redirect(); //init serial

  while(1) {
    count_trans(); // count
  }

  return 0;
}

ISR (TIMER2_OVF_vect)
{
  ovf_counter++; 
}



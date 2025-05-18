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
#include "i2cmaster.h"

unsigned char x, y, z1, z2; //creating the variables
unsigned char q0, q1, q2, q0_next, q1_next, q2_next;

void read_xy_values(void); //checking which button is pressed and overriding the global variables x and y depending on which button is pressed
void show_output(void); //printing the state to the serial monitor
void state_transition(void); //changing states depending in the input


int main(void) {  

  uart_init(); // open the communication to the microcontroller
  io_redirect(); // redirect input and output to the communication
  i2c_init(); //initializing the communication protocoll

  DDRC = 0x30; //Buttons are inputs but 5 and 4 are for i2c
  PORTC = 0x3F; //setting the pullup resistors high for the buttons so we can check if they're pressed

  DDRD = 0xF0; //setting the registers as outputs for the LEDS
  PORTD = 0x00; // turning the leds off

  q0 = 0; //setting the inputs initially to zero
  q1 = 0;
  q2 = 0;

  while(1) {
    read_xy_values(); //calling the xy_read value function
    state_transition(); //calling the state_transition function

    q0 = q0_next; //setting the current state equal to the next state, so the next state would become the current
    q1 = q1_next;
    q2 = q2_next;

    show_output(); //calling the show_output function
    _delay_ms(1000);
  }
  return 0;
}
void read_xy_values(void){ //this function overrides the global variables x and y depending on which button is pressed
  if (!(PINC & (1 << PC6)) && !(PINC & (1 << 7))){
    x=0;
    y=0;
  }
  if (!(PINC & (1 << PC2))){
    x=1;
    y=0;
  }
  if (!(PINC & (1 << PC3))){
    x=0;
    y=1;
  }
  if (!(PINC & (1 << PC2)) && !(PINC & (1 << PC3))){
    x=1;
    y=1;
  }
}
void show_output(void){ //this function prints to the serial monitor, it prints the value of x and y and the current state
  char disp = ' ';
  printf("\n-------------------");
  printf("\nx: %d", x);
  printf("\ny: %d", y);

  if (q2==0 && q1==0 && q0 ==0){
    disp = 'A';
  }
  if (q2==0 && q1==0 && q0 ==1){
    disp = 'B';
  }
  if (q2==0 && q1==1 && q0 ==0){
    disp = 'C';
  }
  if (q2==0 && q1==1 && q0 ==1){
    disp = 'D';
  }
  if (q2==1 && q1==0 && q0 ==0){
    disp = 'E';
  }
  if (q2==1 && q1==0 && q0 ==1){
    disp = 'F';
  }
  if (q2==1 && q1==1 && q0 ==0){
    disp = 'G';
  }
  if (q2==1 && q1==1 && q0 ==1){
    disp = 'H';
  }

  z2 = (q2&&q1)||(q2&&(!q0));
  z1 = (!q0)||(!q1)||(q2);

  printf("\nq0*: %d", q0_next);
  printf("\nq1*: %d", q1_next);
  printf("\nq2*: %d", q2_next);
  printf("\n\nz1: %d\nz2: %d", z1,z2);
  printf("\nstate: %c", disp);
}



void state_transition(void){ //this function switches between states, depending on the x and y inputs
  PORTD=0x20;
  q0_next = ((!q1) && x) || (q0 && (!x)) || (q2); //transition equations
  q1_next = ((!q2) && q0 && x) || (q1 && (!x)) || (q2 && q1);
  q2_next = (!q0) && (q2 || ((!x) && y));
  _delay_ms(100);
  PORTD=0x00; //flashes the led
}



// Practice assignment 3, version 2

#include <stdio.h>
#include <avr/io.h>
#include <util/delay.h>

#include <i2cmaster.h>
#include <lcd.h>

unsigned char x, y, z1, z2; // inputs and outputs

void read_xy() {
  if(!((PINC >> 2) & 1)) { //repeat
      x=1;
    }
    else {
      x=0;
    }
      
    if(!((PINC >> 3) & 1)) { //repeat
      y=1;
    }
    else {
      y=0;
    }
}

char stateChar;

typedef enum {
  A_state,
  B_state,
  C_state,
  D_state,
  E_state,
  F_state,
  G_state,
  H_state
}state;

void show_output(state state_to_print)
{
  LCD_set_cursor(0,0);
  printf("%hhu %hhu ",x,y);
  LCD_set_cursor(0,1);
  switch(state_to_print){
    case A_state: printf("A_STATE"); break;
    case B_state: printf("B_STATE"); break;
    case C_state: printf("C_STATE"); break;
    case D_state: printf("D_STATE"); break;
    case E_state: printf("E_STATE"); break;
    case F_state: printf("F_STATE"); break;
    case G_state: printf("G_STATE"); break;
    case H_state: printf("H_STATE"); break;
  }
  LCD_set_cursor(0,2);
  printf("%hhu %hhu ",z1,z2);
}


state state_transition(state current_state)
{
  switch (current_state) {
    case (A_state):
      if((!x)&&(!y)) 
        return A_state;
      if(x)
        return B_state;
      if((!x)&&y)
        return E_state;
    case (B_state):
      if((!x))
        return B_state;
      if(x)
        return D_state;
    case (C_state):
      if(x)
        return A_state;
      if((!x)&&y)
        return G_state;
      if((!x)&&(!y))
        return C_state;
    case (D_state):
      if((!x))
        return D_state;
      if(x)
        return C_state;
    case (E_state):
      return F_state;
    case (F_state):
      return B_state;
    case (G_state):
      return H_state;
    case (H_state):
      return D_state;
 }
 
}

void state_z(state current_state)
{
  switch (current_state) {
    case (A_state):
      z1=1;z2=0;break;
    case (B_state):
      z1=1;z2=0;break;
    case (C_state):
      z1=1;z2=0;break;
    case (D_state):
      z1=0;z2=0;break;
    case (E_state):
      z1=1;z2=1;break;
    case (F_state):
      z1=1;z2=0;break;
    case (G_state):
      z1=1;z2=1;break;
    case (H_state):
      z1=1;z2=1;break;
  }
  
}

#include "i2cmaster.h"
#include "lcd.h" //library init

int main(void) {  

  i2c_init(); // initialize I2C and LCD
  LCD_init();

  state current_state, next_state;

  current_state=A_state;

  DDRB=0b00100000;
  PORTB=0b00000000;
  
  DDRC = 0xF0; // set data direction for port C pins, 0-3 as input (i.e. the buttons)
  PORTC = 0x3F; // set pull-up resistor for port C
  DDRD = 0xFF; // set data direction for port D, all output 
  PORTD= 0x00; // set output for port D (none)
    
  while(1) { // start program loop
    read_xy();
    next_state=state_transition(current_state);
    current_state=next_state;
    state_z(current_state);
    show_output(current_state);
    
    _delay_ms(1000);
  }

  return 0;
}

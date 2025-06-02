// Practice assignment 3, version 1

#include <stdio.h>
#include <avr/io.h>
#include <util/delay.h>

#include <i2cmaster.h>
#include <lcd.h>

unsigned char x, y, z1, z2; // inputs and outputs
unsigned char q0, q1, q2, q0_next, q1_next, q2_next;

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

void show_output()
{
  LCD_set_cursor(0,0);
  printf("%hhu %hhu ",x,y);
  LCD_set_cursor(0,1);
  printf("%hhu %hhu ",z1,z2);
  LCD_set_cursor(0,2);
  printf("%hhu %hhu %hhu",q2,q1,q0);
  LCD_set_cursor(0,3);
  printf("%hhu %hhu %hhu",q2_next,q1_next,q0_next);

  stateChar=(0b01000000|(q2<<2)|(q1<<1)|q0)+1;
  LCD_set_cursor(19,0);
  printf("%c",stateChar);
}

void state_transition()
{
  PORTB |= (1 << PINB5);
  _delay_ms(500);
  PORTB &= ~(1 << PINB5);
}

#include "i2cmaster.h"
#include "lcd.h" //library init

int main(void) {  

  i2c_init(); // initialize I2C and LCD
  LCD_init();

  q1=0;q0=0;q2=0;
  DDRB=0b00100000;
  PORTB=0b00000000;
  
  DDRC = 0xF0; // set data direction for port C pins, 0-3 as input (i.e. the buttons)
  PORTC = 0x3F; // set pull-up resistor for port C
  DDRD = 0xFF; // set data direction for port D, all output 
  PORTD= 0x00; // set output for port D (none)
    
  while(1) { // start program loop
    q0_next=((!q1)&x)|(q0&(!x))|q2;
    q1_next=((!q2)&x&q0)|(q1&(!x))|(q2&q1);
    q2_next=((!q0)&q2)|((!q0)&(!x)&y);

    show_output();
    
    q0=q0_next;
    q1=q1_next;
    q2=q2_next;

    z1=(!q0)|(!q1)|q2;
    z2=(q1&q2)|((!q0)&q2);
    read_xy();
    state_transition();
    
    _delay_ms(500);
  }

  return 0;
}

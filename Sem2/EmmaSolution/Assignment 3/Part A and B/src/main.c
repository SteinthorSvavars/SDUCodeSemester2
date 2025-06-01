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

unsigned char x, y, z1, z2;
unsigned char q0, q1, q2, q0_next, q1_next, q2_next;

void read_xy(void);
void show_output(void);
void state_transistion(void);


int main(void) {  

  uart_init(); // open the communication to the microcontroller
  io_redirect(); // redirect input and output to the communication
  i2c_init();

  DDRC = 0b00110000;
  PORTC = 0b00111111;
  DDRD = 0b11110000;
  PORTD = 0b00000000;

  q0 = 0;
  q1 = 0;
  q2 = 0;
  q0_next = 0;
  q1_next = 0;
  q2_next = 0;

    
  while(1) 
  {
    read_xy();
    show_output();
    state_transistion();
    q0 = q0_next;
    q1 = q1_next;
    q2 = q2_next;
    
    _delay_ms(1000);
  }
  return 0;
}

void read_xy(void)
{
  if (!(PINC & (1<<PC6)) && !(PINC & (1 <<7)))
  {
    x = 0;
    y = 0;
  }
  if (!(PINC & (1<<PC2)))
  {
    x = 1;
    y = 0;
  }
  if(!(PINC & (1<<PC3)))
  {
    x = 0;
    y = 1;
  }
  if(!(PINC & (1 << PC2)) && !(PINC & (1 << PC3)))
  {
    x = 1;
    y = 1;
  }
   
}

void show_output(void)
{
  char display = ' ';
  printf("x = %d", x);
  printf("y = %d\n", y);

  if (q0 == 0 && q1 == 0 && q2 == 0)
  {
    display = 'A';
  }
  if (q0 == 1 && q1 == 0 && q2 == 0)
  {
    display = 'B';
  }
  if (q0 == 0 && q1 == 1 && q2 == 0)
  {
    display = 'C';
  }
  if (q0 == 1 && q1 == 1 && q2 == 0)
  {
    display = 'D';
  }
  if (q0 == 0 && q1 == 0 && q2 == 1)
  {
    display = 'E';
  }
  if (q0 == 1 && q1 == 0 && q2 == 1)
  {
    display = 'F';
  }
  if (q0 == 0 && q1 == 1 && q2 == 1)
  {
    display = 'G';
  }
  if (q0 == 1 && q1 == 1 && q2 == 1)
  {
    display = 'H';
  }

  z1 = (!q0)||(!q1)||(q2);
  z2 = (q2&&q1)||(q2&&(!q0));

  printf("q0_next= %d\n", q0_next);
  printf("q1_next= %d\n", q1_next);
  printf("q2_next= %d\n", q2_next);
  printf("z1 = %d z2 = %d\n", z1, z2);
  printf("state = %c", display);
  printf("-------------\n");
}

void state_transistion(void)
{
  PORTD = 0b00010000;
  q0_next = ((!q1) && x) || (q0 && (!x)) || (q2);
  q1_next = ((!q2) && q0 && x) || (q1 && (!x)) || (q2 && q1);
  q2_next = (!q0) && ((q2) || ((!x) && y));

  PORTD = 0b00000000;
}
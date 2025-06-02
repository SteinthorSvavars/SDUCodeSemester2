// Practice assignment 7, exercise 1

#include <stdio.h>
#include <avr/io.h>
#include <util/delay.h>

#include "i2cmaster.h"
#include "lcd.h" //library init



typedef struct {
  unsigned int hours;
  unsigned char minutes;
  unsigned char seconds;
  unsigned char hund_sec;
} time_t;

time_t cur_time,split_time;

void reset_time () {
  cur_time.hours=0;
  cur_time.minutes=0;
  cur_time.seconds=0;
  cur_time.hund_sec=0;
}

void print_time (unsigned char line, time_t time)
{
  LCD_set_cursor(3,line);
  printf("%u:%02hhu:%02hhu.%02hhu    ",time.hours,time.minutes,time.seconds,time.hund_sec);
}

void count_time() {
  _delay_ms(69);
  cur_time.hund_sec+=10;
  if(cur_time.hund_sec>=100)
  {
    cur_time.hund_sec-=100;
    cur_time.seconds++;
  }
  if(cur_time.seconds>=60)
  {
    cur_time.seconds-=60;
    cur_time.minutes++;
  }
  if(cur_time.minutes>=60)
  {
    cur_time.minutes-=60;
    cur_time.hours++;
  }
}

int main(void) {
  reset_time;
  DDRC = 0x30;
  PORTC = 0x3F;
  DDRD = 0xFF;
  PORTD= 0x00;

  i2c_init(); // initialize I2C and LCD
  LCD_init();
  
  unsigned char prog_state=0;
    
  while(1) { // start program loop
    
    if(!(PINC & 1)) { 
      prog_state=1;
    }

    if(!((PINC >> 1) & 1)) {
      prog_state=0;
    }

    if(!((PINC >> 2) & 1)) { //repeat
      split_time=cur_time;
      print_time(1,split_time);
    }
      
    if(!((PINC >> 3) & 1)) { //repeat
      reset_time();
      LCD_clear();
    }

    if(prog_state==1) {
      count_time();
      print_time(0,cur_time);
      LCD_set_cursor(0,3);
      printf("r");
    }
    else
    {
      LCD_set_cursor(0,3);
      printf("s");
    }
    //Setting the LCD after the LED's means multiple LED's can be on at the same time, without the LCD flickering between different messages. Last button gets priority on the LCD

    /*if(!((PINC >> 3) & 1)) // set LCD, same way to read pin as before
    {
      LCD_set_cursor(0,0);
      printf("DI3 pressed");
    }
    else if(!((PINC >> 2) & 1))
    {
      LCD_set_cursor(0,0);
      printf("DI2 pressed");
    }
    else if(!((PINC >> 1) & 1))
    {
      LCD_set_cursor(0,0);
      printf("DI1 pressed");
    }
    else if(!(PINC & 1))
    {
      LCD_set_cursor(0,0);
      printf("DI0 pressed");
      }
    else // clear LCD by sending it spaces
    {
      LCD_set_cursor(0,0);
      printf("           ");
    }*/
  }

  return 0;
}

/*
 slotduino machine, SDU EMB1 january exam, 14/01/2025, E24

 should fully work, hopefully
 */ 

#include <stdio.h>
#include <avr/io.h>
#include <util/delay.h>

#include "i2cmaster.h"
#include "lcd.h"

unsigned char game_results[3],wheel_index;

void led_cycle(unsigned char stage) //task c and d
{
  PORTD=0x00;

  unsigned char cur_led=4;

  while(!(PINC & 1)) {
    PORTD |= (1 << cur_led);
    _delay_ms(100);
    PORTD &= ~(1 << cur_led);
    game_results[stage]=cur_led-3;
    if(!(PINC & 1)) {
      cur_led++;
      if(cur_led>7) {
        cur_led=4;
      }
    }
    else {
      PORTD |= (1 << cur_led);
      break;
    }
  }
}

void display_results() // task d
{
  LCD_set_cursor(0,0);
  for(unsigned char i=0;i<3;i++)
    if(game_results[i]!=0)
        printf("%hhu", game_results[i]);
}

void calculate_results(unsigned char results[3], int *balance) //task e
{
  if((results[0]==results[1])&&(results[1]==results[2]))
    *balance+=10*results[0];

  if((results[0]==1&&results[1]==2&&results[2]==3)
  ||(results[0]==2&&results[1]==3&&results[2]==4)
  ||(results[0]==3&&results[1]==2&&results[2]==1)
  ||(results[0]==4&&results[1]==3&&results[2]==2))
    *balance+=20;
}

int main(void) {

  i2c_init();
  LCD_init();

  int money=50;

  DDRC = 0xF0;
  PORTC = 0x0F;
  DDRD = 0xFF;
  PORTD = 0x00;
    
  while(1) {

		if(!(PINC & 1)) {

        PORTD = 0x00;
        LCD_clear();
        wheel_index=0;

        for(unsigned char i=0;i<3;i++)
          game_results[i]=0;

        LCD_set_cursor(15,3); //task b
        printf("%d", money);
        //task a
        while(wheel_index==0)
        {
          if(!(PINC & 1))
          {
            while(!(PINC & 1))
              {
                LCD_set_cursor(19,0);
                printf("%hhu", wheel_index);
                led_cycle(0);
              }
            break;
          }
        }

        display_results();
        wheel_index++;

        while(wheel_index==1)
        {
          if(!(PINC & 1))
          {
            while(!(PINC & 1))
              {
                LCD_set_cursor(19,0);
                printf("%hhu", wheel_index);
                led_cycle(1);
              }
            break;
          }
        }

        display_results();
        wheel_index++;

        while(wheel_index==2)
        {
          if(!(PINC & 1))
          {
            while(!(PINC & 1))
              {
                LCD_set_cursor(19,0);
                printf("%hhu", wheel_index);
                led_cycle(2);
              }
            break;
          }
        }

        display_results();
        wheel_index=0;

        money-=10;
        calculate_results(game_results,&money);
    }
  }
  return 0;
}

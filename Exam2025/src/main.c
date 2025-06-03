/*
 * HelloWorld.c
 *
 * Created: 11/9/2023 10:43:27 AM
 * Author : Alin
 */ 



#include <stdio.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>


#include "i2cmaster.h"
#include "lcd.h"

#define BAUDRATE 38400
#define BAUD_PRESCALER (((F_CPU / (BAUDRATE * 16UL))) - 1)

void usart_init(void);

volatile int timer=0; // make variable for timer
float adc_read(uint8_t adc_channel); 
void usart_send(unsigned char data);
int num = 0;
int place = 0;
int count = 0;
int teljari = 0;

void init_interrupts() // set up timer
{
  TCCR2A |= (1 << WGM21);
  OCR2A = 0xF9;
  TIMSK2 |= (1 << OCIE2A);
  sei();
  TCCR2B |= (1 << CS21) | (1 << CS20);
}


int main(void) {  

  DDRC = 0xF0; // set data direction for port C pins, 0-3 as input (i.e. the buttons)
  PORTC = 0x30; // set pull-up resistor for port C
  DDRD = 0xFF; // set data direction for port D, all output 
  PORTD= 0x00; // set output for port D (none)
  DDRB |= (1 << PB5);

  i2c_init(); // initialize I2C and LCD
  LCD_init();
  LCD_clear();
  //usart_init();
  init_interrupts();
  usart_init();
  ADMUX = (1<<REFS0); // Select Vref = AVcc
  ADCSRA = (1<<ADPS2)|(1<<ADPS1)|(1<<ADPS0)|(1<<ADEN); //set prescaler to 128 and turn on the ADC module

  UCSR0B |= (1<<RXCIE0);

  while(1) {
    /*
    //for(i=0;i<4;i++)
    //{
      LCD_set_cursor(0,1);
      printf("%d",teljari);
      //LCD_set_cursor(0,1);
      //printf("ADC %hhd: %4.0f mV", 0,  adc_result*4.8828125);
    //}
    */
  }
    return 0;
}

ISR(USART_RX_vect){
  volatile unsigned char received_data = UDR0;
  if (received_data == 1) {PORTB |= (1 << PB5); count = 1;} //place++; //PORTD = 0b00010000;
  if (received_data == 2) {PORTB &= ~(1 << PB5); count = 0;}
}

void usart_send( unsigned char data){
  while(!(UCSR0A & (1<<UDRE0))); //wait for transmit buffer
  UDR0 = data; //data to be sent
}

ISR(TIMER2_COMPA_vect) // ISR for the timer ticking
{
  timer++;
  if(timer==2000) {
    num++;
    LCD_set_cursor(0,0);
    printf("%d",num);
    if(num >= 10 && num < 100){
      LCD_set_cursor(2,0);
    }
    else if(num > 100){
      LCD_set_cursor(3,0);
    }
    else{
      LCD_set_cursor(1,0);
    }
    printf("s");
    if(count == 1){
      place++;
    }
    else{
      count = count;
    }
    LCD_set_cursor(8,0);
    printf("%d",place);
    if(place < 20){
      LCD_set_cursor(place,1);
      printf("R");
    }
    if(place > 0){
      LCD_set_cursor(place-1,1);
      printf(" ");
    }
    uint16_t adc_result;
    adc_result = adc_read(0);
    if((adc_result*4.8828125)/1000 > 3.3) // send , same way to read pin as before
      {
        usart_send(place);
        adc_result = 0;
      }
    timer=0;
  }
}

void usart_init(void){
  UBRR0H = (uint8_t)(BAUD_PRESCALER>>8);
  UBRR0L = (uint8_t)(BAUD_PRESCALER);
  UCSR0B = (1<<RXEN0)|(1<<TXEN0);
  UCSR0C = ((1<<UCSZ00)|(1<<UCSZ01));
}

float adc_read(uint8_t adc_channel){
  ADMUX &= 0xf0; // clear any previously used channel, but keep internal reference

  ADMUX |= adc_channel; // set the desired channel 

  ADCSRA |= (1<<ADSC); //start a conversion

  while ( (ADCSRA & (1<<ADSC)) ); // now wait for the conversion to complete
    // now we have the result, so we return it to the calling function as a 16 bit unsigned int

  return ADC;
}

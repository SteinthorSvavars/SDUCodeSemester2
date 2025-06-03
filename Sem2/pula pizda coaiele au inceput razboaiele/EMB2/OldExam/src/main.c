// Practice assignment 6, part a

#include <stdio.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/eeprom.h>
#include <util/delay.h>

#include "i2cmaster.h"
#include "lcd.h"

#define BAUDRATE 19200

#define BAUD_PRESCALER (((F_CPU / (BAUDRATE * 16UL))) - 1)

volatile uint8_t received_data=0,call_replay=0,call_erase=0,new_data=0;

volatile unsigned int sex=0,bazinga=0; // make variable for timer

void usart_init(void){
  UBRR0H = (uint8_t)(BAUD_PRESCALER>>8);
  UBRR0L = (uint8_t)(BAUD_PRESCALER);
  UCSR0B = (1<<RXEN0)|(1<<TXEN0);
  UCSR0C = ((1<<UCSZ00)|(1<<UCSZ01));
}

void usart_receive(void){
  while(!(UCSR0A & (1<<RXC0))); //wait for new data
  received_data=UDR0;
  }

void init_timer() // set up timer registers and interrupts
{
  TCCR0A |= (1 << WGM01);
  OCR0A = 0xF9;
  TIMSK0 |= (1 << OCIE0A);
  TCCR0B |= (1 << CS01) | (1 << CS00);
}

void write_val_to_eeprom (uint8_t val)
{
  unsigned int addr;
  addr=0;
  while(eeprom_read_byte((uint8_t *)addr)!=255)
    addr+=1;
  eeprom_write_byte((uint8_t *)addr,val);
}

void led_enable (uint8_t data)
{
  uint8_t x,y;

  bazinga=0;sex=0;
  
  x=data/10;
  y=data%10;
  
  if (x == 1) PORTD = 0b00010000;
  if (x == 2) PORTD = 0b00100000;
  if (x == 3) PORTD = 0b01000000;
  if (x == 4) PORTD = 0b10000000;

  while(sex<=y);

  PORTD &= ~((1 << PORTD4) | (1 << PORTD5) | (1 << PORTD6) | (1 << PORTD7)); 
}

void replay()
{
  unsigned int addr;
  uint8_t numero;
  addr=0;
  LCD_clear();
  _delay_ms(500);
  LCD_set_cursor(0,0);
  while(eeprom_read_byte((uint8_t *)addr)!=255)
  {
    numero=eeprom_read_byte((uint8_t *)addr);
    printf("%hhd ",numero);
    addr+=1;
    led_enable(numero);
  }
  call_replay=0;
}

void erase_eeprom() {
  unsigned int addr;
  addr=0;
  while(eeprom_read_byte((uint8_t *)addr)!=255) {
    eeprom_write_byte((uint8_t *)addr,255);
    addr+=1;
  }
  LCD_clear();
  call_erase=0;
}

int main(void) {  

  usart_init();

  i2c_init();
  LCD_init();
  
  DDRC = 0xF0; // set data direction for port C pins, 0-3 as input (i.e. the buttons)
  PORTC = 0x3F; // set pull-up resistor for port C
  DDRD = 0xFF; // set data direction for port D, all output 
  PORTD= 0x00; // set output for port D (none)
  DDRB = 0b00100000;

  init_timer();

  PCICR |= (1<<PCIE1);

  PCMSK1 |= (1<<PCINT10) | (1<<PCINT11);

  UCSR0B |= (1<<RXCIE0);

  sei();

  while(1) {

    if(call_replay==1)
      replay();

    if(call_erase==1)
      erase_eeprom();
    
    if(new_data==1){
      printf("%hhd ",received_data);
      led_enable(received_data);
      write_val_to_eeprom(received_data);
      new_data=0;
    }
  }

  return 0;
}

ISR (TIMER0_COMPA_vect) // ISR for the timer ticking
{
  bazinga++;
  if(bazinga==1000) {
    PORTB ^= (1 << PINB5); // just increment and when the value reaches what it's supposed to trigger the LED :p
    bazinga=0;
    sex++;
  }
}

ISR(PCINT1_vect)
{
  if(!((PINC >> 2) & 1)) {
    call_replay=1;
  }
  if(!((PINC >> 3) & 1)) {
    call_erase=1;
  }
}

ISR(USART_RX_vect){
  received_data = UDR0;
  new_data=1;
}

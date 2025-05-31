/*
   Example firmware for demonstrating:
   a. Timer1 interrupts generating a 1-second tick.
   b. USART at 19200bps receiving 2-digit messages (range 10–49).
   c. Using the received message "XY" to turn LED number X (i.e. 1–4) ON for Y seconds.
   d. Storing valid messages to EEPROM and, on pressing Button 3, playing them back.
   e. Clearing the EEPROM and LCD on pressing Button 4 using a Pin Change Interrupt.
   
   Make sure you adjust pin definitions and LCD/EEPROM routines to your hardware.
*/

//#define F_CPU 16000000UL

#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/eeprom.h>
#include <stdlib.h>
#include <util/delay.h>

// EEPROM configuration
#define EEPROM_START_ADDR 0x00
#define EEPROM_MAX_MESSAGES 32   // number of messages we plan to store
#define EEPROM_EMPTY 255         // each unwritten byte is 255

// Global variables used by the Timer1 ISR and elsewhere:
// oneSecondFlag is set by the Timer1 interrupt once per second.
volatile uint8_t oneSecondFlag = 0;

// Variables for the LED command that turns on one LED for a number of seconds.
volatile uint8_t ledActive = 0;            // Current active LED (1 to 4); 0 = none.
volatile uint8_t ledSecondsRemaining = 0;  // Count down seconds (updated in the ISR).

// Global EEPROM index (next free slot)
uint8_t ee_index = 0;

// RX buffer for receiving a two-digit message
#define RX_BUFFER_SIZE 3

// ------------------------- Function Prototypes -------------------------
void initTimer1(void);
void initUSART(void);
void initLCD(void);
void initButtons(void);

uint8_t USART_ReceiveByte(void);
void USART_SendByte(uint8_t data);
void USART_SendString(const char *str);

// These LCD routines are placeholders.
void LCD_Clear(void);
void LCD_WriteString(const char *str);

// EEPROM helper routines
void EEPROM_StoreByte(uint8_t data);
uint8_t EEPROM_ReadByte(uint8_t index);

// Playback and clear routines
void playbackMessages(void);
void clearEEPROM(void);

// LED control routines – assume LEDs connected to PORTB bits 0–3.
void LED_On(uint8_t led);
void LED_Off(uint8_t led);

// ------------------------- Interrupt Service Routines -------------------------

// Timer1 Compare Match interrupt: fires every 1 second.
ISR(TIMER1_COMPA_vect) {
    oneSecondFlag = 1;
    
    // If an LED command is active, decrement its timer.
    if (ledSecondsRemaining > 0) {
        ledSecondsRemaining--;
        if (ledSecondsRemaining == 0 && ledActive != 0) {
            LED_Off(ledActive);
            ledActive = 0;
        }
    }
    
    // For testing task (a): toggle PB5 every second.
    PORTB ^= (1 << PB5);
}

// Pin Change Interrupt for Button 4 (to clear EEPROM and LCD).
// Assume Button 4 is connected on PD3 (which on an ATmega328p corresponds to PCINT19).
ISR(PCINT2_vect) {
    // Debounce delay.
    _delay_ms(50);
    
    // Check if Button 4 is pressed (active low).
    if (!(PIND & (1 << PD3))) {
        // Clear the EEPROM and clear the LCD.
        clearEEPROM();
        LCD_Clear();
    }
}

// ------------------------- Main Function -------------------------
int main(void) {
    // Configure PORTB: Bits 0–3 for LEDs 1–4, and PB5 for testing toggling.
    DDRB |= 0x3F;  // Lower 6 bits (PB0 to PB5) as outputs.
    
    // Initialize peripherals.
    initTimer1();
    initUSART();
    initLCD();
    initButtons();
    
    // Enable global interrupts.
    sei();
    
    char rxBuffer[RX_BUFFER_SIZE];
    uint8_t receivedMessage;  // holds the numeric value of the received two-digit message.
    
    while (1) {
        // -------------------------
        // Task (b): Receive message on USART.
        // We expect two ASCII digits (for example: "23", "32", "44", etc.)
        rxBuffer[0] = USART_ReceiveByte();   // receive first digit
        rxBuffer[1] = USART_ReceiveByte();   // receive second digit
        rxBuffer[2] = '\0';                  // null terminate the string
        
        // Convert the received string into an integer.
        receivedMessage = atoi(rxBuffer);
        
        // Validate the message: valid decimal range is 10–49.
        if(receivedMessage < 10 || receivedMessage > 49)
            continue;   // ignore invalid messages
        
        // Display the message on the LCD, appending it to the previous messages.
        LCD_WriteString(rxBuffer);
        LCD_WriteString(" ");  // add a space for separation.
        
        // Store the valid message in EEPROM (if there is available space).
        if (ee_index < EEPROM_MAX_MESSAGES) {
            EEPROM_StoreByte(receivedMessage);
            ee_index++;
        }
        
        // -------------------------
        // Task (c): Process LED command
        // Here the message has the form XY: 
        //   X (tens digit) corresponds to the LED number (1–4)
        //   Y (ones digit) corresponds to the duration in seconds (0–9)
        uint8_t led = receivedMessage / 10;  // extract the LED number
        uint8_t duration = receivedMessage % 10;  // extract the time in seconds
        
        // Although a message like 10 is valid, a duration of 0 means the LED does not really turn ON.
        if (duration > 0 && led >= 1 && led <= 4) {
            // Set globals so that the Timer1 ISR will handle the countdown.
            ledActive = led;
            ledSecondsRemaining = duration;
            LED_On(led);
            
            // Wait until the LED command finishes (when ledSecondsRemaining becomes 0);
            while(ledSecondsRemaining > 0) {
                // Optionally sleep or perform other tasks.
                // The Timer1 ISR is decrementing ledSecondsRemaining every 1 second.
                if (oneSecondFlag) {
                    oneSecondFlag = 0;
                }
            }
        }
        
        // -------------------------
        // Task (d): Check for Button 3 press (for playback).
        // Assume Button 3 is connected on PD2 (active low) and use simple polling.
        if ((PIND & (1 << PD2)) == 0) {
            _delay_ms(50);  // debounce
            playbackMessages();
        }
    }
    return 0;
}

// ------------------------- Initialization Functions -------------------------

// Initializes Timer1 in CTC mode to generate a one second tick.
// With a 16MHz clock and a 1024 prescaler, one second is roughly 16,000,000/1024 - 1 = 15624 ticks.
void initTimer1(void) {
    TCCR1A = 0;
    TCCR1B = 0;
    
    // Set CTC mode.
    TCCR1B |= (1 << WGM12);
    OCR1A = 15624;  // Set compare register for 1-second interval.
    
    // Enable Timer1 Compare Match A interrupt.
    TIMSK1 |= (1 << OCIE1A);
    
    // Start Timer1 with a 1024 prescaler.
    TCCR1B |= (1 << CS12) | (1 << CS10);
}

// Initializes USART to 19200bps, 8 data bits, 1 stop bit, no parity.
void initUSART(void) {
    // Calculate the UBRR value using: UBRR = F_CPU/(16*baud) - 1.
    uint16_t ubrr_value = (F_CPU/16/19200) - 1;
    UBRR0H = (unsigned char)(ubrr_value >> 8);
    UBRR0L = (unsigned char)ubrr_value;
    
    // Enable receiver and transmitter.
    UCSR0B = (1 << RXEN0) | (1 << TXEN0);
    
    // Set frame format: 8 data bits, 1 stop bit, no parity.
    UCSR0C = (1 << UCSZ01) | (1 << UCSZ00);
}

// Placeholder: Initialize the LCD display.
void initLCD(void) {
    // Call your LCD initialization routine here.
}

// Configure Button 3 and Button 4 inputs.
// Button 3 (for playback) is assumed to be on PD2; Button 4 (for clearing) on PD3.
// Enable the pin change interrupt feature for Button 4.
void initButtons(void) {
    // Set PD2 and PD3 as inputs with internal pull-ups.
    DDRD &= ~((1 << PD2) | (1 << PD3));
    PORTD |= (1 << PD2) | (1 << PD3);
    
    // Enable Pin Change Interrupt for Button 4 (on PD3). On an ATmega328p, PD3 corresponds to PCINT19.
    PCICR |= (1 << PCIE2);        // Enable Pin Change Interrupts on PCINT[23:16] (PORTD).
    PCMSK2 |= (1 << PCINT19);      // Enable interrupt for PD3.
}

// ------------------------- USART Functions -------------------------

// Receive one byte via USART (blocking).
uint8_t USART_ReceiveByte(void) {
    while (!(UCSR0A & (1 << RXC0))) {
        // Wait for data.
    }
    return UDR0;
}

// Transmit one byte via USART.
void USART_SendByte(uint8_t data) {
    while (!(UCSR0A & (1 << UDRE0))) {
        // Wait until the transmit buffer is empty.
    }
    UDR0 = data;
}

// Transmit a null-terminated string via USART.
void USART_SendString(const char *str) {
    while (*str) {
        USART_SendByte(*str++);
    }
}

void USART_ReceiveString(char *buffer, uint8_t buffer_size) {
    uint8_t i = 0;
    char ch;
    
    // Read characters one by one
    while (1) {
        ch = USART_Receive();
        
        // If a newline or carriage return is received, terminate the string.
        if (ch == '\n' || ch == '\r') {
            buffer[i] = '\0';
            break;
        }
        
        // Only add character if there is space (reserve space for the null-terminator)
        if (i < buffer_size - 1) {
            buffer[i++] = ch;
        }
    }
}

// ------------------------- LCD Placeholder Routines -------------------------

// Clear the LCD display.
void LCD_Clear(void) {
    // Implement your LCD clear routine.
}

// Write a string to the LCD display.
void LCD_WriteString(const char *str) {
    // Implement your routine to display a string on the LCD.
}

// ------------------------- EEPROM Helper Functions -------------------------

// Store a single byte (a received message) to EEPROM at the current index.
void EEPROM_StoreByte(uint8_t data) {
    eeprom_update_byte((uint8_t*)(EEPROM_START_ADDR + ee_index), data);
}

// Read a byte from EEPROM at a given index.
uint8_t EEPROM_ReadByte(uint8_t index) {
    return eeprom_read_byte((uint8_t*)(EEPROM_START_ADDR + index));
}

// ------------------------- Playback and Clear Functions -------------------------

// Playback stored messages from EEPROM. For each stored message, the corresponding LED (X)
// is turned ON for Y seconds. The LCD is updated to show the played-back messages.
void playbackMessages(void) {
    LCD_WriteString("\nPlayback: ");
    for (uint8_t i = 0; i < EEPROM_MAX_MESSAGES; i++) {
        uint8_t data = EEPROM_ReadByte(i);
        if (data == EEPROM_EMPTY)
            break;  // End of stored messages
        
        // Display the message on the LCD.
        char msg[3];
        itoa(data, msg, 10);
        LCD_WriteString(msg);
        LCD_WriteString(" ");
        
        // Extract LED number and duration.
        uint8_t led = data / 10;
        uint8_t duration = data % 10;
        if (duration > 0 && led >= 1 && led <= 4) {
            ledActive = led;
            ledSecondsRemaining = duration;
            LED_On(led);
            while (ledSecondsRemaining > 0)
                ;  // Wait until Timer1 ISR finishes the countdown.
        }
        _delay_ms(500);  // Optional delay between messages.
    }
}

// Clear the EEPROM area (write 255 to every location) and reset the EEPROM index.
void clearEEPROM(void) {
    for (uint8_t i = 0; i < EEPROM_MAX_MESSAGES; i++) {
        eeprom_update_byte((uint8_t*)(EEPROM_START_ADDR + i), EEPROM_EMPTY);
    }
    ee_index = 0;
}

// ------------------------- LED Control Routines -------------------------

// Turn on LED number 'led'. Assume LED1 is connected to PB0, LED2 to PB1, etc.
void LED_On(uint8_t led) {
    if (led >= 1 && led <= 4)
        PORTB |= (1 << (led - 1));
}

// Turn off LED number 'led'.
void LED_Off(uint8_t led) {
    if (led >= 1 && led <= 4)
        PORTB &= ~(1 << (led - 1));
}

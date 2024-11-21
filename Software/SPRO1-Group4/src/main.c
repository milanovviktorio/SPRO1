#include <stdio.h>
#include <avr/io.h>
#include <util/delay.h>
#include "usart.h"

void printBinary(uint8_t value);

int main(void) {  

  DDRB |= 1 << PB2;
  DDRB |= 0 << PB5;

  uart_init(); 
  io_redirect(); 
  // i2c_init();
  // LCD_init();
  
  while(1) {
    if(PINB & (1 << PB5))
    {
      printf("Detected.\n");
      PORTB |= 1 << PB2;
    }else{
      PORTB &= 0 << PB2;
    } 
    _delay_ms(100);
  }
  return 0;
}

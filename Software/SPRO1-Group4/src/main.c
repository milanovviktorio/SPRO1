#include <stdio.h>
#include <avr/io.h>
#include <util/delay.h>
#include "usart.h"
#include "adcpwm.h"

void printBinary(uint8_t value);

int main(void) {  

  DDRB |= (1 << PB1) | (1 << PB5) | (1 << PB6);//brg
  DDRC |= (0 << PC1) | (0 << PC2) | (0 << PC3);//rgb
  
  float red, green, blue;

  uart_init(); 
  io_redirect(); 
  pwm3_init();
  adc_init();
  // i2c_init();
  // LCD_init();
  
  while(1) {
    red = adc_read(1);
    green = adc_read(2);
    blue = adc_read(3);
    red = red*100/1023;
    green = green*100/1023;
    blue = blue*100/1023;
    printf("Red Value: %d\n", 100-(int)red);
    if(PINB & (1 << PB5))
    {
      printf("Detected.\n");
      pwm3_set_duty(0,0,100-red);
      _delay_ms(100);
    }else{
      PORTB &= 0<<PB2;
      pwm3_set_duty(0,0,0);
    } 
    _delay_ms(100);
  }
  return 0;
}

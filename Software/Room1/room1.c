#include <stdio.h>
#include <avr/io.h>
#include <util/delay.h>
#include "usart.h"
#include "adcpwm.h"

void printBinary(uint8_t value);

int main(void) {  

  DDRB |= 1 << PB2;
  DDRB |= 0 << PB5;
  
  float adc_value;

  uart_init(); 
  io_redirect(); 
  pwm3_init();
  adc_init();
  // i2c_init();
  // LCD_init();
  
  while(1) {
    adc_value = adc_read(0);
    adc_value = adc_value*100/1023;
    printf("ADC Value: %d\n", 100-(int)adc_value);
    if(PINB & (1 << PB5))
    {
      printf("Detected.\n");
      pwm3_set_duty(0,0,100-adc_value);
      _delay_ms(100);
    }else{
      PORTB &= 0<<PB2;
      pwm3_set_duty(0,0,0);
    } 
    _delay_ms(100);
  }
  return 0;
}

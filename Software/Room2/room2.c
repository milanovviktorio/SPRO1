//room2

#include <stdio.h>
#include <avr/io.h>
#include <util/delay.h>

#include "usart.h"
#include "i2cmaster.h"
#include "lcd.h"

#define BUTTON1_PIN PD2  
#define BUTTON2_PIN PD6  
#define RGB_LED_PIN PD7  

int main(void) {
  uart_init();       
  io_redirect();    

  i2c_init();     
  LCD_init();       

 
  DDRD &= ~(1 << BUTTON1_PIN); 
  PORTD |= (1 << BUTTON1_PIN); 

  DDRD &= ~(1 << BUTTON2_PIN);  
  PORTD |= (1 << BUTTON2_PIN);  

 
  DDRD |= (1 << RGB_LED_PIN);  
  PORTD &= ~(1 << RGB_LED_PIN);   

  while (1) {

    if (!(PIND & (1 << BUTTON1_PIN)) ^ (!(PIND & (1 << BUTTON2_PIN)))) {
      LCD_clear();

      LCD_set_cursor(0, 0);
      printf("ROOM 2");       
      LCD_set_cursor(0, 2);
      printf("Light ON");     

       PORTD |= (1 << RGB_LED_PIN); 
    } 
    else {
      LCD_clear();
      LCD_set_cursor(0, 0);
      printf("ROOM 2");      
      LCD_set_cursor(0, 2);
      printf("Light OFF");    

      PORTD &= ~(1 << RGB_LED_PIN);    
    }

    _delay_ms(50); 
  }

  return 0;
}

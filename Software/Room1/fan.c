//room1

#include <stdio.h>
#include <avr/io.h>
#include <util/delay.h>

#include "usart.h"
#include "i2cmaster.h"
#include "lcd.h"


#define BUTTON_FAN_PIN PD11

int main(void) {
  uart_init();       
  io_redirect();    

  i2c_init();     
  LCD_init();       

  DDRD &= ~(1 << BUTTON_FAN_PIN);  
  PORTD |= (1 <<  BUTTON_FAN_PIN); 
 
  

  while (1) {
    if (!(PIND & (1 << BUTTON_FAN_PIN))){
      LCD_clear();

      LCD_set_cursor(0, 0);
      printf("ROOM 2");       
      LCD_set_cursor(0, 2);
      printf("Fan ON");   



      
    }

  }

  return 0;
}

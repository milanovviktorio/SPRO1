#include <stdio.h>
#include <avr/io.h>
#include <util/delay.h>
#include "usart.h"
#include "adcpwm.h"
#include "i2cmaster.h"
#include "lcd.h"
#include "lm75.h"

void mainDoor(void);
void room1(void);
void room2(void);
void room3(void);

float ldr_value, red, green, blue;

int main(void) {  

  int state = 1;//init state - main door

  //maindoor definitions
  DDRB |= 1 << PB2;//white main door
  DDRD |= 0 << PB0;//PIR
  // DDRC |= 0 << PC6;//ldr

  //room1 definitions
  DDRD |= 1 << PD3;//white room 1
  DDRC |= 0 << PC0;//pot room 1
  DDRB |= 1 << PB3 || 0 << PB5;//fan || switch room 1

  //room2 definitions
  DDRD |= 1 << PD2 || 0 << PD7;//white room 2 || switch1 room 2
  DDRB |= 0 << PB4;//switch2 room 2

  //room3 definitions
  DDRB |= (1 << PB1) | (1 << PB5) | (1 << PB6);//brg - diode
  DDRC |= (0 << PC1) | (0 << PC2) | (0 << PC3);//rgb - pots
  DDRC |= 0 << 7;//switch room 3

  // uart_init(); 
  // io_redirect(); 
  pwm_init();
  adc_init();
  i2c_init();
  LCD_init();
  
  while(1) {
    //state machine
    switch (state)
    {
    case 1:
      mainDoor();
      break;
    case 2:
      room1();
      break;
    case 3:
      room2();
      break;
    case 4:
      room3();
      break;
    }

  return 0;
  }
}

void mainDoor(void)
{
  ldr_value = adc_read(6);//LDR
  ldr_value = ldr_value*100/1023;
  
  LCD_set_cursor(0,0);
  printf("Main door \n");
  LCD_set_cursor(0,1);
  printf("Light intensity: %d\n", 100-(int)ldr_value);
  if(PIND & (1 << PD0))
  {
    pwm_set_duty(0,100-(int)ldr_value,0,0,0,0);
    _delay_ms(100);
  }else{
    PORTB &= 0<<PB2;
    pwm_set_duty(0,0,0,0,0,0);
  } 
  _delay_ms(100);
}

void room1(void)
{
  ldr_value = adc_read(6);//LDR
  ldr_value = ldr_value*100/1023;

  red = adc_read(1);
  green = adc_read(2);
  blue = adc_read(3);
  red = red*100/1023;
  green = green*100/1023;
  blue = blue*100/1023;

  LCD_set_cursor(0,0);
  printf("Main door: \n");
  LCD_set_cursor(0,1);
  printf("Light intensity: %d\n", 100-(int)ldr_value);
  if(PIND & (1 << PD0))
  {
    pwm_set_duty(0,0,100-(int)ldr_value,0,0,0);
    _delay_ms(100);
  }else{
    PORTB &= 0<<PB2;
    pwm_set_duty(0,0,0,0,0,0);
  } 
  _delay_ms(100);
}

void room2(void)
{
  ldr_value = adc_read(6);//LDR
  ldr_value = ldr_value*100/1023;

  red = adc_read(1);
  green = adc_read(2);
  blue = adc_read(3);
  red = red*100/1023;
  green = green*100/1023;
  blue = blue*100/1023;

  LCD_set_cursor(0,0);
  printf("Main door: \n");
  LCD_set_cursor(0,1);
  printf("Light intensity: %d\n", 100-(int)ldr_value);
  if(PIND & (1 << PD0))
  {
    pwm_set_duty(0,0,100-(int)ldr_value,0,0,0);
    _delay_ms(100);
  }else{
    PORTB &= 0<<PB2;
    pwm_set_duty(0,0,0,0,0,0);
  } 
  _delay_ms(100);
}

void room3(void)
{
  ldr_value = adc_read(6);//LDR
  ldr_value = ldr_value*100/1023;

  red = adc_read(1);
  green = adc_read(2);
  blue = adc_read(3);
  red = red*100/1023;
  green = green*100/1023;
  blue = blue*100/1023;

  LCD_set_cursor(0,0);
  printf("Main door: \n");
  LCD_set_cursor(0,1);
  printf("Light intensity: %d\n", 100-(int)ldr_value);
  if(PIND & (1 << PD0))
  {
    pwm_set_duty(0,0,100-(int)ldr_value,0,0,0);
    _delay_ms(100);
  }else{
    PORTB &= 0<<PB2;
    pwm_set_duty(0,0,0,0,0,0);
  } 
  _delay_ms(100);
}
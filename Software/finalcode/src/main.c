#include <stdio.h>
#include <avr/io.h>
#include <util/delay.h>
#include "usart.h"
#include "adcpwm.h"
#include "i2cmaster.h"
#include "lcd.h"

void mainDoor(void);
void room1(void);
void room2(void);
void room3(void);

unsigned int ldr_value, red, green, blue;

int main(void) {  

  int state = 1;//init state - main door

  //maindoor definitions
  DDRB |= 1 << PB2;//white main door
  DDRD |= 0 << PD0;//PIR
  // DDRC |= 0 << PC6;//ldr

  //room1 definitions
  DDRD |= 1 << PD3;//white room 1
  DDRC |= 0 << PC0;//pot room 1
  DDRB |= (1 << PB5); //fan enable
  PORTB |= (1 << PB3); //fan switch

  //room2 definitions
  DDRD &= ~(1 << PD7); 
  PORTD |= (1 << PD7); 

  DDRB &= ~(1 << PB4);  
  PORTB |= (1 << PB4);  

  DDRD |= (1 << PD2);  
  PORTD &= ~(1 << PD2); 

  //room3 definitions
  DDRB |= (1 << PB1) | (1 << PB5) | (1 << PB6);//brg - diode
  DDRC |= (0 << PC1) | (0 << PC2) | (0 << PC3);//rgb - pots
  DDRC |= 0 << 7;//switch room 3

  printf("%x %x %x %x", PORTB, PORTD, DDRD, DDRB);
  uart_init(); 
  io_redirect(); 
  pwm_init();
  adc_init();

  //i2c_init();
  //LCD_init();
  
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
  }
}

void mainDoor(void)
{
  ldr_value = adc_read(6)/10;//LDR
  
  /*LCD_set_cursor(0,0);
  printf("Main door: \n");
  LCD_set_cursor(0,1);
  printf("Light intensity: %d\n", 100-ldr_value);*/

  if(PIND & (1 << PD0))
  {
    pwm_set_duty(0,0,0,0,ldr_value);
    _delay_ms(100);
  }
  else{
    PORTB &= 0<<PB2;
    pwm_set_duty(0,0,0,0,0);
  } 
  _delay_ms(100); 
}

void room1(void)
{
  red = adc_read(1)/10;
  green = adc_read(2)/10;
  blue = adc_read(3)/10;

  /*LCD_clear();
  LCD_set_cursor(0,0);
  printf("ROOM 1: \n");
  LCD_set_cursor(0, 2);
  printf("Light intensity: %d\n", red);*/

  pwm_set_duty(red,0,0,0,0);
  //_delay_ms(100);
  
  //fan
  if (!((PINB >> 3) & 1)) { 
      PORTB |= (1 << PB5);

      /*LCD_clear();
      LCD_set_cursor(0, 0);
      printf("ROOM 2");       
      LCD_set_cursor(0, 2);
      printf("Fan ON\n"); */  
    }
  else {
    PORTB &= ~(1 << PB5);

    /*LCD_clear();
      LCD_set_cursor(0, 0);
      printf("ROOM 2");       
      LCD_set_cursor(0, 2);
      printf("Fan OFF\n"); */ 
  }

}

void room2(void)
{
 if (!(PIND & (1 << PD7)) ^ (!(PINB & (1 << PB4)))) {
      /*LCD_clear();
      LCD_set_cursor(0, 0);
      printf("ROOM 2");       
      LCD_set_cursor(0, 2);
      printf("Light ON");*/ 

      PORTD |= (1 << PD2); 
    } 
    else {
      /*LCD_clear();
      LCD_set_cursor(0, 0);
      printf("ROOM 2");      
      LCD_set_cursor(0, 2);
      printf("Light OFF");*/ 
      
      PORTD &= ~(1 << PD2);    
    }
    _delay_ms(50); 
  
}

void room3(void)
{
  red = adc_read(1)/10;
  green = adc_read(2)/10;
  blue = adc_read(3)/10;

  /*LCD_set_cursor(0,0);
  printf("Main door: \n");
  //LCD_set_cursor(0,1);
  printf("Light intensity: %d\n", red);*/
  if(adc_read(7)<127)
  {
    pwm_set_duty(0,red,green,blue,0);
    //_delay_ms(100);
  }
  else{
    PORTB &= 0<<PB2;
    pwm_set_duty(0,0,0,0,0);
  } 
  //_delay_ms(100);
}
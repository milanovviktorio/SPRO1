#include <stdio.h>
#include <avr/io.h>
#include <util/delay.h>
#include "usart.h"
#include "adcpwm.h"
#include "i2cmaster.h"
#include "lcd.h"

void menu(void);
void mainDoor_room1(void);
void room2(void);
void room3(void);
void party(void);

unsigned int ldr_value, red, green, blue, value, md_state, room1_state, room2_state, room3_state;
int fanOn = 0, room2light = 0;

int main(void) {  

  int state = 4;//init state - main door

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
  DDRD &= ~(1 << PD7); //switch 1 room 2 
  PORTD |= (1 << PD7); 

  DDRB &= ~(1 << PB4);  //swtich 2 room 2
  PORTB |= (1 << PB4);  

  DDRD |= (1 << PD2);  //led input room2
  PORTD &= ~(1 << PD2); 

  //room3 definitions
  DDRB |= (1 << PB1) | (1 << PB5) | (1 << PB6);//brg - diode
  DDRC |= (0 << PC1) | (0 << PC2) | (0 << PC3);//rgb - pots
  DDRC |= 0 << 7;//switch room 3

  //menu controlls
  DDRD &= ~((1 << PD1) | (1 << PD4)); // PD1, PD4 as inputs
  PORTD |= ((1 << PD1) | (1 << PD4)); //pullup
  DDRB &= ~(1 << PB0);               // PB0 as input
  PORTB |= (1 << PB0); //pullup

  //uart_init(); 
  //io_redirect(); 
  pwm_init();
  adc_init();

  i2c_init();
  LCD_init();

  while(1) {
    if (!(PIND & (1 << PD4)) && !(PIND & (1 << PD1)))
    {
      state = 4;
    }
    else if (!(PIND & (1 << PD1))) 
    {
      state = 1; 
    }
    else if (!(PINB & (1 << PB0))) 
    {
      state = 2; 
    }
    else if (!(PIND & (1 << PD4))) 
    {
      state = 3; 
    }

    mainDoor_room1();
    room2();
    room3();

    //state machine
    switch (state)
    {
    case 1:
      LCD_set_cursor(0,0);
      printf("Main door: %d   ", ldr_value);
      LCD_set_cursor(0,1);
      printf("ROOM 1              ");
      LCD_set_cursor(0,2);
      printf("Light intensity: %d", adc_read(1));  

      //fan prints
      if (fanOn == 1) { 
        LCD_set_cursor(0, 3);
        printf("Fan ON           ");
      }
      else {
        LCD_set_cursor(0, 3);
        printf("Fan OFF           ");
      }

      break;
    case 2:
      LCD_set_cursor(0, 0);
      printf("ROOM 2       ");
      LCD_set_cursor(0, 1);
      printf("                    ");
      if (room2light == 1) {  
      LCD_set_cursor(0, 2);
      printf("Light ON            ");
      LCD_set_cursor(0, 3);
      printf("                    ");
      }
      else {
      LCD_set_cursor(0, 2);
      printf("Light OFF           ");
      LCD_set_cursor(0, 3);
      printf("                    ");
      }
      break;
    case 3:
      LCD_set_cursor(0, 0);
      printf("ROOM 3             ");    
      LCD_set_cursor(0, 1);
      printf("Red: %d              ", red);
      LCD_set_cursor(0, 2);
      printf("Green: %d           ", green);
      LCD_set_cursor(0, 3);
      printf("Blue: %d            ", blue);
      break;
    case 4:
      //frame1
      LCD_set_cursor(0, 0);
      printf("       O       ");
      LCD_set_cursor(0, 1);
      printf("      \\|/ ");
      LCD_set_cursor(0, 2);
      printf("       |            ");
      LCD_set_cursor(0, 3);
      printf("      / \\ ");
      _delay_ms(300);

      //frame2
      LCD_set_cursor(0, 0);
      printf("       O  ");
      LCD_set_cursor(0, 1);
      printf("       |  ");
      LCD_set_cursor(0, 2);
      printf("       |            ");
      LCD_set_cursor(0, 3);
      printf("       |  ");
      _delay_ms(300);

      //frame3
      LCD_set_cursor(0, 0);
      printf("       O  ");
      LCD_set_cursor(0, 1);
      printf("      -|- ");
      LCD_set_cursor(0, 2);
      printf("       |            ");
      LCD_set_cursor(0, 3);
      printf("      / \\ ");
      _delay_ms(300);

      //frame4
      LCD_set_cursor(0, 0);
      printf("       O  ");
      LCD_set_cursor(0, 1);
      printf("      \\|/ ");
      LCD_set_cursor(0, 2);
      printf("       |            ");
      LCD_set_cursor(0, 3);
      printf("       X  ");
      _delay_ms(300);
      break;
    }
  }
}

void mainDoor_room1(void)
{
  //maindoor
  ldr_value = adc_read(6)/10;//LDR
  //pwm_set_duty(room1_state, red, green, blue, ldr_value - 41);

  //room1 
  room1_state = adc_read(0)/10;
  pwm_set_duty(room1_state, red, green, blue, ldr_value - 41);

  //fan trust
  if (!((PINB >> 3) & 1)) 
  { 
    PORTB |= (1 << PB5);
    fanOn = 1;
  }
  else 
  {
    PORTB &= ~(1 << PB5);
    fanOn = 0;
  }
}

void room2(void)
{
  if (!(PIND & (1 << PD7)) ^ (!(PINB & (1 << PB4)))) 
  {
    room2light = 1;
    PORTD |= (1 << PD2); 
  } 
  else 
  {
    room2light = 0;
    PORTD &= ~(1 << PD2);    
  }
  //_delay_ms(50); 
  
}

void room3(void)
{
  red = adc_read(1)/10;
  green = adc_read(2)/10;
  blue = adc_read(3)/10;

  if(adc_read(7)<127)
  {
    pwm_set_duty(room1_state, red, green, blue, ldr_value - 41);
    //_delay_ms(100);
  }
  
  //_delay_ms(100);
}

void party(void)
{
    // Frame 1: Arms up, legs spread
    pwm_set_duty(room1_state, 0, 0, 0, ldr_value - 41);
    _delay_ms(100);
    pwm_set_duty(room1_state,50,0,0,ldr_value - 41);
    _delay_ms(100);

    // Frame 2: Arms down, legs together
    pwm_set_duty(room1_state,100,0,0,ldr_value - 41);
    _delay_ms(100);
    pwm_set_duty(room1_state,100,50,0,ldr_value - 41);
    _delay_ms(100);

    // Frame 3: Arms waving, legs crossed
    pwm_set_duty(room1_state,100,100,0,ldr_value - 41);
    _delay_ms(100);
      pwm_set_duty(room1_state,100,100,50,ldr_value - 41);
    _delay_ms(100);

    // Frame 4: Arms out, legs crossed
    pwm_set_duty(room1_state,100,100,100,ldr_value - 41);
  }
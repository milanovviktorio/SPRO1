#include <stdio.h>
#include <avr/io.h>
#include <util/delay.h>

#include "adcpwm.h"
#include "i2cmaster.h"
#include "lcd.h"


void menu(void);
void mainDoor_room1(void);
void room2(void);
void room3(void);
void party(void);


unsigned int ldr_value, red, green, blue, value, md_state, room1_state, room2_state, room3_state, md_state, party_counter;

int state = 0;//init state - main door

int fanOn = 0, room2light = 0, menu_counter = 0;

int main(void) {  
  PORTB=0;PORTC=0;PORTD=0;
  PINB=0;PINC=0;PIND=0;
  DDRB=0;DDRC=0;DDRD=0;
  //maindoor definitions
  DDRB |= 1 << PB2;//white main door
  DDRD &= ~(1 << PD4); //switch 1 room 2 ;//PIR
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
  DDRD &= ~((1 << PD1) | (1 << PD0)); // PD1, PD0 as inputs
  PORTD |= ((1 << PD1) | (1 << PD0)); //pullup
  DDRB &= ~(1 << PB0);               // PB0 as input
  PORTB |= (1 << PB0); //pullup
  

  //uart_init(); 
  //io_redirect(); 


  pwm_init();
  adc_init();

  i2c_init();
  LCD_init();

  while(1) {

    if (menu_counter >= 250) {
      state = 0;          
    }
    
    if (!(PIND & (1 << PD0)) && !(PIND & (1 << PD1)))
    {
      state = 4;
      menu_counter = 0;
    }
    else if (!(PIND & (1 << PD1))) 
    {
      state = 1;
      menu_counter = 0;
    }
    else if (!(PINB & (1 << PB0))) 
    {
      state = 2;
      menu_counter = 0;
    }
    else if (!(PIND & (1 << PD0))) 
    {
      state = 3;
      menu_counter = 0;
    }

    menu_counter = menu_counter + 1;

    mainDoor_room1();
    room2();
    room3();
    

    //state machine
    switch (state)
    {
    case 0:
      LCD_set_cursor(0,0);
      printf("Menu options:");

      LCD_set_cursor(0,1);
      printf("Button1: Room 1 Door");

      LCD_set_cursor(0,2);
      printf("Button2: Room 2     ");

      LCD_set_cursor(0,3);
      printf("Button3: Room 3     ");
      break;
    case 1:
      LCD_set_cursor(0,0);
      printf("Main door: %d  ", ldr_value);
      LCD_set_cursor(0,1);
      printf("ROOM 1              ");
      LCD_set_cursor(0,2);
      printf("Light intensity: %d ", room1_state);  

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
      
      if (red>10){
        LCD_set_cursor(0, 1);
        printf("Red: %d             ", red);
      }
      else
      {
        LCD_set_cursor(0, 1);
        printf("Red: %d              ", red); 
      }
      LCD_set_cursor(0, 2);
      printf("Green: %d         ", green);
      LCD_set_cursor(0, 3);
      printf("Blue: %d           ", blue);
      break;
    case 4:
      LCD_set_cursor(0, 0);
      printf("party mode         ");
      LCD_set_cursor(0, 1);
      printf("                    ");
      LCD_set_cursor(0, 2);
      printf("                   ");
      LCD_set_cursor(0, 3);
      printf("                   ");
      party();
      break; 
    }
  }
}

void mainDoor_room1(void)
{
  //maindoor
  room1_state = (adc_read(0)*10)/102;

  //room1
  if (PIND & (1 << PD4)){
    ldr_value = (adc_read(6)*10)/102;
  }
  else
  {
   ldr_value = 0;
  }

  pwm_set_duty(room1_state, red, green, blue, ldr_value);

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

  
}

void room3(void)
{
  if(state!=4) {

  if(!adc_read(7))
  {
    red = (adc_read(1)*10)/102;
    green = (adc_read(2)*10)/102;
    blue = (adc_read(3)*10)/102;
  }
  else
  {
    red=0;
    green=0;
    blue=0;
  }
  }
  pwm_set_duty(room1_state, red, green, blue, ldr_value);
  
}

void party(void)
{   
    party_counter=menu_counter%50;
    if(!adc_read(7) && state==4){
      if (7>party_counter && 0<=party_counter){
        red=0;
        green=0;
        blue=0;
      }
      if (14>party_counter && 7<=party_counter){
        red=50;
        green=0;
        blue=0;
      }
      if (21>party_counter && 14<=party_counter){
        red=100;
        green=0;
        blue=0;
      }
      if (28>party_counter && 21<=party_counter){
        red=100;
        green=50;
        blue=0;
      }
      if (35>party_counter && 28<=party_counter){
        red=100;
        green=100;
        blue=0;
      }
      if (42>party_counter && 35<=party_counter){
        red=100;
        green=100;
        blue=50;
      }
      if (50>party_counter && 42<=party_counter){
        red=100;
        green=100;
        blue=100;
      }
      pwm_set_duty(room1_state, red, green, blue, ldr_value);
    }
}
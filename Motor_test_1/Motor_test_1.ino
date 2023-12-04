/*
TITLE  : Analog Output using PWM
DATE  : 2019/11/12
AUTHOR  : e-Yantra Team

AIM: To decrease the brightness of Red Led and simultaneously increase the brightness of Green Led,
   then to decrease the brightness of Green Led and simultaneously increase the brightness of Blue Led,
   then to decrease the brightness of Blue Led and simultaneously increase the brightness of Red Led
   and repeat this cyclically.
*/

#define F_CPU 16000000UL    // Define Crystal Frequency of eYFi-Mega Board

#include <avr/io.h>         // Standard AVR IO Library
#include <util/delay.h>     // Standard AVR Delay Library
#include <avr/interrupt.h>  // Standard AVR Interrupt Library

#define PIN_PWM_LM  PL3
#define PIN_PWM_RM  PL4

#define PIN_LM1 PD4
#define PIN_LM2 PD5

#define PIN_RM1 PD6
#define PIN_RM2 PD7

#define PIN_LM1_HIGH  (PORTD |=  (1<<PIN_LM1))
#define PIN_LM1_LOW   (PORTD &= ~(1<<PIN_LM1))

#define PIN_LM2_HIGH  (PORTD |=  (1<<PIN_LM2))
#define PIN_LM2_LOW   (PORTD &= ~(1<<PIN_LM2))

#define PIN_RM1_HIGH  (PORTD |=  (1<<PIN_RM1))
#define PIN_RM1_LOW   (PORTD &= ~(1<<PIN_RM1))

#define PIN_RM2_HIGH  (PORTD |=  (1<<PIN_RM2))
#define PIN_RM2_LOW   (PORTD &= ~(1<<PIN_RM2))


void gpio_init(void){
  DDRL    |= (1 << PIN_PWM_LM) | (1 << PIN_PWM_RM);    
  PORTL   |= (1 << PIN_PWM_LM) | (1 << PIN_PWM_RM);

  DDRD    |= (1 << PIN_LM1) | (1 << PIN_LM2) | (1 << PIN_RM1) | (1 << PIN_RM2);
  PORTD   |= (1 << PIN_LM1) | (1 << PIN_LM2) | (1 << PIN_RM1) | (1 << PIN_RM2);   
}

void LM_On(void){
  PORTH &= ~(1 << PIN_PWM_LM);
}

void LM_Off(void){
  PORTH |= (1 << PIN_PWM_LM);
}

void RM_On(void){
  PORTH &= ~(1 << PIN_PWM_RM);
}

void RM_Off(void){
  PORTH |= (1 << PIN_PWM_RM);
}

// Timer 5 initialized in PWM mode for brightness control
// Prescale:256
// PWM 8bit fast, TOP=0x00FF
// Timer Frequency:225.000Hz

void timer5_init()
{
  cli(); //disable all interrupts
  
  TCCR5B = 0x00;  //Stop
  
  TCNT5H = 0xFF;  //Counter higher 8-bit value to which OCR5xH value is compared with
  TCNT5L = 0x00;  //Counter lower 8-bit value to which OCR5xH value is compared with
  
  OCR5AH = 0x00;  //Output compare register high value for Red Led
  OCR5AL = 0xFF;  //Output compare register low value for Red Led
  
  OCR5BH = 0x00;  //Output compare register high value for Blue Led
  OCR5BL = 0xFF;  //Output compare register low value for Blue Led

  OCR5CH = 0x00;  //Output compare register high value for Green Led
  OCR5CL = 0xFF;  //Output compare register low value for Green Led
  
  //  Clear OC4A, OC4B & OC4C on compare match (set output to low level)
  TCCR5A |= (1 << COM5A1) | (1 << COM5B1) | (1 << COM5C1);
  TCCR5A &= ~((1 << COM5A0) | (1 << COM5B0) | (1 << COM5C0));

  // FAST PWM 8-bit Mode
  TCCR5A |= (1 << WGM40);
  TCCR5A &= ~(1 << WGM41);
  TCCR5B |= (1 << WGM42);
  
  // Set Prescalar to 64
  TCCR5B |= (1 << CS41) | (1 << CS40);
  TCCR5B &= ~(1 << CS42);
  
  sei(); //re-enable interrupts
}

// Function for brightness control of all 3 LEDs
void pwm (unsigned char lm, unsigned char rm){
  OCR5AL = 255 - (unsigned char)lm;  // active low thats why subtracting by 255
  OCR5BL = 255 - (unsigned char)rm;
}

//use this function to initialize all devices
void init_devices (void) {
  gpio_init();
  timer5_init();
}

//Main Function
int main(){
  init_devices();
  pwm(80,80);

  
  
  while(1){
 
  PIN_LM1_HIGH;
  PIN_LM2_LOW;

  PIN_RM1_LOW;
  PIN_RM2_HIGH;
  
    delay(5000);


    PIN_LM1_LOW;
    PIN_LM2_HIGH;

    PIN_RM1_HIGH;
    PIN_RM2_LOW;

    delay(5000);
   

  }
}

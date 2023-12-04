#define F_CPU 16000000UL    // Define Crystal Frequency of eYFi-Mega Board
#include <avr/io.h>       // Standard AVR IO Library


#define RM1 PD4
#define RM2 PD5

#define LM1 PD6
#define LM2 PD7

#define PWM_RM 45
#define PWM_LM 46

uint8_t PWM_value = 50;

void setup() {
  // set pins 2 through 13 as outputs:
  //for (int thisPin = lowestPin; thisPin <= highestPin; thisPin++) {

    DDRD  |= (1<<PD7) | (1<<PD6) | (1<<PD5) | (1<<PD4);
    DDRL  |= (1<<PL3) | (1<<PL4);
  
    PORTD |= (1<<PD7) | (1<<PD6) | (1<<PD5) | (1<<PD4);
    PORTL |= (1<<PL3) | (1<<PL4);
    
    //pinMode(PWM_RM, OUTPUT);
    //pinMode(PWM_LM, OUTPUT);
    
    analogWrite(PWM_RM, PWM_value);
    analogWrite(PWM_LM, PWM_value);

    PORTD |=  (1<<RM1);
    PORTD |=  (1<<LM1);
    PORTD &= ~(1<<RM2);
    PORTD &= ~(1<<LM2);
    
  //}
}

void loop() {
  
//    digitalWrite(RM1, HIGH);
//    digitalWrite(RM2, LOW);
//
//    digitalWrite(LM1, HIGH);
//    digitalWrite(LM2, LOW);

    PORTD |=  (1<<RM1);
    PORTD |=  (1<<LM2);
    PORTD &= ~(1<<RM2);
    PORTD &= ~(1<<LM1);
    delay(2000);

    PORTD &= ~(1<<RM1);
    PORTD &= ~(1<<LM2);
    PORTD |=  (1<<RM2);
    PORTD |=  (1<<LM1);
    delay(2000);

   // PWM_value = PWM_value + 10;
    analogWrite(PWM_RM, PWM_value);
    analogWrite(PWM_LM, PWM_value);

//    digitalWrite(RM1, LOW);
//    digitalWrite(RM2, HIGH);
//
//    digitalWrite(LM1, LOW);
//    digitalWrite(LM2, HIGH);
    
//    PORTD &= ~(1<<RM1) | ~(1<<LM1);
//    PORTD |=  (1<<RM2) |  (1<<LM2);
   
//    delay(5000);
    
}

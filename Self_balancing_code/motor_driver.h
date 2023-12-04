/** Motor Driver **/

#include <avr/io.h>         // Standard AVR IO Library
#include <util/delay.h>     // Standard AVR Delay Library

#define RM1 PD4
#define RM2 PD5

#define LM1 PD6
#define LM2 PD7

#define PWM_RM 45
#define PWM_LM 46.

/**********************************************************************************************/

#define INIT_MOTOR_DR_CTRL_PINS	(DDRD   |=  (1<<PD4)); \
								(DDRD   |=  (1<<PD5)); \
								(DDRD   |=  (1<<PD6)); \
								(DDRD   |=  (1<<PD7)); \
								(PORTD  &= ~(1<<PD4)); \
								(PORTD  &= ~(1<<PD5)); \
								(PORTD  &= ~(1<<PD6)); \
								(PORTD  &= ~(1<<PD7))

/*

#define PIN_RM1_SET_OUT     (DDRD  |=  (1<<PD4))  // Set Output Pin PD4 - Right Motor 1 IN1 
#define PIN_RM2_SET_OUT     (DDRD  |=  (1<<PD5))  // Set Output Pin PD5 - Right Motor 2 IN2

#define PIN_LM1_SET_OUT     (DDRD  |=  (1<<PD6))  // Set Output Pin PD6 - Left Motor 1 IN1
#define PIN_LM2_SET_OUT     (DDRD  |=  (1<<PD7))  // Set Output Pin PD7 - Left Motor 2 IN2

#define PIN_RM_PWM_SET_OUT  (DDRL  |=  (1<<PL4))  // Set Output Pin PL4 - Right Motor PWM
#define PIN_LM_PWM_SET_OUT  (DDRL  |=  (1<<PL3))  // Set Output Pin PL3 - Left Motor PWM

*/

/**********************************************************************************************/

#define PIN_RM1_HIGH    (PORTD  |=  (1<<PD4))  // Set Output HIGH Pin PD4 - Right Motor 1 IN1
#define PIN_RM1_LOW		(PORTD  &= ~(1<<PD4))  // Set Output HIGH Pin PD4 - Right Motor 1 IN1

#define PIN_RM2_HIGH    (PORTD  |=  (1<<PD5))  // Set Output HIGH Pin PD5 - Right Motor 2 IN2
#define PIN_RM2_LOW	    (PORTD  &= ~(1<<PD5))  // Set Output HIGH Pin PD5 - Right Motor 2 IN2

#define PIN_LM1_HIGH    (PORTD  |=  (1<<PD6))  // Set Output HIGH Pin PD6 - Left Motor 1 IN1
#define PIN_LM1_LOW	    (PORTD  &= ~(1<<PD6))  // Set Output HIGH Pin PD6 - Left Motor 1 IN2

#define PIN_LM2_HIGH    (PORTD  |=  (1<<PD7))  // Set Output HIGH Pin PD7 - Left Motor 2 IN2
#define PIN_LM2_LOW	    (PORTD  &= ~(1<<PD7))  // Set Output HIGH Pin PD7 - Left Motor 2 IN


#define PIN_RM_PWM_OUT_HIGH	(PORTL  |=  (1<<PL4))  // Set Output HIGH Pin PL4 - Right Motor PWM
#define PIN_RM_PWM_OUT_LOW	(PORTL  &= ~(1<<PL4))  // Set Output HIGH Pin PL4 - Right Motor PWM

#define PIN_LM_PWM_OUT_HIGH	(PORTL  |=  (1<<PL3))  // Set Output HIGH Pin PL3 - Left Motor PWM
#define PIN_LM_PWM_OUT_LOW  (PORTL  &= ~(1<<PL3))  // Set Output HIGH Pin PL3 - Left Motor PWM

/**********************************************************************************************/

// Motor Direction 1

#define BOT_STOP		PIN_RM1_LOW; \
						PIN_RM2_LOW; \
						PIN_LM1_LOW; \
						PIN_LM2_LOW								

#define BOT_FORWARD		PIN_RM1_HIGH; \
						PIN_RM2_LOW; \
						PIN_LM1_LOW; \
						PIN_LM2_HIGH

#define BOT_BACKWARD	PIN_RM1_LOW; \
						PIN_RM2_HIGH; \
						PIN_LM1_HIGH; \
						PIN_LM2_LOW

#define BOT_LEFT		PIN_RM1_HIGH; \
						PIN_RM2_LOW; \
						PIN_LM1_HIGH; \
						PIN_LM2_LOW

#define BOT_RIGHT		PIN_RM1_LOW; \
						PIN_RM2_HIGH; \
						PIN_LM1_LOW; \
						PIN_LM2_HIGH

#define INIT_MOTOR 		INIT_MOTOR_DR_CTRL_PINS; \
						pinMode(PWM_RM, OUTPUT); \
						pinMode(PWM_LM, OUTPUT); \
						BOT_STOP

/**********************************************************************************************/



  

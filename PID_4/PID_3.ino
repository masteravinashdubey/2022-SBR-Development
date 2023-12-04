#define F_CPU 16000000UL    // Define Crystal Frequency of eYFi-Mega Board
#include <avr/io.h>         // Standard AVR IO Library
#include <util/delay.h>     // Standard AVR Delay Library

#include <stdio.h>
#include <math.h>

#include <Wire.h>
#include <MPU6050_light.h>

#include "motor_driver.h"

#define LED_RED   6
#define LED_BLUE  7
#define LED_GREEN 8

#define LED_RED_ON    digitalWrite(LED_RED, LOW)
#define LED_RED_OFF   digitalWrite(LED_RED, HIGH)

#define LED_BLUE_ON   digitalWrite(LED_BLUE, LOW)
#define LED_BLUE_OFF  digitalWrite(LED_BLUE, HIGH)

#define LED_GREEN_ON  digitalWrite(LED_GREEN, LOW)
#define LED_GREEN_OFF digitalWrite(LED_GREEN, HIGH)

//Limiting Variables
#define MAX_PITCH_CORRECTION (250.0f)
#define MAX_PITCH_AREA (450.0f)
#define MAX_PITCH_RATE (450.0f)

//#define kp 
//#define kd  
//#define ki

MPU6050 mpu(Wire);
unsigned long timer = 0;

uint16_t MAX_PWM = 255;
uint16_t MIN_PWM = 25;

float euler_angle[2], mpu_offset[2] = {0.0, 0.0};
float pitch_angle, pitch_error;
float motor_cmd, motor_pwm = 0.0;
float pitch_cmd = 0.0;

uint8_t PWM_value = 50;
uint8_t kp = 18;
uint8_t kd = 30;
uint8_t ki = 0.008;

// bound((motor_cmd), MIN_PWM, MAX_PWM);
float bound(float motor_cmd, float MIN_PWM1, float MAX_PWM1)
{
  if(motor_cmd >= MIN_PWM1 && motor_cmd <= MAX_PWM1)
  {
    return motor_cmd;
  }
  else if(motor_cmd < MIN_PWM1) 
  {
    return MIN_PWM1;
  }
  else
  {
    return MAX_PWM1;
  }

}

// Calculate the motor inputs according to angle of the MPU
void calculate_motor_command(const float pitch_error, float *motor_cmd)
{
  
  /** Error values **/
  // Stores pitch error of previous iteration
  static float prev_pitch_error = 0.0f;
  // Stores sum of product of errors with time interval obtained during each iteration
  static float pitch_area = 0.0f;
  // Stores difference between current error and previous iteration error
  float pitch_error_difference = 0.0f;

  /** Correction values **/
  // Variables for storing corrected values
  float pitch_correction = 0.0f, absolute_pitch_correction = 0.0f;
  // Helper variables for calculating integral and derivative correction
  float pitch_rate = 0.0f;

  // Variables storing correction values of different error terms
  float P_term = 0.0f, I_term = 0.0f, D_term = 0.0f;

  // Evaluated delta(error)
  pitch_error_difference = pitch_error - prev_pitch_error;

  // Evaluated area of the graph error vs time (cumulative error)
  pitch_area += (pitch_error);
  // evaluated delta(error)/delta(time) to calculate rate of change in error w.r.t time
  pitch_rate = pitch_error_difference;

  // Calculating p,i and d terms my multuplying corresponding proportional constants
  P_term = kp * pitch_error;
  I_term = ki * bound(pitch_area, -MAX_PITCH_AREA, MAX_PITCH_AREA);
  D_term = kd * bound(pitch_rate, -MAX_PITCH_RATE, MAX_PITCH_RATE);

  pitch_correction = P_term + I_term + D_term;

  /**
   * Calculating absolute value of pitch_correction since duty cycle can't be negative. 
   * Since it is a floating point variable, fabsf was used instead of abs
  */
  absolute_pitch_correction = fabsf(pitch_correction);

  *motor_cmd = bound(absolute_pitch_correction, 0, MAX_PITCH_CORRECTION);
  prev_pitch_error = pitch_error;
}




void setup() {

  pinMode(LED_RED, OUTPUT);
  pinMode(LED_BLUE, OUTPUT);
  pinMode(LED_GREEN, OUTPUT);

  LED_RED_ON;
  LED_BLUE_OFF;
  LED_GREEN_OFF;

  INIT_MOTOR;

  //Serial.begin(9600);
  Wire.begin();

  //Serial.println("BOT Started ... Do not move for some time ........... ");

  byte status = mpu.begin();
  //Serial.print(F("MPU6050 status: "));
  //Serial.println(status);
  while(status!=0){ } // stop everything if could not connect to MPU6050
  
  //Serial.println(F("Calculating offsets, do not move MPU6050"));
  delay(500);

  // mpu.upsideDownMounting = true; // uncomment this line if the MPU6050 is mounted upside-down
  mpu.calcOffsets(); // gyro and accelero
  //Serial.println("Done setup!\n");

}

void loop() {

  //Serial.println("loop started");

  LED_RED_OFF;
  LED_BLUE_OFF;
  LED_GREEN_ON;

  mpu.update();

  //Serial.println("loop 2");

  //if((millis()-timer)>10){ // print data every 10ms
  //Serial.print("X : ");
  //float Roll = mpu.getAngleX();
  //Serial.print(mpu.getAngleX());
  //Serial.print("\tY : ");
  float Pitch = mpu.getAngleY();
  //Serial.print(mpu.getAngleY());
  //Serial.print("\tZ : ");
  //float Yaw = mpu.getAngleZ();
  //Serial.println(mpu.getAngleZ());
  //timer = millis();  

  pitch_angle = Pitch;
  pitch_error = pitch_cmd - pitch_angle;
  calculate_motor_command(pitch_error, &motor_cmd);
  motor_pwm = bound((motor_cmd), MIN_PWM, MAX_PWM);


  if (pitch_error > 0.3)
  {
//          // setting motor A0 with definite speed(duty cycle of motor driver PWM) in Backward direction
//          set_motor_speed(MOTOR_A_0, MOTOR_BACKWARD, motor_pwm);
//          // setting motor A1 with definite speed(duty cycle of motor driver PWM) in Backward direction
//          set_motor_speed(MOTOR_A_1, MOTOR_BACKWARD, motor_pwm);

    analogWrite(PWM_RM, abs(motor_pwm));
    analogWrite(PWM_LM, abs(motor_pwm));
      if (pitch_error > 15){
        analogWrite(PWM_RM, 255);
        analogWrite(PWM_LM, 255);
      }


    //Serial.print("motor FORWARD : ");
    //Serial.println(abs(motor_pwm));

    BOT_FORWARD;

    LED_BLUE_OFF;
  }

  // Bot tilts downwards
  else if (pitch_error < -0.3)
  {
//          // setting motor A0 with definite speed(duty cycle of motor driver PWM) in Forward direction
//          set_motor_speed(MOTOR_A_0, MOTOR_FORWARD, motor_pwm);
//          // setting motor A1 with definite speed(duty cycle of motor driver PWM) in Forward direction
//          set_motor_speed(MOTOR_A_1, MOTOR_FORWARD, motor_pwm);

          
    analogWrite(PWM_RM, abs(motor_pwm));
    analogWrite(PWM_LM, abs(motor_pwm));

    //Serial.print("motor BACK : ");
    //Serial.println(abs(motor_pwm));
    if (pitch_error < -15){
       analogWrite(PWM_RM, 255);
       analogWrite(PWM_LM, 255);
    }


    BOT_BACKWARD;

    LED_BLUE_OFF;
  }

        // Bot remains in desired region for vertical balance
  else
  {
//          stopping motor A0
//          set_motor_speed(MOTOR_A_0, MOTOR_STOP, 0);
//          stopping motor A1
//          set_motor_speed(MOTOR_A_1, MOTOR_STOP, 0);
    analogWrite(PWM_RM, 0);
    analogWrite(PWM_LM, 0);

    BOT_STOP;

    LED_BLUE_ON;
    //Serial.println("motor stop .... :)");

  }
  delay(10);
}

  
/* lcd.h
 * Created: 10/07/2018 10:47:03
 * Author: e-Yantra Team
 */

#ifndef _MOTOR_H_
#define _MOTOR_H_



#define MagF1            51                       // INB motor pin  Earlier it was PIN 11
#define MagF2            53                       // INB motor pin  Earlier it was PIN 11

#define InL1            13                      // INA motor pin
#define PWML            10                      // PWM motor pin  Earlier it was PIN 12
#define InL2            9                       // INB motor pin  Earlier it was PIN 11
#define encodPinAL      14                      // encoder A pin
#define encodPinBL      15                      // encoder B pin

volatile int encoderPosAL = 0;                  // left count
volatile int encoderPosAR = 0;                  // left count

float thita_wr=0, thita_wr_dot=0, thita_wr_prev=0;

#define InR1            7                      // INA motor pin
#define PWMR            6                      // PWM motor pin
#define InR2            4                      // INB motor pin 
#define encodPinAR      2                      // encoder A pin
#define encodPinBR      3                      // encoder B pin

float PWM_L=0;   
float PWM_R=0;


void MagPick(void)  
{
  //Serial.print(PWM_val);
  //Serial.print(" ");
  digitalWrite(MagF1, LOW);
  digitalWrite(MagF2, HIGH);
}

void MagDrop(void)  
{
  //Serial.print(PWM_val);
  //Serial.print(" ");
  digitalWrite(MagF1, LOW);
  digitalWrite(MagF2, LOW);
}

void motorForwardL(int PWM_val)  
{
  //Serial.print(PWM_val);
  //Serial.print(" ");
  analogWrite(PWML, PWM_val);
  digitalWrite(InL1, LOW);
  digitalWrite(InL2, HIGH);
}

void motorBackwardL(int PWM_val) 
{
  
  //Serial.print(PWM_val);
  //Serial.print("-");
  analogWrite(PWML, PWM_val);
  digitalWrite(InL1, HIGH);
  digitalWrite(InL2, LOW);
}

void motorForwardR(int PWM_val)  
{
  //Serial.println(PWM_val);
  analogWrite(PWMR, PWM_val);
  digitalWrite(InR1, LOW);
  digitalWrite(InR2, HIGH);
}

void motorBackwardR(int PWM_val) 
{
  
  //Serial.println(PWM_val);
  analogWrite(PWMR, PWM_val);
  digitalWrite(InR1, HIGH);
  digitalWrite(InR2, LOW);
}

void motorsStop(void) 
{
  analogWrite(PWMR, 0);
  analogWrite(PWML, 0);
  digitalWrite(InR1, LOW);
  digitalWrite(InR2, LOW);
  digitalWrite(InL1, LOW);
  digitalWrite(InL2, LOW);
}


/////////////////////////////////////////////////////
/////////////////////////////////////////////////////

// ********* Encoder counting ********//
void rencoderL()  
{                                  
    if(digitalRead(encodPinBL)==HIGH)
    {
         encoderPosAL++;
    }
    else
    {
         encoderPosAL--;
    }

    
    //Serial.println(encoderPosAL);
}
/////////////////////////////////////////////////////
/////////////////////////////////////////////////////

// ********* Encoder counting ********//
void rencoderR()  
{                                  
    if(digitalRead(encodPinBR)==HIGH)
    {
         encoderPosAR--;
    }
    else
    {
         encoderPosAR++;
    }
    thita_wr=0.01427997*encoderPosAR;
    thita_wr_dot=(thita_wr-thita_wr_prev);
    thita_wr_prev=thita_wr;
    //Serial.println(encoderPosAR);
    //Serial.println(encoderPosAR);
}






#endif // MOTOR_H_ // 

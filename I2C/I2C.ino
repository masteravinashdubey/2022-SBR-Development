/*
Connections of Motor Driver and Arduino
Motor       -      Arduino
GND         -      GND
SCL         -      SCL
SDA         -      SDA
*/

#include "Wire.h"
#define I2C_ADDRESS 0x10 >> 1 //Default slave address of this drive is 0x10. Arduino works on 7-bit I2C Addressing, so the Address value is shifted to right by 1 bit.
#define MAX_SPEED 255  //Set Maximum Speed (0-255)
#define CB_0 0 // Command Byte "0" to Read/Write Motor Max Speed 
#define CB_1 1 // Command Byte "1" to Read/Write Motor Speed and Direction

void setup(){
  Wire.begin();
  delay(100);
  Wire.beginTransmission(I2C_ADDRESS);
  Wire.write(CB_0);
  Wire.write(MAX_SPEED); // LSB Byte of Maximum Speed Value
  Wire.write(0); // MSB Byte of Maximum Speed Value
  Wire.endTransmission();
  delay(100);
}

void loop(){
  Wire.beginTransmission(I2C_ADDRESS);  //send the slave address
  Wire.write(CB_1); //send the command variable for speed
  Wire.write(255); //LSB Byte of Speed to be set
  Wire.write(0); //MSB Byte of Speed to be set
  Wire.endTransmission(); //send I2C stop 
  delay(1000);

  Wire.beginTransmission(I2C_ADDRESS);  //send the slave address
  Wire.write(CB_1); //send the command variable for speed
  Wire.write(1); //LSB Byte of Speed to be set
  Wire.write(255); //MSB Byte of Speed to be set
  Wire.endTransmission(); //send I2C stop 
  delay(1000);
}

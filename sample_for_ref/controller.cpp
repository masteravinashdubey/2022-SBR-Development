/*
* Project Name: Balance_Bot_2403
* File Name: controller.cpp
*
* Created: 13-Feb-17 8:25:05 PM
* Author: Heethesh Vhavle
*
* Team: eYRC-BB#2403
* Theme: Balance Bot
*
* Library for the Joystick Controller
*
* Functions: read_joystick, get_joystick_zone
* Global Variables: joystick
*/

#include <Arduino.h>
#include "controller.h"
float offsetaddtemp=0.0;
unsigned char remret=0;
// Structure Initialization
JoystickController joystick = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

/**********************************
Function name	:	get_joystick_zone
Functionality	:	To map the Joystick ADC input values to 5 zones (�2, �1, 0)
Arguments		:	Low and High ADC bytes, Offset ADC value
Return Value	:	Zone value
Example Call	:	get_joystick_zone(255, 10, 22)
***********************************/
int get_joystick_zone(unsigned char low_byte, unsigned char high_byte, int offset=0)
{
	int output=0, zone=0;
	
	// Combine bytes and constrain input
	output = constrain((high_byte*255 + low_byte - offset), 0, 1023);
	
	// Map values to -100 and 100 range
	output = map(output, 0, 1023, -100, 100);
	
	// Assign different zone based on the magnitude of the joystick position
	// �2 -> Extreme ends, 0 -> Neutral position
	if (abs(output)<=20) zone = 0;
	else if ((abs(output)>20) && (abs(output)<=60)) zone = 1;
	else if ((abs(output)>60) && (abs(output)<=100)) zone = 2;
	
	if (output<0) zone *= -1;
	return zone;
}

/**********************************
Function name	:	read_joystick
Functionality	:	To read the data from the controller XBee Module and store it
Arguments		:	None
Return Value	:	None
Example Call	:	read_joystick()
***********************************/
unsigned char read_joystick()
{
      
	int sum = 131;
	unsigned char byte_discard, checksum;
	unsigned char digital_data = 0;
	unsigned char AD0[2]={0, 0}, AD1[2]={0, 0};
	
	// Check frame length
	if (Serial1.available() > 18)
	{
		// Check Start Byte of the API Frame
		if (Serial1.read() == 0x7E)
		{
			// Read and discard 2 bytes
			for (int i=0; i<2; i++)
			byte_discard = Serial1.read();
      
      
			// Check Frame Type
			if(Serial1.read() != 0x83) return;
			
			// Read and discard 8 bytes
			for (int i=0; i<8; i++)
			sum += Serial1.read();
			
			// Read Digital Byte
			digital_data = Serial1.read();
			sum += digital_data;
			
			// Read ADC Data
			AD0[1] = Serial1.read();
			AD0[0] = Serial1.read();
			AD1[1] = Serial1.read();
			AD1[0] = Serial1.read();
			sum = sum + AD0[0] + AD0[1] + AD1[0] + AD1[1];
			
			// Read Checksum
			checksum = 0xFF - (0xFF & (unsigned char)sum);
			byte_discard = Serial1.read();
			
			// Discard frame if checksum does not match
			if (byte_discard != checksum) return;
			
			// Update Controller Variables
			if ((digital_data & 0x20) == 0x20) joystick.button_1 = HIGH;
			else joystick.button_1 = LOW;
			
			if ((digital_data & 0x08) == 0x08) joystick.button_2 = HIGH;
			else joystick.button_2 = LOW;
			
			if ((digital_data & 0x10) == 0x10) joystick.button_3 = HIGH;
			else joystick.button_3 = LOW;
			
			if ((digital_data & 0x04) == 0x04) joystick.button_4 = HIGH;
			else joystick.button_4 = LOW;
			
			// Map ADC values to Zones
			joystick.x_position = get_joystick_zone(AD1[0], AD1[1], 9);
			joystick.y_position = get_joystick_zone(AD0[0], AD0[1], 9);
     
     if(joystick.y_position==0)
     {
      remret|=0x04;
      remret&=0xF7;
     }
     if(joystick.y_position==-2)
     {
      remret|=0x08;
      remret&=0xFB;
     }
     if(joystick.x_position==0)
     {
      remret|=0x01;
      remret&=0xFD;
     }
     if(joystick.x_position==-2)
     {
      remret|=0x02;
      remret&=0xFE;
     }
     if(joystick.button_1==1)
     {
      remret|=0x80;
     }
     if(joystick.button_1==0)
     {
      remret&=0x7F;
     }
     if(joystick.x_position==2 && joystick.y_position==2)
     {
      remret&=0x80;
     }
      
    }
  }
 return remret;
}

#include <stdint.h>
#include <avr/io.h>
#include <util/twi.h>
#include <util/delay.h>
#include <avr/pgmspace.h>

//#include "OV7670.h"
#include "OV7670.c"

#define F_CPU 16000000UL

void setup(){
  arduinoUnoInut();
  camInit();
  setResolution();
  setColor();
 writeReg(0x11, 10); //Earlier it had the value:writeReg(0x11, 12); New version works better for me :) !!!!
}

void loop(){
  captureImg(320, 240);
}

/*
Connections of Motor Driver and Arduino
Motor       -      Arduino
GND         -      GND
RXD         -      RX1
TXD         -      TX1
*/
  
//#include <SoftwareSerial.h>
//SoftwareSerial mySerial(10, 11);  //(Rx,Tx)

void setup() {
Serial.begin(9600);           
Serial1.begin(9600);        
delay(1000);
//Serial1.print("E10\r");    //Read/Write I2C address (0-127)
Serial1.print("D0\r");    //Read/Write I2C address (0-127)
}

void loop() { 
 
//  if (Serial1.available()) {
//    Serial.write(Serial1.read());
/// }
 Serial1.print("S255\r");   //Read/Write Motor Speed and Direction (-255 to 255)
 Serial.print("S255\r\n");   //Read/Write Motor Speed and Direction (-255 to 255)
  delay(5000);
  Serial1.print("S-255\r");   //Read/Write Motor Speed and Direction (-255 to 255
  Serial.print("S-255\r\n");   //Read/Write Motor Speed and Direction (-255 to 255)
  delay(5000);
  //mySerial.print("M50\r");  //Read/Write Motor Max Speed (0-255)
  //mySerial.print("D20\r");  //Read/Write Speed Damping (0-255)
  //mySerial.print("E\r");    //Read/Write I2C address (0-127)
  //mySerial.print("Y\r");    //Load Default Values of Speed, Max and Damp
}

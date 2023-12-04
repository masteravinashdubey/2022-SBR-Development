#include <Wire.h>

void setup() {
  // put your setup code here, to run once:
  Wire.begin();
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  Wire.beginTransmission(0x20);
  Wire.write(0x12);
  Wire.endTransmission();
  Wire.requestFrom(0x20,1);
  while(Wire.available()) {
        Serial.print(Wire.read());
        Serial.print(" ");
    }
  Serial.println(" | ");
  delay(100);
}

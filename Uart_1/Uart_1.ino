

void setup() {
  Serial.begin(9600);
  Serial1.begin(9600);
  delay(1000);
  // Check for received Characters from the computer
  if (Serial.available())
  {
   // Write what is received to the default serial port
  Serial1.write(Serial.read());

  }
}

void loop() {
  
}

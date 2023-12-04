
// These constants won't change. They're used to give names to the pins used:
//const int lowestPin = 2;
//const int highestPin = 13;

#define RM1 PD4
#define RM2 PD7
#define LM1 PD5
#define LM2 PD6

#define PWM_RM PL3
#define PWM_LM PL4

void setup() {
  // set pins 2 through 13 as outputs:
  //for (int thisPin = lowestPin; thisPin <= highestPin; thisPin++) {
    pinMode(RM1, OUTPUT);
    pinMode(RM2, OUTPUT);
    pinMode(LM1, OUTPUT);
    pinMode(LM2, OUTPUT);
    pinMode(PL3, OUTPUT);
    pinMode(PL4, OUTPUT);
    analogWrite(PWM_RM, 80);
    analogWrite(PWM_LM, 80);
//     digitalWrite(PWM_RM, HIGH);
//     digitalWrite(PWM_LM, HIGH);
  //}
}

void loop() {
  digitalWrite(RM1, HIGH);
  digitalWrite(LM1, HIGH);
  digitalWrite(RM2, LOW);
  digitalWrite(LM2, LOW);
  delay(5000);
  digitalWrite(RM1, LOW);
  digitalWrite(LM1, LOW);
  digitalWrite(RM2, HIGH);
  digitalWrite(LM2, HIGH);
  delay(5000);
  
}

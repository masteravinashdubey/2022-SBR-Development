


#include <Arduino.h>
// I2Cdev and MPU6050 must be installed as libraries, or else the .cpp/.h files
// for both classes must be in the include path of your project
#include "I2Cdev.h"
#include "MPU6050.h"

// Arduino Wire library is required if I2Cdev I2CDEV_ARDUINO_WIRE implementation
// is used in I2Cdev.h
#if I2CDEV_IMPLEMENTATION == I2CDEV_ARDUINO_WIRE
    #include "Wire.h"
#endif

// class default I2C address is 0x68
// specific I2C addresses may be passed as a parameter here
// AD0 low = 0x68 (default for InvenSense evaluation board)
// AD0 high = 0x69
MPU6050 accelgyro;
//MPU6050 accelgyro(0x69); // <-- use for AD0 high

#include "motor.h"
#include "controller.h"

int16_t ax, ay, az;
int16_t gx, gy, gz;
float offsetadd=0.0;
float K[4]= {0,-219.2228, 0, -29.8880};        //{-1.9709,-809.35,-5.3481,-142.04}//{-1,-443.67,-3.3098,-44.833};  //-0.61644,-73.121,-1.2939,-15.179};
float thita=0, thita_dot=0, thita_prev=0, gyro_angle=0, accel_angle=0, turn=0, offset = 01.1;
float AX=0, AY=0, AZ=0, GX=0, GY=0, GZ=0;
unsigned char readremote=0;
// uncomment "OUTPUT_READABLE_ACCELGYRO" if you want to see a tab-separated
// list of the accel X/Y/Z and then gyro X/Y/Z values in decimal. Easy to read,
// not so easy to parse, and slow(er) over UART.
#define OUTPUT_READABLE_ACCELGYRO

// uncomment "OUTPUT_BINARY_ACCELGYRO" to send all 6 axes of data as 16-bit
// binary, one right after the other. This is very fast (as fast as possible
// without compression or data loss), and easy to parse, but impossible to read
// for a human.
//#define OUTPUT_BINARY_ACCELGYRO


bool blinkState = false;

void motor_init()
{
    pinMode(MagF1, OUTPUT);
    pinMode(MagF2, OUTPUT);
    
    pinMode(InL1, OUTPUT);
    pinMode(InL2, OUTPUT);
    pinMode(PWML, OUTPUT);
    pinMode(encodPinAL, INPUT);
    pinMode(encodPinBL, INPUT);
    digitalWrite(encodPinAL, HIGH);               // turn on pullup resistor
    digitalWrite(encodPinBL, HIGH);               // turn on pullup resistor
    //attachInterrupt(digitalPinToInterrupt(14), rencoderL, RISING);        // 18 as int
    
    pinMode(InR1, OUTPUT);
    pinMode(InR2, OUTPUT);
    pinMode(PWMR, OUTPUT);
    pinMode(encodPinAR, INPUT);
    pinMode(encodPinBR, INPUT);
    digitalWrite(encodPinAR, HIGH);               // turn on pullup resistor
    digitalWrite(encodPinBR, HIGH);
    attachInterrupt(digitalPinToInterrupt(2), rencoderR, RISING);        // 2 as int
}

void setup() {
    // join I2C bus (I2Cdev library doesn't do this automatically)
    #if I2CDEV_IMPLEMENTATION == I2CDEV_ARDUINO_WIRE
        Wire.begin();
    #elif I2CDEV_IMPLEMENTATION == I2CDEV_BUILTIN_FASTWIRE
        Fastwire::setup(400, true);
    #endif

    // initialize serial communication
    // (38400 chosen because it works as well at 8MHz as it does at 16MHz, but
    // it's really up to you depending on your project)
  Serial.begin(19200); // opens serial port, sets data rate to 9600 bps
  Serial1.begin(9600); // opens serial1 port, sets data rate to 9600 bps

    // initialize device
    Serial.println("Initializing I2C devices...");
    accelgyro.initialize();

    // verify connection
    Serial.println("Testing device connections...");
    Serial.println(accelgyro.testConnection() ? "MPU6050 connection successful" : "MPU6050 connection failed");
     pinMode(LED_BUILTIN, OUTPUT);
    // use the code below to change accel/gyro offset values
    /*
    Serial.println("Updating internal sensor offsets...");
    // -76	-2359	1688	0	0	0
    Serial.print(accelgyro.getXAccelOffset()); Serial.print("\t"); // -76
    Serial.print(accelgyro.getYAccelOffset()); Serial.print("\t"); // -2359
    Serial.print(accelgyro.getZAccelOffset()); Serial.print("\t"); // 1688
    Serial.print(accelgyro.getXGyroOffset()); Serial.print("\t"); // 0
    Serial.print(accelgyro.getYGyroOffset()); Serial.print("\t"); // 0
    Serial.print(accelgyro.getZGyroOffset()); Serial.print("\t"); // 0
    Serial.print("\n");
    accelgyro.setXGyroOffset(220);
    accelgyro.setYGyroOffset(76);
    accelgyro.setZGyroOffset(-85);
    Serial.print(accelgyro.getXAccelOffset()); Serial.print("\t"); // -76
    Serial.print(accelgyro.getYAccelOffset()); Serial.print("\t"); // -2359
    Serial.print(accelgyro.getZAccelOffset()); Serial.print("\t"); // 1688
    Serial.print(accelgyro.getXGyroOffset()); Serial.print("\t"); // 0
    Serial.print(accelgyro.getYGyroOffset()); Serial.print("\t"); // 0
    Serial.print(accelgyro.getZGyroOffset()); Serial.print("\t"); // 0
    Serial.print("\n");
    */
    motor_init();
    Serial.println("Motor_init_finish");

    timer5_init();
   // timer3_init();
    //timer1_init();
    Serial.println("timer_init");
}

void loop() {
    // read raw accel/gyro measurements from device
    ///accelgyro.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);

    // these methods (and a few others) are also available
    //accelgyro.getAcceleration(&ax, &ay, &az);
    //accelgyro.getRotation(&gx, &gy, &gz);

        // display tab-separated accel/gyro x/y/z values
        /*Serial.print("a/g:\t");
        Serial.print(KAPingX); Serial.print("\t");
        Serial.print(ay/16384.0); Serial.print("\t");
        Serial.print(az/16384.0); Serial.print("\t");
        Serial.print(gx/131.0); Serial.print("\t");
        Serial.print(gy/131.0); Serial.print("\t");
        Serial.println(gz/131.0);
        */

}


void motor_guide()
{
  //PWM_L= -1*((K[1]*(thita+offset)*10.6/12)+(K[3]*10*10.6/12*thita_dot))+0*error_int; ///old equation with shashi sir

  PWM_L= -255*((K[1]*(thita+offset+offsetadd)*(3.14/180))+(K[3]*thita_dot*(3.14/180)))/12;  ///only body parameters considered
  
  //PWM_L=255*((K[0]*thita_wr)+(K[2]*thita_wr_dot)+(K[1]*(thita+offset+offsetadd)*(3.14/180))+(K[3]*thita_dot*(3.14/180)))/12;  ///wheel and body parameters considered
  
  PWM_R= 1*PWM_L;
  
//  Serial.print("PWM_R "); 
//  Serial.println(PWM_R); 
  /*
  Serial.println(PWM_R); 
  */
   
    if (PWM_L<0)
    {
        if ((-1*PWM_L)>=255) 
        {
            PWM_L=-255;
            PWM_R=-255;
        }
        motorBackwardL(abs(PWM_L+turn));
        motorBackwardR(abs(PWM_R-turn));
//        Serial.println("back "); 
    } 
    
    else if (PWM_L>0)
    {
        if (PWM_L>=255) 
        {
            PWM_L=255;
            PWM_R=255;
        }
        motorForwardL(abs(PWM_L+turn));
        motorForwardR(abs(PWM_R-turn));
//        Serial.println("for "); 
    }
    
    else
    {
        motorsStop();
    } 
}


void timer5_init()
{
    cli(); //Clears the global interrupts
    TIMSK5 = 0x01; //timer5 overflow interrupt enable
    TCCR5B = 0x00; //stop
    TCNT5H = 0xCF; //Counter higher 8 bit value
    TCNT5L = 0x2B; //Counter lower 8 bit value
    TCCR5A = 0x00;
    TCCR5C = 0x00;
    TCCR5B = 0x03; //start Timer, prescaler 64
    sei();   //Enables the global interrupts
}

void timer3_init()
{
    cli(); //Clears the global interrupts
    TIMSK3 = 0x01; //timer5 overflow interrupt enable
    TCCR3B = 0x00; //stop
    TCNT3H = 0xA2; //Counter higher 8 bit value
    TCNT3L = 0x3F; //Counter lower 8 bit value
    TCCR3A = 0x00;
    TCCR3C = 0x00;
    TCCR3B = 0x02; //start Timer, prescaler 8
    sei();   //Enables the global interrupts
}

void timer1_init()
{
    cli(); //Clears the global interrupts
    TIMSK1 = 0x01; //timer5 overflow interrupt enable
    TCCR1B = 0x00; //stop
    TCNT1H = 0xA2; //Counter higher 8 bit value
    TCNT1L = 0x3F; //Counter lower 8 bit value
    TCCR1A = 0x00;
    TCCR1C = 0x00;
    TCCR1B = 0x02; //start Timer, prescaler 8
    sei();   //Enables the global interrupts
}
ISR (TIMER1_OVF_vect)
{
    TCNT1H = 0xA2; //Counter higher 8 bit value
    TCNT1L = 0x3F; //Counter lower 8 bit value

    //Serial.println(millis());
}  

ISR (TIMER3_OVF_vect)
{
    
    TCNT3H = 0xA2; //Counter higher 8 bit value
    TCNT3L = 0x3F; //Counter lower 8 bit value
    readremote = read_joystick();
    if((readremote&0x08)==0x08)
    {
      turn=25;
    }
    if((readremote&0x04)==0x04)
    {
      turn=-25;
    }
    if((readremote&0x0F)==0x00)
    {
      turn=0;
    }
    //Serial.println(offsetadd);
    /*
    if (Serial1.available() > 0) {
    // read the incoming byte:
    incomingByte = Serial1.read();
    
    // say what you got:
    Serial.print("I received: ");
    Serial.println(incomingByte);
    }
    
    */
 
}

    
ISR (TIMER5_OVF_vect)
{
    sei();  
    TCNT5H = 0xCF; //Counter higher 8 bit value
    TCNT5L = 0x2B; //Counter lower 8 bit value
    accelgyro.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);
    cli();
    GY = (float)gy/131.0;
    AX = (float)ax/16384.0;
    AZ = (float)az/16384.0;
    gyro_angle = GY*0.002 + thita;
    accel_angle = (atan2(-AX, AZ)*180.0)/3.1416;
    thita=(0.997*gyro_angle + (1-0.997)*accel_angle);
    thita_dot=(thita-thita_prev);
    thita_prev=thita;
    //motor_guide();
//    Serial.print("  ");

//    Serial.println(millis());
    if (thita>0)
    {
      digitalWrite(LED_BUILTIN, HIGH);
    }
    else
    {
      digitalWrite(LED_BUILTIN, LOW);
    }
}  

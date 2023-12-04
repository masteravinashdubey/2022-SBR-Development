
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SH110X.h>

#include "I2Cdev.h"
#include "MPU6050.h"
#include "HMC5883L.h"
#include "BMP085.h"
//#include "Wire.h"



/* Uncomment the initialize the I2C address , uncomment only one, If you get a totally blank screen try the other*/
#define i2c_Address 0x3c //initialize with the I2C addr 0x3C Typically eBay OLED's
//#define i2c_Address 0x3d //initialize with the I2C addr 0x3D Typically Adafruit OLED's

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
#define OLED_RESET -1   //   QT-PY / XIAO
Adafruit_SH1106G display = Adafruit_SH1106G(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

//#include "eyes.h"
#define NUMFLAKES 10
#define XPOS 0
#define YPOS 1
#define DELTAY 2


#define LOGO16_GLCD_HEIGHT 16
#define LOGO16_GLCD_WIDTH  16


static const char LED = 6;
static const float ACCEL_SENS = 16384.0; // Accel Sensitivity with default +/- 2g scale
static const float GYRO_SENS  = 131.0;   // Gyro Sensitivity with default +/- 250 deg/s scale

// Magnetometer class default I2C address is 0x1E
// specific I2C addresses may be passed as a parameter here
// this device only supports one I2C address (0x1E)
HMC5883L mag;
int16_t mx, my, mz;

// Accel/Gyro class default I2C address is 0x68 (can be 0x69 if AD0 is high)
// specific I2C addresses may be passed as a parameter here
MPU6050 accelgyro;

int16_t ax, ay, az;
int16_t gx, gy, gz;

// Barometer class default I2C address is 0x77
// specific I2C addresses may be passed as a parameter here
// (though the BMP085 supports only one address)
BMP085 barometer;

float temperature;
float pressure;
int32_t lastMicros;




static const unsigned char PROGMEM logo16_glcd_bmp[] =
{ B00000000, B11000000,
  B00000001, B11000000,
  B00000001, B11000000,
  B00000011, B11100000,
  B11110011, B11100000,
  B11111110, B11111000,
  B01111110, B11111111,
  B00110011, B10011111,
  B00011111, B11111100,
  B00001101, B01110000,
  B00011011, B10100000,
  B00111111, B11100000,
  B00111111, B11110000,
  B01111100, B11110000,
  B01110000, B01110000,
  B00000000, B00110000
};


void setup()
{
  boolean state = HIGH;
  unsigned int count = 0;
  
  pinMode(LED, OUTPUT);
  
  Serial.begin(9600);
  while (!Serial && (count < 30) )
  {
    delay(200); // Wait for serial port to connect with timeout. Needed for native USB
    digitalWrite(LED, state);
    state = !state;
    count++;
  }

  digitalWrite(LED, HIGH);

  // join I2C bus (I2Cdev library doesn't do this automatically)
  Wire.begin();

  display.begin(i2c_Address, true); // Address 0x3C default
 //display.setContrast (0); // dim display
 
  display.display();
//  delay(2000);
//
//  // Clear the buffer.
//  display.clearDisplay();
//
  // ==================== MPU6050 ============================
  accelgyro.initialize();
  Serial.print("Testing Accel/Gyro... ");
  Serial.println(accelgyro.testConnection() ? "MPU6050 connection successful" : "MPU6050 connection failed");

  // Starts up with accel +/- 2 g and gyro +/- 250 deg/s scale
  accelgyro.setI2CBypassEnabled(true); // set bypass mode
  // Now we can talk to the HMC5883l

  // ==================== HMC5883L ============================
  mag.initialize();
  Serial.print("Testing Mag...  ");
  Serial.println(mag.testConnection() ? "HMC5883L connection successful" : "HMC5883L connection failed");

  // ==================== BMP085 ============================
  barometer.initialize();
  Serial.print("Testing Pressure...  ");
  Serial.println(barometer.testConnection() ? "BMP085 connection successful" : "BMP085 connection failed");

  Serial.println("Setup Complete");

}

void loop()
{
  static unsigned long ms = 0;
  static boolean state = HIGH;
  
/******************************************************************************************/
//  // Clear the buffer.
//  display.clearDisplay();
//
//  
//  display.display();
//  //delay(2000);
//  display.clearDisplay();

//  // draw many lines
//  testdrawline();
//  display.display();
//  //delay(2000);
//  display.clearDisplay();

//  // draw rectangles
//  testdrawrect();
//  display.display();
//  //delay(2000);
//  display.clearDisplay();

//  // draw multiple rectangles
//  testfillrect();
//  display.display();
//  //delay(2000);
//  display.clearDisplay();

//  // draw mulitple circles
//  testdrawcircle();
//  display.display();
//  //delay(2000);
//  display.clearDisplay();

//  // draw a SH110X_WHITE circle, 10 pixel radius
//  display.fillCircle(display.width() / 2, display.height() / 2, 10, SH110X_WHITE);
//  display.display();
//  //delay(2000);
//  display.clearDisplay();

//  testdrawroundrect();
//  //delay(2000);
//  display.clearDisplay();
//
//  testfillroundrect();
//  //delay(2000);
//  display.clearDisplay();
//
//  testdrawtriangle();
//  //delay(2000);
//  display.clearDisplay();
//
//  testfilltriangle();
//  //delay(2000);
//  display.clearDisplay();
//
//  // draw the first ~12 characters in the font
//  testdrawchar();
//  display.display();
//  //delay(2000);
//  display.clearDisplay();



//  // text display tests
//  display.setTextSize(1);
//  display.setTextColor(SH110X_WHITE);
//  display.setCursor(0, 0);
//  display.println("Failure is always an option");
//  display.setTextColor(SH110X_BLACK, SH110X_WHITE); // 'inverted' text
//  display.println(3.141592);
//  display.setTextSize(2);
//  display.setTextColor(SH110X_WHITE);
//  display.print("0x"); display.println(0xDEADBEEF, HEX);
//  display.display();
//  //delay(2000);
//  display.clearDisplay();

//  // miniature bitmap display
//  display.drawBitmap(30, 16,  logo16_glcd_bmp, 16, 16, 1);
//  display.display();
//  //delay(1);
//
//  // invert the display
//  display.invertDisplay(true);
//  //delay(1000);
//  display.invertDisplay(false);
// // delay(1000);
//  display.clearDisplay();

 // draw a bitmap icon and 'animate' movement
  testdrawbitmap(logo16_glcd_bmp, LOGO16_GLCD_HEIGHT, LOGO16_GLCD_WIDTH);

  
/************************************************************************************************/
  // Serial Output Format
  // === Accel === | === Gyro === | ======= Mag ======= | === Barometer === |
  //   X   Y   Z   |  X   Y   Z   |  X   Y   Z  Heading |  Temp   Pressure  |

  if (millis() - ms > 100)
  {
    // read raw accel/gyro measurements
    accelgyro.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);

    // display tab-separated accel/gyro x/y/z values
    Serial.print(ax/ACCEL_SENS); Serial.print("\t");
    Serial.print(ay/ACCEL_SENS); Serial.print("\t");
    Serial.print(az/ACCEL_SENS); Serial.print("\t");
    Serial.print(gx/GYRO_SENS); Serial.print("\t");
    Serial.print(gy/GYRO_SENS); Serial.print("\t");
    Serial.print(gz/GYRO_SENS); Serial.print("\t");

    // read raw heading measurements
    mag.getHeading(&mx, &my, &mz);

    // display tab-separated mag x/y/z values
    Serial.print(mx); Serial.print("\t");
    Serial.print(my); Serial.print("\t");
    Serial.print(mz); Serial.print("\t");
    
    // To calculate heading in degrees. 0 degree indicates North
    float heading = atan2(my, mx);
    if(heading < 0) heading += 2 * M_PI;
    Serial.print(heading * 180/M_PI); Serial.print("\t");

    // request temperature
    barometer.setControl(BMP085_MODE_TEMPERATURE);
    
    // wait appropriate time for conversion (4.5ms delay)
    lastMicros = micros();
    while (micros() - lastMicros < barometer.getMeasureDelayMicroseconds());

    // read calibrated temperature value in degrees Celsius
    temperature = barometer.getTemperatureC();

    // request pressure (3x oversampling mode, high detail, 23.5ms delay)
    barometer.setControl(BMP085_MODE_PRESSURE_3);
    while (micros() - lastMicros < barometer.getMeasureDelayMicroseconds());

    // read calibrated pressure value in Pascals (Pa)
    pressure = barometer.getPressure();

    // display measured values if appropriate
    Serial.print(temperature); Serial.print("\t");
    Serial.print(pressure/100); Serial.println("\t");

    Wire.beginTransmission(0x20);
    Wire.write(0x12);
    Wire.endTransmission();
    Wire.requestFrom(0x20,1);
    while(Wire.available()) {
      Serial.print(Wire.read());
      Serial.print(" ");
    }
    Serial.println(" | ");
    ms = millis();
    Serial.println(ms);
    digitalWrite(LED, state);
    state = !state;
  }
}

void testdrawbitmap(const uint8_t *bitmap, uint8_t w, uint8_t h) {
  uint8_t icons[NUMFLAKES][3];

  // initialize
  for (uint8_t f = 0; f < NUMFLAKES; f++) {
    icons[f][XPOS] = random(display.width());
    icons[f][YPOS] = 0;
    icons[f][DELTAY] = random(5) + 1;

    Serial.print("x: ");
    Serial.print(icons[f][XPOS], DEC);
    Serial.print(" y: ");
    Serial.print(icons[f][YPOS], DEC);
    Serial.print(" dy: ");
    Serial.println(icons[f][DELTAY], DEC);
  }

  //while (1) {
    // draw each icon
    for (uint8_t f = 0; f < NUMFLAKES; f++) {
      display.drawBitmap(icons[f][XPOS], icons[f][YPOS], bitmap, w, h, SH110X_WHITE);
    }
    display.display();
    delay(200);

    // then erase it + move it
    for (uint8_t f = 0; f < NUMFLAKES; f++) {
      display.drawBitmap(icons[f][XPOS], icons[f][YPOS], bitmap, w, h, SH110X_BLACK);
      // move it
      icons[f][YPOS] += icons[f][DELTAY];
      // if its gone, reinit
      if (icons[f][YPOS] > display.height()) {
        icons[f][XPOS] = random(display.width());
        icons[f][YPOS] = 0;
        icons[f][DELTAY] = random(5) + 1;
      }
    }
  //}
}

/*
void testdrawchar(void) {
  display.setTextSize(1);
  display.setTextColor(SH110X_WHITE);
  display.setCursor(0, 0);

  for (uint8_t i = 0; i < 168; i++) {
    if (i == '\n') continue;
    display.write(i);
    if ((i > 0) && (i % 21 == 0))
      display.println();
  }
  display.display();
  delay(1);
}

void testdrawcircle(void) {
  for (int16_t i = 0; i < display.height(); i += 2) {
    display.drawCircle(display.width() / 2, display.height() / 2, i, SH110X_WHITE);
    display.display();
    delay(1);
  }
}

void testfillrect(void) {
  uint8_t color = 1;
  for (int16_t i = 0; i < display.height() / 2; i += 3) {
    // alternate colors
    display.fillRect(i, i, display.width() - i * 2, display.height() - i * 2, color % 2);
    display.display();
    delay(1);
    color++;
  }
}

void testdrawtriangle(void) {
  for (int16_t i = 0; i < min(display.width(), display.height()) / 2; i += 5) {
    display.drawTriangle(display.width() / 2, display.height() / 2 - i,
                         display.width() / 2 - i, display.height() / 2 + i,
                         display.width() / 2 + i, display.height() / 2 + i, SH110X_WHITE);
    display.display();
    delay(1);
  }
}

void testfilltriangle(void) {
  uint8_t color = SH110X_WHITE;
  for (int16_t i = min(display.width(), display.height()) / 2; i > 0; i -= 5) {
    display.fillTriangle(display.width() / 2, display.height() / 2 - i,
                         display.width() / 2 - i, display.height() / 2 + i,
                         display.width() / 2 + i, display.height() / 2 + i, SH110X_WHITE);
    if (color == SH110X_WHITE) color = SH110X_BLACK;
    else color = SH110X_WHITE;
    display.display();
    delay(1);
  }
}

void testdrawroundrect(void) {
  for (int16_t i = 0; i < display.height() / 2 - 2; i += 2) {
    display.drawRoundRect(i, i, display.width() - 2 * i, display.height() - 2 * i, display.height() / 4, SH110X_WHITE);
    display.display();
    delay(1);
  }
}

void testfillroundrect(void) {
  uint8_t color = SH110X_WHITE;
  for (int16_t i = 0; i < display.height() / 2 - 2; i += 2) {
    display.fillRoundRect(i, i, display.width() - 2 * i, display.height() - 2 * i, display.height() / 4, color);
    if (color == SH110X_WHITE) color = SH110X_BLACK;
    else color = SH110X_WHITE;
    display.display();
    delay(1);
  }
}

void testdrawrect(void) {
  for (int16_t i = 0; i < display.height() / 2; i += 2) {
    display.drawRect(i, i, display.width() - 2 * i, display.height() - 2 * i, SH110X_WHITE);
    display.display();
    delay(1);
  }
}*/
/*
void testdrawline() {
  for (int16_t i = 0; i < display.width(); i += 4) {
    display.drawLine(0, 0, i, display.height() - 1, SH110X_WHITE);
    display.display();
    delay(1);
  }
  for (int16_t i = 0; i < display.height(); i += 4) {
    display.drawLine(0, 0, display.width() - 1, i, SH110X_WHITE);
    display.display();
    delay(1);
  }
  delay(250);

  display.clearDisplay();
  for (int16_t i = 0; i < display.width(); i += 4) {
    display.drawLine(0, display.height() - 1, i, 0, SH110X_WHITE);
    display.display();
    delay(1);
  }
  for (int16_t i = display.height() - 1; i >= 0; i -= 4) {
    display.drawLine(0, display.height() - 1, display.width() - 1, i, SH110X_WHITE);
    display.display();
    delay(1);
  }
  delay(250);

  display.clearDisplay();
  for (int16_t i = display.width() - 1; i >= 0; i -= 4) {
    display.drawLine(display.width() - 1, display.height() - 1, i, 0, SH110X_WHITE);
    display.display();
    delay(1);
  }
  for (int16_t i = display.height() - 1; i >= 0; i -= 4) {
    display.drawLine(display.width() - 1, display.height() - 1, 0, i, SH110X_WHITE);
    display.display();
    delay(1);
  }
  delay(250);

  display.clearDisplay();
  for (int16_t i = 0; i < display.height(); i += 4) {
    display.drawLine(display.width() - 1, 0, 0, i, SH110X_WHITE);
    display.display();
    delay(1);
  }
  for (int16_t i = 0; i < display.width(); i += 4) {
    display.drawLine(display.width() - 1, 0, i, display.height() - 1, SH110X_WHITE);
    display.display();
    delay(1);
  }
  delay(250);
}*/

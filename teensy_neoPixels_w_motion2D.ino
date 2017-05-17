#include <Adafruit_NeoPixel.h>
#include <NXPMotionSense.h>
#include <Wire.h>
#include <EEPROM.h>

#include "ledMatrix.h"

// NeoPixels
#define PIN 6
// #define NUMLEDS 50
//#define NUMLEDS 16
//Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUMLEDS, PIN, NEO_GRB + NEO_KHZ800);

//Motion
NXPMotionSense imu;
NXPSensorFusion filter;
float ax, ay, az;
float gx, gy, gz;
float mx, my, mz;
float roll, pitch, heading;


// Timer
long lastTime = 0;
long timeStep = 500;

// Led Strip
int ledIndex = 0;
int numLeds = NUMLEDS; // 50 - 1
//int matrix[4][4] = [[0,0,0,0],[0,0,0,0],[0,0,0,0],[0,0,0,0]];

// drawMarix

// translation functions


void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Serial.println("Starting...");

  // Motion
  imu.begin();
  filter.begin(100);

  // NeoPixels
//  strip.begin();
//  strip.setBrightness(64);
  //strip.show();
  

}
LedMatrix ledMatrix = LedMatrix(4,4,PIN);

//void testStripPins () {
//  for(int row = 0; row < 4; row++ ) {
//      for(int col = 0; col < 4; col++) {
//        LedMatrix.cells[row][col]
// 
//      }
//    }
//}

void readMotion() {
  if (imu.available()) {
    imu.readMotionSensor(ax, ay, az, gx, gy, gz, mx, my, mz);
    filter.update(gx, gy, gz, ax, ay, az, mx, my, mz);

    roll = filter.getRoll();
    pitch = filter.getPitch();
    heading = filter.getYaw();
//    Serial.print("Orientation: ");
//    Serial.print(heading);
//    Serial.print(" ");
//    Serial.print(pitch);
//    Serial.print(" ");
//    Serial.println(roll);
  }
}

void loop() {
  // put your main code here, to run repeatedly:
//  for(int i = 0; i < 50; i ++ ) {
//    strip.setPixelColor(i, strip.Color(255,0,255));
//    strip.show();
//  }
  

  readMotion();

  int r = map(roll, -90, 90, 0,255);
  int g = map(pitch, -90, 90, 0, 255);
  int b = map(heading,0 , 360, 0, 255);
  //timeStep = int(abs(roll)) * 3;
  timeStep = 100;

  int numLeds = 3;
  int x = map(roll, -80, 80, 0, numLeds);
  int y = map(pitch, -80, 80, 0, numLeds);

  if(millis() - lastTime > timeStep) {
//    Serial.print("y: ");
//    Serial.println(y);
//    Serial.print("x: ");
//    Serial.println(x);
//    Serial.println("roll: ");
//    Serial.println(roll);
//    Serial.println("pitch: ");
//    Serial.println(pitch);
        ledMatrix.setXY(x,y);
    //ledMatrix.randomColorAtXY(x,y);
    ledMatrix.update();
   
    lastTime = millis();
  }
  //  Serial.print("index: ");
  //  Serial.print(mapToLedIndex(ledIndex));
  //  Serial.print("index: ");
  //  strip.setPixelColor(mapToLedIndex(ledIndex), strip.Color(0,0,0));
  //  strip.setPixelColor(mapToLedIndex(ledIndex+1), strip.Color(r,g,b));
  //  strip.setPixelColor(mapToLedIndex(ledIndex+2), strip.Color(r,g,b));
  //  strip.setPixelColor(mapToLedIndex(ledIndex+3), strip.Color(r,g,b));
  //  strip.setPixelColor(mapToLedIndex(ledIndex+4), strip.Color(0,0,0));
  //  strip.show();
  
  ledMatrix.draw();
  

}
int mapToLedIndex(int num) {
  int index = num % numLeds;
  if(index < 0 ) {
    index = numLeds - abs(index);
  }
  return index;
}


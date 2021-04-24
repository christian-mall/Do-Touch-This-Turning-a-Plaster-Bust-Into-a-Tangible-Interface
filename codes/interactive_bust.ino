#include "Arduino.h"
#include <Wire.h> //Capacitive Touch
#include "Adafruit_MPR121.h" //Capacitive Touch
#include <elapsedMillis.h> //Capacitive Touch
#include "SoftwareSerial.h" //Audio
#include "DFRobotDFPlayerMini.h" //Audio
#include "EPD.h" //E-Paper
#include "GUI_Paint.h" //E-Paper
#include "EPD_SDCard.h" //E-Paper

struct Reading {
  int max = 0;
  int min = 10000;
  void calibrate (int v) {
    if (v > max) max = v;
    if (v < min) min = v;
  }
};

Reading readings[9];
int ledPin = 13;
elapsedMillis timer;
Adafruit_MPR121 cap = Adafruit_MPR121();
SoftwareSerial mySoftwareSerial(3,4);
DFRobotDFPlayerMini myDFPlayer;

long timeTouchedForeheadLeft = 0;
long timeTouchedForeheadRight = 0;
long timeTouchedUpperNose = 0;
long timeTouchedLowerNose = 0;
long timeTouchedLipLeft = 0;
long timeTouchedLipRight = 0;
long timeTouchedUpperLock = 0;
long timeTouchedLowerLock = 0;
int maxInterval = 2000;

void setup() {
  pinMode(ledPin, OUTPUT);
  Serial.begin(9600);

  if (!cap.begin(0x5A)) {
    Serial.println("MPR121 not found, check wiring?");
    while (true);
  }

  digitalWrite(ledPin, HIGH);
  Serial.println("CALIBRATING");
  delay(100);
  
  while(timer < 20000) {
    readings[0].calibrate(cap.filteredData(0));
    readings[1].calibrate(cap.filteredData(1));
    readings[2].calibrate(cap.filteredData(2));
    readings[3].calibrate(cap.filteredData(3));
    readings[4].calibrate(cap.filteredData(4));
    readings[5].calibrate(cap.filteredData(5));
    readings[6].calibrate(cap.filteredData(6));
    readings[7].calibrate(cap.filteredData(7));
    readings[8].calibrate(cap.filteredData(8));
  }
  
  digitalWrite(ledPin, LOW);
  Serial.println("INITIALIZING");
  DEV_Module_Init();
  Serial.begin(9600);
  mySoftwareSerial.begin(9600);
  
  if (!myDFPlayer.begin(mySoftwareSerial)) {
    Serial.println(F("Unable to begin:"));
    Serial.println(F("1.Please recheck the connection!"));
    Serial.println(F("2.Please insert the SD card!"));
    while(true);
  }
  
  myDFPlayer.volume(30);
  EPD_5IN83_Init();
  EPD_5IN83_Clear();
  DEV_Delay_ms(300);
  SDCard_Init();
  Paint_NewImage(IMAGE_BW, EPD_5IN83_WIDTH, EPD_5IN83_HEIGHT, IMAGE_ROTATE_0, IMAGE_COLOR_INVERTED);
  Paint_Clear(WHITE);
  Serial.println("READY");
}

void loop() { 
  //SENSOR FOREHEAD SIDE
  if(cap.filteredData(0) <= (readings[0].min + 3)) {
    myDFPlayer.playMp3Folder(1);
    SDCard_ReadBMP("TFI.bmp", 0, 0);
    EPD_5IN83_Display();
  }

  //SENSORS LIPS
  if(cap.filteredData(5) <= (readings[5].min + 4)) {
    timeTouchedLipLeft = millis();
  }
  if(cap.filteredData(6) <= (readings[6].min + 4)) {
    timeTouchedLipRight = millis();
  }
  if ((timeTouchedLipRight > timeTouchedLipLeft && timeTouchedLipRight - timeTouchedLipLeft <= maxInterval)
  || (timeTouchedLipLeft > timeTouchedLipRight && timeTouchedLipLeft - timeTouchedLipRight <= maxInterval)) {
    myDFPlayer.playMp3Folder(2);
    SDCard_ReadBMP("SLI.bmp", 0, 0);
    EPD_5IN83_Display();
    timeTouchedLipLeft = 0;
    timeTouchedLipRight = 0;
  }

  //SENSORS FOREHEAD
  if(cap.filteredData(1) <= (readings[1].min + 3)) {
    timeTouchedForeheadLeft = millis();
  }
  if(cap.filteredData(2) <= (readings[2].min + 3)) {
    timeTouchedForeheadRight = millis();
  }
  if ((timeTouchedForeheadRight > timeTouchedForeheadLeft && timeTouchedForeheadRight - timeTouchedForeheadLeft <= maxInterval)
  || (timeTouchedForeheadLeft > timeTouchedForeheadRight && timeTouchedForeheadLeft - timeTouchedForeheadRight <= maxInterval)) {
    myDFPlayer.playMp3Folder(3);
    SDCard_ReadBMP("SFI.bmp", 0, 0);
    EPD_5IN83_Display();
    timeTouchedForeheadLeft = 0;
    timeTouchedForeheadRight = 0;
  }

  //SENSORS NOSE
  if(cap.filteredData(3) <= (readings[3].min + 2)) {
    timeTouchedUpperNose = millis();
  }
  if(cap.filteredData(4) <= (readings[4].min + 1)) {
    timeTouchedLowerNose = millis();
  }
  if ((timeTouchedLowerNose > timeTouchedUpperNose && timeTouchedLowerNose - timeTouchedUpperNose <= maxInterval)
  || (timeTouchedUpperNose > timeTouchedLowerNose && timeTouchedUpperNose - timeTouchedLowerNose <= maxInterval)) {
    myDFPlayer.playMp3Folder(4);
    SDCard_ReadBMP("SNI.bmp", 0, 0);
    EPD_5IN83_Display();
    timeTouchedUpperNose = 0;
    timeTouchedLowerNose = 0;
  }

  //SENSORS HAIR
  if(cap.filteredData(7) <= (readings[7].min + 2)) {
    timeTouchedUpperLock = millis();
  }
  if(cap.filteredData(8) <= (readings[8].min + 2)) {
    timeTouchedLowerLock = millis();
  }
  if ((timeTouchedLowerLock > timeTouchedUpperLock && timeTouchedLowerLock - timeTouchedUpperLock <= maxInterval)
  || (timeTouchedUpperLock > timeTouchedLowerLock && timeTouchedUpperLock - timeTouchedLowerLock <= maxInterval)) {
    myDFPlayer.playMp3Folder(5);
    SDCard_ReadBMP("RHI.bmp", 0, 0);
    EPD_5IN83_Display();
    timeTouchedUpperLock = 0;
    timeTouchedLowerLock = 0;
  }
}

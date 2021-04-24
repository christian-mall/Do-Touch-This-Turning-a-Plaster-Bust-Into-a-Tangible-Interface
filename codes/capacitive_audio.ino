#include <CapacitiveSensor.h>
#include "Arduino.h" // Audio
#include "SoftwareSerial.h" // Audio
#include "DFRobotDFPlayerMini.h" // Audio

CapacitiveSensor cs_4_2 = CapacitiveSensor(4,2);
CapacitiveSensor cs_4_6 = CapacitiveSensor(4,6);
SoftwareSerial mySoftwareSerial(11, 12);
DFRobotDFPlayerMini myDFPlayer;

void setup() {
  Serial.begin(9600);
  mySoftwareSerial.begin(9600);
  cs_4_2.set_CS_AutocaL_Millis(0xFFFFFFFF);
  cs_4_6.set_CS_AutocaL_Millis(0xFFFFFFFF);
  myDFPlayer.begin(mySoftwareSerial);
  myDFPlayer.volume(30);
}

void loop() {
  long total1 = cs_4_2.capacitiveSensor(30);
  long total2 = cs_4_6.capacitiveSensor(30);
  if(total1 > 1000) {
    myDFPlayer.playMp3Folder(1);
  }
  if(total2 > 1000) {
    myDFPlayer.playMp3Folder(2);
  }
}

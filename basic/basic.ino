#include <SPI.h>
#include <SD.h>
#include "MotorDriver.h"
#include <algorithm>

MotorDriver motor_driver;

const int chipSelect = 10;
const int initWaitTime = 120 * 1000; // 60 seconds wait time
const int depthTime = 20 * 1000;
const int depth1Start = initWaitTime;
const int depth2Start = initWaitTime + depthTime;
const int depth3Start = initWaitTime + depthTime * 2;
const int surfaceStart = initWaitTime + depthTime * 3;
const float depths [3] = {0.3, 0.6, 1};

void setup() {
  Serial.begin(9600);

  Serial.println(millis());
  Serial.print("Initializing SD card...");

  // see if the card is present and can be initialized:
  if (!SD.begin()) {
    Serial.println("Card failed, or not present");
    // don't do anything more:
    // while (1);
  }
  Serial.println("card initialized.");
  File myFile = SD.open("data.csv", FILE_WRITE);
  if (myFile) {
    Serial.println("Writing to SD card...");
    myFile.println("time,temperature,pressure,salinity_max,salinity_min,depth");
    myFile.close();
    Serial.println("done writing to SD card");
  } else {
    Serial.println("Failed to open file");
  }

  motor_driver.init();
}

void loop() {
  // put your main code here, to run repeatedly:
  // read of 0.76V => 236 Arduino units
  float analogToVoltageDenom = 310.5;
  float temperatureVoltage = analogRead(A2) / analogToVoltageDenom;
  float pressureVoltage = analogRead(A3) / analogToVoltageDenom;
  float salinityMaxVoltage = analogRead(A0) / analogToVoltageDenom;
  float salinityMinVoltage = analogRead(A1) / analogToVoltageDenom;

  // depth control
  float depth = -2.666 * pressureVoltage + 5.930;
  float depth_des = 0;
  int current_time = millis();
  if (current_time > surfaceStart) {
    depth_des = 0;
  } else if (current_time > depth3Start) {
    depth_des = depths[2];
  } else if (current_time > depth2Start) {
    depth_des = depths[1];
  } else if (current_time > depth1Start) {
    depth_des = depths[0];
  } // else depth_des is 0 before deploy
  float depth_diff = depth - depth_des;
  float k = 450.;
  float control_effort = std::max(-255.,std::min(k * depth_diff,255.));
  if (current_time > initWaitTime) {
    motor_driver.drive(control_effort,control_effort,0);
  }

  // log to serial monitor
  Serial.println();
  Serial.println();
  Serial.println();
  Serial.println(millis());
  Serial.print("Temperature:");
  Serial.print(temperatureVoltage, 6);
  Serial.print(",");
  Serial.println("Pressure:");
  Serial.print(pressureVoltage, 6);
  Serial.print(",");
  Serial.print("salinity_max:");
  Serial.print(salinityMaxVoltage, 6);
  Serial.print(",");
  Serial.print("salinity_min:");
  Serial.print(salinityMinVoltage, 6);
  Serial.print(",");
  Serial.print("depth:");
  Serial.print(depth, 6);
  Serial.print(",");
  Serial.print("control_effort:");
  Serial.print(control_effort, 6);

  // WRITE TO SD CARD
  File myFile = SD.open("data.csv", FILE_WRITE);

  if (myFile) {
    Serial.println("Writing to SD card...");
    myFile.print(millis());
    myFile.print(",");
    myFile.print(temperatureVoltage);
    myFile.print(",");
    myFile.print(pressureVoltage);
    myFile.print(",");
    myFile.print(salinityMaxVoltage);
    myFile.print(",");
    myFile.print(salinityMinVoltage);
    myFile.print(",");
    myFile.print(depth);
    myFile.println();
    myFile.close();
    Serial.println("done writing to SD card");
  } else {
    Serial.println("Failed to open file");
  }
  delay(250);
}
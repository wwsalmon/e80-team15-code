#include <SPI.h>
#include <SD.h>

const int chipSelect = 10;

void setup() {
  pinMode(24, OUTPUT);
  pinMode(25, OUTPUT);
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
    myFile.println("time,temperature,pressure,salinity_max,salinity_min");
    myFile.close();
    Serial.println("done writing to SD card");
  } else {
    Serial.println("Failed to open file");
  }
}

void loop() {
  // put your main code here, to run repeatedly:
  // read of 0.76V => 236 Arduino units
  float analogToVoltageDenom = 310.5;
  float temperatureVoltage = analogRead(A2) / analogToVoltageDenom; // 
  float pressureVoltage = analogRead(A3) / analogToVoltageDenom;
  float salinityMaxVoltage = analogRead(A0) / analogToVoltageDenom;
  float salinityMinVoltage = analogRead(A1) / analogToVoltageDenom;
  Serial.println();
  Serial.println();
  Serial.println();
  Serial.println(millis());
  Serial.println("Temperature: " + String(temperatureVoltage));
  Serial.println("Pressure: " + String(pressureVoltage));
  Serial.println("Salinity max: " + String(salinityMaxVoltage));
  Serial.println("Salinity min: " + String(salinityMinVoltage));
  if (pressureVoltage < 2.1) { // CHANGE THIS DEPENDING ON DEPTH CALIBRATION
    digitalWrite(24, HIGH);
    digitalWrite(25, LOW);
  } else {
    digitalWrite(24, LOW);
    digitalWrite(25, HIGH);
  }
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
    myFile.println();
    myFile.close();
    Serial.println("done writing to SD card");
  } else {
    Serial.println("Failed to open file");
  }
  delay(250);
}
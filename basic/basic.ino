#include <SPI.h>
#include <SD.h>

File myFile;
const int chipSelect = 4;

void setup() {
  // put your setup code here, to run once:
  // pinMode(A0, INPUT);
  // pinMode(A1, OUTPUT);
  Serial.begin(9600);

  // from https://docs.arduino.cc/learn/programming/sd-guide
  Serial.print("Initializing SD card...");

  // see if the card is present and can be initialized:
  if (!SD.begin(chipSelect)) {
    Serial.println("Card failed, or not present");
    // don't do anything more:
    while (1);
  }
  Serial.println("card initialized.");
}

void loop() {
  // put your main code here, to run repeatedly:
  // read of 0.76V => 236 Arduino units
  float analogToVoltageDenom = 310.5;
  float temperatureVoltage = analogRead(A3) / analogToVoltageDenom; // 
  float pressureVoltage = analogRead(A10) / analogToVoltageDenom;
  float salinityMaxVoltage = analogRead(A0) / analogToVoltageDenom;
  float salinityMinVoltage = analogRead(A1) / analogToVoltageDenom;
  Serial.println("Temperature: " + String(temperatureVoltage));
  Serial.println("Pressure: " + String(temperatureVoltage));
  Serial.println("Salinity max: " + String(salinityMaxVoltage));
  Serial.println("Salinity min: " + String(salinityMinVoltage));
  if (pressureVoltage < 2.65) {
    // digitalWrite(24, HIGH);
    // digitalWrite(25, LOW);
  } else {
    // digitalWrite(24, LOW);
    // digitalWrite(25, HIGH);
  }
  myFile = SD.open("test.txt", FILE_WRITE);
  if (myFile) {
    Serial.println("Writing to SD card...");
    myFile.print(millis());
    myFile.print(",");
    myFile.print(pressureVoltage);
    myFile.println();
    myFile.close();
    Serial.println("done writing to SD card");
  } else {
    Serial.println("Failed to open file");
  }
  delay(250);
}

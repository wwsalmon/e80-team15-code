void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  // read of 0.76V => 236 Arduino units
  float analogToVoltageDenom = 310.5;
  int a1analog = analogRead(A1);
  int a2analog = analogRead(A2);
  float a1voltage = a1analog / analogToVoltageDenom;
  float a2voltage = a2analog / analogToVoltageDenom;
  Serial.println("A1 voltage: " + String(a1voltage));
  Serial.println("A2 voltage: " + String(a2voltage));
  if (a2voltage < 2.65) {
    analogWrite(A3, 255);
  } else {
    analogWrite(A3, 0);
  }
  delay(250);
}

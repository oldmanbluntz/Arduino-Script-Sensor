#include <Wire.h>

const int I2C_ADDRESS = 0x12;
const int pathos_lowerPin = A0;
const int pathos_upperPin = A1;
const int palmPin = A2;
const long readingInterval = 5000; // Update every 5 seconds

unsigned long previousMillisPathosLower = 0;
unsigned long previousMillisPathosUpper = 1000;  // Delayed start
unsigned long previousMillisPalm = 2000;         // Delayed start
int pathos_lowerReading, pathos_upperReading, palmReading;

const int pathos_lowerDry = 100;
const int pathos_lowerWet = 0;
const int pathos_upperDry = 100;
const int pathos_upperWet = 0;
const int palmDry = 100;
const int palmWet = 0;

void setup() {
  Serial.begin(9600);
  Wire.begin(I2C_ADDRESS);
  Wire.onRequest(sendReadings);
  pinMode(pathos_lowerPin, INPUT);
  pinMode(pathos_upperPin, INPUT);
  pinMode(palmPin, INPUT);
}

void loop() {
  unsigned long currentMillis = millis();

  if (currentMillis - previousMillisPathosLower >= readingInterval) {
    previousMillisPathosLower = currentMillis;

    pathos_lowerReading = analogRead(pathos_lowerPin);
    pathos_lowerReading = map(constrain(pathos_lowerReading, pathos_lowerWet, pathos_lowerDry), pathos_lowerWet, pathos_lowerDry, 100, 0);

    // Output the sensor reading to the Serial Monitor
    Serial.print("Pothos Lower Sensor Reading: ");
    Serial.println(pathos_lowerReading);
  }

  if (currentMillis - previousMillisPathosUpper >= readingInterval) {
    previousMillisPathosUpper = currentMillis;

    pathos_upperReading = analogRead(pathos_upperPin);
    pathos_upperReading = map(constrain(pathos_upperReading, pathos_upperWet, pathos_upperDry), pathos_upperWet, pathos_upperDry, 100, 0);

    // Output the sensor reading to the Serial Monitor
    Serial.print("Pothos Upper Sensor Reading: ");
    Serial.println(pathos_upperReading);
  }

  if (currentMillis - previousMillisPalm >= readingInterval) {
    previousMillisPalm = currentMillis;

    palmReading = analogRead(palmPin);
    palmReading = map(constrain(palmReading, palmWet, palmDry), palmWet, palmDry, 100, 0);

    // Output the sensor reading to the Serial Monitor
    Serial.print("Palm Sensor Reading: ");
    Serial.println(palmReading);
  }
}

void sendReadings() {
  byte data[12]; // 3 sensors * 4 bytes each

  // Store sensor readings in bytes
  data[0] = highByte(pathos_lowerReading);
  data[1] = lowByte(pathos_lowerReading);
  data[2] = highByte(pathos_upperReading);
  data[3] = lowByte(pathos_upperReading);
  data[4] = highByte(palmReading);
  data[5] = lowByte(palmReading);

  // Send the structured data over I2C
  Wire.write(data, 12);
}

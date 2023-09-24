#include <Wire.h>
#include <ArduinoJson.h>

const int sensorPin1 = A0;  // Analog input pin for sensor 1 (pothos_upper)
const int sensorPin2 = A1;  // Analog input pin for sensor 2 (pothos_lower)
const int sensorPin3 = A2;  // Analog input pin for sensor 3 (palm)

const int numReadings = 10;
int moistureValues1[numReadings];
int moistureValues2[numReadings];
int moistureValues3[numReadings];
int moistureIndex = 0;

void setup() {
  Wire.begin(0x12);  // Arduino becomes I2C slave with address 0x12
  Wire.onReceive(receiveData);
  Serial.begin(9600); // Initialize Serial Monitor
  // ...

  for (int i = 0; i < numReadings; i++) {
    moistureValues1[i] = 0;
    moistureValues2[i] = 0;
    moistureValues3[i] = 0;
  }
}

void loop() {
  // Your sensor reading code here

  // Delay for 2 hours before taking the next reading
  delay(2 * 3600 * 1000); // Delay for 2 hours
}

void receiveData(int byteCount) {
  while (Wire.available()) {
    char c = Wire.read();
    // Process incoming data from the Raspberry Pi if needed
  }
}

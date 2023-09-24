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
  Wire.begin();
  Serial.begin(9600); // Initialize Serial Monitor
  // ...

  for (int i = 0; i < numReadings; i++) {
    moistureValues1[i] = 0;
    moistureValues2[i] = 0;
    moistureValues3[i] = 0;
  }
}

void loop() {
  // Turn on analog pins when needed
  digitalWrite(sensorPin1, HIGH);
  digitalWrite(sensorPin2, HIGH);
  digitalWrite(sensorPin3, HIGH);

  int moisture1 = analogRead(sensorPin1);
  int moisture2 = analogRead(sensorPin2);
  int moisture3 = analogRead(sensorPin3);

  // Turn off analog pins when done
  digitalWrite(sensorPin1, LOW);
  digitalWrite(sensorPin2, LOW);
  digitalWrite(sensorPin3, LOW);

  moistureValues1[moistureIndex] = moisture1;
  moistureValues2[moistureIndex] = moisture2;
  moistureValues3[moistureIndex] = moisture3;
  moistureIndex = (moistureIndex + 1) % numReadings;

  int avgMoisture1 = 0;
  int avgMoisture2 = 0;
  int avgMoisture3 = 0;
  for (int i = 0; i < numReadings; i++) {
    avgMoisture1 += moistureValues1[i];
    avgMoisture2 += moistureValues2[i];
    avgMoisture3 += moistureValues3[i];
  }
  avgMoisture1 /= numReadings;
  avgMoisture2 /= numReadings;
  avgMoisture3 /= numReadings;

  // Create JSON objects for each sensor's data
  StaticJsonDocument<200> sensor1Json;
  sensor1Json["plant"] = "pothos_upper";
  sensor1Json["moisture"] = avgMoisture1;

  StaticJsonDocument<200> sensor2Json;
  sensor2Json["plant"] = "pothos_lower";
  sensor2Json["moisture"] = avgMoisture2;

  StaticJsonDocument<200> sensor3Json;
  sensor3Json["plant"] = "palm";
  sensor3Json["moisture"] = avgMoisture3;

  // Serialize JSON objects to strings
  String jsonStr1;
  serializeJson(sensor1Json, jsonStr1);
  String jsonStr2;
  serializeJson(sensor2Json, jsonStr2);
  String jsonStr3;
  serializeJson(sensor3Json, jsonStr3);

  // Print JSON strings to the Serial Monitor
  Serial.println(jsonStr1);
  Serial.println(jsonStr2);
  Serial.println(jsonStr3);

  Wire.beginTransmission(0x12);
  Wire.write(jsonStr1.c_str());
  Wire.endTransmission();
  Wire.beginTransmission(0x12);
  Wire.write(jsonStr2.c_str());
  Wire.endTransmission();
  Wire.beginTransmission(0x12);
  Wire.write(jsonStr3.c_str());
  Wire.endTransmission();

  // Delay for 2 hours before taking the next reading
  delay(2 * 3600 * 1000); // Delay for 2 hours
}

#include <Wire.h>
#include <ArduinoJson.h>

// Define the I2C address for communication with Raspberry Pi
const int I2C_ADDRESS = 0x12;

// Define the analog pin assignments for moisture sensors
const int pathos_lowerPin = A0;
const int pathos_upperPin = A1;
const int palmPin = A2;

// Define the time interval for updating sensor readings (1 hour)
const long readingInterval = 3600000; // Update every 1 hour

unsigned long previousMillis = 0;
int pathos_lowerReading, pathos_upperReading, palmReading;

// Define calibrated baseline values for wet and dry conditions
const int pathos_lowerDry = 100;
const int pathos_lowerWet = 0;
const int pathos_upperDry = 100;
const int pathos_upperWet = 0;
const int palmDry = 100;
const int palmWet = 0;

void setup() {
  // Start serial communication at 9600 baud
  Serial.begin(9600);

  // Start I2C communication as a slave with the specified address
  Wire.begin(I2C_ADDRESS);

  // Register callback function for I2C communication to send readings
  Wire.onRequest(sendReadings);

  // Initialize pins as inputs
  pinMode(pathos_lowerPin, INPUT);
  pinMode(pathos_upperPin, INPUT);
  pinMode(palmPin, INPUT);

  // Take the initial sensor readings on startup
  takeInitialReading();
}

void loop() {
  unsigned long currentMillis = millis();

  // Check if it's time to update sensor readings
  if (currentMillis - previousMillis >= readingInterval) {
    previousMillis = currentMillis;

    // Enable analog pins to take readings
    pinMode(pathos_lowerPin, INPUT);
    pinMode(pathos_upperPin, INPUT);
    pinMode(palmPin, INPUT);

    // Delay to allow sensors to stabilize
    delay(1000);

    // Take sensor readings
    pathos_lowerReading = analogRead(pathos_lowerPin);
    pathos_upperReading = analogRead(pathos_upperPin);
    palmReading = analogRead(palmPin);

    // Scale the readings based on the calibrated wet and dry values and invert the range
    pathos_lowerReading = map(constrain(pathos_lowerReading, pathos_lowerWet, pathos_lowerDry), pathos_lowerWet, pathos_lowerDry, 100, 0);
    pathos_upperReading = map(constrain(pathos_upperReading, pathos_upperWet, pathos_upperDry), pathos_upperWet, pathos_upperDry, 100, 0);
    palmReading = map(constrain(palmReading, palmWet, palmDry), palmWet, palmDry, 100, 0);

    // Disable analog pins to reduce electrolysis and increase sensor lifespan
    pinMode(pathos_lowerPin, INPUT_PULLUP);
    pinMode(pathos_upperPin, INPUT_PULLUP);
    pinMode(palmPin, INPUT_PULLUP);

    // Output the calibrated readings to the Serial Monitor
    Serial.println("Sensor Readings:");
    Serial.print("Pathos Lower: ");
    Serial.println(pathos_lowerReading);
    Serial.print("Pathos Upper: ");
    Serial.println(pathos_upperReading);
    Serial.print("Palm: ");
    Serial.println(palmReading);
  }
}

void sendReadings() {
  // Create a JSON document for storing sensor readings
  StaticJsonDocument<100> jsonDoc;
  jsonDoc["pathos_lower"] = pathos_lowerReading;
  jsonDoc["pathos_upper"] = pathos_upperReading;
  jsonDoc["palm"] = palmReading;

  // Serialize the JSON document to a string
  String jsonString;
  serializeJson(jsonDoc, jsonString);

  // Send JSON data to the Raspberry Pi over I2C
  Wire.write(jsonString.c_str());
}

void takeInitialReading() {
  // Enable analog pins to take readings
  pinMode(pathos_lowerPin, INPUT);
  pinMode(pathos_upperPin, INPUT);
  pinMode(palmPin, INPUT);

  // Delay to allow sensors to stabilize
  delay(1000);

  // Take initial sensor readings
  pathos_lowerReading = analogRead(pathos_lowerPin);
  pathos_upperReading = analogRead(pathos_upperPin);
  palmReading = analogRead(palmPin);

  // Scale the initial readings based on the calibrated wet and dry values and invert the range
  pathos_lowerReading = map(constrain(pathos_lowerReading, pathos_lowerWet, pathos_lowerDry), pathos_lowerWet, pathos_lowerDry, 100, 0);
  pathos_upperReading = map(constrain(pathos_upperReading, pathos_upperWet, pathos_upperDry), pathos_upperWet, pathos_upperDry, 100, 0);
  palmReading = map(constrain(palmReading, palmWet, palmDry), palmWet, palmDry, 100, 0);

  // Output the initial calibrated readings to the Serial Monitor
  Serial.println("Initial Readings:");
  Serial.print("Pathos Lower: ");
  Serial.println(pathos_lowerReading);
  Serial.print("Pathos Upper: ");
  Serial.println(pathos_upperReading);
  Serial.print("Palm: ");
  Serial.println(palmReading);
}

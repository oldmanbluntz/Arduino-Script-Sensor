#include <Wire.h>

// Define the I2C address
const int I2C_ADDRESS = 0x12; // Updated I2C address

// Define the analog pin assignments
const int pathos_lowerPin = A0;
const int pathos_upperPin = A1;
const int palmPin = A2;

// Time interval for taking readings (in milliseconds)
const long readingInterval = 2 * 60 * 60 * 1000; // Every 2 hours

unsigned long previousMillis = 0;
int pathos_lowerReading, pathos_upperReading, palmReading;

// Define calibrated baseline values for wet and dry conditions
const int pathos_lowerDry = 100; // Adjust as needed
const int pathos_lowerWet = 0;   // Adjust as needed
const int pathos_upperDry = 100; // Adjust as needed
const int pathos_upperWet = 0;   // Adjust as needed
const int palmDry = 100;         // Adjust as needed
const int palmWet = 0;           // Adjust as needed

void setup() {
  // Start serial communication at 9600 baud
  Serial.begin(9600);

  // Start I2C communication as a slave
  Wire.begin(I2C_ADDRESS); // Updated I2C address

  // Register callback functions for I2C communication
  Wire.onRequest(sendReadings);

  // Initialize pins as inputs
  pinMode(pathos_lowerPin, INPUT);
  pinMode(pathos_upperPin, INPUT);
  pinMode(palmPin, INPUT);
}

void loop() {
  unsigned long currentMillis = millis();

  // Check if it's time to take a reading
  if (currentMillis - previousMillis >= readingInterval) {
    previousMillis = currentMillis;

    // Enable analog pins to take readings
    pinMode(pathos_lowerPin, INPUT);
    pinMode(pathos_upperPin, INPUT);
    pinMode(palmPin, INPUT);

    // Delay to allow sensors to stabilize
    delay(1000);

    // Take readings
    pathos_lowerReading = analogRead(pathos_lowerPin);
    pathos_upperReading = analogRead(pathos_upperPin);
    palmReading = analogRead(palmPin);

    // Scale the readings based on the calibrated wet and dry values, and invert the range
    pathos_lowerReading = map(constrain(pathos_lowerReading, pathos_lowerWet, pathos_lowerDry), pathos_lowerWet, pathos_lowerDry, 100, 0);
    pathos_upperReading = map(constrain(pathos_upperReading, pathos_upperWet, pathos_upperDry), pathos_upperWet, pathos_upperDry, 100, 0);
    palmReading = map(constrain(palmReading, palmWet, palmDry), palmWet, palmDry, 100, 0);

    // Disable analog pins to reduce electrolysis
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
  // Create JSON data for the readings
  String json = "{\"pathos_lower\":" + String(pathos_lowerReading) +
               ",\"pathos_upper\":" + String(pathos_upperReading) +
               ",\"palm\":" + String(palmReading) + "}";

  // Send JSON data to the Raspberry Pi
  Wire.write(json.c_str());
}

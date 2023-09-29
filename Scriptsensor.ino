#include <Wire.h>

const int sensorCount = 3;  // Number of sensors
int AirValue[sensorCount] = {501, 494, 500};  // Replace with appropriate values for each sensor
int WaterValue[sensorCount] = {217, 216, 200}; // Replace with appropriate values for each sensor

int soilMoistureValue[sensorCount] = {0};
int soilMoisturePercent[sensorCount] = {0};

void setup() {
  Serial.begin(9600); // Open serial port, set the baud rate to 9600 bps
  Wire.begin(8);      // Start I2C communication as a slave with address 8
  Wire.onRequest(sendData); // Register callback for I2C request
}

void loop() {
  // Read sensor values and calculate percentages for each sensor
  for (int i = 0; i < sensorCount; i++) {
    soilMoistureValue[i] = analogRead(A0 + i);
    soilMoisturePercent[i] = map(soilMoistureValue[i], AirValue[i], WaterValue[i], 0, 100);
  }

  // Output data to serial monitor for debugging
  for (int i = 0; i < sensorCount; i++) {
    Serial.print("Sensor ");
    Serial.print(i + 1);
    Serial.print(": ");
    if (soilMoisturePercent[i] >= 100) {
      Serial.println("100 %");
    } else if (soilMoisturePercent[i] <= 0) {
      Serial.println("0 %");
    } else {
      Serial.print(soilMoisturePercent[i]);
      Serial.println("%");
    }
  }
  
  delay(500); // Update sensor data every 500ms
}

// Callback function to send data over I2C when requested
void sendData() {
  for (int i = 0; i < sensorCount; i++) {
    byte data[2];
    data[0] = soilMoisturePercent[i] >> 8; // MSB
    data[1] = soilMoisturePercent[i] & 0xFF; // LSB
    Wire.write(data, 2);
  }
}

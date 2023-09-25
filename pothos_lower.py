#!/usr/bin/env python

import smbus2

# Define the I2C bus number and Arduino address
i2c_bus = 1  # Use 1 for Raspberry Pi 2/3, 0 for Raspberry Pi 1
arduino_address = 0x12  # Use the Arduino's I2C address

# Create an I2C bus object
bus = smbus2.SMBus(i2c_bus)

# Define the sensor you want to read (1 for pothos_lower, 2 for pothos_upper, 3 for palm)
sensor_number = 1

try:
    # Request data from the Arduino
    bus.write_byte(arduino_address, sensor_number)

    # Read the moisture value (as a single byte)
    moisture_value = bus.read_byte(arduino_address)

    # Normalize the moisture value to a range of 0-100
    normalized_moisture = int((moisture_value / 1023.0) * 100.0)

    # Print the normalized moisture value as an integer
    print(normalized_moisture)

except KeyboardInterrupt:
    pass
except Exception as e:
    print("Error:", e)

finally:
    bus.close()

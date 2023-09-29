#!/usr/bin/env python
import smbus2
import time

# Define the I2C address of the Arduino Nano
arduino_address = 8

# Create an I2C bus object
bus = smbus2.SMBus(1)  # 1 indicates /dev/i2c-1, for Raspberry Pi 2/3

# Request data from the pothos_lower sensor
bus.write_byte(arduino_address, 0)

# Wait for a short moment to allow the Arduino to respond
time.sleep(0.1)

# Read the response (one byte)
data = bus.read_byte(arduino_address)

# Convert the received data to a float with one decimal place
moisture_percent = float(data) / 10.0

print(moisture_percent)

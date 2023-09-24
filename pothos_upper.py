import smbus2
import json

# Define the I2C bus number and Arduino address
i2c_bus = 1  # Use 1 for Raspberry Pi 2/3, 0 for Raspberry Pi 1
arduino_address = 0x12  # Use the Arduino's I2C address

# Create an I2C bus object
bus = smbus2.SMBus(i2c_bus)

# Define the sensor you want to read (1 for pothos_upper, 2 for pothos_lower, 3 for palm)
sensor_number = 1

try:
    # Request data from the Arduino
    bus.write_byte(arduino_address, sensor_number)

    # Read the JSON data sent by the Arduino
    json_data = ""
    while True:
        char = bus.read_byte(arduino_address)
        if char == 0:
            break
        json_data += chr(char)

    # Parse the JSON data
    sensor_data = json.loads(json_data)

    # Print the sensor data
    print("Plant: {}".format(sensor_data["plant"]))
    print("Moisture Percentage: {}%".format(sensor_data["moisture_percentage"]))

except KeyboardInterrupt:
    pass
except Exception as e:
    print("Error:", e)

finally:
    bus.close()

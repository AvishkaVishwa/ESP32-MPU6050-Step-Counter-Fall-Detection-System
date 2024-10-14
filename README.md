# ESP32 MPU6050 Step Counter & Fall Detection System

This project is a simple IoT-based step counting and fall detection system using an ESP32 microcontroller and the MPU6050 sensor. The system displays real-time step count on an OLED display and provides a web-based interface to monitor accelerometer and gyroscope data along with step count and fall detection alerts.
<img src="/images/photo_2024-10-14_20-50-13.jpg">
## Features
- **Step Counting**: Counts steps using accelerometer data from the MPU6050.
- **Fall Detection**: Detects potential falls based on specific threshold conditions using both accelerometer and gyroscope data.
- **OLED Display**: Displays real-time step count on a 128x64 OLED screen.
- **Wi-Fi Access Point**: ESP32 operates as a Wi-Fi hotspot, allowing users to connect and view data via a web server.
- **Web Interface**: Displays real-time accelerometer and gyroscope data, step count, and fall alerts on a webpage that automatically refreshes every second.

<img src ="/images/photo_2024-10-14_20-50-35.jpg">
## Hardware Requirements
- **ESP32** (WROOM or any compatible model)
- **MPU6050** (Accelerometer & Gyroscope Sensor)
- **SSD1306** OLED display (128x64 pixels)
- **Jumper Wires** for connections

## Libraries Used
- `Wire.h`: I2C communication for MPU6050.
- `WiFi.h`: For Wi-Fi access point and web server functionality.
- `WebServer.h`: To host the webpage on ESP32.
- `Adafruit_GFX.h` and `Adafruit_SSD1306.h`: For controlling the OLED display.


## Installation and Setup

1. **Connect the Hardware**:
   - **MPU6050**: Connect the sensor to ESP32 using I2C pins (SDA and SCL).
   - **SSD1306**: Connect the OLED display to ESP32 using I2C pins.

2. **Upload the Code**:
   - Use Arduino IDE or PlatformIO to upload the code to your ESP32.
   - Ensure you have installed the necessary libraries (Wire, WiFi, Adafruit_GFX, Adafruit_SSD1306).

3. **Access the Web Interface**:
   - After uploading the code, the ESP32 will start a Wi-Fi Access Point.
   - Connect to the Wi-Fi network `ESP32_Hotspot` with the password `password123`.
   - Open a browser and navigate to the ESP32 IP address (displayed in the Serial Monitor) to view real-time data.

## How It Works
- The MPU6050 sensor reads accelerometer and gyroscope data.
- The step counting logic is based on a simple threshold mechanism, incrementing the step count when the Y-axis acceleration exceeds a specific value.
- Fall detection is triggered when the Z-axis acceleration falls below a threshold while the gyroscope detects significant movement on the X-axis.
- The OLED display shows the current step count.
- The web server allows remote monitoring, displaying accelerometer, gyroscope data, and step count in real-time. If a fall is detected, a warning message ("Man Down") is shown.

## Project Structure
- `setup()`: Initializes the MPU6050 sensor, OLED display, and Wi-Fi access point.
- `loop()`: Continuously reads sensor data, updates step count, checks for falls, and serves the web page.
- `handleRoot()`: Sends the HTML content for the web interface with live data.

## Future Improvements
- Fine-tuning the thresholds for step counting and fall detection based on further testing.
- Adding data logging or notification alerts when a fall is detected.
- Extending the system to support more sensors and functionalities like GPS for location tracking.

## License
This project is open-source and available for use under the MIT License.

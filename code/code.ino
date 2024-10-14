#include <Wire.h>
#include <WiFi.h>
#include <WebServer.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
#define OLED_RESET    -1 // Reset pin # (or -1 if sharing Arduino reset pin)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

const char *ssid = "ESP32_Hotspot";
const char *password = "password123";

const int MPU = 0x68; // MPU6050 I2C address
float AccX, AccY, AccZ;
float GyroX, GyroY, GyroZ;
int stepCount = 0; // Variable to count steps
bool stepping = false; // To track stepping state
bool fallDetected = false; // To track fall detection

WebServer server(80);

void setup() {
  Serial.begin(19200);
  Wire.begin();

  // Set up the Wi-Fi hotspot
  WiFi.softAP(ssid, password);
  Serial.println("Access Point started");
  Serial.print("IP Address: ");
  Serial.println(WiFi.softAPIP());

  // Initialize MPU6050
  Wire.beginTransmission(MPU);
  Wire.write(0x6B);
  Wire.write(0x00);
  Wire.endTransmission(true);
  delay(20);

  // Initialize the OLED display
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;);
  }
  display.clearDisplay();
  display.setTextSize(2); // Set text size to make it more readable
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.println("Steps:");
  display.display();
  delay(1000);
  display.clearDisplay();

  // Handle HTTP requests
  server.on("/", handleRoot);
  server.begin();
  Serial.println("HTTP server started");
}

void loop() {
  // Read accelerometer data
  Wire.beginTransmission(MPU);
  Wire.write(0x3B);
  Wire.endTransmission(false);
  Wire.requestFrom(MPU, 6, true);
  AccX = (Wire.read() << 8 | Wire.read()) / 16384.0;
  AccY = (Wire.read() << 8 | Wire.read()) / 16384.0;
  AccZ = (Wire.read() << 8 | Wire.read()) / 16384.0;

  // Read gyroscope data
  Wire.beginTransmission(MPU);
  Wire.write(0x43);
  Wire.endTransmission(false);
  Wire.requestFrom(MPU, 6, true);
  GyroX = (Wire.read() << 8 | Wire.read()) / 131.0;
  GyroY = (Wire.read() << 8 | Wire.read()) / 131.0;
  GyroZ = (Wire.read() << 8 | Wire.read()) / 131.0;

  // Step counting logic
  if (AccY > 1.0) { // Adjust threshold based on your tests
    if (!stepping) {
      stepCount++;
      stepping = true; // Step detected
    }
  } else {
    stepping = false; // No step detected
  }

  // Fall detection logic
  if (AccZ < 0.5 && GyroX > 20.0) { // Adjust thresholds based on your tests
    fallDetected = true;
  } else {
    fallDetected = false;
  }

  // Update OLED display to show only step count
  display.clearDisplay();
  display.setCursor(0, 0);
  display.setTextSize(2);
  display.println("Steps:");
  display.setTextSize(3); // Larger size for the step count
  display.setCursor(0, 30);
  display.println(stepCount);
  display.display();

  // Handle client requests
  server.handleClient();
  
  delay(100); // Adjust as needed for data update frequency
}

void handleRoot() {
  // Serve a web page with accelerometer, gyroscope, step count values, and fall warning
  String html = "<!DOCTYPE html>"
                "<html>"
                "<head>"
                "<meta http-equiv='refresh' content='1'>" // Refresh every second
                "<style>"
                "body { font-family: Arial, sans-serif; text-align: center; margin: 0; padding: 0; background-color: #f4f4f4; }"
                "h1 { color: #333; margin-top: 20px; font-size: 36px; }"
                ".data { font-size: 28px; margin: 10px; }"
                ".warning { color: red; font-weight: bold; font-size: 28px; margin: 20px; }"
                "</style>"
                "</head>"
                "<body>"
                "<h1>ESP32 MPU6050 Data</h1>"
                "<div class='data'>"
                "<p>Accelerometer Values (g):</p>"
                "<p>X: " + String(AccX, 4) + "</p>"
                "<p>Y: " + String(AccY, 4) + "</p>"
                "<p>Z: " + String(AccZ, 4) + "</p>"
                "<p>Gyroscope Values (°/s):</p>"
                "<p>X: " + String(GyroX, 4) + "</p>"
                "<p>Y: " + String(GyroY, 4) + "</p>"
                "<p>Z: " + String(GyroZ, 4) + "</p>"
                "<p>Step Count: " + String(stepCount) + "</p>"
                "</div>";
                
  // Show fall warning if detected
  if (fallDetected) {
    html += "<div class='warning'>Warning: Man Down!</div>";
  }

  html += "</body></html>";
  
  server.send(200, "text/html", html);
}

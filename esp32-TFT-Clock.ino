#include <Arduino.h>
#include <WiFi.h>
#include <WiFiClient.h>
#include <NTPClient.h>
#include <SPI.h>
#include <TFT_eSPI.h>

// Define your WiFi credentials
const char* ssid     = "YourWiFiSSID";
const char* password = "YourWiFiPassword";

// Define NTP Client to get time
const long  gmtOffset_sec = 3600; // GMT offset in seconds for Central European Time (CET)
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org", gmtOffset_sec);

// Define TFT pins
#define TFT_CS     15
#define TFT_RST    4
#define TFT_DC     2
#define TFT_MOSI   23
#define TFT_SCK    18
#define TFT_MISO   19

TFT_eSPI tft = TFT_eSPI();  

void setup() {
  Serial.begin(115200);
  
  // Initialize TFT display
  tft.begin();
  tft.setRotation(1);
  
  // Initialize WiFi connection
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  
  // Initialize NTP client
  timeClient.begin();
  timeClient.update();
}

void loop() {
  timeClient.update(); // Update time from NTP server
  
  // Get current time
  unsigned long epochTime = timeClient.getEpochTime();
  struct tm * timeinfo;
  timeinfo = localtime(&epochTime);
  
  // Format time
  char formattedTime[32];
  strftime(formattedTime, 32, "%H:%M:%S", timeinfo);
  
  // Display time on TFT
  tft.fillScreen(TFT_BLACK);
  tft.setTextSize(2);
  tft.setCursor(20, 60);
  tft.setTextColor(TFT_WHITE);
  tft.print(formattedTime);
  
  delay(1000); // Update time every second
}

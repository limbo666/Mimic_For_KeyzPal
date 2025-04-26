// KeyzPal_Mimic v1.1 for ESP32
// Created by Nikos Georgousis
// Adapted from ESP8266 version 
// Tested on ESP32 Dev Module
// Compatible with most ESP32 dev boards with minor changes

#include <WiFi.h>
#include <WiFiUdp.h>
#include <EEPROM.h>
#include <Adafruit_NeoPixel.h>

const char* ssid = "SSID";         // Replace with your network SSID
const char* password = "PASSword"; // Replace with your network password
const int udpPort = 22689;         // UDP port to listen on
const int ledPin = 2;              // GPIO2 connected to LED strip (adjust if needed)
const int numLEDs = 3;             // Number of LEDs in the strip
const int ledBrightness = 50;      // LED strip brightness (0-255)
const int eepromAddress = 0;       // EEPROM address for color storage
const int colorSize = 3;           // Size of each color component (RGB)
const int eepromSize = 512;        // Size of emulated EEPROM

// Default colors (RGB)
byte numColor[colorSize] = {255, 255, 255};    // White
byte capsColor[colorSize] = {255, 255, 255};   // White
byte scrollColor[colorSize] = {255, 255, 255}; // White

WiFiUDP udp;
Adafruit_NeoPixel strip(numLEDs, ledPin, NEO_GRB + NEO_KHZ800);

// Buffer for UDP packet reading
const int maxPacketSize = 32;
char packetBuffer[maxPacketSize];

void setup() {
  Serial.begin(115200);
  strip.begin();
  strip.setBrightness(ledBrightness);
  strip.clear();
  strip.show(); // Initialize LEDs to off

  // Quick startup LED feedback (red -> green -> off)
  strip.fill(strip.Color(50, 0, 0));
  strip.show();
  delay(100); // Brief flash
  strip.fill(strip.Color(0, 50, 0));
  strip.show();
  delay(100);
  strip.clear();
  strip.show();

  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(100); // Minimal delay for Wi-Fi connection
  }

  // Initialize EEPROM
  EEPROM.begin(eepromSize); // Initialize emulated EEPROM
  restoreColors(); // Load saved colors

  // Start UDP server
  udp.begin(udpPort);

  // Brief diagnostic output
  Serial.print("UDP server running at ");
  Serial.print(WiFi.localIP());
  Serial.print(":");
  Serial.println(udpPort);
}

void loop() {
  // Process UDP packets
  int packetSize = udp.parsePacket();
  if (packetSize) {
    handleUDPPacket(packetSize);
  }
}

void handleUDPPacket(int packetSize) {
  // Read packet into buffer
  if (packetSize > maxPacketSize - 1) packetSize = maxPacketSize - 1;
  int len = udp.read(packetBuffer, packetSize);
  packetBuffer[len] = '\0'; // Null-terminate

  // Compare commands using strcmp for efficiency
  if (strcmp(packetBuffer, "capsON") == 0) {
    setLedColor(0, capsColor);
  } else if (strcmp(packetBuffer, "capsOFF") == 0) {
    turnOffLED(0);
  } else if (strcmp(packetBuffer, "numON") == 0) {
    setLedColor(1, numColor);
  } else if (strcmp(packetBuffer, "numOFF") == 0) {
    turnOffLED(1);
  } else if (strcmp(packetBuffer, "scrollON") == 0) {
    setLedColor(2, scrollColor);
  } else if (strcmp(packetBuffer, "scrollOFF") == 0) {
    turnOffLED(2);
  } else if (strncmp(packetBuffer, "caps", 4) == 0) {
    updateColor(&packetBuffer[4], capsColor);
    saveColors();
    setLedColor(0, capsColor);
  } else if (strncmp(packetBuffer, "num", 3) == 0) {
    updateColor(&packetBuffer[3], numColor);
    saveColors();
    setLedColor(1, numColor);
  } else if (strncmp(packetBuffer, "scroll", 6) == 0) {
    updateColor(&packetBuffer[6], scrollColor);
    saveColors();
    setLedColor(2, scrollColor);
  } else if (strcmp(packetBuffer, "Reboot") == 0) {
    ESP.restart();
  } else if (strcmp(packetBuffer, "Eeprom") == 0) {
    restoreColors();
  }
}

void setLedColor(int ledIndex, const byte* color) {
  if (ledIndex >= 0 && ledIndex < numLEDs) {
    strip.setPixelColor(ledIndex, color[0], color[1], color[2]);
    strip.show();
  }
}

void turnOffLED(int ledIndex) {
  if (ledIndex >= 0 && ledIndex < numLEDs) {
    strip.setPixelColor(ledIndex, 0);
    strip.show();
  }
}

void saveColors() {
  // Write colors to EEPROM only if changed
  static byte lastCapsColor[colorSize], lastNumColor[colorSize], lastScrollColor[colorSize];
  bool changed = false;

  for (int i = 0; i < colorSize; i++) {
    if (lastCapsColor[i] != capsColor[i]) {
      EEPROM.write(eepromAddress + i, capsColor[i]);
      lastCapsColor[i] = capsColor[i];
      changed = true;
    }
    if (lastNumColor[i] != numColor[i]) {
      EEPROM.write(eepromAddress + colorSize + i, numColor[i]);
      lastNumColor[i] = numColor[i];
      changed = true;
    }
    if (lastScrollColor[i] != scrollColor[i]) {
      EEPROM.write(eepromAddress + 2 * colorSize + i, scrollColor[i]);
      lastScrollColor[i] = scrollColor[i];
      changed = true;
    }
  }
  if (changed) {
    EEPROM.commit();
  }
}

void restoreColors() {
  for (int i = 0; i < colorSize; i++) {
    capsColor[i] = EEPROM.read(eepromAddress + i);
    numColor[i] = EEPROM.read(eepromAddress + colorSize + i);
    scroll-wasteColor[i] = EEPROM.read(eepromAddress + 2 * colorSize + i);
  }
}

void updateColor(const char* colorCommand, byte* color) {
  // Map color commands to RGB values
  if (strcmp(colorCommand, "Red") == 0) {
    color[0] = 255; color[1] = 0; color[2] = 0;
  } else if (strcmp(colorCommand, "Green") == 0) {
    color[0] = 0; color[1] = 255; color[2] = 0;
  } else if (strcmp(colorCommand, "Blue") == 0) {
    color[0] = 0; color[1] = 0; color[2] = 255;
  } else if (strcmp(colorCommand, "Purple") == 0) {
    color[0] = 255; color[1] = 0; color[2] = 255;
  } else if (strcmp(colorCommand, "White") == 0) {
    color[0] = 255; color[1] = 255; color[2] = 255;
  } else if (strcmp(colorCommand, "Yellow") == 0) {
    color[0] = 255; color[1] = 255; color[2] = 0;
  } else if (strcmp(colorCommand, "Aqua") == 0) {
    color[0] = 0; color[1] = 255; color[2] = 255;
  } else if (strcmp(colorCommand, "Orange") == 0) {
    color[0] = 255; color[1] = 165; color[2] = 0;
  } else if (strcmp(colorCommand, "Pink") == 0) {
    color[0] = 255; color[1] = 55; color[2] = 180;
  } else if (strcmp(colorCommand, "Magenta") == 0) {
    color[0] = 250; color[1] = 0; color[2] = 165;
  } else if (strcmp(colorCommand, "Lime") == 0) {
    color[0] = 154; color[1] = 235; color[2] = 0;
  }
}

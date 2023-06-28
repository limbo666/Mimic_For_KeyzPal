//KeyzPal_Mimic v1.0
//This code cretaed by Nikos Georgousis
//Test on ESP01 module, which is the bare minimum ESP8266 module.
//Thsi code should be compatible with all ESP8266 dev boards. Some small changes may be needed 

#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <EEPROM.h>
#include <Adafruit_NeoPixel.h>


const char* ssid = "SSID";         // Replace with your network SSID
const char* password = "PASSword"; // Replace with your network password
const int udpPort = 22689;         // UDP port to listen on //Change this port if you have more than one PC on your LAN running KeyzPal to avoid conflicts

const int ledPin = 2;                  // GPIO2 connected to the LED strip
const int numLEDs = 3;                 // Number of LEDs in the strip
const int ledBrightness = 50;          // LED strip brightness (0-255)

const int eepromAddress = 0;           // Starting EEPROM address to save colors
const int colorSize = 3;               // Size of each color component (RGB)

// Default colors
byte numColor[colorSize] = {255, 255, 255};       // White
byte capsColor[colorSize] = {255, 255, 255};      // White
byte scrollColor[colorSize] = {255, 255, 255};    // White

WiFiUDP udp;
Adafruit_NeoPixel strip(numLEDs, ledPin, NEO_GRB + NEO_KHZ800);

// Track the state of each LED
bool capsLEDOn = false;
bool numLEDOn = false;
bool scrollLEDOn = false;

void setup() {
  Serial.begin(115200); // Start the serial communication
  // Initialize LED strip
  strip.begin();
  strip.show();  // Turn off all LEDs initially
  strip.setBrightness(ledBrightness);
setAllLEDsToColor(50,0,0);
  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }

setAllLEDsToColor(0,50,0);
delay(500);
 setAllLEDsToColor(0,0,0); 
  // Initialize EEPROM
  EEPROM.begin(512);
setAllLEDsToColor(0,50,50);
delay(500);
setAllLEDsToColor(0,0,0);
delay(500);
  // Restore saved colors from EEPROM
  restoreColors();

// Convert byte array to int values
int R = capsColor[0];
int G = capsColor[1];
int B = capsColor[2];
// Call the function with the converted values
setLEDColor(0,R, G, B);
delay(500);
// Convert byte array to int values
 R = numColor[0];
 G = numColor[1];
 B = numColor[2];
// Call the function with the converted values
setLEDColor(1,R, G, B);
delay(500);
// Convert byte array to int values
R = scrollColor[0];
G = scrollColor[1];
B = scrollColor[2];
// Call the function with the converted values
setLEDColor(2,R, G, B);
delay(2500);


 setAllLEDsToColor(0,0,0); 
  // Start UDP server
  udp.begin(udpPort);

  // Display diagnostics on the terminal
  Serial.print("UDP server is running at IP address: ");
  Serial.print(WiFi.localIP());
  Serial.print(", port: ");
  Serial.println(udpPort);
 setAllLEDsToColor(0,500,0); 
delay(500);

    turnOffAllLEDs();
  Serial.println("System is up and running");
}

void loop() {
  // Check for UDP packets
  int packetSize = udp.parsePacket();
  if (packetSize) {
    handleUDPPacket(packetSize);
  }
}

void handleUDPPacket(int packetSize) {
  // Receive the UDP packet as a string
  String command = udp.readString();
  command.trim();  // Remove leading/trailing whitespaces

  // Print the received command on the terminal
  Serial.print("Received command: ");
  Serial.println(command);

  // Process the received command
  if (command == "capsON") {
    Serial.println("Valid command: capsON");
    setLedColor(1, capsColor);
  } else if (command == "capsOFF") {
    Serial.println("Valid command: capsOFF");
    turnOffLED(1);
  } else if (command == "numON") {
    Serial.println("Valid command: numON");
    setLedColor(2, numColor);
  } else if (command == "numOFF") {
    Serial.println("Valid command: numOFF");
    turnOffLED(2);
  } else if (command == "scrollON") {
    Serial.println("Valid command: scrollON");
    setLedColor(3, scrollColor);
  } else if (command == "scrollOFF") {
    Serial.println("Valid command: scrollOFF");
    turnOffLED(3);
  } else if (command.startsWith("caps") && command.length() > 4) {
    Serial.println("Valid command: " + command);
    updateColor(command.substring(4), capsColor);
    saveColors();
    if (isLedOn(1)) {
      setLedColor(1, capsColor);
    }
  } else if (command.startsWith("num") && command.length() > 3) {
    Serial.println("Valid command: " + command);
    updateColor(command.substring(3), numColor);
    saveColors();
    if (isLedOn(2)) {
      setLedColor(2, numColor);
    }
  } else if (command.startsWith("scroll") && command.length() > 6) {
    Serial.println("Valid command: " + command);
    updateColor(command.substring(6), scrollColor);
    saveColors();
    if (isLedOn(3)) {
      setLedColor(3, scrollColor);
    }
  } else if (command == "Reboot") {
    Serial.println("Valid command: Reboot");
    Serial.println("Reboot received");
    delay(1000);
    ESP.restart();

  } else if (command == "Eeprom") {
    Serial.println("Valid command: Eeprom");
    Serial.println("Reboot received");
    restoreColors();
    delay(1000);
  //  ESP.restart();
  } else {
    Serial.println("Unknown command: " + command);
  }
}
bool isLedOn(int ledIndex) {
  // Check if the specified LED is currently ON
  if (ledIndex >= 1 && ledIndex <= numLEDs) {
    uint32_t color = strip.getPixelColor(ledIndex - 1);
    return (color != 0);
  }
  return false;
}

void setLedColor(int ledIndex, byte* color) {
  // Set the LED strip color for the specified LED index
  if (ledIndex >= 1 && ledIndex <= numLEDs) {
    strip.setPixelColor(ledIndex - 1, color[0], color[1], color[2]);
    strip.show();
  }
}


void turnOffLED(int ledIndex) {
  // Turn off the specified LED in the strip
  if (ledIndex >= 1 && ledIndex <= numLEDs) {
    strip.setPixelColor(ledIndex - 1, 0);
    strip.show();
  }
}


void saveColors() {
  // Save the colors to EEPROM
//  if (capsLEDOn) {
    for (int i = 0; i < colorSize; i++) {
      EEPROM.write(eepromAddress + i, capsColor[i]);
    }
 // }
//  if (numLEDOn) {
    for (int i = 0; i < colorSize; i++) {
      EEPROM.write(eepromAddress + colorSize + i, numColor[i]);
    }
//  }
//  if (scrollLEDOn) {
    for (int i = 0; i < colorSize; i++) {
      EEPROM.write(eepromAddress + 2 * colorSize + i, scrollColor[i]);
    }
//  }
  EEPROM.commit();

  Serial.println("Colors saved to EEPROM");
}

void restoreColors() {
  // Restore the colors from EEPROM
  for (int i = 0; i < colorSize; i++) {
    capsColor[i] = EEPROM.read(eepromAddress + i);
    numColor[i] = EEPROM.read(eepromAddress + colorSize + i);
    scrollColor[i] = EEPROM.read(eepromAddress + 2 * colorSize + i);
  }

  Serial.println("Colors restored from EEPROM:");
  printColor("numColor", numColor);
  printColor("capsColor", capsColor);
  printColor("scrollColor", scrollColor);
}

void updateColor(const String& colorCommand, byte* color) {
  if (colorCommand == "Red") {
    color[0] = 255;
    color[1] = 0;
    color[2] = 0;
  } else if (colorCommand == "Green") {
    color[0] = 0;
    color[1] = 255;
    color[2] = 0;
  } else if (colorCommand == "Blue") {
    color[0] = 0;
    color[1] = 0;
    color[2] = 255;
  } else if (colorCommand == "Purple") {
    color[0] = 255;
    color[1] = 0;
    color[2] = 255;
  } else if (colorCommand == "White") {
    color[0] = 255;
    color[1] = 255;
    color[2] = 255;
  } else if (colorCommand == "Yellow") {
    color[0] = 255;
    color[1] = 255;
    color[2] = 0;
  } else if (colorCommand == "Aqua") {
    color[0] = 0;
    color[1] = 255;
    color[2] = 255;
  } else if (colorCommand == "Orange") {
    color[0] = 255;
    color[1] = 165;
    color[2] = 0;
  } else if (colorCommand == "Pink") {
    color[0] = 255;
    color[1] = 55;
    color[2] = 180;
  } else if (colorCommand == "Magenta") {
    color[0] = 250;
    color[1] = 0;
    color[2] = 165;
  } else if (colorCommand == "Lime") {
    color[0] = 154;
    color[1] = 235;
    color[2] = 0;
  } else {
    Serial.println("Unknown color command: " + colorCommand);
  }
}


void printColor(const String& colorName, const byte* color) {
  Serial.print(colorName);
  Serial.print(": R=");
  Serial.print(color[0]);
  Serial.print(" G=");
  Serial.print(color[1]);
  Serial.print(" B=");
  Serial.println(color[2]);
}

void turnOffAllLEDs() {
  // Turn off all LEDs in the strip
  for (int i = 0; i < numLEDs; i++) {
    strip.setPixelColor(i, 0);
  }
  strip.show();
}

void setAllLEDsToColor(int R, int G, int B) {
  // Set the color on all LEDs in the strip
  for (int i = 0; i < numLEDs; i++) {
    strip.setPixelColor(i, strip.Color(R, G, B));
  }
  strip.show();
}
void setLEDColor(int ledIndex, int R, int G, int B) {
  strip.setPixelColor(ledIndex, strip.Color(R, G, B));
  strip.show();
}

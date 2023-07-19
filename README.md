# Mimic for KeyzPal (KeyzPal_Mimic)

**KeyzPal_Mimic** is an ESP8266-based project that allows you to control an LED strip using UDP commands sent over a network. It is designed to mimic the behavior of a keyboard's Caps Lock, Num Lock, and Scroll Lock LEDs, but with an LED strip.

## Features

-   Emulates the Caps Lock, Num Lock, and Scroll Lock LEDs using an LED strip.
-   Listens for UDP commands to control the LED colors and states.
-   Supports customization of LED colors for each lock state.
-   Saves and restores LED colors from EEPROM to retain settings after a power cycle.
-   Works with any ESP8266 development board and compatible LED strip.
- 
## Software

-   This hardware is controlled by **KeyzPal** tray utility for windows.

## Requirements

-   ESP8266 module (tested with ESP01)
-   LED strip (NeoPixel or WS2812B)
-   Arduino IDE (or compatible development environment)
-   ESP8266WiFi library
-   WiFiUdp library
-   Adafruit_NeoPixel library

## Installation

1.  Clone or download the KeyzPal_Mimic repository.
2.  Open the Arduino IDE and navigate to **Sketch -> Include Library -> Manage Libraries**.
3.  Search for and install the following libraries:
    -   ESP8266WiFi
    -   WiFiUdp
    -   Adafruit_NeoPixel
4.  Connect your ESP8266 module to your computer.
5.  Open the **KeyzPal_Mimic.ino** file in the Arduino IDE.
6.  Modify the code as needed (e.g., enter your network SSID and password).
7.  Upload the code to your ESP8266 module.
8.  Connect the LED strip to the appropriate GPIO pin on your ESP8266 module.

## Usage

1.  Power on the ESP8266 module.
2.  Ensure that the ESP8266 is connected to your Wi-Fi network.
3.  The LED strip will initially turn red while it connects to Wi-Fi and initializes.
4.  Once connected, the LED strip will turn green briefly, then turn off.
5.  The KeyzPal_Mimic system is now ready to receive commands.

### Testing:

You can send UDP commands to the KeyzPal_Mimic system to control the LED strip. The commands are as follows:

-   **capsON**: Turn on the Caps Lock LED.
-   **capsOFF**: Turn off the Caps Lock LED.
-   **numON**: Turn on the Num Lock LED.
-   **numOFF**: Turn off the Num Lock LED.
-   **scrollON**: Turn on the Scroll Lock LED.
-   **scrollOFF**: Turn off the Scroll Lock LED.
-   **caps`color`**: Set the color of the Caps Lock LED. Replace `color` with a supported color name (e.g., Red, Green, Blue).
-   **num`color`**: Set the color of the Num Lock LED. Replace `color` with a supported color name.
-   **scroll`color`**: Set the color of the Scroll Lock LED. Replace `color` with a supported color name.
-   **Reboot**: Restart the ESP8266 module.
-   **Eeprom**: Restore saved LED colors from EEPROM (for diagnostic purposes).

### Supported Colors

The following colors are supported for LED customization:

-   Red
-   Green
-   Blue
-   Purple
-   White
-   Yellow
-   Aqua
-   Orange
-   Pink
-   Magenta
-   Lime

## Contributing

Contributions to the KeyzPal_Mimic project are welcome! If you encounter any issues or have suggestions for improvements, please feel free to open an issue or submit a pull request.

## License

This project is licensed under the Apache license.

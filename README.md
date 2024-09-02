# ESP32 Web Server with DHT22 Sensor

## Introduction
This project uses an ESP32 microcontroller to create a simple web server that displays the temperature and humidity readings from a DHT22 sensor. The ESP32 is a powerful and versatile microcontroller that can be used for a wide range of IoT and embedded projects.

## Hardware Required
*   ESP32 development board
*   DHT22 temperature and humidity sensor
*   Jumper wires
*   Breadboard

## Software Required
* Arduino IDE or PlatformIO  
* Arduino ESP32 library

## Circuit Diagram
Connect the DHT22 sensor to the ESP32 as follows:
1.  DHT22 VCC pin to ESP32 3.3V
2.  DHT22 GND pin to ESP32 GND
3.  DHT22 DATA pin to any GPIO pin on the ESP32 (e.g., GPIO 4)

## How to Use
1.  Connect the DHT22 sensor to the ESP32 as shown in the circuit diagram.
2.  Open the Arduino IDE or PlatformIO and create a new project.
3.  Copy the code from the project repository and paste it into your project.
4.  Update the ssid and password variables with your Wi-Fi network credentials.
5.  Upload the code to your ESP32 board.
6.  Open a web browser and navigate to the IP address of your ESP32 board (e.g., http://192.168.1.100).
7.  You should see the temperature and humidity values displayed on the web page.

## Troubleshooting
*   If you're having trouble connecting to the web server, make sure your WiFi credentials are correct and that the ESP32 is connected to the network.
*   If you're not getting any sensor readings, double-check the wiring between the ESP32 and the DHT22 sensor.
*   If you encounter any other issues, consult the project repository or the Arduino ESP32 library documentation for more information.

## Conclusion
This project demonstrates how to use an ESP32 microcontroller to create a simple web server that displays data from a DHT22 temperature and humidity sensor. You can use this as a starting point for your own IoT projects and build upon it to add more functionality, such as logging sensor data or controlling other devices.
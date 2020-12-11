# 19-21_VTC-APL-Electronic-A-Group2_Final_Project
Final project for the 19-21 VTC APL Electronic class A. This is the repository for group 2.

## Overview
The aim of this project is to create an electrical system monitor that can be easily implemented to remote embedded devices. The system monitor the electrical status of a power source, feeding the information to an external device which tracks the status and able to send signals to the system for it to react.

To simplify and minimize the module, onboard control is not supported, remote monitoring can still be done easily using a microcomputer.

## Software Design
The software part consist of the firmware for running the system and a library for the client software to control the system

![Refer to connection-flowchart in repo](https://github.com/R3G3N3R4T0R/19-21_VTC-APL-Electronic-A-Group2_Final_Project/blob/main/connection-flowchart.png?raw=true)

Flow of the firmware

![Refer to devflow in repo](https://github.com/R3G3N3R4T0R/19-21_VTC-APL-Electronic-A-Group2_Final_Project/blob/main/devflow.png?raw=true)

### Client Library
The pwrmonclient library is a wrapper for the QBluetooth BLE library to abstract the Qt5 Components from the user. Qt5 is required for the compilation of the library.
The wrapper allows easy modification of the communication format for custom BLE services. the current characteristics of a service is accessed as a string and output as a stringstream delimited with "|".
For the client developer it abstracts away the connection process and Qt signal and slot. The entire connection process is down to scan, connect, access and disconnect.

BLE Descriptor is not implemented and probably never will. So does error logging and other important things

## Dependencies
### Device firmware
- [Arduino Core](https://github.com/arduino/Arduino)
- Arduino Bluetooth (yet to be supported)
#### ESP32
- [Arduino ESP32 Board Definition](https://github.com/espressif/arduino-esp32)
- [ESP32 BLE library](https://github.com/nkolban/ESP32_BLE_Arduino)[(Ardunino IDE)](https://www.arduino.cc/reference/en/libraries/esp32-ble-arduino)
### Client Controller(Library)
- [Qt Bluetooth 5.15.2](https://doc.qt.io/qt-5/qtbluetooth-index.html)
### Documentation
- [Doxygen](https://www.doxygen.nl/index.html)

## Tools
### ESP32 Mainboard
- [CH340 USB Driver](http://www.wch.cn/download/CH341SER_EXE.html)
### Development Environment
- [Vim](www.vim.org)
- [Arduino IDE](https://www.arduino.cc/en/Main/Software)
- [Qt Creator](https://github.com/qt-creator/qt-creator)

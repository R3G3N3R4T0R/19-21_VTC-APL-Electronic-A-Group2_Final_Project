# 19-21_VTC-APL-Electronic-A-Group2_Final_Project
Final project for the 19-21 VTC APL Electronic class A. This is the repository for group 2.

## Overview
The aim of this project is to create an energy monitoring system that can be easily implemented to a EV or other remote device. The system monitor the electrical status of a power source, feeding the information to an external device which tracks the status and able to send signals to the system for it to react.

To simplify and minimize the module, onboard control is not supported, remote monitoring can still be done easily using a microcomputer.

## Software Design
The software part consist of the firmware for running the system and a library for the client software to control the system
![Refer to connection-flowchart in repo]

## Dependencies
### Common
### Device firmware
Arduino Default Library
### Client Controller
Qt5

## Tools
### ESP32 Mainboard
[CH340 USB Driver](http://www.wch.cn/download/CH341SER_EXE.html)
[Arduino ESP32 Board Definition](https://github.com/espressif/arduino-esp32)
[ESP32 BLE library](https://github.com/nkolban/ESP32_BLE_Arduino)
### Development Environment
[Vim](www.vim.org)
[Arduino IDE](https://www.arduino.cc/en/Main/Software)
[Qt Creator](https://github.com/qt-creator/qt-creator)

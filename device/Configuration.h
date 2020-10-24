#pragma once

/********
 * PINS *
 ********/
  //Input ADC (32-39)
#define PIN_POTENTIOMETER 34
#define PIN_AMMETER 35
#define PIN_THERMISTOR 39
#define PINS_IN1 {34,35,39} //array of pins set to be inputs with adc1
    //Input ADC2 (0,2,3,12-15,25-27)
//#define PINS_IN2 {} //array of pins set to be inputs with adc2, comment out if not needed

  //Output DAC
//#define PINS_OUT {} //array of pins set to be output, comment out if not needed

/*********
 * BOARD *
 *********/

/************
 * WIRELESS *
 ************/
#define WL_DEVICE_NAME "SYSTEM_MONITOR" //Device name when advertising service
  //Bluetooth LE
#define SRVC_UUID "4bd6d07f-3de8-4bfc-8652-5761fcb81bcb" //Service UUID of the BLE server
#define CHAR_UUID "eb402e5a-51d6-4280-8b62-441f93fddd70" //Characteristic UUID of the BLE server

/*************
 * FUNCTIONS *
 *************/

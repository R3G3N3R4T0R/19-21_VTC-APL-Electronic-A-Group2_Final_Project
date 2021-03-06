#pragma once

/********
 * PINS *
 ********/
  //Input ADC (32-39)
#define PIN_POTENTIOMETER 34
#define PIN_AMMETER 35
#define PIN_THERMISTOR1 39
#define PINS_IN1 {34,35,39} //array of pins set to be inputs with adc1
    //Input ADC2 (0,2,3,12-15,25-27)
//#define PINS_IN2 {} //array of pins set to be inputs with adc2, comment out if not needed

  //Output DAC
#define PIN_RELAY 18
#define PINS_OUT {18} //array of pins set to be output, comment out if not needed

/*********
 * BOARD *
 *********/
#define LOOP_DELAY_INTERVAL 250 // loop delay in msec

    //Maximum Value, translate 1023 from the sensor to it actual representation
#define V_MAX 15
#define I_MAX 5
#define T_MIN 0
#define T_MAX 75

/************
 * WIRELESS *
 ************/
#define WL_DEVICE_NAME "SYSTEM_MONITOR" //Device name when advertising service
  //Bluetooth LE
#define SRVC_UUID "4bd6d07f-3de8-4bfc-8652-5761fcb81bcb" //Service UUID of the BLE server
#define CHAR1_UUID "eb402e5a-51d6-4280-8b62-441f93fddd70" //Electrical Characteristic UUID of the BLE server
#define CHAR2_UUID "137ae80c-8287-43b0-ac82-d2c7bc28b390" //Thermals Characteristic UUID of the BLE server
#define CHAR3_UUID "ad54a8ca-e96b-43e1-b057-d17d75078c90" //Receiver Characteristic UUID of the BLE server

/*************
 * FUNCTIONS *
 *************/

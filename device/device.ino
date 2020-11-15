#include "Arduino.h"
#include <string.h>
#include "Configuration.h"
#include "EEPROM.h"
#ifndef __AVR__
    #include "BLEDevice.h"
    #include "BLEUtils.h"
    #include "BLEServer.h"
#endif
/********************
 * DEVICE FIRMWARE SOURCE CODE FOR THE FINAL PROJECT OF 19-21 VTC APL EE
 * THE SOURCE IS PUBLISHED WITH THE MIT LICENSE ON GITHUB
 * THE CODE WILL NOT BE MAINTAINED AFTER SUBMISSION
 * PLEASE CHECK THE DOCUMENTATION FOR NECCESARY DEPENDENCIES
 ********************/

void setup()
{
    if(1){ //Scope controller
    //PIN SETUP
    const int pins_in1[] = PINS_IN1;
    for(int pin = 0; pin < sizeof(pins_in1)/sizeof(pins_in1[0]); pin++)
    {
        pinMode(pins_in1[pin], INPUT);
    }
#ifdef PINS_IN2
    const int pins_in2[] = PINS_IN2;
    for(int pin = 0; pin < sizeof(pins_in2)/sizeof(pins_in2[0]); pin++)
    {
        pinMode(pins_in2[pin], INPUT);
    }
#endif
#ifdef PINS_OUT
    const int pins_out[] = PINS_OUT;
    for(int pin = 0; pin < sizeof(pins_out)/sizeof(pins_out[0]); pin++)
    {
        pinMode(pins_out[pin],OUTPUT);
    }
    digitalWrite(PIN_RELAY, HIGH);
    } // End of scope controller
#endif
    //BOARD INIT
    //EEPROM FETCH
    //WIRELESS
      //BLE
    BLEDevice::init(WL_DEVICE_NAME); // Initialize Module
    BLEServer *ble_server = BLEDevice::createServer(); //Create Server
    BLEService *ble_srvc = ble_server->createService(SRVC_UUID); // Create Service using UUID
      //Create Characteristics using UUID
    BLECharacteristic *ble_char_electric = ble_srvc->createCharacteristic(CHAR1_UUID, BLECharacteristic::PROPERTY_READ); //Electrical State Monitoring
    BLECharacteristic *ble_char_thermals = ble_srvc->createCharacteristic(CHAR2_UUID, BLECharacteristic::PROPERTY_READ); //Thermal Monitoring
    BLECharacteristic *ble_char_receiver = ble_srvc->createCharacteristic(CHAR3_UUID, BLECharacteristic::PROPERTY_WRITE | BLECharacteristic::PROPERTY_READ); //Signal Receiver
    ble_char_receiver->setValue("\0"); //Receiver default value
      //Advertising Protocols
    BLEAdvertising *ble_ad = BLEDevice::getAdvertising();
    ble_ad->addServiceUUID(SRVC_UUID);
    ble_ad->setScanResponse(true);
    ble_ad->setMinPreferred(0x06);  // functions that help with iPhone connections issue
    ble_ad->setMinPreferred(0x12);
    BLEDevice::startAdvertising();

// void loop()
    while(1){ // Initiate infinite loop

    //NETWORK IN
    static uint8_t flags = 2; // flag position | reserve 0000 | sleep 0 | fetch 0 | relay state+flag 10
    std::string data_rcv = ble_char_receiver->getValue(); //fetch value from BLE Client
    ble_char_receiver->setValue("\0") //reset receiver characteristic once fetched
    uint8_t data;
    memcpy(&data, data_rcv.c_str(), sizeof(data)); //transfer to a c string

    flags &= ~(0b00001101); //0ing data bits, relay flag is left on its own
    flags |= (data & 0b00001101); //Bits assigning and masking read only bits from data

    //FETCH READINGS
    static int current, voltage, systemp = 0;

    if(flags & 0b00000100) // fetch flag true
    {
        current = analogRead(PIN_AMMETER);
        voltage = analogRead(PIN_POTENTIOMETER);
        systemp = analogRead(PIN_THERMISTOR1);
    }
    //OPERATIONS
    if(flags & 0b00000001) // relay flag true
    {
        digitalWrite(PIN_RELAY, !digitalRead(PIN_RELAY)); // Reverse current state
        flags ^= 0b00000010; // Flip state
    }

    if(flags & 0b00001000) // sleep flag true
    {
#ifndef __AVR__
        esp_deep_sleep_start();
        flags &= ~(0b00001000); //flip down sleep flag if hardware woke up
#endif
    }

    //NETWORK OUT
    static char data_pkg[20]; //cstring for packaging data
    if(flags & 0b00000100) //fetch flag true
    {
        //Electrical data
        sprintf(data_pkg, "%d-%d_%d-%d", voltage, current, V_MAX, I_MAX);
        ble_char_electric->setValue(data_pkg);
        //Thermal data
        sprintf(data_pkg, "%d_%d-%d", systemp, T_MIN, T_MAX);
        ble_char_thermals->setValue(data_pkg);
        //Flip down fetch flag
        flags &= ~(0b00000100);
    }
    delay(LOOP_DELAY_INTERVAL);
    
    } // End of infinite loop
}

void loop(){} // dummy void loop

/*
//For makefile users
int main()
{
    init();
    setup();
    while(1)
    loop();
}
*/

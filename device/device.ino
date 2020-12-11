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
 * THE SOURCE IS PUBLISHED ON GITHUB UNDER APACHE 2.0 LICENSE
 * THE CODE WILL NOT BE MAINTAINED AFTER SUBMISSION
 * PLEASE CHECK README.md FOR NECCESARY DEPENDENCIES
 ********************/

void setup()
{
    //Enumeration
    enum BitMask
    {
        BitMask_rflag = 0x01,
        BitMask_rstat = 0x02,
        BitMask_fetch = 0x04,
        BitMask_sleep = 0x08,
        BitMask_data  = 0x0D
    };
    
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
      //Start BLE Service
    ble_srvc->start();
      //Advertising Protocols
    BLEAdvertising *ble_ad = BLEDevice::getAdvertising();
    ble_ad->addServiceUUID(SRVC_UUID);
    ble_ad->setScanResponse(true);
    ble_ad->setMinPreferred(0x06);  // functions that help with iPhone connections issue
    ble_ad->setMinPreferred(0x12);
    BLEDevice::startAdvertising();

//Start of Loop
while(1)
{
    //NETWORK IN
    static uint8_t flags = 2; // flag position | reserve 0000 | sleep 0 | fetch 0 | relay state+flag 10

    flags |= (ble_char_receiver->getData()[0] & BitMask_data); //fetch only data bits from BLE Client
    uint8_t flags_char = (flags & ~(BitMask_data)); //filter the state for the response
    ble_char_receiver->setValue(&flags_char, 1); //respond receiver characteristic once fetched

    //FETCH READINGS
    static int current, voltage, systemp = 0;

    if(flags & BitMask_fetch) // fetch flag true
    {
        current = analogRead(PIN_AMMETER);
        voltage = analogRead(PIN_POTENTIOMETER);
        systemp = analogRead(PIN_THERMISTOR1);
    }
    //OPERATIONS
    if(flags & BitMask_rflag) // relay flag true
    {
        digitalWrite(PIN_RELAY, !digitalRead(PIN_RELAY)); // Reverse current state
        flags ^= BitMask_rstat; // Flip state
    }

    if(flags & BitMask_sleep) // sleep flag true
    {
#ifndef __AVR__
        esp_deep_sleep_start();
#endif
    }

    //NETWORK OUT
    static char data_pkg[20]; //cstring for packaging data
    if(flags & BitMask_fetch) //fetch flag true
    {
        //Electrical data
        sprintf(data_pkg, "%d-%d_%d-%d", voltage, current, V_MAX, I_MAX);
        ble_char_electric->setValue(data_pkg);
        //Thermal data
        sprintf(data_pkg, "%d_%d-%d", systemp, T_MIN, T_MAX);
        ble_char_thermals->setValue(data_pkg);
    }

    flags &= ~(BitMask_data); //0ing data bits, state flags are left on their own
    delay(LOOP_DELAY_INTERVAL);
} // while(1)   
} // setup()

void loop(){}//Dummy loop() for Arduino IDE

/*
//For makefile users
int main()
{
    init();
    setup();
}
*/

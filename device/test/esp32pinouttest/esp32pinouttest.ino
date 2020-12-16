#include "Arduino.h"
#include <string.h>
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
    Serial.begin(115200);
    delay(1000);
}

void loop()
{
    for(int pin = 16; pin <= 39; pin++)
        if(digitalRead(pin) == HIGH)
            Serial.println(pin);
}

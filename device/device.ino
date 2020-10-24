#include "Arduino.h"
#include "Configuration.h"
#include "EEPROM.h"
#include "WiFi.h"
#ifdef(ESP32_DEV)
	#include "BLEDeivce.h"
	#include "BLEUtils.h"
	#include "BLEServer.h"
	#include "BLE_2902.h"
#endif
/********************
 * DEVICE FIRMWARE SOURCE CODE FOR THE FINAL PROJECT OF 19-21 VTC APL EE
 * THE SOURCE IS PUBLISHED WITH THE MIT LICENSE ON GITHUB
 * THE CODE WILL NOT BE MAINTAINED AFTER SUBMISSION
 * PLEASE CHECK THE DOCUMENTATION FOR NECCESARY DEPENDENCIES
 ********************/

void setup()
{
	//PIN SETUP
	const int pins_in1[] = PINS_IN1;
	for(int pin = 0; pin < sizeof(pins_in1[])/sizeof(pins_in1[0]); pin++)
	{
		pinMode(pins_in2[pin], INPUT);
	}
#ifdef PINS_IN2
	const int pins_in2[] = PINS_IN2;
	for(int pin = 0; pin < sizeof(pins_in2[])/sizeof(pins_in2[0]); pin++)
	{
		pinMode(pins_in2[pin], INPUT);
	}
#endif
#ifdef PINS_OUT
	const int pins_out[] = PINS_OUT;
	for(int pin = 0; pin < sizeof(pins_out[])/sizeof(pins_out[0]); pin++)
	{
		pinMode(pins_out[pin],OUTPUT);
	}
#endif
	//BOARD INIT
	//EEPROM FETCH
	//WIRELESS
	  //BLE
	BLEDevice::init("WL_DEVICE_NAME");
	BLEServer *ble_server = BLEDevice::createServer();
	BLEService *ble_srvc = ble_server->createService(SRVC_UUID);
	BLECharacteristic ble_char1 = ble_srvc->createCharacteristic(CHAR_UUID, BLECharacteristic::PROPERTY_READ);

	BLEAdvertisiing *ble_ad = BLEDevice::getAdvertising();
	ble_ad->addServiceUUID(SRVC_UUID);
	ble_ad->setScanResponse(true);
	ble_ad->setMinPreferred(0x06);  // functions that help with iPhone connections issue
	ble_ad->setMinPreferred(0x12);
	BLEDevice::startAdvertising();
}

void loop()
{	
	//VARIABLE DECLARATION
	static int current, voltage, systemp;
	bool sleep_flag;
	//NETWORK IN
	//FETCH READINGS
	if(data_rcv != NULL)
	{
		current = analogRead(PIN_AMMETER);
		voltage = analogRead(PIN_POTENTIOMETER);
		systemp = analogRead(PIN_THERMISTOR);
	}
	//OPERATIONS
	if(sleep_flag)
	{
#if defined(ESP32_DEV)
		esp_deep_sleep_start();
#endif
	}
	//NETWORK OUT
}

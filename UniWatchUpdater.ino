#include <ESP8266httpUpdate.h>
#include <ESP8266WiFi.h>
#include <ArduinoOTA.h>
#include <ESP8266HTTPClient.h>
#include "FS.h"
/*
 * This sketch is meant to be uploaded with the following files already in the filesystem:
 * OTAWIFI - contaist two lines - first contains SSID and the second one the password. Lines are separated by \n
 *
 * If there is no such a file on a fs, then set the options accordingly
 */

//#define OTAWIFI_FILE_NOT_AVAILABLE
//#define OTA_SSID "test"
//#define OTA_password "test"
int ConnectToWifi(FS &spiffs)
{
#ifndef OTAWIFI_FILE_NOT_AVAILABLE
	auto f = spiffs.open("/OTAWIFI", "r");
	char networkName[50];
	auto l = f.readBytesUntil('\n', &networkName[0],50);
	networkName[l] = '\0';

	char password[50];
	l = f.readBytesUntil('\n', &password[0], 50);
	password[l] = '\0';

	WiFi.begin(networkName, password);
	delay(3000);
	return WiFi.isConnected();
#else
	WiFi.begin(OTA_SSID, OTA_password);
	delay(3000);
	return WiFi.isConnected();
#endif
}
void SetupOTA(){
    ArduinoOTA.onStart([]() {
      String type;
      if (ArduinoOTA.getCommand() == U_FLASH) {
        type = "sketch";
      }
      else { // U_SPIFFS
        type = "filesystem";
      }

      // NOTE: if updating SPIFFS this would be the place to unmount SPIFFS using SPIFFS.end()
      Serial.println("Start updating " + type);
    });
    ArduinoOTA.onEnd([]() {
      Serial.println("\nEnd");
    });
    ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
      Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
    });
    ArduinoOTA.onError([](ota_error_t error) {
      Serial.printf("Error[%u]: ", error);
      if (error == OTA_AUTH_ERROR) {
        Serial.println("Auth Failed");
      }
      else if (error == OTA_BEGIN_ERROR) {
        Serial.println("Begin Failed");
      }
      else if (error == OTA_CONNECT_ERROR) {
        Serial.println("Connect Failed");
      }
      else if (error == OTA_RECEIVE_ERROR) {
        Serial.println("Receive Failed");
      }
      else if (error == OTA_END_ERROR) {
        Serial.println("End Failed");
      }
    });
    ArduinoOTA.begin();
   }
void setup() {
	Serial.begin(115200);
	SPIFFS.begin();
	if (ConnectToWifi(SPIFFS))
	{
		Serial.printf("Connected!");
		SetupOTA();
		Serial.printf("OTA READY!");
	}else
	{
		Serial.printf("Could not connect\n");
	}
	
}

void loop() {
  // put your main code here, to run repeatedly:

}

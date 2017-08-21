#include "./OTA.h"

#include "FS.h"

Logger OTA_Logger("OTA");

void boot_OTA(){
    ArduinoOTA.setPort(8266);
    ArduinoOTA.onStart([]() {
        SPIFFS.end();
        OTA_Logger.enable();
        OTA_Logger.log("Update Start");
        pinMode(LED_BUILTIN, OUTPUT);
    });

    ArduinoOTA.onEnd([]() {
        OTA_Logger.disable();
        OTA_Logger.log("Update End");
        digitalWrite(LED_BUILTIN, 1);
        ESP.reset();
    });
    ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
        digitalWrite(LED_BUILTIN, 0);
        OTA_Logger.log("Progress:\t"+ String((progress*100)/total) +" %");
        digitalWrite(LED_BUILTIN, 1);
    });
    ArduinoOTA.onError([](ota_error_t error) {
        digitalWrite(LED_BUILTIN, 0);
        if (error == OTA_AUTH_ERROR) OTA_Logger.log("Auth Failed");
        else if (error == OTA_BEGIN_ERROR) OTA_Logger.log("Begin Failed");
        else if (error == OTA_CONNECT_ERROR) OTA_Logger.log("Connect Failed");
        else if (error == OTA_RECEIVE_ERROR) OTA_Logger.log("Receive Failed");
        else if (error == OTA_END_ERROR) OTA_Logger.log("End Failed");
    });
    ArduinoOTA.begin();
}

void service_OTA(){
    ArduinoOTA.handle();
}
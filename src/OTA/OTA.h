#ifndef _OTA_H
#define _OTA_H

#include <Arduino.h>
#include <ESP8266mDNS.h>
#include <ArduinoOTA.h>

#include "../Logger/Logger.h"

void boot_OTA();
void service_OTA();

#endif
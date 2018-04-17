#ifndef _WiFi_h
#define _WiFi_h

#include <Arduino.h>
#include <ESP8266WiFi.h>

#include <WiFiClient.h>
#include "../Logger/Logger.h"
extern const char *ssid;
extern const char *password;

void boot_WiFi();
String listNetworks();

#endif
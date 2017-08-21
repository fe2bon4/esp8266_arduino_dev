#ifndef _WEB_H
#define _WEB_H

#include <Arduino.h>
#include <ESP8266WebServer.h>
#include "../Logger/Logger.h"
 
extern ESP8266WebServer server;
void boot_WEB();
void servive_Web();



#endif
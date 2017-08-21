#include "./Logger.h"

// Member functions definitions including constructor
Logger::Logger(String type){
    LOG.println();
    isEnabled = false;
    Type = type;
 }

void Logger::enable() {
    isEnabled = true;
    log("Enabling Logger");
 }
void Logger::disable() {
    isEnabled = false;
 }
 void Logger::log( String logString) {
    if(isEnabled){
        LOG.println(Type+": "+logString);
    }
 }
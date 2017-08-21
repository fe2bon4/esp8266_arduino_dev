#ifndef _LOGGER_H
#define _LOGGER_H

#include <Arduino.h>

#define LOG Serial
class Logger{
    private:
        bool isEnabled;
        String Type;
        String LogPath;
    public:
        void enable();
        void disable();
        void log(String);
        Logger(String);
};



#endif
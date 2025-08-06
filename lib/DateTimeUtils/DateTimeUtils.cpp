#include <Arduino.h>
#include "DateTimeUtils.h"

String DateTimeUtils::formatFromTimestamp(unsigned long timestamp, uint8_t hoursOffset, bool showDate, bool showTime){
    tm* t = getLocalTime(timestamp, hoursOffset);

    char str[30];
    sprintf(str, "%02d.%02d.%04d %02d:%02d", t->tm_mday, t->tm_mon, 1900 + t->tm_year, t->tm_hour, t->tm_min);
    //delete(t);

    return String(str);
}

int DateTimeUtils::getHourFromTimestamp(unsigned long timestamp, uint8_t hoursOffset){
    tm* time = getLocalTime(timestamp, hoursOffset);
    int hour = time->tm_hour;
    //delete(time);
    
    return hour;
}

tm* DateTimeUtils::getLocalTime(unsigned long timestamp, uint8_t hoursOffset){
    time_t rawTime = timestamp + (hoursOffset * 60*60);
    return localtime(&rawTime);
}
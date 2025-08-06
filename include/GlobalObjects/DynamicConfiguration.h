#ifndef DynamicConfiguration_H
#define DynamicConfiguration_H

#include <Arduino.h>

class DynamicConfiguration {    
public:
    int MeasurementDurationInSeconds = 60;
    int MeasurementCalmDownInSeconds = 60;

    uint8_t PM2_5_Level_Warning = 36;
    uint8_t PM2_5_Level_Alert = 56;
    uint8_t VOC_Percent_Level_Warning = 75;
    uint8_t VOC_Percent_Level_Alert = 80;
    float Radiation_Level_Warning = 0.2;
    float Radiation_Level_Alert = 0.3;
    float Radiation_Level_Critical = 0.4;
};
#endif
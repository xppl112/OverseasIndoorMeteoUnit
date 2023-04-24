#ifndef GlobalState_H
#define GlobalState_H

#include <GlobalObjects/DynamicConfiguration.h>
#include <config.h>
#include <Arduino.h>

class GlobalState {    
public:
    DynamicConfiguration configuration;
    uint8_t lightLevelPercent = 100;

    bool isDark() {return lightLevelPercent < LIGHT_LEVEL_FOR_DARKNESS; }

};
#endif
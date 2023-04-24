#include "HardwareModules/BuzzerModule.h"

BuzzerModule::BuzzerModule(PCF8575 *pcf, uint8_t pin)
{
    _pcf = pcf;
    _pin = pin;
}

void BuzzerModule::connect(){
    _pcf->pinMode(_pin, OUTPUT);
    _pcf->digitalWrite(_pin, HIGH);
    _isConnected = true;
}

void BuzzerModule::reset(){
}

void BuzzerModule::beep(uint8_t count, short beepLength, short pauseLength) {
    for(uint8_t i = 0; i < count; i++){
        _pcf->digitalWrite(_pin, LOW);
        delay(beepLength);
        _pcf->digitalWrite(_pin, HIGH);
        if(i+1 != count)delay(pauseLength);
    }
}
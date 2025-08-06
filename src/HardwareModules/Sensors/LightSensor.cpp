#include "HardwareModules/Sensors/LightSensor.h"

LightSensor::LightSensor(uint8_t analogPin){
    _analogPin = analogPin;
    pinMode(_analogPin, INPUT);
}

void LightSensor::connect(){
    _isConnected = true;
}

void LightSensor::reset(){
}

LightSensorData LightSensor::getData(){
    auto rawValue = analogRead(_analogPin);
    auto lightLevel = rawValue * 100 / (PWMRANGE + 1);
    
    LightSensorData data;
    data.isDataReceived = true;
    data.lightLevelInPercent = lightLevel;

    return data;
}
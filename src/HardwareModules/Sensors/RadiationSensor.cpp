#include "HardwareModules/Sensors/RadiationSensor.h"

RadiationSensor::RadiationSensor(uint8_t interuptPin){
    _interuptPin = interuptPin;
    pinMode(_interuptPin, INPUT);
}

void RadiationSensor::connect(){
    _isConnected = true;
}

void RadiationSensor::reset(){
}

RadiationSensorData RadiationSensor::getData(){
    RadiationSensorData data;
    data.isDataReceived = false;
    if(millis()-_lastMeasurementMillis < 10000){
      if(millis() < _lastMeasurementMillis){
        _lastMeasurementMillis = millis();
        _hitCount = 0;
      }
      return data;
    }

    double coefPerMinute = 0.00812037037037;
    data.isDataReceived = true;
    data.RadiationLevel = 
        (double)_hitCount / ((double)(millis()-_lastMeasurementMillis)/(double)1000/(double)60) * coefPerMinute;  

    _lastMeasurementMillis = millis();
    _hitCount = 0;
    return data;
}

void RadiationSensor::registerSignalReceivedCallback(RadiationSensorInterruptCallback callback){
    attachInterrupt(digitalPinToInterrupt(_interuptPin), callback, FALLING); 
}

void RadiationSensor::onRadiationSignalReceived(){
  if(millis()-_previousHitMillis > 10){
    _hitCount++;
    _previousHitMillis = millis();
  }
}


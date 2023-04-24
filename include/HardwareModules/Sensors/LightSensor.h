#ifndef LightSensor_H
#define LightSensor_H

#include "HardwareModules/IHardwareModule.h"
#include "HardwareModules/Sensors/ISensor.h"
#include <Arduino.h>

struct LightSensorData : SensorDataBase {
  int lightLevelInPercent;
};

class LightSensor : public ISensor
{
public:
    LightSensor(uint8_t analogPin);
    HardwareModuleInfo getInfo() override {
        return HardwareModuleInfo {
            .Name = "InternalAnalogLightSensor",
            .ConnectionType = HardwareModuleConnectionType::ANALOG_PINS,
            .PowerVoltage = 3.3,
            .CommunicationVoltage = 3.3,
            .MaxCurrent = 0,
            .IsCritical = false
        };
    };
    short int getFailedDataFetchingTreshold() override { return -1;};

    void connect() override;
    void reset() override;
    bool isConnected() override { return _isConnected; }

    LightSensorData getData();

private:
    uint8_t _analogPin;
    const uint16_t PWMRANGE = 1023;
};

#endif
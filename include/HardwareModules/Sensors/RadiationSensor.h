#ifndef RadiationSensor_H
#define RadiationSensor_H

#include "HardwareModules/IHardwareModule.h"
#include "HardwareModules/Sensors/ISensor.h"
#include <Arduino.h>

typedef void (*RadiationSensorInterruptCallback)();

struct RadiationSensorData : SensorDataBase {
  double RadiationLevel;//uSv/h
};

class RadiationSensor : public ISensor
{
public:
    RadiationSensor(uint8_t interuptPin);
    HardwareModuleInfo getInfo() override {
        return HardwareModuleInfo {
            .Name = "RadiationSensor",
            .ConnectionType = HardwareModuleConnectionType::DIGITAL_PINS,
            .PowerVoltage = 5,
            .CommunicationVoltage = 5,
            .MaxCurrent = 30,
            .IsCritical = false
        };
    };
    short int getFailedDataFetchingTreshold() override { return -1;};

    void connect() override;
    void reset() override;
    bool isConnected() override { return _isConnected; }

    void registerSignalReceivedCallback(RadiationSensorInterruptCallback callback);
    void onRadiationSignalReceived();

    RadiationSensorData getData();

private:
    uint8_t _interuptPin;

    volatile unsigned long _hitCount = 0;
    volatile unsigned long _previousHitMillis = 0;

    volatile unsigned long _lastMeasurementMillis = 0;
};

#endif
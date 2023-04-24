#ifndef MeteoSensor_H
#define MeteoSensor_H

#include "HardwareModules/IHardwareModule.h"
#include "HardwareModules/Sensors/ISensor.h"
#include "BME680Sensor.h"
#include "AHT20Sensor.h"

struct MeteoSensorData : SensorDataBase {
  float temperatureBMECelsium;
  float humidityBME;
  float temperatureAHTCelsium;
  float humidityAHT;
  float pressureInHPascals;
  float gasResistance;
};

class MeteoSensor : public ISensor
{
public:
    MeteoSensor(uint8_t bme_i2c_address, uint8_t aht_i2c_address);
    HardwareModuleInfo getInfo() override {
        return HardwareModuleInfo {
            .Name = "BME680 & AHT20",
            .ConnectionType = HardwareModuleConnectionType::I2C,
            .PowerVoltage = 3.3,
            .CommunicationVoltage = 3.3,
            .MaxCurrent = 0,
            .IsCritical = false
        };
    };
    short int getFailedDataFetchingTreshold() override { return 2;};

    void connect() override;
    void reset() override;
    bool isConnected() override { 
        _isConnected = _bmeSensor->isConnected || _ahtSensor->isConnected; 
        return _isConnected; 
    }

    MeteoSensorData getData();

private:
    BME680Sensor* _bmeSensor;
    AHT20Sensor* _ahtSensor;
};

#endif
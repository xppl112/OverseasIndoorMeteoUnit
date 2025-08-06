#ifndef PCFExtender_H
#define PCFExtender_H

#include "HardwareModules/IHardwareModule.h"
#include "PCF8575.h"
#include <Arduino.h>

class PCFExtender : public IHardwareModule
{
public:
    PCFExtender(uint8_t i2c_address);
    HardwareModuleInfo getInfo() override {
        return HardwareModuleInfo {
            .Name = "PCF8575",
            .ConnectionType = HardwareModuleConnectionType::I2C,
            .PowerVoltage = 3.3,
            .CommunicationVoltage = 3.3,
            .MaxCurrent = 0,
            .IsCritical = true
        };
    };

    void connect() override;
    void reset() override;
    bool isConnected() override { return _isConnected; }

    PCF8575* get(){ return _pcf; }

private:
    PCF8575 *_pcf;
};

#endif
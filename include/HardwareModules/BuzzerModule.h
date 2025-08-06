#ifndef BuzzerModule_H
#define BuzzerModule_H

#include "HardwareModules/IHardwareModule.h"
#include <Arduino.h>
#include "PCF8575.h"

class BuzzerModule : public IHardwareModule
{
public:
    BuzzerModule(PCF8575 *pcf, uint8_t pin);
    HardwareModuleInfo getInfo() override {
        return HardwareModuleInfo {
            .Name = "Active buzzer (PCF)",
            .ConnectionType = HardwareModuleConnectionType::DIGITAL_PINS,
            .PowerVoltage = 3.3,
            .CommunicationVoltage = 3.3,
            .MaxCurrent = 15,
            .IsCritical = false
        };
    };

    void connect() override;
    void reset() override;
    bool isConnected() override { return _isConnected; }

    void beep(uint8_t count = 1, short beepLength = 300, short pauseLength = 300);

private:
    PCF8575 *_pcf;
    uint8_t _pin;
  
};

#endif
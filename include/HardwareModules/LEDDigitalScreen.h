#ifndef LEDDigitalScreen_H
#define LEDDigitalScreen_H

#include "HardwareModules/IHardwareModule.h"
#include <TM1637LED.h>
#include <Arduino.h>

class LEDDigitalScreen : public IHardwareModule
{
public:
    LEDDigitalScreen(
        uint8_t dioPin,
        uint8_t clkPin);
    HardwareModuleInfo getInfo() override {
        return HardwareModuleInfo {
            .Name = "TM1637 LED Digital screen",
            .ConnectionType = HardwareModuleConnectionType::DIGITAL_PINS,
            .PowerVoltage = 3.3,
            .CommunicationVoltage = 3.3,
            .MaxCurrent = 30,
            .IsCritical = false
        };
    };

    void connect() override;
    void reset() override;
    bool isConnected() override { return _isConnected; }

    TM1637LED* get(){ return _screen; }

private:
    TM1637LED *_screen;

    int _dioPin;
    int _clkPin;  
};

#endif

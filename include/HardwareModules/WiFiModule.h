#ifndef WiFiModule_H
#define WiFiModule_H

#include "HardwareModules/IHardwareModule.h"
#include <Arduino.h>
#include <ESP8266WiFi.h>

class WiFiModule : public IHardwareModule
{
public:
    WiFiModule();
    HardwareModuleInfo getInfo() override {
        return HardwareModuleInfo {
            .Name = "Internal ESP Wifi",
            .ConnectionType = HardwareModuleConnectionType::UNKNOWN,
            .PowerVoltage = 3.3,
            .CommunicationVoltage = 3.3,
            .MaxCurrent = 0,
            .IsCritical = false
        };
    };

    void connect() override;
    void reset() override;
    bool isConnected() override { return _isConnected; }

    short GetWifiSignalLevelPercent();
    void sleep(bool enable = true);

private:
    const int MAX_STRENGTH = -20; // define maximum signal strength (in dBm)
    const int MIN_STRENGTH = -80; // define minimum signal strength (in dBm)
  
};

#endif
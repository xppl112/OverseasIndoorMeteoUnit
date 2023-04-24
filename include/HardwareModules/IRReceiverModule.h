#ifndef IRReceiverModule_H
#define IRReceiverModule_H

#include "HardwareModules/IHardwareModule.h"
#include <Arduino.h>
#include <IRremoteESP8266.h>
#include <IRrecv.h>
#include <IRutils.h>

class IRReceiverModule : public IHardwareModule
{
public:
    IRReceiverModule(uint8_t signalPin);
    HardwareModuleInfo getInfo() override {
        return HardwareModuleInfo {
            .Name = "IR Receiver module",
            .ConnectionType = HardwareModuleConnectionType::DIGITAL_PINS,
            .PowerVoltage = 3.3,
            .CommunicationVoltage = 3.3,
            .MaxCurrent = 10,
            .IsCritical = false
        };
    };

    void connect() override;
    void reset() override;
    bool isConnected() override { return _isConnected; }
    
    uint64_t checkSignal();

private:
    IRrecv *_receiver;
    int _signalPin; 
};

#endif

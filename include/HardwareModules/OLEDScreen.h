#ifndef OLEDScreen_H
#define OLEDScreen_H

#include "HardwareModules/IHardwareModule.h"
#include <Adafruit_SSD1306.h>
#include <Arduino.h>

class OLEDScreen : public IHardwareModule
{
public:
    OLEDScreen(
        uint8_t i2c_address,
        int width, 
        int height);
    HardwareModuleInfo getInfo() override {
        return HardwareModuleInfo {
            .Name = "SSD1306 OLED",
            .ConnectionType = HardwareModuleConnectionType::I2C,
            .PowerVoltage = 3.3,
            .CommunicationVoltage = 3.3,
            .MaxCurrent = 50,
            .IsCritical = false
        };
    };

    void connect() override;
    void reset() override;
    bool isConnected() override { return _isConnected; }

    Adafruit_SSD1306* get(){ return _screen; }

private:
    Adafruit_SSD1306 *_screen;

    uint8_t _i2c_address;
    int _width;
    int _height;  
};

#endif

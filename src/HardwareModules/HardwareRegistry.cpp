#include <Arduino.h>
#include "Config.h"
#include "HardwareModules/HardwareRegistry.h"

HardwareRegistry::HardwareRegistry(){
    initializeDevices();
}

void HardwareRegistry::initializeDevices(){
    _airParticiplesSensor = new AirParticiplesSensor(&Serial);
    _radiationSensor = new RadiationSensor(RADIATION_SENSOR_PIN);
    _meteoSensor = new MeteoSensor(BME_I2C_ADDR, AHT_I2C_ADDR);
    _lightSensor = new LightSensor(ANALOG_INPUT_PIN);
    _PCFExtender = new PCFExtender(IO_I2C_ADDR);
    _buzzerModule = new BuzzerModule(_PCFExtender->get(), BEEPER_PIN);
    _IRReceiverModule = new IRReceiverModule(IR_PIN);

    _topLedScreen = new LEDDigitalScreen(SCREEN1_DIO_PIN, SCREENS_CLK_PIN);
    _middleLedScreen = new LEDDigitalScreen(SCREEN2_DIO_PIN, SCREENS_CLK_PIN);
    _bottomLedScreen = new LEDDigitalScreen(SCREEN3_DIO_PIN, SCREENS_CLK_PIN);
    _OLEDScreen = new OLEDScreen(OLED_I2C_ADDR, 128, 64);
}

void HardwareRegistry::reconnectAllDisconnectedDevices(){    
    if(!_airParticiplesSensor->isConnected())_airParticiplesSensor->connect();
    if(!_radiationSensor->isConnected())_radiationSensor->connect();
    if(!_meteoSensor->isConnected())_meteoSensor->connect();
    if(!_lightSensor->isConnected())_lightSensor->connect();
    if(!_PCFExtender->isConnected())_PCFExtender->connect();
    if(!_buzzerModule->isConnected())_buzzerModule->connect();
    if(!_IRReceiverModule->isConnected())_IRReceiverModule->connect();

    if(!_topLedScreen->isConnected())_topLedScreen->connect();
    if(!_middleLedScreen->isConnected())_middleLedScreen->connect();
    if(!_bottomLedScreen->isConnected())_bottomLedScreen->connect();
    if(!_OLEDScreen->isConnected())_OLEDScreen->connect();
}

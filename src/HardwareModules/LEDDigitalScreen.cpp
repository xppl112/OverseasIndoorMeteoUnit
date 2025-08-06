#include "HardwareModules/LEDDigitalScreen.h"

LEDDigitalScreen::LEDDigitalScreen(
    uint8_t dioPin,
    uint8_t clkPin)
{
    _dioPin = dioPin;
    _clkPin = clkPin;

    _screen = new TM1637LED(clkPin, dioPin);
}

void LEDDigitalScreen::connect(){
    _isConnected = _screen->connect();
}

void LEDDigitalScreen::reset(){
    connect();
}
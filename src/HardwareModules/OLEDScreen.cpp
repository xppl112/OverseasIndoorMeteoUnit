#include "HardwareModules/OLEDScreen.h"

OLEDScreen::OLEDScreen(
    uint8_t i2c_address,
    int width, 
    int height)
{
    _i2c_address = i2c_address;
    _width = width;
    _height = height;

    _screen = new Adafruit_SSD1306(width, height);
}

void OLEDScreen::connect(){
    _screen->begin(SSD1306_SWITCHCAPVCC, _i2c_address);
    _isConnected = true;
}

void OLEDScreen::reset(){
    connect();
}
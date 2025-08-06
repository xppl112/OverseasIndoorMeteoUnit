#include "TM1637LED.h"

TM1637LED::TM1637LED(uint8_t clkPin, uint8_t dioPin){
    _clkPin = clkPin;
    _dioPin = dioPin;
}

bool TM1637LED::connect(){
    _display = new TM1637(_clkPin, _dioPin);
    _display->begin();
    _display->setBrightness(8);
    clear();
    isConnected = true;
    return isConnected;
}

void TM1637LED::clear(){
    _display->display("    ");
    _display->refresh();
    //_display->clearScreen();
}

void TM1637LED::setBrightness(uint8_t brightnessLevelPercent){
    _display->changeBrightness(1 + brightnessLevelPercent * (8 - 1) / 100);
    
   _display->refresh();
}

void TM1637LED::print(String str){
    clear();
    _display->display(str);
}

void TM1637LED::printIntWithUnit(int number, char unit){
    auto str = String(number) + String(unit);
    auto offset = 4 - (str.length());
    //_display->colonOff();
    _display->display(str, false, false, offset);
}

void TM1637LED::printDecimal(float number, int decimalsCount){
    auto str = String(number, decimalsCount);
    auto offset = 4 - (str.length()-1);
    _display->display(str, false, false, offset);
}

void TM1637LED::printInt(int number){
    auto str = String(number);
    auto offset = 4 - (str.length());
    _display->display(str, false, false, offset);
}

void TM1637LED::printDecimalWithUnit(float number, char unit){
    auto decimalPlacesCount = number > -10 ? 1 : 0;
    auto str = String(number, decimalPlacesCount);

    //if(decimalPlacesCount == 0)_display->colonOff();
    str += String(unit);  

    auto offset = 4 - (str.length()-decimalPlacesCount);
    _display->display(str, false, false, offset);
}

void TM1637LED::toggleColon(){
    isColonShowed = !isColonShowed;
    if(isColonShowed)_display->colonOn();
    else _display->colonOff();
}
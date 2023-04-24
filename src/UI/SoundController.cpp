#include <Arduino.h>
#include "UI/SoundController.h"
#include "Config.h"

SoundController::SoundController(HardwareRegistry* hardwareRegistry){
    _buzzerModule = hardwareRegistry->_buzzerModule;
    _buzzerModule->connect();

    _timer = new Ticker(1000);
    _timer->start();
}

void SoundController::updateTimer(){
    _timer->update();
    if(_timer->state() == FIRED){
        if(_currentWarningLevel == WarningLevel::CRITICAL && !_isAlertBypassed)longBeep();
        _timer->start();
    }
}

void SoundController::setWarningLevel(WarningLevel level, bool alertBypassed){
    if(_currentWarningLevel != level){
        if(_currentWarningLevel != WarningLevel::HI_WARNING_LEVEL && level == WarningLevel::WARNING)shortBeep();
        if(level == WarningLevel::HI_WARNING_LEVEL)shortBeep(2);
    }

    _currentWarningLevel = level;
    _isAlertBypassed = alertBypassed;
}

void SoundController::shortBeep(uint8_t count){
    _buzzerModule->beep(count, 200, 200);
}

void SoundController::longBeep(uint8_t count){
    _buzzerModule->beep(count, 500);
}

void SoundController::radiationBeep(){
    _buzzerModule->beep(1, 1);
}
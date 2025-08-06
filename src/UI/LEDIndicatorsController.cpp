#include <Arduino.h>
#include "UI/LEDIndicatorsController.h"
#include "Config.h"

LEDIndicatorsController::LEDIndicatorsController(HardwareRegistry* hardwareRegistry, GlobalState* globalState){
    _globalState = globalState;
    _pcf = hardwareRegistry->_PCFExtender;

    _timerSlow = new Ticker(1000, NULL, MILLIS); 
    _timerSlow->start();
    _timerFast = new Ticker(400, NULL, MILLIS); 
    _timerFast->start();

    _pcf->get()->pinMode(STANDBY_LED_PIN, OUTPUT);
    _pcf->get()->pinMode(RAIN1_LED_PIN, OUTPUT);
    _pcf->get()->pinMode(RAIN2_LED_PIN, OUTPUT);
    _pcf->get()->pinMode(RAIN3_LED_PIN, OUTPUT);

    _pcf->get()->pinMode(AIR_0R5_LED_PIN, OUTPUT);
    _pcf->get()->pinMode(AIR_R_LED_PIN, OUTPUT);

    _pcf->get()->pinMode(AIR_0G3_LED_PIN, OUTPUT);
    _pcf->get()->pinMode(AIR_0G5_LED_PIN, OUTPUT);
    _pcf->get()->pinMode(AIR_G_LED_PIN, OUTPUT);

    _pcf->get()->pinMode(AIR_0B5_LED_PIN, OUTPUT);
    _pcf->get()->pinMode(AIR_B_LED_PIN, OUTPUT);

    clearAllIndicators();
    setStandbyMode(false);
}

void LEDIndicatorsController::setWeatherStatus(PresentingIndoorWeatherData data){
    auto weatherData = data.weatherMonitorHistoricalData.back();

    int pm = weatherData.PM_2_5;
    int gas = weatherData.calculateGasAirPollution();
    if(pm >= (int)PM2_5Level::VeryUnhealthy || gas >= (int)GasLevel::VeryUnhealthy)
        setRGBLedColor(RGBLedColor::VIOLET, _globalState->isDark());
    else if(pm >= (int)PM2_5Level::Unhealthy || gas >= (int)GasLevel::Unhealthy)
        setRGBLedColor(RGBLedColor::RED, _globalState->isDark());
    else if(pm >= (int)PM2_5Level::UnhealthyForSensitiveGroups || gas >= (int)GasLevel::UnhealthyForSensitiveGroups) 
        setRGBLedColor(RGBLedColor::ORANGE, _globalState->isDark());
    else if(pm >= (int)PM2_5Level::Moderate || gas >= (int)GasLevel::Moderate) 
        setRGBLedColor(RGBLedColor::YELLOW, _globalState->isDark());
    else setRGBLedColor(RGBLedColor::GREEN, _globalState->isDark());
}

void LEDIndicatorsController::setWeatherStatus(PresentingOutdoorWeatherData data){  
    setRainLed(data.weatherData.getRainLevel());
}

void LEDIndicatorsController::setRGBLedColor(RGBLedColor color, bool isDimmed){
    _pcf->get()->digitalWrite(AIR_0R5_LED_PIN, HIGH);
    _pcf->get()->digitalWrite(AIR_R_LED_PIN, HIGH);
    _pcf->get()->digitalWrite(AIR_0G3_LED_PIN, HIGH);
    _pcf->get()->digitalWrite(AIR_0G5_LED_PIN, HIGH);
    _pcf->get()->digitalWrite(AIR_G_LED_PIN, HIGH);
    _pcf->get()->digitalWrite(AIR_0B5_LED_PIN, HIGH);
    _pcf->get()->digitalWrite(AIR_B_LED_PIN, HIGH);

    switch (color)
    {
    case RGBLedColor::GREEN:
        if(!isDimmed)_pcf->get()->digitalWrite(AIR_G_LED_PIN, LOW);
        else _pcf->get()->digitalWrite(AIR_0G5_LED_PIN, LOW);
        break;

    case RGBLedColor::YELLOW:
        if(!isDimmed){
            _pcf->get()->digitalWrite(AIR_R_LED_PIN, LOW);
            _pcf->get()->digitalWrite(AIR_G_LED_PIN, LOW);
        }
        else {
            _pcf->get()->digitalWrite(AIR_0R5_LED_PIN, LOW);
            _pcf->get()->digitalWrite(AIR_0G5_LED_PIN, LOW);
        }
        break;

    case RGBLedColor::ORANGE:
        if(!isDimmed){
            _pcf->get()->digitalWrite(AIR_R_LED_PIN, LOW);
            _pcf->get()->digitalWrite(AIR_0G5_LED_PIN, LOW);
        }
        else {
            _pcf->get()->digitalWrite(AIR_0R5_LED_PIN, LOW);
            _pcf->get()->digitalWrite(AIR_0G3_LED_PIN, LOW);
        }
        break;
    case RGBLedColor::RED:
        if(!isDimmed)_pcf->get()->digitalWrite(AIR_R_LED_PIN, LOW);
        else _pcf->get()->digitalWrite(AIR_0R5_LED_PIN, LOW);
        break;
    case RGBLedColor::BLUE:
        if(!isDimmed)_pcf->get()->digitalWrite(AIR_B_LED_PIN, LOW);
        else _pcf->get()->digitalWrite(AIR_0B5_LED_PIN, LOW);
        break;
    case RGBLedColor::VIOLET:
        if(!isDimmed){
            _pcf->get()->digitalWrite(AIR_R_LED_PIN, LOW);
            _pcf->get()->digitalWrite(AIR_B_LED_PIN, LOW);
        }
        else {
            _pcf->get()->digitalWrite(AIR_0R5_LED_PIN, LOW);
            _pcf->get()->digitalWrite(AIR_0B5_LED_PIN, LOW);
        }
        break;
      case RGBLedColor::_WHITE:
        if(!isDimmed){
            _pcf->get()->digitalWrite(AIR_R_LED_PIN, LOW);
            _pcf->get()->digitalWrite(AIR_G_LED_PIN, LOW);
            _pcf->get()->digitalWrite(AIR_B_LED_PIN, LOW);
        }
        else {
            _pcf->get()->digitalWrite(AIR_0R5_LED_PIN, LOW);
            _pcf->get()->digitalWrite(AIR_0G5_LED_PIN, LOW);
            _pcf->get()->digitalWrite(AIR_0B5_LED_PIN, LOW);
        }
        break;
    default:
        break;
    }

}
void LEDIndicatorsController::setRainLed(uint8_t level){
    _pcf->get()->digitalWrite(RAIN1_LED_PIN, LOW);
    _pcf->get()->digitalWrite(RAIN2_LED_PIN, LOW);
    _pcf->get()->digitalWrite(RAIN3_LED_PIN, LOW);

    if(level>2)_pcf->get()->digitalWrite(RAIN3_LED_PIN, HIGH);
    if(level>1)_pcf->get()->digitalWrite(RAIN2_LED_PIN, HIGH);
    if(level>0)_pcf->get()->digitalWrite(RAIN1_LED_PIN, HIGH);
}
void LEDIndicatorsController::setStandbyMode(bool isActive){
    if(isActive)clearAllIndicators();
    _pcf->get()->digitalWrite(STANDBY_LED_PIN, isActive);
}

void LEDIndicatorsController::clearAllIndicators(){
    setRainLed(0);
    setRGBLedColor(RGBLedColor::_BLACK, false);
}
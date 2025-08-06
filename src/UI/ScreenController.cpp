#include "UI/ScreenController.h"
#include "StringUtils.h"
#include "UI/Screens/CommonDrawFunctions.h"
#include "Config.h"
#include <Fonts/FreeSansBold18pt7b.h>
#include <Fonts/FreeSans9pt7b.h>

ScreenController::ScreenController(HardwareRegistry* hardwareRegistry, GlobalState* globalState){
    _globalState = globalState;
    _screenDevice = hardwareRegistry->_OLEDScreen;
    _screen = _screenDevice->get();

    _ledScreenTop = hardwareRegistry->_topLedScreen->get();
    _ledScreenMiddle = hardwareRegistry->_middleLedScreen->get();
    _ledScreenBottom = hardwareRegistry->_bottomLedScreen->get();

    clearScreen();
    clearLedScreens();
}

void ScreenController::clearScreen(){
    _screen->clearDisplay();
    _screen->display();
}

void ScreenController::clearLedScreens(){
    _ledScreenTop->clear();
    _ledScreenMiddle->clear();
    _ledScreenBottom->clear();
}

void ScreenController::showDataScreen(View view, PresentingIndoorWeatherData weatherData, bool isOledActive){
    auto data = weatherData.weatherMonitorHistoricalData.back();

    //Dasha's wish :)
    _ledScreenBottom->printDecimal(data.radiation,2);

    switch(view){
        case View::STANDARD:
            _ledScreenMiddle->clear();
            break;
        case View::DETAILED_OUTDOOR_RAIN:
            break;
        case View::DETAILED_PRESSURE:
            break;
        case View::DETAILED_RADIATION:
            _ledScreenMiddle->clear();
            if(isOledActive)showRadiationGraph(weatherData);
            break;
        case View::DETAILED_PM:
            _ledScreenMiddle->printInt(data.PM_2_5);
            if(isOledActive)showMeasurementUnit("", "PM2.5");
            break;
        case View::DETAILED_VOC:
            _ledScreenMiddle->printInt(data.calculateGasAirPollution());
            if(isOledActive)showMeasurementUnit("Gas Pollution", "% VOC");
            break;
        case View::DETAILED_INDOOR_TEMPERATURE:
            _ledScreenMiddle->printDecimal(data.temperatureCelsium,1);
            if(isOledActive)showMeasurementUnit("Temp inside", "`C");
            break;
        case View::DETAILED_INDOOR_HUMIDITY:
            _ledScreenMiddle->printInt(data.humidity);
            if(isOledActive)showMeasurementUnit("Humidity", "%");
            break;
    }
}

void ScreenController::showDataScreen(View view, PresentingOutdoorWeatherData outdoorWeatherData, bool isOledActive){
    auto data = outdoorWeatherData.weatherData;
    
    _ledScreenTop->printDecimalWithUnit(data.temperature, 'c');

    switch(view){
        case View::STANDARD:
            break;
        case View::DETAILED_PM:
            break;
        case View::DETAILED_RADIATION:
            break;
        case View::DETAILED_VOC:
            break;
        case View::DETAILED_INDOOR_TEMPERATURE:
            break;
        case View::DETAILED_INDOOR_HUMIDITY:
            break;
        case View::DETAILED_OUTDOOR_RAIN:
            _ledScreenMiddle->printInt(data.raindropLevel);
            if(isOledActive)showMeasurementUnit("Rain", "%");
            break;
        case View::DETAILED_PRESSURE:
            _ledScreenMiddle->printInt((int)data.pressure);
            if(isOledActive)showMeasurementUnit("Pressure", "hPa");
            break;
    }
}

void ScreenController::showNetworkStatusIcon(NetworkStatus status){
    _screen->fillRect(0, 0, _iconWidth, _iconHeight, BLACK);

    switch (status)
    {
        case NetworkStatus::DISABLED:
        case NetworkStatus::STANDBY:
            break;
        case NetworkStatus::PROBLEM:
            _screen->drawBitmap(0, 0, wifiError_icon, _iconWidth, _iconHeight, WHITE);
            break;
        case NetworkStatus::ACTIVE: 
            _screen->drawBitmap(0, 0, wifi_icon, _iconWidth, _iconHeight, WHITE);
            break;
    }
    _screen->display();
}

void ScreenController::showRadiationGraph(PresentingIndoorWeatherData weatherData){
    clearScreen();
    double maxValue = 0.0;
    for(unsigned int i = 0; i < weatherData.weatherMonitorHistoricalData.size(); i++){
        if(weatherData.weatherMonitorHistoricalData[i].radiation > maxValue)maxValue = weatherData.weatherMonitorHistoricalData[i].radiation;
    }

    auto x = 0;
    int xStep = 128 / DATA_COLLECTION_CAPACITY;
    for(unsigned int i = 0; i < weatherData.weatherMonitorHistoricalData.size(); i++){
        int y = 44 - (weatherData.weatherMonitorHistoricalData[i].radiation * 44 / maxValue);
        _screen->fillRect(x,20+y,xStep,44-y,WHITE);
        x+=xStep;
    }

    _screen->setTextSize(1);
    _screen->setTextColor(INVERSE);
    _screen->setFont(&FreeSans9pt7b);
    _screen->setCursor(0, 14);
    _screen->print("Radiation");
    _screen->setFont(NULL);
    _screen->setCursor(100, 8);
    _screen->print(maxValue, 2);
    _screen->display();
}

void ScreenController::showInfoScreen(String title, String text){
    clearScreen();
    _screen->setTextSize(1);
    _screen->setFont(&FreeSans9pt7b);
    _screen->setTextColor(WHITE);
    _screen->setCursor(0, 14);
    _screen->print(title);
    _screen->setFont(NULL);
    _screen->setTextSize(1);
    _screen->setCursor(0, 26);
    _screen->print(text);
    _screen->display();
}

void ScreenController::showMeasurementUnit(String measurement, String unit){
    clearScreen();
    _screen->setTextSize(1);
    _screen->setFont(&FreeSans9pt7b);
    _screen->setTextColor(WHITE);
    _screen->setCursor(0, 14);
    _screen->print(measurement);
    _screen->setFont(&FreeSansBold18pt7b);
    _screen->setCursor(0, 54);
    _screen->print(unit);
    _screen->display();
}

void ScreenController::setLedScreensBrightness(int percent){
    if(percent > 90) percent = 100;
    if(percent < 30) percent = 30;
    _ledScreenTop->setBrightness(percent);
    _ledScreenMiddle->setBrightness(percent);
    _ledScreenBottom->setBrightness(percent);
}

void ScreenController::blinkRadiation(){
    _ledScreenMiddle->print("   -");
    delay(100);
    _ledScreenMiddle->clear();
}
#include "UI/Screens/DashboardScreen.h"
#include "StringUtils.h"
#include "config.h"
#include <Adafruit_GFX.h>

DashboardScreen::DashboardScreen(Adafruit_SSD1306* screen){
    _screen = screen;
}

void DashboardScreen::showWeatherData(PresentingWeatherData weatherData){

}

void DashboardScreen::showBackendWeatherData(PresentingBackendWeatherData backendWeatherData){

}
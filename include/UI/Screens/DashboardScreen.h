#ifndef DashboardScreen_H
#define DashboardScreen_H

#include "Models/PresentingIndoorWeatherData.h"
#include "Models/PresentingOutdoorWeatherData.h"
#include <Adafruit_SSD1306.h>

class DashboardScreen
{
public:
    DashboardScreen(Adafruit_SSD1306* screen);

    void showWeatherData(PresentingIndoorWeatherData weatherData);
    void showOutdoorWeatherData(PresentingOutdoorWeatherData outdoorWeatherData);

private:
    Adafruit_SSD1306* _screen;
};

#endif
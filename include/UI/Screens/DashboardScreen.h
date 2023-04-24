#ifndef DashboardScreen_H
#define DashboardScreen_H

#include "Models/PresentingWeatherData.h"
#include "Models/PresentingBackendWeatherData.h"
#include <Adafruit_SSD1306.h>

class DashboardScreen
{
public:
    DashboardScreen(Adafruit_SSD1306* screen);

    void showWeatherData(PresentingWeatherData weatherData);
    void showBackendWeatherData(PresentingBackendWeatherData backendWeatherData);

private:
    Adafruit_SSD1306* _screen;
};

#endif
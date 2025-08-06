#include "HardwareModules/HardwareRegistry.h"
#include "HardwareModules/PCFExtender.h"
#include "Models/PresentingIndoorWeatherData.h"
#include "Models/PresentingOutdoorWeatherData.h"
#include "Models/Enums/View.h"
#include "GlobalObjects/GlobalState.h"
#include <Ticker.h>

enum class RGBLedColor { _BLACK, RED, GREEN, BLUE, YELLOW, ORANGE, VIOLET, _WHITE };

class LEDIndicatorsController
{
public:
    LEDIndicatorsController(HardwareRegistry* hardwareRegistry, GlobalState* globalState);
    void setWeatherStatus(PresentingIndoorWeatherData data);
    void setWeatherStatus(PresentingOutdoorWeatherData data);
    void setStandbyMode(bool isActive);

private:
    Ticker* _timerSlow;
    Ticker* _timerFast;
    bool _fastBlinkingLedOn = false;
    bool _slowBlinkingLedOn = false;

    GlobalState* _globalState;

    PCFExtender* _pcf;

    void setRGBLedColor(RGBLedColor color, bool isDimmed);
    void setRainLed(uint8_t level);
    void clearAllIndicators();
};
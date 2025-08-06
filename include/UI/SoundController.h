#include "HardwareModules/HardwareRegistry.h"
#include "HardwareModules/BuzzerModule.h"
#include "Models/Enums/WarningLevel.h"
#include <Ticker.h>

class SoundController
{
public:
    SoundController(HardwareRegistry* hardwareRegistry);
    void setWarningLevel(WarningLevel level, bool alertBypassed);

    void updateTimer();

    void shortBeep(uint8_t count = 1);
    void longBeep(uint8_t count = 1);
    void radiationBeep();

private:
    BuzzerModule* _buzzerModule;

    WarningLevel _currentWarningLevel;
    Ticker* _timer;
    bool _isAlertBypassed = false;
};
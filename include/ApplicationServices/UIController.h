#include "HardwareModules/HardwareRegistry.h"
#include "GlobalObjects/GlobalState.h"
#include "UI/LEDIndicatorsController.h"
#include "UI/SoundController.h"
#include "UI/ScreenController.h"
#include "UI/InputsController.h"
#include "Models/Enums/View.h"
#include "Models/PresentingWeatherData.h"
#include "Models/PresentingBackendWeatherData.h"
#include "Models/Enums/NetworkStatus.h"
#include <Ticker.h>

class UIController
{
public:
    UIController(HardwareRegistry* HardwareRegistry, GlobalState* globalState);
    void updateUI();
    void onPresentingWeatherDataUpdate(PresentingWeatherData presentingWeatherData);
    void onPresentingBackendWeatherDataUpdate(PresentingBackendWeatherData presentingBackendWeatherData);
    void onNetworkStatusChange(NetworkStatus networkStatus);
    void onBlocking(bool isBlocked);
    void registerRadiationHit();

private:
    void updateInputs();

    void changeStandby(bool standby);
    void toggleSoundEnabling();
    void changeView();
    void changeView(View view);

    void presentWeatherData();
    void presentBackendWeatherData();

    void updateScreensBrightness();
    void showRadiationHit();

    GlobalState* _globalState;

    ScreenController* _screen;
    LEDIndicatorsController* _ledIndicators;
    SoundController* _soundController;
    InputsController* _inputsController;

    LightSensor* _lightSensor;

    Ticker* _oledIdleTimer;
    Ticker* _lightCheckTimer;

    PresentingWeatherData _currentPresentingWeatherData;
    PresentingBackendWeatherData _currentPresentingBackendWeatherData;

    View _currentView = View::STANDARD;
    bool _isSoundEnabled = true;
    bool _isStandby = false;
    bool _isWarning = false;
    bool _isAlert = false;
    bool _isAlertBypassed = false;
    bool _isOledActive = true;

    volatile bool _radiationHit = false;
};
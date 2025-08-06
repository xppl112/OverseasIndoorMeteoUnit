#include "ApplicationServices/UIController.h"
#include "config.h"

//#define DEBUG

UIController::UIController(HardwareRegistry* HardwareRegistry, GlobalState* globalState){   
    _globalState = globalState;
    _screen = new ScreenController(HardwareRegistry, globalState);
    _ledIndicators = new LEDIndicatorsController(HardwareRegistry, globalState);
    _soundController = new SoundController(HardwareRegistry);
    _inputsController = new InputsController(HardwareRegistry);

    _lightSensor = HardwareRegistry->_lightSensor;

    _oledIdleTimer = new Ticker(OLED_IDLE_TIMEOUT * 1000);
    _lightCheckTimer = new Ticker(LIGHT_CHECK_TIMEOUT * 1000);
    _lightCheckTimer->start();

    //Draw splash data
    onPresentingIndoorWeatherDataUpdate(_currentPresentingIndoorWeatherData);
    onPresentingOutdoorWeatherDataUpdate(_currentPresentingOutdoorWeatherData);
    onNetworkStatusChange(NetworkStatus::DISABLED);
    _screen->showInfoScreen("Starting", "Performing\nmeasurements...");
}

void UIController::updateUI() {
    updateInputs();
    _soundController->updateTimer();
    
    _oledIdleTimer->update();
    if(_oledIdleTimer->state() == FIRED){
        _isOledActive = false;
    }

    _lightCheckTimer->update();
    if(_lightCheckTimer->state() == FIRED){
        if(!_isWarning){
            _globalState->lightLevelPercent = _lightSensor->getData().lightLevelInPercent;
            updateScreensBrightness();
        }
        _lightCheckTimer->start();
    }

    if(_radiationHit){
        showRadiationHit();
        _radiationHit = false;
    }
}

void UIController::updateInputs() {
    ButtonPressed buttonPressed = _inputsController->updateInputs();
    //auto sig = _inputsController->getSignalCode();
    //if(sig > 0)_screen->showInfoScreen("IR", String(sig));

    switch(buttonPressed){
        case ButtonPressed::STANDBY_ON:
            changeStandby(true);
            break;
        case ButtonPressed::STANDBY_OFF:
            changeStandby(false);
            break;
        case ButtonPressed::DISPLAY_RESET:
            changeView(View::STANDARD);
            break;
        case ButtonPressed::CHANGE_SCREEN:
            changeView();
            break;
        case ButtonPressed::SOUND_TOGGLE:
            toggleSoundEnabling();
            break;
        case ButtonPressed::NONE:break;
    }
}

void UIController::onPresentingIndoorWeatherDataUpdate(PresentingIndoorWeatherData presentingIndoorWeatherData){
    #ifdef DEBUG
    Serial.print("onPresentingIndoorWeatherDataUpdate. Data count: ");Serial.println(presentingIndoorWeatherData.weatherMonitorHistoricalData.size());
    #endif
    _currentPresentingIndoorWeatherData = presentingIndoorWeatherData;
    if(presentingIndoorWeatherData.weatherMonitorHistoricalData.size() == 0)return;
    
    auto warningLevel = _currentPresentingIndoorWeatherData.GetOverallWarningLevel();

    if(warningLevel > WarningLevel::LOW_WARNING_LEVEL){
        //Warning appear
        _isWarning = true;
        _isOledActive = true;
        _oledIdleTimer->start();
        if(_currentPresentingIndoorWeatherData.RadiationWarningLevel > WarningLevel::LOW_WARNING_LEVEL)_currentView = View::DETAILED_RADIATION;
        else if(_currentPresentingIndoorWeatherData.PMWarningLevel > WarningLevel::LOW_WARNING_LEVEL)_currentView = View::DETAILED_PM;
        else if(_currentPresentingIndoorWeatherData.VOCWarningLevel > WarningLevel::LOW_WARNING_LEVEL)_currentView = View::DETAILED_VOC;
        
        _globalState->lightLevelPercent = 100;
        updateScreensBrightness();
    }
    else if(_isWarning){
        //Warning is gone
        _isWarning = false;
        _currentView = View::STANDARD;
    }

    if(warningLevel == WarningLevel::CRITICAL){
        _isSoundEnabled = true;
        _isAlert = true;
        changeStandby(false);
    }
    else {_isAlert = false;_isAlertBypassed = false;}

    if(_isSoundEnabled)_soundController->setWarningLevel(warningLevel, _isAlertBypassed);
    if(!_isStandby){
        presentWeatherData();
    }
}

void UIController::onPresentingOutdoorWeatherDataUpdate(PresentingOutdoorWeatherData presentingOutdoorWeatherData){
    #ifdef DEBUG
    Serial.print("onPresentingOutdoorWeatherDataUpdate.");
    #endif

   _currentPresentingOutdoorWeatherData = presentingOutdoorWeatherData;
    if(presentingOutdoorWeatherData.weatherData.temperature == -100.0f)return;
    
    if(!_isStandby){
        presentOutdoorWeatherData();
    }
}

void UIController::presentWeatherData(){
    _ledIndicators->setWeatherStatus(_currentPresentingIndoorWeatherData);
    _screen->clearScreen();
    _screen->showDataScreen(_currentView, _currentPresentingIndoorWeatherData, _isOledActive);
}

void UIController::presentOutdoorWeatherData(){
    _ledIndicators->setWeatherStatus(_currentPresentingOutdoorWeatherData);
    _screen->showDataScreen(_currentView, _currentPresentingOutdoorWeatherData, _isOledActive);
}

void UIController::onNetworkStatusChange(NetworkStatus networkStatus){
    if(!_isStandby){
        _screen->showNetworkStatusIcon(networkStatus);
        if(networkStatus == NetworkStatus::STANDBY)presentWeatherData();
    }
}

void UIController::changeStandby(bool standby){
    _isStandby = standby;
    _ledIndicators->setStandbyMode(standby);

    if(_isStandby) {
        _screen->clearLedScreens();
        _screen->clearScreen();
    }
    else {
        presentWeatherData();
        presentOutdoorWeatherData();
    }
}

void UIController::toggleSoundEnabling(){
    if(_isAlert && !_isAlertBypassed){
        _isAlertBypassed = true;
        _soundController->setWarningLevel(_currentPresentingIndoorWeatherData.GetOverallWarningLevel(), _isAlertBypassed);
    }
    else {
        _isSoundEnabled = !_isSoundEnabled;
    }
    if(_isSoundEnabled){
        _soundController->shortBeep();
    }
}

void UIController::changeView(View view){
    _currentView = view;

    _isOledActive = true;
    _oledIdleTimer->start();
    
    presentWeatherData();
    presentOutdoorWeatherData();
}

void UIController::changeView(){
    int viewValue = (int)_currentView;
    viewValue++;
    if(viewValue > (int)View::DETAILED_PRESSURE) viewValue = (int)View::STANDARD;
    changeView(static_cast<View>(viewValue));
}

void UIController::updateScreensBrightness(){
    if(!_isStandby){
        _screen->setLedScreensBrightness(_globalState->lightLevelPercent);
        _ledIndicators->setWeatherStatus(_currentPresentingIndoorWeatherData);
    }
}

void UIController::registerRadiationHit(){
    _radiationHit = true;
}

void UIController::showRadiationHit(){
    if(_currentView == View::DETAILED_RADIATION){
        _screen->blinkRadiation();
        if((!_isAlert || _isAlertBypassed) && _isSoundEnabled)_soundController->radiationBeep();
    }
}
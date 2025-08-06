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
    onPresentingWeatherDataUpdate(_currentPresentingWeatherData);
    onPresentingBackendWeatherDataUpdate(_currentPresentingBackendWeatherData);
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

void UIController::onPresentingWeatherDataUpdate(PresentingWeatherData presentingWeatherData){
    #ifdef DEBUG
    Serial.print("onPresentingWeatherDataUpdate. Data count: ");Serial.println(presentingWeatherData.weatherMonitorHistoricalData.size());
    #endif
    _currentPresentingWeatherData = presentingWeatherData;
    if(presentingWeatherData.weatherMonitorHistoricalData.size() == 0)return;
    
    auto warningLevel = _currentPresentingWeatherData.GetOverallWarningLevel();

    if(warningLevel > WarningLevel::LOW_WARNING_LEVEL){
        //Warning appear
        _isWarning = true;
        _isOledActive = true;
        _oledIdleTimer->start();
        if(_currentPresentingWeatherData.RadiationWarningLevel > WarningLevel::LOW_WARNING_LEVEL)_currentView = View::DETAILED_RADIATION;
        else if(_currentPresentingWeatherData.PMWarningLevel > WarningLevel::LOW_WARNING_LEVEL)_currentView = View::DETAILED_PM;
        else if(_currentPresentingWeatherData.VOCWarningLevel > WarningLevel::LOW_WARNING_LEVEL)_currentView = View::DETAILED_VOC;
        
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

void UIController::onPresentingBackendWeatherDataUpdate(PresentingBackendWeatherData presentingBackendWeatherData){
    #ifdef DEBUG
    Serial.print("onPresentingBackendWeatherDataUpdate. Data count: ");Serial.println(presentingBackendWeatherData.backendWeatherHistoricalData.size());
    #endif

   _currentPresentingBackendWeatherData = presentingBackendWeatherData;
    if(presentingBackendWeatherData.backendWeatherHistoricalData.size() == 0)return;
    
    if(!_isStandby){
        presentBackendWeatherData();
    }
}

void UIController::presentWeatherData(){
    _ledIndicators->setWeatherStatus(_currentPresentingWeatherData);
    _screen->clearScreen();
    _screen->showDataScreen(_currentView, _currentPresentingWeatherData, _isOledActive);
}

void UIController::presentBackendWeatherData(){
    _ledIndicators->setWeatherStatus(_currentPresentingBackendWeatherData);
    _screen->showDataScreen(_currentView, _currentPresentingBackendWeatherData, _isOledActive);
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
        presentBackendWeatherData();
    }
}

void UIController::toggleSoundEnabling(){
    if(_isAlert && !_isAlertBypassed){
        _isAlertBypassed = true;
        _soundController->setWarningLevel(_currentPresentingWeatherData.GetOverallWarningLevel(), _isAlertBypassed);
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
    presentBackendWeatherData();
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
        _ledIndicators->setWeatherStatus(_currentPresentingWeatherData);
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
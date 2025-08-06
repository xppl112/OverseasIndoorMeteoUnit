#include <config.h>
#include "GlobalObjects/GlobalState.h"
#include "ApplicationServices/WeatherMonitor.h"
#include "ApplicationServices/NetworkController.h"
#include "ApplicationServices/UIController.h"

GlobalState* globalState;
HardwareRegistry* hardwareRegistry;
WeatherMonitor* weatherMonitor;
NetworkController* networkController;
UIController* uiController;

void onPresentingIndoorWeatherUpdateEventHandler(PresentingIndoorWeatherData PresentingIndoorWeatherData);
void onPresentingOutdoorWeatherUpdateEventHandler(PresentingOutdoorWeatherData PresentingOutdoorWeatherData);
void onNetworkStatusChangeEventHandler(NetworkStatus networkStatus);

IRAM_ATTR void radiationSensorInterruptCallback() {
    hardwareRegistry->_radiationSensor->onRadiationSignalReceived();
    uiController->registerRadiationHit();
}

void setup() {
    Serial.begin(9600);
    delay(500);

    hardwareRegistry = new HardwareRegistry();
    hardwareRegistry->reconnectAllDisconnectedDevices();
    hardwareRegistry->_radiationSensor->registerSignalReceivedCallback(radiationSensorInterruptCallback);

    globalState = new GlobalState();
    uiController = new UIController(hardwareRegistry, globalState);
    weatherMonitor = new WeatherMonitor(hardwareRegistry, globalState);
    networkController = new NetworkController(globalState);

    weatherMonitor->addWeatherUpdatedEventHandler(onPresentingIndoorWeatherUpdateEventHandler);
    weatherMonitor->run();

    networkController->addOutdoorWeatherUpdatedEventHandler(onPresentingOutdoorWeatherUpdateEventHandler);
    networkController->addNetworkStatusChangedEventHandler(onNetworkStatusChangeEventHandler);
    networkController->startListeningToOutdoorUnits();
}

void loop()
{
    uiController->updateUI();
    weatherMonitor->updateTimers();
    networkController->loop();
}

void onPresentingIndoorWeatherUpdateEventHandler(PresentingIndoorWeatherData PresentingIndoorWeatherData){
    static int updateCounter = 0;
    uiController->onPresentingIndoorWeatherDataUpdate(PresentingIndoorWeatherData);

    updateCounter++;
    if (updateCounter >= 3) {
        networkController->sendWeatherData(PresentingIndoorWeatherData.weatherMonitorHistoricalData.back());
        updateCounter = 0;
    }
}

void onPresentingOutdoorWeatherUpdateEventHandler(PresentingOutdoorWeatherData PresentingOutdoorWeatherData){
    uiController->onPresentingOutdoorWeatherDataUpdate(PresentingOutdoorWeatherData);
    networkController->queueSendingWeatherData(PresentingOutdoorWeatherData.weatherData);
}

void onNetworkStatusChangeEventHandler(NetworkStatus networkStatus){
    uiController->onNetworkStatusChange(networkStatus);
}
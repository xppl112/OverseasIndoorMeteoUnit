#include <config.h>
#include "GlobalObjects/GlobalState.h"
#include "ApplicationServices/WeatherMonitor.h"
#include "ApplicationServices/BackendIntegrator.h"
#include "ApplicationServices/UIController.h"

GlobalState* globalState;
HardwareRegistry* hardwareRegistry;
WeatherMonitor* weatherMonitor;
BackendIntegrator* backendIntegrator;
UIController* uiController;

void onPresentingWeatherDataUpdateEventHandler(PresentingWeatherData PresentingWeatherData);
void onPresentingBackendWeatherDataUpdateEventHandler(PresentingBackendWeatherData PresentingBackendWeatherData);
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
    backendIntegrator = new BackendIntegrator(globalState);

    weatherMonitor->addUpdatedEventHandler(onPresentingWeatherDataUpdateEventHandler);
    weatherMonitor->run();

    backendIntegrator->addUpdatedEventHandler(onPresentingBackendWeatherDataUpdateEventHandler);
    backendIntegrator->addNetworkStatusChangedEventHandler(onNetworkStatusChangeEventHandler);
}

void loop()
{
    //hardwareRegistry->healthCheck();
    uiController->updateUI();
    weatherMonitor->updateTimers();
}

void onPresentingWeatherDataUpdateEventHandler(PresentingWeatherData PresentingWeatherData){
    uiController->onPresentingWeatherDataUpdate(PresentingWeatherData);
    backendIntegrator->sendWeatherData(PresentingWeatherData.weatherMonitorHistoricalData.back());
}

void onPresentingBackendWeatherDataUpdateEventHandler(PresentingBackendWeatherData PresentingBackendWeatherData){
    uiController->onPresentingBackendWeatherDataUpdate(PresentingBackendWeatherData);
}

void onNetworkStatusChangeEventHandler(NetworkStatus networkStatus){
    uiController->onNetworkStatusChange(networkStatus);
}
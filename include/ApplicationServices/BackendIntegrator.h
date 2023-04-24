#include "HardwareModules/HardwareRegistry.h"
#include "GlobalObjects/GlobalState.h"
#include "Models/BackendWeatherData.h"
#include "Models/PresentingBackendWeatherData.h"
#include "Models/WeatherMonitorData.h"
#include "Models/Enums/NetworkStatus.h"
#include <Ticker.h>
#include <vector>
#include "EspWifiClient.h"
#include "config.h"

typedef void (*BackendWeatherUpdatedEventCallback)(PresentingBackendWeatherData);
typedef void (*BackendNetworkStatusChangedEventCallback)(NetworkStatus);

class BackendIntegrator
{
public:
    BackendIntegrator(GlobalState* globalState);
    //void run();
    //void stop();
    //void updateTimers();
    void addUpdatedEventHandler(BackendWeatherUpdatedEventCallback callback);
    void addNetworkStatusChangedEventHandler(BackendNetworkStatusChangedEventCallback callback);
    void sendWeatherData(WeatherMonitorData data);
    
    bool IsConnected = false;

private:
    bool connectWifi();
    void disconnectWifi();

    GlobalState* _globalState;

    BackendWeatherUpdatedEventCallback _onUpdateCallback;
    BackendNetworkStatusChangedEventCallback _onNetworkStatusChangeCallback;
    //Ticker* _timer;
    std::vector<BackendWeatherData> _backendWeatherHistoricalData;

    EspWifiClient* _esp;
    BackendClientConfig _config;
};

#include "HardwareModules/HardwareRegistry.h"
#include "GlobalObjects/GlobalState.h"
#include "Models/OutdoorWeatherData.h"
#include "Models/PresentingOutdoorWeatherData.h"
#include "Models/IndoorWeatherData.h"
#include "Models/OutdoorUnitResponse.h"
#include "Models/Enums/NetworkStatus.h"
#include <Ticker.h>
#include <vector>
#include <functional>
#include "EspWifiClient.h"
#include "config.h"
#include <ESP8266WebServer.h>

typedef void (*OutdoorWeatherUpdatedEventCallback)(PresentingOutdoorWeatherData);
typedef void (*BackendNetworkStatusChangedEventCallback)(NetworkStatus);

class NetworkController
{
public:
    NetworkController(GlobalState* globalState);
    void addOutdoorWeatherUpdatedEventHandler(OutdoorWeatherUpdatedEventCallback callback);
    void addNetworkStatusChangedEventHandler(BackendNetworkStatusChangedEventCallback callback);
    void startListeningToOutdoorUnits();

    void queueSendingWeatherData(OutdoorWeatherData data);
    void sendWeatherData(IndoorWeatherData data);
    void sendWeatherData(OutdoorWeatherData data);
    void loop(); // To handle server requests

    bool IsConnected = false;

private:
    bool connectWifi();
    void disconnectWifi();
    void startAccessPoint();
    bool switchToStation();
    void handleOutdoorUnitData();

    GlobalState* _globalState;

    OutdoorWeatherUpdatedEventCallback _onOutdoorDataReceivedCallback;
    BackendNetworkStatusChangedEventCallback _onNetworkStatusChangeCallback;

    std::vector<OutdoorWeatherData> _outdoorWeatherHistoricalData;

    EspWifiClient* _esp;
    BackendClientConfig _config;
    ESP8266WebServer server{80};
};

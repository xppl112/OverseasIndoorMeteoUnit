#include "ApplicationServices/BackendIntegrator.h"
#include <ArduinoJson.h>

//#define DEBUG

BackendIntegrator::BackendIntegrator(GlobalState* globalState){   
    _globalState = globalState;
    _esp = new EspWifiClient();
    //_timer = new Ticker(globalState->configuration.NetworkDataUpdateIntervalInSeconds * 1000);
}
/*
void BackendIntegrator::run(){
    _timer->start(true);
}

void BackendIntegrator::stop(){
    _timer->stop();
    disconnectWifi();
}

void BackendIntegrator::updateTimers(){
    _timer->update();
    if(_timer->state() == FIRED){
        requestWeatherData();
        _timer->start();
    }
}
*/
void BackendIntegrator::addUpdatedEventHandler(BackendWeatherUpdatedEventCallback callback){
    _onUpdateCallback = callback;
}

void BackendIntegrator::addNetworkStatusChangedEventHandler(BackendNetworkStatusChangedEventCallback callback)
{
    _onNetworkStatusChangeCallback = callback;
}

void BackendIntegrator::sendWeatherData(WeatherMonitorData data){
    #ifdef DEBUG
    Serial.println("sendWeatherData execution");
    #endif

    if(!connectWifi())return;
    _onNetworkStatusChangeCallback(NetworkStatus::ACTIVE);

    #ifdef DEBUG
    Serial.println("sendWeatherData: wifi connected");
    #endif

    HttpResponse response = _esp->sendPostJsonRequest(
        _config.ServerHost, 
        String(_config.ServerApiPostWeatherDataUrl),
        data.toJson(),
        _config.HttpRequestTimeout);

    #ifdef DEBUG
    Serial.print("sendWeatherData: response code: ");Serial.println(response.statusCode);
    Serial.print("sendWeatherData: payload: ");Serial.println(response.payload);
    #endif

    if(response.success && response.statusCode == 200){
        _onNetworkStatusChangeCallback(NetworkStatus::STANDBY);

        DynamicJsonDocument jsonDoc(3000);
        deserializeJson(jsonDoc, response.payload);
        //unsigned long currentTimestamp = jsonDoc["cts"];
        _backendWeatherHistoricalData.clear();
        BackendWeatherData wd;
        /*
        JsonArray dataArray = jsonDoc["data"].as<JsonArray>();
        for (JsonObject dataItem : dataArray) {
            long ts = ((millis()/1000) - (long)dataItem["ts"]) * 1000;
            if(ts<0)wd.timestamp = 0;
            else wd.timestamp = ts;
            wd.raindropLevel = dataItem["rd"];
            wd.windLevel = dataItem["w"];
            wd.temperatureCelsium = dataItem["t"];
            wd.pressureInHPascals = 1000.0+(float)dataItem["p"];

            _backendWeatherHistoricalData.push_back(wd);
        }
        */
        wd.raindropLevel = jsonDoc["rd"];
        wd.windLevel = jsonDoc["w"];
        wd.temperatureCelsium = jsonDoc["t"];
        wd.pressureInHPascals = 1000.0+(float)jsonDoc["p"];
        _backendWeatherHistoricalData.push_back(wd);

        #ifdef DEBUG
        Serial.print("sendWeatherData: items count: ");Serial.println(_backendWeatherHistoricalData.size());
        #endif

        if(_onUpdateCallback != NULL){
            PresentingBackendWeatherData presentingBackendWeatherData {
                .backendWeatherHistoricalData = _backendWeatherHistoricalData
            };

            if(wd.windLevel > 50) presentingBackendWeatherData.WindWarningLevel = WarningLevel::WARNING;
            else if(wd.windLevel != -1)presentingBackendWeatherData.WindWarningLevel = WarningLevel::LOW_WARNING_LEVEL;

            if(wd.raindropLevel > 60) presentingBackendWeatherData.RainWarningLevel = WarningLevel::WARNING;
            else if(wd.windLevel != -1)presentingBackendWeatherData.RainWarningLevel = WarningLevel::LOW_WARNING_LEVEL;

            _onUpdateCallback(presentingBackendWeatherData);
        }
    }
    else _onNetworkStatusChangeCallback(NetworkStatus::PROBLEM);
    
    disconnectWifi();
}

bool BackendIntegrator::connectWifi(){
    if(!IsConnected || !_esp->isWifiConnected){
        _onNetworkStatusChangeCallback(NetworkStatus::ACTIVE);

        IsConnected = _esp->connectWifi(_config.WifiSSID, _config.WifiPassword, _config.WifiConnectionTimeout);

        if(!IsConnected) _onNetworkStatusChangeCallback(NetworkStatus::PROBLEM);
        //else _onNetworkStatusChangeCallback(NetworkStatus::STANDBY);
    }

    return IsConnected;
}
void BackendIntegrator::disconnectWifi(){
    _esp->disconnectWifi();
    //_onNetworkStatusChangeCallback(NetworkStatus::DISABLED);
}
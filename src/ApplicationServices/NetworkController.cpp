#include "ApplicationServices/NetworkController.h"
#include <ESP8266WebServer.h>
#include <ArduinoJson.h>
#include <vector>

//#define DEBUG

// Buffer for unsent data
std::vector<IndoorWeatherData> _unsentIndoorDataBuffer;
std::vector<OutdoorWeatherData> _unsentOutdoorDataBuffer;

NetworkController::NetworkController(GlobalState* globalState){   
    _globalState = globalState;
    _esp = new EspWifiClient();
}

void NetworkController::addOutdoorWeatherUpdatedEventHandler(OutdoorWeatherUpdatedEventCallback callback){
    _onOutdoorDataReceivedCallback = callback;
}

void NetworkController::addNetworkStatusChangedEventHandler(BackendNetworkStatusChangedEventCallback callback)
{
    _onNetworkStatusChangeCallback = callback;
}

void NetworkController::startListeningToOutdoorUnits() {
    startAccessPoint();
}

// Start as AP to receive data from clients
void NetworkController::startAccessPoint() {
    _esp->startAccessPoint(_config.AP_SSID, _config.AP_Password, true, _config.AP_IP, _config.AP_GATEWAY, _config.AP_SUBNET);

    // Register HTTP handler
    server.on("/outdoor-data", HTTP_POST, [this]() { this->handleOutdoorUnitData(); });
    server.begin();

    _onNetworkStatusChangeCallback(NetworkStatus::AP_ACTIVE);
}

void NetworkController::loop() {
    server.handleClient();
    if (_unsentOutdoorDataBuffer.size() > 0) {
        sendWeatherData(_unsentOutdoorDataBuffer.back());
        _unsentOutdoorDataBuffer.pop_back();
    }
}

void NetworkController::handleOutdoorUnitData() {
    if (server.method() == HTTP_POST) {
        String body = server.arg("plain");
        OutdoorWeatherData data;
        bool ok = data.fromJson(body);

        if (ok) {
            // Create response model
            OutdoorUnitResponse response;
            response.sleepDurationSeconds = 1200;
            server.send(200, "application/json", response.toJson());
            delay(200);

            // Call the callback if set
            if (_onOutdoorDataReceivedCallback) {
                auto presentingOutdoorWeatherData = PresentingOutdoorWeatherData();
                presentingOutdoorWeatherData.weatherData = data;
                _onOutdoorDataReceivedCallback(presentingOutdoorWeatherData);
            }
        } else {
            server.send(400, "text/plain", "Invalid JSON");
        }
    } else {
        server.send(405, "text/plain", "Method Not Allowed");
    }
}

// Stop AP and connect to WiFi for upload
bool NetworkController::switchToStation() {
    _esp->stopAccessPoint();
    return connectWifi();
}

void NetworkController::sendWeatherData(IndoorWeatherData data){
    #ifdef DEBUG
    Serial.println("sendWeatherData execution");
    #endif

    if(!switchToStation()) {
        startAccessPoint();
        return;
    }
    _onNetworkStatusChangeCallback(NetworkStatus::ACTIVE);

    #ifdef DEBUG
    Serial.println("sendWeatherData: wifi connected");
    #endif

    HttpResponse response = _esp->sendPostJsonRequest(
        _config.ServerHost, 
        String(_config.ServerApiPostIndoorWeatherDataUrl),
        data.toJson(),
        _config.HttpRequestTimeout);

    #ifdef DEBUG
    Serial.print("sendWeatherData: response code: ");Serial.println(response.statusCode);
    Serial.print("sendWeatherData: payload: ");Serial.println(response.payload);
    #endif

    if(response.success && response.statusCode == 200){
        _onNetworkStatusChangeCallback(NetworkStatus::STANDBY);
    }
    else _onNetworkStatusChangeCallback(NetworkStatus::PROBLEM);
    
    disconnectWifi();
    startAccessPoint();
}

void NetworkController::queueSendingWeatherData(OutdoorWeatherData data){
    _unsentOutdoorDataBuffer.push_back(data);
}

void NetworkController::sendWeatherData(OutdoorWeatherData data){
    #ifdef DEBUG
    Serial.println("sendWeatherData execution");
    #endif

    if(!switchToStation()) {
        startAccessPoint();
        return;
    }
    _onNetworkStatusChangeCallback(NetworkStatus::ACTIVE);

    #ifdef DEBUG
    Serial.println("sendWeatherData: wifi connected");
    #endif

    HttpResponse response = _esp->sendPostJsonRequest(
        _config.ServerHost, 
        String(_config.ServerApiPostOutdoorWeatherDataUrl),
        data.toJson(),
        _config.HttpRequestTimeout);

    #ifdef DEBUG
    Serial.print("sendWeatherData: response code: ");Serial.println(response.statusCode);
    Serial.print("sendWeatherData: payload: ");Serial.println(response.payload);
    #endif

    if(response.success && response.statusCode == 200){
        _onNetworkStatusChangeCallback(NetworkStatus::STANDBY);
    }
    else _onNetworkStatusChangeCallback(NetworkStatus::PROBLEM);
    
    disconnectWifi();
    startAccessPoint();
}

bool NetworkController::connectWifi(){
    if(!IsConnected || !_esp->isWifiConnected){
        _onNetworkStatusChangeCallback(NetworkStatus::ACTIVE);

        IsConnected = _esp->connectWifi(_config.WifiSSID, _config.WifiPassword, _config.WifiConnectionTimeout);

        if(!IsConnected) _onNetworkStatusChangeCallback(NetworkStatus::PROBLEM);
    }

    return IsConnected;
}
void NetworkController::disconnectWifi(){
    _esp->disconnectWifi();
}
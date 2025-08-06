#include "HardwareModules/WiFiModule.h"

WiFiModule::WiFiModule()
{
}

void WiFiModule::connect(){
    _isConnected = true;
}

void WiFiModule::reset(){
}

short WiFiModule::GetWifiSignalLevelPercent(){
    long rssi = WiFi.RSSI();
    short strength = map(rssi, MIN_STRENGTH, MAX_STRENGTH, 0, 11);

    return strength;
}

void WiFiModule::sleep(bool enable){
    if(enable) WiFi.forceSleepBegin();
    else WiFi.forceSleepWake();
}
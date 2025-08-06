#pragma once

#include <ArduinoJson.h>

struct OutdoorWeatherData {
    unsigned long timestamp;

    float temperature = -100.0f;
    unsigned int humidity;
    float temperatureBMP;
    float pressure;
    
    unsigned int raindropLevel;
    float uvLevel;
    unsigned int lightLevel;
    unsigned int windLevel;

    float batteryVoltage;
    float batteryCurrentIdle;
    float batteryCurrentWifiOn;

    int wifiSignalLevel;
    int wifiErrorsCount;

    uint8_t getRainLevel(){
        if(raindropLevel<7)return 0;
        if(raindropLevel<13)return 1;
        if(raindropLevel<40)return 2;
        return 3;
    }

    String toJson(){
        DynamicJsonDocument doc(2048);

        doc["ts"] = timestamp;

        doc["t"] = temperature;
        doc["h"] = humidity;
        doc["tb"] = temperatureBMP;
        doc["p"] = pressure;

        doc["rd"] = raindropLevel;
        doc["uv"] = uvLevel;
        doc["ll"] = lightLevel;
        doc["w"] = windLevel;

        doc["bv"] = batteryVoltage;
        doc["bci"] = batteryCurrentIdle;
        doc["bcw"] = batteryCurrentWifiOn;

        doc["wsl"] = wifiSignalLevel;
        doc["we"] = wifiErrorsCount;

        String output;
        serializeJson(doc, output);
        doc.clear();
        doc.garbageCollect();

        return output;
    }

    bool fromJson(const String& json) {
        DynamicJsonDocument doc(2048);
        DeserializationError error = deserializeJson(doc, json);
        if (error) return false;

        timestamp = doc["ts"] | 0UL;

        temperature = doc["t"] | 0.0f;
        humidity = doc["h"] | 0U;
        temperatureBMP = doc["tb"] | 0.0f;
        pressure = doc["p"] | 0.0f;

        raindropLevel = doc["rd"] | 0U;
        uvLevel = doc["uv"] | 0.0f;
        lightLevel = doc["ll"] | 0U;
        windLevel = doc["w"] | 0U;

        batteryVoltage = doc["bv"] | 0.0f;
        batteryCurrentIdle = doc["bci"] | 0.0f;
        batteryCurrentWifiOn = doc["bcw"] | 0.0f;

        wifiSignalLevel = doc["wsl"] | 0;
        wifiErrorsCount = doc["we"] | 0;

        return true;
    }
};
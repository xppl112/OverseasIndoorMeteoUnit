#pragma once

#include "ArduinoJson.h"

enum class PM2_5Level {
    Good = 0,
    Moderate = 18,
    UnhealthyForSensitiveGroups = 56,
    Unhealthy = 100,
    VeryUnhealthy = 150,
    Hazardous = 400
};

enum class GasLevel {
    Good = 0,
    Moderate = 60,
    UnhealthyForSensitiveGroups = 75,
    Unhealthy = 80,
    VeryUnhealthy = 85,
    Hazardous = 90
};

struct IndoorWeatherData
{
public:
    unsigned long timestamp;

    double radiation = -1;

    int PM_1_0 = -1;
    int PM_2_5 = -1;
    int PM_10_0 = -1;

    float temperatureCelsium = 0;
    int humidity = 0;
    float temperatureBMECelsium = 0;
    int humidityAHT = 0;
    float pressureInHPascals = 0;
    float gasResistance = -1;

    // AQI (US)
    int calculateAQI(){
        int AQI = 0;

        if(PM_2_5 >= 0 && PM_2_5 < 12) AQI = PM_2_5 * 50 / 12;
        else if(PM_2_5 >= 12 && PM_2_5 < 35) AQI = PM_2_5 * 100 / 35;
        else if(PM_2_5 >= 35 && PM_2_5 < 55) AQI = PM_2_5 * 150 / 55;
        else if(PM_2_5 >= 55 && PM_2_5 < 150) AQI = PM_2_5 * 200 / 150;
        else if(PM_2_5 >= 150 && PM_2_5 < 250) AQI = PM_2_5 * 300 / 250;
        else if(PM_2_5 >= 250 && PM_2_5 < 350) AQI = PM_2_5 * 400 / 350;
        else if(PM_2_5 >= 350) AQI = PM_2_5 * 500 / 500;
    
        return AQI;
    }

    float calculateGasAirPollution(){
        if(gasResistance <= 0)return 0;

        float _gasLowerLimit = 5.0;  // Worst air quality limit
        float _gasUpperLimit = 35.0; // Best air quality limit
        auto gasPollution = 100 - (gasResistance - _gasLowerLimit) * 100.0 / (_gasUpperLimit - _gasLowerLimit);
        if (gasPollution > 100) gasPollution = 100; // Sometimes gas readings can go outside of expected scale maximum
        if (gasPollution < 0) gasPollution = 0;  // Sometimes gas readings can go outside of expected scale minimum
        return gasPollution;
    }

    String toJson(){
        DynamicJsonDocument doc(2048);

        doc["ts"] = timestamp;

        doc["t"] = temperatureCelsium;
        doc["h"] = humidity;
        doc["t2"] = temperatureBMECelsium;
        doc["h2"] = humidityAHT;
        doc["p"] = pressureInHPascals;
        doc["gr"] = gasResistance;

        doc["r"] = radiation;

        doc["pm1"] = PM_1_0;
        doc["pm25"] = PM_2_5;
        doc["pm10"] = PM_10_0;

        String output;
        serializeJson(doc, output);
        doc.clear();
        doc.garbageCollect();

        return output;
    }
};
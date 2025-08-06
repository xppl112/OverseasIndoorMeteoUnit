#pragma once
#include "Models/OutdoorWeatherData.h"
#include "Models/IndoorWeatherData.h"

struct MonitorsData
{
public:
    unsigned long timestamp;
    OutdoorWeatherData outdoorWeatherData;
    IndoorWeatherData indoorWeatherData;

    String toJson(){
        DynamicJsonDocument doc(2048);

        doc["ts"] = timestamp;

        doc["t"] = indoorWeatherData.temperatureCelsium;
        doc["h"] = indoorWeatherData.humidity;
        doc["t2"] = indoorWeatherData.temperatureBMECelsium;
        doc["h2"] = indoorWeatherData.humidityAHT;
        doc["p"] = indoorWeatherData.pressureInHPascals;
        doc["gr"] = indoorWeatherData.gasResistance;

        doc["r"] = indoorWeatherData.radiation;

        doc["pm1"] = indoorWeatherData.PM_1_0;
        doc["pm25"] = indoorWeatherData.PM_2_5;
        doc["pm10"] = indoorWeatherData.PM_10_0;

        String output;
        serializeJson(doc, output);
        doc.clear();
        doc.garbageCollect();

        return output;
    }
};
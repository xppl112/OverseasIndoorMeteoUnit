#pragma once

#include <ArduinoJson.h>

struct OutdoorUnitResponse {
    bool dataReceived;
    unsigned int sleepDurationSeconds;

    String toJson(){
        DynamicJsonDocument doc(100);

        doc["sds"] = sleepDurationSeconds;

        String output;
        serializeJson(doc, output);
        doc.clear();
        doc.garbageCollect();

        return output;
    }
};
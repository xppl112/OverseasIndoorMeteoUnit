#pragma once

#include <vector>
#include "Models/BackendWeatherData.h"
#include "Models/Enums/WarningLevel.h"

struct PresentingBackendWeatherData
{
public:
    std::vector<BackendWeatherData> backendWeatherHistoricalData;

    WarningLevel WindWarningLevel = WarningLevel::UNKNOWN;
    WarningLevel RainWarningLevel = WarningLevel::UNKNOWN;

    WarningLevel GetOverallWarningLevel(){
        if(RainWarningLevel == WarningLevel::WARNING || WindWarningLevel == WarningLevel::WARNING)
            return WarningLevel::WARNING;
        else if(RainWarningLevel == WarningLevel::LOW_WARNING_LEVEL || WindWarningLevel == WarningLevel::LOW_WARNING_LEVEL)
            return WarningLevel::LOW_WARNING_LEVEL;
        
        return WarningLevel::UNKNOWN;
    }
};
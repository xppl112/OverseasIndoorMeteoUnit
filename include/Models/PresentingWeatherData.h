#pragma once

#include <vector>
#include "Models/WeatherMonitorData.h"
#include "Models/Enums/WarningLevel.h"

struct PresentingWeatherData
{
public:
    std::vector<WeatherMonitorData> weatherMonitorHistoricalData;

    WarningLevel RadiationWarningLevel = WarningLevel::UNKNOWN;
    WarningLevel PMWarningLevel = WarningLevel::UNKNOWN;
    WarningLevel VOCWarningLevel = WarningLevel::UNKNOWN;

    WarningLevel GetOverallWarningLevel(){
        if(RadiationWarningLevel == WarningLevel::CRITICAL)
            return WarningLevel::CRITICAL;
        else if(RadiationWarningLevel == WarningLevel::HI_WARNING_LEVEL || 
        PMWarningLevel == WarningLevel::HI_WARNING_LEVEL || 
        VOCWarningLevel == WarningLevel::HI_WARNING_LEVEL)
            return WarningLevel::HI_WARNING_LEVEL;
        else if(RadiationWarningLevel == WarningLevel::WARNING || 
        PMWarningLevel == WarningLevel::WARNING || 
        VOCWarningLevel == WarningLevel::WARNING)
            return WarningLevel::WARNING;
        else if(RadiationWarningLevel == WarningLevel::LOW_WARNING_LEVEL || 
        PMWarningLevel == WarningLevel::LOW_WARNING_LEVEL || 
        VOCWarningLevel == WarningLevel::LOW_WARNING_LEVEL)
            return WarningLevel::LOW_WARNING_LEVEL;
        
        return WarningLevel::UNKNOWN;
    }
};
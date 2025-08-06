#include "ApplicationServices/WeatherMonitor.h"
#include "Config.h"

//#define DEBUG

WeatherMonitor::WeatherMonitor(HardwareRegistry* hardwareRegistry, GlobalState* globalState){   
    _globalState = globalState;
    _airParticiplesSensor = hardwareRegistry->_airParticiplesSensor;
    _radiationSensor = hardwareRegistry->_radiationSensor;
    _meteoSensor = hardwareRegistry->_meteoSensor;
    
    _timer = new Ticker(0);
}

void WeatherMonitor::run(){
    state = WeatherMonitorState::IDLE;
    _timer->start(true);
}

void WeatherMonitor::updateTimers(){
    _timer->update();
    if(_timer->state() == FIRED){
        if(state == WeatherMonitorState::IDLE){
            startMeasuring();
            state = WeatherMonitorState::MEASURING;
            _timer->interval(_globalState->configuration.MeasurementDurationInSeconds * 1000);
        }
        else if(state == WeatherMonitorState::MEASURING){
            finishMeasuring();
            state = WeatherMonitorState::IDLE;   
            _timer->interval(_globalState->configuration.MeasurementCalmDownInSeconds * 1000);
        }
        _timer->start();
    }
}

void WeatherMonitor::addWeatherUpdatedEventHandler(WeatherMonitorUpdatedEventCallback callback){
    _onUpdateCallback = callback;
}

void WeatherMonitor::startMeasuring(){
    #ifdef DEBUG
    Serial.println("startMeasuring execution");
    #endif
    _airParticiplesSensor->beginMeasurement();
}

void WeatherMonitor::finishMeasuring(bool runWithoutStart){
    #ifdef DEBUG
    Serial.println("finishMeasuring execution");
    #endif
    IndoorWeatherData data;
    data.timestamp = millis();

    AirParticiplesSensorData airParticiplesData;
    if(!runWithoutStart)airParticiplesData = _airParticiplesSensor->endMeasurement();

    auto radiationData = _radiationSensor->getData();
    auto meteoData = _meteoSensor->getData();

    if(airParticiplesData.isDataReceived){
        data.PM_1_0 = airParticiplesData.PM_1_0;
        data.PM_2_5 = airParticiplesData.PM_2_5;
        data.PM_10_0 = airParticiplesData.PM_10_0;
    }

    if(radiationData.isDataReceived){
        data.radiation = radiationData.RadiationLevel;
    }

    if(meteoData.isDataReceived){
        data.temperatureCelsium = meteoData.temperatureAHTCelsium;
        data.humidity = meteoData.humidityBME;
        data.temperatureBMECelsium = meteoData.temperatureBMECelsium;
        data.humidityAHT = meteoData.humidityAHT;
        data.pressureInHPascals = meteoData.pressureInHPascals;
        data.gasResistance = meteoData.gasResistance;
    }

    registerWeatherData(data);
}

void WeatherMonitor::registerWeatherData(IndoorWeatherData data){
    _weatherMonitorHistoricalData.push_back(data);
    if(_weatherMonitorHistoricalData.size() > DATA_COLLECTION_CAPACITY){
        _weatherMonitorHistoricalData.erase(_weatherMonitorHistoricalData.begin());
    }

    if(_onUpdateCallback != NULL){
        PresentingIndoorWeatherData PresentingIndoorWeatherData {
            .weatherMonitorHistoricalData = _weatherMonitorHistoricalData
        };

        if(data.PM_2_5 >= _globalState->configuration.PM2_5_Level_Alert) PresentingIndoorWeatherData.PMWarningLevel = WarningLevel::HI_WARNING_LEVEL;
        else if(data.PM_2_5 >= _globalState->configuration.PM2_5_Level_Warning) PresentingIndoorWeatherData.PMWarningLevel = WarningLevel::WARNING;
        else if(data.PM_2_5 != -1) PresentingIndoorWeatherData.PMWarningLevel = WarningLevel::LOW_WARNING_LEVEL;
       
        auto gasAirPollution = data.calculateGasAirPollution();
        if(gasAirPollution >= _globalState->configuration.VOC_Percent_Level_Alert) PresentingIndoorWeatherData.VOCWarningLevel = WarningLevel::HI_WARNING_LEVEL;
        else if(gasAirPollution >= _globalState->configuration.VOC_Percent_Level_Warning) PresentingIndoorWeatherData.VOCWarningLevel = WarningLevel::WARNING;
        else if(gasAirPollution != -1) PresentingIndoorWeatherData.VOCWarningLevel = WarningLevel::LOW_WARNING_LEVEL;
        
        if(data.radiation >= _globalState->configuration.Radiation_Level_Critical) PresentingIndoorWeatherData.RadiationWarningLevel = WarningLevel::CRITICAL;
        else if(data.radiation >= _globalState->configuration.Radiation_Level_Alert) PresentingIndoorWeatherData.RadiationWarningLevel = WarningLevel::HI_WARNING_LEVEL;
        else if(data.radiation >= _globalState->configuration.Radiation_Level_Warning) PresentingIndoorWeatherData.RadiationWarningLevel = WarningLevel::WARNING;
        else if(data.radiation != -1)PresentingIndoorWeatherData.RadiationWarningLevel = WarningLevel::LOW_WARNING_LEVEL;

         _onUpdateCallback(PresentingIndoorWeatherData);
    }
}
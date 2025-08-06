#include "HardwareModules/Sensors/MeteoSensor.h"

MeteoSensor::MeteoSensor(uint8_t bme_i2c_address, uint8_t aht_i2c_address){
    _bmeSensor =  new BME680Sensor(bme_i2c_address);
    _ahtSensor =  new AHT20Sensor(aht_i2c_address);
}

void MeteoSensor::connect(){
    _bmeSensor->connect();
    _ahtSensor->connect();
}

void MeteoSensor::reset(){

}

MeteoSensorData MeteoSensor::getData(){
    MeteoSensorData data;
    data.isDataReceived = false;

    if(!_bmeSensor->isConnected || !_ahtSensor->isConnected)
        return data;

    auto bmeData = _bmeSensor->readData();
    auto ahtData = _ahtSensor->readData();
    this->registerDataFetching(bmeData.isDataReceived && ahtData.isDataReceived);

    data.isDataReceived = bmeData.isDataReceived && ahtData.isDataReceived;
    data.temperatureBMECelsium = bmeData.temperatureCelsium;
    data.humidityBME = bmeData.humidity;
    data.pressureInHPascals = bmeData.pressureInHPascals;
    data.gasResistance = bmeData.gasResistance;
    data.temperatureAHTCelsium = ahtData.temperatureCelsium - 1.0;
    data.humidityAHT = ahtData.humidity;

    return data;
}
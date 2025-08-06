#include "HardwareModules/PCFExtender.h"

PCFExtender::PCFExtender(uint8_t i2c_address)
{
    _pcf = new PCF8575(i2c_address);
}

void PCFExtender::connect(){
    _pcf->begin();
    _isConnected = true;
}

void PCFExtender::reset(){
    connect();
}
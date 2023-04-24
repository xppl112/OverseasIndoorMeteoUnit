#include "HardwareModules/IRReceiverModule.h"

IRReceiverModule::IRReceiverModule(uint8_t signalPin)
{
    _signalPin = signalPin;
    _receiver = new IRrecv(_signalPin);
}

void IRReceiverModule::connect(){
    _receiver->enableIRIn();
    _isConnected = true;
}

void IRReceiverModule::reset(){
    connect();
}

uint64_t IRReceiverModule::checkSignal(){
    decode_results results;
    if (_receiver->decode(&results)) {
        //serialPrintUint64(results.value, HEX);
        _receiver->resume(); 
        return results.value;
    }
    return 0;
}
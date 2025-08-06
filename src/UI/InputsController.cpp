#include "UI/InputsController.h"
#include "Config.h"

InputsController::InputsController(HardwareRegistry* hardwareRegistry){
    _ir = hardwareRegistry->_IRReceiverModule;
}

ButtonPressed InputsController::updateInputs(){
    auto signal = _ir->checkSignal();
    if(signal == 16769565)return ButtonPressed::STANDBY_ON;
    else if(signal == 16753245)return ButtonPressed::STANDBY_OFF;
    else if(signal == 16761405)return ButtonPressed::DISPLAY_RESET;
    else if(signal == 16720605)return ButtonPressed::CHANGE_SCREEN;
    else if(signal == 16736925)return ButtonPressed::SOUND_TOGGLE;
    return ButtonPressed::NONE;
}

uint64_t InputsController::getSignalCode(){
    auto signal = _ir->checkSignal();
    return signal;
}
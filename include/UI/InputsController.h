#ifndef INPUTSCONTROLLER_H
#define INPUTSCONTROLLER_H

#include "HardwareModules/HardwareRegistry.h"

enum class ButtonPressed {NONE, STANDBY_ON, STANDBY_OFF, DISPLAY_RESET, CHANGE_SCREEN, SOUND_TOGGLE};

class InputsController
{
public:
    InputsController(HardwareRegistry* hardwareRegistry);
    ButtonPressed updateInputs();
    uint64_t getSignalCode();

private: 
    IRReceiverModule* _ir;
};

#endif
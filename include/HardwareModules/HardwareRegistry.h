#ifndef HardwareRegistry_H
#define HardwareRegistry_H

#include "HardwareModules/IHardwareModule.h"
#include <stdint.h>
#include <Ticker.h>

#include "HardwareModules/Sensors/AirParticiplesSensor.h"
#include "HardwareModules/Sensors/RadiationSensor.h"
#include "HardwareModules/Sensors/MeteoSensor.h"
#include "HardwareModules/Sensors/LightSensor.h"
#include "HardwareModules/PCFExtender.h"
#include "HardwareModules/OLEDScreen.h"
#include "HardwareModules/LEDDigitalScreen.h"
#include "HardwareModules/BuzzerModule.h"
#include "HardwareModules/IRReceiverModule.h"

class HardwareRegistry
{
public:
    HardwareRegistry();

    void reconnectAllDisconnectedDevices();
    
    AirParticiplesSensor *_airParticiplesSensor;
    RadiationSensor *_radiationSensor;
    MeteoSensor *_meteoSensor;
    LightSensor *_lightSensor;
    PCFExtender *_PCFExtender;
    IRReceiverModule *_IRReceiverModule;

    OLEDScreen *_OLEDScreen;
    LEDDigitalScreen *_topLedScreen;
    LEDDigitalScreen *_middleLedScreen;
    LEDDigitalScreen *_bottomLedScreen;
    BuzzerModule *_buzzerModule;

private:
    void initializeDevices();
};

#endif
#include "HardwareModules/HardwareRegistry.h"
#include "GlobalObjects/GlobalState.h"
#include "Models/WeatherMonitorData.h"
#include "Models/PresentingWeatherData.h"
#include <Ticker.h>
#include "HardwareModules/Sensors/AirParticiplesSensor.h"
#include "HardwareModules/Sensors/RadiationSensor.h"
#include "HardwareModules/Sensors/MeteoSensor.h"
#include <vector>

typedef void (*WeatherMonitorUpdatedEventCallback)(PresentingWeatherData);

class WeatherMonitor
{
public:
    WeatherMonitor(HardwareRegistry* hardwareRegistry, GlobalState* globalState);
    void run();
    void updateTimers();
    void addUpdatedEventHandler(WeatherMonitorUpdatedEventCallback callback);
    void reconnectSensors();
    void resetSensors();
    enum class WeatherMonitorState {DISABLED, IDLE, MEASURING};
    WeatherMonitorState state = WeatherMonitorState::DISABLED;

private:
    void startMeasuring();
    void finishMeasuring(bool runWithoutStart = false);
    void registerWeatherData(WeatherMonitorData data);

    GlobalState* _globalState;

    WeatherMonitorUpdatedEventCallback _onUpdateCallback;
    Ticker* _timer;
    unsigned long _startMeasuringTimestamp;

    AirParticiplesSensor* _airParticiplesSensor;
    RadiationSensor* _radiationSensor;
    MeteoSensor* _meteoSensor;

    std::vector<WeatherMonitorData> _weatherMonitorHistoricalData;
};

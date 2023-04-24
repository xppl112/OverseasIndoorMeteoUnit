#ifndef IHardwareModule_H
#define IHardwareModule_H

enum class HardwareModuleConnectionType {UNKNOWN, I2C, UART, SPI, DIGITAL_PINS, ANALOG_PINS, WIRELESS};

struct HardwareModuleInfo {
    const char* Name;
    HardwareModuleConnectionType ConnectionType;
    float PowerVoltage;
    float CommunicationVoltage;
    int MaxCurrent;
    bool IsCritical;
};

class IHardwareModule
{
    public:
        virtual HardwareModuleInfo getInfo() = 0;

        virtual void connect() = 0;
        virtual bool isConnected() = 0;
        virtual void reset() = 0;

    protected:
        bool _isConnected = false;
};

#endif
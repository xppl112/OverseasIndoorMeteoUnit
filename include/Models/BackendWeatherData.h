#pragma once

struct BackendWeatherData
{
public:
    unsigned long timestamp;

    int raindropLevel;
    int windLevel;

    float temperatureCelsium;
    float pressureInHPascals;

    uint8_t getRainLevel(){
        if(raindropLevel<7)return 0;
        if(raindropLevel<13)return 1;
        if(raindropLevel<40)return 2;
        return 3;
    }
};
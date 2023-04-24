#include <Arduino.h>
#include "PCF8575.h"

#ifndef CONFIG_H
#define CONFIG_H

const uint8_t DATA_COLLECTION_CAPACITY = 30;
const uint8_t LIGHT_LEVEL_FOR_DARKNESS = 30;

const int OLED_IDLE_TIMEOUT = 300;
const int LIGHT_CHECK_TIMEOUT = 60;

const uint8_t ANALOG_INPUT_PIN = A0;

const uint8_t RADIATION_SENSOR_PIN = D7;
const uint8_t IR_PIN = D4;

// SCREENS
const uint8_t SCREENS_CLK_PIN = D6;
const uint8_t SCREEN1_DIO_PIN = D0;
const uint8_t SCREEN2_DIO_PIN = D5;
const uint8_t SCREEN3_DIO_PIN = D3;

// IO EXTENDER
const uint8_t STANDBY_LED_PIN = P14;
const uint8_t RAIN1_LED_PIN = P11;
const uint8_t RAIN2_LED_PIN = P12;
const uint8_t RAIN3_LED_PIN = P13;

const uint8_t AIR_0R5_LED_PIN = P9;
const uint8_t AIR_R_LED_PIN = P8;

const uint8_t AIR_0G3_LED_PIN = P4;
const uint8_t AIR_0G5_LED_PIN = P5;
const uint8_t AIR_G_LED_PIN = P6;

const uint8_t AIR_0B5_LED_PIN = P7;
const uint8_t AIR_B_LED_PIN = P10;

const uint8_t BEEPER_PIN = P3;

//Hardware I2C pins used
//const uint8_t I2C_SCL = D1;
//const uint8_t I2C_SDA = D2;
const uint8_t BMP_I2C_ADDR = 0x76;
const uint8_t BME_I2C_ADDR = 0X77;
const uint8_t AHT_I2C_ADDR = 0x38;
const uint8_t OLED_I2C_ADDR = 0x3C;
const uint8_t IO_I2C_ADDR = 0x20;

struct BackendClientConfig {
    const char* WifiSSID = "BELL763";
    const char* WifiPassword = "";
    const int WifiConnectionTimeout = 20;

    const char* ServerHost = "http://weather.liashko.space";
    const char* ServerApiPostWeatherDataUrl = "/api/weather/PostOverseasIndoorReport";
    const int HttpRequestTimeout = 10;
};

#endif
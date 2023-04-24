#ifndef TM1637LED_H
#define TM1637LED_H

#include <Arduino.h>
#include <TM1637.h>

class TM1637LED
{
public:
  TM1637LED(uint8_t clkPin, uint8_t dioPin);

  bool connect();
  void clear();
  void print(String str);
  void printIntWithUnit(int number, char unit);
  void printDecimal(float number, int decimalsCount);
  void printInt(int number);
  void printDecimalWithUnit(float number, char unit);
  void toggleColon();
  void setBrightness(uint8_t brightnessLevelPercent);

  bool isConnected;
  bool isColonShowed = false;

private:
  TM1637* _display;
  uint8_t _clkPin;
  uint8_t _dioPin;
};

#endif
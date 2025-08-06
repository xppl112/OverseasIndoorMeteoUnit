#pragma once

#include <Adafruit_SSD1306.h>

class CommonDrawFunctions
{
public:
    static void drawCenteredText(Adafruit_SSD1306* _screen,
        String text, uint8_t fontSize, uint16_t textColor, 
        uint16_t x1, uint16_t x2, uint16_t y);

    static uint16_t getTextWidth(Adafruit_SSD1306* _screen, String text);
};
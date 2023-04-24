#include "UI/Screens/CommonDrawFunctions.h" 
#include <Adafruit_GFX.h> 
#include "Fonts/FreeSans9pt7b.h" 
#include "Fonts/FreeSansBold18pt7b.h" 

void CommonDrawFunctions::drawCenteredText(
    Adafruit_SSD1306* _screen,
    String text, uint8_t fontSize, uint16_t textColor,
    uint16_t x1, uint16_t x2, uint16_t y){
        _screen->setTextSize(1);
        if(fontSize == 6)_screen->setFont(NULL);
        else if(fontSize == 9)_screen->setFont(&FreeSans9pt7b);
        else if(fontSize == 18)_screen->setFont(&FreeSansBold18pt7b);
        
        auto widthBound = getTextWidth(_screen, text);

        //_screen->setTextColor(ST77XX_BLACK);
        _screen->setCursor(x1 + ((x2-x1) / 2) - (widthBound / 2) +1,y+1);
        _screen->print(text);
        _screen->setTextColor(textColor);
        _screen->setCursor(x1 + ((x2-x1) / 2) - (widthBound / 2),y);
        _screen->print(text);
}

uint16_t CommonDrawFunctions::getTextWidth(Adafruit_SSD1306* _screen, String text){
    int16_t xBound, yBound; uint16_t widthBound, heightBound;
    _screen->getTextBounds(text, 0, 0, &xBound, &yBound, &widthBound, &heightBound);
    return widthBound;
}
// MyDisplay.cpp
#include "MyDisplay.h"

MyDisplay::MyDisplay(GyverOLED<SSD1306_128x64, OLED_BUFFER>& oled) : oled(oled) {}

void MyDisplay::myDisplay(uint8_t x, uint8_t y, uint8_t size_font, float volts, String text)
{
    oled.setScale(size_font);
    oled.setCursorXY(x, y); // курсор в (пиксель X, строка Y)
    oled.print(text);
    oled.print(volts, 1);
    oled.update();
}

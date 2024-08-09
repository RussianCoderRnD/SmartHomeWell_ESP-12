// MyDisplay.cpp
#include "MyDisplayString.h"

MyDisplayString::MyDisplayString(GyverOLED<SSD1306_128x64, OLED_BUFFER>& oled) : oled(oled) {}

void MyDisplayString::myDisplayString(uint8_t x, uint8_t y, uint8_t size_font,String text)
{
    oled.setScale(size_font);
    oled.setCursorXY(x, y); // курсор в (пиксель X, строка Y)
    oled.print(text);
    oled.update();
}

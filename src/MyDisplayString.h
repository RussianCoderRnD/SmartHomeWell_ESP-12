// MyDisplayString.h
#ifndef MYDISPLAYSTRING_H
#define MYDISPLAYSTRING_H

#include <GyverOLED.h> // Добавьте необходимые библиотеки

class MyDisplayString
{
public:
    MyDisplayString(GyverOLED<SSD1306_128x64, OLED_BUFFER>& oled);
    void myDisplayString(uint8_t x, uint8_t y, uint8_t size_font, String text = "Error");

private:
    GyverOLED<SSD1306_128x64, OLED_BUFFER>& oled;
};

#endif // MYDISPLAY_H

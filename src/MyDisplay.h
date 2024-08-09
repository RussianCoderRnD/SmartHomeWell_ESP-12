// MyDisplay.h
#ifndef MYDISPLAY_H
#define MYDISPLAY_H

#include <GyverOLED.h> // Добавьте необходимые библиотеки

class MyDisplay
{
public:
    MyDisplay(GyverOLED<SSD1306_128x64, OLED_BUFFER>& oled);
    void myDisplay(uint8_t x, uint8_t y, uint8_t size_font, float volts, String text = "Error");

private:
    GyverOLED<SSD1306_128x64, OLED_BUFFER>& oled;
};

#endif // MYDISPLAY_H

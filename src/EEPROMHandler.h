#ifndef EEPROMHANDLER_H
#define EEPROMHANDLER_H

#include <Arduino.h>          // Основная библиотека для Arduino
#include <EEPROM.h>

class EEPROMHandler {
public:
    // Инициализация EEPROM, вызывается в setup()
    void begin(size_t eepromSize) {
        EEPROM.begin(eepromSize);
    }

    // Сохранение float значения в EEPROM по заданному адресу
    void saveFloat(int address, float value) {
        // Разделение float на байты
        byte *ptr = (byte*)(void*)&value;
        for (size_t i = 0; i < sizeof(float); i++) {
            EEPROM.write(address + i, *(ptr + i));
        }
        EEPROM.commit(); // Запись в EEPROM
    }

    // Чтение float значения из EEPROM по заданному адресу
    float readFloat(int address) {
        float value = 0.0;
        byte *ptr = (byte*)(void*)&value;
        for (size_t i = 0; i < sizeof(float); i++) {
            *(ptr + i) = EEPROM.read(address + i);
        }
        return value;
    }
};

#endif // EEPROMHANDLER_H

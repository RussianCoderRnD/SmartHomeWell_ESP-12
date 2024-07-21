#ifndef VARIABLES_H
#define VARIABLES_H

#include <Arduino.h> // Подключаем библиотеку Arduino

float countSet = 0.0;            // Переменная для хранения значения
float count = 0.0;            // Переменная для хранения значения
float hysteresis = 0.0;       //
uint32_t eepromTimer = 0; // Переменная для таймера записи в EEPROM
boolean eepromFlag = false;
boolean onoff = true;
uint32_t tmr;         // Переменная таймера
float pressure = 0.0; // Переменная давления
#endif
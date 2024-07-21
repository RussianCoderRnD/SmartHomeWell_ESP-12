#ifndef VARIABLES_H
#define VARIABLES_H

#include <Arduino.h> // Подключаем библиотеку Arduino

float countSet = 0.0;  // Переменная для хранения значения count, которое будет установлено
float count = 0.0;     // Переменная для хранения текущего значения count
float hysteresis = 0.0;// Переменная для хранения значения гистерезиса
uint32_t eepromTimer = 0; // Переменная для хранения времени последней записи в EEPROM
boolean eepromFlag = false; // Флаг, указывающий на необходимость записи данных в EEPROM
boolean onoff = true; // Флаг, указывающий на состояние включено/выключено
boolean DS18B20_sensor_status = 0; // Переменная для хранения статуса датчика DS18B20
uint32_t tmr;          // Переменная для хранения времени последнего выполнения цикла
float pressure = 0.0;  // Переменная для хранения значения давления
float DS18B20_temperature = 0.0; // Переменная для хранения значения температуры с датчика DS18B20

#endif
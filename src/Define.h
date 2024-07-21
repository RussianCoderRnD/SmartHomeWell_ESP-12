#ifndef DEFINE_H
#define DEFINE_H

// Конфигурация датчика давления
#define PRESSURE_SENSOR_PIN A0// ПИН датчика давления
#define MAX_PRESSURE 1.2 // Максимальное давление (в МПа)


#define INIT_ADDR 1023 // Номер резервной ячейки EEPROM для первого запуска
#define INIT_KEY 50    // Ключ первого запуска EEPROM

#define Pin_PUMP D0           // Пин реле насоса
#define Pin_DS18B20 D1           // Пин реле насоса
#define Pin_PressureSensor A0 // Пин для подключения датчика давления

#define ALARM_TEMP 95 // Температура тревоги

#define EEPROM_WRITE_DELAY 10000   // Задержка перед записью в EEPROM

#define MONITOR_SPEED 115200       // Скорость монитора порта

#define INIT_ADDR 1023             // Номер резервной ячейки EEPROM для первого запуска
#define INIT_KEY 50                // Ключ первого запуска EEPROM
#define MEMTEMP_ADDR 0             // Адрес ячейки памяти EEPROM для хранения температуры


#define MY_PERIOD 500 // период в мс


#endif 

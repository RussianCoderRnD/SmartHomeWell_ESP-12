#ifndef LIBRARIES_H
#define LIBRARIES_H

#include <Arduino.h>          // Основная библиотека для Arduino
#include <ArduinoJson.h>      // Библиотека для работы с JSON
#include <EEPROM.h>           // Библиотека для работы с EEPROM
#include <Wire.h>             // Библиотека для работы с I2C
#include <ESP8266mDNS.h>      // Библиотека для работы с mDNS на ESP8266
#include <WiFiUdp.h>          // Библиотека для работы с UDP на ESP8266
#include <PinController.h>    // Класс для управления пинами
#include <PressureSensor.h>   // Класс для работы с датчиком давления
#include <Define.h>           // Файл с инструкциями #define
#include <variables.h>        // Файл с объявлением переменных
#include <PressureController.h> // Класс для управления давлением
#include <EEPROMHandler.h>    // Обработчик EEPROM
#include <MyOTA.h>            // Обновление ПО через воздух (OTA)
#include <WiFiAP.h>           // Настройка точки доступа (AP) WiFi
#include <microDS18B20.h>     // Библиотека для работы с датчиком температуры DS18B20
#include <MyDisplay.h>
#include <MyDisplayString.h>


#endif 
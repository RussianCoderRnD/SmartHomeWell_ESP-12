// WiFiAP.h
#ifndef WIFIAP_H
#define WIFIAP_H

#include <ESP8266WiFi.h>       // Подключаем библиотеку для работы с WiFi на ESP8266
#include <ESP8266WebServer.h>  // Подключаем библиотеку для создания веб-сервера на ESP8266

// Объявление внешних переменных и объектов, используемых в функции
extern const char *ssid;         // Имя сети (SSID), к которой будет подключаться точка доступа (AP)
extern const char *password;     // Пароль сети (SSID) для точки доступа
extern IPAddress local_ip;       // Локальный IP-адрес для точки доступа
extern IPAddress gateway;        // IP-адрес шлюза (обычно совпадает с локальным IP-адресом)
extern IPAddress subnet;         // Маска подсети
extern ESP8266WebServer server;  // Объект веб-сервера, который будет обрабатывать HTTP-запросы

void WiFiAP();  // Объявление функции для инициализации и настройки точки доступа (AP)

#endif

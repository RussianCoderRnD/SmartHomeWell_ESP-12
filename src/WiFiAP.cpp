// WiFiAP.cpp
#include "WiFiAP.h"
#include <ArduinoJson.h>

void handleRoot();
void handleCount();
void handleOnOff();
void handleHysteresis();
void handleRes();

void WiFiAP()
{
  /* Инициализация точки доступа (AP) */
  if (WiFi.softAP(ssid, password))
  {
    WiFi.softAPConfig(local_ip, gateway, subnet);
    Serial.println();
    Serial.print("Точка доступа создана с именем: ");
    Serial.println(ssid);
    Serial.print("IP-адрес: ");
    Serial.println(WiFi.softAPIP());
    IPAddress ip = WiFi.softAPIP();

    /* Настройка маршрутов для веб-сервера */
    server.on("/", HTTP_GET, handleRoot);
    server.on("/count", HTTP_GET, handleCount);           /* Регистрация обработчика для URI /count */
    server.on("/onoff", HTTP_GET, handleOnOff);           /* Регистрация обработчика для URI /onoff */
    server.on("/hysteresis", HTTP_GET, handleHysteresis); /* Регистрация обработчика для URI /hysteresis */
    server.on("/res", HTTP_GET, handleRes);
    server.begin(); /* Запуск веб-сервера */
  }
  else
  {
    Serial.println(F("Error: The access point could not be created"));
    ESP.restart(); /* Перезапуск ESP в случае ошибки */
  }
}

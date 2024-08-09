// WiFiAP.cpp
#include "WiFiAP.h"
#include <ArduinoJson.h>

extern MyDisplayString MyDisS; // Убедитесь, что переменная MyDis объявлена как extern

void handleRoot();
void handleCount();
void handleHysteresis();
void handleRes();
void handleOnOff();

// Создание экземпляра класса MyDisplay
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
    MyDisS.myDisplayString(0, 0, 0,  "IP name: ");
    MyDisS.myDisplayString(0, 15, 0,  ssid);
    MyDisS.myDisplayString(0, 30, 0,  "IP adress: ");
    MyDisS.myDisplayString(0, 45, 0,  WiFi.softAPIP().toString());
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

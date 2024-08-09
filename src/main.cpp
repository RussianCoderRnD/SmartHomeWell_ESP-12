//---------------- БИБЛИОТЕКИ START ---------------------
#include <Lidraries.h> /* Файл со списком подключенных библиотек */
//----------------- БИБЛИОТЕКИ END-----------------------

//------------ СОЗДАНИЕ ЭКЗЕМПЛЯРА START ----------------
IPAddress local_ip(192, 168, 4, 11); /* Локальный IP-адрес */
IPAddress gateway(192, 168, 4, 11);  /* Шлюз */
IPAddress subnet(255, 255, 255, 0);  /* Маска подсети */

ESP8266WebServer server(80); /* Создание экземпляра веб-сервера на порту 80 */

GyverOLED<SSD1306_128x64, OLED_BUFFER> oled;
EEPROMHandler eepromHandler;
//------------- СОЗДАНИЕ ЭКЗЕМПЛЯРА END -----------------

//----------------- CONSTANTS START ---------------------
const char *ssid = "ESP_CKB";      /* Имя сети (SSID) */
const char *password = "19781503"; /* Пароль сети */

// const float humidityThreshold = 60.0; // Порог влажности для включения вентилятора

//----------------- CONSTANTS END -----------------------

//----------------- СПИСОК ФУНКЦИЙ START-----------------

void setBright();   // Функция для ограничения температуры и установки флага EEPROM
void checkEEPROM(); // Функция для проверки и записи данных в EEPROM

// Создание экземпляра класса MyDisplay
MyDisplay MyDis(oled);
MyDisplayString MyDisS(oled);

void setup()
{
  Serial.begin(MONITOR_SPEED); /* Открываем порт для связи с ПК */

  oled.init();   // инициализация
  oled.clear();  // очистить дисплей (или буфер)
  oled.update(); // обновить. Только для режима с буфером! OLED_BUFFER

  WiFiAP(); /* Инициализация точки доступа (AP) */
  MyOTA();  // Настройка OTA
  delay(5000);

  eepromHandler.begin(32); /* Инициализация EEPROM с размером 512 байт */

  oled.clear();  // очистить дисплей (или буфер)
  oled.update(); // обновить. Только для режима с буфером! OLED_BUFFER
}

void loop()
{
  checkEEPROM(); /* Проверка и запись в EEPROM */
  if (millis() - tmr >= MY_PERIOD)
  {
    tmr = millis(); /* Сброс таймера */
    MyDis.myDisplay(0, 5, 0, 1, "Влажность: ");
    MyDis.myDisplay(0, 20, 0, 2, "Температура: ");
    MyDis.myDisplay(0, 35, 0, 3, "Порог влажности: ");
  }
  server.handleClient(); /* Обработка клиентских запросов */
  ArduinoOTA.handle();
}

//========== Обработчик корневого запроса "/"
void handleRoot()
{
  server.send(200, "text/plain", "Hi, this is ESP_RangeHood");
  Serial.println("Hi, this is ESP_CKB");
}

//========== Обработчик запроса "/res"
void handleRes()
{
  /* Создаем JSON объект */
  StaticJsonDocument<200> jsonDoc;
  /* Добавляем переменные в JSON объект */
  jsonDoc["PR"] = 4.1;                     // pressure;
  jsonDoc["COUNT"] = 2.2;                  // count;
  jsonDoc["ONOFF"] = true;                 // onoff;
  jsonDoc["HYSTERESIS"] = 0.3;             // hysteresis;
  jsonDoc["RelayOnOff"] = true;            // RelayOnOff;
  jsonDoc["DS18B20_sensor_status"] = true; // DS18B20_sensor_status;
  jsonDoc["DS18B20_temperature"] = 24.6;   // DS18B20_temperature;

  /* Преобразуем JSON объект в строку */
  String jsonString;
  serializeJson(jsonDoc, jsonString);

  /* Отправляем JSON клиенту */
  server.send(200, "application/json", jsonString);
  Serial.println("jsonString " + jsonString);
}

//========== Обработчик запроса "/onoff"
void handleOnOff()
{
  String message = "";
  if (server.hasArg("onoff"))
  {
    onoff = server.arg("onoff").toInt(); /* Получаем значение параметра onoff и конвертируем в int */
    // setBright();
    message = "The onoff value is set: " + String(onoff);
    Serial.println("The onoff value is set: " + String(onoff));
  }
  else
  {
    message = "Error: The onoff parameter is missing or the format is incorrect";
    Serial.println("Error: The onoff parameter is missing or the format is incorrect");
  }
  server.send(200, "text/plain", message);
}

//========== Обработчик запроса "/hysteresis"
void handleHysteresis()
{
}

//========== Обработчик запроса "/count"
void handleCount()
{
}

//====================setBright==========================
void setBright(void)
{
}

//===================checkEEPROM=========================
void checkEEPROM(void)
{
}

//====================EEPROMRead=========================
void EEPROMRead(void)
{
}
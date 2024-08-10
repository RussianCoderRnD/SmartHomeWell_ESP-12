//---------------- БИБЛИОТЕКИ START ---------------------
#include <Lidraries.h> /* Файл со списком подключенных библиотек */
//----------------- БИБЛИОТЕКИ END-----------------------

//------------ СОЗДАНИЕ ЭКЗЕМПЛЯРА START ----------------
PressureSensor pressureSensor(PRESSURE_SENSOR_PIN, 7.2); /* Создание объекта датчика давления */

PressureController pressureController(Pin_PUMP); /* Создание объекта с номером ПИНа реле */

GyverOLED<SSD1306_128x64, OLED_BUFFER> oled;

IPAddress local_ip(192, 168, 4, 10); /* Локальный IP-адрес */
IPAddress gateway(192, 168, 4, 10);  /* Шлюз */
IPAddress subnet(255, 255, 255, 0);  /* Маска подсети */

ESP8266WebServer server(80); /* Создание экземпляра веб-сервера на порту 80 */

EEPROMHandler eepromHandler;

// Создание экземпляра класса MyDisplay
MyDisplay MyDis(oled);
MyDisplayString MyDisS(oled);

// Датчики на D2
MicroDS18B20<Pin_DS18B20> DS18B20_sensor;
//------------- СОЗДАНИЕ ЭКЗЕМПЛЯРА END -----------------

//----------------- CONSTANTS START ---------------------
const char *ssid = "ESP_CKB";      /* Имя сети (SSID) */
const char *password = "19781503"; /* Пароль сети */

// const float humidityThreshold = 60.0; // Порог влажности для включения вентилятора

//----------------- CONSTANTS END -----------------------

//----------------- СПИСОК ФУНКЦИЙ START-----------------

void setBright();   // Функция для ограничения температуры и установки флага EEPROM
void checkEEPROM(); // Функция для проверки и записи данных в EEPROM

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

  hysteresis = eepromHandler.readFloat(6); /* Чтение значения из EEPROM */
  count = eepromHandler.readFloat(0);      /* Чтение значения из EEPROM */

  oled.clear();  // очистить дисплей (или буфер)
  oled.update(); // обновить. Только для режима с буфером! OLED_BUFFER
  MyDisS.myDisplayString(0, 0, 0, "DS18B20: ");
  MyDisS.myDisplayString(0, 10, 0, "Pressure: ");
  MyDisS.myDisplayString(0, 24, 0, "Count: ");
  MyDisS.myDisplayString(0, 35, 0, "Hysteresis: ");
}

void loop()
{
  checkEEPROM(); /* Проверка и запись в EEPROM */ //
                                                  // boolean temps = digitalRead(pressureController.getRelayPin()) ? "ON" : "OFF";

  if (millis() - tmr >= 1000)
  {
    tmr = millis(); /* Сброс таймера */

    pressure = pressureSensor.getPressure(); /* Получение давления */

    pressureController.setParameters(count, hysteresis);

    // просто выводим температуру первого датчика
    DS18B20_temperature = DS18B20_sensor.getTemp();

    // проверяем успешность чтения и выводим
    if (DS18B20_sensor.readTemp())
      DS18B20_sensor_status = true;
    else
      DS18B20_sensor_status = false;

    DS18B20_sensor.requestTemp();

    /* Обновление состояния реле на основе текущего давления */
    pressureController.update(pressure, onoff);

    MyDisS.myDisplayString(80, 0, 0, (String)DS18B20_temperature);
    MyDisS.myDisplayString(80, 10, 0, (String)pressure);
    MyDisS.myDisplayString(80, 24, 0, (String)count);
    MyDisS.myDisplayString(80, 35, 0, (String)hysteresis);
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
  boolean RelayOnOff = digitalRead(Pin_PUMP);
  /* Создаем JSON объект */
  StaticJsonDocument<200> jsonDoc;
  /* Добавляем переменные в JSON объект */
  jsonDoc["PR"] = String(pressure,2);                     // pressure;
  jsonDoc["COUNT"] = String(count,2);                 // count;
  jsonDoc["ONOFF"] = onoff;                 // onoff;
  jsonDoc["HYSTERESIS"] = String(hysteresis,2);             // hysteresis;
  jsonDoc["RelayOnOff"] = RelayOnOff;            // RelayOnOff;
  jsonDoc["DS18B20_sensor_status"] = DS18B20_sensor_status; // DS18B20_sensor_status;
  jsonDoc["DS18B20_temperature"] =  DS18B20_temperature;   // DS18B20_temperature;

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
  String message = "";
  if (server.hasArg("hysteresis"))
  {
    hysteresis = server.arg("hysteresis").toFloat(); // Получаем значение параметра hysteresis и конвертируем в int
    setBright();
    message = "The hysteresis value is set: " + String(hysteresis);
    Serial.println("The hysteresis value is set: " + String(hysteresis));
  }
  else
  {
    message = "Error: The hysteresis parameter is missing or the format is incorrect";
    Serial.println("Error: The hysteresis parameter is missing or the format is incorrect");
  }
  server.send(200, "text/plain", message);
}

//========== Обработчик запроса "/count"
void handleCount()
{
  String message = "";
  if (server.hasArg("count"))
  {
    count = server.arg("count").toFloat(); /* Получаем значение параметра count и конвертируем в float */
    setBright();
    message = "The count value is set: " + String(count);
    Serial.println("The count value is set: " + String(count));
  }
  else
  {
    message = "Error: The count parameter is missing or the format is incorrect";
    Serial.println("Error: The count parameter is missing or the format is incorrect");
  }
  server.send(200, "text/plain", message);
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
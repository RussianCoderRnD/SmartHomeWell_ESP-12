/**/

//---------------- БИБЛИОТЕКИ START ---------------------
#include <Lidraries.h> //Фаил со списком подключенных библиотек
//----------------- БИБЛИОТЕКИ END-----------------------

//------------ СОЗДАНИЕ ЭКЗЕМПЛЯРА START ----------------
PressureSensor pressureSensor(PRESSURE_SENSOR_PIN, 1.2); // Создание объекта датчика давления
// Создание объекта PressureController
PressureController pressureController(Pin_PUMP); // Создание объекта с целевым давлением 1.0 и гистерезисом 0.1

// PinController TTP_PUMP(Pin_PUMP);                                 // Создаем объект класса для пина /*D0 (GPIO16)*/

IPAddress local_ip(192, 168, 4, 10); // Локальный IP-адрес
IPAddress gateway(192, 168, 4, 10);  // Шлюз
IPAddress subnet(255, 255, 255, 0);  // Маска подсети

ESP8266WebServer server(80); // Создание экземпляра веб-сервера на порту 80

EEPROMHandler eepromHandler;
//------------- СОЗДАНИЕ ЭКЗЕМПЛЯРА END -----------------

//----------------- CONSTANTS START ---------------------
const char *ssid = "ESP_CKB";      // Имя сети (SSID)
const char *password = "19781503"; // Пароль сети//----------------- CONSTANTS END -----------------------

//---------------- ПЕРЕМЕННЫЕ START----------------------
// в файле variables.h
//----------------- ПЕРЕМЕННЫЕ END ----------------------

//----------------- СПИСОК ФУНКЦИЙ START-----------------
void handleCount(); // Обработчик запроса на получение значения count
void handleRoot();
void handleRes();
void handleOnOff();
void handleHysteresis();
void setBright();   // Функция для ограничения температуры и установки флага EEPROM
void checkEEPROM(); // Функция для проверки и записи данных в EEPROM
void WiFiAP();
void OTA();
//----------------- СПИСОК ФУНКЦИЙ END ------------------

//--------------------- SETUP START ---------------------
void setup(void)
{
  Serial.begin(MONITOR_SPEED); // открываем порт для связи с ПК
onoff =false;
  eepromHandler.begin(32); // Инициализация EEPROM с размером 512 байт

  hysteresis =  eepromHandler.readFloat(6);   // Чтение значения из EEPROM
  count = eepromHandler.readFloat(0);   // Чтение значения из EEPROM

  WiFiAP(); // Инициализация точки доступа (AP)
  OTA();    // Настройка OTA
}
//--------------------- SETUP END ----------------------

//------------------- LOOP START -----------------------
void loop(void)
{
checkEEPROM(); // Проверка и запись в EEPROM
  if (millis() - tmr >= MY_PERIOD)
  {                 // ищем разницу
    tmr = millis(); // сброс таймера

    pressure = pressureSensor.getPressure(); // Получение давления

    pressureController.setParameters(count, hysteresis);

    // Обновление состояния реле на основе текущего давления
    pressureController.update(pressure, onoff);

    Serial.print("pressure ");
    Serial.print(pressure);
    Serial.print("   count ");
    Serial.print(count);
    Serial.print("  hysteresis ");
    Serial.print(hysteresis);
    Serial.print(" bar, Relay State: ");
    Serial.println(digitalRead(pressureController.getRelayPin()) ? "ON" : "OFF");
    
  }
  ArduinoOTA.handle();
  server.handleClient(); // Обработка клиентских запросов
}
//--------------------- LOOP END ----------------------

//====================setBright==========================
void setBright(void)
{
  eepromFlag = true;      // поднять флаг
  eepromTimer = millis(); // сбросить таймер
}

//===================checkEEPROM=========================
void checkEEPROM(void)
{
  if (eepromFlag && (millis() - eepromTimer >= EEPROM_WRITE_DELAY))
  {
    eepromHandler.saveFloat(0, count);      // Сохранение значения по адресу 0
    eepromHandler.saveFloat(6, hysteresis); // Сохранение значения по адресу 1
    // если флаг поднят и с последнего нажатия прошло 10 секунд (10 000 мс)
    eepromFlag = false; // опустили флаг

    Serial.print("\ncheckEEPROM(");
    Serial.print(" count ");
    Serial.print(count);
    Serial.print(" hysteresis ");
    Serial.println(hysteresis);
  }
}

//====================OTA=========================
void OTA()
{
  // Настройка OTA
  ArduinoOTA.onStart([]()
                     {
    String type;
    if (ArduinoOTA.getCommand() == U_FLASH) {
      type = "sketch";
    } else { // U_SPIFFS
      type = "filesystem";
    }
    // NOTE: если обновляется файловая система, то надо отмонтировать SPIFFS
    Serial.println("Start updating " + type); });
  ArduinoOTA.onEnd([]()
                   { Serial.println("\nEnd"); });
  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total)
                        { Serial.printf("Progress: %u%%\r", (progress / (total / 100))); });
  ArduinoOTA.onError([](ota_error_t error)
                     {
    Serial.printf("Error[%u]: ", error);
    if (error == OTA_AUTH_ERROR) {
      Serial.println("Auth Failed");
    } else if (error == OTA_BEGIN_ERROR) {
      Serial.println("Begin Failed");
    } else if (error == OTA_CONNECT_ERROR) {
      Serial.println("Connect Failed");
    } else if (error == OTA_RECEIVE_ERROR) {
      Serial.println("Receive Failed");
    } else if (error == OTA_END_ERROR) {
      Serial.println("End Failed");
    } });

  ArduinoOTA.begin();
}

//====================WiFiAP=========================
void WiFiAP()
{
  // Инициализация точки доступа (AP)
  if (WiFi.softAP(ssid, password))
  {
    WiFi.softAPConfig(local_ip, gateway, subnet);
    Serial.println();
    Serial.print("Точка доступа создана с именем: ");
    Serial.println(ssid);
    Serial.print("IP-адрес: ");
    Serial.println(WiFi.softAPIP());
    IPAddress ip = WiFi.softAPIP();

    // Настройка маршрутов для веб-сервера
    server.on("/", HTTP_GET, handleRoot);
    server.on("/count", HTTP_GET, handleCount);           // Регистрация обработчика для URI /count
    server.on("/onoff", HTTP_GET, handleOnOff);           // Регистрация обработчика для URI /count
    server.on("/hysteresis", HTTP_GET, handleHysteresis); // Регистрация обработчика для URI /count
    server.on("/res", HTTP_GET, handleRes);
    server.begin(); // Запуск веб-сервера
  }
  else
  {
    Serial.println(F("Error: The access point could not be created"));
    ESP.restart(); // Перезапускаем ESP в случае ошибки
  }
}

//========== Обработчик запроса "/res"
void handleRes()
{
  boolean RelayOnOff = digitalRead(Pin_PUMP);
  // Создаем JSON объект
  StaticJsonDocument<200> jsonDoc;
  // Добавляем переменные в JSON объект
  jsonDoc["PR"] = pressure;
  jsonDoc["COUNT"] = count;
  jsonDoc["ONOFF"] = onoff;
  jsonDoc["HYSTERESIS"] = hysteresis;
  jsonDoc["RELAY"] = RelayOnOff;

  // Преобразуем JSON объект в строку
  String jsonString;
  serializeJson(jsonDoc, jsonString);

  // Отправляем JSON клиенту
  server.send(200, "application/json", jsonString);
  Serial.println("jsonString " + jsonString);
}

//========== Обработчик запроса "/count"
void handleCount()
{
  String message = "";
  if (server.hasArg("count"))
  {
    count = server.arg("count").toFloat(); // Получаем значение параметра count и конвертируем в int
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

void handleOnOff()
{
  String message = "";
  if (server.hasArg("onoff"))
  {
    onoff = server.arg("onoff").toInt(); // Получаем значение параметра count и конвертируем в int
    setBright();
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

//========== Обработчик корневого запроса "/"
void handleRoot()
{
  server.send(200, "text/plain", "Hi, this is ESP_CKB");
  Serial.println("Hi, this is ESP_CKB");
}

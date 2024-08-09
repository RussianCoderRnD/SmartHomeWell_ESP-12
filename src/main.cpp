//---------------- БИБЛИОТЕКИ START ---------------------
#include <Lidraries.h> /* Файл со списком подключенных библиотек */
//----------------- БИБЛИОТЕКИ END-----------------------

//------------ СОЗДАНИЕ ЭКЗЕМПЛЯРА START ----------------
PressureSensor pressureSensor(PRESSURE_SENSOR_PIN, 1.2); /* Создание объекта датчика давления */

PressureController pressureController(Pin_PUMP); /* Создание объекта с номером ПИНа реле */
GyverOLED<SSD1306_128x64, OLED_BUFFER> oled;
// Датчики на D2
MicroDS18B20<Pin_DS18B20> DS18B20_sensor;
IPAddress local_ip(192, 168, 4, 10); /* Локальный IP-адрес */
IPAddress gateway(192, 168, 4, 10);  /* Шлюз */
IPAddress subnet(255, 255, 255, 0);  /* Маска подсети */

ESP8266WebServer server(80); /* Создание экземпляра веб-сервера на порту 80 */

PinController _pinController();

EEPROMHandler eepromHandler;
//------------- СОЗДАНИЕ ЭКЗЕМПЛЯРА END -----------------

//----------------- CONSTANTS START ---------------------
const char *ssid = "ESP_CKB"; /* Имя сети (SSID) */
const char *password = "19781503";  /* Пароль сети */

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
  oled.init();                 // инициализация
  oled.clear();                // очистить дисплей (или буфер)
  oled.update();               // обновить. Только для режима с буфером! OLED_BUFFER
  eepromHandler.begin(32);     /* Инициализация EEPROM с размером 512 байт */

  hysteresis = eepromHandler.readFloat(6); /* Чтение значения из EEPROM */
  count = eepromHandler.readFloat(0);      /* Чтение значения из EEPROM */

  WiFiAP(); /* Инициализация точки доступа (AP) */
  MyOTA();  /* Настройка OTA */

  oled.clear();  // очистить дисплей (или буфер)
  oled.update(); // обновить. Только для режима с буфером! OLED_BUFFER
}

void loop()
{
  checkEEPROM(); /* Проверка и запись в EEPROM */
  if (millis() - tmr >= MY_PERIOD)
  {                 /* Ищем разницу */
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

    Serial.print("DS18B20_sensor ");
    Serial.print(DS18B20_temperature);
    Serial.print("  pressure ");
    Serial.print(pressure);
    Serial.print("  count ");
    Serial.print(count);
    Serial.print("  hysteresis ");
    Serial.print(hysteresis);
    Serial.print(" bar, Relay State: ");
    Serial.println(digitalRead(pressureController.getRelayPin()) ? "ON" : "OFF");
    MyDisS.myDisplayString(0, 0, 0, "DS18B20: "+(String)DS18B20_temperature);
    MyDisS.myDisplayString(0, 10, 0, "Pressure: "+(String)pressure);
    MyDisS.myDisplayString(0, 24, 0, "Count: "+(String)count);
    MyDisS.myDisplayString(0, 35, 0, "Hysteresis: "+(String)hysteresis);
    MyDisS.myDisplayString(0, 48, 0, "Relay State: ");
    MyDisS.myDisplayString(80, 48, 0, (String)digitalRead(pressureController.getRelayPin()) ? "ON" : "OFF");
  }
  server.handleClient(); /* Обработка клиентских запросов */

  ArduinoOTA.handle();
}
//--------------------- LOOP END ----------------------

//====================setBright==========================
void setBright(void)
{
  eepromFlag = true;      /* Поднять флаг */
  eepromTimer = millis(); /* Сбросить таймер */
}

//===================checkEEPROM=========================
void checkEEPROM(void)
{
  if (eepromFlag && (millis() - eepromTimer >= EEPROM_WRITE_DELAY))
  {
    eepromHandler.saveFloat(0, count);      /* Сохранение значения по адресу 0 */
    eepromHandler.saveFloat(6, hysteresis); /* Сохранение значения по адресу 6 */
    /* Если флаг поднят и с последнего нажатия прошло 10 секунд (10 000 мс) */
    eepromFlag = false; /* Опустить флаг */

    Serial.print("\ncheckEEPROM(");
    Serial.print(" count ");
    Serial.print(count);
    Serial.print(" hysteresis ");
    Serial.println(hysteresis);
  }
}

//========== Обработчик запроса "/res"
void handleRes()
{
  boolean RelayOnOff = digitalRead(Pin_PUMP);
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

//========== Обработчик запроса "/onoff"
void handleOnOff()
{
  String message = "";
  if (server.hasArg("onoff"))
  {
    onoff = server.arg("onoff").toInt(); /* Получаем значение параметра onoff и конвертируем в int */
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

//========== Обработчик корневого запроса "/"
void handleRoot()
{
  server.send(200, "text/plain", "Hi, this is ESP_CKB");
  Serial.println("Hi, this is ESP_CKB");
}

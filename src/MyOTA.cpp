#include "MyOTA.h"

void MyOTA() {
  /* Настройка OTA (Over-The-Air обновление прошивки) */

  // Установка функции-обработчика для начала OTA
  ArduinoOTA.onStart([]() {
    String type;
    // Определение типа обновления: прошивка или файловая система
    if (ArduinoOTA.getCommand() == U_FLASH) {
      type = "sketch"; // Обновление прошивки
    } else { // U_SPIFFS
      type = "filesystem"; // Обновление файловой системы
    }
    // Печать сообщения о начале обновления
    // ПРИМЕЧАНИЕ: если обновляется файловая система, нужно отмонтировать SPIFFS
    Serial.println("Start updating " + type);
  });

  // Установка функции-обработчика для завершения OTA
  ArduinoOTA.onEnd([]() {
    Serial.println("\nEnd"); // Печать сообщения о завершении обновления
  });

  // Установка функции-обработчика для отображения прогресса обновления
  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
    Serial.printf("Progress: %u%%\r", (progress / (total / 100))); // Печать прогресса обновления в процентах
  });

  // Установка функции-обработчика для обработки ошибок OTA
  ArduinoOTA.onError([](ota_error_t error) {
    Serial.printf("Error[%u]: ", error); // Печать кода ошибки
    if (error == OTA_AUTH_ERROR) {
      Serial.println("Auth Failed"); // Ошибка аутентификации
    } else if (error == OTA_BEGIN_ERROR) {
      Serial.println("Begin Failed"); // Ошибка начала обновления
    } else if (error == OTA_CONNECT_ERROR) {
      Serial.println("Connect Failed"); // Ошибка подключения
    } else if (error == OTA_RECEIVE_ERROR) {
      Serial.println("Receive Failed"); // Ошибка получения данных
    } else if (error == OTA_END_ERROR) {
      Serial.println("End Failed"); // Ошибка завершения обновления
    }
  });

  // Инициализация OTA
  ArduinoOTA.begin();
}

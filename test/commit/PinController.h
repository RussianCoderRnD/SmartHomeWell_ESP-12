#ifndef PINCONTROLLER_H
#define PINCONTROLLER_H

#include <Arduino.h>  // Подключаем библиотеку Arduino

// Объявление класса PinController
class PinController {
  public:
    // Конструктор класса, принимающий номер пина
    PinController(uint8_t pin) : pin_(pin) {
      pinMode(pin_, OUTPUT);  // Устанавливаем пин как выход
      digitalWrite(pin_, LOW);  // Начальное состояние пина - LOW (выключено)
    }

    // Публичный метод для установки состояния пина
    void setState(bool state) {
      if (state) {
        on();  // Если state = true, вызываем метод on
      } else {
        off();  // Если state = false, вызываем метод off
      }
    }

  private:
    uint8_t pin_;  // Номер пина, который контролируется

    // Приватный метод для включения пина
    void on() {
      digitalWrite(pin_, HIGH);  // Устанавливаем пин в HIGH (включено)
    }

    // Приватный метод для выключения пина
    void off() {
      digitalWrite(pin_, LOW);  // Устанавливаем пин в LOW (выключено)
    }
};

#endif // PINCONTROLLER_H

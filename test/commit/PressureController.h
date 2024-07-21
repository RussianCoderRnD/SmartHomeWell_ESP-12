#ifndef PRESSURECONTROLLER_H
#define PRESSURECONTROLLER_H

#include <Arduino.h>

class PressureController
{
public:
    // Конструктор класса
    PressureController(uint8_t relayPin)
        : relayPin_(relayPin), relayState_(LOW)
    {
        pinMode(relayPin_, OUTPUT);
        digitalWrite(relayPin_, relayState_); // Начальное состояние реле - выключено
    }

    // Метод для установки нового значения давления и гистерезиса
    void setParameters(float pressure, float hysteresis)
    {
        pressure_ = pressure;
        hysteresis_ = hysteresis;
    }

    // Метод для обновления состояния реле на основе текущего давления
    void update(float currentPressure, boolean RelayOnOff)
    {
        if (RelayOnOff == true)
        {
            if (currentPressure >= pressure_ + hysteresis_ && relayState_ == HIGH)
            {
                // Если текущее давление превысило порог плюс гистерезис и реле включено, выключаем реле
                digitalWrite(relayPin_, LOW);
                relayState_ = LOW;
            }
            else if (currentPressure <= pressure_ - hysteresis_ && relayState_ == LOW)
            {
                // Если текущее давление упало ниже порога минус гистерезис и реле выключено, включаем реле
                digitalWrite(relayPin_, HIGH);
                relayState_ = HIGH;
            }
        }
        else
        {
            digitalWrite(relayPin_, LOW);
            relayState_ = LOW;
        }

        // Иначе оставляем текущее состояние реле без изменений
    }

    // Открытый метод для получения пина реле
    uint8_t getRelayPin() const
    {
        return relayPin_; // Возвращает номер пина реле
    }

private:
    float pressure_;     // Текущее значение давления
    float hysteresis_;   // Значение гистерезиса
    uint8_t relayPin_;   // Номер пина для управления реле
    uint8_t relayState_; // Текущее состояние реле (HIGH или LOW)
};

#endif // PRESSURECONTROLLER_H

/*

// Создание объекта PressureController
PressureController pressureController(count, hysteresis); // Начальные значения: текущее давление 1.0, гистерезис 0.1

 // Обновление состояния реле на основе текущего давления
    pressureController.update(pressure);

#ifndef PRESSURECONTROLLER_H
#define PRESSURECONTROLLER_H

#include <Arduino.h>

class PressureController {
public:
    // Конструктор класса
    PressureController(float pressure, float hysteresis)
        : pressure_(pressure), hysteresis_(hysteresis), relayPin_(D0) {
            pinMode(relayPin_, OUTPUT);
            digitalWrite(relayPin_, LOW); // Начальное состояние реле - выключено
    }

    // Метод для установки нового значения давления и гистерезиса
    void setParameters(float pressure, float hysteresis) {
        pressure_ = pressure;
        hysteresis_ = hysteresis;
    }

    // Метод для обновления состояния реле на основе текущего давления
    void update(float currentPressure) {
        if (currentPressure < pressure_ - hysteresis_) {
            // Давление ниже порогового значения минус гистерезис - включаем реле
            digitalWrite(relayPin_, HIGH);
        } else if (currentPressure > pressure_ + hysteresis_) {
            // Давление выше порогового значения плюс гистерезис - выключаем реле
            digitalWrite(relayPin_, LOW);
        }
        // Иначе оставляем текущее состояние реле без изменений
    }

    // Открытый метод для получения пина реле
    uint8_t getRelayPin() const {
        return relayPin_;
    }

private:
    float pressure_;    // Значение давления
    float hysteresis_;  // Значение гистерезиса
    uint8_t relayPin_;  // Номер пина для управления реле
};

#endif // PRESSURECONTROLLER_H
*/
#ifndef PRESSURESENSOR_H
#define PRESSURESENSOR_H

#include <Arduino.h>

class PressureSensor {
  public:
    // Конструктор принимает номер аналогового пина и максимальное давление
    PressureSensor(uint8_t pin, float maxPressure)
      : pin_(pin), maxPressure_(maxPressure) {}

    // Метод для получения давления в МПа
    float getPressure() {
      int adcValue = analogRead(pin_);
      float voltage = adcValue * (3.3 / 1023.0);
      return (voltage / 3.3) * maxPressure_;
    }

  private:
    uint8_t pin_;  // Номер аналогового пина
    float maxPressure_;  // Максимальное давление, соответствующее максимальному напряжению (в МПа)
};

#endif // PRESSURESENSOR_H

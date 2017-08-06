#pragma once

// SparkFun SEN-13322
class MoistureSensor {
public:
  MoistureSensor(int pin) : pin_(pin) {
    pinMode(pin_, INPUT);
  }

  int Read() const {
    return analogRead(pin_);
  }

private:
  int pin_ = 0;
};

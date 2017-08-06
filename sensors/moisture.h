#pragma once

#include "sensor.h" 

// SparkFun SEN-13322
class MoistureSensor : public Sensor {
public:
  MoistureSensor(int pin) : pin_(pin) {
    pinMode(pin_, INPUT);
  }

  int Read() const override {
    return analogRead(pin_);
  }

  const char* type() const override {
    static const char* sensor_type = "Moisture";
    return sensor_type;
  }

  const char* name() const override {
    static const char* sensor_name = "SEN-13322";
    return sensor_name;
  }

private:
  int pin_ = 0;
};

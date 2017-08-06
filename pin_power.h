#pragma once

#include "delay.h"

// A digital out power handler. After turning it On() then after a short delay
// IsOn() will return true. This ensures that the pin have had enough time to
// power up whatever it's driving.
class PinPower {
public:
  PinPower(int pin, int delay = 10 /* ms */) : pin_(pin), delay_(delay) {
    pinMode(pin_, OUTPUT);
    digitalWrite(pin_, LOW);
  }

  void On() {
    if (!IsPoweringUp()) {
      digitalWrite(pin_, HIGH);
      delay_.Start();
    }
  }

  void Off() {
    digitalWrite(pin_, LOW);
    delay_.Stop();
  }

  // TODO(thomthom): Add a boolean operator that return this?
  bool IsOn() const {
    return static_cast<bool>(delay_);
  }

  bool IsPoweringUp() const {
    return delay_.Waiting();
  }

private:
  int pin_;
  Delay delay_;
};

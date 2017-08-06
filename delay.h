#pragma once

// A non-blocking delay state machine. It will evalute to true when a given
// interval have elapsed. By resetting it can be used as a repeating timer.
class Delay {
public:
  Delay(unsigned long interval) : interval_(interval) {}

  void Start() {
    time_started_ = millis();
    enabled_ = true;
  }

  void Stop() {
    enabled_ = false;
    time_started_ = 0;
  }

  void Restart() {
    time_started_ = 0;
  }

  bool Started() const {
    return enabled_;
  }

  bool Waiting() const {
    return enabled_ && millis() < time_started_;
  }

  explicit operator bool() const {
    return enabled_ && millis() >= time_started_;
  }
  
private:
  unsigned long time_started_ = 0;
  unsigned long interval_ = 0;
  bool enabled_ = false;
};

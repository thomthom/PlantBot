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


// A digital out power handler. After turning it On() then after a short delay
// IsOn() will return true. This ensures that the ping had had enough time to
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


// The intervall at which the sensors are read.
Delay read_sensors(1000 * 2); // ms

// The digital pin that will be providing power to the
// sensors.
// TODO(thomthom): Can a single digital out power multiple
// sensors?
PinPower power(7);

// The sensors to sample data from.
const int num_moisture_sensors = 2;
MoistureSensor moisture[num_moisture_sensors] { A0, A1 };

void setup() {
  Serial.begin(9600);
  read_sensors.Start();
  // TODO(thomthom): Read sensors upon startup?
}

void loop() {
  if (read_sensors) {
    // This switches the pin on - but IsOn will not return 
    // true until after a short delay.
    power.On();
    // Restart the timer to read again at a steady interval.
    read_sensors.Restart();
  }

  // After a short delay since powering the pin, read from
  // the sensors.
  if (power.IsOn()) {
    Serial.println("Moisture:");
    for (auto i = 0; i < num_moisture_sensors; ++i) {
      Serial.print("> Sensor #");
      Serial.print(i + 1);
      Serial.print(": ");
      Serial.println(moisture[i].Read());
    }
    Serial.println("*****");
    power.Off();
  }
}
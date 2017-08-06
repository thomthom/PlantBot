#include "delay.h"
#include "pin_power.h"
#include "sensors/moisture.h"

// The intervall at which the sensors are read.
Delay read_sensors(1000 * 2); // ms

// The digital pin that will be providing power to the
// sensors.
// TODO(thomthom): Can a single digital out power multiple
// sensors? Maybe use a transistor?
PinPower power(7);

// The sensors to sample data from.
MoistureSensor sensors[] { A0, A1 };
const int kNumSensors = sizeof(sensors) / sizeof(MoistureSensor);

void setup() {
  Serial.begin(9600);
  read_sensors.Start();
  // TODO(thomthom): Read sensors upon startup?
  // Instead of starting the timer, set read_sensor to a true state.
  // That way the loop should read the sensors as soon as they have
  // had time to make some readings.
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
    for (auto i = 0; i < kNumSensors; ++i) {
      Serial.print("> Sensor #");
      Serial.print(i + 1);
      Serial.print(": ");
      Serial.println(sensors[i].Read());
    }
    Serial.println("*****");
    power.Off();
  }
}

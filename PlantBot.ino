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
static MoistureSensor s1(A0);
static MoistureSensor s2(A1);
Sensor* sensors[] {
  &s1, &s2
};
const int kNumSensors = sizeof(sensors) / sizeof(Sensor);

void setup() {
  Serial.begin(9600);
  // Power on during startup for an initial reading. Otherwise there will be
  // a longer lag before data appear.
  power.On();
  // Start the timer that controls the interval for reading from the sensors.
  read_sensors.Start();
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
    Serial.println("Sensor Readings:");
    for (auto i = 0; i < kNumSensors; ++i) {
      Sensor& sensor = *sensors[i];
      Serial.print("> Sensor #");
      Serial.print(i + 1);
      Serial.print(" ");
      Serial.print(sensor.type());
      Serial.print(": ");
      Serial.println(sensor.Read());
    }
    Serial.println("*****");
    power.Off();
  }
}

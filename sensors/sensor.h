#pragma once

class Sensor {
public:

  virtual int Read() const;

  virtual const char* type() const;
  virtual const char* name() const;

};

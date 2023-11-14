// Battery.h

#ifndef BATTERY_H
#define BATTERY_H
//#define BATTERY_PIN A1

#include <Arduino.h>

class Battery {
  private:
    int _adc_value;
    float _current_voltage;
    float _current_percent;
    float _zero_percent_voltage;
    float _max_voltage;
    float mapfloat(float x, float in_min, float in_max, float out_min, float out_max);
    
  public:
    float refresh_battery();
    
};

#endif

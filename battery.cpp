//Battery.cpp

#include "battery.h"
#include <stdio.h>
float Battery::mapfloat(float x, float in_min, float in_max, float out_min, float out_max){
      return (x - in_min) * (out_max - out_min) / (in_max -in_min) + out_min;
     
      }

float Battery::refresh_battery()
{
analogReadResolution(12);

_adc_value = analogRead(A1); 

const float factor = 2.0; // 1.984636;              // 3.88 * 4095 / (3.3 * 2426);

const float adc_reference_voltage = 3.3; // TODO: does droop as the battery voltage drops?

const int number_of_levels = 4095; // 2^12

 _current_voltage = factor * _adc_value * adc_reference_voltage / number_of_levels;

 _current_percent = mapfloat(_current_voltage,3.6,4.2,0,100);
//_current_percent = map(_adc_value,0,3000,0,100);

//SerialUSB.print("Pin Value: ");
//SerialUSB.println(_adc_value);
//SerialUSB.print("Voltage: ");
//SerialUSB.println(_current_voltage);
//SerialUSB.print("Percent: " );
//SerialUSB.println( _current_percent);
return  _current_percent;
}

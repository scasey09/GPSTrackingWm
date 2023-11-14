#ifndef GPSFORMAT_H
#define GPSFORMAT_H
#include <Arduino.h>
#include <string.h>

struct latLong {
    String lattitude;
    String longitude;
};

class GPSFormat { 
  public:
   //void gpsLocation(String local);
   String conversion(String local, int i);
   latLong gpsLocation(String local);
};

#endif
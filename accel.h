// accel.h

#ifndef ACCEL_H
#define ACCEL_H

//#include <avr/sleep.h>
#define LIS3DH_CLK 13
#define LIS3DH_MISO 12
#define LIS3DH_MOSI 11
// Used for hardware & software SPI
#define LIS3DH_CS 10



class Accel {
  private:
  int ledPin = 13; 
  int intPin = 2;
  int LIS3DH_ADDR = 0x18; // change this to 0x19 for alternative i2c address
  int reading = 0; //counter for number of readings taken - for diagnostic use
    
  public:
    void setupAccel();
    void listenforAccel();
  
  };


#endif


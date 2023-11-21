// accel.h
#include <Arduino.h>
#include <string.h>
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
    int ledPin; 
    int intPin;
    int LIS3DH_ADDR = 0x18; // change this to 0x19 for alternative i2c address
    int reading; //counter for number of readings taken - for diagnostic use
    void writeRegister(byte reg, byte data);
    unsigned int readRegister(byte reg);
    void init_ACC(void);
    
  public:
    void setupAccel(int Pin, int LIS3DH_ADDR);
    void listenforAccel();
    void pin2Interrupt();
  };


#endif

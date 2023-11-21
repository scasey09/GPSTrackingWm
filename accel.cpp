//accel.cpp

#include "accel.h"
#include <Wire.h>
#include <SPI.h>
#include <Adafruit_LIS3DH.h>
#include <Adafruit_Sensor.h>
#include <avr/interrupt.h>

//Private Functions

unsigned int Accel::readRegister(byte reg) {
  Wire.beginTransmission(LIS3DH_ADDR);
  Wire.write(reg);
  Wire.endTransmission();
  
  Wire.requestFrom(LIS3DH_ADDR, 1);
  return Wire.read();
}


void Accel::writeRegister(byte reg, byte data) {
  Wire.beginTransmission(LIS3DH_ADDR);
  Wire.write(reg);
  Wire.write(data);
  Wire.endTransmission();
}

void Accel::init_ACC(void)
{
    // configurations for control registers                     //5F on 0X20 is lower power 100 hz mode 01010111
    writeRegister(0x20, 0x5F); //Write 57h into CTRL_REG1;      // Turn on the sensor, enable X, Y, Z axes with ODR = 100Hz normal mode. 01011111
    writeRegister(0x21, 0x09); //Write 09h into CTRL_REG2;      // High-pass filter (HPF) enabled
    writeRegister(0x22, 0x40); //Write 40h into CTRL_REG3;      // ACC AOI1 interrupt signal is routed to INT1 pin.
    writeRegister(0x23, 0x00); //Write 00h into CTRL_REG4;      // Full Scale = +/-2 g
    writeRegister(0x24, 0x00); //Write 08h into CTRL_REG5;      // Default value is 00 for no latching. Interrupt signals on INT1 pin is not latched. 
                                                                //Users don’t need to read the INT1_SRC register to clear the interrupt signal.
    // configurations for wakeup and motionless detection
    writeRegister(0x32, 0x10); //Write 10h into INT1_THS;          // Threshold (THS) = 16LSBs * 15.625mg/LSB = 250mg.
    writeRegister(0x33, 0x00); //Write 00h into INT1_DURATION;     // Duration = 1LSBs * (1/10Hz) = 0.1s.
    //readRegister();  //Dummy read to force the HP filter to set reference acceleration/tilt value
    writeRegister(0x30, 0x2A); //Write 2Ah into INT1_CFG;          // Enable XLIE, YLIE, ZLIE interrupt generation, OR logic.
 
}

//Public Functions
void Accel::setupAccel(int intPin, int LIS3DH_ADDR){

  Adafruit_LIS3DH lis = Adafruit_LIS3DH();
  pinMode(intPin, INPUT); //set interrupt pin 
  interrupts(); 
  if (! lis.begin(LIS3DH_ADDR)) {   // change this to 0x19 for alternative i2c address
    SerialUSB.println("Couldnt start");
    while (1);
  }
  SerialUSB.println("LIS3DH found!");
  lis.setRange(LIS3DH_RANGE_2_G);   // 2, 4, 8 or 16 G!
  SerialUSB.print("Range = "); SerialUSB.print(2 << lis.getRange());  
  SerialUSB.println("G");
  init_ACC();
}

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
    // configurations for control registers
    writeRegister(0x20, 0x57); //Write A7h into CTRL_REG1;      // Turn on the sensor, enable X, Y, Z axes with ODR = 100Hz normal mode.
    writeRegister(0x21, 0x09); //Write 09h into CTRL_REG2;      // High-pass filter (HPF) enabled
    writeRegister(0x22, 0x40); //Write 40h into CTRL_REG3;      // ACC AOI1 interrupt signal is routed to INT1 pin.
    writeRegister(0x23, 0x00); //Write 00h into CTRL_REG4;      // Full Scale = +/-2 g
    writeRegister(0x24, 0x00); //Write 08h into CTRL_REG5;      // Default value is 00 for no latching. Interrupt signals on INT1 pin is not latched. 
                                                                //Users don’t need to read the INT1_SRC register to clear the interrupt signal.
    // configurations for wakeup and motionless detection
    writeRegister(0x32, 0x40); //Write 10h into INT1_THS;          // Threshold (THS) = 16LSBs * 15.625mg/LSB = 250mg.
    writeRegister(0x33, 0x00); //Write 00h into INT1_DURATION;     // Duration = 1LSBs * (1/10Hz) = 0.1s.
    //readRegister();  //Dummy read to force the HP filter to set reference acceleration/tilt value
    writeRegister(0x30, 0x2A); //Write 2Ah into INT1_CFG;          // Enable XLIE, YLIE, ZLIE interrupt generation, OR logic.
 
}

//Public Functions
void Accel::setupAccel(int Pin, int LIS3DH_ADDR){
// int ledPin = 13; //LED TESTING
 //int intPin = 2; //interrupt pin 
  
  //int LIS3DH_ADDR = 0x18; // change this to 0x19 for alternative i2c address
  //int reading = 0; //counter for number of readings taken - for diagnostic use
  Adafruit_LIS3DH lis = Adafruit_LIS3DH();
  
 // pinMode(13, OUTPUT);  //For LED Testing only
  pinMode(intPin, INPUT); //set interrupt pin 
  digitalWrite(intPin, LOW); //make sure it starts out low
  interrupts(); 
  //attach function to interrupt
  //digitalWrite(ledPin, LOW); //turn off LED 
  Serial.begin(115200);
  Serial.println("LIS3DH test!");

  if (! lis.begin(0x18)) {   // change this to 0x19 for alternative i2c address
    Serial.println("Couldnt start");
    while (1);
  }
  Serial.println("LIS3DH found!");
  lis.setRange(LIS3DH_RANGE_2_G);   // 2, 4, 8 or 16 G!
  Serial.print("Range = "); Serial.print(2 << lis.getRange());  
  Serial.println("G");
  init_ACC();
}

void Accel::listenforAccel(){
  
}
//GPS Upload TEST
#include <stdio.h>                                                      //Include the required libraries
#include <string.h>
#include <ArduinoLowPower.h>
#include "battery.h"
#include "GPSFormat.h"
#include "GPSSleep.h"
#include "accel.h"

String Apikey = "HDU74GAZ625355R9";                                                //Thingspeak write API key
Battery b;                                                                         //Battery Class Object
GPSFormat GPS;                                                                     //Formatting GPS String
Accel aSensor; 
GPSSleep sleep;

#define DEBUG true                                                    //Define LTE parameters and pin functions
#define LTE_RESET_PIN 6
#define LTE_PWRKEY_PIN 5
#define LTE_FLIGHT_PIN 7
#define BATTERY_PIN A1
//#define pinInterrupt A0                                               //Wind speed sensor interrupt pin
//#define SLEEP_TIME 10

unsigned long lastMillis;                                
//app variables that get pushed to thingspeak
String loc = "";
int _message;          //Move to class 
//battery variables
int _adc_value;
float _current_voltage;
float _current_percent;
float _zero_percent_voltage;
float _max_voltage;
String lattitude = ""; 
String longitude = ""; 
int ledPin = 13; 
int intPin = 2; //interrupt pin 
int LIS3DH_ADDR = 0x18;
int sleepTime = 1; 

void setup()
{
    SerialUSB.begin(115200);                                            //Start serial communication for debugging
    Serial1.begin(115200);
  
    pinMode(LTE_RESET_PIN, OUTPUT);                                     //Set up LTE pin modes and initial states
    digitalWrite(LTE_RESET_PIN, LOW);
    pinMode(LTE_PWRKEY_PIN, OUTPUT);
    digitalWrite(LTE_RESET_PIN, LOW);
    delay(100);
    digitalWrite(LTE_PWRKEY_PIN, HIGH);
    delay(2000);
    digitalWrite(LTE_PWRKEY_PIN, LOW);
    pinMode(LTE_FLIGHT_PIN, OUTPUT);
    digitalWrite(LTE_FLIGHT_PIN, LOW);                                  //Normal Mode
    //INITIALIZING GPS MODULE  
    SerialUSB.print("You are here");
    delay(20000);
    sendData("AT+CGPS=0",3000,DEBUG);
    sendData("AT+CGPS=1",3000,DEBUG);

    //aSensor.setupAccel(intPin, LIS3DH_ADDR);                                        //Setup accelerometer with Accel Class
    //attachInterrupt(digitalPinToInterrupt(intPin), pin2Interrupt, HIGH);            //Attach Interrupt to accelerometer 
}

void loop()
{ 
  sleep.timedSleep(sleepTime);
   bool newData = false;                                                      
   //  if (millis() - lastMillis >= 2*60*1000UL)
  //  if (millis() - lastMillis >= 2*10*1000UL) 
  //{
  // lastMillis = millis();  //get ready for the next iteration
   getGPS();
    
  //}
   
}

void getGPS()
{
   float _battery_level = b.refresh_battery();
   loc = sendData("AT+CGPSINFO\r\n",3000,DEBUG);
   latLong returnedValues = GPS.gpsLocation(loc);
   lattitude = returnedValues.lattitude;
   longitude=returnedValues.longitude;
   String http_str ="";
  
   if(lattitude!=""&& longitude!="")
      {
      _message = 1;
      String lat = GPS.conversion(lattitude,0);
      String lon = GPS.conversion(longitude,1);
      http_str = "AT+HTTPPARA=\"URL\",\"https://api.thingspeak.com/update?api_key=" + Apikey + "&field1=" + lat + "&field2=-" + lon + "&field3=" + _message + "&field4=" + _battery_level + "\"\r\n";
    
      }else{
        String lat = "1";
        String lon = "1";
        
       _message = 0;
       SerialUSB.println("No GPS Data");
       http_str = "AT+HTTPPARA=\"URL\",\"https://api.thingspeak.com/update?api_key=" + Apikey + "&field1=" + lat + "&field2=" + lon + "&field3=" + _message + "&field4=" + _battery_level + "\"\r\n";
     }
    sendData("AT+HTTPINIT\r\n", 2000, DEBUG);                          //Send the data to Thingspeak
    sendData(http_str, 2000, DEBUG);
    sendData("AT+HTTPACTION=0\r\n", 3000, DEBUG);
    sendData("AT+HTTPTERM\r\n", 3000, DEBUG);
}

String sendData(String command, const int timeout, boolean debug)       //Function to send data over 4G
{
  String response = "";
  Serial1.println(command);
  
  long int time = millis();
  while ( (time + timeout) > millis())
  {
    while (Serial1.available())
    {
      char c = Serial1.read();
      response += c;
    }
  }
  if (debug)
  {
    SerialUSB.print(response);
  }
  return response;
}

void pin2Interrupt(){
  /* Detach interrupt to prevent continous fireing */
  //sleep_disable();
  Serial.println("Pin2Interrupt");
  digitalWrite(ledPin, HIGH);
  getGPS();
  /*  Serial.print("  \tinterrupt: ");
    Serial.print(reading++); Serial.print(",  ");
    Serial.print(readRegister(0x21)); Serial.print(",  "); //read register to reset high-pass filter 
    Serial.print(readRegister(0x26)); Serial.print(",  "); //read register to set reference acceleration
    Serial.print(readRegister(LIS3DH_REG_INT1SRC)); Serial.print(",  "); //Read INT1_SRC to de-latch;
    Serial.println(); */
   
  //detachInterrupt(0);
} 
//GPS Upload TEST
#include <stdio.h>                                                      //Include the required libraries
#include <string.h>
#include <ArduinoLowPower.h>
#include "battery.h"
#include "GPSFormat.h"
#include "GPSSleep.h"
#include "accel.h"
#include "ntfy.h"

String Apikey = "HDU74GAZ625355R9";                                                //Thingspeak write API key
Battery b;                                                                         //Battery Class Object
GPSFormat GPS;                                                                     //Formatting GPS String
Accel aSensor; 
GPSSleep sleep;
ntfy notification;

#define DEBUG true                                                    //Define LTE parameters and pin functions
#define LTE_RESET_PIN 6
#define LTE_PWRKEY_PIN 5
#define LTE_FLIGHT_PIN 7
#define BATTERY_PIN A1
#define LED_PIN 2

//#define pinInterrupt A0                                               //Wind speed sensor interrupt pin
//#define SLEEP_TIME 10
int sleepCount = 10; // Track how many times we have slept so we can reset to an hour when 60 have gone by 
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

int ledPin = 3; 
int intPin = 2; //interrupt pin 
//const byte intPin = A1;
int LIS3DH_ADDR = 0x18;
int sleepTime = 600; // works out to seconds 

int calledByInterrupt = 0; 
int notificationStatusAccel = 0; 
int notificationStatusGPS = 0; 
int notificationStatusLocation = 0; 
int notificationStatusBattery = 0; 

String warningBatteryPwr = "You%20are%20on%20battery%20power";
String warningBatteryLvl = "Your%20battery%20is%20below%2050%";
String warningAccelInter = "Your%20bike%20thinks%20it%20has%20been%20moved";
String warningCurrentLoc = "Your%20bike%20location%20has%20changed";
String warningNoGPS = "No%20GPS%20signal";
String warningUnkown = "Unknown%20error";

void setup()
{
    SerialUSB.begin(115200);                                            //Start serial communication for debugging
    Serial1.begin(115200);
    digitalWrite(ledPin, LOW);
    pinMode(LTE_RESET_PIN, OUTPUT);                                     //Set up LTE pin modes and initial states
    digitalWrite(LTE_RESET_PIN, LOW);
    pinMode(LTE_PWRKEY_PIN, OUTPUT);
    digitalWrite(LTE_RESET_PIN, LOW);
    delay(100);
    digitalWrite(LTE_PWRKEY_PIN, HIGH);
    delay(2000);
    digitalWrite(LTE_PWRKEY_PIN, LOW);
    pinMode(LTE_FLIGHT_PIN, OUTPUT);
    digitalWrite(LTE_FLIGHT_PIN, LOW); 
                                     //Normal Mode
    //INITIALIZING GPS MODULE  
    SerialUSB.print("You are here");
    delay(20000);
    notification.sendDataClass("AT+CGPS=0",3000,DEBUG);
    notification.sendDataClass("AT+CGPS=1",3000,DEBUG);
    aSensor.setupAccel(intPin, LIS3DH_ADDR);       //Setup accelerometer with Accel Class
    delay(1000);
    //attachInterrupt(digitalPinToInterrupt(intPin), pin2Interrupt, CHANGE);            //Attach Interrupt to accelerometer 
    LowPower.attachInterruptWakeup(intPin, pin2Interrupt, CHANGE);
}

void loop()
{ 
  if(calledByInterrupt == 0){
      //
      digitalWrite(ledPin, LOW);

  }else{
    
    // sleepTime = checkGPSTimer(); // If waken up by interrupt, set timer to check once a minute for an hour for GPS location, send Push once
     if (notificationStatusAccel !=1){
     notification.sendNotification("Motorcycle%20thinks%20it%20has%20moved", "Accelerometer%20warning", "skull", 4);
     notificationStatusAccel = 1; 
     
      }
  
  }
  sleepTime = checkGPSTimer();
  sleep.timedSleep(sleepTime);
  bool newData = false;
  
  getGPS();
   //Reset Variables for notifications, so that they are not sending all the time, maybe once per day 
     for (int i = 0; i < 5; ++i) {
        notificationStatusAccel = notification.delayNotification(0, notificationStatusAccel);
        notificationStatusGPS = notification.delayNotification(1, notificationStatusGPS);
        notificationStatusLocation = notification.delayNotification(2, notificationStatusLocation);
        notificationStatusBattery = notification.delayNotification(3, notificationStatusBattery);
    }

}

void getGPS()
{
   float _battery_level = b.refresh_battery();
   loc = notification.sendDataClass("AT+CGPSINFO\r\n",3000,DEBUG);
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
      if (notificationStatusGPS !=1){
       notification.sendNotification("NO%20GPS%20Data", "GPS%20Data%20Missing", "skull", 4);
       notificationStatusGPS = 1; 
      }

     }
     notification.sendDataClass("AT+HTTPINIT\r\n", 2000, DEBUG);                          //Send the data to Thingspeak
     notification.sendDataClass(http_str, 2000, DEBUG);
     notification.sendDataClass("AT+HTTPACTION=0\r\n", 3000, DEBUG);
     notification.sendDataClass("AT+HTTPTERM\r\n", 3000, DEBUG);
}

void pin2Interrupt(){

  SerialUSB.println("Pin2Interrupt");
  digitalWrite(ledPin, HIGH);
  calledByInterrupt = 1;
  sleepCount = 0;

}

int checkGPSTimer(){
   // SerialUSB.print("Sleep Count: ");
   // SerialUSB.print(sleepCount);
   
    if (sleepCount == 10)
    {
      sleepTime = 60;
      digitalWrite(ledPin, LOW);
    }else{
       sleepCount ++;
       sleepTime = 10;
    }
    return sleepTime;
}

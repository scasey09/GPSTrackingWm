#include "ntfy.h"
#include <iostream>
#include <ctime>
#include <unordered_map>

int nStatus = 1;
unsigned long initialTimestamps[4];

void ntfy::sendNotification(String title, String message, String tags, int priority){
//const unsigned long oneDayMillis = 24 * 60 * 60 * 1000; 

  http_push = "AT+HTTPPARA=\"URL\",\"https://ntfy.sh/_maduino_gps_push_?title=" + title + "&message=" + message + "&tags=" + tags + " + &Priority=" + priority + "\"\r\n";

   sendDataClass("AT+HTTPINIT\r\n", 2000, DEBUG);                          //Send the data to Thingspeak
   sendDataClass(http_push, 2000, DEBUG);
   sendDataClass("AT+HTTPACTION=1\r\n", 3000, DEBUG);
   sendDataClass("AT+HTTPTERM\r\n", 3000, DEBUG);
}

String ntfy::sendDataClass(String command, const int timeout, boolean debug)
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

//int ntfy::delayNotification(int notificationStatus){
/* int delayNotification(const String& variableName, int notificationStatus) {
  unsigned long currentMillis = millis();
  unsigned long elapsedTime = currentMillis - initialTimestamp;

        // Check if a day has passed
        if (elapsedTime >= oneDayMillis) {
            // Reset the initial timestamp
            initialTimestamp = currentMillis;
            
            // Update notification status or perform other actions
            notificationStatus = 0;

            // Print a message (replace with your actual logic)
            //std::cout << "A day has passed. Notification status reset to 0." << std::endl;
        }

        // Return the updated notification status
        return notificationStatus;
//set Millis for timing on when to reset notification for events 
} */ 

  ntfy::ntfy() : oneDayMillis(24UL * 60 * 60 * 1000) {
        // Initialize initial timestamps for each variable
        for (int i = 0; i < 4; ++i) {
            initialTimestamps[i] = millis();
        }
    }

int ntfy::delayNotification(int variableIndex, int notificationStatus) {
        // Check if the notification status is 0
        if (notificationStatus == 0) {
            // If status is 0, return unchanged status without performing updates
            return notificationStatus;
        }
        const unsigned long oneDayMillis = 24 * 60 * 60 * 1000; 
        unsigned long currentMillis = millis();
        unsigned long elapsedTime = currentMillis - initialTimestamps[variableIndex];

        // Check if a day has passed
        if (elapsedTime >= oneDayMillis) {
            // Reset the initial timestamp for the specific variable
            initialTimestamps[variableIndex] = currentMillis;
            
            // Update notification status or perform other actions
            notificationStatus = 0;

            // Print a message (replace with your actual logic)
            Serial.println("A day has passed for variable '" + variableNames[variableIndex] + "'. Notification status reset to 0.");
        }

        // Return the updated notification status
        return notificationStatus;
    }
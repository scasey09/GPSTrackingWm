#include "ntfy.h"

int nStatus = 1;

void ntfy::sendNotification(String title, String message, String tags){

  http_push = "AT+HTTPPARA=\"URL\",\"https://ntfy.sh/_maduino_gps_push_?title=" + title + "&message=" + message + "&tags=" + tags + "\"\r\n";

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

int ntfy::delayNotification(int noti){

return noti;
//set Millis for timing on when to reset notification for events 

}
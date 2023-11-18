#include <Arduino.h>
#include <string.h>

#ifndef NTFY_H
#define NTFY_H

#define DEBUG true

class ntfy {
  private:
  String title = title;
  String message = message;
  String tags = tags;
  const unsigned long oneDayMillis; //= 24 * 60 * 60 * 1000;  // One day in milliseconds
  String variableNames[4] = {"Battery", "GPS", "Accel", "Location"};
  
  public:
    int nStatus;
    String http_push;
    void sendNotification(String title, String message, String tags, int priority);
    String sendDataClass(String command, const int timeout, boolean debug);
    //int delayNotification(int notificationStatus);
    int delayNotification(int variableIndex, int notificationStatus);
    ntfy();
};

#endif
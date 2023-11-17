#include <Arduino.h>
#include <string.h>
#define DEBUG true

class ntfy {
  private:
  String title = title;
  String message = message;
  String tags = tags;
 
  public:
  int nStatus;
  String http_push;
  void sendNotification(String title, String message, String tags);
  String sendDataClass(String command, const int timeout, boolean debug);
  };
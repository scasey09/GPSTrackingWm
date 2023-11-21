#include "GPSSleep.h"
#include <stdio.h>

void GPSSleep::timedSleep(int sleepTime){

  LowPower.sleep(1000 * sleepTime);
}
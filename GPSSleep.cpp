#include "GPSSleep.h"
#include <stdio.h>

void GPSSleep::timedSleep(){

  LowPower.sleep(1000 * SLEEP_TIME);
}
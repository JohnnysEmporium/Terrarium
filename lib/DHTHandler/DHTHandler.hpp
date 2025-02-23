#ifndef DHT_HANDLER
#define DHT_HANDLER

#include "../AVR-DHT/DHT.h"
#include "../hd44780/hd44780.h"
#include "../LCDHandler/LCDHandler.hpp"
#include <stdlib.h>

extern const int DHTArrSize;

void DHTInit();
void DHTUpdate(double& temp, double& hum);

#endif
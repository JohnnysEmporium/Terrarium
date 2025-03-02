#ifndef DHT_HANDLER
#define DHT_HANDLER

#include "../../lib/AVR-DHT/DHT.h"
//#include "../hd44780/hd44780.h"
//#include "../LCDHandler/LCDHandler.hpp"
#include <stdlib.h>

void setTempAndHumToPrint(char[], char[]);

extern const int DHTArrSize;
extern char temp_char[5];
extern char hum_char[5];

void DHTInit();
void DHTUpdate(double& temp, double& hum);

#endif
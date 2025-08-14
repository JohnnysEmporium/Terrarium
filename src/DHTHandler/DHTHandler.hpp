#ifndef DHT_HANDLER
#define DHT_HANDLER

#include "../../lib/AVR-DHT/DHT.h"
#include <stdlib.h>

extern const int DHTArrSize;
extern char temp_char[5];
extern char hum_char[5];

void DHTInit();
void DHTUpdate(double* temp, double* hum);
bool DHTisInit();
char* DHTgetHumChar();
char* DHTgetTempChar();

#endif
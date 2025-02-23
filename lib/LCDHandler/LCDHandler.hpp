#ifndef LCD_HANDLER
#define LCD_HANDLER

extern "C" {
    #include "../hd44780/hd44780.h"
}
#include "../Enum/Enum.hpp"
#include "avr\sfr_defs.h"
#include "LCDHandler.hpp"
#include "../DelayHandler/DelayHandler.hpp"
#include <avr\sfr_defs.h>
#include <stdlib.h>
#include <string.h>
#include <avr/io.h>

//Positions of items on LCD
#define LCD_SEM_0       0x02
#define LCD_SEM_1       0x05
#define LCD_H_POS       0x00
#define LCD_M_POS       0x03
#define LCD_S_POS       0x06
#define LCD_DEBUG_POS   0x0C
#define LCD_TEMP_CONST  0x40
#define LCD_RH_CONST    0x48
#define LCD_TEMP        0x43
#define LCD_HUM         0x4C



void LCDInit();
void LCDPutConstantSymbols();
void LCDGoTo(uint8_t pos);
void LCDPuts(const char *s);
void LCDOffTimer();
void LCDWakeUp();
void printAllTime();
void printTime(int c, uint8_t val);

#endif
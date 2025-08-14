#ifndef LCD_HANDLER
#define LCD_HANDLER

#include <avr/sfr_defs.h>
#include <stdlib.h>
#include <string.h>
#include <avr/io.h>

#include <LiquidCrystal_PCF8574.h>
#include "../Enum/Enum.hpp"
#include "../ButtonHandler/ButtonHandler.hpp"
#include "../TimeHandler/TimeHandler.hpp"
#include "LCDConstants.hpp"

// void setTempAndHumToPrint();
void printMainScreenTime();
void setPrintStats();
bool isPrintingStats();
bool isLCDOn();
void LCDOn();
void LCDInit();
void LCDGoTo(uint8_t col, uint8_t row);
void setTimeToPrint();
void setTimeToPrint(ptEnum);
void LCDPuts(char *s);
void LCDPuts(double s);
void LCDResetSleepTimer();
void printConstantSymbols();
void LCDOffTimer();
void printScreenLoop();

#endif

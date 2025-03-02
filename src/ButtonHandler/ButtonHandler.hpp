#ifndef BUTTON_HANDLER
#define BUTTON_HANDLER

//#include "../TimeHandler/TimeHandler.hpp"
//#include "../LCDHandler/LCDHandler.hpp"
#include "../DelayHandler/DelayHandler.hpp"
#include "../Enum/Enum.hpp"
#include "../LCDHandler/LCDConstants.hpp"
#include <stdint.h>
#include <avr/io.h>
#include <avr/interrupt.h>

//FORWARD DECLARATION
void incrementCounter(ptEnum);
void LCDWakeUp();
bool isLCDOn();
void LCDGoTo(uint8_t pos);
void LCDPuts(const char *s);
void setTimeToPrint();
void setTimeToPrint(ptEnum);
void printStats();
extern volatile uint8_t M;
extern volatile uint8_t H;

bool initButtonHandler();
void buttonLoop();

extern bool printStatsBool;
extern bool time_editing_engaged;

#endif 
// BUTTON_HANDLER
#ifndef BUTTON_HANDLER
#define BUTTON_HANDLER

#include <stdint.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include "../Enum/Enum.hpp"
#include "../LCDHandler/LCDConstants.hpp"
#include "../LCDHandler/LCDHandler.hpp"

#endif 

// BUTTON_HANDLER
void initButtonHandler();
void buttonLoop();
bool getTimeEditEngaged();

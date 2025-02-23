#ifndef TIME_HANDLER
#define TIME_HANDLER

#include "../LCDHandler/LCDHandler.hpp"
#include "../hd44780/hd44780.h"
#include "../LCDHandler/LCDHandler.hpp"
#include "../DHTHandler/DHTHandler.hpp"
#include "../DelayHandler/DelayHandler.hpp"
#include <stdlib.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdint.h>

extern volatile uint8_t MSEC_CNT;
extern volatile uint8_t S;
extern volatile uint8_t M;
extern volatile uint8_t H;


void initTimeHandler();
void setTimeEditingEngaged(bool& addr);

void pause_timer0();
void pause_timer1();

void start_timer0();
void start_timer1();

void incrementCounter(ptEnum counter);

void manageTime();

#endif
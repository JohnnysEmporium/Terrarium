#ifndef TIME_HANDLER
#define TIME_HANDLER

#include <stdlib.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdint.h>

#include <LCDHandler/LCDHandler.hpp>
#include <DHTHandler/DHTHandler.hpp>
#include <Enum/Enum.hpp>
#include <RelayHanlder/RelayHandler.hpp>

extern volatile uint8_t S;
extern volatile uint8_t M;
extern volatile uint8_t H;

extern uint8_t humidifierWorkingS;
extern uint8_t humidifierWorkingM;
extern uint8_t humidifierWorkingH;
extern uint8_t lampWorkingS;
extern uint8_t lampWorkingM;
extern uint8_t lampWorkingH;

void initTimeHandler();

unsigned long time();

void incrementCounter(ptEnum counter);

void mainTimeLoop();

#endif
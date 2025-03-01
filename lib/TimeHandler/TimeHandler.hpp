#ifndef TIME_HANDLER
#define TIME_HANDLER

//#include "../LCDHandler/LCDHandler.hpp"
//#include "../LCDHandler/LCDHandler.hpp"
//#include "../DHTHandler/DHTHandler.hpp"
#include "../DelayHandler/DelayHandler.hpp"
#include "../Enum/Enum.hpp"
#include <stdlib.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdint.h>

//FORWARD DECLARATION
void setTimeToPrint();
void setTimeToPrint(ptEnum);
void DHTUpdate(double& temp, double& hum);


extern volatile uint8_t MSEC_CNT;
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
void setTimeEditingEngaged(bool& addr);

void pause_timer0();
void pause_timer1();

void start_timer0();
void start_timer1();

void incrementCounter(ptEnum counter);

void manageTime();

#endif
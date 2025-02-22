#include <stdint.h>
#include "../LCDHandler/LCDHandler.hpp"

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
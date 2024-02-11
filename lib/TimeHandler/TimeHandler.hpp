#include <stdint.h>

extern volatile uint8_t S_CNT;
extern volatile uint8_t M_CNT;
extern volatile uint8_t H_CNT;


bool* initTimeHandler();

void pause_timer0();
void pause_timer1();

void start_timer0();
void start_timer1();

void wait(uint16_t milis);
bool* set_flag_after_milis(uint16_t milis, bool &flag);

void manageTime();
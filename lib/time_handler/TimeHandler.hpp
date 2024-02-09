void printTime(int c, uint8_t val);
void initTimeHandler(volatile uint8_t *MSEC_CNT_addr);

void pause_timer0();
void pause_timer1();

void start_timer0();
void start_timer1();

void wait(uint16_t milis);
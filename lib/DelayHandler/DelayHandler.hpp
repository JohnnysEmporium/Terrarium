#ifndef DELAY_HANDLER
#define DELAY_HANDLER

#include <stdint.h>
#//include "../TimeHandler/TimeHandler.hpp"

//FORWARD DECLARATION
void start_timer0();
void pause_timer0();
extern volatile uint8_t MSEC_CNT;
extern volatile uint8_t S;

class DelayHandler {
    private:
        uint16_t set_flag_after_milis_compare_value;
        uint16_t set_flag_after_milis_counter_old;
        bool set_flag_after_milis_running;

        uint16_t set_flag_after_seconds_compare_value;
        uint16_t set_flag_after_seconds_counter_old;
        bool set_flag_after_seconds_running;

    public:
        DelayHandler();

        bool* set_flag_after_milis(uint16_t milis, bool &flag);
        void resetMilisCounter();
        bool* set_flag_after_seconds(uint16_t seconds, bool &flag);
        void resetSecondsCounter();
};

#endif
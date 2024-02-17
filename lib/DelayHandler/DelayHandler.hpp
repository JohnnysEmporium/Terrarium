#include <stdint.h>
#include "../TimeHandler/TimeHandler.hpp"

class DelayHandler {
    uint16_t set_flag_after_milis_compare_value;
    uint16_t set_flag_after_milis_counter_old;
    bool set_flag_after_milis_running;

    uint16_t set_flag_after_seconds_compare_value;
    uint16_t set_flag_after_seconds_counter_old;
    bool set_flag_after_seconds_running;

public:
    DelayHandler();

    bool* set_flag_after_milis(uint16_t milis, bool &flag);
    bool* set_flag_after_seconds(uint16_t seconds, bool &flag);
    void resetSecondsTimer();
};
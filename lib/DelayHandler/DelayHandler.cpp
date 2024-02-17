#include "DelayHandler.hpp"
#include <avr/interrupt.h>

DelayHandler::DelayHandler(): 
    set_flag_after_milis_compare_value(0),
    set_flag_after_milis_counter_old(0),
    set_flag_after_milis_running(false),
    set_flag_after_seconds_compare_value(0),
    set_flag_after_seconds_counter_old(0),
    set_flag_after_seconds_running(false) {}

bool* DelayHandler::set_flag_after_milis(uint16_t milis, bool &flag) {
    if (!set_flag_after_milis_running) {
        set_flag_after_milis_running = true;
        set_flag_after_milis_compare_value = milis / 20 + 1;
    }
    pause_timer0();
    uint8_t counter = MSEC_CNT;
    start_timer0();

    if (set_flag_after_milis_compare_value == 0) {
        set_flag_after_milis_running = false;
        flag = !flag;
        pause_timer0();
    } else if (counter == set_flag_after_milis_counter_old + 1) {
        set_flag_after_milis_compare_value--;
    }

    set_flag_after_milis_counter_old = counter;
    return &set_flag_after_milis_running;
}

bool* DelayHandler::set_flag_after_seconds(uint16_t seconds, bool &flag){
    if (!set_flag_after_seconds_running) {
        set_flag_after_seconds_running = true;
        set_flag_after_seconds_compare_value = seconds;
    }
    cli();
    uint8_t counter = S;
    sei();

    if (set_flag_after_seconds_compare_value == 0) {
        set_flag_after_seconds_running = false;
        flag = !flag;
    } else if (counter == set_flag_after_seconds_counter_old + 1) {
        set_flag_after_seconds_compare_value--;
    }

    set_flag_after_seconds_counter_old = counter;
}

void DelayHandler::resetSecondsTimer(){
    set_flag_after_seconds_running = false;
}

// void wait(uint16_t milis) {
//   uint16_t loop_cnt = milis/20 + 1;
//   uint8_t msec = 0;
 
//   while(loop_cnt != 0) {
//     pause_timer0();
//     msec = MSEC_CNT;
//     start_timer0();
//     if(msec == MSEC_OLD + 1) {
//       loop_cnt--;
//     }
//     MSEC_OLD = msec;
//   }
//   pause_timer0();
// }
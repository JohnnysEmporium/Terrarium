#ifndef BUTTON_HANDLER
#define BUTTON_HANDLER

#include "../TimeHandler/TimeHandler.hpp"
#include "../LCDHandler/LCDHandler.hpp"
#include "../DelayHandler/DelayHandler.hpp"
#include <stdint.h>
#include <avr/io.h>
#include <avr/interrupt.h>

class ButtonHandler {
    private:
        bool *SET_FLAG_AFTER_MILIS_RUNNING_ADDR;
        bool *SET_FLAG_AFTER_MILIS_RUNNING_ADDR2;

        DelayHandler delayHandler1;
        DelayHandler delayHandler2;

        bool time_increase_pressed; //Initialize/Declare the Pressed variable
        bool time_increase_delay_engaged;
        bool time_editing_pressed;
        bool is_editing_time;
        bool time_printed_after_edit;

        uint8_t timeframe_for_time_edit;
        uint8_t timeframe_for_time_edit_s_value_old;

        uint8_t time_editing_section;

        bool blink_flag;

        bool isTimeEditTimedOut();
        void resetTimeframeForTimeEdit();
        void timeIncrementBasedOnSection();
        void timeIncrementButtonLoop();
        void timeEditButtonLoop();
        void timeEditBlink();
        void buttonLoop();

    public:
        bool time_editing_engaged;
        ButtonHandler();
        bool initButtonHandler();
};




#endif 
// BUTTON_HANDLER
#ifndef BUTTON_HANDLER
#define BUTTON_HANDLER

//#include "../TimeHandler/TimeHandler.hpp"
//#include "../LCDHandler/LCDHandler.hpp"
#include "../DelayHandler/DelayHandler.hpp"
#include "../Enum/Enum.hpp"
#include "../LCDHandler/LCDConstants.hpp"
#include <stdint.h>
#include <avr/io.h>
#include <avr/interrupt.h>

//FORWARD DECLARATION
void incrementCounter(ptEnum);
void LCDWakeUp();
bool isLCDOn();
void LCDGoTo(uint8_t pos);
void LCDPuts(const char *s);
void setTimeToPrint();
void setTimeToPrint(ptEnum);
void printStats();
extern volatile uint8_t M;
extern volatile uint8_t H;

class ButtonHandler {
    private:

        bool time_increase_pressed; //Initialize/Declare the Pressed variable
        bool time_increase_delay_engaged;
        bool editButtonPressed;
        bool is_editing_time;
        bool time_printed_after_edit;

        uint8_t timeframe_for_time_edit;
        uint8_t timeframe_for_time_edit_s_value_old;

        ptEnum time_editing_section;

        bool blinkFlag;
        bool statsFlag;

        bool isTimeEditTimedOut();
        void resetTimeframeForTimeEdit();
        void timeIncrementBasedOnSection();
        void timeIncrementButtonLoop();
        void timeEditButtonLoop();
        void timeEditBlink();
        void handleAndResetEditButton();
        
    public:
        bool time_editing_engaged; 
        void buttonLoop();
        ButtonHandler();
        bool initButtonHandler();
};

extern bool printStatsBool;



#endif 
// BUTTON_HANDLER
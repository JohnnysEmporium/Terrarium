#include "ButtonHandler.hpp"
#include <stdlib.h> 

DelayHandler DH_IncrementButtonSpeed = DelayHandler();
DelayHandler DH_TimeEditButtonEngaged = DelayHandler();
DelayHandler DH_StatsButtonEngaged = DelayHandler();
DelayHandler DH_Blink = DelayHandler();
DelayHandler DH_IncrementButtonLongPress = DelayHandler();
DelayHandler DH_EditButtonLongPress = DelayHandler();

extern bool printStatsBool = false;
extern bool timeEditingEngaged = false;
bool isIncrementShortPress = false;
bool isEditShortPress = false;
bool isIncrementLongPress = false;
bool isEditLongPress = false;
bool isIncrementButtonReset = false;
bool isEditButtonReset = false;
bool incrementFirstRun = false;
bool editFirstRun = true;
bool statsFlag = false;
bool blinkFlag = false;

bool wasLCDWakeUp = false;


bool time_printed_after_edit = false;

bool is_editing_time = false;
bool timeIncreaseDelayEngaged = false;
bool time_increase_pressed = false;

ptEnum time_editing_section = LCD_H;
uint8_t timeframe_for_time_edit_s_value_old = -1;
uint8_t timeframe_for_time_edit = 5;
uint8_t pressTime = 0;


bool initButtonHandler() {
  //Data Direction Register input PINC5
  DDRC &= ~(1 << PINC5); 
  //Set PINC5 pull-up resistor
  PORTC |= 1 << PINC5;

  DDRD &= ~(1 << PIND2);
  PORTD |= 1 << PIND2;
}

void resetTimeframeForTimeEdit(){
  timeframe_for_time_edit = 5;
}

void timeIncrementBasedOnSection(){
  incrementCounter(time_editing_section);
}

bool isEditButtonPressed(){
  return (!(PINC & (1<<PC5)));
}

bool isIncrementButtonPressed(){
  return (!(PIND & (1<<PD2)));
}

bool isTimeEditTimedOut(){
  if(S == timeframe_for_time_edit_s_value_old + 1){
    timeframe_for_time_edit--;
  }
  timeframe_for_time_edit_s_value_old = S;
  
  if(timeframe_for_time_edit == 0){
    timeframe_for_time_edit = 5;
    return true;
  } else {
    return false;
  }
}

void handleAndResetIncrementButton(bool reset) {
  if(isIncrementLongPress && timeEditingEngaged){
    isIncrementShortPress = false;
    DH_IncrementButtonSpeed.set_flag_after_milis(100, incrementFirstRun);

    if(incrementFirstRun){
      incrementFirstRun = false;
      timeIncrementBasedOnSection();
      resetTimeframeForTimeEdit();
    }

  } else if(isIncrementShortPress  && timeEditingEngaged) {
    timeIncrementBasedOnSection();
    resetTimeframeForTimeEdit();
  }

  if(reset){
    DH_IncrementButtonLongPress.resetMilisCounter();
    isIncrementButtonReset = true;
    isIncrementShortPress = false;
    isIncrementLongPress = false;
  }
}

void handleAndResetEditButton(bool reset){
  if(isEditLongPress && !printStatsBool){
    resetTimeframeForTimeEdit();
    timeEditingEngaged = true;
    time_editing_section = static_cast<ptEnum>(0);

  } else if(isEditShortPress && timeEditingEngaged){
    resetTimeframeForTimeEdit();
    setTimeToPrint();
    int temp = static_cast<uint8_t>(time_editing_section);
    temp++;
    temp = temp % 3;
    time_editing_section = static_cast<ptEnum>(temp);
 
  } else if(isEditShortPress && !timeEditingEngaged){
    printStatsBool = true;
  }

  if(reset){
    DH_EditButtonLongPress.resetMilisCounter();
    wasLCDWakeUp = false;
    isEditShortPress = false;
    isEditLongPress = false;
    isEditButtonReset = true;
  }
}

void timeIncrementButtonLoop() {
  if(!isIncrementButtonPressed() && !isIncrementButtonReset) {
    handleAndResetIncrementButton(true);
    return;
  } else if (!isIncrementButtonPressed()){
    return;
  }
  
  isIncrementShortPress = true;
  isIncrementButtonReset = false;
  DH_IncrementButtonLongPress.set_flag_after_milis(1000, isIncrementLongPress);
  
  if(isIncrementLongPress){
    DH_IncrementButtonLongPress.resetMilisCounter();
    handleAndResetIncrementButton(false);
  }
}

void timeEditButtonLoop(){
  if(!isEditButtonPressed() && !isEditButtonReset){
    handleAndResetEditButton(true);
    return;
  } else if (!isEditButtonPressed()){
    return;
  }

  //if this variable is set to true, it means that button was depressed
  isEditButtonReset = false;
  bool LCDOn = isLCDOn();
  isEditShortPress = true;
  
  //Prevents changing the screens when waking up LCD with initial button press
  if(!LCDOn){
    LCDWakeUp();
    wasLCDWakeUp = true;
  } else {
    wasLCDWakeUp = false;
  }

  if(!wasLCDWakeUp){
    DH_EditButtonLongPress.set_flag_after_milis(2000, isEditLongPress);
    if(isEditLongPress) {
      DH_EditButtonLongPress.resetMilisCounter();
      handleAndResetEditButton(false);
    }
    
  }  
}

void timeEditBlink(){
  if(timeEditingEngaged){

    LCDWakeUp();

    //Assign value for monitoring how long time editing is engaged
    if(!blinkFlag){
      
      uint8_t lcd_pos;
      switch(time_editing_section){
        case LCD_H:
          lcd_pos = LCD_H_POS;
          break;
        case LCD_M:
          lcd_pos = LCD_M_POS;
          break;
        case LCD_S:
          lcd_pos = LCD_S_POS;
          break;
      }

      LCDGoTo(lcd_pos);
      LCDPuts("  ");
      DH_Blink.set_flag_after_milis(200, blinkFlag);
    }

    if(blinkFlag){
      setTimeToPrint(time_editing_section);
      DH_Blink.set_flag_after_milis(500, blinkFlag);
    }

    if(isTimeEditTimedOut()){
      /*
      Found during button testing:
      Due to compilation optimization, directly using s_value in setTimeToPrint, exacly on position 2,
        leads to extended amount of time waiting for set_flag_after_milis function, this causes 
        the program to enter time editing mode after 2-4x more time.
      No idea why/how that happens and honestly don't care at this point. Replacing -Os with -O2...
      */
      setTimeToPrint();
      blinkFlag = false;
      timeEditingEngaged = false;
    }
  }
}

void buttonLoop(){
  timeEditButtonLoop();
  timeEditBlink();
  timeIncrementButtonLoop();      
}
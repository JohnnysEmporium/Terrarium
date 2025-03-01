#include "ButtonHandler.hpp"
#include <stdlib.h> 

ButtonHandler::ButtonHandler():
  time_increase_pressed(false), //Initialize/Declare the Pressed variable
  time_increase_delay_engaged(false),
  editButtonPressed(false),
  time_editing_engaged(false),
  is_editing_time(false),
  time_printed_after_edit(false),
  timeframe_for_time_edit(5),
  timeframe_for_time_edit_s_value_old(-1),
  time_editing_section(LCD_H),
  blinkFlag(false),
  statsFlag(false)
  {
    initButtonHandler();
  }

DelayHandler DH_TimeEditButtonEngaged = DelayHandler();
DelayHandler DH_StatsButtonEngaged = DelayHandler();
DelayHandler DH_Blink = DelayHandler();
DelayHandler DH_TimeIncrement = DelayHandler();
DelayHandler DH_EditButtonLongPress = DelayHandler();

bool isShortPress = false;
bool isLongPress = false;
bool wasLCDWakeUp = false;
bool isEditButtonReset = false;
extern bool printStatsBool = false;
uint8_t pressTime = 0;
bool timeEditFirstRun = true;


bool ButtonHandler::initButtonHandler() {
  //Data Direction Register input PINC5
  DDRC &= ~(1 << PINC5); 
  //Set PINC5 pull-up resistor
  PORTC |= 1 << PINC5;

  DDRD &= ~(1 << PIND2);
  PORTD |= 1 << PIND2;

  return time_editing_engaged;
}


bool ButtonHandler::isTimeEditTimedOut(){
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

void ButtonHandler::resetTimeframeForTimeEdit(){
  timeframe_for_time_edit = 5;
}

void ButtonHandler::timeIncrementBasedOnSection(){
  incrementCounter(time_editing_section);
}

void ButtonHandler::timeIncrementButtonLoop() {

  //Check if PIND2 is clear
  if (!(PIND & (1<<PD2)) && time_editing_engaged) {

    //Make sure that the button was released first
    if (!time_increase_pressed) {
      // LCDOn();
      time_increase_pressed = true;
      timeIncrementBasedOnSection();

    } else if(time_increase_pressed && !time_increase_delay_engaged){
      DH_TimeIncrement.set_flag_after_milis(200, time_increase_delay_engaged);
    
    } else if(time_increase_pressed && time_increase_delay_engaged){
      time_increase_delay_engaged = false;
      timeIncrementBasedOnSection();

    }
    resetTimeframeForTimeEdit();

  } else {
    // LCDOff();
    time_increase_delay_engaged = false;
    time_increase_pressed = false;
    is_editing_time = false;
  }
}

bool isEditButtonPressed() {
  return (!(PINC & (1<<PC5)));
}

void ButtonHandler::timeEditButtonLoop(){
  if(!isEditButtonPressed() && !isEditButtonReset){
    handleAndResetEditButton();
    return;
  } else if (!isEditButtonPressed()){
    return;
  }

  isEditButtonReset = false;
  bool LCDOn = isLCDOn();
  isShortPress = true;
  
  //Prevents changing the screens when waking up LCD with initial button press
  if(!LCDOn){
    LCDWakeUp();
    wasLCDWakeUp = true;
  } else {
    wasLCDWakeUp = false;
  }

  if(!wasLCDWakeUp){
    editButtonPressed = true;
    isShortPress = true;
    DH_EditButtonLongPress.set_flag_after_milis(2000, isLongPress);

    if(isLongPress){
      time_editing_engaged = true;
      timeEditFirstRun = true;
    }
  }  
}

void ButtonHandler::handleAndResetEditButton(){
    if(isShortPress && !time_editing_engaged){
      printStatsBool = true;
    } 

    if (time_editing_engaged){

      if(timeEditFirstRun){
        time_editing_section = static_cast<ptEnum>(0);
        timeEditFirstRun = false;
      
      } else if(isShortPress){
        resetTimeframeForTimeEdit();
        setTimeToPrint();
        int temp = static_cast<uint8_t>(time_editing_section);
        temp++;
        temp = temp % 3;
        time_editing_section = static_cast<ptEnum>(temp);
      }
    }

    DH_EditButtonLongPress.resetMilisCounter();
    editButtonPressed = false;
    wasLCDWakeUp = false;
    isShortPress = false;
    isLongPress = false;
    isEditButtonReset = true;
}

void ButtonHandler::timeEditBlink(){
  if(time_editing_engaged){

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
      time_editing_engaged = false;
    }
  }
}

void ButtonHandler::buttonLoop(){
  timeEditButtonLoop();
  timeEditBlink();
  timeIncrementButtonLoop();      
}
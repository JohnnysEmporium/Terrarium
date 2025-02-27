#include "ButtonHandler.hpp"

ButtonHandler::ButtonHandler():
  time_increase_pressed(false), //Initialize/Declare the Pressed variable
  time_increase_delay_engaged(false),
  editButtonPressed(false),
  time_editing_engaged(false),
  is_editing_time(false),
  time_printed_after_edit(false),
  timeframe_for_time_edit(5),
  timeframe_for_time_edit_s_value_old(-1),
  time_editing_section(0),
  blinkFlag(false),
  statsFlag(false)
  {
    initButtonHandler();
  }

DelayHandler DH_TimeEditButtonEngaged = DelayHandler();
DelayHandler DH_StatsButtonEngaged = DelayHandler();
DelayHandler DH_Blink = DelayHandler();
DelayHandler DH_TimeIncrement = DelayHandler();
DelayHandler DH_EditButtonShortPress = DelayHandler();
DelayHandler DH_EditButtonLongPress = DelayHandler();

bool isShortPress = false;
bool isLongPress = false;
bool wasLCDWakeUp = false;
bool timeEditButtonLoopRun = false;
extern bool stopMainScreenPrinting = false;
uint8_t pressTime = 0;


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
  incrementCounter(static_cast<ptEnum>(time_editing_section));
}

void ButtonHandler::timeIncrementButtonLoop() {

  //Check if PIND2 is clear
  if (!(PIND & (1<<PD2)) && time_editing_engaged) {
  
    LCDWakeUp();

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

//TEST METHOD FOR TIME EDITION
void ButtonHandler::timeEditButtonLoop(){

  if(isEditButtonPressed()){
    timeEditButtonLoopRun = false;
    bool LCDOn = isLCDOn();
    
    if(!LCDOn){
      LCDWakeUp();
      wasLCDWakeUp = true;
    }

    if(!wasLCDWakeUp){
      editButtonPressed = true;
      DH_EditButtonShortPress.set_flag_after_milis(50, isShortPress);
      DH_EditButtonLongPress.set_flag_after_milis(2000, isLongPress);

      if(isLongPress){
        time_editing_engaged = true;
      }
    }

  } else if(editButtonPressed && isShortPress && !time_editing_engaged && !stopMainScreenPrinting){
    printStats();
    stopMainScreenPrinting = true;
  
  } else if(editButtonPressed && !timeEditButtonLoopRun) {
    timeEditButtonLoopRun = true;
    DH_EditButtonShortPress.resetMilisCounter();
    DH_EditButtonLongPress.resetMilisCounter();
    editButtonPressed = false;
    wasLCDWakeUp = false;
    isShortPress = false;
    isLongPress = false;
    // LCDGoTo(LCD_DEBUG_POS);
    // LCDPuts("A");
  }
  
  // if (isEditButtonPressed()) {
    
    
  //   if(!LCDOn){
  //     LCDWakeUp();
  //   } else if(LCDOn) {
  //     editButtonPressed = true;
  //     DH_EditButtonShortPress.set_flag_after_milis(20, isShortPress);

  //     // #############################################################################################################################################
  //     //If not in edition time, enter it after given delay
  //     //time_editing_pressed ensures button was depressed first, before f.e. changing edit section
  //     if (!time_editing_engaged) {
  //       //Set flag that will tell the buttonLoop() to blink
  //       editButtonPressed = true;
  //       DH_TimeEditButtonEngaged.set_flag_after_milis(2000, time_editing_engaged);
  //       time_editing_section = 0;
  //     } else if(time_editing_engaged && !editButtonPressed) {
  //       editButtonPressed = true;
  //       time_editing_section++;
  //       time_editing_section = time_editing_section % 3;
  //       //Printing all time values when changing edit section. Without it when section blinks and than changes it can be left empty.
  //       printTime();
  //       timeframe_for_time_edit = 5;
  //     // #############################################################################################################################################     
  //     }

  //     if (isShortPress && !editButtonPressed) {
  //       isShortPress = false;
  //       printStats();
  //     }
  //   }
  // //Reset timer running bool for if
  // } else {
  //   DH_TimeEditButtonEngaged.resetMilisCounter();
  //   editButtonPressed = false;
  //   isShortPress = false;
  // }
  
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
      
      uint8_t time_value_to_restore;
      switch(time_editing_section){
        case LCD_H:
          cli();
          time_value_to_restore = H;
          sei();
          break;
        case LCD_M:
          cli();
          time_value_to_restore = M;
          sei();
          break;
        case LCD_S:
          time_value_to_restore = S;
          break;
      }

      printTime(time_editing_section, time_value_to_restore);
      DH_Blink.set_flag_after_milis(500, blinkFlag);
    }

    if(isTimeEditTimedOut()){
      /*
      Found during button testing:
      Due to compilation optimization, directly using s_value in printTime, exacly on position 2,
        leads to extended amount of time waiting for set_flag_after_milis function, this causes 
        the program to enter time editing mode after 2-4x more time.
      No idea why/how that happens and honestly don't care at this point. Replacing -Os with -O2...
      */
      printTime(LCD_H, H);
      printTime(LCD_M, M);
      printTime(LCD_S, S);
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

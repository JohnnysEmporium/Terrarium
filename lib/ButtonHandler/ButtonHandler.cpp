#include "../TimeHandler/TimeHandler.hpp"
#include "../LCDHandler/LCDHandler.hpp"
#include <stdint.h>
#include <avr/io.h>
#include <avr/interrupt.h>

bool *SET_FLAG_AFTER_MILIS_RUNNING_ADDR;
bool *SET_FLAG_AFTER_MILIS_RUNNING_ADDR2;

bool time_increase_pressed = false; //Initialize/Declare the Pressed variable
bool time_increase_delay_engaged = false;
bool time_editing_pressed = false;
bool time_editing_engaged = false;
bool is_editing_time = false;
bool time_printed_after_edit = false;

uint8_t s_value;
uint8_t m_value;
uint8_t h_value;

uint8_t timeframe_for_time_edit = 5;
uint8_t timeframe_for_time_edit_s_value_old = -1;

uint8_t time_editing_section = 0;

bool blink_flag = false;

bool* initButtonHandler() {
  //Data Direction Register input PINC5
  DDRC &= ~(1 << PINC5); 
  //Set PINC5 pull-up resistor
  PORTC |= 1 << PINC5;

  DDRD &= ~(1 << PIND2);
  PORTD |= 1 << PIND2;

  return &time_editing_engaged;
}

bool isTimeEditTimedOut(){
  cli();
  s_value = S_CNT;
  sei();
  if(s_value == timeframe_for_time_edit_s_value_old + 1){
    timeframe_for_time_edit--;
  }
  timeframe_for_time_edit_s_value_old = s_value;
  
  if(timeframe_for_time_edit == 0){
    timeframe_for_time_edit = 5;
    return true;
  } else {
    return false;
  }
}

void resetTimeframeForTimeEdit(){
  timeframe_for_time_edit = 5;
}

void timeIncrementBasedOnSection(){
  switch(time_editing_section){
    case LCD_H:
      cli();
      H_CNT++;
      sei();
      break;
    case LCD_M:
      cli();
      M_CNT++;
      sei();
      break;
    case LCD_S:
      cli();
      S_CNT++;
      sei();
      break;
  }
}

void timeIncrementButtonLoop() {
  //Check if PIND2 is clear
  if (!(PIND & (1<<PD2)) && time_editing_engaged) {
    //Make sure that the button was released first
    if (!time_increase_pressed) {
      // LCDOn();
      time_increase_pressed = true;
      timeIncrementBasedOnSection();

    } else if(time_increase_pressed && !time_increase_delay_engaged){
      SET_FLAG_AFTER_MILIS_RUNNING_ADDR2 = set_flag_after_milis2(200, time_increase_delay_engaged);
    
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
    *SET_FLAG_AFTER_MILIS_RUNNING_ADDR2 = false;
  }
}

void setTimeValues() {
  cli();
  h_value = H_CNT;
  m_value = M_CNT;
  s_value = S_CNT;
  sei();
}
//TEST METHOD FOR TIME EDITION
void timeEditButtonLoop(){
  
  if (!(PINC & (1<<PC5))) {
    // printTime(1,5);
    //If not in edition time, enter it after given delay
    //time_editing_pressed ensures button was depressed first, before f.e. changing edit section
    if (!time_editing_engaged) {
      //Set flag that will tell the buttonLoop() to blink
      SET_FLAG_AFTER_MILIS_RUNNING_ADDR = set_flag_after_milis(1000, time_editing_engaged);
      time_editing_pressed = true;
      time_editing_section = 0;
      
    } else if(time_editing_engaged && !time_editing_pressed) {
      time_editing_pressed = true;
      time_editing_section++;
      time_editing_section = time_editing_section % 3;
      //Printing all time values when changing edit section. Without it when section blinks and than changes it can be left empty.
      setTimeValues();
      printTime(LCD_H, h_value);
      printTime(LCD_M, m_value);
      printTime(LCD_S, s_value);
      timeframe_for_time_edit = 5;
    
    
    }   
  
  //Reset timer running bool for if
  } else if(!time_editing_engaged){
      *SET_FLAG_AFTER_MILIS_RUNNING_ADDR = false;
      time_editing_pressed = false;
  } else {
    time_editing_pressed = false;
  }
}

void buttonLoop(){
  timeEditButtonLoop();
  timeIncrementButtonLoop();

  if(time_editing_engaged){
    time_printed_after_edit = false;
    
    //Assign value for monitoring how long time editing is engaged
    if(!blink_flag){
      
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
      SET_FLAG_AFTER_MILIS_RUNNING_ADDR = set_flag_after_milis(200, blink_flag);
    }

    if(blink_flag){
      
      uint8_t time_value_to_restore;
      switch(time_editing_section){
        case LCD_H:
          cli();
          time_value_to_restore = H_CNT;
          sei();
          break;
        case LCD_M:
          cli();
          time_value_to_restore = M_CNT;
          sei();
          break;
        case LCD_S:
          time_value_to_restore = S_CNT;
          break;
      }

      printTime(time_editing_section, time_value_to_restore);
      SET_FLAG_AFTER_MILIS_RUNNING_ADDR = set_flag_after_milis(500, blink_flag);
    }

    if(isTimeEditTimedOut() && !time_printed_after_edit){
      /*
      Found during button testing:
      Due to compilation optimization, directly using s_value in printTime, exacly on position 2,
        leads to extended amount of time waiting for set_flag_after_milis function, this causes 
        the program to enter time editing mode after 2-4x more time.
      No idea why/how that happens and honestly don't care at this point. Replacing -Os with -O2...
      */
      time_printed_after_edit = true;
      setTimeValues();
      printTime(LCD_H, h_value);
      printTime(LCD_M, m_value);
      printTime(LCD_S, s_value);
      blink_flag = false;
      time_editing_engaged = false;
    }
    
  }
}
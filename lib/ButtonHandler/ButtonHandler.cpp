#include "../TimeHandler/TimeHandler.hpp"
#include "../LCDHandler/LCDHandler.hpp"
#include <stdint.h>
#include <avr/io.h>
#include <avr/interrupt.h>

bool *SET_FLAG_AFTER_MILIS_RUNNING;

bool time_increase_pressed = false; //Initialize/Declare the Pressed variable
bool time_increase_delay_engaged = false;
bool time_editing_pressed = false;
bool is_editing_time = false;

uint8_t time_editing_section = 0;

void initButtonHandler() {
  //Data Direction Register input PINC5
  DDRC &= ~(1 << PINC5); 
  //Set PINC5 pull-up resistor
  PORTC |= 1 << PINC5;
}

void button_loop() {
  //Check if PINC5 is clear
  if (!(PINC & (1<<PC4))) {
    //Make sure that the button was released first
    if (!time_increase_pressed) {
      // LCDOn();
      time_increase_pressed = true;
      is_editing_time = true;
      cli();
      H_CNT++;
      sei();
    
    //Introduce delay
    } else if(time_increase_pressed & !time_increase_delay_engaged){
      time_increase_delay_engaged = true;
      wait(500);
    
    //After above delay, start incrementing every ~200ms
    } else if(time_increase_pressed & time_increase_delay_engaged){
      cli();
      H_CNT++;
      sei();
      wait(200);
    }
  } else {
    // LCDOff();
    time_increase_delay_engaged = false;
    time_increase_pressed = false;
    is_editing_time = false;
  }

  if (!(PINC & (1<<PC5))) {
    if(!time_editing_pressed){
      time_editing_pressed = true;
    } else {
      time_editing_pressed = false;
    }
  }
}

void test1() {
  //Check if PINC5 is clear
  if (!(PINC & (1<<PC5))) {
    //Make sure that the button was released first
    if (!time_increase_pressed) {
      cli();
    }
  } else {
    sei();
    time_editing_pressed = false;
  }
}

//TEST METHOD FOR TIME EDITION
void test2(){
  LCDGoTo(0x00);
  LCDPuts("  ");
  if (!(PINC & (1<<PC5))) {
    //Make sure that the button was released first
    // if (!time_editing_pressed && !set_flag_after_milis_running) {
      time_editing_pressed = true;
      SET_FLAG_AFTER_MILIS_RUNNING = set_flag_after_milis(3000, time_editing_pressed);
    // }
    if(time_editing_pressed) {
      *SET_FLAG_AFTER_MILIS_RUNNING = false;
      time_editing_pressed = false;
      LCDOff();
    }
  } else {
    LCDOn();
    time_editing_pressed = false;
  }
}
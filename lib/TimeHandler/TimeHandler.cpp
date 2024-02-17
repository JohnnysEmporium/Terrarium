#include "../hd44780/hd44780.h"
#include "../LCDHandler/LCDHandler.hpp"
#include "../DHTHandler/DHTHandler.hpp"
#include <stdlib.h>
#include <avr/io.h>
#include <avr/interrupt.h>

// global variables for time management
volatile uint8_t MSEC_CNT = 0;
volatile uint8_t S_CNT = 0;
volatile uint8_t M_CNT = 0;
volatile uint8_t H_CNT = 12;

uint8_t MSEC_OLD = -1;
uint8_t S_OLD = -1;
uint8_t M_OLD = -1;
uint8_t H_OLD = -1;

uint8_t H = 0;
uint8_t M = 0;
uint8_t S = 0;

uint16_t set_flag_after_milis_compare_value = 0;
uint16_t set_flag_after_milis_counter_old = 0;
bool set_flag_after_milis_running = false;

uint16_t set_flag_after_milis_compare_value2 = 0;
uint16_t set_flag_after_milis_counter_old2 = 0;
bool set_flag_after_milis_running2 = false;

bool *time_editing_engaged_addr;


//Timer1 initialization
void init_timer1() {
  //Timer Mode 4: Clear Timer on Compare match (CTC)
  TCCR1B |= (1 << WGM12);
  //Initialize Timer staring value
  TCNT1 = 0;
  //Set compare value for 1s overflow
  OCR1A = 31249;
  //Enable Timer Compare A Match interrupt
  TIMSK1 |= (1 << OCIE1A);
  //Start Timer & Clock Select: Prescale I/O clock by 256
  TCCR1B |= (1 << CS12);
}

void setTimeEditingEngagedAddr(bool *addr){
  time_editing_engaged_addr = addr;
}

void pause_timer1() {
  //Disabling clock source
  TCCR1B &= ~(1 << CS12);
}

void start_timer1() {
  //Start Timer & Clock Select: Prescale I/O clock by 256
  TCCR1B |= (1 << CS12);
}

void init_timer0() {
  //Timer Mode 2: CTC
  TCCR0A |= (1 << WGM01);
  //Initialize Timer starting value
  TCNT0 = 0;
  //Set compare value for 20ms overflow
  OCR0A = 155;
  //Enable Timer Compare A Match Interrupt
  TIMSK0 |= (1 << OCIE0A);
  //Prescale timer by 1024 and start counting
  TCCR0B |= (1 << CS00) | (1 << CS02);
}

void pause_timer0() {
  //Set CS bits to 0 - no clock source
  TCCR0B &= ~((1 << CS00) | (1 << CS01) | (1 << CS02));
}

void start_timer0() {
  //Prescale timer by 1024 and start counting
  TCCR0B |= (1 << CS00) | (1 << CS02);
}

void incrementCounter(ptEnum counter){
  switch(counter){
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

void initTimeHandler(){
  init_timer0();
  pause_timer0();
  init_timer1();
}

void manageTime() {
  //Stopping interrupts to assign variables
  cli();
  //Assign volatile int to be processed
  H = H_CNT;
  M = M_CNT;
  S = S_CNT;
  //Starting interrupts
  sei();

  if (S >= 60) {
    cli();
    S_CNT = 0;
    if(!*time_editing_engaged_addr) M_CNT++;
    sei();
  }
  if (M >= 60) {
    cli();
    M_CNT = 0;
    if(!*time_editing_engaged_addr) H_CNT++;
    sei();
  }
  if (H >= 24) {
    cli();
    H_CNT = 0;
    sei();
  }

  if(H_OLD != H){
    H_OLD = H;
    printTime(LCD_H, H);
  }
  
  if(M_OLD != M){
    M_OLD = M;
    printTime(LCD_M, M);
  }

  if(S_OLD != S){
    
    
    DHTMain();
      S_OLD = S;
      printTime(LCD_S, S);
    //Pump should start after the array with temp/hum values fills up, maybe embed it in an if to limit usage 1time/5s?
    // if(IS_STARTING_CNT < TEMP_HUM_VALUES_SIZE){
    //   managePump();
    // } else {
    //   IS_STARTING_CNT++;
    // }
  }

}

// Timer1 output compare match A interrupt rutine
/*
 * Incrementing hours and minutes in the ISR creates possibility
 * for the uC to be put to sleep when it's functions are not needed
 */
ISR(TIMER1_COMPA_vect) {
  S_CNT++;
}

/*
MSEC_CNT relies on variable overflow.
Once it reaches 255 it will reset back to 0.
There's no need to zero this value explicitly as the timer0 is used only for wait routine
  and counts miliseconds MSEC_CNT = 1 = 20ms
*/
ISR(TIMER0_COMPA_vect) {
  MSEC_CNT++;
}
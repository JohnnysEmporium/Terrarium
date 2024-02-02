extern "C" {
  #include "lib/hd44780/hd44780.h"
  #include "lib/AVR-DHT/DHT.h"
}
#include <avr/interrupt.h>
#include <util/delay.h>
#include "lib/DHT_handler/DHTHandler.hpp"
#include "lib/LCD_handler/LCDHandler.hpp"
#include "lib/time_handler/TimeHandler.hpp"

// global variables for time management
volatile uint8_t SEC_CNT = 00;
volatile uint8_t MIN_CNT = 00;
volatile uint8_t H_CNT = 12;
uint8_t SEC_OLD = -1;
uint8_t MIN_OLD = -1;
uint8_t H_OLD = -1;

// Timer1 initialization
void init_timer1()
{
  // Timer Mode 4: Clear Timer on Compare match (CTC)
  TCCR1B |= (1 << WGM12);
  // Initialize Timer staring value
  TCNT1 = 0;
  // Set Compare value for 1s overflow
  OCR1A = 31249;
  // Enable Timer Compare A Match interrupt
  TIMSK1 |= (1 << OCIE1A);
  // Start Timer & Clock Select: Prescale I/O clock by 256
  TCCR1B |= (1 << CS12);
}

// Timer1 output compare match A interrupt rutine
/*
 * Incrementing hours and minutes in the ISR creates possibility
 * for the uC to be put to sleep when it's functions are not needed
 */
ISR(TIMER1_COMPA_vect) {
  SEC_CNT++;
  if (SEC_CNT == 60) {
    SEC_CNT = 0;
    MIN_CNT++;
  }
  if (MIN_CNT == 60) {
    MIN_CNT = 0;
    H_CNT++;
  }
  if (H_CNT == 24) {
    H_CNT = 0;
  }
}

void managePump(){
  
}

//Setting up necessary resources
void setup() {
  init_timer1();
  LCDInit();
  DHTInit();
  sei();
}


void manageTime(){
  static uint8_t h = 0;
  static uint8_t m = 0;
  static uint8_t s = 0;

  //Stop interrupts
  cli();
  //Assign volatile int to be processed
  h = H_CNT;
  m = MIN_CNT;
  s = SEC_CNT;
  //Restore interrupts
  sei();

  if(H_OLD != h){
    H_OLD = h;
    printTime(0, h);
  }
  
  if(MIN_OLD != m){
    MIN_OLD = m;
    printTime(1, m);
  }
  
  if(SEC_OLD != s){
    
    SEC_OLD = s;
    printTime(2, s);
    DHTMain();
    //Pump should start after the array with temp/hum values fills up, maybe embed it in an if to limit usage 1time/5s?
    // if(IS_STARTING_CNT < TEMP_HUM_VALUES_SIZE){
    //   managePump();
    // } else {
    //   IS_STARTING_CNT++;
    // }
  }
}

//For stopping lights after evening, use one of the sleep modes (idle?) by setting SM bits, wake up with timer1 interrupt
int main()
{
  setup();
  while (1)
  {
    manageTime();
  }
}
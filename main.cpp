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
volatile uint8_t MSEC_CNT = 0;
volatile uint8_t SEC_CNT = 0;
volatile uint8_t MIN_CNT = 0;
volatile uint8_t H_CNT = 12;
// uint8_t MSEC_OLD = -1;
uint8_t SEC_OLD = -1;
uint8_t MIN_OLD = -1;
uint8_t H_OLD = -1;

uint8_t H = 0;
uint8_t M = 0;
uint8_t S = 0;

bool time_increase_pressed = false; //Initialize/Declare the Pressed variable
bool time_increase_delay_engaged = false;
bool time_editing_pressed = false;
bool is_editing_time = false;

uint8_t time_editing_section = 0;

uint16_t set_flag_after_milis_compare_value = 0;
uint16_t set_flag_after_milis_counter_old = 0;
bool set_flag_after_milis_running = false;
bool FLAG = false;


// Timer1 output compare match A interrupt rutine
/*
 * Incrementing hours and minutes in the ISR creates possibility
 * for the uC to be put to sleep when it's functions are not needed
 */
ISR(TIMER1_COMPA_vect) {
  SEC_CNT++;
  printTime(1, SEC_CNT);
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

/*
MSEC_CNT relies on variable overflow.
Once it reaches 255 it will reset back to 0.
There's no need to zero this value explicitly as the timer0 is used only for wait routine
  and counts miliseconds MSEC_CNT = 1 = 20ms
*/
ISR(TIMER0_COMPA_vect) {
  MSEC_CNT++;
}

void managePump() {
}

//Setting up necessary resources
void setup() {
  initTimeHandler(&MSEC_CNT);
  LCDInit();
  DHTInit();
  sei();
}

/* 
Synchronous wait
MAX WAIT TIME IS 3.1s
MIN WAIT TIME IS 20ms
After 5.1s the 'lock' will release, allowing further execution.
Resolution is 20ms (based on timer0)
*/
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

//Asynchronous wait
void set_flag_after_milis(uint16_t milis, bool &flag) {
  if(!set_flag_after_milis_running) {
    start_timer0();
    set_flag_after_milis_running = true;
    set_flag_after_milis_compare_value = milis/20 + 1;
  }
  pause_timer0();
  uint8_t counter = MSEC_CNT;
  start_timer0();  
  
  if(set_flag_after_milis_compare_value == 0) {
    set_flag_after_milis_running = false;
    flag = true;
    pause_timer0();
  } else if(counter == set_flag_after_milis_counter_old + 1) {
    set_flag_after_milis_compare_value--;  
  }
  set_flag_after_milis_counter_old = counter;
}

void manageTime() {
  
  

  //Pause timer1 in order to avoid conflicts when assigning values
  pause_timer1();
  //Assign volatile int to be processed
  H = H_CNT;
  M = MIN_CNT;
  S = SEC_CNT;
  //Start timer1
  start_timer1();

  if(H_OLD != H && !is_editing_time){
    H_OLD = H;
    printTime(0, H);
  }
  
  if(MIN_OLD != M && !is_editing_time){
    MIN_OLD = M;
    printTime(1, M);
  }

  if(SEC_OLD != S && !is_editing_time){
    
    SEC_OLD = S;
    printTime(2, S);
    // DHTMain();
    //Pump should start after the array with temp/hum values fills up, maybe embed it in an if to limit usage 1time/5s?
    // if(IS_STARTING_CNT < TEMP_HUM_VALUES_SIZE){
    //   managePump();
    // } else {
    //   IS_STARTING_CNT++;
    // }
  }

}

void button_setup() {
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
      pause_timer1();
      H_CNT++;
      start_timer1();
    
    //Introduce delay
    } else if(time_increase_pressed & !time_increase_delay_engaged){
      time_increase_delay_engaged = true;
      wait(500);
    
    //After above delay, start incrementing every ~200ms
    } else if(time_increase_pressed & time_increase_delay_engaged){
      pause_timer1();
      H_CNT++;
      start_timer1();
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
      pause_timer1();
    }
  } else {
    start_timer1();
    time_editing_pressed = false;
  }
}

//TEST METHOD FOR TIME EDITION
void test2(){
  lcd_goto(0x00);
  lcd_puts("  ");
  if (!(PINC & (1<<PC5))) {
    //Make sure that the button was released first
    // if (!time_editing_pressed && !set_flag_after_milis_running) {
      time_editing_pressed = true;
      set_flag_after_milis(3000, FLAG);
    // }
    if(FLAG) {
      FLAG = false;
      LCDOff();
    }
  } else {
    LCDOn();
    set_flag_after_milis_running = false;
    time_editing_pressed = false;
  }
}

//For stopping lights after evening, use one of the sleep modes (idle?) by setting SM bits, wake up with timer1 interrupt
int main()
{
  setup();
  button_setup();
  while (1)
  {
    manageTime();
    // test2();
    wait(3000);
  }
}
#include "TimeHandler.hpp"

// global variables for time management
volatile uint8_t MSEC_CNT = 0;
volatile uint8_t S_CNT = 0;
volatile uint8_t M_CNT = 0;
volatile uint8_t H_CNT = 12;

uint8_t MSEC_OLD = -1;
uint8_t S_OLD = -1;
uint8_t M_OLD = -1;
uint8_t H_OLD = -1;

extern volatile uint8_t H = 0;
extern volatile uint8_t M = 0;
extern volatile uint8_t S = 0;

extern uint8_t humidifierWorkingS = 0;
extern uint8_t humidifierWorkingM = 0;
extern uint8_t humidifierWorkingH = 0;
extern uint8_t lampWorkingS = 0;
extern uint8_t lampWorkingM = 0;
extern uint8_t lampWorkingH = 0;


bool time_editing_engaged;
double temp, hum;

bool DHTSpoolUpTimerFlag = true;

DelayHandler spoolUpDelayHandler = DelayHandler();


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

void setTimeEditingEngaged(bool& addr){
  time_editing_engaged = addr;
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

  //MOVED TO ISR - DON'T KNOW YET IF IT'S A GOOD MOVE BUT SEEMS LIKE IT
  // if (S >= 60) {
  //   cli();
  //   S_CNT = 0;
  //   // if(!time_editing_engaged) M_CNT++;
  //   sei();
  // }
  // if (M >= 60) {
  //   cli();
  //   M_CNT = 0;
  //   // if(!time_editing_engaged) H_CNT++;
  //   sei();
  // }
  // if (H >= 24) {
  //   cli();
  //   H_CNT = 0;
  //   sei();
  // }

  if(H_OLD != H){
    H_OLD = H;
    setTimeToPrint(LCD_H);
  }
  
  if(M_OLD != M){
    M_OLD = M;
    setTimeToPrint(LCD_M);
  }

  if(S_OLD != S){
    
    S_OLD = S;
    setTimeToPrint(LCD_S);

    // Allow for DHT to 'spool up' and fill it's arrays with temp and hum values
    // During the first 10 seconds, DHTUpdate runs every second, after that, every 5 seconds
    // Every 5 seconds, uC will make a decision whether to turn on/off lamp/humidifier
    if(DHTSpoolUpTimerFlag) {
      spoolUpDelayHandler.set_flag_after_seconds(10, DHTSpoolUpTimerFlag);
      DHTUpdate(temp, hum);
    } else {
      if(S % 5 == 0){
        DHTUpdate(temp, hum);
        if(temp < 27){
          lampWorkingS += 5;
          if(lampWorkingS == 60) lampWorkingM++;
          if(lampWorkingH == 60) lampWorkingH++;
          //Turn on lamp
        } else {
          
          //Turn off lamp
        }

        if(hum < 70){
          humidifierWorkingS += 5;
          if(humidifierWorkingS == 60) humidifierWorkingM++;
          if(humidifierWorkingM == 60) humidifierWorkingH++;
          //Turn on humidifier
        } else {
          //Turn off humidifier
        }
      }
    }
  }
}

// Timer1 output compare match A interrupt rutine
/*
 * Incrementing hours and minutes in the ISR creates possibility
 * for the uC to be put to sleep when it's functions are not needed
 */
ISR(TIMER1_COMPA_vect) {
  S_CNT++;
  if(S_CNT >= 60) {
    S_CNT = 0;
    // Prevents incrementing minutes and hours when in time editing mode
    // Minutes will not be incremented when the counter reaches 60
    if(!time_editing_engaged) M_CNT++;
  }
  if(M_CNT >= 60) {
    M_CNT = 0;
    if(!time_editing_engaged) H_CNT++;
  }
  if(H_CNT >= 24) {
    H_CNT = 0;
  }
}

/*
MSEC_CNT relies on variable overflow.
Once it reaches 255 it will reset back to 0.
There's no need to zero this value explicitly as the timer0 is used only for wait routine
  and counts miliseconds MSEC_CNT = 1 = 20ms (that's why in the DelayHandler it's divided by 20)
*/
ISR(TIMER0_COMPA_vect) {
  MSEC_CNT++;
}
#include "TimeHandler.hpp"
#include "../LCDHandler/LCDHandler.hpp"

// global variables for time management

// Timer0 milis count, returned via now();
volatile unsigned long milliseconds = 0;

volatile uint8_t S_CNT = 0;
volatile uint8_t M_CNT = 0;
volatile uint8_t H_CNT = 12;

uint8_t MSEC_OLD = -1;
uint8_t S_OLD = -1;
uint8_t M_OLD = -1;
uint8_t H_OLD = -1;

volatile uint8_t H = 0;
volatile uint8_t M = 0;
volatile uint8_t S = 0;

uint8_t humidifierWorkingS = 0;
uint8_t humidifierWorkingM = 0;
uint8_t humidifierWorkingH = 0;
uint8_t lampWorkingS = 0;
uint8_t lampWorkingM = 0;
uint8_t lampWorkingH = 0;

extern bool isTimeEditEngaged;
double temp, hum;

bool DHTSpoolUpTimerFlag = true;

// Timer1 initialization
void init_timer1() {
  // CTC mode
  TCCR1A = 0;
  TCCR1B = 0;
  TCCR1B |= (1 << WGM12);

  // Compare match value for 1s interrupt
  // 8 MHz / 256 prescaler = 31,250 ticks per second
  // Want 1 Hz => OCR1A = 31249 (0-based count)
  OCR1A = 31249;

  // Enable Timer1 Compare A interrupt
  TIMSK1 |= (1 << OCIE1A);

  // Start timer with 256 prescaler
  TCCR1B |= (1 << CS12);

  // Enable global interrupts
  sei();
}

// Timer1 output compare match A interrupt rutine
/*
 * Incrementing hours and minutes in the ISR creates possibility
 * for the uC to be put to sleep when it's functions are not needed
 */
ISR(TIMER1_COMPA_vect) {
  S_CNT++;
  if (S_CNT >= 60) {
    S_CNT = 0;
    // Prevents incrementing minutes and hours when in time editing mode
    // Minutes will not be incremented when the counter reaches 60
    if (!isTimeEditEngaged) M_CNT++;
  }
  if (M_CNT >= 60) {
    M_CNT = 0;
    if (!isTimeEditEngaged) H_CNT++;
  }
  if (H_CNT >= 24) {
    H_CNT = 0;
    if (!getTimeEditEngaged()) {
      lampWorkingH = 0;
      lampWorkingM = 0;
      lampWorkingS = 0;
      humidifierWorkingH = 0;
      humidifierWorkingM = 0;
      humidifierWorkingS = 0;
    }
  }
}

void init_timer0() {
    // Set Timer0 to CTC mode
    TCCR0A = (1 << WGM01);                 // CTC mode
    TCCR0B = (1 << CS01) | (1 << CS00);    // Prescaler 64
    OCR0A = 124;                           // 1 ms at 8 MHz / 64
    TIMSK0 = (1 << OCIE0A);                // Enable compare match interrupt
    sei();                                 // Enable global interrupts
}

ISR(TIMER0_COMPA_vect) {
    milliseconds++;
}

void incrementCounter(ptEnum counter) {
  switch (counter) {
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

// Equivalent of Arduino's millis()
unsigned long time() {
    unsigned long ms;

    cli();
    ms = milliseconds;
    sei();

    return ms;
}

void initTimeHandler() {
  init_timer1();
  init_timer0();
}

void incrementStatsCounter(uint8_t* s, uint8_t* m, uint8_t* h) {
  *s += 5;

  if (*s == 60) {
    *s = 0;
    *(m)++;
  }
  if (*m == 60) {
    *m = 0;
    *(h)++;
  }
}

// THE Controller
void mainTimeLoop() {
  // Stopping interrupts to assign variables
  cli();
  // Assign volatile int to be processed
  H = H_CNT;
  M = M_CNT;
  S = S_CNT;
  // Starting interrupts
  sei();

  if (H_OLD != H) {
    H_OLD = H;
    setTimeToPrint(LCD_H);

    if (H >= 20 && H <= 8) {
      // LED off
    } else {
      // LED on
    }
  }

  if (M_OLD != M) {
    M_OLD = M;
    setTimeToPrint(LCD_M);
  }

  if (S_OLD != S) {
    S_OLD = S;
    setTimeToPrint(LCD_S);

    // PORTC ^= (1 << PC2);
    // Allow for DHT to 'spool up' and fill it's arrays with temp and hum values
    // During the first 10 seconds, DHTUpdate runs every second, after that, every 5 seconds
    // Every 5 seconds, uC will make a decision whether to turn on/off lamp/humidifier
    if (DHTisInit()) {
      DHTUpdate(&temp, &hum);
    } else {
      if (S % 5 == 0) {
        DHTUpdate(&temp, &hum);

        // testing relays
        // PORTB ^= (1 << PB2);
        // PORTC ^= (1 << PC2);

        if (temp < 25) {
          incrementStatsCounter(&lampWorkingS, &lampWorkingM, &lampWorkingH);
          relay1on();
        } else {
          relay1off();
        }

        if (hum < 70) {
          incrementStatsCounter(&humidifierWorkingS, &humidifierWorkingM, &humidifierWorkingH);
          relay2on();
        } else {
          relay2off();
        }
      }
    }
  }
}
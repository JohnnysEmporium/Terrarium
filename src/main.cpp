#include <avr/interrupt.h>
#include <util/delay.h>

#include <LCDHandler/LCDHandler.hpp>
#include <ButtonHandler/ButtonHandler.hpp>
#include <DHTHandler/DHTHandler.hpp>
#include <TimeHandler/TimeHandler.hpp>
#include <RelayHanlder/RelayHandler.hpp>

#define F_CPU 8000000UL
#define DHT_Type  DHT22  // DHT11 or DHT22
#define DHT_Pin   B, 0

// etting up necessary resources
void setup() {
  initTimeHandler();

  // passing timeEditingEngaged by reference
  initButtonHandler();

  LCDInit();

  DHTInit();

  relayInit();
  relay1off();
  relay2off();
  relay3off();

  // The fuck is that? Can't remember xD
  // DDRD &= ~(1 << PIND0);
  // PORTD |= (1 << PIND0);
}

// For stopping lights after evening, use one of the sleep modes (idle?) by setting SM bits, wake up with timer1 and/or INT0 interrupts
int main()
{
  setup();
  while (1) {
    mainTimeLoop();
    printScreenLoop();
    buttonLoop();
    LCDOffTimer();
  }
}

// TODO:  fix setTimeToPrint in LCDHandler, are these many repetitions necessary? Maybe use h_disp[1] = buff; instead of strcat and \0  // NOLINT
//        Add functionality to reset the 2nd screen time values.
//        Reset lcdoff timer each time any button is pressed when the screen is on


//        Someday, in the future, create a Button class, both increment and edit have the same mechanisms - no reason for it, just make it prettier
//            Also, optimize DelayHandler - set_flag_after_milis is not accurate, it takes longer than milisecond for its counter to decrement,  // NOLINT
//            therefore if we put f.e. 2000ms as 1st parameter, it in fact will be smth around 3000ms. 
//            Additionally, fix the way it's called, now, it's executing unnecessary math, instead of just storing the 1st parameter in a variable.

extern "C" {
  #include "lib/hd44780/hd44780.h"
  #include "lib/AVR-DHT/DHT.h"
}
#include <avr/interrupt.h>
#include <util/delay.h>
#include "./src/LCDHandler/LCDHandler.hpp"
#include "./src/ButtonHandler/ButtonHandler.hpp"
#include "./src/DHTHandler/DHTHandler.hpp"
#include "./src/TimeHandler/TimeHandler.hpp"


//Setting up necessary resources
void setup() {
  initTimeHandler();
  //passing timeEditingEngaged by reference
  initButtonHandler();
  LCDInit();
  DHTInit();
  
  DDRD &= ~(1 << PIND0);
  PORTD |= (1 << PIND0);

  DDRB |= (1 << PINB1);
  sei();
}

//For stopping lights after evening, use one of the sleep modes (idle?) by setting SM bits, wake up with timer1 and/or INT0 interrupts
int main()
{
  setup();
  while (1)
  {
    manageTime();
    printScreen();
    buttonLoop();
    // LCDOffTimer();


    // if(bit_is_clear(PIND, 0)){
    //   setTimeToPrint(0, 20);
    // } else {
    //   setTimeToPrint(0,10);
    // }

    // PORTB ^= (1 << PINB1);
    // _delay_ms(1000);
    // PORTB ^= (1 << PINB1);
    // _delay_ms(2000);

  }
}

// TODO:  fix setTimeToPrint in LCDHandler, are these many repetitions necessary? Maybe use h_disp[1] = buff; instead of strcat and \0
//        Someday, in the future, create a Button class, both increment and edit have the same mechanisms - no reason for it, just make it prettier
//            Also, optimize DelayHandler - set_flag_after_milis is not accurate, it takes longer than milisecond for its counter to decrement,
//            therefore if we put f.e. 2000ms as 1st parameter, it in fact will be smth around 3000ms. 
//            Additionally, fix the way it's called, now, it's executing unnecessary math, instead of just storing the 1st parameter in a variable.

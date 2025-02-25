extern "C" {
  #include "lib/hd44780/hd44780.h"
  #include "lib/AVR-DHT/DHT.h"
}
#include <avr/interrupt.h>
#include <util/delay.h>
#include "lib/LCDHandler/LCDHandler.hpp"
#include "lib/ButtonHandler/ButtonHandler.hpp"
#include "lib/DHTHandler/DHTHandler.hpp"
#include "lib/TimeHandler/TimeHandler.hpp"

ButtonHandler buttonHandler;
//Setting up necessary resources
void setup() {
  initTimeHandler();
  buttonHandler = ButtonHandler();
  bool time_editing_engaged = buttonHandler.initButtonHandler();
  //passing time_editing_engaged by reference
  setTimeEditingEngaged(time_editing_engaged);
  LCDInit(buttonHandler);
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
    buttonHandler.buttonLoop();
    // LCDOffTimer();


    // if(bit_is_clear(PIND, 0)){
    //   printTime(0, 20);
    // } else {
    //   printTime(0,10);
    // }

    // PORTB ^= (1 << PINB1);
    // _delay_ms(1000);
    // PORTB ^= (1 << PINB1);
    // _delay_ms(2000);

  }
}

// TODO:  fix printTime in LCDHandler, are these many repetitions necessary? Maybe use h_disp[1] = buff; instead of strcat and \0
//        stopMainPrint in LCDHandler is not reflecting changes from ButtonHandler
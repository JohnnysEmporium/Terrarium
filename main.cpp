extern "C" {
  #include "lib/hd44780/hd44780.h"
  #include "lib/AVR-DHT/DHT.h"
}
#include <avr/interrupt.h>
#include <util/delay.h>
#include "lib/DHTHandler/DHTHandler.hpp"
#include "lib/LCDHandler/LCDHandler.hpp"
#include "lib/TimeHandler/TimeHandler.hpp"
#include "lib/ButtonHandler/ButtonHandler.hpp"



//Setting up necessary resources
void setup() {
  initTimeHandler();
  bool* time_editing_engaged_addr = initButtonHandler();
  //setting address of time_editing_engaged variable from ButtonHandler
  //done this way not to make 'extern' and include ButtonHanlder.hpp in TimeHandler unnecessairly
  setTimeEditingEngagedAddr(time_editing_engaged_addr);
  LCDInit();
  // DHTInit();
  sei();
}

//For stopping lights after evening, use one of the sleep modes (idle?) by setting SM bits, wake up with timer1 and/or INT0 interrupts
int main()
{
  setup();
  while (1)
  {
    manageTime();
    buttonLoop();
    LCDOffTimer();
  }
}
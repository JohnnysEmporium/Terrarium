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
  initButtonHandler();
  LCDInit();
  DHTInit();
  sei();
}

//For stopping lights after evening, use one of the sleep modes (idle?) by setting SM bits, wake up with timer1 interrupt
int main()
{
  setup();
  // button_setup();
  while (1)
  {
    manageTime();

  }
}